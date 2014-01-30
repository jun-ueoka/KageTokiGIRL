//***********************************************************************************
//!	�t�@�C����		Class_ZTextureShader.cpp
//
//	�@�\			Z�e�N�X�`�����쐬����N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/12/14	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_ZTextureShader.h"

//-----------------------------------------------------------------------
//!	�V�F�[�_�̖��O���
//-----------------------------------------------------------------------
namespace SHA
{
	//================================================================================
	//	�C���X�^���X�����ꏊ
	//================================================================================
	CZTEXTURE_SHADER* CZTEXTURE_SHADER::gUniqueInstance = NULL;		

	//================================================================================
	//!	���\�b�h��	CZTEXTURE_SHADER::�R���X�g���N�^
	//
	//	�X�V		2008/12/14	<�V�K>
	//================================================================================
	CZTEXTURE_SHADER::CZTEXTURE_SHADER(void)
	{
	//	�����̏�����
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
	//!	���\�b�h��	CZTEXTURE_SHADER::�f�X�g���N�^
	//
	//	�X�V		2008/12/14	<�V�K>
	//================================================================================
	CZTEXTURE_SHADER::~CZTEXTURE_SHADER(void)
	{
	//	�����̉��
		SAFE_RELEASE(effect);
		SAFE_RELEASE(devDepthBufferSurf);
		SAFE_RELEASE(devBufferSurf);
		SAFE_RELEASE(depthBufferSurf);
		SAFE_RELEASE(zTexSurf);
		SAFE_RELEASE(zTex);
	}

	//================================================================================
	//!	���\�b�h��	CZTEXTURE_SHADER::getInstance
	//
	//	�@�\		�C���X�^���X�̐���
	//	�߂�l		�C���X�^���X��Ԃ�
	//	�X�V		2008/12/14	<�V�K>
	//================================================================================
	CZTEXTURE_SHADER*
	CZTEXTURE_SHADER::getInstance(void)
	{
	//!	�������Ă��Ȃ������ꍇ
		if(!gUniqueInstance)
		{
		//!	�C���X�^���X����
			gUniqueInstance = new CZTEXTURE_SHADER();
		}
	//!	�C���X�^���X��Ԃ�
		return gUniqueInstance;
	}

	//================================================================================
	//!	���\�b�h��	CZTEXTURE_SHADER::releaseInstance
	//
	//	�@�\		�C���X�^���X�̉��
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	CZTEXTURE_SHADER::releaseInstance(void)
	{
		SAFE_DELETE(gUniqueInstance);	//!< �C���X�^���X�̉��
	}

	//================================================================================
	//!	���\�b�h��	CZTEXTURE_SHADER::lostInstance
	//
	//	�@�\		�C���X�^���X���X�g
	//	�X�V		2008/12/18		<�V�K>
	//================================================================================
	void
	CZTEXTURE_SHADER::lostInstance(void)
	{
	//!	��������Ă���G�t�F�N�g���X�g
		if(	gUniqueInstance)
		{
			gUniqueInstance->lostExecute();
		}
	}

	//================================================================================
	//!	���\�b�h��	CZTEXTURE_SHADER::resetInstance
	//
	//	�@�\		�C���X�^���X���Z�b�g
	//	�X�V		2008/12/18		<�V�K>
	//================================================================================
	void
	CZTEXTURE_SHADER::resetInstance(void)
	{
	//!	��������Ă���G�t�F�N�g���Z�b�g
		if(	gUniqueInstance)
		{
			gUniqueInstance->resetExecute();
		}
	}

	//================================================================================
	//!	���\�b�h��	CZTEXTURE_SHADER::initialize
	//
	//	�@�\		������
	//	����		inDev			Direct3D�f�o�C�X
	//				inZTexWidth		��������e�N�X�`���̉���
	//				inZTexHeight	��������e�N�X�`���̏c��
	//				inZTexFormat	��������e�N�X�`���̌`��
	//	�߂�l		TRUE			����������
	//				FALSE			���������s
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	BOOL
	CZTEXTURE_SHADER::initialize(DEV inDev, UINT inZTexWidth, UINT inZTexHeight)
	{
	//	�G���[���
		if(	!inDev)									return	FALSE;
		if(	inZTexWidth == 0 || inZTexHeight == 0)	return	FALSE;

	//	�������
		finitialize();

	//	�G�t�F�N�g�t�@�C������Z�e�N�X�`���v���O�������擾
		if(	FAILED(D3DXCreateEffectFromFile(
				inDev, PATH_ZTEXSHADER, NULL, NULL, 0, NULL,
				&effect, NULL)))
		{
			return	FALSE;
		}

	//	�G�t�F�N�g���̊e��p�����[�^�n���h�����擾
		hWorldLightViewProjectionMat	= effect->GetParameterByName(NULL, "mat");
		hTechnique= effect->GetTechniqueByName("ZValuePlotTec");
		if(	!hWorldLightViewProjectionMat		||
			!hTechnique)
		{
			return	FALSE;
		}

	//	�w���Z�l�e�N�X�`���𐶐�
		if(	FAILED(D3DXCreateTexture(
				inDev, inZTexWidth, inZTexHeight, 1,
				D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
				D3DPOOL_DEFAULT, &zTex)))
		{
			return	FALSE;
		}

	//	�T�[�t�F�C�X�擾
		zTex->GetSurfaceLevel(0, &zTexSurf);

	//	�`��f�o�C�X�ɒ�`����Ă���o�b�t�@�̔\�͂��擾
		SURF	pSurf;
		inDev->GetDepthStencilSurface(&pSurf);
		D3DSURFACE_DESC	desc;
		pSurf->GetDesc(&desc);
		pSurf->Release();

	//	�Ǝ��[�x�o�b�t�@���쐬
		if( FAILED(inDev->CreateDepthStencilSurface(
				inZTexWidth, inZTexHeight, desc.Format,
				desc.MultiSampleType, desc.MultiSampleQuality,
				TRUE, &depthBufferSurf, NULL)))
		{
			return	FALSE;
		}

	//	�����m��
		dev			= inDev;
		zTexWidth	= inZTexWidth;
		zTexHeight	= inZTexHeight;

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CZTEXTURE_SHADER::finitialize
	//
	//	�@�\		���
	//	�X�V		2009/01/22		<�V�K>
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
	//!	���\�b�h��	CZTEXTURE_SHADER::lostExecute
	//
	//	�@�\		�f�o�C�X���X�g���̏���
	//	�X�V		2009/03/03		<�V�K>
	//================================================================================
	void
	CZTEXTURE_SHADER::lostExecute(void)
	{
	//	�G�t�F�N�g���ꎞ�I�Ƀ��X�g����
		this->effect->OnLostDevice();
	//	�f�[�^�����
		this->finitialize();
	}

	//================================================================================
	//!	���\�b�h��	CZTEXTURE_SHADER::resetExecute
	//
	//	�@�\		�f�o�C�X���Z�b�g���̏���
	//	�X�V		2009/03/03		<�V�K>
	//================================================================================
	void
	CZTEXTURE_SHADER::resetExecute(void)
	{
	//	�G�t�F�N�g����蒼��
		this->initialize(this->dev, this->zTexWidth, this->zTexHeight);
	}

	//================================================================================
	//!	���\�b�h��	CZTEXTURE_SHADER::setWorldMatrix
	//
	//	�@�\		�`��ΏۃI�u�W�F�N�g�̃��[���h�ϊ��s���ݒ�
	//	����		inMat		�`��Ώۂ̃��[���h�}�g���b�N�X
	//	�X�V		2008/12/14	<�V�K>
	//================================================================================
	void
	CZTEXTURE_SHADER::setWorldMatrix(MAT *inMat)
	{
		effect->SetMatrix(
			hWorldLightViewProjectionMat,
			&((*inMat) * matrixLightViewProjection));
	}

	//================================================================================
	//!	���\�b�h��	CZTEXTURE_SHADER::setViewMatrix
	//
	//	�@�\		�r���[�s���ݒ�
	//	����		inMat		�r���[�s��
	//	�X�V		2008/12/14	<�V�K>
	//================================================================================
	void
	CZTEXTURE_SHADER::setLightViewProjMatrix(MAT *inMat)
	{
		matrixLightViewProjection	= *inMat;
	}

	//================================================================================
	//!	���\�b�h��	CZTEXTURE_SHADER::begin
	//
	//	�@�\		�`��̊J�n��錾����
	//	�߂�l		S_OK		�`��J�n����
	//				E_FAIL		�`��J�n���s
	//	�X�V		2008/12/14	<�V�K>
	//================================================================================
	HRESULT
	CZTEXTURE_SHADER::begin(void)
	{
	//	�f�o�C�X�������Ă���o�b�t�@���ꎞ�ۑ�
		dev->GetRenderTarget(0, &devBufferSurf);
		dev->GetDepthStencilSurface(&devDepthBufferSurf);

	//	�f�o�C�X��Z�l�e�N�X�`���T�[�t�F�C�X�Ɛ[�x�o�b�t�@��ݒ�
		dev->SetRenderTarget(0, zTexSurf);
		dev->SetDepthStencilSurface(depthBufferSurf);

	//	�e�T�[�t�F�C�X��������
		dev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						 D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);

	//	�v���O���}�u���V�F�[�_�̃e�N�j�b�N��ݒ�
		effect->SetTechnique(hTechnique);

	//	�V�F�[�_�̊J�n��錾
		UINT	tmp;
		effect->Begin(&tmp, 0);

		return S_OK;
	}

	//================================================================================
	//!	���\�b�h��	CZTEXTURE_SHADER::end
	//
	//	�@�\		�`��̏I����錾����
	//	�߂�l		S_OK		�`��I������
	//				E_FAIL		�`��I�����s
	//	�X�V		2008/12/14	<�V�K>
	//================================================================================
	HRESULT
	CZTEXTURE_SHADER::end(void)
	{
		effect->End();

	//	�f�o�C�X�Ɍ��̃T�[�t�F�C�X��߂�
		dev->SetRenderTarget(0, devBufferSurf);
		dev->SetDepthStencilSurface(devDepthBufferSurf);

		devBufferSurf		= NULL;
		devDepthBufferSurf	= NULL;

	//	�Œ�@�\�ɖ߂�
		dev->SetVertexShader(NULL);
		dev->SetPixelShader(NULL);

		return S_OK;
	}

	//================================================================================
	//!	���\�b�h��	CZTEXTURE_SHADER::beginPass
	//
	//	�@�\		�p�X�̊J�n��錾����
	//	�߂�l		S_OK		�p�X�J�n����
	//				E_FAIL		�p�X�J�n���s
	//	�X�V		2008/12/14	<�V�K>
	//================================================================================
	HRESULT
	CZTEXTURE_SHADER::beginPass(void)
	{
		return	effect->BeginPass(0);
	}

	//================================================================================
	//!	���\�b�h��	CZTEXTURE_SHADER::endPass
	//
	//	�@�\		�p�X�̏I����錾����
	//	�߂�l		S_OK		�p�X�I������
	//				E_FAIL		�p�X�I�����s
	//	�X�V		2008/12/14	<�V�K>
	//================================================================================
	HRESULT
	CZTEXTURE_SHADER::endPass(void)
	{
		return	effect->EndPass();
	}

	//================================================================================
	//!	���\�b�h��	CZTEXTURE_SHADER::getZTex
	//
	//	�@�\		Z�l�e�N�X�`�����擾����
	//	�߂�l		Z�l�e�N�X�`��
	//	�X�V		2008/12/14	<�V�K>
	//================================================================================
	TEX
	CZTEXTURE_SHADER::getZTex(void)
	{
		return	zTex;
	}
}
//-----------------------------------------------------------------------
//!	�V�F�[�_�̖��O���
//-----------------------------------------------------------------------