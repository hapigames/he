//
//  Payment.h
//  hero
//
//  Created by xujianfeng on 13-12-17.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_Payment_h
#define hero_Payment_h
#include "../common/Const.h"
#include "../event/EventQueue.h"
#include <string>
using namespace std;

struct PaymentParam
{
    uid_type uid;
    string *verify_info;
    EventQueue * eq;
    int fd_;
    string * tran_id;
    int server_id;
};
struct PaymentAdd
{
    uid_type uid;
    string * tran_id;
    string * product_id;
    EventQueue * eq;
    int server_id;
    int fd_;
};
struct NewPaymentAdd
{
    uid_type uid;
    int local_product_id;
    string * tran_id;
    EventQueue * eq;
    int server_id;
    int fd_;
};
class PaymentInfo
{
public:
    PaymentInfo(string &tran_id,string &product_id,string &check_info);
    ~PaymentInfo();
    string tran_id_;
    uid_type uid_;
    long long pay_time_;
    string product_id_;
    int server_id_;
    int status_;
    string check_info_;
};
class NewPaymentInfo
{
public:
    NewPaymentInfo(string &tran_id,string &product_id,string &order_money,string &local_tran_id);
    ~NewPaymentInfo();
    string tran_id_;
    uid_type uid_;
    string product_id_;
    int server_id_;
    int order_status_;
    string order_money_;
    string local_tran_id_;
};
class PaymentConfig
{
public:
    PaymentConfig(string &produce_id,string &price,int amount,int local_id);
    ~PaymentConfig();
    string product_id_;
    string price_;
    int amount_;
    int local_id_;
};
#endif
