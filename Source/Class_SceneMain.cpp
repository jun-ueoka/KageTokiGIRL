//***********************************************************************************
//!	ファイル名		Class_SceneMain.cpp
//
//	機能			メインのシーンを実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2008/07/10	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneMain.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_MAIN::CSCENE_MAIN
	//
	//	機能		コンストラクタ
	//	更新		2008/07/10		<新規>
	//================================================================================
	CSCENE_MAIN::CSCENE_MAIN(void)	:
	CSCENE(eGAME_MAIN),		//!< 基礎シーンフラグ
	CSCENE_GAME()			//!< ゲームシーン
	{
	//	初期化
	}

	//================================================================================
	//!	メソッド名	CSCENE_MAIN::~CSCENE_MAIN
	//
	//	機能		デストラクタ
	//	更新		2008/07/10		<新規>
	//================================================================================
	CSCENE_MAIN::~CSCENE_MAIN(void)
	{
	//	解放
		this->finishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_MAIN::create
	//
	//	機能		生成
	//	引数		inNum			番号
	//	更新		2008/07/10		<新規>
	//================================================================================
	void
	CSCENE_MAIN::create(int inStageNum)
	{
		this->sgCreateStage(inStageNum);
	}

	//================================================================================
	//!	メソッド名	CSCENE_MAIN::motion
	//
	//	機能		操作、動作
	//	更新		2008/07/12		<新規>
	//================================================================================
	void
	CSCENE_MAIN::motion(CINPUT* input)
	{
	//	ゲーム処理
		this->sgExecuteGame(input);
	}

	//================================================================================
	//!	メソッド名	CSCENE_MAIN::render
	//
	//	機能		描画
	//	更新		2008/07/12		<新規>
	//================================================================================
	void
	CSCENE_MAIN::render(void)
	{
	//	ゲーム描画
		switch(this->sgRenderGame())
		{
		//	ゲームクリア
			case	eSGSTATE_GAMECLEAR:
				break;
		//	ゲームオーバー
			case	eSGSTATE_GAMEOVER:
				break;
		//	ゲーム終了
			case	eSGSTATE_GAMEEND:
				break;
		//	ゲームエラー
			case	eSGSTATE_ERROR:
				break;
		}//	default	break
	}

	//================================================================================
	//!	メソッド名	CSCENE_MAIN::initialize
	//
	//	機能		初期化
	//	引数		inNum			外部データ番号
	//	戻り値		TRUE			読み込み成功
	//				FALSE			読み込み失敗
	//	更新		2008/07/10		<新規>
	//================================================================================
	void
	CSCENE_MAIN::initialize(int inStageNum)
	{
	//	初期解放
		this->finishalize();

	//	初期化処理
		this->create(inStageNum);
	}

	//================================================================================
	//!	メソッド名	CSCENE_MAIN::finishalize
	//
	//	機能		解放
	//	更新		2008/07/10		<新規>
	//================================================================================
	void
	CSCENE_MAIN::finishalize(void)
	{
	//	解放処理
	}

	//================================================================================
	//!	メソッド名	CSCENE_MAIN::main
	//
	//	機能		シーンメイン
	//	更新		2008/07/10		<新規>
	//================================================================================
	HRESULT
	CSCENE_MAIN::main(CINPUT* input)
	{
	#ifdef	_DEBUG
		if(	!input->checkPress(MYKEY::eL1))
	#endif
		{
			this->motion(input);
			this->render();
		}
		return	this->hrReturnFlag;
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------