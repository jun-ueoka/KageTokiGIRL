//***********************************************************************************
//!	ファイル名		Class_SceneAdventureGame.cpp
//
//	機能			ゲームシーンの初期化、解放、処理分岐、描画分岐部分の実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2008/07/10	<新規>
//					2008/11/06	<変更>	全体的に書き直し
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneAdventureGame.h"
//#include	"Class_BackSky.h"		//!< 精神世界背景

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgInitializeGame
	//
	//	機能		ゲームの開始初期化
	//	更新		2008/11/06		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgInitializeGame(void)
	{
	//	ゲーム情報の初期解放
		this->sgFinishalizeGame();
	//	ゲーム情報の構築
	//	環境クラスから
		this->gDev			= CGRAPHIC_DEVICE::getInstance();
		this->effectCenter	= C3D_EFFECT_CENTER::getInstance();
		this->effectCenter->finishalize();
		SAVER::CSAVE_RESOURCE::getInstance(gDev->getDevice());
	//	その他の初期化が必要なゲーム情報
		this->playerChara	= NULL;
		this->player		= new CADVENT_PLAYER(this->gDev->getDevice(), &this->playerChara, this->ssData);
		this->camera		= new C3D_OBJECT_CAMERA(this->playerChara, TRUE, 0.0f, 0.0f, FALSE);
		this->gameMap		= new CGAME_MAP();
		this->enemyManager	= EM::CADVENT_ENEMY_MANAGER::getInstance(this->gDev->getDevice());

	//	デバッグ処理
#ifdef	_DEBUG
		this->fontBase		= CFONT_BASE::getInstance();
	//	コメント用のフォントが生成されて無ければ、生成する
		if(	!this->fontBase->getFontValid(eFONT_DEBUG))
		{
			this->fontBase->createFont(
				gDev->getDevice(),
				eFONT_DEBUG,
				"MSゴシック",
				16, 8, 10, 0);
		}
#else
#ifdef	PRESENTATION
		this->fontBase		= CFONT_BASE::getInstance();
	//	コメント用のフォントが生成されて無ければ、生成する
		if(	!this->fontBase->getFontValid(eFONT_DEBUG))
		{
			this->fontBase->createFont(
				gDev->getDevice(),
				eFONT_DEBUG,
				"MSゴシック",
				16, 8, 10, 0);
		}
#endif
#endif

	//	その他の初期化
	//	カメラのモード設定
		this->camera->setCameraMode(eCAMO_PARALLEL_CHASE);
	//	プレイヤーにマップグループを登録
		this->player->setViewG(this->gameMap->getViewG());
		this->player->setStageG(this->gameMap->getHitG());
		this->player->setEnemyG(this->gameMap->getEnemyG());
	//	プレイヤーにカメラを登録
		this->player->setCameraPointer(this->camera);

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

	//	コンティニュー関連属性
		this->effectCenter->addEffects(
			EFFECT_CONTINUE_YES_SMOKE,
			new C3D_EFFECT(this->gDev->getDevice(), EFFECT_CONTINUE_YES_SMOKE, VEC3(0.0f,0.0f,0.0f)));
		this->effectCenter->addEffects(
			EFFECT_CONTINUE_NO_SMOKE,
			new C3D_EFFECT(this->gDev->getDevice(), EFFECT_CONTINUE_NO_SMOKE,  VEC3(0.0f,0.0f,0.0f)));
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgInitializeScript
	//
	//	機能		スクリプトの開始初期化
	//	更新		2008/11/06		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgInitializeScript(void)
	{
	//	スクリプト情報の初期開放
		this->sgFinishalizeScript();
	//	スクリプト情報の構築
		this->eventCommentG	= new CCOMMENT_GROUP();
		this->eventInput	= new CDUMMY_INPUT();
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgFinishalizeGame
	//
	//	機能		ゲームの解放
	//	更新		2008/11/07		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgFinishalizeGame(void)
	{
	//	ゲーム情報の解放
		SAFE_DELETE(this->player);
		SAFE_DELETE(this->camera);
		SAFE_DELETE(this->gameMap);
		if(	this->enemyManager)
		{	this->enemyManager->releaseEnemy();	}

		SAFE_DELETE(this->back);
		SAFE_DELETE(this->backSound);
		this->stageChangeSwitch.clear();
		if(	this->effectCenter)
		{	this->effectCenter->finishalize();	}

		CZTEX_SHA::releaseInstance();
		CDEPTH_SHA::releaseInstance();
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;

	//	コンティニュー関連属性
		SAFE_DELETE(this->effContinue);
		SAFE_DELETE(this->effContinueYes);
		SAFE_DELETE(this->effContinueNo);
		SAFE_DELETE(this->texInterface);

	//	デバッグ処理
#ifdef	_DEBUG
		if(	this->fontBase){
			this->fontBase	= NULL;
			CFONT_BASE::releaseInstance();
		}
#endif
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgFinishalizeScript
	//
	//	機能		スクリプトの解放
	//	更新		2008/11/07		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgFinishalizeScript(void)
	{
	//	スクリプト情報の解放
		if(	this->eventFile){	fclose(this->eventFile);	}
		this->eventStep		= eEVSTEP_FALSE;
		this->eventStartSwitch.clear();
		this->eventObject.clear();
		this->releaseEventExecuter();
		SAFE_DELETE(this->eventCommentG);
		SAFE_DELETE(this->eventInput);
		this->eventWaitKey	= MYKEY::eMAX;
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgCreateStage
	//
	//	機能		ゲームステージの生成
	//	機能概要	一つのステージ情報を読み込む
	//				・マップの生成				・初期キャラクター位置の設定
	//				・カメラの設定				・BGMの設定
	//				・イベントスイッチの設定	・背景の設定
	//				・その他の初期化
	//	引数		inStageNum		ステージ番号
	//	更新		2008/11/06		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgCreateStage(int inStageNum)
	{
	//	ステージ番号を確保
		this->iStageNum	= inStageNum;

	//-------------------------------------------------
	//	ファイルを開く
	//-------------------------------------------------
		FILE*	stageFile			= NULL;		//!< ステージテキストファイルポインター
		char	szString[MAX_PATH]	= "";		//!< テキスト名作成用

	//	テキスト名を生成
		sprintf_s(szString, sizeof(szString), PATH_GAMESTAGE, this->iStageNum);
	//	ファイルを開く
		fopen_s(&stageFile, szString, "r");

	//	ファイルを開けなかった時
		if(	!stageFile)
		{
		//	読み込みを行わず、処理を返す
			return;
		}

	//-------------------------------------------------
	//	読み込み開始
	//-------------------------------------------------
	//	読み込み用変数群
		VEC3	vec3	= VEC3(0.0f,0.0f,0.0f);
		int		num		= 0;

	//-------------------------------------------------
	//	プレイヤー開始位置
	//-------------------------------------------------
		fscanf_s(stageFile, "%f, %f, %f, ", &vec3.x, &vec3.y, &vec3.z);
		this->player->resetPlayer(&vec3);

	//-------------------------------------------------
	//	カメラの情報
	//-------------------------------------------------
	//	カメラの近視野、遠視野
		fscanf_s(stageFile, "%f, %f, ", &vec3.x, &vec3.y);
		this->camera->setViewNear(vec3.x);
		this->camera->setViewFar(vec3.y);
	//	視点シフト値
		fscanf_s(stageFile, "%f, %f, %f, ", &vec3.x, &vec3.y, &vec3.z);
		this->camera->setViewShift(&vec3);
	//	追跡対象からのシフト値
		fscanf_s(stageFile, "%f, %f, %f, ", &vec3.x, &vec3.y, &vec3.z);
		this->camera->setViewTargetShift(&vec3);

	//-------------------------------------------------
	//	ライト関連
	//-------------------------------------------------
	//	ライトのオフセット値
		fscanf_s(stageFile, "%f, %f,     ", &lightNearFar.x, &lightNearFar.y);
		fscanf_s(stageFile, "%f, %f, %f, ", &lightShift.x,   &lightShift.y,   &lightShift.z);
	//	ライト射影行列の設定
		D3DXMatrixPerspectiveFovLH(&this->lightProj, D3DXToRadian(90), 1.0f, lightNearFar.x, lightNearFar.y);

	//-------------------------------------------------
	//	マップの読み込み
	//-------------------------------------------------
		fscanf_s(stageFile, "%d, ", &num);
		this->gameMap->initialize(this->gDev->getDevice(), num);

	//-------------------------------------------------
	//	イベントの読み込み
	//-------------------------------------------------
		fscanf_s(stageFile, "%d, ", &num);
		this->sgLoadEventStartSwitch(num);

	//-------------------------------------------------
	//	バックサウンドの読み込み
	//-------------------------------------------------
		fscanf_s(stageFile, "%s ", szString, sizeof(szString));
		SAFE_DELETE(this->backSound);
		this->backSound	= new SSTREAMING(szString);

	//-------------------------------------------------
	//	背景の読み込み
	//-------------------------------------------------
		fscanf_s(stageFile, "%d, ", &num);
		SAFE_DELETE(this->back);
		switch(num)
		{
		//	精神世界
			case	eSGBACK_SKY:
				this->back = new CBACK_SKY(this->gDev->getDevice(), &this->playerChara->getLoc());
				break;
		}//	default	break;

	//	ファイルを閉じる
		fclose(stageFile);

	//-------------------------------------------------
	//	その他の共通初期化
	//-------------------------------------------------
	//	ゲームプレイへ変更
		this->gameFlag	= eSGFLAG_NO;
		this->sgPlayChange();
	//	シェーダの設定
		this->camera->viewTransform(this->gDev->getDevice());
	//	敵を生成する
		this->enemyManager->loadEnemyFromFile(
			this->gDev->getDevice(), this->iStageNum, &this->playerChara,
			this->gameMap->getViewG(), this->gameMap->getHitG(), this->gameMap->getEnemyG());
		this->sgCreateEnemy(this->iStageNum);
	//	バックサウンド再生
		this->backSound->play(0, DSBPLAY_LOOPING);
	//	ゲーム移行演出処理
	//	幕を黒くする
		this->gDev->setCurtainColor(0, 0, 0);
		this->gDev->setCurtainColorAlpha(255);
	//	幕を消滅
		this->gDev->curtainMotion(FALSE, 5);
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgLoadEventStartSwitch
	//
	//	機能		スクリプトの開始初期化
	//	引数		inStageNum		ステージ番号
	//	更新		2008/11/06		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgLoadEventStartSwitch(int inStageNum)
	{
	//-------------------------------------------------
	//	ファイルを開く
	//-------------------------------------------------
		FILE*	switchFile			= NULL;		//!< ステージテキストファイルポインター
		char	szString[MAX_PATH]	= "";		//!< テキスト名作成用

	//	テキスト名を生成
		sprintf_s(szString, sizeof(szString), PATH_EVENTSWITCH, this->iStageNum);
	//	ファイルを開く
		fopen_s(&switchFile, szString, "r");

	//	ファイルを開けなかった時
		if(	!switchFile)
		{
		//	読み込みを行わず、処理を返す
			return;
		}

	//	前のデータを削除
		this->eventStartSwitch.clear();
		this->stageChangeSwitch.clear();

	//-------------------------------------------------
	//	読み込み開始
	//-------------------------------------------------
		int		iEventNum	= 0;
		MY_CUBE	evPosition;

	//	▽▼▽▼▽▼▽▼	イベント開始スイッチを読み込むループ
		do
		{
			fscanf_s(switchFile, "%d, ", &iEventNum);
		//	イベント番号が不正なら終了
			if(	iEventNum < 0)	break;
		//	位置を読み込む
			fscanf_s(switchFile, "%f, %f, %f, %f, %f, %f, ",
				&evPosition.left,
				&evPosition.right,
				&evPosition.top,
				&evPosition.bottom,
				&evPosition.front,
				&evPosition.rear);
		//	位置をマップに登録
			this->eventStartSwitch.insert(pair<int, MY_CUBE>(iEventNum, evPosition));
		}
		while(1);
	//	▲△▲△▲△▲△	終了命令があるまで、繰り返す

	//	▽▼▽▼▽▼▽▼	ステージチェンジスイッチを読み込むループ
		do
		{
			fscanf_s(switchFile, "%d, ", &iEventNum);
		//	イベント番号が不正なら終了
			if(	iEventNum < 0)	break;
		//	位置を読み込む
			fscanf_s(switchFile, "%f, %f, %f, %f, %f, %f, ",
				&evPosition.left,
				&evPosition.right,
				&evPosition.top,
				&evPosition.bottom,
				&evPosition.front,
				&evPosition.rear);
		//	位置をマップに登録
			this->stageChangeSwitch.insert(pair<int, MY_CUBE>(iEventNum, evPosition));
		}
		while(1);
	//	▲△▲△▲△▲△	終了命令があるまで、繰り返す

	//	▽▼▽▼▽▼▽▼	ステージアウトスイッチを読み込むループ
		do
		{
			fscanf_s(switchFile, "%d, ", &iEventNum);
		//	イベント番号が不正なら終了
			if(	iEventNum < 0)	break;
		//	位置を読み込む
			fscanf_s(switchFile, "%f, %f, %f, %f, %f, %f, ",
				&evPosition.left,
				&evPosition.right,
				&evPosition.top,
				&evPosition.bottom,
				&evPosition.front,
				&evPosition.rear);
		//	位置をマップに登録
			this->stageOutSwitch.insert(pair<int, MY_CUBE>(iEventNum, evPosition));
		}
		while(1);
	//	▲△▲△▲△▲△	終了命令があるまで、繰り返す

	//	ファイル番号を保存
		this->eventStartSwitchNum	= inStageNum;

	//	ファイルを閉じる
		fclose(switchFile);
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgUpdateLight
	//
	//	機能		ライトの更新
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgUpdateLight(void)
	{
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
	//!	メソッド名	CSCENE_ADVENTURE_GAME::CSCENE_ADVENTURE_GAME
	//
	//	機能		コンストラクタ
	//	引数		inFlag			ゲームフラグ
	//				inData			シーン共有データ
	//	更新		2008/07/10		<新規>
	//				2008/11/06		<変更>	全体的に書き直し
	//================================================================================
	CSCENE_ADVENTURE_GAME::CSCENE_ADVENTURE_GAME(	SCENE_FLAG inFlag,
													SCENE_FLAG inClearFlag,
													SCENE_FLAG inQuitFlag,
													SCENE_SHARE_DATA* inData)	:
	CSCENE(inFlag, inFlag, inData)
	{
	//	ゲーム関連のNULL化
		this->gDev				= NULL;
		this->effectCenter		= NULL;
		this->playerChara		= NULL;
		this->player			= NULL;
		this->camera			= NULL;
		this->gameMap			= NULL;
		this->enemyManager		= NULL;
		this->back				= NULL;
		this->backSound			= NULL;
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;

		D3DXMatrixIdentity(&this->lightView);
		D3DXMatrixIdentity(&this->lightProj);
		this->lightNearFar		= VEC2(0.0f, 1000.0f);
		this->lightShift		= VEC3(0.0f, 0.0f, 0.0f);
		this->gameFlag			= eSGFLAG_NO;
		
		this->iStageNum			= 0;
		this->iNextStageNum		= 0;
		this->clearFlag			= inClearFlag;
		this->quitFlag			= inQuitFlag;

	//	スクリプト関連のNULL化
		this->eventFile				= NULL;
		this->eventStep				= eEVSTEP_FALSE;
		this->eventStartSwitchNum	= 0;
		this->eventCommentG			= NULL;
		this->eventWaitCnt			= 0;
		this->eventInput			= NULL;
		this->eventWaitKey			= MYKEY::eMAX;

	//	コンティニュー関連属性
		this->continueFrameCnt	= 0;
		this->continueChoice	= TRUE;
		this->continueFlag		= eSGCONTI_START;
		this->effContinue		= NULL;
		this->effContinueYes	= NULL;
		this->effContinueNo		= NULL;
		this->texInterface		= NULL;
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::~CSCENE_ADVENTURE_GAME
	//
	//	機能		デストラクタ
	//	更新		2008/07/10		<新規>
	//				2008/11/06		<変更>	全体的に書き直し
	//================================================================================
	CSCENE_ADVENTURE_GAME::~CSCENE_ADVENTURE_GAME(void)
	{
	//	解放
		this->sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sceneInitialize
	//
	//	機能		初期化
	//	引数		inNum			外部データ番号
	//	戻り値		TRUE			読み込み成功
	//				FALSE			読み込み失敗
	//	更新		2008/07/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sceneInitialize(int inStageNum)
	{
	//	ゲームの初期化
		this->sgInitializeGame();
	//	スクリプトの初期化
		this->sgInitializeScript();
	//	初期化処理
		this->sgCreateStage(inStageNum);
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sceneFinishalize
	//
	//	機能		解放
	//	更新		2008/07/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sceneFinishalize(void)
	{
	//	ゲーム情報の解放
		this->sgFinishalizeGame();
	//	スクリプト情報の解放
		this->sgFinishalizeScript();
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sceneMain
	//
	//	機能		シーンメイン
	//	更新		2008/09/19		<新規>
	//================================================================================
	SCENE_FLAG
	CSCENE_ADVENTURE_GAME::sceneMain(CINPUT* input)
	{
	//	ゲームフラグで処理分岐
		switch(this->gameFlag)
		{
		//	ゲームプレイ中
			case	eSGFLAG_PLAY:
				this->sgPlayExecute(input);
				this->sgPlayRender();
				break;
		//	ゲームイベント中
			case	eSGFLAG_EVENT:
				this->sgEventExecute(input);
				this->sgEventRender();
				break;
		//	コンティニュー中
			case	eSGFLAG_CONTINUE:
				this->sgContinueExecute(input);
				this->sgContinueRender();
				break;
		//	マップチェンジ中
			case	eSGFLAG_STAGE_CHANGE:
				this->sgStageChangeExecute(input);
				this->sgStageChangeRender();
				break;
		//	再スタート中
			case	eSGFLAG_RESET:
				break;
		//	ゲームクリア中
			case	eSGFLAG_CLEAR:
				this->sgClearExecute(input);
				this->sgClearRender();
				break;
		//	ゲーム終了中
			case	eSGFLAG_QUIT:
				this->sgQuitExecute(input);
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

#ifdef	_DEBUG
	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::drawDebug
	//
	//	機能		デバッグ用処理
	//	更新		2009/01/29		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::drawDebug(void)
	{
	//	フォントで情報を描画
		char	str[MAX_PATH]	= "";
		SPRITE	sprite			= this->gDev->getSprite();

		sprintf_s(str, sizeof(str), "CharacterPos : %10f, %10f, %10f",
			this->playerChara->getLocX(),
			this->playerChara->getLocY(),
			this->playerChara->getLocZ());

		this->fontBase->drawText(sprite, eFONT_DEBUG, str, 10, 10);
	}
#else
#ifdef	PRESENTATION
	void
	CSCENE_ADVENTURE_GAME::drawDebug(void)
	{
	}
#endif
#endif
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------