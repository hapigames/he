//
//  Item.h
//  hero
//
//  Created by xujianfeng on 13-8-22.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_Item_h
#define hero_Item_h

#include "../common/Const.h"

class Soul
{
public:
    Soul();
    ~Soul();
    int soul_mid;
    int type;
    int mid;
    int star;
    int level;
    int amount;
    int gold;
};

class Item
{
public:
    Item();
    ~Item();
    
    long long item_id_;//0
    uid_type owner_id_;//1
    int type_;//2
    int mid_;//3
    int amount_;//4
    long long get_time_;
    
};


class GearInf {
public:
    GearInf():id_(0),uid_(0), hero_id_(0), mid_(0), level_(0) {}
    GearInf(int gid, int uid, int hid, int mid, int lv)
        :id_(gid), uid_(uid), hero_id_(hid), mid_(mid), level_(lv) {}

    long long id_;
    long long uid_;
    long long hero_id_; //被装备在哪个英雄上
    int mid_;
    int level_;

};

#endif
