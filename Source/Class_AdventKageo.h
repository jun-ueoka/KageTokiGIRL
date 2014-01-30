//***********************************************************************************
//!	ファイル名		Class_AdventKageo.h
//
//	機能			アドベンチャーゲームモードの「カゲオ」クラスの定義
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2008/12/09	<新規>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_KageTokiAdventCharacter.h"	//!< アドベンチャーゲームモードキャラクター
#include	"Class_KageoBall.h"					//!< カゲオボール
#include	"Class_Sound_OggVorbis.h"			//!< oggライブラリ(サウンドを発生させる為)

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	専用マクロ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		MAXFRAME_SHADOW_BALL	15				//!< シャドウボールが出るまでのフレーム
	#define		MAXFRAME_SHINE_BALL		15				//!< シャインボールが出るまでのフレーム

	#define		KAGEO_SHADOW_BALL		MYKEY::eDUST	//!< 技「シャドウボール」
	#define		KAGEO_SHINE_BALL		MYKEY::eSOFT	//!< 技「シャインボール」

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	カゲオの入力有効制御
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ADVENT_KAGEO_ACTION_LIMIT
	{
		eADKAGEOL_LV1 = 1,								//!< レベル1(1ボールが打てない)
		eADKAGEOL_LV2,									//!< レベル2(シャドウボールが打てる)
		eADKAGEOL_LV3,									//!< レベル3(シャインボールが打てる)
		eADKAGEOL_LV4,									//!< レベル4(方向を定められる)
		eADKAGEOL_MAX,									//!< 入力有効制御の数(何でも出来る)
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	カゲオの行動パターン
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ADVENT_KAGEO_ACTION
	{
		eADKAGEOA_WAIT,									//!< 待機
		eADKAGEOA_WALK,									//!< 歩行
		eADKAGEOA_FLIGHT,								//!< 飛行
		eADKAGEOA_SKILL_SHADOW_BALL,					//!< 技「シャドウボール」
		eADKAGEOA_SKILL_SHINE_BALL,						//!< 技「シャインボール」
		eADKAGEOA_DAMAGE_LV1,							//!< ダメージLV1反動
		eADKAGEOA_DAMAGE_LV2,							//!< ダメージLV2反動
		eADKAGEOA_MAX,									//!< 行動の数
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CADVENT_KAGEO
	//
	//	機能			アドベンチャーゲームモードの「カゲオ」クラス
	//	更新			2008/12/09	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CADVENT_KAGEO : public CKAGETOKI_ADVENT_CHARACTER
	{
			//==================================================================
			//	アドベンチャーモードのカゲオのXファイル名
			//==================================================================
				#define	ADVENT_KAGEO				"advent_kageo"
				#define	KAGEO_COMMENT_SHADOWBALL	"kageo_comment_shadowball"
				#define	KAGEO_COMMENT_SHINEBALL		"kageo_comment_shineball"

	private:		//-------------------- private --------------------//

				ADVENT_KAGEO_ACTION_LIMIT	limit;			//!< カゲオの入力有効制御
				ADVENT_KAGEO_ACTION			action;			//!< カゲオの状態
				int				iAnimTemp[eADKAGEOA_MAX];	//!< アニメーションテンプレート
				int				iHitFrame;					//!< 衝突点滅フレーム
				BOOL			bJunpInput;					//!< 飛翔入力確認
				CTEXTURE*		texArrow;					//!< 矢印

			//==================================================================
			//	サウンドデータ
			//==================================================================
				int				stepCnt;					//!< ステップカウンタ
				SSTATIC*		sStaticStep;				//!< 足音
				SSTATIC*		sStaticJumpStart;			//!< ジャンプ音
				SSTATIC*		sStaticJumpEnd;				//!< 着地音
				SSTATIC*		sStaticShotShadow;			//!< シャドウボール発射音
				SSTATIC*		sStaticShotShine;			//!< シャインボール発射音

			//==================================================================
			//	サウンド操作
			//==================================================================
				void			soundStep(void);			//!< 足音の発生
				void			soundJumpStart(void);		//!< ジャンプの発生
				void			soundJumpEnd(void);			//!< 着地音
				void			soundShotShadow(void);		//!< シャドウボール発射音
				void			soundShotShine(void);		//!< シャインボール発射音

			//==================================================================
			//	「シャドウボール」「シャインボール」
			//==================================================================
				VEC3			ballAngle;					//!< ボールの向き

				CKAGEO_BALL*	originShadowBall;			//!< オリジナル「シャドウボール」
				float			shadowBallSpeed;			//!< シャドウボールの速度
				int				shadowBallFrame;			//!< シャドウボールが消えるまでの時間

				CKAGEO_BALL*	originShineBall;			//!< オリジナル「シャインボール」
				float			shineBallSpeed;				//!< シャインボールの速度
				int				shineBallFrame;				//!< シャインボールが消えるまでの時間

			//==================================================================
			//	条件操作の処理分け
			//==================================================================
				void	opeWait(CINPUT* input);				//!< 待機時
				void	opeWalk(CINPUT* input);				//!< 歩行時
				void	opeFlight(CINPUT* input);			//!< 飛行時
				void	opeSkillShadowBall(CINPUT* input);	//!< 技「シャドウボール」時
				void	opeSkillShineBall(CINPUT* input);	//!< 技「シャインボール」時
				void	opeDamageLv1(CINPUT* input);		//!< ダメージLv1反動時
				void	opeDamageLv2(CINPUT* input);		//!< ダメージLv1反動時

				BOOL	opeSkillCheck(CINPUT* input);		//!< 技入力判定
				void	opeBallAngle(CINPUT* input);		//!< ボール技の方向判定

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	技発生
			//==================================================================
				void	skillShadowBall(void);				//!< シャドウボール
				void	skillShineBall(void);				//!< シャインボール

	public:			//-------------------- public --------------------//

				CADVENT_KAGEO(
					CONST DEV	inDev,
					CONST LPSTR	inName     = ADVENT_KAGEO,
					CONST LPSTR	inResource = PATH_SKINMESH,
					CONST LPSTR	inTexture  = PATH_SKINMESHTEX);	//!< コンストラクタ
		virtual	~CADVENT_KAGEO(void);

			//==================================================================
			//	access	入力有効制御
			//==================================================================
				int		getLimit(void)	{ return	(int)limit;		}
				void	setLimit(ADVENT_KAGEO_ACTION_LIMIT inLimit)
				{
					if		(inLimit < eADKAGEOL_LV1)	limit = eADKAGEOL_LV1;
					else if	(inLimit > eADKAGEOL_MAX)	limit = eADKAGEOL_MAX;
					else								limit = inLimit;
				}

			//==================================================================
			//	状態変化
			//==================================================================
				BOOL	changeAction(ADVENT_KAGEO_ACTION inNewAction);

			//==================================================================
			//	リアクション(正式なアクションをするサブクラスでオーバーライド)
			//==================================================================
				void	reactionMessage(REACTION_MESSAGE inMessage);

			//==================================================================
			//	基本操作
			//==================================================================
				void	operation(CINPUT* input);

			//==================================================================
			//	描画
			//==================================================================
				BOOL	draw(CONST DEV inDev);
	};
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------