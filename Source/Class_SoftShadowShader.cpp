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
		this->dev			= NULL;
		this->shadowMapTex	= NULL;
		this->effect		= NULL;
	//	this->fRadius		= 5.0f;
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::�f�X�g���N�^
	//
	//	�X�V		2008/12/14	<�V�K>
	//================================================================================
	CDEPTHBUF_SHADER::~CDEPTHBUF_SHADER(void)
	{
	//	�����̉��
		SAFE_RELEASE(this->effect);
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
	//!	��������Ă���G�t�F�N�g���Z�b�g
		if(	gUniqueInstance)
		{
		//!	�C���X�^���X����
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
		//!	�C���X�^���X����
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
	//	�G�t�F�N�g�t�@�C������[�x�o�b�t�@�V���h�E�V�F�[�_�v���O�������擾
		if(	FAILED(D3DXCreateEffectFromFileEx(
				inDev, PATH_DEPTHBUFSHADER, NULL, NULL, NULL, 0, NULL,
				&this->effect, NULL)))
		{
			return	FALSE;
		}

	//	�G�t�F�N�g���̊e��p�����[�^�n���h�����擾
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

	////	�K�E�X���z�I�t�Z�b�g�z��̏����ݒ�
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

	//	�f�o�C�X���m��
		this->dev	= inDev;

		return	TRUE;
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

		this->shadowMapTex	= inShadowMapTex;
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
		this->matrixWorld	= *inMat;
		this->effect->SetMatrix(this->hWorldMat, &this->matrixWorld);
		this->dev->SetTransform(D3DTS_WORLD, &this->matrixWorld);
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::setCameraViewMatrix
	//
	//	�@�\		�J�����̃r���[�s���ݒ�
	//	����		inMat			�J�����̃r���[�s��
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setCameraViewMatrix(MAT *inMat)
	{
		this->matrixCameraView	= *inMat;
		this->effect->SetMatrix(this->hCameraViewMat, &this->matrixCameraView);
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::setCameraProjMatrix
	//
	//	�@�\		�J�����̎ˉe�ϊ��s���ݒ�
	//	����		inMat			�J�����̎ˉe�ϊ��s��
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setCameraProjMatrix(MAT *inMat)
	{
		this->matrixCameraProj	= *inMat;
		this->effect->SetMatrix(this->hCameraProjMat, &this->matrixCameraProj);
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::setLightViewMatrix
	//
	//	�@�\		���C�g�̃r���[�s���ݒ�
	//	����		inMat			���C�g�̃r���[�s��
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setLightViewMatrix(MAT *inMat)
	{
		this->matrixLightView	= *inMat;
		this->effect->SetMatrix(this->hLightViewMat, &this->matrixLightView);
	}

	//================================================================================
	//!	���\�b�h��	CDEPTHBUF_SHADER::setLightProjMatrix
	//
	//	�@�\		���C�g�̎ˉe�ϊ��s���ݒ�
	//	����		inMat			���C�g�̎ˉe�ϊ��s��
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	CDEPTHBUF_SHADER::setLightProjMatrix(MAT *inMat)
	{
		this->matrixLightProj	= *inMat;
		this->effect->SetMatrix(this->hLightProjMat, &this->matrixLightProj);
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
		this->effect->SetValue(this->hMateialColor, inMatColor, 16);
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
		this->effect->SetTexture(this->hMeshTexture, inTex);
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
		this->effect->SetTechnique(this->hTechnique);

	//	�J�n�錾
		UINT	pass;
		this->effect->Begin(&pass, 0);

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
		this->effect->End();

	//	�Œ�@�\�ɖ߂�
		this->dev->SetVertexShader(NULL);
		this->dev->SetPixelShader(NULL);

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
		this->effect->BeginPass(0);
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
		this->effect->EndPass();
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
		this->effect->SetTexture(this->hShadowMapTex, this->shadowMapTex);
		return	TRUE;
	}
}