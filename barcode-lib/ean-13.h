#ifndef _ean_13__
#define _ean_13__

#include "barcode-lib.h"
using namespace z_ean_13;


#define		EANCODE_LENGTH			96		// 保存EANCode的char类型数组长度
#define		EANCODE_SIDE_SIGN		"101"	// 两边的警戒标志
#define		EANCODE_MIDDLE_SIGN		"01010"	// 中间的警戒标志

int ComputCheckCode(const int * inputNumber);
int TextToNumbers(int * numbers, const char * enCodeStr);
int PrintEAN13Code(char * codeBits, const int * numbers);


#endif // _ean_13__
