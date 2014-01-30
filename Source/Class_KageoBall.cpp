//***********************************************************************************
//!	ファイル名		Class_KageoBall.cpp
//
//	機能			カゲオが放つ、玉を扱うクラスの実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2007/12/08	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_KageoBall.h"
//#include	"Class_3DEffectCenter.h"	//!< エフェクトセンター(エフェクトを発生させる為)

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CKAGEO_BALL::コンストラクタ
	//
	//	引数		inDev			Direct3Dデバイス
	//				inKind			ボールの種類
	//	更新		2008/08/26		<新規>
	//================================================================================
	CKAGEO_BALL::CKAGEO_BALL(CONST DEV		inDev,
							KAGEO_BALL_KIND	inKind)	:
	CMESH(inDev, getBallTextureName(inKind))
	{
	//	属性の初期化
		this->activate		= FALSE;
		this->hitG			= NULL;
		this->iFrame		= 0;
		this->sStaticHit	= NULL;

	//	ボールの種類により、初期化処理分岐
		switch(inKind)
		{
		//	シャドウボール
			case	eKOBALL_SHADOW:
			//	見えない影を、見える影にするメッセージ
				this->judgMessage	= eTMSG_NO_SEE_SHADOW;
				this->reactMessage	= eREAMSG_SHADOW_HIT;
				this->setThisMessage(eTMSG_SHADOW_BALL);
			//	エフェクト関連
				this->iEffectCnt	= 3;
				strcpy_s(this->effectName, sizeof(effectName), EFFECT_SHADOW);
			//	煙の色
				this->smokeColor	= D3DCOLOR_RGBA(10, 10, 10, 255);
			//	音を生成
				this->sStaticHit	= new SSTATIC(SOUND_SHADOWB_HIT);
				break;

		//	シャインボール
			case	eKOBALL_SHINE:
			//	見える影を、見えない影にするメッセージ
				this->judgMessage	= eTMSG_SEE_SHADOW;
				this->reactMessage	= eREAMSG_SHINE_HIT;
				this->setThisMessage(eTMSG_SHINE_BALL);
			//	エフェクト関連
				this->iEffectCnt	= 3;
				strcpy_s(this->effectName, sizeof(effectName), EFFECT_SHINE);
			//	音を生成
				this->sStaticHit	= new SSTATIC(SOUND_SHINEB_HIT);
			//	煙の色
				this->smokeColor	= D3DCOLOR_RGBA(255, 255, 200, 255);
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
	//!	メソッド名	CKAGEO_BALL::コンストラクタ
	//
	//	機能		コピー
	//	引数		inCopy			コピー元
	//	更新		2008/08/26		<新規>
	//================================================================================
	CKAGEO_BALL::CKAGEO_BALL(CKAGEO_BALL *inCopy)
	:	CMESH(inCopy)
	{
	//	属性の初期化
		this->activate		= FALSE;
		this->judgMessage	= inCopy->getJudgMessage();
		this->reactMessage	= inCopy->getReactMessage();
		this->setThisMessage(inCopy->getThisMessage());
		this->hitG			= NULL;
		strcpy_s(this->effectName, sizeof(this->effectName), inCopy->getEffectName());
		this->iEffectCnt	= inCopy->getEffectCnt();
		this->iFrame		= 0;
		this->smokeColor	= inCopy->getSmokeColor();

	//	音の生成
		LPSTR	inHitSoundName	= inCopy->getHitSoundName();
		if(	inHitSoundName)	{	this->sStaticHit = new SSTATIC(inHitSoundName);	}
		else				{	this->sStaticHit = NULL;						}
	}

	//================================================================================
	//!	メソッド名	CKAGEO_BALL::デストラクタ
	//
	//	更新		2008/08/26		<新規>
	//================================================================================
	CKAGEO_BALL::~CKAGEO_BALL(void)
	{
	}

	//================================================================================
	//!	メソッド名	CKAGEO_BALL::getBallTextureName
	//
	//	機能		カゲオボールの種類にで、テクスチャ名の検索
	//	引数		inPtn		コメントパターン
	//	戻り値		テクスチャ名
	//	更新		2008/09/23	<新規>
	//================================================================================
	LPSTR
	CKAGEO_BALL::getBallTextureName(KAGEO_BALL_KIND inKind)
	{
	//	パターン分岐
		switch(inKind)
		{
			case	eKOBALL_SHADOW:	return	SHADOW_BALL;		break;
			case	eKOBALL_SHINE:	return	SHINE_BALL;			break;
			default:				return	NULL;				break;
		}
	}

	//================================================================================
	//!	メソッド名	CKAGEO_BALL::shot
	//
	//	機能		発射(生成した後に使用)
	//	引数		*inAngle		発射方向
	//				*inRotX			X軸の回転マトリックス
	//				*inRotY			Y軸の回転マトリックス
	//				*inRotZ			Z軸の回転マトリックス
	//				*inLoc			発射地点
	//				*inHitG			衝突判定グループ
	//				inFrame			描画フレーム
	//	更新		2008/08/26		<新規>
	//================================================================================
	void
	CKAGEO_BALL::shot(VEC3* inAngle, VEC3* inLoc, C3D_OBJECT_GROUP* inHitG, int inFrame)
	{
	//	発射可能の時
		if(	!this->activate)
		{
		//	発射
			this->hitG		= inHitG;
			this->activate	= TRUE;
			this->iFrame	= inFrame;
			this->setVisible(TRUE);
			this->setAngle(inAngle);
			this->setLoc(inLoc);
		}
	}

	//================================================================================
	//!	メソッド名	CKAGEO_BALL::intersectG
	//
	//	機能		衝突判定
	//	更新		2008/08/26		<新規>
	//================================================================================
	void
	CKAGEO_BALL::intersectG(void)
	{
	//	衝突判定グループが設定されていないなら、処理をしない
		if(	!this->hitG)	return;

	//	衝突判定
		if(	this->hitG->intersect(this, this->getLine()))
		{
		//	見えない影オブジェクトなら、シャドウボールと衝突した意味のメッセージを与える
			if(	this->hitG->getPrevHitObject()->getThisMessage() == this->judgMessage)
			{
				this->hitG->getPrevHitObject()->reactionMessage(this->reactMessage);
			}
		//	球消滅
			this->setVisible(FALSE);
			this->activate	= FALSE;
		//	衝突音発生
			this->soundHit();
		//	煙エフェクト発生
			C3D_EFFECT_CENTER::occuEffectsInstance(
				"smoke", this->getLoc(), 2.0f, VEC3(0.0f,0.0f,0.0f), this->smokeColor);
		//	球に消滅メッセージ与える
			this->setOrderMessage(eOMSG_DELETE);
		}
	}

	//================================================================================
	//!	メソッド名	CKAGEO_BALL::draw
	//
	//	機能		描画
	//	引数		inDev			Direct3Dデバイス
	//	更新		2008/08/26		<新規>
	//================================================================================
	BOOL
	CKAGEO_BALL::draw(CONST DEV inDev)
	{
	//	発射されているなら、動作を行い、衝突判定をする
		if(	this->activate)
		{
		//	前進
			this->move();

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
		}

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CKAGEO_BALL::draw
	//
	//	機能		描画(深度バッファシェーダ使用)
	//	引数		inDepthSha		深度バッファシェーダ
	//	更新		2008/12/11		<新規>
	//================================================================================
	BOOL
	CKAGEO_BALL::draw(CDEPTH_SHA* inDepthSha)
	{
		return	this->draw(inDepthSha->getDevice());
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------