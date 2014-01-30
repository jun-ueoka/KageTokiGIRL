//***********************************************************************************
//!	ファイル名		Class_SceneAdventureGame.h
//
//	機能			ゲームシーンを定義
//	機能概要		コレにゲームの主軸を実装して、派生させる
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2008/07/10	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_SceneAdventureGameData.h"		//!< ゲームデータ
#include	"Class_Scene.h"							//!< シーン

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CSCENE_ADVENTURE_GAME
	//
	//	機能			ゲームシーンのクラス
	//	更新			2008/11/06	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_ADVENTURE_GAME : public CSCENE, protected CEVENT_JUDGE_TASKER
	{
			//==================================================================
			//	定数群
			//==================================================================
				#define	READ_SOUND_NONE		"S_NONE"				//!< 読み込みサウンド無し確認

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	環境クラス
			//==================================================================
				CGRAPHIC_DEVICE*			gDev;					//!< グラフィックデバイス
				C3D_EFFECT_CENTER*			effectCenter;			//!< エフェクトセンター
				SGAME_FLAG					gameFlag;				//!< ゲームフラグ
				SGAME_TIMING_STEP			gameStep;				//!< ゲームステップフラグ

			//==================================================================
			//	ゲーム関連
			//==================================================================
			//	ゲームを構成する部分関係
				CKAGETOKI_ADVENT_CHARACTER*	playerChara;			//!< プレイヤーキャラクター
				CADVENT_PLAYER*				player;					//!< プレイヤー
				C3D_OBJECT_CAMERA*			camera;					//!< カメラ
				CGAME_MAP*					gameMap;				//!< ゲームマップ
				EM::CADVENT_ENEMY_MANAGER*	enemyManager;			//!< 敵管理
				CBACK*						back;					//!< 背景
				SSTREAMING*					backSound;				//!< ステージBGM(あれば自動的に再生)
			//	ゲーム演出クラス
				CZTEX_SHA*					shaderZTexture;			//!< Zテクスチャシェーダ
				CDEPTH_SHA*					shaderDepthBuf;			//!< 深度バッファシェーダ
				MAT							lightView;				//!< ライトビュー行列
				MAT							lightProj;				//!< ライト射影行列
				VEC3						lightShift;				//!< 視点からのライトビューの位置
				VEC2						lightNearFar;			//!< ライトの遠視近視
			//	ゲーム自体のステータス関係
				int							iStageNum;				//!< ステージ番号
				SCENE_FLAG					clearFlag;				//!< ゲームクリア時に返すフラグ
				SCENE_FLAG					quitFlag;				//!< ゲーム終了時に返すフラグ
			//	ステージチェンジ関係
				int							iNextStageNum;			//!< 次のステージ番号
				map<int, MY_CUBE>			stageChangeSwitch;		//!< ステージチェンジスイッチマップ
				map<int, MY_CUBE>			stageOutSwitch;			//!< ステージアウトスイッチマップ

			//	デバッグ処理
#ifdef	_DEBUG
				CFONT_BASE*					fontBase;				//!< フォントベース
				void						drawDebug(void);		//!< デバッグ処理
#else
#ifdef	PRESENTATION
				CFONT_BASE*					fontBase;				//!< フォントベース
				void						drawDebug(void);		//!< デバッグ処理
#endif
#endif

			//==================================================================
			//	イベントスクリプト関連
			//==================================================================
				FILE*						eventFile;				//!< イベントファイルポインタ
				EVENT_STEP					eventStep;				//!< イベントステップフラグ
				int							eventStartSwitchNum;	//!< イベント開始スイッチマップの番号
				map<int, MY_CUBE>			eventStartSwitch;		//!< イベント開始スイッチマップ
				map<int, C3D_OBJECT*>		eventObject;			//!< イベントオブジェクトマップ
				vector<CEXECUTER*>			eventExecuter;			//!< イベント動作処理グループ
				CCOMMENT_GROUP*				eventCommentG;			//!< コメントグループ
				int							eventWaitCnt;			//!< 待機フレームカウンタ
				CDUMMY_INPUT*				eventInput;				//!< ダミー入力情報
				MYKEY::TYPENAME				eventWaitKey;			//!< 入力待機のキー

			//==================================================================
			//	再スタート関連
			//==================================================================
				int							continueFrameCnt;		//!< フレームカウンタ
				BOOL						continueChoice;			//!< 選択確認(TRUE=「YES」 NO=「NO」)
				SGAME_CONTINUE_FLAG			continueFlag;			//!< コンティニューフラグ
				C3D_EFFECT*					effContinue;			//!< コンティニューエフェクト
				C3D_EFFECT*					effContinueYes;			//!< コンティニュー「YES」エフェクト
				C3D_EFFECT*					effContinueNo;			//!< コンティニュー「No」エフェクト
				CTEXTURE_BASE*				texInterface;			//!< インターフェーステクスチャ

	private:		//-------------------- private --------------------//

			//==================================================================
			//	上記データの解放処理
			//==================================================================
			//	イベント動作処理の解放
				void	releaseEventExecuter(void)		{
					SAFE_VECTOR_DELETE(eventExecuter, CEXECUTER*);
				}

			//==================================================================
			//	フラグ変更確認
			//==================================================================
				BOOL	flagChange(SGAME_FLAG inFlag)	{
					if(	gameFlag != inFlag)
					{
						gameFlag = inFlag;
						switch(gameFlag)
						{
						case eSGFLAG_PLAY:			gameStep = eSGTSTEP_CHANGE_PLAY;		break;
						case eSGFLAG_EVENT:			gameStep = eSGTSTEP_CHANGE_EVENT;		break;
						case eSGFLAG_CONTINUE:		gameStep = eSGTSTEP_CHANGE_CONTINUE;	break;
						case eSGFLAG_STAGE_CHANGE:	gameStep = eSGTSTEP_CHANGE_STAGE_CHANGE;break;
						case eSGFLAG_RESET:			gameStep = eSGTSTEP_CHANGE_RESET;		break;
						case eSGFLAG_CLEAR:			gameStep = eSGTSTEP_CHANGE_CLEAR;		break;
						case eSGFLAG_QUIT:			gameStep = eSGTSTEP_CHANGE_QUIT;		break;
						case eSGFLAG_ERROR:			gameStep = eSGTSTEP_CHANGE_ERROR;		break;
						}
						return	TRUE;
					}
					else					{						return	FALSE;	}
				}

		//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
		//	Class_SceneAdventureGame.cpp
		//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	private:		//-------------------- private --------------------//

				void	sgInitializeGame(void);						//!< ゲームの開始初期化
				void	sgInitializeScript(void);					//!< スクリプトの開始初期化

				void	sgFinishalizeGame(void);					//!< ゲームの解放
				void	sgFinishalizeScript(void);					//!< スクリプトの解放

	protected:		//-------------------- protected --------------------//

				void	sgCreateStage(int inStageNum);				//!< ゲームステージ生成
				void	sgLoadEventStartSwitch(int inStageNum);		//!< イベントスイッチの読み込み
		virtual	void	sgCreateEnemy(int inStageNum)	{}			//!< 敵生成
				void	sgUpdateLight(void);						//!< ライトの更新

	public:			//-------------------- public --------------------//

				CSCENE_ADVENTURE_GAME(	SCENE_FLAG inFlag,
										SCENE_FLAG inClearFlag,
										SCENE_FLAG inQuitFlag,
										SCENE_SHARE_DATA* inData);	//!< コンストラクタ
		virtual	~CSCENE_ADVENTURE_GAME(void);						//!< デストラクタ

		virtual	void	sceneInitialize(int);						//!< 初期化
		virtual	void	sceneFinishalize(void);						//!< 解放

		virtual	SCENE_FLAG	sceneMain(CINPUT* input);				//!< シーンメイン

		//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
		//	Class_SceneAdventureGamePlay.cpp
		//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	protected:		//-------------------- protected --------------------//

				void	sgPlayEventCheck(VEC3* inLoc);				//!< イベント発生チェック
				void	sgPlayOpenEventFile(int inEventNum);		//!< イベントファイルを開く

		virtual	void	sgPlayChange(void);							//!< ゲームプレイ移行時の初期化
		virtual	void	sgPlayExecute(CINPUT* inInput);				//!< ゲームプレイ中処理
		virtual	void	sgPlayRender(void);							//!< ゲームプレイ中描画

		//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
		//	Class_SceneAdventureGameEvent.cpp
		//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	private:		//-------------------- private --------------------//

				void	sgEventProc(CINPUT* input);					//!< イベント処理(再帰メソッド)

				void	sgEventStepRead(void);						//!< イベントファイルを読む
				void	sgEventStepExecute(void);					//!< イベント実行
				void	sgEventStepTalk(CINPUT* input);				//!< イベント会話中
				void	sgEventStepInputWait(CINPUT* input);		//!< イベント入力待機中

				void	sgEventExecuter(void);						//!< イベント動作処理の実行

				void	sgEventProcTalk(void);						//!< コメント生成
				void	sgEventProcTalkStart(void);					//!< コメント開始
				void	sgEventProcObjCreate(void);					//!< 物体の生成
				void	sgEventProcObjChange(void);					//!< 物体の状態変化
				void	sgEventProcObjMove(void);					//!< 物体移動
				void	sgEventProcObjClear(void);					//!< 物体消滅
				void	sgEventProcEffCreate(void);					//!< エフェクトの生成
				void	sgEventProcEffOccu(void);					//!< エフェクトの発生
				void	sgEventProcEffClear(void);					//!< エフェクトの消滅
				void	sgEventProcInputOpe(void);					//!< ダミー入力の操作
				void	sgEventProcInputWait(void);					//!< 入力待機移行
				void	sgEventProcExecute(void);					//!< 待機移行
				void	sgEventProcGameClear(void);					//!< ゲームクリア移行
				void	sgEventProcEventQuit(void);					//!< イベント終了

	protected:		//-------------------- protected --------------------//

		virtual	void	sgEventChange(void);						//!< ゲームイベント移行時の初期化
		virtual	void	sgEventExecute(CINPUT* inInput);			//!< ゲームイベント中処理
		virtual	void	sgEventRender(void);						//!< ゲームイベント中描画

		//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
		//	Class_SceneAdventureGameContinue.cpp
		//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	protected:		//-------------------- protected --------------------//

		virtual	void	sgContinueChange(void);						//!< コンティニュー移行時の初期化
		virtual	void	sgContinueExecute(CINPUT* inInput);			//!< コンティニュー中処理
		virtual	void	sgContinueRender(void);						//!< コンティニュー中描画

		//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
		//	Class_SceneAdventureGameStageChange.cpp
		//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	protected:		//-------------------- protected --------------------//

		virtual	void	sgStageChange(int inNewStage);				//!< ステージチェンジ移行時の初期化
		virtual	void	sgStageChangeExecute(CINPUT* inInput);		//!< ステージチェンジ中処理
		virtual	void	sgStageChangeRender(void);					//!< ステージチェンジ中描画

		//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
		//	Class_SceneAdventureGameStageOut.cpp
		//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	protected:		//-------------------- protected --------------------//

		virtual	void	sgStageOutChange(void);						//!< ステージアウト移行時の初期化
		virtual	void	sgStageOutExecute(CINPUT* inInput);			//!< ステージアウト中処理
		virtual	void	sgStageOutRender(void);						//!< ステージアウト中描画

		//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
		//	Class_SceneAdventureGameReset.cpp
		//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	protected:		//-------------------- protected --------------------//

		virtual	void	sgResetChange(void);						//!< 再スタート移行時の初期化
		virtual	void	sgResetExecute(CINPUT* inInput);			//!< 再スタート中処理
		virtual	void	sgResetRender(void);						//!< 再スタート中描画

		//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
		//	Class_SceneAdventureGameClear.cpp
		//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	protected:		//-------------------- protected --------------------//

		virtual	void	sgClearChange(void);						//!< ゲームクリア移行時の初期化
		virtual	void	sgClearExecute(CINPUT* inInput);			//!< ゲームクリア中処理
		virtual	void	sgClearRender(void);						//!< ゲームクリア中描画

		//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
		//	Class_SceneAdventureGameQuit.cpp
		//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	protected:		//-------------------- protected --------------------//

		virtual	void	sgQuitChange(void);							//!< ゲーム終了移行時の初期化
		virtual	void	sgQuitExecute(CINPUT* inInput);				//!< ゲーム終了中処理
		virtual	void	sgQuitRender(void);							//!< ゲーム終了中描画
	};
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------