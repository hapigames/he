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
    dh_->getUserPvpRank(user);
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
            || !safeAtoll(check_cmd[1], uid)) {
        return;
    }
    string& eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_LOAD_BUILDINGS, e.fd_, eqid);
    if (user == NULL || user->user_level_ < game_config.pvp_req_user_level_) {
        return;
    }
    if (!safeLoadBuildings(user, CMD_LOAD_BUILDINGS, e.fd_)
            || !safeLoadHero(user, CMD_LOAD_BUILDINGS, e.fd_)
            || !safeLoadTeam(user, CMD_LOAD_BUILDINGS, e.fd_)
            || !safeLoadItem(user, CMD_LOAD_BUILDINGS, e.fd_)
            || !safeLoadSoldier(user, CMD_LOAD_BUILDINGS, e.fd_)) {
        return;
    }
   
    string str = buildLoadBuildingsResponse(user);
    nh_->sendString(e.fd_, str);
}

void EventHandler::processLoadBuildingsById(EventCmd &e, vector<string> &check_cmd) {
    long long uid,tuid;
    int bttype;
    if (check_cmd.size() != 5
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoll(check_cmd[3], tuid)
            || !safeAtoi(check_cmd[4], bttype)
            || bttype < 0 || bttype > 1) {
        return ;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_LOAD_BUILDINGS_BY_ID, e.fd_, eqid);
    User *tuser = safeGetUser(tuid, CMD_LOAD_BUILDINGS_BY_ID, e.fd_);
    if (user == NULL || tuser == NULL || user->user_level_ < game_config.pvp_req_user_level_) {
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

    string str = buildLoadBuildingsByIdResponse(tuser, CMD_LOAD_BUILDINGS_BY_ID, bttype);
    nh_->sendString(e.fd_, str);
}

void EventHandler::buildPvpRanksResponse(ostringstream &oss, vector <int>& rankids, int cmd_code) {

    for (size_t i = 0; i < rankids.size(); i++) {

        int rank = rankids[i];
        long long tuid = dh_->pvp_rank_uids_[rank];

        int fd = 0;

        User *tuser = safeGetUser(tuid, cmd_code, fd);
        if (tuser != NULL
                && safeLoadBuildings(tuser, cmd_code, fd)
                && safeLoadHero(tuser, cmd_code, fd)
                && safeLoadTeam(tuser, cmd_code, fd)) {

            oss<<tuid<<","<<tuser->nick_name_<<","<<tuser->user_level_<<",";

            int ndfault = 1;
            if (tuser->pvp_team_id_ > 0) {
                Team *team = tuser->getTeam(tuser->pvp_team_id_);
                if (team) {
                    Hero *hero = tuser->getHero(team->hero_id_);
                    if (hero) {
                        ndfault = 0;
                        oss<<hero->mid_<<","<<hero->star_<<","<<hero->level_<<",";
                    }
                }
            }
            if (ndfault) {
                oss<<0<<","<<0<<","<<0<<",";
            }

            oss<<rank<<";";

        }
        else {
            oss<<tuid<<","<<" "<<","<<1<<","<<0<<","<<0<<","<<0<<","<<rank<<";";
        }

        //hstr<<rankstr;
    }
}

void EventHandler::processLoadPvpRanks(EventCmd &e, vector<string> &check_cmd) {
    long long uid;
    if (check_cmd.size() != 3
            || !safeAtoll(check_cmd[1], uid)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_LOAD_PVP_RANKS, e.fd_, eqid);
    if (user == NULL || user->user_level_ < game_config.pvp_req_user_level_) {
        return;
    }
    if (!safeLoadBuildings(user, CMD_LOAD_PVP_RANKS, e.fd_)) {
        return;
    }

    //hstring hstr(";");
    ostringstream oss;

    oss<<cmd_list[CMD_LOAD_PVP_RANKS]<<";"<<0<<";"<<user->pvp_rank_<<";";
    
    int rank_size = min((int)dh_->pvp_rank_uids_.size()-1, game_config.pvp_load_rank_size_);

    vector <int>  rankids;

    for (int i = 1; i <= game_config.pvp_load_rank_size_; i++) {
        if ( i >= (int) dh_->pvp_rank_uids_.size()) {
            break;
        }
        rankids.push_back(i);
        if ((int)rankids.size() >= rank_size) {
            break;
        }
    }
    
    oss<<((int)rankids.size())<<";";

    //hstring rankstr(";");
    
    buildPvpRanksResponse(oss, rankids, CMD_LOAD_PVP_RANKS);

    //hstr<<rankstr;
    string str = oss.str();
    nh_->sendString(e.fd_, str);
}

void EventHandler::processLoadPvpTargets(EventCmd &e, vector<string> &check_cmd) {
    long long uid;
    if (check_cmd.size() != 3
            || !safeAtoll(check_cmd[1], uid)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_LOAD_PVP_TARGETS, e.fd_, eqid);
    if (user == NULL
            || user->user_level_ < game_config.pvp_req_user_level_
            || !safeLoadBuildings(user, CMD_LOAD_PVP_TARGETS, e.fd_)) {
        return;
    }

    int size = game_config.pvp_rank_attack_before_ + game_config.pvp_rank_attack_after_; 
    int rank = user->pvp_rank_;
    int pos = game_config.randIdx(game_config.pvp_rank_interval_range_, rank);
    int step = game_config.pvp_rank_interval_[pos];
    int interval = step; 
   
    vector <int> rankids;
    rankids.push_back(rank);
    int cnt = 0;

    while (cnt < size) {
        int add = 0;
        if (rank - interval > 0) {
            rankids.push_back(rank-interval);
            cnt ++;
            add ++;
        }

        if (cnt >= size) {
            break;
        }

        if (rank + interval < (int) dh_->pvp_rank_uids_.size()) {
            rankids.push_back(rank+interval);
            cnt++;
            add ++;
        }

        interval += step;

        //TODO 防止没有这么多用户情况下
        if (add == 0) {
            break;
        }
    }

    sort(rankids.begin(), rankids.end());
    
    //hstring hstr(";");
    ostringstream oss;
    
    oss<<cmd_list[CMD_LOAD_PVP_TARGETS]<<";"<<0<<";";
    oss<<(game_config.pvp_rank_attack_daily_limit_ - user->pvp_rank_attack_count_)<<";"<<(game_config.pvp_rob_attack_daily_limit_ - user->pvp_rob_attack_count_)<<";";
    oss<<((int)rankids.size())<<";";

    buildPvpRanksResponse(oss, rankids, CMD_LOAD_PVP_TARGETS);

    string str = oss.str();
    nh_->sendString(e.fd_, str);

}

void EventHandler::processLoadPvpLootTargets(EventCmd &e, vector <string> &check_cmd) {
    long long uid;
    if (check_cmd.size() != 3
            || !safeAtoll(check_cmd[1], uid)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_LOAD_PVP_LOOT_TARGETS, e.fd_, eqid);
    if (user == NULL
            || user->user_level_ < game_config.pvp_req_user_level_
            || !safeLoadBuildings(user, CMD_LOAD_PVP_LOOT_TARGETS, e.fd_)) {
        return;
    }
    
    int succ = 0;
    int rank = user->pvp_rank_;
    int start = rank - game_config.rob_range_before_;
    int end = rank + game_config.rob_range_after_;
    start = max(1, start);
    end = min(end, ((int)dh_->pvp_rank_uids_.size() - 1));

    vector <int> rankids;

    if (end <= 0) {
        succ = ERROR_PVP_RANK_IS_EMPTY;
    }
    else {
        //user 
        vector <int> pos;
        for (int i = start; i <= end; i++) {
            if (dh_->pvp_rank_uids_[i] == uid) {
                continue;
            }
            pos.push_back(i);
        }
        for (int i = 0; i < game_config.rob_player_num_; i++) {
            if (i >= (int)pos.size()) {
                break;
            }
            int idx = rand() % (int)(pos.size()-i);
            rankids.push_back(pos[idx+i]);
            pos[idx+i] = pos[i];
        }
        //TODO robot
    }
    //hstring hstr(";");
    ostringstream oss;

    oss<<cmd_list[CMD_LOAD_PVP_LOOT_TARGETS]<<";"<<succ<<";";
    oss<<(game_config.pvp_rank_attack_daily_limit_ - user->pvp_rank_attack_count_)<<";"<<(game_config.pvp_rob_attack_daily_limit_ - user->pvp_rob_attack_count_)<<";";
    if (succ == 0) {

        //TODO 暂时只加入 player
        oss<<(int)rankids.size()<<";";
        buildPvpRanksResponse(oss, rankids, CMD_LOAD_PVP_LOOT_TARGETS);
    }
    string str = oss.str();
    nh_->sendString(e.fd_, str);
}

void EventHandler::processPvpBattleStart(EventCmd &e, vector<string> &check_cmd) {
    long long uid,tuid;
    int bttype;
    if (check_cmd.size() != 5
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoll(check_cmd[3], tuid)
            || !safeAtoi(check_cmd[4], bttype)
            || uid == tuid
            || bttype <0 || bttype > 1) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_PVP_BATTLE_START, e.fd_, eqid);
    if (user == NULL || user->user_level_ < game_config.pvp_req_user_level_) {
        return;
    }
    User *tuser = safeGetUser(tuid, CMD_PVP_BATTLE_START, e.fd_);
    if (tuser == NULL) {
        return;
    }

    if (!safeLoadBuildings(user, CMD_PVP_BATTLE_START, e.fd_)
            || !safeLoadBuildings(tuser, CMD_PVP_BATTLE_START, e.fd_)
            || !safeLoadHero(tuser, CMD_PVP_BATTLE_START, e.fd_)
            || !safeLoadItem(tuser, CMD_PVP_BATTLE_START, e.fd_)
            || !safeLoadTeam(tuser, CMD_PVP_BATTLE_START, e.fd_)
            || !safeLoadPvpGears(tuser, CMD_PVP_BATTLE_START, e.fd_)
            || !safeLoadSoldier(tuser, CMD_PVP_BATTLE_START, e.fd_)) {
        return;
    }

    dailyUpdateUser(user, CMD_PVP_BATTLE_START, e.fd_);
    
    int succ = 0;
    if (bttype == 0 && user->pvp_rank_attack_count_ >= game_config.pvp_rank_attack_daily_limit_) {
        succ = ERROR_PVP_ATTACK_DAILI_LIMIT;
    }
    else if (bttype == 1 && user->pvp_rob_attack_count_ >= game_config.pvp_rob_attack_daily_limit_) {
        succ = ERROR_PVP_ATTACK_DAILI_LIMIT;
    }
    else if (!tuser->canAttack(now_time_) || !user->canAttack(now_time_)) {
        succ = ERROR_PVP_UNDER_ATTACK;
    }
    else {
        user->pvp_attack_type_ = bttype;
        user->pvp_attack_tuid_ = tuid;
        user->pvp_attack_start_time_ = now_time_;
        tuser->pvp_attacked_tuid_ = uid;
        tuser->pvp_attack_start_time_ = now_time_;

        if (bttype == 0) {
            user->pvp_rank_attack_count_ ++;
        }
        else {
            user->pvp_rob_attack_count_ ++;
        }

        dh_->addUserPvpInfo(user);
    }
    //hstring hstr(";");
    string str = buildLoadBuildingsByIdResponse(tuser, CMD_PVP_BATTLE_START, bttype);
    //nh_->sendString(e.fd_, hstr.str(), hstr.size());
    nh_->sendString(e.fd_, str);
}

void EventHandler::processPvpBattleEnd(EventCmd &e, vector<string> &check_cmd) {
    long long uid;
    int win,bttype;
    if (check_cmd.size() != 5
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoi(check_cmd[3], win)
            || !safeAtoi(check_cmd[4], bttype)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_PVP_BATTLE_END, e.fd_, eqid);
    if (user == NULL || user->user_level_ < game_config.pvp_req_user_level_) {
        return;
    }
    User *tuser = NULL;
    int succ = 0;
    int get_h = 0, get_gold=0, get_dmd=0, get_w = 0, get_st=0;
    long long tuid = user->pvp_attack_tuid_;
    if (tuid <= 0) {
        succ = ERROR_PVP_NOT_ATTACK_TARGET;
    }
    else if (user->pvp_attack_type_ != bttype) {
        succ = ERROR_PVP_BATTLE_TYPE_ERROR;
    }
    else {
        tuser = safeGetUser(tuid, CMD_PVP_BATTLE_END, e.fd_);
        if (tuser == NULL) {
            return;
        }
        if (win == 0) {
            get_h = game_config.pvp_lose_honor_; 
        }
        else if (bttype == 0) {
            get_h = game_config.pvp_win_honor_;
            if (user->pvp_rank_  > tuser->pvp_rank_ && tuser->pvp_rank_ != 0) {
                int rank = user->pvp_rank_;
                user->pvp_rank_ = tuser->pvp_rank_;
                tuser->pvp_rank_ = rank;
                dh_->savePvpRank(user->pvp_rank_, user->uid_);
                dh_->savePvpRank(tuser->pvp_rank_, tuser->uid_);
            }
        }
        else if (bttype == 1) {
            get_h = game_config.pvp_win_honor_;
            get_gold = (int) tuser->gold_ * game_config.rob_gold_percent_ / 100;
            get_gold = max(get_gold , 1);
            get_w = (int) tuser->wood_ * game_config.rob_log_percent_ / 100;
            get_w = max(get_w, 1);
            get_st = (int) tuser->stone_ * game_config.rob_stone_percent_ / 100;
            get_st = max(get_st, 1);
        }
        //增加奖励 扣掉奖励
        user->honor_ += get_h;
        user->gold_ += get_gold;
        user->diamond_ += get_dmd;
        user->wood_ += get_w;
        user->stone_ += get_st;
        user->clearAttack(now_time_);

        tuser->gold_ -= get_gold;
        tuser->diamond_ -= get_dmd;
        tuser->wood_ -= get_w;
        tuser->stone_ -= get_st;
        tuser->gold_ = max(tuser->gold_, 0);
        tuser->diamond_ = max(tuser->diamond_, 0);
        tuser->wood_ = max(tuser->wood_, 0);
        tuser->stone_ = max(tuser->stone_, 0);
        tuser->clearAttack(now_time_);

        //TODO LOG
        dh_->saveUser(user);
        dh_->saveUser(tuser);
        dh_->addUserPvpInfo(user);
        dh_->addUserPvpInfo(tuser);
    }

    ostringstream oss;

    oss<<cmd_list[CMD_PVP_BATTLE_END]<<";"<<succ<<";";

    int rk_limit = game_config.pvp_rank_attack_daily_limit_ - user->pvp_rank_attack_count_;
    int rob_limit = game_config.pvp_rob_attack_daily_limit_ - user->pvp_rob_attack_count_;
    if (succ == 0) {
        oss<<user->pvp_rank_<<";"<<rk_limit<<";"<<rob_limit<<";"<<user->honor_<<";"<<user->gold_<<";"<<user->diamond_<<";"<<user->wood_<<";"<<user->stone_<<";";
        oss<<get_h<<";"<<get_gold<<";"<<get_dmd<<";"<<get_w<<";"<<get_st;
    }
    string str = oss.str();
    nh_->sendString(e.fd_, str);
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
    if (user == NULL || user->user_level_ < game_config.pvp_req_user_level_) {
        return;
    }
    if (!safeLoadBuildings(user, CMD_LOAD_BUILDINGS, e.fd_)) {
        return;
    }
    int succ = 0;
    long long id = -1;
    //star 从 1开始
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
        succ = ERROR_BUILDING_ITEM_NOT_ENOUGH;
    }
    else {
        id = dh_->addBuilding(user, mid, position);
        if (id < 0) {
            succ = ERROR_SYSTEM_CRITICAL_ERROR;
            LOG4CXX_ERROR(logger_, "addbuilding failed, id:"<<id);
        }
        else {
            dh_->delBuildingReqItem(user, game_config.build_conf_[mid][1].req_items_);
        }
    }
    ostringstream oss;
    oss<<cmd_list[CMD_ADD_BUILDING]<<";"<<succ<<";";
    if (succ == 0) {
        oss<<id<<";"<<mid<<";"<<position<<";"<<1<<";"<<user->honor_<<";"<<user->gold_<<";"<<user->diamond_<<";"<<user->wood_<<";"<<user->stone_;
    }
    string str = oss.str();
    nh_->sendString(e.fd_, str);
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
    if ((int)check_cmd.size() != (4 + bcount)) {
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
    if (user == NULL || user->user_level_ < game_config.pvp_req_user_level_) {
        return;
    }
    if (!safeLoadBuildings(user, CMD_SAVE_BUILDINGS, e.fd_)) {
        return;
    }
    int succ = 0;
    BuildInf *tower = user->towerInf();
    if (tower == NULL) {
        succ = ERROR_TOWER_NOT_FOUND;    
    }
    else {
        //check poses
        vector <int> poses;
        user->loadBuildingsPositions(poses);
        for (size_t i = 0; i < bidpos.size(); i++) {
            map <long long, BuildInf>::iterator iter = user->build_infs_.find(bidpos[i].first);
            if (iter == user->build_infs_.end()) {
                succ = ERROR_SAVE_BUILDING_ID_ERROR;
                break;
            }
            else if (iter->second.mid_ == BUILDING_TOWER) {
                succ = ERROR_SAVE_BUILDING_ID_ERROR;
                break;
            }
            else if (!game_config.checkInVector(game_config.build_conf_[tower->mid_][tower->level_].positions_, bidpos[i].second)) {
                succ = ERROR_BUILDING_POSITION_ERROR;
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
        //TODO 位置从1·开始
        for (size_t i = 1; i < poses.size() ;i++) {
            if (poses[i] > 1) {
                succ = ERROR_SAVE_BUILDING_POS_ERROR;
            }
        }
    
        if (succ == 0) {
            for (size_t i = 0; i < bidpos.size(); i++) {
                dh_->saveBuilding(user, bidpos[i].first, bidpos[i].second);
            }
        }
    }
    ostringstream oss;
    oss<<cmd_list[CMD_SAVE_BUILDINGS]<<";"<<succ;
    string str = oss.str();
    nh_->sendString(e.fd_, str);

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
    if (user == NULL || user->user_level_ < game_config.pvp_req_user_level_) {
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
            succ = ERROR_BUILDING_ITEM_NOT_ENOUGH;
        }
        else {
            binf.level_ ++;
            dh_->saveBuilding(user, binf);
            dh_->delBuildingReqItem(user, game_config.build_conf_[binf.mid_][binf.level_].req_items_);
            //TODO LOG
        }
    }

    ostringstream oss;
    oss<<cmd_list[CMD_UPGRADE_BUILDING]<<";"<<succ<<";";
    if (succ == 0) {
        oss<<user->honor_<<";"<<user->gold_<<";"<<user->diamond_<<";"<<user->wood_<<";"<<user->stone_;
    }
    string str = oss.str();
    nh_->sendString(e.fd_, str);
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
    if (user == NULL || user->user_level_ < game_config.pvp_req_user_level_) {
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
        dh_->addBuildingRewardItem(user, game_config.build_conf_[binf.mid_][binf.level_].reward_items_);
        //TODO log
    }

    ostringstream oss;
    oss<<cmd_list[CMD_DESTROY_BUILDING]<<";"<<succ<<";";
    if (succ == 0) {
        oss<<user->honor_<<";"<<user->gold_<<";"<<user->diamond_<<";"<<user->wood_<<";"<<user->stone_;
    }
    string str = oss.str();
    nh_->sendString(e.fd_, str);
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
    if (user == NULL || user->user_level_ < game_config.pvp_req_user_level_) {
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
        dh_->saveUser(user);
        //TODO
    }

    ostringstream oss;
    oss<<cmd_list[CMD_SET_GEM]<<";"<<succ<<";";
    if (succ == 0) {
        oss<<user->honor_<<";"<<user->gold_<<";"<<user->diamond_<<";"<<user->wood_<<";"<<user->stone_;
    }
    string str = oss.str();
    nh_->sendString(e.fd_, str); 
}

void EventHandler::processLoadHonorExcStatus(EventCmd &e, vector<string> &check_cmd) {
    long long uid;
    if (check_cmd.size() != 3
            || !safeAtoll(check_cmd[1], uid)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_LOAD_HONOR_EXC_STATUS, e.fd_, eqid);
    if (user == NULL || user->user_level_ < game_config.pvp_req_user_level_) {
        return;
    }
    if (!safeLoadHonorExcStatus(user, CMD_LOAD_HONOR_EXC_STATUS, e.fd_)) {
        return;
    }
    ostringstream oss;
    oss<<cmd_list[CMD_LOAD_HONOR_EXC_STATUS]<<";"<<0<<";"<<user->honor_exc_infs_.size()<<";";
    for (map <int, HonorExcInf>::iterator iter = user->honor_exc_infs_.begin();
            iter != user->honor_exc_infs_.end(); ++iter) {
        oss<<iter->first<<","<<iter->second.daily_exc_count_<<","<<iter->second.all_exc_count_;
    }
    string str = oss.str();
    nh_->sendString(e.fd_, str);
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
    if (user == NULL || user->user_level_ < game_config.pvp_req_user_level_) {
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
        if (!addReward(user, &(game_config.honor_exc_conf_[index].reward))) {
            //TODO
        }
        dh_->saveUser(user);
        dh_->saveHonorExcStatus(user, index, iter->second.daily_exc_count_, iter->second.all_exc_count_);
        //TODO 
    }

    ostringstream oss;
    oss<<cmd_list[CMD_HONOR_EXC]<<";"<<succ<<";";
    if (succ == 0 ) {
        oss<<index<<";"<<iter->second.daily_exc_count_<<";"<<iter->second.all_exc_count_;
    }
    string str = oss.str();
    nh_->sendString(e.fd_, str);
}

void EventHandler::processLoadGears(EventCmd &e, vector<string> &check_cmd) {
    long long uid;
    if (check_cmd.size() != 3
            || !safeAtoll(check_cmd[1], uid)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_LOAD_GEARS, e.fd_, eqid);
    if (user == NULL || user->user_level_ < game_config.pvp_req_user_level_) {
        return;
    }
    if (!safeLoadPvpGears(user, CMD_LOAD_GEARS, e.fd_)) {
        return;
    }

    ostringstream oss;
    oss<<cmd_list[CMD_LOAD_GEARS]<<";"<<0<<";"<<user->gear_infs_.size()<<";";

    for (map<long long, GearInf>::iterator iter = user->gear_infs_.begin(); iter != user->gear_infs_.end(); ++ iter) {

        GearInf & ginf = iter->second;
        oss<<ginf.id_<<","<<ginf.mid_<<","<<ginf.level_<<","<<ginf.hero_id_;
    }
    string str = oss.str();
    nh_->sendString(e.fd_, str);
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
    if (user == NULL || user->user_level_ < game_config.pvp_req_user_level_) {
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
        if (ginf.level_ >= (int)game_config.gear_enhance_req_gold_[gconf.position_][gconf.star_].size() - 1) {
            succ = ERROR_GEAR_REACH_MAX_LEVEL;
        }
        else if (user->gold_ < game_config.gear_enhance_req_gold_[gconf.position_][gconf.star_][ginf.level_+1]) {
            succ = ERROR_GOLD_NOT_ENOUGH;
        }
        else {
            ginf.level_ ++;
            user->gold_ -= game_config.gear_enhance_req_gold_[gconf.position_][gconf.star_][ginf.level_];
            //save data
            dh_->saveUser(user);
            dh_->saveGear(user, ginf);
        }
    }
    ostringstream oss;
    oss<<cmd_list[CMD_UPGRADE_GEAR]<<";"<<succ<<";";
    if (succ == 0) {
        oss<<gid<<";"<<iter->second.level_<<";"<<user->gold_<<";"<<user->diamond_;
    }
    string str = oss.str();
    nh_->sendString(e.fd_, str);
}

void EventHandler::processSetDefendTeam(EventCmd &e, vector <string> &check_cmd) {
    long long uid,teamid;
    if (check_cmd.size() != 4
            || !safeAtoll(check_cmd[1], uid)
            || !safeAtoll(check_cmd[3], teamid)) {
        return;
    }
    string &eqid = check_cmd[2];
    User *user = safeGetUser(uid, CMD_SET_DEFEND_TEAM, e.fd_, eqid);
    if (user == NULL
            || user->user_level_ < game_config.pvp_req_user_level_
            || !safeLoadBuildings(user, CMD_SET_DEFEND_TEAM, e.fd_)
            || !safeLoadTeam(user, CMD_SET_DEFEND_TEAM, e.fd_)) {
        return;
    }
    
    int succ = 0;
    if (user->teams_.find(teamid) == user->teams_.end()) {
        succ = ERROR_TEAM_CANNOT_BE_FOUND;
    }
    else {
        user->pvp_team_id_ = teamid;
        dh_->addUserPvpInfo(user);
    }

    ostringstream oss;
    oss<<cmd_list[CMD_SET_DEFEND_TEAM]<<";"<<succ;
    string str = oss.str();
    nh_->sendString(e.fd_, str);
}



