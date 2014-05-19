//
//  NetUtil.h
//  hero
//
//  Created by xujianfeng on 13-8-5.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_NetUtil_h
#define hero_NetUtil_h

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int init_socket(const char * address,int port);
int open_listener(const char *addr ,int port);

int set_nonblock_fd(int fd);

#endif
