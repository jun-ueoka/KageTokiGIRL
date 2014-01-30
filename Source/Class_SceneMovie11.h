//***********************************************************************************
//!	�t�@�C����		Class_SceneMovie11.h
//
//	�@�\			���[�r�[�V�[���N���X�A���[�r�[11[��������i]�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2009/02/10	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"GameCommon.h"					//!< �Q�[���R����
#include	"Class_SceneMovieBase.h"		//!< �C�x���g�V�[��
#include	"Class_Sound_OggVorbis.h"		//!< ogg���C�u����(�T�E���h�𔭐��������)

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CSCENE_MOVIE11
	//
	//	�@�\			���[�r�[�V�[���N���X�A���[�r�[11[��������i]�̃N���X
	//	�X�V			2009/02/10	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_MOVIE11 : public CSCENE_MOVIE_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	���o����
			//==================================================================
				C3D_OBJECT_CAMERA*	camera;						//!< �J����
				CSKIN_MESH*			kageo;						//!< �J�Q�I
				CSKIN_MESH*			kagemi;						//!< �J�Q�~
				CGAME_MAP*			objMap;						//!< �I�u�W�F�N�g�}�b�v
				CTEXTURE*			sky;						//!< ��

				SSTREAMING*			backSound;					//!< �X�e�[�WBGM

				CZTEX_SHA*			shaderZTexture;				//!< Z�e�N�X�`���V�F�[�_
				CDEPTH_SHA*			shaderDepthBuf;				//!< �[�x�o�b�t�@�V�F�[�_

				MAT					lightView;					//!< ���C�g�r���[�s��
				MAT					lightProj;					//!< ���C�g�ˉe�s��
				VEC3				lightShift;					//!< ���_����̃��C�g�r���[�̈ʒu
				VEC2				lightNearFar;				//!< ���C�g�̉����ߎ�

	protected:		//-------------------- protected --------------------//

				void	executeProcOnce(int inProc);			//!< �����������s
				void	executeProc(int inProc);				//!< �������s
				void	updateLight(void);						//!< ���C�g�̍X�V

	public:			//-------------------- public --------------------//

				CSCENE_MOVIE11(SCENE_SHARE_DATA* inData);
		virtual	~CSCENE_MOVIE11(void);

				void		sceneInitialize(int inStageNum);	//!< ������
				void		sceneFinishalize(void);				//!< �J��

				SCENE_FLAG	sceneMain(CINPUT* input);			//!< �V�[�����C��

				void		execute(CINPUT* input);				//!< ����
				void		render(void);						//!< �`��
	};
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------