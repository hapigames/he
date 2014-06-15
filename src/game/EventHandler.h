//
//  EventHandler.h
//  hero
//
//  Created by xujianfeng on 13-8-9.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_EventHandler_h
#define hero_EventHandler_h

#include "../net/NetHandler.h"
#include "../event/EventQueue.h"
#include "DataHandler.h"
#include "Message.h"
#include "../common/GameConfig.h"
#include "../class/Stage.h"
#include "../class/Payment.h"
#include "../utils/Util.h"
#include "../utils/md5.h"
#include "../common/Platform.h"
#include <map>
#include <iostream>
#include "../common/json-util.h"
#include <curl/curl.h>

#include <log4cxx/logger.h>
#include <log4cxx/log4cxx.h>
#include <log4cxx/propertyconfigurator.h>
//#include "3rd-party/lib4cxx/include/log4cxx/logger.h"
//#include "3rd-party/lib4cxx/include/log4cxx/log4cxx.h"
//#include "3rd-party/lib4cxx/include/log4cxx/propertyconfigurator.h"

using namespace std;

extern GameConfig game_config;
extern string cmd_list[];
class EventHandler
{
public:
    EventHandler(NetHandler * nh,EventQueue * eq,int server_id);
    ~EventHandler();
    
    void handle(EventCmd &e);
    
    NetHandler * nh_;
    EventQueue * eq_;
    DataHandler * dh_;
    log4cxx::LoggerPtr logger_;
    int server_id_;
    
    void processRegister(EventCmd &e,vector<string> &cmd);
    void processLogin(EventCmd &e,vector<string> &cmd);
    void processAuthorize(EventCmd &e,vector<string> &cmd);
    void processLoadUser(EventCmd &e,vector<string> &cmd);
    void processCheckNick(EventCmd &e,vector<string> &cmd);
    void processCreateUser(EventCmd &e,vector<string> &cmd);
    void processLoadAll(EventCmd &e,vector<string> &cmd);
    void processLoadHero(EventCmd &e,vector<string> &cmd);
    void processLoadSoldier(EventCmd &e,vector<string> &cmd);
    void processLoadItem(EventCmd &e,vector<string> &cmd);
    void processChangeHero(EventCmd &e,vector<string> &cmd);
    void processChangeSoldier(EventCmd &e,vector<string> &cmd);
    void processLoadAssistant(EventCmd &e,vector<string> &cmd);
    void processBattleStart(EventCmd &e,vector<string> &cmd);
    void processBattleEnd(EventCmd &e,vector<string> &cmd);
    void processFriendRequest(EventCmd &e,vector<string> &cmd);
    void processSellHero(EventCmd &e,vector<string> &cmd);
    void processSellSoldier(EventCmd &e,vector<string> &cmd);
    void processLockHero(EventCmd &e,vector<string> &cmd);
    void processLockSoldier(EventCmd &e,vector<string> &cmd);
    void processHeroExpUp(EventCmd &e,vector<string> &cmd);
    void processSoldierExpUp(EventCmd &e,vector<string> &cmd);
    void processHeroEnhance(EventCmd &e,vector<string> &cmd);
    void processSoldierEnhance(EventCmd &e,vector<string> &cmd);
    void processSkillLevelUp(EventCmd &e,vector<string> &cmd);
    void processSendEnergy(EventCmd &e,vector<string> &cmd);
    void processAcceptEnergy(EventCmd &e,vector<string> &cmd);
    void processLoadFriend(EventCmd &e,vector<string> &cmd);
    void processLoadFriendRequest(EventCmd &e,vector<string> &cmd);
    void processAddFriend(EventCmd &e,vector<string> &cmd);
    void processRefuseFriendRequest(EventCmd &e,vector<string> &cmd);
    void processDeleteFriend(EventCmd &e,vector<string> &cmd);
    void processSoulMerge(EventCmd &e,vector<string> &cmd);
    void processLoadUserById(EventCmd &e,vector<string> &cmd);
    void processLoadMail(EventCmd &e,vector<string> &cmd);
    void processOpenMail(EventCmd &e,vector<string> &cmd);
    void processDeleteMail(EventCmd &e,vector<string> &cmd);
    void processSendMail(EventCmd &e,vector<string> &cmd);
    void processAcceptMailItem(EventCmd &e,vector<string> &cmd);
    void processGetNewMailNum(EventCmd &e,vector<string> &cmd);
    void processLoadMission(EventCmd &e,vector<string> &cmd);
    void processGetCMNum(EventCmd &e,vector<string> &cmd);
    void processGetMissionReward(EventCmd &e,vector<string> &cmd);
    void processBuyEnergy(EventCmd &e,vector<string> &cmd);
    void processExpandHero(EventCmd &e,vector<string> &cmd);
    void processExpandSoldier(EventCmd &e , vector<string> &cmd);
    void processExpandFriend(EventCmd &e, vector<string> &cmd);
    void processSummon(EventCmd &e,vector<string> &cmd);
    void processLoadNews(EventCmd &e,vector<string> &cmd);
    void processPaymentVerify(EventCmd &e,vector<string> &cmd);
    void processPaymentVerifyReceived(EventCmd &e,vector<string> &cmd);
    void processAdminAlterUser(EventCmd &e,vector<string> &cmd);
    void processAdminAddHero(EventCmd &e,vector<string> &cmd);
    void processAdminAlterHero(EventCmd &e,vector<string> &cmd);
    void processAdminAddSoldier(EventCmd &e,vector<string> &cmd);
    void processAdminAddItem(EventCmd &e,vector<string> &cmd);
    void processLog(EventCmd &e,vector<string> &cmd);
    void processGuide(EventCmd &e,vector<string> &cmd);
    void processAlterUserName(EventCmd &e, vector<string> &cmd);
    void processPaymentVerifyComplete(EventCmd &e,vector<string> &cmd);
    void processPaymentAddDiamondRequest(EventCmd &e,vector<string> &cmd);
    void processPaymentAddDiamondResponse(EventCmd &e,vector<string> &cmd);
    void processPaymentTest(EventCmd &e,vector<string> &cmd);
    void processSessionVerify(EventCmd &e, vector<string> &cmd);
    void processNewPaymentverify(EventCmd &e,vector<string> &cmd);
    void processPBak(EventCmd &e,vector<string> &cmd);
    void processLoadAct(EventCmd &e ,vector<string> &cmd);
    void processAct(EventCmd &e,vector<string> &cmd);
    void processLoadTeam(EventCmd &e,vector<string> &cmd);
    void processSaveTeam(EventCmd &e,vector<string> &cmd);
    void processChangeTeam(EventCmd &e,vector<string> &cmd);
    void processChangeNick(EventCmd &e,vector<string> &cmd);
    void processForceHEU(EventCmd &e,vector<string> &cmd);
    void processForceSEU(EventCmd &e,vector<string> &cmd);
    void processAddFakeUser(EventCmd &e,vector<string> &cmd);
    void processDebug(EventCmd &e,vector<string> &cmd);
    void processInstantBattle(EventCmd &e, vector<string> &cmd);
    void processResetEliteStage(EventCmd &e, vector<string> &cmd);
    void processInstantTrial(EventCmd &e, vector <string> &cmd);
    void processTrialProgress(EventCmd &e, vector <string> &cmd);
    void processGetTrialRank(EventCmd &e, vector <string> &cmd);
    void processResetTrialStage(EventCmd &e, vector <string> &cmd);
    void processTrialRelive(EventCmd &e, vector <string> &check_cmd);

    bool safeLoadBuildings(User *user, int cmd, int fd);
    bool safeLoadPvpHeros(User *user, int cmd, int fd);
    bool safeLoadPvpGears(User *user, int cmd, int fd);
    bool safeLoadHonorExcStatus(User *user, int cmd, int fd);

    void processLoadbuildings(EventCmd &e, vector<string> &check_cmd) ;
    void processLoadBuildingsById(EventCmd &e, vector<string> &check_cmd) ;
    void processPvpRanks(EventCmd &e, vector<string> &check_cmd) ;
    void processPvpTargets(EventCmd &e, vector<string> &check_cmd) ;
    void processPvpBattleStart(EventCmd &e, vector<string> &check_cmd) ;
    void processPvpBattleEnd(EventCmd &e, vector<string> &check_cmd) ;
    void processAddBuilding(EventCmd &e, vector<string> &check_cmd) ;
    void processSaveBuildings(EventCmd &e, vector<string> &check_cmd) ;
    void processUpgradeBuilding(EventCmd &e, vector<string> &check_cmd) ;
    void processDestroyBuilding(EventCmd &e, vector<string> &check_cmd) ;
    void processSetGem(EventCmd &e, vector<string> &check_cmd) ;
    void processLoadHonorExcStatus(EventCmd &e, vector<string> &check_cmd) ;
    void processHonorExc(EventCmd &e, vector<string> &check_cmd) ;
    void processLoadGears(EventCmd &e, vector<string> &check_cmd) ;
    void processUpgradeGear(EventCmd &e, vector<string> &check_cmd) ;
    
    void sendErrorResponse(int fd,int cmd_code,int error_code);
    void sendErrorResponseFroPayment(int fd,int cmd_code,int error_code,string &tran_id);
    User * safeGetUser(uid_type uid,int cmd_code,int fd);
    User * safeGetUser(uid_type uid,int cmd_code,int fd,string &eqid);
    Hero * safeGetHero(User * user,long long hero_id,int cmd_code,int fd);
    Soldier * safeGetSoldier(User * user,long long soldier_id,int cmd_code,int fd);
    FriendInfo * safeGetFriendInfo(User * user,uid_type fuid,int cmd_code,int fd);
    FriendRequestInfo * safeGetFriendRequestInfo(User * user,uid_type fuid,int cmd_code,int fd);
    Item * safeGetItem(User * user,int type,int mid);
    Mail * safeGetMail(User * user,long long mail_id,int cmd_code,int fd);
    Mission * safeGetMission(User * user,long long mission_id,int cmd_code,int fd);
    bool safeLoadHero(User * user,int cmd_code,int fd);
    bool safeLoadSoldier(User * user, int cmd_code,int fd);
    bool safeLoadItem(User * user, int cmd_code,int fd);
    bool safeLoadFriend(User * user,int cmd_code,int fd);
    bool safeLoadFriendRequest(User * user,int cmd_code,int fd);
    bool safeLoadMail(User * user,int cmd_code,int fd);
    bool safeLoadMission(User * user,int cmd_code,int fd);
    bool safeLoadMilitary(User * user);
    bool safeLoadStageRecord(User * user,int cmd_code,int fd);
    bool safeLoadAct(User * user,int cmd_code,int fd);
    bool safeLoadTeam(User * user,int cmd_code,int fd);
    
    bool preCheckLeadershipByChangeHero(User * user, Hero * hero);
    bool preCheckLeadershipByChangeSoldier(User * user,Soldier * soldier,int pid);
    bool teamPrepare(User * user,long long hero_id,vector<long long > &soldier_id,Hero * &hero,vector<Soldier *> &soldiers);
    bool preCheckLeadership(User * user,Hero * hero,vector<Soldier*> &soldiers);
    bool checkEnergy(User * user,int energy_needed,long long now);
    bool addAssistantReward(User *user,User * assistant);
    void costEnergyForBattle(User * user,Stage * stage);
    void addBasicBattleReward(User * user,Stage * stage,int type);
    void setStageInfo(User * user);
    void resetBattleInfo(User * user);
    bool addStageReward(User * user,StageReward *rew,long long now);
    Stage *  preCheckCanBattle(User * user,int type,int chapter_id,int stage_id,long long now,int cmd_code,int fd);
    bool preCheckBagLength(User * user , Stage * stage);
    bool dailyUpdateUser(User * user,int cmd_code,int fd);
    bool checkCost(User * user,EnhanceCost &ec);
    bool safeCost(User * user,EnhanceCost &ec,int cmd_code,int fd);
    bool sendSystemMail(User * user);
    bool addDailyRewardMission(User * user);
    bool addLinkMission(User * user, MissionConfig * mc,int param);
    bool updateMissionAfterBattle(User * user,vector<Mission *> &mv);
    bool addDailyFreshMission(User * user);
    bool addSendEnergyMission(User * user);
    bool initMission(User * user);
    bool eliteDailyCheck(User * user,int chapter_id, int stage_id,int times);
    bool setEliteDaily(User * user,int chapter_id,int stage_id);
    bool specialDailyCheck(User * user,int chapter_id, int stage_id,int times);
    bool setSpecialDaily(User * user,int chapter_id,int stage_id);
    bool resetEliteDaily(User * user, int type, int chapter_id, int stage_id);
    bool setTrialDaily(User *user, int chapter_id, int stage_id);
    void trialDailyCheck(User *user, int chapter_id);
    bool specialTimeCheck(User * user,Stage * stage,long long now);
    bool dateChange(long long now ,long long comp);
    int getSpecialChaptertoSave(int chapter_id);
    int getTrialChaptertoSave(int chapter_id);
    int getChaptertoSaveKey(int type, int chapter_id);
    Team * inTeamHero(User * user,long long hero_id);
    Team * inTeamSoldier(User *user, long long solider_id);
    void modifyBattlereward(vector<StageReward *> &rewards,int limit);
    
    Act * getAct(User * user,int act_id);
    void initAct(Act * act,int version);
    void initAct2(Act * act,int version);
    void initAct3(Act * act,int version);
    
    
    void act_1(User * user,int amount);
    void act_2(uid_type uid,vector<string> &cmd,int fd);
    void act_3(uid_type uid,vector<string> &cmd,int fd);

#if (PLATFORM & PLATFORM_APPSTORE) != 0
#elif (PLATFORM & PLATFORM_91) != 0
#endif
};
size_t writeDataCurl(void *buffer, size_t size, size_t nmemb, void *userp);

#if (PLATFORM & PLATFORM_APPSTORE) != 0

void * verify(void * args);
void * paymentAdd(void * args);

#elif (PLATFORM & PLATFORM_91) != 0

struct sessionVerifyInfo
{
    uid_type uid;
    string * nickname;
    string * eqid;
    EventQueue * eq;
    int fd_;
    string * sign;
};
void * sessionVerify(void * args);
void * paymentAdd(void * args);

#endif

#endif
