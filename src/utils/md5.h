//
//  md5.h
//  MD5
//
//  Created by xujianfeng on 13-12-17.
//  Copyright (c) 2013年 feng. All rights reserved.
//

#ifndef MD5_md5_h
#define MD5_md5_h

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
using namespace std;

typedef unsigned char * POINTER;
typedef unsigned short int UINT2;
//typedef unsigned long int UINT4;
typedef unsigned int UINT4;
struct MD5_CTX
{
    UINT4 state[4];
    UINT4 count[2];
    unsigned char buffer[64];
};
string MDString(const char *);
void MD5Init(MD5_CTX *);
void MD5Update(MD5_CTX *,unsigned char *,unsigned int);
void MD5Final (unsigned char[16],MD5_CTX *);

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21


#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#ifdef _MSC_VER
#pragma intrinsic(_rotl)
#define ROTATE_LEFT(x, n) _rotl((x), (n))
#else
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#endif

#define FF(a, b, c, d, x, s, ac) { \
(a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
(a) = ROTATE_LEFT ((a), (s)); \
(a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
(a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
(a) = ROTATE_LEFT ((a), (s)); \
(a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
(a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
(a) = ROTATE_LEFT ((a), (s)); \
(a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
(a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
(a) = ROTATE_LEFT ((a), (s)); \
(a) += (b); \
}

#endif
