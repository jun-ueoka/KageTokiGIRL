//***********************************************************************************
//!	ファイル名		Class_FontBase.h
//
//	機能			フォントベースに関する定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/09/09	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"DirectX.h"				//!< DirectX使用
#include	"UtilityFunction.h"		//!< ユーティリティ

//-----------------------------------------------------------------------
//!	フォントベースの名前空間
//-----------------------------------------------------------------------
namespace FB
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	マクロ定義
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		FONT_COLOR			D3DCOLOR_RGBA(  0,  0,  0,255)

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	フォントの列挙
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	FONT_KIND
	{
		eFONT_PTN1,					//!< ポップ体
		eFONT_PTN2,					//!< ムービーの字幕
		eFONT_COMMENT,				//!< コメント用
		eFONT_STAFFROLL,			//!< スタッフロール用
		eFONT_DEBUG,				//!< デバッグ用
		eFONT_MAX					//!< フォント最大数
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	フォント情報
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	FONT_DATA
	{
		LPD3DXFONT		font;			//!< フォント
		D3DXFONT_DESC	fontData;		//!< 読み込んだフォントのデータ

	//	実態化時の初期化
		FONT_DATA(void)
		{
			font		= NULL;
			ZeroMemory(&fontData, sizeof(fontData));
		}

	//	解放
		~FONT_DATA(void)
		{
		//	テクスチャ解放
			SAFE_RELEASE(font);
		}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	フォント文字列情報
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef struct	FONT_STRING
	{
		FONT_KIND		font;			//!< フォント番号
		char			szWord[256];	//!< 描画する文字列
		D3DCOLOR		color;			//!< 色
		RECT			rcPos;			//!< 位置
		float			fPriority;		//!< プライオリティ
		int				iTextCnt;		//!< テキストカウンタ(表示するバイト数)
		DWORD			dFormat;		//!< 描画方法

	//	実態化時の初期化(値指定)
		FONT_STRING(FONT_KIND	inFont	= eFONT_PTN1,
					LPSTR		inWord	= "",
					int			left	= 0,
					int			top		= 0,
					D3DCOLOR	inColor	= D3DCOLOR_RGBA(0,0,0,255),
					float		inPriority= 0.0f,
					int			inTextCnt = -1,
					DWORD		inFormat  = DT_NOCLIP)
		{
			font		= inFont;
			ZeroMemory(szWord, sizeof(szWord));
			strcpy_s(szWord, sizeof(szWord), inWord);
			SetRect(&rcPos, left, top, 1500, 1500);
			color		= inColor;
			fPriority	= inPriority;
			iTextCnt	= inTextCnt;
			dFormat		= inFormat;
		}
	}FONT_STR;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	徐々に表示するフォント文字列情報
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef struct	FONT_GRADUALLY_STRING : public FONT_STRING
	{
		int	iTextGraduallySpeed;			//!< 徐々表示する速度
		int	iTextGraduallyCnt;				//!< 徐々表示するフレームカウンタ

	//	実態化時の初期化(値指定)
		FONT_GRADUALLY_STRING(
			FONT_KIND	inFont	= eFONT_PTN1,
			LPSTR		inWord	= "",
			int			left	= 0,
			int			top		= 0,
			D3DCOLOR	inColor	= D3DCOLOR_RGBA(0,0,0,255),
			float		inPriority= 0.0f,
			DWORD		inFormat  = DT_NOCLIP,
			int			inTextCnt = -1,
			int			inTextGraduallySpeed = 10)	:
		FONT_STRING(inFont, inWord, left, top, inColor, inPriority, inTextCnt, inFormat)
		{
			iTextGraduallySpeed	= inTextGraduallySpeed;
			iTextGraduallyCnt	= 0;
		}

	//	徐々に表示する処理
		void step(void)
		{
		//	現在見ている文字がNULLでなければ、表示カウンタを進める
			if(	szWord[iTextCnt] != '\0')
			{
				iTextGraduallyCnt++;
				iTextCnt = (iTextGraduallyCnt / iTextGraduallySpeed);
			}
		}

	}FONT_GRASTR;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CFONT_BASE
	//
	//	機能			フォントベース
	//	更新			2008/09/09	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CFONT_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	オフセットマクロ
			//==================================================================
		#define	DEFAULT_SIZE		16					//!< デフォルトのサイズ
		#define	DEFAULT_WEIGHT		1					//!< デフォルトの幅
		#define	DEFAULT_RCSIZE		1000				//!< 矩形サイズ

			//==================================================================
			//	共有インスタンス
			//==================================================================
		static	CFONT_BASE*				gUniqueInstance;

			//==================================================================
			//	基本属性
			//==================================================================
				FONT_DATA*				fonts[eFONT_MAX];	//!< フォント配列
				vector<FONT_STRING*>	strings;			//!< 登録文字列

	public:			//-------------------- public --------------------//

				CFONT_BASE(void);							//!< コンストラクタ
		virtual	~CFONT_BASE(void);							//!< デストラクタ

		static	CFONT_BASE*	getInstance(void);				//!< 共有インスタンス生成
		static	void		releaseInstance(void);			//!< 共有インスタンス解放
		static	void		resetInstance(CONST DEV inDev);	//!< 共有インスタンスリセット

			//==================================================================
			//	リセット実行
			//==================================================================
				void	resetExecute(CONST DEV inDev);

			//==================================================================
			//	フォントの生成
			//==================================================================
				BOOL	createFont(	CONST DEV	inDev,
									FONT_KIND	inKind,
									LPSTR		inFontStyle,
									int			inHeight	= DEFAULT_SIZE,
									int			inWidth		= DEFAULT_SIZE,
									int			inWeight	= DEFAULT_WEIGHT,
									BOOL		inItalic	= FALSE);

			//==================================================================
			//	文字列の登録
			//==================================================================
				void	registerText(	LPSTR		inWord,
										FONT_KIND	inKind,
										int			inPosX,
										int			inPosY,
										float		inPosZ,
										D3DCOLOR	inColor,
										int			inSizeX	= DEFAULT_RCSIZE,
										int			inSizeY	= DEFAULT_RCSIZE,
										int			inTextCnt = -1,
										DWORD		inFormat  = DT_NOCLIP);

			//==================================================================
			//	フォントの登録確認
			//==================================================================
				BOOL	getFontValid(FONT_KIND inKind);

			//==================================================================
			//	描画（登録文字列の一斉描画）
			//==================================================================
				void	drawText(CONST SPRITE inSprite);
				void	drawText(CONST SPRITE inSprite, FONT_STR* inString);
				void	drawText(CONST SPRITE inSprite, FONT_GRASTR* inGraString);
				void	drawText(CONST SPRITE inSprite, FONT_KIND inKind, LPSTR inString,
								 int inLeft, int inTop, DWORD inColor = 0xFF000000,
								 DWORD inFormat = DT_NOCLIP);

			//==================================================================
			//	登録文字列の一斉解放
			//==================================================================
				void	releaseText(void);
	};
}
//-----------------------------------------------------------------------
//!	テクスチャベースの名前空間
//-----------------------------------------------------------------------