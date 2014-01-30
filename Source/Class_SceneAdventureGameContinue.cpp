//***********************************************************************************
//!	ファイル名		Class_SceneAdventureGameContinue.cpp
//
//	機能			ゲームシーンのコンティニュー処理部分の実装
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
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgContinueChange
	//
	//	機能		コンティニュー移行時の初期化
	//	更新		2008/11/12		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgContinueChange(void)
	{
	//	コンティニュー中にフラグを変更できたら、準備する
		if(	this->flagChange(eSGFLAG_CONTINUE))
		{
		//	使用する変数の初期化
			this->continueFrameCnt	= 0;
			this->continueFlag		= eSGCONTI_START;
			this->continueChoice	= TRUE;

		//	プレイヤーを煙で消す
			this->player->setAllVisible(FALSE);
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE, this->playerChara->getLoc(), 3.0f);

		//	コンティニュー関連属性
			SAFE_DELETE(this->effContinue);
			SAFE_DELETE(this->effContinueYes);
			SAFE_DELETE(this->effContinueNo);
			SAFE_DELETE(this->texInterface);
			this->effContinue	 =
				new C3D_EFFECT(this->gDev->getDevice(), EFFECT_CONTINUE,     VEC3(512.0f, 200.0f, 0.0f));
			this->effContinueYes =
				new C3D_EFFECT(this->gDev->getDevice(), EFFECT_CONTINUE_YES, VEC3(312.0f, 400.0f, 0.0f));
			this->effContinueNo	 =
				new C3D_EFFECT(this->gDev->getDevice(), EFFECT_CONTINUE_NO,  VEC3(712.0f, 400.0f, 0.0f));
			this->texInterface	 =
				new CTEXTURE_BASE(this->gDev->getDevice(), "game_interface_player", VEC2(1.0f, 1.0f), 0xFFFFFF96);

		//	コンティニュー属性を初期化
			this->effContinue->setVisible(FALSE);
			this->effContinueYes->setVisible(FALSE);
			this->effContinueNo->setVisible(FALSE);

		//	プレイヤーのダメージ判定確認を無効にする
			this->player->setDamageCheck(FALSE);

		//	残気がなければ、ゲームオーバー
			if(	this->ssData->dwPlayerLife <= 0)
			{
				this->continueFlag	= eSGCONTI_GAMEOVER;
			}
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgContinueExecute
	//
	//	機能		コンティニュー中処理
	//	引数		inInput			入力情報
	//	更新		2008/11/12		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgContinueExecute(CINPUT* inInput)
	{
	//	プレイヤーの操作
		this->player->operation(this->eventInput);
	//	プレイヤーキャラクターの操作
		this->playerChara->operation(this->eventInput);

	//	コンティニュー処理フラグ分岐
		switch(this->continueFlag)
		{
		//	コンティニュー処理開始
			case eSGCONTI_START:
			//	フレームカウンタが既定のフレーム数まで上がったら、
			//	ブラックアウト
				this->continueFrameCnt++;
				if(	this->continueFrameCnt > 60)
				{
					this->continueFlag		= eSGCONTI_BLACKOUT;
					this->continueFrameCnt	= 0;
					this->gDev->setCurtainColor(0, 0, 0);
					this->gDev->setCurtainColorAlpha(0);
					this->gDev->curtainMotion(TRUE, 5);
				}
				break;
		//	ブラックアウト中
			case eSGCONTI_BLACKOUT:
			//	幕の透明度が150以上なら、コンティニュー選択処理移行中へ
				if(	this->gDev->getCurtainColorAlpha() >= 150)
				{
					this->gDev->setCurtainAction(FALSE);
					this->gDev->setCurtainZLoc(0.1f);
					this->continueFlag	= eSGCONTI_CONTINUE_INIT;
					this->effContinue->setVisible(TRUE);
					C3D_EFFECT_CENTER::occu2DEffectsInstance(
						EFFECT_SMOKE, this->effContinue->getLoc());
				}
				break;
		//	コンティニュー選択処理移行中
			case eSGCONTI_CONTINUE_INIT:
			//	フレームカウンタが既定のフレーム数まで上がったら、
			//	コンティニュー選択処理中へ
				this->continueFrameCnt++;
				if(	this->continueFrameCnt > 60)
				{
					this->continueFlag		= eSGCONTI_CONTINUE;
					this->continueFrameCnt	= 0;
					this->effContinueYes->setVisible(TRUE);
					this->effContinueNo->setVisible(TRUE);
					C3D_EFFECT_CENTER::occu2DEffectsInstance(
						EFFECT_CONTINUE_YES_SMOKE, this->effContinueYes->getLoc());
					C3D_EFFECT_CENTER::occu2DEffectsInstance(
						EFFECT_CONTINUE_NO_SMOKE,  this->effContinueNo->getLoc());
				}
				break;
		//	コンティニュー選択処理中
			case eSGCONTI_CONTINUE:
			//	「YES」選択中
				if(	this->continueChoice)
				{
				//	「YES」は動き、「NO」は停止中
					this->effContinueYes->setMotionBool(TRUE);
					this->effContinueYes->setShowSize(VEC2(400.0f, 200.0f));
					this->effContinueNo->setMotionBool(FALSE);
					this->effContinueNo->setShowSize(VEC2(200.0f, 100.0f));
				//	右が押されたら、「NO」へ
					if(	inInput->checkPressOnce(MYKEY::eRIGHT))
					{
						this->continueChoice	= FALSE;
						C3D_EFFECT_CENTER::occu2DEffectsInstance(
							EFFECT_CONTINUE_NO_SMOKE,  this->effContinueNo->getLoc());
					}
				//	決定キーが押されたら、「YES」選択後処理中へ
					if(	inInput->checkPressOnce(MYKEY::eSTART))
					{
						this->continueFrameCnt	= 0;
						this->continueFlag		= eSGCONTI_CHOICE_YES;
						C3D_EFFECT_CENTER::occu2DEffectsInstance(
							EFFECT_CONTINUE_YES_SMOKE,  this->effContinueYes->getLoc(), 1.5f);
						C3D_EFFECT_CENTER::occu2DEffectsInstance(
							EFFECT_SMOKE,
							VEC3(float(300+(100*(this->ssData->dwPlayerLife-1))+(200/2)), 600.0f, 0.0f));
						this->ssData->dwPlayerLife--;
					}
				}
			//	「NO」選択中
				else
				{
				//	「NO」は動き、「YES」は停止中
					this->effContinueYes->setMotionBool(FALSE);
					this->effContinueYes->setShowSize(VEC2(200.0f, 100.0f));
					this->effContinueNo->setMotionBool(TRUE);
					this->effContinueNo->setShowSize(VEC2(400.0f, 200.0f));
				//	左が押されたら、「YES」へ
					if(	inInput->checkPressOnce(MYKEY::eLEFT))
					{
						this->continueChoice	= TRUE;
						C3D_EFFECT_CENTER::occu2DEffectsInstance(
							EFFECT_CONTINUE_YES_SMOKE, this->effContinueYes->getLoc());
					}
				//	決定キーが押されたら、「NO」選択後処理中へ
					if(	inInput->checkPressOnce(MYKEY::eSTART))
					{
						this->continueFrameCnt	= 0;
						this->continueFlag		= eSGCONTI_CHOICE_NO;
						C3D_EFFECT_CENTER::occu2DEffectsInstance(
							EFFECT_CONTINUE_NO_SMOKE,  this->effContinueNo->getLoc(), 1.5f);
					}
				}
				break;
		//	「YES」選択後処理中
			case eSGCONTI_CHOICE_YES:
			//	フレームカウンタが既定のフレーム数まで上がったら、
			//	リセットへ
				this->continueFrameCnt++;
				if(	this->continueFrameCnt > 60)
				{
					this->gDev->setCurtainZLoc(0.0f);
					this->sgResetChange();
				}
				break;
		//	「NO」選択後処理中
			case eSGCONTI_CHOICE_NO:
			//	フレームカウンタが既定のフレーム数まで上がったら、
			//	ゲームオーバーへ
				this->continueFrameCnt++;
				if(	this->continueFrameCnt > 60)
				{
					this->gDev->setCurtainZLoc(0.0f);
					this->sgQuitChange();
				}
				break;
		//	強制ゲームオーバー処理中
			case eSGCONTI_GAMEOVER:
			//	フレームカウンタが既定のフレーム数まで上がったら、
			//	ゲームオーバーへ
				this->continueFrameCnt++;
				if(	this->continueFrameCnt > 60)	
				{
					this->sgQuitChange();
				}
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgContinueRender
	//
	//	機能		コンティニュー中描画
	//	更新		2008/11/12		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgContinueRender(void)
	{
	//	デバイスを確保
		DEV		device	= this->gDev->getDevice();
		SPRITE	sprite	= this->gDev->getSprite();

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

	//	コンティニュー選択処理移行中の描画
		if(	this->continueFlag == eSGCONTI_CONTINUE_INIT)
		{
			this->effContinue->draw(sprite);
			this->effectCenter->drawEffects2D(sprite);
		}
	//	コンティニュー選択処理中の描画
		else
		if(	(this->continueFlag == eSGCONTI_CONTINUE)	||
			(this->continueFlag == eSGCONTI_CHOICE_YES)	||
			(this->continueFlag == eSGCONTI_CHOICE_NO))
		{
			this->effContinue->draw(sprite);
			this->effContinueYes->draw(sprite);
			this->effContinueNo->draw(sprite);
			this->effectCenter->drawEffects2D(sprite);

		//	残気の描画
			RECT	rcFrom	= {192,  0,256, 64};
			RECT	rcTo	= {  0,  0,  0,  0};
			LOOP((int)this->ssData->dwPlayerLife)
			{
				SetRect(&rcTo, 300+(100*i), 500, 300+200+(100*i), 500+200);
				this->texInterface->drawTexture(sprite, rcFrom, rcTo);
			}
		}
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