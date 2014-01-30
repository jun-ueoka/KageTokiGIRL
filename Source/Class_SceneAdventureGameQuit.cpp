//***********************************************************************************
//!	ファイル名		Class_SceneAdventureGameQuit.cpp
//
//	機能			ゲームシーンのゲーム終了処理部分の実装
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
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgQuitChange
	//
	//	機能		ゲーム終了移行時の初期化
	//	更新		2008/11/13		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgQuitChange(void)
	{
	//	ゲーム終了中にフラグを変更できたら、準備する
		if(	this->flagChange(eSGFLAG_QUIT))
		{
		//	プレイヤーのダメージ判定確認を無効にする
			this->player->setDamageCheck(FALSE);
		//	幕を出現させる
			this->gDev->curtainMotion(TRUE, 5);
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgQuitExecute
	//
	//	機能		ゲーム終了中処理
	//	引数		inInput			入力情報
	//	更新		2008/11/13		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgQuitExecute(CINPUT* inInput)
	{
	//	幕の透明度が最大なら、タイトルへ
		if(	this->gDev->getCurtainColorAlpha() >= 255)
		{
			this->returnFlag	= this->quitFlag;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgQuitRender
	//
	//	機能		ゲーム終了中描画
	//	更新		2008/11/13		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgQuitRender(void)
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