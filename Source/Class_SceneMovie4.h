//***********************************************************************************
//!	ファイル名		Class_SceneMovie4.h
//
//	機能			ムービーシーンクラス、ムービー4[敵を追って…]の定義
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/02/17	<新規>
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
	//!	クラス名		CSCENE_MOVIE4
	//
	//	機能			ムービーシーンクラス、ムービー4[敵を追って…]のクラス
	//	更新			2009/02/17	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_MOVIE4 : public CSCENE_MOVIE_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	演出属性
			//==================================================================
				C3D_OBJECT_CAMERA*	camera;						//!< カメラ
				CSKIN_MESH*			kageo;						//!< カゲオ
				CSKIN_MESH*			kagemi;						//!< カゲミ
				CGAME_MAP*			objMap;						//!< オブジェクトマップ
				CTEXTURE*			sky;						//!< 空

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

				CSCENE_MOVIE4(SCENE_SHARE_DATA* inData);
		virtual	~CSCENE_MOVIE4(void);

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