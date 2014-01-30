//***********************************************************************************
//!	�t�@�C����		Class_MovieCamera.cpp
//
//	�@�\			���[�r�[�Ɏg�p����J�����̎���
//	�L�q			�����@�q��
//	�v���W�F�N�g	����
//	�X�V			2008/12/15	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace MATH
{
	//================================================================================
	//!	���\�b�h��	SPLINE::SPLINE
	//
	//	�@�\		�R���X�g���N�^
	//	����		�Ȃ�
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	SPLINE::SPLINE(void)
	{
	//	�ϐ��̏�����
		this->iNumber = 0;
	}

	//================================================================================
	//!	���\�b�h��	SPLINE::setSplineDate
	//
	//	�@�\		�X�v���C������ݒ�
	//	����		float	*LocDate ���_�z��
	//				int		inNumber ���_��	
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	void SPLINE::setSplineDate(float *LocDate, int inNumber)
	{
		float	tmp; 
		float	w[MaxSplineSize+1];
		int		iCnt;										//�J�E���g�p

		this->iNumber = inNumber - 1;						//��Ԃ̐����L�^

		// �R����������0���W��(a)��ݒ�
		for(iCnt = 0; iCnt <= inNumber; ++iCnt) 
		{
       		this->a[iCnt] = LocDate[iCnt];
		}

		// �R����������2���W��(c)���v�Z
		c[0] = c[this->iNumber] = 0.0f;
		for(iCnt = 1; iCnt < this->iNumber; ++iCnt)
		{
			this->c[iCnt] = 3.0f * (this->a[iCnt-1] - 2.0f * this->a[iCnt] + this->a[iCnt+1]);
		}

	//-----------------�����̈Ӗ����܂��킩��Ȃ�-------------------------
   		// ����������
  		w[0]=0.0f;
		for(iCnt = 1; iCnt < inNumber; ++iCnt) 
		{
			tmp = 4.0f - w[iCnt-1];
			this->c[iCnt] = (this->c[iCnt] - this->c[iCnt-1]) / tmp;
			w[iCnt] = 1.0f / tmp;
		}
		// �E�������
		for(iCnt = this->iNumber - 1; iCnt > 0; --iCnt) 
		{
			this->c[iCnt] = this->c[iCnt] - this->c[iCnt+1] * w[iCnt];
		}
	//-----------------�����̈Ӗ����܂��킩��Ȃ�-------------------------

		// �R����������1���W��(b)��3���W��(b)���v�Z
		this->b[inNumber] = this->d[inNumber] =0.0f;
		for(iCnt = 0; iCnt < inNumber; ++iCnt) 
		{
			this->d[iCnt] = ( this->c[iCnt+1] - this->c[iCnt]) / 3.0f;
			this->b[iCnt] = this->a[iCnt+1] - this->a[iCnt] - this->c[iCnt] - this->d[iCnt];
		}
	}

	//================================================================================
	//!	���\�b�h��	SPLINE::getSplineCalc
	//
	//	�@�\		�X�v���C������ݒ�
	//	����		float	inMedia �}��ϐ�
	//	�߂�l		float	�v�Z����
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	float SPLINE::getSplineCalc(float inMedia)
	{
		int		iSection;								//��Ԃ����߂�̂Ɏg�p
		float	fMedia;									//�C�����������}��ϐ��̋L�^�Ɏg�p

		iSection = (int)floor(inMedia);					// �����_�ȉ��؎̂�

		//�덷�C��
		if(iSection < 0)
		{
			iSection = 0; 
		}
		else if (iSection >= this->iNumber)
		{
			iSection = this->iNumber = 1;
		}
		fMedia = inMedia - (float)iSection;

		//���߂��l��Ԃ�
		float Test = this->a[iSection] + ( this->b[iSection] + (this->c[iSection] + this->d[iSection] * fMedia) * fMedia ) * fMedia;
		return Test;
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	C3D_MOVIE_CAMERA::C3D_MOVIE_CAMERA
	//
	//	�@�\		�R���X�g���N�^(�ǐՑΏۂ���)
	//	����		inTarget		����Ώ�
	//				inChased		�ǐՊm�F
	//				inOffsetBack	�ǐՎ��̌���ʒu
	//				inLookHeight	�ǐՎ��̍���
	//				inPan			�p���m�F
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	C3D_MOVIE_CAMERA::C3D_MOVIE_CAMERA(	C3D_OBJECT*	inTarget, 
										BOOL inChased, 
										float inOffsetBack, 
										float inLookHeight, 
										BOOL inPan)	
	: C3D_OBJECT_CAMERA(inTarget, inChased, inOffsetBack, inLookHeight, inPan)	
	{
	//	�l�̏�����
		this->iCameraLocVertex			= 0;
		this->fCameraLocMedia			= 0.0f;
		this->fCameraLocSpeed			= 0.0f;
		this->bCameraLocLoop			= FALSE;
		this->iCameraViewPointVertex	= 0;
		this->fCameraViewPointMedia		= 0.0f;
		this->fCameraViewPointSpeed		= 0.0f;
		this->bCameraViewPointLoop		= FALSE;
	}

	//================================================================================
	//!	���\�b�h��	C3D_MOVIE_CAMERA::C3D_MOVIE_CAMERA
	//
	//	�@�\		�R���X�g���N�^(�ǐՑΏۂȂ�)
	//	����		inLoc			����Ώ�
	//				inViewPoint		�ǐՊm�F
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	C3D_MOVIE_CAMERA::C3D_MOVIE_CAMERA(	VEC3* inLoc,
										VEC3* inViewPoint)
	: C3D_OBJECT_CAMERA(inLoc, inViewPoint)
	{
	//	�l�̏�����
		this->iCameraLocVertex			= 0;
		this->fCameraLocMedia			= 0.0f;
		this->fCameraLocSpeed			= 0.0f;
		this->bCameraLocLoop			= FALSE;
		this->iCameraViewPointVertex	= 0;
		this->fCameraViewPointMedia		= 0.0f;
		this->fCameraViewPointSpeed		= 0.0f;
		this->bCameraViewPointLoop		= FALSE;
	}

	//================================================================================
	//!	���\�b�h��	C3D_MOVIE_CAMERA::~C3D_MOVIE_CAMERA
	//
	//	�@�\		�f�X�g���N�^
	//	����		�Ȃ�
	//	�X�V		2008/12/08		<�V�K>
	//================================================================================
	C3D_MOVIE_CAMERA::~C3D_MOVIE_CAMERA()
	{
	}

	//================================================================================
	//!	���\�b�h��	C3D_MOVIE_CAMERA::setLocSpline
	//
	//	�@�\		�J�����ʒu�̓����̐ݒ�(�X�v���C���Ȑ������Ă�)
	//	����		*inLoc		���_�z��
	//				inVertex	���_��
	//				inLoop		���[�v�t���O
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	void C3D_MOVIE_CAMERA::setLocSpline(D3DXVECTOR3 *inLoc, int inVertex, BOOL inLoop)
	{
		float fLocX[MaxSplineSize];
		float fLocY[MaxSplineSize];
		float fLocZ[MaxSplineSize];

		for(int i = 0; i < inVertex; ++i)
		{
			fLocX[i] = inLoc[i].x;
			fLocY[i] = inLoc[i].y;
			fLocZ[i] = inLoc[i].z;
		}

	//	�X�v���C���̐ݒ�
		this->vCameraLoc.x.setSplineDate(fLocX, inVertex);	//���_X
		this->vCameraLoc.y.setSplineDate(fLocY, inVertex);	//���_Y
		this->vCameraLoc.z.setSplineDate(fLocZ, inVertex);	//���_Z
		this->bCameraLocLoop = inLoop;						//���[�v�t���O�̋L�^
		this->iCameraLocVertex = inVertex;					//���_��
	}

	//================================================================================
	//!	���\�b�h��	C3D_MOVIE_CAMERA::doLocSpline
	//
	//	�@�\		�J�����ʒu�̓����̓���
	//	����		�Ȃ�
	//	�߂�l		�I���t���O�@TRUE�@�I���@FALSE�@�p��
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	BOOL C3D_MOVIE_CAMERA::doLocSpline(void)
	{
	//	�I������
		//	�܂��I�����Ă��Ȃ��ꍇ
		if(this->fCameraLocMedia < (float)(this->iCameraLocVertex - 1))
		{
			VEC3 vLoc;	//�X�V���钸�_�ʒu�̕ۑ��p

		//	�X�v���C���v�Z���ʂ��擾����
			vLoc.x = this->vCameraLoc.x.getSplineCalc(this->fCameraLocMedia);
			vLoc.y = this->vCameraLoc.y.getSplineCalc(this->fCameraLocMedia);
			vLoc.z = this->vCameraLoc.z.getSplineCalc(this->fCameraLocMedia);

		//	�ʒu���X�V����
			this->setLoc(&vLoc);

		//	�}��ϐ��𑝂₷
			this->fCameraLocMedia += this->fCameraLocSpeed;
			
			return FALSE;
		}
		//	�I�����Ă���ꍇ
		else
		{
		//	���[�v����
			//	���[�v���s��
			if(this->bCameraLocLoop)
			{
				this->fCameraLocMedia = 0.0f;
				return FALSE;
			}
			//	���[�v���s��Ȃ�
			else
			{
				return TRUE;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_MOVIE_CAMERA::setViewPointSpline
	//
	//	�@�\		�J�������_�̓����̐ݒ�(�X�v���C���Ȑ������Ă�)
	//	����		*inLoc		���_�z��
	//				inVertex	���_��
	//				inLoop		���[�v�t���O
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	void C3D_MOVIE_CAMERA::setViewPointSpline(D3DXVECTOR3 *inLoc, int inVertex, BOOL inLoop)
	{
		float fLocX[MaxSplineSize];
		float fLocY[MaxSplineSize];
		float fLocZ[MaxSplineSize];

		for(int i = 0; i < inVertex; ++i)
		{
			fLocX[i] = inLoc[i].x;
			fLocY[i] = inLoc[i].y;
			fLocZ[i] = inLoc[i].z;
		}

	//	�X�v���C���̐ݒ�
		this->vCameraViewPoint.x.setSplineDate(fLocX, inVertex);	//���_X
		this->vCameraViewPoint.y.setSplineDate(fLocY, inVertex);	//���_Y
		this->vCameraViewPoint.z.setSplineDate(fLocZ, inVertex);	//���_Z
		this->bCameraViewPointLoop = inLoop;						//���[�v�t���O�̋L�^
		this->iCameraViewPointVertex = inVertex;					//���_��
	}

	//================================================================================
	//!	���\�b�h��	C3D_MOVIE_CAMERA::setViewPointSpline
	//
	//	�@�\		�J�������_�̓����̐ݒ�(�X�v���C���Ȑ������Ă�)
	//	����		*inLoc		���_�z��
	//				inVertex	���_��
	//				inLoop		���[�v�t���O
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	BOOL C3D_MOVIE_CAMERA::doViewPointSpline(void)
	{
		//	�I������
		//	�܂��I�����Ă��Ȃ��ꍇ
		if(this->fCameraViewPointMedia < (float)(this->iCameraViewPointVertex - 1))
		{
			VEC3 vLoc;	//�X�V���钸�_�ʒu�̕ۑ��p

		//	�X�v���C���v�Z���ʂ��擾����
			vLoc.x = this->vCameraViewPoint.x.getSplineCalc(this->fCameraViewPointMedia);
			vLoc.y = this->vCameraViewPoint.y.getSplineCalc(this->fCameraViewPointMedia);
			vLoc.z = this->vCameraViewPoint.z.getSplineCalc(this->fCameraViewPointMedia);

		//	�ʒu���X�V����
			this->setViewPoint(&vLoc);

		//	�}��ϐ��𑝂₷
			this->fCameraViewPointMedia += this->fCameraViewPointSpeed;
			
			return FALSE;
		}
		//	�I�����Ă���ꍇ
		else
		{
		//	���[�v����
			//	���[�v���s��
			if(this->bCameraViewPointLoop)
			{
				this->fCameraViewPointMedia = 0.0f;
				return FALSE;
			}
			//	���[�v���s��Ȃ�
			else
			{
				return TRUE;
			}
		}
	}
}