//***********************************************************************************
//!	�t�@�C����		Class_Direction.cpp
//
//	�@�\			DirectX���o�@�\���g�������N���X�̒�`
//	�@�\�T�v		DirectX�̊֐��A�@�\���J�v�Z�������A
//					�l�𑗂邾���ŁA���̒l�ɉ��������o�����s�o����
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/12/01	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_Direction.h"

//-----------------------------------------------------------------------
//!	DX���C�u�����̖��O���
//-----------------------------------------------------------------------
namespace DX
{
	//================================================================================
	//!	���\�b�h��	CDIRECTION::�R���X�g���N�^
	//
	//	����		inDev			���o�Ώۂ�Direct3D�f�o�C�X
	//				inScreenSizeX	�X�N���[���T�C�YX��
	//				inScreenSizeY	�X�N���[���T�C�YY��
	//	�X�V		2008/12/01		<�V�K>
	//================================================================================
	CDIRECTION::CDIRECTION(DEV inDev, int inScreenSizeX, int inScreenSizeY)
	{
	//	�����̏�����
		targetDevice	= inDev;
		iScreenSizeX	= inScreenSizeX;
		iScreenSizeY	= inScreenSizeY;
		cspd			= NULL;
	}

	//================================================================================
	//!	���\�b�h��	CDIRECTION::�f�X�g���N�^
	//
	//	�X�V		2008/12/01		<�V�K>
	//================================================================================
	CDIRECTION::~CDIRECTION(void)
	{
	//	���ꂼ����
		release();
	}

	//================================================================================
	//!	���\�b�h��	CDIRECTION::initCinemaScopePaintOut
	//
	//	�@�\		�V�l�}�X�R�[�v(�h��ׂ��^�C�v)�̏���
	//	����		inTopHeight		�㕔�̍���
	//				inBottomHeight	�����̍���
	//				inColor			�F
	//				inTexName		�h��ׂ��Ɏg���e�N�X�`���̃p�X(NULL)
	//	�X�V		2008/12/01		<�V�K>
	//================================================================================
	void
	CDIRECTION::initCinemaScopePaintOut(int inTopHeight, int inBottomHeight, float fPosZ,
										DWORD inColor, LPSTR inTexName)
	{
	//	�������
		releaseCinemaScopePaintOut();
	//	����
		cspd	= new CINEMASCOPE_PAINTOUT_DATA();
	//	���s������A������Ԃ�
		if(	!cspd){ cspd = NULL; return;	}
	//	�e�N�X�`���̐���(�e�N�X�`������NULL�Ȃ琶�����Ȃ�)
		if(	!inTexName)	{ cspd->texture = NULL;	}
		else
		{
		//	�e�N�X�`���̐����Ɏ��s������ANULL�����Ă���
			if(	FAILED(D3DX_CREATE_TEXTURE_EX(
						targetDevice,	inTexName, 0,
						NULL, &cspd->texture)))
			{
				cspd->texture	= NULL;
			}
		}
	//	�������p�z��
	//	�㕔
		setVertex2D(&cspd->surfaceTop[0],
			-1.0f, -1.0f, fPosZ, 0.0f, inColor, 0.0f, 0.0f);
		setVertex2D(&cspd->surfaceTop[1],
			(float)iScreenSizeX, -1.0f, fPosZ, 0.0f, inColor, 1.0f, 0.0f);
		setVertex2D(&cspd->surfaceTop[2],
			(float)iScreenSizeX, (float)inTopHeight, fPosZ, 0.0f, inColor, 1.0f, 1.0f);
		setVertex2D(&cspd->surfaceTop[3],
			-1.0f, (float)inTopHeight, fPosZ, 0.0f, inColor, 0.0f, 1.0f);
	//	����
		setVertex2D(&cspd->surfaceBottom[0],
			-1.0f, (float)iScreenSizeY-inBottomHeight, fPosZ, 0.0f, inColor, 0.0f, 0.0f);
		setVertex2D(&cspd->surfaceBottom[1],
			(float)iScreenSizeX, (float)iScreenSizeY-inBottomHeight, fPosZ, 0.0f, inColor, 1.0f, 0.0f);
		setVertex2D(&cspd->surfaceBottom[2],
			(float)iScreenSizeX, (float)iScreenSizeY, fPosZ, 0.0f, inColor, 1.0f, 1.0f);
		setVertex2D(&cspd->surfaceBottom[3],
			-1.0f, (float)iScreenSizeY, fPosZ, 0.0f, inColor, 0.0f, 1.0f);
	}

	//================================================================================
	//!	���\�b�h��	CDIRECTION::directCinemaScopePaintOut
	//
	//	�@�\		�V�l�}�X�R�[�v(�h��ׂ��^�C�v)�̉��o
	//	�X�V		2008/12/01		<�V�K>
	//================================================================================
	void
	CDIRECTION::directCinemaScopePaintOut(void)
	{
	//	����������ĂȂ���΁A������Ԃ�
		if(	!cspd)	return;

		DWORD	dwFVF=0;	//!< FVF�̃o�b�N�A�b�v�p

	//	�e�N�X�`���̃Z�b�g
		targetDevice->SetTexture(0, cspd->texture);
		targetDevice->GetFVF(&dwFVF);
		targetDevice->SetFVF(D3DFVF_VERTEX_2D);
	//	�㕔�̕`��
		targetDevice->DrawPrimitiveUP(
			D3DPT_TRIANGLEFAN, 2, cspd->surfaceTop, sizeof(CUSTOM_VERTEX_2D));
	//	�����̕`��
		targetDevice->DrawPrimitiveUP(
			D3DPT_TRIANGLEFAN, 2, cspd->surfaceBottom, sizeof(CUSTOM_VERTEX_2D));
		targetDevice->SetFVF(dwFVF);
		targetDevice->SetTexture(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CDIRECTION::releaseCinemaScopePaintOut
	//
	//	�@�\		�V�l�}�X�R�[�v(�h��ׂ��^�C�v)�̉��
	//	�X�V		2008/12/01		<�V�K>
	//================================================================================
	void
	CDIRECTION::releaseCinemaScopePaintOut(void)
	{
	//	���
		SAFE_DELETE(cspd);
	}
}
//-----------------------------------------------------------------------
//!	DX���C�u�����̖��O���
//-----------------------------------------------------------------------