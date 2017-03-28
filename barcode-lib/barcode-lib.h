
// 文件名称:barcode-lib.h
// 版本信息:2011-4-13 10:11:59 By 88911562@qq.com
// 联系我们:88911562@qq.com
******************************************/

/******************************************	
// 使用说明
// pdf417伪代码_____________________________________________:
//***********************************************************
// int w=0,h=0,left,top;
// PDF417PARAM pdf417param = {0};
// HDC tmpHdc = NULL;
// pdf417param.options = PDF417_INVERT_BITMAP|PDF417_USE_ERROR_LEVEL;
// pdf417param.errorLevel = 2;
// pdf417param.outBits = NULL;
// pdf417param.lenBits = 0;
// pdf417param.error = 0;
// pdf417param.lenText = -1;
// pdf417param.text = "hello";
// pdf417param.yHeight = 3;
// pdf417param.aspectRatio = 0.5;
// // hdc传入null,指定每个码字宽和高的像素分别为1和3.
// // w,h返回pdf417图像的需要的宽度高度.
// if ( 0!=z_pdf417::PDF417DrawImg(NULL,&pdf417param,w,h,1,3) )
// {
// 	return ;
// }
// if (  0==w || 0==h )
// {
// 	return;
// }
// 	// ....
// 	// 创建位图,宽度为w,高度为h
// 	// ....

// if ( 0==z_pdf417::PDF417DrawImg(tmpHdc,&pdf417param,left,top,1,3) )
// {
// // 成功绘制了图像
// }
// pdf417伪代码_____________________________________________//

// ----------------------------------------------------------

// qrcode伪代码_____________________________________________:
//***********************************************************
// int w=0,h=0,left,top;
// HDC tmpHdc = NULL;
// QRCODEPARAM qrcodeparam = {0};
// qrcodeparam.nLevel = QR_LEVEL_M;
// qrcodeparam.nVersion = 3;
// qrcodeparam.bAutoExtent = true;
// qrcodeparam.nMaskingNo = -1;
// qrcodeparam.lpsSource = "hello";
// // hdc传入null,指定每个码字的高度宽度都为3
// // left和top返回qr_code图像需要的高度和宽度(w=h)
// if ( 0!=z_qrcode::QRCODEDrawImg(NULL,&qrcodeparam,w,h,3) )
// {
// return;
// }
// 	if ( 0==w || 0==h )
// 	{
// 		return;
// 	}
// // ....
// // 创建位图,宽度为w,高度为h
// // ....
// if ( 0!=z_qrcode::QRCODEDrawImg(tmpHdc,&qrcodeparam,w,h,3) )
// {
// //成功绘制了qr_code图像
// }
// qrcode伪代码_____________________________________________//

******************************************/


#ifndef  barcode_lib_h
#define  barcode_lib_h


#if defined(_WIN32_WINNT ) && (_WIN32_WINNT <0x0400)
	#undef _WIN32_WINNT
	#define _WIN32_WINNT  0x0500
#elif !defined(_WIN32_WINNT)
	#define _WIN32_WINNT  0x0500
#endif


#include <windows.h>
#pragma comment(lib,"gdi32.lib")

namespace z_pdf417
{
	#define PDF417_USE_ASPECT_RATIO			0	//- use aspectRatio to set the y/x dimension. Also uses yHeight
	#define PDF417_FIXED_RECTANGLE			1	//- make the barcode dimensions at least codeColumns by codeRows
	#define PDF417_FIXED_COLUMNS			2	//- make the barcode dimensions at least codeColumns
	#define PDF417_FIXED_ROWS				4	//- make the barcode dimensions at least codeRows
	#define PDF417_AUTO_ERROR_LEVEL			0	//- automatic error level depending on text size
	#define PDF417_USE_ERROR_LEVEL			16	// - the error level is errorLevel. The used errorLevel may be different
	#define PDF417_USE_RAW_CODEWORDS		64	//- use codewords instead of text
	#define PDF417_INVERT_BITMAP			128	//- invert the resulting bitmap	
	// pdf417错误信息
	enum PDF417_ERROR{
		PDF417_ERROR_SUCCESS		= 0,
		PDF417_ERROR_TEXT_TOO_BIG	= 1,
		PDF417_ERROR_INVALID_PARAMS = 2,
	};

	typedef struct _PDF417PARAM 
	{
		char *outBits;      // 图像数据
		int lenBits;        // outBits的长度
		int bitColumns;     // 图像的每1行的bit位数
		int codeRows;       // 图像的行数
		int codeColumns;    // 编码的列数
		int codewords[928]; // the code words may be input. Is always output
		int lenCodewords;   // 码字长度
		int errorLevel;     // 纠错等级0-8
		char *text;         // 编码文本
		int lenText;        // 编码文本的长度。传入-1,自动获取长度
		int options;        // 编码设置
		float aspectRatio;  // y/x 的比值
		float yHeight;      // the y/x dot ratio
		int error;          // 返回错误信息
	} PDF417PARAM, *PPDF417PARAM;
	// 返回错误信息,PDF417_ERROR_SUCCESS表示成功.
	// hdc传NULL时,left,top分别返回需要的位图宽度和高度
	HRESULT PDF417DrawImg(HDC hdc,PPDF417PARAM pPDF417Param,int &left,
		int &top,int code_x=1,int code_y=3);
}

namespace z_qrcode
{
	// 纠错等级
	#define QR_LEVEL_L	0
	#define QR_LEVEL_M	1
	#define QR_LEVEL_Q	2
	#define QR_LEVEL_H	3
	// qr_code图像边沿空白
	#define QR_MARGIN	0

	typedef struct _QRCODEPARAM
	{
		int		nLevel;			// 纠错等级
		int		nVersion;		// qrcode版本 0 - 40
		BOOL	bAutoExtent;	// 设置为 true
		int		nMaskingNo;		// 设置为 -1
		LPSTR	lpsSource;		// 编码的字符串
		//int		ncSource;	// 0
	}QRCODEPARAM,*PQRCODEPARAM;

	// 函数返回 0 表示成功.
	// hdc传NULL时,left,top分别返回需要的位图宽度和高度
	HRESULT QRCODEDrawImg(HDC hdc,PQRCODEPARAM pQrCodeeParam,
		int &left,int &top,int code_c=3);
}

namespace z_ean13
{
	#define LONG_HEIGHT			8*3	//原为8
	#define LEFT_MARGE			7*3	//原5
	#define EAN13_FONT_SIZE		12*3	//原为12	

	enum EAN13CODE_ERROR
	{
		EANCode_Error_Success	= 0,
		EANCode_Error_InputData	= 1,
	};

	// 函数返回EANCode_Error_Success表示成功
	// hdc传NULL时,left,top分别返回需要的位图宽度和高度 
	HRESULT NTKOEan13DrawImg(HDC hdc,const char * enCodeStr,int &left,
		int &top,int code_x=1,int code_y=30);
}

namespace z_code_128
{
	#define MAX_Encode128Str_LEN	32		// 最长32位字符串
	#define	MAX_Code128Bits_LEN	256		
	#define CODE128_FONT_SIZE	14*3			//原为14
	// 编码类型
	enum CODE128_TYPE
	{
		CODE128_TYPE_A	= 1,
		CODE128_TYPE_B	= 2,
		CODE128_TYPE_C	= 3,
		CODE128_TYPE_AUTO = 4,
	};

	enum CODE128_ERROR
	{
		CODE128_Error_Success	= 0,
		CODE128_Error_InputData	= 1,
		CODE128_Error_STRLEN	= 2,
		CODE128_Error_InvalidEncodeType = 4,
	};
	
	// 函数返回EANCode_Error_Success表示成功
	// hdc传NULL时,left,top分别返回需要的位图宽度和高度 
	HRESULT draw_image(HDC hdc,const char * enCodeStr, int &left,
		int &top,int code_x=1,int code_y=30,
		CODE128_TYPE code128Type=CODE128_TYPE_AUTO,
		bool isUseEAN128=true);
}

namespace z_code_39
{
	#define MAX_Encode39Str_LEN	24		// 最长32位字符串
	#define	MAX_Code39Bits_LEN	335		// 应该为 (MAX_Encode39Str_LEN+2)*13
	#define CODE39_FONT_SIZE	14*3		//原为14
	// 编码类型
	enum CODE39_ERROR
	{
		CODE39_Error_Success	= 0,
		CODE39_Error_InputData	= 1,
		CODE39_Error_STRLEN		= 2,
	};
	// hdc传NULL时,left,top分别返回需要的位图宽度和高度 
	HRESULT draw_image(HDC,const char * enCodeStr,int &left, int &top,
		int code_x=1,int code_y=30);

}

#endif // barcode_lib_h
