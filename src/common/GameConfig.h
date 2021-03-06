//
//  GameConfig.h
//  hero
//
//  Created by xujianfeng on 13-8-19.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_GameConfig_h
#define hero_GameConfig_h

#include "../class/Hero.h"
#include "../class/Soldier.h"
#include "../class/Stage.h"
#include "../class/Item.h"
#include "../class/Mail.h"
#include "../class/Mission.h"
#include "../class/Payment.h"
#include "../class/Act.h"
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
//#include </opt/local/include/libconfig.h++>
//#include "3rd-party/libconfig/include/libconfig.h++"
#include <libconfig.h++>
#include <log4cxx/logger.h>
#include <log4cxx/log4cxx.h>
#include <log4cxx/propertyconfigurator.h>
//#include "3rd-party/lib4cxx/include/log4cxx/logger.h"
//#include "3rd-party/lib4cxx/include/log4cxx/log4cxx.h"
//#include "3rd-party/lib4cxx/include/log4cxx/propertyconfigurator.h"
using namespace std;
using namespace libconfig;
struct sconfig{
    int server_id;
    int server_type;
    string ip;
    int port;
    string db_address;
    string db_user;
    string db_passwd;
    string db_name;
    int db_port;
    
    string pay_db_address;
    string pay_db_user;
    string pay_db_passwd;
    string pay_db_name;
    int pay_db_port;
};

class GearConf {
public:
    GearConf():mid_(0),star_(0),position_(0) {}
    int mid_;
    int star_;
    int position_; //部位
};

class ItemConf {
public:
    ItemConf(): type_(0), subtype_(0), amount_(0) {}
    int type_;
    int subtype_;
    int amount_;
};

class BuildConf {
public:
    BuildConf() :mid_(0), level_(0), req_user_level_(0), req_tower_level_(0) {}
    int mid_;
    int level_;
    int req_user_level_;
    int req_tower_level_; //主基地等级 当mid不是主基地时才有效
    vector <Reward> req_items_;
    vector <Reward> reward_items_;
    vector <int> positions_;
};

class HonorExcConf {
public:
    HonorExcConf() : index_(0), req_user_level_(0), req_honor_(0), daily_exc_limit_(0), all_exc_limit_(0) {}
    int index_;
    int req_user_level_;
    int req_honor_;
    int daily_exc_limit_;
    int all_exc_limit_;
    Reward reward;
};

class GemConf {
public:
    int mid_;
    int req_gold_;
    int req_diamond_;
};

class GameConfig
{
public:
    GameConfig(const char * server_config,const char * game_config);
    ~GameConfig();
    
    bool loadFromFile();
    
    int leadershipLimit(int ulevel);
    int heroLeadershipRequire(int mid,int star);
    int soldierLeadershipRequire(int mid,int star);
    int getHeroStarLevelLimit(int star);
    
    Stage * getStage(int type,int chapter_id,int stage_id);
    Stage * getNormalStage(int chapter_id,int stage_id);
    Stage * getEliteStage(int chapter_id,int stage_id);
    Stage * getSpecialStage(int chapter_id,int stage_id);
    //TODO
    Stage*  getTrialStage(int chapter_id, int stage_id);
    
    log4cxx::LoggerPtr logger_;
    string server_config_;
    string game_config_;
    Config scfg_;
    Config gcfg_;
    
    //server
    /*int server_type_;
    string local_address_;
    int local_port_;
    string db_address_;
    string db_user_;
    string db_passwd_;
    string db_name_;
    int db_port_;*/
    map<int,sconfig * > serverconfigs_;
    
    //game
    vector<int> leadership_level_limit_;
    vector<int> energy_level_limit_;
    vector<int> exp_level_limit_;
    vector<int> hero_length_limit_;
    vector<int> soldier_length_limit_;
    vector<int> friend_length_limit_;
    vector<int> recharge_level_limit_;
    
    //stage
    vector< vector<Stage> > stages_;
    vector< vector<Stage> > elite_stages_;
    map<int , vector<Stage> > special_stages_;
    //TODO
    map <int, vector <Stage> > trial_stages_;
    
    
    //system
    int fresh_interval_;
    int fresh_step_count_;
    int fresh_level_size_;
    int friend_fresh_interval_;
    int fresh_choice_num_;
    int fresh_friend_num_limit_;
    int fresh_level_limit_;
    int friend_request_limit_;
    
    int recover_interval_;
    int spark_assistant_is_friend_;
    int spark_assistant_not_friend_;
    int spark_daily_limit_;
    int spark_sum_limit_;
    int send_energy_daily_limit_;
    int accept_energy_daily_limit_;
    int energy_per_act_;
    int p2p_mail_limit_;
    int system_mail_limit_;
    int p2p_mail_length_limit_;
    float enhance_exp_times_;
    float exp_to_gold_rate_hero_;
    float exp_to_gold_rate_soldier_;
    int summon_price_diamond_1_;
    int summon_price_diamond_10_;
    int summon_price_spark_1_;
    int summon_price_spark_10_;
    int elite_stage_daily_limit_;
    int normal_stage_reward_limit_;
    int elite_stage_reward_limit_;
    int special_stage_reward_limit_;
    int normal_stage_exp_rate_;
    int elite_stage_exp_rate_;
    int elite_stage_reset_cost_;
    
    long long time_area_fix_;
    
    int max_team_num_;
    
    int force_hero_exp_up_;
    int force_soldier_exp_up_;
    
    vector<int> energy_price_;
    
    //init user
    int init_user_gold_;
    int init_user_diamond_;
    Reward init_user_hero_;
    vector<Reward> init_user_soldier_;
    
    Reward fake_user_hero_;
    vector<Reward> fake_user_soldier_;
    vector<int> fake_user_skill_;
    string fake_user_nick_;
    string fake_user_eqid_;
    
    //hero
    map<int, vector<HeroConfig> > hero_config_;
    vector<vector<int> > hero_level_up_exp_;
    vector<vector<int> > hero_exp_sum_;
    vector<int> hero_level_up_gold_cost_;
    //map<int , vector<int> > hero_skill_id_;
    //map<int, vector<vector<EnhanceCost> > > skill_level_up_cost_;
    vector<vector<vector<EnhanceCost> > > skill_level_up_cost_;
    vector<vector<EnhanceCost> > hero_enhance_;
    
    //soldier
    map<int, vector<SoldierConfig> > soldier_config_;
    //vector<int> soldier_level_up_exp_;
    //vector<int> soldier_exp_sum_;
    vector<vector<int> > soldier_level_up_exp_;
    vector<vector<int> > soldier_exp_sum_;
    vector< vector<EnhanceCost> > soldier_enhance_;
    
    vector<int> soldier_level_up_gold_cost_;
    
    //soul
    vector<Soul> souls_;
    
    //mail
    vector<MailConfig> mail_config_;
    //mission
    vector<vector<MissionConfig> > mission_config_;
    //shopping
    int buy_energy_daily_limit_;
    int cost_per_buy_energy_;
    int expand_hero_limit_;
    int cost_per_expand_hero_;
    int place_per_expand_hero_;
    int expand_soldier_limit_;
    int cost_per_expand_soldier_;
    int place_per_expand_soldier_;
    int expand_friend_limit_;
    int cost_per_expand_friend_;
    int place_per_expand_friend_;
    int trial_times_per_day_;
    int trial_time_per_stage_;
    int trial_rank_num_;
    
    vector<StageReward> summon_items_;
    vector<double> summon_pby_;
    
    vector<StageReward> summon_items_fix_;
    vector<double> summon_pby_fix_;
    
    vector<StageReward> diamond_summon_items_;
    vector<double> diamond_summon_pby_;
    
    vector<StageReward> diamond_summon_items_fix_;
    vector<double> diamond_summon_pby_fix_;
    
    //payment
    map<string,PaymentConfig* > payment_;
    
    map<int,ActConfig *> act_config_;

    int pvp_req_user_level_;
    
    //pvp conf
    //gear :mid. start 1
    vector <GearConf> gear_conf_;
    //gear upgrade: position,star,enhance
    vector <vector <vector <int> > > gear_enhance_req_gold_;
    vector <vector <vector <int> > > gear_sell_gold_;
    //build :mid,level start 1 //到多少等级需要的配置
    vector <vector <BuildConf> > build_conf_;
    //honor exc: index
    vector <HonorExcConf> honor_exc_conf_;
    //gem:  mid 
    vector <GemConf> gem_conf_;
    //rank 全服排行版大小
    int pvp_rank_list_size_; 
    //排名显示
    vector <int> pvp_rank_interval_range_;
    vector <int> pvp_rank_interval_;
    //排名奖励
    vector <int> pvp_rank_reward_id_;
    vector <int> pvp_rank_reward_range_;
    vector <int> pvp_rank_reward_honor_;
    vector <vector <Reward> > pvp_rank_reward_;
    //掠夺系统奖励
    vector <int>  rob_system_reward_id_;
    vector <int>  rob_system_reward_range_;
    vector <int> rob_system_reward_honor_;

    //
    int rob_range_before_;
    int rob_range_after_;
    int rob_player_num_;
    int rob_robot_num_;
    int rob_gold_percent_;
    int rob_log_percent_;
    int rob_stone_percent_;
    int pvp_win_honor_; //掠夺成功荣誉奖励
    int pvp_lose_honor_;//掠夺失败荣誉奖励

    int pvp_load_rank_size_;
    int pvp_rank_attack_before_;
    int pvp_rank_attack_after_;
    
    int pvp_rob_attack_daily_limit_;
    int pvp_rank_attack_daily_limit_;

    int pvp_init_wood_;
    int pvp_init_stone_;


    //util
    void readIntVectorFillZero(vector<int> &v,Setting & st,const char * name);
    void readIntVector(vector <int> &v, Setting &st, const char *tit);
    void readReward(Reward &rew,Setting &st);
    void readReward(vector <Reward> &v, Setting &st);
    void readReward(vector <vector <Reward> > &v, Setting &st, const char *tit);
    void readStageReward(StageReward &rew,Setting &st);
    void readEnhanceCost(EnhanceCost &ec,Setting &st);
    void readSoul(Soul &soul,Setting &st);
    int getValueFromIntVectorFilledZero(int index,vector<int> &v);
    HeroConfig * getHeroConfig(int mid,int star);
    SoldierConfig * getSoldierConfig(int mid,int star);
    Soul * getSoulConfig(int mid);
    MailConfig * getMailConfig(int mid);
    MissionConfig *getMissionConfig(int type,int subtype);
    StageReward * getSummonReward();
    StageReward * getSummonRewardFix();
    StageReward * getDiamondSummonReward();
    StageReward * getDiamondSummonRewardFix();
    sconfig * getSconfig(int sid);
    PaymentConfig * getPaymentConfig(string &product_id);
    PaymentConfig * getPaymentConfigByLocal(int local);
    ActConfig * getActConfig(int act_id);

    int  rewardCombine(int type);
    void insertRewardStage(vector <StageReward> &rewards, StageReward &sr);

    //vi 不能为空
    int randIdx(vector <int> &vi, int weight);
    bool checkInVector(vector <int> &vi, int n) { return find(vi.begin(), vi.end(), n) != vi.end(); }

    /////////////////////////////////////////////////
    vector<int> act_2_level_;
    vector< vector<StageReward> > act_2_reward_;
    vector<int> act_3_level_;
    vector< vector<StageReward> > act_3_reward_;
};

#endif
