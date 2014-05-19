//
//  EventHandlerAppStore.cpp
//  hero
//
//  Created by xujianfeng on 14-3-11.
//  Copyright (c) 2014年 HYLD. All rights reserved.
//

#include "EventHandler.h"

#if (PLATFORM & PLATFORM_APPSTORE) != 0
void EventHandler::processRegister(EventCmd &e,vector<string> &check_cmd)
{
    if(check_cmd.size()!=4) return;
    //1.eqid 2.name 3.passwd
    if(check_cmd[1].size() == 0 || check_cmd[2].size() == 0 || check_cmd[3].size() == 0) return;
    if(check_cmd[1].size()>32 || check_cmd[2].size()>32 || check_cmd[3].size()>32) return;
    if(checkString(check_cmd[1])==false || checkString(check_cmd[2])==false || checkString(check_cmd[3]) == false){
        LOG4CXX_WARN(logger_, "check string false:"<<check_cmd[1]<<";"<<check_cmd[2]<<";"<<check_cmd[3]);
    }
    bool check = false;
    check = dh_->usernameHasBeenUsed(check_cmd[2]);
    if(check == true){
        sendErrorResponse(e.fd_, CMD_REGISTER,ERROR_USERNAME_HAS_BEEN_USED);
        return;
    }else{
        if(dh_->db_error_ != 0){
            sendErrorResponse(e.fd_, CMD_REGISTER, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        long long user_id;
        check = dh_->registerNewUser(check_cmd[2],check_cmd[3],check_cmd[1],user_id);
        if(check == true){
            LOG4CXX_INFO(logger_, "register;"<<user_id);
            string res = buildRegisterResponse();
            nh_->sendString(e.fd_, res);
            return;
        }else{
            sendErrorResponse(e.fd_, CMD_REGISTER,ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    //string res = buildRegisterResponse();
    //nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processLogin(EventCmd &e, vector<string> &check_cmd)
{
    cout<<check_cmd[1]<<endl;
    if(check_cmd.size()!=4) return;
    if(check_cmd[1].size() == 0 || check_cmd[2].size() == 0 || check_cmd[3].size() == 0) return;
    if(check_cmd[1].size()>32 || check_cmd[2].size()>32 || check_cmd[3].size() > 32) return;
    if(checkString(check_cmd[1])==false || checkString(check_cmd[2])==false || checkString(check_cmd[3]) == false){
        LOG4CXX_WARN(logger_, "check string false:"<<check_cmd[1]<<";"<<check_cmd[2]<<";"<<check_cmd[3]);
    }
    UserBasicInfo ubi;
    bool check = dh_->getUserBasicInfo(check_cmd[2], check_cmd[3], ubi);
    if(check == false){
        if(dh_->db_error_ != 0){
            sendErrorResponse(e.fd_, CMD_LOGIN,ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }else{
            sendErrorResponse(e.fd_, CMD_LOGIN,ERROR_USER_OR_PASSWD_WRONG);
            return;
        }
    }else{
        /*if(check_cmd[3] == ubi.eqid_){
         string res = buildLoginResponse(ubi.uid_, -1);
         nh_->sendString(e.fd_, res);
         return;
         }else{
         check = dh_->changeEquipment(ubi.uid_, check_cmd[3]);
         if(dh_->db_error_ !=0||check == false){
         sendErrorResponse(e.fd_, CMD_LOGIN,ERROR_SYSTEM_CRITICAL_ERROR);
         return;
         }else{
         string res = buildLoginResponse(ubi.uid_, getAuthorizeCode(ubi.uid_));
         nh_->sendString(e.fd_, res);
         return;
         }
         }*/
        if(check_cmd[1]!= ubi.eqid_){
            check = dh_->changeEquipment(ubi.uid_, check_cmd[1]);
            if(dh_->db_error_ !=0||check == false){
                sendErrorResponse(e.fd_, CMD_LOGIN,ERROR_SYSTEM_CRITICAL_ERROR);
                return;
            }
        }
        LOG4CXX_INFO(logger_, "login;"<<ubi.uid_);
        long long now = time(NULL);
        long long reaccess_check = now/86400 - ubi.create_time_/86400;
        if((reaccess_check>=1 && reaccess_check<=6)||reaccess_check == 13 || reaccess_check == 29 || reaccess_check == 59 || reaccess_check == 89){
            LOG4CXX_INFO(logger_, "reaccess;"<<reaccess_check<<";"<<ubi.uid_);
        }
        string res = buildLoginResponse(ubi.uid_, getAuthorizeCode(ubi.uid_));
        nh_->sendString(e.fd_, res);
        return;
    }
}
void EventHandler::processAuthorize(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4) return;
    if(check_cmd[1].size() == 0 || check_cmd[2].size() == 0) return;
    if(check_cmd[1].size()>32 || check_cmd[2].size()>32) return;
    if(checkString(check_cmd[2])==false){
        LOG4CXX_WARN(logger_, "check string false:"<<check_cmd[2]);
    }
    uid_type uid ;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    int authorize_code;
    check = safeAtoi(check_cmd[3], authorize_code);
    if(check == false) return;
    User * user = dh_->getUser(uid);
    if(dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_AUTHORIZE,ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    if(user == NULL){
        sendErrorResponse(e.fd_, CMD_AUTHORIZE,ERROR_USER_DONOT_EXIST);
        return;
    }
    if(authorize_code != getAuthorizeCode(uid)){
        sendErrorResponse(e.fd_, CMD_AUTHORIZE, ERROR_AUTHORIZE_CODE_WRONG);
        return;
    }
    user->eqid_ = check_cmd[2];
    user->dirty_ = true;
    check = dh_->saveEquipmentId(user);
    if(dh_->db_error_ != 0 || check == false){
        sendErrorResponse(e.fd_, CMD_AUTHORIZE,ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildAuthorizeResponse();
    nh_->sendString(e.fd_, res);
    return;
}

void EventHandler::processCreateUser(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=5)return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    if(check_cmd[3].size() == 0 || check_cmd[3].size()>32) return;
    if(checkString(check_cmd[3]) == false) return;
    int authorize_code;
    check = safeAtoi(check_cmd[4], authorize_code);
    if(check == false) return;
    if(authorize_code != getAuthorizeCode(uid)){
        sendErrorResponse(e.fd_, CMD_CREATEUSER, ERROR_AUTHORIZE_CODE_WRONG);
        return;
    }
    User * user = dh_->getUser(uid);
    if(dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_CREATEUSER, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    if(user != NULL){
        sendErrorResponse(e.fd_, CMD_CREATEUSER, ERROR_USER_ALREADY_REGISTERED);
        delete user;
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
    string res = buildCreateUserResponse();
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processPaymentVerify(EventCmd &e,vector<string> &check_cmd)
{
    //if(check_cmd.size()!=4) return;
    /*    uid_type uid;
     bool check = safeAtoll(check_cmd[1], uid);
     if(check == false) return;
     User * user = safeGetUser(uid, CMD_PAYMENTVERIFY, e.fd_, check_cmd[2]);
     if(user == NULL) return;
     string input = e.cmd_.substr(e.cmd_.find('{'));
     if(input.size()>=3000){
     sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
     return;
     }
     char * outputc = (char * ) malloc (4*((input.length()+2)/3));
     if(outputc == NULL){
     sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
     return;
     }
     base64_encode((unsigned char * )input.c_str(), (int)input.length(),
     (unsigned char  *)outputc, (int)4*((input.length()+2)/3));
     string output = string(outputc,(int)4*((input.length()+2)/3));
     free(outputc);
     string * test = new string("{\"receipt-data\":\"" + output + "\"}");
     //pp->verify_info = &check_cmd[3];
     pp->verify_info = test;
     pthread_create(&tid,NULL,verify,(void *)pp);
     */
    
    //read params
    uid_type uid;
    int server_id;
    string product_id;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false)return;
    if(check_cmd[3].size()>64 ||check_cmd[3].size()<=0||check_cmd[5].size()>64 ||check_cmd[5].size()<=0) return;
    //string tran_id = check_cmd[3];
    string * tran_id = new string(check_cmd[3]);
    check = safeAtoi(check_cmd[4], server_id);
    if(check == false) return;
    product_id = check_cmd[5];
    
    sconfig * sc = game_config.getSconfig(server_id);
    if(sc == NULL){
        sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_INFO_ERROR);
        return;
    }
    if(game_config.payment_.find(product_id) == game_config.payment_.end()){
        sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_INFO_ERROR);
        return;
    }
    
    //try to load
    PaymentInfo *pi = NULL;
    check = dh_->loadPayment(*tran_id, pi);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    
    string input = e.cmd_.substr(e.cmd_.find('{'));
    if(input.size()>=3000){
        //sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    char * outputc = (char *) malloc (4*((input.length()+2)/3));
    if(outputc == NULL){
        sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    base64_encode((unsigned char * )input.c_str(), (int)input.length(),
                  (unsigned char  *)outputc, (int)4*((input.length()+2)/3));
    string output = string(outputc,(int)4*((input.length()+2)/3));
    free(outputc);
    //string * test = new string("{\"receipt-data\":\"" + output + "\"}");
    
    //check
    string checkcode = MDString(output.c_str());
    if(pi == NULL){
        pi = new PaymentInfo(*tran_id,product_id,checkcode);
        if(pi == NULL){
            sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        //pi->tran_id_ = tran_id;
        pi->uid_ = uid;
        pi->pay_time_ = time(NULL);
        //pi->product_id_ = product_id;
        pi->server_id_ = server_id;
        pi->status_ = PAYMENT_STATUS_REQUEST_RECEIVED;
        pi->status_ = PAYMENT_STATUS_VERIFY_SEND;
        //pi->check_info_ = checkcode;
        
        check = dh_->addNewPayment(pi);
        if(check == false || dh_->db_error_ != 0){
            delete pi;
            sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        
    }else{
        if(uid != pi->uid_ || server_id_ != pi->server_id_ || product_id != pi->product_id_ || checkcode != pi->check_info_){
            delete pi;
            //sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_ORDER_INFO_NOT_MATCH);
            sendErrorResponseFroPayment(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_ORDER_INFO_NOT_MATCH, (*tran_id));
            return;
        }
        if(pi->status_ == PAYMENT_STATUS_REQUEST_RECEIVED || pi->status_ == PAYMENT_STATUS_VERIFY_SUCC){
        }else if(pi->status_ == PAYMENT_STATUS_VERIFY_SEND
                 || pi->status_ == PAYMENT_STATUS_ADDCMD_SEND){
            delete pi;
            sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_WRONG_TIME);
            return;
        }else if(pi->status_ == PAYMENT_STATUS_REQUEST_REFUSED || pi->status_ == PAYMENT_STATUS_ADDCMD_SUCC){
            delete  pi;
            //sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_TRADE_COMPLETE);
            sendErrorResponseFroPayment(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_TRADE_COMPLETE, (*tran_id));
            return;
        }
    }
    if(pi!=NULL){
        delete pi;
    }
    string * test = new string("{\"receipt-data\":\"" + output + "\"}");
    pthread_t tid;
    struct PaymentParam *pp;
    pp = (struct PaymentParam *) malloc (sizeof(PaymentParam));
    if(pp == NULL) {
        sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    pp->uid = uid;
    pp->tran_id = tran_id;
    pp->eq = eq_;
    pp->fd_ = e.fd_;
    pp->verify_info = test;
    pp->server_id = server_id;
    pthread_create(&tid,NULL,verify,(void *)pp);
    return;
}
void EventHandler::processPaymentVerifyReceived(EventCmd &e, vector<string> &check_cmd)
{
    /*    if(check_cmd.size()!=4)return;
     uid_type uid;
     int status = 0;
     bool check = safeAtoll(check_cmd[1], uid);
     if(check == false) return;
     check = safeAtoi(check_cmd[2], status);
     if(check == false) return;
     User * user = safeGetUser(uid, CMD_PAYMENTVERIFYRECEIVED, e.fd_);
     if(user == NULL) return;
     if(status != 0){
     sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_VERIFY_FAIL);
     return;
     }
     map<string,int>::iterator it = game_config.payment_.find(check_cmd[3]);
     if(it == game_config.payment_.end()){
     sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_INFO_ERROR);
     return;
     }
     user->diamond_ += it->second;
     check = dh_->saveUser(user);
     if(check == false) {
     sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
     return;
     }
     string res = buildPaymentVerifyReceivedResponse(user);
     nh_->sendString(e.fd_, res);*/
    return;
    
}
void EventHandler::processPaymentVerifyComplete(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=6) return;
    uid_type uid;
    int status = 0;
    int server_id = 0;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoi(check_cmd[2], status);
    if(check == false) return;
    string * tran_id = new string(check_cmd[4]);
    string * product_id = new string(check_cmd[3]);
    check = safeAtoi(check_cmd[5], server_id);
    if(check == false) return;
    
    PaymentInfo * pi = NULL;
    check = dh_->loadPayment(*tran_id, pi);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    
    if(pi == NULL){
        sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_WRONG_TIME);
        return;
    }else{
        server_id = pi->server_id_;
        if(uid != pi->uid_ || *product_id != pi->product_id_){
            delete pi;
            sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_ORDER_INFO_NOT_MATCH);
            return;
        }
        if(pi!=NULL)
            delete pi;
        if(status != 0){
            check = dh_->savePaymentStatus(*tran_id, PAYMENT_STATUS_REQUEST_RECEIVED);
            if(check == false){
                sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
                return;
            }else{
                sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_WRONG_TIME);
                return;
            }
        }else{
            check = dh_->savePaymentStatus(*tran_id, PAYMENT_STATUS_ADDCMD_SEND);
            if(check == false){
                sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
                return;
            }else{
                pthread_t tid;
                struct PaymentAdd *pa;
                pa = (struct PaymentAdd *) malloc (sizeof(PaymentAdd));
                if(pa == NULL) {
                    sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
                    return;
                }
                pa->eq = this->eq_;
                pa->fd_ = e.fd_;
                pa->product_id = product_id;
                pa->server_id = server_id;
                pa->tran_id = tran_id;
                pa->uid = uid;
                pthread_create(&tid,NULL,paymentAdd,(void *)pa);
            }
        }
    }
    return;
}
void EventHandler::processPaymentAddDiamondRequest(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=5)return;
    cout<<"cmd received!"<<endl;
    uid_type uid;
    string tran_id;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    tran_id = check_cmd[3];
    string product_id = check_cmd[2];
    int dst_fd = 0;
    check = safeAtoi(check_cmd[4], dst_fd);
    if(check == false) return;
    
    string erret = buildPaymentAddDiamondResponseCmd(1, uid, product_id, tran_id,0,dst_fd);
    
    User * user = safeGetUser(uid, CMD_PAYMENT_ADD_DIAMOND_RESPONSE, e.fd_);
    PaymentConfig * pc = game_config.getPaymentConfig(product_id);
    if(pc == NULL){
        nh_->sendString(e.fd_, erret);
        return;
    }
    
    user->diamond_ += pc->amount_;
    user->pay_count_ ++;
    ActConfig * ac = game_config.getActConfig(ACT_1);
    if(ac != NULL){
        act_1(user,pc->amount_);
    }

    check = dh_->saveUser(user);
    if(check == false) {
        nh_->sendString(e.fd_, erret);
        return;
    }
    string res = buildPaymentAddDiamondResponseCmd(0, uid, product_id, tran_id,user->diamond_,dst_fd);
    nh_->sendString(e.fd_, res);
    return;
    
}
void EventHandler::processPaymentAddDiamondResponse(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=7)return;
    int succ = 0;
    bool check = safeAtoi(check_cmd[1], succ);
    if(check == false) return;
    uid_type uid;
    string tran_id;
    check = safeAtoll(check_cmd[2], uid);
    if(check == false) return;
    tran_id = check_cmd[4];
    string product_id = check_cmd[3];
    int diamond = 0;
    check = safeAtoi(check_cmd[5], diamond);
    if(check == false) return;
    int dst_fd = 0;
    check = safeAtoi(check_cmd[6], dst_fd);
    if(check == false) return;
    
    PaymentInfo *pi = NULL;
    check = dh_->loadPayment(tran_id, pi);
    if(check == false){
        sendErrorResponse(dst_fd, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    if(pi == NULL){
        sendErrorResponse(dst_fd, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_WRONG_TIME);
        return;
    }else{
        if(uid != pi->uid_ || product_id != pi->product_id_ || tran_id != pi->tran_id_){
            delete pi;
            //sendErrorResponse(dst_fd, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_ORDER_INFO_NOT_MATCH);
            sendErrorResponseFroPayment(dst_fd, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_ORDER_INFO_NOT_MATCH, tran_id);
            return;
        }
        if(succ != 0){
            delete pi;
            check = dh_->savePaymentStatus(tran_id, PAYMENT_STATUS_VERIFY_SUCC);
            if(check == false){
                sendErrorResponse(dst_fd, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
                return;
            }else{
                sendErrorResponse(dst_fd, CMD_PAYMENTVERIFYRECEIVED, ERROR_PAYMENT_WRONG_TIME);
                return;
            }
        }else{
            check = dh_->savePaymentStatus(tran_id, PAYMENT_STATUS_ADDCMD_SUCC);
            if(check ==false){
                sendErrorResponse(dst_fd, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
                return;
            }else{
                string res = buildPaymentVerifyReceivedResponse(uid,product_id,tran_id,diamond);
                nh_->sendString(dst_fd, res);
                return;
            }
        }
    }
    return;
}
void EventHandler::processPBak(EventCmd &e, vector<string> &cmd)
{
    return;
}
void EventHandler::processSessionVerify(EventCmd &e, vector<string> &cmd)
{
    return;
}
void * verify(void * args)
{
    struct PaymentParam  * pp;
    pp = (PaymentParam *) args;
    uid_type uid = pp->uid;
    //Json::Value receipt;
    //receipt["receipt_data"] = *(pp->verify_info);
    //Json::FastWriter writer;
    //string msg = writer.write(receipt);
    string msg = *(pp->verify_info);
    cout<<msg<<endl;
    delete pp->verify_info;
    string readBuffer = "";
    
    CURL * easy_handle = curl_easy_init();
    curl_easy_setopt(easy_handle, CURLOPT_URL,"https://sandbox.itunes.apple.com/verifyReceipt");
    //curl_easy_setopt(easy_handle, CURLOPT_URL,"https://sandbox.itunes.apple.com/verifyReceipt");
    curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS,msg.c_str());
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, writeDataCurl);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA,&readBuffer);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT,30);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT,30);
    curl_easy_perform(easy_handle);
    curl_easy_cleanup(easy_handle);
    cout<<"response:"<<readBuffer<<endl;
    
    int status = 1;
    string product_id = "NULL";
    if(readBuffer.length()==0 || readBuffer[0] == '<'){
        cout<<"connection failed"<<endl;
    }else{
        Json::Reader reader;
        Json::Value root;
        if(reader.parse(readBuffer, root)){
            status = root["status"].asInt();
            if(status == 0){
                Json::Value receipt = root["receipt"];
                product_id = receipt["product_id"].asString();
            }
        }
    }
    
    string cmd = buildReceiptCmd(uid, status, product_id, *(pp->tran_id),pp->server_id);
    pp->eq->pushCmd(pp->fd_, pp->uid,cmd);
    delete pp->tran_id;
    delete pp;
    return NULL;
    
}

/*void * paymentAdd(void * args)
 {
 struct PaymentAdd * pa = (PaymentAdd *) args;
 
 int server_id = pa->server_id;
 sconfig * sc = game_config.getSconfig(server_id);
 if(sc == NULL){
 string res= buildPaymentAddDiamondResponseCmd(1, pa->uid, *(pa->product_id), *(pa->tran_id),0,pa->fd_);
 pa->eq->pushCmd(pa->fd_,pa->uid,res);
 delete pa->product_id;
 delete pa->tran_id;
 delete pa;
 return NULL;
 }
 string product_id = *(pa->product_id);
 string tran_id = *(pa->tran_id);
 
 int sock;
 struct sockaddr_in toAddr;
 memset(&toAddr,0,sizeof(toAddr));
 toAddr.sin_addr.s_addr = inet_addr(sc->ip.c_str());
 toAddr.sin_port = htons(sc->port);
 cout<<sc->ip<<";"<<sc->port<<endl;
 sock = socket(AF_INET,SOCK_STREAM,0);
 if(sock == -1){
 string res= buildPaymentAddDiamondResponseCmd(1, pa->uid, *(pa->product_id), *(pa->tran_id),0,pa->fd_);
 pa->eq->pushCmd(pa->fd_,pa->uid,res);
 delete pa->product_id;
 delete pa->tran_id;
 delete pa;
 return NULL;
 }
 int timeout = 30000;
 setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
 int ret = connect(sock,(struct sockaddr *) &toAddr,sizeof(toAddr));
 if(ret != 0){
 string res= buildPaymentAddDiamondResponseCmd(1, pa->uid, *(pa->product_id), *(pa->tran_id),0,pa->fd_);
 pa->eq->pushCmd(pa->fd_,pa->uid,res);
 delete pa->product_id;
 delete pa->tran_id;
 delete pa;
 return NULL;
 }
 
 char cmd[1024] = {0};
 int count = snprintf(cmd, 1024, "%s;%lld;%s;%s;%d",cmd_list[CMD_PAYMENT_ADD_DIAMOND_REQUEST].c_str(),pa->uid,product_id.c_str(),tran_id.c_str(),pa->fd_);
 //string cmd = buildPaymentaddDiamondRequestCmd(pa->uid, *(pa->product_id), *(pa->tran_id));
 //cout<<cmd<<endl;
 ret = send(sock,cmd,count+1,0);
 if(ret < 0){
 string res= buildPaymentAddDiamondResponseCmd(1, pa->uid, *(pa->product_id), *(pa->tran_id),0,pa->fd_);
 pa->eq->pushCmd(pa->fd_,pa->uid,res);
 delete pa->product_id;
 delete pa->tran_id;
 delete pa;
 return NULL;
 }
 
 cout<<"send succ"<<endl;
 char recvbuf[1024] = {0};
 count = recv(sock,recvbuf,1024,0);
 cout<<"receieve!"<<recvbuf<<endl;
 if(count != 0){
 string resp = string(recvbuf,count);
 pa->eq->pushCmd(pa->fd_, pa->uid, resp);
 delete pa->product_id;
 delete pa->tran_id;
 delete pa;
 return NULL;
 }else{
 string res= buildPaymentAddDiamondResponseCmd(1, pa->uid, *(pa->product_id), *(pa->tran_id),0,pa->fd_);
 pa->eq->pushCmd(pa->fd_,pa->uid,res);
 delete pa->product_id;
 delete pa->tran_id;
 delete pa;
 return NULL;
 }
 
 return NULL;
 }*/
void * paymentAdd(void * args)
{
    struct PaymentAdd * pa = (PaymentAdd *) args;
    char recvbuf[1024] = {0};
    int count = 0 ;
    
    bool check = true;
    while(1){
        int server_id = pa->server_id;
        sconfig * sc = game_config.getSconfig(server_id);
        if(sc == NULL){
            check = false;
            break;
        }
        
        string product_id = *(pa->product_id);
        string tran_id = *(pa->tran_id);
        
        int sock;
        struct sockaddr_in toAddr;
        memset(&toAddr,0,sizeof(toAddr));
        toAddr.sin_addr.s_addr = inet_addr(sc->ip.c_str());
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
        count = snprintf(cmd, 1024, "%s;%lld;%s;%s;%d",cmd_list[CMD_PAYMENT_ADD_DIAMOND_REQUEST].c_str(),pa->uid,product_id.c_str(),tran_id.c_str(),pa->fd_);
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
        cout<<resp<<endl;
        pa->eq->pushCmd(pa->fd_, pa->uid, resp);
    }else{
        string res= buildPaymentAddDiamondResponseCmd(1, pa->uid, *(pa->product_id), *(pa->tran_id),0,pa->fd_);
        pa->eq->pushCmd(pa->fd_,pa->uid,res);
    }
    if(pa->product_id!=NULL) delete pa->product_id;
    if(pa->tran_id != NULL) delete pa->tran_id;
    if(pa!= NULL) delete pa;
    return NULL;
}

#endif