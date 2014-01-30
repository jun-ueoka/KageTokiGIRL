//***********************************************************************************
//!	�t�@�C����		Class_3DObjectCamera.h
//
//	�@�\			�O�����̕��̂�`�ʂ���J�����̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2007/12/08	<�V�K>
//					2008/07/09	<�ǉ�>	�S�̓I�ɃR�����g�̋���
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_3DObject.h"

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�J�����̃��[�h
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	CAMERA_MODE
	{
		eCAMO_PARALLEL,				//!< ���_���t�ړ����[�h
		eCAMO_PARALLEL_CHASE,		//!< �J�������t�ړ����[�h
		eCAMO_DOUBLE_TARGET,		//!< ��d�ǐՃ��[�h(2D���_)
		eCAMO_NO					//!< �J�������[�h�Ȃ�
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		C3D_OBJECT_CAMERA
	//
	//	�@�\			�O�������̂̕`���Ԃ𐶐�����
	//	�@�\�T�v		���̃J������Direct3D�f�o�C�X�𑗂�ƁA
	//					3D��Ԃ������������Ă���܂�
	//					���Ȃ�KageToki�d�l�Ȃ̂ŁA�ėp�ł��Ȃ��ꍇ������
	//	�X�V			2008/07/09	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	C3D_OBJECT_CAMERA : public C3D_OBJECT
	{
	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	�ǐՊ֘A
			//==================================================================
				C3D_OBJECT*		target;					//!< �ǐՑΏ�
				BOOL			bChased;				//!< �ǐՊm�F
				float			fOffsetBack;			//!< �ǐՑΏۂ���̌������

				C3D_OBJECT*		secondTarget;			//!< ��d�ǐՑΏ�
				float			fTargetsDist;			//!< ��̒ǐՑΏۂ̋���

			//==================================================================
			//	�J�������
			//==================================================================
				VEC3	viewPointFinish;				//!< �ŏI�I�Ȏ��_
				VEC3	viewPoint;						//!< ���_

				VEC3	viewMovePoint;					//!< ���_�̈ړ��I���_
				int		iViewMoveCnt;					//!< ���_�ړ��J�E���^
				int		iMaxViewMoveCnt;				//!< �ő压�_�ړ��J�E���^

				VEC3	cameraMovePoint;				//!< �J�����̈ړ��I���_
				int		iCameraMoveCnt;					//!< �J�����ړ��J�E���^
				int		iMaxCameraMoveCnt;				//!< �ő�J�����ړ��J�E���^

				VEC3	keepViewTargetShift;			//!< �ǐՑΏۃJ�����ʒu�ۑ��p
				BOOL	bViewTargetShiftMove;			//!< �ǐՑΏۃJ�����ʒu�ϓ��m�F
				VEC3	viewTargetShiftMovePoint;		//!< �ǐՑΏۃJ�����ʒu�̈ړ��I���_
				int		iViewTargetShiftMoveCnt;		//!< �ǐՑΏۃJ�����ʒu�ړ��J�E���^
				int		iMaxViewTargetShiftMoveCnt;		//!< �ő�ǐՑΏۃJ�����ʒu�ړ��J�E���^

				VEC3		viewShift;					//!< ���_�␳
				VEC3		viewPointShift;				//!< �J�����̎��_����̈ʒu
				VEC3		viewTargetShift;			//!< �ǐՑΏۂ���́A�J�����̈ʒu
				float		viewNear;					//!< �ߎ���
				float		viewFar;					//!< ������
				VEC3		currentUp;					//!< �J�����g���
				BOOL		bPan;						//!< �p���m�F
				float		fLookHeight;				//!< �J�����̍���
				float		fFieldOfView;				//!< �B�e����p�x
				float		fAspect;					//!< �c����
				CAMERA_MODE	cameraMode;					//!< �J�����̃��[�h

				MAT16			matrixView;				//!< �r���[�}�g���b�N�X
				MAT16			matrixProjection;		//!< �v���W�F�N�V�����}�g���b�N�X

				VEC3	doubleTargetLoc;				//!< ��̎��_�Ώۂ̊Ԃ̈ʒu
				VEC3	getDoubleTargetLoc(void);		//!< ��̎��_�Ώۂ̊Ԃ̈ʒu���Z�o
				void	viewMovement(void);				//!< ���_�ʒu�ϓ�����
				void	cameraMovement(void);			//!< �J�����ʒu�ϓ�����
				void	viewTargetShiftMovement(void);	//!< �ǐՑΏۃJ�����ʒu�ϓ�����

	public:			//-------------------- public --------------------//

				C3D_OBJECT_CAMERA(C3D_OBJECT*	inTarget,
								  BOOL			inChased,
								  float			inOffsetBack,
								  float			inLookHeight,
								  BOOL			inPan);				//!< �R���X�g���N�^(�ǐՑΏۂȂ�)
				C3D_OBJECT_CAMERA(VEC3* inLoc, VEC3* inViewPoint);	//!< �R���X�g���N�^(�ǐՑΏۂ���)
		virtual	~C3D_OBJECT_CAMERA();								//!< �f�X�g���N�^
		
	public:			//-------------------- public --------------------//

				void	updateCamera(void);							//!< �J�������X�V����
				void	initialize(void);							//!< ������
		virtual	void	viewTransform(CONST DEV inDev);				//!< ���[���h�ϊ����{��

			//==================================================================
			//	access	�ǐՑΏ�
			//==================================================================
				C3D_OBJECT*	getTarget(void)const					{ return	target;			}
				void		setTarget(C3D_OBJECT* inTarget)			{ target = inTarget;		}
				C3D_OBJECT*	getSecondTarget(void)const				{ return	secondTarget;	}
				void		setSecondTarget(C3D_OBJECT* inTarget)	{ secondTarget	= inTarget;	}

			//==================================================================
			//	access	�ǐՊm�F
			//==================================================================
				BOOL	getChase(void)const						{ return	bChased;			}
				void	setChase(BOOL inChased)					{ bChased = inChased;			}

			//==================================================================
			//	access	�������
			//==================================================================
				float	getOffsetBack(void)const				{ return	fOffsetBack;		}
				void	setOffsetBack(float inOffsetBack)		{ fOffsetBack = inOffsetBack;
																  setMoveQuantity(fOffsetBack);	}

			//==================================================================
			//	access	�ŏI�I�Ȏ��_
			//==================================================================
				VEC3	getViewPointFinish(void)const			{ return	viewPointFinish;		}
				void	setViewPointFinish(VEC3* inViewPoint)	{ viewPointFinish = *inViewPoint;	}

			//==================================================================
			//	access	���_
			//==================================================================
				VEC3	getViewPoint(void)const				{ return	viewPoint;			}
				float	getViewPointX(void)const			{ return	viewPoint.x;		}
				float	getViewPointY(void)const			{ return	viewPoint.y;		}
				float	getViewPointZ(void)const			{ return	viewPoint.z;		}
				void	setViewPoint(VEC3* inViewPoint)		{ viewPoint		= *inViewPoint;	}
				void	setViewPointX(float inViewPointX)	{ viewPoint.x	= inViewPointX;	}
				void	setViewPointY(float inViewPointY)	{ viewPoint.y	= inViewPointY;	}
				void	setViewPointZ(float inViewPointZ)	{ viewPoint.z	= inViewPointZ;	}
				void	calcViewPoint(VEC3* inViewPoint)	{ viewPoint	   += *inViewPoint;	}
				void	calcViewPointX(float inViewPointX)	{ viewPoint.x  += inViewPointX;	}
				void	calcViewPointY(float inViewPointY)	{ viewPoint.y  += inViewPointY;	}
				void	calcViewPointZ(float inViewPointZ)	{ viewPoint.z  += inViewPointZ;	}

			//==================================================================
			//	access	�ő压�_�ړ��J�E���^
			//==================================================================
				int		getMaxViewMoveCnt(void)const		{ return	iMaxViewMoveCnt;	}
				void	setMaxViewMoveCnt(int inValue)		{ iMaxViewMoveCnt = inValue;	}

			//==================================================================
			//	access	�ő�J�����ړ��J�E���^
			//==================================================================
				int		getMaxCameraMoveCnt(void)const		{ return	iMaxCameraMoveCnt;	}
				void	setMaxCameraMoveCnt(int inValue)	{ iMaxCameraMoveCnt = inValue;	}

			//==================================================================
			//	access	���_�␳
			//==================================================================
				VEC3	getViewShift(void)const				{ return	viewShift;			}
				float	getViewShiftX(void)const			{ return	viewShift.x;		}
				float	getViewShiftY(void)const			{ return	viewShift.y;		}
				float	getViewShiftZ(void)const			{ return	viewShift.z;		}
				void	setViewShift(VEC3* inViewShift)		{ viewShift		= *inViewShift;	}
				void	setViewShiftX(float inViewShiftX)	{ viewShift.x	= inViewShiftX;	}
				void	setViewShiftY(float inViewShiftY)	{ viewShift.y	= inViewShiftY;	}
				void	setViewShiftZ(float inViewShiftZ)	{ viewShift.z	= inViewShiftZ;	}
				void	calcViewShift(VEC3* inViewShift)	{ viewShift	   += *inViewShift;	}
				void	calcViewShiftX(float inViewShiftX)	{ viewShift.x  += inViewShiftX;	}
				void	calcViewShiftY(float inViewShiftY)	{ viewShift.y  += inViewShiftY;	}
				void	calcViewShiftZ(float inViewShiftZ)	{ viewShift.z  += inViewShiftZ;	}

			//==================================================================
			//	access	�J��������́A�ǐՑΏۂ̈ʒu
			//==================================================================
				VEC3	getViewPointShift(void)const					{ return	viewPointShift;					}
				float	getViewPointShiftX(void)const					{ return	viewPointShift.x;				}
				float	getViewPointShiftY(void)const					{ return	viewPointShift.y;				}
				float	getViewPointShiftZ(void)const					{ return	viewPointShift.z;				}
				void	setViewPointShift(VEC3* inViewPointShift)		{ viewPointShift	= *inViewPointShift;	}
				void	setViewPointShiftX(float inViewPointShiftX)		{ viewPointShift.x	= inViewPointShiftX;	}
				void	setViewPointShiftY(float inViewPointShiftY)		{ viewPointShift.y	= inViewPointShiftY;	}
				void	setViewPointShiftZ(float inViewPointShiftZ)		{ viewPointShift.z	= inViewPointShiftZ;	}
				void	calcViewPointShift(VEC3* inViewPointShift)		{ viewPointShift   += *inViewPointShift;	}
				void	calcViewPointShiftX(float inViewPointShiftX)	{ viewPointShift.x  += inViewPointShiftX;	}
				void	calcViewPointShiftY(float inViewPointShiftY)	{ viewPointShift.y  += inViewPointShiftY;	}
				void	calcViewPointShiftZ(float inViewPointShiftZ)	{ viewPointShift.z  += inViewPointShiftZ;	}

			//==================================================================
			//	access	�ǐՑΏۂ���́A�J�����̈ʒu
			//==================================================================
				VEC3	getViewTargetShift(void)const					{ return	viewTargetShift;				}
				float	getViewTargetShiftX(void)const					{ return	viewTargetShift.x;				}
				float	getViewTargetShiftY(void)const					{ return	viewTargetShift.y;				}
				float	getViewTargetShiftZ(void)const					{ return	viewTargetShift.z;				}
				void	setViewTargetShift(VEC3* inViewTargetShift)		{ viewTargetShift	 = *inViewTargetShift;	}
				void	setViewTargetShiftX(float inViewTargetShiftX)	{ viewTargetShift.x	 = inViewTargetShiftX;	}
				void	setViewTargetShiftY(float inViewTargetShiftY)	{ viewTargetShift.y	 = inViewTargetShiftY;	}
				void	setViewTargetShiftZ(float inViewTargetShiftZ)	{ viewTargetShift.z	 = inViewTargetShiftZ;	}
				void	calcViewTargetShift(VEC3* inViewTargetShift)	{ viewTargetShift	+= *inViewTargetShift;	}
				void	calcViewTargetShiftX(float inViewTargetShiftX)	{ viewTargetShift.x	+= inViewTargetShiftX;	}
				void	calcViewTargetShiftY(float inViewTargetShiftY)	{ viewTargetShift.y	+= inViewTargetShiftY;	}
				void	calcViewTargetShiftZ(float inViewTargetShiftZ)	{ viewTargetShift.z	+= inViewTargetShiftZ;	}

			//==================================================================
			//	access	�ߎ���,������
			//==================================================================
				float	getViewNear(void)const				{ return	viewNear;			}
				float	getViewFar(void)const				{ return	viewFar;			}
				void	setViewNear(float inViewNear)		{ viewNear = inViewNear;		}
				void	setViewFar(float inViewFar)			{ viewFar = inViewFar;			}

			//==================================================================
			//	access	�J�����g���
			//==================================================================
				VEC3	getCurrentUp(void)const				{ return	currentUp;			}
				void	setCurrentUp(VEC3* inCurrentUp)		{ currentUp = *inCurrentUp;		}

			//==================================================================
			//	access	�p��
			//==================================================================
				BOOL	getPan(void)const					{ return	bPan;				}
				void	setPan(BOOL inPan)					{ bPan = inPan;	updateCamera();	}

			//==================================================================
			//	access	���_�̍���
			//==================================================================
				float	getLookHeight(void)const			{ return	fLookHeight;		}
				void	setLookHeight(float inLookHeight)	{ fLookHeight = inLookHeight;	}

			//==================================================================
			//	access	�B�e����p�x
			//==================================================================
				float	getFieldOfView(void)const			{ return	fFieldOfView;		}
				void	setFieldOfView(float inFieldOfView)	{ fFieldOfView  = inFieldOfView;}
				void	calcFieldOfView(float inFieldOfView){ fFieldOfView += inFieldOfView;}

			//==================================================================
			//	access	�B�e����p�x
			//==================================================================
				float	getAspect(void)const				{ return	fAspect;			}
				void	setAspect(FLOAT inAspect)			{ fAspect = inAspect;			}

			//==================================================================
			//	access	�J�������[�h
			//==================================================================
				CAMERA_MODE	getCameraMode(void)const		{ return	cameraMode;			}
				void		setCameraMode(CAMERA_MODE inMode);

			//==================================================================
			////	access	�}�g���b�N�X
			//==================================================================
				MAT16*	getMatrixView(void)					{ return	&matrixView;		}
				MAT16*	getMatrixProjection(void)			{ return	&matrixProjection;	}

			//==================================================================
			//	���z���\�b�h���
			//==================================================================
				BOOL	draw(CONST DEV inDev)				{ return	FALSE;				}
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------