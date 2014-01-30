//***********************************************************************************
//!	ファイル名		Class_Direction.cpp
//
//	機能			DirectX演出機能を拡張したクラスの定義
//	機能概要		DirectXの関数、機能をカプセル化し、
//					値を送るだけで、その値に応じた演出を実行出来る
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/12/01	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_Direction.h"

//-----------------------------------------------------------------------
//!	DXライブラリの名前空間
//-----------------------------------------------------------------------
namespace DX
{
	//================================================================================
	//!	メソッド名	CDIRECTION::コンストラクタ
	//
	//	引数		inDev			演出対象のDirect3Dデバイス
	//				inScreenSizeX	スクリーンサイズX軸
	//				inScreenSizeY	スクリーンサイズY軸
	//	更新		2008/12/01		<新規>
	//================================================================================
	CDIRECTION::CDIRECTION(DEV inDev, int inScreenSizeX, int inScreenSizeY)
	{
	//	属性の初期化
		targetDevice	= inDev;
		iScreenSizeX	= inScreenSizeX;
		iScreenSizeY	= inScreenSizeY;
		cspd			= NULL;
	}

	//================================================================================
	//!	メソッド名	CDIRECTION::デストラクタ
	//
	//	更新		2008/12/01		<新規>
	//================================================================================
	CDIRECTION::~CDIRECTION(void)
	{
	//	それぞれ解放
		release();
	}

	//================================================================================
	//!	メソッド名	CDIRECTION::initCinemaScopePaintOut
	//
	//	機能		シネマスコープ(塗り潰しタイプ)の準備
	//	引数		inTopHeight		上部の高さ
	//				inBottomHeight	下部の高さ
	//				inColor			色
	//				inTexName		塗り潰しに使うテクスチャのパス(NULL)
	//	更新		2008/12/01		<新規>
	//================================================================================
	void
	CDIRECTION::initCinemaScopePaintOut(int inTopHeight, int inBottomHeight, float fPosZ,
										DWORD inColor, LPSTR inTexName)
	{
	//	初期解放
		releaseCinemaScopePaintOut();
	//	生成
		cspd	= new CINEMASCOPE_PAINTOUT_DATA();
	//	失敗したら、処理を返す
		if(	!cspd){ cspd = NULL; return;	}
	//	テクスチャの生成(テクスチャ名がNULLなら生成しない)
		if(	!inTexName)	{ cspd->texture = NULL;	}
		else
		{
		//	テクスチャの生成に失敗したら、NULLを入れておく
			if(	FAILED(D3DX_CREATE_TEXTURE_EX(
						targetDevice,	inTexName, 0,
						NULL, &cspd->texture)))
			{
				cspd->texture	= NULL;
			}
		}
	//	初期化用配列
	//	上部
		setVertex2D(&cspd->surfaceTop[0],
			-1.0f, -1.0f, fPosZ, 0.0f, inColor, 0.0f, 0.0f);
		setVertex2D(&cspd->surfaceTop[1],
			(float)iScreenSizeX, -1.0f, fPosZ, 0.0f, inColor, 1.0f, 0.0f);
		setVertex2D(&cspd->surfaceTop[2],
			(float)iScreenSizeX, (float)inTopHeight, fPosZ, 0.0f, inColor, 1.0f, 1.0f);
		setVertex2D(&cspd->surfaceTop[3],
			-1.0f, (float)inTopHeight, fPosZ, 0.0f, inColor, 0.0f, 1.0f);
	//	下部
		setVertex2D(&cspd->surfaceBottom[0],
			-1.0f, (float)iScreenSizeY-inBottomHeight, fPosZ, 0.0f, inColor, 0.0f, 0.0f);
		setVertex2D(&cspd->surfaceBottom[1],
			(float)iScreenSizeX, (float)iScreenSizeY-inBottomHeight, fPosZ, 0.0f, inColor, 1.0f, 0.0f);
		setVertex2D(&cspd->surfaceBottom[2],
			(float)iScreenSizeX, (float)iScreenSizeY, fPosZ, 0.0f, inColor, 1.0f, 1.0f);
		setVertex2D(&cspd->surfaceBottom[3],
			-1.0f, (float)iScreenSizeY, fPosZ, 0.0f, inColor, 0.0f, 1.0f);
	}

	//================================================================================
	//!	メソッド名	CDIRECTION::directCinemaScopePaintOut
	//
	//	機能		シネマスコープ(塗り潰しタイプ)の演出
	//	更新		2008/12/01		<新規>
	//================================================================================
	void
	CDIRECTION::directCinemaScopePaintOut(void)
	{
	//	準備がされてなければ、処理を返す
		if(	!cspd)	return;

		DWORD	dwFVF=0;	//!< FVFのバックアップ用

	//	テクスチャのセット
		targetDevice->SetTexture(0, cspd->texture);
		targetDevice->GetFVF(&dwFVF);
		targetDevice->SetFVF(D3DFVF_VERTEX_2D);
	//	上部の描画
		targetDevice->DrawPrimitiveUP(
			D3DPT_TRIANGLEFAN, 2, cspd->surfaceTop, sizeof(CUSTOM_VERTEX_2D));
	//	下部の描画
		targetDevice->DrawPrimitiveUP(
			D3DPT_TRIANGLEFAN, 2, cspd->surfaceBottom, sizeof(CUSTOM_VERTEX_2D));
		targetDevice->SetFVF(dwFVF);
		targetDevice->SetTexture(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CDIRECTION::releaseCinemaScopePaintOut
	//
	//	機能		シネマスコープ(塗り潰しタイプ)の解放
	//	更新		2008/12/01		<新規>
	//================================================================================
	void
	CDIRECTION::releaseCinemaScopePaintOut(void)
	{
	//	解放
		SAFE_DELETE(cspd);
	}
}
//-----------------------------------------------------------------------
//!	DXライブラリの名前空間
//-----------------------------------------------------------------------