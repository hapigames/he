//
//  DataHandler.h
//  hero
//
//  Created by xujianfeng on 13-8-9.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_DataHandler_h
#define hero_DataHandler_h

#include "../class/User.h"
#include "../class/Hero.h"
#include "../class/Soldier.h"
#include "../class/Item.h"
#include "../class/Friend.h"
#include "../class/Mail.h"
#include "../common/Const.h"
#include "../utils/Util.h"
#include "../common/GameConfig.h"
#include "../class/Payment.h"
#include "../class/Act.h"
#include <map>
#include <queue>
#include <vector>
#include <mysql.h>
//#include "3rd-party/mysql/include/mysql.h"
#include <cstdio>
#include <iostream>

#include <log4cxx/logger.h>
#include <log4cxx/log4cxx.h>
#include <log4cxx/propertyconfigurator.h>
//#include "3rd-party/lib4cxx/include/log4cxx/logger.h"
//#include "3rd-party/lib4cxx/include/log4cxx/log4cxx.h"
//#include "3rd-party/lib4cxx/include/log4cxx/propertyconfigurator.h"
using namespace std;

class DataHandler
{
public:
    DataHandler(int server_id);
    ~DataHandler();
    
    bool dhInitMysql();
    void closeDbConnection();
    
    User * getUser(uid_type uid);
    Hero * getHero(User * user,long long hid);
    Soldier * getSoldier(User * user,long long sid);
    User * readUser(MYSQL_ROW &row);
    
    bool getUserBasicInfo(string &name,string &passwd,UserBasicInfo &ubi);
    bool usernameHasBeenUsed(string &name);
    bool registerNewUser(string &name,string &passwd,string &eqid,long long &user_id);
    bool changeEquipment(uid_type uid, string &eqid);
    bool saveEquipmentId(User * user);
    bool checkNick(string &nickname);
    bool createUser(uid_type uid, vector<string> &param);
    bool createFakeUser();
    bool loadAllHeroes(User * user);
    bool loadHero(MYSQL_ROW &row,int mnf,User * user);
    bool loadAllSoldiers(User * user);
    bool loadSoldier(MYSQL_ROW &row,int mnf,User * user);
    bool loadAllItems(User * user);
    bool loadItem(MYSQL_ROW &row,int mnf,User * user);
    bool loadAllFriends(User * user);
    bool loadFriend(MYSQL_ROW &row,int mnf,User * user);
    bool loadAllFriendRequest(User * user);
    bool loadFriendRequest(MYSQL_ROW &row,int mnf,User * user);
    bool loadAllMails(User * user);
    bool loadMail(MYSQL_ROW &row,int mnf,User * user);
    bool loadAllMissions(User * user);
    bool loadMission(MYSQL_ROW &row,int mnf,User * user);
    bool loadAllStageRecord(User * user);
    bool loadStageRecord(MYSQL_ROW &row,int mnf,User * user);
    bool loadPayment(string & tran_id,PaymentInfo *&pi);
    NewPaymentInfo * loadNewPaymentInfo(string &tran_id);
    NewPaymentInfo * loadNewPaymentInfoBak(string &tran_id);
    bool loadAllAct(User * user);
    bool loadAct(MYSQL_ROW &row,int mnf,User * user);
    bool loadAllTeams(User * user);
    bool loadTeam(MYSQL_ROW &row,int mnf,User * user);

    bool saveUser(User * user);
    bool saveUserSInfo(User * user);
    bool saveHero(Hero * hero);
    bool saveSoldier(Soldier * soldier);
    bool saveItem(Item * item);
    bool saveFriendInfo(FriendInfo *);
    bool saveMail(Mail * mail);
    bool saveMission(Mission * mission);
    bool saveUbi(UserBasicInfo * ubi);
    bool savePaymentStatus(string &tran_id,int status);
    bool saveStageRecord(StageRecord * sr);
    bool saveNewPaymentStatus(string &tran_id,int status);
    bool saveNewPaymentStatusBak(string &tran_id,int status);
    bool saveAct(Act * act);
    bool saveTeam(Team * team);

    bool addNewHero(User * user,int mid,int level,int star,long long now);
    bool addNewSoldier(User * user,int mid,int level,int star,long long now);
    bool addItem(User * user,int type,int mid,int amount,long long now);
    bool addFriendRequest(User * user,uid_type tuid,uid_type fuid);
    bool addFriend(User * user,User * tuser);
    bool addNewMail(User * tuser,uid_type fuid,string &title,string &text,int acceptable);
    bool addNewMission(User * user,int type,int subtype,int record,int complete);
    bool addNewStageRecord(User * user,int chapter_id,int stage_id);
    bool addNewPayment(PaymentInfo * pi);
    bool addNewActRecord(User * user,int act_id);
    bool addNewTeam(User * user,long long now);
    
    void updateFresh(User * user);
    
    bool deleteFriendRequest(long long record_id);
    bool deleteHero(long long hero_id);
    bool deleteSoldier(long long soldier_id);
    bool deleteFriend(long long record_id);
    bool deleteMail(long long mail_id);
    bool deleteMission(long long mission_id);
    
    void addNews(string &name,int action,int param_1,int param_2,int param_3);
    
    map<uid_type,User*> users_;
    MYSQL mysql_;
    //MYSQL_ROW row_;
    //MYSQL_RES * res_;
    int db_error_;
    bool dbc_open_;
    
    vector< vector< User * > > fresh_;
    vector< int > fresh_pointer_;
    
    vector<News> news_;
    int news_pointer_;
    int global_news_index_;
    int server_id_;
    
    log4cxx::LoggerPtr logger_;
    
};

#endif
