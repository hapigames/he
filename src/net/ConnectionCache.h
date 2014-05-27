//
//  ConnectionCache.h
//  hero
//
//  Created by xujianfeng on 13-8-5.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_ConnectionCache_h
#define hero_ConnectionCache_h

#include "../common/Const.h"
#include "stddef.h"
#include "../utils/Util.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
using namespace std;


class ConnectionCache
{
public:
    ConnectionCache(int fd);
    ~ConnectionCache();
    //bool read();
    int read();
    bool assemble(string &str);
    bool write(string &s);
    inline void lockWrite(){
        pthread_mutex_lock(&write_mutex_);
    }
    inline void unlockWrite(){
        pthread_mutex_unlock(&write_mutex_);
    }
    bool prepareToWrite(string &s);
    bool prepareToWrite(const char *str, size_t len);
    bool write(bool block  = false);

    int fd_;
    uid_type uid_;
    char *rbuf_;
    char *wbuf_;
    char *assemblebuf_;
    size_t rsize_;
    size_t wsize_;
    int rpos_;
    int wpos_;
    long long create_time_;
    
    bool remove_;
    bool aborted_;
    bool idle_;
    pthread_mutex_t write_mutex_;
};

#endif
