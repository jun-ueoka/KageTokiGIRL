//***********************************************************************************
//!	ファイル名		Class_SceneMovie3.cpp
//
//	機能			ムービーシーンクラス、ムービー3[シャドウクリミナルアジト]の実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/02/10	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneMovie2.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_MOVIE3::コンストラクタ
	//
	//	引数		inFlag		現在のシーンフラグ
	//				inNextFlag	次のシーンフラグ
	//				inData		シーン共有データ
	//	更新		2009/02/10	<新規>
	//================================================================================
	CSCENE_MOVIE3::CSCENE_MOVIE3(SCENE_SHARE_DATA* inData)	:
	//!	ゲームシーン
	CSCENE_MOVIE_BASE(eSCENE_CHAPTER1_MOVIE3, eSCENE_CHAPTER1_STAGE3, inData)
	{
	//	初期化
		this->camera			= NULL;
		this->cameraTarget		= NULL;

		this->kageo				= NULL;
		this->kagemi			= NULL;
		this->shabird			= NULL;
		this->clime				= NULL;
		this->tima				= NULL;
		this->feather			= NULL;
		this->yamakage			= NULL;
		this->exe				= NULL;

		this->objMapForest		= NULL;
		this->objMapCave		= NULL;
		this->back				= NULL;
		this->cave				= NULL;

		this->movieFlag			= eM3F_FOREST;

		this->backSound			= NULL;
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;

		D3DXMatrixIdentity(&this->lightView);
		D3DXMatrixIdentity(&this->lightProj);

		this->lightShift		= VEC3(0.0f, 0.0f, 0.0f);
		this->lightNearFar		= VEC2(0.0f, 0.0f);
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE3::デストラクタ
	//
	//	更新		2009/02/10	<新規>
	//================================================================================
	CSCENE_MOVIE3::~CSCENE_MOVIE3(void)
	{
	//	解放
		sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE3::sceneInitialize
	//
	//	機能		初期化
	//	引数		inStageNum	外部データ番号
	//	更新		2009/02/10	<新規>
	//================================================================================
	void
	CSCENE_MOVIE3::sceneInitialize(int inStageNum)
	{
	//	ムービーの初期化
		this->initializeMovie(3);

	//	初期解放
		this->sceneFinishalize();

	//	初期化

	//	カメラ
		this->camera	= new C3D_OBJECT_CAMERA(&VEC3(0.0f,0.0f,0.0f), &VEC3(0.0f,0.0f,0.0f));
		this->camera->setViewFar(20000.0f);
		this->camera->setViewShift(&VEC3(100.0f, -500.0f,  2000.0f));
	//	カメラターゲット
		this->cameraTarget	= new C3D_OBJECT();

	//	カゲオ
		this->kageo		= new CSKIN_MESH(gDev->getDevice(), "movie_kageo");
	//	カゲミ
		this->kagemi	= new CSKIN_MESH(gDev->getDevice(), "movie_kagemi");
	//	シャバード
		this->shabird	= new CSKIN_MESH(gDev->getDevice(), "enemy_shabird");
	//	クライム
		this->clime		= new CSKIN_MESH(gDev->getDevice(), "movie_clime");
	//	ティマ
		this->tima		= new CSKIN_MESH(gDev->getDevice(), "movie_tima");
	//	フェザー
		this->feather	= new CSKIN_MESH(gDev->getDevice(), "movie_feather");
	//	ヤマカゲ
		this->yamakage	= new CSKIN_MESH(gDev->getDevice(), "movie_yamakage");
	//	エクス
		this->exe		= new CSKIN_MESH(gDev->getDevice(), "movie_exe");

	//	空背景
		this->back		= new CBACK_SKY(this->gDev->getDevice(), &VEC3(0.0f,0.0f,0.0f));

	//	使用するエフェクトの生成
		this->effectCenter->addEffects(
			EFFECT_SMOKE, new C3D_EFFECT(gDev->getDevice(), EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));

	//	洞窟
		this->cave	= new CMESH(gDev->getDevice(), "kage_stage_doom2");

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
	//	ライトビューのオフセット値を設定
		this->lightNearFar	= VEC2(1000.0f, 12000.0f);
		this->lightShift	= VEC3(5000.0f,  5000.0f, -6000.0f);

	//	ライト射影行列の設定
		D3DXMatrixPerspectiveFovLH(&this->lightProj, D3DXToRadian(70), 1.0f, lightNearFar.x, lightNearFar.y);
	//	------------------------------------------------------

	//	3Dマップ(森)
		this->objMapForest	= new CGAME_MAP();
		this->objMapForest->initialize(this->gDev->getDevice(), 203);

	//	3Dマップ(シャドウクリミナルアジト洞窟)
		this->objMapCave	= new CGAME_MAP();
		this->objMapCave->initialize(this->gDev->getDevice(), 204);

		this->movieFlag		= eM3F_FOREST;

	//	ムービーの開始
		this->movieStartProc();
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE3::sceneFinishalize
	//
	//	機能		解放
	//	更新		2009/02/10	<新規>
	//================================================================================
	void
	CSCENE_MOVIE3::sceneFinishalize(void)
	{
	//	解放
		SAFE_DELETE(this->camera);
		SAFE_DELETE(this->cameraTarget);

		SAFE_DELETE(this->kageo);
		SAFE_DELETE(this->kagemi);
		SAFE_DELETE(this->shabird);
		SAFE_DELETE(this->clime);
		SAFE_DELETE(this->tima);
		SAFE_DELETE(this->feather);
		SAFE_DELETE(this->yamakage);
		SAFE_DELETE(this->exe);

		SAFE_DELETE(this->objMapForest);
		SAFE_DELETE(this->objMapCave);
		SAFE_DELETE(this->backSound);

		SAFE_DELETE(this->back);
		SAFE_DELETE(this->cave);

		CZTEX_SHA::releaseInstance();
		CDEPTH_SHA::releaseInstance();
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE3::sceneMain
	//
	//	機能		シーンメイン
	//	機能概要	ゲームシーンメイン
	//	引数		input		入力情報
	//	戻り値		次のシーン
	//	更新		2009/02/10	<新規>
	//================================================================================
	SCENE_FLAG
	CSCENE_MOVIE3::sceneMain(CINPUT* input)
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
	//!	メソッド名	CSCENE_MOVIE3::executeInstantChengeMovieSpeed
	//
	//	機能		ムービー処理速度変更時処理
	//	更新		2009/02/15	<新規>
	//================================================================================
	void
	CSCENE_MOVIE3::executeInstantChengeMovieSpeed(void)
	{
		this->kageo->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->kagemi->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->shabird->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->clime->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->tima->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->feather->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->yamakage->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->exe->setAnimSpeed(0.016f * float(this->movieSpeed));
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE3::executeProcOnce
	//
	//	機能		即時処理実行
	//	引数		inProc		処理番号
	//	更新		2009/02/10	<新規>
	//================================================================================
	void
	CSCENE_MOVIE3::executeProcOnce(int inProc)
	{
	//	ムービーフラグが森なら、以下を処理
		if(	this->movieFlag == eM3F_FOREST)
		{
		//	処理番号で分岐
			switch(inProc)
			{
				case 0:
					this->kageo->setLoc (&VEC3(-3000.0f, 0.0f,  50.0f));
					this->kageo->setAngleY(270.0f);
					this->kageo->setMoveQuantity(3.0f);
					this->kageo->changeAnimationAndBlend(0);
					this->kagemi->setLoc(&VEC3(-3000.0f, 0.0f,-150.0f));
					this->kagemi->setAngleY(270.0f);
					this->kagemi->setMoveQuantity(3.0f);
					this->kagemi->changeAnimationAndBlend(0);
					this->shabird->setLoc(&VEC3(800.0f, 1800.0f, 500.0f));
					this->shabird->setMoveQuantity(3.0f);
					this->camera->setCameraMode(eCAMO_PARALLEL);
					this->camera->setLoc(&VEC3(-3000.0f, 700.0f, -1500.0f));
					this->camera->setViewPointShift(&VEC3(0.0f, -500.0f, 1500.0f));
					break;
				case 1:
					this->kagemi->setMoveQuantity(5.0f);
					this->kagemi->changeAnimationAndBlend(5);
					break;
				case 2:
					this->kageo->setMoveQuantity(5.0f);
					this->kageo->changeAnimationAndBlend(5);
					break;
				case 3:
					this->shabird->setMoveQuantity(10.0f);
					break;
				case 4:
					this->gDev->curtainMotion(TRUE, 4);
					this->gDev->createCurtainTex();
					break;
				case 5:
					this->movieFlag		= eM3F_CAVE;
					break;
			}
		}
	//	ムービーフラグが洞窟なら、以下を処理	
		else
		{
		//	処理番号で分岐
			switch(inProc)
			{
				case 6:
					this->camera->setCameraMode(eCAMO_NO);
					this->camera->setChase(FALSE);
					this->camera->setTarget(this->shabird);
					this->camera->setLoc(&VEC3(0.0f, 4500.0f, 0.0f));
					this->camera->setViewShift(&VEC3(0.0f, 0.0f, 0.0f));
					this->shabird->setLoc(&VEC3(2500.0f, 5000.0f, -5000.0f));
					this->shabird->setAngleX(-16.0f);
					this->shabird->setAngleY(180.0f);
					this->shabird->setMoveQuantity(10.0f);
					this->clime->setLoc(&VEC3(0.0f, 3000.0f, 2000.0f));
					this->clime->setAngle(&VEC3(0.0f, 0.0f, 0.0f));
					this->tima->setLoc(&VEC3(2000.0f, 3000.0f, 1000.0f));
					this->tima->setAngle(&VEC3(0.0f, 72.0f, 0.0f));
					this->feather->setLoc(&VEC3(-2000.0f, 3000.0f, 1000.0f));
					this->feather->setAngle(&VEC3(0.0f, 288.0f, 0.0f));
					this->yamakage->setLoc(&VEC3(-1000.0f, 3000.0f, -2000.0f));
					this->yamakage->setAngle(&VEC3(0.0f, 216.0f, 0.0f));
					this->exe->setLoc(&VEC3(1000.0f, 3000.0f, -2000.0f));
					this->exe->setAngle(&VEC3(0.0f, 144.0f, 0.0f));
					this->gDev->curtainMotion(FALSE, 4);
					break;
				case 7:
					this->camera->setCameraMode(eCAMO_PARALLEL);
					this->camera->setLocY(3100.0f);
					this->camera->setViewPointShift(&VEC3(2000.0f, 250.0f, 1000.0f));
					this->camera->setTarget(NULL);
					this->shabird->setAngleX(0.0f);
					break;
				case 8:
					this->camera->setCameraMode(eCAMO_PARALLEL_CHASE);
					this->camera->setTarget(this->cameraTarget);
					this->camera->setMaxViewMoveCnt(100);
					this->cameraTarget->setLoc(&VEC3(1000.0f, 3250.0f, -2000.0f));
					break;
				case 9:
					this->cameraTarget->setLoc(&VEC3(-1000.0f, 3250.0f, -2000.0f));
					break;
				case 10:
					this->cameraTarget->setLoc(&VEC3(-2000.0f, 3250.0f, 1000.0f));
					break;
				case 11:
					this->cameraTarget->setLoc(&VEC3(0.0f, 3250.0f, 2000.0f));
					break;
				case 12:
					this->tima->setAngle(&VEC3(0.0f, 100.0f, 0.0f));
					this->cameraTarget->setLoc(&VEC3(2000.0f, 3250.0f, 1000.0f));
					break;
				case 13:
					this->tima->changeAnimationAndBlend(4);
					break;
				case 14:
					this->effectCenter->occuEffects(
						EFFECT_SMOKE, this->shabird->getLoc(), 1.0f);
					this->shabird->setVisible(FALSE);
					this->tima->changeAnimationAndBlend(0);
					break;
				case 15:
					this->camera->setCameraMode(eCAMO_NO);
					this->camera->setTarget(NULL);
					this->camera->setLoc(&VEC3(-1000.0f, 3300.0f, 1000.0f));
					this->camera->setViewPoint(&VEC3(1000.0f, 3250.0f, 2000.0f));
					break;
				case 16:
					this->tima->changeAnimationAndBlend(1);
					break;
				case 17:
					this->tima->changeAnimationAndBlend(0);
					break;
				case 18:
					this->camera->setCameraMode(eCAMO_PARALLEL_CHASE);
					this->camera->setTarget(this->cameraTarget);
					this->cameraTarget->setLoc(&VEC3(0.0f, 3250.0f, 2000.0f));
					break;
				case 19:
					this->cameraTarget->setLoc(&VEC3(1000.0f, 3250.0f, -2000.0f));
					break;
				case 20:
					this->exe->changeAnimationAndBlend(3);
					break;
				case 21:
					this->camera->setCameraMode(eCAMO_NO);
					this->camera->setTarget(this->exe);
					this->exe->setMoveQuantity(90.0f);
					this->exe->changeAnimationAndBlend(2);
					this->effectCenter->occuEffects(
						EFFECT_SMOKE, this->exe->getLoc(), 1.0f);
					break;
				case 22:
					this->camera->setTarget(NULL);
					break;
				case 23:
					this->camera->setViewPoint(&VEC3(0.0f, 3250.0f, 2000.0f));
					this->camera->setLoc(&VEC3(0.0f, 3100.0f, 1700.0f));
					this->clime->setAngleY(0.0f);
					break;
				case 24:
					this->clime->changeAnimationAndBlend(4);
					break;
			}
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE3::executeProc
	//
	//	機能		処理実行
	//	引数		inProc		処理番号
	//	更新		2009/02/10	<新規>
	//================================================================================
	void
	CSCENE_MOVIE3::executeProc(int inProc)
	{
	//	ムービーフラグが森なら、以下を処理
		if(	this->movieFlag == eM3F_FOREST)
		{
		//	処理番号で分岐
			switch(inProc)
			{
				case 0:
					this->camera->calcLocX(2.5f);
					this->camera->calcViewPointShiftX(0.5f);
					this->kageo->moveFront();
					this->kagemi->moveFront();
					break;
				case 1:
					this->camera->calcLocY(1.0f);
					this->camera->calcLocZ(0.5f);
					this->camera->calcViewPointShiftX(3.0f);
					this->kageo->moveFront();
					this->kagemi->moveFront();
					break;
				case 2:
					this->camera->calcLocY(1.0f);
					this->camera->calcViewPointShiftX(-5.0f);
					this->camera->calcViewPointShiftY(4.0f);
					this->camera->calcViewPointShiftZ(3.0f);
					this->kageo->moveFront();
					this->kagemi->moveFront();
					break;
				case 3:
					this->camera->calcLocY(1.0f);
					this->camera->calcViewPointShiftX(-5.0f);
					this->camera->calcViewPointShiftY(4.0f);
					this->camera->calcViewPointShiftZ(3.0f);
					this->kageo->moveFront();
					this->kagemi->moveFront();
					this->shabird->moveFront();
					break;
				case 4:
					this->camera->calcLocY(1.0f);
					this->camera->calcViewPointShiftX(-5.0f);
					this->camera->calcViewPointShiftY(4.0f);
					this->camera->calcViewPointShiftZ(3.0f);
					this->shabird->calcAngleX(0.2f);
					this->shabird->calcAngleY(1.0f);
					this->shabird->moveFront();
					break;
				case 5:
					this->camera->calcLocY(1.0f);
					this->camera->calcViewPointShiftX(-5.0f);
					this->camera->calcViewPointShiftY(4.0f);
					this->camera->calcViewPointShiftZ(3.0f);
					this->shabird->moveFront();
					break;
			}
		}
	//	ムービーフラグが洞窟なら、以下を処理	
		else
		{
		//	処理番号で分岐
			switch(inProc)
			{
				case 6:
					this->camera->calcLocY(-1.0f);
					this->shabird->calcAngleY(-0.01f);
					this->shabird->moveFront();
					break;
				case 7:
					this->camera->calcViewPointShiftY(-0.25f);
					this->camera->calcFieldOfView(-0.1f);
					this->shabird->calcAngleY(-1.0f);
					this->shabird->moveFront();
					break;
				case 8:
					this->shabird->calcAngleY(-1.0f);
					this->shabird->moveFront();
					break;
				case 9:
					this->camera->calcFieldOfView(0.2f);
					this->camera->calcLocZ(6.0f);
					this->camera->calcLocX(-2.0f);
					break;
				case 10:
					this->shabird->calcAngleY(-1.0f);
					this->shabird->moveFront();
					this->camera->calcLocZ(-1.0f);
					this->camera->calcLocX(4.0f);
					break;
				case 11:
					this->clime->calcAngleY(-2.0f);
					break;
				case 12:
					this->camera->calcLocX(2.0f);
					this->camera->calcLocZ(-5.0f);
					this->clime->calcAngleY(0.02f);
					break;
				case 13:
					this->camera->calcLocX(1.0f);
					this->camera->calcLocZ(-5.0f);
					break;
				case 14:
					this->exe->calcAngleY(7.0f);
					break;
				case 15:
					this->camera->calcLocX(-20.0f);
					this->camera->calcLocY(20.0f);
					this->exe->calcAngleX(-8.0f);
					this->exe->moveUp();
					break;
				case 16:
					this->exe->moveUp();
					break;
				case 17:
					this->camera->calcLocZ(-1.0f);
					break;
			}
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE3::updateLight
	//
	//	機能		ライトの更新
	//	更新		2009/02/10	<新規>
	//================================================================================
	void
	CSCENE_MOVIE3::updateLight(void)
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
	//!	メソッド名	CSCENE_MOVIE3::execute
	//
	//	機能		処理
	//	引数		input		入力情報
	//	更新		2009/02/10	<新規>
	//================================================================================
	void
	CSCENE_MOVIE3::execute(CINPUT* input)
	{
		this->checkMovieInput(input);
		this->checkProcFollowMovieSpeed();
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE3::render
	//
	//	機能		描画
	//	更新		2009/02/10	<新規>
	//================================================================================
	void
	CSCENE_MOVIE3::render(void)
	{
	//	ムービーフラグが森なら、以下を処理
		if(	this->movieFlag == eM3F_FOREST)
		{
		//	描画開始
			gDev->renderBegin(D3DCOLOR_RGBA(255,255,255,0));

			DEV	inDev	= this->gDev->getDevice();

		//	カメラの更新
			this->camera->updateCamera();

		//	ライトの更新
			this->updateLight();

		//	Zテクスチャシェーダでテクスチャに深度値情報を書き込む
			this->shaderZTexture->begin();
			this->objMapForest->squeezeSortByCameraPositionNotCheckMessageWithDrawZTexSha(
				this->camera, &this->camera->getViewPointFinish(), this->shaderZTexture);
			this->shaderZTexture->end();

			this->gDev->bufferClear(D3DCOLOR_RGBA(255,255,255,255));

		//	カメラでデバイス更新
			this->camera->viewTransform(inDev);

		//	空背景の描画
			this->back->motion(&this->camera->getViewPointFinish());
			this->back->drawBack(inDev, this->gDev->getSprite());

		//	深度バッファシェーダで描画
			this->shaderDepthBuf->setCameraViewProjMatrix(
				&((*this->camera->getMatrixView()) * (*this->camera->getMatrixProjection())));

		//	森の描画
			this->shaderDepthBuf->begin();
			this->objMapForest->squeezeDrawWithDepthSha(this->shaderDepthBuf);
			this->shaderDepthBuf->end();

		//	キャラクターの描画
			this->kageo->draw(inDev);
			this->kagemi->draw(inDev);
			this->shabird->draw(inDev);

		//	エフェクトの描画
			this->effectCenter->drawEffects(inDev, this->camera);

		//	シネマスコープの描画
			this->direction->directCinemaScopePaintOut();

		//	フォントの描画
			this->gDev->spriteBegin();
			this->executeDrawTelop();
			this->gDev->spriteEnd();

		//	幕の描画
			this->gDev->renderCurtainInDev();
			this->gDev->renderEnd();
		}
	//	ムービーフラグが洞窟なら、以下を処理
		else
		{
		//	描画開始
			gDev->renderBegin(D3DCOLOR_RGBA(255,255,255,0));

			DEV	inDev	= this->gDev->getDevice();

		//	カメラの更新
			this->camera->updateCamera();

		//	ライトの更新
			this->updateLight();

		//	Zテクスチャシェーダでテクスチャに深度値情報を書き込む
			this->shaderZTexture->begin();
			this->objMapCave->squeezeSortByCameraPositionNotCheckMessageWithDrawZTexSha(
				this->camera, &this->camera->getViewPointFinish(), this->shaderZTexture);
			this->shaderZTexture->end();

			this->gDev->bufferClear(D3DCOLOR_RGBA(255,255,255,255));

		//	カメラでデバイス更新
			this->camera->viewTransform(inDev);

		//	洞窟表面の描画
			this->cave->draw(inDev);

		//	深度バッファシェーダで描画
			this->shaderDepthBuf->setCameraViewProjMatrix(
				&((*this->camera->getMatrixView()) * (*this->camera->getMatrixProjection())));

		//	洞窟の描画
			this->shaderDepthBuf->begin();
			this->objMapCave->squeezeDrawWithDepthSha(this->shaderDepthBuf);
			this->shaderDepthBuf->end();

		//	キャラクターの描画
			this->shabird->draw(inDev);
			this->clime->draw(inDev);
			this->tima->draw(inDev);
			this->feather->draw(inDev);
			this->yamakage->draw(inDev);
			this->exe->draw(inDev);

		//	エフェクトの描画
			this->effectCenter->drawEffects(inDev, this->camera);

		//	シネマスコープの描画
			this->direction->directCinemaScopePaintOut();

		//	フォントの描画
			this->gDev->spriteBegin();
			this->executeDrawTelop();
			this->gDev->spriteEnd();

		//	幕の描画
			this->gDev->renderCurtainInDev();
			this->gDev->renderEnd();
		}
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------