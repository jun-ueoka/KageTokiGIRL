//***********************************************************************************
//!	ファイル名		Class_YamakageWeapon.h
//
//	機能			ヤマカゲが操る兵器クラスの定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2009/02/17	<新規>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_SkinMesh.h"				//!< 3Dオブジェクトスキンメッシュ
#include	"Class_3DObjectGroup.h"			//!< 3Dオブジェクトグループ
#include	"Class_3DEffectCenter.h"		//!< エフェクト中枢
#include	"Class_EnemyBall.h"				//!< エネミーボール

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	ヤマカゲ兵器の動作制御
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	YAMAKAGE_WEAPON_ACTION
	{
		eOPEYW_FLOATING,					//!< 浮遊
		eOPEYW_ATTACK,						//!< 攻撃
		eOPEYW_MAX,							//!< 動作制御の最大数
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CYAMAKAGE_WEAPON
	//
	//	機能			ヤマカゲが操る兵器クラス
	//	機能概要		対象の上空を飛び、攻撃命令が出たら玉を放つ
	//	更新			2009/02/17	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CYAMAKAGE_WEAPON : public CSKIN_MESH
	{
				#define		NAME_YAMAKAGE_WEAPON	"boss_yamakage_weapon"

	private:		//-------------------- private --------------------//

			//==================================================================
			//	外部ポインター
			//==================================================================
				CKAGETOKI_ADVENT_CHARACTER**	player;		//!< プレイヤー

				C3D_OBJECT_GROUP*		viewG;				//!< 描画グループ
				C3D_OBJECT_GROUP*		stageG;				//!< ステージグループ
				YAMAKAGE_WEAPON_ACTION	weaponAction;		//!< 動作制御
				int						iSmokeFrame;		//!< 描画フレーム
				MY_COLOR_RGBA			smokeColor;			//!< 煙の色

				int						iMoveFrame;			//!< 移動フレーム
				VEC3					targetLoc;			//!< 目標座標

				CENEMY_BALL*			originEnemyBall;	//!< エネミーボールのオリジナル
				SSTATIC*				ssEnemyBallShot;	//!< エネミーボール発射音

	public:			//-------------------- public --------------------//

				CYAMAKAGE_WEAPON(CONST DEV inDev);
		virtual	~CYAMAKAGE_WEAPON(void);

			//==================================================================
			//	access	動作制御
			//==================================================================
				YAMAKAGE_WEAPON_ACTION	getWeaponAction(void)const			{ return	weaponAction;		}
				void					setWeaponAction(YAMAKAGE_WEAPON_ACTION inAction);

			//==================================================================
			//	access	外部ポインター
			//==================================================================
				void	setTargetPlayer(CKAGETOKI_ADVENT_CHARACTER** inPlayer)
				{
					this->player	= inPlayer;
				}
				void	setViewG(C3D_OBJECT_GROUP* inG)		{ viewG		= inG;	}
				void	setStageG(C3D_OBJECT_GROUP* inG)	{ stageG	= inG;	}

			//==================================================================
			//	リアクション
			//==================================================================
				void	reactionMessage(REACTION_MESSAGE inMessage);

			//==================================================================
			//	基本操作
			//==================================================================
				void	operation(void);

			//==================================================================
			//	描画(サブクラスでオーバーライド可能)
			//==================================================================
				BOOL	draw(CONST DEV inDev);
	};
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------