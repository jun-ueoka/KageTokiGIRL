//***********************************************************************************
//!	�t�@�C����		Class_MovieCamera.h
//
//	�@�\			���[�r�[�Ɏg�p����J�����̒�`
//	�L�q			�����@�q��
//	�v���W�F�N�g	����
//	�X�V			2008/12/15	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_3DObjectCamera.h"

namespace MATH
{
	//==================================================================
	//	�X�v���C���Ȑ��̋�Ԃ̍ő�l
	//==================================================================
	#define MaxSplineSize 100

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CSPLINE
	//
	//	�@�\			���[�r�[�p�J�����Ɏg�p����X�v���C���Ȑ��̌v�Z
	//	�@�\�T�v		
	//	�X�V			2008/12/15	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class SPLINE
	{
	protected:		//-------------------- protected --------------------//
		//====================================================================
		//	�X�v���C���֘A
		//====================================================================
		int		iNumber;				//��Ԃ̐�
   		float	a[MaxSplineSize+1];		//0���W��(a)
		float	b[MaxSplineSize+1];		//1���W��(b)
		float	c[MaxSplineSize+1];		//2���W��(c)
		float	d[MaxSplineSize+1];		//3���W��(b)
	public:			//-------------------- public --------------------//
		SPLINE();
		//==================================================================
		//	�X�v���C���̏��̐ݒ�
		//==================================================================
			void setSplineDate(float *LocDate, int inNumber);
		//==================================================================
		//	�X�v���C���Ȑ��̌v�Z���ʂ�Ԃ�
		//==================================================================
			float getSplineCalc(float inMedia);
	};
}


//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//==================================================================
	//	
	//==================================================================
	struct Vec3Spline
	{
		MATH::SPLINE x;
		MATH::SPLINE y;
		MATH::SPLINE z;
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		C3D_MOVIE_CAMERA
	//
	//	�@�\			�O�������̂̕`����(���[�r�[�p)�𐶐�����
	//	�@�\�T�v		
	//	�X�V			2008/12/15	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class C3D_MOVIE_CAMERA : public C3D_OBJECT_CAMERA
	{
	protected:		//-------------------- protected --------------------//
		//	�J�����ʒu
			int			iCameraLocVertex;							//!< �J�����ʒu�̓����̃��C���̒��_��
			Vec3Spline	vCameraLoc;									//!< �J�����ʒu�̓����̃��C��
			float		fCameraLocMedia;							//!< �J�����ʒu�̓����̔}��ϐ�
			float		fCameraLocSpeed;							//!< �J�����ʒu�̓����̑��x
			BOOL		bCameraLocLoop;								//!< �J�����ʒu�̓����̃��[�v�t���O
		//	�J�������_
			int			iCameraViewPointVertex;						//!< �J�������_�̓����̃��C���̒��_��
			Vec3Spline	vCameraViewPoint;							//!< �J�������_�̓����̃��C��
			float		fCameraViewPointMedia;						//!< �J�������_�̓����̔}��ϐ�
			float		fCameraViewPointSpeed;						//!< �J�������_�̓����̑��x
			BOOL		bCameraViewPointLoop;						//!< �J�������_�̓����̃��[�v�t���O
	public:			//-------------------- public --------------------//
			C3D_MOVIE_CAMERA(	C3D_OBJECT*	inTarget,
								BOOL		inChased,
								float		inOffsetBack,
								float		inLookHeight,
								BOOL		inPan);				//!< �R���X�g���N�^(�ǐՑΏۂ���)
			C3D_MOVIE_CAMERA(VEC3* inLoc, VEC3* inViewPoint);	//!< �R���X�g���N�^(�ǐՑΏۂȂ�)
	virtual ~C3D_MOVIE_CAMERA();

	//==================================================================
	//	�J�����ʒu�̓����̐ݒ�A����i�X�v���C���Ȑ��ݒ�j
	//==================================================================
		void	setLocSpline(VEC3 *inLoc, int inVertex, BOOL inLoop);
		BOOL	doLocSpline(void);
		void	setLocSpeed(float inSpeed)									{ fCameraLocSpeed = inSpeed;	}
		float	getLocSpeed(void)											{ return fCameraLocSpeed;		}
		void	setLocMedia(float inMedia)									{ fCameraLocMedia = inMedia;	}
		float	getLocMedia(void)											{ return fCameraLocMedia;		}
		void	setLocLoop(BOOL inLoop)										{ bCameraLocLoop = inLoop;		}
		BOOL	getLocLoop(void)											{ return bCameraLocLoop;		}
	//==================================================================
	//	�J�������_�̓����̐ݒ�A����i�X�v���C���Ȑ��ݒ�j
	//==================================================================
		void	setViewPointSpline(VEC3 *inLoc, int inVertex, BOOL inLoop);
		BOOL	doViewPointSpline(void);
		void	setViewPointSpeed(float inSpeed)							{ fCameraViewPointSpeed = inSpeed;	}
		float	getViewPointSpeed(void)										{ return fCameraViewPointSpeed;		}
		void	setViewPointMedia(float inMedia)							{ fCameraViewPointMedia = inMedia;	}
		float	getViewPointMedia(void)										{ return fCameraViewPointMedia;		}
		void	setViewPointLoop(BOOL inLoop)								{ bCameraViewPointLoop	= inLoop;	}
		BOOL	getViewPointLoop(void)										{ return bCameraViewPointLoop;		}
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------