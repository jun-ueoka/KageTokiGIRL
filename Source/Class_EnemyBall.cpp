//***********************************************************************************
//!	ファイル名		Class_EnemyBall.cpp
//
//	機能			敵が放つ玉を扱うクラスの実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2009/02/17		<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_EnemyBall.h"
//#include	"Class_3DEffectCenter.h"	//!< エフェクトセンター(エフェクトを発生させる為)

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CENEMY_BALL::コンストラクタ
	//
	//	引数		inDev			Direct3Dデバイス
	//				inKind			ボールの種類
	//	更新		2008/02/17		<新規>
	//================================================================================
	CENEMY_BALL::CENEMY_BALL(CONST DEV inDev, ENEMY_BALL_KIND inKind,
							 LPSTR inMeshName, LPSTR inEffectName)	:
	CMESH(inDev, inMeshName)
	{
	//	属性の初期化
		this->activate		= FALSE;
		this->stageG		= NULL;
		this->player		= NULL;
		this->iFrame		= 0;
		this->ballKind		= inKind;
		this->reactMessage	= eREAMSG_ENEMY_ATTACK_LV1;
		ZeroMemory(this->effectName, sizeof(this->effectName));
		this->iEffectCnt	= 10;
		this->iFrame		= 0;
		this->effectColor	= 0xFFFFFFFF;

	//	エフェクトの名前の確保
		sprintf_s(this->effectName, sizeof(this->effectName), inEffectName);

	//	エフェクトの登録
		C3D_EFFECT_CENTER*	effectCenter = C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			this->effectName, new C3D_EFFECT(inDev, this->effectName, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			EFFECT_SMOKE, new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));
	}

	//================================================================================
	//!	メソッド名	CENEMY_BALL::コンストラクタ
	//
	//	機能		コピー
	//	引数		inCopy			コピー元
	//	更新		2008/02/17		<新規>
	//================================================================================
	CENEMY_BALL::CENEMY_BALL(CENEMY_BALL *inCopy)
	:	CMESH(inCopy)
	{
	//	属性の初期化
		this->activate		= FALSE;
		this->stageG		= NULL;
		this->player		= NULL;
		this->ballKind		= inCopy->getBallKind();
		this->reactMessage	= inCopy->getReactMessage();
		strcpy_s(this->effectName, sizeof(this->effectName), inCopy->getEffectName());
		this->iEffectCnt	= inCopy->getEffectCnt();
		this->iFrame		= 0;
		this->effectColor	= inCopy->getEffectColor();
	}

	//================================================================================
	//!	メソッド名	CENEMY_BALL::デストラクタ
	//
	//	更新		2008/02/17		<新規>
	//================================================================================
	CENEMY_BALL::~CENEMY_BALL(void)
	{
	}

	//================================================================================
	//!	メソッド名	CENEMY_BALL::shot
	//
	//	機能		発射(生成した後に使用)
	//	引数		inAngle			発射角度
	//				inLoc			発射位置
	//				inStageG		判定ステージ
	//				inPlayer		判定プレイヤー
	//				inFrame			描画フレーム
	//	更新		2008/02/17		<新規>
	//================================================================================
	void
	CENEMY_BALL::shot(VEC3* inAngle, VEC3* inLoc, C3D_OBJECT_GROUP* inStageG,
					  CKAGETOKI_ADVENT_CHARACTER** inPlayer, int inFrame)
	{
	//	発射可能の時
		if(	!this->activate)
		{
		//	発射
			this->activate	= TRUE;
			this->stageG	= inStageG;
			this->player	= inPlayer;
			this->iFrame	= inFrame;
			this->setVisible(TRUE);
			this->setAngle(inAngle);
			this->setLoc(inLoc);
		}
	}

	//================================================================================
	//!	メソッド名	CENEMY_BALL::intersectG
	//
	//	機能		衝突判定
	//	更新		2008/02/17		<新規>
	//================================================================================
	void
	CENEMY_BALL::intersectG(void)
	{
	//	敵グループ衝突判定
		if(	this->player	&&
			(*this->player)->intersectRadius(this))
		{
		//	プレイヤーに当たったら、攻撃リアクションメッセージを渡す
			(*this->player)->reactionMessage(this->reactMessage);
		//	球消滅
			this->activate	= FALSE;
			this->iFrame	= 0;
		//	煙エフェクト発生
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE, this->getLoc(), 1.0f, VEC3(0.0f,0.0f,0.0f), D3DCOLOR_RGBA(0,0,0,255));
		}

		else
	//	ステージグループ衝突判定
		if(	this->stageG	&&
			this->stageG->intersect(this, this->getLine(), this->getMoveQuantity() + 1.0f))
		{
		//	球消滅
			this->activate	= FALSE;
			this->iFrame	= 0;
		//	煙エフェクト発生
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE, this->getLoc(), 1.0f, VEC3(0.0f,0.0f,0.0f), D3DCOLOR_RGBA(0,0,0,255));
		}
	}

	//================================================================================
	//!	メソッド名	CENEMY_BALL::opeStraight
	//
	//	機能		一直線に飛んでいく処理
	//	引数		inDev			Direct3Dデバイス
	//	更新		2008/02/20		<新規>
	//================================================================================
	void
	CENEMY_BALL::opeStraight(CONST DEV inDev)
	{
	//	発射されているなら、動作を行い、衝突判定をする
		if(	this->activate)
		{
		//	現在の描画フレームが、
		//	エフェクトの発生間隔フレームで割り切れたら、エフェクト発生
			if(	!(this->iFrame % this->iEffectCnt))
			{
				C3D_EFFECT_CENTER::occuEffectsInstance(
					this->effectName, this->getLoc(), 1.0f);
			}

		//	描画フレームを減らし、カウントが0以下になったら、自動消滅
			this->iFrame--;
			if(	this->iFrame <= 0)
			{
				this->setVisible(FALSE);
				this->activate	= FALSE;
			//	球に消滅メッセージ与える
				this->setOrderMessage(eOMSG_DELETE);
			}
		//	カウントが残っているなら、衝突判定
			else
			{
				this->intersectG();
			}
		//	前進
			this->move();
		}
		else
		{
			this->iFrame--;
			if(	this->iFrame <= 0)
			{
			//	球に消滅メッセージ与える
				this->setOrderMessage(eOMSG_DELETE);
				this->setVisible(FALSE);
			}
		}
	//	メッシュ描画
		CMESH::draw(inDev);
	}

	//================================================================================
	//!	メソッド名	CENEMY_BALL::opeShadollArrow
	//
	//	機能		弓矢の様に飛んでいく処理
	//	引数		inDev			Direct3Dデバイス
	//	更新		2008/02/20		<新規>
	//================================================================================
	void
	CENEMY_BALL::opeShadollArrow(CONST DEV inDev)
	{
	//	発射されているなら、動作を行い、衝突判定をする
		if(	this->activate)
		{
		//	現在の描画フレームが、
		//	エフェクトの発生間隔フレームで割り切れたら、エフェクト発生
			if(	!(this->iFrame % this->iEffectCnt))
			{
				C3D_EFFECT_CENTER::occuEffectsInstance(
					this->effectName, this->getLoc(), 0.1f);
			}

		//	敵グループ衝突判定
			if(	this->player	&&
				(*this->player)->intersectRadius(this))
			{
			//	プレイヤーに当たったら、攻撃リアクションメッセージを渡す
				(*this->player)->reactionMessage(this->reactMessage);
			//	球消滅
				this->activate	= FALSE;
				this->iFrame	= 0;
			//	煙エフェクト発生
				C3D_EFFECT_CENTER::occuEffectsInstance(
					EFFECT_SMOKE, this->getLoc(), 1.0f, VEC3(0.0f,0.0f,0.0f), D3DCOLOR_RGBA(0,0,0,255));
			}

			else
		//	ステージグループ衝突判定
			if(	this->stageG	&&
				this->stageG->intersect(this, this->getLine(), this->getMoveQuantity() + 1.0f))
			{
			//	球消滅
				this->activate	= FALSE;
				this->iFrame	= 50;
			}

		//	描画フレームを減らし、フレームが-15～0になったら、角度変更
			this->iFrame--;
			if(	(this->iFrame <    0) &&
				(this->iFrame >= -15))
			{
				this->calcAngleX(-6.0f);
			}
		//	200以下になったら、消滅
			else
			if(	this->iFrame <= -200)
			{
				this->setVisible(FALSE);
				this->activate	= FALSE;
			//	球に消滅メッセージ与える
				this->setOrderMessage(eOMSG_DELETE);
			}
		//	前進
			this->move();
		}
		else
		{
		//	メッシュ描画
			this->iFrame--;
			if(	this->iFrame <= 0)
			{
			//	球に消滅メッセージ与える
				this->setOrderMessage(eOMSG_DELETE);
				this->setVisible(FALSE);
			}
		}
		CMESH::draw(inDev);
	}

	//================================================================================
	//!	メソッド名	CENEMY_BALL::draw
	//
	//	機能		描画
	//	引数		inDev			Direct3Dデバイス
	//	更新		2008/02/17		<新規>
	//================================================================================
	BOOL
	CENEMY_BALL::draw(CONST DEV inDev)
	{
	//	ボールの種類で処理分岐
		switch(this->ballKind)
		{
		case eENEMYB_STRAIGHT:			this->opeStraight(inDev);			break;
		case eENEMYB_SHADOLL_ARROW:		this->opeShadollArrow(inDev);		break;
		}
		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CENEMY_BALL::draw
	//
	//	機能		描画(深度バッファシェーダ使用)
	//	引数		inDepthSha		深度バッファシェーダ
	//	更新		2008/02/17		<新規>
	//================================================================================
	BOOL
	CENEMY_BALL::draw(CDEPTH_SHA* inDepthSha)
	{
		return	this->draw(inDepthSha->getDevice());
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------