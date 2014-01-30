//***********************************************************************************
//!	ファイル名		Class_SceneDemo.cpp
//
//	機能			デモのシーンを実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2008/09/29	<新規>
//					2008/11/17	<変更>	ゲームシーンを継承
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneDemo.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_DEMO::CSCENE_DEMO
	//
	//	機能		コンストラクタ
	//	引数		inData				シーン共有データ
	//	更新		2008/09/29	<新規>
	//				2008/11/17	<変更>	ゲームシーンを継承
	//================================================================================
	CSCENE_DEMO::CSCENE_DEMO(SCENE_SHARE_DATA* inData)	:
	CSCENE_ADVENTURE_GAME(eSCENE_DEMO, eSCENE_TITLE, eSCENE_TITLE, inData)		//!< ゲームシーン
	{
	//	初期化
		this->effDemo	= NULL;			//!< デモ表示のエフェクト
	}

	//================================================================================
	//!	メソッド名	CSCENE_DEMO::~CSCENE_DEMO
	//
	//	機能		デストラクタ
	//	更新		2008/09/29	<新規>
	//				2008/11/17	<変更>	ゲームシーンを継承
	//================================================================================
	CSCENE_DEMO::~CSCENE_DEMO(void)
	{
	//	解放
		this->sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_DEMO::sdInitialize
	//
	//	機能		デモの初期化
	//	更新		2008/11/17		<新規>
	//================================================================================
	void
	CSCENE_DEMO::sdInitialize(void)
	{
	//	初期開放
		this->sdFinishalize();
	//	デモエフェクト番号を乱数生成
		int	inDemoEffect	= DEMO_EFFECT_NUM;
	//	デモ表示のエフェクトパターン数を取得
		TCHAR	effectName[MAX_PATH]	= "";
	//	デモエフェクト名を生成
		sprintf_s(effectName, sizeof(effectName), NAME_DEMO, inDemoEffect);
	//	生成
		this->effDemo	=
			new C3D_EFFECT(	this->gDev->getDevice(),
							effectName,
							VEC3(float(this->gDev->getScreenSizeX()) / 2.0f,
								 float(this->gDev->getScreenSizeY()) / 2.0f,
								 0.0f));
	}

	//================================================================================
	//!	メソッド名	CSCENE_DEMO::sdFinishalize
	//
	//	機能		デモの初期化
	//	更新		2008/11/17		<新規>
	//================================================================================
	void
	CSCENE_DEMO::sdFinishalize(void)
	{
	//	解放
		SAFE_DELETE(this->effDemo);
	}

	//================================================================================
	//!	メソッド名	CSCENE_DEMO::sceneInitialize
	//
	//	機能		初期化
	//	引数		inStageNum		外部データ番号
	//	更新		2008/11/18		<新規>
	//================================================================================
	void
	CSCENE_DEMO::sceneInitialize(int inStageNum)
	{
	//	デモ用のステージ番号を取得
		inStageNum	= DEMO_STAGE_NUM;
	//	ゲームシーンの初期化
		CSCENE_ADVENTURE_GAME::sceneInitialize(inStageNum);
	//	デモの初期化
		this->sdInitialize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_DEMO::sceneFinishalize
	//
	//	機能		解放
	//	更新		2008/11/18		<新規>
	//================================================================================
	void
	CSCENE_DEMO::sceneFinishalize(void)
	{
	//	デモの解放
		this->sdFinishalize();
	//	ゲームシーンの開放
		CSCENE_ADVENTURE_GAME::sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_DEMO::sceneMain
	//
	//	機能		シーンメイン
	//	機能概要	ゲームシーンのメソッドを使い、デモを形成
	//	更新		2008/09/19		<新規>
	//================================================================================
	SCENE_FLAG
	CSCENE_DEMO::sceneMain(CINPUT* input)
	{
	//	ゲームフラグで処理分岐
		switch(this->gameFlag)
		{
		//	ゲームイベント中
			case	eSGFLAG_EVENT:
				this->sgEventExecute(input);
				this->sgEventRender();
			//	ゲームイベント中にゲームプレイに移ったか、
			//	又はスタートボタンが押されたら、デモ終了
				if(	(this->gameFlag == eSGFLAG_PLAY)	||
					(input->checkPressOnce(MYKEY::eSTART)))
				{
					this->sgQuitChange();
				}
				break;
		//	ゲーム終了中(デモ終了)
			case	eSGFLAG_QUIT:
				this->sgQuitExecute(input);
				this->sgQuitRender();
				break;
		//	ゲームエラー
			case	eSGFLAG_ERROR:
				this->returnFlag	= eSCENE_ERROR;
				break;
		//	上記以外
			default:
			//	強制終了
				this->returnFlag	= eSCENE_TITLE;
				break;
		}

		return	this->returnFlag;
	}

	//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	//	オーバーライド処理
	//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	//================================================================================
	//!	メソッド名	CSCENE_DEMO::sgEventRender
	//
	//	機能		ゲームイベント中描画
	//	更新		2008/11/27		<新規>
	//================================================================================
	void
	CSCENE_DEMO::sgEventRender(void)
	{
	//	デバイスを確保
		DEV	device	= this->gDev->getDevice();
	//	描画開始
		this->gDev->renderBegin();

	//	カメラの更新
		this->camera->updateCamera();
	//	ライトの更新
		this->sgUpdateLight();

	//-------------------------------------------------
	//	3D物体の描画
	//-------------------------------------------------
	//	Zテクスチャシェーダでテクスチャに深度値情報を書き込む
		this->shaderZTexture->begin();
		this->gameMap->squeezeSortByCameraPositionWithDrawZTexSha(
			this->camera, &this->camera->getViewPointFinish(), this->shaderZTexture);
		this->shaderZTexture->end();

		this->gDev->bufferClear(D3DCOLOR_RGBA(255,255,255,255));

	//	カメラでデバイス更新
		this->camera->viewTransform(device);

	//	背景の描画
		this->back->motion(&this->camera->getViewPointFinish());
		this->back->drawBack(device, this->gDev->getSprite());

	//	深度バッファシェーダで描画
		this->shaderDepthBuf->setCameraViewProjMatrix(
			&((*this->camera->getMatrixView()) * (*this->camera->getMatrixProjection())));

		this->playerChara->draw(device);
		this->gameMap->drawEnemyG(device, &this->camera->getViewPointFinish());

	//	ライティングを無効
		this->shaderDepthBuf->begin();
		this->gameMap->squeezeDrawWithDepthSha(this->shaderDepthBuf);
		this->shaderDepthBuf->end();

		this->effectCenter->drawEffects(device, this->camera);

	//-------------------------------------------------
	//	2D物体の描画
	//-------------------------------------------------
		this->gDev->spriteBegin();
		this->eventCommentG->drawComment(this->gDev->getSprite());
		this->effectCenter->drawEffects2D(this->gDev->getSprite());
		this->effDemo->draw(this->gDev->getSprite());
		this->gDev->spriteEnd();

	//	幕の描画
		this->gDev->renderCurtainInDev();
	//	描画終了
		this->gDev->renderEnd();
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------