//***********************************************************************************
//!	ファイル名		Class_KagemiBall.cpp
//
//	機能			カゲミが放つ、玉を扱うクラスの実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/12/11		<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_KagemiBall.h"
//#include	"Class_3DEffectCenter.h"	//!< エフェクトセンター(エフェクトを発生させる為)

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CKAGEMI_BALL::コンストラクタ
	//
	//	引数		inDev			Direct3Dデバイス
	//				inKind			ボールの種類
	//	更新		2008/12/11		<新規>
	//================================================================================
	CKAGEMI_BALL::CKAGEMI_BALL(CONST DEV inDev, KAGEMI_BALL_KIND inKind)	:
	CMESH(inDev, getBallTextureName(inKind))
	{
	//	属性の初期化
		this->activate	= FALSE;
		this->stageG	= NULL;
		this->enemyG	= NULL;
		this->iFrame	= 0;

	//	ボールの種類により、初期化処理分岐
		switch(inKind)
		{
		//	シャドウボール
			case	eKMIBALL_ENERGY:
			//	敵にダメージを与えるメッセージ
				this->judgMessage	= eTMSG_ENEMY;
				this->reactMessage	= eREAMSG_ENERGY_HIT;
				this->setThisMessage(eTMSG_ENERGY_BALL);
			//	エフェクト関連
				this->iEffectCnt	= 3;
				strcpy_s(this->effectName, sizeof(effectName), EFFECT_ENERGY);
			//	煙の色
				this->smokeColor	= D3DCOLOR_RGBA(255, 200, 200, 255);
				break;

		//	上記以外(エラー数値)
			default:
				this->judgMessage	= eTMSG_NOMSG;
				this->reactMessage	= eREAMSG_NOMSG;
				this->setThisMessage(eTMSG_NOMSG);
				this->iEffectCnt	= 100000;
				strcpy_s(this->effectName, sizeof(effectName), "");
				this->smokeColor	= D3DCOLOR_RGBA(255, 255, 255, 255);
				return;
		}

	//	エフェクトの登録
		C3D_EFFECT_CENTER*	effectCenter = C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			this->effectName,
			new C3D_EFFECT(inDev, this->effectName, VEC3(0.0f,0.0f,0.0f)));
	}

	//================================================================================
	//!	メソッド名	CKAGEMI_BALL::コンストラクタ
	//
	//	機能		コピー
	//	引数		inCopy			コピー元
	//	更新		2008/12/11		<新規>
	//================================================================================
	CKAGEMI_BALL::CKAGEMI_BALL(CKAGEMI_BALL *inCopy)
	:	CMESH(inCopy)
	{
	//	属性の初期化
		this->activate		= FALSE;
		this->judgMessage	= inCopy->getJudgMessage();
		this->reactMessage	= inCopy->getReactMessage();
		this->setThisMessage(inCopy->getThisMessage());
		this->stageG		= NULL;
		this->enemyG		= NULL;
		strcpy_s(this->effectName, sizeof(this->effectName), inCopy->getEffectName());
		this->iEffectCnt	= inCopy->getEffectCnt();
		this->iFrame		= 0;
		this->smokeColor	= inCopy->getSmokeColor();
	}

	//================================================================================
	//!	メソッド名	CKAGEMI_BALL::デストラクタ
	//
	//	更新		2008/12/11		<新規>
	//================================================================================
	CKAGEMI_BALL::~CKAGEMI_BALL(void)
	{
	}

	//================================================================================
	//!	メソッド名	CKAGEMI_BALL::getBallTextureName
	//
	//	機能		カゲミボールの種類にで、テクスチャ名の検索
	//	引数		inPtn		コメントパターン
	//	戻り値		テクスチャ名
	//	更新		2008/12/11		<新規>
	//================================================================================
	LPSTR
	CKAGEMI_BALL::getBallTextureName(KAGEMI_BALL_KIND inKind)
	{
	//	パターン分岐
		switch(inKind)
		{
			case	eKMIBALL_ENERGY:	return	ENERGY_BALL;		break;
			default:					return	NULL;				break;
		}
	}

	//================================================================================
	//!	メソッド名	CKAGEMI_BALL::shot
	//
	//	機能		発射(生成した後に使用)
	//	引数		*inAngle		発射方向
	//				*inRotX			X軸の回転マトリックス
	//				*inRotY			Y軸の回転マトリックス
	//				*inRotZ			Z軸の回転マトリックス
	//				*inLoc			発射地点
	//				*inHitG			衝突判定グループ
	//				inFrame			描画フレーム
	//	更新		2008/12/11		<新規>
	//================================================================================
	void
	CKAGEMI_BALL::shot(VEC3* inAngle, VEC3* inLoc, C3D_OBJECT_GROUP* inStageG,
					   C3D_OBJECT_GROUP* inEnemyG, int inFrame)
	{
	//	発射可能の時
		if(	!this->activate)
		{
		//	発射
			this->stageG	= inStageG;
			this->enemyG	= inEnemyG;
			this->activate	= TRUE;
			this->iFrame	= inFrame;
			this->setVisible(TRUE);
			this->setAngle(inAngle);
			this->setLoc(inLoc);
		}
	}

	//================================================================================
	//!	メソッド名	CKAGEMI_BALL::intersectG
	//
	//	機能		衝突判定
	//	更新		2008/12/11		<新規>
	//================================================================================
	void
	CKAGEMI_BALL::intersectG(void)
	{
	//	敵グループ衝突判定
		if(	this->enemyG	&&
			this->enemyG->intersect(this, this->getLine(), this->getMoveQuantity() + 1.0f))
		{
		//	敵に当たったら、ダメージを与える
			if(	this->enemyG->getPrevHitObject()->getThisMessage() == this->judgMessage)
			{
				this->enemyG->getPrevHitObject()->reactionMessage(this->reactMessage);
			}
		//	球消滅
			this->activate	= FALSE;
			this->iFrame	= 30;
		//	煙エフェクト発生
			C3D_EFFECT_CENTER::occuEffectsInstance(
				"smoke", this->getLoc(), 2.0f, VEC3(0.0f,0.0f,0.0f), this->smokeColor);
		}

		else
	//	ステージグループ衝突判定
		if(	this->stageG	&&
			this->stageG->intersect(this, this->getLine(), this->getMoveQuantity() + 1.0f))
		{
		//	球消滅
			this->activate	= FALSE;
			this->iFrame	= 30;
		//	煙エフェクト発生
			C3D_EFFECT_CENTER::occuEffectsInstance(
				"smoke", this->getLoc(), 2.0f, VEC3(0.0f,0.0f,0.0f), this->smokeColor);
		}
	}

	//================================================================================
	//!	メソッド名	CKAGEMI_BALL::draw
	//
	//	機能		描画
	//	引数		inDev			Direct3Dデバイス
	//	更新		2008/12/11		<新規>
	//================================================================================
	BOOL
	CKAGEMI_BALL::draw(CONST DEV inDev)
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
			return	TRUE;
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
			return	TRUE;
		}
	}

	//================================================================================
	//!	メソッド名	CKAGEMI_BALL::draw
	//
	//	機能		描画(深度バッファシェーダ使用)
	//	引数		inDepthSha		深度バッファシェーダ
	//	更新		2008/12/11		<新規>
	//================================================================================
	BOOL
	CKAGEMI_BALL::draw(CDEPTH_SHA* inDepthSha)
	{
		return	this->draw(inDepthSha->getDevice());
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------