//***********************************************************************************
//!	ファイル名		Class_SceneAdventureGameData.cpp
//
//	機能			ゲームのシーンに必要なデータを実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			22008/11/06	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneAdventureGameData.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CEVENT_JUDGE_TASKER::judgeEventOrderToString
	//
	//	機能		イベント命令識別判定
	//	引数		szString		判定文字列
	//	戻り値		イベント命令識別判定結果
	//	更新		2008/11/10		<新規>
	//================================================================================
	EVENT_ORDER
	CEVENT_JUDGE_TASKER::judgeEventOrderToString(LPSTR szString)
	{
	//	イベント判定用文字配列
		char	orderJudger[eEVORDER_MAX][9]	=
		{
			"TALK____",
			"TALK_STA",
			"OBJ_CREA",
			"OBJ_CHAN",
			"OBJ_MOVE",
			"OBJ_CLEA",
			"EFF_CREA",
			"EFF_OCCU",
			"EFF_CLEA",
			"INPUTOPE",
			"INPUTWAI",
			"EXECUTE_",
			"GAME_CLE",
			"EV_QUIT_"
		};
	//	イベントの種類を、数値として検出
		int	iFlag;
		for(iFlag=0; iFlag<eEVORDER_MAX; iFlag++)
		{
			if(	strcmp(szString, orderJudger[iFlag]) == 0)	break;
		}
	//	イベントの種類を返す
		return	EVENT_ORDER(iFlag);
	}

	//================================================================================
	//!	メソッド名	CEVENT_JUDGE_TASKER::judgeEventObjectToString
	//
	//	機能		物体識別判定
	//	引数		szString		判定文字列
	//	戻り値		物体識別判定結果
	//	更新		2008/11/10		<新規>
	//================================================================================
	EVENT_OBJECT
	CEVENT_JUDGE_TASKER::judgeEventObjectToString(LPSTR szString)
	{
	//	物体判定用文字配列
		char	objectJudger[eEVOBJ_MAX][9]	=
		{
			"PLAYER__",
			"CAMERA__",
			"EVOBJECT"
		};
	//	物体の種類を、数値として検出
		int	iFlag;
		for(iFlag=0; iFlag<eEVOBJ_MAX; iFlag++)
		{
			if(	strcmp(szString, objectJudger[iFlag]) == 0)	break;
		}
	//	物体の種類を返す
		return	EVENT_OBJECT(iFlag);
	}

	//================================================================================
	//!	メソッド名	CEVENT_JUDGE_TASKER::judgeEventChangeToString
	//
	//	機能		状態変化識別判定
	//	引数		szString		判定文字列
	//	戻り値		状態変化識別判定結果
	//	更新		2008/11/10		<新規>
	//================================================================================
	EVENT_CHANGE
	CEVENT_JUDGE_TASKER::judgeEventChangeToString(LPSTR szString)
	{
	//	状態変化判定用文字配列
		char	changeJudger[eEVCHAN_MAX][9]	=
		{
			"VISIBLE_",
			"ANIMCHAN",
			"LIMIT___",
			"LIMIT_KO",
			"LIMIT_KI",
			"CHARCHAN",
			"CHANGEKO",
			"CHANGEKI",
			"PARALLEL",
			"PARACHAS",
			"DOUBLETA"
		};
	//	状態変化の種類を、数値として検出
		int	iFlag;
		for(iFlag=0; iFlag<eEVCHAN_MAX; iFlag++)
		{
			if(	strcmp(szString, changeJudger[iFlag]) == 0)	break;
		}
	//	状態変化の種類を返す
		return	EVENT_CHANGE(iFlag);
	}

	//================================================================================
	//!	メソッド名	CEVENT_JUDGE_TASKER::judgeEventCreateObjToString
	//
	//	機能		物体生成識別判定
	//	引数		szString		判定文字列
	//	戻り値		物体生成識別判定結果
	//	更新		2008/11/10		<新規>
	//====================================================b============================
	EVENT_CREATE_OBJ
	CEVENT_JUDGE_TASKER::judgeEventCreateObjToString(LPSTR szString)
	{
	//	物体生成判定用文字配列
		char	createObjJudger[eEVCRE_MAX][9]	=
		{
			"CTEX____",
			"CMESH___",
			"CSKIN___",
			"CEFFECT_"
		};
	//	物体生成の種類を、数値として検出
		int	iFlag;
		for(iFlag=0; iFlag<eEVCRE_MAX; iFlag++)
		{
			if(	strcmp(szString, createObjJudger[iFlag]) == 0)	break;
		}
	//	物体生成の種類を返す
		return	EVENT_CREATE_OBJ(iFlag);
	}

	//================================================================================
	//!	メソッド名	CEVENT_JUDGE_TASKER::judgeEventInsertToString
	//
	//	機能		オブジェクト登録先識別判定
	//	引数		szString		判定文字列
	//	戻り値		オブジェクト登録先識別判定結果
	//	更新		2008/11/10		<新規>
	//====================================================b============================
	EVENT_INSERT
	CEVENT_JUDGE_TASKER::judgeEventInsertToString(LPSTR szString)
	{
	//	オブジェクト登録先判定用文字配列
		char	insertJudger[eEVINS_MAX][9]	=
		{
			"MAP_____",
			"WAIT_2D_"
		};
	//	オブジェクト登録先の種類を、数値として検出
		int	iFlag;
		for(iFlag=0; iFlag<eEVINS_MAX; iFlag++)
		{
			if(	strcmp(szString, insertJudger[iFlag]) == 0)	break;
		}
	//	オブジェクト登録先の種類を返す
		return	EVENT_INSERT(iFlag);
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------