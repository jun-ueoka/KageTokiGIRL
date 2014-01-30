//***********************************************************************************
//!	ファイル名		Class_SceneAdventureGameStageOut.cpp
//
//	機能			ゲームシーンのステージアウト処理部分の実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/02/27	<新規>
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
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgStageOutChange
	//
	//	機能		ステージアウト移行時の初期化
	//	更新		2009/02/27	<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgStageOutChange(void)
	{
	//	ゲーム終了中にフラグを変更できたら、準備する
		if(	this->flagChange(eSGFLAG_STAGE_OUT))
		{
		//	プレイヤーのダメージ判定確認を無効にする
			this->player->setDamageCheck(FALSE);
		//	カメラのモードを変更
			this->camera->setCameraMode(eCAMO_PARALLEL);
		//	コンティニューフレームを初期化
			this->continueFrameCnt	= 0;
		//	死亡エフェクト発生
			this->player->occuPlayerEffect(eADPEFF_DATHE);
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgStageOutExecute
	//
	//	機能		ステージアウト中処理
	//	引数		inInput			入力情報
	//	更新		2009/02/27		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgStageOutExecute(CINPUT* inInput)
	{
	//	プレイヤーの操作
		this->playerChara->operation(this->eventInput);
	//	フレームによって処理分岐
		this->continueFrameCnt++;
	//	60フレーム目にゲームプレイ中に変更
		if(	this->continueFrameCnt > 60)
		{
			this->sgContinueChange();
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgStageOutRender
	//
	//	機能		ステージアウト中描画
	//	更新		2009/02/27		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgStageOutRender(void)
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