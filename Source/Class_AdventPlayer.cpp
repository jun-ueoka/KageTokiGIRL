//***********************************************************************************
//!	ファイル名		Class_AdventPlayer.h
//
//	機能			アドベンチャーゲームモードのプレイヤークラスの実装
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2008/12/11	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_AdventPlayer.h"		//!< アドベンチャーゲームプレイヤー

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::コンストラクタ
	//
	//	引数		inDev			Direct3Dデバイス
	//	更新		2008/12/11		<新規>
	//================================================================================
	CADVENT_PLAYER::CADVENT_PLAYER(CONST DEV inDev, CKAGETOKI_ADVENT_CHARACTER** inTarget,
								   SCENE_SHARE_DATA* inShare)
	{
	//	カゲオとカゲミの生成
		this->adventKageo		= new CADVENT_KAGEO(inDev);
		this->adventKagemi		= new CADVENT_KAGEMI(inDev);
	//	初期はカゲオ選択中
		this->choiceChara		= eADCC_KAGEO;
		this->bChoiceCharaChange= TRUE;
	//	属性の初期化
		this->target			= inTarget;
		this->camera			= NULL;
		this->bDamageCheck		= TRUE;
		this->iMaxLife			= 15;
		this->iLife				= this->iMaxLife;
		this->share				= inShare;
		this->keepStartLoc		= VEC3(0.0f, 0.0f, 0.0f);

	//	エフェクトの登録
		C3D_EFFECT_CENTER*	effectCenter	= C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			EFFECT_SMOKE,
			new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEO_COMMENT_ACHE,
			new C3D_EFFECT(inDev, KAGEO_COMMENT_ACHE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEO_COMMENT_ANGER,
			new C3D_EFFECT(inDev, KAGEO_COMMENT_ANGER, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEO_COMMENT_HASTE,
			new C3D_EFFECT(inDev, KAGEO_COMMENT_HASTE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEO_COMMENT_DATHE,
			new C3D_EFFECT(inDev, KAGEO_COMMENT_DATHE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEMI_COMMENT_ACHE,
			new C3D_EFFECT(inDev, KAGEMI_COMMENT_ACHE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEMI_COMMENT_ANGER,
			new C3D_EFFECT(inDev, KAGEMI_COMMENT_ANGER, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEMI_COMMENT_HASTE,
			new C3D_EFFECT(inDev, KAGEMI_COMMENT_HASTE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEMI_COMMENT_DATHE,
			new C3D_EFFECT(inDev, KAGEMI_COMMENT_DATHE, VEC3(0.0f,0.0f,0.0f)));
	//	初期キャラクターを設定
		*this->target			= this->getPlayChara();

	//	インターフェース関連の初期化
	//	カゲオのライフベース
		this->texKageoLife
			= new CTEXTURE(inDev, NAME_INTERFACE, VEC2(1.0f, 1.0f), 0xFFFFFF96);
		this->texKageoLife->setToRect(VEC2(256.0f, 256.0f));
		this->texKageoLife->setFromRect(0, 64, 256, 320);
		this->texKageoLife->setLoc(&VEC3(150.0f, 620.0f, 0.5f));
	//	カゲミのライフベース
		this->texKagemiLife = new CTEXTURE(texKageoLife);
		this->texKagemiLife->setToRect(VEC2(256.0f, 256.0f));
		this->texKagemiLife->setFromRect(256, 64, 512, 320);
		this->texKagemiLife->setLoc(&VEC3(150.0f, 620.0f, 0.5f));
	}

	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::デストラクタ
	//
	//	更新		2008/12/11		<新規>
	//================================================================================
	CADVENT_PLAYER::~CADVENT_PLAYER(void)
	{
	//	カゲオとカゲミの解放
		SAFE_DELETE(this->adventKageo);
		SAFE_DELETE(this->adventKagemi);
	//	インターフェースの解放
		SAFE_DELETE(this->texKageoLife);
		SAFE_DELETE(this->texKagemiLife);
	}

	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::getPlayChara
	//
	//	機能		選択中のキャラクターのポインターを取得
	//	戻り値		選択中のキャラクターのポインター
	//	更新		2008/12/11		<新規>
	//================================================================================
	CKAGETOKI_ADVENT_CHARACTER*
	CADVENT_PLAYER::getPlayChara(void)
	{
	//	選択中のキャラクターを返す
		switch(this->choiceChara)
		{
			case eADCC_KAGEO:	return	this->adventKageo;
			case eADCC_KAGEMI:	return	this->adventKagemi;
		}
		return	NULL;
	}

	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::setViewG
	//
	//	機能		描画グループの確保
	//	引数		inGroup			描画グループポインター
	//	更新		2008/12/11		<新規>
	//================================================================================
	void
	CADVENT_PLAYER::setViewG(C3D_OBJECT_GROUP* inGroup)
	{
		this->adventKageo->setViewG(inGroup);
		this->adventKagemi->setViewG(inGroup);
	}

	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::setStageG
	//
	//	機能		ステージグループの確保
	//	引数		inGroup			ステージグループポインター
	//	更新		2008/12/11		<新規>
	//================================================================================
	void
	CADVENT_PLAYER::setStageG(C3D_OBJECT_GROUP* inGroup)
	{
		this->adventKageo->setStageG(inGroup);
		this->adventKagemi->setStageG(inGroup);
	}

	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::setEnemyG
	//
	//	機能		敵グループの確保
	//	引数		inGroup			敵グループポインター
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CADVENT_PLAYER::setEnemyG(C3D_OBJECT_GROUP* inGroup)
	{
		this->adventKageo->setEnemyG(inGroup);
		this->adventKagemi->setEnemyG(inGroup);
	}

	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::setAllLoc
	//
	//	機能		一斉に位置設定
	//	引数		inLoc			設定する位置
	//	更新		2008/12/11		<新規>
	//================================================================================
	void
	CADVENT_PLAYER::setAllLoc(VEC3* inLoc)
	{
		this->adventKageo->setLoc(inLoc);
		this->adventKagemi->setLoc(inLoc);
	}

	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::setAllAngle
	//
	//	機能		一斉に角度設定
	//	引数		inAngle			設定する角度
	//	更新		2008/12/11		<新規>
	//================================================================================
	void
	CADVENT_PLAYER::setAllAngle(VEC3* inAngle)
	{
		this->adventKageo->setAngle(inAngle);
		this->adventKagemi->setAngle(inAngle);
	}

	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::setAllVisible
	//
	//	機能		一斉に描画確認設定
	//	引数		inVisible		新たな描画確認
	//	更新		2008/12/11		<新規>
	//================================================================================
	void
	CADVENT_PLAYER::setAllVisible(BOOL inVisible)
	{
		this->adventKageo->setVisible(inVisible);
		this->adventKagemi->setVisible(inVisible);
	}

	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::setAllLimit
	//
	//	機能		一斉に入力有効制御設定
	//	引数		inLimit			入力有効制御レベル
	//	更新		2009/01/24		<新規>
	//================================================================================
	void
	CADVENT_PLAYER::setAllLimit(int inLimit)
	{
		this->adventKageo->setLimit((ADVENT_KAGEO_ACTION_LIMIT)inLimit);
		this->adventKagemi->setLimit((ADVENT_KAGEMI_ACTION_LIMIT)inLimit);
	}

	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::resetAllWay
	//
	//	機能		一斉に向き初期化
	//	更新		2008/12/11		<新規>
	//================================================================================
	void
	CADVENT_PLAYER::resetAllWay(void)
	{
		this->adventKageo->resetWay();
		this->adventKagemi->resetWay();
	}

	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::resetPlayer
	//
	//	機能		プレイヤーのリセット
	//	引数		inStartLoc		初期位置
	//	更新		2009/02/05		<新規>
	//================================================================================
	void
	CADVENT_PLAYER::resetPlayer(VEC3* inStartLoc)
	{
	//	初期位置が設定されているなら、更新
		if(	inStartLoc)
		{
			this->keepStartLoc	= *inStartLoc;
		}
		this->setAllLoc(&this->keepStartLoc);
		this->setAllVisible(TRUE);
		this->resetAllWay();
		this->setAllAngle(&VEC3(0.0f, ADVENT_RIGHTANGLE, 0.0f));
		this->setChoiceCharacter(eADCC_KAGEO);
		this->iLife	= this->iMaxLife;
	}

	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::setChoiceCharacter
	//
	//	機能		選択中キャラクターの変更
	//	引数		inChara			新たな選択中キャラクター
	//	更新		2009/01/24		<新規>
	//================================================================================
	void
	CADVENT_PLAYER::setChoiceCharacter(ADVENT_CHOICE_CHARACTER inChara)
	{
	//	選択中のキャラクターと新たな選択中キャラクターが一致した場合、処理を返す
		if(	this->choiceChara == inChara)			return;
	//	操作対象がカゲミなら、安全に待機に戻す
		if(	this->choiceChara == eADCC_KAGEMI)
		{
			this->adventKagemi->safeResetChangeToWait();
		}

	//	位置などの同期
		this->setAllLoc(&this->getPlayChara()->getLoc());
		this->setAllAngle(&this->getPlayChara()->getAngle());
		this->resetAllWay();

	//	煙エフェクトの色確保用
		D3DCOLOR	color	= 0x00000000;
	//	キャラクターチェンジ
		switch(this->choiceChara)
		{
			case eADCC_KAGEO:
				this->choiceChara = eADCC_KAGEMI;
				color = 0xFFFF9696;
				break;
			case eADCC_KAGEMI:
				this->choiceChara = eADCC_KAGEO;
				color = 0xFF9696FF;
				break;
		}
	//	煙エフェクト発生
		C3D_EFFECT_CENTER::occuEffectsInstance(
			EFFECT_SMOKE,
			(this->getPlayChara()->getLoc()+VEC3(0.0f,0.0f,-200.0f)),
			3.0f,
			VEC3(0.0f,0.0f,0.0f),
			color);
	//	対象の設定
		*this->target	= this->getPlayChara();
		this->camera->setTarget(*this->target);
	}

	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::occuPlayerEffect
	//
	//	機能		プレイヤーエフェクトの発生
	//	引数		inEffectFlag	発生させるプレイヤーエフェクト
	//	更新		2009/02/27		<新規>
	//================================================================================
	void
	CADVENT_PLAYER::occuPlayerEffect(ADVENT_PLAYER_EFFECT inEffectFlag)
	{
	//	発生エフェクト名確保用
		LPSTR	inEffectName	= NULL;
	//	現在の選択中キャラクターにより分岐
		switch(this->choiceChara)
		{
		//	カゲオ選択中
			case eADCC_KAGEO:
			//	送られてきたプレイヤーエフェクトによって分岐	
				switch(inEffectFlag)
				{
					case eADPEFF_ACHE:
						inEffectName = KAGEO_COMMENT_ACHE;
						break;
					case eADPEFF_ANGER:
						inEffectName = KAGEO_COMMENT_ANGER;
						break;
					case eADPEFF_HASTE:
						inEffectName = KAGEO_COMMENT_HASTE;
						break;
					case eADPEFF_DATHE:
						inEffectName = KAGEO_COMMENT_DATHE;
						break;
				}
				break;
		//	カゲミ選択中
			case eADCC_KAGEMI:
			//	送られてきたプレイヤーエフェクトによって分岐	
				switch(inEffectFlag)
				{
					case eADPEFF_ACHE:
						inEffectName = KAGEMI_COMMENT_ACHE;
						break;
					case eADPEFF_ANGER:
						inEffectName = KAGEMI_COMMENT_ANGER;
						break;
					case eADPEFF_HASTE:
						inEffectName = KAGEMI_COMMENT_HASTE;
						break;
					case eADPEFF_DATHE:
						inEffectName = KAGEMI_COMMENT_DATHE;
						break;
				}
				break;
		}
	//	発生させるエフェクト名が存在したら、エフェクト発生
		if(	inEffectName)
		{
			C3D_EFFECT_CENTER::occuEffectsInstance(
				inEffectName, (*this->target)->getWorldCenter(), 1.0f);
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::operation
	//
	//	機能		プレイヤー操作
	//	引数		input			入力情報
	//	更新		2008/12/11		<新規>
	//================================================================================
	void
	CADVENT_PLAYER::operation(CINPUT* input)
	{
	//	外部ポインター群が設定されていなければ、処理をしない
		if(	!(this->target)	||
			!(this->camera))	return;

	//	ダメージ判定
	//	現在のキャラクターにダメージを受けた形跡がある時
		if(	(*this->target)->getDamageBoolen())
		{
		//	ダメージを受けた形跡を消しておく
			(*this->target)->setDamageBoolen(FALSE);
		//	ダメージ判定確認が有効の時
			if(	this->bDamageCheck)
			{
				this->iLife	-= (*this->target)->getDamageValue();
			//	ライフによって吹き出し処理分岐
				int		inValue			= this->iMaxLife / 3;
			//	ライフに余裕がある時
				if(	this->iLife <  this->iMaxLife	&&
					this->iLife >= this->iMaxLife - inValue)
				{
					this->occuPlayerEffect(eADPEFF_ACHE);
				}
			//	ライフに少し減って来た時
				else
				if(	this->iLife <  this->iMaxLife -  inValue	&&
					this->iLife >= this->iMaxLife - (inValue * 2))
				{
					this->occuPlayerEffect(eADPEFF_ANGER);
				}
			//	ライフに余裕が無くなった時
				else
				if(	this->iLife <  this->iMaxLife - (inValue * 2)	&&
					this->iLife >  0)
				{
					this->occuPlayerEffect(eADPEFF_HASTE);
				}
			//	ライフが無くなった時
				else
				{
					this->occuPlayerEffect(eADPEFF_DATHE);
				}
			}
		}
	//	ライフが0以下になったら、処理をしない
		if(	this->iLife <= 0)	{ this->iLife = 0;	goto next;	}

	//	キャラクター切り替え
		if(	input->checkPressOnce(MYKEY::eHARD)	&&
			this->bChoiceCharaChange)
		{
		//	操作対象キャラクターが空中なら、処理を返す
			if(	this->getPlayChara()->getFlighting())	goto next;
		//	操作対象がカゲミで、
		//	「エナジーボール発射」か「エナジーボール操作中」なら、処理を返す
			if(	(this->choiceChara == eADCC_KAGEMI)	&&
			   ((this->adventKagemi->getAction() == eADKAGEMIA_SKILL_ENERGY_BALL_SHOT) ||
			    (this->adventKagemi->getAction() == eADKAGEMIA_SKILL_ENERGY_BALL_OPERATION)))
			{
				goto next;
			}

		//	位置などの同期
			this->setAllLoc(&this->getPlayChara()->getLoc());
			this->setAllAngle(&this->getPlayChara()->getAngle());
			this->resetAllWay();

		//	煙エフェクトの色確保用
			D3DCOLOR	color	= 0x00000000;
		//	キャラクターチェンジ
			switch(this->choiceChara)
			{
				case eADCC_KAGEO:
					this->choiceChara = eADCC_KAGEMI;
					color = 0xFFFF9696;
					break;
				case eADCC_KAGEMI:
					this->choiceChara = eADCC_KAGEO;
					color = 0xFF9696FF;
					break;
			}
		//	煙エフェクト発生
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE,
				(this->getPlayChara()->getLoc()+VEC3(0.0f,0.0f,-200.0f)),
				3.0f,
				VEC3(0.0f,0.0f,0.0f),
				color);
		//	対象の設定
			*this->target	= this->getPlayChara();
			this->camera->setTarget(*this->target);
		}

	next:

	//	カゲミ専用の挙動
		if(	this->choiceChara == eADCC_KAGEMI)
		{
		//	カメラ対象の変更
		//	エナジーボール操作中の時は、エナジーボールが操作対象
			if(	this->adventKagemi->getOpeEnergyBall())
			{
				this->camera->setTarget(this->adventKagemi->getOpeEnergyBall());
			}
		//	エナジーボール操作中でない時は、カゲミが操作対象
			else
			{
				this->camera->setTarget(this->adventKagemi);
			}
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_PLAYER::drawInterface
	//
	//	機能		インターフェースの描画
	//	引数		inSprite		スプライトデバイス
	//	更新		2008/12/11		<新規>
	//================================================================================
	void
	CADVENT_PLAYER::drawInterface(SPRITE inSprite)
	{
	//	選択中のキャラクター分岐
		switch(this->choiceChara)
		{
			case eADCC_KAGEO:	this->texKageoLife->draw(inSprite);		break;
			case eADCC_KAGEMI:	this->texKagemiLife->draw(inSprite);	break;
		}

	//	ライフベースの表示
		RECT	rcFrom	= {  0,320,224,352};
		RECT	rcTo	= { 35,679,259,711};
		this->texKageoLife->drawTexture(inSprite, rcFrom, rcTo);
	//	ライフの表示
		SetRect(&rcFrom,  14,357,209,378);
		SetRect(&rcTo,    50,689,243,701);
		rcTo.right	=
			rcTo.left + (int)(((float)(rcTo.right - rcTo.left) / this->iMaxLife) * (float)this->iLife);
		this->texKageoLife->drawTexture(inSprite, rcFrom, rcTo);
	//	ライフ枠の表示
		SetRect(&rcFrom,   0,384,224,416);
		SetRect(&rcTo,    35,679,259,711);
		this->texKageoLife->drawTexture(inSprite, rcFrom, rcTo);

	//	残気の描画
		SetRect(&rcFrom, 192,  0,256, 64);
		for(DWORD i = 0; i < this->share->dwPlayerLife; i++)
		{
			SetRect(&rcTo, 30+(30*i), 640, 30+50+(30*i), 640+50);
			this->texKagemiLife->drawTexture(inSprite, rcFrom, rcTo);
		}
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------