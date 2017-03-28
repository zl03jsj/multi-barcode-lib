#include "code-39.h"
#include <stdio.h>
static int FindChar(const char* tb, int tbSize,char c)
{
	for (int i=0; i<tbSize; i++)
	{
		if ( tb[i]==c )
		{
			return i;
		}
	}
	return -1;
}

int PrintCODE39Code(char * codeBits,const char * enCodeStr)
{
	const char * CODE39_NUMBERTABLE[10] = 
	{
		"101001101101","110100101011","101100101011","110110010101","101001101011",
		"110100110101","101100110101","101001011011","110100101101","101100101101"
	};
	const char * CODE39_CHARTABLE[26] =
	{
		"110101001011","101101001011","110110100101","101011001011","110101100101",
		"101101100101","101010011011","110101001101","101101001101","101011001101",
		"110101010011","101101010011","110110101001","101011010011","110101101001",
		"101101101001","101010110011","110101011001","101101011001","101011011001",
		"110010101011","100110101011","110011010101","100101101011","110010110101",
		"100110110101"
	};
	const char * CODE39_SIGNTABLE[8] =			
	{
		"100101001001","100101011011","100101101101","100100101001","101001001001",
		"100100100101","110010101101","100110101101"
	};
	const char SIGNS[8] = {'+','-','*','/','%','$','.',' '};
	int codeStrLen = strlen(enCodeStr);
	int codeBitsPos = 0;
	char c;
	int signIndex = -1;

	if ( codeStrLen>MAX_Encode39Str_LEN )
	{
		return CODE39_Error_STRLEN;
	}
	codeBitsPos = sprintf(codeBits, CODE39_SIGNTABLE[2]); // 以*为起始符号
	for (int i=0; i<codeStrLen; i++ )
	{
		c = enCodeStr[i];
		if ( 47<c && 58>c )
		{
			codeBitsPos += sprintf(codeBits+codeBitsPos,CODE39_NUMBERTABLE[c-48]);
		}
		else if ( 64<c && 91>c)
		{
			codeBitsPos += sprintf(codeBits+codeBitsPos,CODE39_CHARTABLE[c-65]);
		}
		else if ( -1!=(signIndex=FindChar(SIGNS,8,c)) )
		{
			codeBitsPos += sprintf(codeBits+codeBitsPos,CODE39_SIGNTABLE[signIndex]);
		}
		else
		{
			return CODE39_Error_InputData;
		}
	}
	codeBitsPos += sprintf(codeBits+codeBitsPos,CODE39_SIGNTABLE[2]);
	return CODE39_Error_Success;
}

namespace z_code_39
{	
	void Code39DrawOneCode(HDC hdc,int l,int t,int w,int h,HPEN hPen,HBRUSH hBrush);
	
	HRESULT draw_image(HDC hdc,const char * enCodeStr,int &left, int &top,
		int code_x,int code_y)
	{
		char codeBits[MAX_Code39Bits_LEN] = {0};
		HRESULT ret = PrintCODE39Code(codeBits,enCodeStr);

		if ( ret!=CODE39_Error_Success )
		{
			return ret;
		}
		int codeBitsLen = strlen(codeBits);
		if ( NULL==hdc )
		{
			left = code_x*codeBitsLen;
			top	 = code_y + CODE39_FONT_SIZE;
			return CODE39_Error_Success;
		}

		HPEN hBlackPen= (HPEN)GetStockObject(BLACK_PEN);
		HPEN hWhitePen= (HPEN)GetStockObject(WHITE_PEN);
		HBRUSH hBlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		HBRUSH hWhiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
		int l,t;
		l = left; 
		t = top;

		for (int i=0; i<codeBitsLen; i++)
		{
			if ( 48==codeBits[i])
			{
				Code39DrawOneCode(hdc,l,t,code_x,code_y,hWhitePen,hWhiteBrush);
			}
			else //if ( 49==codeBits[i])
			{
				Code39DrawOneCode(hdc,l,t,code_x,code_y,hBlackPen,hBlackBrush);
			}
			l += code_x;
		}

		LOGFONT logFont = {
			CODE39_FONT_SIZE,
			0,0,0, 
			FW_NORMAL,
			FALSE,
			FALSE,
			FALSE,
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			CLEARTYPE_QUALITY,
			FF_MODERN	
		};
		
		sprintf(logFont.lfFaceName,"Comic Sans MS");
		SetBkMode(hdc,TRANSPARENT);
		
		HFONT hFont = CreateFontIndirect(&logFont);
		HFONT oldHFont = NULL;
		RECT rt = {left, top+code_y, 
			l, top+code_y+CODE39_FONT_SIZE};
		
		oldHFont = (HFONT)SelectObject(hdc, hFont);
		
		DrawText(hdc,enCodeStr,strlen(enCodeStr),&rt,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		SelectObject(hdc, oldHFont);
		DeleteObject(hFont);
		return ret;
	}

	void Code39DrawOneCode(HDC hdc,int l,int t,int w,int h,HPEN hPen,HBRUSH hBrush)
	{
		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);
		Rectangle(hdc,l,t,l+w,t+h);
	}
}
