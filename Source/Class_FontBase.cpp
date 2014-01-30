//***********************************************************************************
//!	ファイル名		Class_FontBase.cpp
//
//	機能			フォントベースに関する定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/09/09	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_FontBase.h"				//!< フォントベース

//-----------------------------------------------------------------------
//!	フォントベースの名前空間
//-----------------------------------------------------------------------
namespace FB
{
	//================================================================================
	//	共有インスタンス
	//================================================================================
	CFONT_BASE*		CFONT_BASE::gUniqueInstance	= NULL;

	//================================================================================
	//!	メソッド名	CFONT_BASE::getInstance
	//
	//	機能		共有インスタンス生成
	//	戻り値		生成されたインスタンス
	//	更新		2008/09/09		<新規>
	//================================================================================
	CFONT_BASE*
	CFONT_BASE::getInstance(void)
	{
	//	生成されていなければ、生成する
		if(!gUniqueInstance)
		{
			gUniqueInstance	= new CFONT_BASE();
		}
	//	インスタンスを返す
		return	gUniqueInstance;
	}

	//================================================================================
	//!	メソッド名	CFONT_BASE::releaseInstance
	//
	//	機能		共有インスタンス解放
	//	更新		2008/09/09		<新規>
	//================================================================================
	void
	CFONT_BASE::releaseInstance(void)
	{
	//	解放
		SAFE_DELETE(gUniqueInstance);
	}

	//================================================================================
	//!	メソッド名	CFONT_BASE::resetInstance
	//
	//	機能		共有インスタンスリセット
	//	引数		inDev			Direct3Dデバイス
	//	更新		2009/03/02		<新規>
	//================================================================================
	void
	CFONT_BASE::resetInstance(CONST DEV inDev)
	{
	//	生成されていなければ、生成する
		if(!gUniqueInstance)
		{
			gUniqueInstance->resetExecute(inDev);
		}
	}

	//================================================================================
	//!	メソッド名	CFONT_BASE::resetExecute
	//
	//	機能		リセット実行
	//	引数		inDev			Direct3Dデバイス
	//	更新		2009/03/02		<新規>
	//================================================================================
	void
	CFONT_BASE::resetExecute(CONST DEV inDev)
	{
	//	フォントが生成されている配列だけリセット
		for(int i=0; i<eFONT_MAX; i++)
		{
			if(	fonts[i])
			{
			//	先に解放
				SAFE_RELEASE(fonts[i]->font);
			//	取っておいたフォントデータで、フォントを復元
				if(	FAILED(D3DXCreateFontIndirect(inDev, &fonts[i]->fontData, &fonts[i]->font)))
				{
				//	生成データを破棄
					SAFE_DELETE(fonts[i]);
					DX::errorMsgNoFile(inDev, fonts[i]->fontData.FaceName);
				}
			}
		}
	}

	//================================================================================
	//!	メソッド名	CFONT_BASE::コンストラクタ
	//
	//	更新		2008/09/09		<新規>
	//================================================================================
	CFONT_BASE::CFONT_BASE(void)
	{
	//	フォント配列の初期化
		for(int i=0; i<eFONT_MAX; i++)
		{
			fonts[i]	= NULL;
		}
	}

	//================================================================================
	//!	メソッド名	CFONT_BASE::デストラクタ
	//
	//	更新		2008/09/09		<新規>
	//================================================================================
	CFONT_BASE::~CFONT_BASE(void)
	{
	//	フォント配列の解放
		for(int i=0; i<eFONT_MAX; i++)
		{
			SAFE_DELETE(fonts[i]);
		}
	//	登録文字列の解放
		releaseText();
	}

	//================================================================================
	//!	メソッド名	CFONT_BASE::createFont
	//
	//	機能		フォントの生成
	//	引数		inDev			Direct3Dデバイス
	//				inKind			フォントの種類
	//				inFontStyle		フォントの名称
	//				inHeight		フォントの縦サイズ
	//				inWidth			フォントの横サイズ
	//				inWeight		フォントの横幅
	//				inItalic		イタリック確認
	//	戻り値		TRUE			生成成功
	//				FALSE			生成失敗
	//	更新		2008/09/09		<新規>
	//================================================================================
	BOOL
	CFONT_BASE::createFont(	CONST DEV	inDev,
							FONT_KIND	inKind,
							LPSTR		inFontStyle,
							int			inHeight,
							int			inWidth,
							int			inWeight,
							BOOL		inItalic)
	{
	//	送られてきた情報に不正があるなら、処理をせずに失敗を返す
		if(	(inKind < 0) && (inKind >= eFONT_MAX))	return	FALSE;

	//	新たなフォントデータを生成
		FONT_DATA*	newFont		= new FONT_DATA();
		
	//	フォントの設定
		sprintf_s(	newFont->fontData.FaceName,
					sizeof(newFont->fontData.FaceName),
					inFontStyle);											//!< フォントの設定
		newFont->fontData.Height			= inHeight;						//!< フォントの高さ
		newFont->fontData.Width				= inWidth;						//!< フォントの幅
		newFont->fontData.Weight			= inWeight;						//!< フォントの太さ
		newFont->fontData.MipLevels			= 0;							//!< フォントのミップレベル
		newFont->fontData.Italic			= inItalic;						//!< 斜体
		newFont->fontData.CharSet			= SHIFTJIS_CHARSET;				//!< フォントの文字セット(デフォルトのもの)
		newFont->fontData.OutputPrecision	= OUT_DEFAULT_PRECIS;			//!< 出力精度
		newFont->fontData.Quality			= ANTIALIASED_QUALITY;			//!< 出力品質
		newFont->fontData.PitchAndFamily	= DEFAULT_PITCH | FF_DONTCARE;	//!< ピッチとファミリ

	//	フォントの生成に失敗した時
		if(	FAILED(D3DXCreateFontIndirect(inDev, &newFont->fontData, &newFont->font)))
		{
		//	生成データを破棄
			SAFE_DELETE(newFont);
			DX::errorMsgNoFile(inDev, inFontStyle);
		//	生成失敗を返す
			return FALSE;
		}

	//	フォントの生成に成功した時、前のデータを上書き
		SAFE_DELETE(fonts[inKind]);
		fonts[inKind]	= newFont;

	//	生成成功を返す
		return TRUE;
	}

	//================================================================================
	//!	メソッド名	CFONT_BASE::registerText
	//
	//	機能		文字列の登録
	//	引数		inWord			文字列
	//				inKind			使用するフォント
	//				inPosX			X座標位置
	//				inPosY			Y座標位置
	//				inPosZ			Z座標位置
	//				inColor			描画カラー
	//				inSizeX			矩形サイズX
	//				inSizeY			矩形サイズY
	//				inTextCnt		テキストカウンタ(表示する文字数)
	//	更新		2008/09/09		<新規>
	//================================================================================
	void
	CFONT_BASE::registerText(LPSTR		inWord,
							FONT_KIND	inKind,
							int			inPosX,
							int			inPosY,
							float		inPosZ,
							D3DCOLOR	inColor,
							int			inSizeX,
							int			inSizeY,
							int			inTextCnt,
							DWORD		inFormat)
	{
	//	送られてきた情報に不正があるなら、処理をせずに終了
		if(	(inKind < 0) && (inKind >= eFONT_MAX))	return;

	//	新たな文字列データを生成
		FONT_STRING*	newString	= new FONT_STRING();

	//	描画位置の設定
		RECT	 rcDraw		= {	inPosX, inPosY, inSizeX, inSizeY};

	//	文字列の設定
		sprintf_s(newString->szWord, sizeof(newString->szWord), inWord);	//!< 表示文字列設定
		newString->font			= inKind;									//!< 使用フォント設定
		newString->rcPos		= rcDraw;									//!< 表示位置設定
		newString->fPriority	= inPosZ;									//!< プライオリティ
		newString->color		= inColor;									//!< 色の設定
		newString->iTextCnt		= inTextCnt;								//!< 表示する文字数
		newString->dFormat		= inFormat;									//!< 描画方法

	//	表示文字列一覧に登録
		strings.push_back(newString);
	}

	//================================================================================
	//!	メソッド名	CFONT_BASE::getFontValid
	//
	//	機能		フォントの登録確認
	//	引数		inKind			確認する種類
	//	戻り値		TRUE			登録済み
	//				FALSE			未登録
	//	更新		2008/09/09		<新規>
	//================================================================================
	BOOL
	CFONT_BASE::getFontValid(FONT_KIND inKind)
	{
	//	確認する種類が不正なら、未登録を返す
		if(	(inKind < 0) || (inKind >= eFONT_MAX))	return	FALSE;

	//	登録確認し、結果を返す
		return	((fonts[inKind]) ? TRUE : FALSE);
	}

	//================================================================================
	//!	メソッド名	CFONT_BASE::drawText
	//
	//	機能		登録文字列の一斉描画
	//	引数		inSprite		スプライトデバイス
	//	更新		2008/09/09		<新規>
	//================================================================================
	void
	CFONT_BASE::drawText(CONST SPRITE inSprite)
	{
	//	登録されている文字列が無ければ、処理をしないで終了
		if(	strings.empty())	return;

	//	描画開始
		vector<FONT_STRING*>::iterator	p;
		for(p  = strings.begin();
			p != strings.end();
			p++)
		{
			drawText(inSprite, *p);
		}
	}

	//================================================================================
	//!	メソッド名	CFONT_BASE::drawText
	//
	//	機能		登録文字列の一斉描画
	//	引数		inSprite		スプライトデバイス
	//				inString		描画文字列
	//	更新		2008/09/09		<新規>
	//================================================================================
	void
	CFONT_BASE::drawText(CONST SPRITE inSprite, FONT_STR* inString)
	{
	//	スプライトが無ければ、処理をしないで終了
		if(	!inSprite)			return;

	//	フォント配列に、指定フォントがあれば描画
		if(	fonts[inString->font])
		{
		//	プライオリティ設定用行列
			MAT		mat;
		//	プライオリティを行列に設定
			D3DXMatrixTranslation(&mat, 0.0f, 0.0f, inString->fPriority);
		//	プライオリティを反映させる
			inSprite->SetTransform(&mat);

		//	文字列の描画
			fonts[inString->font]->font->DrawText(
				inSprite,
				inString->szWord,
				inString->iTextCnt,
				&inString->rcPos,
				inString->dFormat,
				inString->color);
		}
	}

	//================================================================================
	//!	メソッド名	CFONT_BASE::drawText
	//
	//	機能		登録文字列の一斉描画(段々表示)
	//	引数		inSprite		スプライトデバイス
	//				inGraString		段々表示する描画文字列
	//	更新		2008/09/09		<新規>
	//================================================================================
	void
	CFONT_BASE::drawText(CONST SPRITE inSprite, FONT_GRASTR* inGraString)
	{
	//	スプライトが無ければ、処理をしないで終了
		if(	!inSprite)			return;

	//	フォント配列に、指定フォントがあれば描画
		if(	fonts[inGraString->font])
		{
		//	現在見ている文字がNULLでなければ、表示カウンタを進める
			if(	inGraString->szWord[inGraString->iTextCnt] != '\0')
			{
				inGraString->iTextGraduallyCnt++;
				inGraString->iTextCnt = (inGraString->iTextGraduallyCnt /
										 inGraString->iTextGraduallySpeed);
			}
		//	プライオリティ設定用行列
			MAT		mat;
		//	プライオリティを行列に設定
			D3DXMatrixTranslation(&mat, 0.0f, 0.0f, inGraString->fPriority);
		//	プライオリティを反映させる
			inSprite->SetTransform(&mat);

		//	文字列の描画
			fonts[inGraString->font]->font->DrawText(
				inSprite,
				inGraString->szWord,
				inGraString->iTextCnt,
				&inGraString->rcPos,
				inGraString->dFormat,
				inGraString->color);
		}
	}

	//================================================================================
	//!	メソッド名	CFONT_BASE::drawText
	//
	//	機能		登録文字列の一斉描画(段々表示)
	//	引数		inSprite		スプライトデバイス
	//				inKind			列挙番号
	//				inString		描画する文字列
	//	更新		2008/09/09		<新規>
	//================================================================================
	void
	CFONT_BASE::drawText(CONST SPRITE inSprite, FONT_KIND inKind, LPSTR inString,
						 int inLeft, int inTop, DWORD inColor, DWORD inFormat)
	{
	//	スプライトが無ければ、処理をしないで終了
		if(	!inSprite)			return;

	//	フォント配列に、指定フォントがあれば描画
		if(	fonts[inKind])
		{
		//	プライオリティ設定用行列
			MAT		mat;
		//	プライオリティを行列に設定
			D3DXMatrixTranslation(&mat, 0.0f, 0.0f, 0.0f);
		//	プライオリティを反映させる
			inSprite->SetTransform(&mat);

		//	描画範囲
			RECT	rcDraw	= { inLeft, inTop, 2000, 2000 };

		//	文字列の描画
			fonts[inKind]->font->DrawText(
				inSprite, inString, (UINT)strlen(inString),
				&rcDraw, inFormat, inColor);
		}
	}

	//================================================================================
	//!	メソッド名	CFONT_BASE::releaseText
	//
	//	機能		登録文字列の一斉解放
	//	更新		2008/09/09		<新規>
	//================================================================================
	void
	CFONT_BASE::releaseText(void)
	{
	//	解放開始
		vector<FONT_STRING*>::iterator	p;
		for(p  = strings.begin();
			p != strings.end();
			p++)
		{
			SAFE_DELETE(*p);
		}
		strings.clear();
	}
}
//-----------------------------------------------------------------------
//!	フォントベースの名前空間
//-----------------------------------------------------------------------