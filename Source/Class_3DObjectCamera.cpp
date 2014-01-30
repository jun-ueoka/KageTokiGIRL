//***********************************************************************************
//!	�t�@�C����		Class_3DObjectCamera.cpp
//
//	�@�\			�O�����̕��̂�`�ʂ���J�����̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2007/12/08	<�V�K>
//					2008/07/09	<�ǉ�>	�S�̓I�ɃR�����g�̋���
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_3DObjectCamera.h"

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_CAMERA::C3D_OBJECT_CAMERA
	//
	//	�@�\		�R���X�g���N�^(�ǐՑΏۂ���)
	//	����		inTarget		����Ώ�
	//				inChased		�ǐՊm�F
	//				inOffsetBack	�ǐՎ��̌���ʒu
	//				inLookHeight	�ǐՎ��̍���
	//				inPan			�p���m�F
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	C3D_OBJECT_CAMERA::C3D_OBJECT_CAMERA(C3D_OBJECT*  inTarget,
										 BOOL		  inChased,
										 float		  inOffsetBack,
										 float		  inLookHeight,
										 BOOL		  inPan)
	{
	//	�J�����̏�����
		C3D_OBJECT_CAMERA::initialize();

	//	�ǐՊ֘A�̏�����
		bChased		= inChased;			//!< �ǐՊm�F
		bPan		= inPan;			//!< �㉺���E����m�F
		fLookHeight	= inLookHeight;		//!< �J�����̍���
		setOffsetBack(inOffsetBack);	//!< �������

	//	�ʒu�̏�����
		setLoc(&VEC3(0.0f, fLookHeight, fOffsetBack));

	//	�ǐՑΏۂ̐ݒ�
		setTarget(inTarget);
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_CAMERA::C3D_OBJECT_CAMERA
	//
	//	�@�\		�R���X�g���N�^(�ǐՑΏۂȂ�)
	//	����		inLoc			�����ʒu
	//				inViewPoint		�������_
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	C3D_OBJECT_CAMERA::C3D_OBJECT_CAMERA(VEC3 *inLoc, VEC3 *inViewPoint)
	{
	//	�J�����̏�����
		C3D_OBJECT_CAMERA::initialize();

	//	������
		viewPoint	= *inViewPoint;	//!< ���_
		setLoc(inLoc);				//!< �ʒu
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_CAMERA::~C3D_OBJECT_CAMERA
	//
	//	�@�\		�f�X�g���N�^
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	C3D_OBJECT_CAMERA::~C3D_OBJECT_CAMERA(void)
	{
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_CAMERA::getDoubleTargetLoc
	//
	//	�@�\		��̎��_�Ώۂ̊Ԃ̈ʒu���Z�o
	//	�߂�l		VEC3			�ʒu
	//	�X�V		2009/02/19		<�V�K>
	//================================================================================
	VEC3
	C3D_OBJECT_CAMERA::getDoubleTargetLoc(void)
	{
	//	���_�Ώۂ��ݒ肳��Ă���΁A��̎��_�Ώۂ̊Ԃ̈ʒu���Z�o
		if(	this->target && this->secondTarget)
		{
			VEC3	inDist			= this->target->getLoc() - this->secondTarget->getLoc();
			this->fTargetsDist		= D3DXVec3Length(&inDist);
			this->doubleTargetLoc	= this->secondTarget->getLoc() + (inDist * 0.5f);
			return	this->doubleTargetLoc;
		}
	//	�ݒ肳��ĂȂ���Ώ����������A���݂̎��_�Ώۂ�Ԃ�
		return	this->viewPoint;
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_CAMERA::viewMovement
	//
	//	�@�\		���_�ʒu�ϓ�����
	//	�X�V		2009/02/19		<�V�K>
	//================================================================================
	void
	C3D_OBJECT_CAMERA::viewMovement(void)
	{
	//	���_�ړ��J�E���^�����鎞
		if(	this->iViewMoveCnt   > 0)
		{
			this->iViewMoveCnt--;
		//	���_�̈ړ��I���_�ƌ��݂̎��_�̂܂ł̋������v�Z
			VEC3	moveVec	= ((this->viewMovePoint - this->viewPoint) * 0.15f);
			this->viewPoint += moveVec;
		}
	//	���_�ړ��J�E���^���c���Ė����Ȃ�A���̂܂܎��_���X�V
		else
		{
			this->viewPoint = this->viewMovePoint;
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_CAMERA::cameraMovement
	//
	//	�@�\		�J�����ʒu�ϓ�����
	//	�X�V		2009/02/19		<�V�K>
	//================================================================================
	void
	C3D_OBJECT_CAMERA::cameraMovement(void)
	{
	//	�J�����ʒu�ړ��J�E���^�����鎞
		if(	this->iCameraMoveCnt   > 0)
		{
			this->iCameraMoveCnt--;
		//	�J�����̈ړ��I���_�ƌ��݂̃J�����̂܂ł̋������v�Z
			VEC3	moveVec	= ((this->cameraMovePoint - this->getLoc()) * 0.15f);
			this->calcLoc(&moveVec);
		}
	//	�J�����ړ��J�E���^���c���Ė����Ȃ�A���̂܂܃J�����ʒu���X�V
		else
		{
			this->setLoc(&cameraMovePoint);
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_CAMERA::viewTargetShiftMovement
	//
	//	�@�\		�ǐՑΏۃJ�����ʒu�ϓ�����
	//	�X�V		2009/02/19		<�V�K>
	//================================================================================
	void
	C3D_OBJECT_CAMERA::viewTargetShiftMovement(void)
	{
	//	�ǐՑΏۃJ�����ʒu�ϓ��m�F���L���̎�
		if(	this->bViewTargetShiftMove)
		{
		//	�ǐՑΏۃJ�����ʒu�ړ��J�E���^�����鎞
			if(	this->iViewTargetShiftMoveCnt   > 0)
			{
				this->iViewTargetShiftMoveCnt--;
			//	�ǐՑΏۃJ�����ʒu�̈ړ��I���_�ƌ��݂̒ǐՑΏۃJ�����ʒu�܂ł̋������v�Z
				VEC3	moveVec	= ((this->viewTargetShiftMovePoint - this->viewTargetShift) * 0.15f);
				this->viewTargetShift += moveVec;
			}
		//	�ǐՑΏۃJ�����ʒu�ړ��J�E���^���c���Ė����Ȃ�A���̂܂ܒǐՑΏۃJ�����ʒu���X�V
			else
			{
				this->viewTargetShift		= this->viewTargetShiftMovePoint;
				this->bViewTargetShiftMove	= FALSE;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_CAMERA::updateCamera
	//
	//	�@�\		�J�������X�V����
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	void
	C3D_OBJECT_CAMERA::updateCamera(void)
	{
	//	���[�h����
		switch(cameraMode)
		{
		//	���_���t�ړ����[�h
			case	eCAMO_PARALLEL:
				viewPoint		= viewPointShift + getLoc();
				viewPointFinish	= viewPoint;
				break;

		//	�J�������t�ړ����[�h
			case	eCAMO_PARALLEL_CHASE:
			//	�Ώۂ�������΁A���������Ȃ�
				if(	target)
				{
				//	���_�ʒu�ϓ�����
					this->viewMovement();
				//	���_�̈ړ��I���_�ƒǐՑΏۈʒu����v���Ȃ���
					if(	viewMovePoint != target->getLoc())
					{
					//	�V���Ɏ��_�̈ړ��I���_��ݒ肵�A���_�ړ��J�E���^�J�n
						viewMovePoint  = target->getLoc();
						iViewMoveCnt   = iMaxViewMoveCnt;
					}
				//	�ǐՑΏۃJ�����ʒu�ϓ�����
					this->viewTargetShiftMovement();

				//	�ǐՂ��鎞
					if(	bChased)
					{
					//	�p�����鎞
						if(	bPan)
						{
						}
					//	�p�����Ȃ���
						else
						{
						//	�J�����̈ʒu���A�^�[�Q�b�g����̃J�����̈ʒu�̃I�t�Z�b�g�l�Őݒ�
							setLoc(&(viewPoint + viewTargetShift));
						}
					}
				//	�ǐՂ��Ȃ���
					else
					{
					//	�p�����鎞
						if(	bPan)
						{
						}
					//	�p�����Ȃ���
						else
						{
						}
					}
					viewPointFinish = viewPoint	+ viewShift;
				//	���_����̃J�����̈ʒu�̍X�V
					viewPointShift = viewPointFinish - getLoc();
				}
				break;

		//	��d�ǐՃ��[�h
			case	eCAMO_DOUBLE_TARGET:
			//	�Ώۂ�������΁A���������Ȃ�
				if(	this->target && this->secondTarget)
				{
				//	���_�ʒu�ϓ�����
					this->viewMovement();
				//	���_�̈ړ��I���_�ƒǐՑΏۈʒu����v���Ȃ���
					if(	this->viewMovePoint != this->getDoubleTargetLoc())
					{
					//	�V���Ɏ��_�̈ړ��I���_��ݒ肵�A���_�ړ��J�E���^�J�n
						this->viewMovePoint  = this->doubleTargetLoc;
						this->iViewMoveCnt   = this->iMaxViewMoveCnt;
					//	�ǐՑΏۃJ�����ʒu�̕ϓ��J�n
						this->bViewTargetShiftMove			= TRUE;
						this->viewTargetShiftMovePoint.z	= -(this->fTargetsDist + 1500.0f);
						this->iViewTargetShiftMoveCnt		= this->iMaxViewTargetShiftMoveCnt;
					}
				//	�ǐՑΏۃJ�����ʒu�ϓ�����
					this->viewTargetShiftMovement();

				//	�ǐՂ��鎞
					if(	this->bChased)
					{
					//	�p�����鎞
						if(	this->bPan)
						{
						}
					//	�p�����Ȃ���
						else
						{
						//	�J�����̈ʒu���A�^�[�Q�b�g����̃J�����̈ʒu�̃I�t�Z�b�g�l�Őݒ�
							this->setLoc(&(this->viewPoint + this->viewTargetShift));
						}
					}
				//	�ǐՂ��Ȃ���
					else
					{
					//	�p�����鎞
						if(	this->bPan)
						{
						}
					//	�p�����Ȃ���
						else
						{
						}
					}
					this->viewPointFinish = this->viewPoint	+ this->viewShift;
				//	���_����̃J�����̈ʒu�̍X�V
					this->viewPointShift = this->viewPointFinish - this->getLoc();
				}
				break;

			default:
			//	�ǐՑΏۂ����݂��鎞
				if(	this->target)
				{
				//	���_
					this->viewPoint	= this->target->getLoc();
				//	�ǐՂ��鎞
					if(	this->bChased)
					{
					//	�ǐՂ��邽�߃J�����̈ʒu���X�V
						this->setLoc(&this->target->getLoc());
					//	�p�����鎞
						if(	this->bPan)
						{
						//	�p������
							this->setAngleY(this->target->getAngleY());
						}
					//	����ֈړ�
						this->moveRear();
					//	�J�����̍���������
						this->setLocY(this->fLookHeight);
					}
				//	���_�␳
					this->viewPointFinish = this->viewPoint	+ this->viewShift;
				}
				else
				{	
					this->viewPointFinish = this->viewPoint;
				}
			//	���_����̃J�����̈ʒu�̍X�V
				this->viewPointShift	= this->viewPointFinish - this->getLoc();
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_CAMERA::initialize
	//
	//	�@�\		�J�����̏�����
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	void
	C3D_OBJECT_CAMERA::initialize(void)
	{
	//	�X�[�p�[�N���X�ł̏�����
		C3D_OBJECT::initialize();

	//	������
		target			= NULL;									//!< �ǐՑΏ�
		secondTarget	= NULL;									//!< ��d�ǐՑΏ�
		fTargetsDist	= 0.0f;									//!< ��̒ǐՑΏۂ̋���
		bChased			= FALSE;								//!< �ǐՊm�F
		setOffsetBack(0.0f);									//!< �������
		viewPointFinish	= VEC3(0.0f, 0.0f, 0.0f);				//!< �ŏI�I�Ȏ��_
		viewPoint		= VEC3(0.0f, 0.0f, 0.0f);				//!< ���_

		viewMovePoint	= VEC3(0.0f, 0.0f, 0.0f);				//!< ���_�̈ړ��I���_
		iViewMoveCnt	= 0;									//!< ���_�ړ��J�E���^
		iMaxViewMoveCnt	= 60;									//!< �ő压�_�ړ��J�E���^

		cameraMovePoint		= VEC3(0.0f, 0.0f, 0.0f);			//!< �J�����̈ړ��I���_
		iCameraMoveCnt		= 0;								//!< �J�����ړ��J�E���^
		iMaxCameraMoveCnt	= 60;								//!< �ő�J�����ړ��J�E���^

		keepViewTargetShift			= VEC3(0.0f, 0.0f, 0.0f);	//!< �ǐՑΏۃJ�����ʒu�ۑ��p
		bViewTargetShiftMove		= FALSE;					//!< �ǐՑΏۃJ�����ʒu�ϓ��m�F
		viewTargetShiftMovePoint	= VEC3(0.0f, 0.0f, 0.0f);	//!< �ǐՑΏۃJ�����ʒu�̈ړ��I���_
		iViewTargetShiftMoveCnt		= 0;						//!< �ǐՑΏۃJ�����ʒu�ړ��J�E���^
		iMaxViewTargetShiftMoveCnt	= 60;						//!< �ő�ǐՑΏۃJ�����ʒu�ړ��J�E���^

		doubleTargetLoc	= VEC3(0.0f, 0.0f, 0.0f);				//!< ��̎��_�Ώۂ̊Ԃ̈ʒu
		viewShift		= VEC3(0.0f, 0.0f, 0.0f);				//!< ���_�␳
		viewPointShift	= VEC3(0.0f, 0.0f, 0.0f);				//!< ���_����̃J�����̈ʒu
		viewNear		= 1.0f;									//!< �ߎ���(�K��)
		viewFar			= 1000.0f;								//!< ������(�K��)
		currentUp		= VEC3(0.0f, 1.0f, 0.0f);				//!< �J�����g���
		bPan			= FALSE;								//!< �p���m�F
		fLookHeight		= 0.0f;									//!< �J�����̍���
		fFieldOfView	= 60.0f;								//!< �B�e�p�x
		fAspect			= 4.0f / 3.0f;							//!< �c����(���ϓI�l)
		cameraMode		= eCAMO_NO;								//!< �J�������[�h
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_CAMERA::viewTransform
	//
	//	�@�\		���[���h�ϊ����{��
	//	�@�\�T�v	�����Ă����f�o�C�X�ɁA���[���h�ϊ����{��
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	void
	C3D_OBJECT_CAMERA::viewTransform(CONST DEV inDev)
	{
	//	�r���[�}�g���b�N�X�̐���
		D3DXMatrixLookAtLH(
			&matrixView,					//!< �r���[�}�g���b�N�X�̊i�[��
			&getLoc(),						//!< ��
			&viewPointFinish,				//!< ���_
			&currentUp);					//!< �J�����g���
	//	�v���W�F�N�V�����}�g���b�N�X�̐���
		D3DXMatrixPerspectiveFovLH(
			&matrixProjection,				//!< �v���W�F�N�V�����}�g���b�N�̊i�[��
			D3DXToRadian(fFieldOfView),		//!< �B�e����p�x
			fAspect,						//!< �c����
			viewNear,						//!< �ߎ���
			viewFar);						//!< ������

	//	���[���h�r���[�ϊ�
		inDev->SetTransform(D3DTS_VIEW,			&matrixView);
	//	���[���h�v���W�F�N�V�����ϊ�
		inDev->SetTransform(D3DTS_PROJECTION,	&matrixProjection);
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_CAMERA::setCameraMode
	//
	//	�@�\		�J�������[�h�̕ύX
	//	�@�\�T�v	�J�������[�h�̕ύX���s���A�ύX��̐ݒ������
	//	����		inMode			�V���ȃJ�������[�h
	//	�X�V		2009/02/07		<�V�K>
	//================================================================================
	void
	C3D_OBJECT_CAMERA::setCameraMode(CAMERA_MODE inMode)
	{
	//	���̃J�������[�h�ƁA�V���ȃJ�������[�h���������Ȃ�A������Ԃ�
		if(	this->cameraMode == inMode)	return;

	//	�V�K���[�h����
		switch(inMode)
		{
		//	���_���t�ړ����[�h
			case	eCAMO_PARALLEL:
				break;

		//	�J�������t�ړ����[�h
			case	eCAMO_PARALLEL_CHASE:
				if(	target)
				{
				//	�O�̃��[�h����d�ǐՃ��[�h�̎�
					if(	this->cameraMode == eCAMO_DOUBLE_TARGET)
					{
						bViewTargetShiftMove	 = TRUE;
						viewTargetShiftMovePoint = keepViewTargetShift;
						iViewTargetShiftMoveCnt	 = iMaxViewTargetShiftMoveCnt;
					}
					else
					{
						viewPoint		= viewPoint	- viewShift;
						viewMovePoint	= target->getLoc();
					}
				}
				iViewMoveCnt	= iMaxViewMoveCnt;
				break;

		//	��d�ǐՃ��[�h
			case	eCAMO_DOUBLE_TARGET:
			//	�ǐՑΏۂ̂ǂ��炩�������Ă���A���[�h��ύX���Ȃ�
				if(	!target	|| !secondTarget)	return;
			//	�ǐՑΏۂ̊Ԃ̒l���A���_�ړ��I���_�ɃZ�b�g
				viewMovePoint				= getDoubleTargetLoc();
				iViewMoveCnt				= iMaxCameraMoveCnt;
				viewTargetShiftMovePoint	= viewTargetShift;
				keepViewTargetShift			= viewTargetShift;
				break;

			default:
				break;
		}

		this->cameraMode = inMode;
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------