//***********************************************************************************
//!	ファイル名		Class_KageTokiAdventCharacter.h
//
//	機能			アドベンチャーゲームモードのキャラクタークラス
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2008/12/09	<新規>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_DirectInput.h"			//!< 入力情報
#include	"Class_SkinMesh.h"				//!< 3Dオブジェクトスキンメッシュ
#include	"Class_3DObjectGroup.h"			//!< 3Dオブジェクトグループ

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	専用マクロ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		ADVENT_LEFT				MYKEY::eLEFT	//!< 左移動
	#define		ADVENT_RIGHT			MYKEY::eRIGHT	//!< 右移動
	#define		ADVENT_TOP				MYKEY::eUP		//!< 魔法を上へ
	#define		ADVENT_BOTTOM			MYKEY::eDOWN	//!< 魔法を下へ
	#define		ADVENT_JUMP				MYKEY::eMIDDLE	//!< ジャンプ
	#define		ADVENT_JUMP_HEIGHT		55.0f			//!< ジャンプの高さ

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CKAGETOKI_ADVENT_CHARACTER
	//
	//	機能			アドベンチャーゲームモードのキャラクタークラス
	//	更新			2008/12/09	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CKAGETOKI_ADVENT_CHARACTER : public CSKIN_MESH
	{
			//==================================================================
			//	キャラクターの向きの挙動制御
			//==================================================================
				#define	ADVENT_LEFTANGLE	 90.0f				//!< 左向きの角度
				#define	ADVENT_RIGHTANGLE	270.0f				//!< 右向きの角度
				#define	ADVENT_FRONTHIT		eFR_FRONT			//!< 前方への衝突判定引数
				#define	ADVENT_REARHIT		eFR_REAR			//!< 後方への衝突判定引数
				#define	ADVENT_FRONTMOVE	this->moveFront()	//!< 前方移動時の命令
				#define	ADVENT_REALMOVE		this->moveRear()	//!< 前方移動時の命令

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	キャラクターの向きの列挙
			//==================================================================
				enum	ADVENT_CHARA_WAY
				{
					eADCW_LEFT,								//!< 左向き
					eADCW_LEFT_TURN,						//!< 左に向く途中
					eADCW_RIGHT,							//!< 右向き
					eADCW_RIGHT_TURN,						//!< 右に向く途中
					eADCW_NO,								//!< 初期状態
				};

			//==================================================================
			//	移動情報
			//==================================================================
				float	fUpDownSpeed;						//!< 上下移動速度
				float	fMaxUpSpeed;						//!< 最大上昇速度
				float	fMaxDownSpeed;						//!< 最大下降速度
				float	fUpDownAccelerate;					//!< 上下移動加速値
				float	fUpDownSlowDown;					//!< 上下移動減速値

				LINE	UDLine;								//!< 操作対象の上下判定方向
				LINE	LRLine;								//!< 操作対象の左右判定方向
				LINE	FRLine;								//!< 操作対象の前後判定方向

				float	fFloorSpace;						//!< 床との間隔
				float	fWallSpace;							//!< 壁との間隔
				bool	bFlighting;							//!< 空中確認
				bool	bHeightValid;						//!< 高さ有効確認
				bool	bWidthValid;						//!< 横幅有効確認

				float	fHeightValue;						//!< 高さ
				float	fWidthValue;						//!< 横幅

				ADVENT_CHARA_WAY	way;					//!< キャラクターの向き
				int		iFrameCnt;							//!< フレームカウンタ(多様)

				float	fJumpHeight;						//!< ジャンプ速度
				float	fLeftRightMoveSpeed;				//!< 左右移動スピード
				float	fLeftRightMoveMaxSpeed;				//!< 左右移動最大スピード
				float	fLeftRightMoveAccele;				//!< 左右移動加速値

				BOOL	bDamage;							//!< ダメージ確認
				int		iDamage;							//!< ダメージ値

			//==================================================================
			//	判定グループ(これらは全て外部の物)
			//==================================================================
				C3D_OBJECT_GROUP*	viewG;					//!< 描画オブジェクトグループポインター
				C3D_OBJECT_GROUP*	stageG;					//!< ステージオブジェクトグループポインター
				C3D_OBJECT_GROUP*	enemyG;					//!< 敵オブジェクトグループポインター

	public:			//-------------------- public --------------------//

				CKAGETOKI_ADVENT_CHARACTER(
					CONST DEV	inDev,
					CONST LPSTR	inName,
					CONST LPSTR	inResource = PATH_SKINMESH,
					CONST LPSTR	inTexture  = PATH_SKINMESHTEX);	//!< コンストラクタ
		virtual	~CKAGETOKI_ADVENT_CHARACTER(void);				//!< デストラクタ
		
			//==================================================================
			//	飛躍処理
			//==================================================================
				void	moveJumping(float inValue);

			//==================================================================
			//	access	高さ有効確認
			//==================================================================
				void	setHeightValid(bool inBoolen)			{ bHeightValid	= inBoolen;		}

			//==================================================================
			//	access	横幅有効確認
			//==================================================================
				void	setWidthValid(bool inBoolen)			{ bWidthValid	= inBoolen;		}

			//==================================================================
			//	access	移動速度
			//==================================================================
				float	getUpDownSpeed(void)const				{ return	fUpDownSpeed;		}
				float	getIntegralDownSpeed(void)				//!< 整数化して渡す版
				{
					return	(fUpDownSpeed > 0.0f ? fUpDownSpeed : -fUpDownSpeed);
				}

			//==================================================================
			//	access	上下移動情報
			//==================================================================
				float	getMaxUpSpeed(void)const				{ return	fMaxUpSpeed;		}
				float	getMaxDownSpeed(void)const				{ return	fMaxDownSpeed;		}
				float	getUpAccelerate(void)const				{ return	fUpDownAccelerate;	}
				float	getUpSlowDown(void)const				{ return	fUpDownSlowDown;	}
				void	setMaxUpSpeed(float inValue)			{ fMaxUpSpeed		= inValue;	}
				void	setMaxDownSpeed(float inValue)			{ fMaxDownSpeed		= inValue;	}
				void	setUpAccelerate(float inValue)			{ fUpDownAccelerate	= inValue;	}
				void	setUpSlowDown(float inValue)			{ fUpDownSlowDown	= inValue;	}

			//==================================================================
			//	access	床関係
			//==================================================================
				float	getFloorSpace(void)const				{ return	fFloorSpace;		}
				void	setFloorSpace(float inValue)			{ fFloorSpace	= inValue;		}

			//==================================================================
			//	access	壁関係
			//==================================================================
				float	getWallSpace(void)const					{ return	fWallSpace;			}
				void	setWallSpace(float inValue)				{ fWallSpace	= inValue;		}

			//==================================================================
			//	access	飛行確認
			//==================================================================
				bool	getFlighting(void)const					{ return	bFlighting;			}

			//==================================================================
			//	access	高さ情報
			//==================================================================
				float	getHeightValue(void)const				{ return	fHeightValue;		}
				void	setHeightValue(float inValue)			{ fHeightValue	= inValue;		}

			//==================================================================
			//	access	横幅情報
			//==================================================================
				float	getWidthValue(void)const				{ return	fWidthValue;		}
				void	setWidthValue(float inValue)			{ fWidthValue	= inValue;		}

			//==================================================================
			//	access	ダメージ確認
			//==================================================================
				BOOL	getDamageBoolen(void)const				{ return	bDamage;			}
				void	setDamageBoolen(BOOL inBoolen)			{ bDamage	= inBoolen;			}
				int		getDamageValue(void)const				{ return	iDamage;			}

			//==================================================================
			//	access	グループ
			//==================================================================
				void	setViewG(C3D_OBJECT_GROUP* inG)			{ viewG		= inG;				}
				void	setStageG(C3D_OBJECT_GROUP* inG)		{ stageG	= inG;				}
				void	setEnemyG(C3D_OBJECT_GROUP* inG)		{ enemyG	= inG;				}

			//==================================================================
			//	操作
			//==================================================================
				void	opeAdvent(CINPUT* input);				//!< アドベンチャー操作
				void	opeEffectGravity(void);					//!< 重力操作
				void	opeLocShift(void);						//!< 位置合わせ操作

			//==================================================================
			//	向きの初期化	
			//==================================================================
				void	resetWay(void)							{ way	= eADCW_NO;				}

			//==================================================================
			//	基本操作(サブクラスでオーバーライド)
			//==================================================================
		virtual	void	operation(CINPUT* input);
	};
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------