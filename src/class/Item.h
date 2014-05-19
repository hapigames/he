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

#endif
