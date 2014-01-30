//***********************************************************************************
//!	ファイル名		Class_SceneLogo.cpp
//
//	機能			ロゴシーンの実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2008/9/30	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneLogo.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_LOGO::CSCENE_LOGO
	//
	//	機能		コンストラクタ
	//	引数		inData			シーン共有データ
	//	更新		2008/09/30		<新規>
	//				2009/01/24		<変更>書き直し
	//================================================================================
	CSCENE_LOGO::CSCENE_LOGO(SCENE_SHARE_DATA* inData)	:
	CSCENE(eSCENE_LOGO, eSCENE_TITLE, inData)		//!< 基礎シーンフラグの設定
	{
	//	初期化
		this->gDev			= CGRAPHIC_DEVICE::getInstance();
		this->effectCenter	= C3D_EFFECT_CENTER::getInstance();
		this->effectCenter->finishalize();
		this->iFrameCnt		= 0;
		this->iStepCnt		= 0;
		this->logoJec		= NULL;
		this->logoProject	= NULL;
		this->sStaLogo		= NULL;
		this->bEndCheck		= FALSE;
	}

	//================================================================================
	//!	メソッド名	CSCENE_LOGO::~CSCENE_LOGO
	//
	//	機能		デストラクタ
	//	更新		2008/09/30		<新規>
	//				2009/01/24		<変更>書き直し
	//================================================================================
	CSCENE_LOGO::~CSCENE_LOGO(void)
	{
	//	解放
		sceneFinishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_LOGO::sceneInitialize
	//
	//	機能		初期化
	//	引数		inNum			外部データ番号
	//	更新		2008/09/30		<新規>
	//				2009/01/24		<変更>書き直し
	//================================================================================
	void
	CSCENE_LOGO::sceneInitialize(int inStageNum)
	{
	//	初期解放
		sceneFinishalize();

	//	日本電子専門学校ロゴの準備
		this->logoJec		=
			new CTEXTURE(gDev->getDevice(), "logo_jec", VEC2(512.0f, 512.0f));
		this->logoJec->setLoc(&VEC3(512.0f, 384.0f, 0.0f));

	//	プロジェクトロゴの準備
		this->logoProject	=
			new C3D_EFFECT(gDev->getDevice(), "logo_project", VEC3(512.0f, 384.0f, 0.0f));

	//	ロゴ出現音の準備
		this->sStaLogo		= new SSTATIC("logo");

	//	使用するエフェクトの生成
		this->effectCenter->addEffects(
			EFFECT_SHINE, new C3D_EFFECT(gDev->getDevice(), EFFECT_SHINE, VEC3(0.0f,0.0f,0.0f)));

	//	幕を消す
		gDev->setCurtainColor(0,0,0);
		gDev->setCurtainColorAlpha(255);
		gDev->curtainMotion(FALSE, 10);
	}

	//================================================================================
	//!	メソッド名	CSCENE_LOGO::sceneFinishalize
	//
	//	機能		解放
	//	更新		2008/09/30		<新規>
	//				2009/01/24		<変更>書き直し
	//================================================================================
	void
	CSCENE_LOGO::sceneFinishalize(void)
	{
	//	解放
		SAFE_DELETE(this->logoJec);
		SAFE_DELETE(this->logoProject);
		SAFE_DELETE(this->sStaLogo);
		this->effectCenter->finishalize();
	}

	//================================================================================
	//!	メソッド名	CSCENE_LOGO::initEndCheck
	//
	//	機能		終了準備
	//	更新		2009/02/24		<新規>
	//================================================================================
	void
	CSCENE_LOGO::initEndCheck(void)
	{
	//	終了準備
		if(	!this->bEndCheck)
		{
			this->bEndCheck	= TRUE;
		//	幕を出現させる
			gDev->curtainMotion(TRUE, 10);
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_LOGO::endCheck
	//
	//	機能		終了確認処理
	//	更新		2009/02/24		<新規>
	//================================================================================
	void
	CSCENE_LOGO::endCheck(void)
	{
	//	終了準備
		if(	this->bEndCheck)
		{
		//	ボリュームを下げる
			this->sStaLogo->setVolume(this->sStaLogo->getVolume() - 100);
			if(	gDev->getCurtainColorAlpha() >= 255)
			{
				returnFlag	= nextFlag;
			}
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_LOGO::sceneMain
	//
	//	機能		シーンメイン
	//	引数		input			入力情報
	//	更新		2008/09/30		<新規>
	//				2009/01/24		<変更>書き直し
	//================================================================================
	SCENE_FLAG
	CSCENE_LOGO::sceneMain(CINPUT* input)
	{
	//	処理
		execute(input);
	//	描画
		render();
	//	終了確認処理
		endCheck();

		return	returnFlag;
	}

	//================================================================================
	//!	メソッド名	CSCENE_LOGO::execute
	//
	//	機能		処理
	//	引数		input			入力情報
	//	更新		2009/01/24		<新規>
	//				2009/02/24		<更新>演出変更
	//================================================================================
	void
	CSCENE_LOGO::execute(CINPUT* input)
	{
	//	ステップカウンタにより、スキップ判定
		if(	(this->iStepCnt >= 0)	&&
			(this->iStepCnt <= 3))
		{
			if(	input->checkPressOnce(MYKEY::eDUST)		||
				input->checkPressOnce(MYKEY::eHARD)		||
				input->checkPressOnce(MYKEY::eMIDDLE)	||
				input->checkPressOnce(MYKEY::eSOFT)		||
				input->checkPressOnce(MYKEY::eSTART))
			{
				this->iStepCnt	= 4;
			}
		}
		else
		if(	(this->iStepCnt >= 4))
		{
			if(	input->checkPressOnce(MYKEY::eDUST)		||
				input->checkPressOnce(MYKEY::eHARD)		||
				input->checkPressOnce(MYKEY::eMIDDLE)	||
				input->checkPressOnce(MYKEY::eSOFT)		||
				input->checkPressOnce(MYKEY::eSTART))
			{
				this->initEndCheck();
			}
		}
	//	ステップにより、処理を行う
		switch(iStepCnt)
		{
		//	プロジェクトロゴを隠し、日本電子専門学校のロゴを透明にする
			case 0:
				logoProject->setVisible(FALSE);
				logoJec->setColorAlpha(0);
				iStepCnt	= 1;
				break;

		//	日本電子専門学校のロゴを少しずつ出現させる
			case 1:
				logoJec->calcColorAlpha(4);
				iFrameCnt++;
				if(	iFrameCnt >= 60)	{ iStepCnt = 2; iFrameCnt = 0;	}
				break;

		//	60フレーム待機
			case 2:
				iFrameCnt++;
				if(	iFrameCnt >= 60)	{ iStepCnt = 3; iFrameCnt = 0;	}
				break;

		//	日本電子専門学校のロゴを少しずつ消滅させる
			case 3:
				logoJec->calcColorAlpha(-10);
				iFrameCnt++;
				if(	iFrameCnt >= 50)	{ iStepCnt = 4; iFrameCnt = 0;	}
				break;

		//	日本電子専門学校のロゴの存在確認を無効にし、
		//	ロゴ出現音を発生
			case 4:
				logoJec->setVisible(FALSE);
				this->sStaLogo->play(0, NULL);
				iStepCnt	= 5;
				iFrameCnt	= 0;
				break;

		//	数フレーム待機し、
		//	プロジェクトロゴの存在確認を有効
			case 5:
				iFrameCnt++;
				if(	iFrameCnt >= 30)
				{ 
					iStepCnt	= 6;
					iFrameCnt	= 0;
					logoProject->setVisible(TRUE);
				}
				break;

		//	120フレーム待機
			case 6:
			//	3フレーム毎にエフェクト毎にエフェクト発生
				if(	!(iFrameCnt % 3))
				{
				//	位置決定
					VEC3	inLoc	= logoProject->getLoc();
					inLoc.x	+= float((rand() % (int)logoProject->getWidth())  - int(logoProject->getWidth()  * 0.5f));
					inLoc.y	+= float((rand() % (int)logoProject->getHeight()) - int(logoProject->getHeight() * 0.5f));
				//	スケール決定
					float	inScale	= logoProject->getWidth() / 400.0f;
					this->effectCenter->occu2DEffects(
						EFFECT_SHINE, inLoc, inScale, VEC3(0.0f,0.0f,0.0f), D3DRGBA(255,255,255,100));
				}
				iFrameCnt++;
				if(	iFrameCnt >= 360)
				{ 
					iStepCnt	= 7;
					iFrameCnt	= 0;
				//	終了準備へ移行
					this->initEndCheck();
				}
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_LOGO::render
	//
	//	機能		描画
	//	更新		2009/01/24		<新規>
	//================================================================================
	void
	CSCENE_LOGO::render(void)
	{
	//	描画開始
		gDev->renderBegin(D3DCOLOR_RGBA(255,255,255,0));
		gDev->spriteBegin();

		logoJec->draw(gDev->getSprite());					//!< 日本電子専門学校ロゴの描画
		logoProject->draw(gDev->getSprite());				//!< プロジェクトロゴの描画
		effectCenter->drawEffects2D(gDev->getSprite());		//!< エフェクト2Dの描画

		gDev->spriteEnd();
	//	幕の描画
		gDev->renderCurtainInDev();
		gDev->renderEnd();
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------