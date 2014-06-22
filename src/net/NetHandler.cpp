//
//  NetHandler.cpp
//  hero
//
//  Created by xujianfeng on 13-8-5.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#include "NetHandler.h"

extern int errno;
extern GameConfig game_config;
NetHandler::NetHandler(EventQueue * eq,int server_id)
{
    eq_ = eq;
    fd_cache_.clear();
    nh_id_ = server_id;
    
    show_status_timestamp_ = time(NULL);
    log4cxx::PropertyConfigurator::configure("log4cxx.config");
    logger_ = log4cxx::Logger::getLogger("NetHandler");
    pthread_mutex_init(&fd_cache_mutex_, NULL);
}

NetHandler::~NetHandler()
{
}

void NetHandler::start()
{
    sconfig * sc = game_config.getSconfig(nh_id_);
    if(sc == NULL){
        LOG4CXX_ERROR(logger_, "system;cannot find server config");
        return;
    }
    fd_set readFd;
    FD_ZERO(&master_);
    listener_fd_ = init_socket(sc->ip.c_str(),sc->port);
    if(listener_fd_<0){
        LOG4CXX_ERROR(logger_, "system;failed to init socket,exit");
        return;
    }
    FD_SET(listener_fd_,&master_);
    int fd_max = listener_fd_;
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    for(;;){
        time_t now = time(NULL);
        if(now - show_status_timestamp_ >= 30){
            showStatus();
            cleanZombieConnection(now);
            show_status_timestamp_ = now;
        }
        
        readFd = master_;

        int count = select(fd_max+1,&readFd,NULL,NULL,&timeout);
        if(count<0){
            LOG4CXX_ERROR(logger_, "system;select error,exit");
            return;
            
        }
        
        if(count == 0){
            continue;
        }
        for(int i=0;i<=fd_max;i++){
            if(FD_ISSET(i,&readFd)){
                if(i == listener_fd_){
                    struct sockaddr_in sa;
                    socklen_t slen = sizeof(sa);
                    int new_fd = accept(i,(struct sockaddr *)&sa,&slen);
                    if(new_fd<0){
                        LOG4CXX_ERROR(logger_, "system;accept error");
                        LOG4CXX_ERROR(logger_, "system;errorno "<<errno);
                    }else{
                        FD_SET(new_fd,&master_);
                        if(new_fd>fd_max){
                            fd_max = new_fd;
                        }
                        addConnection(new_fd);
                        LOG4CXX_DEBUG(logger_, "system;connection accepted");
                    }
                }else{
                    ConnectionCache * cache = getConnectionByFd(i);
                    if(cache!=NULL){
                        /*bool readSucc = cache->read();
                        if(readSucc){
                            string req;
                            while(cache->assemble(req) && cache->remove_==false){
                                cout<<"try0!:"<<req<<endl;
                                eq_->pushCmd(i,0,req);
                            }
                        }
                        if(!readSucc || (cache->remove_ && cache->wpos_>0)){
                            LOG4CXX_DEBUG(logger_, "system;connection closed");
                            closeConnection(i);
                        }*/
                        int readSucc = cache->read();
                        if(readSucc == 0){
                            string req;
                            while(cache->assemble(req) && cache->remove_==false){
                                cout<<"try0!:"<<req<<endl;
                                eq_->pushCmd(i,0,req);
                            }
                        }
                        if(readSucc == -1|| (cache->remove_ && cache->wpos_>0)){
                            LOG4CXX_DEBUG(logger_, "system;connection closed");
                            closeConnection(i);
                        }
                    }

                }
            }
        }
    }
}

ConnectionCache * NetHandler::addConnection(int fd)
{
    ConnectionCache * cache = new ConnectionCache(fd);
    pthread_mutex_lock(&fd_cache_mutex_);
    fd_cache_.insert(make_pair(fd,cache));
    pthread_mutex_unlock(&fd_cache_mutex_);
    return cache;
}

ConnectionCache * NetHandler::getConnectionByFd(int fd)
{
    //pthread_mutex_lock(&fd_cache_mutax_);
    map<int , ConnectionCache * >::const_iterator it = fd_cache_.find(fd);
    if(it != fd_cache_.end()){
        //pthread_mutex_unlock(&fd_cache_mutax_);
        return it->second;
    }else{
        //pthread_mutex_unlock(&fd_cache_mutax_);
        return NULL;
    }
}

void NetHandler::removeConnection(int fd)
{
    pthread_mutex_lock(&fd_cache_mutex_);
    ConnectionCache * cache = getConnectionByFd(fd);
    if(cache != NULL)
    {
        fd_cache_.erase(fd);
        delete cache;
    }
    pthread_mutex_unlock(&fd_cache_mutex_);
}

void NetHandler::closeConnection(int fd)
{
    if(fd>0){
        FD_CLR(fd,&master_);
        close(fd);
        shutdown(fd,SHUT_RDWR);
        removeConnection(fd);
    }
}

bool NetHandler::sendString(int fd,string &s)
{
    LOG4CXX_DEBUG(logger_, "sendString;"<<s);
    ConnectionCache * cache = getConnectionByFd(fd);
    if(cache == NULL) return false;
    if(cache->prepareToWrite(s)==false)return false;
    return cache->write();
}

bool NetHandler::sendString(int fd, const char *str, size_t len) {
    LOG4CXX_DEBUG(logger_, "sendString:"<<string(str));
    ConnectionCache *cache = getConnectionByFd(fd);
    if (cache == NULL) return false;
    if (cache->prepareToWrite(str, len) == false) return false;
    return cache->write();
}

void NetHandler::showStatus()
{
    //cout<<"########################"<<endl;
    //cout<<"NetHandlerStatus:"<<endl;
    //cout<<"Num of Cache:"<<fd_cache_.size()<<endl;
    //cout<<"########################"<<endl;
}

void NetHandler::cleanZombieConnection(long long time)
{
    pthread_mutex_lock(&fd_cache_mutex_);
    for(map<int,ConnectionCache * >::iterator it = fd_cache_.begin();
        it != fd_cache_.end();){
        if(time - it->second->create_time_ >= 30){
            //printf("Zombie connection,removed!\n");
            LOG4CXX_INFO(logger_, "Zombie connection,removed");
            FD_CLR(it->first,&master_);
            shutdown(it->first,SHUT_RDWR);
            delete it->second;
            fd_cache_.erase(it++);
        }else{
            it++;
        }
    }
    pthread_mutex_unlock(&fd_cache_mutex_);
}
