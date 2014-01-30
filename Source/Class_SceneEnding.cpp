//***********************************************************************************
//!	ファイル名		Class_SceneEnding.cpp
//
//	機能			エンディングシーンの実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2008/9/30	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneEnding.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_ENDING::CSCENE_ENDING
	//
	//	機能		コンストラクタ
	//	引数		inFlag			現在のシーンフラグ
	//				inNextFlag		次のシーンフラグ
	//				inData			シーン共有データ
	//	更新		2008/10/05		<新規>
	//================================================================================
	CSCENE_ENDING::CSCENE_ENDING(SCENE_SHARE_DATA* inData)	:
	CSCENE_MOVIE_BASE(eSCENE_ENDING, eSCENE_LOGO, inData)	//!< 基礎シーンフラグの設定
	{
	//	演出属性の初期化
		this->camera	= NULL;
		this->kageo		= NULL;
		this->kagemi	= NULL;
		this->drawMap	= NULL;
		this->objMap1	= NULL;
		this->objMap2	= NULL;
		this->objMap3	= NULL;
		this->objMap4	= NULL;
		this->drawBack	= NULL;
		this->back1		= NULL;

		this->iRoll		= 0;

		this->sStreamingBgm		= NULL;
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;
	}

	//================================================================================
	//!	メソッド名	CSCENE_ENDING::~CSCENE_ENDING
	//
	//	機能		デストラクタ
	//	更新		2008/10/05		<新規>
	//================================================================================
	CSCENE_ENDING::~CSCENE_ENDING(void)
	{
	//	解放
		sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_ENDING::sceneInitialize
	//
	//	機能		初期化
	//	引数		inNum			外部データ番号
	//	更新		2008/10/05		<新規>
	//================================================================================
	void
	CSCENE_ENDING::sceneInitialize(int inStageNum)
	{
	//	ムービーの初期化
		this->initializeMovie(1000);

	//	初期解放
		sceneFinishalize();

	//	カメラ
		this->camera	= new C3D_OBJECT_CAMERA(&VEC3(0.0f,2000.0f,-5000.0f), &VEC3(0.0f,0.0f,0.0f));
		this->camera->setViewFar(20000.0f);
		this->camera->setViewShift(&VEC3(100.0f, -500.0f,  2000.0f));

	//	カゲオ
		this->kageo		= new CSKIN_MESH(gDev->getDevice(), "movie_kageo");
		this->kageo->setLoc(&VEC3(400.0f, 0.0f, 0.0f));
		this->kageo->changeAnimationAndBlend(0);

	//	カゲミ
		this->kagemi	= new CSKIN_MESH(gDev->getDevice(), "movie_kagemi");
		this->kagemi->setLoc(&VEC3(-400.0f, 0.0f, 0.0f));
		this->kagemi->changeAnimationAndBlend(0);


	//	演出属性の初期化
		sStreamingBgm	= new SSTREAMING("ainoaisatu");
		this->sStreamingBgm->play(0, DSBPLAY_LOOPING);

	//	Zテクスチャシェーダの生成
		this->shaderZTexture	= CZTEX_SHA::getInstance();
		this->shaderZTexture->initialize(
			this->gDev->getDevice(),
			this->gDev->getScreenSizeX(), 
			this->gDev->getScreenSizeY());

	//	深度バッファシェーダの生成
		this->shaderDepthBuf	= CDEPTH_SHA::getInstance();
		this->shaderDepthBuf->initialize(this->gDev->getDevice());
		this->shaderDepthBuf->setShadowMap(this->shaderZTexture->getZTex());

	//	ライトの設定------------------------------------------
		D3DXMatrixIdentity(&this->lightView);
		D3DXMatrixIdentity(&this->lightProj);

	//	ライトビューのオフセット値を設定
		this->lightNearFar	= VEC2(1000.0f, 12000.0f);
		this->lightShift	= VEC3(2000.0f,  5000.0f, -6000.0f);

	//	ライト射影行列の設定
		D3DXMatrixPerspectiveFovLH(&this->lightProj, D3DXToRadian(70), 1.0f, lightNearFar.x, lightNearFar.y);
	//	------------------------------------------------------

	//	3Dマップ
		this->objMap1	= new CGAME_MAP();
		this->objMap1->initialize(this->gDev->getDevice(), 201);
		this->objMap2	= new CGAME_MAP();
		this->objMap2->initialize(this->gDev->getDevice(), 10);
		this->objMap3	= new CGAME_MAP();
		this->objMap3->initialize(this->gDev->getDevice(), 20);
		this->objMap4	= new CGAME_MAP();
		this->objMap4->initialize(this->gDev->getDevice(), 30);
		this->drawMap	= this->objMap1;

	//	背景
		this->back1		= new CBACK_SKY(this->gDev->getDevice(), &VEC3(0.0f,0.0f,0.0f));
		this->drawBack	= this->back1;

	//	エンディング用のフォントを生成
		this->font->createFont(
			this->gDev->getDevice(),
			eFONT_STAFFROLL,
			"HG創英角ﾎﾟｯﾌﾟ体",
			40, 20, 3, 0);

	//	ロール文字列の登録
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "REOJECT KageToki",
			300,   900, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "Staff",
			450,  1800, 0xFF000000));


		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "Programmer Leader",
			200,  2600, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "上岡 純",
			250,  2800, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "(うえおか じゅん)",
			500,  2850, 0xFF000000));


		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "Designer Leader",
			200,  3650, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "小松 優也",
			250,  3850, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "(こまつ ゆうや)",
			500,  3900, 0xFF000000));


		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "Programmer",
			200,  4700, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "小金 智弥",
			250,  4900, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "(こがね ともや)",
			500,  4950, 0xFF000000));


		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "Designer",
			200,  5750, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "川原 瑛",
			250,  5950, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "(かわはら あきら)",
			500,  6000, 0xFF000000));


		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "平山 広夢",
			250,  6400, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "(ひらやま ひろむ)",
			500,  6450, 0xFF000000));

		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "及川 佳子",
			250,  6800, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "(おいかわ けいこ)",
			500,  6850, 0xFF000000));

		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "おわり",
			450,  8000, 0xFF000000));

	//	ムービーの開始
		this->movieStartProc();
	}

	//================================================================================
	//!	メソッド名	CSCENE_ENDING::sceneFinishalize
	//
	//	機能		解放
	//	更新		2008/10/05		<新規>
	//================================================================================
	void
	CSCENE_ENDING::sceneFinishalize(void)
	{
	//	演出属性の解放
		SAFE_DELETE(this->camera);
		SAFE_DELETE(this->kageo);
		SAFE_DELETE(this->kagemi);
		SAFE_DELETE(this->objMap1);
		SAFE_DELETE(this->objMap2);
		SAFE_DELETE(this->objMap3);
		SAFE_DELETE(this->objMap4);
		SAFE_DELETE(this->back1);
		this->drawMap	= NULL;
		this->drawBack	= NULL;
		SAFE_DELETE(this->sStreamingBgm);

		CZTEX_SHA::releaseInstance();
		CDEPTH_SHA::releaseInstance();
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;

		this->staffRoll.clear();
	}

	//================================================================================
	//!	メソッド名	CSCENE_ENDING::sceneMain
	//
	//	機能		シーンメイン
	//	引数		input			入力情報
	//	戻り値		現在のシーンフラグ
	//	更新		2008/10/05		<新規>
	//================================================================================
	SCENE_FLAG
	CSCENE_ENDING::sceneMain(CINPUT* input)
	{
	//	処理
		this->execute(input);
	//	描画
		this->render();
	//	ムービーの終了チェック
		this->movieEndCheck();

		return	this->returnFlag;
	}

	//================================================================================
	//!	メソッド名	CSCENE_ENDING::motionStaffRoll
	//
	//	機能		ロール処理
	//	更新		2009/02/23		<新規>
	//================================================================================
	void
	CSCENE_ENDING::motionStaffRoll(void)
	{
		this->iRoll	+= ROLL_SPEED;
	}

	//================================================================================
	//!	メソッド名	CSCENE_ENDING::renderStaffRoll
	//
	//	機能		スタッフロール画面描画
	//	更新		2008/02/23		<新規>
	//================================================================================
	void
	CSCENE_ENDING::renderStaffRoll(void)
	{
	//	スタッフロール文字列を、ロール値だけずらして、描画
		VEC_LOOP(FONT_STRING, this->staffRoll)
		{
			OffsetRect(&(p->rcPos), 0, -this->iRoll);
			if(	((p->rcPos).top			< 768)	&&
				((p->rcPos).top + 100	> 0))
			{
				this->font->drawText(this->gDev->getSprite(), &(*p));
			}
		}
	//	ロール値を初期化
		this->iRoll	= 0;
	}

	//================================================================================
	//!	メソッド名	CSCENE_ENDING::executeInstantChengeMovieSpeed
	//
	//	機能		ムービー処理速度変更時処理
	//	更新		2009/02/15	<新規>
	//================================================================================
	void
	CSCENE_ENDING::executeInstantChengeMovieSpeed(void)
	{
		this->kageo->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->kagemi->setAnimSpeed(0.016f * float(this->movieSpeed));
	}

	//================================================================================
	//!	メソッド名	CSCENE_ENDING::executeProcOnce
	//
	//	機能		即時処理実行
	//	引数		inProc		処理番号
	//	更新		2009/01/27	<新規>
	//================================================================================
	void
	CSCENE_ENDING::executeProcOnce(int inProc)
	{
	//	処理番号で分岐
		switch(inProc)
		{
		//	初期設定
			case 0:
				this->kageo->setLoc (&VEC3(-5000.0f, 0.0f,  50.0f));
				this->kageo->setAngleY(270.0f);
				this->kageo->changeAnimationAndBlend(5);
				this->kagemi->setLoc(&VEC3(-5000.0f, 0.0f,-150.0f));
				this->kagemi->setAngleY(270.0f);
				this->kagemi->changeAnimationAndBlend(5);
				this->camera->setCameraMode(eCAMO_PARALLEL);
				this->camera->setLoc(&VEC3(-4000.0f, 500.0f, -1500.0f));
				this->camera->setViewPointShift(&VEC3(0.0f, -300.0f, 1500.0f));
				this->curtainColor.color	= 0x00000000;
				break;
		//
			case 1:
				this->kageo->setVisible(FALSE);
				this->kagemi->setVisible(FALSE);
				break;
		//
			case 2:
				this->drawMap	= this->objMap2;
				this->camera->setLoc(&VEC3(1000.0f, 500.0f, -1500.0f));
				this->camera->setViewPointShift(&VEC3(0.0f, -300.0f, 1500.0f));
				this->executeProcOnce(6);
				break;
		//
			case 3:
				this->drawMap	= this->objMap3;
				this->camera->setLoc(&VEC3(5000.0f, 1500.0f, -3000.0f));
				this->camera->setViewPointShift(&VEC3(0.0f, -300.0f, 3000.0f));
				this->executeProcOnce(6);
				break;
		//
			case 4:
				this->drawMap	= this->objMap4;
				this->camera->setLoc(&VEC3(1000.0f, 1000.0f, -1500.0f));
				this->camera->setViewPointShift(&VEC3(0.0f, -300.0f, 1500.0f));
				this->executeProcOnce(6);
				break;
		//
			case 5:
				this->gDev->curtainMotion(TRUE, 6);
				this->gDev->createCurtainTex();
				break;
		//
			case 6:
				this->gDev->curtainMotion(FALSE, 6);
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ENDING::executeProc
	//
	//	機能		処理実行
	//	引数		inProc		処理番号
	//	更新		2009/01/27	<新規>
	//================================================================================
	void
	CSCENE_ENDING::executeProc(int inProc)
	{
	//	処理番号で分岐
		switch(inProc)
		{
		//
			case 0:
				this->camera->calcLocX(2.5f);
				this->kageo->calcLocX(3.5f);
				this->kagemi->calcLocX(8.0f);
				break;
		//
			case 1:
				this->camera->calcLocX(2.5f);
				this->kageo->calcLocX(8.0f);
				this->kagemi->calcLocX(8.0f);
				break;
		//
			case 2:
				this->camera->calcLocX(2.5f);
				break;
		//
			case 3:
				this->camera->calcLocX(2.5f);
				this->motionStaffRoll();
				break;
		//
			case 4:
				this->camera->calcLocX(2.5f);
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ENDING::updateLight
	//
	//	機能		ライトの更新
	//	更新		2009/02/05	<新規>
	//================================================================================
	void
	CSCENE_ENDING::updateLight(void)
	{
	//	ライトの更新
		VEC3	lightEye	= this->camera->getViewPointFinish();
		D3DXMatrixLookAtLH(
			&this->lightView,
			&(lightEye + this->lightShift),
			&lightEye,
			&VEC3(0,1,0));

	//	ライトの方向の設定
		this->shaderDepthBuf->setLight(&(lightEye + this->lightShift));
	//	シェーダに設定
		this->shaderDepthBuf->setLightViewProjMatrix(&(this->lightView * this->lightProj));

	//	Zバッファシェーダにライトビュー射影変換行列を登録
		this->shaderZTexture->setLightViewProjMatrix(&(this->lightView * this->lightProj));
	}

	//================================================================================
	//!	メソッド名	CSCENE_ENDING::execute
	//
	//	機能		処理
	//	引数		input		入力情報
	//	更新		2009/01/27	<新規>
	//================================================================================
	void
	CSCENE_ENDING::execute(CINPUT* input)
	{
		this->checkMovieInput(input);
		this->checkProcFollowMovieSpeed();
	}

	//================================================================================
	//!	メソッド名	CSCENE_ENDING::render
	//
	//	機能		描画
	//	更新		2009/01/26		<新規>
	//================================================================================
	void
	CSCENE_ENDING::render(void)
	{
	//	描画開始
		gDev->renderBegin(D3DCOLOR_RGBA(0,0,0,0));

		DEV	inDev	= this->gDev->getDevice();

	//	カメラの更新
		this->camera->updateCamera();

	//	ライトの更新
		this->updateLight();

	//	Zテクスチャシェーダでテクスチャに深度値情報を書き込む
		this->shaderZTexture->begin();
		this->drawMap->squeezeSortByCameraPositionNotCheckMessageWithDrawZTexSha(
			this->camera, &this->camera->getViewPointFinish(), this->shaderZTexture);
		this->shaderZTexture->end();

		this->gDev->bufferClear(D3DCOLOR_RGBA(255,255,255,255));

		gDev->spriteBegin();
		gDev->spriteEnd();

	//	カメラでデバイス更新
		this->camera->viewTransform(inDev);

	//	背景の描画
		this->drawBack->motion(&this->camera->getViewPointFinish());
		this->drawBack->drawBack(inDev, this->gDev->getSprite());

	//	深度バッファシェーダで描画
		this->shaderDepthBuf->setCameraViewProjMatrix(
			&((*this->camera->getMatrixView()) * (*this->camera->getMatrixProjection())));

		this->shaderDepthBuf->begin();
		this->drawMap->squeezeDrawWithDepthSha(this->shaderDepthBuf);
		this->shaderDepthBuf->end();

		kageo->draw(inDev);
		kagemi->draw(inDev);

	//	シネマスコープの描画
		direction->directCinemaScopePaintOut();

	//	フォントの描画
		gDev->spriteBegin();
		renderStaffRoll();
		executeDrawTelop();
		gDev->spriteEnd();

	//	幕の描画
		gDev->renderCurtainInDev();
		gDev->renderEnd();
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------