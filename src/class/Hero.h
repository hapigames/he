//
//  Hero.h
//  hero
//
//  Created by xujianfeng on 13-8-20.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_Hero_h
#define hero_Hero_h

#include "../common/Const.h"
#include "Reward.h"
#include <vector>
using namespace std;
class HeroConfig
{
public:
    HeroConfig();
    ~HeroConfig();
    
    int mid_;
    int star_;
    int leadership_require_;
    int price_;
    int price_plus_;
    int level_limit_;
    int type_;
    //vector<EnhanceCost> enhance_cost_;
    int fake_exp_;
};
class Hero
{
public:
    Hero();
    ~Hero();
    
    long long hero_id_;//0
    uid_type owner_id_;//1
    long long get_time_;//2
    int mid_;//3
    int exp_;//4
    int level_;//5
    int star_;//6
    vector<int> skill_;//7
    int locked_;//8
    
};

#endif
