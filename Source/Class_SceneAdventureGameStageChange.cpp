//***********************************************************************************
//!	ファイル名		Class_SceneAdventureGameStageChange.cpp
//
//	機能			ゲームシーンのマップ変更処理部分の実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2008/07/10	<新規>
//					2008/11/06	<変更>	全体的に書き直し
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneAdventureGameData.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgStageChange
	//
	//	機能		ステージチェンジ移行時の初期化
	//	引数		inNewStage		新しいステージ番号
	//	更新		2008/11/13		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgStageChange(int inNewStage)
	{
	//	ステージチェンジ中にフラグを変更できたら、準備する
		if(	this->flagChange(eSGFLAG_STAGE_CHANGE))
		{
		//	ダミー入力情報を初期化
			this->eventInput->resetKey();
		//	自キャラの向いてる方向にダミーキーを入力
		//	左方向を向いている
			if(	(this->playerChara->getAngleY() >=  90.0f)	&&
				(this->playerChara->getAngleY() <  270.0f))
			{
				this->eventInput->setPress(MYKEY::eLEFT, true);
			}
		//	右方向を向いている
			else
			{
				this->eventInput->setPress(MYKEY::eRIGHT, true);
			}
		//	カメラの視点を停止
			this->camera->setCameraMode(eCAMO_PARALLEL);
		//	次のステージ番号を登録
			this->iNextStageNum	= inNewStage;
		//	プレイヤーのダメージ判定確認を無効にする
			this->player->setDamageCheck(FALSE);
		//	幕の出現
			this->gDev->curtainMotion(TRUE, 5);
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgStageChangeExecute
	//
	//	機能		ステージチェンジ中処理
	//	引数		inInput			入力情報
	//	更新		2008/11/13		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgStageChangeExecute(CINPUT* inInput)
	{
	//	キャラクターをダミー入力で操作
		this->playerChara->operation(this->eventInput);
	//	幕の不透明度が最大なら、ステージチェンジ
		if(	this->gDev->getCurtainColorAlpha() >= 255)
		{
		//	カメラの平衡移動モードにする
			this->camera->setCameraMode(eCAMO_PARALLEL_CHASE);
		//	ダミー入力情報を初期化
			this->eventInput->resetKey();
		//	ゲームの状態をゲームプレイ中に変更しておく
			this->sgPlayChange();
		//	幕が暗い内にステージ読み込み
			this->sgCreateStage(this->iNextStageNum);
		//	幕を開ける処理
			this->gDev->curtainMotion(FALSE, 5);
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgStageChangeRender
	//
	//	機能		ステージチェンジ中描画
	//	更新		2008/11/13		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgStageChangeRender(void)
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