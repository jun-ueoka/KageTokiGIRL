//***********************************************************************************
//!	�t�@�C����		Class_AdventPlayer.h
//
//	�@�\			�A�h�x���`���[�Q�[�����[�h�̃v���C���[�N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2008/12/11	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_AdventPlayer.h"		//!< �A�h�x���`���[�Q�[���v���C���[

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	CADVENT_PLAYER::CADVENT_PLAYER(CONST DEV inDev, CKAGETOKI_ADVENT_CHARACTER** inTarget,
								   SCENE_SHARE_DATA* inShare)
	{
	//	�J�Q�I�ƃJ�Q�~�̐���
		this->adventKageo		= new CADVENT_KAGEO(inDev);
		this->adventKagemi		= new CADVENT_KAGEMI(inDev);
	//	�����̓J�Q�I�I��
		this->choiceChara		= eADCC_KAGEO;
		this->bChoiceCharaChange= TRUE;
	//	�����̏�����
		this->target			= inTarget;
		this->camera			= NULL;
		this->bDamageCheck		= TRUE;
		this->iMaxLife			= 15;
		this->iLife				= this->iMaxLife;
		this->share				= inShare;
		this->keepStartLoc		= VEC3(0.0f, 0.0f, 0.0f);

	//	�G�t�F�N�g�̓o�^
		C3D_EFFECT_CENTER*	effectCenter	= C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			EFFECT_SMOKE,
			new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEO_COMMENT_ACHE,
			new C3D_EFFECT(inDev, KAGEO_COMMENT_ACHE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEO_COMMENT_ANGER,
			new C3D_EFFECT(inDev, KAGEO_COMMENT_ANGER, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEO_COMMENT_HASTE,
			new C3D_EFFECT(inDev, KAGEO_COMMENT_HASTE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEO_COMMENT_DATHE,
			new C3D_EFFECT(inDev, KAGEO_COMMENT_DATHE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEMI_COMMENT_ACHE,
			new C3D_EFFECT(inDev, KAGEMI_COMMENT_ACHE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEMI_COMMENT_ANGER,
			new C3D_EFFECT(inDev, KAGEMI_COMMENT_ANGER, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEMI_COMMENT_HASTE,
			new C3D_EFFECT(inDev, KAGEMI_COMMENT_HASTE, VEC3(0.0f,0.0f,0.0f)));
		effectCenter->addEffects(
			KAGEMI_COMMENT_DATHE,
			new C3D_EFFECT(inDev, KAGEMI_COMMENT_DATHE, VEC3(0.0f,0.0f,0.0f)));
	//	�����L�����N�^�[��ݒ�
		*this->target			= this->getPlayChara();

	//	�C���^�[�t�F�[�X�֘A�̏�����
	//	�J�Q�I�̃��C�t�x�[�X
		this->texKageoLife
			= new CTEXTURE(inDev, NAME_INTERFACE, VEC2(1.0f, 1.0f), 0xFFFFFF96);
		this->texKageoLife->setToRect(VEC2(256.0f, 256.0f));
		this->texKageoLife->setFromRect(0, 64, 256, 320);
		this->texKageoLife->setLoc(&VEC3(150.0f, 620.0f, 0.5f));
	//	�J�Q�~�̃��C�t�x�[�X
		this->texKagemiLife = new CTEXTURE(texKageoLife);
		this->texKagemiLife->setToRect(VEC2(256.0f, 256.0f));
		this->texKagemiLife->setFromRect(256, 64, 512, 320);
		this->texKagemiLife->setLoc(&VEC3(150.0f, 620.0f, 0.5f));
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::�f�X�g���N�^
	//
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	CADVENT_PLAYER::~CADVENT_PLAYER(void)
	{
	//	�J�Q�I�ƃJ�Q�~�̉��
		SAFE_DELETE(this->adventKageo);
		SAFE_DELETE(this->adventKagemi);
	//	�C���^�[�t�F�[�X�̉��
		SAFE_DELETE(this->texKageoLife);
		SAFE_DELETE(this->texKagemiLife);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::getPlayChara
	//
	//	�@�\		�I�𒆂̃L�����N�^�[�̃|�C���^�[���擾
	//	�߂�l		�I�𒆂̃L�����N�^�[�̃|�C���^�[
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	CKAGETOKI_ADVENT_CHARACTER*
	CADVENT_PLAYER::getPlayChara(void)
	{
	//	�I�𒆂̃L�����N�^�[��Ԃ�
		switch(this->choiceChara)
		{
			case eADCC_KAGEO:	return	this->adventKageo;
			case eADCC_KAGEMI:	return	this->adventKagemi;
		}
		return	NULL;
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::setViewG
	//
	//	�@�\		�`��O���[�v�̊m��
	//	����		inGroup			�`��O���[�v�|�C���^�[
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	void
	CADVENT_PLAYER::setViewG(C3D_OBJECT_GROUP* inGroup)
	{
		this->adventKageo->setViewG(inGroup);
		this->adventKagemi->setViewG(inGroup);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::setStageG
	//
	//	�@�\		�X�e�[�W�O���[�v�̊m��
	//	����		inGroup			�X�e�[�W�O���[�v�|�C���^�[
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	void
	CADVENT_PLAYER::setStageG(C3D_OBJECT_GROUP* inGroup)
	{
		this->adventKageo->setStageG(inGroup);
		this->adventKagemi->setStageG(inGroup);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::setEnemyG
	//
	//	�@�\		�G�O���[�v�̊m��
	//	����		inGroup			�G�O���[�v�|�C���^�[
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CADVENT_PLAYER::setEnemyG(C3D_OBJECT_GROUP* inGroup)
	{
		this->adventKageo->setEnemyG(inGroup);
		this->adventKagemi->setEnemyG(inGroup);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::setAllLoc
	//
	//	�@�\		��ĂɈʒu�ݒ�
	//	����		inLoc			�ݒ肷��ʒu
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	void
	CADVENT_PLAYER::setAllLoc(VEC3* inLoc)
	{
		this->adventKageo->setLoc(inLoc);
		this->adventKagemi->setLoc(inLoc);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::setAllAngle
	//
	//	�@�\		��ĂɊp�x�ݒ�
	//	����		inAngle			�ݒ肷��p�x
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	void
	CADVENT_PLAYER::setAllAngle(VEC3* inAngle)
	{
		this->adventKageo->setAngle(inAngle);
		this->adventKagemi->setAngle(inAngle);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::setAllVisible
	//
	//	�@�\		��Ăɕ`��m�F�ݒ�
	//	����		inVisible		�V���ȕ`��m�F
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	void
	CADVENT_PLAYER::setAllVisible(BOOL inVisible)
	{
		this->adventKageo->setVisible(inVisible);
		this->adventKagemi->setVisible(inVisible);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::setAllLimit
	//
	//	�@�\		��Ăɓ��͗L������ݒ�
	//	����		inLimit			���͗L�����䃌�x��
	//	�X�V		2009/01/24		<�V�K>
	//================================================================================
	void
	CADVENT_PLAYER::setAllLimit(int inLimit)
	{
		this->adventKageo->setLimit((ADVENT_KAGEO_ACTION_LIMIT)inLimit);
		this->adventKagemi->setLimit((ADVENT_KAGEMI_ACTION_LIMIT)inLimit);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::resetAllWay
	//
	//	�@�\		��ĂɌ���������
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	void
	CADVENT_PLAYER::resetAllWay(void)
	{
		this->adventKageo->resetWay();
		this->adventKagemi->resetWay();
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::resetPlayer
	//
	//	�@�\		�v���C���[�̃��Z�b�g
	//	����		inStartLoc		�����ʒu
	//	�X�V		2009/02/05		<�V�K>
	//================================================================================
	void
	CADVENT_PLAYER::resetPlayer(VEC3* inStartLoc)
	{
	//	�����ʒu���ݒ肳��Ă���Ȃ�A�X�V
		if(	inStartLoc)
		{
			this->keepStartLoc	= *inStartLoc;
		}
		this->setAllLoc(&this->keepStartLoc);
		this->setAllVisible(TRUE);
		this->resetAllWay();
		this->setAllAngle(&VEC3(0.0f, ADVENT_RIGHTANGLE, 0.0f));
		this->setChoiceCharacter(eADCC_KAGEO);
		this->iLife	= this->iMaxLife;
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::setChoiceCharacter
	//
	//	�@�\		�I�𒆃L�����N�^�[�̕ύX
	//	����		inChara			�V���ȑI�𒆃L�����N�^�[
	//	�X�V		2009/01/24		<�V�K>
	//================================================================================
	void
	CADVENT_PLAYER::setChoiceCharacter(ADVENT_CHOICE_CHARACTER inChara)
	{
	//	�I�𒆂̃L�����N�^�[�ƐV���ȑI�𒆃L�����N�^�[����v�����ꍇ�A������Ԃ�
		if(	this->choiceChara == inChara)			return;
	//	����Ώۂ��J�Q�~�Ȃ�A���S�ɑҋ@�ɖ߂�
		if(	this->choiceChara == eADCC_KAGEMI)
		{
			this->adventKagemi->safeResetChangeToWait();
		}

	//	�ʒu�Ȃǂ̓���
		this->setAllLoc(&this->getPlayChara()->getLoc());
		this->setAllAngle(&this->getPlayChara()->getAngle());
		this->resetAllWay();

	//	���G�t�F�N�g�̐F�m�ۗp
		D3DCOLOR	color	= 0x00000000;
	//	�L�����N�^�[�`�F���W
		switch(this->choiceChara)
		{
			case eADCC_KAGEO:
				this->choiceChara = eADCC_KAGEMI;
				color = 0xFFFF9696;
				break;
			case eADCC_KAGEMI:
				this->choiceChara = eADCC_KAGEO;
				color = 0xFF9696FF;
				break;
		}
	//	���G�t�F�N�g����
		C3D_EFFECT_CENTER::occuEffectsInstance(
			EFFECT_SMOKE,
			(this->getPlayChara()->getLoc()+VEC3(0.0f,0.0f,-200.0f)),
			3.0f,
			VEC3(0.0f,0.0f,0.0f),
			color);
	//	�Ώۂ̐ݒ�
		*this->target	= this->getPlayChara();
		this->camera->setTarget(*this->target);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::occuPlayerEffect
	//
	//	�@�\		�v���C���[�G�t�F�N�g�̔���
	//	����		inEffectFlag	����������v���C���[�G�t�F�N�g
	//	�X�V		2009/02/27		<�V�K>
	//================================================================================
	void
	CADVENT_PLAYER::occuPlayerEffect(ADVENT_PLAYER_EFFECT inEffectFlag)
	{
	//	�����G�t�F�N�g���m�ۗp
		LPSTR	inEffectName	= NULL;
	//	���݂̑I�𒆃L�����N�^�[�ɂ�蕪��
		switch(this->choiceChara)
		{
		//	�J�Q�I�I��
			case eADCC_KAGEO:
			//	�����Ă����v���C���[�G�t�F�N�g�ɂ���ĕ���	
				switch(inEffectFlag)
				{
					case eADPEFF_ACHE:
						inEffectName = KAGEO_COMMENT_ACHE;
						break;
					case eADPEFF_ANGER:
						inEffectName = KAGEO_COMMENT_ANGER;
						break;
					case eADPEFF_HASTE:
						inEffectName = KAGEO_COMMENT_HASTE;
						break;
					case eADPEFF_DATHE:
						inEffectName = KAGEO_COMMENT_DATHE;
						break;
				}
				break;
		//	�J�Q�~�I��
			case eADCC_KAGEMI:
			//	�����Ă����v���C���[�G�t�F�N�g�ɂ���ĕ���	
				switch(inEffectFlag)
				{
					case eADPEFF_ACHE:
						inEffectName = KAGEMI_COMMENT_ACHE;
						break;
					case eADPEFF_ANGER:
						inEffectName = KAGEMI_COMMENT_ANGER;
						break;
					case eADPEFF_HASTE:
						inEffectName = KAGEMI_COMMENT_HASTE;
						break;
					case eADPEFF_DATHE:
						inEffectName = KAGEMI_COMMENT_DATHE;
						break;
				}
				break;
		}
	//	����������G�t�F�N�g�������݂�����A�G�t�F�N�g����
		if(	inEffectName)
		{
			C3D_EFFECT_CENTER::occuEffectsInstance(
				inEffectName, (*this->target)->getWorldCenter(), 1.0f);
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::operation
	//
	//	�@�\		�v���C���[����
	//	����		input			���͏��
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	void
	CADVENT_PLAYER::operation(CINPUT* input)
	{
	//	�O���|�C���^�[�Q���ݒ肳��Ă��Ȃ���΁A���������Ȃ�
		if(	!(this->target)	||
			!(this->camera))	return;

	//	�_���[�W����
	//	���݂̃L�����N�^�[�Ƀ_���[�W���󂯂��`�Ղ����鎞
		if(	(*this->target)->getDamageBoolen())
		{
		//	�_���[�W���󂯂��`�Ղ������Ă���
			(*this->target)->setDamageBoolen(FALSE);
		//	�_���[�W����m�F���L���̎�
			if(	this->bDamageCheck)
			{
				this->iLife	-= (*this->target)->getDamageValue();
			//	���C�t�ɂ���Đ����o����������
				int		inValue			= this->iMaxLife / 3;
			//	���C�t�ɗ]�T�����鎞
				if(	this->iLife <  this->iMaxLife	&&
					this->iLife >= this->iMaxLife - inValue)
				{
					this->occuPlayerEffect(eADPEFF_ACHE);
				}
			//	���C�t�ɏ��������ė�����
				else
				if(	this->iLife <  this->iMaxLife -  inValue	&&
					this->iLife >= this->iMaxLife - (inValue * 2))
				{
					this->occuPlayerEffect(eADPEFF_ANGER);
				}
			//	���C�t�ɗ]�T�������Ȃ�����
				else
				if(	this->iLife <  this->iMaxLife - (inValue * 2)	&&
					this->iLife >  0)
				{
					this->occuPlayerEffect(eADPEFF_HASTE);
				}
			//	���C�t�������Ȃ�����
				else
				{
					this->occuPlayerEffect(eADPEFF_DATHE);
				}
			}
		}
	//	���C�t��0�ȉ��ɂȂ�����A���������Ȃ�
		if(	this->iLife <= 0)	{ this->iLife = 0;	goto next;	}

	//	�L�����N�^�[�؂�ւ�
		if(	input->checkPressOnce(MYKEY::eHARD)	&&
			this->bChoiceCharaChange)
		{
		//	����ΏۃL�����N�^�[���󒆂Ȃ�A������Ԃ�
			if(	this->getPlayChara()->getFlighting())	goto next;
		//	����Ώۂ��J�Q�~�ŁA
		//	�u�G�i�W�[�{�[�����ˁv���u�G�i�W�[�{�[�����쒆�v�Ȃ�A������Ԃ�
			if(	(this->choiceChara == eADCC_KAGEMI)	&&
			   ((this->adventKagemi->getAction() == eADKAGEMIA_SKILL_ENERGY_BALL_SHOT) ||
			    (this->adventKagemi->getAction() == eADKAGEMIA_SKILL_ENERGY_BALL_OPERATION)))
			{
				goto next;
			}

		//	�ʒu�Ȃǂ̓���
			this->setAllLoc(&this->getPlayChara()->getLoc());
			this->setAllAngle(&this->getPlayChara()->getAngle());
			this->resetAllWay();

		//	���G�t�F�N�g�̐F�m�ۗp
			D3DCOLOR	color	= 0x00000000;
		//	�L�����N�^�[�`�F���W
			switch(this->choiceChara)
			{
				case eADCC_KAGEO:
					this->choiceChara = eADCC_KAGEMI;
					color = 0xFFFF9696;
					break;
				case eADCC_KAGEMI:
					this->choiceChara = eADCC_KAGEO;
					color = 0xFF9696FF;
					break;
			}
		//	���G�t�F�N�g����
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE,
				(this->getPlayChara()->getLoc()+VEC3(0.0f,0.0f,-200.0f)),
				3.0f,
				VEC3(0.0f,0.0f,0.0f),
				color);
		//	�Ώۂ̐ݒ�
			*this->target	= this->getPlayChara();
			this->camera->setTarget(*this->target);
		}

	next:

	//	�J�Q�~��p�̋���
		if(	this->choiceChara == eADCC_KAGEMI)
		{
		//	�J�����Ώۂ̕ύX
		//	�G�i�W�[�{�[�����쒆�̎��́A�G�i�W�[�{�[��������Ώ�
			if(	this->adventKagemi->getOpeEnergyBall())
			{
				this->camera->setTarget(this->adventKagemi->getOpeEnergyBall());
			}
		//	�G�i�W�[�{�[�����쒆�łȂ����́A�J�Q�~������Ώ�
			else
			{
				this->camera->setTarget(this->adventKagemi);
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_PLAYER::drawInterface
	//
	//	�@�\		�C���^�[�t�F�[�X�̕`��
	//	����		inSprite		�X�v���C�g�f�o�C�X
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	void
	CADVENT_PLAYER::drawInterface(SPRITE inSprite)
	{
	//	�I�𒆂̃L�����N�^�[����
		switch(this->choiceChara)
		{
			case eADCC_KAGEO:	this->texKageoLife->draw(inSprite);		break;
			case eADCC_KAGEMI:	this->texKagemiLife->draw(inSprite);	break;
		}

	//	���C�t�x�[�X�̕\��
		RECT	rcFrom	= {  0,320,224,352};
		RECT	rcTo	= { 35,679,259,711};
		this->texKageoLife->drawTexture(inSprite, rcFrom, rcTo);
	//	���C�t�̕\��
		SetRect(&rcFrom,  14,357,209,378);
		SetRect(&rcTo,    50,689,243,701);
		rcTo.right	=
			rcTo.left + (int)(((float)(rcTo.right - rcTo.left) / this->iMaxLife) * (float)this->iLife);
		this->texKageoLife->drawTexture(inSprite, rcFrom, rcTo);
	//	���C�t�g�̕\��
		SetRect(&rcFrom,   0,384,224,416);
		SetRect(&rcTo,    35,679,259,711);
		this->texKageoLife->drawTexture(inSprite, rcFrom, rcTo);

	//	�c�C�̕`��
		SetRect(&rcFrom, 192,  0,256, 64);
		for(DWORD i = 0; i < this->share->dwPlayerLife; i++)
		{
			SetRect(&rcTo, 30+(30*i), 640, 30+50+(30*i), 640+50);
			this->texKagemiLife->drawTexture(inSprite, rcFrom, rcTo);
		}
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------