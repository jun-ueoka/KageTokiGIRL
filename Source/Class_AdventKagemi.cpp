//***********************************************************************************
//!	ファイル名		Class_AdventKagemi.cpp
//
//	機能			アドベンチャーゲームモードの「カゲミ」クラスの実装
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2008/12/09	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_AdventKagemi.h"		//!< アドベンチャーゲーム「カゲミ」

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::コンストラクタ
	//
	//	引数		inDev			Direct3Dデバイス
	//				inViewG			描画グループ(シャドウボールを入れる)
	//				inName			Xファイルの名称
	//				inResource		メッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2008/12/10		<新規>
	//================================================================================
	CADVENT_KAGEMI::CADVENT_KAGEMI(CONST DEV	inDev,
								   CONST LPSTR	inName,
								   CONST LPSTR	inResource,
								   CONST LPSTR	inTexture)	:
	CKAGETOKI_ADVENT_CHARACTER(inDev, inName, inResource, inTexture)
	{
	//	属性の初期化
		this->fJumpHeight				= ADVENT_JUMP_HEIGHT;	//!< ジャンプの高さ
		this->fLeftRightMoveSpeed		= 0.0f;					//!< 左右移動スピード
		this->fLeftRightMoveMaxSpeed	= 20.0f;				//!< 左右移動最大スピード
		this->fLeftRightMoveAccele		= 2.0f;					//!< 左右移動加速値

		this->fFloorSpace		= 8.0f;							//!< 床との間隔
		this->fWallSpace		= 100.0f;						//!< 壁との間隔

	//	アニメーションの割り当てを行う(カゲミ専用のクラスなので)
		this->iAnimTemp[eADKAGEMIA_WAIT]						= 0;
		this->iAnimTemp[eADKAGEMIA_WALK]						= 5;
		this->iAnimTemp[eADKAGEMIA_FLIGHT]						= 2;
		this->iAnimTemp[eADKAGEMIA_SKILL_ENERGY_BALL_SHOT]		= 4;
		this->iAnimTemp[eADKAGEMIA_SKILL_ENERGY_BALL_OPERATION]	= 11;
		this->iAnimTemp[eADKAGEMIA_SKILL_ENERGY_SHOOT]			= 11;
		this->iAnimTemp[eADKAGEMIA_DAMAGE_LV1]					= 7;
		this->iAnimTemp[eADKAGEMIA_DAMAGE_LV2]					= 8;

	//	フラグ群
		this->limit				= eADKAGEMIL_MAX;				//!< 制御レベル4
		this->action			= eADKAGEMIA_MAX;				//!< 最初は待機以外何でも良い
		this->changeAction(eADKAGEMIA_WAIT);					//!< 初期状態は待機
		this->iHitFrame			= 0;							//!< 衝突して点滅する時間
		this->bJunpInput		= FALSE;						//!< 飛翔入力確認

	//	照準矢印
		this->texArrow	= new CTEXTURE(inDev, "arrowKagemi", VEC2(200.0f, 200.0f));
		this->texArrow->setMoveQuantity(300.0f);

		this->ballAngle			= VEC3(0.0f, ADVENT_RIGHTANGLE, 0.0f);
	//	オリジナル「エナジーボール」の作成、設定
		this->originEnergyBall
			= new CKAGEMI_BALL(inDev, eKMIBALL_ENERGY);
		this->opeEnergyBall		= NULL;
		this->energyBallSpeed	= 20.0f;
		this->energyBallFrame	= 300;
		this->energyShootSpeed	= 50.0f;
		this->energyShootFrame	= 100;

	//	キャラクターなので、当たり判定方法に高さを付ける
		this->setHeightValid(true);
		this->setHeightValue(this->getWorldRadius());
		this->setWidthValue(this->getWorldRadius() / 2.0f);

		this->stepCnt			= 0;
	//	音のセット
		this->sStaticStep		= new SSTATIC(SOUND_STEP);
		this->sStaticJumpStart	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticJumpEnd	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticShotEnergy	= new SSTATIC(SOUND_SHOT_ENERGY);

	//	エフェクトの登録
		C3D_EFFECT_CENTER*	effectCenter	= C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			EFFECT_SMOKE,
			new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEMI_COMMENT_ENERGYBALL,
			new C3D_EFFECT(inDev, KAGEMI_COMMENT_ENERGYBALL, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEMI_COMMENT_ENERGYSHOOT,
			new C3D_EFFECT(inDev, KAGEMI_COMMENT_ENERGYSHOOT, VEC3(0.0f,0.0f,0.0f)));

	//	ローカル座標の変更
		this->sd_ptr->mesh->localCenter = VEC3(0.0f, 100.0f, 0.0f);
		this->sd_ptr->mesh->localRadius	= 150.0f;
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::デストラクタ
	//
	//	更新		2008/12/10		<新規>
	//================================================================================
	CADVENT_KAGEMI::~CADVENT_KAGEMI(void)
	{
	//	音関連の解放
		SAFE_DELETE(this->sStaticStep);				//!< 足音
		SAFE_DELETE(this->sStaticJumpStart);		//!< ジャンプ音
		SAFE_DELETE(this->sStaticJumpEnd);			//!< 着地音
		SAFE_DELETE(this->sStaticShotEnergy);		//!< エナジーボール発射音

		SAFE_DELETE(this->originEnergyBall);		//!< オリジナル「エナジーボール」の解放
		SAFE_DELETE(this->opeEnergyBall);			//!< 操作中エナジーボール
		SAFE_DELETE(this->texArrow);				//!< 照準矢印
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::soundStep
	//
	//	機能		足音管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::soundStep(void)
	{
		this->stepCnt++;

	//	スモークは15フレーム毎に発生
		if(	this->stepCnt > 15)
		{
			C3D_EFFECT_CENTER::occuEffectsInstance(EFFECT_SMOKE, this->getLoc(), 0.3f);
			this->sStaticStep->setPlayCur(0);
			this->sStaticStep->play(0, NULL);
			this->stepCnt	= 0;
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::soundJumpStart
	//
	//	機能		ジャンプ音管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::soundJumpStart(void)
	{
	//	ジャンプ音を鳴らす
		this->sStaticJumpStart->play(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::soundJumpEnd
	//
	//	機能		着地音管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::soundJumpEnd(void)
	{
	//	着地音を鳴らす
		this->sStaticJumpEnd->play(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::soundShotEnergy
	//
	//	機能		エナジーボール発射音管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::soundShotEnergy(void)
	{
	//	エナジーボール発射音を交互に鳴らす
		this->sStaticShotEnergy->setPlayCur(0);
		this->sStaticShotEnergy->play(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::opeWait
	//
	//	機能		待機時操作
	//	引数		*input			入力管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::opeWait(CINPUT* input)
	{
	//	アドベンチャーゲーム操作
		this->opeAdvent(input);
	//	重力操作
		this->opeEffectGravity();

	//	判定
		if(	input->checkPressOnce(ADVENT_JUMP))
		{
			this->moveJumping(this->fJumpHeight);
		//	ジャンプ音
			this->soundJumpStart();
		//	飛翔入力有り
			this->bJunpInput	= TRUE;
		}

	//	上記の処理で、空中にいるなら、状態を「飛行」に変更
		if(	this->bFlighting)
		{
			this->changeAction(eADKAGEMIA_FLIGHT);
		}
	//	技判定
		else if(this->opeSkillCheck(input))
		{
		}
	//	空中にいないで、方向キーが押されているなら、状態を「歩行」に変更
		else if(input->checkPress(ADVENT_LEFT)	||
				input->checkPress(ADVENT_RIGHT))
		{
			this->changeAction(eADKAGEMIA_WALK);
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::opeWalk
	//
	//	機能		歩行時操作
	//	引数		*input			入力管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::opeWalk(CINPUT* input)
	{
	//	操作中のエナジーボールがあるなら、グループに処理を任せる
		if(	this->opeEnergyBall)
		{
		//	操作を描画に任せる
			this->viewG->add(this->opeEnergyBall);
		//	操作中エナジーボールのアクセスを切る
			this->opeEnergyBall	= NULL;
		}
	//	足音の発生
		this->soundStep();

	//	アドベンチャーゲーム操作
		this->opeAdvent(input);
	//	重力操作
		this->opeEffectGravity();

	//	ジャンプ判定
		if(	input->checkPressOnce(ADVENT_JUMP))
		{
			this->moveJumping(this->fJumpHeight);
		//	ジャンプ音
			this->soundJumpStart();
		//	飛翔入力有り
			this->bJunpInput	= TRUE;
		}

	//	上記の処理で、空中にいるなら、状態を「飛行」に変更
		if(	this->bFlighting)
		{
			this->changeAction(eADKAGEMIA_FLIGHT);
		}
	//	技判定
		else if(this->opeSkillCheck(input))
		{
		}
	//	空中にいないで、左右に移動していないなら、状態を「待機」に変更
		else if(this->fLeftRightMoveSpeed == 0.0f)
		{
			this->changeAction(eADKAGEMIA_WAIT);
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::opeFlight
	//
	//	機能		飛行時操作
	//	引数		*input			入力管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::opeFlight(CINPUT* input)
	{
	//	アドベンチャーゲーム操作
		this->opeAdvent(input);
	//	重力操作
		this->opeEffectGravity();

	//	飛翔入力がされていて、飛翔入力がされ続けてなかったら、ジャンプ力をあげる
		if(	this->bJunpInput				&&
			!input->checkPress(ADVENT_JUMP)	&&
			(this->fUpDownSpeed > 0.0f))
		{
			this->fUpDownSpeed -= 5.0f;
		}

	//	技判定
		if(	this->opeSkillCheck(input))
		{
		}
	//	上記の処理で、空中にいないなら、状態を待機に変更
		else if(!this->bFlighting)
		{
			this->changeAction(eADKAGEMIA_WAIT);
		//	着地音
			this->soundJumpEnd();
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::opeSkillEnergyBallShot
	//
	//	機能		技「エナジーボール発射」時
	//	引数		*input			入力管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::opeSkillEnergyBallShot(CINPUT* input)
	{
	//	フレームを進めて、エナジーボールが出るタイミングになった時
		this->iFrameCnt++;
		if(	this->iFrameCnt >= MAXFRAME_ENERGY_BALL)
		{
		//	アニメーションをエナジーボール操作に変更し、エナジーボール発射
			this->changeAction(eADKAGEMIA_SKILL_ENERGY_BALL_OPERATION);
		//	フレームカウンタを初期化
			this->iFrameCnt	= 0;
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::opeSkillEnergyBallOperation
	//
	//	機能		技「エナジーボール操作」時
	//	引数		*input			入力管理
	//	更新		2008/12/11		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::opeSkillEnergyBallOperation(CINPUT* input)
	{
	//	操作中のエナジーボールがなければ、待機に移る
		if(	!this->opeEnergyBall)
		{
			this->changeAction(eADKAGEMIA_WAIT);
			return;
		}

	//	上入力が
		
	//	入力をバイト型で確保
		BYTE	byInput	=
			((input->checkPress(ADVENT_TOP)		? 0x01 : 0x00)	|
			 (input->checkPress(ADVENT_BOTTOM)	? 0x02 : 0x00)	|
			 (input->checkPress(ADVENT_LEFT)	? 0x04 : 0x00)	|
			 (input->checkPress(ADVENT_RIGHT)	? 0x08 : 0x00));
		float	targetAngle	= this->opeEnergyBall->getAngleZ();
	//	入力確認
		switch(byInput)
		{
			case 0x01:	targetAngle = 90.0f;	break;	//!< 上
			case 0x02:	targetAngle = 270.0f;	break;	//!< 下
			case 0x04:	targetAngle = 180.0f;	break;	//!< 左
			case 0x08:	targetAngle = 0.0f;		break;	//!< 右
			case 0x05:	targetAngle = 135.0f;	break;	//!< 左上
			case 0x09:	targetAngle = 45.0f;	break;	//!< 右上
			case 0x06:	targetAngle = 225.0f;	break;	//!< 左下
			case 0x10:	targetAngle = 315.0f;	break;	//!< 右下
			case 0x07:	targetAngle = 180.0f;	break;	//!< 上下左
			case 0x11:	targetAngle = 0.0f;		break;	//!< 上下右
			case 0x13:	targetAngle = 90.0f;	break;	//!< 上左右
			case 0x14:	targetAngle = 270.0f;	break;	//!< 下左右
		}
		this->opeEnergyBall->setAngleZ(targetAngle);

		this->iFrameCnt++;
	//	エナジーボール発射ボタンが押されてない時か、
	//	フレームを進めて、エナジーボールが操作終了タイミングになった時
		if(	!(input->checkPress(KAGEMI_ENERGY_BALL))	||
			(this->iFrameCnt >= MAXFRAME_ENERGY_BALL_OPERATION))
		{
		//	安全に待機状態に以降
			this->safeResetChangeToWait();
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::opeSkillEnergyShoot
	//
	//	機能		技「エナジーシュート」時
	//	引数		*input			入力管理
	//	更新		2009/01/24		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::opeSkillEnergyShoot(CINPUT* input)
	{
	//	重力操作
		this->opeEffectGravity();
	//	フレームを進めて、エナジーシュートが出るタイミングになった時
		this->opeBallAngle(input);

		this->iFrameCnt++;
		if(	((this->iFrameCnt > 10)	&&
			!input->checkPress(KAGEMI_ENERGY_SHOOT))	||
			this->iFrameCnt >= MAXFRAME_ENERGY_SHOOT)
		{
		//	エナジーボールを放つ
			this->skillEnergyShoot();
		//	フレームカウンタを初期化
			this->iFrameCnt	= 0;
		//	アニメーションを待機に戻す
			this->changeAction(eADKAGEMIA_WAIT);
		//	エナジーシュートの吹き出し
			C3D_EFFECT_CENTER::occuEffectsInstance(
				KAGEMI_COMMENT_ENERGYSHOOT, this->getWorldCenter(), 1.0f);
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::opeDamageLv1
	//
	//	機能		ダメージLv1反動時操作
	//	引数		*input			入力管理
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::opeDamageLv1(CINPUT* input)
	{
	//	重力操作
		this->opeEffectGravity();
	//	フレームが一定まで行ったら、待機
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10)
		{
			this->changeAction(eADKAGEMIA_WAIT);
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::opeDamageLv2
	//
	//	機能		ダメージLv2反動時操作
	//	引数		*input			入力管理
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::opeDamageLv2(CINPUT* input)
	{
	//	重力操作
		this->opeEffectGravity();
	//	フレームが一定まで行ったら、待機
		this->iFrameCnt++;
		if(	this->iFrameCnt > 20)
		{
			this->changeAction(eADKAGEMIA_WAIT);
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::opeSkillCheck
	//
	//	機能		技入力判定
	//	引数		*input			入力管理
	//	戻り値		TRUE			行動パターンが変わった
	//				FALSE			行動パターンが変わっていない
	//	更新		2008/12/10		<新規>
	//================================================================================
	BOOL
	CADVENT_KAGEMI::opeSkillCheck(CINPUT* input)
	{
	//	ちゃんと左右を向いていなければ、処理をしない
		if(	this->way != eADCW_LEFT &&
			this->way != eADCW_RIGHT)	return FALSE;

	//	新たな行動パターンを確保する為
		ADVENT_KAGEMI_ACTION	newAction	= this->action;

	//	「エナジーボール発射」判定
		if(	(this->limit>= eADKAGEMIL_LV2)					&&
			(input->checkPressOnce(KAGEMI_ENERGY_BALL)))
		{
			newAction	= eADKAGEMIA_SKILL_ENERGY_BALL_SHOT;
			if		(this->way == eADCW_RIGHT)	this->ballAngle.z	= 360.0f;
			else if	(this->way == eADCW_LEFT)	this->ballAngle.z	= 180.0f;
		}

	//	「エナジーシュート」判定
		else
		if(	(this->limit>= eADKAGEMIL_LV3)					&&
			(input->checkPressOnce(KAGEMI_ENERGY_SHOOT)))
		{
			newAction	= eADKAGEMIA_SKILL_ENERGY_SHOOT;
			if		(this->way == eADCW_RIGHT)	this->ballAngle.z	= 360.0f;
			else if	(this->way == eADCW_LEFT)	this->ballAngle.z	= 180.0f;
		}

	//	新たな行動パターンに変化したかの、結果を返す
		return	this->changeAction(newAction);
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::opeBallAngle
	//
	//	機能		ボール技の方向判定
	//	引数		*input			入力管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::opeBallAngle(CINPUT* input)
	{
	//	右向きの場合
		if		(this->way == eADCW_RIGHT)
		{
			if(	input->checkPress(ADVENT_TOP))
			{
				if(	(this->ballAngle.z + 5.0f) < 450.0f)	this->ballAngle.z += 5.0f;
				else										this->ballAngle.z  = 450.0f;
			}
			else if(input->checkPress(ADVENT_BOTTOM))
			{
				if(	(this->ballAngle.z - 5.0f) > 270.0f)	this->ballAngle.z -= 5.0f;
				else										this->ballAngle.z  = 270.0f;
			}
		}
	//	左向きの場合
		else if	(this->way == eADCW_LEFT)
		{
			if(	input->checkPress(ADVENT_TOP))
			{
				if(	(this->ballAngle.z - 5.0f) >  90.0f)	this->ballAngle.z -= 5.0f;
				else										this->ballAngle.z  = 90.0f;
			}
			else if(input->checkPress(ADVENT_BOTTOM))
			{
				if(	(this->ballAngle.z + 5.0f) < 270.0f)	this->ballAngle.z += 5.0f;
				else										this->ballAngle.z  = 270.0f;
			}
		}
		this->ballAngle.y	= ADVENT_RIGHTANGLE;
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::skillEnergyBall
	//
	//	機能		技「エナジーボール」
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::skillEnergyBall(void)
	{
	//	エナジーパワーに、エナジーボールを撃つだけの余裕がある時

		//	エナジーボールを生成
			CKAGEMI_BALL*	newBall	= new CKAGEMI_BALL(this->originEnergyBall);
		//	エナジーボールの設定
			newBall->shot(
				&this->ballAngle,								//!< 角度
				&this->getWorldCenter(),						//!< 発射位置
				this->stageG,									//!< ステージオブジェクトグループ
				this->enemyG,									//!< 敵オブジェクトグループ
				this->energyBallFrame);							//!< 消滅時間
			newBall->setMoveQuantity(this->energyBallSpeed);	//!< スピード設定
			newBall->setScale(3.0f);							//!< 拡大
			newBall->setLine(ADVENT_FRONTHIT);					//!< 向き(3D空間に対して)
			newBall->setOrderMessage(eOMSG_DELETE_IN);			//!< 他解放
		//	煙エフェクト発生
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE,
				newBall->getLoc(),
				0.5f,
				VEC3(0.0f,0.0f,0.0f),
				D3DCOLOR_RGBA(255, 200, 200, 255));
		//	エナジーボール発射音
			this->soundShotEnergy();
		//	エナジーボール操作対象に登録
			this->opeEnergyBall	= newBall;
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::skillEnergyShoot
	//
	//	機能		技「エナジーシュート」
	//	更新		2009/01/24		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::skillEnergyShoot(void)
	{
	//	エナジーパワーに、エナジーシュートを撃つだけの余裕がある時

		//	エナジーシュートを生成
			CKAGEMI_BALL*	newBall	= new CKAGEMI_BALL(this->originEnergyBall);
		//	エナジーシュートの設定
			newBall->shot(
				&this->ballAngle,								//!< 角度
				&this->getWorldCenter(),						//!< 発射位置
				this->stageG,									//!< ステージオブジェクトグループ
				this->enemyG,									//!< 敵オブジェクトグループ
				this->energyShootFrame);						//!< 消滅時間
			newBall->setMoveQuantity(this->energyShootSpeed);	//!< スピード設定
			newBall->setScale(3.0f);							//!< 拡大
			newBall->setLine(ADVENT_FRONTHIT);					//!< 向き(3D空間に対して)
			newBall->setOrderMessage(eOMSG_DELETE_IN);			//!< 他解放
		//	煙エフェクト発生
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE,
				newBall->getLoc(),
				0.5f,
				VEC3(0.0f,0.0f,0.0f),
				D3DCOLOR_RGBA(255, 200, 200, 255));
		//	エナジーボール発射音
			this->soundShotEnergy();
		//	エナジーボール操作対象に登録
			this->viewG->add(newBall);
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::changeAction
	//
	//	機能		状態変化
	//	引数		inNewAction		新たな行動パターン
	//	戻り値		TRUE			新たな行動パターンに変化した
	//				FALSE			新たな行動パターンに変化していない
	//	更新		2008/12/10		<新規>
	//================================================================================
	BOOL
	CADVENT_KAGEMI::changeAction(ADVENT_KAGEMI_ACTION inNewAction)
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
		//	待機時
			case	eADKAGEMIA_WAIT:
				this->bJunpInput	= FALSE;
				this->setLocZ(0.0f);
				break;
		//	歩行時
			case	eADKAGEMIA_WALK:
				break;
		//	飛行時
			case	eADKAGEMIA_FLIGHT:
				break;
		//	技「エナジーボール発射」
			case	eADKAGEMIA_SKILL_ENERGY_BALL_SHOT:
			//	エナジーボールの吹き出し
				C3D_EFFECT_CENTER::occuEffectsInstance(
					KAGEMI_COMMENT_ENERGYBALL, this->getWorldCenter(), 1.0f);
				this->bJunpInput	= FALSE;
				break;
		//	技「エナジーボール操作」
			case	eADKAGEMIA_SKILL_ENERGY_BALL_OPERATION:
			//	エナジーボール発生
				this->skillEnergyBall();
				this->bJunpInput	= FALSE;
				break;
		//	技「エナジーシュート」
			case	eADKAGEMIA_SKILL_ENERGY_SHOOT:
				this->bJunpInput	= FALSE;
				break;
		//	ダメージLv1反動時
			case	eADKAGEMIA_DAMAGE_LV1:
			//	リアクション時の上昇値を設定
				this->fUpDownSpeed	= 20;
				this->bJunpInput	= FALSE;
				break;
		//	ダメージLv2反動時
			case	eADKAGEMIA_DAMAGE_LV2:
			//	リアクション時の上昇値を設定
				this->fUpDownSpeed	= 40;
				this->bJunpInput	= FALSE;
				break;
		//	上記以外
			default:
				break;
		}
	//	フレームカウンタを初期化
		this->iFrameCnt	= 0;

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::safeResetChangeToWait
	//
	//	機能		安全に待機に移行する
	//	更新		2009/02/18		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::safeResetChangeToWait(void)
	{
	//	操作中のエナジーボールがある場合
		if(	this->opeEnergyBall)
		{
		//	操作を描画に任せる
			this->viewG->add(this->opeEnergyBall);
		//	操作中エナジーボールのアクセスを切る
			this->opeEnergyBall	= NULL;
		}
	//	アニメーションを待機に戻す
		this->changeAction(eADKAGEMIA_WAIT);
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::reactionMessage
	//
	//	機能		リアクション
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::reactionMessage(REACTION_MESSAGE inMessage)
	{
	//	リアクション分岐
		switch(inMessage)
		{
		//	敵の攻撃Lv1
			case eREAMSG_ENEMY_ATTACK_LV1:
			//	衝突フレームが0以下なら、ダメージ
				if(	this->iHitFrame <= 0)
				{
				//	攻撃以外の時なら、後ずさる
					this->iHitFrame = 100;
					this->bDamage	= TRUE;
					this->iDamage	= 2;
					this->safeResetChangeToWait();
					this->changeAction(eADKAGEMIA_DAMAGE_LV1);
				}
				break;
		//	敵の攻撃Lv2
			case eREAMSG_ENEMY_ATTACK_LV2:
			//	衝突フレームが0以下なら、ダメージ
				if(	this->iHitFrame <= 0)
				{
				//	攻撃以外の時なら、後ずさる
					this->iHitFrame = 100;
					this->bDamage	= TRUE;
					this->iDamage	= 3;
					this->safeResetChangeToWait();
					this->changeAction(eADKAGEMIA_DAMAGE_LV1);
				}
				break;
		//	敵の攻撃Lv3
			case eREAMSG_ENEMY_ATTACK_LV3:
			//	衝突フレームが0以下なら、ダメージ
				if(	this->iHitFrame <= 0)
				{
				//	攻撃以外の時なら、後ずさる
					this->iHitFrame = 100;
					this->bDamage	= TRUE;
					this->iDamage	= 4;
					this->safeResetChangeToWait();
					this->changeAction(eADKAGEMIA_DAMAGE_LV2);
				}
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::operation
	//
	//	機能		基本操作
	//	引数		*input			入力管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEMI::operation(CINPUT* input)
	{
	//	表示されていなければ、処理しない
		if(	!this->getVisible())	return;

	////	グループの登録がなければ、処理しない
	//	if(	!this->viewG	||
	//		!this->stageG	||
	//		!this->enemyG)	return;

#ifdef	_DEBUG
		if(	input->checkPress(MYKEY::eF2))
		{
			if(	input->checkPress(MYKEY::eLEFT))	this->calcLocX(-50.0f);
			if(	input->checkPress(MYKEY::eRIGHT))	this->calcLocX( 50.0f);
			if(	input->checkPress(MYKEY::eDOWN))	this->calcLocY(-50.0f);
			if(	input->checkPress(MYKEY::eUP))		this->calcLocY( 50.0f);
			return;
		}
#endif

	//	状態による操作の分岐
		switch(this->action)
		{
		//	待機時
			case	eADKAGEMIA_WAIT:
				this->opeWait(input);
				break;
		//	歩行時
			case	eADKAGEMIA_WALK:
				this->opeWalk(input);
				break;
		//	飛行時
			case	eADKAGEMIA_FLIGHT:
				this->opeFlight(input);
				break;
		//	技「エナジーボール発射」
			case	eADKAGEMIA_SKILL_ENERGY_BALL_SHOT:
				this->opeSkillEnergyBallShot(input);
				break;
		//	技「エナジーボール操作」
			case	eADKAGEMIA_SKILL_ENERGY_BALL_OPERATION:
				this->opeSkillEnergyBallOperation(input);
				break;
		//	技「エナジーシュート」
			case	eADKAGEMIA_SKILL_ENERGY_SHOOT:
				this->opeSkillEnergyShoot(input);
				break;
		//	ダメージLv1反動時
			case	eADKAGEMIA_DAMAGE_LV1:
				this->opeDamageLv1(input);
				break;
		//	ダメージLv2反動時
			case	eADKAGEMIA_DAMAGE_LV2:
				this->opeDamageLv2(input);
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEMI::draw
	//
	//	機能		描画
	//	機能概要	スキンメッシュの描画と、操作中エナジーボールの描画を行う
	//	引数		inDev			Direct3Dデバイス
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2008/12/10		<新規>
	//================================================================================
	BOOL
	CADVENT_KAGEMI::draw(CONST DEV inDev)
	{
	//	操作中エナジーボールがあるなら、描画する
		if(	this->opeEnergyBall)
		{
			this->opeEnergyBall->draw(inDev);
			if(	!(this->opeEnergyBall->getVisible()))
			{
			//	操作を描画に任せる
				this->viewG->add(this->opeEnergyBall);
			//	操作中エナジーボールのアクセスを切る
				this->opeEnergyBall	= NULL;
			}
		}

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

	//	制御Lv2異常なら、照準矢印を描画
		if(	this->action == eADKAGEMIA_SKILL_ENERGY_SHOOT)
		{
			this->texArrow->setLoc(&this->getWorldCenter());
			this->texArrow->setAngle(&this->ballAngle);
			this->texArrow->moveFront();
			this->texArrow->setAngleY(0.0f);
			this->texArrow->draw(inDev);
		}

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------