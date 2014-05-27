//
//  Const.h
//  hero
//
//  Created by xujianfeng on 13-8-5.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_Const_h
#define hero_Const_h

#include <string>
using namespace std;

//General def
typedef long long uid_type;
typedef int gold_type;

#define APPID_91 112338
#define APPKEY_91 "c095277dcb60b19569d198b8f125fbccce581198ed5600fa"
#define VERIFYADDR_91 "http://service.sj.91.com/usercenter/AP.aspx"

#define FAKE_USER_ID -1

#define PAYMENT_STATUS_REQUEST_REFUSED -1
#define PAYMENT_STATUS_REQUEST_RECEIVED 0
#define PAYMENT_STATUS_VERIFY_SEND 1
#define PAYMENT_STATUS_VERIFY_SUCC 2
#define PAYMENT_STATUS_ADDCMD_SEND 3
#define PAYMENT_STATUS_ADDCMD_SUCC 4

#define NEW_PAYMENT_RECEIVED 1
#define NEW_PAYMENT_SENDDING 2
#define NEW_PAYMENT_COMPLETE 3

#define NEWS_MAX_STAR 5
#define NEWS_TYPE_SUMMON 1
#define NEWS_TYPE_ENHANCE 2
#define NEWS_TYPE_EXPUP 3

#define SERVER_TYPE_LOGIN 0
#define SERVER_TYPE_PAYMENT 1
#define SERVER_TYPE_LOGIC 2

#define RAND_MAX_CUSTOM 65535

#define READCACHESIZE 10240
#define WRITECACHESIZE 19240

#define PBY_PRECISION 1000
#define SOLDIER_INGROUND 4
#define MAX_SKILL_POSITION 3
#define MAX_GUIDE_STEP 4

#define IS_LOCKED 1
#define NO_ENERGY_TO_ACCEPT 0
#define HAVE_ENERGY_TO_ACCEPT 1
#define MAX_ITEM_AMOUNT 999

#define SOUL_TYPE_HERO 1
#define SOUL_TYPE_SOLDIER 2

//mail type
enum MAIL_TYPE_ {
    MAIL_TYPE_TRIAL_INSTANT = 4,
};

#define SYSTEM_MAIL_SENDER_ID 0
#define SYSTEM_MAIL_RECORD_INTERVAL 30
#define SYSTEM_MAIL_RECORD_LENGTH 3
#define MAIL_NOT_READ 0
#define MAIL_READ 1
#define MAIL_NO_ITEM 0
#define MAIL_WITH_ITEM 1
#define MAIL_ITEM_ACCEPTED 2

#define MISSION_NOT_COMPLETED 0
#define MISSION_COMPLETED 1
#define MISSION_NO_MISSION_TO_LINK -1

#define MISSION_DAILY_REWARD 0
#define MISSION_LEVEL_UP 1
#define MISSION_NORMAL_CHAPTER_CLEAR 2
#define MISSION_ELITE_CHAPTER_CLEAR 3
#define MISSION_LOGIN_SUM 4
#define MISSION_SEND_ENERGY 5
#define MISSION_ADD_FRIEND 6
//Special def

#define FRESH_LEVEL_INTERVAL 10
#define FRESH_LEVEL_STEP_COUNT 5
#define FRESH_LEVEL_SIZE 20

//stage type
#define STAGE_TYPE_NORMAL 1
#define STAGE_TYPE_ELITE 2
#define STAGE_TYPE_SPECIAL 3
#define STAGE_TYPE_TRIAL   4

//load status
#define LOADED_USER_INFO 0x1
#define LOADED_HERO_INFO 0x2
#define LOADED_SOLDIER_INFO 0x4
#define LOADED_ITEM_INFO 0x8
#define LOADED_FRIEND_INFO 0x10
#define LOADED_FRIENDREQUEST_INFO 0x20
#define LOADED_MAIL 0x40
#define LOADED_MISSION 0x80
#define LOADED_STAGERECORD 0X100
#define LOADED_ACT_RECORD 0x200
#define LOADED_TEAM 0x400
//item type
#define ITEM_TYPE_HERO 0
#define ITEM_TYPE_SOLDIER 1
#define ITEM_TYPE_RESOURCE 2
#define ITEM_TYPE_CHIP 3
#define ITEM_TYPE_GOLD 4
#define ITEM_TYPE_DIAMOND 5

//cmd_code
#define CMD_REGISTER 0
#define CMD_LOGIN 1
#define CMD_AUTHORIZE 2
#define CMD_CHECKNICK 3
#define CMD_CREATEUSER 4
#define CMD_LOADALL 5
#define CMD_LOADUSER 6
#define CMD_LOADHERO 7
#define CMD_LOADSOLDIER 8
#define CMD_LOADITEM 9
#define CMD_CHANGEHERO 10
#define CMD_CHANGESOLDIER 11
#define CMD_LOADASSISTANT 12
#define CMD_BATTLESTART 13
#define CMD_BATTLEEND 14
#define CMD_FRIENDREQUEST 15
#define CMD_SELLHERO 16
#define CMD_SELLSOLDIER 17
#define CMD_LOCKHERO 18
#define CMD_LOCKSOLDIER 19
#define CMD_HEROEXPUP 20
#define CMD_SOLDIEREXPUP 21
#define CMD_HEROENHANCE 22
#define CMD_SOLDIERENHANCE 23
#define CMD_SKILLLEVELUP 24
#define CMD_SENDENERGY 25
#define CMD_ACCEPTENERGY 26
#define CMD_LOADFRIEND 27
#define CMD_LOADFRIENDREQUEST 28
#define CMD_ADDFRIEND 29
#define CMD_REFUSEFRIENDREQ 30
#define CMD_DELETEFRIEND 31
#define CMD_SOULMERGE 32
#define CMD_LOADUSERBYID 33
#define CMD_LOADMAIL 34
#define CMD_OPENMAIL 35
#define CMD_DELETEMAIL 36
#define CMD_SENDMAIL 37
#define CMD_ACCEPTMAILITEM 38
#define CMD_GETNEWMAILNUM 39
#define CMD_LOADMISSION 40
#define CMD_GETCMNUM 41
#define CMD_GETMISSIONREWARD 42
#define CMD_BUYENERGY 43
#define CMD_EXPANDHERO 44
#define CMD_EXPANDSOLDIER 45
#define CMD_EXPANDFRIEND 46
#define CMD_SUMMON 47
#define CMD_LOADNEWS 48
#define CMD_PAYMENTVERIFY 49
#define CMD_PAYMENTVERIFYRECEIVED 50
#define CMD_ADMINALTERUSER 51
#define CMD_ADMINADDHERO 52
#define CMD_ADMINALTERHERO 53
#define CMD_ADMINADDSOLDIER 54
#define CMD_ADMINADDITEM 55
#define CMD_LOG 56
#define CMD_GUIDE 57
#define CMD_ALTERUSERNAME 58
#define CMD_PAYMENTVERIFYCOMPLETE 59
#define CMD_PAYMENT_ADD_DIAMOND_REQUEST 60
#define CMD_PAYMENT_ADD_DIAMOND_RESPONSE 61
#define CMD_PAYMENTTEST 62
#define CMD_SESSION_VERIFY 63
#define CMD_NEWPAYMENTVERIFY 64
#define CMD_PBAK 65
#define CMD_LOADACT 66
#define CMD_ACT 67
#define CMD_LOADTEAM 68
#define CMD_SAVETEAM 69
#define CMD_CHANGETEAM 70
#define CMD_CHANGENICK 71
#define CMD_FORCEHEU 72
#define CMD_FORCESEU 73
#define CMD_ADDFAKEUSER 74
#define CMD_DEBUG 75
#define CMD_INSTANT_BATTLE 76
#define CMD_RESET_ELITE_STAGE 77
#define CMD_INSTANT_TRIAL 78
#define CMD_TRIAL_PROGRESS 79
#define CMD_GET_TRIAL_RANK 80
#define CMD_TRIAL_RESET 81
#define CMD_TRIAL_RELIVE 82

//error
#define ERROR_SYSTEM_CRITICAL_ERROR 1
#define ERROR_USERNAME_HAS_BEEN_USED 2
#define ERROR_USER_OR_PASSWD_WRONG 3
#define ERROR_EQID_WRONG 4
#define ERROR_USER_DONOT_EXIST 5
#define ERROR_AUTHORIZE_CODE_WRONG 6
#define ERROR_AUTHORIZE_FAIL 7
#define ERROR_HERO_NOT_FOUND 8
#define ERROR_OWNER_DISMATCH 9
#define ERROR_NICKNAME_USED 10
#define ERROR_USER_ALREADY_REGISTERED 11
#define ERROR_CREATE_USER_FAIL 12
#define ERROR_SOLDIER_NOT_FOUND 13
#define ERROR_SOLDIER_HAS_BEEN_SET 14
#define ERROR_SOLDIER_THE_SAME_MID 15
#define ERROR_LEADERSHIP_NOT_ENOUGH 16
#define ERROR_STAGE_NOT_EXIST 17
#define ERROR_ENERGY_NOT_ENOUGH 18
#define ERROR_STAGE_LOCKED 19
#define ERROR_BATTLE_NOT_INITED 20
#define ERROR_ALREADY_BEEN_FRIENDS 21
#define ERROR_TARGET_USER_NOT_FOUND 22
#define ERROR_ALREADY_HAS_FRIEND_REQUEST 23
#define ERROR_LEVEL_NOT_ENOUGH 24
#define ERROR_ASSISTANT_NOT_EXIST 25
#define ERROR_BAG_IS_FULL 26
#define ERROR_HERO_IS_IN_ARMY 27
#define ERROR_SOLDIER_IS_IN_ARMY 28
#define ERROR_HERO_IS_LOCKED 29
#define ERROR_SOLDIER_IS_LOCKED 30
#define ERROR_CONFIG_ERROR 31
#define ERROR_GOLD_NOT_ENOUGH 32
#define ERROR_CANNOT_PAY_COST 33
#define ERROR_NOT_FRIEND 34
#define ERROR_CANNOT_SEND_ENERGY_TODAY 35
#define ERROR_HAVE_SENT_ENERGY 36
#define ERROR_NO_ENERGY_TO_ACCEPT 37
#define ERROR_CANNOT_ACCEPT_ENERY_TODAY 38
#define ERROR_NO_NEED_TO_ACCEPT_ENERGY 39
#define ERROR_SELF_NO_MORE_FRIEND 40
#define ERROR_TARGET_NO_MORE_FRIEND 41
#define ERROR_NO_SUCH_REQUEST 42
#define ERROR_CANNOT_FIND_SOUL_CONFIG 43
#define ERROR_POSITION_ERROR 44
#define ERROR_CANNOT_FIND_MAIL 45
#define ERROR_MAIL_TOO_LONG 46
#define ERROR_NOT_SYSTEM_MAIL 47
#define ERROR_NOT_ACCEPTABLE 48
#define ERROR_CANNOT_FIND_MISSION 49
#define ERROR_MISSION_NOT_COMPLETE 50
#define ERROR_NO_MORE_BUY_ENERGY 51
#define ERROR_GOT_LIMIT 52
#define ERROR_UID_CONFILICT 53
#define ERROR_NO_NEED_TO_SEND_REQ 54
#define ERROR_GUIDE_RECORD_WRONG 55
#define ERROR_PAYMENT_VERIFY_FAIL 56
#define ERROR_PAYMENT_ORDER_INFO_NOT_MATCH 57
#define ERROR_PAYMENT_WRONG_TIME 58
#define ERROR_PAYMENT_TRADE_COMPLETE 59
#define ERROR_PAYMENT_INFO_ERROR 60
#define ERROR_TIME_NOT_FIT 61
#define ERROR_SESSIONID_ERROR 62
#define ERROR_PAYMENT_NOT_RECEIVED_YET 63
#define ERROR_ACT_NOT_AVAIABLE 64
#define ERROR_TEAM_CANNOT_BE_FOUND 65
#define ERROR_STAGE_NOT_PASSED 66
#define ERROR_TRIAL_RESET_COUNT_ERROR 67
#define ERROR_TRIAL_RESET_NO_RECORD 68
#define ERROR_TRIAL_INSTANTING 69
#define ERROR_TRIAL_NO_INSTANT 70
#define ERROR_TRIAL_SAVE_STAGE_RECORD 71
#define ERROR_TRIAL_RELIVE_STAGE_ERROR 72
#define ERROR_TRIAL_RELIVE_DIAMOND_NOT_ENOUGH 73

#define CMD_TYPE 83 
static string cmd_list[] = {
    "register",//0
    "login",//1
    "authorize",//2
    "checknick",//3
    "createuser",//4
    "loadall",//5
    "loaduser",//6
    "loadhero",//7
    "loadsoldier",//8
    "loaditem",//9
    "changehero",//10
    "changesoldier", //11
    "loadassistant",//12
    "battlestart",//13
    "battleend",//14
    "friendrequest",//15
    "sellhero",//16
    "sellsoldier",//17
    "lockhero",//18
    "locksoldier",//19
    "heroexpup",//20
    "soldierexpup",//21
    "heroenhance",//22
    "soldierenhance",//23
    "skilllevelup",//24
    "sendenergy",//25
    "acceptenergy",//26
    "loadfriend",//27
    "loadfriendreq",//28
    "addfriend",//29
    "refusefriendreq",//30
    "deletefriend",//31
    "soulmerge",//32
    "loaduserbyid",//33
    "loadmail",//34
    "openmail",//35
    "deletemail",//36
    "sendmail",//37
    "acceptmailitem",//38
    "getnewmailnum",//39
    "loadmission",//40
    "getcmnum",//41
    "getmissionreward",//42
    "buyenergy",//43
    "expandhero",//44
    "expandsoldier",//45
    "expandfriend",//46
    "summon",//47
    "loadnews",//48
    "paymentverify",//49
    "paymentverifyreceived",//50
    "adminalteruser",//51
    "adminaddhero",//52
    "adminalterhero",//53
    "adminaddsoldier",//54
    "adminadditem",//55
    "log",//56
    "guiderecord",//57
    "alterusername",//58
    "paymentverifycomplete",//59
    "paymentadddiamondreq",//60
    "paymentadddiamondres",//61
    "paymenttest",//62
    "sessionVerify",//63
    "newpaymentverify",//64
    "pbak",//65
    "loadact",//66
    "act",//67
    "loadteam",//68
    "saveteam",//69
    "changeteam",//70
    "changenick",//71
    "forceheu",//72
    "forceseu",//73
    "addfakeuser",//74
    "debug",//75
    "instantbattle",//76
    "resetelitestage",//77
    "instanttrial", //78
    "trialprogress", //79
    "trialrank", //80
    "resettrial", //81
    "trialrelive",//82
};

#define ACT_1 1 //首充三倍
#define ACT_2 2 //连登奖励
#define ACT_3 3 //冲级活动

#endif
