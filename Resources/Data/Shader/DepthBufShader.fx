float4x4	matWorldCameraViewProjection;	//!< ���[���h�J�����r���[�ˉe�ϊ��s��
float4x4	matWorldLightViewProjection;	//!< ���[���h���C�g�r���[�ˉe�ϊ��s��
float4x4	matWorldShadow;					//!< ���[���h�V���h�E�ϊ��s��
float4		matCol;							//!< �}�e���A���J���[
float4		gaussOffset[12];				//!< �K�E�X���z�I�t�Z�b�g�z��
float4		lightDir;						//!< ���C�g�̕���

texture		texShadowMap;					//!< �V���h�E�}�b�v�e�N�X�`���i�{�����[���V���h�[�����邽�߂́j
texture		meshTexture;					//!< ���b�V���ɔ��f����e�N�X�`��

sampler DefSampler = sampler_state			//!< �T���v���[�X�e�[�g
{
	texture		= (texShadowMap);
	AddressU	= CLAMP;
	AddressV	= CLAMP;
	AddressW	= CLAMP;
	MIPFILTER	= LINEAR;
	MINFILTER	= LINEAR;
	MAGFILTER	= LINEAR;
};

sampler meshTex = sampler_state				//!<���b�V���ɒ���e�N�X�`��
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

// ���_�V�F�[�_
VS_OUTPUT_SHADOW DepthBufShadow_VS( float4 Pos : POSITION, float3 Norm : NORMAL, float2 uv : TEXCOORD0)
{
	VS_OUTPUT_SHADOW	Out	= (VS_OUTPUT_SHADOW)0;

	// ���ʂɃJ�����̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
	Out.Position	= mul( Pos, matWorldCameraViewProjection );

	// �@���ƃ��C�g�̕������璸�_�̐F������
	// �Z���Ȃ肷���Ȃ��悤�ɒ��߂��Ă��܂�
	Out.Diffuse		= max( dot(lightDir.xyz, Norm), 0 );
	
	// ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
	Out.Pos			= mul( Pos, matWorldLightViewProjection );

	// �V���h�E�}�b�v�̐ݒ�
	Out.ShadowMap	= mul( Pos, matWorldShadow );

	//UV���W�̐ݒ�
	Out.TextureUV.xy	= uv;
	Out.TextureUV.wz	= 0;
	return	Out;
}


// �\�t�g�V�F�[�_
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


// �s�N�Z���V�F�[�_
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

// �e�N�j�b�N
technique	DepthBufShadowTec
{
	pass	P0
	{
		VertexShader	= compile vs_2_0 DepthBufShadow_VS();
		PixelShader		= compile ps_2_0 DepthBufShadow_PS();
	}
}
