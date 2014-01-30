//***********************************************************************************
//!	�t�@�C����		Class_EnemyShadollArchery.cpp
//
//	�@�\			�G�L�����N�^�[�N���X�u�V���h�[���A�[�`�F���[�v�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2009/02/20	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_EnemyShadollArchery.h"		//!< �A�h�x���`���[�Q�[���u�V���h�[���A�[�`�F���[�v

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_ARCHERY::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		���b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2009/02/20		<�V�K>
	//================================================================================
	CENEMY_SHADOLL_ARCHERY::CENEMY_SHADOLL_ARCHERY(
									CONST DEV	inDev,
									CONST LPSTR	inName,
									CONST LPSTR	inResource,
									CONST LPSTR	inTexture)	:
	CKAGETOKI_ADVENT_ENEMY(inDev, inName, inResource, inTexture)
	{
	//	�����̏�����
		this->fFloorSpace				= 15.0f;				//!< ���Ƃ̊Ԋu
		this->fWallSpace				= 10.0f;				//!< �ǂƂ̊Ԋu

	//	�A�j���[�V�����̊��蓖�Ă��s��
		this->iAnimTemp[eESHADOLLARCHERYA_WAIT]			= 0;
		this->iAnimTemp[eESHADOLLARCHERYA_ARROWINIT]	= 1;
		this->iAnimTemp[eESHADOLLARCHERYA_ARROW]		= 2;

		this->iMaxLife					= 2;					//!< �ő�̗�
		this->iLife						= this->iMaxLife;		//!< �c��̗�
		this->iHitFrame					= 0;					//!< �Փ˂��ē_�ł��鎞��

	//	�t���O�Q
		this->action			= eESHADOLLARCHERYA_MAX;		//!< �ŏ��͑ҋ@�ȊO���ł��ǂ�
		this->changeAction(eESHADOLLARCHERYA_WAIT);				//!< ������Ԃ͏o��
		this->setThisMessage(eTMSG_ENEMY);						//!< �G�\��

	//	�I���W�i���u�G�l�~�[�{�[���v�̐���
		this->originArrow
			= new CENEMY_BALL(inDev, eENEMYB_SHADOLL_ARROW, "enemy_shadoll_archery_arrow", EFFECT_SMOKE);
		this->originArrow->setEffectColor(D3DCOLOR_RGBA(100,100,100,255));
		this->originArrow->setReactMessage(eREAMSG_ENEMY_ATTACK_LV1);
		this->originArrow->setEffectCnt(3);
		this->ssArrowShot	= NULL;

	//	�G�t�F�N�g�̓o�^
		C3D_EFFECT_CENTER*	effectCenter	= C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			EFFECT_SMOKE, new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));

	//	���[�J�����W�̕ύX
		this->sd_ptr->mesh->localCenter = VEC3(0.0f, 50.0f, 0.0f);
		this->sd_ptr->mesh->localRadius	= 150.0f;
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_ARCHERY::�f�X�g���N�^
	//
	//	�X�V		2009/02/20		<�V�K>
	//================================================================================
	CENEMY_SHADOLL_ARCHERY::~CENEMY_SHADOLL_ARCHERY(void)
	{
	//	���
		SAFE_DELETE(this->originArrow);
		SAFE_DELETE(this->ssArrowShot);
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_ARCHERY::opeWait
	//
	//	�@�\		�ҋ@������
	//	�X�V		2009/02/20		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOLL_ARCHERY::opeWait(void)
	{
	//	�d�͌���
		this->opeEffectGravity();
	//	�L�����N�^�[�̕��ɕ����]��
		if(	(*this->player)->getLocX() < this->getLocX())
		{
			this->way = eADCW_LEFT_TURN;
		}
		else
		{
			this->way = eADCW_RIGHT_TURN;
		}
		this->opeTurn();

	//	�v���C���[�ɓ���������A�_���[�W
		if(	(*this->player)->intersectRadius(this))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

	//	�t���[�������܂ŏオ������ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10)
		{
			this->changeAction(eESHADOLLARCHERYA_ARROWINIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_ARCHERY::opeArrowInit
	//
	//	�@�\		���s������
	//	�X�V		2009/02/20		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOLL_ARCHERY::opeArrowInit(void)
	{
	//	�d�͌���
		this->opeEffectGravity();

	//	�v���C���[�ɓ���������A�_���[�W
		if(	(*this->player)->intersectRadius(this))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

	//	�t���[�������܂ŏオ������ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 60)
		{
			this->changeAction(eESHADOLLARCHERYA_ARROW);
		}
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_ARCHERY::opeArrow
	//
	//	�@�\		�_�b�V��������
	//	�X�V		2009/02/20		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOLL_ARCHERY::opeArrow(void)
	{
	//	�d�͌���
		this->opeEffectGravity();

	//	�v���C���[�ɓ���������A�_���[�W
		if(	(*this->player)->intersectRadius(this))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

	//	�t���[�������܂ŏオ������ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 30)
		{
			this->changeAction(eESHADOLLARCHERYA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::skillArrowShot
	//
	//	�@�\		�Z�u�A���[�V���b�g�v
	//	�X�V		2009/02/21		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOLL_ARCHERY::skillArrowShot(void)
	{
	//	�A���[�𐶐�
		CENEMY_BALL*	newArrow	= new CENEMY_BALL(this->originArrow);

	//	�A���[�̐ݒ�
		newArrow->shot(
			&this->getAngle(),								//!< �p�x
			&this->getWorldCenter(),						//!< ���ˈʒu
			this->stageG,									//!< �X�e�[�W�I�u�W�F�N�g�O���[�v
			this->player,									//!< �G�I�u�W�F�N�g�O���[�v
			100);											//!< ���s���Ď���
		newArrow->setMoveQuantity(20.0f);					//!< �X�s�[�h�ݒ�
		newArrow->setScale(1.0f);							//!< �g��
		newArrow->setLine(ADVENT_FRONTHIT);					//!< ����(3D��Ԃɑ΂���)
		newArrow->setOrderMessage(eOMSG_DELETE_IN);			//!< �����
	//	�A���[���ˉ�

	//	�A���[��`��O���[�v�ɓo�^
		this->viewG->add(newArrow);
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_ARCHERY::changeAction
	//
	//	�@�\		��ԕω�
	//	����		inNewAction		�V���ȍs���p�^�[��
	//	�߂�l		TRUE			�V���ȍs���p�^�[���ɕω�����
	//				FALSE			�V���ȍs���p�^�[���ɕω����Ă��Ȃ�
	//	�X�V		2009/02/20		<�V�K>
	//================================================================================
	BOOL
	CENEMY_SHADOLL_ARCHERY::changeAction(ENEMYSHADOLLARCHERY_ACTION inNewAction)
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
			case	eESHADOLLARCHERYA_WAIT:			//!< �ҋ@��
				this->setMoveQuantity(0.0f);
				break;
			case	eESHADOLLARCHERYA_ARROWINIT:	//!< ��̏���
				this->setMoveQuantity(0.0f);
				break;
			case	eESHADOLLARCHERYA_ARROW:		//!< ����˂�
				this->setMoveQuantity(0.0f);
				this->skillArrowShot();
				break;
			default:								//!< ��L�ȊO
				break;
		}

	//	�t���[���J�E���^��������
		this->iFrameCnt	= 0;

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_ARCHERY::reactionMessage
	//
	//	�@�\		���A�N�V����
	//	�X�V		2009/02/20		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOLL_ARCHERY::reactionMessage(REACTION_MESSAGE inMessage)
	{
	//	�G�i�W�[�{�[���ɓ���������
		if(	inMessage == eREAMSG_ENERGY_HIT)
		{
		//	�Փ˃t���[����0�ȉ��Ȃ�A�_���[�W
			if(	this->iHitFrame <= 0)
			{
				this->iHitFrame  = 100;
				this->iLife--;
			}
		}

	//	���C�t���Ȃ���������A������
		if(	this->iLife <= 0)
		{
			this->setVisible(FALSE);
		}
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_ARCHERY::operation
	//
	//	�@�\		��{����
	//	����		*input			���͊Ǘ�
	//	�X�V		2009/02/20		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOLL_ARCHERY::operation(CINPUT* input)
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
			case	eESHADOLLARCHERYA_WAIT:			//!< �ҋ@��
				this->opeWait();
				break;
			case	eESHADOLLARCHERYA_ARROWINIT:	//!< ��̏���
				this->opeArrowInit();
				break;
			case	eESHADOLLARCHERYA_ARROW:		//!< ����˂�
				this->opeArrow();
				break;
		}//	default	break;
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_ARCHERY::draw
	//
	//	�@�\		�`��
	//	����		inDev			Direct3D�f�o�C�X
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2009/02/20		<�V�K>
	//================================================================================
	BOOL
	CENEMY_SHADOLL_ARCHERY::draw(CONST DEV inDev)
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

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------