//***********************************************************************************
//!	ファイル名		Class_GraphicDevice.cpp
//
//	機能			グラフィック・描画関係の機能の実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/04/17	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_GraphicDevice.h"
//#include	"Class_ZTextureShader.h"
//#include	"Class_DepthBufShader.h"

//-----------------------------------------------------------------------
//!	グラフィックデバイスの名前空間
//-----------------------------------------------------------------------
namespace GD
{
	//================================================================================
	//	インスタンス生成場所
	//================================================================================
	CGRAPHIC_DEVICE* CGRAPHIC_DEVICE::gUniqueInstance = NULL;		

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::コンストラクタ
	//
	//	更新		2008/04/18		<新規>
	//================================================================================
	CGRAPHIC_DEVICE::CGRAPHIC_DEVICE(void)
	:	d3D9(NULL),				//!< Direct3D
		dev(NULL),				//!< Direct3Dデバイス
		d3dsp(NULL),			//!< スプライトデータ
		surface(NULL),			//!< サーフェス
		curtainTex(NULL)		//!< 幕のテクスチャ
	{
		initialize();
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::デストラクタ
	//
	//	更新		2008/04/18		<新規>
	//================================================================================
	CGRAPHIC_DEVICE::~CGRAPHIC_DEVICE(void)
	{
		finishalize();
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::getInstance
	//
	//	機能		インスタンスの生成
	//	戻り値		インスタンスを返す
	//	更新		2008/04/18		<新規>
	//================================================================================
	CGRAPHIC_DEVICE*
	CGRAPHIC_DEVICE::getInstance(void)
	{
	//!	生成していなかった場合
		if(!gUniqueInstance)
		{
		//!	インスタンス生成
			gUniqueInstance = new CGRAPHIC_DEVICE();
		}
	//!	インスタンスを返す
		return gUniqueInstance;
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::releaseInstance
	//
	//	機能		インスタンスの解放
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::releaseInstance(void)
	{
		SAFE_DELETE(gUniqueInstance);	//!< インスタンスの解放
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::initialize
	//
	//	機能		初期化処理
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::initialize(void)
	{
	//	先に解放
		finishalize();

	//	各パラメータの初期化
		ZeroMemory(&d3dpp,	sizeof(D3DPP));			//!< プレゼンテーションパラメータ
		ZeroMemory(&caps,	sizeof(D3DCAPS9));		//!< Caps情報

		hWnd			= NULL;						//!< ターゲットウィンドウハンドル
		iScreenSizeX	= 0;						//!< スクリーンサイズ横幅
		iScreenSizeY	= 0;						//!< スクリーンサイズ縦幅
		iRefRate		= 60;						//!< リフレッシュレート
		bFullScreen		= FALSE;					//!< フルスクリーン確認フラグ
		dwWindowStyle	= 0;						//!< ウィンドウスタイル情報
		bCurtainAction	= FALSE;					//!< 幕の動作確認
		bCurtainAppear	= FALSE;					//!< 幕の透明確認
		curtainColor	= MY_COLOR_RGBA();			//!< 幕のカラー
		curtainTex		= NULL;						//!< 幕のテクスチャ
		curtainZLoc		= 0.0f;						//!< 幕のプライオリティ
		surface			= NULL;						//!< サーフェス
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::finishalize
	//
	//	機能		解放処理
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::finishalize(void)
	{
	//	各パラメータ解放
		SAFE_RELEASE(surface);			//!< サーフェス解放
		SAFE_RELEASE(d3dsp);			//!< スプライトデバイス解放
		SAFE_RELEASE(dev);				//!< Direct3Dデバイス解放
		SAFE_RELEASE(d3D9);				//!< DirectX解放
		SAFE_RELEASE(curtainTex);		//!< 幕のテクスチャ
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::initDevice
	//
	//	機能		デバイスの準備
	//	引数		inWnd			ウィンドウハンドル
	//				inScreenSizeX	スクリーンサイズ横幅
	//				inScreenSizeY	スクリーンサイズ縦幅
	//				inFullScreen	フルスクリーン確認フラグ
	//	戻り値		TRUE			成功
	//				FALSE			失敗
	//	更新		2008/04/18		<新規>
	//================================================================================
	BOOL
	CGRAPHIC_DEVICE::initDevice(HWND inWnd, int inScreenSizeX, int inScreenSizeY, BOOL inFullScreen)
	{
	//	各種情報の確保
		hWnd			= inWnd;
		iScreenSizeX	= inScreenSizeX;
		iScreenSizeY	= inScreenSizeY;
		bFullScreen		= inFullScreen;

	//	Direct3Dオブジェクトの生成
		d3D9 = Direct3DCreate9(D3D_SDK_VERSION);
		if(	d3D9 == NULL)
		{
		//	失敗したらエラー報告
			errMessage(eGDERR_CREATE3D);
			return FALSE;		//!< 失敗を返す
		}
	//	デバイスの能力を確認
		d3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	//	補足ウィンドウメモリの指定されたオフセット位置に32ビット値を設定
		dwWindowStyle	= GetWindowLong(hWnd, GWL_STYLE);
	//	プレゼンテーションパラメータの設定(Present関数に影響)
		setParam();

	//	デバイスの生成
		if(	!createDevice())
		{
		//	失敗したらエラー報告
			errMessage(eGDERR_CREATE3DDEVICE);
			return FALSE;		//!< 失敗を返す
		}
	//	ステータスの設定
		setState();
	//	各種管理クラスを生成する
		createManageClass();

	//	設定正常終了
		return TRUE;
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::setParam
	//
	//	機能		プレゼンテーションパラメータを設定
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::setParam(void)
	{
	//	ディスプレイの状態を確保
		D3DDISPLAYMODE	dispMode;
		if(	FAILED(d3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dispMode)))
		{
		//	失敗したらエラー報告
			errMessage(eGDERR_NOGETDISPLAYMODE);
		}

		ZeroMemory(&d3dpp,	sizeof(D3DPP));
	//	ウィンドウスタイルとサイズの設定
		if(	bFullScreen)
		{
		//	リフレッシュレートをフルスクリーン用に設定
			d3dpp.Windowed						= FALSE;				//!< ウィンドウモードではない
			d3dpp.BackBufferFormat				= dispMode.Format;		//!< バックバッファ設定
		//	d3dpp.FullScreen_RefreshRateInHz	= 0;
		//	d3dpp.PresentationInterval			= 1;
		}
		else
		{
		//	作業用 新たなウィンドウの位置
			RECT rcWindowSize =
			{
				(GetSystemMetrics(SM_CXSCREEN) / 2) - ((iScreenSizeX	+ 4)  / 2),	//!< ウィンドウ位置X
				(GetSystemMetrics(SM_CYSCREEN) / 2) - ((iScreenSizeY	+ 29) / 2),	//!< ウィンドウ位置Y
				iScreenSizeX ,														//!< ウィンドウ幅
				iScreenSizeY,														//!< ウィンドウ高さ
			};
		//	ウィンドウの位置,Zオーダーを設定
			SetWindowPos(hWnd, HWND_NOTOPMOST, 
				rcWindowSize.left,
				rcWindowSize.top,
				rcWindowSize.right,
				rcWindowSize.bottom,
				SWP_SHOWWINDOW);
		//	リフレッシュレートをウィンドウモード用に設定
			d3dpp.Windowed				= TRUE;							//!< ウィンドウモード
			d3dpp.BackBufferFormat		= dispMode.Format;				//!< バックバッファは画面依存
		}

		d3dpp.BackBufferWidth			= iScreenSizeX;					//!< バックバッファの横幅
		d3dpp.BackBufferHeight			= iScreenSizeY;					//!< バックバッファの縦幅
		d3dpp.BackBufferCount			= 1;							//!< バックバッファの数
		d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;		//!< スワップチェーンの管理フラグ
		d3dpp.EnableAutoDepthStencil	= TRUE;							//!< 深度バッファを管理する
		d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;					//!< 16ビットフォーマット
		d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;			//!< マルチサンプルタイプ

		d3dpp.Flags	= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::createDevice
	//
	//	機能		デバイスの生成
	//	戻り値		TRUE			成功
	//				FALSE			失敗
	//	更新		2008/04/18		<新規>
	//================================================================================
	BOOL
	CGRAPHIC_DEVICE::createDevice(void)
	{
	//!	HALで生成（ハードウェア＆ソフトウェアによる頂点処理）
		if(	!dev)
		{
			if(FAILED(	d3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
						D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &dev)))
			{
			//!	HALで生成（ハードウェアによる頂点処理）
				if(FAILED(	d3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
							D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &dev)))
				{
				//!	HALで生成（ソフトウェアによる頂点処理）
					if(FAILED(	d3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
								D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &dev)))
					{
					//!	REFで生成
						if(FAILED(	d3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &dev)))
						{
						//!	生成失敗
							return FALSE;
						}
					}
				}
			}
		}
		else
		{
		//	既にデバイスがある時はリセット
			if(	FAILED(dev->Reset(&d3dpp)))
			{
				return	FALSE;
			}
		}
	//	生成成功
		return TRUE;
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::setState
	//
	//	機能		ステータスを設定
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::setState(void)
	{
	//	標準ステータス
		dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			//!< 裏ポリゴンを表示しない
		dev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);				//!< Zバッファを利用する
		dev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				//!< 深度バッファへの書き込みを有効にする
		dev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);		//!< グローシェーディングモード
	//	アルファブレンディングの設定
		dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//	アルファテストの設定
		dev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//!< アルファテストを行う
		dev->SetRenderState(D3DRS_ALPHAREF, 0x00000001);			//!< アルファ値の設定
		dev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	//!< アルファ設定
	//	アルファ成分の設定
		dev->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	//	ライトの設定
		dev->SetRenderState(D3DRS_LIGHTING, TRUE);
		dev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);			//!< 頂点法線の自動正規化を有効
		dev->SetRenderState(D3DRS_DITHERENABLE, TRUE);				//!< ディザリングを行う（高品質描画）
	//	描画関連ステート
		dev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	//!< ジャギ取り
	//	dev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//!< ジャギ取り
		dev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);	//!< ジャギ取り
		dev->SetRenderState(D3DRS_COLORVERTEX, TRUE);				//!< 頂点カラーの設定
		dev->SetRenderState(D3DRS_LASTPIXEL, FALSE);				//!< 最後の1ピクセルを描画しない

		//dev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
		//dev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		//dev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

		dev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRRORONCE);
		dev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRRORONCE);

		setLight(true, 0, VEC3( 1.0f,  1.0f,  1.0f));			//!< ライトの設定(全方向)
		setLight(true, 1, VEC3(-1.0f,  1.0f, -1.0f));			//!< ライトの設定
		setLight(true, 2, VEC3( 1.0f, -1.0f, -1.0f));			//!< ライトの設定
		setLight(true, 3, VEC3(-1.0f, -1.0f,  1.0f));			//!< ライトの設定

	//	キャップスのステータス設定
		caps.RasterCaps	= D3DPRASTERCAPS_WFOG;
	//	スプライトデバイスに一回解放処理を行う
		SAFE_RELEASE(d3dsp);
	//	スプライトレンダリングオブジェクトの生成
		D3DXCreateSprite(dev, &d3dsp);
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::setCurtainColor
	//
	//	機能		幕のカラーの設定
	//	引数		inRed			赤成分
	//				inGreen			緑成分
	//				inBlue			青成分
	//	更新		2008/09/30		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::setCurtainColor(int inRed, int inGreen, int inBlue)
	{
		curtainColor.rgba.red	= BYTE(inRed);
		curtainColor.rgba.green	= BYTE(inGreen);
		curtainColor.rgba.blue	= BYTE(inBlue);
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::calcCurtainColor
	//
	//	機能		幕のカラーの演算
	//	引数		inRed			赤成分
	//				inGreen			緑成分
	//				inBlue			青成分
	//	更新		2008/09/30		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::calcCurtainColor(int inRed, int inGreen, int inBlue)
	{
	//	RGBAカラーの加減算
		inRed	=	(int)curtainColor.rgba.red		+ inRed;
		inGreen	=	(int)curtainColor.rgba.green	+ inGreen;
		inBlue	=	(int)curtainColor.rgba.blue		+ inBlue;
	//	範囲設定
		if(	inRed   < 0){ inRed   = 0;	}	else if(inRed   > 255){ inRed   = 255;	}
		if(	inGreen < 0){ inGreen = 0;	}	else if(inGreen > 255){ inGreen = 255;	}
		if(	inBlue  < 0){ inBlue  = 0;	}	else if(inBlue  > 255){ inBlue  = 255;	}
	//	改めて設定
		setCurtainColor(inRed,inGreen,inBlue);
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::setState
	//
	//	機能		ライトの設定を行う
	//	引数		bLight			ライトを使用するかどうかのフラグ
	//				inStage			指定ステージ(配列上の位置)
	//				inLoc			位置
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::setLight(bool bLight, int inStage, VEC3 inLoc)
	{
	//	ライトを作る
		if(	bLight)
		{
			D3DLIGHT9		light;										//!< ライト情報構造体
			D3DCOLORVALUE	diffuse		= {1.0f, 1.0f, 1.0f, 1.0f};		//!< 色
			D3DCOLORVALUE	specular	= {0.5f, 0.5f, 0.5f, 0.5f};		//!< 反射角

			ZeroMemory(&light, sizeof(D3DLIGHT9));						//!< ライト情報の初期化

		//	ディクショナリライト
			light.Type			= D3DLIGHT_DIRECTIONAL;		//!< ポイントライト
			light.Diffuse		= diffuse;					//!< 色を設定
			light.Direction		= inLoc;					//!< 位置を設定
			light.Specular		= specular;					//!< 反射角を設定

			dev->SetLight(inStage, &light);					//!< 指定したステージにライトを設定
			dev->LightEnable(inStage, TRUE);				//!< 指定したステージのライトをオン
		}
	//	ライトを消す
		else
		{
			dev->SetLight(inStage, NULL);					//!< 指定したステージのライトを消去
			dev->LightEnable(inStage, FALSE);				//!< 指定したステージのライトをオフ
		}
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::createManageClass
	//
	//	機能		各種管理するクラスを生成する
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::createManageClass(void)
	{
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::setFog
	//
	//	機能		フォグの設定をする
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::setFog(void)
	{
		const DWORD	dwFogColor	= D3DCOLOR_ARGB(255,255,255,255);			//!< フォグの色
		const float fFogStart	= 0.0f;										//!< フォグ開始位置
		const float fFogEnd		= 10000.0f;									//!< フォグ最終位置
	//! ※D3DRS_FOGVERTEXMODE	→　頂点フォグ
	//! ※D3DRS_FOGTABLEMODE	→　ピクセルフォグ
		const D3DRENDERSTATETYPE fogType	= D3DRS_FOGTABLEMODE;			//!< フォグのタイプ

		dev->SetRenderState(D3DRS_FOGENABLE,	TRUE);						//!< フォグを使用する
		dev->SetRenderState(D3DRS_FOGCOLOR,		dwFogColor);				//!< フォグの色を設定
		dev->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_EXP);				//!< フォグのタイプ設定
		dev->SetRenderState(fogType,			D3DFOG_LINEAR);				//!< フォグのタイプ設定
		dev->SetRenderState(D3DRS_FOGSTART,		*(DWORD*)(&fFogStart));		//!< フォグの開始位置設定
		dev->SetRenderState(D3DRS_FOGEND,		*(DWORD*)(&fFogEnd));		//!< フォグの最終位置設定
		dev->SetRenderState(D3DRS_AMBIENT,		D3DCOLOR_XRGB(255, 255, 255));
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::createCurtainTex
	//
	//	機能		幕テクスチャを生成する
	//	引数		inTexName		ロード画面に使うテクスチャ名
	//	更新		2008/12/11		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::createCurtainTex(LPSTR inTexName)
	{
	//	幕テクスチャの解放
		SAFE_RELEASE(curtainTex);
	//	テクスチャが指定されていれば、幕テクスチャを生成
		if(	inTexName)
		{
		//	テクスチャ名にパスをつける
			char	realName[MAX_PATH]	= "";
		//	失敗したら、リソースファイルへのパスをつけてトライ
			sprintf_s(realName, sizeof(realName), "..//Resources//LoadingTexture//%s.dds", inTexName);

		//	テクスチャを読み込んでみる
			if(	FAILED(D3DX_CREATE_TEXTURE_EX(
					dev, realName, D3DCOLOR_RGBA(255,255,255,100),
					NULL, &curtainTex)))
			{
			//	失敗したら、NULLを入れる
				curtainTex	= NULL;
			}
		}
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::renderCurtain
	//
	//	機能		塗りつぶし描画
	//	引数		inColor
	//	更新		2008/09/26		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::renderCurtain(TEX inTex, DWORD inColor)
	{
	//	全体描画用頂点
		CUSTOM_VERTEX_2D	surface[4]	=
		{
			{			   -1.0f,				-1.0f, curtainZLoc, 0.0f, inColor, 0.0f, 0.0f },
			{(float)iScreenSizeX,				-1.0f, curtainZLoc, 0.0f, inColor, 1.0f, 0.0f },
			{(float)iScreenSizeX, (float)iScreenSizeY, curtainZLoc, 0.0f, inColor, 1.0f, 0.75f},
			{			   -1.0f, (float)iScreenSizeY, curtainZLoc, 0.0f, inColor, 0.0f, 0.75f}
		};
		DWORD	dwFVF=0;	//!< FVFのバックアップ用

	//	ポリゴン描画
		dev->SetTexture(0, inTex);
		dev->GetFVF(&dwFVF);
		dev->SetFVF(D3DFVF_VERTEX_2D);
		dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, surface, sizeof(CUSTOM_VERTEX_2D));
		dev->SetFVF(dwFVF);
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::renderCurtainInDev
	//
	//	機能		塗りつぶし描画
	//	機能概要	スプライト起動時は意味が無い
	//	引数		inColor
	//	更新		2008/09/26		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::renderCurtainInDev(void)
	{
	//!	幕の動作処理
		this->curtainMotionExecute();

	//	幕のアルファが255以上なら、テクスチャで幕を描画
		if(	curtainColor.rgba.alpha >= 255	&&
			(this->curtainTex))
		{
			renderCurtain(curtainTex, D3DCOLOR_RGBA(255, 255, 255, 255));
		}
	//	幕のアルファが0より上なら、幕を描画
		else
		if(	curtainColor.rgba.alpha > 0)
		{
			renderCurtain(NULL, curtainColor.color);
		}
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::curtainMotionExecute
	//
	//	機能		幕の動作処理
	//	更新		2009/02/12		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::curtainMotionExecute(void)
	{
	//!	幕の動作確認
 		if(	bCurtainAction)
		{
			int	inAlpha	 = (int)curtainColor.rgba.alpha;
		//	幕の確認が有効なら、出現
			if(	bCurtainAppear)
			{
				inAlpha	+= iAlphaSpeed;
			//	アルファが最大になったら、動作確認を無効
				if(	inAlpha	> 255)
				{
					bCurtainAction	= FALSE;
					inAlpha			= 255;
				}
			}
		//	無効なら、透明化
			else
			{
				inAlpha	-= iAlphaSpeed;
			//	アルファが最小になったら、動作確認を無効
				if(	inAlpha < 0)
				{
					bCurtainAction	= FALSE;
					inAlpha			= 0;
				}
			}
		//	アルファのセット
			curtainColor.rgba.alpha	= BYTE(inAlpha);
		}
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::curtainMotion
	//
	//	機能		幕の動作
	//	機能概要	幕を張るか、幕を消滅させる
	//	引数		inBoolen		TRUE(幕を張る) : FALSE(幕を消滅)
	//				inAlpha			アルファ値変動値
	//	更新		2008/09/26		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::curtainMotion(BOOL inBoolen, int inAlpha)
	{
	//	幕の動作確認を有効
		bCurtainAction	= TRUE;
	//	幕の出現確認を確保
		bCurtainAppear	= inBoolen;
	//	幕の透明度変動値を確保
		iAlphaSpeed		= inAlpha;
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::errMessage
	//
	//	機能		エラー報告
	//	引数		err				エラーの種類
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::errMessage(GD_ERROR err)
	{
		char	szErr[MAX_PATH] = "";
	//	エラー文を生成
		switch(err){
			case eGDERR_CREATE3D:			wsprintf(szErr, "Direct3Dの生成に失敗");				break;
			case eGDERR_CREATE3DDEVICE:		wsprintf(szErr, "Direct3Dデバイスの生成に失敗");		break;
			case eGDERR_RESETDEVICE:		wsprintf(szErr, "デバイスの再起動に失敗");				break;
			case eGDERR_BUFFERCLEAR:		wsprintf(szErr, "バッファがクリアされない");			break;
			case eGDERR_BEGINSCENE:			wsprintf(szErr, "シーンを開始できない");				break;
			case eGDERR_BEGINSPRITE:		wsprintf(szErr, "スプライトを開始できない");			break;
			case eGDERR_ENDSPRITE:			wsprintf(szErr, "スプライトを終了できない");			break;
			case eGDERR_ENDSCENE:			wsprintf(szErr, "シーンを終了できない");				break;
			case eGDERR_NOGETDISPLAYMODE:	wsprintf(szErr, "ディスプレイの状態を取得できない");	break;
			default:						wsprintf(szErr, "エラーの種類が未確定");				break;
		}
	//	メッセージボックスの表示
		MessageBox(hWnd, szErr, "GraphicDeviceエラー", MB_OK);
		//!	プログラム終了へ
		SendMessage(hWnd, WM_DESTROY, 0, 0);
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::renewalScreenMode
	//
	//	機能		スクリーンの更新
	//	引数		inWnd			ウィンドウハンドル
	//				inFullScreen	フルスクリーン確認フラグ
	//				inScreenSizeX	スクリーンサイズ横幅
	//				inScreenSizeY	スクリーンサイズ縦幅
	//	戻り値		TRUE			成功
	//				FALSE			失敗
	//	更新		2008/04/19		<新規>
	//================================================================================
	BOOL
	CGRAPHIC_DEVICE::renewalScreenMode(HWND inWnd, BOOL inFullScreen, int inScreenSizeX, int inScreenSizeY)
	{
	//	各種情報の確保
		hWnd	= inWnd;
	//	ウィンドウサイズを変更するか判定する
		if(	inScreenSizeX != 0 && inScreenSizeY != 0)
		{
			iScreenSizeX	= inScreenSizeX;
			iScreenSizeY	= inScreenSizeY;
		}
	//	サイズ変更でない場合はフルスクリーン変更と判断
		else
		{
			bFullScreen	= inFullScreen;
		}
	//	パラメータの再設定
		setParam();
	//	デバイスをリセット
		if(	createDevice())
		{
		//	成功したらステータスの初期化
			setState();
			return	TRUE;
		}
		else
		{
		//	失敗したらスリープ
			Sleep(1);
			return	FALSE;
		}
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::bufferClear
	//
	//	機能		バッファのクリアを行う
	//	引数		dwColor			バッファクリア時に塗りつぶす色
	//	戻り値		S_OK			成功
	//				E_FAIL			失敗
	//	更新		2008/04/19		<新規>
	//================================================================================
	HRESULT
	CGRAPHIC_DEVICE::bufferClear(DWORD dwColor)
	{
	//!	バッファのクリア
		if(FAILED(dev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, dwColor, 1.0f, 0)))
		{
		//	失敗したらエラー報告
			errMessage(eGDERR_BUFFERCLEAR);
			return E_FAIL;		//!	失敗を返す
		}
		return S_OK;			//!< 成功を返す
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::renderBegin
	//
	//	機能		レンダリングの準備を行う
	//	引数		dwColor			バッファクリア時に塗りつぶす色
	//	戻り値		S_OK			成功
	//				E_FAIL			失敗
	//	更新		2008/04/19		<新規>
	//================================================================================
	HRESULT
	CGRAPHIC_DEVICE::renderBegin(DWORD dwColor)
	{
	//!	バッファのクリア
		if(FAILED(dev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, dwColor, 1.0f, 0)))
		{
		//	失敗したらエラー報告
			errMessage(eGDERR_BUFFERCLEAR);
			return E_FAIL;		//!	失敗を返す
		}
	//!	シーンの開始
		if(FAILED(dev->BeginScene()))
		{
		//	失敗したらエラー報告
			errMessage(eGDERR_BEGINSCENE);
			return E_FAIL;		//!< 失敗を返す
		}
		return S_OK;			//!< 成功を返す
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::renderEnd
	//
	//	機能		レンダリングの更新を行う
	//	戻り値		S_OK			成功
	//				E_FAIL			失敗
	//	更新		2008/04/19		<新規>
	//================================================================================
	HRESULT
	CGRAPHIC_DEVICE::renderEnd(void)
	{
	//!	シーンの終了
		if(FAILED(dev->EndScene()))
		{
		//	失敗したらエラー報告
			errMessage(eGDERR_ENDSCENE);
			return E_FAIL;				//!< 失敗を返す
		}
	//!	バックバッファから転送
		if(FAILED(dev->Present(NULL, NULL, NULL, NULL)))
		{
			resetDevice();				//!< デバイスをリセットする
			return E_FAIL;				//!< 失敗を返す
		}
		return S_OK;					//!< 成功を返す
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::spriteBegin
	//
	//	機能		スプライトを開始
	//	戻り値		S_OK			成功
	//				E_FAIL			失敗
	//	更新		2008/04/19		<新規>
	//================================================================================
	HRESULT
	CGRAPHIC_DEVICE::spriteBegin(void)
	{
	//!	スプライトの描画を許可
		if(FAILED(d3dsp->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK)))
		{
		//	失敗したらエラー報告
			errMessage(eGDERR_BEGINSPRITE);
			return E_FAIL;				//!< 失敗を返す
		}
		return S_OK;					//!< 成功を返す
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::spriteBegin
	//
	//	機能		スプライトを終了
	//	戻り値		S_OK			成功
	//				E_FAIL			失敗
	//	更新		2008/04/19		<新規>
	//================================================================================
	HRESULT
	CGRAPHIC_DEVICE::spriteEnd(void)
	{
	//!	スプライト描画終了
		if(FAILED(d3dsp->End()))
		{
		//	失敗したらエラー報告
			errMessage(eGDERR_ENDSPRITE);
			return E_FAIL;				//!< 失敗を返す
		}
		return S_OK;					//!< 成功を返す
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::setFogEnable
	//
	//	機能		フォグの使用・不使用設定
	//	引数		bEnable			フォグの使用・不使用の変更
	//	更新		2008/04/19		<新規>
	//================================================================================
	void
	CGRAPHIC_DEVICE::setFogEnable(const BOOL bEnable)
	{
		dev->SetRenderState(D3DRS_FOGENABLE,	  bEnable);	//!< フォグを使用・不使用変更
		dev->SetRenderState(D3DRS_RANGEFOGENABLE, bEnable);	//!< フォグを有効・無効変更
	}

	//================================================================================
	//!	メソッド名	CGRAPHIC_DEVICE::resetDevice
	//
	//	機能		デバイスをリセットする
	//	戻り値		TRUE			成功
	//				FALSE			失敗
	//	更新		2008/04/18		<新規>
	//================================================================================
	BOOL
	CGRAPHIC_DEVICE::resetDevice(void)
	{
		FILE*	fp	= NULL;
	//	デバイスが消失していたら再構築する
		switch(dev->TestCooperativeLevel())
		{
			case D3DERR_DEVICELOST:			//!< リセット出来ない
				Sleep(50);
				return	FALSE;
			case D3DERR_DEVICENOTRESET:		//!< リセット出来る
			//	その他リセット
				setParam();					//!< パラメータの再設定
				if(	createDevice())			//!< デバイスの再生成(リセット)
				{
					CZTEX_SHA::resetInstance();
					CDEPTH_SHA::resetInstance();
				//	Zテクスチャを共有する
					CDEPTH_SHA::getInstance()->setEffectTexture(
						CZTEX_SHA::getInstance()->getZTex());
				//	成功したらステータスのリセット
					setState();
					return	TRUE;
				}
				break;
		}
		return	FALSE;
	}
}
//-----------------------------------------------------------------------
//!	グラフィックデバイスの名前空間
//-----------------------------------------------------------------------