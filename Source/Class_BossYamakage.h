//***********************************************************************************
//!	ファイル名		Class_BossYamakage.h
//
//	機能			ボスキャラクタークラス「ヤマカゲ」の定義
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2009/02/16	<新規>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_KageTokiAdventBoss.h"	//!< アドベンチャーゲームボスベース
#include	"Class_Sound_OggVorbis.h"		//!< oggライブラリ(サウンドを発生させる為)
#include	"Class_YamakageWeapon.h"		//!< ヤマカゲの兵器

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	ヤマカゲの行動パターン
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	BOSSYAMAKAGE_ACTION
	{
		eBYAMAKAGEA_APPEAR,						//!< 出現時
		eBYAMAKAGEA_APPEAR_WAIT,				//!< 出現待機時
		eBYAMAKAGEA_WAIT,						//!< 待機時
		eBYAMAKAGEA_DAMAGE,						//!< ダメージ反動時
		eBYAMAKAGEA_WEAPON_TAKEOUT,				//!< 武器の取り出し
		eBYAMAKAGEA_WEAPON_ATTACK,				//!< 武器の攻撃
		eBYAMAKAGEA_ATTACK_ANDMOVE,				//!< 攻撃しながら移動
		eBYAMAKAGEA_DISAPPEAR_WAIT,				//!< 消滅待機時
		eBYAMAKAGEA_DISAPPEAR,					//!< 消滅時
		eBYAMAKAGEA_MAX,						//!< 行動の最大数
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CBOSS_YAMAKAGE
	//
	//	機能			アドベンチャーゲームモードの「ヤマカゲ」クラス
	//	更新			2009/02/16	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CBOSS_YAMAKAGE : public CKAGETOKI_ADVENT_BOSS
	{
			//==================================================================
			//	ボス「ヤマカゲ」マクロ
			//==================================================================
				#define	NAME_YAMAKAGE			"boss_yamakage"
				#define	YAMAKAGE_MAXLIFE		12

	private:		//-------------------- private --------------------//

				BOSSYAMAKAGE_ACTION	action;					//!< ヤマカゲの状態
				int					iAnimTemp[eBYAMAKAGEA_MAX];	//!< アニメーションテンプレート
				int					iHitFrame;					//!< 衝突点滅フレーム

				CYAMAKAGE_WEAPON*	yamakageWeaponA;		//!< ヤマカゲ兵器A
				CYAMAKAGE_WEAPON*	yamakageWeaponB;		//!< ヤマカゲ兵器B
				CYAMAKAGE_WEAPON*	yamakageWeaponC;		//!< ヤマカゲ兵器C
				CYAMAKAGE_WEAPON*	yamakageWeaponD;		//!< ヤマカゲ兵器D
				int					iAttackCnt;				//!< 攻撃回数制御用
				int					iMoveCnt;				//!< 移動制御用

			//==================================================================
			//	サウンドデータ
			//==================================================================
				SSTATIC*		sStaticJumpStart;			//!< ジャンプ音
				SSTATIC*		sStaticJumpEnd;				//!< 着地音

				void	soundJumpStart(void);				//!< ジャンプの発生
				void	soundJumpEnd(void);					//!< 着地音

			//==================================================================
			//	条件操作の処理分け
			//==================================================================
				void	opeAppear(void);					//!< 出現時
				void	opeAppearWait(void);				//!< 出現待機時
				void	opeWait(void);						//!< 待機時
				void	opeDamage(void);					//!< ダメージ反動時
				void	opeWeaponTakeOut(void);				//!< 武器取り出し時
				void	opeWeaponAttack(void);				//!< 武器攻撃
				void	opeAttackAndMove(void);				//!< 攻撃しながら移動
				void	opeDisappearWait(void);				//!< 消滅待機時
				void	opeDisappear(void);					//!< 消滅時

	protected:		//-------------------- protected --------------------//

	public:			//-------------------- public --------------------//

				CBOSS_YAMAKAGE(
					CONST DEV	inDev,
					CONST LPSTR	inName     = NAME_YAMAKAGE,
					CONST LPSTR	inResource = PATH_SKINMESH,
					CONST LPSTR	inTexture  = PATH_SKINMESHTEX);	//!< コンストラクタ
		virtual	~CBOSS_YAMAKAGE(void);

			//==================================================================
			//	状態変化
			//==================================================================
				BOOL	changeAction(BOSSYAMAKAGE_ACTION inNewAction);

			//==================================================================
			//	リアクション(正式なアクションをするサブクラスでオーバーライド)
			//==================================================================
				void	reactionMessage(REACTION_MESSAGE inMessage);

			//==================================================================
			//	リセット
			//==================================================================
				void	resetParameter(void);

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