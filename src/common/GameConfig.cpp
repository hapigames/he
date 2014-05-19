//
//  GameConfig.cpp
//  hero
//
//  Created by xujianfeng on 13-8-19.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#include "GameConfig.h"
GameConfig::GameConfig(const char * server_config,const char * game_config):server_config_(server_config),game_config_(game_config)
{
    log4cxx::PropertyConfigurator::configure("log4cxx.config");
    logger_ = log4cxx::Logger::getLogger("GameConfig");

}
GameConfig::~GameConfig()
{}
bool GameConfig::loadFromFile()
{
    try{
        scfg_.readFile(server_config_.c_str());
        Setting &st = scfg_.lookup("app");
        /*server_type_ = st["server_type"];
        local_address_ = (const char *)st["local_address"];
        local_port_ = st["local_port"];
        db_address_ = (const char *)st["db_address"];
        db_user_ = (const char *)st["db_user"];
        db_passwd_ = (const char *)st["db_passwd"];
        db_name_ = (const char *)st["db_name"];
        db_port_ = st["db_port"];*/
        for(int i = 0;i<st["servers"].getLength();i++){
            sconfig  * sc = new sconfig();
            sc->server_id = st["servers"][i]["server_id"];
            sc->server_type = st["servers"][i]["server_type"];
            sc->ip = (const char *)st["servers"][i]["local_address"];
            sc->port = st["servers"][i]["local_port"];
            sc->db_address = (const char *)st["servers"][i]["db_address"];
            sc->db_user = (const char *)st["servers"][i]["db_user"];
            sc->db_passwd = (const char *)st["servers"][i]["db_passwd"];
            sc->db_name = (const char *)st["servers"][i]["db_name"];
            sc->db_port = st["servers"][i]["db_port"];
            
            sc->pay_db_address = (const char *)st["pay_db_address"];
            sc->pay_db_user = (const char *)st["pay_db_user"];
            sc->pay_db_passwd = (const char *)st["pay_db_passwd"];
            sc->pay_db_name = (const char *)st["pay_db_name"];
            sc->pay_db_port = st["pay_db_port"];
            serverconfigs_.insert(make_pair(sc->server_id,sc));
        }
    }
    catch(ParseException ex){
        LOG4CXX_ERROR(logger_, "system;Parse config "<<server_config_.c_str()<<" failed at line "<<ex.getLine());
        return false;
    }
    catch(FileIOException ex){
        LOG4CXX_ERROR(logger_, "system;Read config "<<server_config_.c_str()<<" failed");
        return false;
    }
    catch(SettingNotFoundException ex){
        LOG4CXX_ERROR(logger_, "system;Read config "<<server_config_.c_str()<<" failed, setting "<<ex.getPath()<<" not found");
        return false;
    }
    
    try{
        gcfg_.readFile(game_config_.c_str());
        Setting &st = gcfg_.lookup("app");
        
        //global
        Setting &sys_config = st["system"];
        fresh_interval_ = sys_config["fresh_interval"];
        fresh_step_count_ = sys_config["fresh_step_count"];
        fresh_level_size_ = sys_config["fresh_level_size"];
        friend_fresh_interval_ = sys_config["friend_fresh_interval"];
        fresh_choice_num_ = sys_config["fresh_choice_num"];
        fresh_friend_num_limit_ = sys_config["fresh_friend_num_limit"];
        fresh_level_limit_ = sys_config["fresh_level_limit"];
        recover_interval_ = sys_config["recover_interval"];
        friend_request_limit_ = sys_config["friend_request_limit"];
        spark_assistant_is_friend_ = sys_config["spark_assistant_is_friend"];
        spark_assistant_not_friend_ = sys_config["spark_assistant_not_friend"];
        spark_sum_limit_ = sys_config["spark_sum_limit"];
        spark_daily_limit_ = sys_config["spark_daily_limit"];
        send_energy_daily_limit_ = sys_config["send_energy_daily_limit"];
        accept_energy_daily_limit_ = sys_config["accept_energy_daily_limit"];
        energy_per_act_ = sys_config["energy_per_act"];
        p2p_mail_limit_ = sys_config["p2p_mail_limit"];
        system_mail_limit_ = sys_config["system_mail_limit"];
        p2p_mail_length_limit_ = sys_config["p2p_mail_length_limit"];
        enhance_exp_times_ = ((int)sys_config["enhance_exp_times"] * 1.0)/100.0;
        exp_to_gold_rate_hero_ = ((int)sys_config["exp_to_gold_rate_hero"] * 1.0)/100.0;
        exp_to_gold_rate_soldier_ = ((int)sys_config["exp_to_gold_rate_warrior"] * 1.0)/100.0;
        summon_price_diamond_1_ = sys_config["summon_price_diamond_1"];
        summon_price_diamond_10_ = sys_config["summon_price_diamond_10"];
        summon_price_spark_1_ = sys_config["summon_price_spark_1"];
        summon_price_spark_10_ = sys_config["summon_price_spark_10"];
        elite_stage_daily_limit_ = sys_config["elite_stage_daily_limit"];
        max_team_num_ = sys_config["max_team_num"];
        normal_stage_reward_limit_ = sys_config["normal_stage_reward_limit"];
        elite_stage_daily_limit_ = sys_config["elite_stage_daily_limit"];
        special_stage_reward_limit_ = sys_config["special_stage_reward_limit"];
        force_hero_exp_up_ = sys_config["force_hero_exp_up"];
        force_soldier_exp_up_ = sys_config["force_soldier_exp_up"];
        fake_user_nick_ = (const char *)sys_config["fake_user_nick"];
        fake_user_eqid_ = (const char *)sys_config["fake_user_eqid"];
        normal_stage_exp_rate_ = sys_config["normal_stage_exp_rate"];
        elite_stage_exp_rate_ = sys_config["elite_stage_exp_rate"];
        elite_stage_reset_cost_ = sys_config["elite_stage_reset_cost"];
        
        time_area_fix_ = 1391270400LL;
        energy_price_.clear();
        for(int i = 0;i<sys_config["energy_price"].getLength();i++){
            energy_price_.push_back(sys_config["energy_price"][i]);
        }
        
        //act
        act_2_level_.clear();
        for(int i = 0;i<sys_config["act_2_level"].getLength();i++){
            act_2_level_.push_back(sys_config["act_2_level"][i]);
        }
        act_2_reward_.clear();
        for(int i = 0;i<sys_config["act_2_reward"].getLength();i++){
            vector<StageReward> srv;
            srv.clear();
            for(int j=0;j<sys_config["act_2_reward"][i].getLength();j++){
                StageReward sr;
                readStageReward(sr, sys_config["act_2_reward"][i][j]);
                srv.push_back(sr);
            }
            act_2_reward_.push_back(srv);
        }
        
        act_3_level_.clear();
        for(int i = 0;i<sys_config["act_3_level"].getLength();i++){
            act_3_level_.push_back(sys_config["act_3_level"][i]);
        }
        act_3_reward_.clear();
        for(int i = 0;i<sys_config["act_3_reward"].getLength();i++){
            vector<StageReward> srv;
            srv.clear();
            for(int j=0;j<sys_config["act_3_reward"][i].getLength();j++){
                StageReward sr;
                readStageReward(sr, sys_config["act_3_reward"][i][j]);
                srv.push_back(sr);
            }
            act_3_reward_.push_back(srv);
        }

        
        //shopping
        buy_energy_daily_limit_ = sys_config["buy_energy_daily_limit"];
        cost_per_buy_energy_ = sys_config["cost_per_buy_energy"];
        expand_hero_limit_ = sys_config["expand_hero_limit"];
        cost_per_expand_hero_ = sys_config["cost_per_expand_hero"];
        place_per_expand_hero_ = sys_config["place_per_expand_hero"];
        expand_soldier_limit_ = sys_config["expand_soldier_limit"];
        cost_per_expand_soldier_ = sys_config["cost_per_expand_soldier"];
        place_per_expand_soldier_ = sys_config["place_per_expand_soldier"];
        expand_friend_limit_ = sys_config["expand_friend_limit"];
        cost_per_expand_friend_ = sys_config["cost_per_expand_friend"];
        place_per_expand_friend_ = sys_config["place_per_expand_friend"];
        //user
        Setting &user_config = st["user"];
        
        readIntVectorFillZero(leadership_level_limit_, user_config, "leadership_level_limit");
        readIntVectorFillZero(energy_level_limit_, user_config, "energy_level_limit");
        readIntVectorFillZero(exp_level_limit_, user_config, "exp_level_limit");
        readIntVectorFillZero(hero_length_limit_, user_config, "hero_length_limit");
        readIntVectorFillZero(soldier_length_limit_, user_config, "soldier_length_limit");
        readIntVectorFillZero(friend_length_limit_, user_config, "friend_length_limit");
        readIntVectorFillZero(recharge_level_limit_, user_config, "recharge_level_limit");
        
        init_user_gold_ = sys_config["init_user_gold"];
        init_user_diamond_ = sys_config["init_user_diamond"];
        readReward(init_user_hero_, sys_config["init_user_hero"]);
        for(int i = 0;i<sys_config["init_user_soldier"].getLength();i++){
            Reward rew;
            readReward(rew, sys_config["init_user_soldier"][i]);
            init_user_soldier_.push_back(rew);
        }
        
        readReward(fake_user_hero_, sys_config["fake_user_hero"]);
        for(int i = 0;i<sys_config["fake_user_soldier"].getLength();i++){
            Reward rew;
            readReward(rew, sys_config["fake_user_soldier"][i]);
            fake_user_soldier_.push_back(rew);
        }
        fake_user_skill_.clear();
        for(int i = 0;i<sys_config["fake_user_skill"].getLength();i++){
            int skill = sys_config["fake_user_skill"][i];
            fake_user_skill_.push_back(skill);
        }
        
        //stage
        
        Setting &stage_config = st["stage"];
        stages_.clear();
        for(int i = 0;i<stage_config["stage_info"].getLength();i++){
            Stage stage;
            stage.chapter_id_ = stage_config["stage_info"][i]["chapter_id"];
            while(stage.chapter_id_>=(int) stages_.size()){
                vector<Stage> v;
                v.clear();
                stages_.push_back(v);
            }
            stage.stage_id_ = stage_config["stage_info"][i]["stage_id"];
            while(stage.stage_id_>=(int) stages_[stage.chapter_id_].size()){
                Stage temp;
                stages_[stage.chapter_id_].push_back(temp);
            }
            stage.level_required_ = stage_config["stage_info"][i]["level_required"];
            stage.energy_required_ = stage_config["stage_info"][i]["energy_required"];
            stage.reward_gold_ = stage_config["stage_info"][i]["reward_gold"];
            stage.reward_exp_ = stage_config["stage_info"][i]["reward_exp"];
            stage.depend_chapter_ = 0;
            stage.depend_stage_ = 0;
            stage.rewards_.clear();
            stages_[stage.chapter_id_][stage.stage_id_] = stage;
        }
        for(int i = 0;i<stage_config["stage_reward"].getLength();i++){
            int chapter_id = stage_config["stage_reward"][i]["chapter"];
            int stage_id = stage_config["stage_reward"][i]["stage"];
            if(chapter_id <= 0 || chapter_id >= (int) stages_.size()){
                cout<<"ci:"<<chapter_id<<endl;
                LOG4CXX_ERROR(logger_, "system;Read config stage reward failed");
                return false;
            }
            if(stage_id<=0 || stage_id>= (int) stages_[chapter_id].size()){
                cout<<"si:"<<chapter_id<<endl;
                LOG4CXX_ERROR(logger_, "system;Read config stage reward failed");
                return false;
            }
            StageReward sr;
            readStageReward(sr, stage_config["stage_reward"][i]["reward"]);
            stages_[chapter_id][stage_id].rewards_.push_back(sr);
        }
        elite_stages_.clear();
        for(int i = 0;i<stage_config["elite_stage_info"].getLength();i++){
            Stage stage;
            stage.chapter_id_ = stage_config["elite_stage_info"][i]["chapter_id"];
            while(stage.chapter_id_>= (int) elite_stages_.size()){
                vector<Stage> v;
                v.clear();
                elite_stages_.push_back(v);
            }
            stage.stage_id_ = stage_config["elite_stage_info"][i]["stage_id"];
            while(stage.stage_id_>= (int) elite_stages_[stage.chapter_id_].size()){
                Stage temp;
                elite_stages_[stage.chapter_id_].push_back(temp);
            }
            stage.level_required_ = stage_config["elite_stage_info"][i]["level_required"];
            stage.energy_required_ = stage_config["elite_stage_info"][i]["energy_required"];
            stage.reward_gold_ = stage_config["elite_stage_info"][i]["reward_gold"];
            stage.reward_exp_ = stage_config["elite_stage_info"][i]["reward_exp"];
            stage.depend_chapter_ = stage_config["elite_stage_info"][i]["depend_chapter"];
            stage.depend_stage_ = stage_config["elite_stage_info"][i]["depend_stage"];
            stage.rewards_.clear();
            elite_stages_[stage.chapter_id_][stage.stage_id_] = stage;
        }
        for(int i = 0;i<stage_config["elite_stage_reward"].getLength();i++){
            int chapter_id = stage_config["elite_stage_reward"][i]["chapter"];
            int stage_id = stage_config["elite_stage_reward"][i]["stage"];
            if(chapter_id <= 0 || chapter_id >= (int) elite_stages_.size()){
                cout<<"ci:"<<chapter_id<<endl;
                LOG4CXX_ERROR(logger_, "system;Read config elite stage reward failed");
                return false;
            }
            if(stage_id<=0 || stage_id>= (int) elite_stages_[chapter_id].size()){
                cout<<"si:"<<chapter_id<<endl;
                LOG4CXX_ERROR(logger_, "system;Read config elite stage reward failed");
                return false;
            }
            StageReward sr;
            readStageReward(sr, stage_config["elite_stage_reward"][i]["reward"]);
            elite_stages_[chapter_id][stage_id].rewards_.push_back(sr);
        }
        special_stages_.clear();
        for(int i = 0;i<stage_config["special_stage_info"].getLength();i++){
            Stage stage;
            stage.chapter_id_ = stage_config["special_stage_info"][i]["chapter_id"];
            if(special_stages_.find(stage.chapter_id_) == special_stages_.end()){
                vector<Stage> vs;
                vs.clear();
                special_stages_.insert(make_pair(stage.chapter_id_, vs));
            }
            stage.stage_id_ = stage_config["special_stage_info"][i]["stage_id"];
            while(stage.stage_id_>= (int) special_stages_[stage.chapter_id_].size()){
                Stage temp;
                special_stages_[stage.chapter_id_].push_back(temp);
            }
            stage.level_required_ = stage_config["special_stage_info"][i]["level_required"];
            stage.energy_required_ = stage_config["special_stage_info"][i]["energy_required"];
            stage.reward_gold_ = stage_config["special_stage_info"][i]["reward_gold"];
            stage.reward_exp_ = stage_config["special_stage_info"][i]["reward_exp"];
            stage.reset_cost_ = stage_config["special_stage_info"][i]["reset_cost"];
            stage.rewards_.clear();
            stage.ssc_ = new SpecialStageCondition();
            if(stage.ssc_ == NULL) return false;
            stage.ssc_->start_ = stage_config["special_stage_info"][i]["start"];
            stage.ssc_->end_ = stage_config["special_stage_info"][i]["end"];
            stage.ssc_->day_.clear();
            for(int j = 0;j<stage_config["special_stage_info"][i]["day"].getLength();j++){
                stage.ssc_->day_.push_back(stage_config["special_stage_info"][i]["day"][j]);
            }
            stage.ssc_->interval_ = stage_config["special_stage_info"][i]["interval"];
            stage.ssc_->moment_.clear();
            for(int j = 0;j<stage_config["special_stage_info"][i]["moment"].getLength();j++){
                int start = stage_config["special_stage_info"][i]["moment"][j][0];
                int end = stage_config["special_stage_info"][i]["moment"][j][1];
                stage.ssc_->moment_.push_back(make_pair(start,end));
            }
            stage.ssc_->times_ = stage_config["special_stage_info"][i]["times"];
            special_stages_[stage.chapter_id_][stage.stage_id_] = stage;
        }
        for(int i = 0;i<stage_config["special_stage_reward"].getLength();i++){
            int chapter_id = stage_config["special_stage_reward"][i]["chapter"];
            int stage_id = stage_config["special_stage_reward"][i]["stage"];
            map<int , vector<Stage> >::iterator it = special_stages_.find(chapter_id);
            if(it == special_stages_.end()) return false;
            if(stage_id <=0 || stage_id >= (int) (it->second).size()) return false;
            StageReward sr;
            readStageReward(sr, stage_config["special_stage_reward"][i]["reward"]);
            (it->second)[stage_id].rewards_.push_back(sr);
        }
        //hero
        Setting &hero_config = st["hero"];
        for(int i = 0;i<hero_config["hero_info"].getLength();i++){
            HeroConfig hc;
            hc.mid_ = hero_config["hero_info"][i]["mid"];
            if(hero_config_.find(hc.mid_) == hero_config_.end()){
                vector<HeroConfig> hcv;
                hcv.clear();
                hero_config_.insert(make_pair(hc.mid_, hcv));
            }
            hc.star_ = hero_config["hero_info"][i]["star"];
            while((int) hero_config_[hc.mid_].size()<= hc.star_){
                HeroConfig tmp;
                hero_config_[hc.mid_].push_back(tmp);
            }
            hc.leadership_require_ = hero_config["hero_info"][i]["leadership"];
            hc.price_ = hero_config["hero_info"][i]["price"];
            hc.price_plus_ = hero_config["hero_info"][i]["price_plus"];
            hc.level_limit_ = hero_config["hero_info"][i]["level_limit"];
            hc.type_ = hero_config["hero_info"][i]["type"];
            hc.fake_exp_ = hero_config["hero_info"][i]["fake_exp"];
            hero_config_[hc.mid_][hc.star_] = hc;
        }
        for(int i = 0;i<hero_config["hero_enhance"].getLength();i++){
            int star = hero_config["hero_enhance"][i]["star"];
            while((int) hero_enhance_.size()<=star){
                vector<EnhanceCost> evc;
                evc.clear();
                hero_enhance_.push_back(evc);
            }
            for(int j = 0;j<hero_config["hero_enhance"][i]["cost"].getLength();j++){
                EnhanceCost ec;
                readEnhanceCost(ec, hero_config["hero_enhance"][i]["cost"][j]);
                hero_enhance_[star].push_back(ec);
            }
        }
        for(int i = 0;i<hero_config["skill_lvup_cost"].getLength();i++){
            int position = hero_config["skill_lvup_cost"][i]["position"];
            while((int) skill_level_up_cost_.size()<=position){
                vector<vector<EnhanceCost> > evvc;
                evvc.clear();
                skill_level_up_cost_.push_back(evvc);
            }
            int lv = hero_config["skill_lvup_cost"][i]["lv"];
            while((int) skill_level_up_cost_[position].size()<= lv){
                vector<EnhanceCost> evc;
                evc.clear();
                skill_level_up_cost_[position].push_back(evc);
            }
            for(int j = 0;j<hero_config["skill_lvup_cost"][i]["cost"].getLength();j++){
                EnhanceCost ec;
                readEnhanceCost(ec, hero_config["skill_lvup_cost"][i]["cost"][j]);
                skill_level_up_cost_[position][lv].push_back(ec);
            }
        }
        vector<int> hero_level_up_exp_v_tmp;
        hero_level_up_exp_v_tmp.clear();
        hero_level_up_exp_.clear();
        hero_exp_sum_.clear();
        hero_level_up_exp_.push_back(hero_level_up_exp_v_tmp);
        hero_exp_sum_.push_back(hero_level_up_exp_v_tmp);
        for(int i = 0;i<hero_config["hero_level_up_exp"].getLength();i++){
            vector<int> v;
            vector<int> sumv;
            v.clear();
            sumv.clear();
            v.push_back(0);
            sumv.push_back(0);
            sumv.push_back(0);
            int sum = 0;;
            for(int j = 0;j<hero_config["hero_level_up_exp"][i].getLength();j++){
                int exp = hero_config["hero_level_up_exp"][i][j];
                v.push_back(exp);
                sum += exp;
                sumv.push_back(sum);
            }
            hero_level_up_exp_.push_back(v);
            hero_exp_sum_.push_back(sumv);
        }
        for(int i = 0;i<sys_config["hero_exp_fix"].getLength();i++){
            //hero_exp_sum_[i+1][1] = sys_config["hero_exp_fix"][i];
            hero_exp_sum_[i+1][1] = (int)(hero_exp_sum_[i+1][2] * 0.8);
        }
        readIntVectorFillZero(hero_level_up_gold_cost_, hero_config, "hero_level_up_gold_cost");
        //soldier
        Setting &soldier_config = st["soldier"];
        for(int i = 0;i<soldier_config["soldier_info"].getLength();i++){
            SoldierConfig sc;
            sc.mid_ = soldier_config["soldier_info"][i]["mid"];
            if(soldier_config_.find(sc.mid_) == soldier_config_.end()){
                vector<SoldierConfig> scv;
                scv.clear();
                soldier_config_.insert(make_pair(sc.mid_, scv));
            }
            sc.star_ = soldier_config["soldier_info"][i]["star"];
            while((int) soldier_config_[sc.mid_].size()<= sc.star_){
                SoldierConfig tmp;
                soldier_config_[sc.mid_].push_back(tmp);
            }
            sc.leadership_require_ = soldier_config["soldier_info"][i]["leadership"];
            sc.price_ = soldier_config["soldier_info"][i]["price"];
            sc.price_plus_ = soldier_config["soldier_info"][i]["price_plus"];
            sc.level_limit_ = soldier_config["soldier_info"][i]["level_limit"];
            sc.type_ = soldier_config["soldier_info"][i]["type"];
            sc.fake_exp_ = soldier_config["soldier_info"][i]["fake_exp"];
            soldier_config_[sc.mid_][sc.star_] = sc;
        }
        for(int i = 0;i<soldier_config["soldier_enhance"].getLength();i++){
            int star = soldier_config["soldier_enhance"][i]["star"];
            while((int) soldier_enhance_.size()<=star){
                vector<EnhanceCost> evc;
                evc.clear();
                soldier_enhance_.push_back(evc);
            }
            for(int j = 0;j<soldier_config["soldier_enhance"][i]["cost"].getLength();j++){
                EnhanceCost ec;
                readEnhanceCost(ec, soldier_config["soldier_enhance"][i]["cost"][j]);
                soldier_enhance_[star].push_back(ec);
            }
        }
        vector<int> soldier_level_up_exp_v_tmp;
        soldier_level_up_exp_v_tmp.clear();
        soldier_level_up_exp_.clear();
        soldier_exp_sum_.clear();
        soldier_level_up_exp_.push_back(soldier_level_up_exp_v_tmp);
        soldier_exp_sum_.push_back(soldier_level_up_exp_v_tmp);
        for(int i = 0;i<soldier_config["soldier_level_up_exp"].getLength();i++){
            vector<int> v;
            vector<int> sumv;
            v.clear();
            sumv.clear();
            v.push_back(0);
            sumv.push_back(0);
            sumv.push_back(0);
            int sum = 0;;
            for(int j = 0;j<soldier_config["soldier_level_up_exp"][i].getLength();j++){
                int exp = soldier_config["soldier_level_up_exp"][i][j];
                v.push_back(exp);
                sum += exp;
                sumv.push_back(sum);
            }
            soldier_level_up_exp_.push_back(v);
            soldier_exp_sum_.push_back(sumv);
        }
        for(int i = 0;i<sys_config["soldier_exp_fix"].getLength();i++){
            //soldier_exp_sum_[i+1][1] = sys_config["soldier_exp_fix"][i];
            soldier_exp_sum_[i+1][1] = (int)(soldier_exp_sum_[i+1][2]*0.8);
        }
        readIntVectorFillZero(soldier_level_up_gold_cost_, soldier_config, "soldier_level_up_gold_cost");
        Setting &soul_config = st["soul"];
        souls_.clear();
        Soul soul_tmp;
        souls_.push_back(soul_tmp);
        for(int i = 0 ;i < soul_config["soul_info"].getLength();i++){
            Soul soul;
            readSoul(soul, soul_config["soul_info"][i]);
            souls_.push_back(soul);
        }
        //mail
        Setting &mail_config = st["mail"];
        mail_config_.clear();
        for(int i = 0;i<mail_config["system_mail"].getLength();i++){
            MailConfig mc ;
            mc.mid = mail_config["system_mail"][i]["mid"];
            mc.start_time = mail_config["system_mail"][i]["start"];
            mc.end_time = mail_config["system_mail"][i]["end"];
            mc.title = (const char * )mail_config["system_mail"][i]["title"];
            mc.reward.clear();
            for(int j=0;j<mail_config["system_mail"][i]["reward"].getLength();j++){
                StageReward sr;
                readStageReward(sr, mail_config["system_mail"][i]["reward"][j]);
                mc.reward.push_back(sr);
            }
            mail_config_.push_back(mc);
        }
        //mission
        Setting &mission_config = st["mission"];
        mission_config_.clear();
        for(int i = 0 ;i<=MISSION_ADD_FRIEND;i++){
            vector<MissionConfig> v;
            v.clear();
            mission_config_.push_back(v);
        }
        for(int i = 0;i<mission_config["mission_list"].getLength();i++){
            MissionConfig mc;
            mc.type = mission_config["mission_list"][i]["type"];
            while(mc.type >= (int) mission_config_.size()){
                vector<MissionConfig> v;
                v.clear();
                mission_config_.push_back(v);
            }
            mc.subtype = mission_config["mission_list"][i]["subtype"];
            while(mc.subtype >= (int) mission_config_[mc.type].size()){
                mission_config_[mc.type].resize(mc.subtype + 1);
            }
            mc.param = mission_config["mission_list"][i]["param"];
            mc.link_id = mission_config["mission_list"][i]["link_id"];
            for(int j = 0;j<mission_config["mission_list"][i]["reward"].getLength();j++){
                StageReward sr;
                readStageReward(sr, mission_config["mission_list"][i]["reward"][j]);
                mc.reward.push_back(sr);
            }
            mission_config_[mc.type][mc.subtype] = mc;
        }
        //summon
        Setting &summon_config = st["summon"];
        summon_items_.clear();
        summon_pby_.clear();
        //pay attention to this bug
        //summon_pby_.push_back(0.0);
        for(int i = 0;i<summon_config["summon_list"].getLength();i++){
            StageReward sr;
            readStageReward(sr, summon_config["summon_list"][i]);
            summon_items_.push_back(sr);
            summon_pby_.push_back(summon_config["summon_pby"][i]);
        }
        summon_items_fix_.clear();
        summon_pby_fix_.clear();
        for(int i = 0;i<summon_config["summon_list_fix"].getLength();i++){
            StageReward sr;
            readStageReward(sr, summon_config["summon_list_fix"][i]);
            summon_items_fix_.push_back(sr);
            summon_pby_fix_.push_back(summon_config["summon_pby_fix"][i]);
        }
        
        diamond_summon_items_.clear();
        diamond_summon_pby_.clear();
        //diamond_summon_pby_.push_back(0.0);
        for(int i = 0;i<summon_config["diamond_summon_list"].getLength();i++){
            StageReward sr;
            readStageReward(sr, summon_config["diamond_summon_list"][i]);
            diamond_summon_items_.push_back(sr);
            diamond_summon_pby_.push_back(summon_config["diamond_summon_pby"][i]);
        }
        diamond_summon_items_fix_.clear();
        diamond_summon_pby_fix_.clear();
        for(int i = 0;i<summon_config["diamond_summon_list_fix"].getLength();i++){
            StageReward sr;
            readStageReward(sr, summon_config["diamond_summon_list_fix"][i]);
            diamond_summon_items_fix_.push_back(sr);
            diamond_summon_pby_fix_.push_back(summon_config["diamond_summon_pby_fix"][i]);
        }
        
        //payment
        Setting &payment_config = st["payment"];
        payment_.clear();
        /*for(int i = 0;i<payment_config["payment_list"].getLength();i++){
         string product_id = payment_config["payment_list"][i]["product_id"];
         int amount = payment_config["payment_list"][i]["amount"];
         payment_.insert(make_pair(product_id, amount));
         }*/
        for(int i = 0;i<payment_config["payment_list"].getLength();i++){
            string product_id = payment_config["payment_list"][i]["product_id"];
            int amount = payment_config["payment_list"][i]["amount"];
            string price = payment_config["payment_list"][i]["price"];
            int local_id = payment_config["payment_list"][i]["local_id"];
            PaymentConfig * pc = new PaymentConfig(product_id,price,amount,local_id);
            payment_.insert(make_pair(product_id,pc));
        }
        //string pid = "test.com.companyname.appname.diamonds1100";
        //payment_.insert(make_pair(pid, 1100));
        
        act_config_.clear();
        Setting &act_config = st["act"];
        for(int i = 0;i<act_config["actconfig"].getLength();i++){
            ActConfig * ac = new ActConfig();
            ac->act_id_ = act_config["actconfig"][i]["act_id"];
            ac->start_ = act_config["actconfig"][i]["start"];
            ac->end_ = act_config["actconfig"][i]["end"];
            ac->version_ = act_config["actconfig"][i]["version"];
            act_config_.insert(make_pair(ac->act_id_,ac));
        }

    }
    catch(ParseException ex){
        LOG4CXX_ERROR(logger_, "system;Parse config "<<game_config_.c_str()<<" failed at line "<<ex.getLine());
        return false;
    }
    catch(FileIOException ex){
        LOG4CXX_ERROR(logger_, "system;Read config "<<game_config_.c_str()<<" failed");
        return false;
    }
    catch(SettingNotFoundException ex){
        LOG4CXX_ERROR(logger_, "system;Read config "<<game_config_.c_str()<<" failed, setting "<<ex.getPath()<<" not found");
        return false;
    }
    
    LOG4CXX_INFO(logger_,"system;Load config succ");
    return true;
}
int GameConfig::leadershipLimit(int ulevel)
{
    return leadership_level_limit_[ulevel];
}
int GameConfig::heroLeadershipRequire(int mid, int star)
{
    map<int,vector<HeroConfig> >::iterator it = hero_config_.find(mid);
    if(it == hero_config_.end()) return -1;
    if(star >= (int) (it->second).size()) return -1;
    return (it->second)[star].leadership_require_;
}
int GameConfig::soldierLeadershipRequire(int mid, int star)
{
    map<int,vector<SoldierConfig> >::iterator it = soldier_config_.find(mid);
    if(it == soldier_config_.end()) return -1;
    if(star >= (int) (it->second).size()) return -1;
    return (it->second)[star].leadership_require_;
}
void GameConfig::readIntVectorFillZero(vector<int> &v, Setting & st,const char *name)
{
    v.clear();
    v.push_back(0);
    Setting &vaddress = st[name];
    for(int i = 0;i<vaddress.getLength();i++){
        v.push_back(vaddress[i]);
    }
}
void GameConfig::readReward(Reward &rew, Setting &st)
{
    rew.type = st[0];
    rew.subtype = st[1];
    rew.param_1 = st[2];
    rew.param_2 = st[3];
}
void GameConfig::readStageReward(StageReward &rew, Setting &st)
{
    rew.type = st[0];
    rew.subtype = st[1];
    rew.param_1 = st[2];
    rew.param_2 = st[3];
    rew.pby = st[4];
}
void GameConfig::readEnhanceCost(EnhanceCost &ec, Setting &st)
{
    ec.type = st[0];
    ec.mid = st[1];
    ec.param = st[2];
    ec.param2 = st[3];
    ec.position = st[4];
}
void GameConfig::readSoul(Soul &soul,Setting &st)
{
    soul.type = st[0];
    soul.mid = st[1];
    soul.star = st[2];
    soul.level = st[3];
    soul.amount = st[4];
    soul.gold = st[5];
}
Stage * GameConfig::getStage(int type, int chapter_id, int stage_id)
{
    if(type == STAGE_TYPE_NORMAL) return getNormalStage(chapter_id, stage_id);
    else if(type == STAGE_TYPE_ELITE) return getEliteStage(chapter_id, stage_id);
    else if(type == STAGE_TYPE_SPECIAL) return getSpecialStage(chapter_id, stage_id);
    else if (type == STAGE_TYPE_TRIAL) return getTrialStage(stage_id);
    return NULL;
}
Stage * GameConfig::getNormalStage(int chapter_id, int stage_id)
{
    if(chapter_id <=0 || chapter_id >= (int) stages_.size())
        return NULL;
    if(stage_id<=0 || stage_id >= (int) stages_[chapter_id].size())
        return NULL;
    if(stages_[chapter_id][stage_id].chapter_id_ == 0)
        return NULL;
    return &stages_[chapter_id][stage_id];
}
Stage * GameConfig::getEliteStage(int chapter_id, int stage_id)
{
    if(chapter_id <=0 || chapter_id >= (int) elite_stages_.size())
        return NULL;
    if(stage_id<=0 || stage_id >= (int) elite_stages_[chapter_id].size())
        return NULL;
    if(elite_stages_[chapter_id][stage_id].chapter_id_ == 0)
        return NULL;
    return &elite_stages_[chapter_id][stage_id];
}
Stage * GameConfig::getSpecialStage(int chapter_id, int stage_id)
{
    if(chapter_id <=0 || stage_id <= 0) return NULL;
    map<int,vector<Stage> >::iterator it = special_stages_.find(chapter_id);
    if(it == special_stages_.end()) return NULL;
    if(stage_id >= (int) (it->second).size() ) return NULL;
    return &(it->second)[stage_id];
}

Stage * GameConfig::getTrialStage(int stage_id) {
    if (stage_id <= 0 || stage_id >= (int) trial_stages_.size()) {
        return NULL;
    }
    return &trial_stages_[stage_id];
}

int GameConfig::getValueFromIntVectorFilledZero(int index, vector<int> &v)
{
    if(index<=0||index>= (int) v.size()) return -1;
    return v[index];
}
HeroConfig * GameConfig::getHeroConfig(int mid,int star)
{
    map<int,vector<HeroConfig> >::iterator it = hero_config_.find(mid);
    if(it == hero_config_.end()) return NULL;
    if(star >= (int) (it->second).size()) return NULL;
    return &(it->second)[star];
}
SoldierConfig * GameConfig::getSoldierConfig(int mid,int star)
{
    map<int,vector<SoldierConfig> >::iterator it = soldier_config_.find(mid);
    if(it == soldier_config_.end()) return NULL;
    if(star >= (int) (it->second).size()) return NULL;
    return &(it->second)[star];
}
Soul * GameConfig::getSoulConfig(int mid)
{
    if(mid<=0||mid>= (int) souls_.size()) return NULL;
    return &souls_[mid];
}
MailConfig * GameConfig::getMailConfig(int mid)
{
    for(size_t i = 0;i<mail_config_.size();i++){
        if(mail_config_[i].mid == mid)
            return &mail_config_[i];
    }
    return NULL;
}
MissionConfig * GameConfig::getMissionConfig(int type, int subtype)
{
    if(type < 0 || type >= (int) mission_config_.size()) return NULL;
    if(subtype < 0|| subtype >= (int) mission_config_[type].size()) return NULL;
    return &mission_config_[type][subtype];
}
StageReward * GameConfig::getSummonReward()
{
    double p = (rand()%RAND_MAX_CUSTOM)/(double)RAND_MAX_CUSTOM;
    int pos = int(lower_bound(summon_pby_.begin(), summon_pby_.end(), p) - summon_pby_.begin());
    if(pos < 0) pos = 0;
    if(pos >= (int) summon_pby_.size()) pos = int(summon_pby_.size()-1);
    return &summon_items_[pos];
}
StageReward * GameConfig::getSummonRewardFix()
{
    double p = (rand()%RAND_MAX_CUSTOM)/(double)RAND_MAX_CUSTOM;
    int pos = int(lower_bound(summon_pby_fix_.begin(), summon_pby_fix_.end(), p) - summon_pby_fix_.begin());
    if(pos <0 ) pos = 0;
    if(pos >= (int) summon_pby_fix_.size()) pos = int(summon_pby_fix_.size()-1);
    return &summon_items_fix_[pos];
}
StageReward * GameConfig::getDiamondSummonReward()
{
    double p = (rand()%RAND_MAX_CUSTOM)/(double)RAND_MAX_CUSTOM;
    int pos = int(lower_bound(diamond_summon_pby_.begin(), diamond_summon_pby_.end(), p) - diamond_summon_pby_.begin());
    if(pos < 0) pos = 0;
    if(pos >= (int) diamond_summon_pby_.size()) pos = int(diamond_summon_pby_.size()-1);
    return &diamond_summon_items_[pos];
}
StageReward * GameConfig::getDiamondSummonRewardFix()
{
    double p = (rand()%RAND_MAX_CUSTOM)/(double)RAND_MAX_CUSTOM;
    int pos = int(lower_bound(diamond_summon_pby_fix_.begin(), diamond_summon_pby_fix_.end(), p) - diamond_summon_pby_fix_.begin());
    if(pos < 0) pos = 0;
    if(pos >= (int) diamond_summon_pby_fix_.size()) pos = int(diamond_summon_pby_fix_.size()-1);
    return &diamond_summon_items_fix_[pos];
}
sconfig * GameConfig::getSconfig(int sid)
{
    map<int,sconfig *>::iterator it = serverconfigs_.find(sid);
    if(it!=serverconfigs_.end())
        return it->second;
    else{
        return NULL;
    }
}
PaymentConfig * GameConfig::getPaymentConfig(string &product_id)
{
    map<string,PaymentConfig * >::iterator it = payment_.find(product_id);
    if(it == payment_.end()) return NULL;
    return it->second;
}
PaymentConfig * GameConfig::getPaymentConfigByLocal(int local)
{
    for(map<string,PaymentConfig *>::iterator it = payment_.begin();
        it!=payment_.end();it++){
        if(it->second->local_id_ == local){
            return it->second;
        }
    }
    return NULL;
}
ActConfig * GameConfig::getActConfig(int act_id)
{
    map<int,ActConfig*>::iterator it = act_config_.find(act_id);
    if(it == act_config_.end()) return NULL;
    long long now = time(NULL);
    if(now < it->second->start_ || now > it->second->end_) return NULL;
    return it->second;
}
