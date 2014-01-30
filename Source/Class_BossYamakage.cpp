//***********************************************************************************
//!	�t�@�C����		Class_BossYamakage.cpp
//
//	�@�\			�A�h�x���`���[�Q�[�����[�h�́u���}�J�Q�v�N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2009/02/16	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_BossYamakage.h"		//!< �A�h�x���`���[�Q�[���u���}�J�Q�v

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CBOSS_YAMAKAGE::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		���b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	CBOSS_YAMAKAGE::CBOSS_YAMAKAGE(CONST DEV	inDev,
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

	//	�A�j���[�V�����̊��蓖�Ă��s��(���}�J�Q��p�̃N���X�Ȃ̂ŁA�A�A)
		this->iAnimTemp[eBYAMAKAGEA_APPEAR]			= 1;
		this->iAnimTemp[eBYAMAKAGEA_APPEAR_WAIT]	= 0;
		this->iAnimTemp[eBYAMAKAGEA_WAIT]			= 0;
		this->iAnimTemp[eBYAMAKAGEA_DAMAGE]			= 3;
		this->iAnimTemp[eBYAMAKAGEA_WEAPON_TAKEOUT]	= 2;
		this->iAnimTemp[eBYAMAKAGEA_WEAPON_ATTACK]	= 2;
		this->iAnimTemp[eBYAMAKAGEA_ATTACK_ANDMOVE]	= 1;
		this->iAnimTemp[eBYAMAKAGEA_DISAPPEAR_WAIT]	= 0;
		this->iAnimTemp[eBYAMAKAGEA_DISAPPEAR]		= 0;

		this->iMaxLife			= YAMAKAGE_MAXLIFE;				//!< �ő�̗�
		this->iLife				= this->iMaxLife;				//!< �c��̗�
		this->iHitFrame			= 0;							//!< �Փ˂��ē_�ł��鎞��

	//	���}�J�Q����
		this->yamakageWeaponA	= new CYAMAKAGE_WEAPON(inDev);
		this->yamakageWeaponA->setVisible(FALSE);
		this->yamakageWeaponB	= new CYAMAKAGE_WEAPON(inDev);
		this->yamakageWeaponB->setVisible(FALSE);
		this->yamakageWeaponC	= new CYAMAKAGE_WEAPON(inDev);
		this->yamakageWeaponC->setVisible(FALSE);
		this->yamakageWeaponD	= new CYAMAKAGE_WEAPON(inDev);
		this->yamakageWeaponD->setVisible(FALSE);
		this->iAttackCnt		= 0;
		this->iMoveCnt			= 0;

	//	�t���O�Q
		this->action			= eBYAMAKAGEA_MAX;				//!< �ŏ��͑ҋ@�ȊO���ł��ǂ�
		this->changeAction(eBYAMAKAGEA_APPEAR);					//!< ������Ԃ͏o��

	//	���̃Z�b�g
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
	//!	���\�b�h��	CBOSS_YAMAKAGE::�f�X�g���N�^
	//
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	CBOSS_YAMAKAGE::~CBOSS_YAMAKAGE(void)
	{
	//	���֘A�̉��
		SAFE_DELETE(this->sStaticJumpStart);		//!< �W�����v��
		SAFE_DELETE(this->sStaticJumpEnd);			//!< ���n��
		SAFE_DELETE(this->yamakageWeaponA);			//!< ���}�J�Q����A
		SAFE_DELETE(this->yamakageWeaponB);			//!< ���}�J�Q����B
		SAFE_DELETE(this->yamakageWeaponC);			//!< ���}�J�Q����A
		SAFE_DELETE(this->yamakageWeaponD);			//!< ���}�J�Q����B
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_YAMAKAGE::soundJumpStart
	//
	//	�@�\		�W�����v���Ǘ�
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_YAMAKAGE::soundJumpStart(void)
	{
	//	�W�����v����炷
		this->sStaticJumpStart->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_YAMAKAGE::soundJumpEnd
	//
	//	�@�\		���n���Ǘ�
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_YAMAKAGE::soundJumpEnd(void)
	{
	//	���n����炷
		this->sStaticJumpEnd->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_YAMAKAGE::opeAppear
	//
	//	�@�\		�o��������
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeAppear(void)
	{
	//	�d�͌���
		this->opeEffectGravity();
	//	�v���C���[�̕�������
		this->opeTurnToPlayer();

	//	���}�J�Q���n�ʂɕt������A�ҋ@
		if(	!this->bFlighting)
		{
			this->changeAction(eBYAMAKAGEA_APPEAR_WAIT);
			this->soundJumpEnd();	//!< ���n��
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_YAMAKAGE::opeAppearWait
	//
	//	�@�\		�o���ҋ@������
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeAppearWait(void)
	{
	//	�d�͌���
		this->opeEffectGravity();
	//	�v���C���[�̕�������
		this->opeTurnToPlayer();
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_YAMAKAGE::opeWait
	//
	//	�@�\		�ҋ@������
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeWait(void)
	{
	//	�d�͌���
		this->opeEffectGravity();

	//	����̑���
		this->yamakageWeaponA->operation();
		this->yamakageWeaponB->operation();

	//	�󒆂ɂ��Ȃ��Ȃ�
		if(	!this->bFlighting)
		{
		//	�v���C���[�̕�������
			this->opeTurnToPlayer();
		//	�t���[�������܂ōs������A�����_���ɍU���Ɉڂ�
			this->iFrameCnt++;
			if(	this->iFrameCnt > 15)
			{
			//	���C�t�ɉe���͂Ȃ��A����A�ƕ���B���o��
				if(	!this->yamakageWeaponA->getVisible()	||
					!this->yamakageWeaponB->getVisible())
				{
					this->changeAction(eBYAMAKAGEA_WEAPON_TAKEOUT);
				}
			//	���C�t��8�ȉ��ɂȂ�����A����C���o��
				else
				if(	this->iLife <= 8	&&
					!this->yamakageWeaponC->getVisible())
				{
					this->changeAction(eBYAMAKAGEA_WEAPON_TAKEOUT);
				}
			//	���C�t��4�ȉ��ɂȂ�����A����D���o��
				else
				if(	this->iLife <= 4	&&
					!this->yamakageWeaponD->getVisible())
				{
					this->changeAction(eBYAMAKAGEA_WEAPON_TAKEOUT);
				}
			//	��L�ȊO�͍U��
				else
				{
					this->changeAction(eBYAMAKAGEA_WEAPON_ATTACK);
				}
				this->iFrameCnt	= 0;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_YAMAKAGE::opeDamage
	//
	//	�@�\		�_���[�W������
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeDamage(void)
	{
	//	�ジ����
		this->moveRear();
	//	�t���[�������܂ōs������A�ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10)
		{
		//	�v���C���[�̕�������
			this->opeTurnToPlayer();
			this->changeAction(eBYAMAKAGEA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_YAMAKAGE::opeWeaponTakeOut
	//
	//	�@�\		������o����
	//	�X�V		2009/02/17		<�V�K>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeWeaponTakeOut(void)
	{
	//	�d�͌���
		this->opeEffectGravity();

	//	����̑���
		this->yamakageWeaponA->operation();
		this->yamakageWeaponB->operation();
		this->yamakageWeaponC->operation();
		this->yamakageWeaponD->operation();

	//	�w�肷��t���[����������A������o��
		this->iFrameCnt++;
		if(	this->iFrameCnt > 50)
		{
			this->changeAction(eBYAMAKAGEA_WAIT);
			if(!this->yamakageWeaponA->getVisible())
			{
				this->yamakageWeaponA->setVisible(TRUE);
				this->yamakageWeaponA->setTargetPlayer(this->player);
				this->yamakageWeaponA->setViewG(this->viewG);
				this->yamakageWeaponA->setStageG(this->stageG);
				this->yamakageWeaponA->setLoc(
					&(this->getLoc() + VEC3(0.0f, 200.0f, 0.0f)));
				this->stageG->add(this->yamakageWeaponA);
				return;
			}
			if(!this->yamakageWeaponB->getVisible())
			{
				this->yamakageWeaponB->setVisible(TRUE);
				this->yamakageWeaponB->setTargetPlayer(this->player);
				this->yamakageWeaponB->setViewG(this->viewG);
				this->yamakageWeaponB->setStageG(this->stageG);
				this->yamakageWeaponB->setLoc(
					&(this->getLoc() + VEC3(0.0f, 200.0f, 0.0f)));
				this->stageG->add(this->yamakageWeaponB);
				return;
			}
			if(!this->yamakageWeaponC->getVisible())
			{
				this->yamakageWeaponC->setVisible(TRUE);
				this->yamakageWeaponC->setTargetPlayer(this->player);
				this->yamakageWeaponC->setViewG(this->viewG);
				this->yamakageWeaponC->setStageG(this->stageG);
				this->yamakageWeaponC->setLoc(
					&(this->getLoc() + VEC3(0.0f, 200.0f, 0.0f)));
				this->stageG->add(this->yamakageWeaponC);
				return;
			}
			if(!this->yamakageWeaponD->getVisible())
			{
				this->yamakageWeaponD->setVisible(TRUE);
				this->yamakageWeaponD->setTargetPlayer(this->player);
				this->yamakageWeaponD->setViewG(this->viewG);
				this->yamakageWeaponD->setStageG(this->stageG);
				this->yamakageWeaponD->setLoc(
					&(this->getLoc() + VEC3(0.0f, 200.0f, 0.0f)));
				this->stageG->add(this->yamakageWeaponD);
				return;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_YAMAKAGE::opeWeaponAttack
	//
	//	�@�\		����U��
	//	�X�V		2009/02/17		<�V�K>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeWeaponAttack(void)
	{
	//	�d�͌���
		this->opeEffectGravity();

	//	����̑���
		this->yamakageWeaponA->operation();
		this->yamakageWeaponB->operation();
		this->yamakageWeaponC->operation();
		this->yamakageWeaponD->operation();

	//	�w�肷��t���[����������A����U��
		this->iFrameCnt++;
		if(	this->iFrameCnt > 80)
		{
			this->yamakageWeaponA->setWeaponAction(eOPEYW_ATTACK);
			this->yamakageWeaponB->setWeaponAction(eOPEYW_ATTACK);
			this->yamakageWeaponC->setWeaponAction(eOPEYW_ATTACK);
			this->yamakageWeaponD->setWeaponAction(eOPEYW_ATTACK);
			this->iFrameCnt	= 0;
			this->iAttackCnt++;
		//	�U���񐔂�3��ȏ�ɂȂ�����A�U�����Ȃ���ړ�����
			if(	this->iAttackCnt >= 3)
			{
				this->changeAction(eBYAMAKAGEA_ATTACK_ANDMOVE);
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_YAMAKAGE::opeAttackAndMove
	//
	//	�@�\		�U�����Ȃ���ړ�
	//	�X�V		2009/02/17		<�V�K>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeAttackAndMove(void)
	{
	//	����̑���
		this->yamakageWeaponA->operation();
		this->yamakageWeaponB->operation();
		this->yamakageWeaponC->operation();
		this->yamakageWeaponD->operation();

	//	�ړ�����t���[���ŁA�ړ�����
		this->iMoveCnt++;
		if(	this->iMoveCnt < 90)
		{
			this->moveUpDown(eUD_UP, 2.0f);
		}
		else
		{
		//	�ːi���A�ǂɓ���������ҋ@
			if(	this->stageG->intersect(
					this,
					ADVENT_FRONTHIT,
					this->fWallSpace + 20.0f + 150.0f))
			{
				this->changeAction(eBYAMAKAGEA_WAIT);
				return;
			}
		//	�O�i
			this->moveFrontRear(eFR_FRONT, 20.0f);
		}

	//	�w�肷��t���[����������A����U��
		this->iFrameCnt++;
		if(	this->iFrameCnt > 120)
		{
			this->yamakageWeaponA->setWeaponAction(eOPEYW_ATTACK);
			this->yamakageWeaponB->setWeaponAction(eOPEYW_ATTACK);
			this->yamakageWeaponC->setWeaponAction(eOPEYW_ATTACK);
			this->yamakageWeaponD->setWeaponAction(eOPEYW_ATTACK);
			this->iFrameCnt	= 0;
			this->iAttackCnt++;
		//	�U���񐔂�3��ȏ�ɂȂ�����A�U�����Ȃ���ړ�����
			if(	this->iAttackCnt >= 3)
			{
				this->changeAction(eBYAMAKAGEA_WAIT);
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_YAMAKAGE::opeDisappearWait
	//
	//	�@�\		���őҋ@��
	//	�X�V		2009/02/17		<�V�K>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeDisappearWait(void)
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
	//!	���\�b�h��	CBOSS_YAMAKAGE::opeDisappear
	//
	//	�@�\		���Ŏ�
	//	�X�V		2009/02/17		<�V�K>
	//================================================================================
	void
	CBOSS_YAMAKAGE::opeDisappear(void)
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
	//!	���\�b�h��	CBOSS_YAMAKAGE::changeAction
	//
	//	�@�\		��ԕω�
	//	����		inNewAction		�V���ȍs���p�^�[��
	//	�߂�l		TRUE			�V���ȍs���p�^�[���ɕω�����
	//				FALSE			�V���ȍs���p�^�[���ɕω����Ă��Ȃ�
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	BOOL
	CBOSS_YAMAKAGE::changeAction(BOSSYAMAKAGE_ACTION inNewAction)
	{
	//	�O�̏�ԂƐV���ȏ�Ԃ��ς�Ȃ��Ȃ�A�������Ȃ�
		if(	this->action == inNewAction)	return	FALSE;
	//	��L�̏������X���[�����̂ŁA��Ԃ̕ύX
		this->action = inNewAction;
	//	�A�j���[�V�������ύX
		this->changeAnimationAndBlend(this->iAnimTemp[this->action]);

	//	�t���[���J�E���^��������
		this->iFrameCnt	= 0;

	//	�V���ȏ�ԂŒǉ�����������Ȃ炱����
		switch(this->action)
		{
			case	eBYAMAKAGEA_APPEAR:				//!< �o����
				this->setMoveQuantity(0.0f);
				this->setAngleZ(0.0f);
				this->yamakageWeaponA->setVisible(FALSE);
				this->yamakageWeaponB->setVisible(FALSE);
				this->yamakageWeaponC->setVisible(FALSE);
				this->yamakageWeaponD->setVisible(FALSE);
				break;
			case	eBYAMAKAGEA_APPEAR_WAIT:		//!< �o���ҋ@��
				this->setMoveQuantity(0.0f);
				this->setAngleZ(0.0f);
				break;
			case	eBYAMAKAGEA_WAIT:				//!< �ҋ@��
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				break;
			case	eBYAMAKAGEA_DAMAGE:				//!< �_���[�W������
				this->setMoveQuantity(10.0f);
				this->setAngleZ(0.0f);
				break;
			case	eBYAMAKAGEA_WEAPON_TAKEOUT:		//!< ������o����
				this->setMoveQuantity(0.0f);
				this->setAngleZ(0.0f);
				break;
			case	eBYAMAKAGEA_WEAPON_ATTACK:		//!< ����U��
				this->setMoveQuantity(0.0f);
				this->setAngleZ(0.0f);
				this->iAttackCnt	= 0;
				break;
			case	eBYAMAKAGEA_ATTACK_ANDMOVE:		//!< �U�����Ȃ���ړ�
				this->setMoveQuantity(10.0f);
				this->setAngleZ(0.0f);
				this->iAttackCnt	= 0;
				this->iMoveCnt		= 0;
				break;
			case	eBYAMAKAGEA_DISAPPEAR_WAIT:		//!< ���őҋ@��
				this->setMoveQuantity(0.0f);
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				break;
			case	eBYAMAKAGEA_DISAPPEAR:			//!< ���Ŏ�
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				this->iHitFrame	= 150;
				break;
			default:								//!< ��L�ȊO
				break;
		}

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_YAMAKAGE::reactionMessage
	//
	//	�@�\		���A�N�V����
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_YAMAKAGE::reactionMessage(REACTION_MESSAGE inMessage)
	{
	//	���Ŏ��A���őҋ@���̎��͏�����Ԃ�
		if(	this->action == eBYAMAKAGEA_DISAPPEAR_WAIT	||
			this->action == eBYAMAKAGEA_DISAPPEAR)	return;

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
					this->changeAction(eBYAMAKAGEA_DISAPPEAR_WAIT);
				}
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_YAMAKAGE::resetParameter
	//
	//	�@�\		�p�����[�^�̃��Z�b�g
	//	�X�V		2009/02/18		<�V�K>
	//================================================================================
	void
	CBOSS_YAMAKAGE::resetParameter(void)
	{
		CKAGETOKI_ADVENT_BOSS::resetParameter();	//!< ���C�t�̃��Z�b�g
		this->iHitFrame		= 0;					//!< �Փ˂��ē_�ł��鎞��
		this->changeAction(eBYAMAKAGEA_APPEAR);		//!< ������Ԃ͏o��
		this->yamakageWeaponA->setVisible(FALSE);	//!< ����A�𑶍݊m�F����
		this->yamakageWeaponB->setVisible(FALSE);	//!< ����B�𑶍݊m�F����
		this->yamakageWeaponC->setVisible(FALSE);	//!< ����A�𑶍݊m�F����
		this->yamakageWeaponD->setVisible(FALSE);	//!< ����B�𑶍݊m�F����
		this->iAttackCnt	= 0;					//!< �U������J�E���^��������
		this->iMoveCnt		= 0;					//!< ���쐧��J�E���^��������
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_YAMAKAGE::operation
	//
	//	�@�\		��{����
	//	����		*input			���͊Ǘ�
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_YAMAKAGE::operation(CINPUT* input)
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
			case	eBYAMAKAGEA_APPEAR:				//!< �o����
				this->opeAppear();
				break;
			case	eBYAMAKAGEA_APPEAR_WAIT:		//!< �o���ҋ@��
				this->opeAppearWait();
				break;
			case	eBYAMAKAGEA_WAIT:				//!< �ҋ@��
				this->opeWait();
				break;
			case	eBYAMAKAGEA_DAMAGE:				//!< �_���[�W������
				this->opeDamage();
				break;
			case	eBYAMAKAGEA_WEAPON_TAKEOUT:		//!< ������o����
				this->opeWeaponTakeOut();
				break;
			case	eBYAMAKAGEA_WEAPON_ATTACK:		//!< ����U��
				this->opeWeaponAttack();
				break;
			case	eBYAMAKAGEA_ATTACK_ANDMOVE:		//!< �U�����Ȃ���ړ�
				this->opeAttackAndMove();
				break;
			case	eBYAMAKAGEA_DISAPPEAR_WAIT:		//!< ���őҋ@��
				this->opeDisappearWait();
				break;
			case	eBYAMAKAGEA_DISAPPEAR:			//!< ���Ŏ�
				this->opeDisappear();
				break;
		}//	default	break;

	//	��Ƀv���C���[�ɓ���������A�_���[�W
		if(	(*this->player)->intersectRadius(this))
		{
			(*this->player)->reactionMessage(eREAMSG_ENEMY_ATTACK_LV1);
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_YAMAKAGE::draw
	//
	//	�@�\		�`��
	//	����		inDev			Direct3D�f�o�C�X
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	BOOL
	CBOSS_YAMAKAGE::draw(CONST DEV inDev)
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

	//	���C�t���]���Ă���A���C�t��`�悷��
		if(	this->iLife > 0)
		{
			this->yamakageWeaponA->draw(inDev);
			this->yamakageWeaponB->draw(inDev);
			this->yamakageWeaponC->draw(inDev);
			this->yamakageWeaponD->draw(inDev);
		}

	//	�C���^�[�t�F�[�X�`��
		this->drawInterface(inDev);

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------