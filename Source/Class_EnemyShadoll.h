//***********************************************************************************
//!	ファイル名		Class_EnemyShadoll.h
//
//	機能			敵キャラクタークラス「シャドール」の定義
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2009/01/15	<新規>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_KageTokiAdventEnemy.h"		//!< アドベンチャーゲームモード敵キャラクター
#include	"Class_Sound_OggVorbis.h"			//!< oggライブラリ(サウンドを発生させる為)

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	シャドールの行動パターン
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ENEMYSHADOLL_ACTION
	{
		eESHADOLLA_WAIT,							//!< 待機時
		eESHADOLLA_WALK,							//!< 歩行時
		eESHADOLLA_DASH,							//!< ダッシュ時
		eESHADOLLA_MAX,								//!< 行動の最大数
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CENEMY_SHADOLL
	//
	//	機能			アドベンチャーゲームモードの「シャドール」クラス
	//	更新			2009/01/015	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CENEMY_SHADOLL : public CKAGETOKI_ADVENT_ENEMY
	{
			//==================================================================
			//	敵「シャドール」マクロ
			//==================================================================
				#define	NAME_SHADOLL			"enemy_shadoll"

	private:		//-------------------- private --------------------//

				ENEMYSHADOLL_ACTION	action;						//!< シャドールの状態
				int					iAnimTemp[eESHADOLLA_MAX];	//!< アニメーションテンプレート

				int					iHitFrame;					//!< 衝突点滅フレーム

			//==================================================================
			//	サウンドデータ
			//==================================================================
				int				stepCnt;					//!< ステップカウンタ
				SSTATIC*		sStaticStep;				//!< 足音
				SSTATIC*		sStaticJumpStart;			//!< ジャンプ音
				SSTATIC*		sStaticJumpEnd;				//!< 着地音

				void	soundStep(void);					//!< 足音の発生
				void	soundJumpStart(void);				//!< ジャンプの発生
				void	soundJumpEnd(void);					//!< 着地音
				void	soundDash(void);					//!< ダッシュ音

			//==================================================================
			//	条件操作の処理分け
			//==================================================================
				void	opeWait(void);						//!< 待機時
				void	opeWalk(void);						//!< 歩行時
				void	opeDash(void);						//!< ダッシュ時

	public:			//-------------------- public --------------------//

				CENEMY_SHADOLL(
					CONST DEV	inDev,
					CONST LPSTR	inName     = NAME_SHADOLL,
					CONST LPSTR	inResource = PATH_SKINMESH,
					CONST LPSTR	inTexture  = PATH_SKINMESHTEX);	//!< コンストラクタ
		virtual	~CENEMY_SHADOLL(void);

			//==================================================================
			//	状態変化
			//==================================================================
				BOOL	changeAction(ENEMYSHADOLL_ACTION inNewAction);

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