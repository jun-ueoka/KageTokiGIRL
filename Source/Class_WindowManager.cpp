//***********************************************************************************
//!	ファイル名		Class_WindowMamager.cpp
//
//	機能			ウィンドウを生成する時の処理の実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/04/16	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_WindowManager.h"
//#include	"UtilityFunction.h"
//#include	"resource.h"

//-----------------------------------------------------------------------
//	ウィンドウポロック
//-----------------------------------------------------------------------
extern	LRESULT CALLBACK WndProc(HWND inWnd, UINT inMessage, WPARAM inWParam, LPARAM inLParam);

//-----------------------------------------------------------------------
//!	ウィンドウマネージャの名前空間
//-----------------------------------------------------------------------
namespace WM
{
	//================================================================================
	//	インスタンス生成場所
	//================================================================================
	CWINDOW_MANAGER*	CWINDOW_MANAGER::gUniqueInstance = NULL;

	//================================================================================
	//!	メソッド名	CWINDOW_MANAGER::コンストラクタ
	//
	//	引数		inInstance		プログラムに割り当てられたインスタンスハンドル
	//				inShowCmd		ウィンドウ表示方法
	//	更新		2008/04/16		<新規>
	//================================================================================
	CWINDOW_MANAGER::CWINDOW_MANAGER(HINSTANCE inInstance, int inShowCmd)
	{
	//!	各メンバ初期化
		initialize();

	//	スクリーンモードの設定
		int	iCheck	= 0;
	//	デバッグモードの場合は、表示形式を選択する
		iCheck	= MessageBox(
					hWnd,
					"フルスクリーンにしますか？",
					"スクリーン設定",
					MB_YESNOCANCEL);
	//	選択結果を判定
		switch(iCheck)
		{
		//	「はい」
			case	IDYES:		bFullScreen	= TRUE;				break;
		//	「いいえ」
			case	IDNO:		bFullScreen	= FALSE;			break;
		//	上記以外
			default:			bGenerationFailure	= TRUE;		return;
		}

	//!	プログラムが起動していない
		if(	!findOperationWindow())
		{
			bGenerationFailure	= TRUE;
		//!	内部生成
		//	ウィンドウクラスの登録に失敗
			if(	!registerClass(inInstance))
			{
			//!	エラー報告
				MessageBox(hWnd, "ウィンドウの登録に失敗", "エラー", MB_OK);
			//!	プログラム終了へ
				SendMessage(hWnd, WM_DESTROY, 0, 0);
				return;
			}
		//	ウィンドウの生成に失敗
			if(	!generationWindow(inInstance))
			{
			//!	エラー報告
				MessageBox(hWnd, "ウィンドウの生成に失敗", "エラー", MB_OK);
			//!	プログラム終了へ
				SendMessage(hWnd, WM_DESTROY, 0, 0);
				return;
			}
		//	アプリケーション生成に失敗
			if(	!initApplication(inInstance))
			{
			//!	エラー報告
				MessageBox(hWnd, "アプリケーションの初期化に失敗", "エラー", MB_OK);
			//!	プログラム終了へ
				SendMessage(hWnd, WM_DESTROY, 0, 0);
				return;
			}
		//!	ここまで来たら、正常
			bGenerationFailure	= FALSE;
		//!	ウィンドウを更新しループの準備
			initLoop(inShowCmd);
		}
	//!	既にプログラムが起動している
		else
		{
		//!	起動している事を代入
			bOperation = TRUE;
		}
	}

	//================================================================================
	//!	メソッド名	CWINDOW_MANAGER::デストラクタ
	//
	//	更新		2008/04/17		<新規>
	//================================================================================
	CWINDOW_MANAGER::~CWINDOW_MANAGER(void)
	{
	//!	解放処理
		finishalize();
	}

	//================================================================================
	//!	メソッド名	CWINDOW_MANAGER::getInstance
	//
	//	機能		インスタンスの生成
	//	引数		inInstance		プログラムに割り当てられたインスタンスハンドル
	//				inShowCmd		ウィンドウ表示方法
	//	戻り値		インスタンスを返す
	//	更新		2008/04/17		<新規>
	//================================================================================
	CWINDOW_MANAGER*
	CWINDOW_MANAGER::getInstance(HINSTANCE inInstance, int inShowCmd)
	{
	//!	生成されていなければ、生成する
		if(	!gUniqueInstance)
		{
		//!	インスタンス生成
			gUniqueInstance = new CWINDOW_MANAGER(inInstance, inShowCmd);
		}
	//!	インスタンスを返す
		return gUniqueInstance;
	}


	//================================================================================
	//!	メソッド名	CWINDOW_MANAGER::releaseInstance
	//
	//	機能		インスタンスの解放
	//	更新		2008/04/17		<新規>
	//================================================================================
	void
	CWINDOW_MANAGER::releaseInstance(void)
	{
	//!	インスタンスの解放
		SAFE_DELETE(gUniqueInstance);
	}

	//================================================================================
	//!	メソッド名	CWINDOW_MANAGER::initialize
	//
	//	機能		メンバの初期化を行う
	//	更新		2008/04/17		<新規>
	//================================================================================
	void
	CWINDOW_MANAGER::initialize(void)
	{
		hWnd				= NULL;					//!< ウィンドウハンドル初期化
		bGenerationFailure	= FALSE;				//!< 生成失敗確認フラグ初期化
		bFullScreen			= FALSE;				//!< フルスクリーンモード確認フラグ初期化（ウィンドウモード）
		bResetDevice		= FALSE;				//!< デバイス確認フラグを初期化
		bOperation			= FALSE;				//!< 起動状態確認フラグ初期化
		dwTime				= 0;					//!< ゲームループ用時間格納変数初期化
		dwFrames			= 0;					//!< フレーム数格納変数初期化
		dwLast				= timeGetTime();		//!< 現在の時間を格納
		app					= NULL;					//!< アプリケーション管理クラスポインタ初期化
		gDev				= NULL;					//!< グラフィックデバイスポインタ初期化
		pScreenSize.x		= kWIN_WIDTH;			//!< ウィンドウの横幅の初期化
		pScreenSize.y		= kWIN_HEIGHT;			//!< ウィンドウの縦幅の初期化
		my_sprintf(szFps, sizeof(szFps), "\0");		//!< FPS表示文字列初期化
	}

	//================================================================================
	//!	メソッド名	CWINDOW_MANAGER::finishalize
	//
	//	機能		解放処理
	//	更新		2008/04/17		<新規>
	//================================================================================
	void
	CWINDOW_MANAGER::finishalize(void)
	{
		ShowCursor(TRUE);								//!< カーソルを表示状態にする
		timeEndPeriod(1);								//!< タイマー更新時間を戻す
		GAPP::CGAME_APPLICATION	::releaseInstance();	//!< ゲーム環境解放処理
		GD	::CGRAPHIC_DEVICE	::releaseInstance();	//!< グラフィック環境の初期化
	}

	//================================================================================
	//!	メソッド名	CWINDOW_MANAGER::findOperationWindow
	//
	//	機能		このプログラムが既に起動しているか調べる
	//	戻り値		TRUE			起動している
	//				FALSE			起動していない
	//	更新		2008/04/17		<新規>
	//================================================================================
	BOOL
	CWINDOW_MANAGER::findOperationWindow(void)
	{
	//!	クラス名からウィンドウを検索
		HWND hWnd(FindWindow(kWINCLASS, NULL));
	//!	既に起動している場合
		if(	hWnd)
		{
			SetForegroundWindow(hWnd);					//!< 既に起動している方を最前面にする
			return TRUE;								//!< 起動していることを返す
		}
		return FALSE;									//!< 起動していないことを返す
	}

	//================================================================================
	//!	メソッド名	CWINDOW_MANAGER::registerClass
	//
	//	機能		ウィンドウを登録する
	//	引数		inInstance		インスタンスハンドル
	//	戻り値		TRUE			成功
	//				FALSE			失敗
	//	更新		2008/04/17		<新規>
	//================================================================================
	BOOL
	CWINDOW_MANAGER::registerClass(HINSTANCE inInstance)
	{
		WNDCLASSEX wcex;		//!< 定義用構造体

		ZeroMemory(&wcex, sizeof(WNDCLASSEX));						//!< 構造体初期化
		wcex.cbSize			= sizeof(WNDCLASSEX);					//!< WNDCLASSEX構造体のサイズ
		wcex.hInstance		= inInstance;							//!< インスタンス
		wcex.lpfnWndProc	= (WNDPROC)WndProc;						//!< ウィンドウプロシージャ
		wcex.style			= CS_HREDRAW | CS_VREDRAW;				//!< クラススタイル
		wcex.lpszMenuName	= NULL;									//!< メニュー
		wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);	//!< 背景色
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);			//!< カーソル
	//!< アイコン
		wcex.hIcon			= LoadIcon(inInstance, MAKEINTRESOURCE(ID_KAGETOKI_ICON));
		wcex.hIconSm		= NULL;									//!< 小さいアイコン
		wcex.lpszClassName	= kWINCLASS;							//!< ウィンドウクラス
		wcex.cbClsExtra		= 0;									//!< 補足メモリのブロック
		wcex.cbWndExtra		= 0;									//!< 補足メモリのサイズ

	//!	Windowsに登録
		if(	!RegisterClassEx(&wcex))
		{
			return FALSE;		//!< 失敗したことを返す
		}
		return TRUE;			//!< 成功したことを返す
	}

	//================================================================================
	//!	メソッド名	CWINDOW_MANAGER::generationWindow
	//
	//	機能		ウィンドウを生成する
	//	引数		inInstance		インスタンスハンドル
	//	戻り値		TRUE			成功
	//				FALSE			失敗
	//	更新		2008/04/17		<新規>
	//================================================================================
	BOOL
	CWINDOW_MANAGER::generationWindow(HINSTANCE inInstance)
	{
	//!	フルスクリーンモード
		if(	bFullScreen)
		{
			hWnd	=
				CreateWindowEx(
					WS_EX_APPWINDOW,				//!< 拡張ウィンドウスタイル
					kWINCLASS,						//!< ウィンドウクラス名
					kGAMETITLE,						//!< ウィンドウ名
					WS_POPUP,						//!< ウィンドウスタイル
					0,								//!< ウィンドウ位置X
					0,								//!< ウィンドウ位置Y
					pScreenSize.x,					//!< ウィンドウ幅
					pScreenSize.y,					//!< ウィンドウ高さ
					NULL,							//!< 親ウィンドウのハンドル
					NULL,							//!< メニューのハンドル
					inInstance,						//!< アプリケーションのインスタンス
					NULL);							//!< ウィンドウ作成データ
		}
	//!	ウィンドウモード
		else
		{
			hWnd	=
				CreateWindowEx(
					WS_EX_APPWINDOW,				//!< 拡張ウィンドウスタイル
					kWINCLASS,						//!< ウィンドウクラス名
					kGAMETITLE,						//!< ウィンドウ名
					WS_CAPTION | WS_SYSMENU,		//!< ウィンドウスタイル
					(GetSystemMetrics(SM_CXSCREEN) / 2) - ((pScreenSize.x	+ 4)  / 2),	//!< ウィンドウ位置X
					(GetSystemMetrics(SM_CYSCREEN) / 2) - ((pScreenSize.y	+ 29) / 2),	//!< ウィンドウ位置Y
					pScreenSize.x + 4,				//!< ウィンドウ幅
					pScreenSize.y + 29,				//!< ウィンドウ高さ
					NULL,							//!< 親ウィンドウのハンドル
					NULL,							//!< メニューのハンドル
					inInstance,						//!< アプリケーションのインスタンス
					NULL);							//!< ウィンドウ作成データ
		}

	//!	ウィンドウ生成失敗確認
		if(	!hWnd)
		{
			return FALSE;							//!< 生成失敗を返す
		}

		ImmAssociateContext(hWnd, NULL);		//!< 日本語入力を無効にする
		return TRUE;								//!< 生成成功を返す
	}

	//================================================================================
	//!	メソッド名	CWINDOW_MANAGER::initApplication
	//
	//	機能		アプリケーションの生成と初期化を行う
	//	引数		inInstance		インスタンスハンドル
	//	戻り値		TRUE			成功
	//				FALSE			失敗
	//	更新		2008/04/17		<新規>
	//				2008/04/24		<追加>	格闘情報の生成
	//================================================================================
	BOOL
	CWINDOW_MANAGER::initApplication(HINSTANCE inInstance)
	{
	//!	ゲームインスタンスを取得する
		app	= GAPP::CGAME_APPLICATION::getInstance(hWnd);
	//!	生成失敗
		if(	!app)
		{
			return	FALSE;	//!< 生成失敗を返す
		}

	//!	グラフィックデバイスを生成
		gDev	= GD::CGRAPHIC_DEVICE::getInstance();
	//!	デバイスの生成
		if(	!gDev->initDevice(hWnd, kWIN_WIDTH, kWIN_HEIGHT, bFullScreen))
		{
			return	FALSE;	//!	生成失敗
		}

	//!	初期シーンの生成
		APP_SCENE_FLAG	inFlag	= eSCENE_LOGO;
#ifdef	_DEBUG
		inFlag	= DEBUG_FLAG;
#endif
#ifdef	PRESENTATION
		inFlag	= PRESENTATION_FLAG;
#endif

		app->flagChack(inFlag);
		return TRUE;		//!< 生成成功を返す
	}

	//================================================================================
	//!	メソッド名	CWINDOW_MANAGER::initLoop
	//
	//	機能		ウィンドウを更新し、ループの準備をする
	//	引数		inShowCmd		ウィンドウ表示方法
	//	更新		2008/04/17		<新規>
	//================================================================================
	void
	CWINDOW_MANAGER::initLoop(int inShowCmd)
	{
		CoInitialize(NULL);
		ShowWindow(hWnd, inShowCmd);		//!< 表示状態設定
		UpdateWindow(hWnd);				//!< ウィンドウを更新する
		timeBeginPeriod(1);						//!< タイマー更新時間を1000分の1秒に設定
		ShowCursor(FALSE);						//!< カーソルを非表示にする
		dwTime = timeGetTime();			//!< 時間の取得
	}

	//================================================================================
	//!	メソッド名	CWINDOW_MANAGER::messageLoop
	//
	//	機能		メッセージループ
	//	更新		2008/04/17		<新規>
	//================================================================================
	int
	CWINDOW_MANAGER::messageLoop(void)
	{
	//!	既にプログラムが起動している場合、ループに入らず終了
		if(bOperation)			return 0;
	//!	登録・生成に失敗している場合、ループに入らず終了
		if(bGenerationFailure)	return 0;

	//!	メッセージループ
		while(TRUE)
		{
		//!	メッセージが来ているか
			if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
			//!	メッセージの列挙が終わった
				if(!GetMessage(&msg, NULL, 0, 0))
				{
					break;
				}
				TranslateMessage(&msg);		//!< 仮想キーメッセージを文字メッセージへ変換する
				DispatchMessage(&msg);		//!< メッセージをウィンドウプロシージャに転送する
			}
			else
			{
			//!	メッセージが無く、ウィンドウがアクティブ状態の時
				if(GetActiveWindow() == hWnd)
				{
				//!	デバイスをリセットする必要がある時
					if(	bResetDevice)
					{
					//!	デバイスをリセット
						if(	gDev->resetDevice())
						{
						//	デバイスリセットが完了したら、フラグを戻す
							bResetDevice	= FALSE;
						}
					}
				//!	フレーム制御
					else if(timeGetTime() > dwTime + 15)
					{
					//! 処理時間を更新する
						dwTime = timeGetTime();				
					//!	ウィンドウモードの場合
						if(!bFullScreen)
						{
						//!	FPSを計算し表示する
							calclateFPS();
						}
					//!	ゲーム処理をして、メッセージを判定
						app->main();
					}
				//!	上記に満たさない場合、OSに処理を返す
					else
					{
						Sleep(1);
					}
				}
			//!	ウィンドウが非アクティブの時
				else
				{
				//	フルスクリーンの時
					if(	!bResetDevice	&&
						 bFullScreen)
					{
					//	デバイスを復帰する必要がある
						bResetDevice	= TRUE;
						CDEPTH_SHA::lostInstance();
						CZTEX_SHA::lostInstance();
					}
				//!	OSに制御を渡す
					Sleep(1);
				}
			}
		}
		return 0;
	}

	//================================================================================
	//!	メソッド名	CWINDOW_MANAGER::calclateFPS
	//
	//	機能		1秒間あたりのフレーム数を求める
	//	更新		2008/04/17		<新規>
	//================================================================================
	void
	CWINDOW_MANAGER::calclateFPS(void)
	{
		DWORD	dwCurrent(timeGetTime());		//!< 現在の時間を格納
		dwFrames++;								//!< フレーム数更新

	//!	FPS更新
		if(500 <= dwCurrent - dwLast)
		{
			float	dt((float)(dwCurrent - dwLast) / 1000.0f);				//!< 時間隔を計算
			float	fps((float)dwFrames / dt);								//!< fpsを計算
			dwLast		= dwCurrent;										//!< 現在の時間を格納
			dwFrames	= 0;												//!< フレームカウンタ初期化
			sprintf_s(szFps, sizeof(szFps), kGAMETITLE, fps);				//!< タイトル文の更新
			SetWindowText(hWnd, szFps);										//!< タイトルの更新
		}
	}
}
//-----------------------------------------------------------------------
//!	ウィンドウマネージャの名前空間
//-----------------------------------------------------------------------