//
//  EventHandler.cpp
//  hero
//
//  Created by xujianfeng on 13-8-9.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#include "EventHandler.h"
#include "../utils/hstring.h"



EventHandler::EventHandler(NetHandler * nh,EventQueue * eq,int server_id)
{
    nh_ = nh;
    eq_ = eq;
    dh_ = new DataHandler(server_id);
    server_id_ = server_id;
    //curl_global_init(CURL_GLOBAL_ALL);
    log4cxx::PropertyConfigurator::configure("log4cxx.config");
    logger_ = log4cxx::Logger::getLogger("EventHandler");    
}
EventHandler::~EventHandler()
{
    //curl_global_cleanup();
}
void EventHandler::handle(EventCmd &event)
{
    LOG4CXX_DEBUG(logger_, "received:"<<event.cmd_);
    vector<string> check_cmd;
    check_cmd.clear();
    string delim = ";";
    
    tokenize(event.cmd_,check_cmd,delim);
    int cmd_index = get_cmd_id(check_cmd[0]);
    if(cmd_index == -1) return;
    if(dh_->db_error_ != 0){
        sendErrorResponse(event.fd_, cmd_index, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    switch(cmd_index){
        case CMD_REGISTER:
            processRegister(event, check_cmd);
            break;
        case CMD_LOGIN:
            processLogin(event,check_cmd);
            break;
        case CMD_AUTHORIZE:
            processAuthorize(event, check_cmd);
            break;
        case CMD_CHECKNICK:
            processCheckNick(event,check_cmd);
            break;
        case CMD_CREATEUSER:
            processCreateUser(event, check_cmd);
            break;
        case CMD_LOADALL:
            processLoadAll(event,check_cmd);
            break;
        case CMD_LOADUSER:
            processLoadUser(event,check_cmd);
            break;
        case CMD_LOADHERO:
            processLoadHero(event, check_cmd);
            break;
        case CMD_LOADSOLDIER:
            processLoadSoldier(event,check_cmd);
            break;
        case CMD_LOADITEM:
            processLoadItem(event,check_cmd);
            break;
        case CMD_CHANGEHERO:
            processChangeHero(event, check_cmd);
            break;
        case CMD_CHANGESOLDIER:
            processChangeSoldier(event,check_cmd);
            break;
        case CMD_LOADASSISTANT:
            processLoadAssistant(event, check_cmd);
            break;
        case CMD_BATTLESTART:
            processBattleStart(event, check_cmd);
            break;
        case CMD_BATTLEEND:
            processBattleEnd(event, check_cmd);
            break;
        case CMD_FRIENDREQUEST:
            processFriendRequest(event,check_cmd);
            break;
        case CMD_SELLHERO:
            processSellHero(event, check_cmd);
            break;
        case CMD_SELLSOLDIER:
            processSellSoldier(event,check_cmd);
            break;
        case CMD_LOCKHERO:
            processLockHero(event, check_cmd);
            break;
        case CMD_LOCKSOLDIER:
            processLockSoldier(event, check_cmd);
            break;
        case CMD_HEROEXPUP:
            processHeroExpUp(event, check_cmd);
            break;
        case CMD_SOLDIEREXPUP:
            processSoldierExpUp(event, check_cmd);
            break;
        case CMD_HEROENHANCE:
            processHeroEnhance(event, check_cmd);
            break;
        case CMD_SOLDIERENHANCE:
            processSoldierEnhance(event, check_cmd);
            break;
        case CMD_SKILLLEVELUP:
            processSkillLevelUp(event, check_cmd);
            break;
        case CMD_SENDENERGY:
            processSendEnergy(event, check_cmd);
            break;
        case CMD_ACCEPTENERGY:
            processAcceptEnergy(event, check_cmd);
            break;
        case CMD_LOADFRIEND:
            processLoadFriend(event, check_cmd);
            break;
        case CMD_LOADFRIENDREQUEST:
            processLoadFriendRequest(event,check_cmd);
            break;
        case CMD_ADDFRIEND:
            processAddFriend(event, check_cmd);
            break;
        case CMD_REFUSEFRIENDREQ:
            processRefuseFriendRequest(event, check_cmd);
            break;
        case CMD_DELETEFRIEND:
            processDeleteFriend(event, check_cmd);
            break;
        case CMD_SOULMERGE:
            processSoulMerge(event, check_cmd);
            break;
        case CMD_LOADUSERBYID:
            processLoadUserById(event, check_cmd);
            break;
        case CMD_LOADMAIL:
            processLoadMail(event,check_cmd);
            break;
        case CMD_OPENMAIL:
            processOpenMail(event, check_cmd);
            break;
        case CMD_DELETEMAIL:
            processDeleteMail(event, check_cmd);
            break;
        case CMD_SENDMAIL:
            processSendMail(event,check_cmd);
            break;
        case CMD_ACCEPTMAILITEM:
            processAcceptMailItem(event, check_cmd);
            break;
        case CMD_GETNEWMAILNUM:
            processGetNewMailNum(event, check_cmd);
            break;
        case CMD_LOADMISSION:
            processLoadMission(event,check_cmd);
            break;
        case CMD_GETCMNUM:
            processGetCMNum(event, check_cmd);
            break;
        case CMD_GETMISSIONREWARD:
            processGetMissionReward(event, check_cmd);
            break;
        case CMD_BUYENERGY:
            processBuyEnergy(event, check_cmd);
            break;
        case CMD_EXPANDHERO:
            processExpandHero(event, check_cmd);
            break;
        case CMD_EXPANDSOLDIER:
            processExpandSoldier(event,  check_cmd);
            break;
        case CMD_EXPANDFRIEND:
            processExpandFriend(event, check_cmd);
            break;
        case CMD_SUMMON:
            processSummon(event, check_cmd);
            break;
        case CMD_LOADNEWS:
            processLoadNews(event,check_cmd);
            break;
        case CMD_PAYMENTVERIFY:
            processPaymentVerify(event , check_cmd);
            break;
        case CMD_PAYMENTVERIFYRECEIVED:
            processPaymentVerifyReceived(event, check_cmd);
            break;
        case CMD_ADMINALTERUSER:
            processAdminAlterUser(event, check_cmd);
            break;
        case CMD_ADMINADDHERO:
            processAdminAddHero(event, check_cmd);
            break;
        case CMD_ADMINALTERHERO:
            processAdminAlterHero(event,check_cmd);
            break;
        case CMD_ADMINADDSOLDIER:
            processAdminAddSoldier(event, check_cmd);
            break;
        case CMD_ADMINADDITEM:
            processAdminAddItem(event, check_cmd);
            break;
        case CMD_LOG:
            processLog(event, check_cmd);
            break;
        case CMD_GUIDE:
            processGuide(event, check_cmd);
            break;
        case CMD_ALTERUSERNAME:
            processAlterUserName(event, check_cmd);
            break;
        case CMD_PAYMENTVERIFYCOMPLETE:
            processPaymentVerifyComplete(event, check_cmd);
            break;
        case CMD_PAYMENT_ADD_DIAMOND_REQUEST:
            processPaymentAddDiamondRequest(event, check_cmd);
            break;
        case CMD_PAYMENT_ADD_DIAMOND_RESPONSE:
            processPaymentAddDiamondResponse(event, check_cmd);
            break;
        case CMD_PAYMENTTEST:
            processPaymentTest(event, check_cmd);
            break;
        case CMD_SESSION_VERIFY:
            processSessionVerify(event, check_cmd);
            break;
        /*case CMD_NEWPAYMENTVERIFY:
            processNewPaymentverify(event, check_cmd);
            break;*/
        case CMD_PBAK:
            processPBak(event, check_cmd);
            break;
        case CMD_LOADACT:
            processLoadAct(event, check_cmd);
            break;
        case CMD_ACT:
            processAct(event,check_cmd);
            break;
        case CMD_LOADTEAM:
            processLoadTeam(event,check_cmd);
            break;
        case CMD_SAVETEAM:
            processSaveTeam(event, check_cmd);
            break;
        case CMD_CHANGETEAM:
            processChangeTeam(event, check_cmd);
            break;
        case CMD_CHANGENICK:
            processChangeNick(event, check_cmd);
            break;
        case CMD_FORCEHEU:
            processForceHEU(event, check_cmd);
            break;
        case CMD_FORCESEU:
            processForceSEU(event, check_cmd);
            break;
        case CMD_ADDFAKEUSER:
            processAddFakeUser(event, check_cmd);
            break;
        case CMD_DEBUG:
            processDebug(event,check_cmd);
            break;
        case CMD_INSTANT_BATTLE:
            processInstantBattle(event, check_cmd);
            break;
        case CMD_RESET_ELITE_STAGE:
            processResetEliteStage(event, check_cmd);
            break;
        case CMD_INSTANT_TRIAL:
            processInstantTrial(event ,check_cmd);
            break;
        case CMD_TRIAL_PROGRESS:
            processTrialProgress(event ,check_cmd);
            break;
        case CMD_GET_TRIAL_RANK:
            processGetTrialRank(event, check_cmd);
            break;
        case CMD_TRIAL_RESET:
            processResetTrialStage(event, check_cmd);
            break;
        case CMD_TRIAL_RELIVE:
            processTrialRelive(event, check_cmd);
            break;
        default:
            break;
    }
}
void EventHandler::processCheckNick(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=2) return;
    if(check_cmd[1].size()>32 || check_cmd[1].size() == 0) return;
    bool check = dh_->checkNick(check_cmd[1]);
    if(dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_CHECKNICK, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    if(check == false){
        sendErrorResponse(e.fd_, CMD_CHECKNICK, ERROR_NICKNAME_USED);
        return;
    }else{
        string res = buildCheckNickResponse();
        nh_->sendString(e.fd_, res);
        return;
    }
}
void EventHandler::processLoadAll(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=3) return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_LOADALL, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadHero(user, CMD_LOADALL, e.fd_);
    if(check == false) return;
    check = safeLoadSoldier(user, CMD_LOADALL, e.fd_);
    if(check == false) return;
    check = safeLoadStageRecord(user, CMD_LOADALL, e.fd_);
    if(check == false) return;
    check = safeLoadTeam(user, CMD_LOADALL, e.fd_);
    if(check == false) return;
    string res = buildLoadAllResponse(user);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    sendSystemMail(user);
    LOG4CXX_INFO(logger_, "loadall;"<<user->uid_);
    return;
}
void EventHandler::processLoadUser(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=3) return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_LOADUSER, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadStageRecord(user, CMD_LOADUSER,e.fd_);
    if(check == false) return;
    check = safeLoadTeam(user, CMD_LOADUSER, e.fd_);
    if(check == false) return;
    string res = buildLoadUserResponse(user);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processLoadHero(EventCmd &e,vector<string> &check_cmd)
{
    if(check_cmd.size()!=3) return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_LOADHERO, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadHero(user, CMD_LOADHERO, e.fd_);
    if(check == false) return;
    string res = buildLoadHeroResponse(user);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processLoadSoldier(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=3) return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_LOADSOLDIER, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadSoldier(user, CMD_LOADSOLDIER, e.fd_);
    if(check == false) return;
    string res = buildLoadSoldierResponse(user);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processLoadItem(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4) return;
    uid_type uid;
    int item_type;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoi(check_cmd[3], item_type);
    if(check == false) return;
    if(item_type != ITEM_TYPE_RESOURCE && item_type != ITEM_TYPE_CHIP) return;
    User * user = safeGetUser(uid, CMD_LOADITEM, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadItem(user, CMD_LOADITEM, e.fd_);
    if(check == false) return;
    string res = buildLoadItemResponse(user,item_type);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processChangeHero(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4) return;
    uid_type uid;
    long long hid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], hid);
    User * user = safeGetUser(uid, CMD_CHANGEHERO, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadHero(user, CMD_CHANGEHERO, e.fd_);
    if(check == false) return;
    check = safeLoadSoldier(user, CMD_CHANGEHERO, e.fd_);
    if(check == false)return;
    check = safeLoadTeam(user, CMD_CHANGEHERO, e.fd_);
    if(check == false) return;
    Hero * hero = dh_->getHero(user,hid);
    if(hero == NULL){
        sendErrorResponse(e.fd_, CMD_CHANGEHERO, ERROR_HERO_NOT_FOUND);
        return;
    }
    if(hero->owner_id_ != uid){
        sendErrorResponse(e.fd_, CMD_CHANGEHERO, ERROR_OWNER_DISMATCH);
        return;
    }
    if(preCheckLeadershipByChangeHero(user, hero) == false){
        sendErrorResponse(e.fd_, CMD_CHANGEHERO, ERROR_LEADERSHIP_NOT_ENOUGH);
        return;
    }
    
    map<long long,Team *>::iterator it = user->teams_.find(user->team_id_);
    if(it==user->teams_.end()){
        sendErrorResponse(e.fd_, CMD_SAVETEAM, ERROR_TEAM_CANNOT_BE_FOUND);
        return;
    }
    Team * team = it->second;
    if(team == NULL){
        sendErrorResponse(e.fd_, CMD_SAVETEAM, ERROR_TEAM_CANNOT_BE_FOUND);
        return;
    }
    team->hero_id_ = hid;
    user->hero_id_ = hid;
    user->dirty_ = true;
    check = dh_->saveUser(user);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_CHANGEHERO,ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    check = dh_->saveTeam(team);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_CHANGEHERO,ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildChangeHeroResponse(user);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processChangeSoldier(EventCmd &e,vector<string> &check_cmd)
{
    if(check_cmd.size()!=4)return;
    uid_type uid;
    vector<long long> sids;
    sids.clear();
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    string delims = ",";
    check = longtokenize(check_cmd[3], sids, delims);
    if(check == false)return;
    if(sids.size()!=SOLDIER_INGROUND)return;
    User * user = safeGetUser(uid, CMD_CHANGESOLDIER, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadHero(user, CMD_CHANGESOLDIER, e.fd_);
    if(check == false) return;
    check = safeLoadSoldier(user, CMD_CHANGESOLDIER, e.fd_);
    if(check == false)return;
    check = safeLoadTeam(user, CMD_CHANGEHERO, e.fd_);
    if(check == false) return;
    map<long long,Team *>::iterator it = user->teams_.find(user->team_id_);
    if(it==user->teams_.end()){
        sendErrorResponse(e.fd_, CMD_SAVETEAM, ERROR_TEAM_CANNOT_BE_FOUND);
        return;
    }
    Team * team = it->second;
    if(team == NULL){
        sendErrorResponse(e.fd_, CMD_SAVETEAM, ERROR_TEAM_CANNOT_BE_FOUND);
        return;
    }
    vector<Soldier *> chosen_soldiers;
    chosen_soldiers.clear();
    int leadership_sum = 0;
    Hero * hero = dh_->getHero(user,user->hero_id_);
    if(hero == NULL){
        sendErrorResponse(e.fd_, CMD_CHANGEHERO, ERROR_HERO_NOT_FOUND);
        return;
    }
    int ls_temp = game_config.heroLeadershipRequire(hero->mid_, hero->star_);
    if(ls_temp < 0){
        sendErrorResponse(e.fd_, CMD_CHANGESOLDIER, ERROR_CONFIG_ERROR);
        return;
    }
    leadership_sum += ls_temp;
    for(size_t i = 0;i<sids.size();i++){
        Soldier * soldier = safeGetSoldier(user, sids[i], CMD_CHANGESOLDIER, e.fd_);
        if(soldier == NULL) return;
        for(size_t j = 0;j<chosen_soldiers.size();j++){
            if(chosen_soldiers[j]->mid_ == soldier->mid_){
                sendErrorResponse(e.fd_, CMD_CHANGESOLDIER, ERROR_SOLDIER_THE_SAME_MID);
                return;
            }
        }
        chosen_soldiers.push_back(soldier);
        leadership_sum += game_config.soldierLeadershipRequire(soldier->mid_, soldier->star_);
    }
    if(leadership_sum > game_config.leadershipLimit(user->user_level_)){
        sendErrorResponse(e.fd_, CMD_CHANGESOLDIER, ERROR_LEADERSHIP_NOT_ENOUGH);
        return;
    }
    for(int i = 0;i<SOLDIER_INGROUND;i++){
        user->soldier_id_[i] = sids[i];
        team->soldier_id_[i] = sids[i];
    }
    user->dirty_ = true;
    check = dh_->saveUser(user);
    if(check == false||dh_->db_error_){
        sendErrorResponse(e.fd_, CMD_CHANGESOLDIER, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    check = dh_->saveTeam(team);
    if(check == false||dh_->db_error_){
        sendErrorResponse(e.fd_, CMD_CHANGESOLDIER, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildChangeSoldierResponse(user);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processLoadAssistant(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=3) return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_LOADASSISTANT, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadFriend(user, CMD_LOADASSISTANT, e.fd_);
    if(check == false) return;
    vector<User *> temp;
    vector<Hero *> htemp;
    long long now = time(NULL);
    for(map<long long,FriendInfo *>::iterator it = user->friends_.begin();
        it!= user->friends_.end();it++){
        User * friend_u = dh_->getUser(it->second->fuid_);
        if(dh_->db_error_ != 0){
            sendErrorResponse(e.fd_, CMD_LOADASSISTANT,ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        if(friend_u == NULL) continue;
        if(safeLoadHero(friend_u, CMD_LOADASSISTANT, e.fd_) == false)return;
        Hero * fhero = safeGetHero(friend_u, friend_u->hero_id_, CMD_LOADASSISTANT, e.fd_);
        if(fhero==NULL) continue;
        if(!(now - friend_u->last_active_time_ <= game_config.friend_fresh_interval_
             && abs(user->user_level_ - friend_u->user_level_)<game_config.fresh_level_limit_)){
            continue;
        }
        temp.push_back(friend_u);
        htemp.push_back(fhero);
        if((int) temp.size()>=game_config.fresh_friend_num_limit_)break;
    }
    int friend_num = (int)temp.size();
    int flevel = (user->user_level_ - 1)/game_config.fresh_interval_;
    for(size_t i = 0;i<dh_->fresh_[flevel].size();i++){
        bool fresh_check = false;
        User * fresh = dh_->fresh_[flevel][i];
        if(fresh == NULL) continue;
        if(fresh == user) continue;
        fresh_check = false;
        for(int j = 0;j<friend_num;j++){
            if(temp[j] == fresh){
                fresh_check = true;
                break;
            }
        }
        if(fresh_check == true) continue;
        fresh_check = safeLoadHero(fresh, CMD_LOADASSISTANT, e.fd_);
        if(fresh_check == false) return;
        Hero * fhero = dh_->getHero(fresh, fresh->hero_id_);
        if(dh_->db_error_ != 0){
            sendErrorResponse(e.fd_, CMD_LOADASSISTANT, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        if(fhero == NULL) continue;
        temp.push_back(fresh);
        htemp.push_back(fhero);
        if((int) temp.size()>=game_config.fresh_choice_num_){
            break;
        }
    }
    string res = buildLoadAssistantResponse(user, temp, friend_num,htemp);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processBattleStart(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=7) return;
    uid_type uid,assistant_uid;
    int type;
    int chapter_id;
    int stage_id;
    if ( !safeAtoll(check_cmd[1], uid)
        || !safeAtoi(check_cmd[3], type)
        || !safeAtoi(check_cmd[4], chapter_id)
        || !safeAtoi(check_cmd[5], stage_id)
        || !safeAtoll(check_cmd[6], assistant_uid)) {
        return;
    }
    User * user = safeGetUser(uid, CMD_BATTLESTART, e.fd_, check_cmd[2]);
    if (user == NULL 
            || !safeLoadHero(user, CMD_BATTLESTART, e.fd_)
            || !safeLoadSoldier(user, CMD_BATTLEEND, e.fd_)
            || !safeLoadStageRecord(user, CMD_BATTLESTART, e.fd_)) {
        return;
    }

    User * assistant = NULL;
    if(assistant_uid != 0){
        assistant = dh_->getUser(assistant_uid);
        if(assistant == NULL){
            sendErrorResponse(e.fd_ , CMD_BATTLESTART, ERROR_ASSISTANT_NOT_EXIST);
            return;
        }
    }else{
        user->assistant_id_ = 0;
    }

    long long now = time(NULL);
    Stage * stage = preCheckCanBattle(user, type, chapter_id, stage_id, now, CMD_BATTLESTART, e.fd_);
    if(stage == NULL) return;
    int hero_limit = game_config.getValueFromIntVectorFilledZero(user->user_level_, game_config.hero_length_limit_);
    int soldier_limit = game_config.getValueFromIntVectorFilledZero(user->user_level_, game_config.soldier_length_limit_);
    if(hero_limit == -1 || soldier_limit == -1) return;
    if((int) user->heroes_.size() >= hero_limit + user->expand_hero_ * game_config.place_per_expand_hero_ 
       || (int)user->soldiers_.size()  >= soldier_limit + user->expand_soldier_ * game_config.place_per_expand_soldier_){
        sendErrorResponse(e.fd_, CMD_BATTLESTART, ERROR_BAG_IS_FULL);
        return;
    }
    user->stage_type_ = type;
    user->battle_chapter_id_ = chapter_id;
    user->battle_stage_id_ = stage_id;
    if(assistant!=NULL){
        user->assistant_id_ = assistant_uid;
    }
    user->rewards_.clear();
    for(size_t i = 0;i<stage->rewards_.size();i++){
        int pby = rand()%PBY_PRECISION;
        if(pby<stage->rewards_[i].pby){
            user->rewards_.push_back(&(stage->rewards_[i]));
        }
    }
    if(type == STAGE_TYPE_NORMAL){
        modifyBattlereward(user->rewards_, game_config.normal_stage_reward_limit_);
    }else if(type == STAGE_TYPE_ELITE){
        modifyBattlereward(user->rewards_, game_config.elite_stage_daily_limit_);
    }else if(type == STAGE_TYPE_SPECIAL){
        modifyBattlereward(user->rewards_, game_config.special_stage_reward_limit_);
    }

    LOG4CXX_INFO(logger_, "battlestart;"<<uid<<";"<<assistant_uid<<";"<<type<<";"<<chapter_id<<";"<<stage_id);
    string res = buildBattleStartResponse(user);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);

    return;
}

void EventHandler::processBattleEnd(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4) return;
    uid_type uid ;
    int win;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_BATTLEEND, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeAtoi(check_cmd[3], win);
    if(check == false) return;
    if(win!= 0 && win!= 1) return;
    check = safeLoadFriend(user, CMD_BATTLEEND, e.fd_);
    if(check == false) return;
    check = safeLoadMission(user, CMD_BATTLEEND, e.fd_);
    if(check == false) return;
    check = safeLoadItem(user, CMD_BATTLEEND, e.fd_);
    if(check == false) return;
    check = safeLoadHero(user, CMD_BATTLEEND, e.fd_);
    if(check == false) return;
    check = safeLoadSoldier(user,CMD_BATTLEEND,e.fd_);
    if(check == false) return;
    check = safeLoadStageRecord(user, CMD_BATTLEEND, e.fd_);
    if(user->stage_type_ == 0 || user->battle_chapter_id_ == 0 || user->battle_stage_id_ == 0){
        sendErrorResponse(e.fd_, CMD_BATTLEEND, ERROR_BATTLE_NOT_INITED);
        return ;
    }
    vector<Mission *> mv;
    mv.clear();
    if(win == 0){
        LOG4CXX_INFO(logger_, "battlefail;"<<uid);
        resetBattleInfo(user);
        string fail_res = buildBattleEndResponse(user,mv);
        nh_->sendString(e.fd_, fail_res);
        return;
    }
    User * assistant = NULL;
    if(user->assistant_id_ != 0){
        assistant = dh_->getUser(user->assistant_id_);
        if(assistant == NULL){
            resetBattleInfo(user);
            return;
        }
    }
    Stage * stage = game_config.getStage(user->stage_type_, user->battle_chapter_id_, user->battle_stage_id_);
    if(stage == NULL){
        sendErrorResponse(e.fd_, CMD_BATTLEEND, ERROR_STAGE_NOT_EXIST);
        resetBattleInfo(user);
        return;
    }
    check = addAssistantReward(user,assistant);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_BATTLEEND, ERROR_SYSTEM_CRITICAL_ERROR);
        resetBattleInfo(user);
        return;
    }
    if(assistant!= NULL){
        LOG4CXX_INFO(logger_, "battlespark;"<<assistant->uid_<<";"<<assistant->spark_num_<<";"<<assistant->spark_daily_num_);
    }
    if (user->stage_type_ != STAGE_TYPE_TRIAL) {
        costEnergyForBattle(user, stage);
    }
    addBasicBattleReward(user, stage,user->stage_type_);
    setStageInfo(user);

    if(user->stage_type_ == STAGE_TYPE_ELITE){
        check = setEliteDaily(user, user->battle_chapter_id_, user->battle_stage_id_);
        if(check == false){
            sendErrorResponse(e.fd_, CMD_BATTLEEND, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    else if(user->stage_type_ == STAGE_TYPE_SPECIAL){
        check = setSpecialDaily(user, user->battle_chapter_id_, user->battle_stage_id_);
        if(check == false){
            sendErrorResponse(e.fd_, CMD_BATTLEEND, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    else if (user->stage_type_ == STAGE_TYPE_TRIAL) {
        //TODO
        check = setTrialDaily(user, user->battle_chapter_id_, user->battle_stage_id_);
        if (check == false) {
            sendErrorResponse(e.fd_, CMD_BATTLEEND, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    
    long long now = time(NULL);
    for(size_t i = 0;i<user->rewards_.size();i++){
        if(addStageReward(user, user->rewards_[i], now) == false){
            sendErrorResponse(e.fd_, CMD_BATTLEEND, ERROR_SYSTEM_CRITICAL_ERROR);
            resetBattleInfo(user);
            return;
        }
    }
    resetBattleInfo(user);
    check = dh_->saveUser(user);
    if(dh_->db_error_ != 0 || check == false){
        sendErrorResponse(e.fd_, CMD_BATTLEEND, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "battleend;"<<uid<<";"<<user->stage_type_<<";"<<user->battle_chapter_id_<<";"
                 <<user->battle_stage_id_<<";"<<user->gold_<<";"<<user->energy_<<";"<<user->user_level_<<";"
                 <<user->user_exp_<<";"<<user->spark_num_<<";"<<user->spark_daily_num_);

    updateMissionAfterBattle(user,mv);
    string res = buildBattleEndResponse(user,mv);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    
    return ;
}
void EventHandler::processFriendRequest(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size() != 4) return;
    uid_type fuid,tuid;
    bool check = safeAtoll(check_cmd[1], fuid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], tuid);
    if(check == false) return;
    if(fuid == tuid){
        sendErrorResponse(e.fd_, CMD_FRIENDREQUEST, ERROR_UID_CONFILICT);
        return;
    }
    User * fuser = safeGetUser(fuid, CMD_FRIENDREQUEST, e.fd_, check_cmd[2]);
    if(fuser == NULL) return;
    check = safeLoadFriend(fuser, CMD_FRIENDREQUEST, e.fd_);
    if(check == false) return;
    check = safeLoadFriendRequest(fuser, CMD_FRIENDREQUEST, e.fd_);
    if(check == false) return;
    check = safeLoadMission(fuser, CMD_FRIENDREQUEST, e.fd_);
    if(check == false) return;
    map<long long,FriendInfo *>::iterator it = fuser->friends_.find(tuid);
    if(it!=fuser->friends_.end()){
        sendErrorResponse(e.fd_, CMD_FRIENDREQUEST, ERROR_ALREADY_BEEN_FRIENDS);
        return;
    }
    User * tuser = dh_->getUser(tuid);
    if(dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_FRIENDREQUEST, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    if(tuser == NULL){
        sendErrorResponse(e.fd_, CMD_FRIENDREQUEST, ERROR_TARGET_USER_NOT_FOUND);
        return;
    }
    check = safeLoadFriend(tuser, CMD_FRIENDREQUEST, e.fd_);
    if(check == false) return;
    if(tuid == FAKE_USER_ID){
        check = dh_->addFriend(fuser,tuser);
        if(check == false || dh_->db_error_ != 0){
            sendErrorResponse(e.fd_, CMD_FRIENDREQUEST, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        check = dh_->addFriend(tuser,fuser);
        if(check == false || dh_->db_error_ != 0){
            sendErrorResponse(e.fd_, CMD_FRIENDREQUEST, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        FriendInfo * fr = safeGetFriendInfo(fuser, tuid, CMD_SENDENERGY, e.fd_);
        if(fr == NULL) return;
        long long now = time(NULL);
        fr->last_receive_time_ = now;
        fr->accepted_ = HAVE_ENERGY_TO_ACCEPT;
        check = dh_->saveFriendInfo(fr);
        if(check == false || dh_->db_error_ != 0){
            sendErrorResponse(e.fd_, CMD_FRIENDREQUEST,ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        string res = buildFriendRequestResponse();
        nh_->sendString(e.fd_, res);
        return;
    }
    map<long long,FriendRequestInfo *>::iterator frit = fuser->friend_requests_.find(tuid);
    if(frit!=fuser->friend_requests_.end()){
        sendErrorResponse(e.fd_, CMD_FRIENDREQUEST, ERROR_NO_NEED_TO_SEND_REQ);
        return;
    }

    check = safeLoadFriendRequest(tuser, CMD_FRIENDREQUEST, e.fd_);
    if(check == false) return;
    for(map<long long,FriendRequestInfo * >::iterator it = tuser->friend_requests_.begin();
        it!=tuser->friend_requests_.end();it++){
        if(it->second->fuid_ == fuid){
            sendErrorResponse(e.fd_, CMD_FRIENDREQUEST, ERROR_ALREADY_HAS_FRIEND_REQUEST);
            return;
        }
    }
    if((int) tuser->friend_requests_.size() >= game_config.friend_request_limit_){
        long long record_to_delete = 0;
        long long timecheck = time(NULL);
        long long uid_to_delete = 0;
        for(map<long long,FriendRequestInfo * >::iterator it = tuser->friend_requests_.begin();
            it!=tuser->friend_requests_.end();it++){
            if(it->second->send_time_ < timecheck){
                timecheck = it->second->send_time_;
                record_to_delete = it->second->record_id_;
                uid_to_delete = it->second->fuid_;
            }
        }
        check = dh_->deleteFriendRequest(record_to_delete);
        if(dh_->db_error_ != 0 || check == false){
            sendErrorResponse(e.fd_, CMD_FRIENDREQUEST, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        tuser->friend_requests_.erase(uid_to_delete);
    }
    check = dh_->addFriendRequest(tuser, tuid, fuid);
    if(dh_->db_error_ !=0 || check == false){
        sendErrorResponse(e.fd_, CMD_FRIENDREQUEST, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "friendrequest;"<<fuid<<";"<<tuid);
    string res = buildFriendRequestResponse();
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(fuser);
    return;
    
}
void EventHandler::processSellHero(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4)return;
    uid_type uid;
    vector<long long> hero_id;
    string delems = ",";
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = longtokenize(check_cmd[3], hero_id, delems);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_SELLHERO, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadHero(user,CMD_SELLHERO, e.fd_);
    if(check == false) return;
    vector<Hero *> to_sell;
    to_sell.clear();
    for(size_t i = 0;i<hero_id.size();i++){
        map<long long ,Hero * >::iterator it = user->heroes_.find(hero_id[i]);
        if(it == user->heroes_.end()){
            sendErrorResponse(e.fd_, CMD_SELLHERO, ERROR_HERO_NOT_FOUND);
            return;
        }
        if(it->second->hero_id_ == user->hero_id_){
            sendErrorResponse(e.fd_, CMD_SELLHERO, ERROR_HERO_IS_IN_ARMY);
            return;
        }
        if(it->second->locked_ == IS_LOCKED){
            sendErrorResponse(e.fd_, CMD_SELLHERO, ERROR_HERO_IS_LOCKED);
            return;
        }
        if(inTeamHero(user, it->second->hero_id_) != NULL){
            sendErrorResponse(e.fd_, CMD_SELLHERO, ERROR_HERO_IS_IN_ARMY);
            return;
        }
        to_sell.push_back(it->second);
    }
    for(size_t i = 0;i<to_sell.size();i++){
        HeroConfig * hc = game_config.getHeroConfig(to_sell[i]->mid_, to_sell[i]->star_);
        if(hc == NULL)continue;
        
        int exps = game_config.getValueFromIntVectorFilledZero(to_sell[i]->level_, game_config.hero_exp_sum_[to_sell[i]->star_]);
        if(exps == -1){
            sendErrorResponse(e.fd_, CMD_SELLHERO, ERROR_CONFIG_ERROR);
            return;
        }
        //int sgold = int((int)(exps * game_config.enhance_exp_times_) * game_config.exp_to_gold_rate_hero_);
        int sgold = hc->price_ + int(hc->price_plus_ * (to_sell[i]->level_-1));
        
        user->gold_ += sgold;
        check = dh_->deleteHero(to_sell[i]->hero_id_);
        if(check == false||dh_->db_error_ !=0){
            sendErrorResponse(e.fd_, CMD_SELLHERO, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        user->heroes_.erase(to_sell[i]->hero_id_);
        LOG4CXX_INFO(logger_, "sellhero;"<<uid<<";"<<to_sell[i]->hero_id_<<";"
                     <<to_sell[i]->mid_<<";"<<to_sell[i]->star_<<";"<<to_sell[i]->level_<<";"
                     <<user->gold_);
        delete to_sell[i];
    }
    user->dirty_ = true;
    check = dh_->saveUser(user);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_SELLHERO, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    
    string res = buildSellHeroResponse(user);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processSellSoldier(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4)return;
    uid_type uid;
    vector<long long> soldier_id;
    string delems = ",";
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = longtokenize(check_cmd[3], soldier_id, delems);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_SELLSOLDIER, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadSoldier(user, CMD_SELLSOLDIER, e.fd_);
    if(check == false) return;
    vector<Soldier *> to_sell;
    to_sell.clear();
    for(size_t i = 0;i<soldier_id.size();i++){
        map<long long ,Soldier * >::iterator it = user->soldiers_.find(soldier_id[i]);
        if(it == user->soldiers_.end()){
            sendErrorResponse(e.fd_, CMD_SELLSOLDIER, ERROR_SOLDIER_NOT_FOUND);
            return;
        }
        if(it->second->locked_ == IS_LOCKED){
            sendErrorResponse(e.fd_, CMD_SELLSOLDIER, ERROR_SOLDIER_IS_LOCKED);
            return;
        }
        for(size_t j = 0;j<user->soldier_id_.size();j++){
            if(user->soldier_id_[j] == soldier_id[i]){
                sendErrorResponse(e.fd_, CMD_SELLSOLDIER, ERROR_SOLDIER_IS_IN_ARMY);
                return;
            }
        }
        if(inTeamSoldier(user, it->second->soldier_id_) != NULL){
            sendErrorResponse(e.fd_, CMD_SELLSOLDIER, ERROR_SOLDIER_IS_IN_ARMY);
            return;
        }
        to_sell.push_back(it->second);
    }
    for(size_t i = 0;i<to_sell.size();i++){
        SoldierConfig * sc = game_config.getSoldierConfig(to_sell[i]->mid_, to_sell[i]->star_);
        if(sc == NULL)continue;
        
        int exps = game_config.getValueFromIntVectorFilledZero(to_sell[i]->level_, game_config.soldier_exp_sum_[to_sell[i]->star_]);
        if(exps == -1){
            sendErrorResponse(e.fd_, CMD_SELLSOLDIER, ERROR_CONFIG_ERROR);
            return;
        }
        
        //int sgold = int((int)(exps * game_config.enhance_exp_times_) * game_config.exp_to_gold_rate_soldier_);
        int sgold = int(sc->price_ + sc->price_plus_* (to_sell[i]->level_-1));
        
        user->gold_ += sgold;
        check = dh_->deleteSoldier(to_sell[i]->soldier_id_);
        if(check == false||dh_->db_error_ !=0){
            sendErrorResponse(e.fd_, CMD_SELLSOLDIER, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        user->soldiers_.erase(to_sell[i]->soldier_id_);
        LOG4CXX_INFO(logger_, "sellsoldier;"<<uid<<";"<<to_sell[i]->soldier_id_<<";"
                     <<to_sell[i]->mid_<<";"<<to_sell[i]->star_<<";"<<to_sell[i]->level_<<";"
                     <<user->gold_);

        delete to_sell[i];
    }
    user->dirty_ = true;
    check = dh_->saveUser(user);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_SELLSOLDIER, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildSellSoldierResponse(user);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processLockHero(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=5) return;
    uid_type uid;
    long long hero_id;
    int op_type;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], hero_id);
    if(check == false) return;
    check = safeAtoi(check_cmd[4], op_type);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_LOCKHERO, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadHero(user, CMD_LOCKHERO, e.fd_);
    if(check == false) return;
    map<long long,Hero *>::iterator it = user->heroes_.find(hero_id);
    if(it == user->heroes_.end()){
        sendErrorResponse(e.fd_, CMD_LOCKHERO, ERROR_HERO_NOT_FOUND);
        return;
    }
    it->second->locked_ = op_type;
    check = dh_->saveHero(it->second);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_LOCKHERO, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildLockHeroResponse();
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processLockSoldier(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=5) return;
    uid_type uid;
    long long soldier_id;
    int op_type;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], soldier_id);
    if(check == false) return;
    check = safeAtoi(check_cmd[4], op_type);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_LOCKSOLDIER, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadSoldier(user, CMD_LOCKSOLDIER, e.fd_);
    if(check == false) return;
    map<long long,Soldier *>::iterator it = user->soldiers_.find(soldier_id);
    if(it == user->soldiers_.end()){
        sendErrorResponse(e.fd_, CMD_LOCKSOLDIER, ERROR_SOLDIER_NOT_FOUND);
        return;
    }
    it->second->locked_ = op_type;
    check = dh_->saveSoldier(it->second);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_LOCKSOLDIER, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildLockSoldierResponse();
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processHeroExpUp(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=5)return;
    uid_type uid;
    long long target_hero_id;
    vector<long long> hero_list;
    string delim = ",";
    hero_list.clear();
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], target_hero_id);
    if(check == false) return;
    check = longtokenize(check_cmd[4], hero_list, delim);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_HEROEXPUP, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadHero(user, CMD_HEROEXPUP, e.fd_);
    if(check == false) return;
    map<long long,Hero *>::iterator hero_it = user->heroes_.find(target_hero_id);
    if(hero_it == user->heroes_.end()){
        sendErrorResponse(e.fd_, CMD_HEROEXPUP, ERROR_HERO_NOT_FOUND);
        return;
    }
    Hero * hero = hero_it->second;
    HeroConfig * hc  = game_config.getHeroConfig(hero->mid_, hero->star_);
    if(hc == NULL){
        sendErrorResponse(e.fd_, CMD_HEROEXPUP, ERROR_CONFIG_ERROR);
        return;
    }
    if(hc->fake_exp_!=0){
        sendErrorResponse(e.fd_, CMD_HEROEXPUP, ERROR_CONFIG_ERROR);
        return;
    }
    vector<Hero * > to_delete;
    to_delete.clear();
    int exp_sum = 0;
    int exp_fake_sum = 0;
    int sgold_cost = 0;
    for(size_t i = 0;i<hero_list.size();i++){
        map<long long ,Hero *>::iterator it = user->heroes_.find(hero_list[i]);
        if(it == user->heroes_.end()){
            sendErrorResponse(e.fd_, CMD_HEROEXPUP, ERROR_HERO_NOT_FOUND);
            return;
        }
        if(it->second->locked_ == IS_LOCKED){
            sendErrorResponse(e.fd_, CMD_HEROEXPUP, ERROR_SOLDIER_IS_LOCKED);
            return;
        }
        if(it->second->hero_id_ == user->hero_id_){
            sendErrorResponse(e.fd_, CMD_HEROEXPUP, ERROR_HERO_IS_IN_ARMY);
            return;
        }
        if(it->second->hero_id_ == hero->hero_id_){
            sendErrorResponse(e.fd_, CMD_HEROEXPUP, ERROR_UID_CONFILICT);
            return;
        }
        to_delete.push_back(it->second);
        int exps = 0;
        exps = game_config.getValueFromIntVectorFilledZero(it->second->level_, game_config.hero_exp_sum_[it->second->star_]);
        if(exps == -1){
            sendErrorResponse(e.fd_, CMD_HEROEXPUP, ERROR_CONFIG_ERROR);
            return;
        }
        hc = game_config.getHeroConfig(it->second->mid_, it->second->star_);
        if(hc == NULL){
            sendErrorResponse(e.fd_, CMD_HEROEXPUP, ERROR_CONFIG_ERROR);
            return;
        }
        if(hc->fake_exp_ == 0){
            exp_sum += int(exps * game_config.enhance_exp_times_);
            sgold_cost += int(((int)(exps * game_config.enhance_exp_times_)) * game_config.exp_to_gold_rate_hero_);
        }else{
            exp_fake_sum += hc->fake_exp_;
            sgold_cost += int(hc->fake_exp_ * game_config.exp_to_gold_rate_hero_);
        }
         
    }
    /*int sgold_cost = game_config.getValueFromIntVectorFilledZero(hero->level_, game_config.hero_level_up_gold_cost_);
    if(sgold_cost == -1){
        sendErrorResponse(e.fd_, CMD_HEROEXPUP, ERROR_CONFIG_ERROR);
        return;
    }
    if(sgold_cost * to_delete() > user->gold_){
        sendErrorResponse(e.fd_, CMD_HEROEXPUP, ERROR_GOLD_NOT_ENOUGH);
        return;
    }*/

    if(sgold_cost> user->gold_){
        sendErrorResponse(e.fd_, CMD_HEROEXPUP, ERROR_GOLD_NOT_ENOUGH);
        return;
    }

    for(size_t i =0;i<to_delete.size();i++){
        check = dh_->deleteHero(to_delete[i]->hero_id_);
        if(check == false||dh_->db_error_ !=0){
            sendErrorResponse(e.fd_, CMD_HEROEXPUP, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        user->heroes_.erase(to_delete[i]->hero_id_);
        LOG4CXX_INFO(logger_, "heroexpupcost;"<<uid<<";"<<to_delete[i]->hero_id_<<";"
                     <<to_delete[i]->mid_<<";"<<to_delete[i]->star_<<";"<<to_delete[i]->level_);
        delete to_delete[i];
    }
    hero->exp_ += exp_sum;
    hero->exp_ += exp_fake_sum;
    while(hero->exp_ >= game_config.getValueFromIntVectorFilledZero(hero->level_, game_config.hero_level_up_exp_[hero->star_])){
        if(hero->level_ >= (int) game_config.hero_level_up_exp_[hero->star_].size()-1
           || hero->level_ >= game_config.hero_config_[hero->mid_][hero->star_].level_limit_){
            hero->exp_ = 0;
            break;
        }else{
            hero->exp_ -= game_config.hero_level_up_exp_[hero->star_][hero->level_];
            hero->level_ ++;
        }
    }
    user->gold_-= sgold_cost;
    check = dh_->saveHero(hero);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_HEROEXPUP, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    check = dh_->saveUser(user);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_HEROEXPUP, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "heroexpup;"<<uid<<";"<<user->gold_<<";"<<hero->mid_<<";"<<hero->mid_<<";"
                 <<hero->star_<<";"<<hero->level_<<";"<<hero->exp_);
    string res = buildHeroExpUpResponse(user, hero);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    if(hero->star_ == NEWS_MAX_STAR & hero->level_ == (int)game_config.hero_level_up_exp_[hero->star_].size()-1){
        dh_->addNews(user->nick_name_, NEWS_TYPE_EXPUP, ITEM_TYPE_HERO, hero->mid_, hero->star_);
    }
    return;
}
void EventHandler::processSoldierExpUp(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=5)return;
    uid_type uid;
    long long target_soldier_id;
    vector<long long> soldier_list;
    string delim = ",";
    soldier_list.clear();
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], target_soldier_id);
    if(check == false) return;
    check = longtokenize(check_cmd[4], soldier_list, delim);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_SOLDIEREXPUP, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadSoldier(user, CMD_SOLDIEREXPUP, e.fd_);
    if(check == false) return;
    map<long long,Soldier * >::iterator soldier_it = user->soldiers_.find(target_soldier_id);
    if(soldier_it == user->soldiers_.end()){
        sendErrorResponse(e.fd_, CMD_SOLDIEREXPUP, ERROR_SOLDIER_NOT_FOUND);
        return;
    }
    Soldier * soldier = soldier_it->second;
    SoldierConfig * sc  = game_config.getSoldierConfig(soldier->mid_, soldier->star_);
    if(sc == NULL){
        sendErrorResponse(e.fd_, CMD_SOLDIEREXPUP, ERROR_CONFIG_ERROR);
        return;
    }
    if(sc->fake_exp_!=0){
        sendErrorResponse(e.fd_, CMD_SOLDIEREXPUP, ERROR_CONFIG_ERROR);
        return;
    }
    vector<Soldier * > to_delete;
    to_delete.clear();
    int exp_sum = 0;
    int exp_fake_sum = 0;
    int sgold_cost = 0;
    for(size_t i = 0;i<soldier_list.size();i++){
        map<long long ,Soldier *>::iterator it = user->soldiers_.find(soldier_list[i]);
        if(it == user->soldiers_.end()){
            sendErrorResponse(e.fd_, CMD_SOLDIEREXPUP, ERROR_SOLDIER_NOT_FOUND);
            return;
        }
        if(it->second->locked_ == IS_LOCKED){
            sendErrorResponse(e.fd_, CMD_SOLDIEREXPUP, ERROR_SOLDIER_IS_LOCKED);
            return;
        }
        if(it->second->soldier_id_ == soldier->soldier_id_){
            sendErrorResponse(e.fd_, CMD_SOLDIEREXPUP, ERROR_UID_CONFILICT);
            return;
        }
        for(size_t j = 0;j<user->soldier_id_.size();j++){
            if(user->soldier_id_[j] == soldier_list[i]){
                sendErrorResponse(e.fd_, CMD_SOLDIEREXPUP, ERROR_SOLDIER_IS_IN_ARMY);
                return;
            }
        }
        to_delete.push_back(it->second);
        int exps = game_config.getValueFromIntVectorFilledZero(it->second->level_, game_config.soldier_exp_sum_[it->second->star_]);
        if(exps == -1){
            sendErrorResponse(e.fd_, CMD_SOLDIEREXPUP, ERROR_CONFIG_ERROR);
            return;
        }
        sc = game_config.getSoldierConfig(it->second->mid_, it->second->star_);
        if(sc == NULL){
            sendErrorResponse(e.fd_, CMD_SOLDIEREXPUP, ERROR_CONFIG_ERROR);
            return;
        }
        if(sc->fake_exp_ == 0){
            exp_sum += int(exps* game_config.enhance_exp_times_);
            sgold_cost += int((int)(exps * game_config.enhance_exp_times_) * game_config.exp_to_gold_rate_soldier_);
        }else{
            exp_fake_sum += sc->fake_exp_;
            sgold_cost += int(sc->fake_exp_ * game_config.exp_to_gold_rate_soldier_);
        }
    }
    if(sgold_cost> user->gold_){
        sendErrorResponse(e.fd_, CMD_SOLDIEREXPUP, ERROR_GOLD_NOT_ENOUGH);
        return;
    }
    for(size_t i =0;i<to_delete.size();i++){

        check = dh_->deleteSoldier(to_delete[i]->soldier_id_);
        if(check == false||dh_->db_error_ !=0){
            sendErrorResponse(e.fd_, CMD_SOLDIEREXPUP, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        user->soldiers_.erase(to_delete[i]->soldier_id_);
        LOG4CXX_INFO(logger_, "soldierexpupcost;"<<uid<<";"<<to_delete[i]->soldier_id_<<";"
                     <<to_delete[i]->mid_<<";"<<to_delete[i]->star_<<";"<<to_delete[i]->level_);
        delete to_delete[i];
    }
    soldier->exp_ += exp_sum;
    soldier->exp_ += exp_fake_sum;
    while(soldier->exp_ >= game_config.getValueFromIntVectorFilledZero(soldier->level_, game_config.soldier_level_up_exp_[soldier->star_])){
        if(soldier->level_ >= (int)game_config.soldier_level_up_exp_[soldier->star_].size()-1
           || soldier->level_ >= game_config.soldier_config_[soldier->mid_][soldier->star_].level_limit_){
            soldier->exp_ = 0;
            break;
        }else{
            soldier->exp_ -= game_config.soldier_level_up_exp_[soldier->star_][soldier->level_];
            soldier->level_ ++;
        }
    }
    user->gold_-= sgold_cost;
    check = dh_->saveSoldier(soldier);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_SOLDIEREXPUP, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    check = dh_->saveUser(user);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_SOLDIEREXPUP, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "soldierexpup;"<<uid<<";"<<user->gold_<<";"<<soldier->mid_<<";"<<soldier->mid_<<";"
                 <<soldier->star_<<";"<<soldier->level_<<";"<<soldier->exp_);
    string res = buildSoldierExpUpResponse(user, soldier);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    if(soldier->star_ == NEWS_MAX_STAR & soldier->level_ == (int) game_config.soldier_level_up_exp_[soldier->star_].size()-1){
        dh_->addNews(user->nick_name_, NEWS_TYPE_EXPUP, ITEM_TYPE_HERO, soldier->mid_, soldier->star_);
    }
    return;

}
void EventHandler::processHeroEnhance(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size() != 6) return;
    string delims = ",";
    uid_type uid;
    long long hero_id;
    vector<int> position;
    vector<long long> hero_to_cost;
    bool hc_empty = false;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], hero_id);
    if(check == false) return;
    check = numtokenize(check_cmd[4], position, delims);
    if(check == false) return;
    check = longtokenize(check_cmd[5], hero_to_cost, delims);
    if(check == false) return;
    if(position.size()!=hero_to_cost.size())return;
    if(position.size() == 0 || hero_to_cost.size() == 0)return;
    if(position[0]==0 || hero_to_cost[0]==0){
        hc_empty = true;
    }
    User * user = safeGetUser(uid, CMD_HEROENHANCE, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadHero(user, CMD_HEROENHANCE, e.fd_);
    if(check == false) return;
    check = safeLoadSoldier(user, CMD_HEROENHANCE, e.fd_);
    if(check == false) return;
    check = safeLoadItem(user, CMD_HEROENHANCE, e.fd_);
    if(check == false) return;
    Hero * hero = safeGetHero(user, hero_id, CMD_HEROENHANCE, e.fd_);
    if(hero == NULL) return;
    HeroConfig * hcc = game_config.getHeroConfig(hero->mid_, hero->star_);
    if(hcc == NULL){
        sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_CONFIG_ERROR);
        return;
    }
    if(hcc->fake_exp_ != 0){
        sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_CONFIG_ERROR);
        return;
    }
    if(hcc->level_limit_ != hero->level_){
        sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_LEVEL_NOT_ENOUGH);
        return;
    }
    if(user->hero_id_ == hero->hero_id_){
        hero->star_ ++;
        check = preCheckLeadershipByChangeHero(user, hero);
        hero->star_ --;
        if(check == false){
            sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_LEADERSHIP_NOT_ENOUGH);
            return;
        }
    }
    
    Team * team = inTeamHero(user, hero_id);
    if(team != NULL){
        Hero * pc_hero = NULL;
        vector<Soldier *> pc_soldiers;
        pc_soldiers.clear();
        check = teamPrepare(user, team->hero_id_, team->soldier_id_, pc_hero, pc_soldiers);
        if(check == false) {
            sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_SOLDIER_NOT_FOUND);
            return;
        }
        hero->star_ ++;
        check = preCheckLeadership(user, pc_hero, pc_soldiers);
        hero->star_ --;
        if(check == false){
            sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_LEADERSHIP_NOT_ENOUGH);
            return;
        }

    }
    
    vector<EnhanceCost> &hc = game_config.hero_enhance_[hero->star_];
    for(size_t i = 0;i<hc.size();i++){
        if(hc[i].param2 != hcc->type_)continue;
        if(hc[i].type == ITEM_TYPE_HERO){
            if(hc_empty == true){
                sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_CONFIG_ERROR);
                return;
            }
            check = false;
            int position_index = 0;
            for(size_t j = 0;j<position.size();j++){
                if(position[j] == hc[i].position){
                    check = true;
                    position_index = j;
                    break;
                }
            }
            if(check == false){
                sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_POSITION_ERROR);
                return;
            }
            Hero * hero_cost = safeGetHero(user, hero_to_cost[position_index], CMD_HEROENHANCE, e.fd_);
            if(hero_cost == NULL)return;
            if(hero_cost->locked_ == IS_LOCKED){
                sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_HERO_IS_LOCKED);
                return;
            }
            if(hero_cost->hero_id_ == hero->hero_id_){
                sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_UID_CONFILICT);
                return;
            }
            if(hero_cost->hero_id_ == user->hero_id_){
                sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_HERO_IS_IN_ARMY);
                return;
            }
            if(hero_cost->mid_ != hero->mid_ || hero_cost->star_!=hc[i].param){
                sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_CANNOT_PAY_COST);
                return;
            }
        }else if(hc[i].type == ITEM_TYPE_RESOURCE ||hc[i].type == ITEM_TYPE_GOLD){
            check = checkCost(user, hc[i]);
            if(check == false){
                sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_CANNOT_PAY_COST);
                return;
            }
        }
    }
    for(size_t i = 0;i<hc.size();i++){
        if(hc[i].param2 != hcc->type_)continue;
        if(hc[i].type == ITEM_TYPE_HERO){
            if(hc_empty == true){
                sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_CONFIG_ERROR);
                return;
            }
            check = false;
            int position_index = 0;
            for(size_t j = 0;j<position.size();j++){
                if(position[j] == hc[i].position){
                    check = true;
                    position_index = j;
                    break;
                }
            }
            if(check == false){
                sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_POSITION_ERROR);
                return;
            }
            Hero * hero_cost = safeGetHero(user, hero_to_cost[position_index], CMD_HEROENHANCE, e.fd_);
            if(hero_cost == NULL)return;
            check = dh_->deleteHero(hero_cost->hero_id_);
            if(check == false || dh_->db_error_ != 0){
                sendErrorResponse(e.fd_, CMD_HEROENHANCE,ERROR_SYSTEM_CRITICAL_ERROR);
                return;
            }
            LOG4CXX_INFO(logger_, "heroenhancecost;"<<uid<<";"<<hero_cost->hero_id_<<";"
                         <<hero_cost->mid_<<";"<<hero_cost->star_<<";"<<hero_cost->level_);
            delete hero_cost;
            user->heroes_.erase(hero_to_cost[position_index]);
        }else if(hc[i].type == ITEM_TYPE_RESOURCE
                 ||hc[i].type == ITEM_TYPE_GOLD){
            check = safeCost(user, hc[i], CMD_HEROENHANCE, e.fd_);
            if(check == false) return;
            Item * item = safeGetItem(user, hc[i].type, hc[i].mid);
            if(item != NULL){
                LOG4CXX_INFO(logger_, "heroenhanceitem;"<<uid<<";"<<hc[i].mid<<";"<<item->amount_);
            }
            
        }
    }
    hero->star_ ++;
    check = dh_->saveHero(hero);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_HEROENHANCE,ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "heroenhance;"<<uid<<";"<<user->gold_<<";"<<hero->hero_id_<<";"<<hero->mid_<<";"
                 <<hero->star_<<";"<<hero->level_<<";"<<hero->exp_);
    string res = buildHeroEnhanceResponse(user, hero);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    if(hero->star_ == NEWS_MAX_STAR){
        dh_->addNews(user->nick_name_, NEWS_TYPE_ENHANCE, ITEM_TYPE_HERO, hero->mid_, hero->star_);
    }
    return;
}
void EventHandler::processSoldierEnhance(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size() != 6) return;
    string delims = ",";
    uid_type uid;
    long long soldier_id;
    vector<int> position;
    vector<long long> soldier_to_cost;
    bool sc_empty = false;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], soldier_id);
    if(check == false) return;
    check = numtokenize(check_cmd[4], position, delims);
    if(check == false) return;
    check = longtokenize(check_cmd[5], soldier_to_cost, delims);
    if(check == false) return;
    if(position.size()!=soldier_to_cost.size())return;
    if(position.size() == 0 || soldier_to_cost.size() == 0)return;
    if(position[0]==0 || soldier_to_cost[0]==0){
        sc_empty = true;
    }
    User * user = safeGetUser(uid, CMD_SOLDIERENHANCE, e.fd_ , check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadSoldier(user, CMD_SOLDIERENHANCE, e.fd_);
    if(check == false) return;
    check = safeLoadItem(user, CMD_SOLDIERENHANCE, e.fd_);
    if(check == false) return;
    Soldier * soldier = safeGetSoldier(user, soldier_id, CMD_SOLDIERENHANCE, e.fd_);
    if(soldier == NULL) return;
    SoldierConfig * scc = game_config.getSoldierConfig(soldier->mid_, soldier->star_);
    if(scc == NULL){
        sendErrorResponse(e.fd_, CMD_SOLDIERENHANCE, ERROR_CONFIG_ERROR);
        return;
    }
    if(scc->fake_exp_ != 0){
        sendErrorResponse(e.fd_, CMD_SOLDIERENHANCE, ERROR_CONFIG_ERROR);
        return;
    }
    if(scc->level_limit_ != soldier->level_){
        sendErrorResponse(e.fd_, CMD_SOLDIERENHANCE, ERROR_LEVEL_NOT_ENOUGH);
        return;
    }
    for(size_t i = 0;i<user->soldier_id_.size();i++){
        if(user->soldier_id_[i] == soldier->soldier_id_){
            soldier->star_++;
            check = preCheckLeadershipByChangeSoldier(user, soldier, i);
            soldier->star_--;
            if(check == false){
                sendErrorResponse(e.fd_, CMD_SOLDIERENHANCE, ERROR_LEADERSHIP_NOT_ENOUGH);
                return;
            }
        }
    }
    Team * team = inTeamSoldier(user, soldier_id);
    if(team != NULL){
        Hero * pc_hero = NULL;
        vector<Soldier *> pc_soldiers;
        pc_soldiers.clear();
        check = teamPrepare(user, team->hero_id_, team->soldier_id_, pc_hero, pc_soldiers);
        if(check == false) {
            sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_SOLDIER_NOT_FOUND);
            return;
        }
        soldier->star_ ++;
        check = preCheckLeadership(user, pc_hero, pc_soldiers);
        soldier->star_ --;
        if(check == false){
            sendErrorResponse(e.fd_, CMD_HEROENHANCE, ERROR_LEADERSHIP_NOT_ENOUGH);
            return;
        }
        
    }
    vector<EnhanceCost> &sc = game_config.soldier_enhance_[soldier->star_];
    for(size_t i = 0;i<sc.size();i++){
        if(sc[i].param2 != scc->type_)continue;
        if(sc[i].type == ITEM_TYPE_SOLDIER){
            if(sc_empty == true){
                sendErrorResponse(e.fd_, CMD_SOLDIERENHANCE, ERROR_CONFIG_ERROR);
                return;
            }
            check = false;
            int position_index = 0;
            for(size_t j = 0;j<position.size();j++){
                if(position[j] == sc[i].position){
                    check = true;
                    position_index = j;
                    break;
                }
            }
            if(check == false){
                sendErrorResponse(e.fd_, CMD_SOLDIERENHANCE, ERROR_POSITION_ERROR);
                return;
            }
            Soldier * soldier_cost = safeGetSoldier(user, soldier_to_cost[position_index], CMD_SOLDIERENHANCE, e.fd_);
            if(soldier_cost == NULL)return;
            if(soldier_cost->soldier_id_ == soldier->soldier_id_){
                sendErrorResponse(e.fd_, CMD_SOLDIERENHANCE, ERROR_UID_CONFILICT);
                return;
            }
            if(soldier_cost->locked_ == IS_LOCKED){
                sendErrorResponse(e.fd_, CMD_SOLDIERENHANCE, ERROR_SOLDIER_IS_LOCKED);
                return;
            }
            for(size_t k = 0;k<user->soldier_id_.size();k++){
                if(soldier_cost->soldier_id_ == user->soldier_id_[k]){
                    sendErrorResponse(e.fd_, CMD_SOLDIERENHANCE, ERROR_SOLDIER_IS_IN_ARMY);
                    return;
                }
            }
            if(soldier_cost->mid_ != soldier->mid_ || soldier_cost->star_!=sc[i].param){
                sendErrorResponse(e.fd_, CMD_SOLDIERENHANCE, ERROR_CANNOT_PAY_COST);
                return;
            }
        }else if(sc[i].type == ITEM_TYPE_RESOURCE ||sc[i].type == ITEM_TYPE_GOLD){
            check = checkCost(user, sc[i]);
            if(check == false){
                sendErrorResponse(e.fd_, CMD_SOLDIERENHANCE, ERROR_CANNOT_PAY_COST);
                return;
            }
        }
    }
    for(size_t i = 0;i<sc.size();i++){
        if(sc[i].param2 != scc->type_)continue;
        if(sc[i].type == ITEM_TYPE_SOLDIER){
            if(sc_empty == true){
                sendErrorResponse(e.fd_, CMD_SOLDIERENHANCE, ERROR_CONFIG_ERROR);
                return;
            }
            check = false;
            int position_index = 0;
            for(size_t j = 0;j<position.size();j++){
                if(position[j] == sc[i].position){
                    check = true;
                    position_index = j;
                    break;
                }
            }
            if(check == false){
                sendErrorResponse(e.fd_, CMD_SOLDIERENHANCE, ERROR_POSITION_ERROR);
                return;
            }
            Soldier * soldier_cost = safeGetSoldier(user, soldier_to_cost[position_index], CMD_SOLDIERENHANCE, e.fd_);
            if(soldier_cost == NULL)return;
            check = dh_->deleteSoldier(soldier_cost->soldier_id_);
            if(check == false || dh_->db_error_ != 0){
                sendErrorResponse(e.fd_, CMD_SOLDIERENHANCE,ERROR_SYSTEM_CRITICAL_ERROR);
                return;
            }
            LOG4CXX_INFO(logger_, "soldierenhancecost;"<<uid<<";"<<soldier_cost->soldier_id_<<";"
                         <<soldier_cost->mid_<<";"<<soldier_cost->star_<<";"<<soldier_cost->level_);
            delete soldier_cost;
            user->soldiers_.erase(soldier_to_cost[position_index]);
        }else if(sc[i].type == ITEM_TYPE_RESOURCE
                 ||sc[i].type == ITEM_TYPE_GOLD){
            check = safeCost(user, sc[i], CMD_SOLDIERENHANCE, e.fd_);
            if(check == false) return;
            Item * item = safeGetItem(user, sc[i].type, sc[i].mid);
            if(item != NULL){
                LOG4CXX_INFO(logger_, "soldierenhanceitem;"<<uid<<";"<<sc[i].mid<<";"<<item->amount_);
            }
        }
    }
    soldier->star_ ++;
    check = dh_->saveSoldier(soldier);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_SOLDIERENHANCE,ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "soldierenhance;"<<uid<<";"<<user->gold_<<";"<<";"<<soldier->soldier_id_<<";"<<soldier->mid_<<";"
                 <<soldier->star_<<";"<<soldier->level_<<";"<<soldier->exp_);
    string res = buildSoldierEnhanceResponse(user, soldier);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    if(soldier->star_ == NEWS_MAX_STAR){
        dh_->addNews(user->nick_name_, NEWS_TYPE_ENHANCE, ITEM_TYPE_SOLDIER, soldier->mid_, soldier->star_);
    }
    return;
}
void EventHandler::processSkillLevelUp(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=5) return ;
    uid_type uid;
    long long hero_id;
    int position;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], hero_id);
    if(check == false) return;
    check = safeAtoi(check_cmd[4], position);
    if(check == false) return;
    if(position<=0 || position>MAX_SKILL_POSITION) return;
    User * user = safeGetUser(uid, CMD_SKILLLEVELUP, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadHero(user,CMD_SKILLLEVELUP, e.fd_);
    if(check == false) return;
    Hero * hero = safeGetHero(user, hero_id, CMD_SKILLLEVELUP, e.fd_);
    if(hero == NULL)return;
    check = safeLoadItem(user, CMD_SKILLLEVELUP, e.fd_);
    if(check == false) return;
    /*int skill_id = game_config.getSkillIdByPosition(hero->mid_, position);
    if(skill_id<=0){
        sendErrorResponse(e.fd_, CMD_SKILLLEVELUP, ERROR_CONFIG_ERROR);
        return;
    }
    map<int, vector<vector<EnhanceCost> > >::iterator it = game_config.skill_level_up_cost_.find(skill_id);
    if(it==game_config.skill_level_up_cost_.end()){
        sendErrorResponse(e.fd_, CMD_SKILLLEVELUP, ERROR_CONFIG_ERROR);
        return;
    }
    int lv = hero->skill_[position-1];
    if(lv >= (it->second).size()){
        sendErrorResponse(e.fd_, CMD_SKILLLEVELUP, ERROR_CONFIG_ERROR);
        return;
    }
    for(size_t i = 0;i<(it->second)[lv].size();i++){
        if((it->second)[lv][i].type == ITEM_TYPE_RESOURCE ||(it->second)[lv][i].type == ITEM_TYPE_GOLD){
            check = checkCost(user, (it->second)[lv][i]);
            if(check == false){
                sendErrorResponse(e.fd_, CMD_SKILLLEVELUP, ERROR_CANNOT_PAY_COST);
                return;
            }
        }
    }
    for(size_t i = 0;i<(it->second)[lv].size();i++){
        if((it->second)[lv][i].type == ITEM_TYPE_RESOURCE
           ||(it->second)[lv][i].type == ITEM_TYPE_GOLD){
            check = safeCost(user, (it->second)[lv][i], CMD_SKILLLEVELUP, e.fd_);
            if(check == false) return;
        }
    }*/
    if(hero->skill_[position-1] >= (int) game_config.skill_level_up_cost_[position].size())return;
    vector<EnhanceCost> &evc = game_config.skill_level_up_cost_[position][hero->skill_[position-1]];
    for(size_t i = 0;i<evc.size();i++){
        if(evc[i].type == ITEM_TYPE_RESOURCE ||evc[i].type == ITEM_TYPE_GOLD){
            check = checkCost(user, evc[i]);
            if(check == false){
                sendErrorResponse(e.fd_, CMD_SKILLLEVELUP, ERROR_CANNOT_PAY_COST);
                return;
            }
        }
    }
    for(size_t i = 0;i<evc.size();i++){
        if(evc[i].type == ITEM_TYPE_RESOURCE
           ||evc[i].type == ITEM_TYPE_GOLD){
            check = safeCost(user, evc[i], CMD_SKILLLEVELUP, e.fd_);
            if(check == false) return;
            if(evc[i].type == ITEM_TYPE_RESOURCE){
                Item * item = safeGetItem(user, evc[i].type, evc[i].mid);
                if(item != NULL){
                    LOG4CXX_INFO(logger_, "skillupitem;"<<uid<<";"<<evc[i].mid<<";"<<item->amount_);
                }
            }
        }
    }
    hero->skill_[position-1] ++;
    check = dh_->saveHero(hero);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_SKILLLEVELUP,ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "skillup;"<<uid<<";"<<hero->mid_<<";"<<position<<";"<<hero->skill_[position-1]);
    string res = buildSkillLevelUpResponse(user, hero, position);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processSendEnergy(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4)return;
    uid_type uid;
    uid_type fuid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false)return;
    check = safeAtoll(check_cmd[3], fuid);
    if(check == false)return;
    User * user = safeGetUser(uid, CMD_SENDENERGY, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadFriend(user, CMD_SENDENERGY, e.fd_);
    if(check == false)return;
    check = safeLoadMission(user, CMD_SENDENERGY, e.fd_);
    if(check == false) return;
    User * fuser = dh_->getUser(fuid);
    if(dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_SENDENERGY, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    if(fuser == NULL){
        sendErrorResponse(e.fd_, CMD_SENDENERGY, ERROR_TARGET_USER_NOT_FOUND);
        return;
    }
    check = safeLoadFriend(fuser, CMD_SENDENERGY, e.fd_);
    if(check == false)return;
    FriendInfo * fr = safeGetFriendInfo(user, fuid, CMD_SENDENERGY, e.fd_);
    if(fr == NULL) return;
    long long now = time(NULL);
    if(dateChange(now,fr->last_send_time_)==false){
        sendErrorResponse(e.fd_, CMD_SENDENERGY, ERROR_HAVE_SENT_ENERGY);
        return;
    }
    fr->last_send_time_ = now;
    check = dh_->saveFriendInfo(fr);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_SENDENERGY,ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    user->send_energy_daily_limit_ += game_config.energy_per_act_;
    check = dh_->saveUser(user);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_SENDENERGY,ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    map<long long ,FriendInfo * >::iterator it = fuser->friends_.find(user->uid_);
    if(it == fuser->friends_.end()){
        sendErrorResponse(e.fd_, CMD_SENDENERGY, ERROR_NOT_FRIEND);
        return;
    }
    if(dateChange(now,it->second->last_receive_time_)==true){
        it->second->last_receive_time_ = now;
        it->second->accepted_ = HAVE_ENERGY_TO_ACCEPT;
        check = dh_->saveFriendInfo(it->second);
        if(check == false || dh_->db_error_ != 0){
            sendErrorResponse(e.fd_, CMD_SENDENERGY,ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    LOG4CXX_INFO(logger_, "sendenergy;"<<uid<<";"<<fuid);

    vector<Mission * > mv;
    mv.clear();
    for(map<long long , Mission*>::iterator it = user->missions_.begin();
        it!=user->missions_.end();it++){
        if(it->second->mission_type_ == MISSION_SEND_ENERGY){
            MissionConfig * mc = game_config.getMissionConfig(it->second->mission_type_, it->second->mission_subtype_);
            if(mc == NULL) continue;
            it->second->mission_record_ ++;
            if(it->second->mission_record_ >= mc->param && it->second->complete_ == MISSION_NOT_COMPLETED){
                it->second->complete_ = MISSION_COMPLETED;
                mv.push_back(it->second);
            }
            check = dh_->saveMission(it->second);
            if(check == false) return;
            break;
        }
    }
    string res = buildSendEnergyResponse(user,mv);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processAcceptEnergy(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4) return;
    uid_type uid;
    uid_type fuid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_ACCEPTENERGY, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeAtoll(check_cmd[3], fuid);
    if(check == false) return;
    FriendInfo * fr = safeGetFriendInfo(user, fuid, CMD_ACCEPTENERGY, e.fd_);
    if(fr == NULL) return;
    if(fr->accepted_ != HAVE_ENERGY_TO_ACCEPT){
        sendErrorResponse(e.fd_, CMD_ACCEPTENERGY, ERROR_NO_ENERGY_TO_ACCEPT);
        return;
    }
    if(user->receive_energy_daily_limit_ >= game_config.accept_energy_daily_limit_){
        sendErrorResponse(e.fd_, CMD_ACCEPTENERGY, ERROR_CANNOT_ACCEPT_ENERY_TODAY);
        return;
    }
    if((user->energy_ + game_config.energy_per_act_) >
       game_config.getValueFromIntVectorFilledZero(user->user_level_, game_config.energy_level_limit_)){
        sendErrorResponse(e.fd_, CMD_ACCEPTENERGY, ERROR_NO_NEED_TO_ACCEPT_ENERGY);
        return;
    }
    fr->accepted_ = NO_ENERGY_TO_ACCEPT;
    check = dh_->saveFriendInfo(fr);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_ACCEPTENERGY,ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    user->energy_ += game_config.energy_per_act_;
    user->receive_energy_daily_limit_+=game_config.energy_per_act_;
    check = dh_->saveUser(user);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_ACCEPTENERGY,ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "acceptenergy;"<<uid<<";"<<fuid);
    string res = buildAcceptEnergyResponse(user);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processLoadFriend(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=3) return;
    uid_type uid ;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_LOADFRIEND, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadFriend(user, CMD_LOADFRIEND, e.fd_);
    if(check == false) return;
    vector<User *> utmp;
    utmp.clear();
    vector<Hero * > htmp;
    htmp.clear();
    vector<FriendInfo *> frtmp;
    frtmp.clear();
    for(map<long long ,FriendInfo *>::iterator it = user->friends_.begin();
        it!=user->friends_.end();it++){
        User * fuser = safeGetUser(it->second->fuid_, CMD_LOADFRIEND, e.fd_);
        if(fuser == NULL) return;
        check = safeLoadHero(fuser, CMD_LOADFRIEND, e.fd_);
        if(check == false) return;
        Hero * fhero = dh_->getHero(fuser, fuser->hero_id_);
        if(dh_->db_error_ != 0){
            sendErrorResponse(e.fd_, CMD_LOADFRIEND, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        if(fhero == NULL) continue;
        frtmp.push_back(it->second);
        utmp.push_back(fuser);
        htmp.push_back(fhero);
    }
    string res = buildLoadFriendResponse(utmp, frtmp, htmp);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
    
}
void EventHandler::processLoadFriendRequest(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=3) return;
    uid_type uid ;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_LOADFRIENDREQUEST, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadFriendRequest(user, CMD_LOADFRIENDREQUEST, e.fd_);
    if(check == false) return;
    vector<User *> utmp;
    utmp.clear();
    vector<Hero * > htmp;
    htmp.clear();
    for(map<long long ,FriendRequestInfo * >::iterator it = user->friend_requests_.begin();
        it!=user->friend_requests_.end();it++){
        User * fuser = safeGetUser(it->second->fuid_, CMD_LOADFRIENDREQUEST, e.fd_);
        if(fuser == NULL) return;
        check = safeLoadHero(fuser, CMD_LOADFRIENDREQUEST, e.fd_);
        if(check == false) return;
        Hero * fhero = dh_->getHero(fuser, fuser->hero_id_);
        if(dh_->db_error_ != 0){
            sendErrorResponse(e.fd_, CMD_LOADFRIENDREQUEST, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        if(fhero == NULL) continue;
        utmp.push_back(fuser);
        htmp.push_back(fhero);
    }
    string res = buildLoadFriendRequestResponse(utmp, htmp);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processAddFriend(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4) return;
    uid_type uid;
    uid_type tuid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], tuid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_ADDFRIEND, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadFriendRequest(user, CMD_ADDFRIEND, e.fd_);
    if(check == false) return;
    check = safeLoadFriend(user, CMD_ADDFRIEND, e.fd_);
    if(check == false) return;
    check = safeLoadMission(user, CMD_ADDFRIEND, e.fd_);
    if(check == false) return;
    User * tuser = safeGetUser(tuid, CMD_ADDFRIEND, e.fd_);
    if(tuser == NULL) return;
    check = safeLoadFriend(tuser, CMD_ADDFRIEND, e.fd_);
    if(check == false) return;
    check = safeLoadMission(tuser, CMD_ADDFRIEND, e.fd_);
    if(check == false) return;
    if((int) user->friends_.size()>= game_config.friend_length_limit_[user->user_level_] + game_config.place_per_expand_friend_ * user->expand_friend_){
        sendErrorResponse(e.fd_, CMD_ADDFRIEND, ERROR_SELF_NO_MORE_FRIEND);
        return;
    }
    if((int) tuser->friends_.size()>=game_config.friend_length_limit_[tuser->user_level_] + game_config.place_per_expand_friend_ * tuser->expand_friend_){
        sendErrorResponse(e.fd_, CMD_ADDFRIEND, ERROR_TARGET_NO_MORE_FRIEND);
        return;
    }
    FriendRequestInfo * fri = safeGetFriendRequestInfo(user, tuid, CMD_ADDFRIEND, e.fd_);
    if(fri == NULL) return;
    /*FriendInfo * fr = safeGetFriendInfo(user, tuid, CMD_ADDFRIEND, e.fd_);
    if(fr == NULL) return;*/
    map<long long,FriendInfo *>::iterator frcheck= user->friends_.find(tuid);
    if(frcheck != user->friends_.end()){
        sendErrorResponse(e.fd_, CMD_ADDFRIEND, ERROR_ALREADY_BEEN_FRIENDS);
        return;
    }
    check = dh_->addFriend(user,tuser);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_ADDFRIEND, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    check = dh_->addFriend(tuser,user);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_ADDFRIEND, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    check = dh_->deleteFriendRequest(fri->record_id_);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_ADDFRIEND, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "addfriend;"<<uid<<";"<<tuid);
    user->friend_requests_.erase(fri->fuid_);
    vector<Mission *> mv;
    mv.clear();
    for(map<long long ,Mission * >::iterator it = user->missions_.begin();
        it!=user->missions_.end();it++){
        if(it->second->mission_type_ == MISSION_ADD_FRIEND){
            it->second->mission_record_ ++;
            MissionConfig * mc = game_config.getMissionConfig(it->second->mission_type_, it->second->mission_subtype_);
            if(mc == NULL) continue;
            if(it->second->mission_record_>=mc->param && it->second->complete_ == MISSION_NOT_COMPLETED){
                it->second->complete_ = MISSION_COMPLETED;
                mv.push_back(it->second);
            }
            check = dh_->saveMission(it->second);
            if(check == false) return;
        }
    }
    string res = buildAddFriendResponse(mv);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);

    for(map<long long ,Mission * >::iterator it = tuser->missions_.begin();
        it!=tuser->missions_.end();it++){
        if(it->second->mission_type_ == MISSION_ADD_FRIEND){
            it->second->mission_record_ ++;
            MissionConfig * mc = game_config.getMissionConfig(it->second->mission_type_, it->second->mission_subtype_);
            if(mc == NULL) continue;
            if(it->second->mission_record_>=mc->param){
                it->second->complete_ = MISSION_COMPLETED;
            }
            check = dh_->saveMission(it->second);
            if(check == false) return;
        }
    }
    return;
}
void EventHandler::processRefuseFriendRequest(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4)return;
    uid_type uid;
    uid_type fuid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], fuid);
    if(check == false) return;
    User * user  = safeGetUser(uid, CMD_REFUSEFRIENDREQ, e.fd_,check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadFriendRequest(user, CMD_REFUSEFRIENDREQ, e.fd_);
    if(check == false) return;
    FriendRequestInfo * fri = safeGetFriendRequestInfo(user, fuid, CMD_REFUSEFRIENDREQ, e.fd_);
    if(fri == NULL)return;
    check = dh_->deleteFriendRequest(fri->record_id_);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_REFUSEFRIENDREQ, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    user->friend_requests_.erase(fri->fuid_);
    string res = buildRefuseFriendReqestResponse();
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processDeleteFriend(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4)return;
    uid_type uid;
    uid_type fuid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], fuid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_DELETEFRIEND, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadFriend(user, CMD_DELETEFRIEND, e.fd_);
    if(check == false) return;
    User * fuser = safeGetUser(fuid, CMD_DELETEFRIEND,e.fd_);
    if(fuser == NULL) return;
    check = safeLoadFriend(fuser, CMD_DELETEFRIEND, e.fd_);
    if(check == false)return;
    FriendInfo * fr = safeGetFriendInfo(user, fuid, CMD_DELETEFRIEND,e.fd_);
    if(fr == NULL)return;
    check = dh_->deleteFriend(fr->record_id_);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_DELETEFRIEND, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    user->friends_.erase(fr->fuid_);
    FriendInfo * ffr = safeGetFriendInfo(fuser,uid, CMD_DELETEFRIEND, e.fd_);
    if(ffr == NULL) return;
    check = dh_->deleteFriend(ffr->record_id_);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_DELETEFRIEND, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    fuser->friends_.erase(ffr->fuid_);
    string res = buildDeleteFriendResponse();
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processSoulMerge(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4)return;
    uid_type uid;
    int soul_mid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoi(check_cmd[3], soul_mid);
    if(check == false) return;
    User * user= safeGetUser(uid, CMD_SOULMERGE, e.fd_,check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadItem(user, CMD_SOULMERGE, e.fd_);
    if(check == false) return;
    check = safeLoadHero(user, CMD_SOULMERGE, e.fd_);
    if(check == false) return;
    check = safeLoadSoldier(user, CMD_SOULMERGE, e.fd_);
    if(check == false) return;
    Soul * soul = game_config.getSoulConfig(soul_mid);
    if(soul == NULL){
        sendErrorResponse(e.fd_, CMD_SOULMERGE, ERROR_CANNOT_FIND_SOUL_CONFIG);
        return;
    }
    Item * item = safeGetItem(user, ITEM_TYPE_CHIP, soul_mid);
    if(item == NULL || item->amount_ < soul->amount || soul->gold > user->gold_){
        sendErrorResponse(e.fd_, CMD_SOULMERGE, ERROR_CANNOT_PAY_COST);
        return;
    }
    user->gold_ -= soul->gold;
    check = dh_->saveUser(user);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_SOULMERGE, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    item->amount_ -= soul->amount;
    check = dh_->saveItem(item);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_SOULMERGE, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    long long now = time(NULL);
    if(soul->type == ITEM_TYPE_HERO){
        check = dh_->addNewHero(user, soul->mid, soul->level, soul->star, now);
        if(check == false){
            sendErrorResponse(e.fd_, CMD_SOULMERGE, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }else if(soul->type == ITEM_TYPE_SOLDIER){
        check = dh_->addNewSoldier(user, soul->mid, soul->level, soul->star, now);
        if(check == false){
            sendErrorResponse(e.fd_, CMD_SOULMERGE, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    LOG4CXX_INFO(logger_, "soulmerge;"<<uid<<";"<<item->mid_<<";"<<item->amount_);
    string res = buildSoulMergeResponse(user);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processLoadUserById(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4)return;
    uid_type uid,tuid;
    bool check = safeAtoll(check_cmd[1],uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], tuid);
    if(check == false) return;
    if(uid == tuid){
        sendErrorResponse(e.fd_, CMD_LOADUSERBYID, ERROR_UID_CONFILICT);
        return;
    }
    User * tuser = safeGetUser(tuid, CMD_LOADUSERBYID, e.fd_);
    if(tuser == NULL) return;
    check = safeLoadHero(tuser, CMD_LOADUSERBYID, e.fd_);
    if(check == false) return;
    Hero * hero = safeGetHero(tuser, tuser->hero_id_, CMD_LOADUSERBYID, e.fd_);
    if(hero == NULL) return;
    check = safeLoadSoldier(tuser, CMD_LOADUSERBYID, e.fd_);
    if(check == false) return;
    vector<Soldier *> sv;
    sv.clear();
    for(size_t i = 0;i<tuser->soldier_id_.size();i++){
        Soldier * soldier = safeGetSoldier(tuser, tuser->soldier_id_[i], CMD_LOADUSERBYID, e.fd_);
        if(soldier == NULL) return;
        sv.push_back(soldier);
    }
    string res = buildLoadUserByIdResponse(tuser, hero,sv);
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processLoadMail(EventCmd &e,vector<string> &check_cmd)
{
    if(check_cmd.size()!=3)return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false)return;
    User * user = safeGetUser(uid, CMD_LOADMAIL, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadMail(user, CMD_LOADMAIL, e.fd_);
    if(check == false) return ;
    vector<User *> utmp;
    utmp.clear();
    vector<Hero * > htmp;
    htmp.clear();
    vector<Mail *> mtmp;
    mtmp.clear();
    for(map<long long ,Mail *>::iterator it = user->mails_.begin();
        it!=user->mails_.end();it++){
        if(it->second->sender_id_ == SYSTEM_MAIL_SENDER_ID) continue;
        User * fuser = safeGetUser(it->second->sender_id_, CMD_LOADMAIL, e.fd_);
        if(fuser == NULL) return;
        check = safeLoadHero(fuser, CMD_LOADMAIL, e.fd_);
        if(check == false) return;
        Hero * fhero = dh_->getHero(fuser, fuser->hero_id_);
        if(dh_->db_error_ != 0){
            sendErrorResponse(e.fd_, CMD_LOADMAIL, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        if(fhero == NULL) continue;
        mtmp.push_back(it->second);
        utmp.push_back(fuser);
        htmp.push_back(fhero);
    }

    string res = buildLoadMailResponse(user,mtmp,utmp,htmp);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processOpenMail(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4)return;
    uid_type uid;
    long long mail_id;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false)return;
    check = safeAtoll(check_cmd[3], mail_id);
    if(check == false)return;
    User * user = safeGetUser(uid, CMD_OPENMAIL, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadMail(user, CMD_OPENMAIL, e.fd_);
    if(check == false) return;
    Mail * mail = safeGetMail(user,mail_id, CMD_OPENMAIL, e.fd_);
    if(mail == NULL) return;
    if(mail->haveread_ == MAIL_NOT_READ){
        mail->haveread_ = MAIL_READ;
        check = dh_->saveMail(mail);
        if(check == false || dh_->db_error_ != 0){
            sendErrorResponse(e.fd_, CMD_OPENMAIL, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    string res = buildOpenMailResponse(mail);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processDeleteMail(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4)return;
    uid_type uid;
    long long mail_id;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], mail_id);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_DELETEMAIL, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadMail(user, CMD_DELETEMAIL, e.fd_);
    if(check == false) return;
    Mail * mail = safeGetMail(user, mail_id, CMD_DELETEMAIL, e.fd_);
    if(mail == NULL) return;
    check = dh_->deleteMail(mail_id);
    if(check == false||dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_DELETEMAIL, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    user->mails_.erase(mail_id);
    string res = buildDeleteMailResponse(mail_id);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processSendMail(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=5)return;
    uid_type uid,tuid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], tuid);
    if(check == false) return;
    if(uid == tuid){
        sendErrorResponse(e.fd_, CMD_SENDMAIL, ERROR_UID_CONFILICT);
        return;
    }
    if((int) check_cmd[4].size()>game_config.p2p_mail_length_limit_){
        sendErrorResponse(e.fd_, CMD_SENDMAIL, ERROR_MAIL_TOO_LONG);
        return;
    }
    User * user = safeGetUser(uid, CMD_SENDMAIL, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadFriend(user, CMD_SENDMAIL, e.fd_);
    if(check == false) return;
    FriendInfo * fr = safeGetFriendInfo(user, tuid, CMD_SENDMAIL, e.fd_);
    if(fr == NULL) return;
    User * tuser = safeGetUser(tuid, CMD_SENDMAIL, e.fd_);
    if(tuser == NULL) return;
    check = safeLoadMail(tuser, CMD_SENDMAIL, e.fd_);
    if(check == false) return;
    int count = 0;
    for(map<long long,Mail *>::iterator it = tuser->mails_.begin();
        it!=tuser->mails_.end();it++){
        if(it->second->sender_id_!=SYSTEM_MAIL_SENDER_ID){
            count ++;
        }
    }
    while(count >= game_config.p2p_mail_limit_){
        long long time_check = time(NULL);
        long long mail_id = 0;
        for(map<long long,Mail *>::iterator it = tuser->mails_.begin();
            it!=tuser->mails_.end();it++){
            if(it->second->send_time_ < time_check){
                mail_id = it->second->mail_id_;
            }
        }
        check = dh_->deleteMail(mail_id);
        if(check == false){
            sendErrorResponse(e.fd_, CMD_SENDMAIL, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
        tuser->mails_.erase(mail_id);
    }
    string str="";
    check = dh_->addNewMail(tuser, uid, user->nick_name_, check_cmd[4], MAIL_NO_ITEM, 0, str);
    if(check == false) return;
    string res = buildSendMailResponse();
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processAcceptMailItem(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4)return;
    uid_type uid;
    long long mail_id;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], mail_id);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_ACCEPTMAILITEM, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadMail(user, CMD_ACCEPTMAILITEM, e.fd_);
    if(check == false) return;
    check = safeLoadHero(user, CMD_ACCEPTMAILITEM, e.fd_);
    if(check == false) return;
    check = safeLoadSoldier(user,CMD_ACCEPTMAILITEM,e.fd_);
    if(check == false) return;
    check = safeLoadItem(user, CMD_ACCEPTENERGY, e.fd_);
    if(check == false) return;
    Mail * mail = safeGetMail(user, mail_id, CMD_ACCEPTMAILITEM, e.fd_);
    if(mail == NULL) return;
    if(mail->sender_id_ != SYSTEM_MAIL_SENDER_ID){
        sendErrorResponse(e.fd_, CMD_ACCEPTMAILITEM, ERROR_NOT_SYSTEM_MAIL);
        return;
    }
    if(mail->acceptable_ != MAIL_WITH_ITEM){
        sendErrorResponse(e.fd_, CMD_ACCEPTMAILITEM, ERROR_NOT_ACCEPTABLE);
        return;
    }
    int mail_mid;
    check = safeAtoi(mail->text_, mail_mid);
    if(check == false)return;
    MailConfig * mc = game_config.getMailConfig(mail_mid);
    if(mc == NULL){
        sendErrorResponse(e.fd_, CMD_ACCEPTMAILITEM, ERROR_CONFIG_ERROR);
        return;
    }
    long long now = time(NULL);
    for(size_t i = 0;i<mc->reward.size();i++){
        check = addStageReward(user, &(mc->reward[i]), now);
        if(check == false){
            sendErrorResponse(e.fd_, CMD_ACCEPTMAILITEM, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    if (mail->contents.size() > 1) {
        vector <StageReward> strewards;
        parseUserRewardsContent(strewards, mail->contents);
        for (size_t i = 0; i < strewards.size(); i++) {
            if (addStageReward(user, &strewards[i], now) == false) {
                //LOG
            }
        }
    }

    mail->acceptable_ = MAIL_ITEM_ACCEPTED;
    check = dh_->saveMail(mail);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_ACCEPTMAILITEM, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "mailreward;"<<uid<<";"<<mail->text_);
    string res = buildAcceptMailItemResponse();
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processGetNewMailNum(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=3)return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_GETNEWMAILNUM, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadMail(user, CMD_GETNEWMAILNUM, e.fd_);
    if(check == false) return;
    int count = 0;
    for(map<long long,Mail*>::iterator it = user->mails_.begin();
        it!=user->mails_.end();it++){
        if(it->second->haveread_ == MAIL_NOT_READ){
            count ++;
        }
    }
    string res = buildGetNewMailNumResponse(count);
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processLoadMission(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=3)return;
    uid_type uid ;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return ;
    User * user = safeGetUser(uid, CMD_LOADMISSION, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadMission(user,CMD_LOADMISSION,e.fd_);
    if(check == false) return;
    string res = buildLoadMissionResponse(user);
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processGetCMNum(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=3)return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_GETCMNUM, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadMission(user, CMD_GETCMNUM, e.fd_);
    if(check == false) return;
    int count = 0;
    for(map<long long,Mission *>::iterator it = user->missions_.begin();
        it!= user->missions_.end();it++){
        if(it->second->complete_ == MISSION_COMPLETED){
            count ++;
        }
    }
    string res = buildGetCMNumResponse(count);
    nh_->sendString(e.fd_, res);
    return;
    
}
void EventHandler::processGetMissionReward(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4)return;
    uid_type uid;
    long long mission_id;
    int record = 0;
    long long now = time(NULL);
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], mission_id);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_GETMISSIONREWARD, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadMission(user, CMD_GETMISSIONREWARD, e.fd_);
    if(check == false) return;
    check = safeLoadItem(user, CMD_GETMISSIONREWARD, e.fd_);
    if(check == false) return;
    check = safeLoadHero(user, CMD_GETMISSIONREWARD, e.fd_);
    if(check == false) return;
    check = safeLoadSoldier(user, CMD_GETMISSIONREWARD, e.fd_);
    if(check == false) return;
    Mission * mission = safeGetMission(user, mission_id, CMD_GETMISSIONREWARD, e.fd_);
    if(mission == NULL) return;
    record = mission->mission_record_;
    if(mission ->complete_ == MISSION_NOT_COMPLETED){
        sendErrorResponse(e.fd_, CMD_GETMISSIONREWARD, ERROR_MISSION_NOT_COMPLETE);
        return;
    }
    MissionConfig * mc = game_config.getMissionConfig(mission->mission_type_, mission->mission_subtype_);
    if(mc == NULL){
        sendErrorResponse(e.fd_, CMD_GETMISSIONREWARD, ERROR_CONFIG_ERROR);
        return;
    }
    check = dh_->deleteMission(mission_id);
    if(check == false|| dh_->db_error_!= 0){
        sendErrorResponse(e.fd_, CMD_GETMISSIONREWARD, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    user->missions_.erase(mission_id);
    for(size_t i = 0;i<mc->reward.size();i++){
        check = addStageReward(user, &(mc->reward[i]), now);
        if(check == false){
            sendErrorResponse(e.fd_, CMD_GETMISSIONREWARD, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    LOG4CXX_INFO(logger_, "missionreward;"<<uid<<";"<<mission_id<<";"<<mc->type<<";"<<mc->subtype);
    addLinkMission(user, mc,record);
    string res = buildGetMissionRewardResponse(user,mission_id);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processBuyEnergy(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=3)return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_BUYENERGY, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    if(user->energy_ >= game_config.getValueFromIntVectorFilledZero(user->user_level_, game_config.energy_level_limit_)){
        sendErrorResponse(e.fd_, CMD_BUYENERGY, ERROR_NO_NEED_TO_ACCEPT_ENERGY);
        return;
    }
    if(user->buy_energy_daily_ >= game_config.buy_energy_daily_limit_){
        sendErrorResponse(e.fd_, CMD_BUYENERGY, ERROR_NO_MORE_BUY_ENERGY);
        return;
    }
    if(user->diamond_<game_config.cost_per_buy_energy_){
        sendErrorResponse(e.fd_, CMD_BUYENERGY, ERROR_CANNOT_PAY_COST);
        return;
    }
    user->energy_ = game_config.getValueFromIntVectorFilledZero(user->user_level_, game_config.energy_level_limit_);
    user->diamond_ -= game_config.energy_price_[user->buy_energy_daily_];
    user->buy_energy_daily_ ++;
    check = dh_->saveUser(user);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_BUYENERGY, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "buyenergy;"<<uid<<";"<<user->diamond_<<";"<<user->energy_);
    string res = buildBuyEnergyResponse(user->buy_energy_daily_);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processExpandHero(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=3)return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_EXPANDHERO, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    if(user->expand_hero_ >= game_config.expand_hero_limit_){
        sendErrorResponse(e.fd_, CMD_EXPANDHERO, ERROR_GOT_LIMIT);
        return;
    }
    if(user->diamond_ < game_config.cost_per_expand_hero_){
        sendErrorResponse(e.fd_, CMD_EXPANDHERO, ERROR_CANNOT_PAY_COST);
        return;
    }
    user->diamond_-=game_config.cost_per_expand_hero_;
    user->expand_hero_ ++;
    check = dh_->saveUser(user);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_EXPANDHERO, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "expandhero;"<<uid<<";"<<user->diamond_<<";"<<user->expand_hero_);
    string res = buildExpandHeroResponse();
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processExpandSoldier(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=3)return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_EXPANDSOLDIER, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    if(user->expand_soldier_ >= game_config.expand_soldier_limit_){
        sendErrorResponse(e.fd_, CMD_EXPANDSOLDIER, ERROR_GOT_LIMIT);
        return;
    }
    if(user->diamond_ < game_config.cost_per_expand_soldier_){
        sendErrorResponse(e.fd_, CMD_EXPANDSOLDIER, ERROR_CANNOT_PAY_COST);
        return;
    }
    user->diamond_-=game_config.cost_per_expand_soldier_;
    user->expand_soldier_ ++;
    check = dh_->saveUser(user);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_EXPANDSOLDIER, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "expandsoldier;"<<uid<<";"<<user->diamond_<<";"<<user->expand_soldier_);
    string res = buildExpandSoldierResponse();
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processExpandFriend(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=3)return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_EXPANDFRIEND, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    if(user->expand_friend_ >= game_config.expand_friend_limit_){
        sendErrorResponse(e.fd_, CMD_EXPANDFRIEND, ERROR_GOT_LIMIT);
        return;
    }
    if(user->diamond_ < game_config.cost_per_expand_friend_){
        sendErrorResponse(e.fd_, CMD_EXPANDFRIEND, ERROR_CANNOT_PAY_COST);
        return;
    }
    user->diamond_-=game_config.cost_per_expand_friend_;
    user->expand_friend_ ++;
    check = dh_->saveUser(user);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_EXPANDFRIEND, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "expandfriend;"<<uid<<";"<<user->diamond_<<";"<<user->expand_friend_);
    string res = buildExpandFriendResponse();
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processSummon(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=5) return;
    uid_type uid;
    int type = 0;
    int amount = 0;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoi(check_cmd[3], amount);
    if(check == false) return;
    if(amount != 1 && amount != 10) return;
    check = safeAtoi(check_cmd[4], type);
    if(check == false) return;
    if(check != 1 && check != 2) return;
    User * user = safeGetUser(uid, CMD_SUMMON, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadHero(user, CMD_SUMMON, e.fd_);
    if(check == false) return;
    check = safeLoadSoldier(user, CMD_SUMMON, e.fd_);
    if(check == false) return;
    check = safeLoadItem(user, CMD_SUMMON, e.fd_);
    if(check == false) return;
    vector<StageReward *> rewards;
    rewards.clear();
    long long now = time(NULL);
    if(type == 1){
        if(amount == 1){
            if(user->diamond_< game_config.summon_price_diamond_1_){
                sendErrorResponse(e.fd_, CMD_SUMMON, ERROR_CANNOT_PAY_COST);
                return;
            }
            user->diamond_ -= game_config.summon_price_diamond_1_;
        }
        if(amount == 10){
            if(user->diamond_< game_config.summon_price_diamond_10_){
                sendErrorResponse(e.fd_, CMD_SUMMON, ERROR_CANNOT_PAY_COST);
                return;
            }
            user->diamond_ -= game_config.summon_price_diamond_10_;
        }
        for(int i = 0;i<amount; i++){
            StageReward * sr = game_config.getDiamondSummonReward();
            if(sr == NULL){
                sendErrorResponse(e.fd_, CMD_SUMMON, ERROR_CONFIG_ERROR);
                return;
            }
            rewards.push_back(sr);
            /*if(addStageReward(user, sr, now) == false){
                sendErrorResponse(e.fd_, CMD_SUMMON, ERROR_SYSTEM_CRITICAL_ERROR);
                return;
            }
            if(sr->type == ITEM_TYPE_HERO || sr->type == ITEM_TYPE_SOLDIER){
                if(sr->param_2 >= 5){
                    dh_->addNews(user->nick_name_, NEWS_TYPE_SUMMON, sr->type, sr->subtype, sr->param_2);
                }
            }*/
        }
        if(amount == 10){
            StageReward * sr = game_config.getDiamondSummonRewardFix();
            if(sr == NULL){
                sendErrorResponse(e.fd_, CMD_SUMMON, ERROR_CONFIG_ERROR);
                return;
            }
            rewards[9] = sr;
        }
        for(size_t i = 0;i<rewards.size();i++){
            StageReward * sr = rewards[i];
            if(addStageReward(user, sr, now) == false){
                sendErrorResponse(e.fd_, CMD_SUMMON, ERROR_SYSTEM_CRITICAL_ERROR);
                return;
            }
            if(sr->type == ITEM_TYPE_HERO || sr->type == ITEM_TYPE_SOLDIER){
                if(sr->param_2 >= 5){
                    dh_->addNews(user->nick_name_, NEWS_TYPE_SUMMON, sr->type, sr->subtype, sr->param_2);
                }
            }
        }
        LOG4CXX_INFO(logger_, "summon;"<<type<<";"<<amount<<";"<<uid<<";"<<user->diamond_);
    }else if(type == 2){
        if(amount == 1){
            if(user->spark_num_< game_config.summon_price_spark_1_){
                sendErrorResponse(e.fd_, CMD_SUMMON, ERROR_CANNOT_PAY_COST);
                return;
            }
            user->spark_num_ -= game_config.summon_price_spark_1_;
        }
        if(amount == 10){
            if(user->spark_num_< game_config.summon_price_spark_10_){
                sendErrorResponse(e.fd_, CMD_SUMMON, ERROR_CANNOT_PAY_COST);
                return;
            }
            user->spark_num_ -= game_config.summon_price_spark_10_;
        }
        for(int i = 0;i<amount; i++){
            StageReward * sr = game_config.getSummonReward();
            if(sr == NULL){
                sendErrorResponse(e.fd_, CMD_SUMMON, ERROR_CONFIG_ERROR);
                return;
            }
            rewards.push_back(sr);
        }
        if(amount == 10){
            StageReward * sr = game_config.getSummonRewardFix();
            if(sr == NULL){
                sendErrorResponse(e.fd_, CMD_SUMMON, ERROR_CONFIG_ERROR);
                return;
            }
            rewards[9] = sr;
        }
        for(int i = 0 ;i<amount;i++){
            StageReward * sr = rewards[i];
            if(addStageReward(user, sr, now) == false){
                sendErrorResponse(e.fd_, CMD_SUMMON, ERROR_SYSTEM_CRITICAL_ERROR);
                return;
            }
            if(sr->type == ITEM_TYPE_HERO || sr->type == ITEM_TYPE_SOLDIER){
                if(sr->param_2 >= 5){
                    dh_->addNews(user->nick_name_, NEWS_TYPE_SUMMON, sr->type, sr->subtype, sr->param_2);
                }
            }
        }
        LOG4CXX_INFO(logger_, "summon;"<<type<<";"<<amount<<";"<<uid<<";"<<user->spark_num_);
    }
    string res = buildSummonResponse(user, rewards);
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processLoadNews(EventCmd &e, vector<string> &check_cmd)
{
    int last_news_id;
    bool check = safeAtoi(check_cmd[1], last_news_id);
    if(check == false) return;
    vector<News *> news;
    news.clear();
    for(size_t i = 0;i<dh_->news_.size();i++){
        if(dh_->news_[i].news_id>last_news_id){
            news.push_back(&(dh_->news_[i]));
        }
    }
    string res = buildLoadNewsResponse(news);
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processAdminAlterUser(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4) return;
    uid_type uid;
    int subject ,amount;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoi(check_cmd[2], subject);
    if(check == false) return;
    check = safeAtoi(check_cmd[3], amount);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_ADMINALTERUSER, e.fd_);
    if(user == NULL) return;
    if(subject == 1){
        user->gold_ = amount;
    }else if(subject == 2){
        user->diamond_ = amount;
    }else if(subject == 3){
        user->energy_ = amount;
    }else if(subject == 4){
        user->spark_num_ = amount;
    }else if(subject == 5){
        user->user_level_ = min(amount,int(game_config.exp_level_limit_.size() - 1));
    }else if(subject == 6){
        user->user_exp_ = amount;
    }else if(subject == 7){
        user->normal_chapter_id_ = amount/10000;
        user->normal_stage_id_ = amount%10000;
    }else if(subject == 8){
        user->elite_chapter_id_ = amount/10000;
        user->elite_stage_id_ = amount%10000;
    }else if(subject == 9){
        user->expand_hero_ = amount;
    }else if(subject == 10){
        user->expand_soldier_ = amount;
    }else if(subject == 11){
        user->expand_friend_ = amount;
    }else if(subject == 12){
        user->guide_record_ = amount;
    }else if(subject == 13){
        user->diamond_ += amount;
    }
    check = dh_->saveUser(user);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_ADMINALTERUSER, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildAdminResponse();
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processAdminAddHero(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=5)return;
    uid_type uid;
    int mid,star,level;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoi(check_cmd[2], mid);
    if(check == false) return;
    check = safeAtoi(check_cmd[3], star);
    if(check == false) return;
    check = safeAtoi(check_cmd[4], level);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_ADMINADDHERO, e.fd_);
    if(user == NULL) return;
    check = safeLoadHero(user, CMD_ADMINADDHERO, e.fd_);
    if(check == false) return;
    long long now = time(NULL);
    check = dh_->addNewHero(user, mid, level, star, now);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_ADMINADDHERO, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildAdminResponse();
    nh_->sendString(e.fd_, res);
    return;
    
}
void EventHandler::processAdminAlterHero(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=8)return;
    uid_type uid,hid;
    int star,level,skill1,skill2,skill3;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[2], hid);
    if(check == false) return;
    check = safeAtoi(check_cmd[3], star);
    if(check == false) return;
    check = safeAtoi(check_cmd[4], level);
    if(check == false) return;
    check = safeAtoi(check_cmd[5], skill1);
    if(check == false) return;
    check = safeAtoi(check_cmd[6], skill2);
    if(check == false) return;
    check = safeAtoi(check_cmd[7], skill3);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_ADMINADDHERO, e.fd_);
    if(user == NULL) return;
    check = safeLoadHero(user, CMD_ADMINALTERHERO, e.fd_);
    if(check == false) return;
    Hero * hero = safeGetHero(user, hid, CMD_ADMINALTERHERO, e.fd_);
    if(hero == NULL) return;
    hero->star_ = star;
    hero->level_ = level;
    hero->skill_[0] = skill1;
    hero->skill_[1] = skill2;
    hero->skill_[2] = skill3;
    check = dh_->saveHero(hero);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_ADMINALTERHERO, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildAdminResponse();
    nh_->sendString(e.fd_, res);
    return;
    
}
void EventHandler::processAdminAddSoldier(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=5)return;
    uid_type uid;
    int mid,star,level;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoi(check_cmd[2], mid);
    if(check == false) return;
    check = safeAtoi(check_cmd[3], star);
    if(check == false) return;
    check = safeAtoi(check_cmd[4], level);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_ADMINADDSOLDIER, e.fd_);
    if(user == NULL) return;
    check = safeLoadSoldier(user,CMD_ADMINADDSOLDIER,e.fd_);
    if(check == false) return;
    long long now = time(NULL);
    check = dh_->addNewSoldier(user, mid, level, star, now);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_ADMINADDSOLDIER, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildAdminResponse();
    nh_->sendString(e.fd_, res);
    return;
    
}
void EventHandler::processAdminAddItem(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=5)return;
    uid_type uid;
    int type,subtype,amount;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoi(check_cmd[2], type);
    if(check == false) return;
    check = safeAtoi(check_cmd[3], subtype);
    if(check == false) return;
    check = safeAtoi(check_cmd[4], amount);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_ADMINADDITEM, e.fd_);
    if(user == NULL) return;
    check = safeLoadItem(user, CMD_ADMINADDITEM, e.fd_);
    if(check == false) return;
    long long now = time(NULL);
    check = dh_->addItem(user, type, subtype, amount, now);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_ADMINADDITEM, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildAdminResponse();
    nh_->sendString(e.fd_,res);
    return;
    
}
void EventHandler::processLog(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=2)return;
    LOG4CXX_INFO(logger_, check_cmd[1]);
    string res = buildLogResponse();
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processGuide(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4)return;
    uid_type uid;
    int record = 0;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoi(check_cmd[3], record);
    User * user = safeGetUser(uid, CMD_GUIDE, e.fd_,check_cmd[2]);
    if(user == NULL) return;
    user->guide_record_ = record;
    check = dh_->saveUser(user);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_GUIDE, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildGuideResponse();
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processAlterUserName(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=5) return;
    if(check_cmd[1].size() == 0 || check_cmd[2].size() == 0
       || check_cmd[3].size() == 0 || check_cmd[4].size() == 0) return;
    if(check_cmd[1].size()>32 || check_cmd[2].size()>32 || check_cmd[3].size()>32 || check_cmd[4].size()>32) return;
    if(checkString(check_cmd[1])==false || checkString(check_cmd[2])==false
       || checkString(check_cmd[3]) == false || checkString(check_cmd[4]) == false){
        LOG4CXX_WARN(logger_, "check string false:"<<check_cmd[1]<<";"\
                     <<check_cmd[2]<<";"<<check_cmd[3]<<";"<<check_cmd[4]);
    }
    bool check = false;
    check = dh_->usernameHasBeenUsed(check_cmd[2]);
    if(check == true){
        sendErrorResponse(e.fd_, CMD_REGISTER,ERROR_USERNAME_HAS_BEEN_USED);
        return;
    }
    if(dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_REGISTER, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    UserBasicInfo ubi;
    check = dh_->getUserBasicInfo(check_cmd[1], check_cmd[1], ubi);
    if(check == false){
        if(dh_->db_error_ != 0){
            sendErrorResponse(e.fd_, CMD_LOGIN,ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }else{
            sendErrorResponse(e.fd_, CMD_LOGIN,ERROR_USER_OR_PASSWD_WRONG);
            return;
        }
    }
    if(ubi.eqid_ != check_cmd[4]){
        sendErrorResponse(e.fd_, CMD_ALTERUSERNAME, ERROR_EQID_WRONG);
        return;
    }
    ubi.username_ = check_cmd[2];
    ubi.password_ = check_cmd[3];
    check = dh_->saveUbi(&ubi);
    if(dh_->db_error_!=0 || check == false){
        sendErrorResponse(e.fd_, CMD_ALTERUSERNAME, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildAlterUserNameResponse();
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processPaymentTest(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size() != 4)return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_PAYMENTVERIFYRECEIVED, e.fd_,check_cmd[2]);
    if(user == NULL) return;
    user->diamond_ += 10000;
    check = dh_->saveUser(user);
    if(check == false) {
        sendErrorResponse(e.fd_, CMD_PAYMENTVERIFYRECEIVED, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildPaymentVerifyReceivedResponse(user);
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processLoadAct(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4) return;
    uid_type uid;
    int act_id;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoi(check_cmd[3], act_id);
    if(check == false) return;
    ActConfig * ac = game_config.getActConfig(act_id);
    if(ac == NULL){
        string res = buildLoadActFailResponse(ERROR_ACT_NOT_AVAIABLE, act_id);
        return;
    }
    User * user = safeGetUser(uid, CMD_LOADACT, e.fd_,check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadAct(user, CMD_LOADACT, e.fd_);
    if(check == false) return;
    
    Act * act = getAct(user, act_id);
    if(act == NULL){
        string res = buildLoadActFailResponse(ERROR_ACT_NOT_AVAIABLE, act_id);
        return;
    }
    
    if(act_id == ACT_3){
        check = dh_->saveAct(act);
        if(check == false){
            sendErrorResponse(e.fd_, CMD_ACT, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    
    string actrecord = vectorLongToString(act->record_);
    string res = buildLoadActResponse(act_id, actrecord);
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processAct(EventCmd &e,vector<string> &check_cmd)
{
    if(check_cmd.size()<4)return;
    uid_type uid ;
    int act_id ;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoi(check_cmd[3], act_id);
    if(check == false) return;
    switch(act_id){
        case ACT_2:
            act_2(uid,check_cmd,e.fd_);
            break;
        case ACT_3:
            act_3(uid,check_cmd,e.fd_);
            break;
        default:
            break;
    }
    return;
}
void EventHandler::processLoadTeam(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=3)return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_LOADTEAM, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadTeam(user, CMD_LOADTEAM, e.fd_);
    if(check == false) return;
    string res = buildLoadTeamResponse(user);
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processSaveTeam(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=6) return;
    string delims = ",";
    uid_type uid;
    long long team_id;
    long long hero_id;
    vector<long long> soldier_id;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], team_id);
    if(check == false) return;
    check = safeAtoll(check_cmd[4], hero_id);
    if(check == false) return;
    check = longtokenize(check_cmd[5], soldier_id, delims);
    if(check == false) return;
    if(soldier_id.size()!= SOLDIER_INGROUND) return;
    User * user = safeGetUser(uid, CMD_SAVETEAM, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadHero(user, CMD_SAVETEAM, e.fd_);
    if(check == false) return;
    check = safeLoadSoldier(user, CMD_SAVETEAM, e.fd_);
    if(check == false) return;
    check = safeLoadTeam(user, CMD_SAVETEAM, e.fd_);
    if(check == false) return;
    map<long long,Team *>::iterator it = user->teams_.find(team_id);
    if(it==user->teams_.end()){
        sendErrorResponse(e.fd_, CMD_SAVETEAM, ERROR_TEAM_CANNOT_BE_FOUND);
        return;
    }
    Team * team = it->second;
    if(team == NULL){
        sendErrorResponse(e.fd_, CMD_SAVETEAM, ERROR_TEAM_CANNOT_BE_FOUND);
        return;
    }
    check = true;
    if(hero_id == 0){
        check = false;
    }
    if(check == true){
        Hero * hero = NULL;
        vector<Soldier *> soldiers;
        soldiers.clear();
        check = teamPrepare(user, hero_id, soldier_id, hero, soldiers);
        if(check == false) {
            sendErrorResponse(e.fd_, CMD_SAVETEAM, ERROR_SOLDIER_NOT_FOUND);
            return;
        }
        check = preCheckLeadership(user, hero, soldiers);
        if(check == false){
            sendErrorResponse(e.fd_, CMD_SAVETEAM, ERROR_LEADERSHIP_NOT_ENOUGH);
            return;
        }
        bool recreate_check = false;
        if(team->hero_id_ == 0){
            recreate_check = true;
        }
        team->hero_id_ = hero_id;
        team->soldier_id_.clear();
        for(size_t i = 0 ;i<soldier_id.size();i++){
            team->soldier_id_.push_back(soldier_id[i]);
        }
        if(recreate_check == true){
            team->recreate_time_ = time(NULL);
        }
    }else{
        team->hero_id_ = 0LL;
        team->soldier_id_.clear();
        for(int i = 0 ;i<SOLDIER_INGROUND;i++){
            team->soldier_id_.push_back(0LL);
        }
    }
    check = dh_->saveTeam(team);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_SAVETEAM, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    if(user->team_id_ == team->team_id_){
        user->team_id_ = team_id;
        user->hero_id_ = team->hero_id_;
        user->soldier_id_.clear();
        for(size_t i = 0;i<team->soldier_id_.size();i++){
            user->soldier_id_.push_back(team->soldier_id_[i]);
        }
        check = dh_->saveUser(user);
        if(check == false){
            sendErrorResponse(e.fd_, CMD_SAVETEAM, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    string res = buildSaveTeamResponse(user, team_id);
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processChangeTeam(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=4) return;
    uid_type uid ;
    long long team_id;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], team_id);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_CHANGETEAM, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadHero(user, CMD_CHANGETEAM, e.fd_);
    if(check == false) return;
    check = safeLoadSoldier(user, CMD_CHANGETEAM, e.fd_);
    if(check == false) return;
    check = safeLoadTeam(user, CMD_CHANGETEAM, e.fd_);
    if(check == false) return;
    map<long long,Team *>::iterator it = user->teams_.find(team_id);
    if(it==user->teams_.end()){
        sendErrorResponse(e.fd_, CMD_CHANGETEAM, ERROR_TEAM_CANNOT_BE_FOUND);
        return;
    }
    Team * team = it->second;
    if(team == NULL){
        sendErrorResponse(e.fd_, CMD_CHANGETEAM, ERROR_TEAM_CANNOT_BE_FOUND);
        return;
    }
    Hero * hero = safeGetHero(user, team->hero_id_, CMD_CHANGETEAM, e.fd_);
    if(hero == NULL) return;
    for(size_t i = 0 ; i<team->soldier_id_.size();i++){
        Soldier * soldier = safeGetSoldier(user, team->soldier_id_[i], CMD_CHANGETEAM, e.fd_);
        if(soldier == NULL) return;
    }
    user->team_id_ = team_id;
    user->hero_id_ = team->hero_id_;
    user->soldier_id_.clear();
    for(size_t i = 0;i<team->soldier_id_.size();i++){
        user->soldier_id_.push_back(team->soldier_id_[i]);
    }
    if(check == false){
        sendErrorResponse(e.fd_, CMD_CHANGETEAM, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildChangeTeamResponse(user, team_id);
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processChangeNick(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!= 4) return;
    uid_type uid ;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    if(checkString(check_cmd[3])==false) return;
    User * user = safeGetUser(uid, CMD_CHANGENICK, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = dh_->checkNick(check_cmd[3]);
    if(dh_->db_error_ != 0){
        sendErrorResponse(e.fd_, CMD_CHANGENICK, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    if(check == false){
        sendErrorResponse(e.fd_, CMD_CHANGENICK, ERROR_NICKNAME_USED);
        return;
    }
    user->nick_name_ = check_cmd[3];
    check = dh_->saveUserSInfo(user);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_CHANGENICK, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildChangeNickResponse();
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processForceHEU(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size() != 4)return;
    uid_type uid;
    long long hero_id;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], hero_id);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_FORCEHEU, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadHero(user, CMD_FORCEHEU, e.fd_);
    if(check == false) return;
    Hero * hero = safeGetHero(user, hero_id, CMD_FORCEHEU, e.fd_);
    if(hero == NULL) return;
    hero->level_ = game_config.force_hero_exp_up_;
    check = dh_->saveHero(hero);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_FORCEHEU, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildForceHEUResponse();
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processForceSEU(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size() != 4)return;
    uid_type uid;
    long long soldier_id;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoll(check_cmd[3], soldier_id);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_FORCESEU, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadSoldier(user, CMD_FORCESEU, e.fd_);
    if(check == false) return;
    Soldier * soldier = safeGetSoldier(user, soldier_id, CMD_FORCESEU, e.fd_);
    if(soldier == NULL) return;
    soldier->level_ = game_config.force_soldier_exp_up_;
    check = dh_->saveSoldier(soldier);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_FORCESEU, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildForceSEUResponse();
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processAddFakeUser(EventCmd &e, vector<string> &check_cmd)
{
    User * user = safeGetUser(FAKE_USER_ID, CMD_ADDFAKEUSER, e.fd_);
    if(user != NULL) return;
    bool check = dh_->createFakeUser();
    if(check == false){
        sendErrorResponse(e.fd_, CMD_ADDFAKEUSER, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = "addfakeuser;0";
    nh_->sendString(e.fd_, res);
    return;
}
void EventHandler::processDebug(EventCmd &e, vector<string> &check_cmd)
{
    if(check_cmd.size()!=2) return;
    uid_type uid;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_DEBUG, e.fd_);
    if(user == NULL) return;
    check = safeLoadHero(user, CMD_DEBUG, e.fd_);
    if(check == false) return;
    check = safeLoadSoldier(user,CMD_DEBUG,e.fd_);
    if(check == false) return;
    check = safeLoadItem(user, CMD_DEBUG, e.fd_);
    if(check == false) return;
    long long now = time(NULL);
    for(map<int,vector<HeroConfig> >::iterator it = game_config.hero_config_.begin();it!=game_config.hero_config_.end();it++){
        if(it->first<1||it->first>8)continue;
        for(size_t i =1;i<it->second.size();i++){
            check = dh_->addNewHero(user, it->second[i].mid_, 1 , i, now);
            if(check == false){
                sendErrorResponse(e.fd_, CMD_DEBUG, ERROR_SYSTEM_CRITICAL_ERROR);
                return;
            }
        }
    }
    for(map<long long,Hero *> ::iterator it = user->heroes_.begin();it!=user->heroes_.end();it++){
        Hero * hero = it->second;
        hero->level_ = 20;
        check = dh_->saveHero(hero);
        if(check == false){
            sendErrorResponse(e.fd_, CMD_DEBUG, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    for(map<int,vector<SoldierConfig> >::iterator it = game_config.soldier_config_.begin();it!=game_config.soldier_config_.end();it++){
        if(it->first<1||it->first>24)continue;
        for(size_t i =1;i<it->second.size();i++){
            check = dh_->addNewSoldier(user, it->second[i].mid_, 1 , i, now);
            if(check == false){
                sendErrorResponse(e.fd_, CMD_DEBUG, ERROR_SYSTEM_CRITICAL_ERROR);
                return;
            }
        }
    }
    for(map<long long,Soldier *> ::iterator it = user->soldiers_.begin();it!=user->soldiers_.end();it++){
        Soldier * soldier = it->second;
        soldier->level_ = 20;
        check = dh_->saveSoldier(soldier);
        if(check == false){
            sendErrorResponse(e.fd_, CMD_DEBUG, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    for(size_t i = 1 ; i <=38;i++ ){
        check = dh_->addItem(user, 2, i, 100, now);
        if(check == false){
            sendErrorResponse(e.fd_, CMD_ADMINADDITEM, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    for(int i = 1 ; i <=64;i++ ){
        check = dh_->addItem(user, 3, i, 100, now);
        if(check == false){
            sendErrorResponse(e.fd_, CMD_ADMINADDITEM, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    user->user_level_ = 50;
    user->gold_ = 999900;
    user->diamond_ = 10000;
    user->elite_chapter_id_ = 8;
    user->elite_stage_id_ = 7;
    user->normal_chapter_id_ = 8;
    user->normal_stage_id_ = 10;
    user->expand_hero_ = 1000;
    user->expand_soldier_ = 1000;
    user->expand_friend_ = 1000;
    user->guide_record_ = 0x1ff06;
    string res = buildLoadUserResponse(user);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
    return;
}
void EventHandler::processInstantBattle(EventCmd &e, vector<string> &check_cmd) {
    if(check_cmd.size()!=5) return;
    uid_type uid;
    int chapter_id;
    int stage_id;
    bool check = safeAtoll(check_cmd[1], uid);
    if(check == false) return;
    check = safeAtoi(check_cmd[3], chapter_id);
    if(check == false) return;
    check = safeAtoi(check_cmd[4], stage_id);
    if(check == false) return;
    User * user = safeGetUser(uid, CMD_INSTANT_BATTLE, e.fd_, check_cmd[2]);
    if(user == NULL) return;
    check = safeLoadHero(user, CMD_INSTANT_BATTLE, e.fd_);
    if(check == false) return;
    check = safeLoadSoldier(user, CMD_INSTANT_BATTLE, e.fd_);
    if(check == false) return;
    check = safeLoadStageRecord(user, CMD_INSTANT_BATTLE, e.fd_);
    if(check == false) return;
    if (chapter_id > user->normal_chapter_id_ || (chapter_id == user->normal_chapter_id_ && stage_id > user->normal_stage_id_)) return;
    long long now = time(NULL);
    Stage * stage = preCheckCanBattle(user, STAGE_TYPE_NORMAL, chapter_id, stage_id, now, CMD_INSTANT_BATTLE, e.fd_);
    if(stage == NULL) return;
    int hero_limit = game_config.getValueFromIntVectorFilledZero(user->user_level_, game_config.hero_length_limit_);
    int soldier_limit = game_config.getValueFromIntVectorFilledZero(user->user_level_, game_config.soldier_length_limit_);
    if(hero_limit == -1 || soldier_limit == -1) return;
    if((int) user->heroes_.size() >= hero_limit + user->expand_hero_ * game_config.place_per_expand_hero_
       || (int) user->soldiers_.size()  >= soldier_limit + user->expand_soldier_ * game_config.place_per_expand_soldier_){
        sendErrorResponse(e.fd_, CMD_INSTANT_BATTLE, ERROR_BAG_IS_FULL);
        return;
    }
    user->stage_type_ = STAGE_TYPE_NORMAL;
    user->battle_chapter_id_ = chapter_id;
    user->battle_stage_id_ = stage_id;
    user->rewards_.clear();
    for(size_t i = 0;i<stage->rewards_.size();i++){
        int pby = rand()%PBY_PRECISION;
        if(pby<stage->rewards_[i].pby){
            user->rewards_.push_back(&(stage->rewards_[i]));
        }
    }
    modifyBattlereward(user->rewards_, game_config.normal_stage_reward_limit_);
    
    vector<StageReward * > rewards;
    rewards.clear();
    rewards = user->rewards_;
    check = safeLoadMission(user, CMD_INSTANT_BATTLE, e.fd_);
    if(check == false) return;
    Stage * stageConfig = game_config.getStage(user->stage_type_, user->battle_chapter_id_, user->battle_stage_id_);
    if(stageConfig == NULL){
        sendErrorResponse(e.fd_, CMD_INSTANT_BATTLE, ERROR_STAGE_NOT_EXIST);
        resetBattleInfo(user);
        return;
    }
    costEnergyForBattle(user, stage);
    addBasicBattleReward(user, stage,user->stage_type_);
    for(size_t i = 0;i<user->rewards_.size();i++){
        if(addStageReward(user, user->rewards_[i], now) == false){
            sendErrorResponse(e.fd_, CMD_INSTANT_BATTLE, ERROR_SYSTEM_CRITICAL_ERROR);
            resetBattleInfo(user);
            return;
        }
    }
    resetBattleInfo(user);
    check = dh_->saveUser(user);
    if(dh_->db_error_ != 0 || check == false){
        sendErrorResponse(e.fd_, CMD_INSTANT_BATTLE, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    LOG4CXX_INFO(logger_, "battleend;"<<uid<<";"<<user->stage_type_<<";"<<user->battle_chapter_id_<<";"
                 <<user->battle_stage_id_<<";"<<user->gold_<<";"<<user->energy_<<";"<<user->user_level_<<";"
                 <<user->user_exp_<<";"<<user->spark_num_<<";"<<user->spark_daily_num_);
    vector<Mission *> mv;
    mv.clear();
    updateMissionAfterBattle(user,mv);
    string res = buildInstantBattleResponse(user, mv, rewards);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
}
void EventHandler::processResetEliteStage(EventCmd &e, vector<string> &check_cmd) {
    if(check_cmd.size()!=6) return;
    uid_type uid;
    int type;
    int chapter_id;
    int stage_id;
    int cost;

    if (!safeAtoll(check_cmd[1], uid)
            || !safeAtoi(check_cmd[3], type)
            || !safeAtoi(check_cmd[4], chapter_id)
            || !safeAtoi(check_cmd[5], stage_id)) {
        return ;
    }

    User * user = safeGetUser(uid, CMD_RESET_ELITE_STAGE, e.fd_, check_cmd[2]);
    if(user == NULL) return;

    if (type == STAGE_TYPE_SPECIAL) {
        Stage* stage = game_config.getStage(type, chapter_id, stage_id);
        cost = stage->reset_cost_;
    }
    else {
        cost = game_config.elite_stage_reset_cost_;
    }

    if (cost > user->diamond_) {
        sendErrorResponse(e.fd_, CMD_RESET_ELITE_STAGE, ERROR_CANNOT_PAY_COST);
        return;
    }
    user->diamond_ -= cost;
    bool check = dh_->saveUser(user);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_RESET_ELITE_STAGE, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    check = resetEliteDaily(user, type, chapter_id, stage_id);
    if(check == false){
        sendErrorResponse(e.fd_, CMD_RESET_ELITE_STAGE, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    string res = buildResetEliteStageResponse(user);
    nh_->sendString(e.fd_, res);
    dh_->updateFresh(user);
}

void EventHandler::processInstantTrial(EventCmd &e, vector<string> &check_cmd) {
    uid_type uid;
    int stage_id;
    if (check_cmd.size() != 4
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoi(check_cmd[3], stage_id)) {
        return;
    }
    string &eqid = check_cmd[2];

    User *user = safeGetUser(uid, CMD_INSTANT_TRIAL, e.fd_, eqid);
    if (user == NULL) return;
    int succ = 0;
    long long timeleft = 0;
    if (user->trial_instant_start_time_ > 0) {
        succ = ERROR_TRIAL_INSTANTING;
    }
    else if (user->trial_stage_id_ > user->trial_max_stage_id_) {
        succ = ERROR_TRIAL_NO_INSTANT;
    }
    else {
        long long now = time(NULL);

        long long need_time = (user->trial_max_stage_id_ - user->trial_stage_id_ + 1) * game_config.trial_time_per_stage_;
    
        user->trial_instant_start_time_ = now;

        timeleft = need_time - (now - user->trial_instant_start_time_);;
    
        dh_->saveUserTrialInfo(user);

        hstring logstr(",");
        logstr<<uid<<user->trial_stage_id_<<user->trial_max_stage_id_<<now;
        LOG4CXX_DEBUG(logger_, "trialinstant: "<<logstr);
    }


    ostringstream ost;
    ost<<cmd_list[CMD_INSTANT_TRIAL]<<";"<<succ<<";"<<timeleft;
    nh_->sendString(e.fd_, ost.str().c_str(), ost.str().size());
    dh_->updateFresh(user);

}

void EventHandler::processTrialProgress(EventCmd &e, vector<string> &check_cmd) {
    uid_type uid;
    if (check_cmd.size() != 3
            || !safeAtoll(check_cmd[1], uid)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_TRIAL_PROGRESS, e.fd_, eqid);
    if (user == NULL) {
        return;
    }
    safeLoadStageRecord(user, CMD_TRIAL_PROGRESS, e.fd_);
    
    trialDailyCheck(user, 1);

    int succ = 0;
    long long timeleft = 0;

    if (user->trial_instant_start_time_ > 0) {

        long long now = time(NULL);
        long long need_time = (user->trial_max_stage_id_ - user->trial_stage_id_ + 1) * game_config.trial_time_per_stage_;
        timeleft = need_time - (now - user->trial_instant_start_time_);
        timeleft = max(timeleft, 0);

        if (timeleft == 0) {
            //TODO 1
            int srkey = getTrialChaptertoSave(1);
            map <int, StageRecord *>::iterator iter = user->stage_record_.find(srkey);
            if (iter == user->stage_record_.end()) {
                //以防数据库出错
                if (dh_->addNewStageRecord(user, srkey, user->trial_stage_id_)) {
                    iter = user->stage_record_.find(srkey);
                }
            }
            if (iter != user->stage_record_.end()) {
                StageRecord *sr = iter->second;
                if ((int) sr->record_.size() <= user->trial_max_stage_id_) {
                    sr->record_.resize(user->trial_max_stage_id_ + 1, 1);
                }
                for (size_t i = 1; i< sr->record_.size(); i++) {
                    sr->record_[i] = 1;
                }
                succ = 0;  

                vector<StageReward> streward;

                for (int i = user->trial_stage_id_; i <= user->trial_max_stage_id_; i++) {
                    Stage * stage = game_config.getTrialStage(1, i);
                    if (stage == NULL) {
                        continue;
                    }
                    for(size_t j = 0; j<stage->rewards_.size(); j++){
                        int pby = rand()%PBY_PRECISION;
                        if(pby<stage->rewards_[j].pby){
                            game_config.insertRewardStage(streward, stage->rewards_[j]);
                        }
                    }
                    StageReward str(ITEM_TYPE_GOLD, 0, stage->reward_gold_, 0);
                    game_config.insertRewardStage(streward, str);

                }
                string contents = buildUserRewardsContent(streward);
                string title="title";
                string text= itoa(MAIL_TYPE_TRIAL_INSTANT);
                //get mail text and title
                MailConfig *mc = game_config.getMailConfig(MAIL_TYPE_TRIAL_INSTANT);
                if (mc) {
                    title = mc->title;
                }
                dh_->addNewMail(user, SYSTEM_MAIL_SENDER_ID, title, text, 1, MAIL_TYPE_TRIAL_INSTANT, contents);

                user->trial_instant_start_time_ = 0;
                user->trial_stage_id_ = user->trial_max_stage_id_ + 1;

                hstring logstr(",");
                logstr<<uid<<user->trial_stage_id_<<user->trial_max_stage_id_<<user->trial_instant_start_time_;
                LOG4CXX_INFO(logger_, "trialinstantprize: "<<logstr<<","<<contents);
                
                if (!dh_->saveStageRecord(sr) || !dh_->saveUserTrialInfo(user)) {
                    LOG4CXX_ERROR(logger_, "trial;save trial info data failed");
                    //succ = ERROR_TRIAL_SAVE_STAGE_RECORD;
                }
            }
            else {
                succ = ERROR_SYSTEM_CRITICAL_ERROR;
            }
        }
    }

    ostringstream ost;
    ost<<cmd_list[CMD_TRIAL_PROGRESS]<<";"<<succ<<";"<<user->trial_stage_id_<<";"<<user->trial_max_stage_id_<<";"<<timeleft<<";"<<(game_config.trial_times_per_day_ - user->trial_daily_reset_);
    nh_->sendString(e.fd_, ost.str().c_str(), ost.str().size());
    dh_->updateFresh(user);
}

void EventHandler::processGetTrialRank(EventCmd &e, vector<string> &check_cmd) {
    uid_type uid;
    if (check_cmd.size() != 3
            || !safeAtoll(check_cmd[1], uid)){
        return;
    }
    //string &eqid = check_cmd[2];
    size_t num = dh_->trial_ranks_.size();
    ostringstream ost;
    ost<<cmd_list[CMD_GET_TRIAL_RANK]<<";0;"<<num<<";";
    for (size_t i = 0; i < num; i++) {
        long long uid = dh_->trial_ranks_[i].uid;
        User *user = dh_->getUser(uid);
        if (!user) {
            continue;
        }
        safeLoadHero(user, CMD_GET_TRIAL_RANK, e.fd_);
        map <long long, Hero *>::iterator iter = user->heroes_.find(user->hero_id_);
        if (iter == user->heroes_.end()) {
            continue;
        }
        Hero *hero = iter->second;
        ost<<uid<<","<<user->nick_name_<<","<<user->user_level_<<","<<user->trial_max_stage_id_<<","<<dh_->trial_ranks_[i].time\
            <<","<<hero->mid_<<","<<hero->level_<<","<<hero->star_<<";";
    }
    nh_->sendString(e.fd_, ost.str().c_str(), ost.str().size());
}

void EventHandler::processResetTrialStage(EventCmd &e, vector <string> &check_cmd) {
    uid_type uid;
    int chapter_id;
    if (check_cmd.size() != 4
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoi(check_cmd[3], chapter_id)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_TRIAL_RESET, e.fd_, eqid);
    if (user == NULL) {
        return;
    }
    safeLoadStageRecord(user, CMD_TRIAL_RESET, e.fd_);

    //TODO can not reset when instant
    int succ = 0;
    if (user->trial_instant_start_time_ > 0) {
        succ = ERROR_TRIAL_INSTANTING;
    }

    long long now = time(NULL);
    int srkey = getTrialChaptertoSave(chapter_id);

    map<int, StageRecord *>::iterator it = user->stage_record_.find(srkey);
    if(succ == 0 && it != user->stage_record_.end()){

        trialDailyCheck(user, chapter_id);

        StageRecord *sr = it->second;

        //trial daily limit
        if (user->trial_daily_reset_ >= game_config.trial_times_per_day_) {
            succ = ERROR_TRIAL_RESET_COUNT_ERROR;
        }
        else {
            for (size_t i = 1; i < sr->record_.size(); i++) {
                sr->record_[i] = 0;
            }
            user->trial_stage_id_ = 1;
            user->trial_daily_reset_ ++;
            dh_->saveUserTrialInfo(user);
            sr->update_time_ = now;
            dh_->saveStageRecord(sr);

            hstring logstr(",");
            logstr<<uid<<user->trial_stage_id_<<user->trial_max_stage_id_<<user->trial_daily_reset_;
            LOG4CXX_INFO(logger_, "trialreset: "<<logstr);
        }
    }
    else if (succ == 0 ) {
        succ = ERROR_TRIAL_RESET_NO_RECORD;
    }

    ostringstream ost;
    ost<<cmd_list[CMD_TRIAL_RESET]<<";"<<succ<<";"<<user->trial_stage_id_<<";"<<user->trial_max_stage_id_<<";"<<(game_config.trial_times_per_day_ - user->trial_daily_reset_);
    nh_->sendString(e.fd_, ost.str().c_str(), ost.str().size());
}

void EventHandler::processTrialRelive(EventCmd &e, vector <string> &check_cmd) {
    uid_type uid;
    int chid;
    int stid;
    if (check_cmd.size() != 5
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoi(check_cmd[3], chid)
            || !safeAtoi(check_cmd[4], stid)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_TRIAL_RELIVE, e.fd_, eqid);
    if (user == NULL) {
        return;
    }
    int succ = 0;
    if (chid != user->battle_chapter_id_ || stid != user->battle_stage_id_) {
        succ = ERROR_TRIAL_RELIVE_STAGE_ERROR;
    }
    else if (user->diamond_ <= 0) {
        succ = ERROR_TRIAL_RELIVE_DIAMOND_NOT_ENOUGH;
    }
    else {
        user->diamond_ --;
        //TODO log
        hstring logstr(",");
        logstr<<uid<<chid<<stid<<user->diamond_;
        LOG4CXX_INFO(logger_, "tiralrelive: "<<logstr);
    }
    ostringstream ost;
    ost<<cmd_list[CMD_TRIAL_RELIVE]<<";"<<succ<<";"<<user->diamond_;
    nh_->sendString(e.fd_, ost.str().c_str(), ost.str().size());
}


void EventHandler::sendErrorResponse(int fd, int cmd_code, int error_code)
{
    if(error_code == ERROR_SYSTEM_CRITICAL_ERROR){
        LOG4CXX_ERROR(logger_, "system;Connection to db closed");
    }
    string res = buildErrorResponse(cmd_code, error_code);
    nh_->sendString(fd,res);
}
void EventHandler::sendErrorResponseFroPayment(int fd,int cmd_code,int error_code,string &tran_id)
{
    if(error_code == ERROR_SYSTEM_CRITICAL_ERROR){
        LOG4CXX_ERROR(logger_, "system;Connection to db closed");
    }
    string res = buildErrorResponseForPayment(cmd_code, error_code,tran_id);
    nh_->sendString(fd,res);
}
User *  EventHandler::safeGetUser(uid_type uid,int cmd_code,int fd,string & eqid)
{
    User * user = dh_->getUser(uid);
    if(dh_->db_error_ !=0){
        sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        return NULL;
    }
    if(user == NULL){
        sendErrorResponse(fd, cmd_code,ERROR_USER_DONOT_EXIST);
        return NULL;
    }
    if(user->eqid_ != eqid){
        sendErrorResponse(fd, cmd_code,ERROR_AUTHORIZE_FAIL);
        return NULL;
    }
    bool check = safeLoadMission(user, cmd_code, fd);
    if(check == false){
        sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        return NULL;  
    }
    check = safeLoadFriend(user, cmd_code, fd);
    if(check == false) return NULL;
    if(dailyUpdateUser(user, cmd_code, fd)==false){
        sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        return NULL;
    }
    return user;
}
User * EventHandler::safeGetUser(uid_type uid, int cmd_code, int fd)
{
    User * user = dh_->getUser(uid);
    if(dh_->db_error_ !=0){
        sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        return NULL;
    }
    if(user == NULL){
        sendErrorResponse(fd, cmd_code,ERROR_USER_DONOT_EXIST);
        return NULL;
    }
    return user;
}
Hero * EventHandler::safeGetHero(User * user, long long hero_id, int cmd_code,int fd)
{
    map<long long , Hero *>::iterator it = user->heroes_.find(hero_id);
    if(it== user->heroes_.end()){
        sendErrorResponse(fd, cmd_code, ERROR_HERO_NOT_FOUND);
        return NULL;
    }
    return it->second;
}
Soldier * EventHandler::safeGetSoldier(User *user, long long soldier_id, int cmd_code, int fd)
{
    map<long long,Soldier *>::iterator it = user->soldiers_.find(soldier_id);
    if(it == user->soldiers_.end()){
        sendErrorResponse(fd, cmd_code, ERROR_SOLDIER_NOT_FOUND);
        return NULL;
    }
    return it->second;
}
FriendInfo * EventHandler::safeGetFriendInfo(User *user, uid_type fuid,int cmd_code,int fd)
{
    map<long long ,FriendInfo *>::iterator it = user->friends_.find(fuid);
    if(it == user->friends_.end()){
        if(fuid == FAKE_USER_ID){
            return NULL;
        }
        sendErrorResponse(fd, cmd_code, ERROR_NOT_FRIEND);
        return NULL;
    }
    return it->second;
}
FriendRequestInfo * EventHandler::safeGetFriendRequestInfo(User *user, uid_type fuid, int cmd_code, int fd)
{
    //map<long long ,FriendRequestInfo * >::iterator it = user->friend_requests_.find(fuid);
    map<long long ,FriendRequestInfo * >::iterator it;
    for(it = user->friend_requests_.begin();it!= user->friend_requests_.end();it++){
        if(it->second->fuid_ == fuid)break;
    }
    if(it == user->friend_requests_.end()){
        sendErrorResponse(fd, cmd_code, ERROR_NO_SUCH_REQUEST);
        return NULL;
    }
    return it->second;
}
Item * EventHandler::safeGetItem(User * user,int type,int mid)
{
    for(map<long long,Item * >::iterator it = user->items_.begin();
            it!=user->items_.end();it++){
        if(it->second->type_ == type && it->second->mid_ == mid){
            return it->second;
        }
    }
    return NULL;
}
Mail * EventHandler::safeGetMail(User * user,long long mail_id,int cmd_code,int fd)
{
    map<long long ,Mail*>::iterator it = user->mails_.find(mail_id);
    if(it==user->mails_.end()){
        sendErrorResponse(fd, cmd_code, ERROR_CANNOT_FIND_MAIL);
        return NULL;
    }
    return it->second;

}
Mission * EventHandler::safeGetMission(User * user, long long mission_id, int cmd_code, int fd)
{
    map<long long , Mission *>::iterator it = user->missions_.find(mission_id);
    if(it == user->missions_.end()){
        sendErrorResponse(fd, CMD_GETMISSIONREWARD, ERROR_CANNOT_FIND_MISSION);
        return NULL;
    }
    return it->second;
}
bool EventHandler::safeLoadHero(User * user, int cmd_code, int fd)
{
    if((user->load_staus_ & LOADED_HERO_INFO)!=0) return true;
    bool check = dh_->loadAllHeroes(user);
    if(dh_-> db_error_ != 0 || check == false){
        sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        return false;
    }
    return true;
}
bool EventHandler::safeLoadSoldier(User * user, int cmd_code, int fd)
{
    if((user->load_staus_ & LOADED_SOLDIER_INFO)!=0) return true;
    bool check = dh_->loadAllSoldiers(user);
    if(dh_-> db_error_ != 0 || check == false){
        sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        return false;
    }
    return true;
}
bool EventHandler::safeLoadItem(User * user, int cmd_code, int fd)
{
    if((user->load_staus_ & LOADED_ITEM_INFO)!=0) return true;
    bool check = dh_->loadAllItems(user);
    if(dh_-> db_error_ != 0 || check == false){
        sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        return false;
    }
    return true;
}
bool EventHandler::safeLoadFriend(User *user, int cmd_code, int fd)
{
    if((user->load_staus_ & LOADED_FRIEND_INFO)!=0)return true;
    bool check = dh_->loadAllFriends(user);
    if(dh_->db_error_ != 0 || check == false ){
        sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        return false;
    }
    return true;
}
bool EventHandler::safeLoadFriendRequest(User *user, int cmd_code, int fd)
{
    if((user->load_staus_ & LOADED_FRIENDREQUEST_INFO)!=0) return true;
    bool check = dh_->loadAllFriendRequest(user);
    if(dh_->db_error_ != 0 || check == false ){
        sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        return false;
    }
    return true;
}
bool EventHandler::safeLoadMail(User * user, int cmd_code,int fd)
{
    if((user->load_staus_ &LOADED_MAIL)!=0) return true;
    bool check = dh_->loadAllMails(user);
    if(check == false || dh_->db_error_!=0){
        sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        return false;
    }
    return true;
}
bool EventHandler::safeLoadMission(User * user,int cmd_code,int fd)
{
    if((user->load_staus_ & LOADED_MISSION)!=0) return true;
    bool check = dh_->loadAllMissions(user);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        return false;
    }
    return true;
}
bool EventHandler::safeLoadStageRecord(User *user,int cmd_code,int fd)
{
    if((user->load_staus_ & LOADED_STAGERECORD)!=0) return true;
    bool check = dh_->loadAllStageRecord(user);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        return false;
    }
    return true;
}
bool EventHandler::safeLoadAct(User * user,int cmd_code,int fd)
{
    if((user->load_staus_ & LOADED_ACT_RECORD)!=0) return true;
    bool check = dh_->loadAllAct(user);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        return false;
    }
    return true;
}
bool EventHandler::safeLoadTeam(User * user,int cmd_code,int fd)
{
    if((user->load_staus_ & LOADED_TEAM)!=0) return true;
    bool check = dh_->loadAllTeams(user);
    if(check == false || dh_->db_error_ != 0){
        sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        return false;
    }
    while((int)user->teams_.size()<game_config.max_team_num_){
        long long now =time(NULL);
        check = dh_->addNewTeam(user, now);
        if(check == false){
            sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        }
        if(user->teams_.size() == 1){
            for(map<long long ,Team*>::iterator it = user->teams_.begin();
                    it!= user->teams_.end();it++){
                Team * team = it->second;
                team->hero_id_ = user->hero_id_;
                team->soldier_id_.clear();
                for(size_t i = 0;i<user->soldier_id_.size();i++){
                    team->soldier_id_.push_back(user->soldier_id_[i]);
                }
                check = dh_->saveTeam(team);
                if(check == false){
                    sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
                    return false;
                }
                user->team_id_ = team->team_id_;
                check = dh_->saveUser(user);
                if(check == false){
                    sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
                    return false;
                }
            }
        }
    }
    return true;
}
bool EventHandler::preCheckLeadershipByChangeHero(User * user, Hero *hero)
{
    int leadership = game_config.leadershipLimit(user->user_level_);
    int count = 0;
    int ls_temp = game_config.heroLeadershipRequire(hero->mid_, hero->star_);
    if(ls_temp <0) return false;
    count += ls_temp;
    for(size_t i = 0;i<user->soldier_id_.size();i++){
        Soldier * soldier = dh_->getSoldier(user, user->soldier_id_[i]);
        if(soldier == NULL) return false;
        ls_temp = game_config.soldierLeadershipRequire(soldier->mid_, soldier->star_);
        if(ls_temp < 0){
            return false;
        }
        count += ls_temp;
    }
    if(count > leadership) return false;
    else return true;
}
bool EventHandler::preCheckLeadershipByChangeSoldier(User *user, Soldier *soldier, int pid)
{
    int leadership = game_config.leadershipLimit(user->user_level_);
    Hero * hero = dh_->getHero(user, user->hero_id_);
    if(hero == NULL) return false;
    int count = 0;
    int ls_temp = game_config.heroLeadershipRequire(hero->mid_, hero->star_);
    if(ls_temp <= 0) return false;
    count += ls_temp;
    for(size_t i = 0;i<user->soldier_id_.size();i++){
        Soldier * sp = dh_->getSoldier(user, user->soldier_id_[i]);
        if(sp == NULL) return false;
        if((int)i == pid) continue;
        ls_temp = game_config.soldierLeadershipRequire(sp->mid_, sp->star_);
        if(ls_temp <0)return false;
        count += ls_temp;
    }
    count += game_config.soldierLeadershipRequire(soldier->mid_, soldier->star_);
    if(count > leadership) return false;
    else return true;
}
bool EventHandler::checkEnergy(User *user,int energy_needed,long long now)
{
    int add = (int)(now - user->last_recover_time_)/game_config.recover_interval_;
    int check = 0;
    if(user->energy_ + add >= game_config.energy_level_limit_[user->user_level_]){
        check = game_config.energy_level_limit_[user->user_level_];
    }else{
        check = user->energy_ + add;
    }
    return (check >= energy_needed);
}
Stage *  EventHandler::preCheckCanBattle(User *user, int type, int chapter_id, int stage_id,long long now,int cmd_code,int fd)
{
    Stage * stage = NULL;
    if(type == STAGE_TYPE_NORMAL){
        stage = game_config.getNormalStage(chapter_id, stage_id);
        if(stage == NULL){
            sendErrorResponse(fd, cmd_code, ERROR_STAGE_NOT_EXIST);
            return NULL;
        }
        if(checkEnergy(user, stage->energy_required_, now) == false){
            sendErrorResponse(fd, cmd_code, ERROR_ENERGY_NOT_ENOUGH);
            return NULL;
        }
        if(user->user_level_<stage->level_required_){
            sendErrorResponse(fd, cmd_code, ERROR_LEVEL_NOT_ENOUGH);
            return NULL;
        }
        int uchapter_id = user->normal_chapter_id_;
        int ustage_id = user->normal_stage_id_;
        if(uchapter_id > chapter_id){
            return stage;
        }else if(uchapter_id == chapter_id){
            if(stage_id <= ustage_id + 1){
                return stage;
            }else{
                sendErrorResponse(fd, cmd_code, ERROR_STAGE_LOCKED);
                return NULL;
            }
        }else if(uchapter_id < chapter_id){
            if(chapter_id != uchapter_id + 1
                    || stage_id != 1
                    || (int) game_config.stages_[uchapter_id].size() - 1 > ustage_id){
                sendErrorResponse(fd, cmd_code, ERROR_STAGE_LOCKED);
                return NULL;
            }
            return stage;
        }

    }else if(type == STAGE_TYPE_ELITE){
        stage = game_config.getEliteStage(chapter_id, stage_id);
        if(stage == NULL){
            sendErrorResponse(fd, cmd_code, ERROR_STAGE_NOT_EXIST);
            return NULL;
        }
        if(eliteDailyCheck(user, chapter_id, stage_id, game_config.elite_stage_daily_limit_) == false){
            sendErrorResponse(fd, cmd_code, ERROR_GOT_LIMIT);
            return NULL;
        }

        if(checkEnergy(user, stage->energy_required_, now) == false){
            sendErrorResponse(fd, cmd_code, ERROR_ENERGY_NOT_ENOUGH);
            return NULL;
        }
        if(user->user_level_<stage->level_required_){
            sendErrorResponse(fd, cmd_code, ERROR_LEVEL_NOT_ENOUGH);
            return NULL;
        }
        bool check = false;
        if(user->elite_chapter_id_ > chapter_id){
            check = true;
        }else if(user->elite_chapter_id_ == chapter_id){
            if(stage_id <= user->elite_stage_id_ + 1){
                check = true;
            }
        }else if(user->elite_chapter_id_ < chapter_id){
            if(chapter_id != user->elite_chapter_id_+1
                    || stage_id != 1
                    || (int) game_config.elite_stages_[user->elite_chapter_id_].size()-1 > user->elite_stage_id_){
                check = false;
            }else{
                check = true;
            }
        }
        if(check == false){
            return NULL;
        }
        if(user->normal_chapter_id_ > stage->depend_chapter_){
            check = true;
        }else if(user->normal_chapter_id_ == stage->depend_chapter_){
            /*if(user->normal_stage_id_ >= stage->depend_stage_){
              check = true;
              }else{
              check = false;
              }*/
            if(user->normal_stage_id_ >= (int) game_config.stages_[user->normal_chapter_id_].size()-1){
                check =  true;
            }else{
                check = false;
            }
        }else{
            check = false;
        }
        if(check == true) return stage;
        else return NULL;
    }else if(type == STAGE_TYPE_SPECIAL){
        stage = game_config.getSpecialStage(chapter_id,stage_id);
        if(stage == NULL){
            sendErrorResponse(fd, cmd_code, ERROR_STAGE_NOT_EXIST);
            return NULL;
        }
        if(specialTimeCheck(user, stage, now) == false){
            sendErrorResponse(fd, cmd_code, ERROR_TIME_NOT_FIT);
            return NULL;
        }
        if(specialDailyCheck(user, chapter_id, stage_id, stage->ssc_->times_)==false)
        {
            sendErrorResponse(fd, cmd_code, ERROR_GOT_LIMIT);
            return NULL;
        }
        if(checkEnergy(user, stage->energy_required_, now) == false){
            sendErrorResponse(fd, cmd_code, ERROR_ENERGY_NOT_ENOUGH);
            return NULL;
        }
        if(user->user_level_<stage->level_required_){
            sendErrorResponse(fd, cmd_code, ERROR_LEVEL_NOT_ENOUGH);
            return NULL;
        }
        return stage;
    }
    else if (type == STAGE_TYPE_TRIAL) {
        //TODO 
        stage = game_config.getTrialStage(chapter_id, stage_id);
        if (stage == NULL) {
            sendErrorResponse(fd, cmd_code, ERROR_STAGE_NOT_EXIST);
            return NULL;
        }
        if (stage_id != user->trial_stage_id_) {
            sendErrorResponse(fd, cmd_code, ERROR_STAGE_NOT_EXIST);
            return NULL;
        }
        if (user->trial_instant_start_time_ > 0) {
            sendErrorResponse(fd, cmd_code, ERROR_TRIAL_INSTANTING);
            return NULL;
        }
        return stage;
    }

    return NULL;
}
void EventHandler::costEnergyForBattle(User *user, Stage *stage)
{
    long long now = time(NULL);
    int add = (int)(now - user->last_recover_time_)/game_config.recover_interval_;
    if(user->energy_ + add >= game_config.energy_level_limit_[user->user_level_]){
        user->energy_ = game_config.energy_level_limit_[user->user_level_];
        user->last_recover_time_ = now;
    }else{
        user->energy_ += add;
        user->last_recover_time_ += add * game_config.recover_interval_;
    }
    user->energy_ -= stage->energy_required_;
    LOG4CXX_INFO(logger_, "cost energy;"<<stage->energy_required_<<";"<<user->energy_);
}
void EventHandler::addBasicBattleReward(User *user, Stage *stage,int type)
{
    user->gold_ += stage->reward_gold_;
    //user->user_exp_ += stage->reward_exp_;
    if(type == STAGE_TYPE_NORMAL){
        user->user_exp_ += int(user->user_level_ * game_config.normal_stage_exp_rate_);
    }else if(type == STAGE_TYPE_ELITE){
        user->user_exp_ += int(user->user_level_ * game_config.elite_stage_exp_rate_);
    }else if(type == STAGE_TYPE_SPECIAL){
        user->user_exp_ += stage->reward_exp_;
    }
    while(user->user_exp_ >= game_config.exp_level_limit_[user->user_level_]){
        if(user->user_level_ >= (int) game_config.exp_level_limit_.size() - 1){
            user->user_exp_ = 0;
            break;
        }else{
            user->user_exp_ -= game_config.exp_level_limit_[user->user_level_];
            user->user_level_ ++;
            user->energy_ += game_config.recharge_level_limit_[user->user_level_];
            user->energy_ = min(game_config.energy_level_limit_[user->user_level_],user->energy_);
        }
    }
}
void EventHandler::setStageInfo(User *user)
{
    if(user->stage_type_ == STAGE_TYPE_NORMAL){
        if(user->normal_chapter_id_ > user->battle_chapter_id_)
            return ;
        else if(user->normal_chapter_id_ == user->battle_chapter_id_){
            if(user->normal_stage_id_ >= user->battle_stage_id_)
                return;
            else{
                user->normal_stage_id_ = user->battle_stage_id_;
            }
        }else if(user->normal_chapter_id_ < user->battle_chapter_id_){
            user->normal_chapter_id_ = user->battle_chapter_id_;
            user->normal_stage_id_ = user->battle_stage_id_;
        }
    }else if(user->stage_type_ == STAGE_TYPE_ELITE){
        if(user->elite_chapter_id_ > user->battle_chapter_id_)
            return ;
        else if(user->elite_chapter_id_ == user->battle_chapter_id_){
            if(user->elite_stage_id_ >= user->battle_stage_id_)
                return;
            else{
                user->elite_stage_id_ = user->battle_stage_id_;
            }
        }else if(user->elite_chapter_id_ < user->battle_chapter_id_){
            user->elite_chapter_id_ = user->battle_chapter_id_;
            user->elite_stage_id_ = user->battle_stage_id_;
        }

    }
}
bool EventHandler::addStageReward(User *user, StageReward * rew,long long now)
{
    if(rew->type == ITEM_TYPE_HERO){
        bool check = dh_->addNewHero(user, rew->subtype, rew->param_1, rew->param_2, now);
        if(check == true){
            LOG4CXX_INFO(logger_, "addnewhero;"<<user->uid_<<";"<<rew->subtype<<";"<<rew->param_1<<";"<<rew->param_2);
        }
        return check;
    }else if(rew->type == ITEM_TYPE_SOLDIER){
        bool check = dh_->addNewSoldier(user, rew->subtype, rew->param_1, rew->param_2, now);
        if(check == true){
            LOG4CXX_INFO(logger_, "addnewsoldier;"<<user->uid_<<";"<<rew->subtype<<";"<<rew->param_1<<";"<<rew->param_2);
        }
        return check;
    }else if(rew->type == ITEM_TYPE_RESOURCE || rew->type == ITEM_TYPE_CHIP){
        bool check = dh_->addItem(user, rew->type, rew->subtype, 1, now);
        if(check == true){
            Item * item = safeGetItem(user, rew->type, rew->subtype);
            if(item != NULL){
                LOG4CXX_INFO(logger_, "addnewitem;"<<user->uid_<<";"<<item->amount_);
            }
        }
        return check;
    }else if(rew->type == ITEM_TYPE_GOLD){
        user->gold_ += rew->param_1;
        bool check = dh_->saveUser(user);
        if(check == true){
            LOG4CXX_INFO(logger_, "addgold;"<<user->uid_<<";"<<user->gold_);
        }
        return check;
    }else if(rew->type == ITEM_TYPE_DIAMOND){
        user->diamond_ += rew->param_1;
        bool check = dh_->saveUser(user);
        if(check == true){
            LOG4CXX_INFO(logger_, "adddiamond;"<<user->uid_<<";"<<user->diamond_);
        }
        return check;
    }
    return true;
}
void EventHandler::resetBattleInfo(User *user)
{
    user->stage_type_ = 0;
    user->battle_chapter_id_ = 0;
    user->battle_stage_id_  = 0;
    user->assistant_id_ = 0;
    user->rewards_.clear();
}
bool EventHandler::dailyUpdateUser(User *user,int cmd_code,int fd)
{
    long long now = time(NULL);
    if(dateChange(now, user->last_login_time_)==false){
        return true;
    }

    if(user->last_login_time_ == 0){
        bool check = initMission(user);
        if(check == false) return false;
    }

    user->last_login_time_ = now;
    user->spark_daily_num_ = 0;
    user->send_energy_daily_limit_ = 0;
    user->receive_energy_daily_limit_ = 0;
    user->login_sum_ ++;
    user->buy_energy_daily_ = 0;

    FriendInfo * fr = safeGetFriendInfo(user, FAKE_USER_ID, cmd_code, fd);
    if(fr != NULL){
        fr->last_receive_time_ = now;
        fr->accepted_ = HAVE_ENERGY_TO_ACCEPT;
        bool check = dh_->saveFriendInfo(fr);
        if(check == false) return false;
    }

    for(map<long long ,Mission*>::iterator it = user->missions_.begin();
            it!= user->missions_.end();it++){
        if(it->second->mission_type_ == MISSION_LOGIN_SUM){
            it->second->mission_record_ = user->login_sum_;
            MissionConfig * mc = game_config.getMissionConfig(it->second->mission_type_, it->second->mission_subtype_);
            if(mc == NULL) return false;
            if(user->login_sum_ >= mc->param){
                it->second->complete_ = MISSION_COMPLETED;
            }
            bool check = dh_->saveMission(it->second);
            if(check == false) return false;
        }
    }

    bool check = dh_->saveUser(user);
    if(dh_->db_error_ != 0 || check == false){
        sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
        return false;
    }
    addDailyFreshMission(user);
    Act * act = getAct(user, ACT_3);
    if(act != NULL){
        long long last_now = act->record_[0];
        if(dateChange(now-86400, last_now) == false){
            act->record_[0] = now;
            act->record_[1] ++;
            int reward_id = max(0,int(game_config.act_3_reward_.size() - 1));
            if(act->record_[2] >= 1<<reward_id){
                act->record_[2] -= 1<<reward_id;
            }
        }else{
            act->record_[0] = now;
            act->record_[1] = 1;
            act->record_[2] = 0;
        }
        check = dh_->saveAct(act);
        if(dh_->db_error_ != 0 || check == false){
            sendErrorResponse(fd, cmd_code, ERROR_SYSTEM_CRITICAL_ERROR);
            return false;
        }
    }

    return true;
}
bool EventHandler::addAssistantReward(User *user, User *assistant)
{
    if(assistant == NULL) return true;
    bool is_friend = false;
    if(user->friends_.find(assistant->uid_)!=user->friends_.end()){
        is_friend = true;
    }
    if(is_friend == false){
        user->spark_num_ = min(user->spark_num_+game_config.spark_assistant_is_friend_,game_config.spark_sum_limit_);
        user->spark_daily_num_ = min(user->spark_daily_num_ + game_config.spark_assistant_is_friend_,\
                game_config.spark_daily_limit_);
        //assistant->spark_num_ = min(assistant->spark_num_+game_config.spark_assistant_is_friend_,game_config.spark_sum_limit_);
        //assistant->spark_daily_num_ = min(assistant->spark_daily_num_ + game_config.spark_assistant_is_friend_,game_config.spark_daily_limit_);
    }else{
        user->spark_num_ = min(user->spark_num_+game_config.spark_assistant_not_friend_,game_config.spark_sum_limit_);
        user->spark_daily_num_ = min(user->spark_daily_num_ + game_config.spark_assistant_not_friend_,\
                game_config.spark_daily_limit_);
        //assistant->spark_num_ = min(assistant->spark_num_+game_config.spark_assistant_not_friend_,game_config.spark_sum_limit_);
        //assistant->spark_daily_num_ = min(assistant->spark_daily_num_ + game_config.spark_assistant_not_friend_,game_config.spark_daily_limit_);
    }
    /*bool check = dh_->saveUser(assistant);
      if(check == false || dh_->db_error_ != 0){
      return false;
      }else{
      return true;
      }*/
    return true;
}
bool EventHandler::checkCost(User *user, EnhanceCost &ec)
{
    if(ec.type == ITEM_TYPE_RESOURCE){
        for(map<long long,Item *>::iterator it = user->items_.begin();
                it!=user->items_.end();it++){
            if(it->second->type_ == ITEM_TYPE_RESOURCE
                    && it->second->mid_ == ec.mid
                    && it->second->amount_ >= ec.param){
                return true;
            }
        }
        return false;
    }else if(ec.type == ITEM_TYPE_GOLD){
        if(user->gold_ >= ec.param)
            return true;
        else
            return false;
    }
    return false;
}
bool EventHandler::safeCost(User *user, EnhanceCost &ec,int cmd_code,int fd)
{
    if(ec.type == ITEM_TYPE_RESOURCE){
        map<long long,Item *>::iterator it = user->items_.begin();
        for(;it!= user->items_.end();it++){
            if(it->second->type_ == ec.type
                    && it->second->mid_ == ec.mid){
                break;
            }
        }
        it->second->amount_ -= ec.param;
        bool check = dh_->saveItem(it->second);
        if(check == false || dh_->db_error_ != 0){
            sendErrorResponse(fd, cmd_code,ERROR_SYSTEM_CRITICAL_ERROR);
            return false;
        }
    }else if(ec.type == ITEM_TYPE_GOLD){
        user->gold_ -= ec.param;
        bool check = dh_->saveUser(user);
        if(check == false || dh_->db_error_ != 0){
            sendErrorResponse(fd, cmd_code,ERROR_SYSTEM_CRITICAL_ERROR);
            return false;
        }
    }
    return true;
}
bool EventHandler::sendSystemMail(User *user)
{
    long long now = time(NULL);
    bool is_dirty = false;
    for(size_t i = 0;i<game_config.mail_config_.size();i++){
        if(now > game_config.mail_config_[i].end_time || now < game_config.mail_config_[i].start_time)continue;
        int mid = game_config.mail_config_[i].mid;
        //trial instant prize 
        if (mid == MAIL_TYPE_TRIAL_INSTANT) continue;
        int position = ((mid-1)/SYSTEM_MAIL_RECORD_INTERVAL)%SYSTEM_MAIL_RECORD_LENGTH + 1;
        int offset = (mid - 1)%SYSTEM_MAIL_RECORD_INTERVAL;
        if((mid-1)/SYSTEM_MAIL_RECORD_INTERVAL > user->system_mail_record_[0]){
            user->system_mail_record_[position] = 0;
            user->system_mail_record_[0] ++;
            is_dirty = true;
        }
        if((user->system_mail_record_[position] & 1<<offset)!=0) continue;
        int acceptable ;
        if(game_config.mail_config_[i].reward.size() > 0){
            acceptable = 1;
        }else{
            acceptable = 0;
        }
        string text = itoa(mid);
        string str="";
        bool check = dh_->addNewMail(user, SYSTEM_MAIL_SENDER_ID, game_config.mail_config_[i].title, text, acceptable, 0, str);
        if(check == false || dh_->db_error_ != 0){
            return false;
        }
        user->system_mail_record_[position] |= 1<<offset;
        is_dirty = true;
    }
    if(is_dirty == true){
        bool check = dh_->saveUser(user);
        if(check == false ||dh_->db_error_ != 0){
            return false;
        }
    }
    return true;
}

bool EventHandler::addDailyFreshMission(User * user)
{
    if(addSendEnergyMission(user) == false) return false;
    if(addDailyRewardMission(user) == false) return false;
    return true;
}
bool EventHandler::addSendEnergyMission(User *user)
{
    for(map<long long ,Mission*>::iterator it = user->missions_.begin();
            it!=user->missions_.end();){
        if(it->second->mission_type_ == MISSION_SEND_ENERGY ){
            it->second->add_time_ = time(NULL);
            it->second->mission_record_ = 0;
            it->second->complete_ = MISSION_NOT_COMPLETED;
            return dh_->saveMission(it->second);
        }else{
            it++;
        }
    }
    for(size_t i = 0 ;i<game_config.mission_config_[MISSION_SEND_ENERGY].size();i++){
        bool check = dh_->addNewMission(user,
                MISSION_SEND_ENERGY,
                game_config.mission_config_[MISSION_SEND_ENERGY][i].subtype,
                0,
                MISSION_NOT_COMPLETED);
        if(check == false) return false;
    }
    return true;
}
bool EventHandler::addDailyRewardMission(User *user)
{
    for(map<long long ,Mission*>::iterator it = user->missions_.begin();
            it!=user->missions_.end();){
        if(it->second->mission_type_ == MISSION_DAILY_REWARD ){
            return true;
        }else{
            it++;
        }
    }
    for(size_t i = 0 ;i<game_config.mission_config_[MISSION_DAILY_REWARD].size();i++){
        bool check = dh_->addNewMission(user,
                MISSION_DAILY_REWARD,
                game_config.mission_config_[MISSION_DAILY_REWARD][i].subtype,
                1,
                MISSION_COMPLETED);
        if(check == false) return false;
    }
    return true;
}
bool EventHandler::addLinkMission(User * user, MissionConfig *mc,int param)
{
    if(mc->link_id == MISSION_NO_MISSION_TO_LINK) return true;
    MissionConfig * nmc = game_config.getMissionConfig(mc->type, mc->link_id);
    if(nmc == NULL) return true;
    if(nmc->type == MISSION_LEVEL_UP){
        bool check = dh_->addNewMission(user, MISSION_LEVEL_UP, nmc->subtype, user->user_level_, int(user->user_level_>=nmc->param));
        if(check == false) return false;
    }else if(nmc->type == MISSION_NORMAL_CHAPTER_CLEAR){
        int chapter_r = user->normal_chapter_id_;
        if(user->normal_stage_id_ < (int) game_config.stages_[chapter_r].size() - 1){
            chapter_r --;
        }
        bool check = dh_->addNewMission(user, MISSION_NORMAL_CHAPTER_CLEAR, nmc->subtype, chapter_r, int(chapter_r>=nmc->param));
        if(check == false) return false;
    }else if(nmc->type == MISSION_ELITE_CHAPTER_CLEAR){
        int chapter_r = user->elite_chapter_id_;
        if(user->elite_stage_id_ < (int) game_config.elite_stages_[chapter_r].size() - 1){
            chapter_r --;
        }
        bool check = dh_->addNewMission(user, MISSION_ELITE_CHAPTER_CLEAR, nmc->subtype, chapter_r, int(chapter_r>=nmc->param));
        if(check == false) return false;
    }else if(nmc->type == MISSION_LOGIN_SUM){
        user->login_sum_ -= mc->param;
        bool check = dh_->saveUser(user);
        if(check == false) return false;
        check = dh_->addNewMission(user, MISSION_LOGIN_SUM, nmc->subtype, user->login_sum_, int(user->login_sum_>=nmc->param));
        if(check == false) return false;
    }else if(nmc->type == MISSION_ADD_FRIEND){
        bool check = dh_->addNewMission(user, MISSION_ADD_FRIEND, nmc->subtype, param, int(param>=nmc->param));
        if(check == false) return false;
    }
    return true;
}
bool EventHandler::initMission(User *user)
{
    bool check = true;
    check = dh_->addNewMission(user, MISSION_LEVEL_UP, 0, 1, MISSION_NOT_COMPLETED);
    if(check == false) return false;
    check = dh_->addNewMission(user, MISSION_NORMAL_CHAPTER_CLEAR, 0, 0, MISSION_NOT_COMPLETED);
    if(check == false) return false;
    check = dh_->addNewMission(user, MISSION_ELITE_CHAPTER_CLEAR, 0, 0, MISSION_NOT_COMPLETED);
    if(check == false) return false;
    check = dh_->addNewMission(user, MISSION_LOGIN_SUM, 0, 0, MISSION_NOT_COMPLETED);
    if(check == false) return false;
    check = dh_->addNewMission(user, MISSION_ADD_FRIEND, 0, 0, MISSION_NOT_COMPLETED);
    if(check == false) return false;
    return true;
}
bool EventHandler::updateMissionAfterBattle(User *user,vector<Mission *> &mv)
{
    for(map<long long ,Mission*>::iterator it = user->missions_.begin();
            it!=user->missions_.end();it++){
        if(it->second->mission_type_ == MISSION_LEVEL_UP){
            if(user->user_level_ > it->second->mission_record_){
                it->second->mission_record_ = user->user_level_;
                MissionConfig * mc = game_config.getMissionConfig(it->second->mission_type_, it->second->mission_subtype_);
                if(user->user_level_ >= mc->param && it->second->complete_ == MISSION_NOT_COMPLETED){
                    it->second->complete_ = MISSION_COMPLETED;
                    mv.push_back(it->second);
                }
                bool check = dh_->saveMission(it->second);
                if(check == false) return false;
            }
        }else if(it->second->mission_type_ == MISSION_NORMAL_CHAPTER_CLEAR){
            /*if(user->normal_chapter_id_ > it->second->mission_record_){
              it->second->mission_record_ = user->normal_chapter_id_;
              MissionConfig * mc = game_config.getMissionConfig(it->second->mission_type_, it->second->mission_subtype_);
              if(user->normal_chapter_id_ >= mc->param && it->second->complete_ == MISSION_NOT_COMPLETED){
              it->second->complete_ = MISSION_COMPLETED;
              mv.push_back(it->second);
              }
              bool check = dh_->saveMission(it->second);
              if(check == false) return false;
              }*/
            int uchapter_id = user->normal_chapter_id_;
            if(user->normal_stage_id_ == (int) game_config.stages_[uchapter_id].size() - 1){
                MissionConfig * mc = game_config.getMissionConfig(it->second->mission_type_, it->second->mission_subtype_);
                it->second->mission_record_ = uchapter_id;
                if(uchapter_id == mc->param && it->second->complete_ == MISSION_NOT_COMPLETED){
                    it->second->complete_ = MISSION_COMPLETED;
                    mv.push_back(it->second);
                }
                bool check = dh_->saveMission(it->second);
                if(check == false) return false;
            }

        }else if(it->second->mission_type_ == MISSION_ELITE_CHAPTER_CLEAR){
            /*if(user->elite_chapter_id_ > it->second->mission_record_){
              it->second->mission_record_ = user->elite_chapter_id_;
              MissionConfig * mc = game_config.getMissionConfig(it->second->mission_type_, it->second->mission_subtype_);
              if(user->elite_chapter_id_ >= mc->param && it->second->complete_ == MISSION_NOT_COMPLETED){
              it->second->complete_ = MISSION_COMPLETED;
              mv.push_back(it->second);
              }
              bool check = dh_->saveMission(it->second);
              if(check == false) return false;
              }*/
            int uchapter_id = user->elite_chapter_id_;

            if(user->elite_stage_id_ == (int) game_config.elite_stages_[uchapter_id].size() - 1){
                MissionConfig * mc = game_config.getMissionConfig(it->second->mission_type_, it->second->mission_subtype_);
                it->second->mission_record_ = uchapter_id;
                if(uchapter_id == mc->param && it->second->complete_ == MISSION_NOT_COMPLETED){
                    it->second->complete_ = MISSION_COMPLETED;
                    mv.push_back(it->second);
                }
                bool check = dh_->saveMission(it->second);
                if(check == false) return false;
            }

        }
    }
    return true;
}
bool EventHandler::eliteDailyCheck(User * user, int chapter_id, int stage_id,int times)
{
    long long now = time(NULL);
    map<int ,StageRecord *>::iterator it = user->stage_record_.find(chapter_id);
    if(it != user->stage_record_.end()){
        if(dateChange(now, it->second->update_time_)==true){
            return true; 
        }
        if(stage_id >= (int) it->second->record_.size()){
            return true;
        }
        if(it->second->record_[stage_id] >= times){
            return false;
        }
    }
    return true;
}
bool EventHandler::specialDailyCheck(User * user, int chapter_id, int stage_id,int times)
{
    long long now = time(NULL);
    map<int ,StageRecord *>::iterator it = user->stage_record_.find(getSpecialChaptertoSave(chapter_id));
    if(it != user->stage_record_.end()){
        if(dateChange(now, it->second->update_time_)==true){
            return true;
        }
        if(stage_id >= (int) it->second->record_.size()){
            return true;
        }
        if(it->second->record_[stage_id] >= times){
            return false;
        }
    }
    return true;
}
bool EventHandler::setEliteDaily(User * user,int chapter_id ,int stage_id)
{
    if(chapter_id == 0 || stage_id == 0 ) return false;
    long long now = time(NULL);
    map<int, StageRecord *>::iterator it = user->stage_record_.find(chapter_id);
    bool check = false;
    if(it == user->stage_record_.end()){
        bool check = dh_->addNewStageRecord(user, chapter_id, stage_id);
        if(check == false) return false;
        it = user->stage_record_.find(chapter_id);
    }
    if(it != user->stage_record_.end()){
        while(stage_id>= (int) it->second->record_.size()){
            it->second->record_.push_back(0);
        }
        if(dateChange(now, it->second->update_time_)==true){
            for(size_t i = 1;i<it->second->record_.size();i++){
                it->second->record_[i] = 0;
            }
        }
        it->second->record_[stage_id] ++ ;
        it->second->update_time_ = now;
    }
    check = dh_->saveStageRecord(it->second);
    if(check == false) return false;
    return true;

}

bool EventHandler::setSpecialDaily(User * user,int chapter_id ,int stage_id)
{
    if(chapter_id == 0 || stage_id == 0 ) return false;
    long long now = time(NULL);
    map<int, StageRecord *>::iterator it = user->stage_record_.find(getSpecialChaptertoSave(chapter_id));
    bool check = false;
    if(it == user->stage_record_.end()){
        bool check = dh_->addNewStageRecord(user, getSpecialChaptertoSave(chapter_id), stage_id);
        if(check == false) return false;
        it = user->stage_record_.find(getSpecialChaptertoSave(chapter_id));
    }
    if(it != user->stage_record_.end()){
        while(stage_id>= (int) it->second->record_.size()){
            it->second->record_.push_back(0);
        }
        if(dateChange(now, it->second->update_time_)==true){
            for(size_t i = 1;i<it->second->record_.size();i++){
                it->second->record_[i] = 0;
            }
        }
        it->second->record_[stage_id] ++ ;
        it->second->update_time_ = now;
    }
    check = dh_->saveStageRecord(it->second);
    if(check == false) return false;
    return true;

}

bool EventHandler::setTrialDaily(User *user, int chapter_id, int stage_id) {
    if (chapter_id == 0 || stage_id == 0) {
        return false;
    }
    long long now = time(NULL);
    int srkey = getTrialChaptertoSave(chapter_id);
    map <int, StageRecord *>::iterator it = user->stage_record_.find(srkey);
    if (it == user->stage_record_.end()) {
        if (dh_->addNewStageRecord(user, srkey, stage_id) == false) {
            return false;
        }
        it = user->stage_record_.find(srkey);
    }
    if (it != user->stage_record_.end()) {
        StageRecord *sr = it->second;

        int new_stage = 0;
        while (stage_id >= (int) sr->record_.size()) {
            sr->record_.push_back(1);
            new_stage = 1;
        }
        sr->record_[stage_id] = 1;
        sr->update_time_ = now;

        user->trial_stage_id_ = stage_id + 1;

        if (stage_id > user->trial_max_stage_id_) {
            user->trial_max_stage_id_ = stage_id;
            dh_->updateTrialRank(user->uid_, stage_id, now);
        }
            
        dh_->saveUserTrialInfo(user);

        return dh_->saveStageRecord(sr);
    }
    return false;
}

bool EventHandler::resetEliteDaily(User * user, int type, int chapter_id ,int stage_id)
{
    if(chapter_id == 0 || stage_id == 0 ) return false;
    long long now = time(NULL);
    int key = (type == STAGE_TYPE_ELITE)? chapter_id : getSpecialChaptertoSave(chapter_id);
    map<int, StageRecord *>::iterator it = user->stage_record_.find(key);
    bool check = false;
    if(it == user->stage_record_.end()){
        return false;
    }
    if(it != user->stage_record_.end()){
        while(stage_id >= (int) it->second->record_.size()){
            it->second->record_.push_back(0);
        }
        if(dateChange(now, it->second->update_time_)==true){
            for(size_t i = 1;i<it->second->record_.size();i++){
                it->second->record_[i] = 0;
            }
        }
        it->second->record_[stage_id] = 0 ;
        it->second->update_time_ = now;
    }
    check = dh_->saveStageRecord(it->second);
    if(check == false) return false;
    return true;
}

void EventHandler::trialDailyCheck(User *user, int chapter_id) {

    int srkey = getTrialChaptertoSave(chapter_id);

    map <int, StageRecord *>::iterator iter = user->stage_record_.find(srkey);
    if (iter == user->stage_record_.end()) {
        return;
    } 
    StageRecord *sr = iter->second;
    
    long long now = time(NULL);
    bool datechange = dateChange(now, sr->update_time_);
    if (datechange) {
        user->trial_daily_reset_ = 0;
        dh_->saveUserTrialInfo(user);
    }
    return ;
}

/*
int EventHandler::resetTrialStage(User * user, int chapter_id) {
    if(chapter_id == 0 || stage_id == 0 ) return -1;

    int succ = 1;
    long long now = time(NULL);

    int srkey = getChaptertoSaveKey(STAGE_TYPE_TRIAL, chapter_id);

    map<int, StageRecord *>::iterator it = user->stage_record_.find(srkey);
    if(it != user->stage_record_.end()){

        trialDailyCheck(user);

        StageRecord *sr = it->second;

        //trial daily limit
        if (user->trial_daily_reset_ >= game_config.trial_daily_reset_limit_) {
            return ERROR_TRIAL_RESET_COUNT_ERROR;
        }
        for (size_t i = 0; i < sr->record_.size(); i++) {
            sr->record_[i] = 0;
        }

        sr->update_time_ = now;

        if (dh_->saveStageRecord(sr)) {
            succ = 0;
        }
    }
    return 1;
}
*/

bool EventHandler::specialTimeCheck(User *user, Stage *stage, long long now)
{
    SpecialStageCondition * ssc = stage->ssc_;
    if(ssc == NULL)return false;
    if(now <ssc->start_ || now > ssc->end_) return false;
    int day = ((now - game_config.time_area_fix_)%(86400*7))/86400;
    bool check = false;
    for(size_t i = 0;i<ssc->day_.size();i++){
        if(ssc->day_[i] == day){
            check = true;
            break;
        }
    }
    if(check == false) return false;
    if(((now - game_config.time_area_fix_)%(86400*ssc->interval_ + 1))/86400 != 0) return false;
    int hour = ((now - game_config.time_area_fix_)%86400)/3600;
    check = false;
    for(size_t i = 0;i<ssc->moment_.size();i++){
        if(ssc->moment_[i].first <=hour && ssc->moment_[i].second > hour){
            check =true;
            break;
        }
    }
    if(check == false) return false;
    return true;
}
bool EventHandler::dateChange(long long now, long long comp)
{
    if((now - game_config.time_area_fix_)/86400 != (comp - game_config.time_area_fix_)/86400) return true;
    else return false;
}
bool EventHandler::teamPrepare(User * user,long long hero_id,vector<long long> &soldier_id, Hero *&hero, vector<Soldier *> &soldiers)
{
    if(hero_id==0) return false;
    for(size_t i = 0;i<soldier_id.size();i++){
        if(soldier_id[i]==0) return false;
    }
    map<long long , Hero *>::iterator it = user->heroes_.find(hero_id);
    if(it==user->heroes_.end()) return false;
    hero = it->second;
    soldiers.clear();
    for(size_t i = 0;i<soldier_id.size();i++){
        map<long long,Soldier *>::iterator it = user->soldiers_.find(soldier_id[i]);
        if(it == user->soldiers_.end()) return false;
        soldiers.push_back(it->second);
    }
    return true;
}
bool EventHandler::preCheckLeadership(User *user, Hero *hero, vector<Soldier *> &soldiers)
{
    int leadership = game_config.leadershipLimit(user->user_level_);
    int count = 0;
    int ls_temp = game_config.heroLeadershipRequire(hero->mid_, hero->star_);
    if(ls_temp <0) return false;
    count += ls_temp;
    for(size_t i = 0;i<soldiers.size();i++){
        ls_temp = game_config.soldierLeadershipRequire(soldiers[i]->mid_, soldiers[i]->star_);
        if(ls_temp < 0){
            return false;
        }
        count += ls_temp;
    }
    if(count > leadership) return false;
    else return true;
}

int EventHandler::getChaptertoSaveKey(int type, int chapter_id) {
    int ret = chapter_id;
    if (type == STAGE_TYPE_SPECIAL) {
        ret = getSpecialChaptertoSave(chapter_id);
    }
    else if (type == STAGE_TYPE_TRIAL) {
        ret = getTrialChaptertoSave(chapter_id);
    }
    return ret;
}

int EventHandler::getSpecialChaptertoSave(int chapter_id)
{
    return chapter_id + 10000;
}

int EventHandler::getTrialChaptertoSave(int chapter_id) {
    return chapter_id + 20000;
}

Team * EventHandler::inTeamHero(User *user, long long hero_id)
{
    for(map<long long,Team*>::iterator it = user->teams_.begin();
            it!= user->teams_.end();it++){
        if(hero_id == it->second->hero_id_) return it->second;
    }
    return NULL;
}
Team * EventHandler::inTeamSoldier(User *user, long long soldier_id)
{
    for(map<long long,Team*>::iterator it = user->teams_.begin();
            it!= user->teams_.end();it++){
        for(size_t i = 0 ;i<it->second->soldier_id_.size();i++){
            if(it->second->soldier_id_[i] == soldier_id) return it->second;
        }
    }
    return NULL;
}
void EventHandler::modifyBattlereward(vector<StageReward *> &rewards, int limit)
{
    if((int) rewards.size() <= limit) return;
    while((int) rewards.size()>limit){
        int amount = int(rewards.size());
        int p = rand()%amount;
        rewards[p] = rewards[amount-1];
        rewards.resize(amount-1);
    }
}
Act * EventHandler::getAct(User *user, int act_id)
{
    ActConfig * ac = game_config.getActConfig(act_id);
    if(ac == NULL) return NULL;

    map<int,Act *>::iterator it = user->act_record_.find(act_id);
    if(it == user->act_record_.end()){
        bool check = dh_->addNewActRecord(user, act_id);
        if(check == false) return NULL;
    }
    it = user->act_record_.find(act_id);

    if(it->second->version_ != ac->version_){
        initAct(it->second,ac->version_);
    }
    return it->second;
}

void EventHandler::act_1(User * user,int amount)
{
    if(user->pay_count_ == 1){
        user->diamond_ += amount *2;
    }
    return;
}
void EventHandler::initAct(Act * act,int version)
{
    if(act == NULL) return;
    switch(act->act_id_){
        case ACT_2:
            initAct2(act,version);
            break;
        case ACT_3:
            initAct3(act,version);
            break;
        default:
            break;
    }
}
void EventHandler::initAct2(Act * act,int version)
{
    act->record_.clear();
    act->record_.push_back(0LL);
    act->version_ = version;
}

void EventHandler::act_2(uid_type uid, vector<string> &cmd, int fd)
{
    if(cmd.size()!=5)return;
    int reward_id ;
    bool check = safeAtoi(cmd[4], reward_id);
    if(check == false) return;
    User * user =safeGetUser(uid, CMD_ACT, fd, cmd[2]);
    if(user == NULL) return;
    check = safeLoadAct(user, CMD_ACT, fd);
    if(reward_id <0 || reward_id >= (int) game_config.act_2_level_.size()) return;
    Act * act = getAct(user, ACT_2);
    if(act == NULL){
        sendErrorResponse(fd, CMD_ACT, ERROR_ACT_NOT_AVAIABLE);
        return;
    }
    if(user->user_level_<game_config.act_2_level_[reward_id]){
        sendErrorResponse(fd, CMD_ACT, ERROR_NOT_ACCEPTABLE);
        return;
    }
    if(((act->record_[0]) & (1<<reward_id)) != 0){
        sendErrorResponse(fd, CMD_ACT, ERROR_NOT_ACCEPTABLE);
        return;
    }
    act->record_[0] |= (1<<reward_id);
    check = dh_->saveAct(act);
    if(check == false){
        sendErrorResponse(fd, CMD_ACT, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    long long now = time(NULL);
    for(size_t i = 0;i<game_config.act_2_reward_[reward_id].size();i++){
        check = addStageReward(user, &game_config.act_2_reward_[reward_id][i], now);
        if(check == false){
            sendErrorResponse(fd, CMD_ACT, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    string res = buildAct2Response(reward_id, act);
    nh_->sendString(fd, res);
    return;
}

void EventHandler::initAct3(Act *act, int version)
{
    act->record_.clear();
    long long now = time(NULL);
    act->record_.push_back(now);
    act->record_.push_back(0LL);
    act->record_.push_back(0LL);
    act->version_ = version;
}

void EventHandler::act_3(uid_type uid, vector<string> &cmd, int fd)
{
    if(cmd.size()!=5)return;
    int reward_id ;
    bool check = safeAtoi(cmd[4], reward_id);
    if(check == false) return;
    User * user =safeGetUser(uid, CMD_ACT, fd, cmd[2]);
    if(user == NULL) return;
    check = safeLoadAct(user, CMD_ACT, fd);
    if(reward_id <0 || reward_id >= (int) game_config.act_3_level_.size()) return;
    Act * act = getAct(user, ACT_3);
    if(act == NULL){
        sendErrorResponse(fd, CMD_ACT, ERROR_ACT_NOT_AVAIABLE);
        return;
    }
    if(act->record_[1]<game_config.act_3_level_[reward_id]){
        sendErrorResponse(fd, CMD_ACT, ERROR_NOT_ACCEPTABLE);
        return;
    }
    if(((act->record_[2]) & (1<<reward_id)) != 0){
        sendErrorResponse(fd, CMD_ACT, ERROR_NOT_ACCEPTABLE);
        return;
    }
    act->record_[2] |= 1<<reward_id;
    check = dh_->saveAct(act);
    if(check == false){
        sendErrorResponse(fd, CMD_ACT, ERROR_SYSTEM_CRITICAL_ERROR);
        return;
    }
    long long now = time(NULL);
    for(size_t i = 0;i<game_config.act_3_reward_[reward_id].size();i++){
        check = addStageReward(user, &game_config.act_3_reward_[reward_id][i], now);
        if(check == false){
            sendErrorResponse(fd, CMD_ACT, ERROR_SYSTEM_CRITICAL_ERROR);
            return;
        }
    }
    string res = buildAct3Response(reward_id, act);
    nh_->sendString(fd, res);
    return;
}

size_t writeDataCurl(void *buffer, size_t size, size_t nmemb, void *userp)
{
    ((string *) userp)->append((char*)buffer,size* nmemb);
    return size * nmemb;
}


