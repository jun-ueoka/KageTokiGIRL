//***********************************************************************************
//!	�t�@�C����		WinMain.cpp
//
//	�@�\			WinMain�Ɋւ��鏈��
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/04/16	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_WindowManager.h"
//#include	"UtilityFunction.h"

//================================================================================
//!	�֐���		WndProc
//
//	�@�\		�E�B���h�E�v���V�[�W��
//	����		inWnd		�E�B���h�E�n���h��
//				inMessage	���b�Z�[�W
//				inWParam	W�p�����[�^
//				inLParam	L�p�����[�^
//	�߂�l		�����ɉ������l��Ԃ�
//	�X�V		2008/04/16	<�V�K>
//================================================================================
LRESULT CALLBACK
WndProc(HWND inWnd, UINT inMessage, WPARAM inWParam, LPARAM inLParam)
{
	LRESULT	ret((LRESULT)0);			//!< �߂�l�i�[�p

//!	���b�Z�[�W�ɂ���ď�������
	switch(inMessage)
	{
	//!	�E�B���h�E�j����
		case WM_DESTROY:
			DestroyWindow(inWnd);										//!< �E�B���h�E�̔j��
			PostQuitMessage(0);											//!< �V�X�e���ɏI���v��
			break;

	//! ���̑��̃��b�Z�[�W
		default:
			ret = DefWindowProc(inWnd, inMessage, inWParam, inLParam);	//!< Windows�ɏ��������肢
			break;
	}
	return ret;							//!< �����ɉ������l��Ԃ�
}

//================================================================================
//!	�֐���		WinMain
//
//	�@�\		�����J�n
//	����		hInstance	6�v���O�����Ɋ��蓖�Ă�ꂽ�C���X�^���X�n���h��
//				lpCmdLine	�R�}���h���C������̕�����
//				nShowCmd	�E�B���h�E�\�����@
//	�߂�l		�����I�����Ӗ�����
//	�X�V		2008/04/16	<�V�K>
//================================================================================
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
//! �f�o�b�O���[�h
#ifdef	_DEBUG
//!	���������[�N�̃`�F�b�N���s��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif//_DEBUG

	WM::CWINDOW_MANAGER*	WinMamager	= NULL;						//!< �E�B���h�E�Ǘ�
	int						iRet		= 0;						//!< �߂�l�p

//!	CWINDOW_MANAGER�̐���
	if(	!(WinMamager = WM::CWINDOW_MANAGER::getInstance(hInstance, nShowCmd)))
	{
		return 0;													//!< �����Ɏ��s
	}
	iRet	= WinMamager->messageLoop();							//!< �A�v���P�[�V�����̃��[�v
	WinMamager->releaseInstance();									//!< WinClass�̉��

#ifdef _DEBUG
//!	���[�N�`�F�b�N
	_CrtDumpMemoryLeaks();
#endif 

	return iRet;
}