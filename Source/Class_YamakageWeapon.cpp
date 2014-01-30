//***********************************************************************************
//!	ファイル名		Class_YamakageWeapon.cpp
//
//	機能			ヤマカゲが操る兵器クラスの実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2009/02/17	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CYAMAKAGE_WEAPON::コンストラクタ
	//
	//	引数		inDev			Direct3Dデバイス
	//				inKind			ボールの種類
	//	更新		2009/02/17		<新規>
	//================================================================================
	CYAMAKAGE_WEAPON::CYAMAKAGE_WEAPON(CONST DEV inDev)	:
	CSKIN_MESH(inDev, NAME_YAMAKAGE_WEAPON)
	{
	//	属性の初期化
		this->player			= NULL;
		this->viewG				= NULL;
		this->stageG			= NULL;
		this->weaponAction		= eOPEYW_MAX;
		this->iSmokeFrame		= 0;
		this->iMoveFrame		= 0;
		this->targetLoc			= VEC3(0.0f, 0.0f, 0.0f);
		this->ssEnemyBallShot	= new SSTATIC("shot_shadow");

	//	オリジナル「エネミーボール」の生成
		this->originEnemyBall
			= new CENEMY_BALL(inDev, eENEMYB_STRAIGHT, "shadow_ball", EFFECT_SHADOW);
		this->originEnemyBall->setEffectColor(D3DCOLOR_RGBA(100,100,100,255));
		this->originEnemyBall->setReactMessage(eREAMSG_ENEMY_ATTACK_LV1);

	//	エフェクトの登録
		C3D_EFFECT_CENTER*	effectCenter = C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			EFFECT_SMOKE, new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));

	//	浮遊中に変更
		this->setWeaponAction(eOPEYW_FLOATING);
	//	アニメーションの初期化
		this->changeAnimationAndBlend(0);
	//	オブジェクトの種類を見える影に変更
		this->setThisMessage(eTMSG_SEE_SHADOW);
	}

	//================================================================================
	//!	メソッド名	CYAMAKAGE_WEAPON::デストラクタ
	//
	//	更新		2009/02/17		<新規>
	//================================================================================
	CYAMAKAGE_WEAPON::~CYAMAKAGE_WEAPON(void)
	{
		SAFE_DELETE(this->originEnemyBall);
		SAFE_DELETE(this->ssEnemyBallShot);
	}

	//================================================================================
	//!	メソッド名	CYAMAKAGE_WEAPON::setWeaponAction
	//
	//	機能		動作制御の設定
	//	引数		inAction		新たな動作制御命令
	//	更新		2009/02/17		<新規>
	//================================================================================
	void
	CYAMAKAGE_WEAPON::setWeaponAction(YAMAKAGE_WEAPON_ACTION inAction)
	{
	//	存在しなければ、変更出来ない
		if(	!this->getVisible())			return;
	//	新たな動作制御の命令と現在の動作制御が等しいなら、処理をしない
		if(	this->weaponAction == inAction)	return;
	//	新たな動作制御を設定
		this->weaponAction	= inAction;

		this->iMoveFrame	= 0;
	//	新たな動作制御によって分岐
		switch(this->weaponAction)
		{
			case eOPEYW_FLOATING:
				this->iMoveFrame	= 29;
				break;
			case eOPEYW_ATTACK:
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CYAMAKAGE_WEAPON::reactionMessage
	//
	//	機能		リアクション対応
	//	引数		inMessage		リアクションメッセージ
	//	更新		2009/02/17		<新規>
	//================================================================================
	void
	CYAMAKAGE_WEAPON::reactionMessage(REACTION_MESSAGE inMessage)
	{
	//	シャインボールに当たったら、存在消去
		if(	inMessage == eREAMSG_SHINE_HIT)
		{
			this->setVisible(FALSE);
		}
	}

	//================================================================================
	//!	メソッド名	CYAMAKAGE_WEAPON::operation
	//
	//	機能		基本操作
	//	更新		2009/02/17		<新規>
	//================================================================================
	void
	CYAMAKAGE_WEAPON::operation(void)
	{
	//	エラー対処
		if(	!this->player	||
			!this->getVisible())	return;

	//	現在の描画フレームが、
	//	エフェクトの発生間隔フレームで割り切れたら、エフェクト発生
		this->iSmokeFrame++;
		if(	!(this->iSmokeFrame % 10))
		{
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE, this->getLoc(), 0.2f,
				VEC3(0.0f, 0.0f, 0.0f), this->smokeColor.color);
			this->iSmokeFrame	= 0;
		}

	//	動作制御によって分岐
		switch(this->weaponAction)
		{
		//	浮遊状態
			case eOPEYW_FLOATING:
				this->iMoveFrame++;
			//	最初の30フレームで、移動対象位置を更新する
				if(	this->iMoveFrame >= 30 && this->iMoveFrame <= 30)
				{
					this->targetLoc		 = (*this->player)->getLoc();
					this->targetLoc.y	+= 200.0f;
				//	ランダムに位置を指定し、プレイヤーを追いかける
					this->targetLoc.y	+= float(rand() % 1000 + 1000);
					this->targetLoc.x	+= float(rand() % 4000 - 2000);
				}
			//	31～50フレームで、移動対象位置まで移動する
				else if(this->iMoveFrame > 30 && this->iMoveFrame < 50)
				{
				//	1フレームで移動するベクトルを割り出し、移動させる
					this->calcLoc(&((this->targetLoc - this->getLoc()) / 20.0f));
				}
			//	50フレームに到達したら、フレームを初期化
				else if(this->iMoveFrame >= 50)
				{
					this->iMoveFrame  = 0;
				}
				break;
		//	攻撃
			case eOPEYW_ATTACK:
				this->iMoveFrame++;
			//	50フレームまで、チャージ
				if(	this->iMoveFrame < 50)
				{
					this->smokeColor.rgba.red	-= 6;
					this->smokeColor.rgba.green	-= 6;
					this->smokeColor.rgba.blue	-= 6;
				}
			//	50フレーム以上で、エネミーボールを発射
				else
				{
				//	エネミーボールを生成
					CENEMY_BALL*	newBall;
					newBall	= new CENEMY_BALL(this->originEnemyBall);
				//	ボールの角度を計算
					float	X, Y, dir;
					X	= this->getLocX() - (*this->player)->getLocX();
					Y	= this->getLocY() - (*this->player)->getLocY();
					dir	= atan2(Y, X) * 180.0f / D3DX_PI;
				//	エネミーボールの設定
					newBall->shot(
						&VEC3(0.0f, 90.0f, dir),						//!< 角度
						&this->getWorldCenter(),						//!< 発射位置
						NULL,											//!< ステージオブジェクトグループ
						this->player,									//!< 敵オブジェクトグループ
						300);											//!< 消滅時間
					newBall->setMoveQuantity(20.0f);					//!< スピード設定
					newBall->setScale(1.0f);							//!< 拡大
					newBall->setLine(ADVENT_FRONTHIT);					//!< 向き(3D空間に対して)
					newBall->setOrderMessage(eOMSG_DELETE_IN);			//!< 他解放
				//	エネミーボール発射音
					this->ssEnemyBallShot->setPlayCur(0);
					this->ssEnemyBallShot->play(0, NULL);
				//	エネミーボールを描画グループに登録
					this->viewG->add(newBall);
				//	動作制御を浮遊に変更
					this->setWeaponAction(eOPEYW_FLOATING);
				}
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CYAMAKAGE_WEAPON::draw
	//
	//	機能		描画
	//	引数		inDev			Direct3Dデバイス
	//	更新		2009/02/17		<新規>
	//================================================================================
	BOOL
	CYAMAKAGE_WEAPON::draw(CONST DEV inDev)
	{
	//	通常描画
		return	CSKIN_MESH::draw(inDev);
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------