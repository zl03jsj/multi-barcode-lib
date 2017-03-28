//---(1)ここから-------------------------
//// QR_Encode.h : CQR_Encode クラス宣言およびインターフェイス定義
//// Date 2006/05/17	Ver. 1.12 [Class Ver.1.22] Psytec Inc.

//---(1)ここまでをコメントアウト-----------

//---(2)ここから-----------
#ifndef __QR_ENCODER__
#define __QR_ENCODER__
#include "barcode-lib.h"
using namespace z_qrcode;
//---(2)ここまでを追加-----------

// データモード
#define QR_MODE_NUMERAL		0
#define QR_MODE_ALPHABET	1
#define QR_MODE_8BIT		2
#define QR_MODE_KANJI		3

// バージョン(型番)グループ
#define QR_VRESION_S	0 // 1 〜 9
#define QR_VRESION_M	1 // 10 〜 26
#define QR_VRESION_L	2 // 27 〜 40

#define MAX_ALLCODEWORD	 3706 // 総コードワード数最大値
#define MAX_DATACODEWORD 2956 // データコードワード最大値(バージョン40-L)
#define MAX_CODEBLOCK	  153 // ブロックデータコードワード数最大値(ＲＳコードワードを含む)
#define MAX_MODULESIZE	  177 // 一辺モジュール数最大値
/////////////////////////////////////////////////////////////////////////////
typedef struct tagRS_BLOCKINFO
{
	int ncRSBlock;		// ＲＳブロック数
	int ncAllCodeWord;	// ブロック内コードワード数
	int ncDataCodeWord;	// データコードワード数(コードワード数 - ＲＳコードワード数)

} RS_BLOCKINFO, *LPRS_BLOCKINFO;


/////////////////////////////////////////////////////////////////////////////
// QRコードバージョン(型番)関連情報

typedef struct tagQR_VERSIONINFO
{
	int nVersionNo;	   // バージョン(型番)番号(1〜40)
	int ncAllCodeWord; // 総コードワード数

	// 以下配列添字は誤り訂正率(0 = L, 1 = M, 2 = Q, 3 = H) 
	int ncDataCodeWord[4];	// データコードワード数(総コードワード数 - ＲＳコードワード数)

	int ncAlignPoint;	// アライメントパターン座標数
	int nAlignPoint[6];	// アライメントパターン中心座標

	RS_BLOCKINFO RS_BlockInfo1[4]; // ＲＳブロック情報(1)
	RS_BLOCKINFO RS_BlockInfo2[4]; // ＲＳブロック情報(2)

} QR_VERSIONINFO, *LPQR_VERSIONINFO;


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode クラス

class CQR_Encode
{
// 構築/消滅
public:
	CQR_Encode();
	~CQR_Encode();

public:
	int m_nLevel;		// 誤り訂正レベル
	int m_nVersion;		// バージョン(型番)
	BOOL m_bAutoExtent;	// バージョン(型番)自動拡張指定フラグ
	int m_nMaskingNo;	// マスキングパターン番号

public:
	int m_nSymbleSize;
	BYTE m_byModuleData[MAX_MODULESIZE][MAX_MODULESIZE]; // [x][y]
	// bit5:機能モジュール（マスキング対象外）フラグ
	// bit4:機能モジュール描画データ
	// bit1:エンコードデータ
	// bit0:マスク後エンコード描画データ
	// 20hとの論理和により機能モジュール判定、11hとの論理和により描画（最終的にはBOOL値化）

private:
	int m_ncDataCodeWordBit; // データコードワードビット長
	BYTE m_byDataCodeWord[MAX_DATACODEWORD]; // 入力データエンコードエリア

	int m_ncDataBlock;
	BYTE m_byBlockMode[MAX_DATACODEWORD];
	int m_nBlockLength[MAX_DATACODEWORD];
	
	int m_ncAllCodeWord; // 総コードワード数(ＲＳ誤り訂正データを含む)
	BYTE m_byAllCodeWord[MAX_ALLCODEWORD]; // 総コードワード算出エリア
	BYTE m_byRSWork[MAX_CODEBLOCK];	// ＲＳコードワード算出ワーク
	//BOOL m_isEncoded;	// 添加的成员 标志是否已经编码成功
	// データエンコード関連ファンクション
public:
	BOOL EncodeData(int nLevel,int nVersion,BOOL bAutoExtent,int nMaskingNo,LPCSTR lpsSource,int ncSource = 0);
// 	// 增加的函数
// 	BOOL DrawImg(HDC hdc,int &left,int &top);
private:
	int GetEncodeVersion(int nVersion, LPCSTR lpsSource, int ncLength);
	BOOL EncodeSourceData(LPCSTR lpsSource, int ncLength, int nVerGroup);
	
	int GetBitLength(BYTE nMode, int ncData, int nVerGroup);
	
	int SetBitStream(int nIndex, WORD wData, int ncData);
	
	BOOL IsNumeralData(unsigned char c);
	BOOL IsAlphabetData(unsigned char c);
	BOOL IsKanjiData(unsigned char c1, unsigned char c2);
	
	BYTE AlphabetToBinaly(unsigned char c);
	WORD KanjiToBinaly(WORD wc);
	
	void GetRSCodeWord(LPBYTE lpbyRSWork, int ncDataCodeWord, int ncRSCodeWord);
	
	// モジュール配置関連ファンクション
private:
	void FormatModule();
	
	void SetFunctionModule();
	void SetFinderPattern(int x, int y);
	void SetAlignmentPattern(int x, int y);
	void SetVersionPattern();
	void SetCodeWordPattern();
	void SetMaskingPattern(int nPatternNo);
	void SetFormatInfoPattern(int nPatternNo);
	int CountPenalty();
};
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_)
