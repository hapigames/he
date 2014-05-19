//
//  EventQueue.cpp
//  hero
//
//  Created by xujianfeng on 13-8-6.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#include "EventQueue.h"

EventCmd::EventCmd(int fd,uid_type uid,string &s)
{
    fd_ = fd;
    uid_ = uid;
    cmd_ = s;
}
EventCmd::~EventCmd()
{
}
EventQueue::EventQueue()
{
    pthread_mutex_init(&event_queue_mutex_,NULL);
    while(!eq_.empty()){
        eq_.pop();
    }
}

EventQueue::~EventQueue()
{
    pthread_mutex_destroy(&event_queue_mutex_);
}

void EventQueue::pushCmd(int fd,uid_type uid,string s){
    acquireLock();
    eq_.push(EventCmd(fd,uid,s));
    releaseLock();
}




