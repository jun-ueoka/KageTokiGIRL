//***********************************************************************************
//!	ファイル名		Class_KagemiBall.h
//
//	機能			カゲミが放つ、玉を扱うクラスの定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/12/11	<新規>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_Mesh.h"					//!< 3Dオブジェクトメッシュ
#include	"Class_3DObjectGroup.h"			//!< 3Dオブジェクトグループ
#include	"Class_3DEffectCenter.h"		//!< エフェクト中枢

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	カゲミボールの種類制御
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	KAGEMI_BALL_KIND
	{
		eKMIBALL_ENERGY,					//!< エナジーボール
		eKMIBALL_MAX,						//!< 種類の最大数
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CKAGEMI_BALL
	//
	//	機能			カゲミが放つ、玉を扱うクラス
	//	機能概要		この玉に当たった物のメッセージを変更する
	//					[judgMessage]と同じメッセージを持つ物に衝突すれと、
	//					その物に[reactMessage]を送る
	//	更新			2008/12/11	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CKAGEMI_BALL : public CMESH
	{
				#define		ENERGY_BALL			"energy_ball"
				#define		EFFECT_ENERGY		"energy_ball"
				#define		EFFECT_NAME_MAX		15

	private:		//-------------------- private --------------------//

			//==================================================================
			//	カゲミボールの種類にで、処理検索
			//==================================================================
				LPSTR				getBallTextureName(KAGEMI_BALL_KIND inKind);

				BOOL				activate;						//!< 発射確認(TRUE = 発射済み)
				THIS_MESSAGE		judgMessage;					//!< 条件識別メッセージ
				REACTION_MESSAGE	reactMessage;					//!< リアクションメッセージ
				C3D_OBJECT_GROUP*	stageG;							//!< ステージグループ
				C3D_OBJECT_GROUP*	enemyG;							//!< 敵グループ
				char				effectName[EFFECT_NAME_MAX];	//!< 発生エフェクト名
				int					iEffectCnt;						//!< エフェクトの発生間隔
				int					iFrame;							//!< 描画フレーム
				D3DCOLOR			smokeColor;						//!< 煙の色

	public:			//-------------------- public --------------------//

				CKAGEMI_BALL(CONST DEV inDev, KAGEMI_BALL_KIND inKind);
				CKAGEMI_BALL(CKAGEMI_BALL* inCopy);
		virtual	~CKAGEMI_BALL(void);

			//==================================================================
			//	access	衝突判定確認
			//==================================================================
				BOOL	getActivate(void)const					{ return	activate;			}

			//==================================================================
			//	access	メッセージ
			//==================================================================
				THIS_MESSAGE		getJudgMessage(void)const				{ return	judgMessage;		}
				REACTION_MESSAGE	getReactMessage(void)const				{ return	reactMessage;		}
				void				setJudgMessage(THIS_MESSAGE inMsg)		{ judgMessage	= inMsg;		}
				void				setReactMessage(REACTION_MESSAGE inMsg)	{ reactMessage	= inMsg;		}

			//==================================================================
			//	access	発生エフェクト関連
			//==================================================================
				LPSTR	getEffectName(void)						{ return	effectName;			}
				int		getEffectCnt(void)const					{ return	iEffectCnt;			}

			//==================================================================
			//	access	煙の色
			//==================================================================
				D3DCOLOR	getSmokeColor(void)const			{ return	smokeColor;			}
				void		getSmokeColor(D3DCOLOR inColor)		{ smokeColor	= inColor;		}

			//==================================================================
			//	発射(生成した後に使用)
			//==================================================================
				void	shot(VEC3* inAngle, VEC3* inLoc, C3D_OBJECT_GROUP* inStageG,
							 C3D_OBJECT_GROUP* inEnemyG, int inFrame);

			//==================================================================
			//	衝突判定
			//==================================================================
				void	intersectG(void);

			//==================================================================
			//	描画(サブクラスでオーバーライド可能)
			//==================================================================
		virtual	BOOL	draw(CONST DEV   inDev);
		virtual	BOOL	draw(CZTEX_SHA*  inZTexSha)	{ return	TRUE;	}
		virtual	BOOL	draw(CDEPTH_SHA* inDepthSha);
	};
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------