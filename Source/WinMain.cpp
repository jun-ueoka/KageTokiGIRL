//***********************************************************************************
//!	ファイル名		WinMain.cpp
//
//	機能			WinMainに関する処理
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/04/16	<新規>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_WindowManager.h"
//#include	"UtilityFunction.h"

//================================================================================
//!	関数名		WndProc
//
//	機能		ウィンドウプロシージャ
//	引数		inWnd		ウィンドウハンドル
//				inMessage	メッセージ
//				inWParam	Wパラメータ
//				inLParam	Lパラメータ
//	戻り値		処理に応じた値を返す
//	更新		2008/04/16	<新規>
//================================================================================
LRESULT CALLBACK
WndProc(HWND inWnd, UINT inMessage, WPARAM inWParam, LPARAM inLParam)
{
	LRESULT	ret((LRESULT)0);			//!< 戻り値格納用

//!	メッセージによって処理分岐
	switch(inMessage)
	{
	//!	ウィンドウ破棄時
		case WM_DESTROY:
			DestroyWindow(inWnd);										//!< ウィンドウの破棄
			PostQuitMessage(0);											//!< システムに終了要求
			break;

	//! その他のメッセージ
		default:
			ret = DefWindowProc(inWnd, inMessage, inWParam, inLParam);	//!< Windowsに処理をお願い
			break;
	}
	return ret;							//!< 処理に応じた値を返す
}

//================================================================================
//!	関数名		WinMain
//
//	機能		処理開始
//	引数		hInstance	6プログラムに割り当てられたインスタンスハンドル
//				lpCmdLine	コマンドラインからの文字列
//				nShowCmd	ウィンドウ表示方法
//	戻り値		処理終了を意味する
//	更新		2008/04/16	<新規>
//================================================================================
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
//! デバッグモード
#ifdef	_DEBUG
//!	メモリリークのチェックを行う
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif//_DEBUG

	WM::CWINDOW_MANAGER*	WinMamager	= NULL;						//!< ウィンドウ管理
	int						iRet		= 0;						//!< 戻り値用

//!	CWINDOW_MANAGERの生成
	if(	!(WinMamager = WM::CWINDOW_MANAGER::getInstance(hInstance, nShowCmd)))
	{
		return 0;													//!< 生成に失敗
	}
	iRet	= WinMamager->messageLoop();							//!< アプリケーションのループ
	WinMamager->releaseInstance();									//!< WinClassの解放

#ifdef _DEBUG
//!	リークチェック
	_CrtDumpMemoryLeaks();
#endif 

	return iRet;
}