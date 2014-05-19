//
//  Stage.h
//  hero
//
//  Created by xujianfeng on 13-8-30.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_Stage_h
#define hero_Stage_h

#include "Reward.h"
#include "../common/Const.h"

#include <vector>
using namespace std;

class SpecialStageCondition
{
public:
    SpecialStageCondition();
    ~SpecialStageCondition();
    
    long long start_;
    long long end_;
    vector<int> day_;
    int interval_;
    vector<pair<int,int> > moment_;
    int times_;
    
};

class Stage
{
public:
    Stage();
    ~Stage();
    
    int chapter_id_;
    int stage_id_;
    int level_required_;
    int energy_required_;
    int reward_gold_;
    int reward_exp_;
    int depend_chapter_;
    int depend_stage_;
    int reset_cost_;

    SpecialStageCondition * ssc_;
    
    vector<StageReward> rewards_;
};

class StageRecord
{
public:
    StageRecord();
    ~StageRecord();
    
    long long  record_id_;
    uid_type owner_id_;
    long long update_time_;
    vector<int> record_;
};

#endif
