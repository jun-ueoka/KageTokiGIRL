//***********************************************************************************
//!	ファイル名		Class_SceneMovie3.h
//
//	機能			ムービーシーンクラス、ムービー3[シャドウクリミナルアジト]の定義
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/02/10	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"GameCommon.h"					//!< ゲームコモン
#include	"Class_SceneMovieBase.h"		//!< イベントシーン
#include	"Class_Sound_OggVorbis.h"		//!< oggライブラリ(サウンドを発生させる為)

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	描画シーン切り替えフラグ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum MOVIE3_FLAG
	{
		eM3F_FOREST,			//!< 森
		eM3F_CAVE,				//!< 洞窟
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CSCENE_MOVIE3
	//
	//	機能			ムービーシーンクラス、ムービー3[シャドウクリミナルアジト]のクラス
	//	更新			2009/02/10	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_MOVIE3 : public CSCENE_MOVIE_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	演出属性
			//==================================================================
				C3D_OBJECT_CAMERA*	camera;						//!< カメラ
				C3D_OBJECT*			cameraTarget;				//!< カメラターゲット用オブジェクト

				CSKIN_MESH*			kageo;						//!< カゲオ
				CSKIN_MESH*			kagemi;						//!< カゲミ
				CSKIN_MESH*			shabird;					//!< シャバード
				CSKIN_MESH*			clime;						//!< クライム
				CSKIN_MESH*			tima;						//!< ティマ
				CSKIN_MESH*			feather;					//!< フェザー
				CSKIN_MESH*			yamakage;					//!< ヤマカゲ
				CSKIN_MESH*			exe;						//!< エクス

				CGAME_MAP*			objMapForest;				//!< オブジェクトマップ森
				CGAME_MAP*			objMapCave;					//!< オブジェクトマップ洞窟
				CBACK*				back;						//!< 背景
				CMESH*				cave;						//!< 洞窟

				MOVIE3_FLAG			movieFlag;					//!< ムービーフラグ

				SSTREAMING*			backSound;					//!< ステージBGM

				CZTEX_SHA*			shaderZTexture;				//!< Zテクスチャシェーダ
				CDEPTH_SHA*			shaderDepthBuf;				//!< 深度バッファシェーダ

				MAT					lightView;					//!< ライトビュー行列
				MAT					lightProj;					//!< ライト射影行列
				VEC3				lightShift;					//!< 視点からのライトビューの位置
				VEC2				lightNearFar;				//!< ライトの遠視近視

	protected:		//-------------------- protected --------------------//

				void	executeInstantChengeMovieSpeed(void);	//!< ムービー処理速度変更時処理
				void	executeProcOnce(int inProc);			//!< 即時処理実行
				void	executeProc(int inProc);				//!< 処理実行
				void	updateLight(void);						//!< ライトの更新

	public:			//-------------------- public --------------------//

				CSCENE_MOVIE3(SCENE_SHARE_DATA* inData);
		virtual	~CSCENE_MOVIE3(void);

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