//***********************************************************************************
//!	ファイル名		Class_SceneStage2.h
//
//	機能			ゲームシーンクラス、ステージ2「公園出口」を定義
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
	//!	クラス名		CSCENE_STAGE2
	//
	//	機能			ゲームシーンクラス、ステージ2「公園出口」
	//	機能概要		アドベンチャーゲームクラスを継承している
	//					ステージ2はアドベンチャーゲームモード
	//	更新			2009/01/27	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_STAGE2 : public CSCENE_ADVENTURE_GAME
	{
	private:		//-------------------- private --------------------//

	public:			//-------------------- public --------------------//

				CSCENE_STAGE2(SCENE_SHARE_DATA* inData);		//!< コンストラクタ
		virtual	~CSCENE_STAGE2(void);							//!< デストラクタ

		virtual	void		sceneInitialize(int);				//!< 初期化
		virtual	void		sceneFinishalize(void);				//!< 解放

		virtual	SCENE_FLAG	sceneMain(CINPUT* input);			//!< シーンメイン

			//==================================================================
			//	ステージ2専用処理
			//==================================================================
				void		updateExecute(void);
	};
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------