//***********************************************************************************
//!	ファイル名		Class_SceneMovieBase.h
//
//	機能			ムービースタックを管理するクラスの定義
//	機能概要		デモやムービーに使われるクラス
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2008/12/01	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneMovieBase.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_MOVIE_BASE::コンストラクタ
	//
	//	引数		inFlag			現在のシーンフラグ
	//				inNextFlag		次のシーンフラグ
	//				inData			シーン共有データ
	//	更新		2008/12/02		<新規>
	//================================================================================
	CSCENE_MOVIE_BASE::CSCENE_MOVIE_BASE(SCENE_FLAG inFlag,
										 SCENE_FLAG inNextFlag,
										 SCENE_SHARE_DATA* inData)	:
	CSCENE(inFlag, inNextFlag, inData)
	{
	//	環境属性の初期化
		this->gDev			= CGRAPHIC_DEVICE::getInstance();
		this->effectCenter	= C3D_EFFECT_CENTER::getInstance();
		this->effectCenter->finishalize();
		this->font			= CFONT_BASE::getInstance();
		this->direction		= new DX::CDIRECTION(gDev->getDevice(),
										 gDev->getScreenSizeX(),
										 gDev->getScreenSizeY());
	//	フォントの設定
		this->font->createFont(gDev->getDevice(), eFONT_PTN2, "HG創英角ﾎﾟｯﾌﾟ体", 20, 10, 1, 0);
	//	シネマスコープの設定
		this->direction->initCinemaScopePaintOut(
			CINEMASCOPE_TOP, CINEMASCOPE_BOTTOM, 0.1f, 0xFF000000); 

	//	演出属性の初期化
		this->movieSpeed	= eMSPEED_NORMAL;
		this->iMovieFrame	= 0;
		this->iFinishFrame	= 0;
		this->bEndMovie		= FALSE;
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE_BASE::デストラクタ
	//
	//	更新		2008/12/02		<新規>
	//================================================================================
	CSCENE_MOVIE_BASE::~CSCENE_MOVIE_BASE(void)
	{
	//	解放
		this->finishalizeMovie();
	
	//	環境属性の解放
		this->effectCenter->finishalize();
		this->font	= NULL;
		CFONT_BASE::releaseInstance();
		SAFE_DELETE(direction);
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE_BASE::initializeMovie
	//
	//	機能		ムービーの準備
	//	引数		inFileNum		外部ファイル番号
	//	更新		2009/01/26		<新規>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::initializeMovie(int inFileNum)
	{
	//	読み込み処理
		FILE*	fp;								//!< ファイルポインター
		char	szString[MAX_PATH]		= "";	//!< 多様文字列

	//	テキスト名を生成
		sprintf_s(szString, sizeof(szString), PATH_MOVIEFILE, inFileNum);
	//	ファイルを開く
		fopen_s(&fp, szString, "r");

	//	ファイルを開けなかった時
		if(	!fp)
		{
		//	読み込みを行わず、処理を返す
			return;
		}

	//	初期解放
		finishalizeMovie();

	//	字幕データを読み込む
		MOVIE_TELOP_DATA*	newTelop = NULL;
		int	inStart  = 0;
		int	inEnd    = 0;
		int	r = 0, g = 0, b = 0, a = 0;

		do
		{
		//	開始フレームと終了フレームを読み込む
			fscanf_s(fp, "%d, %d, ", &inStart, &inEnd);

		//	開始フレームと終了フレームの値が一致したら、字幕データの読み込みを終了
			if(	inStart == inEnd)	break;

		//	ここまで来たら、新たな字幕データを生成
			newTelop = new MOVIE_TELOP_DATA();
			newTelop->iStart = inStart;
			newTelop->iEnd   = inEnd;

		//	描画文字列データを読み込み、設定する
			fscanf_s(fp, "%s ", szString, sizeof(szString));
			UT::uf_StrCpy1(newTelop->telop.szWord, (int)sizeof(newTelop->telop.szWord),
						   szString, (int)sizeof(szString));
			fscanf_s(fp, "%d, %d, %d, %d, ", &r, &g, &b, &a);
			newTelop->telop.font				= eFONT_PTN2;
			newTelop->telop.color				= D3DCOLOR_RGBA(r,g,b,a);
			SetRect(&newTelop->telop.rcPos,
				0, gDev->getScreenSizeY() - CINEMASCOPE_BOTTOM,
				gDev->getScreenSizeX(), gDev->getScreenSizeY());
			newTelop->telop.fPriority			= 0.05f;
			newTelop->telop.iTextCnt			= 0;
			newTelop->telop.dFormat				= DT_CENTER;
			newTelop->telop.iTextGraduallySpeed	= 1;

		//	サウンドデータを読み込み、設定する
			fscanf_s(fp, "%s ", szString, sizeof(szString));
			if(	strcmp(szString, "S_NONE"))		//!< ダミーサウンド名以外なら生成
			{
				newTelop->voice	= new SSTATIC(szString);
			}
			else								//!< 上記以外ならNULL化
			{
				newTelop->voice	= NULL;
			}

		//	生成されたデータを登録
			telop.push_back(newTelop);
		}
		while(1);

	//	即時処理データを読み込む
		MOVIE_PROC_DATA_ONCE	newProcOnce;
		do
		{
		//	即時処理フレームを読み込む
			fscanf_s(fp, "%d, ", &newProcOnce.iFrame);

		//	読み込んだ即時処理フレームの値が負の数なら、即時処理データの読み込みを終了
			if(	newProcOnce.iFrame < 0)	break;

		//	残りを読み込む
			fscanf_s(fp, "%d, ", &newProcOnce.iProc);

		//	生成されたデータを登録
			procOnce.push_back(newProcOnce);
		}
		while(1);

	//	処理データを読み込む
		MOVIE_PROC_DATA		newProc;
		do
		{
		//	処理開始フレームを読み込む
			fscanf_s(fp, "%d, ", &newProc.iStart);

		//	読み込んだ処理開始フレームの値が負の数なら、処理開始データの読み込みを終了
			if(	newProc.iStart < 0)	break;

		//	残りを読み込む
			fscanf_s(fp, "%d, ", &newProc.iEnd);
			fscanf_s(fp, "%d, ", &newProc.iProc);

		//	生成されたデータを登録
			proc.push_back(newProc);
		}
		while(1);

	//	ムービー終了フレームを読み込む
		fscanf_s(fp, "%d, ", &iFinishFrame);

	//	ファイルを閉じる
		fclose(fp);

	//	各データのイテレータを確保
		telopI		= telop.begin();
		procOnceI	= procOnce.begin();
		procI		= proc.begin();
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE_BASE::finishalizeMovie
	//
	//	機能		ムービーの解放
	//	更新		2009/01/26		<新規>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::finishalizeMovie(void)
	{
	//	演出属性の解放
		SAFE_VECTOR_DELETE(telop, MOVIE_TELOP_DATA*);
		procOnce.clear();
		proc.clear();

		iMovieFrame		= 0;
		iFinishFrame	= 0;
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE_BASE::movieStartProc
	//
	//	機能		ムービーの開始
	//	更新		2009/02/07		<新規>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::movieStartProc(void)
	{
	//	幕が消滅する
		this->gDev->setCurtainColor(0, 0, 0);
		this->gDev->curtainMotion(FALSE, 10);
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE_BASE::movieEndProc
	//
	//	機能		ムービー終了準備
	//	更新		2009/02/07		<新規>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::movieEndProc(void)
	{
		if(	!this->bEndMovie)
		{
		//	幕が出現する
			this->gDev->curtainMotion(TRUE, 5);
		//	ムービー終了確認を有効
			this->bEndMovie	= TRUE;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE_BASE::movieEndCheck
	//
	//	機能		ムービー終了チェック
	//	更新		2009/02/07		<新規>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::movieEndCheck(void)
	{
	//	ムービー終了確認が有効で、
	//	幕のアルファが最大なら、次の処理へ
		if(	this->bEndMovie	&&
			(this->gDev->getCurtainColorAlpha() >= 255))
		{
			this->returnFlag	= this->nextFlag;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE_BASE::executeDrawTelop
	//
	//	機能		字幕の描画処理
	//	更新		2009/01/26		<新規>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::executeDrawTelop(void)
	{
#ifdef	_DEBUG
		char	szString[20]	= "";
		sprintf_s(szString, sizeof(szString), "%d flame", this->iMovieFrame);
		font->drawText(gDev->getSprite(), eFONT_PTN2, szString, 0, 0, 0xFFFFFFFF);
#else
		char	szString1[30]	= "ｽﾀｰﾄﾎﾞﾀﾝ : ﾑｰﾋﾞｰ ｼｭｳﾘｮｳ";
		char	szString2[30]	= "ｼﾀﾎﾞﾀﾝ   : 3ﾊﾞｲ ﾊﾔｵｸﾘ";
		font->drawText(gDev->getSprite(), eFONT_PTN2, szString1, 0,  0, 0xFFFFFFFF);
		font->drawText(gDev->getSprite(), eFONT_PTN2, szString2, 0, 25, 0xFFFFFFFF);
#endif

	//	現在の字幕データのイテレータが終了を指しているなら、処理しない
		if(	telopI == telop.end())	return;

	//	現在の字幕データが描画されるフレーム区間内に満たしているか確認
		if(	(*telopI)->iStart <= iMovieFrame)
		{
			if(	(*telopI)->iEnd >= iMovieFrame)
			{
			//	ムービー処理速度にあわせて、徐々描画処理を行う
				LOOP(this->movieSpeed - 1)
				{
					(*telopI)->telop.step();
				}
			//	満たしているなら描画
				font->drawText(gDev->getSprite(), &(*telopI)->telop);
			//	音声データがあり、再生済みで無いなら、再生させる
				if(	 (*telopI)->voice	&&
					!(*telopI)->bVoice)
				{
					(*telopI)->voice->play(0, NULL);
					(*telopI)->bVoice	= TRUE;
				}
			}
		//	現在の字幕データが描画終了した時
			else
			{
			//	次の字幕データへ
				telopI++;
			}
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE_BASE::checkMovieInput
	//
	//	機能		即時処理データの更新処理
	//	引数		input			入力情報
	//	更新		2009/02/15		<新規>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::checkMovieInput(CINPUT* input)
	{
	//	右手方の下ボタンが押されてたら3倍速
		if(	input->checkPress(MYKEY::eMIDDLE))
		{
			if(	this->movieSpeed != eMSPEED_3DIAMETER)
			{
				this->movieSpeed  = eMSPEED_3DIAMETER;
				this->executeInstantChengeMovieSpeed();
			}
		}
	#ifdef	_DEBUG
	//	右手方の左ボタンが押されてたら5倍速
		else
		if(	input->checkPress(MYKEY::eDUST))
		{
			if(	this->movieSpeed != eMSPEED_5DIAMETER)
			{
				this->movieSpeed  = eMSPEED_5DIAMETER;
				this->executeInstantChengeMovieSpeed();
			}
		}
	//	右手方の右ボタンが押されてたら10倍速
		else
		if(	input->checkPress(MYKEY::eSOFT))
		{
			if(	this->movieSpeed != eMSPEED_10DIAMETER)
			{
				this->movieSpeed  = eMSPEED_10DIAMETER;
				this->executeInstantChengeMovieSpeed();
			}
		}
	//	右手方の上ボタンが押されてたら15倍速
		else
		if(	input->checkPress(MYKEY::eHARD))
		{
			if(	this->movieSpeed != eMSPEED_20DIAMETER)
			{
				this->movieSpeed  = eMSPEED_20DIAMETER;
				this->executeInstantChengeMovieSpeed();
			}
		}
	//	右手方の上ボタンが押されてたら0倍速
		else
		if(	input->checkPress(MYKEY::eSELECT))
		{
			if(	this->movieSpeed != eMSPEED_STOP)
			{
				this->movieSpeed  = eMSPEED_STOP;
				this->executeInstantChengeMovieSpeed();
			}
		}
	#endif
	//	何も押されてなければ、通常速度
		else
		{
			if(	this->movieSpeed != eMSPEED_NORMAL)
			{
				this->movieSpeed  = eMSPEED_NORMAL;
				this->executeInstantChengeMovieSpeed();
			}
		}
	//	スタートが押されていたら、ムービー終了準備へ
		if(	input->checkPressOnce(MYKEY::eSTART))
		{
			this->movieEndProc();
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE_BASE::checkProcOnce
	//
	//	機能		即時処理データの更新処理
	//	更新		2009/01/26		<新規>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::checkProcOnce(void)
	{
	//	現在の即時処理データのイテレータが終了を指しているなら、処理しない
		if(	procOnceI == procOnce.end())	return;

	//	現在の即時処理データが実行されるフレーム区間内に満たしているか確認
		if(	procOnceI->iFrame <= iMovieFrame)
		{
		//	即時処理実行
			executeProcOnce(procOnceI->iProc);
		//	次の即時処理データへ
			procOnceI++;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE_BASE::checkProc
	//
	//	機能		処理データの更新処理
	//	更新		2009/01/26		<新規>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::checkProc(void)
	{
	//	現在の処理データのイテレータが終了を指しているなら、処理しない
		if(	procI == proc.end())	return;

	//	現在の処理データが実行されるフレーム区間内に満たしているか確認
		if(	procI->iStart <= iMovieFrame)
		{
			if(	procI->iEnd >= iMovieFrame)
			{
			//	満たしているなら実行
				executeProc(procI->iProc);
			}
		//	現在の処理データが実行終了した時
			if(	procI->iEnd <= iMovieFrame)
			{
			//	次の字幕データへ
				procI++;
			}
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_MOVIE_BASE::checkProcFollowMovieSpeed
	//
	//	機能		速度に従い両処理データの更新
	//	更新		2009/02/15		<新規>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::checkProcFollowMovieSpeed(void)
	{
	//	ムービー処理速度だけループ
		LOOP(this->movieSpeed)
		{
			this->checkProcOnce();
			this->checkProc();
			this->iMovieFrame++;
			if(	i > 0){	this->gDev->curtainMotionExecute();	}
		}
	//	現在のフレームが終了フレームに到達したら、ムービー終了準備へ
		if(	iMovieFrame >= iFinishFrame)
		{
			this->movieEndProc();
		}
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------