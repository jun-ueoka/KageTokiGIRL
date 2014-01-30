//***********************************************************************************
//!	ファイル名		Class_EnemyShadoll.cpp
//
//	機能			敵キャラクタークラス「シャドール」の実装
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2009/01/15	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_EnemyShadoll.h"		//!< アドベンチャーゲーム「シャドール」

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL::コンストラクタ
	//
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		メッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2009/01/15		<新規>
	//================================================================================
	CENEMY_SHADOLL::CENEMY_SHADOLL(	CONST DEV	inDev,
									CONST LPSTR	inName,
									CONST LPSTR	inResource,
									CONST LPSTR	inTexture)	:
	CKAGETOKI_ADVENT_ENEMY(inDev, inName, inResource, inTexture)
	{
	//	属性の初期化
		this->fFloorSpace				= 15.0f;				//!< 床との間隔
		this->fWallSpace				= 10.0f;				//!< 壁との間隔

	//	アニメーションの割り当てを行う
		this->iAnimTemp[eESHADOLLA_WAIT]= 0;
		this->iAnimTemp[eESHADOLLA_WALK]= 1;
		this->iAnimTemp[eESHADOLLA_DASH]= 2;

		this->iMaxLife					= 1;					//!< 最大体力
		this->iLife						= this->iMaxLife;		//!< 残り体力
		this->iHitFrame					= 0;					//!< 衝突して点滅する時間

	//	フラグ群
		this->action			= eESHADOLLA_MAX;				//!< 最初は待機以外何でも良い
		this->changeAction(eESHADOLLA_WAIT);					//!< 初期状態は出現
		this->setThisMessage(eTMSG_ENEMY);						//!< 敵表示

		this->stepCnt			= 0;
	//	音のセット
		this->sStaticStep		= new SSTATIC(SOUND_STEP);
		this->sStaticJumpStart	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticJumpEnd	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticStep->setVolume(-1500);
		this->sStaticJumpStart->setVolume(-1500);
		this->sStaticJumpEnd->setVolume(-1500);

	//	エフェクトの登録
		C3D_EFFECT_CENTER*	effectCenter	= C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			EFFECT_SMOKE, new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));

	//	ローカル座標の変更
		this->sd_ptr->mesh->localCenter = VEC3(0.0f, 50.0f, 0.0f);
		this->sd_ptr->mesh->localRadius	= 100.0f;
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL::デストラクタ
	//
	//	更新		2009/01/15		<新規>
	//================================================================================
	CENEMY_SHADOLL::~CENEMY_SHADOLL(void)
	{
	//	音関連の解放
		SAFE_DELETE(this->sStaticStep);				//!< 足音
		SAFE_DELETE(this->sStaticJumpStart);		//!< ジャンプ音
		SAFE_DELETE(this->sStaticJumpEnd);			//!< 着地音
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL::soundStep
	//
	//	機能		足音管理
	//	更新		2009/01/15		<新規>
	//================================================================================
	void
	CENEMY_SHADOLL::soundStep(void)
	{
		this->stepCnt++;

	//	スモークは10フレーム毎に発生
		if(	this->stepCnt > 10)
		{
			C3D_EFFECT_CENTER::occuEffectsInstance(EFFECT_SMOKE, this->getLoc(), 0.3f);
			this->sStaticStep->setPlayCur(0);
			this->sStaticStep->play(0, NULL);
			this->stepCnt	= 0;
		}
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL::soundJumpStart
	//
	//	機能		ジャンプ音管理
	//	更新		2009/01/15		<新規>
	//================================================================================
	void
	CENEMY_SHADOLL::soundJumpStart(void)
	{
	//	ジャンプ音を鳴らす
		this->sStaticJumpStart->play(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL::soundJumpEnd
	//
	//	機能		着地音管理
	//	更新		2009/01/15		<新規>
	//================================================================================
	void
	CENEMY_SHADOLL::soundJumpEnd(void)
	{
	//	着地音を鳴らす
		this->sStaticJumpEnd->play(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL::soundDash
	//
	//	機能		ダッシュ音管理
	//	更新		2009/01/15		<新規>
	//================================================================================
	void
	CENEMY_SHADOLL::soundDash(void)
	{
		this->stepCnt++;

	//	スモークは5フレーム毎に発生
		if(	this->stepCnt > 5)
		{
			C3D_EFFECT_CENTER::occuEffectsInstance(EFFECT_SMOKE, this->getLoc(), 0.3f);
			this->sStaticStep->setPlayCur(0);
			this->sStaticStep->play(0, NULL);
			this->stepCnt	= 0;
		}
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL::opeWait
	//
	//	機能		待機時操作
	//	更新		2009/01/15		<新規>
	//================================================================================
	void
	CENEMY_SHADOLL::opeWait(void)
	{
	//	重力効果
		this->opeEffectGravity();
	//	方向転換
		this->opeTurn();

	//	プレイヤーに当たったら、ダメージ
		if(	(*this->player)->intersect(
					this, ADVENT_FRONTHIT, this->fWallSpace))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

	//	フレームが一定まで上がったら待機
		this->iFrameCnt++;
		if(	this->iFrameCnt > 60)
		{
			this->changeAction(eESHADOLLA_WALK);
		}
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL::opeWalk
	//
	//	機能		歩行時操作
	//	更新		2009/01/15		<新規>
	//================================================================================
	void
	CENEMY_SHADOLL::opeWalk(void)
	{
	//	重力効果
		this->opeEffectGravity();
	//	足音
		this->soundStep();

	//	移動する値を確保
		float	moveValue	= this->fWallSpace + this->getMoveQuantity() + 100.0f;

	//	行動範囲から出ようとしたら待機
		if(	!this->actionArea.intersect(
				this->calcNextLoc(moveValue, &LINE(eUD_NO, eLR_NO, ADVENT_FRONTHIT))))
		{
		//	方向転換
			switch(this->way)
			{
				case eADCW_LEFT:	this->way = eADCW_RIGHT_TURN;	break;
				case eADCW_RIGHT:	this->way = eADCW_LEFT_TURN;	break;
			}
			this->changeAction(eESHADOLLA_WAIT);
			return;
		}

		else
	//	突進し、壁に当たったら待機
		if(	this->stageG->intersect(this, ADVENT_FRONTHIT, moveValue))
		{
		//	方向転換
			switch(this->way)
			{
				case eADCW_LEFT:	this->way = eADCW_RIGHT_TURN;	break;
				case eADCW_RIGHT:	this->way = eADCW_LEFT_TURN;	break;
			}
			this->changeAction(eESHADOLLA_WAIT);
			return;
		}
		else
	//	プレイヤーに当たったら、ダメージ
		if(	(*this->player)->intersect(this, ADVENT_FRONTHIT, this->getMoveQuantity()))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

	//	ここまで来たら、移動
		ADVENT_FRONTMOVE;

	//	フレームが一定まで上がったら待機
		this->iFrameCnt++;
		if(	this->iFrameCnt > 180)
		{
			this->changeAction(eESHADOLLA_DASH);
		}
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL::opeDash
	//
	//	機能		ダッシュ時操作
	//	更新		2009/01/15		<新規>
	//================================================================================
	void
	CENEMY_SHADOLL::opeDash(void)
	{
	//	重力効果
		this->opeEffectGravity();
	//	足音
		this->soundDash();

	//	移動する値を確保
		float	moveValue	= this->fWallSpace + this->getMoveQuantity() + 100.0f;

	//	行動範囲から出ようとしたら待機
		if(	!this->actionArea.intersect(
				this->calcNextLoc(moveValue, &LINE(eUD_NO, eLR_NO, ADVENT_FRONTHIT))))
		{
		//	方向転換
			switch(this->way)
			{
				case eADCW_LEFT:	this->way = eADCW_RIGHT_TURN;	break;
				case eADCW_RIGHT:	this->way = eADCW_LEFT_TURN;	break;
			}
			this->changeAction(eESHADOLLA_WAIT);
			return;
		}

		else
	//	突進し、壁に当たったら待機
		if(	this->stageG->intersect(this, ADVENT_FRONTHIT, moveValue))
		{
		//	方向転換
			switch(this->way)
			{
				case eADCW_LEFT:	this->way = eADCW_RIGHT_TURN;	break;
				case eADCW_RIGHT:	this->way = eADCW_LEFT_TURN;	break;
			}
			this->changeAction(eESHADOLLA_WAIT);
			return;
		}

		else
	//	プレイヤーに当たったら、ダメージ
		if(	(*this->player)->intersect(this, ADVENT_FRONTHIT, this->getMoveQuantity()))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

	//	ここまで来たら、移動
		ADVENT_FRONTMOVE;

	//	フレームが一定まで上がったら待機
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10000)
		{
			this->changeAction(eESHADOLLA_WAIT);
		}
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL::changeAction
	//
	//	機能		状態変化
	//	引数		inNewAction		新たな行動パターン
	//	戻り値		TRUE			新たな行動パターンに変化した
	//				FALSE			新たな行動パターンに変化していない
	//	更新		2009/01/15		<新規>
	//================================================================================
	BOOL
	CENEMY_SHADOLL::changeAction(ENEMYSHADOLL_ACTION inNewAction)
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
			case	eESHADOLLA_WAIT:			//!< 待機時
				this->setMoveQuantity(0.0f);
				break;
			case	eESHADOLLA_WALK:			//!< 歩行時
				this->setMoveQuantity(5.0f);
				break;
			case	eESHADOLLA_DASH:			//!< ダッシュ時
				this->setMoveQuantity(20.0f);
				break;
			default:							//!< 上記以外
				break;
		}

	//	フレームカウンタを初期化
		this->iFrameCnt	= 0;

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL::reactionMessage
	//
	//	機能		リアクション
	//	更新		2009/01/15		<新規>
	//================================================================================
	void
	CENEMY_SHADOLL::reactionMessage(REACTION_MESSAGE inMessage)
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
	//!	メソッド名	CENEMY_SHADOLL::operation
	//
	//	機能		基本操作
	//	引数		*input			入力管理
	//	更新		2009/01/15		<新規>
	//================================================================================
	void
	CENEMY_SHADOLL::operation(CINPUT* input)
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
			case	eESHADOLLA_WAIT:			//!< 待機時
				this->opeWait();
				break;
			case	eESHADOLLA_WALK:			//!< 歩行時
				this->opeWalk();
				break;
			case	eESHADOLLA_DASH:			//!< ダッシュ時
				this->opeDash();
				break;
		}//	default	break;
	}

	//================================================================================
	//!	メソッド名	CENEMY_SHADOLL::draw
	//
	//	機能		描画
	//	引数		inDev			Direct3Dデバイス
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2009/01/15		<新規>
	//================================================================================
	BOOL
	CENEMY_SHADOLL::draw(CONST DEV inDev)
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