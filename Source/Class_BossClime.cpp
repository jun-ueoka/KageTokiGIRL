//***********************************************************************************
//!	ファイル名		Class_BossClime.cpp
//
//	機能			アドベンチャーゲームモードの「クライム」クラスの実装
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2009/02/16	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_BossClime.h"		//!< アドベンチャーゲーム「クライム」

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CBOSS_CLIME::コンストラクタ
	//
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		メッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2009/02/16		<新規>
	//================================================================================
	CBOSS_CLIME::CBOSS_CLIME(CONST DEV		inDev,
							 CONST LPSTR	inName,
							 CONST LPSTR	inResource,
							 CONST LPSTR	inTexture)	:
	CKAGETOKI_ADVENT_BOSS(inDev, inName, inResource, inTexture)
	{
	//	属性の初期化
		this->fJumpHeight				= 80.0f;				//!< ジャンプの高さ
		this->fLeftRightMoveSpeed		= 0.0f;					//!< 左右移動スピード
		this->fLeftRightMoveMaxSpeed	= 20.0f;				//!< 左右移動最大スピード
		this->fLeftRightMoveAccele		= 2.0f;					//!< 左右移動加速値

		this->fFloorSpace				= 8.0f;					//!< 床との間隔
		this->fWallSpace				= 100.0f;				//!< 壁との間隔

	//	アニメーションの割り当てを行う(クライム専用のクラスなので、、、)
		this->iAnimTemp[eBCLIMEA_APPEAR]		= 2;
		this->iAnimTemp[eBCLIMEA_APPEAR_WAIT]	= 0;
		this->iAnimTemp[eBCLIMEA_WAIT]			= 0;
		this->iAnimTemp[eBCLIMEA_DAMAGE]		= 6;

		this->iMaxLife					= CLIME_MAXLIFE;		//!< 最大体力
		this->iLife						= this->iMaxLife;		//!< 残り体力
		this->iHitFrame					= 0;					//!< 衝突して点滅する時間

	//	フラグ群
		this->action			= eBCLIMEA_MAX;					//!< 最初は待機以外何でも良い
		this->changeAction(eBCLIMEA_APPEAR);					//!< 初期状態は出現

		this->stepCnt			= 0;
	//	音のセット
		this->sStaticStep		= new SSTATIC(SOUND_STEP);
		this->sStaticJumpStart	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticJumpEnd	= new SSTATIC(SOUND_JUMP_START);

	//	エフェクトの登録
		C3D_EFFECT_CENTER*	effectCenter	= C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			EFFECT_SMOKE, new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));

	//	ローカル座標の変更
		this->sd_ptr->mesh->localCenter = VEC3(0.0f, 100.0f, 0.0f);
		this->sd_ptr->mesh->localRadius	= 230.0f;

	//	インターフェーステクスチャを読み込む
		if(	FAILED(D3DX_CREATE_TEXTURE_EX(
				inDev, "..//Resources//Texture//boss_exe_interface.dds",
				D3DCOLOR_RGBA(100,100,100,100), NULL, &this->interfaceTex)))
		{
		//	失敗したら、NULLを入れる
			this->interfaceTex	= NULL;
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_CLIME::デストラクタ
	//
	//	更新		2009/02/16		<新規>
	//================================================================================
	CBOSS_CLIME::~CBOSS_CLIME(void)
	{
	//	音関連の解放
		SAFE_DELETE(this->sStaticStep);				//!< 足音
		SAFE_DELETE(this->sStaticJumpStart);		//!< ジャンプ音
		SAFE_DELETE(this->sStaticJumpEnd);			//!< 着地音
	}

	//================================================================================
	//!	メソッド名	CBOSS_CLIME::soundStep
	//
	//	機能		足音管理
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_CLIME::soundStep(void)
	{
		this->stepCnt++;

	//	スモークは20フレーム毎に発生
		if(	this->stepCnt > 5)
		{
			C3D_EFFECT_CENTER::occuEffectsInstance(EFFECT_SMOKE, this->getLoc(), 0.3f);
			this->sStaticStep->setPlayCur(0);
			this->sStaticStep->play(0, NULL);
			this->stepCnt	= 0;
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_CLIME::soundJumpStart
	//
	//	機能		ジャンプ音管理
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_CLIME::soundJumpStart(void)
	{
	//	ジャンプ音を鳴らす
		this->sStaticJumpStart->play(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CBOSS_CLIME::soundJumpEnd
	//
	//	機能		着地音管理
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_CLIME::soundJumpEnd(void)
	{
	//	着地音を鳴らす
		this->sStaticJumpEnd->play(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CBOSS_CLIME::opeAppear
	//
	//	機能		出現時操作
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_CLIME::opeAppear(void)
	{
	//	重力効果
		this->opeEffectGravity();
	//	プレイヤーの方を向く
		this->opeTurnToPlayer();
	//	クライムが地面に付いたら、待機
		if(	!this->bFlighting)
		{
			this->changeAction(eBCLIMEA_APPEAR_WAIT);
			this->soundJumpEnd();	//!< 着地音
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_CLIME::opeAppearWait
	//
	//	機能		出現待機時操作
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_CLIME::opeAppearWait(void)
	{
	//	重力効果
		this->opeEffectGravity();
	//	プレイヤーの方を向く
		this->opeTurnToPlayer();
	}

	//================================================================================
	//!	メソッド名	CBOSS_CLIME::opeWait
	//
	//	機能		待機時操作
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_CLIME::opeWait(void)
	{
	//	重力効果
		this->opeEffectGravity();

	//	上記の処理で、空中にいるなら、状態を「飛行」に変更
		if(	this->bFlighting)
		{
		}
	//	空中にいないなら
		else
		{
		//	プレイヤーの方を向く
			this->opeTurnToPlayer();
		//	フレームが一定まで行ったら、ランダムに攻撃に移る
			this->iFrameCnt++;
			if(	this->iFrameCnt > 30)
			{
				switch(rand() % 2)
				{
				//	空中ドリルクロー
					case 0:
						break;
				//	ダッシュクロー
					case 1:
						break;
				}
			}
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_CLIME::opeDamage
	//
	//	機能		ダメージ反動時
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_CLIME::opeDamage(void)
	{
	//	後ずさり
		this->moveRear();
	//	フレームが一定まで行ったら、待機
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10)
		{
		//	プレイヤーの方を向く
			this->opeTurnToPlayer();
			this->changeAction(eBCLIMEA_WAIT);
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_CLIME::changeAction
	//
	//	機能		状態変化
	//	引数		inNewAction		新たな行動パターン
	//	戻り値		TRUE			新たな行動パターンに変化した
	//				FALSE			新たな行動パターンに変化していない
	//	更新		2009/02/16		<新規>
	//================================================================================
	BOOL
	CBOSS_CLIME::changeAction(BOSSCLIME_ACTION inNewAction)
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
			case	eBCLIMEA_APPEAR:				//!< 出現時
				this->setAngleZ(0.0f);
				break;
			case	eBCLIMEA_APPEAR_WAIT:			//!< 出現待機時
				this->setMoveQuantity(0.0f);
				this->setAngleZ(0.0f);
				break;
			case	eBCLIMEA_WAIT:				//!< 待機時
				this->setMoveQuantity(30.0f);
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				break;
			case	eBCLIMEA_DAMAGE:				//!< ダメージ反動時
				this->setMoveQuantity(10.0f);
				this->setAngleZ(0.0f);
				break;
			default:							//!< 上記以外
				break;
		}

	//	フレームカウンタを初期化
		this->iFrameCnt	= 0;

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CBOSS_CLIME::reactionMessage
	//
	//	機能		リアクション
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_CLIME::reactionMessage(REACTION_MESSAGE inMessage)
	{
	//	エナジーボールに当たったら
		if(	inMessage == eREAMSG_ENERGY_HIT)
		{
		//	衝突フレームが0以下なら、ダメージ
			if(	this->iHitFrame <= 0)
			{
				this->iHitFrame  = 100;
				this->iLife--;
			//	攻撃以外の時なら、後ずさる
				if(	1)
				{
					this->changeAction(eBCLIMEA_DAMAGE);
				}
			}
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_CLIME::resetParameter
	//
	//	機能		パラメータのリセット
	//	更新		2009/02/18		<新規>
	//================================================================================
	void
	CBOSS_CLIME::resetParameter(void)
	{
		CKAGETOKI_ADVENT_BOSS::resetParameter();	//!< ライフのリセット
		this->iHitFrame	= 0;						//!< 衝突して点滅する時間
		this->changeAction(eBCLIMEA_APPEAR);		//!< 初期状態は出現
	}

	//================================================================================
	//!	メソッド名	CBOSS_CLIME::operation
	//
	//	機能		基本操作
	//	引数		*input			入力管理
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_CLIME::operation(CINPUT* input)
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
			case	eBCLIMEA_APPEAR:				//!< 出現時
				this->opeAppear();
				break;
			case	eBCLIMEA_APPEAR_WAIT:			//!< 出現待機時
				this->opeAppearWait();
				break;
			case	eBCLIMEA_WAIT:				//!< 待機時
				this->opeWait();
				break;
			case	eBCLIMEA_DAMAGE:				//!< ダメージ反動時
				this->opeDamage();
				break;
		}//	default	break;
	}

	//================================================================================
	//!	メソッド名	CBOSS_CLIME::draw
	//
	//	機能		描画
	//	引数		inDev			Direct3Dデバイス
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2008/12/10		<新規>
	//================================================================================
	BOOL
	CBOSS_CLIME::draw(CONST DEV inDev)
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

	//	インターフェース描画
		this->drawInterface(inDev);

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------