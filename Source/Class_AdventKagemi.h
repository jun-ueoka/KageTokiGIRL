//***********************************************************************************
//!	ファイル名		Class_AdventKagemi.h
//
//	機能			アドベンチャーゲームモードの「カゲミ」クラスの定義
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2008/12/11		<新規>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_KageTokiAdventCharacter.h"			//!< アドベンチャーゲームモードキャラクター
#include	"Class_KagemiBall.h"						//!< カゲミボール
#include	"Class_Sound_OggVorbis.h"					//!< oggライブラリ(サウンドを発生させる為)

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	専用マクロ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		MAXFRAME_ENERGY_BALL			30		//!< エナジーボールが出るまでのフレーム
	#define		MAXFRAME_ENERGY_BALL_OPERATION	330		//!< エナジーボールを操作出来るフレーム
	#define		MAXFRAME_ENERGY_SHOOT			100		//!< エナジーシュートが出るまでのフレーム

	#define		KAGEMI_ENERGY_BALL		MYKEY::eSOFT	//!< 技「エナジーボール」
	#define		KAGEMI_ENERGY_SHOOT		MYKEY::eDUST	//!< 技「エナジーシュート」

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	カゲミの入力有効制御
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ADVENT_KAGEMI_ACTION_LIMIT
	{
		eADKAGEMIL_LV1 = 1,								//!< レベル1(ボールが打てない)
		eADKAGEMIL_LV2,									//!< レベル2(エナジーボールが打てる)
		eADKAGEMIL_LV3,									//!< レベル3(エナジーシュートが打てる)
		eADKAGEMIL_MAX,									//!< 入力有効制御の数(何でも出来る)
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	カゲミの行動パターン
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ADVENT_KAGEMI_ACTION
	{
		eADKAGEMIA_WAIT,								//!< 待機
		eADKAGEMIA_WALK,								//!< 歩行
		eADKAGEMIA_FLIGHT,								//!< 飛行
		eADKAGEMIA_SKILL_ENERGY_BALL_SHOT,				//!< 技「エナジーボール発射」
		eADKAGEMIA_SKILL_ENERGY_BALL_OPERATION,			//!< 技「エナジーボール操作」
		eADKAGEMIA_SKILL_ENERGY_SHOOT,					//!< 技「エナジーシュート」
		eADKAGEMIA_DAMAGE_LV1,							//!< ダメージLV1反動
		eADKAGEMIA_DAMAGE_LV2,							//!< ダメージLV2反動
		eADKAGEMIA_MAX,									//!< 行動の数
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CADVENT_KAGEMI
	//
	//	機能			アドベンチャーゲームモードの「カゲミ」クラス
	//	更新			2008/12/11		<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CADVENT_KAGEMI : public CKAGETOKI_ADVENT_CHARACTER
	{
			//==================================================================
			//	アドベンチャーモードのカゲミのXファイル名
			//==================================================================
				#define	ADVENT_KAGEMI				"advent_kagemi"
				#define	KAGEMI_COMMENT_ENERGYBALL	"kagemi_comment_energyball"
				#define	KAGEMI_COMMENT_ENERGYSHOOT	"kagemi_comment_energyshoot"

	private:		//-------------------- private --------------------//

				ADVENT_KAGEMI_ACTION_LIMIT	limit;			//!< カゲミの入力有効制御
				ADVENT_KAGEMI_ACTION		action;			//!< カゲミの状態
				int				iAnimTemp[eADKAGEMIA_MAX];	//!< アニメーションテンプレート
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
				SSTATIC*		sStaticShotEnergy;			//!< エナジーボール発射音

			//==================================================================
			//	サウンド操作
			//==================================================================
				void			soundStep(void);			//!< 足音の発生
				void			soundJumpStart(void);		//!< ジャンプの発生
				void			soundJumpEnd(void);			//!< 着地音
				void			soundShotEnergy(void);		//!< エナジーボール発射音

			//==================================================================
			//	「エナジーボール」「エナジーシュート」
			//==================================================================
				VEC3			ballAngle;					//!< ボールの向き

				CKAGEMI_BALL*	originEnergyBall;			//!< オリジナル「エナジーボール」
				CKAGEMI_BALL*	opeEnergyBall;				//!< 操作中エナジーボール
				float			energyBallSpeed;			//!< エナジーボールの速度
				int				energyBallFrame;			//!< エナジーボールが消えるまでの時間
				float			energyShootSpeed;			//!< エナジーシュートの速度
				int				energyShootFrame;			//!< エナジーシュートが消えるまでの時間

			//==================================================================
			//	条件操作の処理分け
			//==================================================================
				void	opeWait(CINPUT* input);						//!< 待機時
				void	opeWalk(CINPUT* input);						//!< 歩行時
				void	opeFlight(CINPUT* input);					//!< 飛行時
				void	opeSkillEnergyBallShot(CINPUT* input);		//!< 技「エナジーボール発射」時
				void	opeSkillEnergyBallOperation(CINPUT* input);	//!< 技「エナジーボール操作」時
				void	opeSkillEnergyShoot(CINPUT* input);			//!< 技「エナジーシュート」時
				void	opeDamageLv1(CINPUT* input);				//!< ダメージLv1反動時
				void	opeDamageLv2(CINPUT* input);				//!< ダメージLv2反動時

				BOOL	opeSkillCheck(CINPUT* input);				//!< 技入力判定
				void	opeBallAngle(CINPUT* input);				//!< ボール技の方向判定

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	技発生
			//==================================================================
				void	skillEnergyBall(void);						//!< エナジーボール
				void	skillEnergyShoot(void);						//!< エナジーシュート

	public:			//-------------------- public --------------------//

				CADVENT_KAGEMI(
					CONST DEV	inDev,
					CONST LPSTR	inName     = ADVENT_KAGEMI,
					CONST LPSTR	inResource = PATH_SKINMESH,
					CONST LPSTR	inTexture  = PATH_SKINMESHTEX);		//!< コンストラクタ
		virtual	~CADVENT_KAGEMI(void);

			//==================================================================
			//	access	入力有効制御
			//==================================================================
				int		getLimit(void)	{ return	(int)limit;	}
				void	setLimit(ADVENT_KAGEMI_ACTION_LIMIT inLimit)
				{
					if		(inLimit < eADKAGEOL_LV1)	limit = eADKAGEMIL_LV1;
					else if	(inLimit > eADKAGEOL_MAX)	limit = eADKAGEMIL_MAX;
					else								limit = inLimit;
				}

			//==================================================================
			//	状態変化
			//==================================================================
				BOOL	changeAction(ADVENT_KAGEMI_ACTION inNewAction);
				ADVENT_KAGEMI_ACTION	getAction(void)const			{ return	action;		}
				void	safeResetChangeToWait(void);			//!< 安全に待機に移行する

			//==================================================================
			//	access	操作中のエナジーボール
			//==================================================================
				CKAGEMI_BALL*	getOpeEnergyBall(void)					{ return	opeEnergyBall;	}

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