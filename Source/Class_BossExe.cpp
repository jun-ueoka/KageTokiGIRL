//***********************************************************************************
//!	�t�@�C����		Class_BossExe.cpp
//
//	�@�\			�A�h�x���`���[�Q�[�����[�h�́u�J�Q�I�v�N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2009/01/13	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_BossExe.h"		//!< �A�h�x���`���[�Q�[���u�G�N�X�v

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		���b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	CBOSS_EXE::CBOSS_EXE(CONST DEV		inDev,
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

	//	�A�j���[�V�����̊��蓖�Ă��s��(�G�N�X��p�̃N���X�Ȃ̂ŁA�A�A)
		this->iAnimTemp[eBEXEA_APPEAR]			= 2;
		this->iAnimTemp[eBEXEA_APPEAR_WAIT]		= 0;
		this->iAnimTemp[eBEXEA_WAIT]			= 0;
		this->iAnimTemp[eBEXEA_DAMAGE]			= 6;
		this->iAnimTemp[eBEXEA_DRILLCLAW_JUMP]	= 2;
		this->iAnimTemp[eBEXEA_DRILLCLAW]		= 7;
		this->iAnimTemp[eBEXEA_DASHCLAW_DASH]	= 1;
		this->iAnimTemp[eBEXEA_DASHCLAW]		= 5;
		this->iAnimTemp[eBEXEA_DISAPPEAR_WAIT]	= 0;
		this->iAnimTemp[eBEXEA_DISAPPEAR]		= 0;

		this->iMaxLife					= EXE_MAXLIFE;			//!< �ő�̗�
		this->iLife						= this->iMaxLife;		//!< �c��̗�
		this->iHitFrame					= 0;					//!< �Փ˂��ē_�ł��鎞��

	//	�t���O�Q
		this->action			= eBEXEA_MAX;					//!< �ŏ��͑ҋ@�ȊO���ł��ǂ�
		this->changeAction(eBEXEA_APPEAR);						//!< ������Ԃ͏o��

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
	//!	���\�b�h��	CBOSS_EXE::�f�X�g���N�^
	//
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	CBOSS_EXE::~CBOSS_EXE(void)
	{
	//	���֘A�̉��
		SAFE_DELETE(this->sStaticStep);				//!< ����
		SAFE_DELETE(this->sStaticJumpStart);		//!< �W�����v��
		SAFE_DELETE(this->sStaticJumpEnd);			//!< ���n��
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::soundStep
	//
	//	�@�\		�����Ǘ�
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::soundStep(void)
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
	//!	���\�b�h��	CBOSS_EXE::soundJumpStart
	//
	//	�@�\		�W�����v���Ǘ�
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::soundJumpStart(void)
	{
	//	�W�����v����炷
		this->sStaticJumpStart->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::soundJumpEnd
	//
	//	�@�\		���n���Ǘ�
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::soundJumpEnd(void)
	{
	//	���n����炷
		this->sStaticJumpEnd->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::opeAppear
	//
	//	�@�\		�o��������
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::opeAppear(void)
	{
	//	�d�͌���
		this->opeEffectGravity();
	//	�v���C���[�̕�������
		this->opeTurnToPlayer();
	//	�G�N�X���n�ʂɕt������A�ҋ@
		if(	!this->bFlighting)
		{
			this->changeAction(eBEXEA_APPEAR_WAIT);
			this->soundJumpEnd();	//!< ���n��
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::opeAppearWait
	//
	//	�@�\		�o���ҋ@������
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::opeAppearWait(void)
	{
	//	�d�͌���
		this->opeEffectGravity();
	//	�v���C���[�̕�������
		this->opeTurnToPlayer();
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::opeWait
	//
	//	�@�\		�ҋ@������
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::opeWait(void)
	{
	//	�d�͌���
		this->opeEffectGravity();

	//	�v���C���[�ɓ���������A�_���[�W
		if(	(*this->player)->intersectRadius(this))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

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
						this->changeAction(eBEXEA_DRILLCLAW_JUMP);
						break;
				//	�_�b�V���N���[
					case 1:
						this->changeAction(eBEXEA_DASHCLAW_DASH);
						break;
				}
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::opeDamage
	//
	//	�@�\		�_���[�W������
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::opeDamage(void)
	{
	//	�ジ����
		this->moveRear();

	//	�t���[�������܂ōs������A�ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10)
		{
		//	�v���C���[�̕�������
			this->opeTurnToPlayer();
			this->changeAction(eBEXEA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::opeDrillClawJump
	//
	//	�@�\		�h�����N���[�����W�����v��
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::opeDrillClawJump(void)
	{
	//	�d�͌���
		this->opeEffectGravity();

	//	�v���C���[�ɓ���������A�_���[�W
		if(	(*this->player)->intersectRadius(this))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

	//	�t���[�������܂ōs������A�h�����N���[
		this->iFrameCnt++;
		if(	this->iFrameCnt > 30)
		{
		//	�v���C���[�̕�������
			this->opeTurnToPlayer();
			this->changeAction(eBEXEA_DRILLCLAW);
		//	�p�x�Ə������킹��
			float	X	= this->getLocX() - (*this->player)->getLocX();
			float	Y	= this->getLocY() - (*this->player)->getLocY();
			float	dir	= atan2(Y, X) * 180.0f / D3DX_PI;
			this->setAngleZ((dir > 90.0f && dir <= 270.0f) ? dir + 180.0f : dir);
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::opeDrillClaw
	//
	//	�@�\		�h�����N���[��
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::opeDrillClaw(void)
	{
	//	�t���[�������ړ��ʂɐؑ�
		this->setMoveQuantity((float)(this->iFrameCnt * 3));
	//	�ːi���A�ǂɓ���������ҋ@
		if(	this->stageG->intersect(
				this,
				ADVENT_FRONTHIT,
				this->fWallSpace + this->getMoveQuantity() + 100.0f))
		{
			this->changeAction(eBEXEA_WAIT);
			return;
		}

		else
	//	�v���C���[�ɓ���������A�_���[�W
		if(	(*this->player)->intersect(
					this,
					ADVENT_FRONTHIT,
					this->fWallSpace + this->getMoveQuantity() + 100.0f))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}
	//	�����܂ŗ�����A�ړ�
		ADVENT_FRONTMOVE;

	//	�t���[�������܂ŏオ������ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 60)
		{
			this->changeAction(eBEXEA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::opeDashClawDash
	//
	//	�@�\		�_�b�V���N���[�����_�b�V����
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::opeDashClawDash(void)
	{
	//	����
		this->soundStep();
	//	�ːi���A�ǂɓ���������ҋ@
		if(	this->stageG->intersect(
				this,
				ADVENT_FRONTHIT,
				this->fWallSpace + this->getMoveQuantity() + 100.0f))
		{
			this->changeAction(eBEXEA_WAIT);
			return;
		}
	//	�O�i
		ADVENT_FRONTMOVE;

	//	�v���C���[�ɓ���������A�_���[�W
		if(	(*this->player)->intersectRadius(this))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}

	//	�v���C���[�L�����N�^�[���߂��Ȃ�A�_�b�V���N���[�U��
		float	length	= (*this->player)->getLocX() - this->getLocX();
		length			= (length >= 0.0f ? length : -length);
		if(	length < 500.0f)
		{
			this->changeAction(eBEXEA_DASHCLAW);
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::opeDashClaw
	//
	//	�@�\		�_�b�V���N���[��
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::opeDashClaw(void)
	{
	//	����
		this->soundStep();
	//	�ːi���A�ǂɓ���������ҋ@
		if(	this->stageG->intersect(
				this,
				ADVENT_FRONTHIT,
				this->fWallSpace + this->getMoveQuantity() + 150.0f))
		{
			this->changeAction(eBEXEA_WAIT);
			return;
		}

		else
	//	�v���C���[�ɓ���������A�_���[�W
		if(	(*this->player)->intersect(
				this,
				ADVENT_FRONTHIT,
				this->fWallSpace + this->getMoveQuantity() + 150.0f))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}
	//	�O�i
		ADVENT_FRONTMOVE;
	//	�t���[�������܂ŏオ������ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 30)
		{
			this->changeAction(eBEXEA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::opeDisappearWait
	//
	//	�@�\		���őҋ@��
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::opeDisappearWait(void)
	{
	//	�d�͌���
		this->opeEffectGravity();

		this->iFrameCnt++;
	//	10�t���[�����݂ŉ�����
		if(	this->iFrameCnt > 10)
		{
		//	���̈ʒu�������_���Z�o
			VEC3	inSmokeLoc	= this->getWorldCenter();
			inSmokeLoc.x	+= float(rand() % 300 - 150);
			inSmokeLoc.y	+= float(rand() % 300 - 150);
			inSmokeLoc.z	+= float(rand() % 100 -  50);

			C3D_EFFECT_CENTER::occuEffectsInstance(EFFECT_SMOKE, inSmokeLoc, 0.3f);
			this->iFrameCnt = 0;
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::opeDisappear
	//
	//	�@�\		���Ŏ�
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::opeDisappear(void)
	{
		this->iFrameCnt++;
	//	8�t���[�����݂ŉ�����
		if(	this->iFrameCnt % 3)
		{
		//	���̈ʒu�������_���Z�o
			VEC3	inSmokeLoc	= this->getWorldCenter();
			inSmokeLoc.x	+= float(rand() % 400 - 200);
			inSmokeLoc.y	+= float(rand() % 400 - 200);
			inSmokeLoc.z	+= float(rand() % 100 -  50);

			C3D_EFFECT_CENTER::occuEffectsInstance(EFFECT_SMOKE, inSmokeLoc, 0.5f);
		}

	//	�t���[�������܂ŏオ������A����
		if(	this->iFrameCnt > 150)
		{
			this->setVisible(FALSE);
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::changeAction
	//
	//	�@�\		��ԕω�
	//	����		inNewAction		�V���ȍs���p�^�[��
	//	�߂�l		TRUE			�V���ȍs���p�^�[���ɕω�����
	//				FALSE			�V���ȍs���p�^�[���ɕω����Ă��Ȃ�
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	BOOL
	CBOSS_EXE::changeAction(BOSSEXE_ACTION inNewAction)
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
			case	eBEXEA_APPEAR:				//!< �o����
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_APPEAR_WAIT:			//!< �o���ҋ@��
				this->setMoveQuantity(0.0f);
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_WAIT:				//!< �ҋ@��
				this->setMoveQuantity(30.0f);
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_DAMAGE:				//!< �_���[�W������
				this->setMoveQuantity(10.0f);
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_DRILLCLAW_JUMP:		//!< �h�����N���[�����W�����v��
				this->setAnimSpeed(0.005000f);
				this->moveJumping(this->fJumpHeight);
				this->soundJumpStart();
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_DRILLCLAW:			//!< �h�����N���[��
				this->setMoveQuantity(0.0f);
				this->setAnimSpeed();
				break;
			case	eBEXEA_DASHCLAW_DASH:		//!< �_�b�V���N���[�����_�b�V����
				this->setMoveQuantity(35.0f);
				this->setAnimSpeed(0.032f);
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_DASHCLAW:			//!< �_�b�V���N���[��
				this->setMoveQuantity(50.0f);
				this->setAnimSpeed(0.032f);
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_DISAPPEAR_WAIT:		//!< ���őҋ@��
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				break;
			case	eBEXEA_DISAPPEAR:			//!< ���Ŏ�
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				this->iHitFrame	= 150;
				break;
			default:							//!< ��L�ȊO
				break;
		}

	//	�t���[���J�E���^��������
		this->iFrameCnt			 = 0;

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::reactionMessage
	//
	//	�@�\		���A�N�V����
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::reactionMessage(REACTION_MESSAGE inMessage)
	{
	//	���Ŏ��A���őҋ@���̎��͏�����Ԃ�
		if(	this->action == eBEXEA_DISAPPEAR_WAIT	||
			this->action == eBEXEA_DISAPPEAR)	return;

	//	�G�i�W�[�{�[���ɓ���������
		if(	inMessage == eREAMSG_ENERGY_HIT)
		{
		//	�Փ˃t���[����0�ȉ��Ȃ�A�_���[�W
			if(	this->iHitFrame <= 0)
			{
				this->iHitFrame  = 100;
				this->iLife--;
			//	���C�t�������Ȃ�����A���ł���
				if(	this->iLife <= 0)
				{
					this->changeAction(eBEXEA_DISAPPEAR_WAIT);
				}
			//	�U���ȊO�̎��Ȃ�A�ジ����
				else
				if(	this->action != eBEXEA_DRILLCLAW	&&
					this->action != eBEXEA_DASHCLAW)
				{
					this->changeAction(eBEXEA_DAMAGE);
				}
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::resetParameter
	//
	//	�@�\		�p�����[�^�̃��Z�b�g
	//	�X�V		2009/02/18		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::resetParameter(void)
	{
		CKAGETOKI_ADVENT_BOSS::resetParameter();	//!< ���C�t�̃��Z�b�g
		this->iHitFrame	= 0;						//!< �Փ˂��ē_�ł��鎞��
		this->changeAction(eBEXEA_APPEAR);			//!< ������Ԃ͏o��
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::operation
	//
	//	�@�\		��{����
	//	����		*input			���͊Ǘ�
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CBOSS_EXE::operation(CINPUT* input)
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
			case	eBEXEA_APPEAR:				//!< �o����
				this->opeAppear();
				break;
			case	eBEXEA_APPEAR_WAIT:			//!< �o���ҋ@��
				this->opeAppearWait();
				break;
			case	eBEXEA_WAIT:				//!< �ҋ@��
				this->opeWait();
				break;
			case	eBEXEA_DAMAGE:				//!< �_���[�W������
				this->opeDamage();
				break;
			case	eBEXEA_DRILLCLAW_JUMP:		//!< �h�����N���[�����W�����v��
				this->opeDrillClawJump();
				break;
			case	eBEXEA_DRILLCLAW:			//!< �h�����N���[��
				this->opeDrillClaw();
				break;
			case	eBEXEA_DASHCLAW_DASH:		//!< �_�b�V���N���[�����_�b�V����
				this->opeDashClawDash();
				break;
			case	eBEXEA_DASHCLAW:			//!< �_�b�V���N���[��
				this->opeDashClaw();
				break;
			case	eBEXEA_DISAPPEAR_WAIT:		//!< ���őҋ@��
				this->opeDisappearWait();
				break;
			case	eBEXEA_DISAPPEAR:			//!< ���Ŏ�
				this->opeDisappear();
				break;
		}//	default	break;
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_EXE::draw
	//
	//	�@�\		�`��
	//	����		inDev			Direct3D�f�o�C�X
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	BOOL
	CBOSS_EXE::draw(CONST DEV inDev)
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