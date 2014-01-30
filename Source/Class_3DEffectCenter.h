//***********************************************************************************
//!	ファイル名		Class_3DEffectCenter.h
//
//	機能			3Dゲーム専用のエフェクトクラスを、
//					一点に纏めて描画するクラスの定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/09/16	<新規>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_3DEffect.h"				//!< エフェクトベース
#include	"Class_3DObjectGroup.h"			//!< 3Dオブジェクトグループ

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	よく使われるエフェクト名
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define	EFFECT_SMOKE				"smoke"					//!< 煙
	#define	EFFECT_SHADOW				"shadow_ball"			//!< シャドウエフェクト
	#define	EFFECT_SHINE				"shine_ball"			//!< シャインエフェクト
	#define	EFFECT_STARDUST				"star_dust"				//!< 星屑
	#define	EFFECT_CONTINUE				"continue"				//!< コンティニューエフェクト名
	#define	EFFECT_CONTINUE_YES			"continue_yes"			//!< コンティニュー「YES」エフェクト名
	#define	EFFECT_CONTINUE_NO			"continue_no"			//!< コンティニュー「NO」エフェクト名
	#define	EFFECT_CONTINUE_YES_SMOKE	"continue_yes_smoke"	//!< コンティニュー「YES」演出
	#define	EFFECT_CONTINUE_NO_SMOKE	"continue_no_smoke"		//!< コンティニュー「NO」演出

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		C3D_EFFECT_CENTER
	//
	//	機能			三次元のエフェクトを、全て扱うクラス(エフェクトセンター)
	//	更新			2008/09/11	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class C3D_EFFECT_CENTER
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	中枢インスタンス
			//==================================================================
		static	C3D_EFFECT_CENTER*			gUniqueInstance;

			//==================================================================
			//	エフェクト管理属性
			//==================================================================
				C3D_OBJECT_GROUP			effects;			//!< 発生エフェクトグループ
				C3D_OBJECT_GROUP			effects2D;			//!< 2D描画エフェクトグループ
				map<string, C3D_OBJECT*>	effectMap;			//!< エフェクトマップ

	public:			//-------------------- public --------------------//

				C3D_EFFECT_CENTER(void);
				~C3D_EFFECT_CENTER(void);

		static	C3D_EFFECT_CENTER*	getInstance(void);			//!< インスタンスを取得
		static	void				releaseInstance(void);		//!< インスタンスを解放


				void	initialize(void);						//!< 初期化処理
				void	finishalize(void);						//!< 解放処理

			//==================================================================
			//	インスタンスにエフェクト登録、発生、解放
			//==================================================================
		static	BOOL	addEffectsInstance(string inName, C3D_OBJECT* inEffect);
		static	void	occuEffectsInstance(	string		inName,
												VEC3		inLoc,
												float		inSize	= 1.0f,
												VEC3		inAngle	= VEC3(0.0f,0.0f,0.0f),
												D3DCOLOR	inColor	= 0xFFFFFFFF);
		static	void	occu2DEffectsInstance(	string		inName,
												VEC3		inLoc,
												float		inSize	= 1.0f,
												VEC3		inAngle	= VEC3(0.0f,0.0f,0.0f),
												D3DCOLOR	inColor	= 0xFFFFFFFF);
		static	void	releaseEffectInstance(string inName);

			//==================================================================
			//	エフェクトの登録、発生、解放
			//==================================================================
				BOOL	addEffects(string inName, C3D_OBJECT* inEffect);
				void	occuEffects(	string		inName,
										VEC3		inLoc,
										float		inSize	= 1.0f,
										VEC3		inAngle	= VEC3(0.0f,0.0f,0.0f),
										D3DCOLOR	inColor	= 0xFFFFFFFF);
				void	occu2DEffects(	string		inName,
										VEC3		inLoc,
										float		inSize	= 1.0f,
										VEC3		inAngle	= VEC3(0.0f,0.0f,0.0f),
										D3DCOLOR	inColor	= 0xFFFFFFFF);
				void	releaseEffect(string inName);

			//==================================================================
			//	解放
			//==================================================================
				void	releaseEffects(void)	{ effects.releaseClear();	}
				void	releaseEffects2D(void)	{ effects2D.releaseClear();	}

			//==================================================================
			//	全エフェクト描画
			//==================================================================
				void	drawEffects(CONST DEV inDev, C3D_OBJECT* inCamera);
				void	drawEffects2D(CONST SPRITE inSprite);
	};
}

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------