//
//  EventQueue.h
//  hero
//
//  Created by xujianfeng on 13-8-6.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_EventQueue_h
#define hero_EventQueue_h

#include "../common/Const.h"
#include <string>
#include <queue>
#include <pthread.h>
using namespace std;


class EventCmd
{
public:
    EventCmd(int fd,uid_type uid,string &cmd);
    ~EventCmd();
    
    int fd_;
    uid_type uid_;
    string cmd_;
};
class EventQueue
{
public:
    EventQueue();
    ~EventQueue();
    
    queue<EventCmd> &getEventQueue(){return eq_;}
    inline void acquireLock(){
        pthread_mutex_lock(&event_queue_mutex_);
    }
    inline void releaseLock(){
        pthread_mutex_unlock(&event_queue_mutex_);
    }
    void pushCmd(int fd,uid_type uid,string s);
    
    queue<EventCmd> eq_;
    pthread_mutex_t event_queue_mutex_;
};

#endif
