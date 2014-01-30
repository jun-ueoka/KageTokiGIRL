//***********************************************************************************
//!	�t�@�C����		Class_SceneMovie3.h
//
//	�@�\			���[�r�[�V�[���N���X�A���[�r�[3[�V���h�E�N���~�i���A�W�g]�̒�`
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
	//!	�`��V�[���؂�ւ��t���O
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum MOVIE3_FLAG
	{
		eM3F_FOREST,			//!< �X
		eM3F_CAVE,				//!< ���A
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CSCENE_MOVIE3
	//
	//	�@�\			���[�r�[�V�[���N���X�A���[�r�[3[�V���h�E�N���~�i���A�W�g]�̃N���X
	//	�X�V			2009/02/10	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_MOVIE3 : public CSCENE_MOVIE_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	���o����
			//==================================================================
				C3D_OBJECT_CAMERA*	camera;						//!< �J����
				C3D_OBJECT*			cameraTarget;				//!< �J�����^�[�Q�b�g�p�I�u�W�F�N�g

				CSKIN_MESH*			kageo;						//!< �J�Q�I
				CSKIN_MESH*			kagemi;						//!< �J�Q�~
				CSKIN_MESH*			shabird;					//!< �V���o�[�h
				CSKIN_MESH*			clime;						//!< �N���C��
				CSKIN_MESH*			tima;						//!< �e�B�}
				CSKIN_MESH*			feather;					//!< �t�F�U�[
				CSKIN_MESH*			yamakage;					//!< ���}�J�Q
				CSKIN_MESH*			exe;						//!< �G�N�X

				CGAME_MAP*			objMapForest;				//!< �I�u�W�F�N�g�}�b�v�X
				CGAME_MAP*			objMapCave;					//!< �I�u�W�F�N�g�}�b�v���A
				CBACK*				back;						//!< �w�i
				CMESH*				cave;						//!< ���A

				MOVIE3_FLAG			movieFlag;					//!< ���[�r�[�t���O

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

				CSCENE_MOVIE3(SCENE_SHARE_DATA* inData);
		virtual	~CSCENE_MOVIE3(void);

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