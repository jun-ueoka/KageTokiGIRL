//***********************************************************************************
//!	ファイル名		DirectInput.cpp
//
//	機能			DirectInput関係。キーの設定、キー処理など
//	プロジェクト	未定
//	更新			2008/04/25	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include "DirectInput.h"

//-----------------------------------------------------------------------
//!	直接入力の名前空間
//-----------------------------------------------------------------------
namespace DI
{
	//================================================================================
	//!	キーデータ配列
	//================================================================================
	static MYKEYDATA g_key[kMAX_KEY] =
	{
		{VK_UP,			FALSE, FALSE, FALSE},		//!< ↑キー
		{VK_DOWN,		FALSE, FALSE, FALSE},		//!< ↓キー
		{VK_LEFT,		FALSE, FALSE, FALSE},		//!< ←キー
		{VK_RIGHT,		FALSE, FALSE, FALSE},		//!< →キー
		{VK_SPACE,		FALSE, FALSE, FALSE},		//!< スペースキー
		{VK_SHIFT,		FALSE, FALSE, FALSE},		//!< シフトキー
		{VK_ESCAPE,		FALSE, FALSE, FALSE},		//!< エスケープキー
		{VK_SPACE,		FALSE, FALSE, FALSE},		//!< スペースキー
		{VK_RETURN,		FALSE, FALSE, FALSE},		//!< エンターキー
		{'A',			FALSE, FALSE, FALSE},		//!< Aキー
		{'C',			FALSE, FALSE, FALSE},		//!< Cキー
		{'D',			FALSE, FALSE, FALSE},		//!< Dキー
		{'S',			FALSE, FALSE, FALSE},		//!< Sキー
		{'W',			FALSE, FALSE, FALSE},		//!< Wキー
		{'X',			FALSE, FALSE, FALSE},		//!< Xキー
		{'Z',			FALSE, FALSE, FALSE},		//!< Zキー
	};

	//================================================================================
	//!	関数名		diCheck_Key
	//
	//	機能		キーデータの情報を更新
	//	更新		2008/04/18		<新規>
	//================================================================================
	void diCheck_Key(void)
	{
		int iCnt;		//!< ループカウンタ

		for(iCnt = 0; iCnt < kMAX_KEY; iCnt++)
		{
			if(GetAsyncKeyState(g_key[iCnt].virtualKey) & 0x8000)
			{
				if((!g_key[iCnt].pressedOnce) && (!g_key[iCnt].pressed))
				{
				//	初めてこのキーが押された
					g_key[iCnt].pressedOnce	= true;
					g_key[iCnt].pressed		= true;
				}
				else
				{
				//	前から押されていた
					g_key[iCnt].pressedOnce	= false;
				}
			}
			else
			{
			//	キーが離された
				if(g_key[iCnt].pressed)
				{
				//	初めて離された場合
					g_key[iCnt].pressedOnce	= false;
					g_key[iCnt].pressed		= false;
					g_key[iCnt].pressedNoneOnce = true;
				}
				else
				{
					g_key[iCnt].pressedNoneOnce = false;
				}
			}
		}
	}

	//================================================================================
	//!	関数名		diCheck_Press
	//
	//	機能		キーが押されているかどうかを調べる
	//	引数		myKey			調べるキー
	//	戻り値		TRUE			押されている
	//				FALSE			押されていない
	//	更新		2008/04/18		<新規>
	//================================================================================
	BOOL diCheck_Press(MYKEY myKey)
	{
		return g_key[myKey].pressed;
	}

	//================================================================================
	//!	関数名		diCheck_PressOnce
	//
	//	機能		キーが押されたかどうかを調べる
	//	引数		myKey			調べるキー
	//	戻り値		TRUE			押された
	//				FALSE			押していない(既に押されている)
	//	更新		2008/04/18		<新規>
	//================================================================================
	BOOL diCheck_PressOnce(MYKEY myKey)
	{
		return g_key[myKey].pressedOnce;
	}

	//================================================================================
	//!	関数名		diCheck_PressNone
	//
	//	機能		キーが押されていないかどうかを調べる
	//	引数		myKey			調べるキー
	//	戻り値		TRUE			何もキーが押されていない
	//				FALSE			何かキーが押されている
	//	更新		2008/04/18		<新規>
	//================================================================================
	BOOL diCheck_PressNone(void)
	{
		BOOL pressNone;		//!< キーが押されていないかどうか

		pressNone = TRUE;	//!< 初期値はTRUE

	//	すべてのキーを調べる
		for(int i = 0; i < kMAX_KEY; i++)
		{
			if((g_key[i].pressed) || (g_key[i].pressedOnce))
			{
			//	どこかのキーが押されていた場合
				pressNone = FALSE;
				break;
			}
		}

		return pressNone;
	}

	//================================================================================
	//!	関数名		diCheck_PressNone
	//
	//	機能		キーが初めて離されたかどうかを調べる
	//	引数		myKey			調べるキー
	//	戻り値		TRUE			今キーが離された
	//				FALSE			前から押されていない、もしくは押されている
	//	更新		2008/04/18		<新規>
	//================================================================================
	BOOL diCheck_PressNoneOnce(MYKEY myKey)
	{
		return g_key[myKey].pressedNoneOnce;
	}
}
//-----------------------------------------------------------------------
//!	直接入力の名前空間
//-----------------------------------------------------------------------