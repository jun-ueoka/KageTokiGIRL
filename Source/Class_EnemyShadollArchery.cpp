//***********************************************************************************
//!	ファイル名		Class_EnemyShadollArchery.cpp
//
//	機能			敵キャラクタークラス「シャドールアーチェリー」の実装
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2009/02/20	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_EnemyShadollArchery.h"		//!< アドベンチャーゲーム「シャドールアーチェリー」

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL_ARCHERY::コンストラクタ
	//
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		メッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2009/02/20		<新規>
	//================================================================================
	CENEMY_SHADOLL_ARCHERY::CENEMY_SHADOLL_ARCHERY(
									CONST DEV	inDev,
									CONST LPSTR	inName,
									CONST LPSTR	inResource,
									CONST LPSTR	inTexture)	:
	CKAGETOKI_ADVENT_ENEMY(inDev, inName, inResource, inTexture)
	{
	//	属性の初期化
		this->fFloorSpace				= 15.0f;				//!< 床との間隔
		this->fWallSpace				= 10.0f;				//!< 壁との間隔

	//	アニメーションの割り当てを行う
		this->iAnimTemp[eESHADOLLARCHERYA_WAIT]			= 0;
		this->iAnimTemp[eESHADOLLARCHERYA_ARROWINIT]	= 1;
		this->iAnimTemp[eESHADOLLARCHERYA_ARROW]		= 2;

		this->iMaxLife					= 2;					//!< 最大体力
		this->iLife						= this->iMaxLife;		//!< 残り体力
		this->iHitFrame					= 0;					//!< 衝突して点滅する時間

	//	フラグ群
		this->action			= eESHADOLLARCHERYA_MAX;		//!< 最初は待機以外何でも良い
		this->changeAction(eESHADOLLARCHERYA_WAIT);				//!< 初期状態は出現
		this->setThisMessage(eTMSG_ENEMY);						//!< 敵表示

	//	オリジナル「エネミーボール」の生成
		this->originArrow
			= new CENEMY_BALL(inDev, eENEMYB_SHADOLL_ARROW, "enemy_shadoll_archery_arrow", EFFECT_SMOKE);
		this->originArrow->setEffectColor(D3DCOLOR_RGBA(100,100,100,255));
		this->originArrow->setReactMessage(eREAMSG_ENEMY_ATTACK_LV1);
		this->originArrow->setEffectCnt(3);
		this->ssArrowShot	= NULL;

	//	エフェクトの登録
		C3D_EFFECT_CENTER*	effectCenter	= C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			EFFECT_SMOKE, new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));

	//	ローカル座標の変更
		this->sd_ptr->mesh->localCenter = VEC3(0.0f, 50.0f, 0.0f);
		this->sd_ptr->mesh->localRadius	= 150.0f;
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL_ARCHERY::デストラクタ
	//
	//	更新		2009/02/20		<新規>
	//================================================================================
	CENEMY_SHADOLL_ARCHERY::~CENEMY_SHADOLL_ARCHERY(void)
	{
	//	解放
		SAFE_DELETE(this->originArrow);
		SAFE_DELETE(this->ssArrowShot);
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL_ARCHERY::opeWait
	//
	//	機能		待機時操作
	//	更新		2009/02/20		<新規>
	//================================================================================
	void
	CENEMY_SHADOLL_ARCHERY::opeWait(void)
	{
	//	重力効果
		this->opeEffectGravity();
	//	キャラクターの方に方向転換
		if(	(*this->player)->getLocX() < this->getLocX())
		{
			this->way = eADCW_LEFT_TURN;
		}
		else
		{
			this->way = eADCW_RIGHT_TURN;
		}
		this->opeTurn();

	//	プレイヤーに当たったら、ダメージ
		if(	(*this->player)->intersectRadius(this))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

	//	フレームが一定まで上がったら待機
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10)
		{
			this->changeAction(eESHADOLLARCHERYA_ARROWINIT);
		}
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL_ARCHERY::opeArrowInit
	//
	//	機能		歩行時操作
	//	更新		2009/02/20		<新規>
	//================================================================================
	void
	CENEMY_SHADOLL_ARCHERY::opeArrowInit(void)
	{
	//	重力効果
		this->opeEffectGravity();

	//	プレイヤーに当たったら、ダメージ
		if(	(*this->player)->intersectRadius(this))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

	//	フレームが一定まで上がったら待機
		this->iFrameCnt++;
		if(	this->iFrameCnt > 60)
		{
			this->changeAction(eESHADOLLARCHERYA_ARROW);
		}
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL_ARCHERY::opeArrow
	//
	//	機能		ダッシュ時操作
	//	更新		2009/02/20		<新規>
	//================================================================================
	void
	CENEMY_SHADOLL_ARCHERY::opeArrow(void)
	{
	//	重力効果
		this->opeEffectGravity();

	//	プレイヤーに当たったら、ダメージ
		if(	(*this->player)->intersectRadius(this))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

	//	フレームが一定まで上がったら待機
		this->iFrameCnt++;
		if(	this->iFrameCnt > 30)
		{
			this->changeAction(eESHADOLLARCHERYA_WAIT);
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::skillArrowShot
	//
	//	機能		技「アローショット」
	//	更新		2009/02/21		<新規>
	//================================================================================
	void
	CENEMY_SHADOLL_ARCHERY::skillArrowShot(void)
	{
	//	アローを生成
		CENEMY_BALL*	newArrow	= new CENEMY_BALL(this->originArrow);

	//	アローの設定
		newArrow->shot(
			&this->getAngle(),								//!< 角度
			&this->getWorldCenter(),						//!< 発射位置
			this->stageG,									//!< ステージオブジェクトグループ
			this->player,									//!< 敵オブジェクトグループ
			100);											//!< 平行飛翔時間
		newArrow->setMoveQuantity(20.0f);					//!< スピード設定
		newArrow->setScale(1.0f);							//!< 拡大
		newArrow->setLine(ADVENT_FRONTHIT);					//!< 向き(3D空間に対して)
		newArrow->setOrderMessage(eOMSG_DELETE_IN);			//!< 他解放
	//	アロー発射音

	//	アローを描画グループに登録
		this->viewG->add(newArrow);
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL_ARCHERY::changeAction
	//
	//	機能		状態変化
	//	引数		inNewAction		新たな行動パターン
	//	戻り値		TRUE			新たな行動パターンに変化した
	//				FALSE			新たな行動パターンに変化していない
	//	更新		2009/02/20		<新規>
	//================================================================================
	BOOL
	CENEMY_SHADOLL_ARCHERY::changeAction(ENEMYSHADOLLARCHERY_ACTION inNewAction)
	{
	//	前の状態と新たな状態が変らないなら、処理しない
		if(	this->action == inNewAction)	return	FALSE;
	//	上記の処理をスルーしたので、状態の変更
		this->action = inNewAction;
	//	アニメーションも変更
		this->changeAnimationAndBlend(this->iAnimTemp[this->action]);

	//	新たな状態で追加処理があるならここで
		switch(this->action)
		{
			case	eESHADOLLARCHERYA_WAIT:			//!< 待機時
				this->setMoveQuantity(0.0f);
				break;
			case	eESHADOLLARCHERYA_ARROWINIT:	//!< 矢の準備
				this->setMoveQuantity(0.0f);
				break;
			case	eESHADOLLARCHERYA_ARROW:		//!< 矢を射る
				this->setMoveQuantity(0.0f);
				this->skillArrowShot();
				break;
			default:								//!< 上記以外
				break;
		}

	//	フレームカウンタを初期化
		this->iFrameCnt	= 0;

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL_ARCHERY::reactionMessage
	//
	//	機能		リアクション
	//	更新		2009/02/20		<新規>
	//================================================================================
	void
	CENEMY_SHADOLL_ARCHERY::reactionMessage(REACTION_MESSAGE inMessage)
	{
	//	エナジーボールに当たったら
		if(	inMessage == eREAMSG_ENERGY_HIT)
		{
		//	衝突フレームが0以下なら、ダメージ
			if(	this->iHitFrame <= 0)
			{
				this->iHitFrame  = 100;
				this->iLife--;
			}
		}

	//	ライフがなくたったら、消える
		if(	this->iLife <= 0)
		{
			this->setVisible(FALSE);
		}
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL_ARCHERY::operation
	//
	//	機能		基本操作
	//	引数		*input			入力管理
	//	更新		2009/02/20		<新規>
	//================================================================================
	void
	CENEMY_SHADOLL_ARCHERY::operation(CINPUT* input)
	{
	//	表示されていなければ、処理しない
		if(	!this->getVisible())	return;
	//	プレイヤーが設定されていなければ処理しない
		if(	!(*this->player))		return;

	////	グループの登録がなければ、処理しない
	//	if(	!this->viewG	||
	//		!this->stageG	||
	//		!this->enemyG)	return;

	//	状態による操作の分岐
		switch(this->action)
		{
			case	eESHADOLLARCHERYA_WAIT:			//!< 待機時
				this->opeWait();
				break;
			case	eESHADOLLARCHERYA_ARROWINIT:	//!< 矢の準備
				this->opeArrowInit();
				break;
			case	eESHADOLLARCHERYA_ARROW:		//!< 矢を射る
				this->opeArrow();
				break;
		}//	default	break;
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL_ARCHERY::draw
	//
	//	機能		描画
	//	引数		inDev			Direct3Dデバイス
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2009/02/20		<新規>
	//================================================================================
	BOOL
	CENEMY_SHADOLL_ARCHERY::draw(CONST DEV inDev)
	{
	//	点滅処理をしながら描画
		if(	this->iHitFrame > 0)
		{
			if(	this->iHitFrame % 2){ CSKIN_MESH::draw(inDev);	}
			else					{ this->animation();		}
			this->iHitFrame--;
		}
	//	点滅してないなら、普通描画
		else
		{
			CSKIN_MESH::draw(inDev);
		}

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------