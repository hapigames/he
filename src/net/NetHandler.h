//
//  NetHandler.h
//  hero
//
//  Created by xujianfeng on 13-8-5.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_NetHandler_h
#define hero_NetHandler_h

#include "ConnectionCache.h"
#include "NetUtil.h"
#include "../common/Const.h"
#include "../event/EventQueue.h"
#include "../common/GameConfig.h"

#include <map>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#include <log4cxx/logger.h>
#include <log4cxx/log4cxx.h>
#include <log4cxx/propertyconfigurator.h>
//#include "3rd-party/lib4cxx/include/log4cxx/logger.h"
//#include "3rd-party/lib4cxx/include/log4cxx/log4cxx.h"
//#include "3rd-party/lib4cxx/include/log4cxx/propertyconfigurator.h"

using namespace std;




class NetHandler{
public:
    NetHandler(EventQueue * eq,int server_id);
    ~NetHandler();
    
    void start();
    ConnectionCache * addConnection(int fd);
    ConnectionCache * getConnectionByFd(int fd);
    void removeConnection(int fd);
    void closeConnection(int fd);
    bool sendString(int fd,string &s);
    void showStatus();
    void cleanZombieConnection(long long time);
    
    int nh_id_;
    pthread_mutex_t fd_cache_mutex_;
    map< int, ConnectionCache * > fd_cache_;
    int listener_fd_;
    EventQueue * eq_;
    fd_set master_;
    
    time_t show_status_timestamp_;
    log4cxx::LoggerPtr logger_;
};


#endif
