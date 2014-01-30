//***********************************************************************************
//!	�t�@�C����		Class_BossTima.cpp
//
//	�@�\			�A�h�x���`���[�Q�[�����[�h�́u�e�B�}�v�N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2009/02/16	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_BossTima.h"		//!< �A�h�x���`���[�Q�[���u�e�B�}�v

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CBOSS_TIMA::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		���b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	CBOSS_TIMA::CBOSS_TIMA(CONST DEV	inDev,
						   CONST LPSTR	inName,
						   CONST LPSTR	inResource,
						   CONST LPSTR	inTexture)	:
	CKAGETOKI_ADVENT_BOSS(inDev, inName, inResource, inTexture)
	{
	//	�����̏�����
		this->fJumpHeight				= 80.0f;				//!< �W�����v�̍���
		this->fLeftRightMoveSpeed		= 0.0f;					//!< ���E�ړ��X�s�[�h
		this->fLeftRightMoveMaxSpeed	= 20.0f;				//!< ���E�ړ��ő�X�s�[�h
		this->fLeftRightMoveAccele		= 2.0f;					//!< ���E�ړ������l

		this->fFloorSpace				= 8.0f;					//!< ���Ƃ̊Ԋu
		this->fWallSpace				= 100.0f;				//!< �ǂƂ̊Ԋu

	//	�A�j���[�V�����̊��蓖�Ă��s��(�e�B�}��p�̃N���X�Ȃ̂ŁA�A�A)
		this->iAnimTemp[eBTIMAA_APPEAR]			= 2;
		this->iAnimTemp[eBTIMAA_APPEAR_WAIT]	= 0;
		this->iAnimTemp[eBTIMAA_WAIT]			= 0;
		this->iAnimTemp[eBTIMAA_DAMAGE]			= 6;

		this->iMaxLife					= TIMA_MAXLIFE;			//!< �ő�̗�
		this->iLife						= this->iMaxLife;		//!< �c��̗�
		this->iHitFrame					= 0;					//!< �Փ˂��ē_�ł��鎞��

	//	�t���O�Q
		this->action			= eBTIMAA_MAX;					//!< �ŏ��͑ҋ@�ȊO���ł��ǂ�
		this->changeAction(eBTIMAA_APPEAR);						//!< ������Ԃ͏o��

		this->stepCnt			= 0;
	//	���̃Z�b�g
		this->sStaticStep		= new SSTATIC(SOUND_STEP);
		this->sStaticJumpStart	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticJumpEnd	= new SSTATIC(SOUND_JUMP_START);

	//	�G�t�F�N�g�̓o�^
		C3D_EFFECT_CENTER*	effectCenter	= C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			EFFECT_SMOKE, new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));

	//	���[�J�����W�̕ύX
		this->sd_ptr->mesh->localCenter = VEC3(0.0f, 100.0f, 0.0f);
		this->sd_ptr->mesh->localRadius	= 230.0f;

	//	�C���^�[�t�F�[�X�e�N�X�`����ǂݍ���
		if(	FAILED(D3DX_CREATE_TEXTURE_EX(
				inDev, "..//Resources//Texture//boss_exe_interface.dds",
				D3DCOLOR_RGBA(100,100,100,100), NULL, &this->interfaceTex)))
		{
		//	���s������ANULL������
			this->interfaceTex	= NULL;
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_TIMA::�f�X�g���N�^
	//
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	CBOSS_TIMA::~CBOSS_TIMA(void)
	{
	//	���֘A�̉��
		SAFE_DELETE(this->sStaticStep);				//!< ����
		SAFE_DELETE(this->sStaticJumpStart);		//!< �W�����v��
		SAFE_DELETE(this->sStaticJumpEnd);			//!< ���n��
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_TIMA::soundStep
	//
	//	�@�\		�����Ǘ�
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_TIMA::soundStep(void)
	{
		this->stepCnt++;

	//	�X���[�N��20�t���[�����ɔ���
		if(	this->stepCnt > 5)
		{
			C3D_EFFECT_CENTER::occuEffectsInstance(EFFECT_SMOKE, this->getLoc(), 0.3f);
			this->sStaticStep->setPlayCur(0);
			this->sStaticStep->play(0, NULL);
			this->stepCnt	= 0;
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_TIMA::soundJumpStart
	//
	//	�@�\		�W�����v���Ǘ�
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_TIMA::soundJumpStart(void)
	{
	//	�W�����v����炷
		this->sStaticJumpStart->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_TIMA::soundJumpEnd
	//
	//	�@�\		���n���Ǘ�
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_TIMA::soundJumpEnd(void)
	{
	//	���n����炷
		this->sStaticJumpEnd->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_TIMA::opeAppear
	//
	//	�@�\		�o��������
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_TIMA::opeAppear(void)
	{
	//	�d�͌���
		this->opeEffectGravity();
	//	�v���C���[�̕�������
		this->opeTurnToPlayer();
	//	�e�B�}���n�ʂɕt������A�ҋ@
		if(	!this->bFlighting)
		{
			this->changeAction(eBTIMAA_APPEAR_WAIT);
			this->soundJumpEnd();	//!< ���n��
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_TIMA::opeAppearWait
	//
	//	�@�\		�o���ҋ@������
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_TIMA::opeAppearWait(void)
	{
	//	�d�͌���
		this->opeEffectGravity();
	//	�v���C���[�̕�������
		this->opeTurnToPlayer();
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_TIMA::opeWait
	//
	//	�@�\		�ҋ@������
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_TIMA::opeWait(void)
	{
	//	�d�͌���
		this->opeEffectGravity();

	//	��L�̏����ŁA�󒆂ɂ���Ȃ�A��Ԃ��u��s�v�ɕύX
		if(	this->bFlighting)
		{
		}
	//	�󒆂ɂ��Ȃ��Ȃ�
		else
		{
		//	�v���C���[�̕�������
			this->opeTurnToPlayer();
		//	�t���[�������܂ōs������A�����_���ɍU���Ɉڂ�
			this->iFrameCnt++;
			if(	this->iFrameCnt > 30)
			{
				switch(rand() % 2)
				{
				//	�󒆃h�����N���[
					case 0:
						break;
				//	�_�b�V���N���[
					case 1:
						break;
				}
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_TIMA::opeDamage
	//
	//	�@�\		�_���[�W������
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_TIMA::opeDamage(void)
	{
	//	�ジ����
		this->moveRear();
	//	�t���[�������܂ōs������A�ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10)
		{
		//	�v���C���[�̕�������
			this->opeTurnToPlayer();
			this->changeAction(eBTIMAA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_TIMA::changeAction
	//
	//	�@�\		��ԕω�
	//	����		inNewAction		�V���ȍs���p�^�[��
	//	�߂�l		TRUE			�V���ȍs���p�^�[���ɕω�����
	//				FALSE			�V���ȍs���p�^�[���ɕω����Ă��Ȃ�
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	BOOL
	CBOSS_TIMA::changeAction(BOSSTIMA_ACTION inNewAction)
	{
	//	�O�̏�ԂƐV���ȏ�Ԃ��ς�Ȃ��Ȃ�A�������Ȃ�
		if(	this->action == inNewAction)	return	FALSE;
	//	��L�̏������X���[�����̂ŁA��Ԃ̕ύX
		this->action = inNewAction;
	//	�A�j���[�V�������ύX
		this->changeAnimationAndBlend(this->iAnimTemp[this->action]);

	//	�V���ȏ�ԂŒǉ�����������Ȃ炱����
		switch(this->action)
		{
			case	eBTIMAA_APPEAR:				//!< �o����
				this->setAngleZ(0.0f);
				break;
			case	eBTIMAA_APPEAR_WAIT:			//!< �o���ҋ@��
				this->setMoveQuantity(0.0f);
				this->setAngleZ(0.0f);
				break;
			case	eBTIMAA_WAIT:				//!< �ҋ@��
				this->setMoveQuantity(30.0f);
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				break;
			case	eBTIMAA_DAMAGE:				//!< �_���[�W������
				this->setMoveQuantity(10.0f);
				this->setAngleZ(0.0f);
				break;
			default:							//!< ��L�ȊO
				break;
		}

	//	�t���[���J�E���^��������
		this->iFrameCnt	= 0;

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_TIMA::reactionMessage
	//
	//	�@�\		���A�N�V����
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_TIMA::reactionMessage(REACTION_MESSAGE inMessage)
	{
	//	�G�i�W�[�{�[���ɓ���������
		if(	inMessage == eREAMSG_ENERGY_HIT)
		{
		//	�Փ˃t���[����0�ȉ��Ȃ�A�_���[�W
			if(	this->iHitFrame <= 0)
			{
				this->iHitFrame  = 100;
				this->iLife--;
			//	�U���ȊO�̎��Ȃ�A�ジ����
				if(	1)
				{
					this->changeAction(eBTIMAA_DAMAGE);
				}
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_TIMA::resetParameter
	//
	//	�@�\		�p�����[�^�̃��Z�b�g
	//	�X�V		2009/02/18		<�V�K>
	//================================================================================
	void
	CBOSS_TIMA::resetParameter(void)
	{
		CKAGETOKI_ADVENT_BOSS::resetParameter();	//!< ���C�t�̃��Z�b�g
		this->iHitFrame	= 0;						//!< �Փ˂��ē_�ł��鎞��
		this->changeAction(eBTIMAA_APPEAR);			//!< ������Ԃ͏o��
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_TIMA::operation
	//
	//	�@�\		��{����
	//	����		*input			���͊Ǘ�
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_TIMA::operation(CINPUT* input)
	{
	//	�\������Ă��Ȃ���΁A�������Ȃ�
		if(	!this->getVisible())	return;
	//	�v���C���[���ݒ肳��Ă��Ȃ���Ώ������Ȃ�
		if(	!(*this->player))		return;

	////	�O���[�v�̓o�^���Ȃ���΁A�������Ȃ�
	//	if(	!this->viewG	||
	//		!this->stageG	||
	//		!this->enemyG)	return;

	//	��Ԃɂ�鑀��̕���
		switch(this->action)
		{
			case	eBTIMAA_APPEAR:				//!< �o����
				this->opeAppear();
				break;
			case	eBTIMAA_APPEAR_WAIT:			//!< �o���ҋ@��
				this->opeAppearWait();
				break;
			case	eBTIMAA_WAIT:				//!< �ҋ@��
				this->opeWait();
				break;
			case	eBTIMAA_DAMAGE:				//!< �_���[�W������
				this->opeDamage();
				break;
		}//	default	break;
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_TIMA::draw
	//
	//	�@�\		�`��
	//	����		inDev			Direct3D�f�o�C�X
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	BOOL
	CBOSS_TIMA::draw(CONST DEV inDev)
	{
	//	�_�ŏ��������Ȃ���`��
		if(	this->iHitFrame > 0)
		{
			if(	this->iHitFrame % 2){ CSKIN_MESH::draw(inDev);	}
			else					{ this->animation();		}
			this->iHitFrame--;
		}
	//	�_�ł��ĂȂ��Ȃ�A���ʕ`��
		else
		{
			CSKIN_MESH::draw(inDev);
		}

	//	�C���^�[�t�F�[�X�`��
		this->drawInterface(inDev);

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------