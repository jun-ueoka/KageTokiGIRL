//***********************************************************************************
//!	ファイル名		Class_SceneAdventureGameData.h
//
//	機能			ゲームのシーンに必要なデータを列挙
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2008/07/10	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_AdventPlayer.h"				//!< アドベンチャーゲームプレイヤー
#include	"Class_Back.h"						//!< 背景
#include	"Class_AdventEnemyManager.h"		//!< エネミーマネージャー

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	ステージリソースファイルへのパス
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_GAMESTAGE		"..//Resources//Data//Stage//GameStage%d.dat"

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	イベントスイッチリソースファイルへのパス
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_EVENTSWITCH	"..//Resources//Data//Event//EventSwitch%d.dat"
	#define		PATH_EVENT			"..//Resources//Data//Event//Event%d.dat"

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	ゲームシーンの分岐フラグ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	SGAME_FLAG
	{
		eSGFLAG_PLAY,					//!< ゲームプレイ中
		eSGFLAG_EVENT,					//!< ゲームイベント中
		eSGFLAG_CONTINUE,				//!< コンティニュー中
		eSGFLAG_STAGE_CHANGE,			//!< マップチェンジ中
		eSGFLAG_STAGE_OUT,				//!< ステージアウト中
		eSGFLAG_RESET,					//!< 再スタート中
		eSGFLAG_CLEAR,					//!< ゲームクリア中
		eSGFLAG_QUIT,					//!< ゲーム終了中
		eSGFLAG_ERROR,					//!< ゲーム中エラー
		eSGFLAG_NO,						//!< フラグ未処理
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	ゲームシーンのタイミングステップフラグ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	SGAME_TIMING_STEP
	{
		eSGTSTEP_END_PROC,				//!< ステップが処理され済み
		eSGTSTEP_CHANGE_PLAY,			//!< ゲームプレイ中へ移行された時
		eSGTSTEP_CHANGE_EVENT,			//!< ゲームイベント中へ移行された時
		eSGTSTEP_CHANGE_CONTINUE,		//!< コンティニュー中へ移行された時
		eSGTSTEP_CHANGE_STAGE_CHANGE,	//!< マップチェンジ中へ移行された時
		eSGTSTEP_CHANGE_RESET,			//!< 再スタート中へ移行された時
		eSGTSTEP_CHANGE_CLEAR,			//!< ゲームクリア中へ移行された時
		eSGTSTEP_CHANGE_QUIT,			//!< ゲーム終了中へ移行された時
		eSGTSTEP_CHANGE_ERROR,			//!< ゲーム中エラーへ移行された時
		eSGTSTEP_NO,					//!< ステップなし
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	再スタートシーン時の処理分岐フラグ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	SGAME_CONTINUE_FLAG
	{
		eSGCONTI_START,					//!< 再スタート処理開始
		eSGCONTI_BLACKOUT,				//!< ブラックアウト中
		eSGCONTI_CONTINUE_INIT,			//!< コンティニュー選択処理移行中
		eSGCONTI_CONTINUE,				//!< コンティニュー選択処理中
		eSGCONTI_CHOICE_YES,			//!< 「YES」選択後処理中
		eSGCONTI_CHOICE_NO,				//!< 「NO」選択後処理中
		eSGCONTI_GAMEOVER,				//!< 強制ゲームオーバー処理中
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	背景の種類フラグ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	SGAME_BACK_KIND
	{
		eSGBACK_SKY	= 0,				//!< 空
		eSGBACK_MAX,					//!< 最大種類フラグ
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	イベントステップ識別
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	EVENT_STEP
	{
		eEVSTEP_FALSE,					//!< イベントが無い
		eEVSTEP_READ,					//!< 読み込み中
		eEVSTEP_EXECUTE,				//!< 実行中(待機と同類)
		eEVSTEP_TALK,					//!< 会話中
		eEVSTEP_INPUTWAIT,				//!< 入力待機中
		eEVSTEP_ERROR,					//!< イベントエラー
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	イベント命令識別
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	EVENT_ORDER
	{
		eEVORDER_TALK,					//!< コメントの生成			//!< TALK____
		eEVORDER_TALK_START,			//!< コメント開始			//!< TALK_STA
		eEVORDER_OBJ_CREATE,			//!< 物体の生成				//!< OBJ_CREA
		eEVORDER_OBJ_CHANGE,			//!< 物体の状態変化			//!< OBJ_CHAN
		eEVORDER_OBJ_MOVE,				//!< 物体移動				//!< OBJ_MOVE
		eEVORDER_OBJ_CLEAR,				//!< 生成した物体の消滅		//!< OBJ_CLEA
		eEVORDER_EFF_CREATE,			//!< エフェクトの生成		//!< EFF_CREA
		eEVORDER_EFF_OCCU,				//!< エフェクトの発生		//!< EFF_OCCU
		eEVORDER_EFF_CLEAR,				//!< エフェクトの消滅		//!< EFF_CLEA
		eEVORDER_INPUT_OPE,				//!< ダミー入力操作			//!< INPUTOPE
		eEVORDER_INPUT_WAIT,			//!< 入力待機				//!< INPUTWAI
		eEVORDER_EXECUTE,				//!< 実行					//!< EXECUTE_
		eEVORDER_GAME_CLEAR,			//!< ゲームクリア			//!< GAME_CLE
		eEVORDER_EVENT_QUIT,			//!< イベント終了			//!< EV_QUIT_
		eEVORDER_MAX,					//!< 最大イベント数
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	物体識別
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	EVENT_OBJECT
	{
		eEVOBJ_PLAYER,					//!< プレイヤー				//!< PLAYER__
		eEVOBJ_CAMERA,					//!< カメラ					//!< CAMERA__
		eEVOBJ_EVOBJECT,				//!< イベント中の物			//!< EVOBJECT
		eEVOBJ_MAX,						//!< 最大物体種数
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	状態変化識別
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	EVENT_CHANGE
	{
	//	プレイヤー限定
		eEVCHAN_VISIBLE,				//!< 表示確認				//!< VISIBLE_
		eEVCHAN_ANIMCHANGE,				//!< アニメーション変更		//!< ANIMCHAN
		eEVCHAN_LIMIT,					//!< 入力有効制御変更		//!< LIMIT___
		eEVCHAN_LIMIT_KAGEO,			//!< カゲオ限定制御変更		//!< LIMIT_KO
		eEVCHAN_LIMIT_KAGEMI,			//!< カゲミ限定制御変更		//!< LIMIT_KI
		eEVCHAN_CHARACHANGE_BOOLEN,		//!< キャラ変更有効確認		//!< CHARCHAN
		eEVCHAN_CHARACHANGE_KAGEO,		//!< カゲオにキャラ変更		//!< CHANGEKO
		eEVCHAN_CHARACHANGE_KAGEMI,		//!< カゲミにキャラ変更		//!< CHANGEKI
	//	カメラ限定
		eEVCHAN_PARALLEL,				//!< 視点平衡移動モード		//!< PARALLEL
		eEVCHAN_PARACHASE,				//!< カメラ平衡移動モード	//!< PARACHAS
		eEVCHAN_DOUBLETARGET,			//!< 二重追跡モード			//!< DOUBLETA
		eEVCHAN_MAX,					//!< 最大状態変化数
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	生成方法識別
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	EVENT_CREATE_OBJ
	{
		eEVCRE_TEXTURE,					//!< テクスチャ生成			//!< CTEX____
		eEVCRE_MESH,					//!< メッシュ生成			//!< CMESH___
		eEVCRE_SKINMESH,				//!< スキンメッシュ生成		//!< CSKIN___
		eEVCRE_EFFECT,					//!< エフェクト生成			//!< CEFFECT_
		eEVCRE_MAX,						//!< 最大生成方法数
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	オブジェクト登録先識別
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	EVENT_INSERT
	{
		eEVINS_GAMEMAP,					//!< ゲームマップ			//!< MAP_____
		eEVINS_WAIT_2D,					//!< 待機中の2D描画			//!< WAIT_2D_
		eEVINS_MAX,						//!< 最大オブジェクト登録先数
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CEVENT_JUDGE_TASKER
	//
	//	機能			イベント判定用クラスの定義
	//	更新			2008/11/10	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CEVENT_JUDGE_TASKER
	{
	public:			//-------------------- public --------------------//
			//	イベント命令識別判定
				EVENT_ORDER			judgeEventOrderToString(LPSTR szString);
			//	物体識別判定
				EVENT_OBJECT		judgeEventObjectToString(LPSTR szString);
			//	状態変化識別判定
				EVENT_CHANGE		judgeEventChangeToString(LPSTR szString);
			//	物体生成識別判定
				EVENT_CREATE_OBJ	judgeEventCreateObjToString(LPSTR szString);
			//	オブジェクト登録先識別判定
				EVENT_INSERT		judgeEventInsertToString(LPSTR szString);
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CEXECUTER
	//
	//	機能			イベント動作処理クラス
	//	機能概要		これをスーパークラスとし、様々な処理を持った
	//					クラスを定義する。
	//	更新			2008/11/10	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CEXECUTER
	{
	public:			//-------------------- public --------------------//
			//	イベント実行
		virtual	BOOL	execute(void)	=0;
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CEXE_MOVE_OBJECT
	//
	//	機能			3Dオブジェクトを動かすイベント
	//	機能概要		移動フレーム数で動かす
	//	更新			2008/11/10	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CEXE_MOVE_OBJECT : public CEXECUTER
	{
	protected:		//-------------------- protected --------------------//
				C3D_OBJECT*		object;				//!< 対象オブジェクト
				int				iMoveFrame;			//!< 移動フレーム
				VEC3			moveValue;			//!< 移動量
	public:			//-------------------- public --------------------//
			//==================================================================
			//	コンストラクタ
			//==================================================================
				CEXE_MOVE_OBJECT(C3D_OBJECT* inObject, int inFrame, VEC3 inValue){
					object		= inObject;
					iMoveFrame	= inFrame;
					moveValue	= inValue;
				}
			//==================================================================
			//	イベント実行
			//==================================================================
				BOOL	execute(void){
					if(	iMoveFrame > 0){
						object->moveValue(&moveValue); iMoveFrame--; return TRUE;
					} else {
						return	FALSE;
					}
				}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CEXE_DUMMY_INPUT
	//
	//	機能			ダミー入力の操作
	//	更新			2008/11/11	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CEXE_DUMMY_INPUT : public CEXECUTER
	{
	protected:		//-------------------- protected --------------------//
				CDUMMY_INPUT*	dummyInput;			//!< 操作対象のダミー入力
				int				iFrame;				//!< 操作フレーム
				DWORD			targetKey;			//!< 操作するキーの種類
	public:			//-------------------- public --------------------//
			//==================================================================
			//	コンストラクタ
			//==================================================================
				CEXE_DUMMY_INPUT(CDUMMY_INPUT* inDummy, int inFrame, DWORD inKey){
					dummyInput	= inDummy;
					iFrame		= inFrame;
					targetKey	= inKey;
				}
			//==================================================================
			//	イベント実行
			//==================================================================
				BOOL	execute(void){
					if(	iFrame > 0){
						dummyInput->setPress((MYKEY::TYPENAME)targetKey, true);
						iFrame--;	return	TRUE;
					} else {
						dummyInput->setPress((MYKEY::TYPENAME)targetKey, false);
						return	FALSE;
					}
				}
	};
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------