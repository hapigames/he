//
//  NetUtil.cpp
//  hero
//
//  Created by xujianfeng on 13-8-5.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#include "NetUtil.h"

int init_socket(const char * address,int port)
{
    int socket_fd = -1;
    struct sockaddr_in addr;
    
    socket_fd = socket(PF_INET,SOCK_STREAM,0);
    if(socket_fd == -1){
        printf("%s\n","socket call failed");
        return -1;
    }
    
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET,address,&addr.sin_addr);
    addr.sin_port = htons(port);
    
    int optval = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    setsockopt(socket_fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
    
    
    if(bind(socket_fd,(struct sockaddr *)&addr,sizeof(addr)) == -1){
        close(socket_fd);
        printf("%s\n","bind call failed");
        return -1;
    }
    
    if(listen(socket_fd,1024) == -1){
        close(socket_fd);
        printf("%s\n","listen call failed");
        return -1;
    }
    
    
    return socket_fd;
}

int open_listener(const char * address,int port)
{
    int socket_fd = init_socket(address,port);
    if(socket_fd < 0){
        return -1;
    }
    if(set_nonblock_fd(socket_fd)<0){
        close(socket_fd);
        return -1;
    }
    return socket_fd;
}

int set_nonblock_fd(int fd)
{
    int flag = fcntl(fd,F_GETFL,0);
    if(fcntl(fd,F_SETFL,flag|O_NONBLOCK) < 0){
        return -1;
    }
    return 0;
}