//
//  GameMaster.h
//  hero
//
//  Created by xujianfeng on 13-8-7.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_GameMaster_h
#define hero_GameMaster_h

#include "../event/EventQueue.h"
#include "../net/NetHandler.h"
#include "EventHandler.h"

#include <iostream>
#include <vector>
#include <pthread.h>

#include <string>
#include <log4cxx/logger.h>
#include <log4cxx/log4cxx.h>
#include <log4cxx/propertyconfigurator.h>
//#include "3rd-party/lib4cxx/include/log4cxx/logger.h"
//#include "3rd-party/lib4cxx/include/log4cxx/log4cxx.h"
//#include "3rd-party/lib4cxx/include/log4cxx/propertyconfigurator.h"


using namespace std;

class GameMaster
{
public:
    GameMaster(EventQueue * eq,NetHandler * nh,int server_id);
    ~GameMaster();
    
    bool start();
    static void * thread_fun(void* args);
    void run();
    
    pthread_t tid_;
    EventQueue * eq_;
    NetHandler * nh_;
    EventHandler * eh_;
    log4cxx::LoggerPtr logger_;
    
    int server_id_;
    long long db_reconnect_time_;
    
};

#endif
