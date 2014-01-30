//***********************************************************************************
//!	�t�@�C����		Class_YamakageWeapon.cpp
//
//	�@�\			���}�J�Q�����镺��N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2009/02/17	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CYAMAKAGE_WEAPON::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inKind			�{�[���̎��
	//	�X�V		2009/02/17		<�V�K>
	//================================================================================
	CYAMAKAGE_WEAPON::CYAMAKAGE_WEAPON(CONST DEV inDev)	:
	CSKIN_MESH(inDev, NAME_YAMAKAGE_WEAPON)
	{
	//	�����̏�����
		this->player			= NULL;
		this->viewG				= NULL;
		this->stageG			= NULL;
		this->weaponAction		= eOPEYW_MAX;
		this->iSmokeFrame		= 0;
		this->iMoveFrame		= 0;
		this->targetLoc			= VEC3(0.0f, 0.0f, 0.0f);
		this->ssEnemyBallShot	= new SSTATIC("shot_shadow");

	//	�I���W�i���u�G�l�~�[�{�[���v�̐���
		this->originEnemyBall
			= new CENEMY_BALL(inDev, eENEMYB_STRAIGHT, "shadow_ball", EFFECT_SHADOW);
		this->originEnemyBall->setEffectColor(D3DCOLOR_RGBA(100,100,100,255));
		this->originEnemyBall->setReactMessage(eREAMSG_ENEMY_ATTACK_LV1);

	//	�G�t�F�N�g�̓o�^
		C3D_EFFECT_CENTER*	effectCenter = C3D_EFFECT_CENTER::getInstance();
		effectCenter->addEffects(
			EFFECT_SMOKE, new C3D_EFFECT(inDev, EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));

	//	���V���ɕύX
		this->setWeaponAction(eOPEYW_FLOATING);
	//	�A�j���[�V�����̏�����
		this->changeAnimationAndBlend(0);
	//	�I�u�W�F�N�g�̎�ނ�������e�ɕύX
		this->setThisMessage(eTMSG_SEE_SHADOW);
	}

	//================================================================================
	//!	���\�b�h��	CYAMAKAGE_WEAPON::�f�X�g���N�^
	//
	//	�X�V		2009/02/17		<�V�K>
	//================================================================================
	CYAMAKAGE_WEAPON::~CYAMAKAGE_WEAPON(void)
	{
		SAFE_DELETE(this->originEnemyBall);
		SAFE_DELETE(this->ssEnemyBallShot);
	}

	//================================================================================
	//!	���\�b�h��	CYAMAKAGE_WEAPON::setWeaponAction
	//
	//	�@�\		���쐧��̐ݒ�
	//	����		inAction		�V���ȓ��쐧�䖽��
	//	�X�V		2009/02/17		<�V�K>
	//================================================================================
	void
	CYAMAKAGE_WEAPON::setWeaponAction(YAMAKAGE_WEAPON_ACTION inAction)
	{
	//	���݂��Ȃ���΁A�ύX�o���Ȃ�
		if(	!this->getVisible())			return;
	//	�V���ȓ��쐧��̖��߂ƌ��݂̓��쐧�䂪�������Ȃ�A���������Ȃ�
		if(	this->weaponAction == inAction)	return;
	//	�V���ȓ��쐧���ݒ�
		this->weaponAction	= inAction;

		this->iMoveFrame	= 0;
	//	�V���ȓ��쐧��ɂ���ĕ���
		switch(this->weaponAction)
		{
			case eOPEYW_FLOATING:
				this->iMoveFrame	= 29;
				break;
			case eOPEYW_ATTACK:
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CYAMAKAGE_WEAPON::reactionMessage
	//
	//	�@�\		���A�N�V�����Ή�
	//	����		inMessage		���A�N�V�������b�Z�[�W
	//	�X�V		2009/02/17		<�V�K>
	//================================================================================
	void
	CYAMAKAGE_WEAPON::reactionMessage(REACTION_MESSAGE inMessage)
	{
	//	�V���C���{�[���ɓ���������A���ݏ���
		if(	inMessage == eREAMSG_SHINE_HIT)
		{
			this->setVisible(FALSE);
		}
	}

	//================================================================================
	//!	���\�b�h��	CYAMAKAGE_WEAPON::operation
	//
	//	�@�\		��{����
	//	�X�V		2009/02/17		<�V�K>
	//================================================================================
	void
	CYAMAKAGE_WEAPON::operation(void)
	{
	//	�G���[�Ώ�
		if(	!this->player	||
			!this->getVisible())	return;

	//	���݂̕`��t���[�����A
	//	�G�t�F�N�g�̔����Ԋu�t���[���Ŋ���؂ꂽ��A�G�t�F�N�g����
		this->iSmokeFrame++;
		if(	!(this->iSmokeFrame % 10))
		{
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE, this->getLoc(), 0.2f,
				VEC3(0.0f, 0.0f, 0.0f), this->smokeColor.color);
			this->iSmokeFrame	= 0;
		}

	//	���쐧��ɂ���ĕ���
		switch(this->weaponAction)
		{
		//	���V���
			case eOPEYW_FLOATING:
				this->iMoveFrame++;
			//	�ŏ���30�t���[���ŁA�ړ��Ώۈʒu���X�V����
				if(	this->iMoveFrame >= 30 && this->iMoveFrame <= 30)
				{
					this->targetLoc		 = (*this->player)->getLoc();
					this->targetLoc.y	+= 200.0f;
				//	�����_���Ɉʒu���w�肵�A�v���C���[��ǂ�������
					this->targetLoc.y	+= float(rand() % 1000 + 1000);
					this->targetLoc.x	+= float(rand() % 4000 - 2000);
				}
			//	31�`50�t���[���ŁA�ړ��Ώۈʒu�܂ňړ�����
				else if(this->iMoveFrame > 30 && this->iMoveFrame < 50)
				{
				//	1�t���[���ňړ�����x�N�g��������o���A�ړ�������
					this->calcLoc(&((this->targetLoc - this->getLoc()) / 20.0f));
				}
			//	50�t���[���ɓ��B������A�t���[����������
				else if(this->iMoveFrame >= 50)
				{
					this->iMoveFrame  = 0;
				}
				break;
		//	�U��
			case eOPEYW_ATTACK:
				this->iMoveFrame++;
			//	50�t���[���܂ŁA�`���[�W
				if(	this->iMoveFrame < 50)
				{
					this->smokeColor.rgba.red	-= 6;
					this->smokeColor.rgba.green	-= 6;
					this->smokeColor.rgba.blue	-= 6;
				}
			//	50�t���[���ȏ�ŁA�G�l�~�[�{�[���𔭎�
				else
				{
				//	�G�l�~�[�{�[���𐶐�
					CENEMY_BALL*	newBall;
					newBall	= new CENEMY_BALL(this->originEnemyBall);
				//	�{�[���̊p�x���v�Z
					float	X, Y, dir;
					X	= this->getLocX() - (*this->player)->getLocX();
					Y	= this->getLocY() - (*this->player)->getLocY();
					dir	= atan2(Y, X) * 180.0f / D3DX_PI;
				//	�G�l�~�[�{�[���̐ݒ�
					newBall->shot(
						&VEC3(0.0f, 90.0f, dir),						//!< �p�x
						&this->getWorldCenter(),						//!< ���ˈʒu
						NULL,											//!< �X�e�[�W�I�u�W�F�N�g�O���[�v
						this->player,									//!< �G�I�u�W�F�N�g�O���[�v
						300);											//!< ���Ŏ���
					newBall->setMoveQuantity(20.0f);					//!< �X�s�[�h�ݒ�
					newBall->setScale(1.0f);							//!< �g��
					newBall->setLine(ADVENT_FRONTHIT);					//!< ����(3D��Ԃɑ΂���)
					newBall->setOrderMessage(eOMSG_DELETE_IN);			//!< �����
				//	�G�l�~�[�{�[�����ˉ�
					this->ssEnemyBallShot->setPlayCur(0);
					this->ssEnemyBallShot->play(0, NULL);
				//	�G�l�~�[�{�[����`��O���[�v�ɓo�^
					this->viewG->add(newBall);
				//	���쐧��𕂗V�ɕύX
					this->setWeaponAction(eOPEYW_FLOATING);
				}
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CYAMAKAGE_WEAPON::draw
	//
	//	�@�\		�`��
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2009/02/17		<�V�K>
	//================================================================================
	BOOL
	CYAMAKAGE_WEAPON::draw(CONST DEV inDev)
	{
	//	�ʏ�`��
		return	CSKIN_MESH::draw(inDev);
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------