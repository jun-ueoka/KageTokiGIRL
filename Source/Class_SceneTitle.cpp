//***********************************************************************************
//!	ファイル名		Class_SceneTitle.cpp
//
//	機能			タイトルシーンの実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2008/9/01	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneTitle.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_TITLE::CSCENE_TITLE
	//
	//	機能		コンストラクタ
	//	引数		inData			シーン共有データ
	//	更新		2008/09/01		<新規>
	//================================================================================
	CSCENE_TITLE::CSCENE_TITLE(SCENE_SHARE_DATA* inData)	:
	CSCENE(eSCENE_TITLE, eSCENE_TITLE, inData)		//!< 基礎シーンフラグの設定
	{
	//	初期化
		this->titleFlag			= eST_INIT_TITLE;			//!< 分岐フラグ(ゲームプレイ中)
		this->startFlag			= eSCENE_CHAPTER1_MOVIE1;	//!< スタート移行フラグ
		this->endFlag			= eSCENE_QUIT;				//!< エンド移行フラグ
		this->loopFlag			= eSCENE_LOGO;				//!< ループフラグ

		this->cursorPos			= eCURP_START;						//!< カーソル位置
		this->gDev				= CGRAPHIC_DEVICE::getInstance();	//!< グラフィックデバイス
		this->effect			= C3D_EFFECT_CENTER::getInstance();	//!< エフェクトセンター

		this->titleBack			= NULL;				//!< タイトル背景
		this->titleTex			= NULL;				//!< タイトル
		this->start				= NULL;				//!< 「START」
		this->end				= NULL;				//!< 「END」
		this->option			= NULL;				//!< 「OPTION」
		this->cursor			= NULL;				//!< カーソル
		this->font				= NULL;				//!< フォント

		this->sStaticCursor		= NULL;				//!< カーソル音
		this->sStaticDecision	= NULL;				//!< 決定音
		this->sStreamingBgm		= NULL;				//!< 背景音

		this->iStack			= 0;				//!< スタックカウンタ
		this->bStack			= FALSE;			//!< スタック確認
		this->iTimeCnt			= 0;				//!< タイムカウンタ

	//	幕の色を設定
		this->gDev->setCurtainColor(0, 0, 0);
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::~CSCENE_TITLE
	//
	//	機能		デストラクタ
	//	更新		2008/09/01		<新規>
	//================================================================================
	CSCENE_TITLE::~CSCENE_TITLE(void)
	{
	//	解放
		this->sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::motionInitTitle
	//
	//	機能		タイトル画面移行演出処理
	//	引数		input			入力情報
	//	更新		2008/09/01		<新規>
	//================================================================================
	void
	CSCENE_TITLE::motionInitTitle(CINPUT* input)
	{
	//	スタックフレームをカウント
		this->iStack++;
	//	タイトル出現
		if(	this->iStack > 100	&&
			this->iStack < 200)
		{
			if(	this->iStack < 150)
			{
				this->titleTex->calcColor(0, 0, 0, 5);
			}
			else
			{
				this->titleTex->calcColor(5, 5, 5, 5);
			}
		}
	//	スタート出現
		if(	this->iStack > 130	&&
			this->iStack < 230)
		{
			if(	this->iStack < 180)
			{
				this->start->calcColor(0, 0, 0, 5);
			}
			else
			{
				this->start->calcColor(5, 5, 5, 5);
			}
		}
	//	オプション出現
		if(	this->iStack > 160	&&
			this->iStack < 260)
		{
			if(	this->iStack < 210)
			{
				this->option->calcColor(0, 0, 0, 5);
			}
			else
			{
				this->option->calcColor(5, 5, 5, 5);
			}
		}
	//	オプション出現
		if(	this->iStack > 190	&&
			this->iStack < 290)
		{
			if(	this->iStack < 240)
			{
				this->end->calcColor(0, 0, 0, 5);
			}
			else
			{
				this->end->calcColor(5, 5, 5, 5);
			}
		}

	//	スペースキーが押されたら、タイトルに移る
		if(	input->checkPress(MYKEY::eSTART)	||
			this->iStack > 350)
		{
			this->initTitle();
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::motionTitle
	//
	//	機能		タイトル画面処理
	//	引数		input			入力情報
	//	更新		2008/09/01		<新規>
	//================================================================================
	void
	CSCENE_TITLE::motionTitle(CINPUT* input)
	{
	//-------------------------------------------------
	//	インターフェースの動作
	//-------------------------------------------------
	//	カーソル位置により、処理分岐
		switch(this->cursorPos)
		{
		//	「START」に位置する
			case eCURP_START:
			//	カーソルを下に移動
				if(	input->checkPressOnce(MYKEY::eDOWN))
				{
					this->cursorPos	= eCURP_GO_OPTION_FROM_START;
				//	カーソル音
					this->soundCursor();
				}
			//	決定キーが押されたら、ゲームスタート演出に移る
				if(	input->checkPressOnce(MYKEY::eSTART))
				{
					this->initStartSwitch();
				//	決定音
					this->soundDecision();
				}
				break;

		//	「START」に向かう
			case eCURP_GO_START:
			//	カーソルを上に移動して、「START」位置についていない時
				if((this->cursor->getLocY() - CURSOR_MOVE_SPEED) >=
					CURSOR_START_POSY)
				{
					this->cursor->calcLocY(-CURSOR_MOVE_SPEED);
				}
			//	カーソル位置が、「START」に付いた時
				else
				{
					this->cursorPos	= eCURP_START;
					this->cursor->setLocY(CURSOR_START_POSY);
				}
				break;

		//	「OPTION」に位置する
			case eCURP_OPTION:
			//	カーソルを上に移動
				if(	input->checkPressOnce(MYKEY::eUP))
				{
					this->cursorPos	= eCURP_GO_START;
				//	カーソル音
					this->soundCursor();
				}
			//	カーソルを下に移動
				if(	input->checkPressOnce(MYKEY::eDOWN))
				{
					this->cursorPos	= eCURP_GO_END;
				//	カーソル音
					this->soundCursor();
				}
			//	決定キーが押されたら、ゲーム終了演出に移る
				if(	input->checkPressOnce(MYKEY::eSTART))
				{
				//	this->initEndSwitch();
				////	決定音
				//	this->soundDecision();
				}
				break;

		//	「START」から「OPTION」に向かう
			case eCURP_GO_OPTION_FROM_START:
			//	カーソルを下に移動して、「OPTION」位置についていない時
				if(	(this->cursor->getLocY() + CURSOR_MOVE_SPEED) <=
					CURSOR_OPTION_POSY)
				{
					this->cursor->calcLocY(CURSOR_MOVE_SPEED);
				}
			//	カーソル位置が、「OPTION」に付いた時
				else
				{
					this->cursorPos	= eCURP_OPTION;
					this->cursor->setLocY(CURSOR_OPTION_POSY);
				}
				break;

		//	「END」から「OPTION」に向かう
			case eCURP_GO_OPTION_FROM_END:
			//	カーソルを上に移動して、「OPTION」位置についていない時
				if(	(this->cursor->getLocY() - CURSOR_MOVE_SPEED) >=
					CURSOR_OPTION_POSY)
				{
					this->cursor->calcLocY(-CURSOR_MOVE_SPEED);
				}
			//	カーソル位置が、「OPTION」に付いた時
				else
				{
					this->cursorPos	= eCURP_OPTION;
					this->cursor->setLocY(CURSOR_OPTION_POSY);
				}
				break;


		//	「END」に位置する
			case eCURP_END:
			//	カーソルを上に移動
				if(	input->checkPressOnce(MYKEY::eUP))
				{
					this->cursorPos	= eCURP_GO_OPTION_FROM_END;
				//	カーソル音
					this->soundCursor();
				}
			//	決定キーが押されたら、ゲーム終了演出に移る
				if(	input->checkPressOnce(MYKEY::eSTART))
				{
					this->initEndSwitch();
				//	決定音
					this->soundDecision();
				}
				break;

		//	「EMD」に向かう
			case eCURP_GO_END:
			//	カーソルを上に移動して、「END」位置についていない時
				if(	(this->cursor->getLocY() + CURSOR_MOVE_SPEED) <=
					CURSOR_END_POSY)
				{
					this->cursor->calcLocY(CURSOR_MOVE_SPEED);
				}
			//	カーソル位置が、「END」に付いた時
				else
				{
					this->cursorPos	= eCURP_END;
					this->cursor->setLocY(CURSOR_END_POSY);
				}
				break;
		}

	//	タイムチェックし、デモ移行フレームまで来たら、デモに移行
		if(	(++this->iTimeCnt) > DEMO_STEP_TIME)
		{
			this->initDemo();
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::motionStartSwitch
	//
	//	機能		ゲームスタート演出処理
	//	引数		input			入力情報
	//	更新		2008/09/20		<新規>
	//================================================================================
	void
	CSCENE_TITLE::motionStartSwitch(CINPUT* input)
	{
	//	「START」拡大
		if(	(this->iStack >  0) &&
			(this->iStack <= STARTSWITCH_EXPAND))
		{
			this->start->calcShowSize(&VEC2(2.3f, 1.0f));
		}

	//	幕の出現
		if(	(this->iStack >= STARTSWITCH_CLEAR_S)	&&
			(this->iStack <= STARTSWITCH_CLEAR_E))
		{
			this->gDev->curtainMotion(TRUE, 10);
		}

	//	スタックフレームをカウント
		this->iStack++;

	//	スタックが一定値まで行ったら、ムービーシーンへ
		if(	this->iStack > STARTSWITCH_END)
		{
			this->returnFlag	= this->startFlag;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::motionEndSwitch
	//
	//	機能		ゲーム終了演出処理
	//	引数		input			入力情報
	//	更新		2008/09/20		<新規>
	//================================================================================
	void
	CSCENE_TITLE::motionEndSwitch(CINPUT* input)
	{
	//	「END」拡大
		if(	(this->iStack >  0) &&
			(this->iStack <= ENDSWITCH_EXPAND))
		{
			this->end->calcShowSize(&VEC2(2.3f, 1.0f));
		}

	//	幕の出現
		if(	(this->iStack >= ENDSWITCH_CLEAR_S)	&&
			(this->iStack <= ENDSWITCH_CLEAR_E))
		{
			this->gDev->curtainMotion(TRUE, 5);
		}

	//	スタックフレームをカウント
		this->iStack++;

	//	スタックが一定値まで行ったら、終了へ
		if(	this->iStack > ENDSWITCH_END)
		{
		//	TGS用処理
			this->returnFlag	= this->endFlag;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::motionDemo
	//
	//	機能		デモ移行演出処理
	//	引数		input			入力情報
	//	更新		2008/09/30		<新規>
	//================================================================================
	void
	CSCENE_TITLE::motionDemo(CINPUT* input)
	{
	//	幕の出現
		if(	(this->iStack >= DEMO_CLEAR_S)	&&
			(this->iStack <= DEMO_CLEAR_E))
		{
			this->gDev->curtainMotion(TRUE, 5);
		}

	//	スタックフレームをカウント
		this->iStack++;

	//	スタックが一定値まで行ったら、デモへ
		if(	this->iStack > DEMO_END)
		{
			this->returnFlag	= this->loopFlag;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::renderInitTitle
	//
	//	機能		タイトル画面移行演出描画
	//	更新		2008/09/01		<新規>
	//================================================================================
	void
	CSCENE_TITLE::renderInitTitle(void)
	{
	//	インターフェースの描画
		this->titleTex->draw(this->gDev->getSprite());
		this->start->draw(this->gDev->getSprite());
		this->option->draw(this->gDev->getSprite());
		this->end->draw(this->gDev->getSprite());
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::renderTitle
	//
	//	機能		タイトル画面描画
	//	更新		2008/09/01		<新規>
	//================================================================================
	void
	CSCENE_TITLE::renderTitle(void)
	{
	//	インターフェースの描画
		this->titleTex->draw(this->gDev->getSprite());
		this->start->draw(this->gDev->getSprite());
		this->option->draw(this->gDev->getSprite());
		this->end->draw(this->gDev->getSprite());
		this->cursor->draw(this->gDev->getSprite());
		this->font->drawText(this->gDev->getSprite());
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::renderStartSwitch
	//
	//	機能		ゲームスタート演出描画
	//	更新		2008/09/20		<新規>
	//================================================================================
	void
	CSCENE_TITLE::renderStartSwitch(void)
	{
	//	インターフェースの描画
		this->titleTex->draw(this->gDev->getSprite());
		this->start->draw(this->gDev->getSprite());
		this->option->draw(this->gDev->getSprite());
		this->end->draw(this->gDev->getSprite());
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::renderEndSwitch
	//
	//	機能		ゲーム終了演出描画
	//	更新		2008/09/20		<新規>
	//================================================================================
	void
	CSCENE_TITLE::renderEndSwitch(void)
	{
	//	インターフェースの描画
		this->titleTex->draw(this->gDev->getSprite());
		this->start->draw(this->gDev->getSprite());
		this->option->draw(this->gDev->getSprite());
		this->end->draw(this->gDev->getSprite());
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::renderDemo
	//
	//	機能		デモ移行演出描画
	//	更新		2008/09/30		<新規>
	//================================================================================
	void
	CSCENE_TITLE::renderDemo(void)
	{
	//	インターフェースの描画
		this->titleTex->draw(this->gDev->getSprite());
		this->start->draw(this->gDev->getSprite());
		this->option->draw(this->gDev->getSprite());
		this->end->draw(this->gDev->getSprite());
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::initInitTitle
	//
	//	機能		タイトル画面移行演出命令
	//	更新		2008/09/20		<新規>
	//================================================================================
	void
	CSCENE_TITLE::initInitTitle(void)
	{
	//	タイトル画面移行演出に変更
		this->titleFlag	= eST_INIT_TITLE;
	//	その他の準備
		iStack			= 0;
		this->titleTex->setColor(0x00000000);
		this->start->setColor(0x00000000);
		this->option->setColor(0x00000000);
		this->end->setColor(0x00000000);
	//	幕の消滅
		this->gDev->setCurtainColor(0,0,0);
		this->gDev->setCurtainColorAlpha(255);
		this->gDev->curtainMotion(FALSE, 5);
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::initTitle
	//
	//	機能		タイトル画面命令
	//	更新		2008/09/20		<新規>
	//================================================================================
	void
	CSCENE_TITLE::initTitle(void)
	{
	//	タイトル画面に変更
		this->titleFlag	= eST_TITLE;
	//	その他の準備
		iStack			= 0;
		this->titleTex->setColor(0xFFFFFFFF);
		this->start->setColor(0xFFFFFFFF);
		this->option->setColor(0xFFFFFFFF);
		this->end->setColor(0xFFFFFFFF);
		this->gDev->setCurtainColorAlpha(0);
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::initStartSwitch
	//
	//	機能		ゲームスタート演出命令
	//	更新		2008/09/20		<新規>
	//================================================================================
	void
	CSCENE_TITLE::initStartSwitch(void)
	{
	//	ゲームスタート演出に変更
		this->titleFlag	= eST_START_SWITCH;
	//	その他の準備
		iStack			= 0;
		this->start->setLocZ(0.0f);
		this->titleTex->setColor(0xFFFFFFFF);
		this->start->setColor(0xFFFFFFFF);
		this->option->setColor(0xFFFFFFFF);
		this->end->setColor(0xFFFFFFFF);
		this->bStack	= FALSE;

	//	幕テクスチャを生成
		this->gDev->setCurtainColor(0, 0, 0);
		this->gDev->createCurtainTex("Loading_KageoKagemiA");
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::initEndSwitch
	//
	//	機能		ゲーム終了演出命令
	//	更新		2008/09/20		<新規>
	//================================================================================
	void
	CSCENE_TITLE::initEndSwitch(void)
	{
	//	ゲーム終了演出に変更
		this->titleFlag	= eST_END_SWITCH;
	//	その他の準備
		iStack			= 0;
		this->end->setLocZ(0.0f);
		this->titleTex->setColor(0xFFFFFFFF);
		this->start->setColor(0xFFFFFFFF);
		this->option->setColor(0xFFFFFFFF);
		this->end->setColor(0xFFFFFFFF);
		this->bStack	= FALSE;
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::initDemo
	//
	//	機能		デモ移行演出命令
	//	更新		2008/09/30		<新規>
	//================================================================================
	void
	CSCENE_TITLE::initDemo(void)
	{
	//	ゲーム終了演出に変更
		this->titleFlag	= eST_DEMO;
	//	その他の準備
		iStack			= 0;
		this->titleTex->setColorAlpha(255);
		this->start->setColorAlpha(255);
		this->option->setColorAlpha(255);
		this->end->setColorAlpha(255);
		this->bStack	= FALSE;
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::sceneInitialize
	//
	//	機能		初期化
	//	引数		inNum			外部データ番号
	//	更新		2008/09/01		<新規>
	//================================================================================
	void
	CSCENE_TITLE::sceneInitialize(int inStageNum)
	{
	//	初期解放
		this->sceneFinishalize();

	//-------------------------------------------------
	//	タイトルのインターフェースを読み込む
	//-------------------------------------------------
	//	タイトル背景準備
		this->titleBack		=
			new CTEXTURE(this->gDev->getDevice(),
						 "title_interface1",
						 VEC2(1024.0f,768.0f),
						 D3DCOLOR_RGBA(0, 0, 0, 100));
		this->titleBack->setFromRect(0, 0, 1024, 768);
		this->titleBack->setLoc(&VEC3((float)this->gDev->getScreenSizeX() / 2.0f,
									  (float)this->gDev->getScreenSizeY() / 2.0f,
									  1.0f));
	//	タイトル準備
		this->titleTex		=
			new CTEXTURE(this->gDev->getDevice(),
						 "title_interface2",
						 VEC2(640.0f,512.0f),
						 D3DCOLOR_RGBA(0, 0, 0, 100));
		this->titleTex->setFromRect(0, 0, 640, 512);
		this->titleTex->setLoc(&VEC3((float)this->gDev->getScreenSizeX() / 2.0f, 200.0f, 0.5f));

	//	「START」準備
		this->start		=	new CTEXTURE(this->titleTex);
		this->start->setToRect(VEC2(300.0f, 90.0f));
		this->start->setFromRect(640, 0, 1024, 128);
		this->start->setLoc(&VEC3(700.0f, CURSOR_START_POSY, 0.5f));

	//	「OPTION」準備
		this->option	=	new CTEXTURE(this->titleTex);
		this->option->setToRect(VEC2(300.0f, 90.0f));
		this->option->setFromRect(640, 256, 1024, 384);
		this->option->setLoc(&VEC3(700.0f, CURSOR_OPTION_POSY, 0.5f));

	//	「END」準備
		this->end		=	new CTEXTURE(this->titleTex);
		this->end->setToRect(VEC2(300.0f, 90.0f));
		this->end->setFromRect(640, 128, 1024, 256);
		this->end->setLoc(&VEC3(700.0f, CURSOR_END_POSY, 0.5f));

	//	カーソル準備
		this->cursor	=	new CTEXTURE(this->titleTex);
		this->cursor->setToRect(VEC2(128.0f, 128.0f));
		this->cursor->setFromRect(0, 512, 128, 640);
		this->cursor->setLoc(&VEC3(550.0f, CURSOR_START_POSY, 0.5f));

	//	コメント準備
		this->font	= CFONT_BASE::getInstance();
	//	コメント用のフォントが生成されて無ければ、生成する
		if(	!this->font->getFontValid(eFONT_PTN1))
		{
			this->font->createFont(
				this->gDev->getDevice(),
				eFONT_PTN1,
				"HG創英角ﾎﾟｯﾌﾟ体",
				40, 20, 2, 0);
		}
	//	文字列の登録
		this->font->registerText(
			"PUSH START BUTTOM",
			eFONT_PTN1,
			540, 450, 0.0f,
			D3DCOLOR_RGBA(255,255,255,255));
		this->font->registerText(
			"COPYRIGHT @ PROJECT KAGETOKI",
			eFONT_PTN1,
			450, 720, 0.0f,
			D3DCOLOR_RGBA(255,255,255,255));

	//	サウンドの準備
		this->sStaticCursor		= new SSTATIC(SOUND_CURSOR);
		this->sStaticDecision	= new SSTATIC(SOUND_DECISION1);
		this->sStaticDecision->setVolume(-1500);

	//	ここで使用するエフェクトの生成
		this->effect->addEffects(
			NAME_SHADOW, new C3D_EFFECT(this->gDev->getDevice(), NAME_SHADOW, VEC3(0.0f,0.0f,0.0f)));
		this->effect->addEffects(
			NAME_SHINE,  new C3D_EFFECT(this->gDev->getDevice(), NAME_SHINE,  VEC3(0.0f,0.0f,0.0f)));

	//	タイトル画面移行演出の実行
		this->initInitTitle();
	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::sceneFinishalize
	//
	//	機能		解放
	//	更新		2008/09/01		<新規>
	//================================================================================
	void
	CSCENE_TITLE::sceneFinishalize(void)
	{
	//	解放処理
		SAFE_DELETE(this->titleBack);
		SAFE_DELETE(this->titleTex);
		SAFE_DELETE(this->start);
		SAFE_DELETE(this->option);
		SAFE_DELETE(this->end);
		SAFE_DELETE(this->cursor);

	//	テキストの解放
		if(	this->font)	this->font->releaseText();

	//	サウンドデータの解放
		SAFE_DELETE(this->sStaticCursor);
		SAFE_DELETE(this->sStaticDecision);
		SAFE_DELETE(this->sStreamingBgm);

	//	ここで生成されたエフェクトを消去
		this->effect->finishalize();

	}

	//================================================================================
	//!	メソッド名	CSCENE_TITLE::sceneMain
	//
	//	機能		シーンメイン
	//	更新		2008/09/01		<新規>
	//================================================================================
	SCENE_FLAG
	CSCENE_TITLE::sceneMain(CINPUT* input)
	{
	//	描画開始
		this->gDev->renderBegin(D3DCOLOR_RGBA(0,0,0,255));
		this->gDev->spriteBegin();

	//	タイトル背景描画
		this->titleBack->draw(this->gDev->getSprite());
	//	フラグ分岐
		switch(this->titleFlag)
		{
		//	タイトル画面移行演出
			case	eST_INIT_TITLE:
				this->motionInitTitle(input);
				this->renderInitTitle();
				break;
		//	タイトル画面
			case	eST_TITLE:
				this->motionTitle(input);
				this->renderTitle();
				break;	
		//	ゲームスタート演出
			case	eST_START_SWITCH:
				this->motionStartSwitch(input);
				this->renderStartSwitch();
				break;		
		//	ゲーム終了演出
			case	eST_END_SWITCH:
				this->motionEndSwitch(input);
				this->renderEndSwitch();
				break;
		//	デモ移行演出
			case	eST_DEMO:
				this->motionDemo(input);
				this->renderDemo();
				break;
			default:
				break;
		}

	//	エフェクト描画
		this->effect->drawEffects2D(this->gDev->getSprite());

	//	描画終了
		this->gDev->spriteEnd();

	//	幕の描画
		this->gDev->renderCurtainInDev();
		this->gDev->renderEnd();

		return	this->returnFlag;
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------