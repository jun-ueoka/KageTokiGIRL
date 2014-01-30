//***********************************************************************************
//!	�t�@�C����		Class_Scene.h
//
//	�@�\			�V�[����������N���X�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/04/21	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	<windows.h>
#include	"Version.h"
#include	"Class_DirectInput.h"

//-----------------------------------------------------------------------
//!	�V�[������t���O
//-----------------------------------------------------------------------
typedef	enum APP_SCENE_FLAG
{
	//�I�[�v�j���O�t���[
	eSCENE_NO = 0,						//!< ���߂Ȃ�
	eSCENE_LOGO,						//!< ���S
	eSCENE_DEMO,						//!< �f��
	eSCENE_TITLE,						//!< �^�C�g��
	eSCENE_OPTION,						//!< �I�v�V����

	//�`���v�^�[1[��̏W�c]
	eSCENE_CHAPTER1_MOVIE1,				//!< ���[�r�[1[�f�[�g]
	eSCENE_CHAPTER1_STAGE1,				//!< �X�e�[�W1[����]
	eSCENE_CHAPTER1_MOVIE2,				//!< ���[�r�[2[�V���h�E�N���~�i���A����]
	eSCENE_CHAPTER1_STAGE2,				//!< �X�e�[�W2[�����o��]
	eSCENE_CHAPTER1_MOVIE3,				//!< ���[�r�[3[�V���h�E�N���~�i���A�W�g]
	eSCENE_CHAPTER1_STAGE3,				//!< �X�e�[�W3[��]
	eSCENE_CHAPTER1_BOSS1,				//!< �{�X1[�G�N�X]

	//�`���v�^�[2[�V���h�E�N���~�i����ǂ�!]
	eSCENE_CHAPTER2_MOVIE4,				//!< ���[�r�[4[�G��ǂ��āc]
	eSCENE_CHAPTER2_STAGE4,				//!< �X�e�[�W4[�r��]
	eSCENE_CHAPTER2_MOVIE5,				//!< ���[�r�[5[�R����c�J����c]
	eSCENE_CHAPTER2_STAGE5,				//!< �X�e�[�W5[�R]
	eSCENE_CHAPTER2_MOVIE6,				//!< ���[�r�[6[�A�W�g?]
	eSCENE_CHAPTER2_STAGE6,				//!< �X�e�[�W6[���A]
	eSCENE_CHAPTER2_BOSS2,				//!< �{�X2[�J�Q���}]

	//�`���v�^�[3[�A�W�g�}��]
	eSCENE_CHAPTER3_MOVIE7,				//!< ���[�r�[7[��������i]
	eSCENE_CHAPTER3_STAGE7,				//!< �X�e�[�W7[���]
	eSCENE_CHAPTER3_BOSS3,				//!< �{�X3[�t�F�U�[]
	eSCENE_CHAPTER3_MOVIE8,				//!< ���[�r�[8[�{���n����]
	eSCENE_CHAPTER3_STAGE8,				//!< �X�e�[�W8[�鉺��]
	eSCENE_CHAPTER3_MOVIE9,				//!< ���[�r�[9[��͒@�����A�󂵂Ă�!]
	eSCENE_CHAPTER3_STAGE9,				//!< �X�e�[�W9[�����]

	//���X�g�`���v�^�[[�N���C�}�b�N�X]
	eSCENE_LAST_CHAPTER_MOVIE10,		//!< ���[�r�[10[�ǂ��l�߂���!]
	eSCENE_LAST_CHAPTER_STAGE10,		//!< �X�e�[�W10[����n�����{]
	eSCENE_LAST_CHAPTER_BOSS4,			//!< �{�X4[�e�B�}]
	eSCENE_LAST_CHAPTER_MOVIE11,		//!< ���[�r�[11[�ŏI����!]
	eSCENE_LAST_CHAPTER_LAST_BOSS,		//!< ���X�{�X[�N���C��]
	eSCENE_LAST_CHAPTER_ENDING_MOVIE,	//!< �G���f�B���O���[�r�[[���a�̖K��]

	//�G���f�B���O�t���[
	eSCENE_ENDING,						//!< �G���f�B���O
	eSCENE_GAMEOVER,					//!< �Q�[���I�[�o�[

	eSCENE_QUIT,						//!< �Q�[���I��
	eSCENE_ERROR,						//!< �Q�[���G���[
	eSCENE_MAX							//!< �t���O�̍ő吔
}	SCENE_FLAG;

//-----------------------------------------------------------------------
//	�f�o�b�O�p�t���O
//-----------------------------------------------------------------------
#define	START_FLAG			eSCENE_LOGO
#ifdef	_DEBUG
#define	DEBUG_FLAG			eSCENE_CHAPTER1_STAGE1
#endif	//_DEBUG

#ifdef	PRESENTATION
#define	PRESENTATION_FLAG	eSCENE_LOGO
#endif


//-----------------------------------------------------------------------
//!	�e�V�[���ŋ��L������
//-----------------------------------------------------------------------
struct	SCENE_SHARE_DATA
{
	DWORD	dwGameLevel;			//!< �Q�[�����x��
	DWORD	dwPlayerLife;			//!< �c�C
};

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CSCENE
	//
	//	�@�\			�V�[�����Ǘ�����N���X
	//	�X�V			2008/04/21	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE
	{
	protected:		//-------------------- protected --------------------//

				SCENE_FLAG			returnFlag;				//!< ���^�[���t���O
				SCENE_FLAG			nextFlag;				//!< ���V�[���t���O
				SCENE_SHARE_DATA*	ssData;					//!< �V�[�����L�f�[�^

	public:			//-------------------- public --------------------//

				CSCENE(SCENE_FLAG inFlag, SCENE_FLAG inNextFlag, SCENE_SHARE_DATA* inData)
				{
					returnFlag	= inFlag;
					nextFlag	= inNextFlag;
					ssData		= inData;
				}
		virtual	~CSCENE(void)	{}

		virtual	void	sceneInitialize(int)		= 0;	//!< ������
		virtual	void	sceneFinishalize(void)		= 0;	//!< ���

		virtual	SCENE_FLAG		sceneMain(CINPUT* input)	= 0;	//!< �V�[�����C��
	};
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------