//***********************************************************************************
//!	�t�@�C����		Class_BackSky.h
//
//	�@�\			�`���[�g���A���̔w�i�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/09/26	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_Back.h"				//!< �w�i�̃X�[�p�[�N���X

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CBACK_SKY
	//
	//	�@�\			�`���[�g���A���̔w�i�̒�`
	//	�X�V			2008/09/26	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CBACK_SKY : public CBACK
	{
			//==================================================================
			//	�`���[�g���A���w�i��p�}�N��
			//==================================================================
				#define	NAME_SKY			"kage_stage_doom"		//!< �w�i���b�V����

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	�w�i�X�N���[���֘A
			//==================================================================
				CMESH*	sky;										//!< �󃁃b�V��

	public:			//-------------------- public --------------------//

				CBACK_SKY(CONST DEV inDev, VEC3* inViewPoint);		//!< �R���X�g���N�^
		virtual	~CBACK_SKY(void);									//!< �f�X�g���N�^

		virtual	void	motion(VEC3* inViewPoint);					//!< �w�i����
		virtual	BOOL	drawBack(CONST DEV, CONST SPRITE);			//!< �w�i�`��
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------