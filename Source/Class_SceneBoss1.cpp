//***********************************************************************************
//!	ファイル名		Class_SceneBoss1.cpp
//
//	機能			ゲームシーンクラス、ボスステージ1「エクス」を実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/01/12	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneBoss1.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_BOSS1::CSCENE_BOSS1
	//
	//	機能		コンストラクタ
	//	引数		inData		シーン共有データ
	//	更新		2009/01/12	<新規>
	//================================================================================
	CSCENE_BOSS1::CSCENE_BOSS1(SCENE_SHARE_DATA* inData)	:
	//!	ゲームシーン
	CSCENE_ADVENTURE_GAME(eSCENE_CHAPTER1_BOSS1,
						  eSCENE_ENDING,
						  eSCENE_GAMEOVER, inData)
	{
		this->eventStepCnt	= 0;
		this->bossExe		= NULL;
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS1::~CSCENE_BOSS1
	//
	//	機能		デストラクタ
	//	更新		2009/01/12	<新規>
	//================================================================================
	CSCENE_BOSS1::~CSCENE_BOSS1(void)
	{
	//	解放
		this->sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS1::scenecfdInitialize
	//
	//	機能		初期化
	//	引数		inStageNum	外部データ番号
	//	更新		2009/01/12	<新規>
	//================================================================================
	void
	CSCENE_BOSS1::sceneInitialize(int inStageNum)
	{
	//	デモ用のステージ番号を取得
		inStageNum	= 110;

	//	ゲームシーンの初期化
		CSCENE_ADVENTURE_GAME::sceneInitialize(inStageNum);
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS1::sceneFinishalize
	//
	//	機能		解放
	//	更新		2009/01/12	<新規>
	//================================================================================
	void
	CSCENE_BOSS1::sceneFinishalize(void)
	{
	//	解放
		SAFE_DELETE(this->bossExe);

	//	ゲームシーンの開放
		CSCENE_ADVENTURE_GAME::sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS1::sceneMain
	//
	//	機能		シーンメイン
	//	機能概要	ゲームシーンのメソッドを使い、デモを形成
	//	更新		2009/01/12	<新規>
	//================================================================================
	SCENE_FLAG
	CSCENE_BOSS1::sceneMain(CINPUT* input)
	{
	//	ゲームフラグで処理分岐
		switch(this->gameFlag)
		{
		//	ゲームプレイ中
			case	eSGFLAG_PLAY:
				this->sgPlayExecute(input);
				this->updateExecute();
				this->sgPlayRender();
				break;
		//	ゲームイベント中
			case	eSGFLAG_EVENT:
				this->sgEventExecute(input);
				this->updateExecute();
				this->sgEventRender();
				break;
		//	コンティニュー中
			case	eSGFLAG_CONTINUE:
				this->sgContinueExecute(input);
				this->updateExecute();
				this->sgContinueRender();
				break;
		//	マップチェンジ中
			case	eSGFLAG_STAGE_CHANGE:
				this->sgStageChangeExecute(input);
				this->updateExecute();
				this->sgStageChangeRender();
				break;
		//	再スタート中
			case	eSGFLAG_RESET:
				this->sgResetExecute(input);
				this->updateExecute();
				this->sgResetRender();
				break;
		//	ゲームクリア中
			case	eSGFLAG_CLEAR:
				this->sgClearExecute(input);
				this->updateExecute();
				this->sgClearRender();
				break;
		//	ゲーム終了中
			case	eSGFLAG_QUIT:
				this->sgQuitExecute(input);
				this->updateExecute();
				this->sgQuitRender();
				break;
		//	ゲーム中エラー
			case	eSGFLAG_ERROR:
				this->returnFlag	= eSCENE_ERROR;
				break;
		//	上記以外
			default:
				break;
		}
		return	this->returnFlag;
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS1::updateLight
	//
	//	機能		ライトの更新
	//	更新		2009/01/12	<新規>
	//================================================================================
	void
	CSCENE_BOSS1::updateExecute(void)
	{
	//	ゲームフラグで追加処理分岐
		switch(this->gameFlag)
		{
		//	ゲームプレイ中
			case	eSGFLAG_PLAY:
			//	イベントステップカウンタが0以下の時は通常動作
				if(	this->eventStepCnt <= 0)
				{
				//	ボスの動作
					this->bossExe->operation(NULL);
				//	ボスの体力がなければ、クリア
					if(	this->bossExe->getLife() <= 0)
					{
					//	1002番目のイベントを発生
						this->sgPlayOpenEventFile(1002);
						this->eventStepCnt	= 1002;
					}
				}
			//	イベントステップカウンタが1002の状態の時
				else
				if(	this->eventStepCnt == 1002)
				{
				//	ボスの動作
					this->bossExe->operation(NULL);
				//	1003番目のイベントを発生
					this->sgPlayOpenEventFile(1003);
					this->eventStepCnt	= 1003;
					this->bossExe->changeAction(eBEXEA_DISAPPEAR);
				}
			//	イベントステップカウンタが1003の状態の時
				else
				if(	this->eventStepCnt == 1003)
				{
				//	ボスの動作
					this->bossExe->operation(NULL);
				//	0番目のイベントを発生
					this->sgPlayOpenEventFile(0);
					this->eventStepCnt	= -1;
				}
				break;
		//	ゲームイベント中
			case	eSGFLAG_EVENT:
			//	ボスの動作
				this->bossExe->operation(NULL);
				break;
		}

	//	ゲームステップフラグで追加処理分岐
		switch(this->gameStep)
		{
		//	ゲームプレイ中に移行された時
			case	eSGTSTEP_CHANGE_PLAY:
				this->bossExe->changeAction(eBEXEA_WAIT);
				this->bossExe->setDrawInterface(TRUE);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		//	ゲームイベント中に移行された時
			case	eSGTSTEP_CHANGE_EVENT:
				this->bossExe->setDrawInterface(FALSE);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		//	コンティニュー中に移行された時
			case	eSGTSTEP_CHANGE_CONTINUE:
				this->bossExe->setDrawInterface(FALSE);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		//	再スタート中に移行された時
			case	eSGTSTEP_CHANGE_RESET:
				this->sgLoadEventStartSwitch(this->eventStartSwitchNum);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS1::sgCreateEnemy
	//
	//	機能		敵生成
	//	機能概要	敵を生成し、マップに登録する
	//	引数		inStageNum		ステージ番号
	//	更新		2009/01/12		<新規>
	//================================================================================
	void
	CSCENE_BOSS1::sgCreateEnemy(int inStageNum)
	{
	//	エクスを生成する
		if(	!this->bossExe)
		{
			this->bossExe	= new CBOSS_EXE(this->gDev->getDevice());
		}

	//	エクスが生成されていたら、位置等を初期化
		if(	this->bossExe)
		{
			this->bossExe->setLoc(&VEC3(1500.0f, 4000.0f, 0.0f));
			this->bossExe->changeAction(eBEXEA_APPEAR);
			this->bossExe->setViewG(this->gameMap->getViewG());
			this->bossExe->setStageG(this->gameMap->getHitG());
			this->bossExe->setEnemyG(this->gameMap->getEnemyG());
			this->bossExe->setTargetPlayer(&this->playerChara);
			this->bossExe->setVisible(TRUE);
			this->bossExe->resetParameter();
		}

	//	エクスを敵グループに登録
		this->gameMap->addEnemyG(bossExe);
		this->gameMap->setEnemyGDistance(20000.0f);
		this->camera->setSecondTarget(bossExe);
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------