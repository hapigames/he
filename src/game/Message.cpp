//
//  Message.cpp
//  hero
//
//  Created by xujianfeng on 13-8-13.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//
#include "Message.h"

string buildErrorResponse(int cmd_code,int error_index)
{
    ostringstream ost;
    ost<<cmd_list[cmd_code]<<";"<<error_index;
    return ost.str();
}
string buildErrorResponseForPayment(int cmd_code,int error_index,string &tran_id)
{
    ostringstream ost;
    ost<<cmd_list[cmd_code]<<";"<<error_index<<";"<<tran_id;
    return ost.str();
}
string buildRegisterResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_REGISTER]<<";"<<0;
    return ost.str();
}
string buildLoginResponse(uid_type uid,int auth_code)
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOGIN]<<";0;"<<uid<<";"<<auth_code;
    return ost.str();
}
string buildAuthorizeResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_AUTHORIZE]<<";"<<0;
    return ost.str();
}
string buildCheckNickResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_CHECKNICK]<<";"<<0;
    return ost.str();
}
string buildLoadAllResponse(User * user)
{
    ostringstream ost;
    ost<<buildLoadUserResponse(user)<<":"<<buildLoadHeroResponse(user)<<":"<<buildLoadSoldierResponse(user);
    return ost.str();
}
string buildLoadUserResponse(User * user)
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOADUSER]<<";0;"<<user->uid_<<";"<<user->nick_name_<<";"<<user->user_level_\
        <<";"<<user->user_exp_<<";"<<user->energy_<<";"<<user->gold_<<";"<<user->diamond_\
        <<";"<<user->last_recover_time_<<";"<<user->hero_id_<<";"<<vectorLongToString(user->soldier_id_)\
        <<";"<<user->normal_chapter_id_<<";"<<user->normal_stage_id_<<";"<<user->elite_chapter_id_<<";"<<user->elite_stage_id_\
        <<";"<<time(NULL)<<";"<<user->spark_num_<<";"<<user->receive_energy_daily_limit_\
    <<";"<<user->send_energy_daily_limit_<<";"<<user->expand_hero_<<";"<<user->expand_soldier_<<";"<<user->expand_friend_<<";"<<user->guide_record_<<";";
    
    int srcount = 0;
    for(map<int,StageRecord *>::iterator it = user->stage_record_.begin();
        it!=user->stage_record_.end();it++){
        if(it->second->record_.size() <= 1){
            continue;
        }
        for(size_t i = 1;i<it->second->record_.size();i++){
            if(it->second->record_[i]!=0){
                srcount ++;
            }
        }
    }
    ost<<srcount<<";";
    for(map<int,StageRecord *>::iterator it = user->stage_record_.begin();
        it!=user->stage_record_.end();it++){
        if(it->second->record_.size() <= 1){
            continue;
        }
        for(size_t i = 1;i<it->second->record_.size();i++){
            if(it->second->record_[i]!=0){
                ost<<it->second->record_[0]<<","<<i<<","<<it->second->record_[i]<<","<<it->second->update_time_<<";";
            }
        }
    }
    ost<<user->buy_energy_daily_<<";"<<user->pay_count_<<";"<<user->team_id_;
    return ost.str();
}
string buildLoadHeroResponse(User * user)
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOADHERO]<<";0;"<<user->heroes_.size()<<";";
    for(map<long long,Hero *>::iterator it = user->heroes_.begin();
        it!=user->heroes_.end();it++){
        Hero * hero = it->second;
        if(hero == NULL) continue;
        ost<<hero->hero_id_<<","<<hero->get_time_<<","<<hero->mid_<<","<<hero->exp_\
            <<","<<hero->level_<<","<<hero->star_<<","<<hero->skill_[0]<<","<<hero->skill_[1]\
            <<","<<hero->skill_[2]<<","<<hero->locked_<<";";
        
    }
    return ost.str();
}
string buildLoadSoldierResponse(User * user)
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOADSOLDIER]<<";0;"<<user->soldiers_.size()<<";";
    for(map<long long,Soldier *>::iterator it = user->soldiers_.begin();
        it!=user->soldiers_.end();it++){
        Soldier * soldier= it->second;
        if(soldier == NULL) continue;
        ost<<soldier->soldier_id_<<","<<soldier->get_time_<<","<<soldier->mid_<<","<<soldier->exp_\
        <<","<<soldier->level_<<","<<soldier->star_<<","<<soldier->locked_<<";";
    }
    return ost.str();
}
string buildLoadItemResponse(User * user,int item_type)
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOADITEM]<<";0;";
    int count = 0;
    for(map<long long,Item *>::iterator it = user->items_.begin();
        it!=user->items_.end();it++){
        Item * item= it->second;
        if(item == NULL) continue;
        if(item->type_!=item_type) continue;
        count ++;
    }
    ost<<count<<";";
    for(map<long long,Item *>::iterator it = user->items_.begin();
        it!=user->items_.end();it++){
        Item * item= it->second;
        if(item == NULL) continue;
        if(item->type_!=item_type) continue;
        ost<<item->type_<<","<<item->item_id_<<","<<item->mid_<<","<<item->amount_<<";";
    }
    return ost.str();
}
string buildChangeHeroResponse(User * user)
{
    ostringstream ost;
    ost<<cmd_list[CMD_CHANGEHERO]<<";0;"<<user->hero_id_;
    return ost.str();
}
string buildChangeSoldierResponse(User * user)
{
    ostringstream ost;
    ost<<cmd_list[CMD_CHANGESOLDIER]<<";0;"<<vectorLongToString(user->soldier_id_);
    return ost.str();
}
string buildCreateUserResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_CREATEUSER]<<";0";
    return ost.str();
}
string buildLoadAssistantResponse(User * user,vector<User *> &assistants,int friend_index,vector<Hero *> &heroes)
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOADASSISTANT]<<";0;"<<(user->spark_daily_num_ < game_config.spark_daily_limit_)<<";"<<assistants.size()<<";";
    for(size_t i =0;i<assistants.size();i++){
        User * fresh = assistants[i];
        if(fresh == NULL) continue;
        ost<<fresh->uid_<<","<<fresh->nick_name_<<",";
        if((int)i<friend_index){
            ost<<1<<",";
        }else{
            ost<<0<<",";
        }
        ost<<fresh->user_level_<<",";
        ost<<heroes[i]->mid_<<","<<heroes[i]->level_<<","<<heroes[i]->star_<<","<<heroes[i]->skill_[0]<<","<<heroes[i]->skill_[1]\
                <<","<<heroes[i]->skill_[2]<<";";
        
    }
    return ost.str();
}
string buildBattleStartResponse(User * user)
{
    ostringstream ost;
    ost<<cmd_list[CMD_BATTLESTART]<<";0;"<<user->rewards_.size()<<";";
    for(size_t i = 0;i<user->rewards_.size();i++){
        StageReward * sr = user->rewards_[i];
        if(sr == NULL) continue;
        ost<<sr->type<<","<<sr->subtype;
        if(sr->type == ITEM_TYPE_GOLD || sr->type == ITEM_TYPE_DIAMOND){
            ost<<","<<sr->param_1;
        }
        if(sr->type == ITEM_TYPE_HERO || sr->type == ITEM_TYPE_SOLDIER){
            ost<<","<<sr->param_1<<","<<sr->param_2;
        }
        ost<<";";
    }
    return ost.str();
}

string buildUserRewardsContent(User *user) {
    ostringstream ost;
    ost<<user->rewards_.size()<<";";
    for (size_t i = 0; i < user->rewards_.size(); i++) {
        StageReward *sr = user->rewards_[i];
        if (sr == NULL) continue;
        ost<<sr->type<<","<<sr->subtype<<","<<sr->param_1<<","<<sr->param_2<<";";
    }
    return ost.str();
}

string buildUserRewardsContent(vector <StageReward> &rewards) {
    ostringstream ost;
    ost<<rewards.size()<<";";

    for (size_t i = 0; i < rewards.size(); i++) {
        StageReward *sr = &rewards[i];
        if (sr == NULL) continue;
        ost<<sr->type<<","<<sr->subtype<<","<<sr->param_1<<","<<sr->param_2<<";";
    }

    return ost.str();
}


int parseUserRewardsContent(vector <StageReward>& strewards, const string &contents) {

    strewards.clear();

    int ret = 0;
    vector <string> tokens;
    string delims=";";
    tokenize(contents,tokens, delims);
    if (tokens.size() > 0 && safeAtoi(tokens[0], ret)) {
        strewards.resize(ret);    
        delims = ",";
        for (size_t i = 1; i < tokens.size(); i++) {
            vector <int> subitems;
            numtokenize(tokens[i], subitems, delims); 
            if (subitems.size() == 4) {
                strewards[i-1].type = subitems[0];
                strewards[i-1].subtype = subitems[1];
                strewards[i-1].param_1 = subitems[2];
                strewards[i-1].param_2 = subitems[3];
            }
        }
    }
    return ret;
}


string buildBattleEndResponse(User * user,vector<Mission *> &mv)
{
    ostringstream ost;
    ost<<cmd_list[CMD_BATTLEEND]<<";0;"<<user->uid_<<";"<<user->user_level_\
        <<";"<<user->user_exp_<<";"<<user->energy_<<";"<<user->gold_<<";"<<user->diamond_\
        <<";"<<user->last_recover_time_<<";"<<user->normal_chapter_id_<<";"<<user->normal_stage_id_\
        <<";"<<user->elite_chapter_id_<<";"<<user->elite_stage_id_<<";"<<user->spark_num_;
    ost<<";"<<mv.size()<<";";
    for(size_t i = 0;i<mv.size();i++){
        ost<<mv[i]->mission_id_<<","<<mv[i]->mission_type_<<","<<mv[i]->mission_subtype_<<";";
    }
    return ost.str();
}
string buildInstantBattleResponse(User* user, vector<Mission*> &mv, vector<StageReward*> &rv)
{
    ostringstream ost;
    ost<<cmd_list[CMD_INSTANT_BATTLE]<<";0;"<<user->uid_<<";"<<user->user_level_\
    <<";"<<user->user_exp_<<";"<<user->energy_<<";"<<user->gold_<<";"<<user->diamond_\
    <<";"<<user->last_recover_time_<<";"<<user->normal_chapter_id_<<";"<<user->normal_stage_id_\
    <<";"<<user->elite_chapter_id_<<";"<<user->elite_stage_id_<<";"<<user->spark_num_;
    ost<<";"<<rv.size()<<";";
    for(size_t i = 0;i<rv.size();i++){
        StageReward * sr = rv[i];
        if(sr == NULL) continue;
        ost<<sr->type<<","<<sr->subtype;
        if(sr->type == ITEM_TYPE_HERO || sr->type == ITEM_TYPE_SOLDIER){
            ost<<","<<sr->param_1<<","<<sr->param_2;
        }
        ost<<";";
    }
    ost<<mv.size()<<";";
    for(size_t i = 0;i<mv.size();i++){
        ost<<mv[i]->mission_id_<<","<<mv[i]->mission_type_<<","<<mv[i]->mission_subtype_<<";";
    }
    return ost.str();
}
string buildResetEliteStageResponse(User* user) {
    ostringstream ost;
    ost<<cmd_list[CMD_RESET_ELITE_STAGE]<<";0;"<<user->diamond_;
    return ost.str();
}
/*
string buildInstantTrialResponse(long long now, User *user) {
    ostringstream ost;
    long long timeleft = game_config.trial_instant_interval_ - (now - user->trial_instant_start_time_);
    timeleft = max(timeleft, 0);
    ost<<cmd_list[CMD_INSTANT_TRIAL]<<";0;"<<timeleft;
    return ost.str();
}
*/

string buildFriendRequestResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_FRIENDREQUEST]<<";0";
    return ost.str();
}
string buildSellHeroResponse(User * user)
{
    ostringstream ost;
    ost<<cmd_list[CMD_SELLHERO]<<";0;"<<user->gold_;
    return ost.str();
}
string buildSellSoldierResponse(User * user)
{
    ostringstream ost;
    ost<<cmd_list[CMD_SELLSOLDIER]<<";0;"<<user->gold_;
    return ost.str();
}
string buildLockHeroResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOCKHERO]<<";0";
    return ost.str();
}
string buildLockSoldierResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOCKSOLDIER]<<";0";
    return ost.str();
}
string buildHeroExpUpResponse(User * user,Hero * hero)
{
    ostringstream ost;
    ost<<cmd_list[CMD_HEROEXPUP]<<";0;"<<hero->level_<<";"<<hero->exp_<<";"<<user->gold_;
    return ost.str();
}
string buildSoldierExpUpResponse(User * user,Soldier * soldier)
{
    ostringstream ost;
    ost<<cmd_list[CMD_SOLDIEREXPUP]<<";0;"<<soldier->level_<<";"<<soldier->exp_<<";"<<user->gold_;
    return ost.str();
}
string buildHeroEnhanceResponse(User * user,Hero * hero)
{
    ostringstream ost;
    ost<<cmd_list[CMD_HEROENHANCE]<<";0;"<<hero->star_<<";"<<user->gold_;
    return ost.str();
}
string buildSoldierEnhanceResponse(User * user,Soldier * soldier)
{
    ostringstream ost;
    ost<<cmd_list[CMD_SOLDIERENHANCE]<<";0;"<<soldier->star_<<";"<<user->gold_;
    return ost.str();
}
string buildSkillLevelUpResponse(User * user,Hero * hero,int position)
{
    ostringstream ost;
    ost<<cmd_list[CMD_SKILLLEVELUP]<<";0;"<<hero->skill_[position-1]<<";"<<user->gold_;
    return ost.str();
}
string buildSendEnergyResponse(User * user,vector<Mission *> &mv)
{
    ostringstream ost;
    ost<<cmd_list[CMD_SENDENERGY]<<";0;"<<user->send_energy_daily_limit_;
    ost<<";"<<mv.size()<<";";
    for(size_t i = 0;i<mv.size();i++){
        ost<<mv[i]->mission_id_<<","<<mv[i]->mission_type_<<","<<mv[i]->mission_subtype_<<";";
    }
    return ost.str();
}
string buildAcceptEnergyResponse(User * user)
{
    ostringstream ost;
    ost<<cmd_list[CMD_ACCEPTENERGY]<<";0;"<<user->receive_energy_daily_limit_;
    return ost.str();
}
string buildLoadFriendResponse(vector<User *> &users,vector<FriendInfo *> &frs,vector<Hero *> &heroes)
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOADFRIEND]<<";0;"<<users.size()<<";";
    for(size_t i = 0;i<users.size();i++){
        ost<<users[i]->uid_<<","<<users[i]->nick_name_<<","<<users[i]->user_level_<<","\
            <<users[i]->last_active_time_<<","\
            <<frs[i]->accepted_<<","<<frs[i]->last_receive_time_<<","<<frs[i]->last_send_time_<<",";
        ost<<heroes[i]->mid_<<","<<heroes[i]->level_<<","<<heroes[i]->star_<<";";
    }
    return ost.str();
}
string buildLoadFriendRequestResponse(vector<User *> &users,vector<Hero *> &heroes)
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOADFRIENDREQUEST]<<";0;"<<users.size()<<";";
    for(size_t i = 0;i<users.size();i++){
        ost<<users[i]->uid_<<","<<users[i]->nick_name_<<","<<users[i]->user_level_<<",";
        if(users[i]->last_active_time_!= 0){
            ost<<users[i]->last_active_time_<<",";
        }else{
            ost<<users[i]->last_login_time_<<",";
        }
        ost<<heroes[i]->mid_<<","<<heroes[i]->level_<<","<<heroes[i]->star_<<";";
    }
    return ost.str();
}
string buildAddFriendResponse(vector<Mission *> &mv)
{
    ostringstream ost;
    ost<<cmd_list[CMD_ADDFRIEND]<<";0";
    ost<<";"<<mv.size()<<";";
    for(size_t i = 0;i<mv.size();i++){
        ost<<mv[i]->mission_id_<<","<<mv[i]->mission_type_<<","<<mv[i]->mission_subtype_<<";";
    }
    return ost.str();
}
string buildRefuseFriendReqestResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_REFUSEFRIENDREQ]<<";0";
    return ost.str();
}
string buildDeleteFriendResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_DELETEFRIEND]<<";0";
    return ost.str();
}
string buildSoulMergeResponse(User * user)
{
    ostringstream ost;
    ost<<cmd_list[CMD_SOULMERGE]<<";0;"<<user->gold_;
    return ost.str();
}
string buildLoadUserByIdResponse(User * user,Hero * hero,vector<Soldier *> sv)
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOADUSERBYID]<<";0;"<<user->nick_name_<<";"<<user->user_level_<<";"\
    <<user->last_login_time_<<";";
    ost<<hero->hero_id_<<","<<hero->get_time_<<","<<hero->mid_<<","<<hero->exp_\
    <<","<<hero->level_<<","<<hero->star_<<","<<hero->skill_[0]<<","<<hero->skill_[1]\
    <<","<<hero->skill_[2]<<","<<hero->locked_<<";";
    for(size_t i =0;i<sv.size();i++){
        Soldier * soldier = sv[i];
        ost<<soldier->soldier_id_<<","<<soldier->get_time_<<","<<soldier->mid_<<","<<soldier->exp_\
        <<","<<soldier->level_<<","<<soldier->star_<<","<<soldier->locked_<<";";
    }
    return ost.str();
}
string buildLoadMailResponse(User * user,vector<Mail *> &mtmp,vector<User * > &utmp,vector<Hero * > &htmp)
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOADMAIL]<<";0;"<<user->mails_.size()<<";";
    for(map<long long,Mail *>::iterator it = user->mails_.begin();
        it!=user->mails_.end();it++){
        if(it->second->sender_id_ == SYSTEM_MAIL_SENDER_ID){
            ost<<it->second->mail_id_<<","<<it->second->sender_id_<<","<<it->second->title_<<","\
                <<it->second->haveread_<<","<<it->second->send_time_<<","<<it->second->acceptable_<<";";
        }else{
            ost<<it->second->mail_id_<<","<<it->second->sender_id_<<","<<it->second->title_<<","\
                <<it->second->haveread_<<","<<it->second->send_time_<<","<<it->second->acceptable_;
            for(size_t i = 0;i<mtmp.size();i++){
                if(mtmp[i] == it->second){
                    ost<<","<<htmp[i]->mid_<<","<<htmp[i]->star_<<","<<htmp[i]->level_;
                    break;
                }
            }
            ost<<";";
        }
    }
    return ost.str();
}
string buildOpenMailResponse(Mail * mail)
{
    ostringstream ost;
    ost<<cmd_list[CMD_OPENMAIL]<<";0;"<<mail->mail_id_<<";"<<mail->sender_id_<<";"<<mail->text_<<";"<<mail->contents<<";";
    return ost.str();
}

string buildDeleteMailResponse(long long mail_id)
{
    ostringstream ost;
    ost<<cmd_list[CMD_DELETEMAIL]<<";0;"<<mail_id;
    return ost.str();
}
string buildSendMailResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_SENDMAIL]<<";0";
    return ost.str();
}
string buildAcceptMailItemResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_ACCEPTMAILITEM]<<";0";
    return ost.str();
}
string buildGetNewMailNumResponse(int num)
{
    ostringstream ost;
    ost<<cmd_list[CMD_GETNEWMAILNUM]<<";0;"<<num;
    return ost.str();
}
string buildLoadMissionResponse(User * user)
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOADMISSION]<<";0;"<<user->missions_.size()<<";";
    for(map<long long ,Mission *>::iterator it = user->missions_.begin();
        it!=user->missions_.end();it++){
        ost<<it->second->mission_id_<<","<<it->second->mission_type_\
        <<","<<it->second->mission_subtype_<<","<<it->second->mission_record_\
        <<","<<it->second->complete_<<";";
    }
    return ost.str();
}
string buildGetCMNumResponse(int num)
{
    ostringstream ost;
    ost<<cmd_list[CMD_GETCMNUM]<<";0;"<<num;
    return ost.str();
}
string buildGetMissionRewardResponse(User * user,long long mission_id)
{
    ostringstream ost;
    ost<<cmd_list[CMD_GETMISSIONREWARD]<<";0;"<<mission_id<<";"<<user->missions_.size()<<";";
    for(map<long long ,Mission *>::iterator it = user->missions_.begin();
        it!=user->missions_.end();it++){
        ost<<it->second->mission_id_<<","<<it->second->mission_type_\
        <<","<<it->second->mission_subtype_<<","<<it->second->mission_record_\
        <<","<<it->second->complete_<<";";
    }
    return ost.str();
}
string buildBuyEnergyResponse(int times)
{
    ostringstream ost;
    ost<<cmd_list[CMD_BUYENERGY]<<";0;"<<times;
    return ost.str();
}
string buildExpandHeroResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_EXPANDHERO]<<";0";
    return ost.str();
}
string buildExpandSoldierResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_EXPANDSOLDIER]<<";0";
    return ost.str();
}
string buildExpandFriendResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_EXPANDFRIEND]<<";0";
    return ost.str();
}
string buildSummonResponse(User * user,vector<StageReward *>v)
{
    ostringstream ost;
    ost<<cmd_list[CMD_SUMMON]<<";0;"<<v.size()<<";";
    for(size_t i = 0;i<v.size();i++){
        StageReward * sr = v[i];
        if(sr == NULL) continue;
        ost<<sr->type<<","<<sr->subtype;
        if(sr->type == ITEM_TYPE_HERO || sr->type == ITEM_TYPE_SOLDIER){
            ost<<","<<sr->param_1<<","<<sr->param_2;
        }
        ost<<";";
    }
    return ost.str();
}
string buildLoadNewsResponse(vector<News *> nv)
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOADNEWS]<<";0;"<<nv.size()<<";";
    for(size_t i = 0;i<nv.size();i++){
        ost<<nv[i]->news_id<<","<<nv[i]->name<<","<<nv[i]->action<<","<<nv[i]->param_1<<","<<nv[i]->param_2<<","<<nv[i]->param_3<<";";
    }
    return ost.str();
}
string buildPaymentVerifyReceivedResponse(uid_type uid,string &product_id,string & tran_id,int diamond)
{
    ostringstream ost;
    //ost<<cmd_list[CMD_PAYMENTVERIFYRECEIVED]<<";0;"<<uid<<";"<<product_id<<";"<<tran_id<<";"<<diamond;
    ost<<cmd_list[CMD_PAYMENTVERIFYRECEIVED]<<";0;"<<tran_id<<";"<<diamond;
    return ost.str();
}
string buildPaymentVerifyReceivedResponse(User * user)
{
    ostringstream ost;
    ost<<cmd_list[CMD_PAYMENTVERIFYRECEIVED]<<";0;"<<user->diamond_;
    return ost.str();
}
string buildAdminResponse()
{
    ostringstream ost;
    ost<<"admin;0";
    return ost.str();
}
string buildLogResponse()
{
    ostringstream ost;
    ost<<"log;0";
    return ost.str();
}
string buildGuideResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_GUIDE]<<";0";
    return ost.str();
}
string buildAlterUserNameResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_ALTERUSERNAME]<<";0";
    return ost.str();
}
string buildVerifyString(vector<string> &check_cmd)
{
    ostringstream ost;
    for(size_t i = 3;i<check_cmd.size();i++){
        ost<<check_cmd[i];
    }
    return ost.str();
}
string buildReceiptCmd(uid_type uid,int status,string &product_id, string& tran_id,int server_id)
{
    ostringstream ost;
    ost<<cmd_list[CMD_PAYMENTVERIFYCOMPLETE]<<";"<<uid<<";"<<status<<";"<<product_id<<";"<<tran_id<<";"<<server_id;
    return ost.str();
}
string buildPaymentaddDiamondRequestCmd(uid_type uid,string &product_id,string & tran_id,int fd)
{
    ostringstream ost;
    ost<<cmd_list[CMD_PAYMENT_ADD_DIAMOND_REQUEST]<<";"<<uid<<";"<<product_id<<";"<<tran_id<<";"<<fd;
    return ost.str();
}
string buildPaymentAddDiamondResponseCmd(int succ,uid_type uid,string &product_id,string & tran_id,int diamond,int fd)
{
    ostringstream ost;
    ost<<cmd_list[CMD_PAYMENT_ADD_DIAMOND_RESPONSE]<<";"<<succ<<";"<<uid<<";"<<product_id<<";"<<tran_id<<";"<<diamond<<";"<<fd;
    return ost.str();
}
string buildSessionVerifyString(vector<string> &cmd){
    ostringstream ost;
    ostringstream ost2;
    long long appid = APPID_91;
    string appkey = APPKEY_91;
    ost2<<appid<<4<<cmd[1]<<cmd[3]<<appkey;
    string sign = ost2.str();
    ost<<VERIFYADDR_91<<"?AppId="<<appid<<"&Act=4&Uin="<<cmd[1]<<"&Sign="<<MDString(sign.c_str())<<"&SessionID="<<cmd[3];
    return ost.str();
}
string buildSessionVerifyCmd(long long uid,int status,string &nickname,string &eqid)
{
    ostringstream ost;
    ost<<cmd_list[CMD_SESSION_VERIFY]<<";"<<uid<<";"<<status<<";"<<nickname<<";"<<eqid;
    return ost.str();
}
string buildSessionVerifyResponse()
{
    ostringstream ost;
    ost<<"login;0";
    return ost.str();
}
string buildPBakResponse(User * user,NewPaymentInfo * npi)
{
    ostringstream ost;
    ost<<cmd_list[CMD_PBAK]<<";0;"<<npi->tran_id_<<";"<<user->diamond_<<";"<<user->pay_count_;
    return ost.str();
}

string buildLoadActFailResponse(int succ,int act_id)
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOADACT]<<";"<<succ<<";"<<act_id;
    return ost.str();
}
string buildLoadActResponse(int act_id,string &s)
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOADACT]<<";0;"<<act_id<<";"<<s;
    return ost.str();
}
string buildAct2Response(int reward_id,Act * act)
{
    ostringstream ost;
    ost<<cmd_list[CMD_ACT]<<";0;"<<ACT_2<<";"<<reward_id<<";"<<vectorLongToString(act->record_);
    return ost.str();
}
string buildAct3Response(int reward_id,Act * act)
{
    ostringstream ost;
    ost<<cmd_list[CMD_ACT]<<";0;"<<ACT_3<<";"<<reward_id<<";"<<vectorLongToString(act->record_);
    return ost.str();
}
string buildLoadTeamResponse(User * user)
{
    ostringstream ost;
    ost<<cmd_list[CMD_LOADTEAM]<<";0;"<<user->teams_.size()<<";";
    for(map<long long,Team *>::iterator it = user->teams_.begin();
        it!=user->teams_.end();it++){
        Team * team = it->second;
        ost<<team->team_id_<<","<<team->hero_id_<<",";
        for(size_t i = 0;i<team->soldier_id_.size();i++){
            ost<<team->soldier_id_[i]<<",";
        }
        ost<<team->recreate_time_<<";";
    }
    return ost.str();
}
string buildSaveTeamResponse(User * user, long long team_id)
{
    ostringstream ost;
    ost<<cmd_list[CMD_SAVETEAM]<<";0;"<<team_id;
    return ost.str();
}
string buildChangeTeamResponse(User * user, long long team_id)
{
    ostringstream ost;
    ost<<cmd_list[CMD_CHANGETEAM]<<";0;"<<team_id;
    return ost.str();
}
string buildChangeNickResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_CHANGENICK]<<";0";
    return ost.str();
}
string buildForceHEUResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_FORCEHEU]<<";0";
    return ost.str();
}
string buildForceSEUResponse()
{
    ostringstream ost;
    ost<<cmd_list[CMD_FORCESEU]<<";0";
    return ost.str();
}

void buildLoadBuildingsResponse(hstring &hstr, User *user) {
    
    hstr<<cmd_list[CMD_LOAD_BUILDINGS]<<0<<user->uid_<<user->pvp_team_id_<<user->build_infs_.size();

    for (map <long long, BuildInf>::iterator iter = user->build_infs_.begin(); iter != user->build_infs_.end(); iter++) {
        hstring buildstr(",");
        const BuildInf &binf = iter->second;

        buildstr<<binf.id_<<binf.mid_<<binf.level_<<binf.position_<<binf.gem_type_;

        hstr<<buildstr;

        buildstr.clear();
    }
    hstr<<user->honor_<<user->gold_<<user->diamond_<<user->wood_<<user->stone_;
}

void buildStartBuildingsResponse(hstring &hstr, User *user) {
    
    hstr<<cmd_list[CMD_LOAD_BUILDINGS_BY_ID]<<0<<user->uid_;
    bool had_team = false;
    long long pvp_hid = 0;
    if (user->pvp_team_id_ > 0) {
        map <long long, Team *>::iterator iter = user->teams_.find(user->pvp_team_id_);
        if (iter != user->teams_.end()) {

            Team * team = iter->second;

            map <long long, Hero *>::iterator hiter = user->heroes_.find(team->hero_id_);
            if (hiter != user->heroes_.end()) {
                hstr<<hiter->second->mid_<<hiter->second->star_<<hiter->second->level_;
            }
            else {
                hstr<<0<<0<<0;
            }

            pvp_hid = team->hero_id_;

            hstr<<team->soldier_id_.size();
            for (size_t i = 0; i < team->soldier_id_.size(); i++) {

                hstring stmp(",");

                map <long long, Soldier *>::iterator siter = user->soldiers_.find(team->soldier_id_[i]);
                if (siter != user->soldiers_.end()) {
                    stmp<<siter->second->mid_<<siter->second->star_<<siter->second->level_;      
                }
                else {
                    stmp<<1<<1<<1;
                }
                hstr<<stmp;
                stmp.clear();
            }

            had_team = true;
        }

    }
    if (!had_team) {
        hstr<<0<<0<<0;
        hstr<<0;
    }

    int gnum = 0;
    if (pvp_hid > 0) {
        for (size_t i = 0; i < user->gear_infs_.size(); i++) {
            if (user->gear_infs_[i].hero_id_ > 0 && user->gear_infs_[i].hero_id_ == pvp_hid) {
                gnum ++;
            }
        }
    }
    hstr<<gnum;
    if (gnum > 0) {
        for (size_t i = 0; i < user->gear_infs_.size(); i++) {
            GearInf &ginf = user->gear_infs_[i];
            if (ginf.hero_id_ > 0 && ginf.hero_id_ == pvp_hid) {
                
                hstring htmp(",");
                htmp<<ginf.mid_<<ginf.level_;

                hstr<<htmp;
                htmp.clear();
            }
        }
    }

    
    hstr<<user->build_infs_.size();

    for (map <long long, BuildInf>::iterator iter = user->build_infs_.begin(); iter != user->build_infs_.end(); iter++) {
        hstring buildstr(",");
        const BuildInf &binf = iter->second;

        buildstr<<binf.id_<<binf.mid_<<binf.level_<<binf.position_<<binf.gem_type_;

        hstr<<buildstr;

        buildstr.clear();
    }

    //TODO 计算获得的资源数
    hstr<<1<<1<<0<<1<<1;
}


