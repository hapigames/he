//
//  ConnectionCache.cpp
//  hero
//
//  Created by xujianfeng on 13-8-5.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#include "ConnectionCache.h"

ConnectionCache::ConnectionCache(int fd)
{
    rpos_ = 0;
    wpos_ = 0;
    fd_ = fd;
    
    wsize_ = WRITECACHESIZE;
    wbuf_ = new(nothrow) char[WRITECACHESIZE];
    if(wbuf_ == NULL){
        printf("allocate wbuf failed\n");
        return;
    }
    
    rsize_ = READCACHESIZE;
    rbuf_ = new(nothrow) char[READCACHESIZE];
    assemblebuf_ = new(nothrow) char[READCACHESIZE+1];
    if(rbuf_ == NULL || assemblebuf_ == NULL){
        printf("allocate rbuf failed\n");
        return;
    }
    
    remove_= false;
    idle_ = false;
    aborted_ = false;
    
    create_time_ = time(NULL);
    
    pthread_mutex_init(&write_mutex_, NULL);
}
ConnectionCache::~ConnectionCache()
{
    if(wbuf_!=NULL)
        delete wbuf_;
    if(rbuf_!=NULL)
        delete rbuf_;
    if(assemblebuf_ != NULL)
        delete assemblebuf_;
}

/*bool ConnectionCache::read()
{
    if((size_t)rpos_+1 == rsize_){
        printf("read cache overflow\n");
        return false;
    }
    size_t size = recv(fd_,rbuf_+rpos_,rsize_-rpos_-1,0);
    if(size<=0){
        return false;
    }
    printf("read size:%d %d\n",(int)size,rpos_);
    rpos_ += (int)size;
    rbuf_[rpos_] = '\0';
    return true;
}*/
int ConnectionCache::read()
{
    if((size_t)rpos_+1 == rsize_){
        printf("read cache overflow\n");
        return -1;
    }
    size_t size = recv(fd_,rbuf_+rpos_,rsize_-rpos_-1,0);
    if(size<=0){
        return -1;
    }
    rpos_ += (int)size;
    if(rpos_ > 0){
        if(rbuf_[rpos_-1]!='\0'){
            //cout<<rbuf_[rpos_-1]<<endl;
            return 1;
        }else{
            rbuf_[rpos_-1] = '\0';
            return 0;
        }
    }
    return -1;
    //return true;
}

bool ConnectionCache::assemble(string &str)
{
    int index = 0;
    while(rbuf_[index]!='\0'){
        assemblebuf_[index] = rbuf_[index];
        index++;
    }
    if(index<0||index >(int)rsize_){
        remove_ = true;
        return false;
    }
    else if(index == 0){
        return false;
    }
    assemblebuf_[index] = '\0';
    int length = 0;
    if(rpos_>index){
        length = index + 1;
    }else if(rpos_ == index){
        length = index;
    }else{
        remove_ = true;
        return false;
    }
    memmove(rbuf_,rbuf_+length,rpos_ - length);
    rpos_ -= length;
    rbuf_[rpos_] = '\0';
    str = string(assemblebuf_,index);
    return true;
}

bool ConnectionCache::prepareToWrite(string &s)
{
    lockWrite();
    int size = (int)s.size() + 1;
    if(size > (int)wsize_ - 1 - wpos_){
        size_t nwsize = max((int)wsize_ * 2,(int)wpos_ + size + 1024);
        printf("resize wbuf\n");
        char * nwbuf = new(nothrow)char[nwsize];
        if(nwbuf == NULL){
            printf("resize wbuf failed\n");
            unlockWrite();
            return false;
        }else{
            if(wpos_>0){
                memcpy(nwbuf,wbuf_,wpos_);
            }
            delete wbuf_;
            wsize_ = nwsize;
            wbuf_ = nwbuf;
        }
    }
    for(size_t i=0;i<s.size();i++){
        wbuf_[wpos_] = s[i];
        wpos_++;
    }
    wbuf_[wpos_] = '\0';
    wpos_ ++;
    printf("ready to send:%s\n",wbuf_);
    unlockWrite();
    return true;
}

bool ConnectionCache::prepareToWrite(const char *str, size_t len) {
    lockWrite();
    int size = (int)len + 1;
    if(size > (int)wsize_ - 1 - wpos_){
        size_t nwsize = max((int)wsize_ * 2,(int)wpos_ + size + 1024);
        printf("resize wbuf\n");
        char * nwbuf = new(nothrow)char[nwsize];
        if(nwbuf == NULL){
            printf("resize wbuf failed\n");
            unlockWrite();
            return false;
        }else{
            if(wpos_>0){
                memcpy(nwbuf,wbuf_,wpos_);
            }
            delete wbuf_;
            wsize_ = nwsize;
            wbuf_ = nwbuf;
        }
    }
    memcpy(wbuf_ + wpos_, str, len);
    wpos_ += len;
    wbuf_[wpos_] = '\0';
    wpos_ ++;
    printf("ready to send:%s\n",wbuf_);
    unlockWrite();
    return true;
}

bool ConnectionCache::write(bool block)
{
    lockWrite();
    if(wpos_ == 0){
        unlockWrite();
        return true;
    }
    size_t size = send(fd_,wbuf_,wpos_,block?0:MSG_DONTWAIT);
    printf("send:%s\n",wbuf_);
    if((int)size < 0){
        unlockWrite();
        printf("write failed\n");
        return false;
    }else{
        memmove(wbuf_,wbuf_ + size,wpos_ - size);
        wpos_ -= (int)size;
        bool finished = (wpos_ == 0);
        unlockWrite();
        if(!finished){
            printf("send not finished\n");
        }
        return finished;
        
    }
}
