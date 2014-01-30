//***********************************************************************************
//!	�t�@�C����		Class_SceneEvent.cpp
//
//	�@�\			�X�^�b�N���Ǘ�����N���X�̎���
//	�@�\�T�v		�X�^�b�N���Ǘ����鎖�ŏ����𕪊����鎖��ړI�Ƃ����N���X
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/09/27	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneEvent.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_EVENT::�R���X�g���N�^
	//
	//	�X�V		2008/09/27		<�V�K>
	//================================================================================
	CSCENE_EVENT::CSCENE_EVENT(void)
	{
	//	�����̏�����
		this->gDev				= CGRAPHIC_DEVICE::getInstance();
		this->effect			= C3D_EFFECT_CENTER::getInstance();
		this->font				= CFONT_BASE::getInstance();

		this->bStatusEnd		= FALSE;
		this->iStack			= 0;
		this->bTimer			= FALSE;
		this->iTimer			= 0;
		this->iTimerNextStack	= 0;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_EVENT::�f�X�g���N�^
	//
	//	�X�V		2008/09/27		<�V�K>
	//================================================================================
	CSCENE_EVENT::~CSCENE_EVENT(void)
	{
	//	�����Ő������ꂽ�G�t�F�N�g������
		this->effect->finishalize();
	//	�e�L�X�g�̉��
		if(	this->font)	this->font->releaseText();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_EVENT::startStatus
	//
	//	�@�\		�J�n
	//	����		inAlpha			���̃A���t�@�ϓ��l
	//	�X�V		2008/09/27		<�V�K>
	//================================================================================
	void
	CSCENE_EVENT::startStatus(int inAlpha)
	{
	//	�I���ڍs�m�F�𖳌�
		this->bStatusEnd	= FALSE;
	//	��������
		this->gDev->curtainMotion(FALSE, inAlpha);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_EVENT::endStatus
	//
	//	�@�\		�I��
	//	����		inAlpha			���̃A���t�@�ϓ��l
	//	�X�V		2008/09/27		<�V�K>
	//================================================================================
	void
	CSCENE_EVENT::endStatus(int inAlpha)
	{
	//	�I���ڍs�m�F�����ɗL���Ȃ珈�����Ȃ�
		if(	this->bStatusEnd)	return;
	//	�I���ڍs�m�F��L��
		this->bStatusEnd	= TRUE;
	//	�����o��
		this->gDev->curtainMotion(TRUE, inAlpha);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_EVENT::checkStatus
	//
	//	�@�\		�I���`�F�b�N
	//	�X�V		2008/09/27		<�V�K>
	//================================================================================
	BOOL
	CSCENE_EVENT::checkStatus(void)
	{
	//	���̕`��
		this->gDev->renderCurtainInDev();
	//	�I���ڍs�m�F���L���̎�
		if(	this->bStatusEnd)
		{
		//	���̓����x���ő�Ȃ�A�I���ڍs
			if(	this->gDev->getCurtainColorAlpha() >= 255)
			{
				return	FALSE;
			}
		}
		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_EVENT::initTimer
	//
	//	�@�\		�^�C�}�[�쓮
	//	����		inTime			���ւ̃t���[����
	//				inStack			�V�����X�^�b�N
	//				inNextStack		���̃X�^�b�N
	//	�X�V		2008/09/27		<�V�K>
	//================================================================================
	void
	CSCENE_EVENT::initTimer(int inTime, int inStack, int inNextStack)
	{
	//	�^�C�}�[�쓮���Ŗ�����
		if(!this->bTimer)
		{
			this->bTimer			= TRUE;
			this->iTimer			= inTime;
			this->iStack			= inStack;
			this->iTimerNextStack	= inNextStack;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_EVENT::checkTimer
	//
	//	�@�\		�^�C�}�[�`�F�b�N
	//	�X�V		2008/09/27		<�V�K>
	//================================================================================
	void
	CSCENE_EVENT::checkTimer(void)
	{
	//	�^�C�}�[�쓮���Ŗ�����
		if(	this->bTimer)
		{
		//	�^�C�}�[���l�����炵�Ă��A0�ȉ��ɂȂ�Ȃ���
			if(	(this->iTimer - 1) > 0)
			{
				this->iTimer--;
			}
		//	0�ȉ��ɂȂ鎞
			else
			{
				this->iTimer	= 0;
				this->bTimer	= FALSE;

			//	���̃X�^�b�N�̐��l�������łȂ���΁A�ڍs����
				if(	this->iTimerNextStack < INVALID_NEXTSTACK)
				{
					this->iStack	= this->iTimerNextStack;
				}
			}
		}
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------