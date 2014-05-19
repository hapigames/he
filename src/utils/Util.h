//
//  Util.h
//  hero
//
//  Created by xujianfeng on 13-8-7.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#ifndef hero_Util_h
#define hero_Util_h

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <map>
#include "../common/Const.h"
using namespace std;

extern string cmd_list[];

int max(int a,int b);
bool tokenize(const string &str,vector<string> &tokens, string &delims);
bool safeAtoi(const string &str,int &num);
bool safeAtoll(const string &str,long long &num);
bool numtokenize(const string &str,vector<int> &all, string &delims);
bool longtokenize(const string &str,vector<long long> &all,string &delims);
int getAuthorizeCode(uid_type uid);
int get_cmd_id(string &cmd);
string vectorLongToString(vector<long long> &v);
string vectorIntToString(vector<int> &v);
string itoa(int input);
bool checkString(string & str);
void base64_encode(unsigned char *in, const int in_len, unsigned char *out, int out_len);
#endif
