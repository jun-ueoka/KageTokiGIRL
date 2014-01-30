//***********************************************************************************
//!	�t�@�C����		Class_KagemiBall.cpp
//
//	�@�\			�J�Q�~�����A�ʂ������N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/12/11		<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_KagemiBall.h"
//#include	"Class_3DEffectCenter.h"	//!< �G�t�F�N�g�Z���^�[(�G�t�F�N�g�𔭐��������)

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CKAGEMI_BALL::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inKind			�{�[���̎��
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	CKAGEMI_BALL::CKAGEMI_BALL(CONST DEV inDev, KAGEMI_BALL_KIND inKind)	:
	CMESH(inDev, getBallTextureName(inKind))
	{
	//	�����̏�����
		this->activate	= FALSE;
		this->stageG	= NULL;
		this->enemyG	= NULL;
		this->iFrame	= 0;

	//	�{�[���̎�ނɂ��A��������������
		switch(inKind)
		{
		//	�V���h�E�{�[��
			case	eKMIBALL_ENERGY:
			//	�G�Ƀ_���[�W��^���郁�b�Z�[�W
				this->judgMessage	= eTMSG_ENEMY;
				this->reactMessage	= eREAMSG_ENERGY_HIT;
				this->setThisMessage(eTMSG_ENERGY_BALL);
			//	�G�t�F�N�g�֘A
				this->iEffectCnt	= 3;
				strcpy_s(this->effectName, sizeof(effectName), EFFECT_ENERGY);
			//	���̐F
				this->smokeColor	= D3DCOLOR_RGBA(255, 200, 200, 255);
				break;

		//	��L�ȊO(�G���[���l)
			default:
				this->judgMessage	= eTMSG_NOMSG;
				this->reactMessage	= eREAMSG_NOMSG;
				this->setThisMessage(eTMSG_NOMSG);
				this->iEffectCnt	= 100000;
				strcpy_s(this->effectName, sizeof(effectName), "");
				this->smokeColor	= D3DCOLOR_RGBA(255, 255, 255, 255);
				return;
		}

	//	�G�t�F�N�g�̓o�^
		C3D_EFFECT_CENTER*	effectCenter = C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			this->effectName,
			new C3D_EFFECT(inDev, this->effectName, VEC3(0.0f,0.0f,0.0f)));
	}

	//================================================================================
	//!	���\�b�h��	CKAGEMI_BALL::�R���X�g���N�^
	//
	//	�@�\		�R�s�[
	//	����		inCopy			�R�s�[��
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	CKAGEMI_BALL::CKAGEMI_BALL(CKAGEMI_BALL *inCopy)
	:	CMESH(inCopy)
	{
	//	�����̏�����
		this->activate		= FALSE;
		this->judgMessage	= inCopy->getJudgMessage();
		this->reactMessage	= inCopy->getReactMessage();
		this->setThisMessage(inCopy->getThisMessage());
		this->stageG		= NULL;
		this->enemyG		= NULL;
		strcpy_s(this->effectName, sizeof(this->effectName), inCopy->getEffectName());
		this->iEffectCnt	= inCopy->getEffectCnt();
		this->iFrame		= 0;
		this->smokeColor	= inCopy->getSmokeColor();
	}

	//================================================================================
	//!	���\�b�h��	CKAGEMI_BALL::�f�X�g���N�^
	//
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	CKAGEMI_BALL::~CKAGEMI_BALL(void)
	{
	}

	//================================================================================
	//!	���\�b�h��	CKAGEMI_BALL::getBallTextureName
	//
	//	�@�\		�J�Q�~�{�[���̎�ނɂŁA�e�N�X�`�����̌���
	//	����		inPtn		�R�����g�p�^�[��
	//	�߂�l		�e�N�X�`����
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	LPSTR
	CKAGEMI_BALL::getBallTextureName(KAGEMI_BALL_KIND inKind)
	{
	//	�p�^�[������
		switch(inKind)
		{
			case	eKMIBALL_ENERGY:	return	ENERGY_BALL;		break;
			default:					return	NULL;				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CKAGEMI_BALL::shot
	//
	//	�@�\		����(����������Ɏg�p)
	//	����		*inAngle		���˕���
	//				*inRotX			X���̉�]�}�g���b�N�X
	//				*inRotY			Y���̉�]�}�g���b�N�X
	//				*inRotZ			Z���̉�]�}�g���b�N�X
	//				*inLoc			���˒n�_
	//				*inHitG			�Փ˔���O���[�v
	//				inFrame			�`��t���[��
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	void
	CKAGEMI_BALL::shot(VEC3* inAngle, VEC3* inLoc, C3D_OBJECT_GROUP* inStageG,
					   C3D_OBJECT_GROUP* inEnemyG, int inFrame)
	{
	//	���ˉ\�̎�
		if(	!this->activate)
		{
		//	����
			this->stageG	= inStageG;
			this->enemyG	= inEnemyG;
			this->activate	= TRUE;
			this->iFrame	= inFrame;
			this->setVisible(TRUE);
			this->setAngle(inAngle);
			this->setLoc(inLoc);
		}
	}

	//================================================================================
	//!	���\�b�h��	CKAGEMI_BALL::intersectG
	//
	//	�@�\		�Փ˔���
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	void
	CKAGEMI_BALL::intersectG(void)
	{
	//	�G�O���[�v�Փ˔���
		if(	this->enemyG	&&
			this->enemyG->intersect(this, this->getLine(), this->getMoveQuantity() + 1.0f))
		{
		//	�G�ɓ���������A�_���[�W��^����
			if(	this->enemyG->getPrevHitObject()->getThisMessage() == this->judgMessage)
			{
				this->enemyG->getPrevHitObject()->reactionMessage(this->reactMessage);
			}
		//	������
			this->activate	= FALSE;
			this->iFrame	= 30;
		//	���G�t�F�N�g����
			C3D_EFFECT_CENTER::occuEffectsInstance(
				"smoke", this->getLoc(), 2.0f, VEC3(0.0f,0.0f,0.0f), this->smokeColor);
		}

		else
	//	�X�e�[�W�O���[�v�Փ˔���
		if(	this->stageG	&&
			this->stageG->intersect(this, this->getLine(), this->getMoveQuantity() + 1.0f))
		{
		//	������
			this->activate	= FALSE;
			this->iFrame	= 30;
		//	���G�t�F�N�g����
			C3D_EFFECT_CENTER::occuEffectsInstance(
				"smoke", this->getLoc(), 2.0f, VEC3(0.0f,0.0f,0.0f), this->smokeColor);
		}
	}

	//================================================================================
	//!	���\�b�h��	CKAGEMI_BALL::draw
	//
	//	�@�\		�`��
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	BOOL
	CKAGEMI_BALL::draw(CONST DEV inDev)
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
			return	TRUE;
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
			return	TRUE;
		}
	}

	//================================================================================
	//!	���\�b�h��	CKAGEMI_BALL::draw
	//
	//	�@�\		�`��(�[�x�o�b�t�@�V�F�[�_�g�p)
	//	����		inDepthSha		�[�x�o�b�t�@�V�F�[�_
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	BOOL
	CKAGEMI_BALL::draw(CDEPTH_SHA* inDepthSha)
	{
		return	this->draw(inDepthSha->getDevice());
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------