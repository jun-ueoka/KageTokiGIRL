//***********************************************************************************
//!	�t�@�C����		Class_AdventKagemi.cpp
//
//	�@�\			�A�h�x���`���[�Q�[�����[�h�́u�J�Q�~�v�N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2008/12/09	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_AdventKagemi.h"		//!< �A�h�x���`���[�Q�[���u�J�Q�~�v

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inViewG			�`��O���[�v(�V���h�E�{�[��������)
	//				inName			X�t�@�C���̖���
	//				inResource		���b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	CADVENT_KAGEMI::CADVENT_KAGEMI(CONST DEV	inDev,
								   CONST LPSTR	inName,
								   CONST LPSTR	inResource,
								   CONST LPSTR	inTexture)	:
	CKAGETOKI_ADVENT_CHARACTER(inDev, inName, inResource, inTexture)
	{
	//	�����̏�����
		this->fJumpHeight				= ADVENT_JUMP_HEIGHT;	//!< �W�����v�̍���
		this->fLeftRightMoveSpeed		= 0.0f;					//!< ���E�ړ��X�s�[�h
		this->fLeftRightMoveMaxSpeed	= 20.0f;				//!< ���E�ړ��ő�X�s�[�h
		this->fLeftRightMoveAccele		= 2.0f;					//!< ���E�ړ������l

		this->fFloorSpace		= 8.0f;							//!< ���Ƃ̊Ԋu
		this->fWallSpace		= 100.0f;						//!< �ǂƂ̊Ԋu

	//	�A�j���[�V�����̊��蓖�Ă��s��(�J�Q�~��p�̃N���X�Ȃ̂�)
		this->iAnimTemp[eADKAGEMIA_WAIT]						= 0;
		this->iAnimTemp[eADKAGEMIA_WALK]						= 5;
		this->iAnimTemp[eADKAGEMIA_FLIGHT]						= 2;
		this->iAnimTemp[eADKAGEMIA_SKILL_ENERGY_BALL_SHOT]		= 4;
		this->iAnimTemp[eADKAGEMIA_SKILL_ENERGY_BALL_OPERATION]	= 11;
		this->iAnimTemp[eADKAGEMIA_SKILL_ENERGY_SHOOT]			= 11;
		this->iAnimTemp[eADKAGEMIA_DAMAGE_LV1]					= 7;
		this->iAnimTemp[eADKAGEMIA_DAMAGE_LV2]					= 8;

	//	�t���O�Q
		this->limit				= eADKAGEMIL_MAX;				//!< ���䃌�x��4
		this->action			= eADKAGEMIA_MAX;				//!< �ŏ��͑ҋ@�ȊO���ł��ǂ�
		this->changeAction(eADKAGEMIA_WAIT);					//!< ������Ԃ͑ҋ@
		this->iHitFrame			= 0;							//!< �Փ˂��ē_�ł��鎞��
		this->bJunpInput		= FALSE;						//!< ���ē��͊m�F

	//	�Ə����
		this->texArrow	= new CTEXTURE(inDev, "arrowKagemi", VEC2(200.0f, 200.0f));
		this->texArrow->setMoveQuantity(300.0f);

		this->ballAngle			= VEC3(0.0f, ADVENT_RIGHTANGLE, 0.0f);
	//	�I���W�i���u�G�i�W�[�{�[���v�̍쐬�A�ݒ�
		this->originEnergyBall
			= new CKAGEMI_BALL(inDev, eKMIBALL_ENERGY);
		this->opeEnergyBall		= NULL;
		this->energyBallSpeed	= 20.0f;
		this->energyBallFrame	= 300;
		this->energyShootSpeed	= 50.0f;
		this->energyShootFrame	= 100;

	//	�L�����N�^�[�Ȃ̂ŁA�����蔻����@�ɍ�����t����
		this->setHeightValid(true);
		this->setHeightValue(this->getWorldRadius());
		this->setWidthValue(this->getWorldRadius() / 2.0f);

		this->stepCnt			= 0;
	//	���̃Z�b�g
		this->sStaticStep		= new SSTATIC(SOUND_STEP);
		this->sStaticJumpStart	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticJumpEnd	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticShotEnergy	= new SSTATIC(SOUND_SHOT_ENERGY);

	//	�G�t�F�N�g�̓o�^
		C3D_EFFECT_CENTER*	effectCenter	= C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			EFFECT_SMOKE,
			new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEMI_COMMENT_ENERGYBALL,
			new C3D_EFFECT(inDev, KAGEMI_COMMENT_ENERGYBALL, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEMI_COMMENT_ENERGYSHOOT,
			new C3D_EFFECT(inDev, KAGEMI_COMMENT_ENERGYSHOOT, VEC3(0.0f,0.0f,0.0f)));

	//	���[�J�����W�̕ύX
		this->sd_ptr->mesh->localCenter = VEC3(0.0f, 100.0f, 0.0f);
		this->sd_ptr->mesh->localRadius	= 150.0f;
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::�f�X�g���N�^
	//
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	CADVENT_KAGEMI::~CADVENT_KAGEMI(void)
	{
	//	���֘A�̉��
		SAFE_DELETE(this->sStaticStep);				//!< ����
		SAFE_DELETE(this->sStaticJumpStart);		//!< �W�����v��
		SAFE_DELETE(this->sStaticJumpEnd);			//!< ���n��
		SAFE_DELETE(this->sStaticShotEnergy);		//!< �G�i�W�[�{�[�����ˉ�

		SAFE_DELETE(this->originEnergyBall);		//!< �I���W�i���u�G�i�W�[�{�[���v�̉��
		SAFE_DELETE(this->opeEnergyBall);			//!< ���쒆�G�i�W�[�{�[��
		SAFE_DELETE(this->texArrow);				//!< �Ə����
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::soundStep
	//
	//	�@�\		�����Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::soundStep(void)
	{
		this->stepCnt++;

	//	�X���[�N��15�t���[�����ɔ���
		if(	this->stepCnt > 15)
		{
			C3D_EFFECT_CENTER::occuEffectsInstance(EFFECT_SMOKE, this->getLoc(), 0.3f);
			this->sStaticStep->setPlayCur(0);
			this->sStaticStep->play(0, NULL);
			this->stepCnt	= 0;
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::soundJumpStart
	//
	//	�@�\		�W�����v���Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::soundJumpStart(void)
	{
	//	�W�����v����炷
		this->sStaticJumpStart->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::soundJumpEnd
	//
	//	�@�\		���n���Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::soundJumpEnd(void)
	{
	//	���n����炷
		this->sStaticJumpEnd->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::soundShotEnergy
	//
	//	�@�\		�G�i�W�[�{�[�����ˉ��Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::soundShotEnergy(void)
	{
	//	�G�i�W�[�{�[�����ˉ������݂ɖ炷
		this->sStaticShotEnergy->setPlayCur(0);
		this->sStaticShotEnergy->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::opeWait
	//
	//	�@�\		�ҋ@������
	//	����		*input			���͊Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::opeWait(CINPUT* input)
	{
	//	�A�h�x���`���[�Q�[������
		this->opeAdvent(input);
	//	�d�͑���
		this->opeEffectGravity();

	//	����
		if(	input->checkPressOnce(ADVENT_JUMP))
		{
			this->moveJumping(this->fJumpHeight);
		//	�W�����v��
			this->soundJumpStart();
		//	���ē��͗L��
			this->bJunpInput	= TRUE;
		}

	//	��L�̏����ŁA�󒆂ɂ���Ȃ�A��Ԃ��u��s�v�ɕύX
		if(	this->bFlighting)
		{
			this->changeAction(eADKAGEMIA_FLIGHT);
		}
	//	�Z����
		else if(this->opeSkillCheck(input))
		{
		}
	//	�󒆂ɂ��Ȃ��ŁA�����L�[��������Ă���Ȃ�A��Ԃ��u���s�v�ɕύX
		else if(input->checkPress(ADVENT_LEFT)	||
				input->checkPress(ADVENT_RIGHT))
		{
			this->changeAction(eADKAGEMIA_WALK);
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::opeWalk
	//
	//	�@�\		���s������
	//	����		*input			���͊Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::opeWalk(CINPUT* input)
	{
	//	���쒆�̃G�i�W�[�{�[��������Ȃ�A�O���[�v�ɏ�����C����
		if(	this->opeEnergyBall)
		{
		//	�����`��ɔC����
			this->viewG->add(this->opeEnergyBall);
		//	���쒆�G�i�W�[�{�[���̃A�N�Z�X��؂�
			this->opeEnergyBall	= NULL;
		}
	//	�����̔���
		this->soundStep();

	//	�A�h�x���`���[�Q�[������
		this->opeAdvent(input);
	//	�d�͑���
		this->opeEffectGravity();

	//	�W�����v����
		if(	input->checkPressOnce(ADVENT_JUMP))
		{
			this->moveJumping(this->fJumpHeight);
		//	�W�����v��
			this->soundJumpStart();
		//	���ē��͗L��
			this->bJunpInput	= TRUE;
		}

	//	��L�̏����ŁA�󒆂ɂ���Ȃ�A��Ԃ��u��s�v�ɕύX
		if(	this->bFlighting)
		{
			this->changeAction(eADKAGEMIA_FLIGHT);
		}
	//	�Z����
		else if(this->opeSkillCheck(input))
		{
		}
	//	�󒆂ɂ��Ȃ��ŁA���E�Ɉړ����Ă��Ȃ��Ȃ�A��Ԃ��u�ҋ@�v�ɕύX
		else if(this->fLeftRightMoveSpeed == 0.0f)
		{
			this->changeAction(eADKAGEMIA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::opeFlight
	//
	//	�@�\		��s������
	//	����		*input			���͊Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::opeFlight(CINPUT* input)
	{
	//	�A�h�x���`���[�Q�[������
		this->opeAdvent(input);
	//	�d�͑���
		this->opeEffectGravity();

	//	���ē��͂�����Ă��āA���ē��͂����ꑱ���ĂȂ�������A�W�����v�͂�������
		if(	this->bJunpInput				&&
			!input->checkPress(ADVENT_JUMP)	&&
			(this->fUpDownSpeed > 0.0f))
		{
			this->fUpDownSpeed -= 5.0f;
		}

	//	�Z����
		if(	this->opeSkillCheck(input))
		{
		}
	//	��L�̏����ŁA�󒆂ɂ��Ȃ��Ȃ�A��Ԃ�ҋ@�ɕύX
		else if(!this->bFlighting)
		{
			this->changeAction(eADKAGEMIA_WAIT);
		//	���n��
			this->soundJumpEnd();
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::opeSkillEnergyBallShot
	//
	//	�@�\		�Z�u�G�i�W�[�{�[�����ˁv��
	//	����		*input			���͊Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::opeSkillEnergyBallShot(CINPUT* input)
	{
	//	�t���[����i�߂āA�G�i�W�[�{�[�����o��^�C�~���O�ɂȂ�����
		this->iFrameCnt++;
		if(	this->iFrameCnt >= MAXFRAME_ENERGY_BALL)
		{
		//	�A�j���[�V�������G�i�W�[�{�[������ɕύX���A�G�i�W�[�{�[������
			this->changeAction(eADKAGEMIA_SKILL_ENERGY_BALL_OPERATION);
		//	�t���[���J�E���^��������
			this->iFrameCnt	= 0;
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::opeSkillEnergyBallOperation
	//
	//	�@�\		�Z�u�G�i�W�[�{�[������v��
	//	����		*input			���͊Ǘ�
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::opeSkillEnergyBallOperation(CINPUT* input)
	{
	//	���쒆�̃G�i�W�[�{�[�����Ȃ���΁A�ҋ@�Ɉڂ�
		if(	!this->opeEnergyBall)
		{
			this->changeAction(eADKAGEMIA_WAIT);
			return;
		}

	//	����͂�
		
	//	���͂��o�C�g�^�Ŋm��
		BYTE	byInput	=
			((input->checkPress(ADVENT_TOP)		? 0x01 : 0x00)	|
			 (input->checkPress(ADVENT_BOTTOM)	? 0x02 : 0x00)	|
			 (input->checkPress(ADVENT_LEFT)	? 0x04 : 0x00)	|
			 (input->checkPress(ADVENT_RIGHT)	? 0x08 : 0x00));
		float	targetAngle	= this->opeEnergyBall->getAngleZ();
	//	���͊m�F
		switch(byInput)
		{
			case 0x01:	targetAngle = 90.0f;	break;	//!< ��
			case 0x02:	targetAngle = 270.0f;	break;	//!< ��
			case 0x04:	targetAngle = 180.0f;	break;	//!< ��
			case 0x08:	targetAngle = 0.0f;		break;	//!< �E
			case 0x05:	targetAngle = 135.0f;	break;	//!< ����
			case 0x09:	targetAngle = 45.0f;	break;	//!< �E��
			case 0x06:	targetAngle = 225.0f;	break;	//!< ����
			case 0x10:	targetAngle = 315.0f;	break;	//!< �E��
			case 0x07:	targetAngle = 180.0f;	break;	//!< �㉺��
			case 0x11:	targetAngle = 0.0f;		break;	//!< �㉺�E
			case 0x13:	targetAngle = 90.0f;	break;	//!< �㍶�E
			case 0x14:	targetAngle = 270.0f;	break;	//!< �����E
		}
		this->opeEnergyBall->setAngleZ(targetAngle);

		this->iFrameCnt++;
	//	�G�i�W�[�{�[�����˃{�^����������ĂȂ������A
	//	�t���[����i�߂āA�G�i�W�[�{�[��������I���^�C�~���O�ɂȂ�����
		if(	!(input->checkPress(KAGEMI_ENERGY_BALL))	||
			(this->iFrameCnt >= MAXFRAME_ENERGY_BALL_OPERATION))
		{
		//	���S�ɑҋ@��ԂɈȍ~
			this->safeResetChangeToWait();
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::opeSkillEnergyShoot
	//
	//	�@�\		�Z�u�G�i�W�[�V���[�g�v��
	//	����		*input			���͊Ǘ�
	//	�X�V		2009/01/24		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::opeSkillEnergyShoot(CINPUT* input)
	{
	//	�d�͑���
		this->opeEffectGravity();
	//	�t���[����i�߂āA�G�i�W�[�V���[�g���o��^�C�~���O�ɂȂ�����
		this->opeBallAngle(input);

		this->iFrameCnt++;
		if(	((this->iFrameCnt > 10)	&&
			!input->checkPress(KAGEMI_ENERGY_SHOOT))	||
			this->iFrameCnt >= MAXFRAME_ENERGY_SHOOT)
		{
		//	�G�i�W�[�{�[�������
			this->skillEnergyShoot();
		//	�t���[���J�E���^��������
			this->iFrameCnt	= 0;
		//	�A�j���[�V������ҋ@�ɖ߂�
			this->changeAction(eADKAGEMIA_WAIT);
		//	�G�i�W�[�V���[�g�̐����o��
			C3D_EFFECT_CENTER::occuEffectsInstance(
				KAGEMI_COMMENT_ENERGYSHOOT, this->getWorldCenter(), 1.0f);
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::opeDamageLv1
	//
	//	�@�\		�_���[�WLv1����������
	//	����		*input			���͊Ǘ�
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::opeDamageLv1(CINPUT* input)
	{
	//	�d�͑���
		this->opeEffectGravity();
	//	�t���[�������܂ōs������A�ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10)
		{
			this->changeAction(eADKAGEMIA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::opeDamageLv2
	//
	//	�@�\		�_���[�WLv2����������
	//	����		*input			���͊Ǘ�
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::opeDamageLv2(CINPUT* input)
	{
	//	�d�͑���
		this->opeEffectGravity();
	//	�t���[�������܂ōs������A�ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 20)
		{
			this->changeAction(eADKAGEMIA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::opeSkillCheck
	//
	//	�@�\		�Z���͔���
	//	����		*input			���͊Ǘ�
	//	�߂�l		TRUE			�s���p�^�[�����ς����
	//				FALSE			�s���p�^�[�����ς���Ă��Ȃ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	BOOL
	CADVENT_KAGEMI::opeSkillCheck(CINPUT* input)
	{
	//	�����ƍ��E�������Ă��Ȃ���΁A���������Ȃ�
		if(	this->way != eADCW_LEFT &&
			this->way != eADCW_RIGHT)	return FALSE;

	//	�V���ȍs���p�^�[�����m�ۂ����
		ADVENT_KAGEMI_ACTION	newAction	= this->action;

	//	�u�G�i�W�[�{�[�����ˁv����
		if(	(this->limit>= eADKAGEMIL_LV2)					&&
			(input->checkPressOnce(KAGEMI_ENERGY_BALL)))
		{
			newAction	= eADKAGEMIA_SKILL_ENERGY_BALL_SHOT;
			if		(this->way == eADCW_RIGHT)	this->ballAngle.z	= 360.0f;
			else if	(this->way == eADCW_LEFT)	this->ballAngle.z	= 180.0f;
		}

	//	�u�G�i�W�[�V���[�g�v����
		else
		if(	(this->limit>= eADKAGEMIL_LV3)					&&
			(input->checkPressOnce(KAGEMI_ENERGY_SHOOT)))
		{
			newAction	= eADKAGEMIA_SKILL_ENERGY_SHOOT;
			if		(this->way == eADCW_RIGHT)	this->ballAngle.z	= 360.0f;
			else if	(this->way == eADCW_LEFT)	this->ballAngle.z	= 180.0f;
		}

	//	�V���ȍs���p�^�[���ɕω��������́A���ʂ�Ԃ�
		return	this->changeAction(newAction);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::opeBallAngle
	//
	//	�@�\		�{�[���Z�̕�������
	//	����		*input			���͊Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::opeBallAngle(CINPUT* input)
	{
	//	�E�����̏ꍇ
		if		(this->way == eADCW_RIGHT)
		{
			if(	input->checkPress(ADVENT_TOP))
			{
				if(	(this->ballAngle.z + 5.0f) < 450.0f)	this->ballAngle.z += 5.0f;
				else										this->ballAngle.z  = 450.0f;
			}
			else if(input->checkPress(ADVENT_BOTTOM))
			{
				if(	(this->ballAngle.z - 5.0f) > 270.0f)	this->ballAngle.z -= 5.0f;
				else										this->ballAngle.z  = 270.0f;
			}
		}
	//	�������̏ꍇ
		else if	(this->way == eADCW_LEFT)
		{
			if(	input->checkPress(ADVENT_TOP))
			{
				if(	(this->ballAngle.z - 5.0f) >  90.0f)	this->ballAngle.z -= 5.0f;
				else										this->ballAngle.z  = 90.0f;
			}
			else if(input->checkPress(ADVENT_BOTTOM))
			{
				if(	(this->ballAngle.z + 5.0f) < 270.0f)	this->ballAngle.z += 5.0f;
				else										this->ballAngle.z  = 270.0f;
			}
		}
		this->ballAngle.y	= ADVENT_RIGHTANGLE;
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::skillEnergyBall
	//
	//	�@�\		�Z�u�G�i�W�[�{�[���v
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::skillEnergyBall(void)
	{
	//	�G�i�W�[�p���[�ɁA�G�i�W�[�{�[�����������̗]�T�����鎞

		//	�G�i�W�[�{�[���𐶐�
			CKAGEMI_BALL*	newBall	= new CKAGEMI_BALL(this->originEnergyBall);
		//	�G�i�W�[�{�[���̐ݒ�
			newBall->shot(
				&this->ballAngle,								//!< �p�x
				&this->getWorldCenter(),						//!< ���ˈʒu
				this->stageG,									//!< �X�e�[�W�I�u�W�F�N�g�O���[�v
				this->enemyG,									//!< �G�I�u�W�F�N�g�O���[�v
				this->energyBallFrame);							//!< ���Ŏ���
			newBall->setMoveQuantity(this->energyBallSpeed);	//!< �X�s�[�h�ݒ�
			newBall->setScale(3.0f);							//!< �g��
			newBall->setLine(ADVENT_FRONTHIT);					//!< ����(3D��Ԃɑ΂���)
			newBall->setOrderMessage(eOMSG_DELETE_IN);			//!< �����
		//	���G�t�F�N�g����
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE,
				newBall->getLoc(),
				0.5f,
				VEC3(0.0f,0.0f,0.0f),
				D3DCOLOR_RGBA(255, 200, 200, 255));
		//	�G�i�W�[�{�[�����ˉ�
			this->soundShotEnergy();
		//	�G�i�W�[�{�[������Ώۂɓo�^
			this->opeEnergyBall	= newBall;
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::skillEnergyShoot
	//
	//	�@�\		�Z�u�G�i�W�[�V���[�g�v
	//	�X�V		2009/01/24		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::skillEnergyShoot(void)
	{
	//	�G�i�W�[�p���[�ɁA�G�i�W�[�V���[�g���������̗]�T�����鎞

		//	�G�i�W�[�V���[�g�𐶐�
			CKAGEMI_BALL*	newBall	= new CKAGEMI_BALL(this->originEnergyBall);
		//	�G�i�W�[�V���[�g�̐ݒ�
			newBall->shot(
				&this->ballAngle,								//!< �p�x
				&this->getWorldCenter(),						//!< ���ˈʒu
				this->stageG,									//!< �X�e�[�W�I�u�W�F�N�g�O���[�v
				this->enemyG,									//!< �G�I�u�W�F�N�g�O���[�v
				this->energyShootFrame);						//!< ���Ŏ���
			newBall->setMoveQuantity(this->energyShootSpeed);	//!< �X�s�[�h�ݒ�
			newBall->setScale(3.0f);							//!< �g��
			newBall->setLine(ADVENT_FRONTHIT);					//!< ����(3D��Ԃɑ΂���)
			newBall->setOrderMessage(eOMSG_DELETE_IN);			//!< �����
		//	���G�t�F�N�g����
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE,
				newBall->getLoc(),
				0.5f,
				VEC3(0.0f,0.0f,0.0f),
				D3DCOLOR_RGBA(255, 200, 200, 255));
		//	�G�i�W�[�{�[�����ˉ�
			this->soundShotEnergy();
		//	�G�i�W�[�{�[������Ώۂɓo�^
			this->viewG->add(newBall);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::changeAction
	//
	//	�@�\		��ԕω�
	//	����		inNewAction		�V���ȍs���p�^�[��
	//	�߂�l		TRUE			�V���ȍs���p�^�[���ɕω�����
	//				FALSE			�V���ȍs���p�^�[���ɕω����Ă��Ȃ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	BOOL
	CADVENT_KAGEMI::changeAction(ADVENT_KAGEMI_ACTION inNewAction)
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
		//	�ҋ@��
			case	eADKAGEMIA_WAIT:
				this->bJunpInput	= FALSE;
				this->setLocZ(0.0f);
				break;
		//	���s��
			case	eADKAGEMIA_WALK:
				break;
		//	��s��
			case	eADKAGEMIA_FLIGHT:
				break;
		//	�Z�u�G�i�W�[�{�[�����ˁv
			case	eADKAGEMIA_SKILL_ENERGY_BALL_SHOT:
			//	�G�i�W�[�{�[���̐����o��
				C3D_EFFECT_CENTER::occuEffectsInstance(
					KAGEMI_COMMENT_ENERGYBALL, this->getWorldCenter(), 1.0f);
				this->bJunpInput	= FALSE;
				break;
		//	�Z�u�G�i�W�[�{�[������v
			case	eADKAGEMIA_SKILL_ENERGY_BALL_OPERATION:
			//	�G�i�W�[�{�[������
				this->skillEnergyBall();
				this->bJunpInput	= FALSE;
				break;
		//	�Z�u�G�i�W�[�V���[�g�v
			case	eADKAGEMIA_SKILL_ENERGY_SHOOT:
				this->bJunpInput	= FALSE;
				break;
		//	�_���[�WLv1������
			case	eADKAGEMIA_DAMAGE_LV1:
			//	���A�N�V�������̏㏸�l��ݒ�
				this->fUpDownSpeed	= 20;
				this->bJunpInput	= FALSE;
				break;
		//	�_���[�WLv2������
			case	eADKAGEMIA_DAMAGE_LV2:
			//	���A�N�V�������̏㏸�l��ݒ�
				this->fUpDownSpeed	= 40;
				this->bJunpInput	= FALSE;
				break;
		//	��L�ȊO
			default:
				break;
		}
	//	�t���[���J�E���^��������
		this->iFrameCnt	= 0;

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::safeResetChangeToWait
	//
	//	�@�\		���S�ɑҋ@�Ɉڍs����
	//	�X�V		2009/02/18		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::safeResetChangeToWait(void)
	{
	//	���쒆�̃G�i�W�[�{�[��������ꍇ
		if(	this->opeEnergyBall)
		{
		//	�����`��ɔC����
			this->viewG->add(this->opeEnergyBall);
		//	���쒆�G�i�W�[�{�[���̃A�N�Z�X��؂�
			this->opeEnergyBall	= NULL;
		}
	//	�A�j���[�V������ҋ@�ɖ߂�
		this->changeAction(eADKAGEMIA_WAIT);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::reactionMessage
	//
	//	�@�\		���A�N�V����
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::reactionMessage(REACTION_MESSAGE inMessage)
	{
	//	���A�N�V��������
		switch(inMessage)
		{
		//	�G�̍U��Lv1
			case eREAMSG_ENEMY_ATTACK_LV1:
			//	�Փ˃t���[����0�ȉ��Ȃ�A�_���[�W
				if(	this->iHitFrame <= 0)
				{
				//	�U���ȊO�̎��Ȃ�A�ジ����
					this->iHitFrame = 100;
					this->bDamage	= TRUE;
					this->iDamage	= 2;
					this->safeResetChangeToWait();
					this->changeAction(eADKAGEMIA_DAMAGE_LV1);
				}
				break;
		//	�G�̍U��Lv2
			case eREAMSG_ENEMY_ATTACK_LV2:
			//	�Փ˃t���[����0�ȉ��Ȃ�A�_���[�W
				if(	this->iHitFrame <= 0)
				{
				//	�U���ȊO�̎��Ȃ�A�ジ����
					this->iHitFrame = 100;
					this->bDamage	= TRUE;
					this->iDamage	= 3;
					this->safeResetChangeToWait();
					this->changeAction(eADKAGEMIA_DAMAGE_LV1);
				}
				break;
		//	�G�̍U��Lv3
			case eREAMSG_ENEMY_ATTACK_LV3:
			//	�Փ˃t���[����0�ȉ��Ȃ�A�_���[�W
				if(	this->iHitFrame <= 0)
				{
				//	�U���ȊO�̎��Ȃ�A�ジ����
					this->iHitFrame = 100;
					this->bDamage	= TRUE;
					this->iDamage	= 4;
					this->safeResetChangeToWait();
					this->changeAction(eADKAGEMIA_DAMAGE_LV2);
				}
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::operation
	//
	//	�@�\		��{����
	//	����		*input			���͊Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEMI::operation(CINPUT* input)
	{
	//	�\������Ă��Ȃ���΁A�������Ȃ�
		if(	!this->getVisible())	return;

	////	�O���[�v�̓o�^���Ȃ���΁A�������Ȃ�
	//	if(	!this->viewG	||
	//		!this->stageG	||
	//		!this->enemyG)	return;

#ifdef	_DEBUG
		if(	input->checkPress(MYKEY::eF2))
		{
			if(	input->checkPress(MYKEY::eLEFT))	this->calcLocX(-50.0f);
			if(	input->checkPress(MYKEY::eRIGHT))	this->calcLocX( 50.0f);
			if(	input->checkPress(MYKEY::eDOWN))	this->calcLocY(-50.0f);
			if(	input->checkPress(MYKEY::eUP))		this->calcLocY( 50.0f);
			return;
		}
#endif

	//	��Ԃɂ�鑀��̕���
		switch(this->action)
		{
		//	�ҋ@��
			case	eADKAGEMIA_WAIT:
				this->opeWait(input);
				break;
		//	���s��
			case	eADKAGEMIA_WALK:
				this->opeWalk(input);
				break;
		//	��s��
			case	eADKAGEMIA_FLIGHT:
				this->opeFlight(input);
				break;
		//	�Z�u�G�i�W�[�{�[�����ˁv
			case	eADKAGEMIA_SKILL_ENERGY_BALL_SHOT:
				this->opeSkillEnergyBallShot(input);
				break;
		//	�Z�u�G�i�W�[�{�[������v
			case	eADKAGEMIA_SKILL_ENERGY_BALL_OPERATION:
				this->opeSkillEnergyBallOperation(input);
				break;
		//	�Z�u�G�i�W�[�V���[�g�v
			case	eADKAGEMIA_SKILL_ENERGY_SHOOT:
				this->opeSkillEnergyShoot(input);
				break;
		//	�_���[�WLv1������
			case	eADKAGEMIA_DAMAGE_LV1:
				this->opeDamageLv1(input);
				break;
		//	�_���[�WLv2������
			case	eADKAGEMIA_DAMAGE_LV2:
				this->opeDamageLv2(input);
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEMI::draw
	//
	//	�@�\		�`��
	//	�@�\�T�v	�X�L�����b�V���̕`��ƁA���쒆�G�i�W�[�{�[���̕`����s��
	//	����		inDev			Direct3D�f�o�C�X
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	BOOL
	CADVENT_KAGEMI::draw(CONST DEV inDev)
	{
	//	���쒆�G�i�W�[�{�[��������Ȃ�A�`�悷��
		if(	this->opeEnergyBall)
		{
			this->opeEnergyBall->draw(inDev);
			if(	!(this->opeEnergyBall->getVisible()))
			{
			//	�����`��ɔC����
				this->viewG->add(this->opeEnergyBall);
			//	���쒆�G�i�W�[�{�[���̃A�N�Z�X��؂�
				this->opeEnergyBall	= NULL;
			}
		}

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

	//	����Lv2�ُ�Ȃ�A�Ə�����`��
		if(	this->action == eADKAGEMIA_SKILL_ENERGY_SHOOT)
		{
			this->texArrow->setLoc(&this->getWorldCenter());
			this->texArrow->setAngle(&this->ballAngle);
			this->texArrow->moveFront();
			this->texArrow->setAngleY(0.0f);
			this->texArrow->draw(inDev);
		}

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------