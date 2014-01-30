//***********************************************************************************
//!	ファイル名		Class_SceneBoss5.cpp
//
//	機能			ゲームシーンクラス、ボスステージ5「クライム」を実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/02/15	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneBoss5.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_BOSS5::CSCENE_BOSS5
	//
	//	機能		コンストラクタ
	//	引数		inData		シーン共有データ
	//	更新		2009/02/15	<新規>
	//================================================================================
	CSCENE_BOSS5::CSCENE_BOSS5(SCENE_SHARE_DATA* inData)	:
	//!	ゲームシーン
	CSCENE_ADVENTURE_GAME(eSCENE_LAST_CHAPTER_LAST_BOSS, eSCENE_LAST_CHAPTER_ENDING_MOVIE,
						  eSCENE_GAMEOVER, inData)
	{
		this->eventStepCnt	= 0;
		this->bossExe		= NULL;
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS5::~CSCENE_BOSS5
	//
	//	機能		デストラクタ
	//	更新		2009/02/15	<新規>
	//================================================================================
	CSCENE_BOSS5::~CSCENE_BOSS5(void)
	{
	//	解放
		this->sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS5::scenecfdInitialize
	//
	//	機能		初期化
	//	引数		inStageNum	外部データ番号
	//	更新		2009/02/15	<新規>
	//================================================================================
	void
	CSCENE_BOSS5::sceneInitialize(int inStageNum)
	{
	//	デモ用のステージ番号を取得
		inStageNum	= 150;

	//	ゲームシーンの初期化
		CSCENE_ADVENTURE_GAME::sceneInitialize(inStageNum);
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS5::sceneFinishalize
	//
	//	機能		解放
	//	更新		2009/02/15	<新規>
	//================================================================================
	void
	CSCENE_BOSS5::sceneFinishalize(void)
	{
	//	解放
		SAFE_DELETE(this->bossExe);

	//	ゲームシーンの開放
		CSCENE_ADVENTURE_GAME::sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS5::sceneMain
	//
	//	機能		シーンメイン
	//	機能概要	ゲームシーンのメソッドを使い、デモを形成
	//	更新		2009/02/15	<新規>
	//================================================================================
	SCENE_FLAG
	CSCENE_BOSS5::sceneMain(CINPUT* input)
	{
	//	ゲームフラグで処理分岐
		switch(this->gameFlag)
		{
		//	ゲームプレイ中
			case	eSGFLAG_PLAY:
			//	イベントステップカウンタが0以下の時は通常動作
				if(	this->eventStepCnt <= 0)
				{
				//	ボスの動作
					this->bossExe->operation(NULL);
					this->sgPlayExecute(input);
					this->updateExecute();
					this->sgPlayRender();
				//	ボスの体力がなければ、クリア
					if(	this->bossExe->getLife() <= 0)
					{
					//	1002番目のイベントを発生
						this->sgPlayOpenEventFile(1002);
						this->eventStepCnt	= 1002;
						this->bossExe->changeAction(eBEXEA_APPEAR);
					}
				}
				else
			//	イベントステップカウンタが1002の状態でここまで来たら
				if(	this->eventStepCnt == 1002)
				{
				//	ボスを上へ飛ばす
					this->bossExe->calcMoveQuantity(10.0f);
					this->bossExe->moveUp();
					this->sgPlayExecute(input);
					this->updateExecute();
					this->sgPlayRender();
				//	ボスが飛び切ったら、ゴール
					if(	this->bossExe->getLocY() > 7000)
					{
					//	0番のイベント発生
						this->sgPlayOpenEventFile(0);
						this->bossExe->setVisible(FALSE);
					}
				}
				break;
		//	ゲームイベント中
			case	eSGFLAG_EVENT:
			//	ボスの動作
				this->bossExe->operation(NULL);
				this->sgEventExecute(input);
				this->updateExecute();
				this->sgEventRender();
			//	上記の処理でゲームプレイ中に変わったら、
				if(	this->gameFlag == eSGFLAG_PLAY)
				{
					this->bossExe->changeAction(eBEXEA_WAIT);
				}
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
	//!	メソッド名	CSCENE_BOSS5::updateLight
	//
	//	機能		ライトの更新
	//	更新		2009/02/15	<新規>
	//================================================================================
	void
	CSCENE_BOSS5::updateExecute(void)
	{
	}

	//================================================================================
	//!	メソッド名	CSCENE_BOSS5::sgCreateEnemy
	//
	//	機能		敵生成
	//	機能概要	敵を生成し、マップに登録する
	//	引数		inStageNum		ステージ番号
	//	更新		2009/02/15		<新規>
	//================================================================================
	void
	CSCENE_BOSS5::sgCreateEnemy(int inStageNum)
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
			this->bossExe->setViewG(this->gameMap->getViewG());
			this->bossExe->setStageG(this->gameMap->getHitG());
			this->bossExe->setEnemyG(this->gameMap->getEnemyG());
			this->bossExe->setTargetPlayer(&this->playerChara);
		}

	//	エクスを敵グループに登録
		this->gameMap->addEnemyG(bossExe);
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------