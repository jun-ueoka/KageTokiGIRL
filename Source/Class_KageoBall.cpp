//***********************************************************************************
//!	�t�@�C����		Class_KageoBall.cpp
//
//	�@�\			�J�Q�I�����A�ʂ������N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2007/12/08	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_KageoBall.h"
//#include	"Class_3DEffectCenter.h"	//!< �G�t�F�N�g�Z���^�[(�G�t�F�N�g�𔭐��������)

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CKAGEO_BALL::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inKind			�{�[���̎��
	//	�X�V		2008/08/26		<�V�K>
	//================================================================================
	CKAGEO_BALL::CKAGEO_BALL(CONST DEV		inDev,
							KAGEO_BALL_KIND	inKind)	:
	CMESH(inDev, getBallTextureName(inKind))
	{
	//	�����̏�����
		this->activate		= FALSE;
		this->hitG			= NULL;
		this->iFrame		= 0;
		this->sStaticHit	= NULL;

	//	�{�[���̎�ނɂ��A��������������
		switch(inKind)
		{
		//	�V���h�E�{�[��
			case	eKOBALL_SHADOW:
			//	�����Ȃ��e���A������e�ɂ��郁�b�Z�[�W
				this->judgMessage	= eTMSG_NO_SEE_SHADOW;
				this->reactMessage	= eREAMSG_SHADOW_HIT;
				this->setThisMessage(eTMSG_SHADOW_BALL);
			//	�G�t�F�N�g�֘A
				this->iEffectCnt	= 3;
				strcpy_s(this->effectName, sizeof(effectName), EFFECT_SHADOW);
			//	���̐F
				this->smokeColor	= D3DCOLOR_RGBA(10, 10, 10, 255);
			//	���𐶐�
				this->sStaticHit	= new SSTATIC(SOUND_SHADOWB_HIT);
				break;

		//	�V���C���{�[��
			case	eKOBALL_SHINE:
			//	������e���A�����Ȃ��e�ɂ��郁�b�Z�[�W
				this->judgMessage	= eTMSG_SEE_SHADOW;
				this->reactMessage	= eREAMSG_SHINE_HIT;
				this->setThisMessage(eTMSG_SHINE_BALL);
			//	�G�t�F�N�g�֘A
				this->iEffectCnt	= 3;
				strcpy_s(this->effectName, sizeof(effectName), EFFECT_SHINE);
			//	���𐶐�
				this->sStaticHit	= new SSTATIC(SOUND_SHINEB_HIT);
			//	���̐F
				this->smokeColor	= D3DCOLOR_RGBA(255, 255, 200, 255);
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
	//!	���\�b�h��	CKAGEO_BALL::�R���X�g���N�^
	//
	//	�@�\		�R�s�[
	//	����		inCopy			�R�s�[��
	//	�X�V		2008/08/26		<�V�K>
	//================================================================================
	CKAGEO_BALL::CKAGEO_BALL(CKAGEO_BALL *inCopy)
	:	CMESH(inCopy)
	{
	//	�����̏�����
		this->activate		= FALSE;
		this->judgMessage	= inCopy->getJudgMessage();
		this->reactMessage	= inCopy->getReactMessage();
		this->setThisMessage(inCopy->getThisMessage());
		this->hitG			= NULL;
		strcpy_s(this->effectName, sizeof(this->effectName), inCopy->getEffectName());
		this->iEffectCnt	= inCopy->getEffectCnt();
		this->iFrame		= 0;
		this->smokeColor	= inCopy->getSmokeColor();

	//	���̐���
		LPSTR	inHitSoundName	= inCopy->getHitSoundName();
		if(	inHitSoundName)	{	this->sStaticHit = new SSTATIC(inHitSoundName);	}
		else				{	this->sStaticHit = NULL;						}
	}

	//================================================================================
	//!	���\�b�h��	CKAGEO_BALL::�f�X�g���N�^
	//
	//	�X�V		2008/08/26		<�V�K>
	//================================================================================
	CKAGEO_BALL::~CKAGEO_BALL(void)
	{
	}

	//================================================================================
	//!	���\�b�h��	CKAGEO_BALL::getBallTextureName
	//
	//	�@�\		�J�Q�I�{�[���̎�ނɂŁA�e�N�X�`�����̌���
	//	����		inPtn		�R�����g�p�^�[��
	//	�߂�l		�e�N�X�`����
	//	�X�V		2008/09/23	<�V�K>
	//================================================================================
	LPSTR
	CKAGEO_BALL::getBallTextureName(KAGEO_BALL_KIND inKind)
	{
	//	�p�^�[������
		switch(inKind)
		{
			case	eKOBALL_SHADOW:	return	SHADOW_BALL;		break;
			case	eKOBALL_SHINE:	return	SHINE_BALL;			break;
			default:				return	NULL;				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CKAGEO_BALL::shot
	//
	//	�@�\		����(����������Ɏg�p)
	//	����		*inAngle		���˕���
	//				*inRotX			X���̉�]�}�g���b�N�X
	//				*inRotY			Y���̉�]�}�g���b�N�X
	//				*inRotZ			Z���̉�]�}�g���b�N�X
	//				*inLoc			���˒n�_
	//				*inHitG			�Փ˔���O���[�v
	//				inFrame			�`��t���[��
	//	�X�V		2008/08/26		<�V�K>
	//================================================================================
	void
	CKAGEO_BALL::shot(VEC3* inAngle, VEC3* inLoc, C3D_OBJECT_GROUP* inHitG, int inFrame)
	{
	//	���ˉ\�̎�
		if(	!this->activate)
		{
		//	����
			this->hitG		= inHitG;
			this->activate	= TRUE;
			this->iFrame	= inFrame;
			this->setVisible(TRUE);
			this->setAngle(inAngle);
			this->setLoc(inLoc);
		}
	}

	//================================================================================
	//!	���\�b�h��	CKAGEO_BALL::intersectG
	//
	//	�@�\		�Փ˔���
	//	�X�V		2008/08/26		<�V�K>
	//================================================================================
	void
	CKAGEO_BALL::intersectG(void)
	{
	//	�Փ˔���O���[�v���ݒ肳��Ă��Ȃ��Ȃ�A���������Ȃ�
		if(	!this->hitG)	return;

	//	�Փ˔���
		if(	this->hitG->intersect(this, this->getLine()))
		{
		//	�����Ȃ��e�I�u�W�F�N�g�Ȃ�A�V���h�E�{�[���ƏՓ˂����Ӗ��̃��b�Z�[�W��^����
			if(	this->hitG->getPrevHitObject()->getThisMessage() == this->judgMessage)
			{
				this->hitG->getPrevHitObject()->reactionMessage(this->reactMessage);
			}
		//	������
			this->setVisible(FALSE);
			this->activate	= FALSE;
		//	�Փˉ�����
			this->soundHit();
		//	���G�t�F�N�g����
			C3D_EFFECT_CENTER::occuEffectsInstance(
				"smoke", this->getLoc(), 2.0f, VEC3(0.0f,0.0f,0.0f), this->smokeColor);
		//	���ɏ��Ń��b�Z�[�W�^����
			this->setOrderMessage(eOMSG_DELETE);
		}
	}

	//================================================================================
	//!	���\�b�h��	CKAGEO_BALL::draw
	//
	//	�@�\		�`��
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2008/08/26		<�V�K>
	//================================================================================
	BOOL
	CKAGEO_BALL::draw(CONST DEV inDev)
	{
	//	���˂���Ă���Ȃ�A������s���A�Փ˔��������
		if(	this->activate)
		{
		//	�O�i
			this->move();

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
		}

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CKAGEO_BALL::draw
	//
	//	�@�\		�`��(�[�x�o�b�t�@�V�F�[�_�g�p)
	//	����		inDepthSha		�[�x�o�b�t�@�V�F�[�_
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	BOOL
	CKAGEO_BALL::draw(CDEPTH_SHA* inDepthSha)
	{
		return	this->draw(inDepthSha->getDevice());
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------