//***********************************************************************************
//!	ファイル名		Class_SceneBoss3.cpp
//
//	機能			ゲームシーンクラス、ボスステージ1「エクス」を実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/02/15	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneBoss3.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_BOSS3::CSCENE_BOSS3
	//
	//	機能		コンストラクタ
	//	引数		inData		シーン共有データ
	//	更新		2009/02/15	<新規>
	//================================================================================
	CSCENE_BOSS3::CSCENE_BOSS3(SCENE_SHARE_DATA* inData)	:
	//!	ゲームシーン
	CSCENE_ADVENTURE_GAME(eSCENE_CHAPTER3_BOSS3,
						  eSCENE_CHAPTER3_MOVIE8,
						  eSCENE_GAMEOVER, inData)
	{
		this->eventStepCnt	= 0;
		this->bossFeather	= NULL;
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS3::~CSCENE_BOSS3
	//
	//	機能		デストラクタ
	//	更新		2009/02/15	<新規>
	//================================================================================
	CSCENE_BOSS3::~CSCENE_BOSS3(void)
	{
	//	解放
		this->sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS3::scenecfdInitialize
	//
	//	機能		初期化
	//	引数		inStageNum	外部データ番号
	//	更新		2009/02/15	<新規>
	//================================================================================
	void
	CSCENE_BOSS3::sceneInitialize(int inStageNum)
	{
	//	デモ用のステージ番号を取得
		inStageNum	= 130;

	//	ゲームシーンの初期化
		CSCENE_ADVENTURE_GAME::sceneInitialize(inStageNum);
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS3::sceneFinishalize
	//
	//	機能		解放
	//	更新		2009/02/15	<新規>
	//================================================================================
	void
	CSCENE_BOSS3::sceneFinishalize(void)
	{
	//	解放
		SAFE_DELETE(this->bossFeather);

	//	ゲームシーンの開放
		CSCENE_ADVENTURE_GAME::sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS3::sceneMain
	//
	//	機能		シーンメイン
	//	機能概要	ゲームシーンのメソッドを使い、デモを形成
	//	更新		2009/02/15	<新規>
	//================================================================================
	SCENE_FLAG
	CSCENE_BOSS3::sceneMain(CINPUT* input)
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
	//!	メソッド名	CSCENE_BOSS3::updateLight
	//
	//	機能		ライトの更新
	//	更新		2009/02/15	<新規>
	//================================================================================
	void
	CSCENE_BOSS3::updateExecute(void)
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
					this->bossFeather->operation(NULL);
				//	ボスの体力がなければ、クリア
					if(	this->bossFeather->getLife() <= 0)
					{
					//	3002番目のイベントを発生
						this->sgPlayOpenEventFile(3002);
						this->eventStepCnt	= 3002;
					}
				}
			//	イベントステップカウンタが3002の状態の時
				else
				if(	this->eventStepCnt == 3002)
				{
				//	ボスの動作
					this->bossFeather->operation(NULL);
				//	3003番目のイベントを発生
					this->sgPlayOpenEventFile(3003);
					this->eventStepCnt	= 3003;
					this->bossFeather->changeAction(eBFEATHERA_DISAPPEAR);
				}
			//	イベントステップカウンタが3003の状態の時
				else
				if(	this->eventStepCnt == 3003)
				{
				//	ボスの動作
					this->bossFeather->operation(NULL);
				//	0番目のイベントを発生
					this->sgPlayOpenEventFile(0);
					this->eventStepCnt	= -1;
				}
				break;
		//	ゲームイベント中
			case	eSGFLAG_EVENT:
			//	ボスの動作
				this->bossFeather->operation(NULL);
				break;
		}

	//	ゲームステップフラグで追加処理分岐
		switch(this->gameStep)
		{
		//	ゲームプレイ中に移行された時
			case	eSGTSTEP_CHANGE_PLAY:
				this->bossFeather->changeAction(eBFEATHERA_WAIT);
				this->bossFeather->setDrawInterface(TRUE);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		//	ゲームイベント中に移行された時
			case	eSGTSTEP_CHANGE_EVENT:
				this->bossFeather->setDrawInterface(FALSE);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		//	コンティニュー中に移行された時
			case	eSGTSTEP_CHANGE_CONTINUE:
				this->bossFeather->setDrawInterface(FALSE);
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
	//!	メソッド名	CSCENE_BOSS3::sgCreateEnemy
	//
	//	機能		敵生成
	//	機能概要	敵を生成し、マップに登録する
	//	引数		inStageNum		ステージ番号
	//	更新		2009/01/12		<新規>
	//================================================================================
	void
	CSCENE_BOSS3::sgCreateEnemy(int inStageNum)
	{
	//	エクスを生成する
		if(	!this->bossFeather)
		{
			this->bossFeather	= new CBOSS_FEATHER(this->gDev->getDevice());
		}
		
	//	エクスが生成されていたら、位置等を初期化
		if(	this->bossFeather)
		{
			this->bossFeather->setLoc(&VEC3(1500.0f, 4000.0f, 0.0f));
			this->bossFeather->setViewG(this->gameMap->getViewG());
			this->bossFeather->setStageG(this->gameMap->getHitG());
			this->bossFeather->setEnemyG(this->gameMap->getEnemyG());
			this->bossFeather->setTargetPlayer(&this->playerChara);
			this->bossFeather->setVisible(TRUE);
			this->bossFeather->resetParameter();
		}

	//	エクスを敵グループに登録
		this->gameMap->addEnemyG(bossFeather);
		this->gameMap->setEnemyGDistance(20000.0f);
		this->camera->setSecondTarget(bossFeather);
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------