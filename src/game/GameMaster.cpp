//
//  GameMaster.cpp
//  hero
//
//  Created by xujianfeng on 13-8-7.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#include "GameMaster.h"
GameMaster * global_gm_;
GameMaster::GameMaster(EventQueue *eq,NetHandler * nh,int server_id)
{
    eq_ = eq;
    nh_ = nh;
    server_id_ = server_id;
    
    eh_ = new EventHandler(nh_,eq_,server_id);
    log4cxx::PropertyConfigurator::configure("log4cxx.config");
    logger_ = log4cxx::Logger::getLogger("GameMaster");
    LOG4CXX_INFO(logger_,"system;Game master start");
    
    db_reconnect_time_ = 0;
}
GameMaster::~GameMaster()
{
}
bool GameMaster::start()
{
    return pthread_create(&tid_,NULL,&GameMaster::thread_fun,(void*)this) == 0;
}
void * GameMaster::thread_fun(void * args)
{
    global_gm_ = static_cast<GameMaster*>(args);
    global_gm_->run();
    pthread_exit(NULL);
}
void GameMaster::run()
{
    if(eh_->dh_->dhInitMysql() == false){
        LOG4CXX_ERROR(logger_,"system;Data handler failed to connect to db,exit!");
        exit(1);
    }
    vector<EventCmd> event_cmds;
    for(;;){
        if(eh_->dh_->db_error_ != 0){
            eh_->dh_->closeDbConnection();
            LOG4CXX_ERROR(logger_,"system;Data handler close connection to db!");
        }
        if(eh_->dh_->dbc_open_ == false){
            long long now = time(NULL);
            if(now - db_reconnect_time_ >= 60){
                if(eh_->dh_->dhInitMysql() == false){
                    LOG4CXX_ERROR(logger_,"system;Data handler failed to reconnect to db!");
                    db_reconnect_time_ = now;
                }
            }
        }
        event_cmds.clear();
        eq_->acquireLock();
        while(!(eq_->getEventQueue().empty())){
            event_cmds.push_back(eq_->getEventQueue().front());
            eq_->getEventQueue().pop();
        }
        eq_->releaseLock();
        for(size_t i=0;i<event_cmds.size();i++){
            LOG4CXX_DEBUG(logger_, "received;"<<event_cmds[i].cmd_);
            //nh_->sendString(event_cmds[i].fd_,event_cmds[i].cmd_);
            eh_->handle(event_cmds[i]);
        }
    }
}
