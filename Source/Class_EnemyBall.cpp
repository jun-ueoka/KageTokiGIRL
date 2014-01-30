//***********************************************************************************
//!	�t�@�C����		Class_EnemyBall.cpp
//
//	�@�\			�G�����ʂ������N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2009/02/17		<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_EnemyBall.h"
//#include	"Class_3DEffectCenter.h"	//!< �G�t�F�N�g�Z���^�[(�G�t�F�N�g�𔭐��������)

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CENEMY_BALL::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inKind			�{�[���̎��
	//	�X�V		2008/02/17		<�V�K>
	//================================================================================
	CENEMY_BALL::CENEMY_BALL(CONST DEV inDev, ENEMY_BALL_KIND inKind,
							 LPSTR inMeshName, LPSTR inEffectName)	:
	CMESH(inDev, inMeshName)
	{
	//	�����̏�����
		this->activate		= FALSE;
		this->stageG		= NULL;
		this->player		= NULL;
		this->iFrame		= 0;
		this->ballKind		= inKind;
		this->reactMessage	= eREAMSG_ENEMY_ATTACK_LV1;
		ZeroMemory(this->effectName, sizeof(this->effectName));
		this->iEffectCnt	= 10;
		this->iFrame		= 0;
		this->effectColor	= 0xFFFFFFFF;

	//	�G�t�F�N�g�̖��O�̊m��
		sprintf_s(this->effectName, sizeof(this->effectName), inEffectName);

	//	�G�t�F�N�g�̓o�^
		C3D_EFFECT_CENTER*	effectCenter = C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			this->effectName, new C3D_EFFECT(inDev, this->effectName, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			EFFECT_SMOKE, new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_BALL::�R���X�g���N�^
	//
	//	�@�\		�R�s�[
	//	����		inCopy			�R�s�[��
	//	�X�V		2008/02/17		<�V�K>
	//================================================================================
	CENEMY_BALL::CENEMY_BALL(CENEMY_BALL *inCopy)
	:	CMESH(inCopy)
	{
	//	�����̏�����
		this->activate		= FALSE;
		this->stageG		= NULL;
		this->player		= NULL;
		this->ballKind		= inCopy->getBallKind();
		this->reactMessage	= inCopy->getReactMessage();
		strcpy_s(this->effectName, sizeof(this->effectName), inCopy->getEffectName());
		this->iEffectCnt	= inCopy->getEffectCnt();
		this->iFrame		= 0;
		this->effectColor	= inCopy->getEffectColor();
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_BALL::�f�X�g���N�^
	//
	//	�X�V		2008/02/17		<�V�K>
	//================================================================================
	CENEMY_BALL::~CENEMY_BALL(void)
	{
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_BALL::shot
	//
	//	�@�\		����(����������Ɏg�p)
	//	����		inAngle			���ˊp�x
	//				inLoc			���ˈʒu
	//				inStageG		����X�e�[�W
	//				inPlayer		����v���C���[
	//				inFrame			�`��t���[��
	//	�X�V		2008/02/17		<�V�K>
	//================================================================================
	void
	CENEMY_BALL::shot(VEC3* inAngle, VEC3* inLoc, C3D_OBJECT_GROUP* inStageG,
					  CKAGETOKI_ADVENT_CHARACTER** inPlayer, int inFrame)
	{
	//	���ˉ\�̎�
		if(	!this->activate)
		{
		//	����
			this->activate	= TRUE;
			this->stageG	= inStageG;
			this->player	= inPlayer;
			this->iFrame	= inFrame;
			this->setVisible(TRUE);
			this->setAngle(inAngle);
			this->setLoc(inLoc);
		}
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_BALL::intersectG
	//
	//	�@�\		�Փ˔���
	//	�X�V		2008/02/17		<�V�K>
	//================================================================================
	void
	CENEMY_BALL::intersectG(void)
	{
	//	�G�O���[�v�Փ˔���
		if(	this->player	&&
			(*this->player)->intersectRadius(this))
		{
		//	�v���C���[�ɓ���������A�U�����A�N�V�������b�Z�[�W��n��
			(*this->player)->reactionMessage(this->reactMessage);
		//	������
			this->activate	= FALSE;
			this->iFrame	= 0;
		//	���G�t�F�N�g����
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE, this->getLoc(), 1.0f, VEC3(0.0f,0.0f,0.0f), D3DCOLOR_RGBA(0,0,0,255));
		}

		else
	//	�X�e�[�W�O���[�v�Փ˔���
		if(	this->stageG	&&
			this->stageG->intersect(this, this->getLine(), this->getMoveQuantity() + 1.0f))
		{
		//	������
			this->activate	= FALSE;
			this->iFrame	= 0;
		//	���G�t�F�N�g����
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE, this->getLoc(), 1.0f, VEC3(0.0f,0.0f,0.0f), D3DCOLOR_RGBA(0,0,0,255));
		}
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_BALL::opeStraight
	//
	//	�@�\		�꒼���ɔ��ł�������
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2008/02/20		<�V�K>
	//================================================================================
	void
	CENEMY_BALL::opeStraight(CONST DEV inDev)
	{
	//	���˂���Ă���Ȃ�A������s���A�Փ˔��������
		if(	this->activate)
		{
		//	���݂̕`��t���[�����A
		//	�G�t�F�N�g�̔����Ԋu�t���[���Ŋ���؂ꂽ��A�G�t�F�N�g����
			if(	!(this->iFrame % this->iEffectCnt))
			{
				C3D_EFFECT_CENTER::occuEffectsInstance(
					this->effectName, this->getLoc(), 1.0f);
			}

		//	�`��t���[�������炵�A�J�E���g��0�ȉ��ɂȂ�����A��������
			this->iFrame--;
			if(	this->iFrame <= 0)
			{
				this->setVisible(FALSE);
				this->activate	= FALSE;
			//	���ɏ��Ń��b�Z�[�W�^����
				this->setOrderMessage(eOMSG_DELETE);
			}
		//	�J�E���g���c���Ă���Ȃ�A�Փ˔���
			else
			{
				this->intersectG();
			}
		//	�O�i
			this->move();
		}
		else
		{
			this->iFrame--;
			if(	this->iFrame <= 0)
			{
			//	���ɏ��Ń��b�Z�[�W�^����
				this->setOrderMessage(eOMSG_DELETE);
				this->setVisible(FALSE);
			}
		}
	//	���b�V���`��
		CMESH::draw(inDev);
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_BALL::opeShadollArrow
	//
	//	�@�\		�|��̗l�ɔ��ł�������
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2008/02/20		<�V�K>
	//================================================================================
	void
	CENEMY_BALL::opeShadollArrow(CONST DEV inDev)
	{
	//	���˂���Ă���Ȃ�A������s���A�Փ˔��������
		if(	this->activate)
		{
		//	���݂̕`��t���[�����A
		//	�G�t�F�N�g�̔����Ԋu�t���[���Ŋ���؂ꂽ��A�G�t�F�N�g����
			if(	!(this->iFrame % this->iEffectCnt))
			{
				C3D_EFFECT_CENTER::occuEffectsInstance(
					this->effectName, this->getLoc(), 0.1f);
			}

		//	�G�O���[�v�Փ˔���
			if(	this->player	&&
				(*this->player)->intersectRadius(this))
			{
			//	�v���C���[�ɓ���������A�U�����A�N�V�������b�Z�[�W��n��
				(*this->player)->reactionMessage(this->reactMessage);
			//	������
				this->activate	= FALSE;
				this->iFrame	= 0;
			//	���G�t�F�N�g����
				C3D_EFFECT_CENTER::occuEffectsInstance(
					EFFECT_SMOKE, this->getLoc(), 1.0f, VEC3(0.0f,0.0f,0.0f), D3DCOLOR_RGBA(0,0,0,255));
			}

			else
		//	�X�e�[�W�O���[�v�Փ˔���
			if(	this->stageG	&&
				this->stageG->intersect(this, this->getLine(), this->getMoveQuantity() + 1.0f))
			{
			//	������
				this->activate	= FALSE;
				this->iFrame	= 50;
			}

		//	�`��t���[�������炵�A�t���[����-15�`0�ɂȂ�����A�p�x�ύX
			this->iFrame--;
			if(	(this->iFrame <    0) &&
				(this->iFrame >= -15))
			{
				this->calcAngleX(-6.0f);
			}
		//	200�ȉ��ɂȂ�����A����
			else
			if(	this->iFrame <= -200)
			{
				this->setVisible(FALSE);
				this->activate	= FALSE;
			//	���ɏ��Ń��b�Z�[�W�^����
				this->setOrderMessage(eOMSG_DELETE);
			}
		//	�O�i
			this->move();
		}
		else
		{
		//	���b�V���`��
			this->iFrame--;
			if(	this->iFrame <= 0)
			{
			//	���ɏ��Ń��b�Z�[�W�^����
				this->setOrderMessage(eOMSG_DELETE);
				this->setVisible(FALSE);
			}
		}
		CMESH::draw(inDev);
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_BALL::draw
	//
	//	�@�\		�`��
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2008/02/17		<�V�K>
	//================================================================================
	BOOL
	CENEMY_BALL::draw(CONST DEV inDev)
	{
	//	�{�[���̎�ނŏ�������
		switch(this->ballKind)
		{
		case eENEMYB_STRAIGHT:			this->opeStraight(inDev);			break;
		case eENEMYB_SHADOLL_ARROW:		this->opeShadollArrow(inDev);		break;
		}
		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CENEMY_BALL::draw
	//
	//	�@�\		�`��(�[�x�o�b�t�@�V�F�[�_�g�p)
	//	����		inDepthSha		�[�x�o�b�t�@�V�F�[�_
	//	�X�V		2008/02/17		<�V�K>
	//================================================================================
	BOOL
	CENEMY_BALL::draw(CDEPTH_SHA* inDepthSha)
	{
		return	this->draw(inDepthSha->getDevice());
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------