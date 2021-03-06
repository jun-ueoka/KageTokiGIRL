//***********************************************************************************
//!	ファイル名		Class_SceneStage6.h
//
//	機能			ゲームシーンクラス、ステージ6「洞窟」を定義
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/01/27	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_SceneAdventureGame.h"		//!< ゲームシーン

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CSCENE_STAGE6
	//
	//	機能			ゲームシーンクラス、ステージ6「洞窟」
	//	機能概要		アドベンチャーゲームクラスを継承している
	//					ステージ6はアドベンチャーゲームモード
	//	更新			2009/01/27	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_STAGE6 : public CSCENE_ADVENTURE_GAME
	{
	private:		//-------------------- private --------------------//

	public:			//-------------------- public --------------------//

				CSCENE_STAGE6(SCENE_SHARE_DATA* inData);		//!< コンストラクタ
		virtual	~CSCENE_STAGE6(void);							//!< デストラクタ

		virtual	void		sceneInitialize(int);				//!< 初期化
		virtual	void		sceneFinishalize(void);				//!< 解放

		virtual	SCENE_FLAG	sceneMain(CINPUT* input);			//!< シーンメイン

			//==================================================================
			//	ステージ6専用処理
			//==================================================================
				void		updateExecute(void);
	};
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------