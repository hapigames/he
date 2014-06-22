//
//  main.cpp
//  hero
//
//  Created by xujianfeng on 13-8-5.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

/*#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include "EventQueue.h"
#include "NetHandler.h"
#include "GameMaster.h"
#include "GameConfig.h"

using namespace std;
GameConfig game_config("server.cfg","game.cfg");
int main(int argc, const char * argv[])
{
    if(game_config.loadFromFile()==false){
        exit(1);
    }
    srand((unsigned int)time(NULL));
    EventQueue * eq = new EventQueue();
    NetHandler * nh = new NetHandler(eq);
    GameMaster * gm = new GameMaster(eq,nh);
    gm->start();
    nh->start();
    return 0;

}*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <iostream>
#include <string>
#include <errno.h>
#include "../event/EventQueue.h"
#include "../net/NetHandler.h"
#include "GameMaster.h"
#include "../common/GameConfig.h"
using namespace std;
GameConfig game_config("server.cfg","game.cfg");
static pthread_mutex_t g_status_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_status_cond = PTHREAD_COND_INITIALIZER;

int server_id = 0;
extern int errno;
static int is_end_proc = 0;
static int end_signo = 0;
static sigset_t sig_mask,old_sig_mask;
pthread_t sig_thread_no;
void *sig_thread(void *arg)
{
    for(;;){
        int signo = 0;
        if(sigwait(&sig_mask, &signo) != 0){
            cout<<"sigwait error"<<endl;
        }
        switch(signo){
            case SIGINT:
            case SIGQUIT:
            case SIGTERM:
                pthread_mutex_lock(&g_status_lock);
                is_end_proc = 1;
                end_signo = signo;
                cout<<"signo:"<<signo<<endl;
                pthread_mutex_unlock(&g_status_lock);
                pthread_cond_signal(&g_status_cond);
                break;
            case SIGPIPE:
                break;
            default:
                break;
        }
    }
    return NULL;
}
int register_signal()
{
    sigemptyset(&sig_mask);
    sigaddset(&sig_mask,SIGINT);
    sigaddset(&sig_mask,SIGQUIT);
    sigaddset(&sig_mask,SIGTERM);
    sigaddset(&sig_mask,SIGPIPE);
    return pthread_sigmask(SIG_BLOCK, &sig_mask, &old_sig_mask);
}
int restore_signal()
{
    return pthread_sigmask(SIG_SETMASK, &old_sig_mask, NULL);
}
static int reset_rlimit()
{
    int max_conns = 4096;
    struct rlimit rlim;
    struct rlimit new_rlim;
    if(getrlimit(RLIMIT_CORE,&rlim) != 0){
        cout<<"fail to get rlimit core size"<<endl;
    }else{
        new_rlim.rlim_cur = new_rlim.rlim_max = RLIM_INFINITY;
        if(setrlimit(RLIMIT_CORE, &new_rlim) != 0){
            new_rlim.rlim_cur = new_rlim.rlim_max = rlim.rlim_max;
            if(setrlimit(RLIMIT_CORE,&new_rlim) != 0){
                cout<<"set rlimit fail"<<endl;
                return -1;
            }
        }
        rlim.rlim_cur = rlim.rlim_max = -1;
        if(getrlimit(RLIMIT_CORE, &rlim) != 0 || rlim.rlim_cur == 0){
            cout<<"fail get rlimit core"<<endl;
            return -2;
        }else{
            cout<<"get rlimit succ"<<endl;
        }
    }
    
    cout<<rlim.rlim_cur<<" "<<rlim.rlim_max<<endl;
    if(getrlimit(RLIMIT_NOFILE, &rlim) != 0){
        cout<<"fail to get rlimitnum of file"<<endl;
    }else{
        cout<<rlim.rlim_cur<<" "<<rlim.rlim_max<<endl;
        if(rlim.rlim_cur < (size_t)max_conns){
            rlim.rlim_cur = max_conns;
        }
        if(rlim.rlim_max < rlim.rlim_cur){
            rlim.rlim_max = rlim.rlim_cur;
        }
        int ret = setrlimit(RLIMIT_NOFILE,&rlim);
        if(ret != 0){
            cout<<"set rlimit nofile fail "<<rlim.rlim_cur<<" "<<rlim.rlim_max<<" "<<errno<<endl;
            return -3;
        }
        rlim.rlim_cur = rlim.rlim_max = 0;
        if(getrlimit(RLIMIT_NOFILE, &rlim) != 0 || rlim.rlim_cur == 0){
            cout<<"fail getrlimit nofile"<<endl;
            return  -4;
        }
        cout<<"rlimit nofile succ"<<endl;
    }
    return 0;
}

void *gamethread_main(void * arg)
{
    if(game_config.loadFromFile()==false){
        exit(1);
    }
    srand((unsigned int)time(NULL));
    EventQueue * eq = new EventQueue();
    NetHandler * nh = new NetHandler(eq,server_id);
    GameMaster * gm = new GameMaster(eq,nh,server_id);
    gm->start();
    nh->start();
    return NULL;
}

int main(int argc, const char * argv[])
{
    if (argc < 2) {
        cout<<" Usage:\n";
        cout<<"      "<<argv[0]<<" serverid\n";
        cout<<"      VERSION: ";
        cout<<__DATE__<<" "<<__TIME__<<endl;
        return 1;
    }
    server_id = atoi(argv[1]);
    //server_id = 1;
    cout<<server_id<<endl;
    string pid_name = "game.e.pid";
    FILE * pid_fp = fopen(pid_name.c_str(),"w");
    if(pid_fp == NULL){
        cout<<"write pid file error"<<endl;
        return 0;
    }
    fclose(pid_fp);
    
    if(reset_rlimit() != 0){
        return 0;
    }
    
    if(register_signal() != 0){
        return 0;
    }
    
    pthread_mutex_lock(&g_status_lock);
    pthread_t thread;
    pthread_create(&sig_thread_no, NULL, sig_thread, NULL);
    
    pthread_create(&thread,NULL,gamethread_main,NULL);
    
    while(!is_end_proc){
        pthread_cond_wait(&g_status_cond, &g_status_lock);
    }
    pthread_mutex_unlock(&g_status_lock);
    
    restore_signal();
    remove(pid_name.c_str());
    
    raise(SIGKILL);
    
    return 0;
    
}
