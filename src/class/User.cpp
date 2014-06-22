//
//  User.cpp
//  hero
//
//  Created by xujianfeng on 13-8-9.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#include "User.h"

extern GameConfig game_config;

UserBasicInfo::UserBasicInfo()
{
}

UserBasicInfo::~UserBasicInfo()
{
}

User::User()
{
    heroes_.clear();
    soldiers_.clear();
    items_.clear();
    friends_.clear();
    rewards_.clear();
    friend_requests_.clear();
    friends_.clear();
    mails_.clear();
    missions_.clear();
    stage_record_.clear();
    act_record_.clear();
    teams_.clear();
    
    last_active_time_ = 0;
    last_login_time_ = 0;
    stage_type_ = 0;
    battle_chapter_id_ = 0;
    battle_stage_id_ = 0;
    
    spark_num_ = 0;
    receive_energy_daily_limit_ = 0;
    send_energy_daily_limit_ = 0;
    expand_hero_ = 0;
    expand_soldier_ = 0;
    expand_friend_ = 0;
    
    guide_record_ = 0;
    pay_count_ = 0;
    
    military = 0;
    load_staus_ = 0ll;
    dirty_ = false;

    trial_instant_start_time_ = 0;
    trial_daily_reset_ = 0;

    honor_ = 0;
    pvp_rank_ = 0;
    pvp_attack_type_ = 0;
    pvp_attack_tuid_ = 0; 
    pvp_attacked_tuid_ = 0;
    pvp_team_id_ = 0;
    wood_ = 0;
    stone_ = 0;
    pvp_rank_attack_count_ = pvp_rob_attack_count_ = 0;
    pvp_attack_start_time_ = time(NULL);
}

User::~User()
{
    for(map<long long , Hero* >::iterator it = heroes_.begin();
        it != heroes_.end();it++){
        if(it->second != NULL)
            delete it->second;
    }
    for(map<long long , Soldier* >::iterator it = soldiers_.begin();
        it != soldiers_.end();it++){
        if(it->second != NULL)
            delete it->second;
    }
    for(map<long long, Item * >::iterator it = items_.begin();
        it!= items_.end();it++){
        if(it->second != NULL)
            delete it->second;
    }
    for(map<long long, FriendRequestInfo * >::iterator it = friend_requests_.begin();
        it!= friend_requests_.end();it++){
        if(it->second != NULL)
            delete it->second;
    }
    for(map<long long, FriendInfo * >::iterator it = friends_.begin();
        it!= friends_.end();it++){
        if(it->second != NULL)
            delete it->second;
    }
    for(map<long long, Mail * >::iterator it = mails_.begin();
        it!= mails_.end();it++){
        if(it->second != NULL)
            delete it->second;
    }
    for(map<long long, Mission * >::iterator it = missions_.begin();
        it!= missions_.end();it++){
        if(it->second != NULL)
            delete it->second;
    }
    for(map<int, StageRecord *>::iterator it = stage_record_.begin();
        it!= stage_record_.end();it++){
        if(it->second != NULL)
            delete it->second;
    }
    for(map<int, Act *>::iterator it = act_record_.begin();
        it!= act_record_.end();it++){
        if(it->second != NULL)
            delete it->second;
    }
    for(map<long long, Team *>::iterator it = teams_.begin();
        it!= teams_.end();it++){
        if(it->second != NULL)
            delete it->second;
    }
}

Team::Team()
{
    
}
Team::~Team()
{
    
}

BuildInf *User::towerInf() {
    BuildInf *binf = NULL;
    for (map <long long, BuildInf>::iterator iter = build_infs_.begin(); iter != build_infs_.end(); ++iter) {
        if (iter->second.mid_ == BUILDING_TOWER) {
            binf = &(iter->second);
        }
    }
    return binf;
}

Team *User::getTeam(long long teamid) {
    map <long long, Team *>::iterator iter = teams_.find(teamid);
    if (iter != teams_.end()) {
        return (iter->second);
    }
    return NULL;
}
Hero *User::getHero(long long heroid) {
    map <long long, Hero *>::iterator iter = heroes_.find(heroid);
    if (iter != heroes_.end()) {
        return (iter->second);
    }
    return NULL;
}


int User::towerLevel() {
    int level = 1;
    for (map <long long, BuildInf>::iterator iter = build_infs_.begin(); iter != build_infs_.end(); ++iter) {
        if (iter->second.mid_ == BUILDING_TOWER) {
            level = iter->second.level_;
            break;
        }
    }
    return level;
}

bool User::hadBuilding(int pos) {
    for (map <long long, BuildInf>::iterator iter = build_infs_.begin(); iter != build_infs_.end(); ++iter) {
        if (iter->second.position_ == pos) {
            return true;
        }
    }
    return false;
}

bool User::checkBuildingPosiiton(int pos) {
    BuildInf *binf = towerInf();
    if (binf != NULL
            && game_config.checkInVector(game_config.build_conf_[binf->mid_][binf->level_].positions_, pos)
            && !hadBuilding(pos) ) {
        return true;
    }
    return false;
}

//TODO 暂时只需要消耗gold,wood,stone 要求其他需要改代码
bool User::checkBuildingReqItem(vector <Reward> &reqitem) {
    int req_gold = 0;
    int req_diamond = 0;
    int req_wood = 0;
    int req_stone = 0;
    bool ret = true;
    for (size_t i = 0; i < reqitem.size(); i++) {
        switch(reqitem[i].type) {
            case ITEM_TYPE_GOLD:
                req_gold += reqitem[i].param_1;
                break;
            case ITEM_TYPE_DIAMOND:
                req_diamond += reqitem[i].param_1;
                break;
            case ITEM_TYPE_WOOD:
                req_wood += reqitem[i].param_1;
                break;
            case ITEM_TYPE_STONE:
                req_stone += reqitem[i].param_1;
                break;
            default:
                ret = false;
                break;
        }
    }

    if (gold_ < req_gold
            || diamond_ < req_diamond
            || wood_ < req_wood
            || stone_ < req_stone) {
        ret = false;
    }
    return ret;
}

void User::loadBuildingsPositions(vector <int> &poses) {
    for (map <long long, BuildInf>::iterator iter = build_infs_.begin(); iter != build_infs_.end(); iter++) {
        if (iter->second.mid_ != BUILDING_TOWER) {
            if ((int) poses.size() <= iter->second.position_) {
                poses.resize(iter->second.position_+1, 0);
            }
            poses[iter->second.position_] = 1;
        }
    }
}

void User::dateChange() {
    pvp_rank_attack_count_ = pvp_rob_attack_count_ = 0;
}

bool User::canAttack(time_t now) {
    if (now >= pvp_attack_start_time_ + 600) {
        pvp_attack_type_ = 0;
        pvp_attack_tuid_ = 0;
        pvp_attacked_tuid_ = 0;
        pvp_attack_start_time_ = now;
    }
    return pvp_attacked_tuid_ == 0;
}

