//***********************************************************************************
//!	�t�@�C����		Class_EnemyShadollSword.cpp
//
//	�@�\			�G�L�����N�^�[�N���X�u�V���h�[���\�[�h�v�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2009/01/19	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_EnemyShadollSword.h"		//!< �A�h�x���`���[�G�l�~�[�u�V���h�[���\�[�h�v

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_SWORD::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		���b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	CENEMY_SHADOLL_SWORD::CENEMY_SHADOLL_SWORD(	CONST DEV	inDev,
												CONST LPSTR	inName,
												CONST LPSTR	inResource,
												CONST LPSTR	inTexture)	:
	CKAGETOKI_ADVENT_ENEMY(inDev, inName, inResource, inTexture)
	{
	//	�����̏�����
		this->fFloorSpace				= 15.0f;				//!< ���Ƃ̊Ԋu
		this->fWallSpace				= 10.0f;				//!< �ǂƂ̊Ԋu

	//	�A�j���[�V�����̊��蓖�Ă��s��
		this->iAnimTemp[eESHADOLLSWORDA_WAIT]	= 0;
		this->iAnimTemp[eESHADOLLSWORDA_WALK]	= 1;
		this->iAnimTemp[eESHADOLLSWORDA_ATTACK]	= 2;

		this->iMaxLife			= 2;							//!< �ő�̗�
		this->iLife				= this->iMaxLife;				//!< �c��̗�
		this->iHitFrame			= 0;							//!< �Փ˂��ē_�ł��鎞��

	//	�t���O�Q
		this->action			= eESHADOLLSWORDA_MAX;			//!< �ŏ��͑ҋ@�ȊO���ł��ǂ�
		this->changeAction(eESHADOLLSWORDA_WAIT);				//!< ������Ԃ͏o��
		this->setThisMessage(eTMSG_ENEMY);						//!< �G�\��

		this->stepCnt			= 0;
	//	���̐ݒ�
		this->sStaticStep		= new SSTATIC(SOUND_STEP);
		this->sStaticJumpStart	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticJumpEnd	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticStep->setVolume(-1500);
		this->sStaticJumpStart->setVolume(-1500);
		this->sStaticJumpEnd->setVolume(-1500);

	//	�G�t�F�N�g�̓o�^
		C3D_EFFECT_CENTER*	effectCenter	= C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			EFFECT_SMOKE, new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));

	//	���[�J�����W�̕ύX
		this->sd_ptr->mesh->localCenter = VEC3(0.0f, 50.0f, 0.0f);
		this->sd_ptr->mesh->localRadius	= 100.0f;
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_SWORD::�f�X�g���N�^
	//
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	CENEMY_SHADOLL_SWORD::~CENEMY_SHADOLL_SWORD(void)
	{
	//	���֘A�̉��
		SAFE_DELETE(this->sStaticStep);				//!< ����
		SAFE_DELETE(this->sStaticJumpStart);		//!< �W�����v��
		SAFE_DELETE(this->sStaticJumpEnd);			//!< ���n��
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_SWORD::soundStep
	//
	//	�@�\		�����Ǘ�
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOLL_SWORD::soundStep(void)
	{
		this->stepCnt++;

	//	�X���[�N��10�t���[�����ɔ���
		if(	this->stepCnt > 10)
		{
			C3D_EFFECT_CENTER::occuEffectsInstance(EFFECT_SMOKE, this->getLoc(), 0.3f);
			this->sStaticStep->setPlayCur(0);
			this->sStaticStep->play(0, NULL);
			this->stepCnt	= 0;
		}
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_SWORD::soundJumpStart
	//
	//	�@�\		�W�����v���Ǘ�
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOLL_SWORD::soundJumpStart(void)
	{
	//	�W�����v����炷
		this->sStaticJumpStart->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_SWORD::soundJumpEnd
	//
	//	�@�\		���n���Ǘ�
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOLL_SWORD::soundJumpEnd(void)
	{
	//	���n����炷
		this->sStaticJumpEnd->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_SWORD::opeWait
	//
	//	�@�\		�ҋ@������
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOLL_SWORD::opeWait(void)
	{
	//	�d�͌���
		this->opeEffectGravity();
	//	�����]��
		this->opeTurn();

	//	�v���C���[�ɓ���������A�_���[�W
		if(	(*this->player)->intersect(
					this, ADVENT_FRONTHIT, this->fWallSpace))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

	//	�t���[�������܂ŏオ������ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 60)
		{
			this->changeAction(eESHADOLLSWORDA_WALK);
		}
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_SWORD::opeWalk
	//
	//	�@�\		���s������
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOLL_SWORD::opeWalk(void)
	{
	//	�d�͌���
		this->opeEffectGravity();
	//	����
		this->soundStep();

	//	�ړ�����l���m��
		float	moveValue	= this->fWallSpace + this->getMoveQuantity() + 100.0f;

	//	�s���͈͂���o�悤�Ƃ�����ҋ@
		if(	!this->actionArea.intersect(
				this->calcNextLoc(moveValue, &LINE(eUD_NO, eLR_NO, ADVENT_FRONTHIT))))
		{
		//	�����]��
			switch(this->way)
			{
				case eADCW_LEFT:	this->way = eADCW_RIGHT_TURN;	break;
				case eADCW_RIGHT:	this->way = eADCW_LEFT_TURN;	break;
			}
			this->changeAction(eESHADOLLSWORDA_WAIT);
			return;
		}

		else
	//	�ːi���A�ǂɓ���������ҋ@
		if(	this->stageG->intersect(this, ADVENT_FRONTHIT, moveValue))
		{
		//	�����]��
			switch(this->way)
			{
				case eADCW_LEFT:	this->way = eADCW_RIGHT_TURN;	break;
				case eADCW_RIGHT:	this->way = eADCW_LEFT_TURN;	break;
			}
			this->changeAction(eESHADOLLSWORDA_WAIT);
			return;
		}
		else
	//	�v���C���[�ɓ���������A�_���[�W
		if(	(*this->player)->intersect(this, ADVENT_FRONTHIT, this->getMoveQuantity()))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

	//	�����܂ŗ�����A�ړ�
		ADVENT_FRONTMOVE;

	//	�t���[�������܂ŏオ������ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 1000)
		{
			this->changeAction(eESHADOLLSWORDA_WAIT);
		}

	//	�v���C���[���߂�������A���ōU��
	//	���a���L����
		if(	(*this->player)->intersectRadius(this, 1000.0f))
		{
			this->changeAction(eESHADOLLSWORDA_ATTACK);
		}
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_SWORD::opeAttack
	//
	//	�@�\		�_�b�V��������
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOLL_SWORD::opeAttack(void)
	{
	//	�d�͌���
		this->opeEffectGravity();
	//	����
		this->soundStep();

	//	�ړ�����l���m��
		float	moveValue	= this->fWallSpace + this->getMoveQuantity() + 100.0f;

	//	�s���͈͂���o�悤�Ƃ�����ҋ@
		if(	!this->actionArea.intersect(
				this->calcNextLoc(moveValue, &LINE(eUD_NO, eLR_NO, ADVENT_FRONTHIT))))
		{
		//	�����]��
			switch(this->way)
			{
				case eADCW_LEFT:	this->way = eADCW_RIGHT_TURN;	break;
				case eADCW_RIGHT:	this->way = eADCW_LEFT_TURN;	break;
			}
			this->changeAction(eESHADOLLSWORDA_WAIT);
			return;
		}

		else
	//	�ːi���A�ǂɓ���������ҋ@
		if(	this->stageG->intersect(this, ADVENT_FRONTHIT, moveValue))
		{
		//	�����]��
			switch(this->way)
			{
				case eADCW_LEFT:	this->way = eADCW_RIGHT_TURN;	break;
				case eADCW_RIGHT:	this->way = eADCW_LEFT_TURN;	break;
			}
			this->changeAction(eESHADOLLSWORDA_WAIT);
			return;
		}

		else
	//	�v���C���[�ɓ���������A�_���[�W
		if(	(*this->player)->intersect(this, ADVENT_FRONTHIT, this->getMoveQuantity()))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV2);
		}

	//	�����܂ŗ�����A�ړ�
		ADVENT_FRONTMOVE;

	//	�t���[�������܂ŏオ������ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 40)
		{
			this->changeAction(eESHADOLLSWORDA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_SWORD::changeAction
	//
	//	�@�\		��ԕω�
	//	����		inNewAction		�V���ȍs���p�^�[��
	//	�߂�l		TRUE			�V���ȍs���p�^�[���ɕω�����
	//				FALSE			�V���ȍs���p�^�[���ɕω����Ă��Ȃ�
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	BOOL
	CENEMY_SHADOLL_SWORD::changeAction(ENEMYSHADOLLSWORD_ACTION inNewAction)
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
			case	eESHADOLLSWORDA_WAIT:			//!< �ҋ@��
				this->setMoveQuantity(0.0f);
				break;
			case	eESHADOLLSWORDA_WALK:			//!< ���s��
				this->setMoveQuantity(20.0f);
				break;
			case	eESHADOLLSWORDA_ATTACK:			//!< ���ōU����
				this->setMoveQuantity(30.0f);
				break;
			default:								//!< ��L�ȊO
				break;
		}

	//	�t���[���J�E���^��������
		this->iFrameCnt	= 0;

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_SWORD::reactionMessage
	//
	//	�@�\		���A�N�V����
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOLL_SWORD::reactionMessage(REACTION_MESSAGE inMessage)
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
	//!	���\�b�h��	CENEMY_SHADOLL_SWORD::operation
	//
	//	�@�\		��{����
	//	����		*input			���͊Ǘ�
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOLL_SWORD::operation(CINPUT* input)
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
			case	eESHADOLLSWORDA_WAIT:			//!< �ҋ@��
				this->opeWait();
				break;
			case	eESHADOLLSWORDA_WALK:			//!< ���s��
				this->opeWalk();
				break;
			case	eESHADOLLSWORDA_ATTACK:			//!< ���ōU����
				this->opeAttack();
				break;
		}//	default	break;
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOLL_SWORD::draw
	//
	//	�@�\		�`��
	//	����		inDev			Direct3D�f�o�C�X
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	BOOL
	CENEMY_SHADOLL_SWORD::draw(CONST DEV inDev)
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