//***********************************************************************************
//!	�t�@�C����		Class_SceneEnding.h
//
//	�@�\			�G���f�B���O�V�[���̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2008/10/05	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"GameCommon.h"					//!< �Q�[���R����
#include	"Class_SceneMovieBase.h"		//!< �V�[��
#include	"Class_Sound_OggVorbis.h"		//!< ogg���C�u����(�T�E���h�𔭐��������)

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CSCENE_ENDING
	//
	//	�@�\			�G���f�B���O�V�[���̃N���X
	//	�X�V			2008/10/05	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_ENDING : public CSCENE_MOVIE_BASE
	{
			//==================================================================
			//	�}�N���Q
			//==================================================================
				#define	ROLL_SPEED			2					//!< ���[���X�s�[�h

	private:		//-------------------- private --------------------//

			//==================================================================
			//	���o����
			//==================================================================
				C3D_OBJECT_CAMERA*	camera;						//!< �J����
				CSKIN_MESH*			kageo;						//!< �J�Q�I
				CSKIN_MESH*			kagemi;						//!< �J�Q�~
				CGAME_MAP*			drawMap;					//!< �`��}�b�v
				CGAME_MAP*			objMap1;					//!< �I�u�W�F�N�g�}�b�v1
				CGAME_MAP*			objMap2;					//!< �I�u�W�F�N�g�}�b�v2
				CGAME_MAP*			objMap3;					//!< �I�u�W�F�N�g�}�b�v3
				CGAME_MAP*			objMap4;					//!< �I�u�W�F�N�g�}�b�v4
				CBACK*				drawBack;					//!< �`��w�i
				CBACK*				back1;						//!< �w�i1

				MY_COLOR_RGBA		curtainColor;				//!< ���̐F
				SSTREAMING*			sStreamingBgm;				//!< BGM

				CZTEX_SHA*			shaderZTexture;				//!< Z�e�N�X�`���V�F�[�_
				CDEPTH_SHA*			shaderDepthBuf;				//!< �[�x�o�b�t�@�V�F�[�_

				MAT					lightView;					//!< ���C�g�r���[�s��
				MAT					lightProj;					//!< ���C�g�ˉe�s��
				VEC3				lightShift;					//!< ���_����̃��C�g�r���[�̈ʒu
				VEC2				lightNearFar;				//!< ���C�g�̉����ߎ�

			//==================================================================
			//	�X�^�b�t���[���֘A
			//==================================================================
				int						iRoll;					//!< ���[���l
				vector<FONT_STRING>		staffRoll;				//!< ���[��������
				void	motionStaffRoll(void);					//!< �X�^�b�t���[����ʏ���
				void	renderStaffRoll(void);					//!< �X�^�b�t���[����ʕ`��

	protected:		//-------------------- protected --------------------//

				void	executeInstantChengeMovieSpeed(void);	//!< ���[�r�[�������x�ύX������
				void	executeProcOnce(int inProc);			//!< �����������s
				void	executeProc(int inProc);				//!< �������s
				void	updateLight(void);						//!< ���C�g�̍X�V

	public:			//-------------------- public --------------------//

				CSCENE_ENDING(SCENE_SHARE_DATA* inData);
		virtual	~CSCENE_ENDING(void);

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