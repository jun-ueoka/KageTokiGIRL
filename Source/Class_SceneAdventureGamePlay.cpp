//***********************************************************************************
//!	ファイル名		Class_SceneAdventureGamePlay.cpp
//
//	機能			ゲームシーンのプレイ部分の実装
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

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgPlayChange
	//
	//	機能		ゲームプレイ移行時の初期化
	//	更新		2008/11/06		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgPlayChange(void)
	{
	//	ゲームプレイ中にフラグを変更できたら、準備する
		if(	this->flagChange(eSGFLAG_PLAY))
		{
		//	初期化時イベントチェック
			this->sgPlayEventCheck(&this->player->getPlayChara()->getLoc());
		//	プレイヤーのダメージ判定確認を有効にする
			this->player->setDamageCheck(TRUE);
		//	幕を消滅
			this->gDev->curtainMotion(FALSE, 5);
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgPlayEventCheck
	//
	//	機能		イベント発生チェック
	//	引数		inLoc			判定位置
	//	更新		2008/11/06		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgPlayEventCheck(VEC3* inLoc)
	{
	//	イベントが無い状態なら、チェックする
		if(	this->eventStep != eEVSTEP_FALSE)	return;

		map<int, MY_CUBE>::iterator	p;

	//	イベントスイッチ判定
		for(p  = this->eventStartSwitch.begin();
			p != this->eventStartSwitch.end();
			p++)
		{
		//	イベント発生位置に入ったら、イベント発生
			if(	p->second.intersect(inLoc))
			{
			//	イベントファイルを開く
				this->sgPlayOpenEventFile(p->first);
			//	イベントが発生済みなので、消す
				this->eventStartSwitch.erase(p);
				return;
			}
		}

	//	ステージチェンジ発生スイッチ判定
		for(p  = this->stageChangeSwitch.begin();
			p != this->stageChangeSwitch.end();
			p++)
		{
		//	ステージチェンジ発生発生位置に入ったら、イベント発生
			if(	p->second.intersect(inLoc))
			{
			//	ステージチェンジ中に変更
				this->sgStageChange(p->first);
				return;
			}
		}

	//	ステージアウト発生スイッチ判定
		for(p  = this->stageOutSwitch.begin();
			p != this->stageOutSwitch.end();
			p++)
		{
		//	ステージチェンジ発生発生位置に入ったら、イベント発生
			if(	p->second.intersect(inLoc))
			{
			//	ステージチェンジ中に変更
				this->sgStageOutChange();
				return;
			}
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgPlayOpenEventFile
	//
	//	機能		イベントファイルを開く
	//	引数		inEventNum		開くイベントファイル番号
	//	更新		2008/11/06		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgPlayOpenEventFile(int inEventNum)
	{
	//-------------------------------------------------
	//	ファイルを開く
	//-------------------------------------------------
		char	textName[MAX_PATH]	= "";		//!< テキスト名作成用

	//	テキスト名を生成
		sprintf_s(textName, sizeof(textName), PATH_EVENT, inEventNum);
	//	ファイルを開く
		fopen_s(&eventFile, textName, "r");

	//	ファイルを開けなかったら、イベントエラーの状態にする
		if(	!eventFile)
		{
			this->eventStep	= eEVSTEP_ERROR;
		}
	//	開けたら、読み込み中の状態にする
		else
		{
		//	ゲームイベント中に切り替え
			this->sgEventChange();
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgPlayExecute
	//
	//	機能		ゲームプレイ中処理
	//	引数		inInput			入力情報
	//	更新		2008/11/06		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgPlayExecute(CINPUT* inInput)
	{
#ifdef	_DEBUG
		if(	inInput->checkPress(MYKEY::eF2))
		{
			if(	inInput->checkPress(MYKEY::eDUST))
			{
				this->camera->calcViewTargetShiftZ(30.0f);
				this->camera->calcViewShiftZ(30.0f);
			}
			if(	inInput->checkPress(MYKEY::eSOFT))
			{
				this->camera->calcViewTargetShiftZ(-30.0f);
				this->camera->calcViewShiftZ(-30.0f);
			}
		}

		if(	inInput->checkPress(MYKEY::eF4))
		{
			if(	inInput->checkPress(MYKEY::eUP))
			{
				this->camera->calcViewTargetShiftY(30.0f);
				this->camera->calcViewShiftY(30.0f);
			}
			if(	inInput->checkPress(MYKEY::eDOWN))
			{
				this->camera->calcViewTargetShiftY(-30.0f);
				this->camera->calcViewShiftY(-30.0f);
			}
			if(	inInput->checkPress(MYKEY::eLEFT))
			{
				this->camera->calcViewTargetShiftX(-30.0f);
				this->camera->calcViewShiftX(-30.0f);
			}
			if(	inInput->checkPress(MYKEY::eRIGHT))
			{
				this->camera->calcViewTargetShiftX(30.0f);
				this->camera->calcViewShiftX(30.0f);
			}
		}
		else
		{
		//	プレイヤーキャラクターの操作
			this->playerChara->operation(inInput);
		}
	//	敵の更新
		this->enemyManager->updateEnemy(&this->camera->getViewPointFinish());
	//	プレイヤーの操作
		this->player->operation(inInput);
	//	イベント発生チェック
		this->sgPlayEventCheck(&this->playerChara->getLoc());
	//	死亡判定
		if(	this->player->getCharacterOver())	this->sgContinueChange();

#else
#ifdef	PRESENTATION
		if(	inInput->checkPress(MYKEY::eF2))
		{
			if(	inInput->checkPress(MYKEY::eDUST))
			{
				this->camera->calcViewTargetShiftZ(30.0f);
				this->camera->calcViewShiftZ(30.0f);
			}
			if(	inInput->checkPress(MYKEY::eSOFT))
			{
				this->camera->calcViewTargetShiftZ(-30.0f);
				this->camera->calcViewShiftZ(-30.0f);
			}
		}

		if(	inInput->checkPress(MYKEY::eF4))
		{
			if(	inInput->checkPress(MYKEY::eUP))
			{
				this->camera->calcViewTargetShiftY(30.0f);
				this->camera->calcViewShiftY(30.0f);
			}
			if(	inInput->checkPress(MYKEY::eDOWN))
			{
				this->camera->calcViewTargetShiftY(-30.0f);
				this->camera->calcViewShiftY(-30.0f);
			}
			if(	inInput->checkPress(MYKEY::eLEFT))
			{
				this->camera->calcViewTargetShiftX(-30.0f);
				this->camera->calcViewShiftX(-30.0f);
			}
			if(	inInput->checkPress(MYKEY::eRIGHT))
			{
				this->camera->calcViewTargetShiftX(30.0f);
				this->camera->calcViewShiftX(30.0f);
			}
		}
		else
		{
		//	プレイヤーキャラクターの操作
			this->playerChara->operation(inInput);
		}
	//	敵の更新
		this->enemyManager->updateEnemy(&this->playerChara->getLoc());
	//	プレイヤーの操作
		this->player->operation(inInput);
	//	イベント発生チェック
		this->sgPlayEventCheck(this->playerChara->getLoc());
	//	死亡判定
		if(	this->player->getCharacterOver())	this->sgContinueChange();
#else
	//	プレイヤーキャラクターの操作
		this->playerChara->operation(inInput);
	//	敵の更新
		this->enemyManager->updateEnemy(&this->playerChara->getLoc());
	//	プレイヤーの操作
		this->player->operation(inInput);
	//	イベント発生チェック
		this->sgPlayEventCheck(&this->playerChara->getLoc());
	//	死亡判定
		if(	this->player->getCharacterOver())	this->sgContinueChange();
#endif
#endif
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgPlayRender
	//
	//	機能		ゲームプレイ中描画
	//	更新		2008/11/06		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgPlayRender(void)
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
		this->player->drawInterface(this->gDev->getSprite());
#ifdef	_DEBUG
		this->drawDebug();
#else
#ifdef PRESENTATION
		this->drawDebug();
#endif
#endif
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