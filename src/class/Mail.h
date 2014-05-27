//
//  Mail.h
//  hero
//
//  Created by xujianfeng on 13-9-18.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_Mail_h
#define hero_Mail_h

#include <string>
#include <vector>
#include "Reward.h"
using namespace std;

class News
{
public:
    News();
    ~News();
    int news_id;
    string name;
    int action;
    int param_1;
    int param_2;
    int param_3;
};

class MailConfig
{
public:
    MailConfig();
    ~MailConfig();
    int mid;
    string title;
    long long start_time;
    long long end_time;
    vector<StageReward> reward;
};
class Mail
{
public:
    Mail();
    ~Mail();
    long long mail_id_;
    long long owner_id_;
    long long sender_id_;
    long long send_time_;
    int haveread_;
    int acceptable_;
    string text_;
    string title_;
    int count;
    string contents;
    
};

#endif
