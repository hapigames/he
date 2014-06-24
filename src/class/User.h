//
//  User.h
//  hero
//
//  Created by xujianfeng on 13-8-9.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_User_h
#define hero_User_h

#include "../common/Const.h"
#include "../common/GameConfig.h"
#include <string>
#include <map>
#include <vector>
#include "Hero.h"
#include "Soldier.h"
#include "Item.h"
#include "Friend.h"
#include "Reward.h"
#include "Mail.h"
#include "Mission.h"
#include "Stage.h"
#include "Act.h"
#include "Build.h"

using namespace std;

class UserBasicInfo
{
public:
    UserBasicInfo();
    ~UserBasicInfo();
    
    uid_type uid_;
    string eqid_;
    string username_;
    string password_;
    long long create_time_;
};

class HonorExcInf {
public:
    HonorExcInf(int idx, int dc, int ac) : index_(idx), daily_exc_count_(dc), all_exc_count_(ac) {}
    HonorExcInf(): index_(0), daily_exc_count_(0), all_exc_count_(0) {}
    int index_;
    int daily_exc_count_;
    int all_exc_count_;
};

class Team
{
public:
    Team();
    ~Team();
    
    long long team_id_;
    long long owner_id_;
    long long recreate_time_;
    long long hero_id_;
    vector<long long > soldier_id_;
    
};

class User
{
public:
    User();
    ~User();

    bool loadStatus(int status) { return (load_staus_ & status) != 0; }
    void setStatus(int status) { load_staus_ |= status; }
    int  towerLevel();
    BuildInf* towerInf();
    Team *getTeam(long long teamid);
    Hero *getHero(long long heroid);
    bool hadBuilding(int pos);
    bool checkBuildingPosiiton(int pos);
    bool checkBuildingReqItem(vector <Reward> &reqitem);
    void loadBuildingsPositions(vector <int> &poses);

    void dateChange();
    void clearAttack(time_t now);
    bool canAttack(time_t now);
    
    uid_type uid_;//0
    string nick_name_;//1
    long long create_time_;//2
    long long last_login_time_;//3
    string eqid_;//4
    int user_level_;//5
    int user_exp_;//6
    int energy_;//7
    int gold_;//8
    int diamond_;//9
    long long last_recover_time_;//10
    long long hero_id_;//11
    vector<long long> soldier_id_;//12
    int normal_chapter_id_;//13
    int normal_stage_id_;//14
    int elite_chapter_id_;//15
    int elite_stage_id_;//16
    int spark_num_;
    int spark_daily_num_;
    int receive_energy_daily_limit_;
    int send_energy_daily_limit_;

    int trial_stage_id_; //28
    int trial_max_stage_id_; //29
    int trial_daily_reset_; //30
    long long trial_instant_start_time_; //31
    int honor_; //32

    
    vector<int> system_mail_record_;
    int login_sum_;
    int buy_energy_daily_;
    int expand_hero_;
    int expand_soldier_;
    int expand_friend_;
    
    int guide_record_;
    int pay_count_;
    long long team_id_;
    
    long long last_active_time_;
    
    map<long long,Hero *> heroes_;
    map<long long,Soldier *> soldiers_;
    map<long long,Item *> items_;
    map<long long,FriendRequestInfo * > friend_requests_;
    map<long long,FriendInfo *> friends_;
    map<long long,Mail* > mails_;
    map<long long,Mission *> missions_;
    map<int,StageRecord *> stage_record_;
    map<int,Act *> act_record_;
    map<long long,Team*> teams_;
    
    int stage_type_;
    int battle_chapter_id_;
    int battle_stage_id_;
    uid_type assistant_id_;
    vector<StageReward * > rewards_;

    //pvp 基地
    int pvp_rank_;
    int pvp_attack_type_;
    time_t pvp_attack_start_time_; //TODO 不用保存
    long long pvp_attack_tuid_; //正攻打的uid
    long long pvp_attacked_tuid_;//正在被谁攻打
    long long pvp_team_id_;
    int wood_;
    int stone_;
    //TODO 还没入库
    int pvp_rank_attack_count_;
    int pvp_rob_attack_count_;

    map <long long, BuildInf> build_infs_;
    map <long long, GearInf> gear_infs_;
    map <int, HonorExcInf> honor_exc_infs_;
    
    int military;
    
    long long load_staus_;
    bool dirty_;
    
};



#endif
