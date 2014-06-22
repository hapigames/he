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
enum STAGE_TYPE_ {
    STAGE_TYPE_NORMAL = 1,
    STAGE_TYPE_ELITE = 2,
    STAGE_TYPE_SPECIAL = 3,
    STAGE_TYPE_TRIAL = 4,
};

//load status
enum LOADED_STATUS_ {
    LOADED_USER_INFO = 0x1,
    LOADED_HERO_INFO = 0x2,
    LOADED_SOLDIER_INFO = 0x4,
    LOADED_ITEM_INFO = 0x8,
    LOADED_FRIEND_INFO = 0x10,
    LOADED_FRIENDREQUEST_INFO = 0x20,
    LOADED_MAIL = 0x40,
    LOADED_MISSION = 0x80,
    LOADED_STAGERECORD = 0X100,
    LOADED_ACT_RECORD = 0x200,
    LOADED_TEAM = 0x400,
    LOADED_BUILDINGS = 0x0800,
    LOADED_GEARS = 0x1000,
    LOADED_HONOR_EXC = 0x2000,
};

//item type
enum ITEM_TYPE_ {
    ITEM_TYPE_HERO = 0,
    ITEM_TYPE_SOLDIER =  1,
    ITEM_TYPE_RESOURCE = 2, //升级技能所需的材料
    ITEM_TYPE_CHIP = 3, //TODO 这个碎片指的是 魂魄 暂时不改它
    ITEM_TYPE_GOLD = 4,
    ITEM_TYPE_DIAMOND = 5,
    ITEM_TYPE_HONOR = 6,
    ITEM_TYPE_GEAR = 7,
    ITEM_TYPE_GEAR_CHIP = 8,
    ITEM_TYPE_PROP = 9,
    ITEM_TYPE_WOOD = 10,
    ITEM_TYPE_STONE = 11,
};

//cmd_code
enum CMD_CODE_ {
    CMD_REGISTER = 0,
    CMD_LOGIN = 1,
    CMD_AUTHORIZE = 2,
    CMD_CHECKNICK = 3,
    CMD_CREATEUSER = 4,
    CMD_LOADALL = 5,
    CMD_LOADUSER = 6,
    CMD_LOADHERO = 7,
    CMD_LOADSOLDIER = 8,
    CMD_LOADITEM = 9,
    CMD_CHANGEHERO = 10,
    CMD_CHANGESOLDIER = 11,
    CMD_LOADASSISTANT = 12,
    CMD_BATTLESTART = 13,
    CMD_BATTLEEND = 14,
    CMD_FRIENDREQUEST = 15,
    CMD_SELLHERO = 16,
    CMD_SELLSOLDIER = 17,
    CMD_LOCKHERO = 18,
    CMD_LOCKSOLDIER = 19,
    CMD_HEROEXPUP = 20,
    CMD_SOLDIEREXPUP = 21,
    CMD_HEROENHANCE = 22,
    CMD_SOLDIERENHANCE = 23,
    CMD_SKILLLEVELUP = 24,
    CMD_SENDENERGY = 25,
    CMD_ACCEPTENERGY = 26,
    CMD_LOADFRIEND = 27,
    CMD_LOADFRIENDREQUEST = 28,
    CMD_ADDFRIEND = 29,
    CMD_REFUSEFRIENDREQ = 30,
    CMD_DELETEFRIEND = 31,
    CMD_SOULMERGE = 32,
    CMD_LOADUSERBYID = 33,
    CMD_LOADMAIL = 34,
    CMD_OPENMAIL = 35,
    CMD_DELETEMAIL = 36,
    CMD_SENDMAIL = 37,
    CMD_ACCEPTMAILITEM = 38,
    CMD_GETNEWMAILNUM = 39,
    CMD_LOADMISSION = 40,
    CMD_GETCMNUM = 41,
    CMD_GETMISSIONREWARD = 42,
    CMD_BUYENERGY = 43,
    CMD_EXPANDHERO = 44,
    CMD_EXPANDSOLDIER = 45,
    CMD_EXPANDFRIEND = 46,
    CMD_SUMMON = 47,
    CMD_LOADNEWS = 48,
    CMD_PAYMENTVERIFY = 49,
    CMD_PAYMENTVERIFYRECEIVED = 50,
    CMD_ADMINALTERUSER = 51,
    CMD_ADMINADDHERO = 52,
    CMD_ADMINALTERHERO = 53,
    CMD_ADMINADDSOLDIER = 54,
    CMD_ADMINADDITEM = 55,
    CMD_LOG = 56,
    CMD_GUIDE = 57,
    CMD_ALTERUSERNAME = 58,
    CMD_PAYMENTVERIFYCOMPLETE = 59,
    CMD_PAYMENT_ADD_DIAMOND_REQUEST = 60,
    CMD_PAYMENT_ADD_DIAMOND_RESPONSE = 61,
    CMD_PAYMENTTEST = 62,
    CMD_SESSION_VERIFY = 63,
    CMD_NEWPAYMENTVERIFY = 64,
    CMD_PBAK = 65,
    CMD_LOADACT = 66,
    CMD_ACT = 67,
    CMD_LOADTEAM = 68,
    CMD_SAVETEAM = 69,
    CMD_CHANGETEAM = 70,
    CMD_CHANGENICK = 71,
    CMD_FORCEHEU = 72,
    CMD_FORCESEU = 73,
    CMD_ADDFAKEUSER = 74,
    CMD_DEBUG = 75,
    CMD_INSTANT_BATTLE = 76,
    CMD_RESET_ELITE_STAGE = 77,
    CMD_INSTANT_TRIAL = 78,
    CMD_TRIAL_PROGRESS = 79,
    CMD_GET_TRIAL_RANK = 80,
    CMD_TRIAL_RESET = 81,
    CMD_TRIAL_RELIVE = 82,

    CMD_LOAD_BUILDINGS = 83,
    CMD_LOAD_BUILDINGS_BY_ID = 84,
    CMD_LOAD_PVP_RANKS = 85, //当前玩排行版
    CMD_LOAD_PVP_TARGETS = 86, //可攻打玩家列表（排行改变)
    CMD_LOAD_PVP_LOOT_TARGETS = 87, //可掠夺玩家列表
    CMD_PVP_BATTLE_START = 88,
    CMD_PVP_BATTLE_END = 89,
    CMD_ADD_BUILDING = 90,
    CMD_SAVE_BUILDINGS = 91,
    CMD_UPGRADE_BUILDING = 92,
    CMD_DESTROY_BUILDING = 93,
    CMD_SET_GEM = 94,
    CMD_LOAD_HONOR_EXC_STATUS = 95,
    CMD_HONOR_EXC = 96,
    CMD_LOAD_GEARS = 97,
    CMD_UPGRADE_GEAR = 98,
    CMD_SET_DEFEND_TEAM = 99,
    
};

//#define CMD_TYPE 83 

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

    "loadbuildings", //83
    "loadbuildingsbyid", //84
    "loadpvprank", //85
    "loadpvptargets", //86
    "loadloottargets", //87
    "pvpbattlestart", //88
    "pvpbattleend", //89
    "addbuilding", //90
    "savebuildings", //91
    "upgradebuilding", //92
    "destroybuilding", //93
    "setgem", //94
    "loadhonorexcstate", //95
    "honorexc", //96
    "loadgears", //97
    "upgradegear", //98
    "setdefendteam" //99


    "", //END 必须以空字符串结尾
};

//error
enum ERR_CODE_ {
    ERROR_SYSTEM_CRITICAL_ERROR =1,
    ERROR_USERNAME_HAS_BEEN_USED =2,
    ERROR_USER_OR_PASSWD_WRONG =3,
    ERROR_EQID_WRONG =4,
    ERROR_USER_DONOT_EXIST =5,
    ERROR_AUTHORIZE_CODE_WRONG =6,
    ERROR_AUTHORIZE_FAIL =7,
    ERROR_HERO_NOT_FOUND =8,
    ERROR_OWNER_DISMATCH =9,
    ERROR_NICKNAME_USED =10,
    ERROR_USER_ALREADY_REGISTERED =11,
    ERROR_CREATE_USER_FAIL =12,
    ERROR_SOLDIER_NOT_FOUND =13,
    ERROR_SOLDIER_HAS_BEEN_SET =14,
    ERROR_SOLDIER_THE_SAME_MID =15,
    ERROR_LEADERSHIP_NOT_ENOUGH =16,
    ERROR_STAGE_NOT_EXIST =17,
    ERROR_ENERGY_NOT_ENOUGH =18,
    ERROR_STAGE_LOCKED =19,
    ERROR_BATTLE_NOT_INITED =20,
    ERROR_ALREADY_BEEN_FRIENDS =21,
    ERROR_TARGET_USER_NOT_FOUND =22,
    ERROR_ALREADY_HAS_FRIEND_REQUEST =23,
    ERROR_LEVEL_NOT_ENOUGH =24,
    ERROR_ASSISTANT_NOT_EXIST =25,
    ERROR_BAG_IS_FULL =26,
    ERROR_HERO_IS_IN_ARMY =27,
    ERROR_SOLDIER_IS_IN_ARMY =28,
    ERROR_HERO_IS_LOCKED =29,
    ERROR_SOLDIER_IS_LOCKED =30,
    ERROR_CONFIG_ERROR =31,
    ERROR_GOLD_NOT_ENOUGH =32,
    ERROR_CANNOT_PAY_COST =33,
    ERROR_NOT_FRIEND =34,
    ERROR_CANNOT_SEND_ENERGY_TODAY =35,
    ERROR_HAVE_SENT_ENERGY =36,
    ERROR_NO_ENERGY_TO_ACCEPT =37,
    ERROR_CANNOT_ACCEPT_ENERY_TODAY =38,
    ERROR_NO_NEED_TO_ACCEPT_ENERGY =39,
    ERROR_SELF_NO_MORE_FRIEND =40,
    ERROR_TARGET_NO_MORE_FRIEND =41,
    ERROR_NO_SUCH_REQUEST =42,
    ERROR_CANNOT_FIND_SOUL_CONFIG =43,
    ERROR_POSITION_ERROR =44,
    ERROR_CANNOT_FIND_MAIL =45,
    ERROR_MAIL_TOO_LONG =46,
    ERROR_NOT_SYSTEM_MAIL =47,
    ERROR_NOT_ACCEPTABLE =48,
    ERROR_CANNOT_FIND_MISSION =49,
    ERROR_MISSION_NOT_COMPLETE =50,
    ERROR_NO_MORE_BUY_ENERGY =51,
    ERROR_GOT_LIMIT =52,
    ERROR_UID_CONFILICT =53,
    ERROR_NO_NEED_TO_SEND_REQ =54,
    ERROR_GUIDE_RECORD_WRONG =55,
    ERROR_PAYMENT_VERIFY_FAIL =56,
    ERROR_PAYMENT_ORDER_INFO_NOT_MATCH =57,
    ERROR_PAYMENT_WRONG_TIME =58,
    ERROR_PAYMENT_TRADE_COMPLETE =59,
    ERROR_PAYMENT_INFO_ERROR =60,
    ERROR_TIME_NOT_FIT =61,
    ERROR_SESSIONID_ERROR =62,
    ERROR_PAYMENT_NOT_RECEIVED_YET =63,
    ERROR_ACT_NOT_AVAIABLE =64,
    ERROR_TEAM_CANNOT_BE_FOUND =65,
    ERROR_STAGE_NOT_PASSED =66,
    ERROR_TRIAL_RESET_COUNT_ERROR =67,
    ERROR_TRIAL_RESET_NO_RECORD =68,
    ERROR_TRIAL_INSTANTING =69,
    ERROR_TRIAL_NO_INSTANT =70,
    ERROR_TRIAL_SAVE_STAGE_RECORD =71,
    ERROR_TRIAL_RELIVE_STAGE_ERROR =72,
    ERROR_TRIAL_RELIVE_DIAMOND_NOT_ENOUGH = 73,

    ERROR_BUILDING_NOT_FOUND = 74,
    ERROR_TOWER_LEVEL_NOT_ENOUGH = 75,
    ERROR_BUILDING_POSITION_ERROR=76,
    ERROR_BUILDING_ITEM_NOT_ENOUGH = 77,
    ERROR_BUILDING_ID_ERROR = 78,
    ERROR_BUILDING_POS_ERROR = 79,
    ERROR_BUILDING_REACH_MAX_LEVEL = 80,
    ERROR_SAVE_BUILDING_ID_ERROR = 81,
    ERROR_SAVE_BUILDING_POS_ERROR = 82,
    ERROR_BUILDING_GEM_TYPE_ERROR = 83,
    ERROR_DIAMOND_NOT_ENOUGH = 84,
    ERROR_HONOR_EXC_REACH_LIMIT = 85,
    ERROR_HONOR_NOT_ENOUGH = 86,
    ERROR_GEAR_NOT_ENOUGH = 87,
    ERROR_GEAR_REACH_MAX_LEVEL = 88,
    ERROR_PVP_UNDER_ATTACK = 89,
    ERROR_PVP_NOT_ATTACK_TARGET = 90,
    ERROR_PVP_RANK_IS_EMPTY = 91,
    ERROR_PVP_BATTLE_TYPE_ERROR = 92,
    ERROR_PVP_ATTACK_DAILI_LIMIT = 93,
    ERROR_TOWER_NOT_FOUND = 94,

};

enum ACT__ {
    ACT_1 = 1, //首充三倍
    ACT_2 = 2, //连登奖励
    ACT_3 = 3, //冲级活动
};

enum BUILDING_TYPE_ {
    BUILDING_TOWER = 1,
    BUILDING_RAILINGS = 2,
    BUILDING_WALL = 3,
    BUILDING_ARROW_TOWER = 4,
    BUILDING_TURRET = 5,
    BUILDING_MAGIC_TOWER = 6,
};


#endif //hero_const_h

