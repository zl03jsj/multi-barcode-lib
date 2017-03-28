#include "ean-code.h"
#include "barcode-lib.h"
#include <stdio.h>

int TextToNumbers(int * numbers, const char * enCodeStr)
{
	if ( 13!=strlen(enCodeStr) ) // ²»¹»13Î»
	{
		return z_ean_13::EANCode_Error_InputData;
	}
	
	for ( int i=0; i<13; i++ )
	{
		if ( enCodeStr[i]<48 || enCodeStr[i]>57) // ²»ÊÇÊý×Ö
		{
			return z_ean_13::EANCode_Error_InputData;
		}
		numbers[i] = enCodeStr[i]-48;
	}
	return z_ean_13::EANCode_Error_Success;
}

int ComputCheckCode(const int * numbers)
{
	int checkCode = 0;
	for (int i=0; i<12; i++)
	{
		if ( 1==i%2 )
			checkCode += 3*numbers[i];
		else
			checkCode += numbers[i];
	}
	
	checkCode = 10-(checkCode%10);
	checkCode = (10==checkCode)?0:checkCode;
	return checkCode;
}
int PrintEAN13Code(char * codeBits, const int * numbers)
{
	// ×ó±ßµÄ±àÂë,¸ù¾ÝµÚ1Î»Êý×Ö,È·¶¨ÆæÅ¼±àÂë·½Ê½
	const char * REG_CODE[10] = {
		"000000", "001011", "001101", "001110", "010011", 
		"011001", "011100", "010101", "010110", "011010" 
	} ;
	
	// Êý×Ö,±àÂë·½Ê½ËùÓ³ÉäµÄÂë×Ö
	const char * STR_CODE_TABLE[10][3] = {
		"0001101", "0100111", "1110010",
		"0011001", "0110011", "1100110",
		"0010011", "0011011", "1101100",
		"0111101", "0100001", "1000010",
		"0100011", "0011101", "1011100",
		"0110001", "0111001", "1001110",
		"0101111", "0000101", "1010000",
		"0111011", "0010001", "1000100",
		"0110111", "0001001", "1001000",
		"0001011", "0010111", "1110100", 
	};
	
	const char * regCode = REG_CODE[ numbers[0] ];
	int number;
	int codeType;
	
	if ( NULL==codeBits )
	{
		return 1;
	}
	sprintf(codeBits,"%s", EANCODE_SIDE_SIGN);
	
	for (int i=0; i<6; i++)
	{
		number =  numbers[i+1];
		codeType = regCode[i] - 48;
		sprintf(codeBits+7*i+3, "%s", STR_CODE_TABLE[number][codeType]);
	}
	
	sprintf(codeBits+7*i+3,"%s",EANCODE_MIDDLE_SIGN);
	
	codeType = 2;
	for (; i<12; i++)
	{	
		number = numbers[i+1];
		sprintf(codeBits+7*i+8,"%s",STR_CODE_TABLE[number][codeType]);
	}
	
	sprintf(codeBits+7*i+8,"%s",EANCODE_SIDE_SIGN);
	return 0;
}

namespace z_ean_13
{
	void EAN13DrawOneCode(HDC hdc,int l,int t,int w,int h,HPEN hPen,HBRUSH hBrush);
	HRESULT draw_image(HDC hdc,const char * enCodeStr,int &left,int &top,
		int code_x,int code_y)
	{
		if ( NULL==enCodeStr )
		{
			return EANCode_Error_InputData;
		}

		if( NULL==hdc )
		{
			left = 95*code_x + LEFT_MARGE;
			top  = code_y + LONG_HEIGHT;
			return EANCode_Error_Success;
		}
		HRESULT ret = EANCode_Error_Success;
		char eanCode[EANCODE_LENGTH] = {0};
		int numbers[13] = {0};
		RECT rt = {0};

		ret = TextToNumbers(numbers, enCodeStr);
		if ( EANCode_Error_Success==ret )
		{
			numbers[12] = ComputCheckCode(numbers);
			ret = PrintEAN13Code(eanCode, numbers);
		}

		if ( EANCode_Error_Success!=ret )
		{
			return ret;
		}

		HPEN hBlackPen= (HPEN)GetStockObject(BLACK_PEN);
		HPEN hWhitePen= (HPEN)GetStockObject(WHITE_PEN);
		HBRUSH hBlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		HBRUSH hWhiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);

		int l,t;
		l = left + LEFT_MARGE;
		t = top;

		// »­×ó±ßÆðÊ¼·û
		for ( int i=0; i<3; i++)
		{
			if ( eanCode[i]==48 )
				EAN13DrawOneCode(hdc,l,t,code_x,code_y+LONG_HEIGHT,hWhitePen,hWhiteBrush);
			else if ( eanCode[i]==49 )
				EAN13DrawOneCode(hdc,l,t,code_x,code_y+LONG_HEIGHT,hBlackPen,hBlackBrush);
			l += code_x;
		}

		// »­×ó±ßÂë×Ö·û
		for (; i<45; i++)
		{
			if ( eanCode[i]==48 )
				EAN13DrawOneCode(hdc,l,t,code_x,code_y,hWhitePen,hWhiteBrush);
			else if ( eanCode[i]==49 )
				EAN13DrawOneCode(hdc,l,t,code_x,code_y,hBlackPen,hBlackBrush);
			l += code_x;
		}

		// »­ÖÐ¼ä¼ä¸ô·û
		for (; i<50; i++)
		{
			if ( eanCode[i]==48 )
				EAN13DrawOneCode(hdc,l,t,code_x,code_y+LONG_HEIGHT,hWhitePen,hWhiteBrush);
			else if ( eanCode[i]==49 )
				EAN13DrawOneCode(hdc,l,t,code_x,code_y+LONG_HEIGHT,hBlackPen,hBlackBrush);
			l += code_x;	
		}

		// »­ÓÒ±ßÂë×Ö·û
		for (; i<92; i++)
		{		
			if ( eanCode[i]==48 )
				EAN13DrawOneCode(hdc,l,t,code_x,code_y,hWhitePen,hWhiteBrush);
			else if ( eanCode[i]==49 )
				EAN13DrawOneCode(hdc,l,t,code_x,code_y,hBlackPen,hBlackBrush);
			l += code_x;
		}
		
		// »­ÓÒ±ß½áÊø·û
		for (; i<95; i++)
		{
			if ( eanCode[i]==48 )
				EAN13DrawOneCode(hdc,l,t,code_x,code_y+LONG_HEIGHT,hWhitePen,hWhiteBrush);
			else if ( eanCode[i]==49 )
				EAN13DrawOneCode(hdc,l,t,code_x,code_y+LONG_HEIGHT,hBlackPen,hBlackBrush);
			l += code_x;
		}

		LOGFONT logFont = {
			EAN13_FONT_SIZE,
			0,0,0, 
			FW_NORMAL,
			FALSE,
			FALSE,
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			FF_MODERN	};

		sprintf(logFont.lfFaceName,"ËÎÌå");
		SetBkMode(hdc,TRANSPARENT);

		HFONT hFont = CreateFontIndirect(&logFont);
		HFONT oldHFont = NULL;

		oldHFont = (HFONT)SelectObject(hdc, hFont);

		l = left;
		t = top + code_y;
		TextOut(hdc,l,t,enCodeStr,1);
		l += 4*code_x+LEFT_MARGE;
		for(i=1; i<7; i++)
		{
			TextOut(hdc,l,t,&enCodeStr[i],1);
			l+= code_x*7;
		}
		l += 5*code_x;
		for(i=7; i<13; i++)
		{
			TextOut(hdc,l,t,&enCodeStr[i],1);
			l+= code_x*7;
		}
		SelectObject(hdc, oldHFont);
		DeleteObject(hFont);

		return 0;
	}
	
	void EAN13DrawOneCode(HDC hdc,int l,int t,int w,int h,HPEN hPen,HBRUSH hBrush)
	{
		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);
		Rectangle(hdc,l,t,l+w,t+h);
	}
	
// 	void EAN13DrawOneCode(HDC hdc,int l,int t,int w,int h,bool isLong,bool b)
// 	{
// 		// 		HPEN hBlackPen= (HPEN)GetStockObject(BLACK_PEN);
// 		// 		HBRUSH hBlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
// 		// 		HPEN hWhitePen= (HPEN)GetStockObject(WHITE_PEN);
// 		// 		HBRUSH hWhiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
// 		HPEN hPen = NULL;
// 		HBRUSH hBrush = NULL;
// 		
// 		if ( b )
// 		{
// 			hPen = (HPEN)GetStockObject(BLACK_PEN);
// 			hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
// 		}
// 		else
// 		{
// 			hPen = (HPEN)GetStockObject(WHITE_PEN);;
// 			hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
// 		}
// 		SelectObject(hdc, hPen);
// 		SelectObject(hdc, hBrush);
// 		
// 		int r,b;
// 		r = l + w - 1;
// 		b = t + h;
// 		if ( isLong )
// 		{
// 			b += LONG_HEIGHT;
// 		}
// 		
// 		Rectangle(hdc,l,t,r,b);
// 	}
}
