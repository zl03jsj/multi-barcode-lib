#include "code-128.h"
#include <stdio.h>

const char * CODE128_TABLE[107] = {
	"212222","222122","222221","121223","121322","131222",
	"122213","122312","132212","221213","221312","231212",
	"112232","122132","122231","113222","123122","123221",
	"223211","221132","221231","213212","223112","312131",
	"311222","321122","321221","312212","322112","322211",
	"212123","212321","232121","111323","131123","131321",
	"112313","132113","132311","211313","231113","231311",
	"112133","112331","132131","113123","113321","133121",
	"313121","211331","231131","213113","213311","213131",
	"311123","311321","331121","312113","312311","332111",
	"314111","221411","431111","111224","111422","121124",
	"121421","141122","141221","112214","112412","122114",
	"122411","142112","142211","241211","221114","413111",
	"241112","134111","111242","121142","121241","114212",
	"124112","124211","411212","421112","421211","212141",
	"214121","412121","111143","111341","131141","114113",
	"114311","411113","411311","113141","114131","311141",
	"411131","211412","211214","211232","2331112" 
};

int PrintCODE128Code(char * codeBits,const char * enCodeStr,CODE128_TYPE code128Type=CODE128_TYPE_AUTO,
	bool isUseEAN128=false,bool isUseCheckCode=true)
{
	int enCodeStrLen = strlen(enCodeStr);
	if ( 0==enCodeStrLen || enCodeStrLen>MAX_Encode128Str_LEN )
	{
		return CODE128_Error_STRLEN;
	}

	int enCodeStrPos=0,codeBitsPos=0;
	CODE128_TYPE curCode128Type = (CODE128_TYPE)0;
	CODE128_ERROR retErr = CODE128_Error_Success;
	DWORD retDWORD = 0;
	int checkCode = 0, weight=1;
	switch (code128Type)
	{
	case CODE128_TYPE_A:
		if ( CANUSE_CODE128TYPE_A(enCodeStr[0]) )
		{
			codeBitsPos += StartCode128CodeType(codeBits,CODE128_TYPE_A,
				checkCode,
				&retErr);
			retDWORD = PrintCODE128Code_A(codeBits+codeBitsPos, 
				enCodeStr+enCodeStrPos,
				&retErr,
				weight,
				checkCode,
				true);
			curCode128Type = CODE128_TYPE_A;
			codeBitsPos  += LOWORD(retDWORD);
			enCodeStrPos += HIWORD(retDWORD);
		}
		else
			return CODE128_Error_InputData;
		break;
	case CODE128_TYPE_B:
		if ( CANUSE_CODE128TYPE_B(enCodeStr[0]) )
		{
			codeBitsPos += StartCode128CodeType(codeBits,
				CODE128_TYPE_A,
				checkCode,
				&retErr);
			retDWORD = PrintCODE128Code_B(codeBits+codeBitsPos, 
				enCodeStr+enCodeStrPos,
				&retErr,
				weight,
				checkCode,
				true);
			curCode128Type = CODE128_TYPE_B;
			codeBitsPos  += LOWORD(retDWORD);
			enCodeStrPos += HIWORD(retDWORD);
		}
		else
			return CODE128_Error_InputData;
		break;
	case CODE128_TYPE_C:
		if ( CANUSE_CODE128TYPE_C(enCodeStr) )
		{
			codeBitsPos += StartCode128CodeType(codeBits,CODE128_TYPE_A,
				checkCode,
				&retErr);
			retDWORD = PrintCODE128Code_C(codeBits+codeBitsPos, 
				enCodeStr+enCodeStrPos,
				&retErr,
				weight,
				checkCode,
				true,
				isUseEAN128);
			curCode128Type = CODE128_TYPE_C;
			codeBitsPos  += LOWORD(retDWORD);
			enCodeStrPos += HIWORD(retDWORD);
		}
		else
			return CODE128_Error_InputData;
		break;
	case CODE128_TYPE_AUTO:
		{
			// 打印起始符
			if ( CANUSE_CODE128TYPE_C(enCodeStr+enCodeStrPos) )
			{
				codeBitsPos += StartCode128CodeType(codeBits,CODE128_TYPE_C,
					checkCode,
					&retErr);
				retDWORD = PrintCODE128Code_C(codeBits+codeBitsPos, 
					enCodeStr+enCodeStrPos,
					&retErr,
					weight,
					checkCode,
					false,
					isUseEAN128);
				curCode128Type = CODE128_TYPE_C;
			}
			else if ( CANUSE_CODE128TYPE_B(enCodeStr[enCodeStrPos]) )
			{
				codeBitsPos += StartCode128CodeType(codeBits,CODE128_TYPE_B,
					checkCode,
					&retErr);
				retDWORD = PrintCODE128Code_B(codeBits+codeBitsPos, 
					enCodeStr+enCodeStrPos,
					&retErr,
					weight,
					checkCode,
					false);
				curCode128Type = CODE128_TYPE_B;
			}
			else if ( CANUSE_CODE128TYPE_A(enCodeStr[enCodeStrPos]) )
			{
				codeBitsPos += StartCode128CodeType(codeBits,CODE128_TYPE_A,
					checkCode,
					&retErr);
				retDWORD = PrintCODE128Code_A(codeBits+codeBitsPos, 
					enCodeStr+enCodeStrPos,
					&retErr,
					weight,
					checkCode,
					false);
				curCode128Type = CODE128_TYPE_A;
			}
			codeBitsPos  += LOWORD(retDWORD);
			enCodeStrPos += HIWORD(retDWORD);

			while ( enCodeStrPos<enCodeStrLen )
			{
				if (codeBitsPos >= MAX_Code128Bits_LEN)
				{
					return CODE128_Error_STRLEN;
				}
				
				if ( curCode128Type!=CODE128_TYPE_C && 
					 CANUSE_CODE128TYPE_C(enCodeStr+enCodeStrPos) )
				{
					codeBitsPos += EnterCode128CodeType(codeBits+codeBitsPos,
						curCode128Type,
						CODE128_TYPE_C,
						weight,
						checkCode
						,&retErr);
					retDWORD = PrintCODE128Code_C(codeBits+codeBitsPos,
						enCodeStr+enCodeStrPos,
						&retErr,
						weight,
						checkCode,
						false,
						isUseEAN128);
					curCode128Type = CODE128_TYPE_C;
				}
				else if ( curCode128Type!=CODE128_TYPE_B && 
						  CANUSE_CODE128TYPE_B(enCodeStr[enCodeStrPos]) )
				{
					codeBitsPos += EnterCode128CodeType(codeBits+codeBitsPos,
						curCode128Type,
						CODE128_TYPE_B,
						weight,
						checkCode,
						&retErr);
					retDWORD = PrintCODE128Code_B(codeBits+codeBitsPos, 
						enCodeStr+enCodeStrPos,
						&retErr,
						weight,
						checkCode,
						false);
					curCode128Type = CODE128_TYPE_B;
				}
				else if ( curCode128Type!=CODE128_TYPE_A && 
						  CANUSE_CODE128TYPE_A(enCodeStr[enCodeStrPos]) )
				{
					codeBitsPos += EnterCode128CodeType(codeBits+codeBitsPos,
						curCode128Type,
						CODE128_TYPE_A,
						weight,
						checkCode,
						&retErr);
					retDWORD = PrintCODE128Code_A(codeBits+codeBitsPos, 
						enCodeStr+enCodeStrPos,
						&retErr,
						weight,
						checkCode,
						false);
					curCode128Type = CODE128_TYPE_A;
				}
				else
				{
					return CODE128_Error_InputData;
				}
				
				codeBitsPos  += LOWORD(retDWORD);
				enCodeStrPos += HIWORD(retDWORD);
			}
		}
		break;
	default:
			return CODE128_Error_InvalidEncodeType;
	}
	
	if( CODE128_Error_Success==retErr)
	{
		if ( isUseCheckCode )
		{
			checkCode = checkCode%103;
			codeBitsPos += sprintf(codeBits+codeBitsPos,CODE128_TABLE[checkCode]);
		}
		sprintf(codeBits+codeBitsPos, CODE128_TABLE[106]);
	}

	return retErr;
}

DWORD PrintCODE128Code_A(char * codeBits,const char * enCodeStr,CODE128_ERROR * code128_Err,
	int &weight,int &checkCode,bool isFix)
{
	int i=0,codeBitsPos=0;	// 已编码的字符长度,产生的编码的长度
	int enCodeStrLen = strlen(enCodeStr);
	bool isNeedCheck = !isFix; //经过检查,与numberPos相连的数字小于4,不能用C类型表示
	int tmp = 0;

	for (i=0; i<enCodeStrLen; i++)
	{
		if	( CANUSE_CODE128TYPE_A(enCodeStr[i]) )	//字符能够用A类型表示
		{
			if ( IS_NOMARL_CHAR(enCodeStr[i]) )		//字符为普通字符
			{
				if ( IS_NUMBER(enCodeStr[i]) )		//字符为数字
				{
					if ( isNeedCheck )
					{
						if ( CANUSE_CODE128TYPE_C(enCodeStr+i) )
							break;
						else
							isNeedCheck = false;
					}
				}
				else
				{
					isNeedCheck = !isFix;
				}
				tmp = enCodeStr[i]-32;
				codeBitsPos += sprintf(codeBits+codeBitsPos, CODE128_TABLE[tmp]);
				checkCode += tmp*weight++;
			}
			else
			{
				tmp = enCodeStr[i]+64;
				codeBitsPos += sprintf(codeBits+codeBitsPos, CODE128_TABLE[tmp]);
				checkCode += tmp*weight++;

			}
		}
		else
		{
			*code128_Err = isFix?CODE128_Error_InputData:CODE128_Error_Success;
			break;
		}
	}
	return MAKELONG(codeBitsPos,i);
}

DWORD PrintCODE128Code_B(char * codeBits,const char * enCodeStr,CODE128_ERROR * code128_Err,
	int &weight,int &checkCode,bool isFix)
{
	int i=0,codeBitsPos=0;	// 已编码的字符长度,产生的编码的长度
	int enCodeStrLen = strlen(enCodeStr);
	bool isNeedCheck = !isFix; //经过检查,与numberPos相连的数字小于4,不能用C类型表示
	int tmp=0;

	for (i=0; i<enCodeStrLen; i++)
	{
		if	( CANUSE_CODE128TYPE_B(enCodeStr[i]) )	//字符能够用A类型表示
		{
			if ( IS_NUMBER(enCodeStr[i]) )		//字符为数字
			{
				if ( isNeedCheck )
				{
					if ( CANUSE_CODE128TYPE_C(enCodeStr+i) )
						break;
					else
						isNeedCheck = false;
				}
			}
			else
				isNeedCheck = !isFix;
			tmp = enCodeStr[i]-32;
			codeBitsPos += sprintf(codeBits+codeBitsPos, CODE128_TABLE[enCodeStr[i]-32]);
			checkCode += tmp*weight++;
		}
		else
		{
			*code128_Err = isFix?CODE128_Error_InputData:CODE128_Error_Success;
			break;
		}
	}

	return MAKELONG(codeBitsPos,i);
}

DWORD PrintCODE128Code_C(char * codeBits,const char * enCodeStr,CODE128_ERROR * code128_Err,
	int &weight,int &checkCode,bool isFix,bool isUseEAN128)
{
	int i=0,codeBitsPos=0;	// 已编码的字符长度,产生的编码的长度
	int enCodeStrLen = strlen(enCodeStr);
	int tmp = 0;
	if ( enCodeStrLen%2 &&  isFix )
	{
		*code128_Err = CODE128_Error_InputData;
		return 0;
	}
	if ( isUseEAN128 )
	{
		codeBitsPos += sprintf(codeBits+codeBitsPos, CODE128_TABLE[102]);
	}


	for (i=0; i<enCodeStrLen; i=i+2)
	{
		if ( IS_NUMBER(enCodeStr[i]) && IS_NUMBER(enCodeStr[i+1]) )
		{
			tmp = (enCodeStr[i]-48)*10 + (enCodeStr[i+1]-48);
			codeBitsPos += sprintf(codeBits+codeBitsPos, CODE128_TABLE[tmp] );
			checkCode += tmp*weight++;

		}
		else
		{
			*code128_Err = isFix?CODE128_Error_InputData:CODE128_Error_Success;
			break;
		}
	}
	
	return MAKELONG(codeBitsPos,i);
}

DWORD StartCode128CodeType(char * codeBits,CODE128_TYPE code128Type,int &checkCode,
	CODE128_ERROR * retError)
{
	if ( NULL!=retError )
	{
		*retError = CODE128_Error_Success;
	}

	int len = 0;
	switch (code128Type)
	{
	case CODE128_TYPE_A:
		checkCode = 103;
		len = sprintf(codeBits, CODE128_TABLE[103]);
		break;
	case CODE128_TYPE_B:
		checkCode = 104;
		len = sprintf(codeBits, CODE128_TABLE[104]);
		break;
	case CODE128_TYPE_C:
		checkCode = 105;
		len = sprintf(codeBits, CODE128_TABLE[105]);
		break;
	default:
		if ( NULL!=retError )
		{
			*retError = CODE128_Error_InvalidEncodeType;
		}
	}
	return len;
}

DWORD EnterCode128CodeType(char * codeBits,CODE128_TYPE code128Type,CODE128_TYPE enterType,
	int &weight,int &checkCode,CODE128_ERROR * retError)
{
	if ( NULL!=retError )
	{
		*retError = CODE128_Error_Success;
	}
	if ( code128Type==enterType )
	{
		return 0;
	}
	
	int len = 0;
	switch(enterType)
	{
	case CODE128_TYPE_A:
		len = sprintf(codeBits, CODE128_TABLE[101]);
		checkCode += 101*weight++;
		break;
	case CODE128_TYPE_B:
		len = sprintf(codeBits, CODE128_TABLE[100]);
		checkCode += 100*weight++;
		break;
	case CODE128_TYPE_C:
		len = sprintf(codeBits, CODE128_TABLE[99]);
		checkCode += 99*weight++;
		break;
	default:
		if ( NULL!=retError )
		{
			*retError = CODE128_Error_InvalidEncodeType;
		}
	}
	
	return len;
}

void Code128DrawOneCode(HDC hdc,int l,int t,int w,int h,HPEN hPen,HBRUSH hBrush)
{
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);
	Rectangle(hdc,l,t,l+w,t+h);
}

namespace z_code_128
{
	HRESULT draw_image(HDC hdc,const char * enCodeStr, 
		int &left,int &top,int code_x,int code_y,CODE128_TYPE code128Type,bool isUseEAN128)
	{
		if ( 32<strlen(enCodeStr) )
		{
			return CODE128_Error_STRLEN;
		}
		
		char codeBits[MAX_Code128Bits_LEN] = {0};// 偶数为b,奇数为s
		HRESULT ret = PrintCODE128Code(codeBits, enCodeStr,code128Type,isUseEAN128) ;
		if ( ret!=CODE128_Error_Success )
		{
			return ret;
		}

		if ( NULL==hdc )
		{
			left = (11*(strlen(codeBits)-7)/6 + 13)*code_x;
			top = code_y+CODE128_FONT_SIZE;
			return CODE128_Error_Success;
		}

		// 开始绘制图像
		int codeBitsLen = strlen(codeBits);
		int tmp = 0;
		int l, t;
		l = left;
		t = top;
		HPEN hBlackPen= (HPEN)GetStockObject(BLACK_PEN);
		HPEN hWhitePen= (HPEN)GetStockObject(WHITE_PEN);
		HBRUSH hBlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		HBRUSH hWhiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);

		for (int i=0; i<codeBitsLen; i++)
		{
			tmp = codeBits[i]-48;
			while ( tmp>0)
			{
				if ( i%2 )//白色
				{
					Code128DrawOneCode(hdc,l,t,code_x,code_y,hWhitePen,hWhiteBrush);				
				}
				else//黑色
				{
					Code128DrawOneCode(hdc,l,t,code_x,code_y,hBlackPen,hBlackBrush);				

				}
				l += code_x;
				tmp--;
			}
		}
		LOGFONT logFont = {
			CODE128_FONT_SIZE,
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
			l, top+code_y+CODE128_FONT_SIZE};
		
		oldHFont = (HFONT)SelectObject(hdc, hFont);
		
		DrawText(hdc,enCodeStr,strlen(enCodeStr),&rt,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		SelectObject(hdc, oldHFont);
		DeleteObject(hFont);

		return CODE128_Error_Success;	
	}
}

