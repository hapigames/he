//
//  Reward.h
//  hero
//
//  Created by xujianfeng on 13-8-30.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_Reward_h
#define hero_Reward_h

class EnhanceCost
{
public:
    EnhanceCost();
    ~EnhanceCost();
    
    int type;
    int mid;
    int param;
    int param2;
    int position;
}
;
class Reward
{
public:
    Reward();
    ~Reward();
    
    int type;
    int subtype;
    int param_1;
    int param_2;
};
class StageReward
{
public:
    StageReward(int t, int st, int p1, int p2):type(t),subtype(st),param_1(p1),param_2(p2) {}
    StageReward();
    ~StageReward();
    
    int type ;
    int subtype;
    int param_1;
    int param_2;
    int pby;
};


#endif
