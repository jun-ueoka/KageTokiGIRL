//***********************************************************************************
//!	ファイル名		Class_SceneAdventureGameReset.cpp
//
//	機能			ゲームシーンの再スタート処理部分の実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/02/03	<新規>
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
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgResetChange
	//
	//	機能		再スタート移行時の初期化
	//	更新		2009/02/03	<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgResetChange(void)
	{
	//	ゲーム終了中にフラグを変更できたら、準備する
		if(	this->flagChange(eSGFLAG_RESET))
		{
		//	プレイヤーのダメージ判定確認を無効にする
			this->player->setDamageCheck(FALSE);
		//	幕を出現させる
			this->gDev->curtainMotion(TRUE, 5);
			this->continueFrameCnt	= 0;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgResetExecute
	//
	//	機能		再スタート中処理
	//	引数		inInput			入力情報
	//	更新		2009/02/03		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgResetExecute(CINPUT* inInput)
	{
	//	幕の透明度が最大なら、タイトルへ
		if(	this->gDev->getCurtainColorAlpha() >= 255)
		{
		//	フレームによって処理分岐
			this->continueFrameCnt++;
		//	2フレーム目まで、キャラクターをリセット
			if		(this->continueFrameCnt < 2)
			{
				this->camera->setCameraMode(eCAMO_PARALLEL_CHASE);
				this->player->resetPlayer();
				this->enemyManager->resetEnemy();
				this->sgCreateEnemy(0);
			}
		//	60フレーム目にゲームプレイ中に変更
			else if	(this->continueFrameCnt > 30)
			{
				this->sgPlayChange();
			}
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgResetRender
	//
	//	機能		再スタート中描画
	//	更新		2009/02/03		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgResetRender(void)
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
		//this->gDev->spriteBegin();
		//this->gDev->spriteEnd();

	//	幕の描画
		this->gDev->renderCurtainInDev();
	//	描画終了
		this->gDev->renderEnd();
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------