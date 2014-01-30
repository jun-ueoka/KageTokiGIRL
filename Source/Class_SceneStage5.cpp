//***********************************************************************************
//!	ファイル名		Class_SceneStage5.cpp
//
//	機能			ゲームシーンクラス、ステージ5「山」を実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/01/27	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneStage5.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_STAGE5::CSCENE_STAGE5
	//
	//	機能		コンストラクタ
	//	引数		inData		シーン共有データ
	//	更新		2009/01/27	<新規>
	//================================================================================
	CSCENE_STAGE5::CSCENE_STAGE5(SCENE_SHARE_DATA* inData)	:
	//!	ゲームシーン
	CSCENE_ADVENTURE_GAME(eSCENE_CHAPTER2_STAGE5, eSCENE_CHAPTER2_MOVIE6,
						  eSCENE_GAMEOVER, inData)
	{
	}

	//================================================================================
	//!	メソッド名	CSCENE_STAGE5::~CSCENE_STAGE5
	//
	//	機能		デストラクタ
	//	更新		2009/01/27	<新規>
	//================================================================================
	CSCENE_STAGE5::~CSCENE_STAGE5(void)
	{
	//	解放
		this->sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_STAGE5::scenecfdInitialize
	//
	//	機能		初期化
	//	引数		inStageNum	外部データ番号
	//	更新		2009/01/27	<新規>
	//================================================================================
	void
	CSCENE_STAGE5::sceneInitialize(int inStageNum)
	{
	//	デモ用のステージ番号を取得
		inStageNum	= 50;

	//	ゲームシーンの初期化
		CSCENE_ADVENTURE_GAME::sceneInitialize(inStageNum);
	}

	//================================================================================
	//!	メソッド名	CSCENE_STAGE5::sceneFinishalize
	//
	//	機能		解放
	//	更新		2009/01/27	<新規>
	//================================================================================
	void
	CSCENE_STAGE5::sceneFinishalize(void)
	{
	//	ゲームシーンの開放
		CSCENE_ADVENTURE_GAME::sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_STAGE5::sceneMain
	//
	//	機能		シーンメイン
	//	機能概要	ゲームシーンのメソッドを使い、デモを形成
	//	更新		2009/01/27	<新規>
	//================================================================================
	SCENE_FLAG
	CSCENE_STAGE5::sceneMain(CINPUT* input)
	{
#ifdef	_DEBUG
		if(	input->checkPress(MYKEY::eF3))
		{
			if(	input->checkPress(MYKEY::eF5))
			{
				if(	input->checkPress(MYKEY::eUP))		this->lightNearFar.x	+= 50.0f;
				if(	input->checkPress(MYKEY::eDOWN))	this->lightNearFar.x	-= 50.0f;
			}
			if(	input->checkPress(MYKEY::eF6))
			{
				if(	input->checkPress(MYKEY::eUP))		this->lightNearFar.y	+= 50.0f;
				if(	input->checkPress(MYKEY::eDOWN))	this->lightNearFar.y	-= 50.0f;
			}
		}
#endif

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
	//!	メソッド名	CSCENE_STAGE5::updateExecute
	//
	//	機能		更新
	//	更新		2009/01/27	<新規>
	//================================================================================
	void
	CSCENE_STAGE5::updateExecute(void)
	{
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------