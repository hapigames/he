#include "EventHandler.h"

bool EventHandler::safeLoadBuildings(User *user, int cmd, int fd) {
    if (user->loadStatus(LOADED_BUILDINGS)) return true;
    bool check = dh_->loadPvpAllBuildings(user);
    if (dh_->db_error_ != 0 || check == false) {
        sendErrorResponse(fd, cmd, ERROR_SYSTEM_CRITICAL_ERROR);
        return false;
    }
    //load pvp team id
    check = dh_->loadUserPvpInfo(user);
    if (dh_->db_error_ != 0 || check == false) {
        sendErrorResponse(fd, cmd, ERROR_SYSTEM_CRITICAL_ERROR);
        return false;
    }
    return true;
}

bool EventHandler::safeLoadPvpGears(User *user, int cmd, int fd) {
    if (user->loadStatus(LOADED_GEARS)) return true;
    bool check = dh_->loadAllGears(user);
    if (dh_->db_error_ != 0 || check == false) {
        sendErrorResponse(fd, cmd, ERROR_SYSTEM_CRITICAL_ERROR);
    }
    return true;
}

bool EventHandler::safeLoadHonorExcStatus(User *user, int cmd, int fd) {
    if (user->loadStatus(LOADED_HONOR_EXC)) return true;
    bool check = dh_->loadAllHonorExcStatus(user);
    if (dh_->db_error_ != 0 || check == false) {
        sendErrorResponse(fd, cmd, CMD_LOAD_HONOR_EXC_STATUS);
    }
    return true;
}


void EventHandler::processLoadbuildings(EventCmd &e, vector<string> &check_cmd) {
    long long uid;
    if (check_cmd.size() != 3 
            && !safeAtoll(check_cmd[1], uid)) {
        return;
    }
    string& eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_LOAD_BUILDINGS, e.fd_, eqid);
    if (user == NULL) {
        return;
    }
    if (!safeLoadBuildings(user, CMD_LOAD_BUILDINGS, e.fd_)
            || !safeLoadHero(user, CMD_LOAD_BUILDINGS, e.fd_)
            || !safeLoadTeam(user, CMD_LOAD_BUILDINGS, e.fd_)
            || !safeLoadItem(user, CMD_LOAD_BUILDINGS, e.fd_)
            || !safeLoadSoldier(user, CMD_LOAD_BUILDINGS, e.fd_)) {
        return;
    }
   
    hstring hstr(";");
    buildLoadBuildingsResponse(hstr, user);
    nh_->sendString(e.fd_, hstr.str(), hstr.size());
}

void EventHandler::processLoadBuildingsById(EventCmd &e, vector<string> &check_cmd) {
    long long uid,tuid;
    if (check_cmd.size() != 4
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoll(check_cmd[3], tuid)) {
        return ;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_LOAD_BUILDINGS_BY_ID, e.fd_, eqid);
    User *tuser = safeGetUser(uid, CMD_LOAD_BUILDINGS_BY_ID, e.fd_);
    if (user == NULL || tuser == NULL) {
        return;
    }

    if (!safeLoadBuildings(tuser, CMD_LOAD_BUILDINGS_BY_ID, e.fd_)
            || !safeLoadHero(tuser, CMD_LOAD_BUILDINGS_BY_ID, e.fd_)
            || !safeLoadItem(tuser, CMD_LOAD_BUILDINGS_BY_ID, e.fd_)
            || !safeLoadTeam(tuser, CMD_LOAD_BUILDINGS_BY_ID, e.fd_)
            || !safeLoadPvpGears(tuser, CMD_LOAD_BUILDINGS_BY_ID, e.fd_)
            || !safeLoadSoldier(tuser, CMD_LOAD_BUILDINGS_BY_ID, e.fd_)) {
        return;
    }

    hstring hstr(";");
    buildLoadBuildingsByIdResponse(hstr, tuser);
    //TODO  计算材料 添加缓存到user
    nh_->sendString(e.fd_, hstr.str(), hstr.size());

}

void EventHandler::processPvpRanks(EventCmd &e, vector<string> &check_cmd) {
    long long uid;
    if (check_cmd.size() != 3
            || !safeAtoll(check_cmd[1], uid)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_LOAD_PVP_RANKS, e.fd_, eqid);
    if (user == NULL) {
        return;
    }

    hstring hstr(";");
    hstr<<cmd_list[CMD_LOAD_PVP_RANKS]<<0<<user->pvp_rank_;
    //TODO 计算显示那些排名
    //

    nh_->sendString(e.fd_, hstr.str(), hstr.size());
}

void EventHandler::processPvpTargets(EventCmd &e, vector<string> &check_cmd) {
    long long uid;
    if (check_cmd.size() != 3
            || !safeAtoll(check_cmd[1], uid)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_LOAD_PVP_TARGETS, e.fd_, eqid);
    if (user == NULL) {
        return;
    }

    hstring hstr(";");
    hstr<<cmd_list[CMD_LOAD_PVP_TARGETS]<<0;
    //TODO 计算显示那些排名 可攻打
    //

    nh_->sendString(e.fd_, hstr.str(), hstr.size());

}

void EventHandler::processPvpBattleStart(EventCmd &e, vector<string> &check_cmd) {
    long long uid,tuid;
    int bttype;
    if (check_cmd.size() != 5
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoll(check_cmd[3], tuid)
            || !safeAtoi(check_cmd[4], bttype)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_PVP_BATTLE_START, e.fd_, eqid);
    if (user == NULL) {
        return;
    }
    User *tuser = safeGetUser(tuid, CMD_PVP_BATTLE_START, e.fd_);
    if (tuser == NULL) {
        return;
    }

    if (!safeLoadBuildings(user, CMD_LOAD_BUILDINGS_BY_ID, e.fd_)
            || !safeLoadBuildings(tuser, CMD_LOAD_BUILDINGS_BY_ID, e.fd_)
            || !safeLoadHero(tuser, CMD_LOAD_BUILDINGS_BY_ID, e.fd_)
            || !safeLoadItem(tuser, CMD_LOAD_BUILDINGS_BY_ID, e.fd_)
            || !safeLoadTeam(tuser, CMD_LOAD_BUILDINGS_BY_ID, e.fd_)
            || !safeLoadPvpGears(tuser, CMD_LOAD_BUILDINGS_BY_ID, e.fd_)
            || !safeLoadSoldier(tuser, CMD_LOAD_BUILDINGS_BY_ID, e.fd_)) {
        return;
    }
    user->pvp_attack_tuid_ = tuid;
    tuser->pvp_attacked_tuid_ = uid;
    dh_->saveUserpvpInfo(user);

    hstring hstr(";");
    buildLoadBuildingsByIdResponse(hstr, tuser);
    //TODO  计算材料 添加缓存到user
    nh_->sendString(e.fd_, hstr.str(), hstr.size());
}

void EventHandler::processPvpBattleEnd(EventCmd &e, vector<string> &check_cmd) {

}

void EventHandler::processAddBuilding(EventCmd &e, vector<string> &check_cmd) {
    long long uid;
    int mid;
    int position;
    if (check_cmd.size() != 5 
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoi(check_cmd[3], mid)
            || !safeAtoi(check_cmd[4], position)
            || mid == BUILDING_TOWER) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_ADD_BUILDING, e.fd_, eqid);
    if (user == NULL) {
        return;
    }
    if (!safeLoadBuildings(user, CMD_LOAD_BUILDINGS, e.fd_)) {
        return;
    }
    int succ = 0;
    long long id = -1;
    if (mid <= 0 || mid >= (int)game_config.build_conf_.size()) {
        succ = ERROR_BUILDING_NOT_FOUND;
    }
    else if (user->user_level_ < game_config.build_conf_[mid][1].req_user_level_){
        succ = ERROR_LEVEL_NOT_ENOUGH;
    }
    else if (user->towerLevel() < game_config.build_conf_[mid][1].req_tower_level_) {
        succ = ERROR_TOWER_LEVEL_NOT_ENOUGH;
    }
    else if (!user->checkBuildingPosiiton(position)) {
        succ = ERROR_BUILDING_POSITION_ERROR;
    }
    else if (!user->checkBuildingReqItem(game_config.build_conf_[mid][1].req_items_)) {
        succ = ERROR_BUILDING_GEAR_NOT_ENOUGH;
    }
    else {
        long long id = dh_->addBuilding(user, mid, position);
        if (id < 0) {
            //error LOG
        }
    }
    hstring hstr(";");
    hstr<<cmd_list[CMD_ADD_BUILDING]<<succ;
    if (succ == 0) {
        hstr<<id<<user->gold_<<user->diamond_<<position<<1<<mid;
    }
    nh_->sendString(e.fd_, hstr.str(), hstr.size());
}

void EventHandler::processSaveBuildings(EventCmd &e, vector<string> &check_cmd) {
    long long uid;
    int bcount = 0;
    if (check_cmd.size() < 4
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoi(check_cmd[3], bcount)) {
        return;
    }
    string &eqid = check_cmd[2];
    if ((int)check_cmd.size() != (4 + bcount * 2)) {
        return;
    }
    vector <pair <long long, long long> > bidpos;
    string del = ",";
    for (int i = 0; i < bcount; i++) {
        vector <long long> vtmp;
        if (longtokenize(check_cmd[4+i], vtmp, del) && vtmp.size() == 2) {
            bidpos.push_back(make_pair(vtmp[0], vtmp[1]));
        }
    }
    if (bcount != (int)bidpos.size()) {
        LOG4CXX_DEBUG(logger_, "saveBuildings error, bcount:"<<bcount<<" bidpos:"<<bidpos.size());
        return;
    }
    User *user = safeGetUser(uid, CMD_SAVE_BUILDINGS, e.fd_, eqid);
    if (user == NULL) {
        return;
    }
    if (!safeLoadBuildings(user, CMD_SAVE_BUILDINGS, e.fd_)) {
        return;
    }
    //check poses
    int succ = 0;
    vector <int> poses;
    user->loadBuildingsPositions(poses);
    for (size_t i = 0; i < bidpos.size(); i++) {
        map <long long, BuildInf>::iterator iter = user->build_infs_.find(bidpos[i].first);
        if (iter == user->build_infs_.end()) {
            succ = ERROR_SAVE_BUILDING_ID_ERROR;
            break;
        }
        if (iter->second.position_ != (int)bidpos[i].second) {
            if ((int) poses.size() <= (int)bidpos[i].second) {
                poses.resize((int)bidpos[i].second+1, 0);
            }
            poses[iter->second.position_] --;
            poses[(int)bidpos[i].second] ++;
        }
    }
    for (size_t i = 0; i < poses.size() ;i++) {
        if (poses[i] > 1) {
            succ = ERROR_SAVE_BUILDING_POS_ERROR;
        }
    }

    if (succ == 0) {
        for (size_t i = 0; i < bidpos.size(); i++) {
            dh_->saveBuilding(user, bidpos[i].first, bidpos[i].second);
        }
    }
    hstring hstr(";");
    hstr<<cmd_list[CMD_SAVE_BUILDINGS]<<succ;
    nh_->sendString(e.fd_, hstr.str(), hstr.size());

}

void EventHandler::processUpgradeBuilding(EventCmd &e, vector<string> &check_cmd) {
    long long uid,bid;
    if (check_cmd.size() != 4
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoll(check_cmd[3], bid)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_UPGRADE_BUILDING, e.fd_, eqid);
    if (user == NULL) {
        return;
    }
    if (!safeLoadBuildings(user, CMD_UPGRADE_BUILDING, e.fd_)) {
        return;
    }
    int succ = 0;
    map <long long, BuildInf>::iterator iter = user->build_infs_.find(bid);
    if (iter == user->build_infs_.end()) {
        succ = ERROR_BUILDING_ID_ERROR;
    }
    else {
        BuildInf & binf = iter->second;
        if (binf.level_ + 1 >= (int) game_config.build_conf_[binf.mid_].size()) {
            succ = ERROR_BUILDING_REACH_MAX_LEVEL;
        }
        else if (user->user_level_ < game_config.build_conf_[binf.mid_][binf.level_+1].req_user_level_) {
            succ = ERROR_LEVEL_NOT_ENOUGH;
        }
        else if (user->towerLevel() < game_config.build_conf_[binf.mid_][binf.level_+1].req_tower_level_) {
            succ = ERROR_TOWER_LEVEL_NOT_ENOUGH;
        }
        else if (!user->checkBuildingReqItem(game_config.build_conf_[binf.mid_][binf.level_+1].req_items_)) {
            succ = ERROR_BUILDING_GEAR_NOT_ENOUGH;
        }
        else {
            binf.level_ ++;
            dh_->saveBuilding(user, binf);
            dh_->delReqItem(user, game_config.build_conf_[binf.mid_][binf.level_].req_items_);
            //TODO LOG
        }
    }

    hstring hstr(";");
    hstr<<cmd_list[CMD_UPGRADE_BUILDING]<<succ;
    if (succ == 0) {
        hstr<<user->gold_<<user->diamond_;
    }
    nh_->sendString(e.fd_, hstr.str(), hstr.size());
}

void EventHandler::processDestroyBuilding(EventCmd &e, vector<string> &check_cmd) {
    long long uid, bid;
    if (check_cmd.size() != 4
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoll(check_cmd[3], bid)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_DESTROY_BUILDING, e.fd_, eqid);
    if (user == NULL) {
        return;
    }
    if (!safeLoadBuildings(user, CMD_DESTROY_BUILDING, e.fd_)) {
        return;
    }
    int succ = 0;
    map <long long, BuildInf>::iterator iter = user->build_infs_.find(bid);
    if (iter == user->build_infs_.end()) {
        succ = ERROR_BUILDING_ID_ERROR;
    }
    else {
        BuildInf &binf = iter->second;
        dh_->destroyBuilding(user, binf.id_);
        dh_->addRewardItem(user, game_config.build_conf_[binf.mid_][binf.level_].reward_items_);
        //TODO log
    }

    hstring hstr(";");
    hstr<<cmd_list[CMD_DESTROY_BUILDING]<<succ;
    if (succ == 0) {
        hstr<<user->honor_<<user->gold_<<user->diamond_<<user->wood_<<user->stone_;
    }
    nh_->sendString(e.fd_, hstr.str(), hstr.size());
}

void EventHandler::processSetGem(EventCmd &e, vector<string> &check_cmd) {
    long long uid,bid;
    int gem_type;
    if (check_cmd.size() != 5
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoll(check_cmd[3], bid)
            || !safeAtoi(check_cmd[4], gem_type)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_SET_GEM, e.fd_, eqid);
    if (user == NULL) {
        return;
    }
    if (!safeLoadBuildings(user, CMD_SET_GEM, e.fd_)) {
        return;
    }
    int succ = 0;
    map <long long, BuildInf>::iterator iter = user->build_infs_.find(bid);
    if (iter == user->build_infs_.end()) {
        succ = ERROR_BUILDING_ID_ERROR;
    }
    else if (gem_type <= 0 || gem_type >= (int) game_config.gem_conf_.size()) {
        succ = ERROR_BUILDING_GEM_TYPE_ERROR;
    }
    else if (user->gold_ < game_config.gem_conf_[gem_type].req_gold_) {
        succ = ERROR_GOLD_NOT_ENOUGH;
    }
    else if (user->diamond_ < game_config.gem_conf_[gem_type].req_diamond_) {
        succ = ERROR_DIAMOND_NOT_ENOUGH;
    }
    else {
        iter->second.gem_type_ = gem_type;
        user->gold_ -= game_config.gem_conf_[gem_type].req_gold_;
        user->diamond_ -= game_config.gem_conf_[gem_type].req_diamond_;
        dh_->saveBuilding(user, iter->second);
        //save basic info
        //TODO
    }

    hstring hstr(";");
    hstr<<cmd_list[CMD_SET_GEM]<<succ;
    if (succ == 0) {
        hstr<<user->gold_<<user->diamond_;
    }
    nh_->sendString(e.fd_, hstr.str(), hstr.size()); 
}

void EventHandler::processLoadHonorExcStatus(EventCmd &e, vector<string> &check_cmd) {
    long long uid;
    if (check_cmd.size() != 3
            || !safeAtoll(check_cmd[1], uid)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_LOAD_HONOR_EXC_STATUS, e.fd_, eqid);
    if (user == NULL) {
        return;
    }
    if (!safeLoadHonorExcStatus(user, CMD_LOAD_HONOR_EXC_STATUS, e.fd_)) {
        return;
    }
    hstring hstr(";");
    hstr<<cmd_list[CMD_LOAD_HONOR_EXC_STATUS]<<0<<user->honor_exc_infs_.size();
    for (map <int, HonorExcInf>::iterator iter = user->honor_exc_infs_.begin();
            iter != user->honor_exc_infs_.end(); ++iter) {
        hstring htmp(",");
        htmp<<iter->first<<iter->second.daily_exc_count_<<iter->second.all_exc_count_;
        hstr<<htmp;
        htmp.clear();
    }
    nh_->sendString(e.fd_, hstr.str(), hstr.size());
}

void EventHandler::processHonorExc(EventCmd &e, vector<string> &check_cmd) {
    long long uid;
    int index;
    if (check_cmd.size() != 4
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoi(check_cmd[3],index)) {
        return;
    }
    if (index < 0 || index >= (int) game_config.honor_exc_conf_.size()) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_HONOR_EXC, e.fd_, eqid);
    if (user == NULL) {
        return;
    }
    //TODO load honor exc status
    int succ = 0;
    map <int, HonorExcInf>::iterator iter = user->honor_exc_infs_.find(index);
    if (iter == user->honor_exc_infs_.end()) {
        user->honor_exc_infs_.insert(make_pair(index, HonorExcInf(index, 0, 0)));
        iter =  user->honor_exc_infs_.find(index);
    }
    if (iter == user->honor_exc_infs_.end()) {
        succ = ERROR_SYSTEM_CRITICAL_ERROR;
    }
    else if (iter->second.daily_exc_count_ >= game_config.honor_exc_conf_[index].daily_exc_limit_ ||
            iter->second.all_exc_count_ >= game_config.honor_exc_conf_[index].all_exc_limit_) {
        succ = ERROR_HONOR_EXC_REACH_LIMIT;
    }
    else if (user->honor_ < game_config.honor_exc_conf_[index].req_honor_) {
        succ = ERROR_HONOR_NOT_ENOUGH;
    }
    else {
        user->honor_ -= game_config.honor_exc_conf_[index].req_honor_;
        iter->second.daily_exc_count_ ++;
        iter->second.all_exc_count_ ++;
        //TODO save data
    }

    hstring hstr(";");
    hstr<<cmd_list[CMD_HONOR_EXC]<<succ;
    if (succ == 0 ) {
        hstr<<index<<iter->second.daily_exc_count_<<iter->second.all_exc_count_;
    }
    nh_->sendString(e.fd_, hstr.str(), hstr.size());
}

void EventHandler::processLoadGears(EventCmd &e, vector<string> &check_cmd) {
    long long uid;
    if (check_cmd.size() != 3
            || !safeAtoll(check_cmd[1], uid)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_LOAD_GEARS, e.fd_, eqid);
    if (user == NULL) {
        return;
    }
    if (!safeLoadPvpGears(user, CMD_LOAD_GEARS, e.fd_)) {
        return;
    }
    hstring hstr(";");
    hstr<<cmd_list[CMD_LOAD_GEARS]<<0<<user->gear_infs_.size();

    for (map<long long, GearInf>::iterator iter = user->gear_infs_.begin(); iter != user->gear_infs_.end(); ++ iter) {

        hstring htmp(",");
        GearInf & ginf = iter->second;
        htmp<<ginf.id_<<ginf.mid_<<ginf.level_<<ginf.hero_id_;
        hstr<<htmp;
        htmp.clear();
    }
    nh_->sendString(e.fd_, hstr.str(), hstr.size());
}

void EventHandler::processUpgradeGear(EventCmd &e, vector<string> &check_cmd) {
    long long uid, gid;
    if (check_cmd.size() != 4
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoll(check_cmd[3], gid)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_UPGRADE_GEAR, e.fd_, eqid);
    if (user == NULL) {
        return;
    }
    if (!safeLoadPvpGears(user, CMD_UPGRADE_GEAR, e.fd_)) {
        return;
    }
    int succ = 0;
    map <long long, GearInf>::iterator iter = user->gear_infs_.find(gid);
    if (iter == user->gear_infs_.end()) {
        succ = ERROR_GEAR_NOT_ENOUGH;
    }
    else {
        GearInf &ginf = iter->second;
        GearConf &gconf = game_config.gear_conf_[ginf.mid_];
        if (ginf.level_ >= (int)game_config.gear_enhance_req_gold_[gconf.position_][gconf.star_].size()) {
            succ = ERROR_GEAR_REACH_MAX_LEVEL;
        }
        else if (user->gold_ < game_config.gear_enhance_req_gold_[gconf.position_][gconf.star_][ginf.level_]) {
            succ = ERROR_GOLD_NOT_ENOUGH;
        }
        else {
            user->gold_ -= game_config.gear_enhance_req_gold_[gconf.position_][gconf.star_][ginf.level_];
            ginf.level_ ++;
            //save data
        }
    }
    hstring hstr(";");
    hstr<<cmd_list[CMD_UPGRADE_GEAR]<<succ;
    if (succ == 0) {
        hstr<<gid<<iter->second.level_<<user->gold_<<user->diamond_;
    }
    nh_->sendString(e.fd_, hstr.str(), hstr.size());
}


