//
//  Message.h
//  hero
//
//  Created by xujianfeng on 13-8-13.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_Message_h
#define hero_Message_h

#include "../common/Const.h"

#include "../utils/Util.h"
#include "../class/User.h"
#include "../class/Hero.h"
#include "../class/Soldier.h"
#include "../class/Reward.h"
#include "../common/GameConfig.h"
#include "../class/Mail.h"
#include "../class/Mission.h"
#include "../utils/md5.h"
#include "../utils/hstring.h"
#include <string>

using namespace std;
extern GameConfig game_config;
extern string cmd_list[];
string buildErrorResponse(int cmd_code,int error_index);
string buildErrorResponseForPayment(int cmd_code,int error_index,string &tran_id);
string buildRegisterResponse();
string buildLoginResponse(uid_type uid,int auth_code);
string buildAuthorizeResponse();
string buildCheckNickResponse();
string buildLoadAllResponse(User * user);
string buildLoadUserResponse(User * user);
string buildLoadHeroResponse(User * user);
string buildLoadSoldierResponse(User * user);
string buildLoadItemResponse(User * user,int item_type);
string buildChangeHeroResponse(User * user);
string buildChangeSoldierResponse(User * user);
string buildCreateUserResponse();
string buildLoadAssistantResponse(User * user,vector<User *> &assistants,int friend_index,vector<Hero *> &heroes);
string buildBattleStartResponse(User * user);
string buildUserRewardsContent(User *user);
string buildUserRewardsContent(vector <StageReward> &rewards);
int    parseUserRewardsContent(vector <StageReward>& strewards, const string &contents);
string buildBattleEndResponse(User * user,vector<Mission *> &mv);
string buildFriendRequestResponse();
string buildSellHeroResponse(User * user);
string buildSellSoldierResponse(User * user);
string buildLockHeroResponse();
string buildLockSoldierResponse();
string buildHeroExpUpResponse(User * user,Hero* hero);
string buildSoldierExpUpResponse(User * user,Soldier * soldier);
string buildHeroEnhanceResponse(User * user,Hero *hero);
string buildSoldierEnhanceResponse(User * user,Soldier * soldier);
string buildSkillLevelUpResponse(User * user,Hero * hero,int position);
string buildSendEnergyResponse(User *user,vector<Mission *> &mv);
string buildAcceptEnergyResponse(User * user);
string buildLoadFriendResponse(vector<User *> &users,vector<FriendInfo *> &frs,vector<Hero *> &heroes);
string buildLoadFriendRequestResponse(vector<User *> &users,vector<Hero *> &heroes);
string buildAddFriendResponse(vector<Mission *> &mv);
string buildRefuseFriendReqestResponse();
string buildDeleteFriendResponse();
string buildSoulMergeResponse(User * user);
string buildLoadUserByIdResponse(User * user,Hero * hero,vector<Soldier *> sv);
string buildLoadMailResponse(User * user,vector<Mail *> &mtmp,vector<User * > &utmp,vector<Hero * > &htmp);
string buildOpenMailResponse(Mail * mail);
string buildDeleteMailResponse(long long mail_id);
string buildSendMailResponse();
string buildAcceptMailItemResponse();
string buildGetNewMailNumResponse(int num);
string buildLoadMissionResponse(User * user);
string buildGetCMNumResponse(int num);
string buildGetMissionRewardResponse(User * user,long long mission_id);
string buildBuyEnergyResponse(int times);
string buildExpandHeroResponse();
string buildExpandSoldierResponse();
string buildExpandFriendResponse();
string buildSummonResponse(User * user,vector<StageReward * > v);
string buildLoadNewsResponse(vector<News *> nv);
string buildPaymentVerifyReceivedResponse(uid_type uid,string &product_id,string &tran_id,int diamond);
string buildPaymentVerifyReceivedResponse(User * user);
string buildAdminResponse();
string buildLogResponse();
string buildAlterUserNameResponse();
string buildGuideResponse();
string buildVerifyString(vector<string> &check_cmd);
string buildReceiptCmd(uid_type uid,int status,string &product_id,string & tran_id,int server_id);
string buildPaymentaddDiamondRequestCmd(uid_type uid,string &product_id,string & tran_id,int fd);
string buildPaymentAddDiamondResponseCmd(int succ,uid_type uid,string &product_id,string & tran_id,int diamond,int fd);
string buildSessionVerifyString(vector<string> &cmd);
string buildSessionVerifyCmd(long long uid,int status,string &nickname,string &eqid);
string buildSessionVerifyResponse();
string buildPBakResponse(User * user,NewPaymentInfo * npi);
string buildLoadActFailResponse(int succ,int act_id);
string buildLoadActResponse(int act_id,string &s);
string buildAct2Response(int reward_id,Act * act);
string buildAct3Response(int reward_id,Act * act);
string buildLoadTeamResponse(User * user);
string buildSaveTeamResponse(User * user,long long team_id);
string buildChangeTeamResponse(User * user, long long team_id);
string buildChangeNickResponse();
string buildForceHEUResponse();
string buildForceSEUResponse();
string buildInstantBattleResponse(User* user, vector<Mission*> &mv, vector<StageReward*> &rv);
string buildResetEliteStageResponse(User* user);

string buildLoadBuildingsResponse(User *user);
string buildLoadBuildingsByIdResponse(User *user, int cmd_code, int bttype);

#endif
