//***********************************************************************************
//!	ファイル名		Class_SceneGameOver.cpp
//
//	機能			ゲームオーバーシーンの実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/02/05	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneGameOver.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_GAMEOVER::CSCENE_GAMEOVER
	//
	//	機能		コンストラクタ
	//	引数		inData			シーン共有データ
	//	更新		2009/02/05		<新規>
	//================================================================================
	CSCENE_GAMEOVER::CSCENE_GAMEOVER(SCENE_SHARE_DATA* inData)	:
	CSCENE(eSCENE_GAMEOVER, eSCENE_LOGO, inData)			//!< 基礎シーンフラグの設定
	{
	//	環境属性の初期化
		gDev			= CGRAPHIC_DEVICE::getInstance();
		font			= CFONT_BASE::getInstance();

	//	演出属性の初期化
		sStreamingBgm	= NULL;
		texGameOver		= NULL;
		iFrameCnt		= 0;
	}

	//================================================================================
	//!	メソッド名	CSCENE_GAMEOVER::~CSCENE_GAMEOVER
	//
	//	機能		デストラクタ
	//	更新		2009/02/05		<新規>
	//================================================================================
	CSCENE_GAMEOVER::~CSCENE_GAMEOVER(void)
	{
	//	解放
		sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_GAMEOVER::sceneInitialize
	//
	//	機能		初期化
	//	引数		inNum			外部データ番号
	//	更新		2009/02/05		<新規>
	//================================================================================
	void
	CSCENE_GAMEOVER::sceneInitialize(int inStageNum)
	{
	//	初期解放
		sceneFinishalize();

	//	演出属性の初期化
		texGameOver	= new CTEXTURE(gDev->getDevice(), "game_over",
								   VEC2(1512.0f, 756.0f), 0xFFFFFF96);
		texGameOver->setLoc(&VEC3(512.0, 384.0f, 0.5f));
		texGameOver->setColorAlpha(0);
		iFrameCnt	= 0;

	//	幕の初期化
	//	幕を黒くする
		gDev->setCurtainColor(0, 0, 0);
		gDev->setCurtainColorAlpha(0);
	}

	//================================================================================
	//!	メソッド名	CSCENE_GAMEOVER::sceneFinishalize
	//
	//	機能		解放
	//	更新		2009/02/05		<新規>
	//================================================================================
	void
	CSCENE_GAMEOVER::sceneFinishalize(void)
	{
	//	演出属性の解放
		SAFE_DELETE(sStreamingBgm);
		SAFE_DELETE(texGameOver);
	}

	//================================================================================
	//!	メソッド名	CSCENE_GAMEOVER::sceneMain
	//
	//	機能		シーンメイン
	//	引数		input			入力情報
	//	戻り値		現在のシーンフラグ
	//	更新		2009/02/05		<新規>
	//================================================================================
	SCENE_FLAG
	CSCENE_GAMEOVER::sceneMain(CINPUT* input)
	{
	//	処理
		this->execute(input);
	//	描画
		this->render();

		return	returnFlag;
	}

	//================================================================================
	//!	メソッド名	CSCENE_GAMEOVER::execute
	//
	//	機能		処理
	//	引数		input			入力情報
	//	更新		2009/02/05		<新規>
	//================================================================================
	void
	CSCENE_GAMEOVER::execute(CINPUT* input)
	{
	//	120フレーム間、ゲームオーバーの文字を縮小し、アルファ値上げる
		iFrameCnt++;
		if(	iFrameCnt < 200)
		{
			texGameOver->calcShowSize(&VEC2(-5.0f, -2.5f));
			texGameOver->calcColorAlpha(2);
		}
	//	
		else
		if(	iFrameCnt < 320)
		{
		}
		else
		if(	iFrameCnt < 325)
		{
		//	幕を出現
			gDev->curtainMotion(TRUE, 5);
		}
		else
		{
		//	幕の透明度が最大なら、コンティニュー選択処理移行中へ
			if(	gDev->getCurtainColorAlpha() >= 255)
			{
				returnFlag	= nextFlag;
			}
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_GAMEOVER::render
	//
	//	機能		描画
	//	更新		2009/02/05		<新規>
	//================================================================================
	void
	CSCENE_GAMEOVER::render(void)
	{
	//	描画開始
		gDev->renderBegin(D3DCOLOR_RGBA(0,0,0,0));

		gDev->spriteBegin();
		texGameOver->draw(gDev->getSprite());
		gDev->spriteEnd();

	//	幕の描画
		gDev->renderCurtainInDev();
	//	描画終了
		gDev->renderEnd();
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------