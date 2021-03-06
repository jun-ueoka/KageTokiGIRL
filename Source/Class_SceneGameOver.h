//***********************************************************************************
//!	ファイル名		Class_SceneGameOver.h
//
//	機能			ゲームオーバーシーンの定義
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/02/05	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"GameCommon.h"					//!< ゲームコモン
#include	"Class_Scene.h"					//!< シーン
#include	"Class_Sound_OggVorbis.h"		//!< oggライブラリ(サウンドを発生させる為)
#include	"Class_GraphicDevice.h"			//!< グラフィックデバイス
#include	"Class_FontBase.h"				//!< フォントベース

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CSCENE_GAMEOVER
	//
	//	機能			ゲームオーバーシーンのクラス
	//	更新			2009/02/05	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_GAMEOVER : public CSCENE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	環境属性
			//==================================================================
				CGRAPHIC_DEVICE*	gDev;						//!< グラフィックデバイス
				CFONT_BASE*			font;						//!< フォントベース

			//==================================================================
			//	演出属性
			//==================================================================
				SSTREAMING*			sStreamingBgm;				//!< BGM
				CTEXTURE*			texGameOver;				//!< ゲームオーバーテクスチャ
				int					iFrameCnt;					//!< フレームカウンタ

	public:			//-------------------- public --------------------//

				CSCENE_GAMEOVER(SCENE_SHARE_DATA* inData);
		virtual	~CSCENE_GAMEOVER(void);

				void		sceneInitialize(int inStageNum);	//!< 初期化
				void		sceneFinishalize(void);				//!< 開放

				SCENE_FLAG	sceneMain(CINPUT* input);			//!< シーンメイン

				void		execute(CINPUT* input);				//!< 処理
				void		render(void);						//!< 描画
	};
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------