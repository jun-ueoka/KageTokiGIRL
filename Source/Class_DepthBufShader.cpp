//***********************************************************************************
//!	�t�@�C����		Class_DepthBufShader.cpp
//
//	�@�\			�[�x�o�b�t�@�V���h�E�G�t�F�N�g�N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/12/14	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_DepthBufShader.h"

//-----------------------------------------------------------------------
//!	�V�F�[�_�̖��O���
//-----------------------------------------------------------------------
namespace SHA
{
	//================================================================================
	//	�C���X�^���X�����ꏊ
	//================================================================================
	CDEPTHBUF_SHADER* CDEPTHBUF_SHADER::gUniqueInstance = NULL;	

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::�R���X�g���N�^
	//
	//	�X�V		2008/12/14	<�V�K>
	//================================================================================
	CDEPTHBUF_SHADER::CDEPTHBUF_SHADER(void)
	{
	//	�����̏�����
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
	//!	���\�b�h��	CDEPTHBUF_SHADER::�f�X�g���N�^
	//
	//	�X�V		2008/12/14	<�V�K>
	//================================================================================
	CDEPTHBUF_SHADER::~CDEPTHBUF_SHADER(void)
	{
	//	�����̉��
		SAFE_RELEASE(effect);
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::getInstance
	//
	//	�@�\		�C���X�^���X�̐���
	//	�߂�l		�C���X�^���X��Ԃ�
	//	�X�V		2008/12/14	<�V�K>
	//================================================================================
	CDEPTHBUF_SHADER*
	CDEPTHBUF_SHADER::getInstance(void)
	{
	//!	�������Ă��Ȃ������ꍇ
		if(!gUniqueInstance)
		{
		//!	�C���X�^���X����
			gUniqueInstance = new CDEPTHBUF_SHADER();
		}
	//!	�C���X�^���X��Ԃ�
		return gUniqueInstance;
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::releaseInstance
	//
	//	�@�\		�C���X�^���X�̉��
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	CDEPTHBUF_SHADER::releaseInstance(void)
	{
		SAFE_DELETE(gUniqueInstance);	//!< �C���X�^���X�̉��
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::lostInstance
	//
	//	�@�\		�C���X�^���X���X�g
	//	�X�V		2008/12/18		<�V�K>
	//================================================================================
	void
	CDEPTHBUF_SHADER::lostInstance(void)
	{
	//!	��������Ă���G�t�F�N�g���X�g
		if(	gUniqueInstance)
		{
			gUniqueInstance->getEffect()->OnLostDevice();
		}
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::resetInstance
	//
	//	�@�\		�C���X�^���X���Z�b�g
	//	�X�V		2008/12/18		<�V�K>
	//================================================================================
	void
	CDEPTHBUF_SHADER::resetInstance(void)
	{
	//!	��������Ă���G�t�F�N�g���Z�b�g
		if(	gUniqueInstance)
		{
			gUniqueInstance->getEffect()->OnResetDevice();
		}
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::initialize
	//
	//	�@�\		������
	//	����		inDev			Direct3D�f�o�C�X
	//	�߂�l		TRUE			����������
	//				FALSE			���������s
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	BOOL
	CDEPTHBUF_SHADER::initialize(DEV inDev)
	{
	//	�G���[���
		if(	!inDev)	return	FALSE;
		
	//	�������
		finitialize();

	//	�G�t�F�N�g�t�@�C������[�x�o�b�t�@�V���h�E�V�F�[�_�v���O�������擾
		if(	FAILED(D3DXCreateEffectFromFile(
				inDev, PATH_DEPTHBUFSHADER, NULL, NULL, 0, NULL,
				&effect, NULL)))
		{
			return	FALSE;
		}

	//	�G�t�F�N�g���̊e��p�����[�^�n���h�����擾
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

	//	�o�C�A�X�X�P�[���ϊ��s��̏����ݒ�
		float	fOffsetX	= 0.5f + (0.5f / (float)SHADOWMAP_SIZE);
		float	fOffsetY	= 0.5f + (0.5f / (float)SHADOWMAP_SIZE);
		matrixBias	=	MAT(0.5f,	  0.0f,     0.0f,   0.0f,
							0.0f,	 -0.5f,     0.0f,   0.0f,
							0.0f,	  0.0f,     0.0f,   0.0f,
							fOffsetX, fOffsetY, 0.0f,   1.0f);

	//	�K�E�X���z�I�t�Z�b�g�z��̏����ݒ�
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

	//	�f�o�C�X���m��
		dev	= inDev;

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::finitialize
	//
	//	�@�\		���
	//	�X�V		2009/01/22		<�V�K>
	//================================================================================
	void
	CDEPTHBUF_SHADER::finitialize(void)
	{
		SAFE_RELEASE(effect);
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::setShadowMap
	//
	//	�@�\		�V���h�E�}�b�v��ݒ�
	//	����		inShadowMapTex	�O���V���h�E�}�b�v�e�N�X�`��
	//	�߂�l		TRUE			�ݒ萬��
	//				FALSE			�ݒ莸�s
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	BOOL
	CDEPTHBUF_SHADER::setShadowMap(TEX inShadowMapTex)
	{
		if( !inShadowMapTex)	return	FALSE;

		shadowMapTex	= inShadowMapTex;
		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::setWorldMatrix
	//
	//	�@�\		�`��ΏۃI�u�W�F�N�g�̃��[���h�ϊ��s���ݒ�
	//	����		inMat			�`��ΏۃI�u�W�F�N�g�̃��[���h�ϊ��s��
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setWorldMatrix(MAT *inMat)
	{
	//	���C�g�̕�����ݒ�
		VEC4	dir(lightDir.x, lightDir.y, lightDir.z, 0.0f);
		MAT		dirMatrix;
		D3DXMatrixInverse(&dirMatrix, NULL, inMat);
		D3DXVec4Transform(&dir, &dir, &dirMatrix);
		D3DXVec4Normalize(&dir, &dir);
		effect->SetVector(hLightDir, &dir);
	//	���[���h�J�����r���[�ˉe�ϊ��s��
		effect->SetMatrix(
			hWorldCameraViewProjMat,
			&((*inMat) * matrixCameraViewProjection));
	//	���[���h���C�g�r���[�ˉe�ϊ��s��
		effect->SetMatrix(
			hWorldLightViewProjMat,
			&((*inMat) * matrixLightViewProjection));
	//	���[���h�V���h�E�ϊ��s��
		effect->SetMatrix(
			hShadowMat,
			&((*inMat) * (matrixLightViewProjection * matrixBias)));
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::setCameraViewMatrix
	//
	//	�@�\		�J�����̃r���[�s���ݒ�
	//	����		inMat			�J�����̃r���[�s��
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setCameraViewProjMatrix(MAT *inMat)
	{
		matrixCameraViewProjection	= *inMat;
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::setLightViewMatrix
	//
	//	�@�\		���C�g�̃r���[�s���ݒ�
	//	����		inMat			���C�g�̃r���[�s��
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setLightViewProjMatrix(MAT *inMat)
	{
		matrixLightViewProjection	= *inMat;
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::setMaterialColor
	//
	//	�@�\		�}�e���A���J���[��ݒ�
	//	����		inMatColor		�}�e���A���J���[
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setMaterialColor(D3DCOLORVALUE* inMatColor)
	{
		effect->SetValue(hMateialColor, inMatColor, 16);
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::setLightProjMatrix
	//
	//	�@�\		�G�t�F�N�g�Ƀe�N�X�`����ݒ�
	//	����		inTex			�ݒ肷��e�N�X�`��
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setEffectTexture(TEX inTex)
	{
		effect->SetTexture(hMeshTexture, inTex);
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::begin
	//
	//	�@�\		�`��̊J�n��錾����
	//	�߂�l		S_OK			�`��J�n����
	//				E_FAIL			�`��J�n���s
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	HRESULT
	CDEPTHBUF_SHADER::begin(void)
	{
	//	�v���O���}�u���V�F�[�_�ɐ؂�ւ�
		effect->SetTechnique(hTechnique);

	//	�J�n�錾
		UINT	pass;
		effect->Begin(&pass, 0);

		return	S_OK;
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::end
	//
	//	�@�\		�`��̏I����錾����
	//	�߂�l		S_OK			�`��I������
	//				E_FAIL			�`��I�����s
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	HRESULT
	CDEPTHBUF_SHADER::end(void)
	{
		effect->End();

	//	�Œ�@�\�ɖ߂�
		dev->SetVertexShader(NULL);
		dev->SetPixelShader(NULL);

		return	S_OK;
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::beginPass
	//
	//	�@�\		�p�X�̊J�n��錾����
	//	�߂�l		S_OK			�p�X�J�n����
	//				E_FAIL			�p�X�J�n���s
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	HRESULT
	CDEPTHBUF_SHADER::beginPass(void)
	{
		effect->BeginPass(0);
		return	S_OK;
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::endPass
	//
	//	�@�\		�p�X�̏I����錾����
	//	�߂�l		S_OK			�p�X�I������
	//				E_FAIL			�p�X�I�����s
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	HRESULT
	CDEPTHBUF_SHADER::endPass(void)
	{
		effect->EndPass();
		return S_OK;
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::setParamToEffect
	//
	//	�@�\		�o�^����Ă���p�����[�^�����G�t�F�N�g�ɃZ�b�g
	//	�߂�l		TRUE			���Z�b�g����
	//				FALSE			���Z�b�g���s
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	BOOL
	CDEPTHBUF_SHADER::setParamToEffect(void)
	{
		effect->SetTexture(hShadowMapTex, shadowMapTex);
		return	TRUE;
	}
}