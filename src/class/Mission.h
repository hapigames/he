//
//  Mission.h
//  hero
//
//  Created by xujianfeng on 13-9-24.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_Mission_h
#define hero_Mission_h

#include <vector>
#include "Reward.h"
using namespace std;
class MissionConfig
{
public:
    MissionConfig();
    ~MissionConfig();
    
    int type;
    int subtype;
    int param;
    int link_id;
    vector<StageReward> reward;
};

class Mission
{
public:
    Mission();
    ~Mission();
    
    long long mission_id_;
    long long owner_id_;
    long long add_time_;
    int mission_type_;
    int mission_subtype_;
    int mission_record_;
    int complete_;
    
};
#endif
