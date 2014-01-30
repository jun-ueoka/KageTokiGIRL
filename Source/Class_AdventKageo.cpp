//***********************************************************************************
//!	�t�@�C����		Class_AdventKageo.cpp
//
//	�@�\			�A�h�x���`���[�Q�[�����[�h�́u�J�Q�I�v�N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2008/12/09	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_AdventKageo.h"		//!< �A�h�x���`���[�Q�[���u�J�Q�I�v

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inViewG			�`��O���[�v(�V���h�E�{�[��������)
	//				inName			X�t�@�C���̖���
	//				inResource		���b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	CADVENT_KAGEO::CADVENT_KAGEO(CONST DEV		inDev,
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

	//	�A�j���[�V�����̊��蓖�Ă��s��(�J�Q�I��p�̃N���X�Ȃ̂ŁA�A�A)
		this->iAnimTemp[eADKAGEOA_WAIT]					= 0;
		this->iAnimTemp[eADKAGEOA_WALK]					= 2;
		this->iAnimTemp[eADKAGEOA_FLIGHT]				= 3;
		this->iAnimTemp[eADKAGEOA_SKILL_SHADOW_BALL]	= 9;
		this->iAnimTemp[eADKAGEOA_SKILL_SHINE_BALL]		= 9;
		this->iAnimTemp[eADKAGEOA_DAMAGE_LV1]			= 7;
		this->iAnimTemp[eADKAGEOA_DAMAGE_LV2]			= 8;

		this->limit				= eADKAGEOL_MAX;				//!< ���䃌�x��3
		this->action			= eADKAGEOA_MAX;				//!< �ŏ��͑ҋ@�ȊO���ł��ǂ�
		this->changeAction(eADKAGEOA_WAIT);						//!< ������Ԃ͑ҋ@
		this->iHitFrame			= 0;							//!< �Փ˓_�Ńt���[��
		this->bJunpInput		= FALSE;						//!< ���ē��͊m�F

	//	�Ə����
		this->texArrow	= new CTEXTURE(inDev, "arrowKageo", VEC2(200.0f, 200.0f));
		this->texArrow->setMoveQuantity(300.0f);

		this->ballAngle			= VEC3(0.0f, ADVENT_RIGHTANGLE, 0.0f);
	//	�I���W�i���u�V���h�E�{�[���v�̍쐬�A�ݒ�
		this->originShadowBall
			= new CKAGEO_BALL(inDev, eKOBALL_SHADOW);
		this->shadowBallSpeed	= 50.0f;
		this->shadowBallFrame	= 100;

	//	�I���W�i���u�V���C���{�[���v�̍쐬�A�ݒ�
		this->originShineBall
			= new CKAGEO_BALL(inDev, eKOBALL_SHINE);
		this->shineBallSpeed	= 50.0f;
		this->shineBallFrame	= 100;

	//	�L�����N�^�[�Ȃ̂ŁA�����蔻����@�ɍ�����t����
		this->setHeightValid(true);
		this->setHeightValue(this->getWorldRadius());
		this->setWidthValue(this->getWorldRadius() / 2.0f);

		this->stepCnt			= 0;
	//	���̃Z�b�g
		this->sStaticStep		= new SSTATIC(SOUND_STEP);
		this->sStaticJumpStart	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticJumpEnd	= new SSTATIC(SOUND_JUMP_START);
		this->sStaticShotShadow	= new SSTATIC(SOUND_SHOT_SHADOW);
		this->sStaticShotShine	= new SSTATIC(SOUND_SHOT_SHINE);

	//	�G�t�F�N�g�̓o�^
		C3D_EFFECT_CENTER*	effectCenter	= C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			EFFECT_SMOKE,
			new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEO_COMMENT_SHADOWBALL,
			new C3D_EFFECT(inDev, KAGEO_COMMENT_SHADOWBALL, VEC3(0.0f, 0.0f, 0.0f)));
		effectCenter->addEffects(
			KAGEO_COMMENT_SHINEBALL,
			new C3D_EFFECT(inDev, KAGEO_COMMENT_SHINEBALL, VEC3(0.0f, 0.0f, 0.0f)));

	//	���[�J�����W�̕ύX
		this->sd_ptr->mesh->localCenter = VEC3(0.0f, 100.0f, 0.0f);
		this->sd_ptr->mesh->localRadius	= 150.0f;
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::�f�X�g���N�^
	//
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	CADVENT_KAGEO::~CADVENT_KAGEO(void)
	{
	//	���֘A�̉��
		SAFE_DELETE(this->sStaticStep);				//!< ����
		SAFE_DELETE(this->sStaticJumpStart);		//!< �W�����v��
		SAFE_DELETE(this->sStaticJumpEnd);			//!< ���n��
		SAFE_DELETE(this->sStaticShotShadow);		//!< �V���h�E�{�[�����ˉ�
		SAFE_DELETE(this->sStaticShotShine);		//!< �V���C���{�[�����ˉ�

		SAFE_DELETE(this->originShadowBall);		//!< �I���W�i���u�V���h�E�{�[���v�̉��
		SAFE_DELETE(this->originShineBall);			//!< �I���W�i���u�V���C���{�[���v�̉��
		SAFE_DELETE(this->texArrow);				//!< �Ə����
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::soundStep
	//
	//	�@�\		�����Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::soundStep(void)
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
	//!	���\�b�h��	CADVENT_KAGEO::soundJumpStart
	//
	//	�@�\		�W�����v���Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::soundJumpStart(void)
	{
	//	�W�����v����炷
		this->sStaticJumpStart->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::soundJumpEnd
	//
	//	�@�\		���n���Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::soundJumpEnd(void)
	{
	//	���n����炷
		this->sStaticJumpEnd->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::soundShotShadow
	//
	//	�@�\		�V���h�E�{�[�����ˉ��Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::soundShotShadow(void)
	{
	//	�V���h�E�{�[�����ˉ������݂ɖ炷
		this->sStaticShotShadow->setPlayCur(0);
		this->sStaticShotShadow->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::soundShotShine
	//
	//	�@�\		�V���C���{�[�����ˉ��Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::soundShotShine(void)
	{
	//	�V���C���{�[�����ˉ������݂ɖ炷
		this->sStaticShotShine->setPlayCur(0);
		this->sStaticShotShine->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::opeWait
	//
	//	�@�\		�ҋ@������
	//	����		*input			���͊Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::opeWait(CINPUT* input)
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
			this->changeAction(eADKAGEOA_FLIGHT);
		}
	//	�Z����
		else if(this->opeSkillCheck(input))
		{
		}
	//	�󒆂ɂ��Ȃ��ŁA�����L�[��������Ă���Ȃ�A��Ԃ��u���s�v�ɕύX
		else if(input->checkPress(ADVENT_LEFT)	||
				input->checkPress(ADVENT_RIGHT))
		{
			this->changeAction(eADKAGEOA_WALK);
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::opeWalk
	//
	//	�@�\		���s������
	//	����		*input			���͊Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::opeWalk(CINPUT* input)
	{
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
			this->changeAction(eADKAGEOA_FLIGHT);
		}
	//	�Z����
		else if(this->opeSkillCheck(input))
		{
		}
	//	�󒆂ɂ��Ȃ��ŁA���E�Ɉړ����Ă��Ȃ��Ȃ�A��Ԃ��u�ҋ@�v�ɕύX
		else if(this->fLeftRightMoveSpeed == 0.0f)
		{
			this->changeAction(eADKAGEOA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::opeFlight
	//
	//	�@�\		��s������
	//	����		*input			���͊Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::opeFlight(CINPUT* input)
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
			this->changeAction(eADKAGEOA_WAIT);
		//	���n��
			this->soundJumpEnd();
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::opeSkillShadowBall
	//
	//	�@�\		�Z�u�V���h�E�{�[���v��
	//	����		*input			���͊Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::opeSkillShadowBall(CINPUT* input)
	{
	//	�d�͑���
		this->opeEffectGravity();

	//	�{�[�����ˊp�x������
		this->opeBallAngle(input);

	//	�t���[����i�߂āA�V���h�E�{�[�����o��^�C�~���O�ɂȂ�����A����
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10	&&
			!input->checkPress(KAGEO_SHADOW_BALL))
		{
		//	�V���h�E�{�[������
			this->skillShadowBall();
		//	�A�j���[�V�������C�ɖ߂�
			this->changeAction(eADKAGEOA_WAIT);
		//	�t���[���J�E���^��������
			this->iFrameCnt	= 0;
		//	�V���h�E�{�[���̐����o��
			C3D_EFFECT_CENTER::occuEffectsInstance(
				KAGEO_COMMENT_SHADOWBALL, this->getWorldCenter(), 1.0f);
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::opeSkillShineBall
	//
	//	�@�\		�Z�u�V���h�E�{�[���v��
	//	����		*input			���͊Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::opeSkillShineBall(CINPUT* input)
	{
	//	�d�͑���
		this->opeEffectGravity();

	//	�{�[�����ˊp�x������
		this->opeBallAngle(input);

	//	�t���[����i�߂āA�V���h�E�{�[�����o��^�C�~���O�ɂȂ�����A����
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10	&&
			!input->checkPress(KAGEO_SHINE_BALL))
		{
		//	�V���h�E�{�[������
			this->skillShineBall();
		//	�A�j���[�V�������C�ɖ߂�
			this->changeAction(eADKAGEOA_WAIT);
		//	�t���[���J�E���^��������
			this->iFrameCnt	= 0;
		//	�V���C���{�[���̐����o��
			C3D_EFFECT_CENTER::occuEffectsInstance(
				KAGEO_COMMENT_SHINEBALL, this->getWorldCenter(), 1.0f);
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::opeDamageLv1
	//
	//	�@�\		�_���[�WLv1����������
	//	����		*input			���͊Ǘ�
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::opeDamageLv1(CINPUT* input)
	{
	//	�d�͑���
		this->opeEffectGravity();
	//	�t���[�������܂ōs������A�ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10)
		{
			this->changeAction(eADKAGEOA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::opeDamageLv2
	//
	//	�@�\		�_���[�WLv2����������
	//	����		*input			���͊Ǘ�
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::opeDamageLv2(CINPUT* input)
	{
	//	�d�͑���
		this->opeEffectGravity();
	//	�t���[�������܂ōs������A�ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 20)
		{
			this->changeAction(eADKAGEOA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::opeSkillCheck
	//
	//	�@�\		�Z���͔���
	//	����		*input			���͊Ǘ�
	//	�߂�l		TRUE			�s���p�^�[�����ς����
	//				FALSE			�s���p�^�[�����ς���Ă��Ȃ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	BOOL
	CADVENT_KAGEO::opeSkillCheck(CINPUT* input)
	{
	//	�����ƍ��E�������Ă��Ȃ���΁A���������Ȃ�
		if(	this->way != eADCW_LEFT &&
			this->way != eADCW_RIGHT)	return FALSE;

	//	�V���ȍs���p�^�[�����m�ۂ����
		ADVENT_KAGEO_ACTION	newAction	= this->action;

	//	�u�V���h�E�{�[���v����
		if(	(this->limit>= eADKAGEOL_LV2)					&&
			(input->checkPressOnce(KAGEO_SHADOW_BALL)))
		{
			newAction	= eADKAGEOA_SKILL_SHADOW_BALL;
			if		(this->way == eADCW_RIGHT)	this->ballAngle.z	= 360.0f;
			else if	(this->way == eADCW_LEFT)	this->ballAngle.z	= 180.0f;
		}

	//	�u�V���C���{�[���v����
		else
		if(	(this->limit>= eADKAGEOL_LV3)					&&
			(input->checkPressOnce(KAGEO_SHINE_BALL)))
		{
			newAction	= eADKAGEOA_SKILL_SHINE_BALL;
			if		(this->way == eADCW_RIGHT)	this->ballAngle.z	= 360.0f;
			else if	(this->way == eADCW_LEFT)	this->ballAngle.z	= 180.0f;
		}

	//	�V���ȍs���p�^�[���ɕω��������́A���ʂ�Ԃ�
		return	this->changeAction(newAction);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::opeBallAngle
	//
	//	�@�\		�{�[���Z�̕�������
	//	����		*input			���͊Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::opeBallAngle(CINPUT* input)
	{
	//	���䃌�x�����������背�x���ɓ��B���ĂȂ��Ȃ�A������Ԃ�
		if(	this->limit >= eADKAGEOL_LV4)	

	//	�E�����̏ꍇ
		if		(this->way == eADCW_RIGHT)
		{
			if(	input->checkPress(ADVENT_TOP))
			{
				if(	(this->ballAngle.z + 3.0f) < 450.0f)	this->ballAngle.z += 3.0f;
				else										this->ballAngle.z  = 450.0f;
			}
			else if(input->checkPress(ADVENT_BOTTOM))
			{
				if(	(this->ballAngle.z - 3.0f) > 270.0f)	this->ballAngle.z -= 3.0f;
				else										this->ballAngle.z  = 270.0f;
			}
		}
	//	�������̏ꍇ
		else if	(this->way == eADCW_LEFT)
		{
			if(	input->checkPress(ADVENT_TOP))
			{
				if(	(this->ballAngle.z - 3.0f) >  90.0f)	this->ballAngle.z -= 3.0f;
				else										this->ballAngle.z  = 90.0f;
			}
			else if(input->checkPress(ADVENT_BOTTOM))
			{
				if(	(this->ballAngle.z + 3.0f) < 270.0f)	this->ballAngle.z += 3.0f;
				else										this->ballAngle.z  = 270.0f;
			}
		}
		this->ballAngle.y	= ADVENT_RIGHTANGLE;
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::skillShadowBall
	//
	//	�@�\		�Z�u�V���h�E�{�[���v
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::skillShadowBall(void)
	{
	//	�V���h�[�p���[�ɁA�V���h�E�{�[�����������̗]�T�����鎞

		//	�V���h�E�{�[���𐶐�
			CKAGEO_BALL*	newBall	= new CKAGEO_BALL(this->originShadowBall);
		//	�V���h�E�{�[���̐ݒ�
			newBall->shot(
				&this->ballAngle,								//!< �p�x
				&this->getWorldCenter(),						//!< ���ˈʒu
				this->stageG,									//!< �e�I�u�W�F�N�g�O���[�v
				this->shadowBallFrame);							//!< ���Ŏ���
			newBall->setMoveQuantity(this->shadowBallSpeed);	//!< �X�s�[�h�ݒ�
			newBall->setScale(3.0f);							//!< �g��
			newBall->setLine(ADVENT_FRONTHIT);					//!< ����(3D��Ԃɑ΂���)
			newBall->setOrderMessage(eOMSG_DELETE_IN);				//!< �����
		//	���G�t�F�N�g����
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE,
				newBall->getLoc(),
				0.5f,
				VEC3(0.0f,0.0f,0.0f),
				D3DCOLOR_RGBA(10, 10, 10, 255));
		//	�V���h�E�{�[�����ˉ�
			this->soundShotShadow();
		//	�����o�[�o�^
			this->viewG->add(newBall);

	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::skillShineBall
	//
	//	�@�\		�Z�u�V���C���{�[���v
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::skillShineBall(void)
	{
	//	�V���h�[�p���[�ɁA�V���h�E�{�[�����������̗]�T�����鎞

		//	�V���h�E�{�[���𐶐�
			CKAGEO_BALL*	newBall	= new CKAGEO_BALL(this->originShineBall);
		//	�V���h�E�{�[���̐ݒ�
			newBall->shot(
				&this->ballAngle,								//!< �p�x
				&this->getWorldCenter(),						//!< ���ˈʒu
				this->stageG,									//!< �e�I�u�W�F�N�g�O���[�v
				this->shineBallFrame);							//!< ���Ŏ���
			newBall->setMoveQuantity(this->shineBallSpeed);		//!< �X�s�[�h�ݒ�
			newBall->setScale(3.0f);							//!< �g��
			newBall->setLine(ADVENT_FRONTHIT);					//!< �����͉E(3D��Ԃɑ΂���)
			newBall->setOrderMessage(eOMSG_DELETE_IN);				//!< �����
		//	���G�t�F�N�g����
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE,
				newBall->getLoc(),
				0.5f,
				VEC3(0.0f,0.0f,0.0f),
				D3DCOLOR_RGBA(255, 255, 200, 255));
		//	�V���C���{�[�����ˉ�
			this->soundShotShine();
		//	�����o�[�o�^
			this->viewG->add(newBall);

	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::changeAction
	//
	//	�@�\		��ԕω�
	//	����		inNewAction		�V���ȍs���p�^�[��
	//	�߂�l		TRUE			�V���ȍs���p�^�[���ɕω�����
	//				FALSE			�V���ȍs���p�^�[���ɕω����Ă��Ȃ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	BOOL
	CADVENT_KAGEO::changeAction(ADVENT_KAGEO_ACTION inNewAction)
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
			case	eADKAGEOA_WAIT:
				this->bJunpInput	= FALSE;
				this->setLocZ(0.0f);
				break;
		//	���s��
			case	eADKAGEOA_WALK:
				break;
		//	��s��
			case	eADKAGEOA_FLIGHT:
				break;
		//	�Z�u�V���h�E�{�[���v
			case	eADKAGEOA_SKILL_SHADOW_BALL:
				this->bJunpInput	= FALSE;
				break;
		//	�Z�u�V���C���{�[���v
			case	eADKAGEOA_SKILL_SHINE_BALL:
				this->bJunpInput	= FALSE;
				break;
		//	�_���[�WLv1������
			case	eADKAGEOA_DAMAGE_LV1:
			//	���A�N�V�������̏㏸�l��ݒ�
				this->fUpDownSpeed	= 20;
				this->bJunpInput	= FALSE;
				break;
		//	�_���[�WLv2������
			case	eADKAGEOA_DAMAGE_LV2:
			//	���A�N�V�������̏㏸�l��ݒ�
				this->fUpDownSpeed	= 40;
				this->bJunpInput	= FALSE;
				break;
			default:								//!< ��L�ȊO
				break;
		}
	//	�t���[���J�E���^��������
		this->iFrameCnt	= 0;

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::reactionMessage
	//
	//	�@�\		���A�N�V����
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::reactionMessage(REACTION_MESSAGE inMessage)
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
					this->changeAction(eADKAGEOA_DAMAGE_LV1);
					this->bDamage	= TRUE;
					this->iDamage	= 1;
				}
				break;
		//	�G�̍U��Lv2
			case eREAMSG_ENEMY_ATTACK_LV2:
			//	�Փ˃t���[����0�ȉ��Ȃ�A�_���[�W
				if(	this->iHitFrame <= 0)
				{
				//	�U���ȊO�̎��Ȃ�A�ジ����
					this->iHitFrame = 100;
					this->changeAction(eADKAGEOA_DAMAGE_LV1);
					this->bDamage	= TRUE;
					this->iDamage	= 2;
				}
				break;
		//	�G�̍U��Lv3
			case eREAMSG_ENEMY_ATTACK_LV3:
			//	�Փ˃t���[����0�ȉ��Ȃ�A�_���[�W
				if(	this->iHitFrame <= 0)
				{
				//	�U���ȊO�̎��Ȃ�A�ジ����
					this->iHitFrame = 100;
					this->changeAction(eADKAGEOA_DAMAGE_LV2);
					this->bDamage	= TRUE;
					this->iDamage	= 3;
				}
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::operation
	//
	//	�@�\		��{����
	//	����		*input			���͊Ǘ�
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CADVENT_KAGEO::operation(CINPUT* input)
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
#else	
#ifdef	PRESENTATION
		if(	input->checkPress(MYKEY::eF2))
		{
			if(	input->checkPress(MYKEY::eLEFT))	this->calcLocX(-50.0f);
			if(	input->checkPress(MYKEY::eRIGHT))	this->calcLocX( 50.0f);
			if(	input->checkPress(MYKEY::eDOWN))	this->calcLocY(-50.0f);
			if(	input->checkPress(MYKEY::eUP))		this->calcLocY( 50.0f);
			return;
		}
#endif
#endif

	//	��Ԃɂ�鑀��̕���
		switch(this->action)
		{
		//	�ҋ@��
			case	eADKAGEOA_WAIT:
				this->opeWait(input);
				break;
		//	���s��
			case	eADKAGEOA_WALK:
				this->opeWalk(input);
				break;
		//	��s��
			case	eADKAGEOA_FLIGHT:
				this->opeFlight(input);
				break;
		//	�Z�u�V���h�E�{�[���v
			case	eADKAGEOA_SKILL_SHADOW_BALL:
				this->opeSkillShadowBall(input);
				break;
		//	�Z�u�V���C���{�[���v
			case	eADKAGEOA_SKILL_SHINE_BALL:
				this->opeSkillShineBall(input);
				break;
		//	�_���[�WLv1������
			case	eADKAGEOA_DAMAGE_LV1:
				this->opeDamageLv1(input);
				break;
		//	�_���[�WLv2������
			case	eADKAGEOA_DAMAGE_LV2:
				this->opeDamageLv2(input);
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_KAGEO::draw
	//
	//	�@�\		�`��
	//	����		*inDev			Direct3D�f�o�C�X
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	BOOL
	CADVENT_KAGEO::draw(CONST DEV inDev)
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

	//	�V���h�[�{�[�����ˑO���V���C���{�[�����ˑO�Ȃ�A�Ə�����`��
		if(	this->action == eADKAGEOA_SKILL_SHADOW_BALL ||
			this->action == eADKAGEOA_SKILL_SHINE_BALL)
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