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
		dev						= NULL;
		shadowMapTex			= NULL;
		effect					= NULL;
		fRadius					= 5.0f;
		lightDir				= VEC3(0.0f, 0.0f, 0.0f);
		D3DXMatrixIdentity(&matrixCameraViewProjection);
		D3DXMatrixIdentity(&matrixLightViewProjection);
		D3DXMatrixIdentity(&matrixBias);
		hLightDir				= NULL;
		hWorldCameraViewProjMat	= NULL;
		hWorldLightViewProjMat	= NULL;
		hShadowMat				= NULL;
		hMateialColor			= NULL;
		hGaussOffset			= NULL;
		hShadowMapTex			= NULL;
		hMeshTexture			= NULL;
		hTechnique				= NULL;
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::デストラクタ
	//
	//	更新		2008/12/14	<新規>
	//================================================================================
	CDEPTHBUF_SHADER::~CDEPTHBUF_SHADER(void)
	{
	//	属性の解放
		SAFE_RELEASE(effect);
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
	//!	生成されてたらエフェクトロスト
		if(	gUniqueInstance)
		{
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
		
	//	初期解放
		finitialize();

	//	エフェクトファイルから深度バッファシャドウシェーダプログラムを取得
		if(	FAILED(D3DXCreateEffectFromFile(
				inDev, PATH_DEPTHBUFSHADER, NULL, NULL, 0, NULL,
				&effect, NULL)))
		{
			return	FALSE;
		}

	//	エフェクト内の各種パラメータハンドルを取得
		hWorldCameraViewProjMat	= effect->GetParameterByName(NULL, "matWorldCameraViewProjection");
		hWorldLightViewProjMat	= effect->GetParameterByName(NULL, "matWorldLightViewProjection");
		hShadowMat				= effect->GetParameterByName(NULL, "matWorldShadow");
		hLightDir				= effect->GetParameterByName(NULL, "lightDir");
		hMateialColor			= effect->GetParameterByName(NULL, "matCol");
		hGaussOffset			= effect->GetParameterByName(NULL, "gaussOffset");
		hShadowMapTex			= effect->GetParameterByName(NULL, "texShadowMap");
		hMeshTexture			= effect->GetParameterByName(NULL, "meshTexture");
		hTechnique				= effect->GetTechniqueByName("DepthBufShadowTec");
		if(	!hWorldCameraViewProjMat||
			!hWorldLightViewProjMat	||
			!hShadowMat				||
			!hLightDir				||
			!hMateialColor			||
			!hGaussOffset			||
			!hShadowMapTex			||
			!hMeshTexture			||
			!hTechnique)
		{
			return	FALSE;
		}

	//	バイアススケール変換行列の初期設定
		float	fOffsetX	= 0.5f + (0.5f / (float)SHADOWMAP_SIZE);
		float	fOffsetY	= 0.5f + (0.5f / (float)SHADOWMAP_SIZE);
		matrixBias	=	MAT(0.5f,	  0.0f,     0.0f,   0.0f,
							0.0f,	 -0.5f,     0.0f,   0.0f,
							0.0f,	  0.0f,     0.0f,   0.0f,
							fOffsetX, fOffsetY, 0.0f,   1.0f);

	//	ガウス分布オフセット配列の初期設定
		VEC4	offset[12]	=
		{
			VEC4( -0.326212f*fRadius/SHADOWMAP_SIZE, -0.405810f*fRadius/SHADOWMAP_SIZE, 0,0),
			VEC4( -0.840144f*fRadius/SHADOWMAP_SIZE, -0.073580f*fRadius/SHADOWMAP_SIZE, 0,0),
            VEC4( -0.695914f*fRadius/SHADOWMAP_SIZE, +0.457137f*fRadius/SHADOWMAP_SIZE, 0,0),
            VEC4( -0.203345f*fRadius/SHADOWMAP_SIZE, +0.620716f*fRadius/SHADOWMAP_SIZE, 0,0),
            VEC4( +0.962340f*fRadius/SHADOWMAP_SIZE, -0.194983f*fRadius/SHADOWMAP_SIZE, 0,0),
            VEC4( +0.473434f*fRadius/SHADOWMAP_SIZE, -0.480026f*fRadius/SHADOWMAP_SIZE, 0,0),
            VEC4( +0.519456f*fRadius/SHADOWMAP_SIZE, +0.767022f*fRadius/SHADOWMAP_SIZE, 0,0),
            VEC4( +0.185461f*fRadius/SHADOWMAP_SIZE, -0.893124f*fRadius/SHADOWMAP_SIZE, 0,0),
            VEC4( +0.507431f*fRadius/SHADOWMAP_SIZE, +0.064425f*fRadius/SHADOWMAP_SIZE, 0,0),
            VEC4( +0.896420f*fRadius/SHADOWMAP_SIZE, +0.412458f*fRadius/SHADOWMAP_SIZE, 0,0),
            VEC4( -0.321940f*fRadius/SHADOWMAP_SIZE, -0.932645f*fRadius/SHADOWMAP_SIZE, 0,0),
            VEC4( -0.791550f*fRadius/SHADOWMAP_SIZE, -0.597710f*fRadius/SHADOWMAP_SIZE, 0,0),
		};
		effect->SetVectorArray(hGaussOffset, offset, 12);

	//	デバイスを確保
		dev	= inDev;

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::finitialize
	//
	//	機能		解放
	//	更新		2009/01/22		<新規>
	//================================================================================
	void
	CDEPTHBUF_SHADER::finitialize(void)
	{
		SAFE_RELEASE(effect);
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

		shadowMapTex	= inShadowMapTex;
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
	//	ライトの方向を設定
		VEC4	dir(lightDir.x, lightDir.y, lightDir.z, 0.0f);
		MAT		dirMatrix;
		D3DXMatrixInverse(&dirMatrix, NULL, inMat);
		D3DXVec4Transform(&dir, &dir, &dirMatrix);
		D3DXVec4Normalize(&dir, &dir);
		effect->SetVector(hLightDir, &dir);
	//	ワールドカメラビュー射影変換行列
		effect->SetMatrix(
			hWorldCameraViewProjMat,
			&((*inMat) * matrixCameraViewProjection));
	//	ワールドライトビュー射影変換行列
		effect->SetMatrix(
			hWorldLightViewProjMat,
			&((*inMat) * matrixLightViewProjection));
	//	ワールドシャドウ変換行列
		effect->SetMatrix(
			hShadowMat,
			&((*inMat) * (matrixLightViewProjection * matrixBias)));
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::setCameraViewMatrix
	//
	//	機能		カメラのビュー行列を設定
	//	引数		inMat			カメラのビュー行列
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setCameraViewProjMatrix(MAT *inMat)
	{
		matrixCameraViewProjection	= *inMat;
	}

	//================================================================================
	//!	メソッド名	CDEPTHBUF_SHADER::setLightViewMatrix
	//
	//	機能		ライトのビュー行列を設定
	//	引数		inMat			ライトのビュー行列
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setLightViewProjMatrix(MAT *inMat)
	{
		matrixLightViewProjection	= *inMat;
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
		effect->SetValue(hMateialColor, inMatColor, 16);
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
		effect->SetTexture(hMeshTexture, inTex);
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
		effect->SetTechnique(hTechnique);

	//	開始宣言
		UINT	pass;
		effect->Begin(&pass, 0);

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
		effect->End();

	//	固定機能に戻す
		dev->SetVertexShader(NULL);
		dev->SetPixelShader(NULL);

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
		effect->BeginPass(0);
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
		effect->EndPass();
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
		effect->SetTexture(hShadowMapTex, shadowMapTex);
		return	TRUE;
	}
}