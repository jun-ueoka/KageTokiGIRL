//***********************************************************************************
//!	ファイル名		Class_KageoBall.h
//
//	機能			カゲオが放つ、玉を扱うクラスの定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/08/25	<新規>
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
	//	カゲオボールの種類制御
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	KAGEO_BALL_KIND
	{
		eKOBALL_SHADOW,						//!< シャドウボール
		eKOBALL_SHINE,						//!< シャインボール
		eKOBALL_MAX,						//!< 種類の最大数
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CKAGEO_BALL
	//
	//	機能			カゲオが放つ、玉を扱うクラス
	//	機能概要		この玉に当たった物のメッセージを変更する
	//					[judgMessage]と同じメッセージを持つ物に衝突すれと、
	//					その物に[reactMessage]を送る
	//	更新			2008/08/25	<新規>
	//					2008/08/27	<変更>	使い回しが利く用にした
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CKAGEO_BALL : public CMESH
	{
				#define		SHADOW_BALL			"shadow_ball"
				#define		SHINE_BALL			"shine_ball"
				#define		EFFECT_SHADOW		"shadow_ball"
				#define		EFFECT_SHINE		"shine_ball"
				#define		EFFECT_NAME_MAX		15

	private:		//-------------------- private --------------------//

			//==================================================================
			//	カゲオボールの種類にで、処理検索
			//==================================================================
				LPSTR				getBallTextureName(KAGEO_BALL_KIND inKind);

				BOOL				activate;						//!< 発射確認(TRUE = 発射済み)
				THIS_MESSAGE		judgMessage;					//!< 条件識別メッセージ
				REACTION_MESSAGE	reactMessage;					//!< リアクションメッセージ
				C3D_OBJECT_GROUP*	hitG;							//!< 衝突判定グループ
				char				effectName[EFFECT_NAME_MAX];	//!< 発生エフェクト名
				int					iEffectCnt;						//!< エフェクトの発生間隔
				int					iFrame;							//!< 描画フレーム
				D3DCOLOR			smokeColor;						//!< 煙の色

			//==================================================================
			//	音操作
			//==================================================================
				SSTATIC*	sStaticHit;								//!< 衝突音
				void		soundHit(void)							//!< 衝突音発生
				{	if(	sStaticHit){
						sStaticHit->setPlayCur(0);
						sStaticHit->play(0, NULL);	}	}
				LPSTR		getHitSoundName(void)					//!< 衝突音名を取得
				{	if(	sStaticHit)	{	return	sStaticHit->getFileName();	}
					else			{	return	NULL;						}	}

	public:			//-------------------- public --------------------//

				CKAGEO_BALL(CONST DEV inDev, KAGEO_BALL_KIND inKind);
				CKAGEO_BALL(CKAGEO_BALL* inCopy);
		virtual	~CKAGEO_BALL(void);

			//==================================================================
			//	access	衝突判定確認
			//==================================================================
				BOOL	getActivate(void)const					{ return	activate;			}

			//==================================================================
			//	access	衝突判定グループ
			//==================================================================
				C3D_OBJECT_GROUP*	getHitGroup(void)const					{ return	hitG;	}
				void				setHitGroup(C3D_OBJECT_GROUP* inHitG)	{ hitG	= inHitG;	}

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
				void	shot(VEC3* inAngle, VEC3* inLoc, C3D_OBJECT_GROUP* inHitG, int inFrame);

			//==================================================================
			//	衝突判定
			//==================================================================
				void	intersectG(void);

			//==================================================================
			//	描画(サブクラスでオーバーライド可能)
			//==================================================================
		virtual	BOOL	draw(CONST DEV inDev);
		virtual	BOOL	draw(CZTEX_SHA*  inZTexSha)	{ return	FALSE;	}
		virtual	BOOL	draw(CDEPTH_SHA* inDepthSha);
	};
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------