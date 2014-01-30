//***********************************************************************************
//!	ファイル名		Class_SceneTitle.h
//
//	機能			タイトルシーンの定義
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2008/09/01	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"GameCommon.h"					//!< ゲームコモン
#include	"Class_Scene.h"					//!< シーン
#include	"Class_Sound_OggVorbis.h"		//!< oggライブラリ(サウンドを発生させる為)

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CSCENE_TITLE
	//
	//	機能			タイトルシーンのクラス
	//	更新			2008/09/01	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_TITLE : public CSCENE
	{
			//==================================================================
			//	カーソルの位置フラグ
			//==================================================================
				enum	CURSOR_POS
				{
					eCURP_START		= 0,					//!< 「START」に位置する
					eCURP_GO_START,							//!< 「START」に向かう
					eCURP_OPTION,							//!< 「OPTION」に位置する
					eCURP_GO_OPTION_FROM_START,				//!< 「START」から「OPTION」に向かう
					eCURP_GO_OPTION_FROM_END,				//!< 「END」から「OPTION」に向かう
					eCURP_END,								//!< 「END」に位置する
					eCURP_GO_END,							//!< 「END」に向かう
				};

			//==================================================================
			//	タイトルシーンの分岐
			//==================================================================
				enum	STITLE_FLAG
				{
					eST_INIT_TITLE,							//!< タイトル画面移行演出
					eST_TITLE,								//!< タイトル画面
					eST_START_SWITCH,						//!< ゲームスタート演出
					eST_END_SWITCH,							//!< ゲーム終了演出
					eST_DEMO,								//!< デモ移行演出
				};

			//==================================================================
			//	マクロ群
			//==================================================================
				#define	CURSOR_START_POSY		530.0f		//!< 「START」のY位置
				#define	CURSOR_OPTION_POSY		600.0f		//!< 「OPTION」のY位置
				#define	CURSOR_END_POSY			670.0f		//!< 「END」のY位置
				#define	CURSOR_MOVE_SPEED		 10.0f		//!< カーソル移動速度

				#define	STARTSWITCH_EXPAND		60			//!< スタート拡大
				#define	STARTSWITCH_CLEAR_S		100			//!< 透明化開始
				#define	STARTSWITCH_CLEAR_E		102			//!< 透明化終了
				#define	STARTSWITCH_END			200			//!< ゲームスタート演出終了

				#define	ENDSWITCH_EXPAND		60			//!< スタート拡大
				#define	ENDSWITCH_CLEAR_S		100			//!< 透明化開始
				#define	ENDSWITCH_CLEAR_E		102			//!< 透明化終了
				#define	ENDSWITCH_END			200			//!< ゲームスタート演出終了

				#define	DEMO_STEP_TIME			1800		//!< デモ移行フレーム
				#define	DEMO_CLEAR_S			0			//!< 透明化開始
				#define	DEMO_CLEAR_E			100			//!< 透明化終了
				#define	DEMO_END				100			//!< デモ移行演出終了

				#define	EFF_OCCU_SPACE			5				//!< エフェクト発生間隔
				#define	NAME_SHADOW				"shadow_ball"	//!< 影エフェクト名
				#define	NAME_SHINE				"shine_ball"	//!< 光エフェクト名

	private:		//-------------------- private --------------------//

			//==================================================================
			//	フラグ分岐先メソッド群
			//==================================================================
				void	motionInitTitle(CINPUT* input);		//!< タイトル画面移行演出処理
				void	motionTitle(CINPUT* input);			//!< タイトル画面処理
				void	motionStartSwitch(CINPUT* input);	//!< ゲームスタート演出処理
				void	motionEndSwitch(CINPUT* input);		//!< ゲーム終了演出処理
				void	motionDemo(CINPUT* input);			//!< デモ移行演出処理

				void	renderInitTitle(void);				//!< タイトル画面移行演出描画
				void	renderTitle(void);					//!< タイトル画面描画
				void	renderStartSwitch(void);			//!< ゲームスタート演出描画
				void	renderEndSwitch(void);				//!< ゲーム終了演出描画
				void	renderDemo(void);					//!< デモ移行演出描画

				void	initInitTitle(void);				//!< タイトル画面移行演出命令
				void	initTitle(void);					//!< タイトル画面命令
				void	initStartSwitch(void);				//!< ゲームスタート演出命令
				void	initEndSwitch(void);				//!< ゲーム終了演出命令
				void	initDemo(void);						//!< デモ移行演出命令

			//==================================================================
			//	サウンドデータ
			//==================================================================
				SSTATIC*	sStaticCursor;					//!< カーソル音
				SSTATIC*	sStaticDecision;				//!< 決定音
				SSTREAMING*	sStreamingBgm;					//!< 背景音

			//==================================================================
			//	サウンド操作
			//==================================================================
				void	soundCursor(void){
					if(sStaticCursor)
					{
						this->sStaticCursor->setPlayCur(0);
						this->sStaticCursor->play(0, NULL);
					}
				}
				void	soundDecision(void){
					if(sStaticDecision)
					{
						this->sStaticDecision->setPlayCur(0);
						this->sStaticDecision->play(0, NULL);
					}
				}

			//==================================================================
			//	演出属性
			//==================================================================
				CTEXTURE*	titleBack;						//!< タイトル背景
				CTEXTURE*	titleTex;						//!< タイトル
				CTEXTURE*	start;							//!< 「START」
				CTEXTURE*	option;							//!< 「OPTION」
				CTEXTURE*	end;							//!< 「END」
				CTEXTURE*	cursor;							//!< カーソル
				CFONT_BASE*	font;							//!< フォントベース
				int			iStack;							//!< スタックカウンタ
				BOOL		bStack;							//!< スタック確認
				int			iTimeCnt;						//!< タイムカウンタ

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	環境クラス
			//==================================================================
				CGRAPHIC_DEVICE*	gDev;					//!< グラフィックデバイス
				C3D_EFFECT_CENTER*	effect;					//!< エフェクトセンター

			//==================================================================
			//	フラグ群
			//==================================================================
				STITLE_FLAG	titleFlag;						//!< 分岐フラグ
				CURSOR_POS	cursorPos;						//!< カーソル位置
				SCENE_FLAG	startFlag;						//!< スタート移行フラグ
				SCENE_FLAG	endFlag;						//!< エンド移行フラグ
				SCENE_FLAG	loopFlag;						//!< ループフラグ(放って置くと移行する)

	public:			//-------------------- public --------------------//

				CSCENE_TITLE(SCENE_SHARE_DATA* inData);
		virtual	~CSCENE_TITLE(void);

				void	sceneInitialize(int inStageNum);	//!< 初期化
				void	sceneFinishalize(void);				//!< 開放

				SCENE_FLAG	sceneMain(CINPUT* input);		//!< シーンメイン
	};
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------