//***********************************************************************************
//!	ファイル名		Class_BossYamakage.cpp
//
//	機能			アドベンチャーゲームモードの「ヤマカゲ」クラスの実装
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2009/02/16	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_BossYamakage.h"		//!< アドベンチャーゲーム「ヤマカゲ」

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CBOSS_YAMAKAGE::コンストラクタ
	//
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		メッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2009/02/16		<新規>
	//================================================================================
	CBOSS_YAMAKAGE::CBOSS_YAMAKAGE(CONST DEV	inDev,
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

	//	アニメーションの割り当てを行う(ヤマカゲ専用のクラスなので、、、)
		this->iAnimTemp[eBYAMAKAGEA_APPEAR]			= 1;
		this->iAnimTemp[eBYAMAKAGEA_APPEAR_WAIT]	= 0;
		this->iAnimTemp[eBYAMAKAGEA_WAIT]			= 0;
		this->iAnimTemp[eBYAMAKAGEA_DAMAGE]			= 3;
		this->iAnimTemp[eBYAMAKAGEA_WEAPON_TAKEOUT]	= 2;
		this->iAnimTemp[eBYAMAKAGEA_WEAPON_ATTACK]	= 2;
		this->iAnimTemp[eBYAMAKAGEA_ATTACK_ANDMOVE]	= 1;
		this->iAnimTemp[eBYAMAKAGEA_DISAPPEAR_WAIT]	= 0;
		this->iAnimTemp[eBYAMAKAGEA_DISAPPEAR]		= 0;

		this->iMaxLife			= YAMAKAGE_MAXLIFE;				//!< 最大体力
		this->iLife				= this->iMaxLife;				//!< 残り体力
		this->iHitFrame			= 0;							//!< 衝突して点滅する時間

	//	ヤマカゲ兵器
		this->yamakageWeaponA	= new CYAMAKAGE_WEAPON(inDev);
		this->yamakageWeaponA->setVisible(FALSE);
		this->yamakageWeaponB	= new CYAMAKAGE_WEAPON(inDev);
		this->yamakageWeaponB->setVisible(FALSE);
		this->yamakageWeaponC	= new CYAMAKAGE_WEAPON(inDev);
		this->yamakageWeaponC->setVisible(FALSE);
		this->yamakageWeaponD	= new CYAMAKAGE_WEAPON(inDev);
		this->yamakageWeaponD->setVisible(FALSE);
		this->iAttackCnt		= 0;
		this->iMoveCnt			= 0;

	//	フラグ群
		this->action			= eBYAMAKAGEA_MAX;				//!< 最初は待機以外何でも良い
		this->changeAction(eBYAMAKAGEA_APPEAR);					//!< 初期状態は出現

	//	音のセット
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
	//!	メソッド名	CBOSS_YAMAKAGE::デストラクタ
	//
	//	更新		2009/02/16		<新規>
	//================================================================================
	CBOSS_YAMAKAGE::~CBOSS_YAMAKAGE(void)
	{
	//	音関連の解放
		SAFE_DELETE(this->sStaticJumpStart);		//!< ジャンプ音
		SAFE_DELETE(this->sStaticJumpEnd);			//!< 着地音
		SAFE_DELETE(this->yamakageWeaponA);			//!< ヤマカゲ兵器A
		SAFE_DELETE(this->yamakageWeaponB);			//!< ヤマカゲ兵器B
		SAFE_DELETE(this->yamakageWeaponC);			//!< ヤマカゲ兵器A
		SAFE_DELETE(this->yamakageWeaponD);			//!< ヤマカゲ兵器B
	}

	//================================================================================
	//!	メソッド名	CBOSS_YAMAKAGE::soundJumpStart
	//
	//	機能		ジャンプ音管理
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_YAMAKAGE::soundJumpStart(void)
	{
	//	ジャンプ音を鳴らす
		this->sStaticJumpStart->play(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CBOSS_YAMAKAGE::soundJumpEnd
	//
	//	機能		着地音管理
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_YAMAKAGE::soundJumpEnd(void)
	{
	//	着地音を鳴らす
		this->sStaticJumpEnd->play(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CBOSS_YAMAKAGE::opeAppear
	//
	//	機能		出現時操作
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeAppear(void)
	{
	//	重力効果
		this->opeEffectGravity();
	//	プレイヤーの方を向く
		this->opeTurnToPlayer();

	//	ヤマカゲが地面に付いたら、待機
		if(	!this->bFlighting)
		{
			this->changeAction(eBYAMAKAGEA_APPEAR_WAIT);
			this->soundJumpEnd();	//!< 着地音
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_YAMAKAGE::opeAppearWait
	//
	//	機能		出現待機時操作
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeAppearWait(void)
	{
	//	重力効果
		this->opeEffectGravity();
	//	プレイヤーの方を向く
		this->opeTurnToPlayer();
	}

	//================================================================================
	//!	メソッド名	CBOSS_YAMAKAGE::opeWait
	//
	//	機能		待機時操作
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeWait(void)
	{
	//	重力効果
		this->opeEffectGravity();

	//	兵器の操作
		this->yamakageWeaponA->operation();
		this->yamakageWeaponB->operation();

	//	空中にいないなら
		if(	!this->bFlighting)
		{
		//	プレイヤーの方を向く
			this->opeTurnToPlayer();
		//	フレームが一定まで行ったら、ランダムに攻撃に移る
			this->iFrameCnt++;
			if(	this->iFrameCnt > 15)
			{
			//	ライフに影響はなく、兵器Aと兵器Bを出す
				if(	!this->yamakageWeaponA->getVisible()	||
					!this->yamakageWeaponB->getVisible())
				{
					this->changeAction(eBYAMAKAGEA_WEAPON_TAKEOUT);
				}
			//	ライフが8以下になったら、兵器Cを出す
				else
				if(	this->iLife <= 8	&&
					!this->yamakageWeaponC->getVisible())
				{
					this->changeAction(eBYAMAKAGEA_WEAPON_TAKEOUT);
				}
			//	ライフが4以下になったら、兵器Dを出す
				else
				if(	this->iLife <= 4	&&
					!this->yamakageWeaponD->getVisible())
				{
					this->changeAction(eBYAMAKAGEA_WEAPON_TAKEOUT);
				}
			//	上記以外は攻撃
				else
				{
					this->changeAction(eBYAMAKAGEA_WEAPON_ATTACK);
				}
				this->iFrameCnt	= 0;
			}
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_YAMAKAGE::opeDamage
	//
	//	機能		ダメージ反動時
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeDamage(void)
	{
	//	後ずさり
		this->moveRear();
	//	フレームが一定まで行ったら、待機
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10)
		{
		//	プレイヤーの方を向く
			this->opeTurnToPlayer();
			this->changeAction(eBYAMAKAGEA_WAIT);
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_YAMAKAGE::opeWeaponTakeOut
	//
	//	機能		武器取り出し時
	//	更新		2009/02/17		<新規>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeWeaponTakeOut(void)
	{
	//	重力効果
		this->opeEffectGravity();

	//	兵器の操作
		this->yamakageWeaponA->operation();
		this->yamakageWeaponB->operation();
		this->yamakageWeaponC->operation();
		this->yamakageWeaponD->operation();

	//	指定するフレームが来たら、武器を出現
		this->iFrameCnt++;
		if(	this->iFrameCnt > 50)
		{
			this->changeAction(eBYAMAKAGEA_WAIT);
			if(!this->yamakageWeaponA->getVisible())
			{
				this->yamakageWeaponA->setVisible(TRUE);
				this->yamakageWeaponA->setTargetPlayer(this->player);
				this->yamakageWeaponA->setViewG(this->viewG);
				this->yamakageWeaponA->setStageG(this->stageG);
				this->yamakageWeaponA->setLoc(
					&(this->getLoc() + VEC3(0.0f, 200.0f, 0.0f)));
				this->stageG->add(this->yamakageWeaponA);
				return;
			}
			if(!this->yamakageWeaponB->getVisible())
			{
				this->yamakageWeaponB->setVisible(TRUE);
				this->yamakageWeaponB->setTargetPlayer(this->player);
				this->yamakageWeaponB->setViewG(this->viewG);
				this->yamakageWeaponB->setStageG(this->stageG);
				this->yamakageWeaponB->setLoc(
					&(this->getLoc() + VEC3(0.0f, 200.0f, 0.0f)));
				this->stageG->add(this->yamakageWeaponB);
				return;
			}
			if(!this->yamakageWeaponC->getVisible())
			{
				this->yamakageWeaponC->setVisible(TRUE);
				this->yamakageWeaponC->setTargetPlayer(this->player);
				this->yamakageWeaponC->setViewG(this->viewG);
				this->yamakageWeaponC->setStageG(this->stageG);
				this->yamakageWeaponC->setLoc(
					&(this->getLoc() + VEC3(0.0f, 200.0f, 0.0f)));
				this->stageG->add(this->yamakageWeaponC);
				return;
			}
			if(!this->yamakageWeaponD->getVisible())
			{
				this->yamakageWeaponD->setVisible(TRUE);
				this->yamakageWeaponD->setTargetPlayer(this->player);
				this->yamakageWeaponD->setViewG(this->viewG);
				this->yamakageWeaponD->setStageG(this->stageG);
				this->yamakageWeaponD->setLoc(
					&(this->getLoc() + VEC3(0.0f, 200.0f, 0.0f)));
				this->stageG->add(this->yamakageWeaponD);
				return;
			}
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_YAMAKAGE::opeWeaponAttack
	//
	//	機能		武器攻撃
	//	更新		2009/02/17		<新規>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeWeaponAttack(void)
	{
	//	重力効果
		this->opeEffectGravity();

	//	兵器の操作
		this->yamakageWeaponA->operation();
		this->yamakageWeaponB->operation();
		this->yamakageWeaponC->operation();
		this->yamakageWeaponD->operation();

	//	指定するフレームが来たら、武器攻撃
		this->iFrameCnt++;
		if(	this->iFrameCnt > 80)
		{
			this->yamakageWeaponA->setWeaponAction(eOPEYW_ATTACK);
			this->yamakageWeaponB->setWeaponAction(eOPEYW_ATTACK);
			this->yamakageWeaponC->setWeaponAction(eOPEYW_ATTACK);
			this->yamakageWeaponD->setWeaponAction(eOPEYW_ATTACK);
			this->iFrameCnt	= 0;
			this->iAttackCnt++;
		//	攻撃回数が3回以上になったら、攻撃しながら移動する
			if(	this->iAttackCnt >= 3)
			{
				this->changeAction(eBYAMAKAGEA_ATTACK_ANDMOVE);
			}
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_YAMAKAGE::opeAttackAndMove
	//
	//	機能		攻撃しながら移動
	//	更新		2009/02/17		<新規>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeAttackAndMove(void)
	{
	//	兵器の操作
		this->yamakageWeaponA->operation();
		this->yamakageWeaponB->operation();
		this->yamakageWeaponC->operation();
		this->yamakageWeaponD->operation();

	//	移動制御フレームで、移動処理
		this->iMoveCnt++;
		if(	this->iMoveCnt < 90)
		{
			this->moveUpDown(eUD_UP, 2.0f);
		}
		else
		{
		//	突進し、壁に当たったら待機
			if(	this->stageG->intersect(
					this,
					ADVENT_FRONTHIT,
					this->fWallSpace + 20.0f + 150.0f))
			{
				this->changeAction(eBYAMAKAGEA_WAIT);
				return;
			}
		//	前進
			this->moveFrontRear(eFR_FRONT, 20.0f);
		}

	//	指定するフレームが来たら、武器攻撃
		this->iFrameCnt++;
		if(	this->iFrameCnt > 120)
		{
			this->yamakageWeaponA->setWeaponAction(eOPEYW_ATTACK);
			this->yamakageWeaponB->setWeaponAction(eOPEYW_ATTACK);
			this->yamakageWeaponC->setWeaponAction(eOPEYW_ATTACK);
			this->yamakageWeaponD->setWeaponAction(eOPEYW_ATTACK);
			this->iFrameCnt	= 0;
			this->iAttackCnt++;
		//	攻撃回数が3回以上になったら、攻撃しながら移動する
			if(	this->iAttackCnt >= 3)
			{
				this->changeAction(eBYAMAKAGEA_WAIT);
			}
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_YAMAKAGE::opeDisappearWait
	//
	//	機能		消滅待機時
	//	更新		2009/02/17		<新規>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeDisappearWait(void)
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
	//!	メソッド名	CBOSS_YAMAKAGE::opeDisappear
	//
	//	機能		消滅時
	//	更新		2009/02/17		<新規>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeDisappear(void)
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
	//!	メソッド名	CBOSS_YAMAKAGE::changeAction
	//
	//	機能		状態変化
	//	引数		inNewAction		新たな行動パターン
	//	戻り値		TRUE			新たな行動パターンに変化した
	//				FALSE			新たな行動パターンに変化していない
	//	更新		2009/02/16		<新規>
	//================================================================================
	BOOL
	CBOSS_YAMAKAGE::changeAction(BOSSYAMAKAGE_ACTION inNewAction)
	{
	//	前の状態と新たな状態が変らないなら、処理しない
		if(	this->action == inNewAction)	return	FALSE;
	//	上記の処理をスルーしたので、状態の変更
		this->action = inNewAction;
	//	アニメーションも変更
		this->changeAnimationAndBlend(this->iAnimTemp[this->action]);

	//	フレームカウンタを初期化
		this->iFrameCnt	= 0;

	//	新たな状態で追加処理があるならここで
		switch(this->action)
		{
			case	eBYAMAKAGEA_APPEAR:				//!< 出現時
				this->setMoveQuantity(0.0f);
				this->setAngleZ(0.0f);
				this->yamakageWeaponA->setVisible(FALSE);
				this->yamakageWeaponB->setVisible(FALSE);
				this->yamakageWeaponC->setVisible(FALSE);
				this->yamakageWeaponD->setVisible(FALSE);
				break;
			case	eBYAMAKAGEA_APPEAR_WAIT:		//!< 出現待機時
				this->setMoveQuantity(0.0f);
				this->setAngleZ(0.0f);
				break;
			case	eBYAMAKAGEA_WAIT:				//!< 待機時
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				break;
			case	eBYAMAKAGEA_DAMAGE:				//!< ダメージ反動時
				this->setMoveQuantity(10.0f);
				this->setAngleZ(0.0f);
				break;
			case	eBYAMAKAGEA_WEAPON_TAKEOUT:		//!< 武器取り出し時
				this->setMoveQuantity(0.0f);
				this->setAngleZ(0.0f);
				break;
			case	eBYAMAKAGEA_WEAPON_ATTACK:		//!< 武器攻撃
				this->setMoveQuantity(0.0f);
				this->setAngleZ(0.0f);
				this->iAttackCnt	= 0;
				break;
			case	eBYAMAKAGEA_ATTACK_ANDMOVE:		//!< 攻撃しながら移動
				this->setMoveQuantity(10.0f);
				this->setAngleZ(0.0f);
				this->iAttackCnt	= 0;
				this->iMoveCnt		= 0;
				break;
			case	eBYAMAKAGEA_DISAPPEAR_WAIT:		//!< 消滅待機時
				this->setMoveQuantity(0.0f);
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				break;
			case	eBYAMAKAGEA_DISAPPEAR:			//!< 消滅時
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				this->iHitFrame	= 150;
				break;
			default:								//!< 上記以外
				break;
		}

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CBOSS_YAMAKAGE::reactionMessage
	//
	//	機能		リアクション
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_YAMAKAGE::reactionMessage(REACTION_MESSAGE inMessage)
	{
	//	消滅時、消滅待機時の時は処理を返す
		if(	this->action == eBYAMAKAGEA_DISAPPEAR_WAIT	||
			this->action == eBYAMAKAGEA_DISAPPEAR)	return;

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
					this->changeAction(eBYAMAKAGEA_DISAPPEAR_WAIT);
				}
			}
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_YAMAKAGE::resetParameter
	//
	//	機能		パラメータのリセット
	//	更新		2009/02/18		<新規>
	//================================================================================
	void
	CBOSS_YAMAKAGE::resetParameter(void)
	{
		CKAGETOKI_ADVENT_BOSS::resetParameter();	//!< ライフのリセット
		this->iHitFrame		= 0;					//!< 衝突して点滅する時間
		this->changeAction(eBYAMAKAGEA_APPEAR);		//!< 初期状態は出現
		this->yamakageWeaponA->setVisible(FALSE);	//!< 兵器Aを存在確認無効
		this->yamakageWeaponB->setVisible(FALSE);	//!< 兵器Bを存在確認無効
		this->yamakageWeaponC->setVisible(FALSE);	//!< 兵器Aを存在確認無効
		this->yamakageWeaponD->setVisible(FALSE);	//!< 兵器Bを存在確認無効
		this->iAttackCnt	= 0;					//!< 攻撃制御カウンタを初期化
		this->iMoveCnt		= 0;					//!< 動作制御カウンタを初期化
	}

	//================================================================================
	//!	メソッド名	CBOSS_YAMAKAGE::operation
	//
	//	機能		基本操作
	//	引数		*input			入力管理
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CBOSS_YAMAKAGE::operation(CINPUT* input)
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
			case	eBYAMAKAGEA_APPEAR:				//!< 出現時
				this->opeAppear();
				break;
			case	eBYAMAKAGEA_APPEAR_WAIT:		//!< 出現待機時
				this->opeAppearWait();
				break;
			case	eBYAMAKAGEA_WAIT:				//!< 待機時
				this->opeWait();
				break;
			case	eBYAMAKAGEA_DAMAGE:				//!< ダメージ反動時
				this->opeDamage();
				break;
			case	eBYAMAKAGEA_WEAPON_TAKEOUT:		//!< 武器取り出し時
				this->opeWeaponTakeOut();
				break;
			case	eBYAMAKAGEA_WEAPON_ATTACK:		//!< 武器攻撃
				this->opeWeaponAttack();
				break;
			case	eBYAMAKAGEA_ATTACK_ANDMOVE:		//!< 攻撃しながら移動
				this->opeAttackAndMove();
				break;
			case	eBYAMAKAGEA_DISAPPEAR_WAIT:		//!< 消滅待機時
				this->opeDisappearWait();
				break;
			case	eBYAMAKAGEA_DISAPPEAR:			//!< 消滅時
				this->opeDisappear();
				break;
		}//	default	break;

	//	常にプレイヤーに当たったら、ダメージ
		if(	(*this->player)->intersectRadius(this))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}
	}

	//================================================================================
	//!	メソッド名	CBOSS_YAMAKAGE::draw
	//
	//	機能		描画
	//	引数		inDev			Direct3Dデバイス
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2008/12/10		<新規>
	//================================================================================
	BOOL
	CBOSS_YAMAKAGE::draw(CONST DEV inDev)
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

	//	ライフが余ってたら、ライフを描画する
		if(	this->iLife > 0)
		{
			this->yamakageWeaponA->draw(inDev);
			this->yamakageWeaponB->draw(inDev);
			this->yamakageWeaponC->draw(inDev);
			this->yamakageWeaponD->draw(inDev);
		}

	//	インターフェース描画
		this->drawInterface(inDev);

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------