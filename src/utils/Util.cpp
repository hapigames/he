//
//  Util.cpp
//  hero
//
//  Created by xujianfeng on 13-8-7.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#include "Util.h"

int max(int a,int b)
{
    if(a > b) return a;
    else return b;
}
int min(int a,int b)
{
    if(a > b) return b;
    else return a;
}
bool tokenize(const string &str,vector<string> & tokens,string &delims)
{
    tokens.clear();
    string::size_type last_pos = 0;
    string::size_type pos = 0;
    while((pos = str.find_first_of(delims,last_pos))!=str.npos){
        tokens.push_back(str.substr(last_pos,pos-last_pos));
        last_pos = pos + 1;
    }
    tokens.push_back(str.substr(last_pos));
    return true;
}
bool safeAtoi(const string &str, int &num)
{
    istringstream iss(str);
    return !(iss>>num).fail();
}
bool safeAtoll(const string &str,long long &num)
{
    istringstream iss(str);
    return !(iss>>num).fail();
}
bool numtokenize(const string &str,vector<int> &all,string &delims)
{
    vector<string> tokens;
    tokens.clear();
    string::size_type last_pos = 0;
    string::size_type pos = 0;
    while((pos = str.find_first_of(delims,last_pos))!=str.npos){
        tokens.push_back(str.substr(last_pos,pos-last_pos));
        last_pos = pos + 1;
    }
    tokens.push_back(str.substr(last_pos));
    int num = 0;
    for(int i=0;i<int (tokens.size());i++){
        if(safeAtoi(tokens[i], num)==false){
            return false;
        }
        all.push_back(num);
    }
    return true;
}
bool longtokenize(const string &str,vector<long long> &all,string &delims)
{
    vector<string> tokens;
    tokens.clear();
    string::size_type last_pos = 0;
    string::size_type pos = 0;
    while((pos = str.find_first_of(delims,last_pos))!=str.npos){
        tokens.push_back(str.substr(last_pos,pos-last_pos));
        last_pos = pos + 1;
    }
    tokens.push_back(str.substr(last_pos));
    long long num = 0;
    for(int i=0;i<int (tokens.size());i++){
        if(safeAtoll(tokens[i], num)==false){
            return false;
        }
        all.push_back(num);
    }
    return true;
}
int getAuthorizeCode(uid_type uid)
{
    int authorize_base[7] = {123,456,789,101,112,131,415};
    time_t t = time(NULL);
    struct tm * tb = localtime(&t);
    cout<<((uid + 20130805)%100000)/100 * authorize_base[tb->tm_wday]<<endl;
    return ((uid + 20130805)%100000)/100 * authorize_base[tb->tm_wday];
    
}
int get_cmd_id(string &cmd){
    for(int i = 0;i<CMD_TYPE;i++){
        if(cmd == cmd_list[i]){
            return i;
        }
    }
    return -1;
}
string vectorLongToString(vector<long long> &v)
{
    ostringstream ost;
    for(size_t i = 0;i<v.size();i++){
        ost<<v[i];
        if(i!=v.size()-1){
            ost<<",";
        }
    }
    return ost.str();
}
string vectorIntToString(vector<int> &v)
{
    ostringstream ost;
    for(size_t i = 0;i<v.size();i++){
        ost<<v[i];
        if(i!=v.size()-1){
            ost<<",";
        }
    }
    return ost.str();
}
string itoa(int input)
{
    ostringstream ost;
    ost<<input;
    return ost.str();
}
bool checkString(string &str)
{
    for(size_t i = 0;i<str.size();i++){
        if(str[i] == ',' || str[i] == '\'' || str[i] == '\"' || str[i] == ':' || str[i] == ';')
            return false;
    }
    return true;
}

static const char *codes ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
void base64_encode(unsigned char *in, const int in_len, unsigned char *out, int out_len)
{
	//int base64_len = 4 * ((in_len+2)/3);
	unsigned char *p = out;
	int times = in_len / 3;
	for(int i=0; i<times; ++i)
	{
		*p++ = codes[(in[0] >> 2) & 0x3f];
		*p++ = codes[((in[0] << 4) & 0x30) + ((in[1] >> 4) & 0xf)];
		*p++ = codes[((in[1] << 2) & 0x3c) + ((in[2] >> 6) & 0x3)];
		*p++ = codes[in[2] & 0x3f];
		in += 3;
	}
	if(times * 3 + 1 == in_len)
	{
		*p++ = codes[(in[0] >> 2) & 0x3f];
		*p++ = codes[((in[0] << 4) & 0x30) + ((in[1] >> 4) & 0xf)];
		*p++ = '=';
		*p++ = '=';
	}
	if(times * 3 + 2 == in_len)
	{
		*p++ = codes[(in[0] >> 2) & 0x3f];
		*p++ = codes[((in[0] << 4) & 0x30) + ((in[1] >> 4) & 0xf)];
		*p++ = codes[((in[1] << 2) & 0x3c)];
		*p++ = '=';
	}
	*p = 0;
}

int saveint(int data, FILE *fp) {
    return fwrite(&data, sizeof(data), 1, fp) == 1;
}

int loadint(int &data, FILE *fp) {
    return fread(&data, sizeof(data), 1, fp) == 1;
}

long long loadlonglong(long long &data, FILE *fp) {
    return fread(&data, sizeof(data), 1, fp) == 1;
}

int savelonglong(long long data, FILE* fp) {
    return fwrite(&data, sizeof(data), 1, fp) == 1;
}

int savestring(const char *str, int size, FILE *fp) {
    if (fwrite(&size, sizeof(size), 1, fp) == 1) {
        return (int)fwrite(str, sizeof(char), size, fp) == size;
    }   
    return 0;
}

int loadstring(string &str, FILE *fp) {
    int ret = 0;
    int s = 0;
    if (loadint(s, fp)) {
        char *buf = (char*)malloc(sizeof(char)*s);
        if (buf != NULL) {
            if ((int)fread(buf, sizeof(char), s, fp) == s) {
                str.assign(buf, s); 
                ret = s;
            }   
            free(buf);
        }   
    }   
    return ret;
}



