//
//  Payment.cpp
//  hero
//
//  Created by xujianfeng on 13-12-17.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#include "Payment.h"

PaymentInfo::PaymentInfo(string &tran_id,string &product_id,string &check_info)
{
    tran_id_ = tran_id;
    product_id_ = product_id;
    check_info_ = check_info;
}
PaymentInfo::~PaymentInfo()
{}
NewPaymentInfo::NewPaymentInfo(string &tran_id,string &product_id,string &order_money,string &local_tran_id)
{
    tran_id_ = tran_id;
    product_id_ = product_id;
    order_money_ = order_money;
    local_tran_id_ = local_tran_id;
}
NewPaymentInfo::~NewPaymentInfo()
{
    
}
PaymentConfig::PaymentConfig(string &product_id,string &price,int amount,int local_id)
{
    product_id_  =product_id;
    price_ = price;
    amount_ = amount;
    local_id_ = local_id;
}
PaymentConfig::~PaymentConfig()
{}