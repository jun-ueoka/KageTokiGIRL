//***********************************************************************************
//!	�t�@�C����		Class_BossFeather.cpp
//
//	�@�\			�A�h�x���`���[�Q�[�����[�h�́u�t�F�U�[�v�N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2009/02/16	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_BossFeather.h"		//!< �A�h�x���`���[�Q�[���u�t�F�U�[�v

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CBOSS_FEATHER::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		���b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	CBOSS_FEATHER::CBOSS_FEATHER(CONST DEV		inDev,
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

	//	�A�j���[�V�����̊��蓖�Ă��s��(�t�F�U�[��p�̃N���X�Ȃ̂ŁA�A�A)
		this->iAnimTemp[eBFEATHERA_APPEAR]			= 2;
		this->iAnimTemp[eBFEATHERA_APPEAR_WAIT]		= 0;
		this->iAnimTemp[eBFEATHERA_WAIT]			= 0;
		this->iAnimTemp[eBFEATHERA_DAMAGE]			= 6;
		this->iAnimTemp[eBFEATHERA_DISAPPEAR_WAIT]	= 0;
		this->iAnimTemp[eBFEATHERA_DISAPPEAR]		= 0;

		this->iMaxLife					= FEATHER_MAXLIFE;		//!< �ő�̗�
		this->iLife						= this->iMaxLife;		//!< �c��̗�
		this->iHitFrame					= 0;					//!< �Փ˂��ē_�ł��鎞��

	//	�t���O�Q
		this->action			= eBFEATHERA_MAX;				//!< �ŏ��͑ҋ@�ȊO���ł��ǂ�
		this->changeAction(eBFEATHERA_APPEAR);					//!< ������Ԃ͏o��

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
	//!	���\�b�h��	CBOSS_FEATHER::�f�X�g���N�^
	//
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	CBOSS_FEATHER::~CBOSS_FEATHER(void)
	{
	//	���֘A�̉��
		SAFE_DELETE(this->sStaticStep);				//!< ����
		SAFE_DELETE(this->sStaticJumpStart);		//!< �W�����v��
		SAFE_DELETE(this->sStaticJumpEnd);			//!< ���n��
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_FEATHER::soundStep
	//
	//	�@�\		�����Ǘ�
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_FEATHER::soundStep(void)
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
	//!	���\�b�h��	CBOSS_FEATHER::soundJumpStart
	//
	//	�@�\		�W�����v���Ǘ�
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_FEATHER::soundJumpStart(void)
	{
	//	�W�����v����炷
		this->sStaticJumpStart->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_FEATHER::soundJumpEnd
	//
	//	�@�\		���n���Ǘ�
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_FEATHER::soundJumpEnd(void)
	{
	//	���n����炷
		this->sStaticJumpEnd->play(0, NULL);
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_FEATHER::opeAppear
	//
	//	�@�\		�o��������
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_FEATHER::opeAppear(void)
	{
	//	�d�͌���
		this->opeEffectGravity();
	//	�v���C���[�̕�������
		this->opeTurnToPlayer();
	//	���̍����ɕt������A�o���ҋ@
		if(	this->getLocY() < 1000.0f)
		{
			this->changeAction(eBFEATHERA_APPEAR_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_FEATHER::opeAppearWait
	//
	//	�@�\		�o���ҋ@������
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_FEATHER::opeAppearWait(void)
	{
	//	�v���C���[�̕�������
		this->opeTurnToPlayer();
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_FEATHER::opeWait
	//
	//	�@�\		�ҋ@������
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_FEATHER::opeWait(void)
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

	//================================================================================
	//!	���\�b�h��	CBOSS_FEATHER::opeDamage
	//
	//	�@�\		�_���[�W������
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_FEATHER::opeDamage(void)
	{
	//	�ジ����
		this->moveRear();
	//	�t���[�������܂ōs������A�ҋ@
		this->iFrameCnt++;
		if(	this->iFrameCnt > 10)
		{
		//	�v���C���[�̕�������
			this->opeTurnToPlayer();
			this->changeAction(eBFEATHERA_WAIT);
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_FEATHER::opeDisappearWait
	//
	//	�@�\		���őҋ@��
	//	�X�V		2009/02/19		<�V�K>
	//================================================================================
	void
	CBOSS_FEATHER::opeDisappearWait(void)
	{
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
	//!	���\�b�h��	CBOSS_FEATHER::opeDisappear
	//
	//	�@�\		���Ŏ�
	//	�X�V		2009/02/19		<�V�K>
	//================================================================================
	void
	CBOSS_FEATHER::opeDisappear(void)
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
	//!	���\�b�h��	CBOSS_FEATHER::changeAction
	//
	//	�@�\		��ԕω�
	//	����		inNewAction		�V���ȍs���p�^�[��
	//	�߂�l		TRUE			�V���ȍs���p�^�[���ɕω�����
	//				FALSE			�V���ȍs���p�^�[���ɕω����Ă��Ȃ�
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	BOOL
	CBOSS_FEATHER::changeAction(BOSSFEATHER_ACTION inNewAction)
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
			case	eBFEATHERA_APPEAR:				//!< �o����
				this->setAngleZ(0.0f);
				break;
			case	eBFEATHERA_APPEAR_WAIT:			//!< �o���ҋ@��
				this->setMoveQuantity(0.0f);
				this->setAngleZ(0.0f);
				break;
			case	eBFEATHERA_WAIT:				//!< �ҋ@��
				this->setMoveQuantity(30.0f);
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				break;
			case	eBFEATHERA_DAMAGE:				//!< �_���[�W������
				this->setMoveQuantity(10.0f);
				this->setAngleZ(0.0f);
				break;
			case	eBFEATHERA_DISAPPEAR_WAIT:		//!< ���őҋ@��
				this->setMoveQuantity(0.0f);
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				break;
			case	eBFEATHERA_DISAPPEAR:			//!< ���Ŏ�
				this->setAnimSpeed();
				this->setAngleZ(0.0f);
				this->iHitFrame	= 150;
				break;
			default:								//!< ��L�ȊO
				break;
		}

	//	�t���[���J�E���^��������
		this->iFrameCnt	= 0;

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_FEATHER::reactionMessage
	//
	//	�@�\		���A�N�V����
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_FEATHER::reactionMessage(REACTION_MESSAGE inMessage)
	{
	//	���Ŏ��A���őҋ@���̎��͏�����Ԃ�
		if(	this->action == eBFEATHERA_DISAPPEAR_WAIT	||
			this->action == eBFEATHERA_DISAPPEAR)	return;

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
					this->changeAction(eBFEATHERA_DISAPPEAR_WAIT);
				}
			//	�U���ȊO�̎��Ȃ�A�ジ����
				else
				if(	1)
				{
					this->changeAction(eBFEATHERA_DAMAGE);
				}
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_FEATHER::resetParameter
	//
	//	�@�\		�p�����[�^�̃��Z�b�g
	//	�X�V		2009/02/18		<�V�K>
	//================================================================================
	void
	CBOSS_FEATHER::resetParameter(void)
	{
		CKAGETOKI_ADVENT_BOSS::resetParameter();	//!< ���C�t�̃��Z�b�g
		this->iHitFrame	= 0;						//!< �Փ˂��ē_�ł��鎞��
		this->changeAction(eBFEATHERA_APPEAR);		//!< ������Ԃ͏o��
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_FEATHER::operation
	//
	//	�@�\		��{����
	//	����		*input			���͊Ǘ�
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CBOSS_FEATHER::operation(CINPUT* input)
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
			case	eBFEATHERA_APPEAR:				//!< �o����
				this->opeAppear();
				break;
			case	eBFEATHERA_APPEAR_WAIT:			//!< �o���ҋ@��
				this->opeAppearWait();
				break;
			case	eBFEATHERA_WAIT:				//!< �ҋ@��
				this->opeWait();
				break;
			case	eBFEATHERA_DAMAGE:				//!< �_���[�W������
				this->opeDamage();
				break;
			case	eBFEATHERA_DISAPPEAR_WAIT:		//!< ���őҋ@��
				this->opeDisappearWait();
				break;
			case	eBFEATHERA_DISAPPEAR:			//!< ���Ŏ�
				this->opeDisappear();
				break;
		}//	default	break;
	}

	//================================================================================
	//!	���\�b�h��	CBOSS_FEATHER::draw
	//
	//	�@�\		�`��
	//	����		inDev			Direct3D�f�o�C�X
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	BOOL
	CBOSS_FEATHER::draw(CONST DEV inDev)
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