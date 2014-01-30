//***********************************************************************************
//!	ファイル名		Class_AdventKageo.cpp
//
//	機能			アドベンチャーゲームモードの「カゲオ」クラスの実装
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2008/12/09	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_AdventKageo.h"		//!< アドベンチャーゲーム「カゲオ」

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::コンストラクタ
	//
	//	引数		inDev			Direct3Dデバイス
	//				inViewG			描画グループ(シャドウボールを入れる)
	//				inName			Xファイルの名称
	//				inResource		メッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2008/12/10		<新規>
	//================================================================================
	CADVENT_KAGEO::CADVENT_KAGEO(CONST DEV		inDev,
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

	//	アニメーションの割り当てを行う(カゲオ専用のクラスなので、、、)
		this->iAnimTemp[eADKAGEOA_WAIT]					= 0;
		this->iAnimTemp[eADKAGEOA_WALK]					= 2;
		this->iAnimTemp[eADKAGEOA_FLIGHT]				= 3;
		this->iAnimTemp[eADKAGEOA_SKILL_SHADOW_BALL]	= 9;
		this->iAnimTemp[eADKAGEOA_SKILL_SHINE_BALL]		= 9;
		this->iAnimTemp[eADKAGEOA_DAMAGE_LV1]			= 7;
		this->iAnimTemp[eADKAGEOA_DAMAGE_LV2]			= 8;

		this->limit				= eADKAGEOL_MAX;				//!< 制御レベル3
		this->action			= eADKAGEOA_MAX;				//!< 最初は待機以外何でも良い
		this->changeAction(eADKAGEOA_WAIT);						//!< 初期状態は待機
		this->iHitFrame			= 0;							//!< 衝突点滅フレーム
		this->bJunpInput		= FALSE;						//!< 飛翔入力確認

	//	照準矢印
		this->texArrow	= new CTEXTURE(inDev, "arrowKageo", VEC2(200.0f, 200.0f));
		this->texArrow->setMoveQuantity(300.0f);

		this->ballAngle			= VEC3(0.0f, ADVENT_RIGHTANGLE, 0.0f);
	//	オリジナル「シャドウボール」の作成、設定
		this->originShadowBall
			= new CKAGEO_BALL(inDev, eKOBALL_SHADOW);
		this->shadowBallSpeed	= 50.0f;
		this->shadowBallFrame	= 100;

	//	オリジナル「シャインボール」の作成、設定
		this->originShineBall
			= new CKAGEO_BALL(inDev, eKOBALL_SHINE);
		this->shineBallSpeed	= 50.0f;
		this->shineBallFrame	= 100;

	//	キャラクターなので、当たり判定方法に高さを付ける
		this->setHeightValid(true);
		this->setHeightValue(this->getWorldRadius());
		this->setWidthValue(this->getWorldRadius() / 2.0f);

		this->stepCnt			= 0;
	//	音のセット
		this->sStaticStep		= new SSTATIC(SOUND_STEP);
		this->sStaticJumpStart	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticJumpEnd	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticShotShadow	= new SSTATIC(SOUND_SHOT_SHADOW);
		this->sStaticShotShine	= new SSTATIC(SOUND_SHOT_SHINE);

	//	エフェクトの登録
		C3D_EFFECT_CENTER*	effectCenter	= C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			EFFECT_SMOKE,
			new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEO_COMMENT_SHADOWBALL,
			new C3D_EFFECT(inDev, KAGEO_COMMENT_SHADOWBALL, VEC3(0.0f, 0.0f, 0.0f)));
		effectCenter->addEffects(
			KAGEO_COMMENT_SHINEBALL,
			new C3D_EFFECT(inDev, KAGEO_COMMENT_SHINEBALL, VEC3(0.0f, 0.0f, 0.0f)));

	//	ローカル座標の変更
		this->sd_ptr->mesh->localCenter = VEC3(0.0f, 100.0f, 0.0f);
		this->sd_ptr->mesh->localRadius	= 150.0f;
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::デストラクタ
	//
	//	更新		2008/12/10		<新規>
	//================================================================================
	CADVENT_KAGEO::~CADVENT_KAGEO(void)
	{
	//	音関連の解放
		SAFE_DELETE(this->sStaticStep);				//!< 足音
		SAFE_DELETE(this->sStaticJumpStart);		//!< ジャンプ音
		SAFE_DELETE(this->sStaticJumpEnd);			//!< 着地音
		SAFE_DELETE(this->sStaticShotShadow);		//!< シャドウボール発射音
		SAFE_DELETE(this->sStaticShotShine);		//!< シャインボール発射音

		SAFE_DELETE(this->originShadowBall);		//!< オリジナル「シャドウボール」の解放
		SAFE_DELETE(this->originShineBall);			//!< オリジナル「シャインボール」の解放
		SAFE_DELETE(this->texArrow);				//!< 照準矢印
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::soundStep
	//
	//	機能		足音管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::soundStep(void)
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
	//!	メソッド名	CADVENT_KAGEO::soundJumpStart
	//
	//	機能		ジャンプ音管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::soundJumpStart(void)
	{
	//	ジャンプ音を鳴らす
		this->sStaticJumpStart->play(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::soundJumpEnd
	//
	//	機能		着地音管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::soundJumpEnd(void)
	{
	//	着地音を鳴らす
		this->sStaticJumpEnd->play(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::soundShotShadow
	//
	//	機能		シャドウボール発射音管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::soundShotShadow(void)
	{
	//	シャドウボール発射音を交互に鳴らす
		this->sStaticShotShadow->setPlayCur(0);
		this->sStaticShotShadow->play(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::soundShotShine
	//
	//	機能		シャインボール発射音管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::soundShotShine(void)
	{
	//	シャインボール発射音を交互に鳴らす
		this->sStaticShotShine->setPlayCur(0);
		this->sStaticShotShine->play(0, NULL);
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::opeWait
	//
	//	機能		待機時操作
	//	引数		*input			入力管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::opeWait(CINPUT* input)
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
			this->changeAction(eADKAGEOA_FLIGHT);
		}
	//	技判定
		else if(this->opeSkillCheck(input))
		{
		}
	//	空中にいないで、方向キーが押されているなら、状態を「歩行」に変更
		else if(input->checkPress(ADVENT_LEFT)	||
				input->checkPress(ADVENT_RIGHT))
		{
			this->changeAction(eADKAGEOA_WALK);
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::opeWalk
	//
	//	機能		歩行時操作
	//	引数		*input			入力管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::opeWalk(CINPUT* input)
	{
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
			this->changeAction(eADKAGEOA_FLIGHT);
		}
	//	技判定
		else if(this->opeSkillCheck(input))
		{
		}
	//	空中にいないで、左右に移動していないなら、状態を「待機」に変更
		else if(this->fLeftRightMoveSpeed == 0.0f)
		{
			this->changeAction(eADKAGEOA_WAIT);
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::opeFlight
	//
	//	機能		飛行時操作
	//	引数		*input			入力管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::opeFlight(CINPUT* input)
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
			this->changeAction(eADKAGEOA_WAIT);
		//	着地音
			this->soundJumpEnd();
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::opeSkillShadowBall
	//
	//	機能		技「シャドウボール」時
	//	引数		*input			入力管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::opeSkillShadowBall(CINPUT* input)
	{
	//	重力操作
		this->opeEffectGravity();

	//	ボール発射角度を決定
		this->opeBallAngle(input);

	//	フレームを進めて、シャドウボールが出るタイミングになったら、発射
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10	&&
			!input->checkPress(KAGEO_SHADOW_BALL))
		{
		//	シャドウボール発生
			this->skillShadowBall();
		//	アニメーションを大気に戻す
			this->changeAction(eADKAGEOA_WAIT);
		//	フレームカウンタを初期化
			this->iFrameCnt	= 0;
		//	シャドウボールの吹き出し
			C3D_EFFECT_CENTER::occuEffectsInstance(
				KAGEO_COMMENT_SHADOWBALL, this->getWorldCenter(), 1.0f);
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::opeSkillShineBall
	//
	//	機能		技「シャドウボール」時
	//	引数		*input			入力管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::opeSkillShineBall(CINPUT* input)
	{
	//	重力操作
		this->opeEffectGravity();

	//	ボール発射角度を決定
		this->opeBallAngle(input);

	//	フレームを進めて、シャドウボールが出るタイミングになったら、発射
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10	&&
			!input->checkPress(KAGEO_SHINE_BALL))
		{
		//	シャドウボール発生
			this->skillShineBall();
		//	アニメーションを大気に戻す
			this->changeAction(eADKAGEOA_WAIT);
		//	フレームカウンタを初期化
			this->iFrameCnt	= 0;
		//	シャインボールの吹き出し
			C3D_EFFECT_CENTER::occuEffectsInstance(
				KAGEO_COMMENT_SHINEBALL, this->getWorldCenter(), 1.0f);
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::opeDamageLv1
	//
	//	機能		ダメージLv1反動時操作
	//	引数		*input			入力管理
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::opeDamageLv1(CINPUT* input)
	{
	//	重力操作
		this->opeEffectGravity();
	//	フレームが一定まで行ったら、待機
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10)
		{
			this->changeAction(eADKAGEOA_WAIT);
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::opeDamageLv2
	//
	//	機能		ダメージLv2反動時操作
	//	引数		*input			入力管理
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::opeDamageLv2(CINPUT* input)
	{
	//	重力操作
		this->opeEffectGravity();
	//	フレームが一定まで行ったら、待機
		this->iFrameCnt++;
		if(	this->iFrameCnt > 20)
		{
			this->changeAction(eADKAGEOA_WAIT);
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::opeSkillCheck
	//
	//	機能		技入力判定
	//	引数		*input			入力管理
	//	戻り値		TRUE			行動パターンが変わった
	//				FALSE			行動パターンが変わっていない
	//	更新		2008/12/10		<新規>
	//================================================================================
	BOOL
	CADVENT_KAGEO::opeSkillCheck(CINPUT* input)
	{
	//	ちゃんと左右を向いていなければ、処理をしない
		if(	this->way != eADCW_LEFT &&
			this->way != eADCW_RIGHT)	return FALSE;

	//	新たな行動パターンを確保する為
		ADVENT_KAGEO_ACTION	newAction	= this->action;

	//	「シャドウボール」判定
		if(	(this->limit>= eADKAGEOL_LV2)					&&
			(input->checkPressOnce(KAGEO_SHADOW_BALL)))
		{
			newAction	= eADKAGEOA_SKILL_SHADOW_BALL;
			if		(this->way == eADCW_RIGHT)	this->ballAngle.z	= 360.0f;
			else if	(this->way == eADCW_LEFT)	this->ballAngle.z	= 180.0f;
		}

	//	「シャインボール」判定
		else
		if(	(this->limit>= eADKAGEOL_LV3)					&&
			(input->checkPressOnce(KAGEO_SHINE_BALL)))
		{
			newAction	= eADKAGEOA_SKILL_SHINE_BALL;
			if		(this->way == eADCW_RIGHT)	this->ballAngle.z	= 360.0f;
			else if	(this->way == eADCW_LEFT)	this->ballAngle.z	= 180.0f;
		}

	//	新たな行動パターンに変化したかの、結果を返す
		return	this->changeAction(newAction);
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::opeBallAngle
	//
	//	機能		ボール技の方向判定
	//	引数		*input			入力管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::opeBallAngle(CINPUT* input)
	{
	//	制御レベルが方向判定レベルに到達してないなら、処理を返す
		if(	this->limit >= eADKAGEOL_LV4)	

	//	右向きの場合
		if		(this->way == eADCW_RIGHT)
		{
			if(	input->checkPress(ADVENT_TOP))
			{
				if(	(this->ballAngle.z + 3.0f) < 450.0f)	this->ballAngle.z += 3.0f;
				else										this->ballAngle.z  = 450.0f;
			}
			else if(input->checkPress(ADVENT_BOTTOM))
			{
				if(	(this->ballAngle.z - 3.0f) > 270.0f)	this->ballAngle.z -= 3.0f;
				else										this->ballAngle.z  = 270.0f;
			}
		}
	//	左向きの場合
		else if	(this->way == eADCW_LEFT)
		{
			if(	input->checkPress(ADVENT_TOP))
			{
				if(	(this->ballAngle.z - 3.0f) >  90.0f)	this->ballAngle.z -= 3.0f;
				else										this->ballAngle.z  = 90.0f;
			}
			else if(input->checkPress(ADVENT_BOTTOM))
			{
				if(	(this->ballAngle.z + 3.0f) < 270.0f)	this->ballAngle.z += 3.0f;
				else										this->ballAngle.z  = 270.0f;
			}
		}
		this->ballAngle.y	= ADVENT_RIGHTANGLE;
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::skillShadowBall
	//
	//	機能		技「シャドウボール」
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::skillShadowBall(void)
	{
	//	シャドーパワーに、シャドウボールを撃つだけの余裕がある時

		//	シャドウボールを生成
			CKAGEO_BALL*	newBall	= new CKAGEO_BALL(this->originShadowBall);
		//	シャドウボールの設定
			newBall->shot(
				&this->ballAngle,								//!< 角度
				&this->getWorldCenter(),						//!< 発射位置
				this->stageG,									//!< 影オブジェクトグループ
				this->shadowBallFrame);							//!< 消滅時間
			newBall->setMoveQuantity(this->shadowBallSpeed);	//!< スピード設定
			newBall->setScale(3.0f);							//!< 拡大
			newBall->setLine(ADVENT_FRONTHIT);					//!< 向き(3D空間に対して)
			newBall->setOrderMessage(eOMSG_DELETE_IN);				//!< 他解放
		//	煙エフェクト発生
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE,
				newBall->getLoc(),
				0.5f,
				VEC3(0.0f,0.0f,0.0f),
				D3DCOLOR_RGBA(10, 10, 10, 255));
		//	シャドウボール発射音
			this->soundShotShadow();
		//	メンバー登録
			this->viewG->add(newBall);

	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::skillShineBall
	//
	//	機能		技「シャインボール」
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::skillShineBall(void)
	{
	//	シャドーパワーに、シャドウボールを撃つだけの余裕がある時

		//	シャドウボールを生成
			CKAGEO_BALL*	newBall	= new CKAGEO_BALL(this->originShineBall);
		//	シャドウボールの設定
			newBall->shot(
				&this->ballAngle,								//!< 角度
				&this->getWorldCenter(),						//!< 発射位置
				this->stageG,									//!< 影オブジェクトグループ
				this->shineBallFrame);							//!< 消滅時間
			newBall->setMoveQuantity(this->shineBallSpeed);		//!< スピード設定
			newBall->setScale(3.0f);							//!< 拡大
			newBall->setLine(ADVENT_FRONTHIT);					//!< 向きは右(3D空間に対して)
			newBall->setOrderMessage(eOMSG_DELETE_IN);				//!< 他解放
		//	煙エフェクト発生
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE,
				newBall->getLoc(),
				0.5f,
				VEC3(0.0f,0.0f,0.0f),
				D3DCOLOR_RGBA(255, 255, 200, 255));
		//	シャインボール発射音
			this->soundShotShine();
		//	メンバー登録
			this->viewG->add(newBall);

	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::changeAction
	//
	//	機能		状態変化
	//	引数		inNewAction		新たな行動パターン
	//	戻り値		TRUE			新たな行動パターンに変化した
	//				FALSE			新たな行動パターンに変化していない
	//	更新		2008/12/10		<新規>
	//================================================================================
	BOOL
	CADVENT_KAGEO::changeAction(ADVENT_KAGEO_ACTION inNewAction)
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
			case	eADKAGEOA_WAIT:
				this->bJunpInput	= FALSE;
				this->setLocZ(0.0f);
				break;
		//	歩行時
			case	eADKAGEOA_WALK:
				break;
		//	飛行時
			case	eADKAGEOA_FLIGHT:
				break;
		//	技「シャドウボール」
			case	eADKAGEOA_SKILL_SHADOW_BALL:
				this->bJunpInput	= FALSE;
				break;
		//	技「シャインボール」
			case	eADKAGEOA_SKILL_SHINE_BALL:
				this->bJunpInput	= FALSE;
				break;
		//	ダメージLv1反動時
			case	eADKAGEOA_DAMAGE_LV1:
			//	リアクション時の上昇値を設定
				this->fUpDownSpeed	= 20;
				this->bJunpInput	= FALSE;
				break;
		//	ダメージLv2反動時
			case	eADKAGEOA_DAMAGE_LV2:
			//	リアクション時の上昇値を設定
				this->fUpDownSpeed	= 40;
				this->bJunpInput	= FALSE;
				break;
			default:								//!< 上記以外
				break;
		}
	//	フレームカウンタを初期化
		this->iFrameCnt	= 0;

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::reactionMessage
	//
	//	機能		リアクション
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::reactionMessage(REACTION_MESSAGE inMessage)
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
					this->changeAction(eADKAGEOA_DAMAGE_LV1);
					this->bDamage	= TRUE;
					this->iDamage	= 1;
				}
				break;
		//	敵の攻撃Lv2
			case eREAMSG_ENEMY_ATTACK_LV2:
			//	衝突フレームが0以下なら、ダメージ
				if(	this->iHitFrame <= 0)
				{
				//	攻撃以外の時なら、後ずさる
					this->iHitFrame = 100;
					this->changeAction(eADKAGEOA_DAMAGE_LV1);
					this->bDamage	= TRUE;
					this->iDamage	= 2;
				}
				break;
		//	敵の攻撃Lv3
			case eREAMSG_ENEMY_ATTACK_LV3:
			//	衝突フレームが0以下なら、ダメージ
				if(	this->iHitFrame <= 0)
				{
				//	攻撃以外の時なら、後ずさる
					this->iHitFrame = 100;
					this->changeAction(eADKAGEOA_DAMAGE_LV2);
					this->bDamage	= TRUE;
					this->iDamage	= 3;
				}
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::operation
	//
	//	機能		基本操作
	//	引数		*input			入力管理
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CADVENT_KAGEO::operation(CINPUT* input)
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
#else	
#ifdef	PRESENTATION
		if(	input->checkPress(MYKEY::eF2))
		{
			if(	input->checkPress(MYKEY::eLEFT))	this->calcLocX(-50.0f);
			if(	input->checkPress(MYKEY::eRIGHT))	this->calcLocX( 50.0f);
			if(	input->checkPress(MYKEY::eDOWN))	this->calcLocY(-50.0f);
			if(	input->checkPress(MYKEY::eUP))		this->calcLocY( 50.0f);
			return;
		}
#endif
#endif

	//	状態による操作の分岐
		switch(this->action)
		{
		//	待機時
			case	eADKAGEOA_WAIT:
				this->opeWait(input);
				break;
		//	歩行時
			case	eADKAGEOA_WALK:
				this->opeWalk(input);
				break;
		//	飛行時
			case	eADKAGEOA_FLIGHT:
				this->opeFlight(input);
				break;
		//	技「シャドウボール」
			case	eADKAGEOA_SKILL_SHADOW_BALL:
				this->opeSkillShadowBall(input);
				break;
		//	技「シャインボール」
			case	eADKAGEOA_SKILL_SHINE_BALL:
				this->opeSkillShineBall(input);
				break;
		//	ダメージLv1反動時
			case	eADKAGEOA_DAMAGE_LV1:
				this->opeDamageLv1(input);
				break;
		//	ダメージLv2反動時
			case	eADKAGEOA_DAMAGE_LV2:
				this->opeDamageLv2(input);
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_KAGEO::draw
	//
	//	機能		描画
	//	引数		*inDev			Direct3Dデバイス
	//	更新		2008/12/10		<新規>
	//================================================================================
	BOOL
	CADVENT_KAGEO::draw(CONST DEV inDev)
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

	//	シャドーボール発射前かシャインボール発射前なら、照準矢印を描画
		if(	this->action == eADKAGEOA_SKILL_SHADOW_BALL ||
			this->action == eADKAGEOA_SKILL_SHINE_BALL)
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