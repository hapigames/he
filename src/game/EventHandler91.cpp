//
//  EventHandler91.cpp
//  hero
//
//  Created by xujianfeng on 14-3-11.
//  Copyright (c) 2014年 HYLD. All rights reserved.
//

#include "EventHandler.h"
#if (PLATFORM & PLATFORM_91) != 0
void EventHandler::processRegister(EventCmd &e, vector<string> &check_cmd)
{
    return;
}
void EventHandler::processAuthorize(EventCmd &e, vector<string> &check_cmd)
{
    return;
}
void EventHandler::processCreateUser(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4)return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    if(check_cmd[2].size() == 0 || check_cmd[2].size()>32) return;
    if(check_cmd[3].size() == 0 || check_cmd[3].size()>32) return;
    if(checkString(check_cmd[2]) == false || checkString(check_cmd[3]) == false) return;
    User * user = dh_->getUser(uid);
    if(dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_CREATEUSER, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    if(user != NULL){
        sendErrorResponse(e.fd_, CMD_CREATEUSER, ERROR_USER_ALREADY_REGISTERED);
        return;
    }
    /*check = dh_->checkNick(check_cmd[3]);
    if(dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_CREATEUSER, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    if(check == false){
        sendErrorResponse(e.fd_, CMD_CREATEUSER, ERROR_NICKNAME_USED);
        return;
    }*/
    check = dh_->createUser(uid,check_cmd);
    if(dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_CREATEUSER, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    if(check == false){
        sendErrorResponse(e.fd_, CMD_CREATEUSER, ERROR_CREATE_USER_FAIL);
        return;
    }
    LOG4CXX_INFO(logger_, "register;"<<uid);
    string res = buildCreateUserResponse();
    nh_->sendString(e.fd_, res);
    return;
}

void EventHandler::processLogin(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!= 5) return;
    if(check_cmd[1].size()==0 || check_cmd[2].size()==0 || check_cmd[3].size()==0 || check_cmd[4].size() == 0) return;
    if(check_cmd[1].size()>32 || check_cmd[2].size()>32 || check_cmd[3].size() > 32 || check_cmd[4].size() >32) return;
    if(checkString(check_cmd[3])==false || checkString(check_cmd[4]) == false) return;
    uid_type uid ;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    struct sessionVerifyInfo * svi = new struct sessionVerifyInfo();
    svi->uid = uid;
    svi->fd_=e.fd_;
    svi->eq = eq_;
    svi->nickname = new string(check_cmd[4]);
    svi->eqid = new string(check_cmd[2]);
    string sign = buildSessionVerifyString(check_cmd);
    svi->sign = new string(sign);
    pthread_t tid;
    pthread_create(&tid, NULL, sessionVerify, (void *)svi );
    return;
}

void EventHandler::processSessionVerify(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=5)return;
    uid_type uid;
    int status;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoi(check_cmd[2], status);
    if(check == false) return;
    if(status != 1){
        sendErrorResponse(e.fd_, CMD_LOGIN, ERROR_SESSIONID_ERROR);
        return;
    }
    User * user = safeGetUser(uid, CMD_LOGIN, e.fd_);
    if(user == NULL) {
        sendErrorResponse(e.fd_, CMD_LOGIN, ERROR_USER_DONOT_EXIST);
        return;
    }
    //user->nick_name_ = check_cmd[3];
    user->eqid_ = check_cmd[4];
    check = dh_->saveUserSInfo(user);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_LOGIN, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "login;"<<user->uid_);
    long long now = time(NULL);
    long long reaccess_check = now/86400 - user->create_time_/86400;
    if((reaccess_check>=1 && reaccess_check<=6)||reaccess_check == 13 || reaccess_check == 29 || reaccess_check == 59 || reaccess_check == 89){
        LOG4CXX_INFO(logger_, "reaccess;"<<reaccess_check<<";"<<user->uid_);
    }
    string res = buildSessionVerifyResponse();
    nh_->sendString(e.fd_, res);
    return;
}
/*void EventHandler::processNewPaymentverify(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=6) return;
    uid_type uid;
    int server_id;
    string tran_id = check_cmd[4];
    string product_id = check_cmd[5];
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoi(check_cmd[3], server_id);
    if(check == false) return;
    if(tran_id.size()==0||tran_id.size() >32) return;
    NewPaymentInfo * npi = dh_->loadNewPaymentInfo(tran_id);
    if(npi == NULL){
        //sendErrorResponse(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_PAYMENT_NOT_RECEIVED_YET);
        sendErrorResponseFroPayment(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_PAYMENT_NOT_RECEIVED_YET,tran_id);
        return;
    }
    
    if(uid!=npi->uid_ || product_id != npi->product_id_ || server_id != npi->server_id_){
        //sendErrorResponse(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_PAYMENT_ORDER_INFO_NOT_MATCH);
        sendErrorResponseFroPayment(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_PAYMENT_ORDER_INFO_NOT_MATCH,tran_id);
        return;
    }
    PaymentConfig * pc = game_config.getPaymentConfig(product_id);
    if(pc == NULL){
        //sendErrorResponse(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_CONFIG_ERROR);
        sendErrorResponseFroPayment(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_CONFIG_ERROR,tran_id);
        return;
    }
    if(pc->price_ != npi->order_money_){
        //sendErrorResponse(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_PAYMENT_ORDER_INFO_NOT_MATCH);
        sendErrorResponseFroPayment(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_PAYMENT_ORDER_INFO_NOT_MATCH,tran_id);
        return;
    }
    if(npi->order_status_ == NEW_PAYMENT_SENDDING){
        //sendErrorResponse(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_PAYMENT_WRONG_TIME);
        sendErrorResponseFroPayment(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_PAYMENT_WRONG_TIME,tran_id);
        return;
    }
    if(npi->order_status_ == NEW_PAYMENT_COMPLETE){
        //sendErrorResponse(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_PAYMENT_TRADE_COMPLETE);
        sendErrorResponseFroPayment(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_PAYMENT_TRADE_COMPLETE,tran_id);
        return;
    }
    npi->order_status_ = NEW_PAYMENT_SENDDING;
    check = dh_->saveNewPaymentStatus(npi->tran_id_, npi->order_status_);
    if(check == false){
        //sendErrorResponse(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_SYSTEM_CRITICAL_ERROR);
        sendErrorResponseFroPayment(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_SYSTEM_CRITICAL_ERROR,tran_id);
        return;
    }
    
    int local_product_id = pc->local_id_;
    
    delete npi;
    npi = NULL;
    
    pthread_t tid;
    struct NewPaymentAdd *npa;
    npa = (struct NewPaymentAdd *) malloc (sizeof(NewPaymentAdd));
    if(npa == NULL) {
        //sendErrorResponse(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_SYSTEM_CRITICAL_ERROR);
        sendErrorResponseFroPayment(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_SYSTEM_CRITICAL_ERROR,tran_id);
        return;
    }
    npa->eq = this->eq_;
    npa->fd_ = e.fd_;
    npa->local_product_id = local_product_id;
    npa->server_id = server_id;
    npa->uid = uid;
    npa->tran_id = new string(tran_id);
    pthread_create(&tid,NULL,paymentAdd,(void *)npa);
    return;
    
}*/
void EventHandler::processPBak(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=6) return;
    uid_type uid;
    int server_id;
    string tran_id = check_cmd[4];
    string product_id = check_cmd[5];
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoi(check_cmd[3], server_id);
    if(check == false) return;
    if(tran_id.size()==0||tran_id.size() >32) return;
    User * user =safeGetUser(uid, CMD_PBAK,e.fd_,check_cmd[2]);
    if(user == NULL) return;
    LOG4CXX_INFO(logger_, "pbak;"<<uid);
    NewPaymentInfo * npi = dh_->loadNewPaymentInfoBak(tran_id);
    if(npi == NULL){
        //sendErrorResponse(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_PAYMENT_NOT_RECEIVED_YET);
        sendErrorResponseFroPayment(e.fd_, CMD_PBAK, ERROR_PAYMENT_NOT_RECEIVED_YET,tran_id);
        return;
    }
    
    if(uid!=npi->uid_ || product_id != npi->product_id_ || server_id != npi->server_id_){
        //sendErrorResponse(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_PAYMENT_ORDER_INFO_NOT_MATCH);
        sendErrorResponseFroPayment(e.fd_, CMD_PBAK, ERROR_PAYMENT_ORDER_INFO_NOT_MATCH,tran_id);
        return;
    }
    if(server_id != this->server_id_){
        sendErrorResponseFroPayment(e.fd_, CMD_PBAK, ERROR_PAYMENT_ORDER_INFO_NOT_MATCH,tran_id);
        return;
    }
    PaymentConfig * pc = game_config.getPaymentConfig(product_id);
    if(pc == NULL){
        //sendErrorResponse(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_CONFIG_ERROR);
        sendErrorResponseFroPayment(e.fd_, CMD_PBAK, ERROR_CONFIG_ERROR,tran_id);
        return;
    }
    if(pc->price_ != npi->order_money_){
        //sendErrorResponse(e.fd_, CMD_NEWPAYMENTVERIFY, ERROR_PAYMENT_ORDER_INFO_NOT_MATCH);
        sendErrorResponseFroPayment(e.fd_, CMD_PBAK, ERROR_PAYMENT_ORDER_INFO_NOT_MATCH,tran_id);
        return;
    }
    if(npi->order_status_ != NEW_PAYMENT_RECEIVED){
        sendErrorResponseFroPayment(e.fd_, CMD_PBAK, ERROR_PAYMENT_TRADE_COMPLETE,tran_id);
        return;
    }
    npi->order_status_ = NEW_PAYMENT_COMPLETE;
    check = dh_->saveNewPaymentStatusBak(npi->tran_id_, npi->order_status_);
    user->diamond_ += pc->amount_;
    user->pay_count_ ++;
    ActConfig * ac = game_config.getActConfig(ACT_1);
    if(ac != NULL){
        act_1(user,pc->amount_);
    }
    check = dh_->saveUser(user);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_PBAK,ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    
    LOG4CXX_INFO(logger_, "pbaksucc;"<<uid<<";"<<user->diamond_);
    
    string res = buildPBakResponse(user, npi);
    nh_->sendString(e.fd_, res);
    
    if(npi!= NULL){
        delete npi;
        npi = NULL;
    }
    return;
}
/*void * paymentAdd(void * args)
{
    struct NewPaymentAdd * npa = (NewPaymentAdd *) args;
    char recvbuf[1024] = {0};
    int count = 0 ;
    int local_product_id = npa->local_product_id;
    string tran_id = *(npa->tran_id);
    
    bool check = true;
    while(1){
        int server_id = npa->server_id;
        sconfig * sc = game_config.getSconfig(server_id);
        if(sc == NULL){
            check = false;
            break;
        }
        
        int sock;
        struct sockaddr_in toAddr;
        memset(&toAddr,0,sizeof(toAddr));
        toAddr.sin_addr.s_addr = inet_addr(sc->ip_addr.c_str());
        toAddr.sin_port = htons(sc->port);
        sock = socket(AF_INET,SOCK_STREAM,0);
        if(sock == -1){
            check = false;
            break;
        }
        
        int timeout = 30000;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
        int ret = connect(sock,(struct sockaddr *) &toAddr,sizeof(toAddr));
        if(ret != 0){
            close(sock);
            check = false;
            break;
        }
        
        char cmd[1024] = {0};
        count = snprintf(cmd, 1024, "%s;%lld;%d;%s;%d",cmd_list[CMD_PAYMENT_ADD_DIAMOND_REQUEST].c_str(),npa->uid,local_product_id,tran_id.c_str(),npa->fd_);
        ret = send(sock,cmd,count+1,0);
        if(ret < 0){
            close(sock);
            shutdown(sock,SHUT_RDWR);
            check = false;
            break;
        }
        count = recv(sock,recvbuf,1024,0);
        if(count <= 0){
            close(sock);
            shutdown(sock,SHUT_RDWR);
            check = false;
            break;
        }
        close(sock);
        shutdown(sock,SHUT_RDWR);
        break;
    }
    if(check == true){
        string resp = string(recvbuf,count);
        npa->eq->pushCmd(npa->fd_, npa->uid, resp);
    }else{
        string res= buildPaymentAddDiamondResponseCmd(1, npa->uid, npa->local_product_id,tran_id,0,npa->fd_);
        npa->eq->pushCmd(npa->fd_,npa->uid,res);
    }
    if(npa->tran_id != NULL) delete npa->tran_id;
    if(npa!= NULL) delete npa;
    return NULL;
}*/
void * sessionVerify(void * args)
{
    struct sessionVerifyInfo * sv;
    sv = (struct sessionVerifyInfo *)args;
    uid_type uid = sv->uid;
    string nickname = *(sv->nickname);
    delete sv->nickname;
    string eqid = *(sv->eqid);
    delete sv->eqid;
    string sign = *(sv->sign);
    delete sv->sign;
    
    string readBuffer = "";
    
    CURL * easy_handle = curl_easy_init();
    curl_easy_setopt(easy_handle, CURLOPT_URL,sign.c_str());
    //curl_easy_setopt(easy_handle, CURLOPT_URL,"https://sandbox.itunes.apple.com/verifyReceipt");
    //curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS,msg.c_str());
    //curl_easy_setopt(easy_handle, CURLOPT_HTTPGET,sign.c_str());
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, writeDataCurl);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA,&readBuffer);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT,30);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT,30);
    curl_easy_perform(easy_handle);
    curl_easy_cleanup(easy_handle);
    
    int status = 0;
    if(readBuffer.length()==0 || readBuffer[0] == '<'){
    }else{
        Json::Reader reader;
        Json::Value root;
        if(reader.parse(readBuffer, root)){
            string code = root["ErrorCode"].asString();
            bool check = safeAtoi(code, status);
            if(check == false){
                delete sv ;
                return NULL;
            }
        }
    }
    
    string res = buildSessionVerifyCmd(sv->uid,status,nickname,eqid);
    sv->eq->pushCmd(sv->fd_, sv->uid, res);
    
    delete sv;
    
    return NULL;
    
}


void EventHandler::processPaymentVerify(EventCmd &e, vector<string> &check_cmd)
{
    return;
}

void EventHandler::processPaymentVerifyComplete(EventCmd &e, vector<string> &check_cmd)
{
    return;
}

void EventHandler::processPaymentVerifyReceived(EventCmd &e, vector<string> &check_cmd)
{
    return;
}
void EventHandler::processPaymentAddDiamondRequest(EventCmd &e, vector<string> &check_cmd)
{
    return;
}

void EventHandler::processPaymentAddDiamondResponse(EventCmd &e, vector<string> &check_cmd)
{
    return;
}

#endif