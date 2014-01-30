//***********************************************************************************
//!	ファイル名		DirectX.cpp
//
//	機能			DirectXを使用する上で、必要な関数、マクロの記述
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/12/02	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"DirectX.h"

//-----------------------------------------------------------------------
//	DirectX拡張関数群の名前空間
//-----------------------------------------------------------------------
namespace	DX
{
	//================================================================================
	//!	関数名		setVertex2D
	//
	//	機能		2D頂点の設定
	//	引数		ver2D			設定先頂点
	//				inX,inY,inZ		XYZ座標
	//				inRhw			RHW値
	//				inColor			色情報
	//				inU,inV			UV座標
	//	更新		2008/12/02		<新規>
	//================================================================================
	void
	setVertex2D(CUSTOM_VERTEX_2D* ver2D, float inX, float inY, float inZ,
				float inRhw, DWORD inColor, float inU, float inV)
	{
	//	値のセット
		ver2D->x		= inX;
		ver2D->y		= inY;
		ver2D->z		= inZ;
		ver2D->rhw		= inRhw;
		ver2D->color	= inColor;
		ver2D->u		= inU;
		ver2D->v		= inV;
	}

	//================================================================================
	//!	関数名		errorMsg
	//
	//	機能		エラーメッセージ&ウィンドウ破棄
	//	引数		inDev			Direct3Dデバイス
	//				inError			エラーメッセージ
	//				inTitle			エラーメッセージタイトル
	//	更新		2008/12/09		<新規>
	//================================================================================
	void
	errorMsg(CONST DEV inDev, LPSTR inError, LPSTR inTitle)
	{
		D3DDEVICE_CREATION_PARAMETERS	dcp;	//!< デバイス生成パラメータ
	//	デバイス作成パラメータを受け取る
		inDev->GetCreationParameters(&dcp);
	//	エラーメッセージの出現
		MessageBox(dcp.hFocusWindow, inError, inTitle, MB_OK);
	//	プログラムの終了
		DestroyWindow(dcp.hFocusWindow);
	}

	//================================================================================
	//!	関数名		errorMsgNoFile
	//
	//	機能		ファイルエラーメッセージ&ウィンドウ破棄
	//	引数		inDev			Direct3Dデバイス
	//				inError			エラーメッセージ
	//				inTitle			エラーメッセージタイトル
	//	更新		2008/12/09		<新規>
	//================================================================================
	void
	errorMsgNoFile(CONST DEV inDev, LPSTR inError, LPSTR inTitle)
	{
		D3DDEVICE_CREATION_PARAMETERS	dcp;	//!< デバイス生成パラメータ
		char	szError[MAX_PATH] = "";			//!< エラーメッセージ
	//	エラーメッセージの生成
		sprintf_s(szError, sizeof(szError), "Error File To [%s]", inError);
	//	デバイス作成パラメータを受け取る
		inDev->GetCreationParameters(&dcp);
	//	エラーメッセージの出現
		MessageBox(dcp.hFocusWindow, szError, inTitle, MB_OK);
	//	プログラムの終了
		DestroyWindow(dcp.hFocusWindow);
	}
}
//-----------------------------------------------------------------------
//	DirectX拡張関数群の名前空間
//-----------------------------------------------------------------------