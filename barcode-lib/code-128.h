#ifndef _code_128__
#define _code_128__

#include "barcode-lib.h"
using namespace z_code_128;
// Code128编码说明
// A 类:
// 0-63  表示ASC码 32-95的字符 
// 64-95 表示ASC码  0-31的字符

// B 类:
// 0-95	 表示ASC码 32-127的字符

// C 类:
// 0-99  表示 00-99 的数字
// 其中c类如果是EAN128那么在 StartC|FUC1 如果是Code128 只有StartC
#define IS_NOMARL_CHAR(c)		( (int)(c)>31 && (int)(c)<96 )	// 
#define IS_NUMBER(c)			( (int)(c)>47 && (int)(c)<58 )	// 
#define IS_CONTROL_SIGN(c)		( (int)(c)>=0 && (int)(c)<32 )	//
#define CANUSE_CODE128TYPE_A(c)	( (int)(c)>=0 && (int)(c)<96 )
#define CANUSE_CODE128TYPE_B(c)	( (int)(c)>31 && (int)(c)<128 )
#define CANUSE_CODE128TYPE_C(c) ( IS_NUMBER((c)[0]) && IS_NUMBER((c)[1]) && IS_NUMBER((c)[2]) && IS_NUMBER((c)[3]) )
// 高字节返回codeBits增加的长度, 低字节返回enCodeStr中被编码的长度
DWORD PrintCODE128Code_A(char * codeBits,const char * enCodeStr,CODE128_ERROR * code128_Err,int &weight,int &checkCode,bool isFix);
DWORD PrintCODE128Code_B(char * codeBits,const char * enCodeStr,CODE128_ERROR * code128_Err,int &weight,int &checkCode,bool isFix);
DWORD PrintCODE128Code_C(char * codeBits,const char * enCodeStr,CODE128_ERROR * code128_Err,int &weight,int &checkCode,bool isFix,bool isUseEAN128);

DWORD StartCode128CodeType(char * codeBits,CODE128_TYPE code128Type,int &checkCode,CODE128_ERROR * retError=NULL);
DWORD EnterCode128CodeType(char * codeBits,CODE128_TYPE code128Type,CODE128_TYPE enterType,int &weight,int &checkCode,CODE128_ERROR * retError=NULL);

#endif // _code_128__
