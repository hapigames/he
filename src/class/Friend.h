//
//  Friend.h
//  hero
//
//  Created by xujianfeng on 13-8-29.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_Friend_h
#define hero_Friend_h

#include "../common/Const.h"

class FriendRequestInfo
{
public:
    FriendRequestInfo();
    ~FriendRequestInfo();
    
    long long record_id_;
    long long fuid_;
    long long send_time_;

};
class FriendInfo
{
public:
    FriendInfo();
    ~FriendInfo();
    
    long long record_id_;
    uid_type fuid_;
    long long last_send_time_;
    long long last_receive_time_;
    int accepted_;
};

#endif
