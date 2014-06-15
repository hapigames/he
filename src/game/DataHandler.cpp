//
//  DataHandler.cpp
//  hero
//
//  Created by xujianfeng on 13-8-9.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#include "DataHandler.h"

extern GameConfig game_config;
DataHandler::DataHandler(int server_id)
{
    users_.clear();
    server_id_ = server_id;
    
    db_error_ = 0;
    dbc_open_ = false;
    
    fresh_.clear();
    fresh_pointer_.clear();
    for(int i = 0;i<game_config.fresh_step_count_;i++){
        vector<User * > v;
        v.clear();
        v.resize(game_config.fresh_level_size_,NULL);
        fresh_.push_back(v);
        fresh_pointer_.push_back(0);
    }
    
    news_.clear();
    for(int i = 0;i<10;i++){
        News anews;
        news_.push_back(anews);
    }
    news_pointer_ = 0;
    global_news_index_ = 1;
    
    log4cxx::PropertyConfigurator::configure("log4cxx.config");
    logger_ = log4cxx::Logger::getLogger("DataHandler");

    trial_rank_need_save_ = false;
    trial_rank_last_save_time_ = 0;
}

DataHandler::~DataHandler()
{
    mysql_close(&mysql_);
    mysql_library_end();
}
bool DataHandler::dhInitMysql()
{
    loadTrialRank();

    sconfig * sc = game_config.getSconfig(server_id_);
    if(sc == NULL) return false;
    mysql_init(&mysql_);
    if(!mysql_real_connect(&mysql_,sc->db_address.c_str(),sc->db_user.c_str(),\
                           sc->db_passwd.c_str(),sc->db_name.c_str(),sc->db_port,NULL,0)){
    //if(!mysql_real_connect(&mysql_,"106.186.119.143","root","","hero",3306,NULL,0)){
        LOG4CXX_ERROR(logger_, "system;Connect to db failed");
        return false;
    }else{
        LOG4CXX_INFO(logger_, "system;Connect to db succ");
        char opt_val = 1;
        mysql_options(&mysql_, MYSQL_OPT_RECONNECT, &opt_val);
        dbc_open_ =true;
        db_error_ = 0;
        return true;
    }
}
void DataHandler::closeDbConnection()
{
    mysql_close(&mysql_);
    mysql_library_end();
    dbc_open_ = false;
}

MYSQL_RES* DataHandler::getStoreResult(const char *sql) {
    MYSQL_RES *res = NULL;
    int ret = 0;
    if ((ret = mysql_query(&mysql_, sql)) == 0) {
        res = mysql_store_result(&mysql_);
    }
    else {
        db_error_ = ret;
    }
    return res;
}

long long DataHandler::execSqlReturnId(const char *sql) {
    long long id = -1;
    int ret = 0;
    if ((ret = mysql_query(&mysql_, sql)) == 0) {
        id = mysql_insert_id(&mysql_);
        LOG4CXX_DEBUG(logger_, "return id:"<<id);
    }
    else {
        db_error_ = ret;
    }
    return id;
}

int DataHandler::execSql(const char *sql) {
    return mysql_query(&mysql_, sql);
}


bool DataHandler::usernameHasBeenUsed(string &name)
{
    //TODO username escape
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql,1024,"%s%s%s","select user_id from ubd where username = \"",name.c_str(),"\" limit 1 ;");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_);
        MYSQL_ROW row = mysql_fetch_row(res);
        if(row){
            LOG4CXX_DEBUG(logger_, "user name has been used:"<<name);
            mysql_free_result(res);
            return true;
        }else{
            LOG4CXX_DEBUG(logger_, "user name has not been used yet:"<<name);
            mysql_free_result(res);
            return false;
        }
    }
    db_error_ = ret;
    return false;
}
bool DataHandler::registerNewUser(string &name, string &passwd,string &eqid,long long &user_id)
{
    //TODO passwd,name,eqid escape
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql,1024,"%s%s%s%s%s%s%s","insert into ubd(username,password,create_time,eqid) values (\"",name.c_str(),"\", md5(\"",passwd.c_str(),"\"), unix_timestamp() , \"",
                     eqid.c_str(),"\");");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        user_id = mysql_insert_id(&mysql_);
        LOG4CXX_DEBUG(logger_, "user registered:"<<name);
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::getUserBasicInfo(string &name,string &passwd,UserBasicInfo &ubi)
{
    //TODO username,password escape
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql,1024,"%s%s%s%s%s","select user_id,eqid,username,password,create_time from ubd where username = \"",name.c_str(),"\" and password = md5(\"",passwd.c_str(),"\") limit 1;");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        LOG4CXX_DEBUG(logger_, "get ubi succ:"<<name);
        MYSQL_RES * res = mysql_store_result(&mysql_);
        MYSQL_ROW row = mysql_fetch_row(res);
        if(row){
            bool check = safeAtoll(row[0], ubi.uid_);
            if(check == false){
                LOG4CXX_WARN(logger_, "fail to translate uid:"<<ubi.uid_);
                mysql_free_result(res);
                return false;
            }
            ubi.eqid_ = row[1];
            //ubi.username_ = row[2];
            //ubi.password_ = row[3];
            if(safeAtoll(row[4], ubi.create_time_) ==false) return false;
            mysql_free_result(res);
            return true;
        }else{
            LOG4CXX_DEBUG(logger_, "user not exist or password wrong");
            mysql_free_result(res);
            return false;
        }
    }
    db_error_ = ret;
    return false;
}
bool DataHandler::changeEquipment(uid_type uid,string &eqid)
{
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql,1024,"%s%s%s%lld%s","update ubd set eqid = \"",eqid.c_str(),"\" where user_id = ",uid,";");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        LOG4CXX_DEBUG(logger_, "user change equipment:"<<uid);
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::checkNick(string &nickname)
{
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql,1024,"%s%s%s","select user_id from user_info where nickname = \"" , nickname.c_str(),"\" limit 1 ;");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_);
        MYSQL_ROW row = mysql_fetch_row(res);
        if(row){
            LOG4CXX_DEBUG(logger_, "nickname has been used:"<<nickname);
            mysql_free_result(res);
            return false;
        }else{
            mysql_free_result(res);
            return true;
        }
    }
    db_error_ = ret;
    return false;
}
User * DataHandler::getUser(uid_type uid)
{
    map<uid_type,User *>::iterator it = users_.find(uid);
    if(it!=users_.end()){
        LOG4CXX_DEBUG(logger_,"user "<<uid<<" data in buffer");
        return it->second;
    }
    LOG4CXX_DEBUG(logger_,"try to load user from db");
    int count = 0;
    char sql[1024] ={0};
    count = snprintf(sql,1024,"%s%lld%s","select * from user_info where user_id = ",uid," limit 1;");
    bool ret = mysql_query(&mysql_,sql);
    cout<<ret<<endl;
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_);
        MYSQL_ROW row = mysql_fetch_row(res);
        if(row){
            User * user = readUser(row);
            if(user == NULL){
                LOG4CXX_ERROR(logger_,"read user "<<uid<<" fail");
                mysql_free_result(res);
                return NULL;
            }else{
                LOG4CXX_DEBUG(logger_,"read user "<<uid<<" succ");
                mysql_free_result(res);
                return user;
            }
        }
    }else{
        db_error_ = ret;
        return NULL;
    }
    return NULL;

}
Hero * DataHandler::getHero(User * user,long long hid)
{
    map<long long,Hero *>::iterator it = user->heroes_.find(hid);
    if(it != user->heroes_.end()){
        return it->second;
    }else{
        return NULL;
    }
}
Soldier * DataHandler::getSoldier(User * user, long long sid)
{
    map<long long , Soldier * >::iterator it = user->soldiers_.find(sid);
    if(it!= user->soldiers_.end()){
        return it->second;
    }else{
        return NULL;
    }
}
User * DataHandler::readUser(MYSQL_ROW &row)
{
    User * user = new User();
    if(user == NULL) return NULL;
    bool check = true;
    string delims = ",";
    
    while(1){
        check = safeAtoll(row[0], user->uid_);
        if(check == false) break;
        user->nick_name_ = row[1];
        check = safeAtoll(row[2], user->create_time_);
        if(check == false) break;
        check = safeAtoll(row[3], user->last_login_time_);
        if(check == false) break;
        user->eqid_ = row[4];
        check = safeAtoi(row[5], user->user_level_);
        if(check == false) break;
        check = safeAtoi(row[6], user->user_exp_);
        if(check == false) break;
        check = safeAtoi(row[7], user->energy_);
        if(check == false) break;
        check = safeAtoi(row[8], user->gold_);
        if(check == false) break;
        check = safeAtoi(row[9], user->diamond_);
        if(check == false) break;
        check = safeAtoll(row[10], user->last_recover_time_);
        if(check == false) break;
        check = safeAtoll(row[11], user->hero_id_);
        if(check == false) break;
        check = longtokenize(row[12], user->soldier_id_, delims);
        if(check == false) break;
        check = safeAtoi(row[13], user->normal_chapter_id_);
        if(check == false) break;
        check = safeAtoi(row[14], user->normal_stage_id_);
        if(check == false) break;
        check = safeAtoi(row[15], user->elite_chapter_id_);
        if(check == false) break;
        check = safeAtoi(row[16], user->elite_stage_id_);
        if(check == false) break;
        check = safeAtoi(row[17], user->spark_num_);
        if(check == false) break;
        check = safeAtoi(row[18], user->spark_daily_num_);
        if(check == false) break;
        check = numtokenize(row[19], user->system_mail_record_, delims);
        if(check == false) break;
        check = safeAtoi(row[20], user->login_sum_);
        if(check == false) break;
        check = safeAtoi(row[21], user->buy_energy_daily_);
        if(check == false) break;
        check = safeAtoi(row[22], user->expand_hero_);
        if(check == false) break;
        check = safeAtoi(row[23], user->expand_soldier_);
        if(check == false) break;
        check = safeAtoi(row[24], user->expand_friend_);
        if(check == false) break;
        check = safeAtoi(row[25], user->guide_record_);
        if(check == false) break;
        check = safeAtoi(row[26], user->pay_count_);
        if(check == false) break;
        check = safeAtoll(row[27], user->team_id_);
        if(check == false) break;
        check = safeAtoi(row[28], user->trial_stage_id_);
        if (check == false) break;
        check = safeAtoi(row[29], user->trial_max_stage_id_);
        if (check == false) break;
        check = safeAtoi(row[30], user->trial_daily_reset_);
        if (check == false) break;
        check = safeAtoll(row[31], user->trial_instant_start_time_);
        if (check == false) break;
        check = safeAtoi(row[32], user->honor_);
        break;
    }
    
    if(check == false){
        delete user;
        return NULL;
    }
    
    user->load_staus_ |= LOADED_USER_INFO;
    user->dirty_ = false;
    
    users_.insert(make_pair(user->uid_,user));
    return user;
    
}
bool DataHandler::saveEquipmentId(User *user)
{
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql,1024,"%s%s%s%lld%s","update user_info set eqid = \"",user->eqid_.c_str(),"\" where user_id = ",user->uid_,";");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        LOG4CXX_DEBUG(logger_, "user change equipment:"<<user->uid_);
        user->dirty_ = false;
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}

bool DataHandler::loadAllHeroes(User * user)
{
    LOG4CXX_DEBUG(logger_,"try to load heroes from db for user "<<user->uid_);
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s" , "select * from hero_info where owner_id = ",user->uid_,";");
    bool ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_);
        int mnf = mysql_num_fields(res);
        MYSQL_ROW row;
        while((row = mysql_fetch_row(res))){
            bool check = loadHero(row, mnf, user);
            if(check == false){
                LOG4CXX_ERROR(logger_,"read hero "<<row[0]<<" fail");
                continue;
            }
        }
        mysql_free_result(res);
        LOG4CXX_DEBUG(logger_,"read hero for "<<user->uid_<<" succ");
        user->load_staus_ |= LOADED_HERO_INFO;
        return true;
    }
    db_error_ = ret;
    return false;
}
bool DataHandler::loadHero(MYSQL_ROW &row ,int mnf,User * user)
{
    Hero * hero = new Hero();
    if(hero == NULL) return false;
    bool check = true;
    string delims = ",";
    
    while(1){
        check = safeAtoll(row[0],hero->hero_id_);
        if(check == false) break;
        check = safeAtoll(row[1], hero->owner_id_);
        if(check == false) break;
        check = safeAtoll(row[2], hero->get_time_);
        if(check == false) break;
        check = safeAtoi(row[3], hero->mid_);
        if(check == false) break;
        check = safeAtoi(row[4], hero->exp_);
        if(check == false) break;
        check = safeAtoi(row[5], hero->level_);
        if(check == false) break;
        check = safeAtoi(row[6], hero->star_);
        if(check == false) break;
        check = numtokenize(row[7], hero->skill_, delims);
        if(check == false) break;
        check = safeAtoi(row[8], hero->locked_);
        if(check == false) break;
        break;
    }
    if(check == false){
        delete hero;
        return false;
    }
    user->heroes_.insert(make_pair(hero->hero_id_, hero));
    return true;
}
bool DataHandler::loadAllSoldiers(User * user)
{
    LOG4CXX_DEBUG(logger_,"try to load soldier from db for user "<<user->uid_);
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s" , "select * from soldier_info where owner_id = ",user->uid_,";");
    bool ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_);
        int mnf = mysql_num_fields(res);
        MYSQL_ROW row;
        while((row = mysql_fetch_row(res))){
            bool check = loadSoldier(row, mnf, user);
            if(check == false){
                LOG4CXX_ERROR(logger_,"read soldier "<<row[0]<<" fail");
                continue;
            }
        }
        mysql_free_result(res);
        LOG4CXX_DEBUG(logger_,"read soldier for "<<user->uid_<<" succ");
        user->load_staus_ |= LOADED_SOLDIER_INFO;
        return true;
    }
    db_error_ = ret;
    return false;
}
bool DataHandler::loadSoldier(MYSQL_ROW &row, int mnf, User *user)
{
    Soldier * soldier = new Soldier();
    if(soldier == NULL) return false;
    bool check = true;
    
    while(1){
        check = safeAtoll(row[0],soldier->soldier_id_);
        if(check == false) break;
        check = safeAtoll(row[1], soldier->owner_id_);
        if(check == false) break;
        check = safeAtoll(row[2], soldier->get_time_);
        if(check == false) break;
        check = safeAtoi(row[3], soldier->mid_);
        if(check == false) break;
        check = safeAtoi(row[4], soldier->exp_);
        if(check == false) break;
        check = safeAtoi(row[5], soldier->level_);
        if(check == false) break;
        check = safeAtoi(row[6], soldier->star_);
        if(check == false) break;
        check = safeAtoi(row[7], soldier->locked_);
        if(check == false) break;
        break;

    }
    if(check == false){
        delete soldier;
        return false;
    }
    user->soldiers_.insert(make_pair(soldier->soldier_id_, soldier));
    return true;
}
bool DataHandler::loadAllItems(User * user)
{
    LOG4CXX_DEBUG(logger_,"try to load item from db for user "<<user->uid_);
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s" , "select * from item_info where owner_id = ",user->uid_,";");
    bool ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_);
        int mnf = mysql_num_fields(res);
        MYSQL_ROW row;
        while((row = mysql_fetch_row(res))){
            bool check = loadItem(row, mnf, user);
            if(check == false){
                LOG4CXX_ERROR(logger_,"read item "<<row[0]<<" fail");
                continue;
            }
        }
        mysql_free_result(res);
        LOG4CXX_DEBUG(logger_,"read item for "<<user->uid_<<" succ");
        user->load_staus_ |= LOADED_ITEM_INFO;
        return true;
    }
    db_error_ = ret;
    return false;
}
bool DataHandler::loadItem(MYSQL_ROW &row, int mnf, User *user)
{
    Item * item = new Item();
    if(item == NULL) return false;
    bool check = true;
    
    while(1){
        check = safeAtoll(row[0],item->item_id_);
        if(check == false) break;
        check = safeAtoll(row[1], item->owner_id_);
        if(check == false) break;
        check = safeAtoi(row[2], item->type_);
        if(check == false) break;
        check = safeAtoi(row[3], item->mid_);
        if(check == false) break;
        check = safeAtoi(row[4], item->amount_);
        if(check == false) break;
        check = safeAtoll(row[5], item->get_time_);
        if(check == false) break;
        break;
        
    }
    if(check == false){
        delete item;
        return false;
    }
    user->items_.insert(make_pair(item->item_id_,item));
    return true;
}
bool DataHandler::loadAllFriends(User *user)
{
    LOG4CXX_DEBUG(logger_, "try to load friends from db for user "<<user->uid_);
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s" , "select * from friend_info where owner_id = ",user->uid_,";");
    bool ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_);
        int mnf = mysql_num_fields(res);
        MYSQL_ROW row;
        while((row = mysql_fetch_row(res))){
            bool check = loadFriend(row, mnf, user);
            if(check == false){
                LOG4CXX_ERROR(logger_,"read friend "<<row[0]<<" fail");
                continue;
            }
        }
        mysql_free_result(res);
        LOG4CXX_DEBUG(logger_,"read friend for "<<user->uid_<<" succ");
        user->load_staus_ |= LOADED_FRIEND_INFO;
        return true;
    }
    db_error_ = ret;
    return false;
}
bool DataHandler::loadFriend(MYSQL_ROW &row, int mnf, User *user)
{
    FriendInfo * fi = new FriendInfo();
    if(fi == NULL) return false;
    bool check = true;
    while(1){
        check = safeAtoll(row[0], fi->record_id_);
        if(check == false) break;
        check = safeAtoll(row[2], fi->fuid_);
        if(check == false) break;
        check = safeAtoll(row[3], fi->last_send_time_);
        if(check == false) break;
        check = safeAtoll(row[4], fi->last_receive_time_);
        if(check == false) break;
        check = safeAtoi(row[5], fi->accepted_);
        if(check == false) break;
        break;
    }
    if(check == false){
        delete fi;
        return false;
    }
    user->friends_.insert(make_pair(fi->fuid_,fi));
    return true;
}
bool DataHandler::loadAllFriendRequest(User *user)
{
    
    LOG4CXX_DEBUG(logger_, "try to load friend request from db for user "<<user->uid_);
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s" , "select * from friend_request_info where tuid = ",user->uid_,";");
    bool ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_);
        int mnf = mysql_num_fields(res);
        MYSQL_ROW row;
        while((row = mysql_fetch_row(res))){
            bool check = loadFriendRequest(row, mnf, user);
            if(check == false){
                LOG4CXX_ERROR(logger_,"read friend request "<<row[0]<<" fail");
                continue;
            }
        }
        mysql_free_result(res);
        LOG4CXX_DEBUG(logger_,"read friend request for "<<user->uid_<<" succ");
        user->load_staus_ |= LOADED_FRIENDREQUEST_INFO;
        return true;
    }
    db_error_ = ret;
    return false;
}
bool DataHandler::loadFriendRequest(MYSQL_ROW &row, int mnf, User *user)
{
    bool check = true;
    FriendRequestInfo * fr = new FriendRequestInfo();
    if(fr == NULL) return false;
    while(1){
        check = safeAtoll(row[0], fr->record_id_);
        if(check == false) break;
        check = safeAtoll(row[1], fr->fuid_);
        if(check == false)break;
        check = safeAtoll(row[2], fr->send_time_);
        if(check == false) break;
        break;
    }
    if(check == false){
        delete fr;
        return false;
    }
    user->friend_requests_.insert(make_pair(fr->fuid_, fr));
    return true;
}
bool DataHandler::loadAllMails(User *user)
{
    LOG4CXX_DEBUG(logger_, "try to load mail from db for user "<<user->uid_);
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s" , "select * from mail_info where owner_id = ",user->uid_,";");
    bool ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_);
        int mnf = mysql_num_fields(res);
        MYSQL_ROW row;
        while((row = mysql_fetch_row(res))){
            bool check = loadMail(row, mnf, user);
            if(check == false){
                LOG4CXX_ERROR(logger_,"read mail "<<row[0]<<" fail");
                continue;
            }
        }
        mysql_free_result(res);
        LOG4CXX_DEBUG(logger_,"read mail for "<<user->uid_<<" succ");
        user->load_staus_ |= LOADED_MAIL;
        return true;
    }
    db_error_ = ret;
    return false;
}
bool DataHandler::loadMail(MYSQL_ROW &row, int mnf, User *user)
{
    Mail * mail = new Mail();
    if(mail == NULL) return false;
    bool check = true;
    
    while(1){
        check = safeAtoll(row[0], mail->mail_id_);
        if(check == false) break;
        check = safeAtoll(row[1], mail->owner_id_);
        if(check == false) break;
        check = safeAtoll(row[2], mail->sender_id_);
        if(check == false) break;
        check = safeAtoll(row[3], mail->send_time_);
        if(check == false) break;
        mail->text_ = row[4];
        check = safeAtoi(row[5], mail->haveread_);
        if(check == false) break;
        check = safeAtoi(row[6], mail->acceptable_);
        if(check == false) break;
        mail->title_ = row[7];
        check = safeAtoi(row[8], mail->count);
        if (check == false) break;
        mail->contents = row[9];
        break;
    }
    if(check == false){
        delete mail;
        return false;
    }
    user->mails_.insert(make_pair(mail->mail_id_, mail));
    return true;
}
bool DataHandler::loadAllMissions(User *user)
{
    LOG4CXX_DEBUG(logger_, "try to load mission from db for user "<<user->uid_);
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s" , "select * from mission_info where owner_id = ",user->uid_,";");
    bool ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_);
        int mnf = mysql_num_fields(res);
        MYSQL_ROW row;
        while((row = mysql_fetch_row(res))){
            bool check = loadMission(row, mnf, user);
            if(check == false){
                LOG4CXX_ERROR(logger_,"read mission "<<row[0]<<" fail");
                continue;
            }
        }
        mysql_free_result(res);
        LOG4CXX_DEBUG(logger_,"read mission for "<<user->uid_<<" succ");
        user->load_staus_ |= LOADED_MISSION;
        return true;
    }
    db_error_ = ret;
    return false;
}
bool DataHandler::loadMission(MYSQL_ROW &row, int mnf, User *user)
{
    Mission * mission = new Mission();
    if(mission == NULL) return false;
    bool check = true;
    
    while(1){
        check = safeAtoll(row[0], mission->mission_id_);
        if(check == false) break;
        check = safeAtoll(row[1], mission->owner_id_);
        if(check == false) break;
        check = safeAtoll(row[2], mission->add_time_);
        if(check == false) break;
        check = safeAtoi(row[3], mission->mission_type_);
        if(check == false) break;
        check = safeAtoi(row[4], mission->mission_subtype_);
        if(check == false) break;
        check = safeAtoi(row[5], mission->mission_record_);
        if(check == false) break;
        check = safeAtoi(row[6], mission->complete_);
        if(check == false) break;
        break;
    }
    if(check == false){
        delete mission;
        return false;
    }
    user->missions_.insert(make_pair(mission->mission_id_,mission));
    return true;
}
bool DataHandler::loadAllStageRecord(User *user)
{
    LOG4CXX_DEBUG(logger_,  "try to load stage record from db for user"<<user->uid_);
    int count = 0;
    char sql[1024] ={0};
    count = snprintf(sql, 1024, "%s%lld%s","select * from stage_record where owner_id = ", user->uid_, ";");
    bool ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_);
        int mnf = mysql_num_fields(res);
        MYSQL_ROW row;
        while((row = mysql_fetch_row(res))){
            bool check = loadStageRecord(row, mnf, user);
            if(check == false){
                LOG4CXX_ERROR(logger_,"read stage record "<<row[0]<<" fail");
                continue;
            }
        }
        mysql_free_result(res);
        LOG4CXX_DEBUG(logger_,"read stage record for "<<user->uid_<<" succ");
        user->load_staus_ |= LOADED_STAGERECORD;
        return true;
    }
    db_error_ = ret;
    return false;
}
bool DataHandler::loadStageRecord(MYSQL_ROW &row, int mnf, User *user)
{
    StageRecord * sr = new StageRecord();
    if(sr == NULL) return false;
    bool check = true;
    string delims = ",";
    while(1){
        check = safeAtoll(row[0], sr->record_id_);
        if(check == false) break;
        check = safeAtoll(row[1], sr->owner_id_);
        if(check == false) break;
        check = safeAtoll(row[2], sr->update_time_);
        if(check == false) break;
        check = numtokenize(row[3], sr->record_, delims);
        if(check == false) break;
        if(sr->record_.size()<=0){
            check = false;
            break;
        }
        break;
    }
    if(check == false){
        delete sr;
        return false;
    }
    user->stage_record_.insert(make_pair(sr->record_[0],sr));
    return true;
}
bool DataHandler::loadPayment(string& tran_id,PaymentInfo (*&pi))
{
    LOG4CXX_DEBUG(logger_, "try to load payment from db");
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%s%s" , "select * from payment where tran_id = \"",tran_id.c_str(),"\";");
    cout<<sql<<endl;
    bool ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_);
        MYSQL_ROW row;
        row = mysql_fetch_row(res);
        bool check = false;
        if(row){
            string product_id = row[3];
            string checkcode = row[6];
            pi = new PaymentInfo(tran_id,product_id,checkcode);
            if(pi == NULL){
                check = false;
            }else{
                check = true;
                while(1){
                    pi->tran_id_ = tran_id;
                    check = safeAtoll(row[1], pi->uid_);
                    if(check == false) break;
                    check = safeAtoll(row[2], pi->pay_time_);
                    if(check == false) break;
                    //pi->product_id_ = row[3];
                    check = safeAtoi(row[4], pi->server_id_);
                    if(check == false) break;
                    check = safeAtoi(row[5], pi->status_);
                    if(check == false) break;
                    //pi->check_info_ = row[6];
                    break;
                }
            }
        }else{
            check = true;
        }
        mysql_free_result(res);
        return check;
    }
    db_error_ = ret;
    return false;
}
NewPaymentInfo * DataHandler::loadNewPaymentInfo(string& tran_id)
{
    LOG4CXX_DEBUG(logger_, "try to load payment from db");
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%s%s" , "select * from payment where tran_id = \"",tran_id.c_str(),"\";");
    bool ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_);
        MYSQL_ROW row;
        row = mysql_fetch_row(res);
        NewPaymentInfo * npi = NULL;
        if(row){
            string product_id = row[4];
            string order_money = row[8];
            string local_tran_id = row[6];
            npi = new NewPaymentInfo(tran_id,product_id,order_money,local_tran_id);
            if(npi != NULL){
                bool check = false;
                while(1){
                    check = safeAtoll(row[1], npi->uid_);
                    if(check == false)break;
                    check = safeAtoi(row[5], npi->server_id_);
                    if(check == false) break;
                    check = safeAtoi(row[7], npi->order_status_);
                    if(check == false) break;
                    check = true;
                    break;
                }
                if(check == false){
                    delete npi;
                    npi = NULL;
                }
            }
        }
        mysql_free_result(res);
        return npi;
    }
    db_error_ = ret;
    return NULL;
}
NewPaymentInfo * DataHandler::loadNewPaymentInfoBak(string& tran_id)
{
    sconfig * sc = game_config.serverconfigs_[server_id_];
    MYSQL mysql_payment_;
    mysql_init(&mysql_payment_);
    if(!mysql_real_connect(&mysql_payment_,sc->pay_db_address.c_str(),sc->pay_db_user.c_str(),\
                           sc->pay_db_passwd.c_str(),sc->pay_db_name.c_str(),sc->pay_db_port,NULL,0)){
        //if(!mysql_real_connect(&mysql_,"106.186.119.143","root","","hero",3306,NULL,0)){
        LOG4CXX_ERROR(logger_, "system;Connect to payment db failed");
        return NULL;
    }
    LOG4CXX_DEBUG(logger_, "try to load payment from db");
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%s%s" , "select * from payment where tran_id = \"",tran_id.c_str(),"\";");
    bool ret = mysql_query(&mysql_payment_,sql);
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_payment_);
        MYSQL_ROW row;
        row = mysql_fetch_row(res);
        NewPaymentInfo * npi = NULL;
        if(row){
            string product_id = row[4];
            string order_money = row[8];
            string local_tran_id = row[6];
            npi = new NewPaymentInfo(tran_id,product_id,order_money,local_tran_id);
            if(npi != NULL){
                bool check = false;
                while(1){
                    check = safeAtoll(row[1], npi->uid_);
                    if(check == false)break;
                    check = safeAtoi(row[5], npi->server_id_);
                    if(check == false) break;
                    check = safeAtoi(row[7], npi->order_status_);
                    if(check == false) break;
                    check = true;
                    break;
                }
                if(check == false){
                    delete npi;
                    npi = NULL;
                }
            }
        }
        mysql_free_result(res);
        mysql_close(&mysql_payment_);
        return npi;
    }
    db_error_ = ret;
    mysql_close(&mysql_payment_);
    return NULL;
}
bool DataHandler::loadAllAct(User *user)
{
    LOG4CXX_DEBUG(logger_,  "try to load act from db for user"<<user->uid_);
    int count = 0;
    char sql[1024] ={0};
    count = snprintf(sql, 1024, "%s%lld%s","select * from act_record where owner_id = ", user->uid_, ";");
    bool ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_);
        int mnf = mysql_num_fields(res);
        MYSQL_ROW row;
        while((row = mysql_fetch_row(res))){
            bool check = loadAct(row, mnf, user);
            if(check == false){
                LOG4CXX_ERROR(logger_,"read act "<<row[0]<<" fail");
                continue;
            }
        }
        mysql_free_result(res);
        LOG4CXX_DEBUG(logger_,"read act for "<<user->uid_<<" succ");
        user->load_staus_ |= LOADED_ACT_RECORD;
        return true;
    }
    db_error_ = ret;
    return false;
}
bool DataHandler::loadAct(MYSQL_ROW &row, int mnf, User *user)
{
    Act * act = new Act();
    if(act == NULL) return false;
    bool check = true;
    string delims = ",";
    while(1){
        check = safeAtoll(row[0], act->global_id_);
        if(check == false) break;
        check = safeAtoi(row[2], act->act_id_);
        if(check == false) break;
        check = safeAtoi(row[3], act->version_);
        if(check == false) break;
        check = longtokenize(row[4], act->record_, delims);
        if(check == false) break;
        if(act->record_.size()<=0){
            check = false;
            break;
        }
        break;
    }
    if(check == false){
        delete act;
        return false;
    }
    user->act_record_.insert(make_pair(act->act_id_, act));
    return true;
}
bool DataHandler::loadAllTeams(User *user)
{
    LOG4CXX_DEBUG(logger_,  "try to load team from db for user"<<user->uid_);
    int count = 0;
    char sql[1024] ={0};
    count = snprintf(sql, 1024, "%s%lld%s","select * from team_info where owner_id = ", user->uid_, ";");
    bool ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        MYSQL_RES * res = mysql_store_result(&mysql_);
        int mnf = mysql_num_fields(res);
        MYSQL_ROW row;
        while((row = mysql_fetch_row(res))){
            bool check = loadTeam(row, mnf, user);
            if(check == false){
                LOG4CXX_ERROR(logger_,"read team "<<row[0]<<" fail");
                continue;
            }
        }
        mysql_free_result(res);
        LOG4CXX_DEBUG(logger_,"read act for "<<user->uid_<<" succ");
        user->load_staus_ |= LOADED_TEAM;
        return true;
    }
    db_error_ = ret;
    return false;
}
bool DataHandler::loadTeam(MYSQL_ROW &row,int mnf,User * user)
{
    Team * team = new Team();
    if(team == NULL) return false;
    bool check = true;
    string delims = ",";
    while(1){
        check = safeAtoll(row[0], team->team_id_);
        if(check == false) break;
        check = safeAtoll(row[2], team->recreate_time_);
        if(check == false) break;
        check = safeAtoll(row[3], team->hero_id_);
        if(check == false)break;
        check = longtokenize(row[4], team->soldier_id_, delims);
        if(check == false) break;
        break;
    }
    if(check == false){
        delete team;
        return false;
    }
    user->teams_.insert(make_pair(team->team_id_, team));
    return true;
}

bool DataHandler::createUser(uid_type uid, vector<string> &param)
{
    db_error_ = 0;
    long long hero_id = 0;
    vector<long long> soldier_id;
    soldier_id.clear();
    long long now = time(NULL);
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql,1024,"%s%lld%s%lld%s%d%s%d%s%d%s","insert into hero_info(owner_id,get_time,mid,exp,hero_level,star,skill) values (",uid,",",now,",",game_config.init_user_hero_.subtype,",0,",game_config.init_user_hero_.param_1,
        ",",game_config.init_user_hero_.param_2,",\"1,1,1\");");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        hero_id = mysql_insert_id(&mysql_);
    }else{
        db_error_ = ret;
        return false;
    }
    for(size_t i= 0;i<game_config.init_user_soldier_.size();i++){
        for(int j = 0;j<1024;j++){
            sql[j] = 0;
        }
        count = snprintf(sql, 1024, "%s%lld%s%lld%s%d%s%d%s%d%s","insert into soldier_info(owner_id,get_time,mid,exp,soldier_level,star) values (",uid,",",now,",",game_config.init_user_soldier_[i].subtype,",0,",game_config.init_user_soldier_[i].param_1,
            ",",game_config.init_user_soldier_[i].param_2,");");
        ret = mysql_query(&mysql_, sql);
        if(ret == 0){
            //soldier_id[i] = mysql_insert_id(&mysql_);
            soldier_id.push_back(mysql_insert_id(&mysql_));
        }else{
            db_error_ = ret;
            return false;
        }
    }
    for(int i = 0;i<1024;i++){
        sql[i] = 0;
    }
    count = snprintf(sql, 1024, "%s%lld%s%s%s%lld%s%d%s%s%s%d%s%d%s%d%s%lld%s%lld%s%lld%s%lld%s%lld%s%lld%s",\
                     "insert into user_info(user_id,nickname,create_time,last_login_time,eqid,user_level,user_exp,energy,gold,\
                     diamond,last_recover_time,hero_id,soldier_id,normal_chapter,normal_stage,elite_chapter,elite_stage,system_mail_record) values ("\
                     ,uid,",\"",param[3].c_str(),"\",",now,",",0,",\"",param[2].c_str(),\
                     "\",1,0,",game_config.energy_level_limit_[1],\
                     ",",game_config.init_user_gold_,\
                     ",",game_config.init_user_diamond_,",",now,",",hero_id,\
                     ",\"",soldier_id[0],",",soldier_id[1],",",soldier_id[2],",",soldier_id[3],\
                     "\",0,0,0,0,\"0,0,0,0\");");
    ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
    return true;
}
bool DataHandler::saveUser(User * user)
{
    char sql[2048] = {0};
    /*
    count = snprintf(sql,1024,"%s%lld%s%d%s%d%s%d%s%d%s%d%s%lld%s%lld%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%lld%s%lld%s",\
                     "update user_info set last_login_time =  ",user->last_login_time_,\
                     " , user_level = ",user->user_level_,\
                     " , user_exp = ",user->user_exp_,\
                     " , energy = " , user->energy_,\
                     " , gold = ", user->gold_,\
                     " , diamond = ", user->diamond_,
                     " , last_recover_time = ",user->last_recover_time_,\
                     " , hero_id = " ,user->hero_id_,\
                     " , soldier_id = \"",vectorLongToString(user->soldier_id_).c_str(),
                     "\" , normal_chapter = " , user->normal_chapter_id_,\
                     " , normal_stage = " , user->normal_stage_id_,\
                     " , elite_chapter = ",user->elite_chapter_id_,\
                     " , elite_stage = ",user->elite_stage_id_,\
                     " , spark_num = ",user->spark_num_,\
                     " , spark_daily_num = ",user->spark_daily_num_,\
                     " , system_mail_record = \"",vectorIntToString(user->system_mail_record_).c_str(),\
                     "\" , login_sum = ",user->login_sum_,\
                     " , buy_energy_daily = ",user->buy_energy_daily_,\
                     " , expand_hero = ",user->expand_hero_,\
                     " , expand_soldier = ",user->expand_soldier_,\
                     " , expand_friend = ",user->expand_friend_,\
                     " , guide = ",user->guide_record_,\
                     " , pay_count = ",user->pay_count_,\
                     " , team_id = " , user->team_id_,\
                     " , honor = ", user->honor_,\
                     " where user_id = ",user->uid_,";");
    */
    int count = snprintf(sql, 2048, "update user_info set "\
            "last_login_time = '%lld', user_level = '%d', user_exp='%d', energy='%d', gold='%d', "\
            "diamond='%d', last_recover_time='%lld', hero_id='%lld', soldier_id='%s', normal_chapter='%d', "\
            "normal_stage='%d', elite_chapter='%d', elite_stage='%d', spark_num='%d', spark_daily_num='%d', "\
            "system_mail_record='%s', login_sum='%d', buy_energy_daily='%d', expand_hero='%d', expand_soldier='%d', "\
            "expand_friend='%d', guide='%d', pay_count='%d', team_id='%lld', honor='%d' "\
            "where user_id='%lld';",
            user->last_login_time_, user->user_level_, user->user_exp_, user->energy_, user->gold_,
            user->diamond_, user->last_recover_time_, user->hero_id_, vectorLongToString(user->soldier_id_).c_str(), user->normal_chapter_id_,
            user->normal_stage_id_, user->elite_chapter_id_, user->elite_stage_id_, user->spark_num_, user->spark_daily_num_,
            vectorIntToString(user->system_mail_record_).c_str(), user->login_sum_, user->buy_energy_daily_, user->expand_hero_, user->expand_soldier_,
            user->expand_friend_, user->guide_record_, user->pay_count_, user->team_id_, user->honor_,
            user->uid_
            );
    if (count == 2048) {
        //TODO
    }
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        user->dirty_ = false;
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::saveUserSInfo(User *user)
{
    //TODO eqid,nickname escape 
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql,1024,"%s%s%s%s%s%lld%s","update user_info set eqid = \"",user->eqid_.c_str(),"\" , nickname = \"",user->nick_name_.c_str(),"\" where user_id = ",user->uid_,";");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        user->dirty_ = false;
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}

bool DataHandler::saveUserTrialInfo(User *user) {
    char sql[1024] = {0};
    snprintf(sql, 1024, "update user_info set `trial_stage_id`='%d', `trial_max_stage_id`='%d', `trial_daily_reset`='%d' , `trial_instant_start`='%lld' where `user_id`='%lld';", 
           user->trial_stage_id_, user->trial_max_stage_id_, user->trial_daily_reset_, user->trial_instant_start_time_, user->uid_);
    int ret = mysql_query(&mysql_, sql);
    if (ret == 0) {
        user->dirty_ = false;
        return true;
    }
    else {
        db_error_ = ret;
        return false;
    }
}

bool DataHandler::saveHero(Hero * hero)
{
    db_error_ = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%d%s%d%s%d%s%d%s%s%s%d%s%lld%s","update hero_info set mid = ",hero->mid_,\
                     " , exp = ", hero->exp_,\
                     " , hero_level = ",hero->level_,\
                     " , star = ",hero->star_,\
                     " , skill = \"",vectorIntToString(hero->skill_).c_str(),\
                     "\" , locked = " ,hero->locked_,\
                     " where hero_id = ",hero->hero_id_,";");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::saveSoldier(Soldier *soldier)
{
    db_error_ = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%d%s%d%s%d%s%d%s%d%s%lld%s","update soldier_info set mid = ",soldier->mid_,\
                     " , exp = ", soldier->exp_,\
                     " , soldier_level = ",soldier->level_,\
                     " , star = ",soldier->star_,\
                     " , locked = " ,soldier->locked_,\
                     " where hero_id = ",soldier->soldier_id_,";");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::saveItem(Item *item)
{
    db_error_ = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%d%s%d%s%d%s%lld%s%lld%s","update item_info set item_type = ",item->type_,\
                     " , mid = ",item->mid_,\
                     " , amount = ",item->amount_,\
                     " , get_time = ",item->get_time_,\
                     " where item_id = ",item->item_id_,";");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::saveFriendInfo(FriendInfo * friend_info)
{
    db_error_ = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s%lld%s%d%s%lld%s","update friend_info set last_send_time = ",friend_info->last_send_time_,\
                     " , last_receive_time = ",friend_info->last_receive_time_,\
                     " , accepted = ",friend_info->accepted_,\
                     " where record_id = ",friend_info->record_id_,";");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::saveMail(Mail *mail)
{
    db_error_ = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%d%s%d%s%lld%s","update mail_info set have_read = ",mail->haveread_,\
                     " , acceptable = ",mail->acceptable_,\
                     " where mail_id = ",mail->mail_id_,";");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::saveMission(Mission *mission)
{
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%d%s%d%s%lld%s","update mission_info set mission_record = ",\
                     mission->mission_record_,", complete = ",\
                     mission->complete_," where mission_id = ",\
                     mission->mission_id_,";");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::saveUbi(UserBasicInfo *ubi)
{
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql,1024,"%s%s%s%s%s%s%s%lld%s","update ubd set eqid = \"",ubi->eqid_.c_str(),\
                     "\",username =\"",ubi->username_.c_str(),\
                     "\" , password = md5(\"", ubi->password_.c_str(),"\") where user_id = ",ubi->uid_,";");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::savePaymentStatus(string & tran_id, int status)
{
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%d%s%s%s","update payment set order_status = ",status," where tran_id = \"",tran_id.c_str(),"\";");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::saveStageRecord(StageRecord *sr)
{
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s%s%s%lld%s","update stage_record set update_time = ",\
                     sr->update_time_ ,", record = \"",\
                     vectorIntToString(sr->record_).c_str(),"\" where record_id = ",\
                     sr->record_id_, ";");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::saveNewPaymentStatus(string & tran_id, int status)
{
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%d%s%s%s","update payment set order_status = ",status," where tran_id = \"",tran_id.c_str(),"\";");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::saveNewPaymentStatusBak(string & tran_id, int status)
{
    sconfig * sc = game_config.serverconfigs_[server_id_];
    MYSQL mysql_payment_;
    mysql_init(&mysql_payment_);
    if(!mysql_real_connect(&mysql_payment_,sc->pay_db_address.c_str(),sc->pay_db_user.c_str(),\
                           sc->pay_db_passwd.c_str(),sc->pay_db_name.c_str(),sc->pay_db_port,NULL,0)){
        //if(!mysql_real_connect(&mysql_,"106.186.119.143","root","","hero",3306,NULL,0)){
        LOG4CXX_ERROR(logger_, "system;Connect to payment db failed");
        return false;
    }
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%d%s%s%s","update payment set order_status = ",status," where tran_id = \"",tran_id.c_str(),"\";");
    int ret = mysql_query(&mysql_payment_,sql);
    if(ret == 0){
        mysql_close(&mysql_payment_);
        return true;
    }else{
        db_error_ = ret;
        mysql_close(&mysql_payment_);
        return false;
    }
}
bool DataHandler::saveAct(Act * act)
{
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%s%s%d%s%lld%s","update act_record set record = \"",\
                     vectorLongToString(act->record_).c_str(),"\" ,act_version = " ,\
                     act->version_," where gact_id = ",\
                     act->global_id_,";");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::saveTeam(Team *team)
{
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s%lld%s%s%s%lld%s","update team_info set recreate_time = ",\
                     team->recreate_time_,",hero_id = ",\
                     team->hero_id_,",soldier_id = \"",\
                     vectorLongToString(team->soldier_id_).c_str(),"\" where team_id = ",\
                     team->team_id_,";");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
void DataHandler::updateFresh(User *user)
{
    if(user == NULL) return;
    user->last_active_time_ = time(NULL);
    int findex = (user->user_level_ - 1)/game_config.fresh_interval_;
    if(findex >= game_config.fresh_step_count_) return;
    
    for(int i = 0;i<game_config.fresh_level_size_;i++){
        if(fresh_[findex][i] == user)return;
    }
    fresh_[findex][fresh_pointer_[findex]] = user;
    fresh_pointer_[findex] = (fresh_pointer_[findex] + 1)%game_config.fresh_level_size_;
    return;
}
bool DataHandler::addNewHero(User *user, int mid, int level, int star, long long now)
{
    db_error_ = 0;
    long long hero_id = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql,1024,"%s%lld%s%lld%s%d%s%d%s%d%s%s%s","insert into hero_info(owner_id,get_time,mid,exp,hero_level,star,skill,locked) values (",user->uid_,",",now,",",mid,",0,",level,",",star,",\"1,1,1\"",",0",");");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        hero_id = mysql_insert_id(&mysql_);
    }else{
        db_error_ = ret;
        return false;
    }
    Hero * hero = new Hero();
    if(hero == NULL) return false;
    hero->hero_id_ = hero_id;
    hero->owner_id_ = user->uid_;
    hero->get_time_ = now;
    hero->mid_ = mid;
    hero->exp_ = 0;
    hero->level_ = level;
    hero->star_ = star;
    for(int i = 0;i<3;i++){
        hero->skill_.push_back(1);
    }
    hero->locked_ = 0;
    user->heroes_.insert(make_pair(hero_id,hero));
    return true;
}
bool DataHandler::addNewSoldier(User *user, int mid, int level, int star, long long now)
{
    db_error_ = 0;
    long long soldier_id = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s%lld%s%d%s%d%s%d%s%s%s","insert into soldier_info(owner_id,get_time,mid,exp,soldier_level,star,locked) values (",user->uid_,",",now,",",mid,",0,",level,",",star,",","0",");");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        soldier_id = mysql_insert_id(&mysql_);
    }else{
        db_error_ = ret;
        return false;
    }
    Soldier * soldier = new Soldier();
    if(soldier == NULL) return false;
    soldier->soldier_id_ = soldier_id;
    soldier->owner_id_ = user->uid_;
    soldier->get_time_ = now;
    soldier->mid_ = mid;
    soldier->exp_ = 0;
    soldier->level_ = level;
    soldier->star_ = star;
    soldier->locked_ = 0;
    user->soldiers_.insert(make_pair(soldier_id, soldier));
    return true;
}
bool DataHandler::addItem(User *user, int type, int mid, int amount, long long now)
{
    db_error_ = 0;
    int count = 0;
    char sql[1024] = {0};
    map<long long,Item * >::iterator it ;
    for(it = user->items_.begin();it!=user->items_.end();it++){
        if(it->second->type_ == type && it->second->mid_ == mid){
            break;
        }
    }
    if(it!= user->items_.end()){
        if(it->second->amount_ == 0){
            it->second->get_time_ = now;
        }
        it->second->amount_ += amount;
        it->second->amount_ = min(it->second->amount_,MAX_ITEM_AMOUNT);
        count = snprintf(sql, 1024, "%s%d%s%lld%s%lld%s","update item_info set amount = ",it->second->amount_,\
                         " , get_time = ",it->second->get_time_,\
                         "  where item_id = ",it->second->item_id_,";");
        int ret = mysql_query(&mysql_, sql);
        if(ret == 0){
            return true;
        }else{
            db_error_ = ret;
            return false;
        }
    }else{
        count = snprintf(sql, 1024, "%s%lld%s%d%s%d%s%d%s%lld%s","insert into item_info(owner_id,item_type,mid,amount,get_time) values (",\
                         user->uid_,",",type,",",mid,",",amount,",",now,");");
        int ret = mysql_query(&mysql_, sql);
        if(ret == 0){
            long long item_id = mysql_insert_id(&mysql_);
            Item * item = new Item();
            if(item == NULL) return false;
            item->item_id_ = item_id;
            item->owner_id_ = user->uid_;
            item->type_ = type;
            item->mid_ = mid;
            item->amount_ = amount;
            item->get_time_ = now;
            user->items_.insert(make_pair(item_id, item));
            return true;
        }else{
            db_error_ = ret;
            return false;
        }
    }
}
bool DataHandler::addNewMail(User * tuser,uid_type fuid,string &title,string &text,int acceptable, int amount, string &contents)
{
    long long mail_id = 0;
    long long now = time(NULL);
    int count = 0;
    char sql[2048] = {0};
    count = snprintf(sql,2048,"%s%lld%s%lld%s%lld%s%s%s%d%s%d%s%s%s%d%s%s%s","insert into mail_info(owner_id,sender_id,send_time,mail_text,have_read,acceptable,title,`count`,`contents`) values (",
                     tuser->uid_,
                     ",",fuid,
                     ",",now,
                     ",'",text.c_str(),
                     "',",MAIL_NOT_READ,
                     ",",acceptable,
                     ",'",title.c_str(),\
                     "',", amount,\
                     ",'", contents.c_str(),\
                     "');");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        mail_id = mysql_insert_id(&mysql_);
    }else{
        db_error_ = ret;
        return false;
    }
    Mail * mail = new Mail();
    if(mail == NULL) return false;
    mail->mail_id_ = mail_id;
    mail->owner_id_ = tuser->uid_;
    mail->sender_id_ = fuid;
    mail->text_ = text;
    mail->title_ = title;
    mail->send_time_ = now;
    mail->haveread_ = MAIL_NOT_READ;
    mail->acceptable_ = acceptable;
    mail->count = amount;
    mail->contents = contents;
    tuser->mails_.insert(make_pair(mail_id, mail));
    return true;
}
bool DataHandler::addNewMission(User *user, int type, int subtype, int record, int complete)
{
    db_error_ = 0;
    long long mission_id = 0;
    long long now = time(NULL);
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql,1024,"%s%lld%s%lld%s%d%s%d%s%d%s%d%s",\
                     "insert into mission_info(owner_id,add_time,mission_type,mission_subtype,mission_record,complete) values (",\
                     user->uid_,",",\
                     now,",",\
                     type,",",\
                     subtype,",",\
                     record,",",\
                     complete,");");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        mission_id = mysql_insert_id(&mysql_);
    }else{
        db_error_ = ret;
        return false;
    }
    Mission * mission = new Mission();
    if(mission == NULL) return false;
    mission->mission_id_ = mission_id;
    mission->owner_id_ = user->uid_;
    mission->add_time_ = now;
    mission->mission_type_ = type;
    mission->mission_subtype_ = subtype;
    mission->mission_record_ = record;
    mission->complete_ = complete;
    user->missions_.insert(make_pair(mission_id, mission));
    return true;
}
bool DataHandler::addNewPayment(PaymentInfo *pi)
{
    db_error_ = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%s%s%lld%s%lld%s%s%s%d%s%d%s%s%s",\
                     "insert into payment(tran_id,owner_id,order_time,product_id,server_id,order_status,check_info) values (\"",\
                     pi->tran_id_.c_str(),"\",",\
                     pi->uid_,",",\
                     pi->pay_time_,",\"",\
                     pi->product_id_.c_str(),"\",",\
                     pi->server_id_,",",\
                     pi->status_,",\"",\
                     pi->check_info_.c_str(),"\");");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::addNewStageRecord(User * user,int chapter_id,int stage_id)
{
    db_error_ = 0;
    long long record_id = 0;
    long long now = time(NULL);
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s%lld%s%d%s" , "insert into stage_record(owner_id,update_time,record) values(",\
                     user->uid_,",",\
                     now,",\"",\
                     chapter_id,"\");"
                     );
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        record_id = mysql_insert_id(&mysql_);
    }else{
        db_error_ = ret;
        return false;
    }
    StageRecord * sr = new StageRecord();
    if(sr == NULL)return false;
    sr->record_id_ = record_id;
    sr->update_time_ = now;
    sr->owner_id_ = user->uid_;
    sr->record_.clear();
    sr->record_.push_back(chapter_id);
    user->stage_record_.insert(make_pair(chapter_id,sr));
    return true;
}
bool DataHandler::addNewActRecord(User * user,int act_id)
{
    db_error_ = 0;
    long long record_id = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s%d%s%d%s" , "insert into act_record(owner_id,act_id,act_version,record) values(",\
                     user->uid_,",",\
                     act_id,",",\
                     0,",\"0\");");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        record_id = mysql_insert_id(&mysql_);
    }else{
        db_error_ = ret;
        return false;
    }
    Act * act = new Act();
    if(act == NULL)return false;
    act->global_id_ = record_id;
    act->act_id_ =act_id;
    act->version_ = 0;
    act->record_.clear();
    user->act_record_.insert(make_pair(act_id,act));
    return true;
}
bool DataHandler::addNewTeam(User * user,long long now)
{
    db_error_ = 0;
    long long record_id = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s%lld%s","insert into team_info(owner_id,recreate_time,hero_id,soldier_id) values(",\
                     user->uid_,",",\
                     now,",0,\"0,0,0,0\");"
                     );
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        record_id = mysql_insert_id(&mysql_);
    }else{
        db_error_ = ret;
        return false;
    }
    
    Team * team = new Team();
    if(team == NULL) return false;
    team->team_id_  = record_id;
    team->recreate_time_ = now;
    team->owner_id_ = user->uid_;
    team->hero_id_ = 0LL;
    team->soldier_id_.clear();
    for(int i = 0;i<SOLDIER_INGROUND;i++){
        team->soldier_id_.push_back(0LL);
    }
    user->teams_.insert(make_pair(team->team_id_, team));
    return true;
}
bool DataHandler::deleteFriendRequest(long long record_id)
{
    db_error_ = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s" , "delete from friend_request_info where record_id = ",record_id,";");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::deleteHero(long long hero_id)
{
    db_error_ = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s" , "delete from hero_info where hero_id = ",hero_id,";");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::deleteSoldier(long long soldier_id)
{
    db_error_ = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s" , "delete from soldier_info where hero_id = ",soldier_id,";");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::deleteFriend(long long record_id)
{
    db_error_ = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s" , "delete from friend_info where record_id = ",record_id,";");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::deleteMail(long long mail_id)
{
    db_error_ = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s" , "delete from mail_info where mail_id = ",mail_id,";");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::deleteMission(long long mission_id)
{
    db_error_ = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s" , "delete from mission_info where mission_id = ",mission_id,";");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::addFriendRequest(User * user,uid_type tuid, uid_type fuid)
{
    long long now = time(NULL);
    db_error_ = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s%lld%s%lld%s" , "insert into friend_request_info(fuid,send_time,tuid) values(",fuid,\
                     ",",now,",",tuid,");");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        FriendRequestInfo * fr = new FriendRequestInfo();
        if(fr == NULL) return false;
        fr->record_id_ = mysql_insert_id(&mysql_);
        fr->fuid_ = fuid;
        fr->send_time_ = now;
        user->friend_requests_.insert(make_pair(fr->fuid_, fr));
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
bool DataHandler::addFriend(User *user, User *tuser)
{
    db_error_ = 0;
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql, 1024, "%s%lld%s%lld%s","insert into friend_info(owner_id,fid,last_send_time,last_receive_time,accepted) values(",\
                     user->uid_,",",tuser->uid_,",0,0,0);");
    int ret = mysql_query(&mysql_,sql);
    if(ret == 0){
        FriendInfo * fr = new FriendInfo();
        if(fr == NULL) return false;
        fr->record_id_ = mysql_insert_id(&mysql_);
        fr->fuid_ = tuser->uid_;
        fr->last_receive_time_ = fr->last_send_time_ = fr->accepted_ = 0;
        user->friends_.insert(make_pair(fr->fuid_, fr));
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
}
void DataHandler::addNews(string &name, int action, int param_1, int param_2, int param_3)
{
    News anews;
    anews.news_id = global_news_index_;
    global_news_index_ ++;
    anews.name = name;
    anews.action = action;
    anews.param_1 = param_1;
    anews.param_2 = param_2;
    anews.param_3 = param_3;
    news_[news_pointer_] = anews;
    news_pointer_ = (news_pointer_ + 1) % int(news_.size());
}
bool DataHandler::createFakeUser()
{
    db_error_ = 0;
    long long uid = FAKE_USER_ID;
    string nickname = "贾仁";
    string eqid = "abcdefgh";
    long long hero_id = 0;
    vector<long long> soldier_id;
    soldier_id.clear();
    long long now = time(NULL);
    int count = 0;
    char sql[1024] = {0};
    count = snprintf(sql,1024,"%s%lld%s%lld%s%d%s%d%s%d%s%s%s","insert into hero_info(owner_id,get_time,mid,exp,hero_level,star,skill) values (",uid,",",now,",",game_config.fake_user_hero_.subtype,",0,",game_config.fake_user_hero_.param_1,
                     ",",game_config.fake_user_hero_.param_2,",\"",vectorIntToString(game_config.fake_user_skill_).c_str(),"\");");
    int ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        hero_id = mysql_insert_id(&mysql_);
    }else{
        db_error_ = ret;
        return false;
    }
    for(size_t i= 0;i<game_config.fake_user_soldier_.size();i++){
        for(int j = 0;j<1024;j++){
            sql[j] = 0;
        }
        count = snprintf(sql, 1024, "%s%lld%s%lld%s%d%s%d%s%d%s","insert into soldier_info(owner_id,get_time,mid,exp,soldier_level,star) values (",uid,",",now,",",game_config.fake_user_soldier_[i].subtype,",0,",game_config.fake_user_soldier_[i].param_1,
                         ",",game_config.fake_user_soldier_[i].param_2,");");
        ret = mysql_query(&mysql_, sql);
        if(ret == 0){
            //soldier_id[i] = mysql_insert_id(&mysql_);
            soldier_id.push_back(mysql_insert_id(&mysql_));
        }else{
            db_error_ = ret;
            return false;
        }
    }
    for(int i = 0;i<1024;i++){
        sql[i] = 0;
    }
    count = snprintf(sql, 1024, "%s%lld%s%s%s%lld%s%d%s%s%s%d%s%d%s%d%s%lld%s%lld%s%lld%s%lld%s%lld%s%lld%s",\
                     "insert into user_info(user_id,nickname,create_time,last_login_time,eqid,user_level,user_exp,energy,gold,\
                     diamond,last_recover_time,hero_id,soldier_id,normal_chapter,normal_stage,elite_chapter,elite_stage,system_mail_record) values ("\
                     ,uid,",\"",game_config.fake_user_nick_.c_str(),"\",",now,",",0,",\"",game_config.fake_user_eqid_.c_str(),\
                     "\",1,0,",game_config.energy_level_limit_[1],\
                     ",",game_config.init_user_gold_,\
                     ",",game_config.init_user_diamond_,",",now,",",hero_id,\
                     ",\"",soldier_id[0],",",soldier_id[1],",",soldier_id[2],",",soldier_id[3],\
                     "\",0,0,0,0,\"0,0,0,0\");");
    ret = mysql_query(&mysql_, sql);
    if(ret == 0){
        return true;
    }else{
        db_error_ = ret;
        return false;
    }
    return true;
}


void DataHandler::updateTrialRank(long long uid, int data, long long now) {
    int in_rank = 0;
    for (size_t i = 0; i < trial_ranks_.size(); i++) {
        if (trial_ranks_[i].uid == uid) {
            in_rank = 1;
            trial_ranks_[i].data = data;
            trial_ranks_[i].time = now;
            trial_rank_need_save_ = true;
        }
    }
    if (in_rank == 0) {
        if ((int)trial_ranks_.size() >= game_config.trial_rank_num_) {
            int last = (int) trial_ranks_.size()-1;
            if (data > trial_ranks_[last].data) {
                trial_ranks_[last].data = data;
                trial_ranks_[last].time = now;
                trial_ranks_[last].uid = uid;
                trial_rank_need_save_ = true;
            }
        }
        else {
            trial_ranks_.push_back(RankItem(uid, now, data));
            trial_rank_need_save_ = true;
        }
    }

    if (trial_rank_need_save_) {
        sort(trial_ranks_.begin(), trial_ranks_.end(), greater<RankItem>());
    }

    //save file
    saveTrialRank();
}

void DataHandler::saveTrialRank() {

    long long now = time(NULL);

    if (trial_rank_need_save_ == false && now - trial_rank_last_save_time_ < 30) {//TODO config
        return ;
    }
    trial_rank_need_save_ = false;
    trial_rank_last_save_time_ = now;
    
    //TODO 配置排行版存储路径
    const char *fileprefix="trialrank_";
    char file[512];
    snprintf(file, 512, "%s%d.dat", fileprefix, server_id_);

    int succ = 0;
    FILE *fp = fopen(file, "wb");
    if (fp) {
        int size = (int)trial_ranks_.size();
        if (saveint(size, fp)) {
            for (size_t i = 0; i < trial_ranks_.size(); i++) {
                savelonglong(trial_ranks_[i].uid, fp);
                savelonglong(trial_ranks_[i].time, fp);
                saveint(trial_ranks_[i].data, fp);
            }
        }
        else {
            succ = 1;
        }

        fclose(fp);
    }
    else {
        succ = 1000;
    }
    LOG4CXX_WARN(logger_, "saveTrialRank to "<<file<<", succ:"<<succ);        
}

void DataHandler::loadTrialRank() {
    const char *fileprefix="trialrank_";
    char file[512];
    snprintf(file, 512, "%s%d.dat", fileprefix, server_id_);

    int succ = 0;
    FILE *fp = fopen(file, "rb");
    if (fp) {
        int size = 0;
        if (loadint(size, fp)) {
            for (int i = 0; i < size; i++) {
                RankItem rkitem;
                loadlonglong(rkitem.uid, fp);
                loadlonglong(rkitem.time, fp);
                loadint(rkitem.data, fp);
                trial_ranks_.push_back(rkitem);
            }
        }
        else {
            succ = 1;
        }

        sort(trial_ranks_.begin(), trial_ranks_.end(), greater<RankItem>());

        fclose(fp);
        LOG4CXX_WARN(logger_, "loadTrialRank from "<<file<<", succ:"<<succ);
    }
    else {
        LOG4CXX_WARN(logger_, "loadTrialRank from "<<file<<", failed[if first load ]");
    }
}

bool DataHandler::loadPvpAllBuildings(User *user) {
    LOG4CXX_DEBUG(logger_,  "try to load builds from db for user"<<user->uid_);
    bool ret = false;
    char sql[1024];
    sprintf(sql, "select id,owner_uid,type,level,position,gem_type from building_info where owner_id = '%lld';", user->uid_);
    int err = 0; 
    if((err = mysql_query(&mysql_, sql)) == 0){
        //先清空
        user->build_infs_.clear();
        MYSQL_RES * res = mysql_store_result(&mysql_);
        //mysql_num_fields(res);
        MYSQL_ROW row;
        while((row = mysql_fetch_row(res))){
            BuildInf binf;
            if (safeAtoll(row[0], binf.id_)
                    && safeAtoll(row[1], binf.owner_uid_)
                    && safeAtoi(row[2], binf.mid_)
                    && safeAtoi(row[3], binf.level_)
                    && safeAtoi(row[4], binf.position_)
                    && safeAtoi(row[5], binf.gem_type_)) {
                user->build_infs_.insert(make_pair(binf.id_, binf));
            }
            else {
                LOG4CXX_ERROR(logger_, "read building "<<row[0]<<" failed");
            }
        }
        mysql_free_result(res);
        ret = true; 
    }
    LOG4CXX_DEBUG(logger_,"read buildings for "<<user->uid_<<" succ:"<<err);
    db_error_ = err;
    return ret; 
}

bool DataHandler::loadUserPvpInfo(User *user) {
    LOG4CXX_DEBUG(logger_, "try to load user pvp_info from db for user:"<<user->uid_);
    bool ret = false;
    char sql[1024];
    sprintf(sql, "select `team_id`,`wood`,`stone`,`attack_tuid`,`attacked_tuid` from `user_pvp_info` where `uid`='%lld';", user->uid_);
    int err = 0;
    if ((err = mysql_query(&mysql_, sql)) == 0) {
        user->pvp_team_id_ = 0;
        MYSQL_RES *res = mysql_store_result(&mysql_);
        MYSQL_ROW row;
        int need_add = 0;
        if((row = mysql_fetch_row(res))) {
            if (!safeAtoll(row[0], user->pvp_team_id_)
                    || !safeAtoi(row[1], user->wood_)
                    || !safeAtoi(row[2], user->stone_)
                    || !safeAtoll(row[3], user->pvp_attack_tuid_)
                    || !safeAtoll(row[4], user->pvp_attacked_tuid_)) {
                LOG4CXX_ERROR(logger_, "read user_pvp_info "<<row[0]<<" failed");
                err = -1;
                ret = false;
            }
            else {
                ret = true;
            }
        }
        else {
            need_add = 1;
        }
        mysql_free_result(res);

        if (need_add) {
            addUserPvpInfo(user);
        }
    }
    else {
        addUserPvpInfo(user);
    }
    LOG4CXX_DEBUG(logger_, "read user_pvp_info for "<<user->uid_<<" succ:"<<err);
    db_error_ = err;
    return ret;
}

void DataHandler::addUserPvpInfo(User *user) {
    char sql[1024];
    sprintf(sql, "insert into user_pvp_info name(uid, team_id, wood, stone, attack_tuid, attacked_tuid) "\
            " values('%lld', '%lld', '%d', '%d', '%lld', '%lld');",
            user->uid_, user->pvp_team_id_, user->wood_, user->stone_, user->pvp_attack_tuid_, user->pvp_attacked_tuid_);
    if (execSql(sql)) {
        LOG4CXX_ERROR(logger_, "pvp_rank_info: add, uid:"<<user->uid_<<" succ:"<<db_error_);
    }
    LOG4CXX_DEBUG(logger_, "pvp_rank_info: add, uid:"<<user->uid_);
}

void DataHandler::saveUserpvpInfo(User *user) {
    char sql[1024];
    sprintf(sql, "update `user_pvp_info` set `team_id`='%lld', `wood`='%d', `stone`='%d', `attack_tuid`='%lld', ``attacked_tuid`='%lld' where `uid`='%lld';",
            user->pvp_team_id_, user->wood_, user->stone_, user->pvp_attack_tuid_, user->pvp_attacked_tuid_, user->uid_);
    int ret = execSql(sql);
    if (ret != 0) {
        LOG4CXX_ERROR(logger_, "save pvp_rank_info uid:"<<user->uid_<<" succ:"<<db_error_);
    }
    LOG4CXX_DEBUG(logger_, "save pvp_rank_info, uid:"<<user->uid_);

}

bool DataHandler::loadAllGears(User *user) {
    int ret = false;
    char sql[1024];
    sprintf(sql, "select `id`,`uid`,`mid`,`level`,`hero_id` from `gear_info` where `uid`='%lld';", user->uid_);
    user->gear_infs_.clear();
    MYSQL_RES* res = getStoreResult(sql);
    if (res) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            GearInf ginf;
            if (safeAtoll(row[0], ginf.id_)
                    && safeAtoll(row[1], ginf.uid_)
                    && safeAtoi(row[2], ginf.mid_)
                    && safeAtoi(row[3], ginf.level_)
                    && safeAtoll(row[4], ginf.hero_id_)) {
                user->gear_infs_.insert(make_pair(ginf.id_, ginf));
            }
            else {
                LOG4CXX_ERROR(logger_, "loadgear error, uid:"<<user->uid_<<" gid:"<<row[0]);
            }
        }

        mysql_free_result(res);
    }
    else {
        LOG4CXX_ERROR(logger_, "loadgear res failed, uid:"<<user->uid_);
    }
    LOG4CXX_DEBUG(logger_, "loadgear "<<(ret?"succeed":"failed")<<" uid:"<<user->uid_<<" succ:"<<db_error_);
    return ret;
}

bool DataHandler::addGear(User *user, int mid) {
    bool ret = false;
    char sql[1024];
    sprintf(sql, "insert into gear_info(`uid`, `mid`, `level`, `hero_id`) values('%lld', '%d', 0, 0);", user->uid_, mid);
    long long id = execSqlReturnId(sql);
    if (id > 0) {
        user->gear_infs_.insert(make_pair(id, GearInf(id, user->uid_, 0, mid, 0)));
        ret = true;
    }
    else {
        LOG4CXX_ERROR(logger_, "addgear, uid:"<<user->uid_<<" mid:"<<mid<<" failed");
    }
    LOG4CXX_DEBUG(logger_, "addgear, uid:"<<user->uid_<<" mid:"<<mid);
    return ret;
}

void DataHandler::saveGear(User *user, GearInf &ginf) {
    char sql[1024];
    sprintf(sql, "update gear_info set `level`='%d', `hero_id`='%lld' where `id`='%lld';", ginf.level_, ginf.hero_id_, ginf.id_);
    int ret = execSql(sql);
    if (ret != 0) {
        LOG4CXX_ERROR(logger_, "savegear error, uid:"<<user->uid_<<" succ:"<<db_error_);
    }
    LOG4CXX_DEBUG(logger_, "savegear, uid:"<<user->uid_<<" succ:"<<db_error_);
}

bool DataHandler::loadPvpRankUids() {
    LOG4CXX_DEBUG(logger_, "try to load pvp rank uids from db");
    bool ret = false;
    char sql[256];
    sprintf(sql, "select uid,rank from pvp_rank_info order by rank ASC;");
    int err = 0;
    if ((err = mysql_query(&mysql_, sql)) == 0) {
        pvp_rank_uids_.clear();
        MYSQL_RES *res = mysql_store_result(&mysql_);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            long long uid;
            int rank;
            if (safeAtoll(row[0], uid)
                    && safeAtoi(row[1], rank)) {
                pvp_rank_uids_.push_back(uid);
                if (rank != (int)pvp_rank_uids_.size()) {
                    LOG4CXX_ERROR(logger_, "read pvp_rank_info uid:"<<uid<<" rank:"<<rank<<" error");
                }
            }
        }
        mysql_free_result(res);
        ret = true;
    }
    LOG4CXX_DEBUG(logger_, "read pvp_rank_info succ:"<<err);
    db_error_ = err;
    return ret;
}

bool DataHandler::savePvpRank(long long uid, int rank) {
    char sql[256];
    sprintf(sql, "insert into pvp_rank_info(`uid`, `rank`) values('%lld', '%d') on duplicate key update `rank`='%d';", uid, rank, rank);
    int ret = execSql(sql);
    LOG4CXX_DEBUG(logger_, "save pvp_rank_info succ:"<<db_error_);
    return ret == 0? true:false;
}

long long DataHandler::addBuilding(User * user, int mid,int position) {
    long long bid = -1;
    db_error_ = 0;
    char sql[1024];
    //id,owner_uid,type,level,position,gem_type
    printf(sql, "insert into building_info(`owner_uid`,`type`,`level`,`position`,`gem_type`) values "\
            "('%lld', '%d', '1', '%d', '0');",
            user->uid_, mid, position);
    int ret = mysql_query(&mysql_, sql);
    if (ret == 0) {
        bid = mysql_insert_id(&mysql_);
        user->build_infs_.insert(make_pair(bid, BuildInf(bid, user->uid_,1,mid,position,0)));
    }
    else {
        db_error_ = ret;
    }
    return bid;
}

void DataHandler::saveBuilding(User *user, long long id, int position) {
    LOG4CXX_DEBUG(logger_, "try to save building, uid:"<<user->uid_);
    char sql[1024];
    sprintf(sql, "update building_info set `position`='%d' where `id`='%lld';", position, user->uid_);
    if (execSql(sql)) {
        LOG4CXX_ERROR(logger_, "save building error, uid:"<<user->uid_<<" succ:"<<db_error_);
    }
}

void DataHandler::saveBuilding(User *user, BuildInf &binf) {
    LOG4CXX_DEBUG(logger_, "try to save building, uid:"<<user->uid_);
    char sql[1024];
    sprintf(sql, "update building_info set `level`='%d', `position`='%d', `gem_type`='%d' where `id`='%lld';", binf.level_, binf.position_, binf.gem_type_, binf.id_);
    if (execSql(sql)) {
        LOG4CXX_ERROR(logger_, "save building error, uid:"<<user->uid_<<" succ:"<<db_error_);
    }
}

void DataHandler::destroyBuilding(User *user, long long bid) {
    LOG4CXX_DEBUG(logger_, "try to destory building, uid:"<<user->uid_);
    char sql[1024];
    sprintf(sql, "delete from building_info where `id`='%lld';", bid);
    if (execSql(sql)) {
        LOG4CXX_ERROR(logger_, "destroy building error, uid:"<<user->uid_<<" succ:"<<db_error_);
    }
    else {
        user->build_infs_.erase(bid);
        //TODO
    }
}

void DataHandler::addRewardItem(User *user, vector <ItemConf> &items) {

}

void DataHandler::delReqItem(User *user, vector <ItemConf> &items) {

}

bool DataHandler::loadAllHonorExcStatus(User *user) {

    return true;
}

void DataHandler::saveHonorExcStatus(User *user, int index) {
}


