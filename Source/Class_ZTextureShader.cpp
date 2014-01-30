//***********************************************************************************
//!	ファイル名		Class_ZTextureShader.cpp
//
//	機能			Zテクスチャを作成するクラスの実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/12/14	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_ZTextureShader.h"

//-----------------------------------------------------------------------
//!	シェーダの名前空間
//-----------------------------------------------------------------------
namespace SHA
{
	//================================================================================
	//	インスタンス生成場所
	//================================================================================
	CZTEXTURE_SHADER* CZTEXTURE_SHADER::gUniqueInstance = NULL;		

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::コンストラクタ
	//
	//	更新		2008/12/14	<新規>
	//================================================================================
	CZTEXTURE_SHADER::CZTEXTURE_SHADER(void)
	{
	//	属性の初期化
		dev							= NULL;
		zTex						= NULL;
		zTexSurf					= NULL;
		depthBufferSurf				= NULL;
		devBufferSurf				= NULL;
		devDepthBufferSurf			= NULL;
		effect						= NULL;
		D3DXMatrixIdentity(&matrixLightViewProjection);
		hWorldLightViewProjectionMat= NULL;
		hTechnique					= NULL;
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::デストラクタ
	//
	//	更新		2008/12/14	<新規>
	//================================================================================
	CZTEXTURE_SHADER::~CZTEXTURE_SHADER(void)
	{
	//	属性の解放
		SAFE_RELEASE(effect);
		SAFE_RELEASE(devDepthBufferSurf);
		SAFE_RELEASE(devBufferSurf);
		SAFE_RELEASE(depthBufferSurf);
		SAFE_RELEASE(zTexSurf);
		SAFE_RELEASE(zTex);
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::getInstance
	//
	//	機能		インスタンスの生成
	//	戻り値		インスタンスを返す
	//	更新		2008/12/14	<新規>
	//================================================================================
	CZTEXTURE_SHADER*
	CZTEXTURE_SHADER::getInstance(void)
	{
	//!	生成していなかった場合
		if(!gUniqueInstance)
		{
		//!	インスタンス生成
			gUniqueInstance = new CZTEXTURE_SHADER();
		}
	//!	インスタンスを返す
		return gUniqueInstance;
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::releaseInstance
	//
	//	機能		インスタンスの解放
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	CZTEXTURE_SHADER::releaseInstance(void)
	{
		SAFE_DELETE(gUniqueInstance);	//!< インスタンスの解放
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::lostInstance
	//
	//	機能		インスタンスロスト
	//	更新		2008/12/18		<新規>
	//================================================================================
	void
	CZTEXTURE_SHADER::lostInstance(void)
	{
	//!	生成されてたらエフェクトロスト
		if(	gUniqueInstance)
		{
			gUniqueInstance->lostExecute();
		}
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::resetInstance
	//
	//	機能		インスタンスリセット
	//	更新		2008/12/18		<新規>
	//================================================================================
	void
	CZTEXTURE_SHADER::resetInstance(void)
	{
	//!	生成されてたらエフェクトリセット
		if(	gUniqueInstance)
		{
			gUniqueInstance->resetExecute();
		}
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::initialize
	//
	//	機能		初期化
	//	引数		inDev			Direct3Dデバイス
	//				inZTexWidth		生成するテクスチャの横幅
	//				inZTexHeight	生成するテクスチャの縦幅
	//				inZTexFormat	生成するテクスチャの形式
	//	戻り値		TRUE			初期化成功
	//				FALSE			初期化失敗
	//	更新		2008/12/14		<新規>
	//================================================================================
	BOOL
	CZTEXTURE_SHADER::initialize(DEV inDev, UINT inZTexWidth, UINT inZTexHeight)
	{
	//	エラー回避
		if(	!inDev)									return	FALSE;
		if(	inZTexWidth == 0 || inZTexHeight == 0)	return	FALSE;

	//	初期解放
		finitialize();

	//	エフェクトファイルからZテクスチャプログラムを取得
		if(	FAILED(D3DXCreateEffectFromFile(
				inDev, PATH_ZTEXSHADER, NULL, NULL, 0, NULL,
				&effect, NULL)))
		{
			return	FALSE;
		}

	//	エフェクト内の各種パラメータハンドルを取得
		hWorldLightViewProjectionMat	= effect->GetParameterByName(NULL, "mat");
		hTechnique= effect->GetTechniqueByName("ZValuePlotTec");
		if(	!hWorldLightViewProjectionMat		||
			!hTechnique)
		{
			return	FALSE;
		}

	//	指定のZ値テクスチャを生成
		if(	FAILED(D3DXCreateTexture(
				inDev, inZTexWidth, inZTexHeight, 1,
				D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
				D3DPOOL_DEFAULT, &zTex)))
		{
			return	FALSE;
		}

	//	サーフェイス取得
		zTex->GetSurfaceLevel(0, &zTexSurf);

	//	描画デバイスに定義されているバッファの能力を取得
		SURF	pSurf;
		inDev->GetDepthStencilSurface(&pSurf);
		D3DSURFACE_DESC	desc;
		pSurf->GetDesc(&desc);
		pSurf->Release();

	//	独自深度バッファを作成
		if( FAILED(inDev->CreateDepthStencilSurface(
				inZTexWidth, inZTexHeight, desc.Format,
				desc.MultiSampleType, desc.MultiSampleQuality,
				TRUE, &depthBufferSurf, NULL)))
		{
			return	FALSE;
		}

	//	情報を確保
		dev			= inDev;
		zTexWidth	= inZTexWidth;
		zTexHeight	= inZTexHeight;

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::finitialize
	//
	//	機能		解放
	//	更新		2009/01/22		<新規>
	//================================================================================
	void
	CZTEXTURE_SHADER::finitialize(void)
	{
		SAFE_RELEASE(effect);
		SAFE_RELEASE(devDepthBufferSurf);
		SAFE_RELEASE(devBufferSurf);
		SAFE_RELEASE(depthBufferSurf);
		SAFE_RELEASE(zTexSurf);
		SAFE_RELEASE(zTex);
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::lostExecute
	//
	//	機能		デバイスロスト時の処理
	//	更新		2009/03/03		<新規>
	//================================================================================
	void
	CZTEXTURE_SHADER::lostExecute(void)
	{
	//	エフェクトを一時的にロストする
		this->effect->OnLostDevice();
	//	データを解放
		this->finitialize();
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::resetExecute
	//
	//	機能		デバイスリセット時の処理
	//	更新		2009/03/03		<新規>
	//================================================================================
	void
	CZTEXTURE_SHADER::resetExecute(void)
	{
	//	エフェクトを作り直す
		this->initialize(this->dev, this->zTexWidth, this->zTexHeight);
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::setWorldMatrix
	//
	//	機能		描画対象オブジェクトのワールド変換行列を設定
	//	引数		inMat		描画対象のワールドマトリックス
	//	更新		2008/12/14	<新規>
	//================================================================================
	void
	CZTEXTURE_SHADER::setWorldMatrix(MAT *inMat)
	{
		effect->SetMatrix(
			hWorldLightViewProjectionMat,
			&((*inMat) * matrixLightViewProjection));
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::setViewMatrix
	//
	//	機能		ビュー行列を設定
	//	引数		inMat		ビュー行列
	//	更新		2008/12/14	<新規>
	//================================================================================
	void
	CZTEXTURE_SHADER::setLightViewProjMatrix(MAT *inMat)
	{
		matrixLightViewProjection	= *inMat;
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::begin
	//
	//	機能		描画の開始を宣言する
	//	戻り値		S_OK		描画開始成功
	//				E_FAIL		描画開始失敗
	//	更新		2008/12/14	<新規>
	//================================================================================
	HRESULT
	CZTEXTURE_SHADER::begin(void)
	{
	//	デバイスが持っているバッファを一時保存
		dev->GetRenderTarget(0, &devBufferSurf);
		dev->GetDepthStencilSurface(&devDepthBufferSurf);

	//	デバイスにZ値テクスチャサーフェイスと深度バッファを設定
		dev->SetRenderTarget(0, zTexSurf);
		dev->SetDepthStencilSurface(depthBufferSurf);

	//	各サーフェイスを初期化
		dev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						 D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);

	//	プログラマブルシェーダのテクニックを設定
		effect->SetTechnique(hTechnique);

	//	シェーダの開始を宣言
		UINT	tmp;
		effect->Begin(&tmp, 0);

		return S_OK;
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::end
	//
	//	機能		描画の終了を宣言する
	//	戻り値		S_OK		描画終了成功
	//				E_FAIL		描画終了失敗
	//	更新		2008/12/14	<新規>
	//================================================================================
	HRESULT
	CZTEXTURE_SHADER::end(void)
	{
		effect->End();

	//	デバイスに元のサーフェイスを戻す
		dev->SetRenderTarget(0, devBufferSurf);
		dev->SetDepthStencilSurface(devDepthBufferSurf);

		devBufferSurf		= NULL;
		devDepthBufferSurf	= NULL;

	//	固定機能に戻す
		dev->SetVertexShader(NULL);
		dev->SetPixelShader(NULL);

		return S_OK;
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::beginPass
	//
	//	機能		パスの開始を宣言する
	//	戻り値		S_OK		パス開始成功
	//				E_FAIL		パス開始失敗
	//	更新		2008/12/14	<新規>
	//================================================================================
	HRESULT
	CZTEXTURE_SHADER::beginPass(void)
	{
		return	effect->BeginPass(0);
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::endPass
	//
	//	機能		パスの終了を宣言する
	//	戻り値		S_OK		パス終了成功
	//				E_FAIL		パス終了失敗
	//	更新		2008/12/14	<新規>
	//================================================================================
	HRESULT
	CZTEXTURE_SHADER::endPass(void)
	{
		return	effect->EndPass();
	}

	//================================================================================
	//!	メソッド名	CZTEXTURE_SHADER::getZTex
	//
	//	機能		Z値テクスチャを取得する
	//	戻り値		Z値テクスチャ
	//	更新		2008/12/14	<新規>
	//================================================================================
	TEX
	CZTEXTURE_SHADER::getZTex(void)
	{
		return	zTex;
	}
}
//-----------------------------------------------------------------------
//!	シェーダの名前空間
//-----------------------------------------------------------------------