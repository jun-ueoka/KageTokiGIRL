//***********************************************************************************
//!	ファイル名		Class_SceneStage9.cpp
//
//	機能			ゲームシーンクラス、ステージ9「城内部」を実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/01/27	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneStage9.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_STAGE9::CSCENE_STAGE9
	//
	//	機能		コンストラクタ
	//	引数		inData		シーン共有データ
	//	更新		2009/01/27	<新規>
	//================================================================================
	CSCENE_STAGE9::CSCENE_STAGE9(SCENE_SHARE_DATA* inData)	:
	//!	ゲームシーン
	CSCENE_ADVENTURE_GAME(eSCENE_CHAPTER3_STAGE9, eSCENE_LAST_CHAPTER_MOVIE10,
						  eSCENE_GAMEOVER, inData)
	{
	}

	//================================================================================
	//!	メソッド名	CSCENE_STAGE9::~CSCENE_STAGE9
	//
	//	機能		デストラクタ
	//	更新		2009/01/27	<新規>
	//================================================================================
	CSCENE_STAGE9::~CSCENE_STAGE9(void)
	{
	//	解放
		sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_STAGE9::scenecfdInitialize
	//
	//	機能		初期化
	//	引数		inStageNum	外部データ番号
	//	更新		2009/01/27	<新規>
	//================================================================================
	void
	CSCENE_STAGE9::sceneInitialize(int inStageNum)
	{
	//	デモ用のステージ番号を取得
		inStageNum	= 90;

	//	ゲームシーンの初期化
		CSCENE_ADVENTURE_GAME::sceneInitialize(inStageNum);
	}

	//================================================================================
	//!	メソッド名	CSCENE_STAGE9::sceneFinishalize
	//
	//	機能		解放
	//	更新		2009/01/27	<新規>
	//================================================================================
	void
	CSCENE_STAGE9::sceneFinishalize(void)
	{
	//	ゲームシーンの開放
		CSCENE_ADVENTURE_GAME::sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_STAGE9::sceneMain
	//
	//	機能		シーンメイン
	//	機能概要	ゲームシーンのメソッドを使い、デモを形成
	//	更新		2009/01/27	<新規>
	//================================================================================
	SCENE_FLAG
	CSCENE_STAGE9::sceneMain(CINPUT* input)
	{
#ifdef	_DEBUG
		if(	input->checkPress(MYKEY::eF3))
		{
			if(	input->checkPress(MYKEY::eF5))
			{
				if(	input->checkPress(MYKEY::eUP))		lightNearFar.x	+= 50.0f;
				if(	input->checkPress(MYKEY::eDOWN))	lightNearFar.x	-= 50.0f;
			}
			if(	input->checkPress(MYKEY::eF6))
			{
				if(	input->checkPress(MYKEY::eUP))		lightNearFar.y	+= 50.0f;
				if(	input->checkPress(MYKEY::eDOWN))	lightNearFar.y	-= 50.0f;
			}
		}
#endif

	//	ゲームフラグで処理分岐
		switch(gameFlag)
		{
		//	ゲームプレイ中
			case	eSGFLAG_PLAY:
				sgPlayExecute(input);
				updateExecute();
				sgPlayRender();
				break;
		//	ゲームイベント中
			case	eSGFLAG_EVENT:
				sgEventExecute(input);
				updateExecute();
				sgEventRender();
				break;
		//	コンティニュー中
			case	eSGFLAG_CONTINUE:
				sgContinueExecute(input);
				updateExecute();
				sgContinueRender();
				break;
		//	マップチェンジ中
			case	eSGFLAG_STAGE_CHANGE:
				sgStageChangeExecute(input);
				updateExecute();
				sgStageChangeRender();
				break;
		//	再スタート中
			case	eSGFLAG_RESET:
				this->sgResetExecute(input);
				this->updateExecute();
				this->sgResetRender();
				break;
		//	ゲームクリア中
			case	eSGFLAG_CLEAR:
				sgClearExecute(input);
				updateExecute();
				sgClearRender();
				break;
		//	ゲーム終了中
			case	eSGFLAG_QUIT:
				sgQuitExecute(input);
				updateExecute();
				sgQuitRender();
				break;
		//	ゲーム中エラー
			case	eSGFLAG_ERROR:
				returnFlag	= eSCENE_ERROR;
				break;
		//	上記以外
			default:
				break;
		}
		return	returnFlag;
	}

	//================================================================================
	//!	メソッド名	CSCENE_STAGE9::updateExecute
	//
	//	機能		更新
	//	更新		2009/01/27	<新規>
	//================================================================================
	void
	CSCENE_STAGE9::updateExecute(void)
	{
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------