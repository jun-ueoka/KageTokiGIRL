//***********************************************************************************
//!	ファイル名		Class_Scene.h
//
//	機能			シーン分けするクラスの定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/04/21	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	<windows.h>
#include	"Version.h"
#include	"Class_DirectInput.h"

//-----------------------------------------------------------------------
//!	シーン分岐フラグ
//-----------------------------------------------------------------------
typedef	enum APP_SCENE_FLAG
{
	//オープニングフロー
	eSCENE_NO = 0,						//!< 命令なし
	eSCENE_LOGO,						//!< ロゴ
	eSCENE_DEMO,						//!< デモ
	eSCENE_TITLE,						//!< タイトル
	eSCENE_OPTION,						//!< オプション

	//チャプター1[謎の集団]
	eSCENE_CHAPTER1_MOVIE1,				//!< ムービー1[デート]
	eSCENE_CHAPTER1_STAGE1,				//!< ステージ1[公園]
	eSCENE_CHAPTER1_MOVIE2,				//!< ムービー2[シャドウクリミナル、現る]
	eSCENE_CHAPTER1_STAGE2,				//!< ステージ2[公園出口]
	eSCENE_CHAPTER1_MOVIE3,				//!< ムービー3[シャドウクリミナルアジト]
	eSCENE_CHAPTER1_STAGE3,				//!< ステージ3[村]
	eSCENE_CHAPTER1_BOSS1,				//!< ボス1[エクス]

	//チャプター2[シャドウクリミナルを追え!]
	eSCENE_CHAPTER2_MOVIE4,				//!< ムービー4[敵を追って…]
	eSCENE_CHAPTER2_STAGE4,				//!< ステージ4[荒野]
	eSCENE_CHAPTER2_MOVIE5,				//!< ムービー5[山あり…谷あり…]
	eSCENE_CHAPTER2_STAGE5,				//!< ステージ5[山]
	eSCENE_CHAPTER2_MOVIE6,				//!< ムービー6[アジト?]
	eSCENE_CHAPTER2_STAGE6,				//!< ステージ6[洞窟]
	eSCENE_CHAPTER2_BOSS2,				//!< ボス2[カゲヤマ]

	//チャプター3[アジト急激]
	eSCENE_CHAPTER3_MOVIE7,				//!< ムービー7[見つけた手段]
	eSCENE_CHAPTER3_STAGE7,				//!< ステージ7[上空]
	eSCENE_CHAPTER3_BOSS3,				//!< ボス3[フェザー]
	eSCENE_CHAPTER3_MOVIE8,				//!< ムービー8[本拠地発見]
	eSCENE_CHAPTER3_STAGE8,				//!< ステージ8[城下町]
	eSCENE_CHAPTER3_MOVIE9,				//!< ムービー9[門は叩かず、壊してけ!]
	eSCENE_CHAPTER3_STAGE9,				//!< ステージ9[城内部]

	//ラストチャプター[クライマックス]
	eSCENE_LAST_CHAPTER_MOVIE10,		//!< ムービー10[追い詰めたぞ!]
	eSCENE_LAST_CHAPTER_STAGE10,		//!< ステージ10[巨大地下迷宮]
	eSCENE_LAST_CHAPTER_BOSS4,			//!< ボス4[ティマ]
	eSCENE_LAST_CHAPTER_MOVIE11,		//!< ムービー11[最終決戦!]
	eSCENE_LAST_CHAPTER_LAST_BOSS,		//!< ラスボス[クライム]
	eSCENE_LAST_CHAPTER_ENDING_MOVIE,	//!< エンディングムービー[平和の訪れ]

	//エンディングフロー
	eSCENE_ENDING,						//!< エンディング
	eSCENE_GAMEOVER,					//!< ゲームオーバー

	eSCENE_QUIT,						//!< ゲーム終了
	eSCENE_ERROR,						//!< ゲームエラー
	eSCENE_MAX							//!< フラグの最大数
}	SCENE_FLAG;

//-----------------------------------------------------------------------
//	デバッグ用フラグ
//-----------------------------------------------------------------------
#define	START_FLAG			eSCENE_LOGO
#ifdef	_DEBUG
#define	DEBUG_FLAG			eSCENE_CHAPTER1_STAGE1
#endif	//_DEBUG

#ifdef	PRESENTATION
#define	PRESENTATION_FLAG	eSCENE_LOGO
#endif


//-----------------------------------------------------------------------
//!	各シーンで共有する情報
//-----------------------------------------------------------------------
struct	SCENE_SHARE_DATA
{
	DWORD	dwGameLevel;			//!< ゲームレベル
	DWORD	dwPlayerLife;			//!< 残気
};

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CSCENE
	//
	//	機能			シーンを管理するクラス
	//	更新			2008/04/21	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE
	{
	protected:		//-------------------- protected --------------------//

				SCENE_FLAG			returnFlag;				//!< リターンフラグ
				SCENE_FLAG			nextFlag;				//!< 次シーンフラグ
				SCENE_SHARE_DATA*	ssData;					//!< シーン共有データ

	public:			//-------------------- public --------------------//

				CSCENE(SCENE_FLAG inFlag, SCENE_FLAG inNextFlag, SCENE_SHARE_DATA* inData)
				{
					returnFlag	= inFlag;
					nextFlag	= inNextFlag;
					ssData		= inData;
				}
		virtual	~CSCENE(void)	{}

		virtual	void	sceneInitialize(int)		= 0;	//!< 初期化
		virtual	void	sceneFinishalize(void)		= 0;	//!< 解放

		virtual	SCENE_FLAG		sceneMain(CINPUT* input)	= 0;	//!< シーンメイン
	};
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------