//***********************************************************************************
//!	ファイル名		Class_BossExe.cpp
//
//	機能			アドベンチャーゲームモードの「カゲオ」クラスの実装
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2009/01/13	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_BossExe.h"		//!< アドベンチャーゲーム「エクス」

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CBOSS_EXE::コンストラクタ
	//
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		メッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2009/01/13		<新規>
	//================================================================================
	CBOSS_EXE::CBOSS_EXE(CONST DEV		inDev,
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

	//	アニメーションの割り当てを行う(エクス専用のクラスなので、、、)
		this->iAnimTemp[eBEXEA_APPEAR]			= 2;
		this->iAnimTemp[eBEXEA_APPEAR_WAIT]		= 0;
		this->iAnimTemp[eBEXEA_WAIT]			= 0;
		this->iAnimTemp[eBEXEA_DAMAGE]			= 6;
		this->iAnimTemp[eBEXEA_DRILLCLAW_JUMP]	= 2;
		this->iAnimTemp[eBEXEA_DRILLCLAW]		= 7;
		this->iAnimTemp[eBEXEA_DASHCLAW_DASH]	= 1;
		this->iAnimTemp[eBEXEA_DASHCLAW]		= 5;
		this->iAnimTemp[eBEXEA_DISAPPEAR_WAIT]	= 0;
		this->iAnimTemp[eBEXEA_DISAPPEAR]		= 0;

		this->iMaxLife					= EXE_MAXLIFE;			//!< 最大体力
		this->iLife						= this->iMaxLife;		//!< 残り体力
		this->iHitFrame					= 0;					//!< 衝突して点滅する時間

	//	フラグ群
		this->action			= eBEXEA_MAX;					//!< 最初は待機以外何でも良い
		this->changeAction(eBEXEA_APPEAR);						//!< 初期状態は出現

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
	//!	メソッド名	CBOSS_EXE::デストラクタ
	//
	//	更新		2009/01/13		<新規>
	//================================================================================
	CBOSS_EXE::~CBOSS_EXE(void)
	{
	//	音関連の解放
		SAFE_DELETE(this->sStaticStep);				//!< 足音
		SAFE_DELETE(this->sStaticJumpStart);		//!< ジャンプ音
		SAFE_DELETE(this->sStaticJumpEnd);			//!< 着地音
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::soundStep
	//
	//	機能		足音管理
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CBOSS_EXE::soundStep(void)
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
	//!	メソッド名	CBOSS_EXE::soundJumpStart
	//
	//	機能		ジャンプ音管理
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CBOSS_EXE::soundJumpStart(void)
	{
	//	ジャンプ音を鳴らす
		this->sStaticJumpStart->play(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::soundJumpEnd
	//
	//	機能		着地音管理
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CBOSS_EXE::soundJumpEnd(void)
	{
	//	着地音を鳴らす
		this->sStaticJumpEnd->play(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::opeAppear
	//
	//	機能		出現時操作
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CBOSS_EXE::opeAppear(void)
	{
	//	重力効果
		this->opeEffectGravity();
	//	プレイヤーの方を向く
		this->opeTurnToPlayer();
	//	エクスが地面に付いたら、待機
		if(	!this->bFlighting)
		{
			this->changeAction(eBEXEA_APPEAR_WAIT);
			this->soundJumpEnd();	//!< 着地音
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::opeAppearWait
	//
	//	機能		出現待機時操作
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CBOSS_EXE::opeAppearWait(void)
	{
	//	重力効果
		this->opeEffectGravity();
	//	プレイヤーの方を向く
		this->opeTurnToPlayer();
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::opeWait
	//
	//	機能		待機時操作
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CBOSS_EXE::opeWait(void)
	{
	//	重力効果
		this->opeEffectGravity();

	//	プレイヤーに当たったら、ダメージ
		if(	(*this->player)->intersectRadius(this))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

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
						this->changeAction(eBEXEA_DRILLCLAW_JUMP);
						break;
				//	ダッシュクロー
					case 1:
						this->changeAction(eBEXEA_DASHCLAW_DASH);
						break;
				}
			}
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::opeDamage
	//
	//	機能		ダメージ反動時
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CBOSS_EXE::opeDamage(void)
	{
	//	後ずさり
		this->moveRear();

	//	フレームが一定まで行ったら、待機
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10)
		{
		//	プレイヤーの方を向く
			this->opeTurnToPlayer();
			this->changeAction(eBEXEA_WAIT);
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::opeDrillClawJump
	//
	//	機能		ドリルクロー準備ジャンプ時
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CBOSS_EXE::opeDrillClawJump(void)
	{
	//	重力効果
		this->opeEffectGravity();

	//	プレイヤーに当たったら、ダメージ
		if(	(*this->player)->intersectRadius(this))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

	//	フレームが一定まで行ったら、ドリルクロー
		this->iFrameCnt++;
		if(	this->iFrameCnt > 30)
		{
		//	プレイヤーの方を向く
			this->opeTurnToPlayer();
			this->changeAction(eBEXEA_DRILLCLAW);
		//	角度照準を合わせる
			float	X	= this->getLocX() - (*this->player)->getLocX();
			float	Y	= this->getLocY() - (*this->player)->getLocY();
			float	dir	= atan2(Y, X) * 180.0f / D3DX_PI;
			this->setAngleZ((dir > 90.0f && dir <= 270.0f) ? dir + 180.0f : dir);
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::opeDrillClaw
	//
	//	機能		ドリルクロー時
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CBOSS_EXE::opeDrillClaw(void)
	{
	//	フレーム数を移動量に切替
		this->setMoveQuantity((float)(this->iFrameCnt * 3));
	//	突進し、壁に当たったら待機
		if(	this->stageG->intersect(
				this,
				ADVENT_FRONTHIT,
				this->fWallSpace + this->getMoveQuantity() + 100.0f))
		{
			this->changeAction(eBEXEA_WAIT);
			return;
		}

		else
	//	プレイヤーに当たったら、ダメージ
		if(	(*this->player)->intersect(
					this,
					ADVENT_FRONTHIT,
					this->fWallSpace + this->getMoveQuantity() + 100.0f))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}
	//	ここまで来たら、移動
		ADVENT_FRONTMOVE;

	//	フレームが一定まで上がったら待機
		this->iFrameCnt++;
		if(	this->iFrameCnt > 60)
		{
			this->changeAction(eBEXEA_WAIT);
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::opeDashClawDash
	//
	//	機能		ダッシュクロー準備ダッシュ時
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CBOSS_EXE::opeDashClawDash(void)
	{
	//	足音
		this->soundStep();
	//	突進し、壁に当たったら待機
		if(	this->stageG->intersect(
				this,
				ADVENT_FRONTHIT,
				this->fWallSpace + this->getMoveQuantity() + 100.0f))
		{
			this->changeAction(eBEXEA_WAIT);
			return;
		}
	//	前進
		ADVENT_FRONTMOVE;

	//	プレイヤーに当たったら、ダメージ
		if(	(*this->player)->intersectRadius(this))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

	//	プレイヤーキャラクターが近いなら、ダッシュクロー攻撃
		float	length	= (*this->player)->getLocX() - this->getLocX();
		length			= (length >= 0.0f ? length : -length);
		if(	length < 500.0f)
		{
			this->changeAction(eBEXEA_DASHCLAW);
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::opeDashClaw
	//
	//	機能		ダッシュクロー時
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CBOSS_EXE::opeDashClaw(void)
	{
	//	足音
		this->soundStep();
	//	突進し、壁に当たったら待機
		if(	this->stageG->intersect(
				this,
				ADVENT_FRONTHIT,
				this->fWallSpace + this->getMoveQuantity() + 150.0f))
		{
			this->changeAction(eBEXEA_WAIT);
			return;
		}

		else
	//	プレイヤーに当たったら、ダメージ
		if(	(*this->player)->intersect(
				this,
				ADVENT_FRONTHIT,
				this->fWallSpace + this->getMoveQuantity() + 150.0f))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}
	//	前進
		ADVENT_FRONTMOVE;
	//	フレームが一定まで上がったら待機
		this->iFrameCnt++;
		if(	this->iFrameCnt > 30)
		{
			this->changeAction(eBEXEA_WAIT);
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::opeDisappearWait
	//
	//	機能		消滅待機時
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CBOSS_EXE::opeDisappearWait(void)
	{
	//	重力効果
		this->opeEffectGravity();

		this->iFrameCnt++;
	//	10フレーム刻みで煙発生
		if(	this->iFrameCnt > 10)
		{
		//	煙の位置をランダム算出
			VEC3	inSmokeLoc	= this->getWorldCenter();
			inSmokeLoc.x	+= float(rand() % 300 - 150);
			inSmokeLoc.y	+= float(rand() % 300 - 150);
			inSmokeLoc.z	+= float(rand() % 100 -  50);

			C3D_EFFECT_CENTER::occuEffectsInstance(EFFECT_SMOKE, inSmokeLoc, 0.3f);
			this->iFrameCnt = 0;
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::opeDisappear
	//
	//	機能		消滅時
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CBOSS_EXE::opeDisappear(void)
	{
		this->iFrameCnt++;
	//	8フレーム刻みで煙発生
		if(	this->iFrameCnt % 3)
		{
		//	煙の位置をランダム算出
			VEC3	inSmokeLoc	= this->getWorldCenter();
			inSmokeLoc.x	+= float(rand() % 400 - 200);
			inSmokeLoc.y	+= float(rand() % 400 - 200);
			inSmokeLoc.z	+= float(rand() % 100 -  50);

			C3D_EFFECT_CENTER::occuEffectsInstance(EFFECT_SMOKE, inSmokeLoc, 0.5f);
		}

	//	フレームが一定まで上がったら、消滅
		if(	this->iFrameCnt > 150)
		{
			this->setVisible(FALSE);
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::changeAction
	//
	//	機能		状態変化
	//	引数		inNewAction		新たな行動パターン
	//	戻り値		TRUE			新たな行動パターンに変化した
	//				FALSE			新たな行動パターンに変化していない
	//	更新		2009/01/13		<新規>
	//================================================================================
	BOOL
	CBOSS_EXE::changeAction(BOSSEXE_ACTION inNewAction)
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
			case	eBEXEA_APPEAR:				//!< 出現時
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_APPEAR_WAIT:			//!< 出現待機時
				this->setMoveQuantity(0.0f);
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_WAIT:				//!< 待機時
				this->setMoveQuantity(30.0f);
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_DAMAGE:				//!< ダメージ反動時
				this->setMoveQuantity(10.0f);
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_DRILLCLAW_JUMP:		//!< ドリルクロー準備ジャンプ時
				this->setAnimSpeed(0.005000f);
				this->moveJumping(this->fJumpHeight);
				this->soundJumpStart();
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_DRILLCLAW:			//!< ドリルクロー時
				this->setMoveQuantity(0.0f);
				this->setAnimSpeed();
				break;
			case	eBEXEA_DASHCLAW_DASH:		//!< ダッシュクロー準備ダッシュ時
				this->setMoveQuantity(35.0f);
				this->setAnimSpeed(0.032f);
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_DASHCLAW:			//!< ダッシュクロー時
				this->setMoveQuantity(50.0f);
				this->setAnimSpeed(0.032f);
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_DISAPPEAR_WAIT:		//!< 消滅待機時
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_DISAPPEAR:			//!< 消滅時
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				this->iHitFrame	= 150;
				break;
			default:							//!< 上記以外
				break;
		}

	//	フレームカウンタを初期化
		this->iFrameCnt			 = 0;

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::reactionMessage
	//
	//	機能		リアクション
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CBOSS_EXE::reactionMessage(REACTION_MESSAGE inMessage)
	{
	//	消滅時、消滅待機時の時は処理を返す
		if(	this->action == eBEXEA_DISAPPEAR_WAIT	||
			this->action == eBEXEA_DISAPPEAR)	return;

	//	エナジーボールに当たったら
		if(	inMessage == eREAMSG_ENERGY_HIT)
		{
		//	衝突フレームが0以下なら、ダメージ
			if(	this->iHitFrame <= 0)
			{
				this->iHitFrame  = 100;
				this->iLife--;
			//	ライフが無くなったら、消滅する
				if(	this->iLife <= 0)
				{
					this->changeAction(eBEXEA_DISAPPEAR_WAIT);
				}
			//	攻撃以外の時なら、後ずさる
				else
				if(	this->action != eBEXEA_DRILLCLAW	&&
					this->action != eBEXEA_DASHCLAW)
				{
					this->changeAction(eBEXEA_DAMAGE);
				}
			}
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::resetParameter
	//
	//	機能		パラメータのリセット
	//	更新		2009/02/18		<新規>
	//================================================================================
	void
	CBOSS_EXE::resetParameter(void)
	{
		CKAGETOKI_ADVENT_BOSS::resetParameter();	//!< ライフのリセット
		this->iHitFrame	= 0;						//!< 衝突して点滅する時間
		this->changeAction(eBEXEA_APPEAR);			//!< 初期状態は出現
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::operation
	//
	//	機能		基本操作
	//	引数		*input			入力管理
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CBOSS_EXE::operation(CINPUT* input)
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
			case	eBEXEA_APPEAR:				//!< 出現時
				this->opeAppear();
				break;
			case	eBEXEA_APPEAR_WAIT:			//!< 出現待機時
				this->opeAppearWait();
				break;
			case	eBEXEA_WAIT:				//!< 待機時
				this->opeWait();
				break;
			case	eBEXEA_DAMAGE:				//!< ダメージ反動時
				this->opeDamage();
				break;
			case	eBEXEA_DRILLCLAW_JUMP:		//!< ドリルクロー準備ジャンプ時
				this->opeDrillClawJump();
				break;
			case	eBEXEA_DRILLCLAW:			//!< ドリルクロー時
				this->opeDrillClaw();
				break;
			case	eBEXEA_DASHCLAW_DASH:		//!< ダッシュクロー準備ダッシュ時
				this->opeDashClawDash();
				break;
			case	eBEXEA_DASHCLAW:			//!< ダッシュクロー時
				this->opeDashClaw();
				break;
			case	eBEXEA_DISAPPEAR_WAIT:		//!< 消滅待機時
				this->opeDisappearWait();
				break;
			case	eBEXEA_DISAPPEAR:			//!< 消滅時
				this->opeDisappear();
				break;
		}//	default	break;
	}

	//================================================================================
	//!	メソッド名	CBOSS_EXE::draw
	//
	//	機能		描画
	//	引数		inDev			Direct3Dデバイス
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2008/12/10		<新規>
	//================================================================================
	BOOL
	CBOSS_EXE::draw(CONST DEV inDev)
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