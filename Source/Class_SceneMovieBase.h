//***********************************************************************************
//!	ファイル名		Class_SceneMovieBase.h
//
//	機能			ムービースタックを管理するクラスの定義
//	機能概要		デモやムービーに使われるクラス
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2008/12/01	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_Scene.h"					//!< シーンクラス
#include	"Class_GraphicDevice.h"			//!< グラフィックデバイス
#include	"Class_FontBase.h"				//!< フォント
#include	"Class_Direction.h"				//!< DirectX演出クラス
#include	"Class_Sound_OggVorbis.h"		//!< oggライブラリ(サウンドを発生させる為)

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	#define	PATH_MOVIEFILE		"..//Resources//Data//MovieBase//MovieFile%d.mdat"
	#define	CINEMASCOPE_TOP		50		//!	シネマスコープの上辺幅
	#define	CINEMASCOPE_BOTTOM	80		//!	シネマスコープの下辺幅

	//----------------
	//!	ムービー処理速度フラグ
	enum	MOVIE_SPEED
	{
		eMSPEED_STOP		= 0,
		eMSPEED_NORMAL		= 1,
		eMSPEED_3DIAMETER	= 3,
		eMSPEED_5DIAMETER	= 5,
		eMSPEED_10DIAMETER	= 10,
		eMSPEED_20DIAMETER	= 20,
	};

	//--------------------
	//!	字幕データの構造体
	struct	MOVIE_TELOP_DATA
	{
		int			iStart;		//!< 開始フレーム
		int			iEnd;		//!< 終了フレーム
		FONT_GRASTR	telop;		//!< 描画文字列データ
		SSTATIC*	voice;		//!< 音声データ
		BOOL		bVoice;		//!< 音声データ再生確認

		MOVIE_TELOP_DATA(void)
		{
			iStart	= 0;
			iEnd	= 0;
			voice	= NULL;
			bVoice	= FALSE;
		}
		~MOVIE_TELOP_DATA(void)
		{
			SAFE_DELETE(voice);
		}
	};

	//------------------------
	//!	即時処理データの構造体
	struct	MOVIE_PROC_DATA_ONCE
	{
		int	iFrame;				//!< 即時処理フレーム
		int	iProc;				//!< 即時処理番号

		MOVIE_PROC_DATA_ONCE(void)
		{
			iFrame	= 0;
			iProc	= 0;
		}
	};

	//--------------------
	//!	処理データの構造体
	struct	MOVIE_PROC_DATA
	{
		int	iStart;				//!< 処理開始フレーム
		int	iEnd;				//!< 処理終了フレーム
		int	iProc;				//!< 処理番号

		MOVIE_PROC_DATA(void)
		{
			iStart	= 0;
			iEnd	= 0;
			iProc	= 0;
		}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CSCENE_MOVIE_BASE
	//
	//	機能			ムービー基盤クラス
	//	更新			2008/12/01	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_MOVIE_BASE : public CSCENE
	{
	private:		//-------------------- private --------------------//

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	環境属性
			//==================================================================
				CGRAPHIC_DEVICE*	gDev;				//!< グラフィックデバイス
				C3D_EFFECT_CENTER*	effectCenter;		//!< エフェクトセンター
				CFONT_BASE*			font;				//!< フォントベース
				DX::CDIRECTION*		direction;			//!< DirectX演出

			//==================================================================
			//	演出属性
			//==================================================================
				vector<MOVIE_TELOP_DATA*>				telop;		//!< 字幕データ
				vector<MOVIE_TELOP_DATA*>::iterator		telopI;		//!< 字幕データイテレータ
				vector<MOVIE_PROC_DATA_ONCE>			procOnce;	//!< 即時処理データ
				vector<MOVIE_PROC_DATA_ONCE>::iterator	procOnceI;	//!< 即時処理データイテレータ
				vector<MOVIE_PROC_DATA>					proc;		//!< 処理データ
				vector<MOVIE_PROC_DATA>::iterator		procI;		//!< 処理データイテレータ
				MOVIE_SPEED		movieSpeed;							//!< ムービー処理速度フラグ
				int				iMovieFrame;						//!< ムービー処理フレーム
				int				iFinishFrame;						//!< ムービー終了フレーム
				BOOL			bEndMovie;							//!< ムービー終了確認

			//==================================================================
			//	環境操作
			//==================================================================
				void	initializeMovie(int inFileNum);				//!< ムービー初期化
				void	finishalizeMovie(void);						//!< ムービー解放

				void	movieStartProc(void);						//!< ムービー開始
				void	movieEndProc(void);							//!< ムービー終了準備
				void	movieEndCheck(void);						//!< ムービー終了チェック

				void	executeDrawTelop(void);						//!< 字幕の描画処理
				void	checkMovieInput(CINPUT* input);				//!< ムービー処理速度の更新処理
				void	checkProcOnce(void);						//!< 即時処理データの更新処理
				void	checkProc(void);							//!< 処理データの更新処理
				void	checkProcFollowMovieSpeed(void);			//!< 速度に従い両処理データの更新

		virtual	void	executeInstantChengeMovieSpeed(void){}		//!< ムービー処理速度変更時処理
		virtual	void	executeProcOnce(int inProc)			{}		//!< 即時処理実行
		virtual	void	executeProc(int inProc)				{}		//!< 処理実行

	public:			//-------------------- public --------------------//

				CSCENE_MOVIE_BASE(SCENE_FLAG inFlag, SCENE_FLAG inNextFlag, SCENE_SHARE_DATA* inData);
		virtual	~CSCENE_MOVIE_BASE(void);
	};
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------