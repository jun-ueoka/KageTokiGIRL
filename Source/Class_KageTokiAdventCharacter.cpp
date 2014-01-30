//***********************************************************************************
//!	ファイル名		Class_KageTokiAdventCharacter.cpp
//
//	機能			アドベンチャーゲームモードのキャラクタークラス
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2008/12/09	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_KageTokiAdventCharacter.h"		//!< アドベンチャーゲームモードキャラクター

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_CHARACTER::コンストラクタ
	//
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		スキンメッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2008/12/09		<新規>
	//================================================================================
	CKAGETOKI_ADVENT_CHARACTER::CKAGETOKI_ADVENT_CHARACTER(
		CONST DEV	inDev,
		CONST LPSTR	inName,
		CONST LPSTR	inResource,
		CONST LPSTR	inTexture)	:
	CSKIN_MESH(inDev, inName, inResource, inTexture)
	{
	//	属性の初期化
		this->bFlighting			= true;					//!< 空中確認
		this->bHeightValid			= false;				//!< 高さ有効確認
		this->bWidthValid			= false;				//!< 横幅有効確認

		this->fUpDownSpeed			=   0.0f;				//!< 上下移動速度

		this->fMaxUpSpeed			=  10.0f;				//!< 最大上昇速度
		this->fMaxDownSpeed			= -50.0f;				//!< 最大下降速度
		this->fUpDownAccelerate		=   2.0f;				//!< 上下移動加速値
		this->fUpDownSlowDown		=   0.5f;				//!< 上下移動減速値

		this->fFloorSpace			=   5.0f;				//!< 床との間隔
		this->fWallSpace			=   5.0f;				//!< 壁との間隔
		this->fHeightValue			=   0.0f;				//!< 高さ
		this->fWidthValue			=   0.0f;				//!< 横幅

		this->UDLine				= LINE();				//!< 操作対象の上下判定方向
		this->LRLine				= LINE();				//!< 操作対象の左右判定方向
		this->FRLine				= LINE();				//!< 操作対象の前後判定方向

		this->way					= eADCW_NO;				//!< キャラクターの向き
		this->iFrameCnt				= 0;					//!< フレームカウンタ

		this->fJumpHeight			= ADVENT_JUMP_HEIGHT;	//!< ジャンプの高さ
		this->fLeftRightMoveSpeed	= 0.0f;					//!< 左右移動スピード
		this->fLeftRightMoveMaxSpeed= 20.0f;				//!< 左右移動最大スピード
		this->fLeftRightMoveAccele	= 2.0f;					//!< 左右移動加速値
		this->bDamage				= FALSE;				//!< ダメージ確認
		this->iDamage				= 0;					//!< ダメージ値

		this->viewG					= NULL;					//!< 描画グループ
		this->stageG				= NULL;					//!< 衝突判定グループ
		this->enemyG				= NULL;					//!< 敵グループ

	//	その他の設定
		this->setRpmY(20.0f);								//!< 回転速度
		this->setAngleY(ADVENT_RIGHTANGLE);					//!< 初期は右向き
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_CHARACTER::デストラクタ
	//
	//	更新		2008/12/09		<新規>
	//================================================================================
	CKAGETOKI_ADVENT_CHARACTER::~CKAGETOKI_ADVENT_CHARACTER(void)
	{
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_CHARACTER::moveJumping
	//
	//	機能		飛躍処理
	//	引数		inValue			飛躍時の加速値
	//	更新		2008/12/09		<新規>
	//================================================================================
	void
	CKAGETOKI_ADVENT_CHARACTER::moveJumping(float inValue)
	{
	//	飛んでる時は、飛躍できない
		if(	!this->bFlighting)
		{
		//	飛躍時の加速値を設定
			this->fUpDownSpeed	= inValue;
		//	空中にいる
			this->bFlighting	= true;
		}
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_CHARACTER::opeAdvent
	//
	//	機能		アドベンチャーゲーム操作
	//	引数		input			入力情報
	//	更新		2008/12/09		<新規>
	//================================================================================
	void
	CKAGETOKI_ADVENT_CHARACTER::opeAdvent(CINPUT* input)
	{
	//	向きの制御
		switch(this->way)
		{
		//	左を向く途中
			case eADCW_LEFT_TURN:
			//	左に向いた後、左に向ききるなら、値を修正
				if(	(this->getAngleY() - this->getRpmY()) < ADVENT_LEFTANGLE)
				{
					this->setAngleY(ADVENT_LEFTANGLE);
					this->way = eADCW_LEFT;
				}
			//	左に向ききらないなら、左を向く
				else
				{
					this->rotateLeft();
				}
				break;

		//	右を向く途中
			case eADCW_RIGHT_TURN:
			//	右に向いた後、右に向ききるなら、値を修正
				if(	(this->getAngleY() + this->getRpmY()) > ADVENT_RIGHTANGLE)
				{
					this->setAngleY(ADVENT_RIGHTANGLE);
					this->way = eADCW_RIGHT;
				}
			//	右に向ききらないなら、右を向く
				else
				{
					this->rotateRight();
				}
				break;

		//	初期状態
			case eADCW_NO:
			//	左寄りの向きになっているなら、左向きに設定する
				if(	(this->getAngleY() >=   0.0f) &&
					(this->getAngleY() <  180.0f))
				{
					this->way = eADCW_LEFT_TURN;
				}
			//	上記以外は、右向きに設定する
				else
				{
					this->way = eADCW_RIGHT_TURN;
				}
				break;

		//	上記以外は、処理をしない
			default:	
				break;
		}

	//	移動の制御
		switch(this->way)
		{
		//	左を向いている
			case eADCW_LEFT:
			//	左が押されている時
				if(	input->checkPress(ADVENT_LEFT))
				{
				//	加速
					if((this->fLeftRightMoveSpeed +  this->fLeftRightMoveAccele) <
						this->fLeftRightMoveMaxSpeed)
					{
						this->fLeftRightMoveSpeed += this->fLeftRightMoveAccele;
						this->calcAnimSpeed(0.002f);
					}
				//	衝突判定(前に進む再は処理空間の中では、右に移動する)
					if(	!this->stageG->intersect(
							this,
							ADVENT_FRONTHIT,
							(this->fLeftRightMoveSpeed + this->fWallSpace)))
					{
					//	空中にいたら予想前方斜め下を調べる
						if(this->bFlighting)
						{
							if(	!this->stageG->intersect(
									this,
									&LINE(eUD_DOWN, eLR_NO, ADVENT_FRONTHIT),
									(this->fLeftRightMoveSpeed + this->fWallSpace)))
							{
							//	衝突しなかったら、進ませる
								this->setMoveQuantity(this->fLeftRightMoveSpeed);
								ADVENT_FRONTMOVE;
							}
						}
						else
						{
						//	衝突しなかったら、進ませる
							this->setMoveQuantity(this->fLeftRightMoveSpeed);
							ADVENT_FRONTMOVE;
						}
					}
				//	衝突していて、空中に居たら位置を調節
					else if(this->bFlighting)
					{
					//	位置を調節する
						this->setMoveQuantity(
							((this->fLeftRightMoveSpeed + this->fWallSpace) - this->getDist()) - 1.0f);
						if(	!this->stageG->intersect(this, ADVENT_REARHIT))
						{
							ADVENT_REALMOVE;
						}
					}
				//	衝突していて、空中にいなく、少し移動出来る余地が有る時
					else if(this->getDist() > 10.0f)
					{
					//	衝突しなかったら、進ませる
						this->setMoveQuantity(5.0f);
						ADVENT_FRONTMOVE;
					}
				}
			//	右が押されている時
				else if(input->checkPress(ADVENT_RIGHT))
				{
				//	右を向く
					this->fLeftRightMoveSpeed	= 0.0f;
					this->setAnimSpeed();
					this->way			= eADCW_RIGHT_TURN;
				}
			//	どちらも押されてない時
				else
				{
				//	停止
					this->fLeftRightMoveSpeed	= 0.0f;
					this->setAnimSpeed();
					this->opeLocShift();
				}
				break;

		//	右を向いている
			case eADCW_RIGHT:
			//	右が押されている時
				if(	input->checkPress(ADVENT_RIGHT))
				{
				//	加速
					if((this->fLeftRightMoveSpeed +  this->fLeftRightMoveAccele) <
						this->fLeftRightMoveMaxSpeed)
					{
						this->fLeftRightMoveSpeed += this->fLeftRightMoveAccele;
						this->calcAnimSpeed(0.002f);
					}
				//	衝突判定(前に進む再は処理空間の中では、左に移動する)
					if(	!this->stageG->intersect(
							this,
							ADVENT_FRONTHIT,
							(this->fLeftRightMoveSpeed + this->fWallSpace)))
					{
					//	空中にいたら予想前方斜め下を調べる
						if(this->bFlighting)
						{
							if(	!this->stageG->intersect(
									this,
									&LINE(eUD_DOWN, eLR_NO, ADVENT_FRONTHIT),
									(this->fLeftRightMoveSpeed + this->fWallSpace)))
							{
							//	衝突しなかったら、進ませる
								this->setMoveQuantity(this->fLeftRightMoveSpeed);
								ADVENT_FRONTMOVE;
							}
						}
						else
						{
						//	衝突しなかったら、進ませる
							this->setMoveQuantity(this->fLeftRightMoveSpeed);
							ADVENT_FRONTMOVE;
						}
					}
				//	衝突していて、空中にいたら位置を調節
					else if(this->bFlighting)
					{
					//	位置を調節する
						this->setMoveQuantity(
							((this->fLeftRightMoveSpeed + this->fWallSpace) - this->getDist()) - 1.0f);
						if(	!this->stageG->intersect(this, ADVENT_REARHIT))
						{
							ADVENT_REALMOVE;
						}
					}
				//	衝突していて、空中にいなく、少し移動出来る余地が有る時
					else if(this->getDist() > 10.0f)
					{
					//	衝突しなかったら、進ませる
						this->setMoveQuantity(5.0f);
						ADVENT_FRONTMOVE;
					}
				}
			//	左が押されている時
				else if(input->checkPress(ADVENT_LEFT))
				{
				//	左を向く
					this->fLeftRightMoveSpeed	= 0.0f;
					this->setAnimSpeed();
					this->way			= eADCW_LEFT_TURN;
				}
			//	どちらも押されてない時
				else
				{
				//	停止
					this->fLeftRightMoveSpeed	= 0.0f;
					this->setAnimSpeed();
					this->opeLocShift();
				}
				break;

		//	上記以外は処理をしない
			default:	
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_CHARACTER::opeEffectGravity
	//
	//	機能		アドベンチャーゲーム操作
	//	引数		input			入力情報
	//	更新		2008/12/09		<新規>
	//================================================================================
	void
	CKAGETOKI_ADVENT_CHARACTER::opeEffectGravity(void)
	{
	//	飛んでいるなら、重力により落下する
		if(	this->bFlighting)
		{
			if((this->fUpDownSpeed	-  this->fUpDownAccelerate) > this->fMaxDownSpeed)
			{
				this->fUpDownSpeed	-= this->fUpDownAccelerate;
			}
		}

		float	inMoveValue			= 0.0f;								//!< 結果的な移動量
		float	inIntersectValue	= this->getIntegralDownSpeed();		//!< 衝突判定する距離(整数)

	//	上下移動速度で、判定方法を決定
		if(	this->fUpDownSpeed > 0.0f)									//!< 上昇中
		{
			this->UDLine.ud		= eUD_UP;
		//	衝突判定距離にキャラクターの大きさをプラス
			inIntersectValue   += this->getWorldRadius();
		}
		else															//!< 下降中、及び停止中
		{
			this->UDLine.ud		= eUD_DOWN;
		}

		BOOL	bHit1	= this->stageG->intersect(this, &this->UDLine, (inIntersectValue + this->fFloorSpace));
		float	fDist	= this->getDist();

		if(	bHit1)
		{
		//	レイの方向が下
			if(	this->UDLine.ud == eUD_DOWN)
			{
			//	空中にいる場合
				if(	this->bFlighting)
				{
				//	空中にいない設定
					this->bFlighting		= false;
				//	結果的な移動量を計算
					inMoveValue				= (fDist - this->fFloorSpace);
				//	上下移動値の停止
					this->fUpDownSpeed		= -(this->fFloorSpace * 2.0f);
				}
			//	空中にいなかった場合
				else
				{
				//	床との距離に変化があった時
					if(	this->fFloorSpace != fDist)
					{
					//	結果的な移動量を計算
						inMoveValue	= (this->fFloorSpace - fDist);
					//	床との距離が、広がった時
						if		(this->fFloorSpace < fDist)
						{
						//	下に移動する
							this->UDLine.ud = eUD_DOWN;
						}
					//	床との距離が、狭まった時
						else if	(this->fFloorSpace > fDist)
						{
						//	上に移動する
							this->UDLine.ud = eUD_UP;
						}
					}
				}
			}
		//	レイの方向が上
			else if(this->UDLine.ud == eUD_UP)
			{
			//	上下移動値の減少
				this->fUpDownSpeed	-= 2.0f;
			}
		}
		else
		{
		//	空中にいる
			this->bFlighting	 = true;
		//	上下移動値を計算
			inMoveValue			 = this->fUpDownSpeed;
		}

	//	結果的な移動量に変化があるなら、移動させる
		if(	inMoveValue	!= 0.0f)
		{
		//	上下移動
			this->moveUpDown(this->UDLine.ud, inMoveValue);
		}
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_CHARACTER::opeLocShift
	//
	//	機能		位置合わせ操作
	//	更新		2008/12/10		<新規>
	//================================================================================
	void
	CKAGETOKI_ADVENT_CHARACTER::opeLocShift(void)
	{
		BOOL		bFront		= FALSE,	bRear		= FALSE;
		float		fFrontDist	= 0.0f,		fRearDist	= 0.0f;
		C3D_OBJECT	*frontObj	= NULL,		*rearObj	= NULL;

	//	空中にいる時
		if(	this->bFlighting)
		{
		//	前方衝突判定結果を取得
			bFront		= this->stageG->intersect(this, ADVENT_FRONTHIT, this->fWallSpace);
			fFrontDist	= this->getDist();
			frontObj	= this->stageG->getPrevHitObject();
		//	後方衝突判定結果を取得
			bRear		= this->stageG->intersect(this, ADVENT_REARHIT,  this->fWallSpace);
			fRearDist	= this->getDist();
			rearObj		= this->stageG->getPrevHitObject();
		//	前方に衝突確認
			if(	bFront)
			{
			//	前方だけに衝突確認
				if(	!bRear)
				{
				//	位置を調節する
					this->setMoveQuantity((this->fWallSpace - fFrontDist) - 1.0f);
					ADVENT_REALMOVE;
				}
			}
		//	後方だけに衝突確認
			else if(bRear)
			{
			//	位置を調節する
				this->setMoveQuantity((this->fWallSpace - fRearDist) - 1.0f);
				ADVENT_FRONTMOVE;
			}
		}
	//	地上に居る時
		else
		{
		//	前方衝突判定結果を取得
			bFront		= this->stageG->intersect(this, ADVENT_FRONTHIT, this->fWallSpace * 3.0f);
			fFrontDist	= this->getDist();
			frontObj	= this->stageG->getPrevHitObject();
		//	後方衝突判定結果を取得
			bRear		= this->stageG->intersect(this, ADVENT_REARHIT,  this->fWallSpace * 3.0f);
			fRearDist	= this->getDist();
			rearObj		= this->stageG->getPrevHitObject();
		//	前方に衝突確認
			if(	bFront)
			{
			//	前方と後方に衝突確認
				if(	bRear)
				{
				//	前方と後方に衝突したオブジェクトが一致時
					if(	frontObj == rearObj)
					{
					//	上方向に衝突判定をし、また同じオブジェクトに衝突したら、
					//	身動き取れない状況と判断し、位置をづらす
						if(	frontObj->intersect(this, eUD_UP, this->getWorldRadius() * 2.0f))
						{
							this->setMoveQuantity(this->getDist() + 10.0f);
							this->moveUp();
						}
					}
				//	不一致時
					else
					{
					//	前方と後方に衝突したオブジェクトが遠くになく
						if(	((fFrontDist < (this->fWallSpace * 2.0f))	&&
							 (fRearDist  < (this->fWallSpace * 2.0f)))	&&
					//	左右どちらも[消せる影]でない時
							(frontObj->getThisMessage() != eTMSG_SEE_SHADOW)	&&
							(rearObj->getThisMessage()  != eTMSG_SEE_SHADOW))
						{
						//	上方向に衝突判定をし、それも消せる影でないならオブジェクトに衝突したら、
						//	身動き取れない状況と判断し、位置をづらす
							if(	this->stageG->intersect(this, eUD_UP, this->getWorldRadius() * 2.0f))
							{
								this->setMoveQuantity(this->getDist()*2.0f + 10.0f);
								this->moveUp();
							}
						}
					}
				}
			//	前方だけに衝突確認
				else
				{
				}
			}
		//	後方だけに衝突確認
			else if(bRear)
			{
			}
		}
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_CHARACTER::operation
	//
	//	機能		上下左右移動の基本操作
	//	引数		*input			入力管理
	//	更新		2008/12/09		<新規>
	//================================================================================
	void
	CKAGETOKI_ADVENT_CHARACTER::operation(CINPUT* input)
	{
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------