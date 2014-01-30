//***********************************************************************************
//!	�t�@�C����		DirectX.cpp
//
//	�@�\			DirectX���g�p�����ŁA�K�v�Ȋ֐��A�}�N���̋L�q
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/12/02	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"DirectX.h"

//-----------------------------------------------------------------------
//	DirectX�g���֐��Q�̖��O���
//-----------------------------------------------------------------------
namespace	DX
{
	//================================================================================
	//!	�֐���		setVertex2D
	//
	//	�@�\		2D���_�̐ݒ�
	//	����		ver2D			�ݒ�撸�_
	//				inX,inY,inZ		XYZ���W
	//				inRhw			RHW�l
	//				inColor			�F���
	//				inU,inV			UV���W
	//	�X�V		2008/12/02		<�V�K>
	//================================================================================
	void
	setVertex2D(CUSTOM_VERTEX_2D* ver2D, float inX, float inY, float inZ,
				float inRhw, DWORD inColor, float inU, float inV)
	{
	//	�l�̃Z�b�g
		ver2D->x		= inX;
		ver2D->y		= inY;
		ver2D->z		= inZ;
		ver2D->rhw		= inRhw;
		ver2D->color	= inColor;
		ver2D->u		= inU;
		ver2D->v		= inV;
	}

	//================================================================================
	//!	�֐���		errorMsg
	//
	//	�@�\		�G���[���b�Z�[�W&�E�B���h�E�j��
	//	����		inDev			Direct3D�f�o�C�X
	//				inError			�G���[���b�Z�[�W
	//				inTitle			�G���[���b�Z�[�W�^�C�g��
	//	�X�V		2008/12/09		<�V�K>
	//================================================================================
	void
	errorMsg(CONST DEV inDev, LPSTR inError, LPSTR inTitle)
	{
		D3DDEVICE_CREATION_PARAMETERS	dcp;	//!< �f�o�C�X�����p�����[�^
	//	�f�o�C�X�쐬�p�����[�^���󂯎��
		inDev->GetCreationParameters(&dcp);
	//	�G���[���b�Z�[�W�̏o��
		MessageBox(dcp.hFocusWindow, inError, inTitle, MB_OK);
	//	�v���O�����̏I��
		DestroyWindow(dcp.hFocusWindow);
	}

	//================================================================================
	//!	�֐���		errorMsgNoFile
	//
	//	�@�\		�t�@�C���G���[���b�Z�[�W&�E�B���h�E�j��
	//	����		inDev			Direct3D�f�o�C�X
	//				inError			�G���[���b�Z�[�W
	//				inTitle			�G���[���b�Z�[�W�^�C�g��
	//	�X�V		2008/12/09		<�V�K>
	//================================================================================
	void
	errorMsgNoFile(CONST DEV inDev, LPSTR inError, LPSTR inTitle)
	{
		D3DDEVICE_CREATION_PARAMETERS	dcp;	//!< �f�o�C�X�����p�����[�^
		char	szError[MAX_PATH] = "";			//!< �G���[���b�Z�[�W
	//	�G���[���b�Z�[�W�̐���
		sprintf_s(szError, sizeof(szError), "Error File To [%s]", inError);
	//	�f�o�C�X�쐬�p�����[�^���󂯎��
		inDev->GetCreationParameters(&dcp);
	//	�G���[���b�Z�[�W�̏o��
		MessageBox(dcp.hFocusWindow, szError, inTitle, MB_OK);
	//	�v���O�����̏I��
		DestroyWindow(dcp.hFocusWindow);
	}
}
//-----------------------------------------------------------------------
//	DirectX�g���֐��Q�̖��O���
//-----------------------------------------------------------------------