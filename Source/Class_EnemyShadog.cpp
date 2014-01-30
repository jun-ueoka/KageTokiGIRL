//***********************************************************************************
//!	�t�@�C����		Class_EnemyShadog.cpp
//
//	�@�\			�G�L�����N�^�[�N���X�u�V���h�b�O�v�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2009/01/19	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_EnemyShadog.h"		//!< �A�h�x���`���[�G�l�~�[�u�V���h�b�O�v

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOG::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		���b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	CENEMY_SHADOG::CENEMY_SHADOG(	CONST DEV	inDev,
									CONST LPSTR	inName,
									CONST LPSTR	inResource,
									CONST LPSTR	inTexture)	:
	CKAGETOKI_ADVENT_ENEMY(inDev, inName, inResource, inTexture)
	{
	//	�����̏�����
		this->fFloorSpace		= 15.0f;				//!< ���Ƃ̊Ԋu
		this->fWallSpace		= 10.0f;				//!< �ǂƂ̊Ԋu
		this->fJumpHeight		= 50.0f;				//!< �W�����v��

	//	�A�j���[�V�����̊��蓖�Ă��s��
		this->iAnimTemp[eESHADOGA_WAIT]		= 0;
		this->iAnimTemp[eESHADOGA_RUN]		= 1;
		this->iAnimTemp[eESHADOGA_JUMP]		= 2;
		this->iAnimTemp[eESHADOGA_ATTACK]	= 3;

		this->iMaxLife			= 2;					//!< �ő�̗�
		this->iLife				= this->iMaxLife;		//!< �c��̗�
		this->iHitFrame			= 0;					//!< �Փ˂��ē_�ł��鎞��

	//	�t���O�Q
		this->action			= eESHADOGA_MAX;		//!< �ŏ��͑ҋ@�ȊO���ł��ǂ�
		this->changeAction(eESHADOGA_WAIT);				//!< ������Ԃ͏o��
		this->setThisMessage(eTMSG_ENEMY);				//!< �G�\��

		this->stepCnt			= 0;
	//	���̃Z�b�g
		this->sStaticStep		= new SSTATIC(SOUND_STEP);
		this->sStaticJumpStart	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticJumpEnd	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticBark1		= new SSTATIC(SOUND_BARK1);
		this->sStaticBark2		= new SSTATIC(SOUND_BARK2);
		this->sStaticStep->setVolume(-1500);
		this->sStaticJumpStart->setVolume(-1500);
		this->sStaticJumpEnd->setVolume(-1500);
		this->sStaticBark1->setVolume(-1000);
		this->sStaticBark2->setVolume(-1000);

	//	�G�t�F�N�g�̓o�^
		C3D_EFFECT_CENTER*	effectCenter	= C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			EFFECT_SMOKE, new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));

	//	���[�J�����W�̕ύX
		this->sd_ptr->mesh->localCenter = VEC3(0.0f, 50.0f, 0.0f);
		this->sd_ptr->mesh->localRadius	= 100.0f;
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOG::�f�X�g���N�^
	//
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	CENEMY_SHADOG::~CENEMY_SHADOG(void)
	{
	//	���֘A�̉��
		SAFE_DELETE(this->sStaticStep);				//!< ����
		SAFE_DELETE(this->sStaticJumpStart);		//!< �W�����v��
		SAFE_DELETE(this->sStaticJumpEnd);			//!< ���n��
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOG::soundStep
	//
	//	�@�\		�����Ǘ�
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOG::soundStep(void)
	{
		this->stepCnt++;

	//	�X���[�N��8�t���[�����ɔ���
		if(	this->stepCnt > 8)
		{
			C3D_EFFECT_CENTER::occuEffectsInstance(EFFECT_SMOKE, this->getLoc(), 0.3f);
			this->sStaticStep->setPlayCur(0);
			this->sStaticStep->play(0, NULL);
			this->stepCnt	= 0;
		}
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOG::soundJumpStart
	//
	//	�@�\		�W�����v���Ǘ�
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOG::soundJumpStart(void)
	{
	//	�W�����v����炷
		this->sStaticJumpStart->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOG::soundJumpEnd
	//
	//	�@�\		���n���Ǘ�
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOG::soundJumpEnd(void)
	{
	//	���n����炷
		this->sStaticJumpEnd->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOG::opeWait
	//
	//	�@�\		�ҋ@������
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOG::opeWait(void)
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

	//	�󒆂ɂ��Ȃ���
	//	�t���[�������܂ŏオ�����瑖�s
		if(	!this->bFlighting)
		{
			this->iFrameCnt++;
			if(	this->iFrameCnt > 1)
			{
				this->changeAction(eESHADOGA_RUN);
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOG::opeRun
	//
	//	�@�\		���s������
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOG::opeRun(void)
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
			this->changeAction(eESHADOGA_WAIT);
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
			this->changeAction(eESHADOGA_WAIT);
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
			this->changeAction(eESHADOGA_WAIT);
		}

	//	�v���C���[���߂�������A�W�����v
	//	���a���L����
		if(	(*this->player)->intersectRadius(this, 1000.0f))
		{
			this->changeAction(eESHADOGA_JUMP);
		}
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOG::opeJunp
	//
	//	�@�\		�W�����v������
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOG::opeJunp(void)
	{
	//	�d�͌���
		this->opeEffectGravity();
	//	�����]��
		this->opeTurn();

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
			this->changeAction(eESHADOGA_WAIT);
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
			this->changeAction(eESHADOGA_WAIT);
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

	//	�t���[�������܂ŏオ������W�����v�U��
		this->iFrameCnt++;
		if(	this->iFrameCnt > 20)
		{
			this->changeAction(eESHADOGA_ATTACK);
		}

	//	�󒆂ɂ��Ȃ��Ȃ�A�ҋ@
		if(	!this->bFlighting)
		{
			this->changeAction(eESHADOGA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOG::opeAttack
	//
	//	�@�\		�_�b�V��������
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOG::opeAttack(void)
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
			this->changeAction(eESHADOGA_WAIT);
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
			this->changeAction(eESHADOGA_WAIT);
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

	//	�󒆂ɂ��Ȃ��Ȃ�A�ҋ@
		if(	!this->bFlighting)
		{
			this->changeAction(eESHADOGA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOG::changeAction
	//
	//	�@�\		��ԕω�
	//	����		inNewAction		�V���ȍs���p�^�[��
	//	�߂�l		TRUE			�V���ȍs���p�^�[���ɕω�����
	//				FALSE			�V���ȍs���p�^�[���ɕω����Ă��Ȃ�
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	BOOL
	CENEMY_SHADOG::changeAction(ENEMYSHADOG_ACTION inNewAction)
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
			case	eESHADOGA_WAIT:			//!< �ҋ@��
				this->setMoveQuantity(0.0f);
				break;
			case	eESHADOGA_RUN:			//!< ���s��
				this->setMoveQuantity(20.0f);
				break;
			case	eESHADOGA_JUMP:			//!< �W�����v��
				this->moveJumping(this->fJumpHeight);
				this->soundBark1();
				this->setMoveQuantity(30.0f);
				break;
			case	eESHADOGA_ATTACK:		//!< �W�����v�U����
				this->soundBark2();
				this->setMoveQuantity(30.0f);
				break;
			default:						//!< ��L�ȊO
				break;
		}

	//	�t���[���J�E���^��������
		this->iFrameCnt	= 0;

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOG::reactionMessage
	//
	//	�@�\		���A�N�V����
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOG::reactionMessage(REACTION_MESSAGE inMessage)
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
	//!	���\�b�h��	CENEMY_SHADOG::operation
	//
	//	�@�\		��{����
	//	����		*input			���͊Ǘ�
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CENEMY_SHADOG::operation(CINPUT* input)
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
			case	eESHADOGA_WAIT:			//!< �ҋ@��
				this->opeWait();
				break;
			case	eESHADOGA_RUN:			//!< ���s��
				this->opeRun();
				break;
			case	eESHADOGA_JUMP:			//!< �W�����v��
				this->opeJunp();
				break;
			case	eESHADOGA_ATTACK:		//!< �W�����v�U����
				this->opeAttack();
				break;
		}//	default	break;
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_SHADOG::draw
	//
	//	�@�\		�`��
	//	����		inDev			Direct3D�f�o�C�X
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	BOOL
	CENEMY_SHADOG::draw(CONST DEV inDev)
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