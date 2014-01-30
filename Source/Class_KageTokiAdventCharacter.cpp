//***********************************************************************************
//!	�t�@�C����		Class_KageTokiAdventCharacter.cpp
//
//	�@�\			�A�h�x���`���[�Q�[�����[�h�̃L�����N�^�[�N���X
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2008/12/09	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_KageTokiAdventCharacter.h"		//!< �A�h�x���`���[�Q�[�����[�h�L�����N�^�[

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_CHARACTER::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		�X�L�����b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2008/12/09		<�V�K>
	//================================================================================
	CKAGETOKI_ADVENT_CHARACTER::CKAGETOKI_ADVENT_CHARACTER(
		CONST DEV	inDev,
		CONST LPSTR	inName,
		CONST LPSTR	inResource,
		CONST LPSTR	inTexture)	:
	CSKIN_MESH(inDev, inName, inResource, inTexture)
	{
	//	�����̏�����
		this->bFlighting			= true;					//!< �󒆊m�F
		this->bHeightValid			= false;				//!< �����L���m�F
		this->bWidthValid			= false;				//!< �����L���m�F

		this->fUpDownSpeed			=   0.0f;				//!< �㉺�ړ����x

		this->fMaxUpSpeed			=  10.0f;				//!< �ő�㏸���x
		this->fMaxDownSpeed			= -50.0f;				//!< �ő剺�~���x
		this->fUpDownAccelerate		=   2.0f;				//!< �㉺�ړ������l
		this->fUpDownSlowDown		=   0.5f;				//!< �㉺�ړ������l

		this->fFloorSpace			=   5.0f;				//!< ���Ƃ̊Ԋu
		this->fWallSpace			=   5.0f;				//!< �ǂƂ̊Ԋu
		this->fHeightValue			=   0.0f;				//!< ����
		this->fWidthValue			=   0.0f;				//!< ����

		this->UDLine				= LINE();				//!< ����Ώۂ̏㉺�������
		this->LRLine				= LINE();				//!< ����Ώۂ̍��E�������
		this->FRLine				= LINE();				//!< ����Ώۂ̑O�㔻�����

		this->way					= eADCW_NO;				//!< �L�����N�^�[�̌���
		this->iFrameCnt				= 0;					//!< �t���[���J�E���^

		this->fJumpHeight			= ADVENT_JUMP_HEIGHT;	//!< �W�����v�̍���
		this->fLeftRightMoveSpeed	= 0.0f;					//!< ���E�ړ��X�s�[�h
		this->fLeftRightMoveMaxSpeed= 20.0f;				//!< ���E�ړ��ő�X�s�[�h
		this->fLeftRightMoveAccele	= 2.0f;					//!< ���E�ړ������l
		this->bDamage				= FALSE;				//!< �_���[�W�m�F
		this->iDamage				= 0;					//!< �_���[�W�l

		this->viewG					= NULL;					//!< �`��O���[�v
		this->stageG				= NULL;					//!< �Փ˔���O���[�v
		this->enemyG				= NULL;					//!< �G�O���[�v

	//	���̑��̐ݒ�
		this->setRpmY(20.0f);								//!< ��]���x
		this->setAngleY(ADVENT_RIGHTANGLE);					//!< �����͉E����
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_CHARACTER::�f�X�g���N�^
	//
	//	�X�V		2008/12/09		<�V�K>
	//================================================================================
	CKAGETOKI_ADVENT_CHARACTER::~CKAGETOKI_ADVENT_CHARACTER(void)
	{
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_CHARACTER::moveJumping
	//
	//	�@�\		��􏈗�
	//	����		inValue			��􎞂̉����l
	//	�X�V		2008/12/09		<�V�K>
	//================================================================================
	void
	CKAGETOKI_ADVENT_CHARACTER::moveJumping(float inValue)
	{
	//	���ł鎞�́A���ł��Ȃ�
		if(	!this->bFlighting)
		{
		//	��􎞂̉����l��ݒ�
			this->fUpDownSpeed	= inValue;
		//	�󒆂ɂ���
			this->bFlighting	= true;
		}
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_CHARACTER::opeAdvent
	//
	//	�@�\		�A�h�x���`���[�Q�[������
	//	����		input			���͏��
	//	�X�V		2008/12/09		<�V�K>
	//================================================================================
	void
	CKAGETOKI_ADVENT_CHARACTER::opeAdvent(CINPUT* input)
	{
	//	�����̐���
		switch(this->way)
		{
		//	���������r��
			case eADCW_LEFT_TURN:
			//	���Ɍ�������A���Ɍ�������Ȃ�A�l���C��
				if(	(this->getAngleY() - this->getRpmY()) < ADVENT_LEFTANGLE)
				{
					this->setAngleY(ADVENT_LEFTANGLE);
					this->way = eADCW_LEFT;
				}
			//	���Ɍ�������Ȃ��Ȃ�A��������
				else
				{
					this->rotateLeft();
				}
				break;

		//	�E�������r��
			case eADCW_RIGHT_TURN:
			//	�E�Ɍ�������A�E�Ɍ�������Ȃ�A�l���C��
				if(	(this->getAngleY() + this->getRpmY()) > ADVENT_RIGHTANGLE)
				{
					this->setAngleY(ADVENT_RIGHTANGLE);
					this->way = eADCW_RIGHT;
				}
			//	�E�Ɍ�������Ȃ��Ȃ�A�E������
				else
				{
					this->rotateRight();
				}
				break;

		//	�������
			case eADCW_NO:
			//	�����̌����ɂȂ��Ă���Ȃ�A�������ɐݒ肷��
				if(	(this->getAngleY() >=   0.0f) &&
					(this->getAngleY() <  180.0f))
				{
					this->way = eADCW_LEFT_TURN;
				}
			//	��L�ȊO�́A�E�����ɐݒ肷��
				else
				{
					this->way = eADCW_RIGHT_TURN;
				}
				break;

		//	��L�ȊO�́A���������Ȃ�
			default:	
				break;
		}

	//	�ړ��̐���
		switch(this->way)
		{
		//	���������Ă���
			case eADCW_LEFT:
			//	����������Ă��鎞
				if(	input->checkPress(ADVENT_LEFT))
				{
				//	����
					if((this->fLeftRightMoveSpeed +  this->fLeftRightMoveAccele) <
						this->fLeftRightMoveMaxSpeed)
					{
						this->fLeftRightMoveSpeed += this->fLeftRightMoveAccele;
						this->calcAnimSpeed(0.002f);
					}
				//	�Փ˔���(�O�ɐi�ލĂ͏�����Ԃ̒��ł́A�E�Ɉړ�����)
					if(	!this->stageG->intersect(
							this,
							ADVENT_FRONTHIT,
							(this->fLeftRightMoveSpeed + this->fWallSpace)))
					{
					//	�󒆂ɂ�����\�z�O���΂߉��𒲂ׂ�
						if(this->bFlighting)
						{
							if(	!this->stageG->intersect(
									this,
									&LINE(eUD_DOWN, eLR_NO, ADVENT_FRONTHIT),
									(this->fLeftRightMoveSpeed + this->fWallSpace)))
							{
							//	�Փ˂��Ȃ�������A�i�܂���
								this->setMoveQuantity(this->fLeftRightMoveSpeed);
								ADVENT_FRONTMOVE;
							}
						}
						else
						{
						//	�Փ˂��Ȃ�������A�i�܂���
							this->setMoveQuantity(this->fLeftRightMoveSpeed);
							ADVENT_FRONTMOVE;
						}
					}
				//	�Փ˂��Ă��āA�󒆂ɋ�����ʒu�𒲐�
					else if(this->bFlighting)
					{
					//	�ʒu�𒲐߂���
						this->setMoveQuantity(
							((this->fLeftRightMoveSpeed + this->fWallSpace) - this->getDist()) - 1.0f);
						if(	!this->stageG->intersect(this, ADVENT_REARHIT))
						{
							ADVENT_REALMOVE;
						}
					}
				//	�Փ˂��Ă��āA�󒆂ɂ��Ȃ��A�����ړ��o����]�n���L�鎞
					else if(this->getDist() > 10.0f)
					{
					//	�Փ˂��Ȃ�������A�i�܂���
						this->setMoveQuantity(5.0f);
						ADVENT_FRONTMOVE;
					}
				}
			//	�E��������Ă��鎞
				else if(input->checkPress(ADVENT_RIGHT))
				{
				//	�E������
					this->fLeftRightMoveSpeed	= 0.0f;
					this->setAnimSpeed();
					this->way			= eADCW_RIGHT_TURN;
				}
			//	�ǂ����������ĂȂ���
				else
				{
				//	��~
					this->fLeftRightMoveSpeed	= 0.0f;
					this->setAnimSpeed();
					this->opeLocShift();
				}
				break;

		//	�E�������Ă���
			case eADCW_RIGHT:
			//	�E��������Ă��鎞
				if(	input->checkPress(ADVENT_RIGHT))
				{
				//	����
					if((this->fLeftRightMoveSpeed +  this->fLeftRightMoveAccele) <
						this->fLeftRightMoveMaxSpeed)
					{
						this->fLeftRightMoveSpeed += this->fLeftRightMoveAccele;
						this->calcAnimSpeed(0.002f);
					}
				//	�Փ˔���(�O�ɐi�ލĂ͏�����Ԃ̒��ł́A���Ɉړ�����)
					if(	!this->stageG->intersect(
							this,
							ADVENT_FRONTHIT,
							(this->fLeftRightMoveSpeed + this->fWallSpace)))
					{
					//	�󒆂ɂ�����\�z�O���΂߉��𒲂ׂ�
						if(this->bFlighting)
						{
							if(	!this->stageG->intersect(
									this,
									&LINE(eUD_DOWN, eLR_NO, ADVENT_FRONTHIT),
									(this->fLeftRightMoveSpeed + this->fWallSpace)))
							{
							//	�Փ˂��Ȃ�������A�i�܂���
								this->setMoveQuantity(this->fLeftRightMoveSpeed);
								ADVENT_FRONTMOVE;
							}
						}
						else
						{
						//	�Փ˂��Ȃ�������A�i�܂���
							this->setMoveQuantity(this->fLeftRightMoveSpeed);
							ADVENT_FRONTMOVE;
						}
					}
				//	�Փ˂��Ă��āA�󒆂ɂ�����ʒu�𒲐�
					else if(this->bFlighting)
					{
					//	�ʒu�𒲐߂���
						this->setMoveQuantity(
							((this->fLeftRightMoveSpeed + this->fWallSpace) - this->getDist()) - 1.0f);
						if(	!this->stageG->intersect(this, ADVENT_REARHIT))
						{
							ADVENT_REALMOVE;
						}
					}
				//	�Փ˂��Ă��āA�󒆂ɂ��Ȃ��A�����ړ��o����]�n���L�鎞
					else if(this->getDist() > 10.0f)
					{
					//	�Փ˂��Ȃ�������A�i�܂���
						this->setMoveQuantity(5.0f);
						ADVENT_FRONTMOVE;
					}
				}
			//	����������Ă��鎞
				else if(input->checkPress(ADVENT_LEFT))
				{
				//	��������
					this->fLeftRightMoveSpeed	= 0.0f;
					this->setAnimSpeed();
					this->way			= eADCW_LEFT_TURN;
				}
			//	�ǂ����������ĂȂ���
				else
				{
				//	��~
					this->fLeftRightMoveSpeed	= 0.0f;
					this->setAnimSpeed();
					this->opeLocShift();
				}
				break;

		//	��L�ȊO�͏��������Ȃ�
			default:	
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_CHARACTER::opeEffectGravity
	//
	//	�@�\		�A�h�x���`���[�Q�[������
	//	����		input			���͏��
	//	�X�V		2008/12/09		<�V�K>
	//================================================================================
	void
	CKAGETOKI_ADVENT_CHARACTER::opeEffectGravity(void)
	{
	//	���ł���Ȃ�A�d�͂ɂ�藎������
		if(	this->bFlighting)
		{
			if((this->fUpDownSpeed	-  this->fUpDownAccelerate) > this->fMaxDownSpeed)
			{
				this->fUpDownSpeed	-= this->fUpDownAccelerate;
			}
		}

		float	inMoveValue			= 0.0f;								//!< ���ʓI�Ȉړ���
		float	inIntersectValue	= this->getIntegralDownSpeed();		//!< �Փ˔��肷�鋗��(����)

	//	�㉺�ړ����x�ŁA������@������
		if(	this->fUpDownSpeed > 0.0f)									//!< �㏸��
		{
			this->UDLine.ud		= eUD_UP;
		//	�Փ˔��苗���ɃL�����N�^�[�̑傫�����v���X
			inIntersectValue   += this->getWorldRadius();
		}
		else															//!< ���~���A�y�ђ�~��
		{
			this->UDLine.ud		= eUD_DOWN;
		}

		BOOL	bHit1	= this->stageG->intersect(this, &this->UDLine, (inIntersectValue + this->fFloorSpace));
		float	fDist	= this->getDist();

		if(	bHit1)
		{
		//	���C�̕�������
			if(	this->UDLine.ud == eUD_DOWN)
			{
			//	�󒆂ɂ���ꍇ
				if(	this->bFlighting)
				{
				//	�󒆂ɂ��Ȃ��ݒ�
					this->bFlighting		= false;
				//	���ʓI�Ȉړ��ʂ��v�Z
					inMoveValue				= (fDist - this->fFloorSpace);
				//	�㉺�ړ��l�̒�~
					this->fUpDownSpeed		= -(this->fFloorSpace * 2.0f);
				}
			//	�󒆂ɂ��Ȃ������ꍇ
				else
				{
				//	���Ƃ̋����ɕω�����������
					if(	this->fFloorSpace != fDist)
					{
					//	���ʓI�Ȉړ��ʂ��v�Z
						inMoveValue	= (this->fFloorSpace - fDist);
					//	���Ƃ̋������A�L��������
						if		(this->fFloorSpace < fDist)
						{
						//	���Ɉړ�����
							this->UDLine.ud = eUD_DOWN;
						}
					//	���Ƃ̋������A���܂�����
						else if	(this->fFloorSpace > fDist)
						{
						//	��Ɉړ�����
							this->UDLine.ud = eUD_UP;
						}
					}
				}
			}
		//	���C�̕�������
			else if(this->UDLine.ud == eUD_UP)
			{
			//	�㉺�ړ��l�̌���
				this->fUpDownSpeed	-= 2.0f;
			}
		}
		else
		{
		//	�󒆂ɂ���
			this->bFlighting	 = true;
		//	�㉺�ړ��l���v�Z
			inMoveValue			 = this->fUpDownSpeed;
		}

	//	���ʓI�Ȉړ��ʂɕω�������Ȃ�A�ړ�������
		if(	inMoveValue	!= 0.0f)
		{
		//	�㉺�ړ�
			this->moveUpDown(this->UDLine.ud, inMoveValue);
		}
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_CHARACTER::opeLocShift
	//
	//	�@�\		�ʒu���킹����
	//	�X�V		2008/12/10		<�V�K>
	//================================================================================
	void
	CKAGETOKI_ADVENT_CHARACTER::opeLocShift(void)
	{
		BOOL		bFront		= FALSE,	bRear		= FALSE;
		float		fFrontDist	= 0.0f,		fRearDist	= 0.0f;
		C3D_OBJECT	*frontObj	= NULL,		*rearObj	= NULL;

	//	�󒆂ɂ��鎞
		if(	this->bFlighting)
		{
		//	�O���Փ˔��茋�ʂ��擾
			bFront		= this->stageG->intersect(this, ADVENT_FRONTHIT, this->fWallSpace);
			fFrontDist	= this->getDist();
			frontObj	= this->stageG->getPrevHitObject();
		//	����Փ˔��茋�ʂ��擾
			bRear		= this->stageG->intersect(this, ADVENT_REARHIT,  this->fWallSpace);
			fRearDist	= this->getDist();
			rearObj		= this->stageG->getPrevHitObject();
		//	�O���ɏՓˊm�F
			if(	bFront)
			{
			//	�O�������ɏՓˊm�F
				if(	!bRear)
				{
				//	�ʒu�𒲐߂���
					this->setMoveQuantity((this->fWallSpace - fFrontDist) - 1.0f);
					ADVENT_REALMOVE;
				}
			}
		//	��������ɏՓˊm�F
			else if(bRear)
			{
			//	�ʒu�𒲐߂���
				this->setMoveQuantity((this->fWallSpace - fRearDist) - 1.0f);
				ADVENT_FRONTMOVE;
			}
		}
	//	�n��ɋ��鎞
		else
		{
		//	�O���Փ˔��茋�ʂ��擾
			bFront		= this->stageG->intersect(this, ADVENT_FRONTHIT, this->fWallSpace * 3.0f);
			fFrontDist	= this->getDist();
			frontObj	= this->stageG->getPrevHitObject();
		//	����Փ˔��茋�ʂ��擾
			bRear		= this->stageG->intersect(this, ADVENT_REARHIT,  this->fWallSpace * 3.0f);
			fRearDist	= this->getDist();
			rearObj		= this->stageG->getPrevHitObject();
		//	�O���ɏՓˊm�F
			if(	bFront)
			{
			//	�O���ƌ���ɏՓˊm�F
				if(	bRear)
				{
				//	�O���ƌ���ɏՓ˂����I�u�W�F�N�g����v��
					if(	frontObj == rearObj)
					{
					//	������ɏՓ˔�������A�܂������I�u�W�F�N�g�ɏՓ˂�����A
					//	�g�������Ȃ��󋵂Ɣ��f���A�ʒu���Â炷
						if(	frontObj->intersect(this, eUD_UP, this->getWorldRadius() * 2.0f))
						{
							this->setMoveQuantity(this->getDist() + 10.0f);
							this->moveUp();
						}
					}
				//	�s��v��
					else
					{
					//	�O���ƌ���ɏՓ˂����I�u�W�F�N�g�������ɂȂ�
						if(	((fFrontDist < (this->fWallSpace * 2.0f))	&&
							 (fRearDist  < (this->fWallSpace * 2.0f)))	&&
					//	���E�ǂ����[������e]�łȂ���
							(frontObj->getThisMessage() != eTMSG_SEE_SHADOW)	&&
							(rearObj->getThisMessage()  != eTMSG_SEE_SHADOW))
						{
						//	������ɏՓ˔�������A�����������e�łȂ��Ȃ�I�u�W�F�N�g�ɏՓ˂�����A
						//	�g�������Ȃ��󋵂Ɣ��f���A�ʒu���Â炷
							if(	this->stageG->intersect(this, eUD_UP, this->getWorldRadius() * 2.0f))
							{
								this->setMoveQuantity(this->getDist()*2.0f + 10.0f);
								this->moveUp();
							}
						}
					}
				}
			//	�O�������ɏՓˊm�F
				else
				{
				}
			}
		//	��������ɏՓˊm�F
			else if(bRear)
			{
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_CHARACTER::operation
	//
	//	�@�\		�㉺���E�ړ��̊�{����
	//	����		*input			���͊Ǘ�
	//	�X�V		2008/12/09		<�V�K>
	//================================================================================
	void
	CKAGETOKI_ADVENT_CHARACTER::operation(CINPUT* input)
	{
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------