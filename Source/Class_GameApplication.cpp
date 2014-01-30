//***********************************************************************************
/*!	\file		Class_GameApplication.cpp
 *
 *	\brief		ゲームアプリケーションの実装.
 *
 *　このファイルはClass_GameApplication.cppです.
 *
 *	\author		上岡　純
 *	\version	KageTokiGIRL
 *	\date		2008/04/17	<新規>
 */
//***********************************************************************************

#include	"Precompiler.h"		//!< プリコンパイラー

//-----------------------------------------------------------------------
namespace GAPP		//!< \namespace ゲームアプリケーションの名前空間
//-----------------------------------------------------------------------
{
	//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
	/*!	\class	CGAME_APPLICATION
	 *
	 *	\brief	ゲームアプリケーション
	 */
	//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[

	//	シーンの名前空間を解放
	using	namespace	SCE;

	//	インスタンス生成場所
	CGAME_APPLICATION*	CGAME_APPLICATION::gUniqueInstance	= NULL;

	//================================================================================
	/*!	\method	コンストラクタ
	 *
	 *	\brief	生成時の処理を行う
	 *
	 *	\param	inWnd		[in]ウィンドウハンドル
	 *
	 *	\date	2008/04/17	<新規>
	 */
	CGAME_APPLICATION::CGAME_APPLICATION(HWND inWnd)
	{
	//	NULL化
		this->scene					= NULL;
		this->inputKeyBoard			= NULL;
		this->inputGamePad			= NULL;
		this->effectCenterInstance	= NULL;
		this->soundInstance			= NULL;

		this->scenePrevFlag			= eSCENE_NO;
		this->sceneCurrFlag			= eSCENE_NO;

		this->initialize(inWnd);
	}

	//================================================================================
	/*!	\method	デストラクタ
	 *
	 *	\brief	解放時の処理を行う
	 *
	 *	\date	2008/04/17	<新規>
	 */
	CGAME_APPLICATION::~CGAME_APPLICATION(void)
	{
		this->finishalize();
	}

	//================================================================================
	/*!	\method	getInstance(クラスメソッド)
	 *
	 *	\brief	一度だけインスタンスを生成し、インスタンスを返す
	 *
	 *	\param	inWnd		[in]ウィンドウハンドル
	 *
	 *	\return	生成されたインスタンス
	 *
	 *	\date	2008/04/17	<新規>
	 */
	CGAME_APPLICATION*
	CGAME_APPLICATION::getInstance(HWND inWnd)
	{
	//	生成されていなければ、生成する
		if(	!gUniqueInstance)
		{
		//	インスタンス生成
			gUniqueInstance = new CGAME_APPLICATION(inWnd);
		}
	//	インスタンスを返す
		return gUniqueInstance;
	}

	//================================================================================
	/*!	\method	releaseInstance(クラスメソッド)
	 *
	 *	\brief	インスタンスを解放する
	 *
	 *	\date	2008/04/17	<新規>
	 */
	void
	CGAME_APPLICATION::releaseInstance(void)
	{
	//	インスタンスの解放
		SAFE_DELETE(gUniqueInstance);
	}

	//================================================================================
	/*!	\method	initialize
	 *
	 *	\brief	初期化処理を行う
	 *
	 *	\param	inWnd		[in]ウィンドウハンドル
	 *
	 *	\date	2008/04/17	<新規>
	 *	\date	2008/04/24	<追加>	シーンの初期化
	 */
	void
	CGAME_APPLICATION::initialize(HWND inWnd)
	{
		this->finishalize();					//	解放
		this->hWnd			= inWnd;			//	ウィンドウハンドルの初期化
		this->scenePrevFlag	= eSCENE_NO;		//	前回のゲームフラグの初期化
		this->sceneCurrFlag	= eSCENE_NO;		//	現在のゲームフラグの初期化

	//	シーン共有データの初期化
		ZeroMemory(&this->sShareData, sizeof(this->sShareData));
		this->sShareData.dwPlayerLife	= 3;

	//	乱数初期化
		srand((unsigned)time(NULL));

	//	入力機器の設定
		MYERROR_DI::TYPENAME	rtvInputDevice;
	//	----------------------------------------------------------------------------------------------						
	//	キーボードをDirectInput機器として設定する
	//	インスタンス取得	入力デバイス0　（キーボード）
		this->inputKeyBoard	= CINPUT::getInstance(0);

		if(	this->inputKeyBoard)
		{
		//	使用するキーと対応させる仮想ボタンを設定
			sKeyParam	keyBoardKeys[]={
			//	キーボードのキー（実キー）		仮想ゲームパッドのボタン		キーの初期状態
				{DIK_UP,						MYKEY::eUP,						MYKEYSTAT::eFREE},		//上キー
				{DIK_DOWN,						MYKEY::eDOWN,					MYKEYSTAT::eFREE},		//下キー
				{DIK_LEFT,						MYKEY::eLEFT,					MYKEYSTAT::eFREE},		//左キー
				{DIK_RIGHT,						MYKEY::eRIGHT,					MYKEYSTAT::eFREE},		//右キー
				{DIK_C,							MYKEY::eSOFT,					MYKEYSTAT::eFREE},		//Ｃキー
				{DIK_X,							MYKEY::eMIDDLE,					MYKEYSTAT::eFREE},		//Ｘキー
				{DIK_B,							MYKEY::eHARD,					MYKEYSTAT::eFREE},		//Ｂキー
				{DIK_Z,							MYKEY::eDUST,					MYKEYSTAT::eFREE},		//Ｚキー
				{DIK_Q,							MYKEY::eL1,						MYKEYSTAT::eFREE},		//Ｑキー
				{DIK_E,							MYKEY::eL2,						MYKEYSTAT::eFREE},		//Ｅキー
				{DIK_R,							MYKEY::eR1,						MYKEYSTAT::eFREE},		//Ｒキー
				{DIK_F,							MYKEY::eR2,						MYKEYSTAT::eFREE},		//Ｆキー
				{DIK_RETURN,					MYKEY::eSELECT,					MYKEYSTAT::eFREE},		//Enter
				{DIK_SPACE,						MYKEY::eSTART,					MYKEYSTAT::eFREE},		//Space
				{DIK_ESCAPE,					MYKEY::eQUIT,					MYKEYSTAT::eFREE},		//Esc
#ifdef	_DEBUG
				{DIK_F1,						MYKEY::eF1,						MYKEYSTAT::eFREE},		//F1
				{DIK_F2,						MYKEY::eF2,						MYKEYSTAT::eFREE},		//F2
				{DIK_F3,						MYKEY::eF3,						MYKEYSTAT::eFREE},		//F3
				{DIK_F4,						MYKEY::eF4,						MYKEYSTAT::eFREE},		//F4
				{DIK_F5,						MYKEY::eF5,						MYKEYSTAT::eFREE},		//F5
				{DIK_F6,						MYKEY::eF6,						MYKEYSTAT::eFREE},		//F6
				{DIK_F7,						MYKEY::eF7,						MYKEYSTAT::eFREE},		//F7
				{DIK_F8,						MYKEY::eF8,						MYKEYSTAT::eFREE},		//F8
				{DIK_F9,						MYKEY::eF9,						MYKEYSTAT::eFREE},		//F9
				{DIK_F10,						MYKEY::eF10,					MYKEYSTAT::eFREE},		//F10
#else 
#ifdef	PRESENTATION
				{DIK_F1,						MYKEY::eF1,						MYKEYSTAT::eFREE},		//F1
				{DIK_F2,						MYKEY::eF2,						MYKEYSTAT::eFREE},		//F2
				{DIK_F3,						MYKEY::eF3,						MYKEYSTAT::eFREE},		//F3
				{DIK_F4,						MYKEY::eF4,						MYKEYSTAT::eFREE},		//F4
				{DIK_F5,						MYKEY::eF5,						MYKEYSTAT::eFREE},		//F5
				{DIK_F6,						MYKEY::eF6,						MYKEYSTAT::eFREE},		//F6
				{DIK_F7,						MYKEY::eF7,						MYKEYSTAT::eFREE},		//F7
				{DIK_F8,						MYKEY::eF8,						MYKEYSTAT::eFREE},		//F8
				{DIK_F9,						MYKEY::eF9,						MYKEYSTAT::eFREE},		//F9
				{DIK_F10,						MYKEY::eF10,					MYKEYSTAT::eFREE},		//F10
				{DIK_1,							MYKEY::e1,						MYKEYSTAT::eFREE},		//1
				{DIK_2,							MYKEY::e2,						MYKEYSTAT::eFREE},		//2
				{DIK_3,							MYKEY::e3,						MYKEYSTAT::eFREE},		//3
				{DIK_4,							MYKEY::e4,						MYKEYSTAT::eFREE},		//4
				{DIK_5,							MYKEY::e5,						MYKEYSTAT::eFREE},		//5
				{DIK_6,							MYKEY::e6,						MYKEYSTAT::eFREE},		//6
				{DIK_7,							MYKEY::e7,						MYKEYSTAT::eFREE},		//7
				{DIK_8,							MYKEY::e8,						MYKEYSTAT::eFREE},		//8
				{DIK_9,							MYKEY::e9,						MYKEYSTAT::eFREE},		//9
				{DIK_0,							MYKEY::e0,						MYKEYSTAT::eFREE},		//10
#endif
#endif
			};
		//	入力デバイス＆使用するキーを設定
			rtvInputDevice = this->inputKeyBoard->init(
								inWnd,										//!< ウィンドウハンドル
								DEVICETYPE::eKEYBOARD,						//デバイスタイプ：キーボード
								sizeof(keyBoardKeys) / sizeof(sKeyParam),	//入力種類数
								keyBoardKeys);
		//	デバイスの生成チェック（失敗場合開放処理へ）
			if(rtvInputDevice != MYERROR_DI::eNOTHING)
			{
				CINPUT::releaseInstance(0);
			}
		}
	//	----------------------------------------------------------------------------------------------						
	//	ゲームパッドをDirectInput機器として設定する
	//	インスタンス取得	入力デバイス1　（ゲームパbrッド１番）
		this->inputGamePad	= CINPUT::getInstance(1);

		if(	this->inputGamePad)
		{
		//	使用するボタンと対応させる仮想ボタンを設定
			sKeyParam	gemePadKeys[]={
			//	ゲームパットのボタン（実ボタン）仮想ゲームパッドのボタン		キーの初期状態
				{MYKEY::eUP,					MYKEY::eUP,						MYKEYSTAT::eFREE},		//上キー
				{MYKEY::eDOWN,					MYKEY::eDOWN,					MYKEYSTAT::eFREE},		//下キー
				{MYKEY::eLEFT,					MYKEY::eLEFT,					MYKEYSTAT::eFREE},		//左キー
				{MYKEY::eRIGHT,					MYKEY::eRIGHT,					MYKEYSTAT::eFREE},		//右キー
				{MYKEY::eSOFT,					MYKEY::eSOFT,					MYKEYSTAT::eFREE},		//右ボタン
				{MYKEY::eMIDDLE,				MYKEY::eMIDDLE,					MYKEYSTAT::eFREE},		//下ボタン
				{MYKEY::eHARD,					MYKEY::eHARD,					MYKEYSTAT::eFREE},		//上ボタン
				{MYKEY::eDUST,					MYKEY::eDUST,					MYKEYSTAT::eFREE},		//左ボタン
				{MYKEY::eL1,					MYKEY::eL1,						MYKEYSTAT::eFREE},		//Ｌ１
				{MYKEY::eL2,					MYKEY::eL2,						MYKEYSTAT::eFREE},		//Ｌ２
				{MYKEY::eR1,					MYKEY::eR1,						MYKEYSTAT::eFREE},		//Ｒ１
				{MYKEY::eR2,					MYKEY::eR2,						MYKEYSTAT::eFREE},		//Ｒ２
				{MYKEY::eSELECT,				MYKEY::eSELECT,					MYKEYSTAT::eFREE},		//スタート
				{MYKEY::eSTART,					MYKEY::eSTART,					MYKEYSTAT::eFREE}		//セレクト
			};
		//	入力デバイス＆使用するキーを設定
			rtvInputDevice = this->inputGamePad->init(
								inWnd,										//!< ウィンドウハンドル
								DEVICETYPE::eGAMEPAD,						//デバイスタイプ：キーボード
								sizeof(gemePadKeys) / sizeof(sKeyParam),	//入力種類数
								gemePadKeys);
		//	デバイスの生成チェック（失敗場合開放処理へ）
			if(rtvInputDevice != MYERROR_DI::eNOTHING)
			{
				CINPUT::releaseInstance(0);
			}
		}

		this->effectCenterInstance	= C3D_EFFECT_CENTER::getInstance();	//!< エフェクトセンター
		this->soundInstance			= new CSOUND(inWnd);				//!< サウンド

	//	グローバルに配置するフォントリソースの生成
		AddFontResource(NAME_FONT1);
		AddFontResource(NAME_FONT2);
	}

	//================================================================================
	//!	メソッド名	CGAME_APPLICATION::finishalize
	//
	//	機能		解放処理
	//	更新		2008/04/17	<新規>
	//================================================================================
	void
	CGAME_APPLICATION::finishalize(void)
	{
		SAFE_DELETE(this->scene);							//!< シーンの解放
		this->inputKeyBoard			= NULL;					//!< キーボード入力アクセス遮断
		this->inputGamePad			= NULL;					//!< ゲームパッド入力アクセス遮断
		this->effectCenterInstance	= NULL;					//!< エフェクトセンターアクセス遮断

		EM::CADVENT_ENEMY_MANAGER::releaseInstance();		//!< エネミーマネージャーの解放
		SAVER::CSAVE_RESOURCE::releaseInstance();			//!< セーブリソースの解放
		CINPUT::releaseInstanceAll();						//!< キーボード入力の解放
		C3D_EFFECT_CENTER::releaseInstance();				//!< エフェクトセンターの解放
		CFONT_BASE::releaseInstance();						//!< フォント管理
		SAFE_DELETE(this->soundInstance);					//!< サウンド

	//	グローバルに配置するフォントリソースの解放
		RemoveFontResource(NAME_FONT1);
		RemoveFontResource(NAME_FONT2);
	}

	//================================================================================
	//!	メソッド名	CGAME_APPLICATION::main
	//
	//	機能		メイン処理
	//	戻り値		アプリメインからのメッセージ
	//	更新		2008/04/17	<新規>
	//				2008/04/24	<追加>	フラグ分岐実装
	//================================================================================
	HRESULT
	CGAME_APPLICATION::main(void)
	{
	//	例外を受け取る
		try
		{
		//	シーンが存在する時
			if(	this->scene)
			{
				CINPUT*	input_ptr	= NULL;
			//	キーボード入力更新
				if(	this->inputKeyBoard)
				{
					this->inputKeyBoard->checkKey();
					input_ptr	= inputKeyBoard;
				}
			//	ゲームパッド入力更新
				if(	this->inputGamePad)
				{
					this->inputGamePad->checkKey();
				//	他に入力があるなら、入力同期
					if(	input_ptr)
						input_ptr->composeKeyStatus(this->inputGamePad);
					else
						input_ptr	= inputGamePad;
				}
			//	入力デバイスが無ければ、エラー報告
				if(	!input_ptr)	{ throw	"入力デバイスがありません";	}

			#ifdef	_DEBUG
			//	F1が押されたら、ロゴシーンに戻る
				if(	input_ptr->checkPressOnce(MYKEY::eF1))
				{
					this->flagChack(eSCENE_LOGO);
				}
			#else
			#ifdef	PRESENTATION
				if(	input_ptr->checkPressOnce(MYKEY::e1))	this->flagChack(eSCENE_CHAPTER1_STAGE1);
				if(	input_ptr->checkPressOnce(MYKEY::e2))	this->flagChack(eSCENE_CHAPTER1_STAGE2);
				if(	input_ptr->checkPressOnce(MYKEY::e3))	this->flagChack(eSCENE_CHAPTER1_STAGE3);
				if(	input_ptr->checkPressOnce(MYKEY::e4))	this->flagChack(eSCENE_CHAPTER2_STAGE4);
				if(	input_ptr->checkPressOnce(MYKEY::e5))	this->flagChack(eSCENE_CHAPTER2_STAGE5);
				if(	input_ptr->checkPressOnce(MYKEY::e6))	this->flagChack(eSCENE_CHAPTER2_STAGE6);
				if(	input_ptr->checkPressOnce(MYKEY::e7))	this->flagChack(eSCENE_CHAPTER3_STAGE7);
				if(	input_ptr->checkPressOnce(MYKEY::e8))	this->flagChack(eSCENE_CHAPTER3_STAGE8);
				if(	input_ptr->checkPressOnce(MYKEY::e9))	this->flagChack(eSCENE_CHAPTER3_STAGE9);
				if(	input_ptr->checkPressOnce(MYKEY::e0))	this->flagChack(eSCENE_LAST_CHAPTER_STAGE10);
			#endif
			#endif

			//	いつでも処理終了
				if(	GetAsyncKeyState(VK_ESCAPE))
				{
					throw 0;
				}

			//	シーンを実行し、帰って来たフラグを見る
				this->flagChack(this->scene->sceneMain(input_ptr));
			}
		//	シーンが無ければ、エラー報告
			else
			{
				throw	"シーンエラー";
			}
		}
	//!	文字列の例外を取得
		catch(char szString[MAX_PATH])
		{
			char	szErr[MAX_PATH] = "";
		//	エラーメッセージの作成
			my_sprintf(szErr, sizeof(szErr), "「%s」が不正です", szString);
		//	メッセージボックスの表示
			MessageBox(this->hWnd, szErr, "リソースエラー", MB_OK);
		//!	プログラム終了へ
			SendMessage(this->hWnd, WM_DESTROY, 0, 0);
		}
	//!	シーンエラー報告
		catch(APP_SCENE_FLAG flag)
		{
		//	エラー報告文
			char	err[eSCENE_MAX][MAX_PATH]	=
			{
				"解明不能",
				"ロゴ",
				"デモ",
				"タイトル",
				"ムービー",
				"ゲームメイン",
				"ゲームクリア",
				"オプション",
				"エンディング",
				"ゲーム終了",
				"解明不能",
			};

		//	エラー報告
			char	szErr[MAX_PATH] = "";
		//	エラーメッセージの作成
			my_sprintf(szErr, sizeof(szErr), "%s時のエラーです。", err[flag]);
		//	メッセージボックスの表示
			MessageBox(this->hWnd, szErr, "ゲームエラー", MB_OK);
		//!	プログラム終了へ
			SendMessage(this->hWnd, WM_DESTROY, 0, 0);
			
		}
	//!	上記以外の例外を取得
		catch(...)
		{
		//	こちらで、ウィンドウを破棄
			SendMessage(this->hWnd, WM_DESTROY, 0, 0);
		}
		return 0;
	}

	//================================================================================
	//!	メソッド名	CGAME_APPLICATION::flagChack
	//
	//	機能		受け取ったフラグをチェック
	//	引数		inGFlag		新しいフラグ
	//	更新		2008/04/21	<新規>
	//================================================================================
	void
	CGAME_APPLICATION::flagChack(SCENE_FLAG inGFlag)
	{
	//	前のフラグと等しいなら、チェックしない
		if(	this->sceneCurrFlag == inGFlag)	return;
	//	前回のフラグを確保
		this->scenePrevFlag	= this->sceneCurrFlag;
	//	新しいフラグを確保
		this->sceneCurrFlag	= inGFlag;
	//	前のシーンを解放
		SAFE_DELETE(this->scene);
	//	フラグ分岐処理
		switch(this->sceneCurrFlag)
		{
		//-------------------------------------
		//	オープニングフロー
		//-------------------------------------
		//	ゲームロゴ
			case	eSCENE_LOGO:
				this->scene = new CSCENE_LOGO(&this->sShareData);
				break;
		//	デモ
			case	eSCENE_DEMO:
				this->scene = new CSCENE_DEMO(&this->sShareData);
				break;
		//	タイトル
			case	eSCENE_TITLE:
				this->sShareData.dwPlayerLife	= 3;
				this->scene = new CSCENE_TITLE(&this->sShareData);
				break;
		//	オプション
			case	eSCENE_OPTION:
				throw	0;
				break;

		//-------------------------------------
		//	チャプター1[謎の集団]
		//-------------------------------------
		//	ムービー1[デート]
			case	eSCENE_CHAPTER1_MOVIE1:
				this->scene = new CSCENE_MOVIE1(&this->sShareData);
				break;
		//	ステージ1[公園]
			case	eSCENE_CHAPTER1_STAGE1:
				this->scene = new CSCENE_STAGE1(&this->sShareData);
				break;
		//	ムービー2[シャドウクリミナル、現る]
			case	eSCENE_CHAPTER1_MOVIE2:
				this->scene = new CSCENE_MOVIE2(&this->sShareData);
				break;
		//	ステージ2[公園出口]
			case	eSCENE_CHAPTER1_STAGE2:
				this->scene = new CSCENE_STAGE2(&this->sShareData);
				break;
		//	ムービー3[シャドウクリミナルアジト]
			case	eSCENE_CHAPTER1_MOVIE3:
				this->scene = new CSCENE_MOVIE3(&this->sShareData);
				break;
		//	ステージ3[村]
			case	eSCENE_CHAPTER1_STAGE3:
				this->scene = new CSCENE_STAGE3(&this->sShareData);
				break;
		//	ボス1[エクス]
			case	eSCENE_CHAPTER1_BOSS1:
				this->scene = new CSCENE_BOSS1(&this->sShareData);
				break;

		//-------------------------------------
		//	チャプター2[シャドウクリミナルを追え!]
		//-------------------------------------
		//	ムービー4[敵を追って…]
			case	eSCENE_CHAPTER2_MOVIE4:
				this->scene = new CSCENE_MOVIE4(&this->sShareData);
				break;
		//	ステージ4[荒野]
			case	eSCENE_CHAPTER2_STAGE4:
				this->scene = new CSCENE_STAGE4(&this->sShareData);
				break;
		//	ムービー5[山あり…谷あり…]
			case	eSCENE_CHAPTER2_MOVIE5:
				this->scene = new CSCENE_MOVIE5(&this->sShareData);
				break;
		//	ステージ5[山]
			case	eSCENE_CHAPTER2_STAGE5:
				this->scene = new CSCENE_STAGE5(&this->sShareData);
				break;
		//	ムービー6[アジト?]
			case	eSCENE_CHAPTER2_MOVIE6:
				this->scene = new CSCENE_MOVIE6(&this->sShareData);
				break;
		//	ステージ6[洞窟]
			case	eSCENE_CHAPTER2_STAGE6:
				this->scene = new CSCENE_STAGE6(&this->sShareData);
				break;
		//	ボス2[カゲヤマ]
			case	eSCENE_CHAPTER2_BOSS2:
				this->scene = new CSCENE_BOSS2(&this->sShareData);
				break;

		//-------------------------------------
		//	チャプター3[アジト急激]
		//-------------------------------------
		//	ムービー7[見つけた手段]
			case	eSCENE_CHAPTER3_MOVIE7:
				this->scene = new CSCENE_MOVIE7(&this->sShareData);
				break;
		//	ステージ7[上空]
			case	eSCENE_CHAPTER3_STAGE7:
				this->scene = new CSCENE_STAGE7(&this->sShareData);
				break;
		//	ボス3[フェザー]
			case	eSCENE_CHAPTER3_BOSS3:
				this->scene = new CSCENE_BOSS3(&this->sShareData);
				break;
		//	ムービー8[本拠地発見]
			case	eSCENE_CHAPTER3_MOVIE8:
				this->scene = new CSCENE_MOVIE8(&this->sShareData);
				break;
		//	ステージ8[城下町]
			case	eSCENE_CHAPTER3_STAGE8:
				this->scene = new CSCENE_STAGE8(&this->sShareData);
				break;
		//	ムービー9[門は叩かず、壊してけ!]
			case	eSCENE_CHAPTER3_MOVIE9:
				this->scene = new CSCENE_MOVIE9(&this->sShareData);
				break;
		//	ステージ9[城内部]
			case	eSCENE_CHAPTER3_STAGE9:
				this->scene = new CSCENE_STAGE9(&this->sShareData);
				break;

		//-------------------------------------
		//	ラストチャプター[クライマックス]
		//-------------------------------------
		//	ムービー10[追い詰めたぞ!]
			case	eSCENE_LAST_CHAPTER_MOVIE10:
				this->scene = new CSCENE_MOVIE10(&this->sShareData);
				break;
		//	ステージ10[巨大地下迷宮]
			case	eSCENE_LAST_CHAPTER_STAGE10:
				this->scene = new CSCENE_STAGE10(&this->sShareData);
				break;
		//	ボス4[ティマ]
			case	eSCENE_LAST_CHAPTER_BOSS4:
				this->scene = new CSCENE_BOSS4(&this->sShareData);
				break;
		//	ムービー11[最終決戦!]
			case	eSCENE_LAST_CHAPTER_MOVIE11:
				this->scene = new CSCENE_MOVIE11(&this->sShareData);
				break;
		//	ラスボス[クライム]
			case	eSCENE_LAST_CHAPTER_LAST_BOSS:
				this->scene = new CSCENE_BOSS5(&this->sShareData);
				break;
		//	エンディングムービー[平和の訪れ]
			case	eSCENE_LAST_CHAPTER_ENDING_MOVIE:
				this->scene = new CSCENE_MOVIE12(&this->sShareData);
				break;

		//-------------------------------------
		//	エンディングフロー
		//-------------------------------------
		//	エンディング
			case	eSCENE_ENDING:
				this->scene = new CSCENE_ENDING(&this->sShareData);
				break;
		//	ゲームオーバー
			case	eSCENE_GAMEOVER:
				this->scene = new CSCENE_GAMEOVER(&this->sShareData);
				break;

		//	ゲーム終了
			case	eSCENE_QUIT:
				throw	0;
				break;
		//	エラー
			case	eSCENE_ERROR:
				throw	this->scenePrevFlag;
				break;
		//	命令なし
			case	eSCENE_NO:
			//	なにもせずに終了
				return;
		//	その他は何もしない
			default:
				break;
		}
	//	シーンが生成されてたら、初期化
		if(	this->scene){ this->scene->sceneInitialize(0);	}
	}
//-----------------------------------------------------------------------
}								//!	ゲームアプリケーションの名前空間
//-----------------------------------------------------------------------