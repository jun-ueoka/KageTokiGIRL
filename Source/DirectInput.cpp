//***********************************************************************************
//!	�t�@�C����		DirectInput.cpp
//
//	�@�\			DirectInput�֌W�B�L�[�̐ݒ�A�L�[�����Ȃ�
//	�v���W�F�N�g	����
//	�X�V			2008/04/25	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include "DirectInput.h"

//-----------------------------------------------------------------------
//!	���ړ��̖͂��O���
//-----------------------------------------------------------------------
namespace DI
{
	//================================================================================
	//!	�L�[�f�[�^�z��
	//================================================================================
	static MYKEYDATA g_key[kMAX_KEY] =
	{
		{VK_UP,			FALSE, FALSE, FALSE},		//!< ���L�[
		{VK_DOWN,		FALSE, FALSE, FALSE},		//!< ���L�[
		{VK_LEFT,		FALSE, FALSE, FALSE},		//!< ���L�[
		{VK_RIGHT,		FALSE, FALSE, FALSE},		//!< ���L�[
		{VK_SPACE,		FALSE, FALSE, FALSE},		//!< �X�y�[�X�L�[
		{VK_SHIFT,		FALSE, FALSE, FALSE},		//!< �V�t�g�L�[
		{VK_ESCAPE,		FALSE, FALSE, FALSE},		//!< �G�X�P�[�v�L�[
		{VK_SPACE,		FALSE, FALSE, FALSE},		//!< �X�y�[�X�L�[
		{VK_RETURN,		FALSE, FALSE, FALSE},		//!< �G���^�[�L�[
		{'A',			FALSE, FALSE, FALSE},		//!< A�L�[
		{'C',			FALSE, FALSE, FALSE},		//!< C�L�[
		{'D',			FALSE, FALSE, FALSE},		//!< D�L�[
		{'S',			FALSE, FALSE, FALSE},		//!< S�L�[
		{'W',			FALSE, FALSE, FALSE},		//!< W�L�[
		{'X',			FALSE, FALSE, FALSE},		//!< X�L�[
		{'Z',			FALSE, FALSE, FALSE},		//!< Z�L�[
	};

	//================================================================================
	//!	�֐���		diCheck_Key
	//
	//	�@�\		�L�[�f�[�^�̏����X�V
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void diCheck_Key(void)
	{
		int iCnt;		//!< ���[�v�J�E���^

		for(iCnt = 0; iCnt < kMAX_KEY; iCnt++)
		{
			if(GetAsyncKeyState(g_key[iCnt].virtualKey) & 0x8000)
			{
				if((!g_key[iCnt].pressedOnce) && (!g_key[iCnt].pressed))
				{
				//	���߂Ă��̃L�[�������ꂽ
					g_key[iCnt].pressedOnce	= true;
					g_key[iCnt].pressed		= true;
				}
				else
				{
				//	�O���牟����Ă���
					g_key[iCnt].pressedOnce	= false;
				}
			}
			else
			{
			//	�L�[�������ꂽ
				if(g_key[iCnt].pressed)
				{
				//	���߂ė����ꂽ�ꍇ
					g_key[iCnt].pressedOnce	= false;
					g_key[iCnt].pressed		= false;
					g_key[iCnt].pressedNoneOnce = true;
				}
				else
				{
					g_key[iCnt].pressedNoneOnce = false;
				}
			}
		}
	}

	//================================================================================
	//!	�֐���		diCheck_Press
	//
	//	�@�\		�L�[��������Ă��邩�ǂ����𒲂ׂ�
	//	����		myKey			���ׂ�L�[
	//	�߂�l		TRUE			������Ă���
	//				FALSE			������Ă��Ȃ�
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	BOOL diCheck_Press(MYKEY myKey)
	{
		return g_key[myKey].pressed;
	}

	//================================================================================
	//!	�֐���		diCheck_PressOnce
	//
	//	�@�\		�L�[�������ꂽ���ǂ����𒲂ׂ�
	//	����		myKey			���ׂ�L�[
	//	�߂�l		TRUE			�����ꂽ
	//				FALSE			�����Ă��Ȃ�(���ɉ�����Ă���)
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	BOOL diCheck_PressOnce(MYKEY myKey)
	{
		return g_key[myKey].pressedOnce;
	}

	//================================================================================
	//!	�֐���		diCheck_PressNone
	//
	//	�@�\		�L�[��������Ă��Ȃ����ǂ����𒲂ׂ�
	//	����		myKey			���ׂ�L�[
	//	�߂�l		TRUE			�����L�[��������Ă��Ȃ�
	//				FALSE			�����L�[��������Ă���
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	BOOL diCheck_PressNone(void)
	{
		BOOL pressNone;		//!< �L�[��������Ă��Ȃ����ǂ���

		pressNone = TRUE;	//!< �����l��TRUE

	//	���ׂẴL�[�𒲂ׂ�
		for(int i = 0; i < kMAX_KEY; i++)
		{
			if((g_key[i].pressed) || (g_key[i].pressedOnce))
			{
			//	�ǂ����̃L�[��������Ă����ꍇ
				pressNone = FALSE;
				break;
			}
		}

		return pressNone;
	}

	//================================================================================
	//!	�֐���		diCheck_PressNone
	//
	//	�@�\		�L�[�����߂ė����ꂽ���ǂ����𒲂ׂ�
	//	����		myKey			���ׂ�L�[
	//	�߂�l		TRUE			���L�[�������ꂽ
	//				FALSE			�O���牟����Ă��Ȃ��A�������͉�����Ă���
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	BOOL diCheck_PressNoneOnce(MYKEY myKey)
	{
		return g_key[myKey].pressedNoneOnce;
	}
}
//-----------------------------------------------------------------------
//!	���ړ��̖͂��O���
//-----------------------------------------------------------------------