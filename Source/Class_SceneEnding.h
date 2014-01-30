//***********************************************************************************
//!	ファイル名		Class_SceneEnding.h
//
//	機能			エンディングシーンの定義
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2008/10/05	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"GameCommon.h"					//!< ゲームコモン
#include	"Class_SceneMovieBase.h"		//!< シーン
#include	"Class_Sound_OggVorbis.h"		//!< oggライブラリ(サウンドを発生させる為)

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CSCENE_ENDING
	//
	//	機能			エンディングシーンのクラス
	//	更新			2008/10/05	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_ENDING : public CSCENE_MOVIE_BASE
	{
			//==================================================================
			//	マクロ群
			//==================================================================
				#define	ROLL_SPEED			2					//!< ロールスピード

	private:		//-------------------- private --------------------//

			//==================================================================
			//	演出属性
			//==================================================================
				C3D_OBJECT_CAMERA*	camera;						//!< カメラ
				CSKIN_MESH*			kageo;						//!< カゲオ
				CSKIN_MESH*			kagemi;						//!< カゲミ
				CGAME_MAP*			drawMap;					//!< 描画マップ
				CGAME_MAP*			objMap1;					//!< オブジェクトマップ1
				CGAME_MAP*			objMap2;					//!< オブジェクトマップ2
				CGAME_MAP*			objMap3;					//!< オブジェクトマップ3
				CGAME_MAP*			objMap4;					//!< オブジェクトマップ4
				CBACK*				drawBack;					//!< 描画背景
				CBACK*				back1;						//!< 背景1

				MY_COLOR_RGBA		curtainColor;				//!< 幕の色
				SSTREAMING*			sStreamingBgm;				//!< BGM

				CZTEX_SHA*			shaderZTexture;				//!< Zテクスチャシェーダ
				CDEPTH_SHA*			shaderDepthBuf;				//!< 深度バッファシェーダ

				MAT					lightView;					//!< ライトビュー行列
				MAT					lightProj;					//!< ライト射影行列
				VEC3				lightShift;					//!< 視点からのライトビューの位置
				VEC2				lightNearFar;				//!< ライトの遠視近視

			//==================================================================
			//	スタッフロール関連
			//==================================================================
				int						iRoll;					//!< ロール値
				vector<FONT_STRING>		staffRoll;				//!< ロール文字列
				void	motionStaffRoll(void);					//!< スタッフロール画面処理
				void	renderStaffRoll(void);					//!< スタッフロール画面描画

	protected:		//-------------------- protected --------------------//

				void	executeInstantChengeMovieSpeed(void);	//!< ムービー処理速度変更時処理
				void	executeProcOnce(int inProc);			//!< 即時処理実行
				void	executeProc(int inProc);				//!< 処理実行
				void	updateLight(void);						//!< ライトの更新

	public:			//-------------------- public --------------------//

				CSCENE_ENDING(SCENE_SHARE_DATA* inData);
		virtual	~CSCENE_ENDING(void);

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