//***********************************************************************************
//!	ファイル名		Class_SceneBoss2.cpp
//
//	機能			ゲームシーンクラス、ボスステージ2「ヤマカゲ」を実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/02/15	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneBoss2.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_BOSS2::CSCENE_BOSS2
	//
	//	機能		コンストラクタ
	//	引数		inData		シーン共有データ
	//	更新		2009/02/15	<新規>
	//================================================================================
	CSCENE_BOSS2::CSCENE_BOSS2(SCENE_SHARE_DATA* inData)	:
	//!	ゲームシーン
	CSCENE_ADVENTURE_GAME(eSCENE_CHAPTER2_BOSS2,
						  eSCENE_CHAPTER3_MOVIE7,
						  eSCENE_GAMEOVER, inData)
	{
		this->eventStepCnt	= 0;
		this->bossYamakage	= NULL;
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS2::~CSCENE_BOSS2
	//
	//	機能		デストラクタ
	//	更新		2009/02/15	<新規>
	//================================================================================
	CSCENE_BOSS2::~CSCENE_BOSS2(void)
	{
	//	解放
		this->sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS2::scenecfdInitialize
	//
	//	機能		初期化
	//	引数		inStageNum	外部データ番号
	//	更新		2009/02/15	<新規>
	//================================================================================
	void
	CSCENE_BOSS2::sceneInitialize(int inStageNum)
	{
	//	デモ用のステージ番号を取得
		inStageNum	= 120;

	//	ゲームシーンの初期化
		CSCENE_ADVENTURE_GAME::sceneInitialize(inStageNum);
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS2::sceneFinishalize
	//
	//	機能		解放
	//	更新		2009/02/15	<新規>
	//================================================================================
	void
	CSCENE_BOSS2::sceneFinishalize(void)
	{
	//	解放
		SAFE_DELETE(this->bossYamakage);

	//	ゲームシーンの開放
		CSCENE_ADVENTURE_GAME::sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS2::sceneMain
	//
	//	機能		シーンメイン
	//	機能概要	ゲームシーンのメソッドを使い、デモを形成
	//	更新		2009/02/15	<新規>
	//================================================================================
	SCENE_FLAG
	CSCENE_BOSS2::sceneMain(CINPUT* input)
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
				this->sgPlayRender();
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
	//!	メソッド名	CSCENE_BOSS2::updateLight
	//
	//	機能		ライトの更新
	//	更新		2009/02/15	<新規>
	//================================================================================
	void
	CSCENE_BOSS2::updateExecute(void)
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
					this->bossYamakage->operation(NULL);
				//	ボスの体力がなければ、クリア
					if(	this->bossYamakage->getLife() <= 0)
					{
					//	2002番目のイベントを発生
						this->sgPlayOpenEventFile(2002);
						this->eventStepCnt	= 2002;
					}
				}
			//	イベントステップカウンタが2002の状態の時
				else
				if(	this->eventStepCnt == 2002)
				{
				//	ボスの動作
					this->bossYamakage->operation(NULL);
				//	2003番目のイベントを発生
					this->sgPlayOpenEventFile(2003);
					this->eventStepCnt	= 2003;
					this->bossYamakage->changeAction(eBYAMAKAGEA_DISAPPEAR);
				}
			//	イベントステップカウンタが2003の状態の時
				else
				if(	this->eventStepCnt == 2003)
				{
				//	ボスの動作
					this->bossYamakage->operation(NULL);
				//	0番目のイベントを発生
					this->sgPlayOpenEventFile(0);
					this->eventStepCnt	= -1;
				}
				break;
		//	ゲームイベント中
			case	eSGFLAG_EVENT:
			//	ボスの動作
				this->bossYamakage->operation(NULL);
				break;
		}

	//	ゲームステップフラグで追加処理分岐
		switch(this->gameStep)
		{
		//	ゲームプレイ中に移行された時
			case	eSGTSTEP_CHANGE_PLAY:
				this->bossYamakage->changeAction(eBYAMAKAGEA_WAIT);
				this->bossYamakage->setDrawInterface(TRUE);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		//	ゲームイベント中に移行された時
			case	eSGTSTEP_CHANGE_EVENT:
				this->bossYamakage->setDrawInterface(FALSE);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		//	コンティニュー中に移行された時
			case	eSGTSTEP_CHANGE_CONTINUE:
				this->bossYamakage->setDrawInterface(FALSE);
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
	//!	メソッド名	CSCENE_BOSS2::sgCreateEnemy
	//
	//	機能		敵生成
	//	機能概要	敵を生成し、マップに登録する
	//	引数		inStageNum		ステージ番号
	//	更新		2009/02/15	<新規>
	//================================================================================
	void
	CSCENE_BOSS2::sgCreateEnemy(int inStageNum)
	{
	//	ヤマカゲを生成する
		if(	!this->bossYamakage)
		{
			this->bossYamakage	= new CBOSS_YAMAKAGE(this->gDev->getDevice());
		}

	//	ヤマカゲが生成されていたら、位置等を初期化
		if(	this->bossYamakage)
		{
			this->bossYamakage->setLoc(&VEC3(1500.0f, 4000.0f, 0.0f));
			this->bossYamakage->changeAction(eBYAMAKAGEA_APPEAR);
			this->bossYamakage->setViewG(this->gameMap->getViewG());
			this->bossYamakage->setStageG(this->gameMap->getHitG());
			this->bossYamakage->setEnemyG(this->gameMap->getEnemyG());
			this->bossYamakage->setTargetPlayer(&this->playerChara);
			this->bossYamakage->setVisible(TRUE);
			this->bossYamakage->resetParameter();
		}

	//	エクスを敵グループに登録
		this->gameMap->addEnemyG(bossYamakage);
		this->gameMap->setEnemyGDistance(20000.0f);
		this->camera->setSecondTarget(bossYamakage);
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------