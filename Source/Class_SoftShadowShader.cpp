//***********************************************************************************
//!	ファイル名		Class_DepthBufShader.cpp
//
//	機能			深度バッファシャドウエフェクトクラスの実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/12/14	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_DepthBufShader.h"

//-----------------------------------------------------------------------
//!	シェーダの名前空間
//-----------------------------------------------------------------------
namespace SHA
{
	//================================================================================
	//	インスタンス生成場所
	//================================================================================
	CDEPTHBUF_SHADER* CDEPTHBUF_SHADER::gUniqueInstance = NULL;	

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::コンストラクタ
	//
	//	更新		2008/12/14	<新規>
	//================================================================================
	CDEPTHBUF_SHADER::CDEPTHBUF_SHADER(void)
	{
	//	属性の初期化
		this->dev			= NULL;
		this->shadowMapTex	= NULL;
		this->effect		= NULL;
	//	this->fRadius		= 5.0f;
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::デストラクタ
	//
	//	更新		2008/12/14	<新規>
	//================================================================================
	CDEPTHBUF_SHADER::~CDEPTHBUF_SHADER(void)
	{
	//	属性の解放
		SAFE_RELEASE(this->effect);
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::getInstance
	//
	//	機能		インスタンスの生成
	//	戻り値		インスタンスを返す
	//	更新		2008/12/14	<新規>
	//================================================================================
	CDEPTHBUF_SHADER*
	CDEPTHBUF_SHADER::getInstance(void)
	{
	//!	生成していなかった場合
		if(!gUniqueInstance)
		{
		//!	インスタンス生成
			gUniqueInstance = new CDEPTHBUF_SHADER();
		}
	//!	インスタンスを返す
		return gUniqueInstance;
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::releaseInstance
	//
	//	機能		インスタンスの解放
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	CDEPTHBUF_SHADER::releaseInstance(void)
	{
		SAFE_DELETE(gUniqueInstance);	//!< インスタンスの解放
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::lostInstance
	//
	//	機能		インスタンスロスト
	//	更新		2008/12/18		<新規>
	//================================================================================
	void
	CDEPTHBUF_SHADER::lostInstance(void)
	{
	//!	生成されてたらエフェクトリセット
		if(	gUniqueInstance)
		{
		//!	インスタンス生成
			gUniqueInstance->getEffect()->OnLostDevice();
		}
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::resetInstance
	//
	//	機能		インスタンスリセット
	//	更新		2008/12/18		<新規>
	//================================================================================
	void
	CDEPTHBUF_SHADER::resetInstance(void)
	{
	//!	生成されてたらエフェクトリセット
		if(	gUniqueInstance)
		{
		//!	インスタンス生成
			gUniqueInstance->getEffect()->OnResetDevice();
		}
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::initialize
	//
	//	機能		初期化
	//	引数		inDev			Direct3Dデバイス
	//	戻り値		TRUE			初期化成功
	//				FALSE			初期化失敗
	//	更新		2008/12/14		<新規>
	//================================================================================
	BOOL
	CDEPTHBUF_SHADER::initialize(DEV inDev)
	{
	//	エラー回避
		if(	!inDev)	return	FALSE;
	//	エフェクトファイルから深度バッファシャドウシェーダプログラムを取得
		if(	FAILED(D3DXCreateEffectFromFileEx(
				inDev, PATH_DEPTHBUFSHADER, NULL, NULL, NULL, 0, NULL,
				&this->effect, NULL)))
		{
			return	FALSE;
		}

	//	エフェクト内の各種パラメータハンドルを取得
		this->hWorldMat		= this->effect->GetParameterByName(NULL, "matWorld");
		this->hCameraViewMat= this->effect->GetParameterByName(NULL, "matCameraView");
		this->hCameraProjMat= this->effect->GetParameterByName(NULL, "matCameraProj");
		this->hLightViewMat	= this->effect->GetParameterByName(NULL, "matLightView");
		this->hLightProjMat	= this->effect->GetParameterByName(NULL, "matLightProj");
		this->hMateialColor	= this->effect->GetParameterByName(NULL, "matCol");
//		this->hShadowOffset	= this->effect->GetParameterByName(NULL, "shadowOffset");
		this->hShadowMapTex	= this->effect->GetParameterByName(NULL, "texShadowMap");
		this->hMeshTexture	= this->effect->GetParameterByName(NULL, "meshTexture");
		this->hTechnique	= this->effect->GetTechniqueByName("DepthBufShadowTec");
		if(	!this->hWorldMat		||
			!this->hCameraViewMat	||
			!this->hCameraProjMat	||
			!this->hLightViewMat	||
			!this->hLightProjMat	||
			!this->hMateialColor	||
//			!this->hShadowOffset	||
			!this->hShadowMapTex	||
			!this->hMeshTexture		||
			!this->hTechnique)
		{
			return	FALSE;
		}

	////	ガウス分布オフセット配列の初期設定
	//	VEC4	offset[12]	=
	//	{
	//		VEC4( -0.326212f*fRadius/SHADOWMAP_SIZE, -0.405810f*fRadius/SHADOWMAP_SIZE, 0,0),
	//		VEC4( -0.840144f*fRadius/SHADOWMAP_SIZE, -0.073580f*fRadius/SHADOWMAP_SIZE, 0,0),
 //           VEC4( -0.695914f*fRadius/SHADOWMAP_SIZE, +0.457137f*fRadius/SHADOWMAP_SIZE, 0,0),
 //           VEC4( -0.203345f*fRadius/SHADOWMAP_SIZE, +0.620716f*fRadius/SHADOWMAP_SIZE, 0,0),
 //           VEC4( +0.962340f*fRadius/SHADOWMAP_SIZE, -0.194983f*fRadius/SHADOWMAP_SIZE, 0,0),
 //           VEC4( +0.473434f*fRadius/SHADOWMAP_SIZE, -0.480026f*fRadius/SHADOWMAP_SIZE, 0,0),
 //           VEC4( +0.519456f*fRadius/SHADOWMAP_SIZE, +0.767022f*fRadius/SHADOWMAP_SIZE, 0,0),
 //           VEC4( +0.185461f*fRadius/SHADOWMAP_SIZE, -0.893124f*fRadius/SHADOWMAP_SIZE, 0,0),
 //           VEC4( +0.507431f*fRadius/SHADOWMAP_SIZE, +0.064425f*fRadius/SHADOWMAP_SIZE, 0,0),
 //           VEC4( +0.896420f*fRadius/SHADOWMAP_SIZE, +0.412458f*fRadius/SHADOWMAP_SIZE, 0,0),
 //           VEC4( -0.321940f*fRadius/SHADOWMAP_SIZE, -0.932645f*fRadius/SHADOWMAP_SIZE, 0,0),
 //           VEC4( -0.791550f*fRadius/SHADOWMAP_SIZE, -0.597710f*fRadius/SHADOWMAP_SIZE, 0,0),
	//	};
	//	this->effect->SetVectorArray(this->hShadowOffset, offset, 12);

	//	デバイスを確保
		this->dev	= inDev;

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::setShadowMap
	//
	//	機能		シャドウマップを設定
	//	引数		inShadowMapTex	外部シャドウマップテクスチャ
	//	戻り値		TRUE			設定成功
	//				FALSE			設定失敗
	//	更新		2008/12/14		<新規>
	//================================================================================
	BOOL
	CDEPTHBUF_SHADER::setShadowMap(TEX inShadowMapTex)
	{
		if( !inShadowMapTex)	return	FALSE;

		this->shadowMapTex	= inShadowMapTex;
		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::setWorldMatrix
	//
	//	機能		描画対象オブジェクトのワールド変換行列を設定
	//	引数		inMat			描画対象オブジェクトのワールド変換行列
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setWorldMatrix(MAT *inMat)
	{
		this->matrixWorld	= *inMat;
		this->effect->SetMatrix(this->hWorldMat, &this->matrixWorld);
		this->dev->SetTransform(D3DTS_WORLD, &this->matrixWorld);
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::setCameraViewMatrix
	//
	//	機能		カメラのビュー行列を設定
	//	引数		inMat			カメラのビュー行列
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setCameraViewMatrix(MAT *inMat)
	{
		this->matrixCameraView	= *inMat;
		this->effect->SetMatrix(this->hCameraViewMat, &this->matrixCameraView);
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::setCameraProjMatrix
	//
	//	機能		カメラの射影変換行列を設定
	//	引数		inMat			カメラの射影変換行列
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setCameraProjMatrix(MAT *inMat)
	{
		this->matrixCameraProj	= *inMat;
		this->effect->SetMatrix(this->hCameraProjMat, &this->matrixCameraProj);
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::setLightViewMatrix
	//
	//	機能		ライトのビュー行列を設定
	//	引数		inMat			ライトのビュー行列
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setLightViewMatrix(MAT *inMat)
	{
		this->matrixLightView	= *inMat;
		this->effect->SetMatrix(this->hLightViewMat, &this->matrixLightView);
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::setLightProjMatrix
	//
	//	機能		ライトの射影変換行列を設定
	//	引数		inMat			ライトの射影変換行列
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setLightProjMatrix(MAT *inMat)
	{
		this->matrixLightProj	= *inMat;
		this->effect->SetMatrix(this->hLightProjMat, &this->matrixLightProj);
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::setMaterialColor
	//
	//	機能		マテリアルカラーを設定
	//	引数		inMatColor		マテリアルカラー
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setMaterialColor(D3DCOLORVALUE* inMatColor)
	{
		this->effect->SetValue(this->hMateialColor, inMatColor, 16);
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::setLightProjMatrix
	//
	//	機能		エフェクトにテクスチャを設定
	//	引数		inTex			設定するテクスチャ
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setEffectTexture(TEX inTex)
	{
		this->effect->SetTexture(this->hMeshTexture, inTex);
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::begin
	//
	//	機能		描画の開始を宣言する
	//	戻り値		S_OK			描画開始成功
	//				E_FAIL			描画開始失敗
	//	更新		2008/12/14		<新規>
	//================================================================================
	HRESULT
	CDEPTHBUF_SHADER::begin(void)
	{
	//	プログラマブルシェーダに切り替え
		this->effect->SetTechnique(this->hTechnique);

	//	開始宣言
		UINT	pass;
		this->effect->Begin(&pass, 0);

		return	S_OK;
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::end
	//
	//	機能		描画の終了を宣言する
	//	戻り値		S_OK			描画終了成功
	//				E_FAIL			描画終了失敗
	//	更新		2008/12/14		<新規>
	//================================================================================
	HRESULT
	CDEPTHBUF_SHADER::end(void)
	{
		this->effect->End();

	//	固定機能に戻す
		this->dev->SetVertexShader(NULL);
		this->dev->SetPixelShader(NULL);

		return	S_OK;
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::beginPass
	//
	//	機能		パスの開始を宣言する
	//	戻り値		S_OK			パス開始成功
	//				E_FAIL			パス開始失敗
	//	更新		2008/12/14		<新規>
	//================================================================================
	HRESULT
	CDEPTHBUF_SHADER::beginPass(void)
	{
		this->effect->BeginPass(0);
		return	S_OK;
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::endPass
	//
	//	機能		パスの終了を宣言する
	//	戻り値		S_OK			パス終了成功
	//				E_FAIL			パス終了失敗
	//	更新		2008/12/14		<新規>
	//================================================================================
	HRESULT
	CDEPTHBUF_SHADER::endPass(void)
	{
		this->effect->EndPass();
		return S_OK;
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::setParamToEffect
	//
	//	機能		登録されているパラメータ情報をエフェクトにセット
	//	戻り値		TRUE			情報セット成功
	//				FALSE			情報セット失敗
	//	更新		2008/12/14		<新規>
	//================================================================================
	BOOL
	CDEPTHBUF_SHADER::setParamToEffect(void)
	{
		this->effect->SetTexture(this->hShadowMapTex, this->shadowMapTex);
		return	TRUE;
	}
}