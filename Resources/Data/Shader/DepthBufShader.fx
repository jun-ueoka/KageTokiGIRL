float4x4	matWorldCameraViewProjection;	//!< ワールドカメラビュー射影変換行列
float4x4	matWorldLightViewProjection;	//!< ワールドライトビュー射影変換行列
float4x4	matWorldShadow;					//!< ワールドシャドウ変換行列
float4		matCol;							//!< マテリアルカラー
float4		gaussOffset[12];				//!< ガウス分布オフセット配列
float4		lightDir;						//!< ライトの方向

texture		texShadowMap;					//!< シャドウマップテクスチャ（ボリュームシャドーをつけるための）
texture		meshTexture;					//!< メッシュに反映するテクスチャ

sampler DefSampler = sampler_state			//!< サンプラーステート
{
	texture		= (texShadowMap);
	AddressU	= CLAMP;
	AddressV	= CLAMP;
	AddressW	= CLAMP;
	MIPFILTER	= LINEAR;
	MINFILTER	= LINEAR;
	MAGFILTER	= LINEAR;
};

sampler meshTex = sampler_state				//!<メッシュに張るテクスチャ
{
	texture		= (meshTexture);
	AddressU	= CLAMP;
	AddressV	= CLAMP;
	AddressW	= CLAMP;
	MIPFILTER	= LINEAR;
	MINFILTER	= LINEAR;
	MAGFILTER	= LINEAR;
};

struct VS_OUTPUT_SHADOW
{
	float4	Position	: POSITION;
	float4	Diffuse		: COLOR0;
	float4	Pos			: TEXCOORD0;
	float4	TextureUV	: TEXCOORD1;
	float4	ShadowMap	: TEXCOORD2;
};

// 頂点シェーダ
VS_OUTPUT_SHADOW DepthBufShadow_VS( float4 Pos : POSITION, float3 Norm : NORMAL, float2 uv : TEXCOORD0)
{
	VS_OUTPUT_SHADOW	Out	= (VS_OUTPUT_SHADOW)0;

	// 普通にカメラの目線によるワールドビュー射影変換をする
	Out.Position	= mul( Pos, matWorldCameraViewProjection );

	// 法線とライトの方向から頂点の色を決定
	// 濃くなりすぎないように調節しています
	Out.Diffuse		= max( dot(lightDir.xyz, Norm), 0 );
	
	// ライトの目線によるワールドビュー射影変換をする
	Out.Pos			= mul( Pos, matWorldLightViewProjection );

	// シャドウマップの設定
	Out.ShadowMap	= mul( Pos, matWorldShadow );

	//UV座標の設定
	Out.TextureUV.xy	= uv;
	Out.TextureUV.wz	= 0;
	return	Out;
}


// ソフトシェーダ
float4 DepthBufShadow_12PS(VS_OUTPUT_SHADOW In) : COLOR
{	
	float2 t  = In.ShadowMap.xy / In.ShadowMap.w;

	float depth = In.Pos.z / In.Pos.w - 0.001f;
	float shade = (( tex2D( DefSampler, t + gaussOffset[ 0].xy ).x < depth ) ? 0 : 1)
				+ (( tex2D( DefSampler, t + gaussOffset[ 1].xy ).x < depth ) ? 0 : 1)
				+ (( tex2D( DefSampler, t + gaussOffset[ 2].xy ).x < depth ) ? 0 : 1)
	            + (( tex2D( DefSampler, t + gaussOffset[ 3].xy ).x < depth ) ? 0 : 1)
	            + (( tex2D( DefSampler, t + gaussOffset[ 4].xy ).x < depth ) ? 0 : 1)
	            + (( tex2D( DefSampler, t + gaussOffset[ 5].xy ).x < depth ) ? 0 : 1)
	            + (( tex2D( DefSampler, t + gaussOffset[ 6].xy ).x < depth ) ? 0 : 1)
	            + (( tex2D( DefSampler, t + gaussOffset[ 7].xy ).x < depth ) ? 0 : 1)
	            + (( tex2D( DefSampler, t + gaussOffset[ 8].xy ).x < depth ) ? 0 : 1)
	            + (( tex2D( DefSampler, t + gaussOffset[ 9].xy ).x < depth ) ? 0 : 1)
	            + (( tex2D( DefSampler, t + gaussOffset[10].xy ).x < depth ) ? 0 : 1)
	            + (( tex2D( DefSampler, t + gaussOffset[11].xy ).x < depth ) ? 0 : 1);

	const float ambient = 0.5;

	float4	value4	= ((1.0f - ambient) / 12.0f) * shade * In.Diffuse;
	
	float4	Color = tex2D(meshTex, In.TextureUV) *
						float4(	value4.r+ambient,
								value4.g+ambient,
								value4.b+ambient,
								value4.a+ambient);
	Color.a	= matCol.a;

	return	Color;
}


// ピクセルシェーダ
float4 DepthBufShadow_PS(VS_OUTPUT_SHADOW In) : COLOR
{ 
	float shade	= ( (tex2Dproj( DefSampler, In.ShadowMap ).x) <
					(In.Pos.z / In.Pos.w) - 0.001f )			? 0 : 1 ;
	
	const float ambient = 0.5;

	float4	value4	= (1.0f - ambient) * shade * In.Diffuse;

	float4	Color	= tex2D( meshTex, In.TextureUV ) * ( value4 + ambient );
	Color.a	= matCol.a;

	return	Color;
}

// テクニック
technique	DepthBufShadowTec
{
	pass	P0
	{
		VertexShader	= compile vs_2_0 DepthBufShadow_VS();
		PixelShader		= compile ps_2_0 DepthBufShadow_PS();
	}
}
