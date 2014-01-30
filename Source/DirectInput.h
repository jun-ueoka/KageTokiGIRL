//***********************************************************************************
//!	�t�@�C����		DirectInput.h
//
//	�@�\			���ړ��͊֌W�̃f�[�^�̒�`
//	�v���W�F�N�g	����
//	�X�V			2008/04/25	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include <windows.h>

//-----------------------------------------------------------------------
//!	���ړ��̖͂��O���
//-----------------------------------------------------------------------
namespace DI
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�L�[�t���O
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef enum MYKEY
	{
		eKEY_UP,		//!< ���L�[
		eKEY_DOWN,		//!< ���L�[
		eKEY_LEFT,		//!< ���L�[
		eKEY_RIGHT,		//!< ���L�[
		eKEY_OK,		//!< �X�y�[�X�L�[
		eKEY_CANCEL,	//!< �V�t�g�L�[
		eKEY_QUIT,		//!< �G�X�P�[�v�L�[
		eKEY_SPACE,		//!< �X�y�[�X�L�[
		eKEY_ENTER,		//!< �G���^�[�L�[
		eKEY_A,			//!< A�L�[
		eKEY_C,			//!< C�L�[
		eKEY_D,			//!< D�L�[
		eKEY_S,			//!< S�L�[
		eKEY_W,			//!< W�L�[
		eKEY_X,			//!< X�L�[
		eKEY_Z,			//!< Z�L�[
		kMAX_KEY		//!< �L�[�e�[�u���̍Ō�
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�L�[�f�[�^
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct MYKEYDATA
	{
		DWORD	virtualKey;					//!< 16�i�L�[���
		BOOL	pressed;					//!< ������Ă��邩
		BOOL	pressedOnce;				//!< ���̃t���[���ŉ����ꂽ��
		BOOL	pressedNoneOnce;			//!< ���̃t���[���ŗ����ꂽ��
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�C���v�b�g����֐��Q
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	void diCheck_Key(void);					//!< �L�[�f�[�^�̏���ϐ��Ɋi�[���܂�
	BOOL diCheck_Press(MYKEY);				//!< �L�[��������Ă��邩�ǂ����𒲂ׂ܂�
	BOOL diCheck_PressOnce(MYKEY);			//!< �L�[�������ꂽ���ǂ����𒲂ׂ܂�
	BOOL diCheck_PressNone(void);			//!< �L�[��������Ă��Ȃ����ǂ����𒲂ׂ܂�
	BOOL diCheck_PressNoneOnce(MYKEY);		//!< �L�[�����߂ė����ꂽ���ǂ����𒲂ׂ܂�
}
//-----------------------------------------------------------------------
//!	���ړ��̖͂��O���
//-----------------------------------------------------------------------