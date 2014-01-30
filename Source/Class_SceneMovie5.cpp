//***********************************************************************************
//!	ファイル名		Class_SceneMovie5.cpp
//
//	機能			ムービーシーンクラス、ムービー5[振り切られるも…]の実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/02/10	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneMovie5.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_MOVIE5::コンストラクタ
	//
	//	引数		inFlag		現在のシーンフラグ
	//				inNextFlag	次のシーンフラグ
	//				inData		シーン共有データ
	//	更新		2009/02/10	<新規>
	//================================================================================
	CSCENE_MOVIE5::CSCENE_MOVIE5(SCENE_SHARE_DATA* inData)	:
	//!	ゲームシーン
	CSCENE_MOVIE_BASE(eSCENE_CHAPTER2_MOVIE5, eSCENE_CHAPTER2_STAGE5, inData)
	{
	//	初期化
		this->camera	= NULL;
		this->kageo		= NULL;
		this->kagemi	= NULL;
		this->sky		= NULL;
		this->objMap	= NULL;

		this->backSound			= NULL;
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE5::デストラクタ
	//
	//	更新		2009/02/10	<新規>
	//================================================================================
	CSCENE_MOVIE5::~CSCENE_MOVIE5(void)
	{
	//	解放
		sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE5::sceneInitialize
	//
	//	機能		初期化
	//	引数		inStageNum	外部データ番号
	//	更新		2009/02/10	<新規>
	//================================================================================
	void
	CSCENE_MOVIE5::sceneInitialize(int inStageNum)
	{
	//	ムービーの初期化
		this->initializeMovie(1);

	//	初期解放
		this->sceneFinishalize();

	//	初期化

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

	//	空
		this->sky	= new CTEXTURE(gDev->getDevice(), "sky", 
							   VEC2((float)gDev->getScreenSizeX(), (float)gDev->getScreenSizeY()));
		this->sky->setLoc(&VEC3(float(gDev->getScreenSizeX()/2), float(gDev->getScreenSizeY()/2), 1.0f));

	//	BGM
		this->backSound	= new SSTREAMING("ainoaisatu");
		this->backSound->play(0, DSBPLAY_LOOPING);

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
		this->objMap	= new CGAME_MAP();
		this->objMap->initialize(this->gDev->getDevice(), 1);

	//	ムービーの開始
		this->movieStartProc();
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE5::sceneFinishalize
	//
	//	機能		解放
	//	更新		2009/02/10	<新規>
	//================================================================================
	void
	CSCENE_MOVIE5::sceneFinishalize(void)
	{
	//	解放
		SAFE_DELETE(this->camera);
		SAFE_DELETE(this->kageo);
		SAFE_DELETE(this->kagemi);
		SAFE_DELETE(this->sky);

		SAFE_DELETE(this->objMap);
		SAFE_DELETE(this->backSound);

		CZTEX_SHA::releaseInstance();
		CDEPTH_SHA::releaseInstance();
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE5::sceneMain
	//
	//	機能		シーンメイン
	//	機能概要	ゲームシーンメイン
	//	引数		input		入力情報
	//	戻り値		次のシーン
	//	更新		2009/02/10	<新規>
	//================================================================================
	SCENE_FLAG
	CSCENE_MOVIE5::sceneMain(CINPUT* input)
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
	//!	メソッド名	CSCENE_MOVIE5::executeInstantChengeMovieSpeed
	//
	//	機能		ムービー処理速度変更時処理
	//	更新		2009/02/15	<新規>
	//================================================================================
	void
	CSCENE_MOVIE5::executeInstantChengeMovieSpeed(void)
	{
		this->kageo->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->kagemi->setAnimSpeed(0.016f * float(this->movieSpeed));
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE5::executeProcOnce
	//
	//	機能		即時処理実行
	//	引数		inProc		処理番号
	//	更新		2009/02/10	<新規>
	//================================================================================
	void
	CSCENE_MOVIE5::executeProcOnce(int inProc)
	{
	//	処理番号で分岐
		switch(inProc)
		{
		//	初期設定
			case 0:
				this->kageo->setLoc (&VEC3(-8400.0f, 0.0f,  50.0f));
				this->kageo->setAngleY(270.0f);
				this->kageo->changeAnimationAndBlend(5);
				this->kagemi->setLoc(&VEC3(-8400.0f, 0.0f,-150.0f));
				this->kagemi->setAngleY(270.0f);
				this->kagemi->changeAnimationAndBlend(5);
				this->camera->setCameraMode(eCAMO_PARALLEL);
				this->camera->setLoc(&VEC3(0.0f, 5000.0f, -1500.0f));
				this->camera->setViewPointShift(&VEC3(0.0f, -500.0f, 1500.0f));
				this->gDev->createCurtainTex("Loading_KageoKagemiB");
				break;

		//
			case 1:
				this->camera->setCameraMode(eCAMO_NO);
				break;
		//
			case 2:
				this->camera->setCameraMode(eCAMO_PARALLEL);
				break;
		//
			case 3:
				this->kageo->changeAnimationAndBlend(0);
				this->kagemi->changeAnimationAndBlend(0);
				break;
		//
			case 4:
				this->kagemi->changeAnimationAndBlend(6);
				break;
		//
			case 5:
				this->kageo->changeAnimationAndBlend(1);
				break;
		//
			case 6:
				this->kagemi->changeAnimationAndBlend(2);
				break;
		//
			case 7:
				this->kagemi->changeAnimationAndBlend(3);
				break;
		//
			case 8:
				this->kageo->changeAnimationAndBlend(2);
				break;
		//
			case 9:
				this->kagemi->changeAnimationAndBlend(0);
				break;
		//
			case 10:
				this->kagemi->changeAnimationAndBlend(5);
				break;
		//
			case 11:
				this->kageo->changeAnimationAndBlend(0);
				break;
		//
			case 12:
				this->kageo->changeAnimationAndBlend(5);
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE5::executeProc
	//
	//	機能		処理実行
	//	引数		inProc		処理番号
	//	更新		2009/02/10	<新規>
	//================================================================================
	void
	CSCENE_MOVIE5::executeProc(int inProc)
	{
	//	処理番号で分岐
		switch(inProc)
		{
		//	初期設定
			case 0:
				this->camera->calcLocY(-4.0f);
				break;
		//	
			case 1:
				this->camera->calcLocX(-4.0f);
				this->kageo->calcLocX(3.0f);
				this->kagemi->calcLocX(3.0f);
				break;
		//	
			case 2:
				this->kageo->calcLocX(3.0f);
				this->kagemi->calcLocX(3.0f);
				break;
		//	
			case 3:
				this->camera->calcViewPointX(-1.1f);
				this->camera->calcViewPointY(-2.0f);
				this->camera->calcLocY(-2.5f);
				this->camera->calcLocZ(2.0f);
				this->kageo->calcLocX(3.0f);
				this->kagemi->calcLocX(3.0f);
				break;
		//	
			case 4:
				this->camera->calcLocX(3.0f);
				this->kageo->calcLocX(3.0f);
				this->kagemi->calcLocX(3.0f);
				break;
		//	
			case 5:
				this->kageo->calcLocX(3.0f);
				this->kagemi->calcAngleY(-1.0f);
				break;
		//	
			case 6:
				this->kageo->calcAngleY(2.3f);
				break;
		//	
			case 7:
				this->kagemi->calcAngleY(1.5f);
				break;
		//	
			case 8:
				this->kagemi->calcLocX(6.0f);
				break;
		//	
			case 9:
				this->camera->calcLocY(3.0f);
				this->kageo->calcAngleY(-3.45f);
				this->kageo->calcLocZ(-2.0f);
				this->kagemi->calcLocX(6.0f);
				break;
		//	
			case 10:
				this->camera->calcLocY(3.0f);
				this->kageo->calcLocX(7.0f);
				this->kagemi->calcLocX(6.0f);
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE5::updateLight
	//
	//	機能		ライトの更新
	//	更新		2009/02/10	<新規>
	//================================================================================
	void
	CSCENE_MOVIE5::updateLight(void)
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
	//!	メソッド名	CSCENE_MOVIE5::execute
	//
	//	機能		処理
	//	引数		input		入力情報
	//	更新		2009/02/10	<新規>
	//================================================================================
	void
	CSCENE_MOVIE5::execute(CINPUT* input)
	{
	//	即時処理データの更新処理
		this->checkProcOnce();
	//	処理データの更新処理
		this->checkProc();

	//	3倍速処理
#ifdef	_DEBUG
		if(	input->checkPress(MYKEY::eSOFT))
		{
			this->iMovieFrame++;
			this->checkProcOnce();
			this->checkProc();
			this->iMovieFrame++;
			this->checkProcOnce();
			this->checkProc();
			this->iMovieFrame++;
			this->checkProcOnce();
			this->checkProc();
			this->iMovieFrame++;
			this->checkProcOnce();
			this->checkProc();

		//	スキンメッシュの早送り
			this->kageo->setAnimSpeed(0.080f);
			this->kagemi->setAnimSpeed(0.080f);
		}
#else
		if(	input->checkPress(MYKEY::eSOFT))
		{
			this->iMovieFrame++;
			this->checkProcOnce();
			this->checkProc();
			this->iMovieFrame++;
			this->checkProcOnce();
			this->checkProc();

		//	スキンメッシュの早送り
			this->kageo->setAnimSpeed(0.048f);
			this->kagemi->setAnimSpeed(0.048f);
		}
#endif
	//	1倍速処理
		else
		{
			this->kageo->setAnimSpeed();
			this->kagemi->setAnimSpeed();
		}

	//	現在のフレームが終了フレームに到達したか、
	//	終了命令がでたら、ムービー終了準備へ
		if(	(iMovieFrame >= iFinishFrame)	||
			input->checkPressOnce(MYKEY::eSTART))
		{
			this->movieEndProc();
		}
		else
		{
			this->iMovieFrame++;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE5::render
	//
	//	機能		描画
	//	更新		2009/02/10	<新規>
	//================================================================================
	void
	CSCENE_MOVIE5::render(void)
	{
	//	描画開始
		gDev->renderBegin(D3DCOLOR_RGBA(255,255,255,0));

		DEV	inDev	= this->gDev->getDevice();

	//	オブジェクトのソート
		this->camera->updateCamera();
		this->objMap->squeezeSortByCameraPosition(
			this->camera, &this->camera->getViewPointFinish());

	//	ライトの更新
		this->updateLight();

	//	Zテクスチャシェーダでテクスチャに深度値情報を書き込む
		this->shaderZTexture->begin();
		this->objMap->squeezeDrawWithZTexSha(this->shaderZTexture);
		this->shaderZTexture->end();

		this->gDev->bufferClear(D3DCOLOR_RGBA(255,255,255,255));

	//	空の描画
		gDev->spriteBegin();
		sky->draw(gDev->getSprite());
		gDev->spriteEnd();

	//	カメラでデバイス更新
		this->camera->viewTransform(inDev);

	//	深度バッファシェーダで描画
		this->shaderDepthBuf->setCameraViewProjMatrix(
			&((*this->camera->getMatrixView()) * (*this->camera->getMatrixProjection())));

		kageo->draw(inDev);
		kagemi->draw(inDev);

		this->shaderDepthBuf->begin();
		this->objMap->squeezeDrawWithDepthSha(this->shaderDepthBuf);
		this->shaderDepthBuf->end();

	//	シネマスコープの描画
		direction->directCinemaScopePaintOut();

	//	フォントの描画
		gDev->spriteBegin();
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