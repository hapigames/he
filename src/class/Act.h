//
//  Act.h
//  hero
//
//  Created by xujianfeng on 14-3-11.
//  Copyright (c) 2014年 HYLD. All rights reserved.
//

#ifndef hero_Act_h
#define hero_Act_h

#include <vector>
using namespace std;

class ActConfig
{
public:
    ActConfig();
    ~ActConfig();
    
    int act_id_;
    long long start_;
    long long end_;
    int version_;
    
};

class Act
{
public:
    Act();
    ~Act();
    
    long long global_id_;
    int act_id_;
    int version_;
    vector<long long> record_;
};


#endif
