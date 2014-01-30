//***********************************************************************************
//!	ファイル名		Class_SceneBoss2.h
//
//	機能			ゲームシーンクラス、ボスステージ2「ヤマカゲ」を定義
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2009/02/15	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_SceneAdventureGame.h"		//!< ゲームシーン
#include	"Class_BossYamakage.h"				//!< ボスクラス「ヤマカゲ」

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CSCENE_BOSS2
	//
	//	機能			ゲームシーンクラス、ボスステージ2「ヤマカゲ」
	//	機能概要		ボスステージを構成する
	//	更新			2009/02/15	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_BOSS2 : public CSCENE_ADVENTURE_GAME
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	ステージステータス
			//==================================================================
				int				eventStepCnt;					//!< イベントステップカウンタ
				CBOSS_YAMAKAGE*	bossYamakage;					//!< 「ヤマカゲ」クラス

	public:			//-------------------- public --------------------//

				CSCENE_BOSS2(SCENE_SHARE_DATA* inData);			//!< コンストラクタ
		virtual	~CSCENE_BOSS2(void);							//!< デストラクタ

				void		sceneInitialize(int);				//!< 初期化
				void		sceneFinishalize(void);				//!< 解放

				SCENE_FLAG	sceneMain(CINPUT* input);			//!< シーンメイン

			//==================================================================
			//	ボスステージ2専用処理
			//==================================================================
				void	updateExecute(void);

			//==================================================================
			//	敵生成(オーバーライト)
			//==================================================================
				void	sgCreateEnemy(int inStageNum);
	};
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------