//***********************************************************************************
//!	�t�@�C����		Class_KageTokiAdventEnemy.cpp
//
//	�@�\			�A�h�x���`���[�Q�[�����[�h�̓G�L�����N�^�[�N���X
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2009/01/16	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_KageTokiAdventEnemy.h"

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_ENEMY::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		�X�L�����b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2009/01/16		<�V�K>
	//================================================================================
	CKAGETOKI_ADVENT_ENEMY::CKAGETOKI_ADVENT_ENEMY(
		CONST DEV	inDev,
		CONST LPSTR	inName,
		CONST LPSTR	inResource,
		CONST LPSTR	inTexture)	:
	CKAGETOKI_ADVENT_CHARACTER(inDev, inName, inResource, inTexture)
	{
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_ENEMY::�f�X�g���N�^
	//
	//	�X�V		2009/01/16		<�V�K>
	//================================================================================
	CKAGETOKI_ADVENT_ENEMY::~CKAGETOKI_ADVENT_ENEMY(void)
	{
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_ENEMY::opeTurn
	//
	//	�@�\		�����]������
	//	�X�V		2009/01/16		<�V�K>
	//================================================================================
	void
	CKAGETOKI_ADVENT_ENEMY::opeTurn(void)
	{
	//	�����̐���
		switch(this->way)
		{
		//	���������r��
			case eADCW_LEFT_TURN:
				this->setAngleY(ADVENT_LEFTANGLE);
				this->way = eADCW_LEFT;
				break;

		//	�E�������r��
			case eADCW_RIGHT_TURN:
				this->setAngleY(ADVENT_RIGHTANGLE);
				this->way = eADCW_RIGHT;
				break;

		//	�������
			case eADCW_NO:
			//	�����̌����ɂȂ��Ă���Ȃ�A�������ɐݒ肷��
				if(	(this->getAngleY() >=   0.0f) &&
					(this->getAngleY() <  180.0f))
				{
					this->setAngleY(ADVENT_LEFTANGLE);
					this->way = eADCW_LEFT;
				}
			//	��L�ȊO�́A�E�����ɐݒ肷��
				else
				{
					this->setAngleY(ADVENT_RIGHTANGLE);
					this->way = eADCW_RIGHT;
				}
				break;

		//	��L�ȊO�́A���������Ȃ�
			default:	
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_ENEMY::setControlAreaOffset
	//
	//	�@�\		����̈�̃I�t�Z�b�g��ݒ�
	//	����		���E�㉺�O��̗̈敝
	//	�X�V		2009/01/16		<�V�K>
	//================================================================================
	void
	CKAGETOKI_ADVENT_ENEMY::setControlAreaOffset(
		float inLeft,   float inRight,
		float inBottom, float inTop,
		float inRear,   float inFront)
	{
		controlAreaOffset.left   = inLeft;
		controlAreaOffset.right  = inRight;
		controlAreaOffset.bottom = inBottom; 
		controlAreaOffset.top    = inTop;
		controlAreaOffset.rear   = inRear;
		controlAreaOffset.front  = inFront;
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_ENEMY::getControlArea
	//
	//	�@�\		����̈�̃I�t�Z�b�g���󂯎��
	//	�߂�l		����̈��␳�����̈�|�C���^�[
	//	�X�V		2009/01/16		<�V�K>
	//================================================================================
	MY_CUBE*
	CKAGETOKI_ADVENT_ENEMY::getControlArea(void)
	{
		controlArea.left	= getLocX() + controlAreaOffset.left;
		controlArea.right	= getLocX() + controlAreaOffset.right;
		controlArea.bottom	= getLocY() + controlAreaOffset.bottom; 
		controlArea.top		= getLocY() + controlAreaOffset.top;
		controlArea.rear	= getLocZ() + controlAreaOffset.rear;
		controlArea.front	= getLocZ() + controlAreaOffset.front;
		return	&controlArea;
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_ENEMY::setActionArea
	//
	//	�@�\		�s���̈��ݒ肷��
	//	�X�V		2009/01/16		<�V�K>
	//================================================================================
	void
	CKAGETOKI_ADVENT_ENEMY::setActionArea(VEC3* inLoc, VEC3* inSize)
	{
		actionArea.left		= inLoc->x - inSize->x;
		actionArea.right	= inLoc->x + inSize->x;
		actionArea.bottom	= inLoc->y - inSize->y; 
		actionArea.top		= inLoc->y + inSize->y;
		actionArea.rear		= inLoc->z - inSize->z;
		actionArea.front	= inLoc->z + inSize->z;
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_ENEMY::setActionArea
	//
	//	�@�\		�s���̈��ݒ肷��(���S�w��)
	//	�X�V		2009/01/16		<�V�K>
	//================================================================================
	void
	CKAGETOKI_ADVENT_ENEMY::setActionArea(
		float inLeft,   float inRight,
		float inBottom, float inTop,
		float inRear,   float inFront)
	{
		actionArea.left   = inLeft;
		actionArea.right  = inRight;
		actionArea.bottom = inBottom; 
		actionArea.top    = inTop;
		actionArea.rear   = inRear;
		actionArea.front  = inFront;
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_ENEMY::resetParameter
	//
	//	�@�\		�s���̈��ݒ肷��(���S�w��)
	//	�X�V		2009/02/10		<�V�K>
	//================================================================================
	void
	CKAGETOKI_ADVENT_ENEMY::resetParameter(void)
	{
		this->iLife	= this->iMaxLife;
		this->setVisible(TRUE);
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------