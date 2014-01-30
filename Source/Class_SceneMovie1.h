//***********************************************************************************
//!	�t�@�C����		Class_SceneMovie1.h
//
//	�@�\			���[�r�[�V�[���N���X�A���[�r�[1[�f�[�g]�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2008/11/27	<�V�K>
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
	//!	�N���X��		CSCENE_MOVIE1
	//
	//	�@�\			���[�r�[�V�[���N���X�A���[�r�[1[�f�[�g]�̃N���X
	//	�X�V			2008/12/02	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_MOVIE1 : public CSCENE_MOVIE_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	���o����
			//==================================================================
				C3D_OBJECT_CAMERA*	camera;						//!< �J����
				CSKIN_MESH*			kageo;						//!< �J�Q�I
				CSKIN_MESH*			kagemi;						//!< �J�Q�~
				CGAME_MAP*			objMap;						//!< �I�u�W�F�N�g�}�b�v
				CBACK*				back;						//!< �w�i

				SSTREAMING*			backSound;					//!< �X�e�[�WBGM

				CZTEX_SHA*			shaderZTexture;				//!< Z�e�N�X�`���V�F�[�_
				CDEPTH_SHA*			shaderDepthBuf;				//!< �[�x�o�b�t�@�V�F�[�_

				MAT					lightView;					//!< ���C�g�r���[�s��
				MAT					lightProj;					//!< ���C�g�ˉe�s��
				VEC3				lightShift;					//!< ���_����̃��C�g�r���[�̈ʒu
				VEC2				lightNearFar;				//!< ���C�g�̉����ߎ�

	protected:		//-------------------- protected --------------------//

				void	executeInstantChengeMovieSpeed(void);	//!< ���[�r�[�������x�ύX������
				void	executeProcOnce(int inProc);			//!< �����������s
				void	executeProc(int inProc);				//!< �������s
				void	updateLight(void);						//!< ���C�g�̍X�V

	public:			//-------------------- public --------------------//

				CSCENE_MOVIE1(SCENE_SHARE_DATA* inData);
		virtual	~CSCENE_MOVIE1(void);

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