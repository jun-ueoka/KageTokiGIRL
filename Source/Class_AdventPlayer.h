//***********************************************************************************
//!	ファイル名		Class_AdventPlayer.h
//
//	機能			アドベンチャーゲームモードのプレイヤークラスの定義
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2008/12/11	<新規>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_AdventKageo.h"		//!< アドベンチャーゲームモード「カゲオ」
#include	"Class_AdventKagemi.h"		//!< アドベンチャーゲームモード「カゲミ」
#include	"Class_3DObjectCamera.h"	//!< 3Dオブジェクトカメラ
#include	"Class_Scene.h"				//!< シーン

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	アドベンチャープレイヤー選択フラグ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ADVENT_CHOICE_CHARACTER
	{
		eADCC_KAGEO = 0,							//!< カゲオ
		eADCC_KAGEMI,								//!< カゲミ
		eADCC_MAX,									//!< 最大数
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	プレイヤーエフェクトフラグ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ADVENT_PLAYER_EFFECT
	{
		eADPEFF_ACHE,								//!< 痛い
		eADPEFF_ANGER,								//!< 怒り
		eADPEFF_HASTE,								//!< 焦り
		eADPEFF_DATHE,								//!< 死
		eADPEFF_MAX,								//!< 最大数
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CADVENT_PLAYER
	//
	//	機能			アドベンチャーゲームモードのプレイヤークラス
	//	更新			2008/12/11	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CADVENT_PLAYER
	{
			//==================================================================
			//	ゲームインターフェース関連
			//==================================================================
				#define	NAME_INTERFACE			"game_interface_player"

			//==================================================================
			//	吹き出し関連
			//==================================================================
				#define	KAGEO_COMMENT_ACHE		"kageo_comment_ache"
				#define	KAGEO_COMMENT_ANGER		"kageo_comment_anger"
				#define	KAGEO_COMMENT_HASTE		"kageo_comment_haste"
				#define	KAGEO_COMMENT_DATHE		"kageo_comment_uwaaaa"
				#define	KAGEMI_COMMENT_ACHE		"kagemi_comment_ache"
				#define	KAGEMI_COMMENT_ANGER	"kagemi_comment_anger"
				#define	KAGEMI_COMMENT_HASTE	"kagemi_comment_haste"
				#define	KAGEMI_COMMENT_DATHE	"kagemi_comment_kyaaaa"

	private:		//-------------------- private --------------------//

			//==================================================================
			//	アドベンチャーカゲオ&カゲミ
			//==================================================================
				CADVENT_KAGEO*		adventKageo;
				CADVENT_KAGEMI*		adventKagemi;

			//==================================================================
			//	ゲームインターフェース関連
			//==================================================================
				CTEXTURE*			texKageoLife;		//!< カゲオライフの土台
				CTEXTURE*			texKagemiLife;		//!< カゲミライフの土台
				BOOL				bDamageCheck;		//!< ダメージ判定確認
				int					iMaxLife;			//!< 最大体力
				int					iLife;				//!< 残り体力
				VEC3				keepStartLoc;		//!< 初期位置

			//==================================================================
			//	外部ポインター
			//==================================================================
				CKAGETOKI_ADVENT_CHARACTER**	target;
				C3D_OBJECT_CAMERA*				camera;
				SCENE_SHARE_DATA*				share;

			//==================================================================
			//	フラグ群
			//==================================================================
				ADVENT_CHOICE_CHARACTER			choiceChara;
				BOOL							bChoiceCharaChange;

	public:			//-------------------- public --------------------//

				CADVENT_PLAYER(CONST DEV inDev, CKAGETOKI_ADVENT_CHARACTER** inTarget,
							   SCENE_SHARE_DATA* inShare);
		virtual	~CADVENT_PLAYER(void);

			//==================================================================
			//	外部ポインター関連
			//==================================================================
				void	setCameraPointer(C3D_OBJECT_CAMERA* inCamera)	{ camera = inCamera;	}
				void	setViewG(C3D_OBJECT_GROUP* inGroup);
				void	setStageG(C3D_OBJECT_GROUP* inGroup);
				void	setEnemyG(C3D_OBJECT_GROUP* inGroup);

			//==================================================================
			//	キャラクター操作関連
			//==================================================================
				void	setAllLoc(VEC3* inLoc);
				void	setAllAngle(VEC3* inAngle);
				void	setAllVisible(BOOL inVisible);
				void	setAllLimit(int inLimit);
				void	resetAllWay(void);
				void	resetPlayer(VEC3* inStartLoc = NULL);

				ADVENT_CHOICE_CHARACTER	getChoiceCharacter(void)const	{ return	choiceChara;			}
				void	setChoiceCharacter(ADVENT_CHOICE_CHARACTER inChara);
				void	setChoiceCharaChangeBoolen(BOOL inBoolen)		{ bChoiceCharaChange = inBoolen;	}

			//==================================================================
			//	access	キャラクター
			//==================================================================
				CKAGETOKI_ADVENT_CHARACTER*	getPlayChara(void);
				CADVENT_KAGEO*				getKageo(void)const		{ return	adventKageo;	}
				CADVENT_KAGEMI*				getKagemi(void)const	{ return	adventKagemi;	}

			//==================================================================
			//	access	ダメージ判定確認
			//==================================================================
				BOOL	getDamageCheck(void)const		{ return	bDamageCheck;				}
				void	setDamageCheck(BOOL inBoolen)	{ bDamageCheck	= inBoolen;				}

			//==================================================================
			//	プレイヤーエフェクト発生
			//==================================================================
				void	occuPlayerEffect(ADVENT_PLAYER_EFFECT inEffectFlag);

			//==================================================================
			//	キャラクター死亡確認
			//==================================================================
				BOOL	getCharacterOver(void)	{ return (iLife <= 0) ? TRUE : FALSE;	}

			//==================================================================
			//	プレイヤー操作
			//==================================================================
				void	operation(CINPUT* input);

			//==================================================================
			//	プレイヤーインターフェースの描画
			//==================================================================
				void	drawInterface(SPRITE inSprite);
	};
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------