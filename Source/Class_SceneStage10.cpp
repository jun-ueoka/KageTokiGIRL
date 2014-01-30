//***********************************************************************************
//!	�t�@�C����		Class_SceneStage10.cpp
//
//	�@�\			�Q�[���V�[���N���X�A�X�e�[�W10�u����n���ʘH�v������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2009/01/27	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneStage10.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_STAGE10::CSCENE_STAGE10
	//
	//	�@�\		�R���X�g���N�^
	//	����		inData		�V�[�����L�f�[�^
	//	�X�V		2009/01/27	<�V�K>
	//================================================================================
	CSCENE_STAGE10::CSCENE_STAGE10(SCENE_SHARE_DATA* inData)	:
	//!	�Q�[���V�[��
	CSCENE_ADVENTURE_GAME(eSCENE_LAST_CHAPTER_STAGE10, eSCENE_LAST_CHAPTER_BOSS4,
						  eSCENE_GAMEOVER, inData)
	{
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_STAGE10::~CSCENE_STAGE10
	//
	//	�@�\		�f�X�g���N�^
	//	�X�V		2009/01/27	<�V�K>
	//================================================================================
	CSCENE_STAGE10::~CSCENE_STAGE10(void)
	{
	//	���
		sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_STAGE10::scenecfdInitialize
	//
	//	�@�\		������
	//	����		inStageNum	�O���f�[�^�ԍ�
	//	�X�V		2009/01/27	<�V�K>
	//================================================================================
	void
	CSCENE_STAGE10::sceneInitialize(int inStageNum)
	{
	//	�f���p�̃X�e�[�W�ԍ����擾
		inStageNum	= 100;

	//	�Q�[���V�[���̏�����
		CSCENE_ADVENTURE_GAME::sceneInitialize(inStageNum);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_STAGE10::sceneFinishalize
	//
	//	�@�\		���
	//	�X�V		2009/01/27	<�V�K>
	//================================================================================
	void
	CSCENE_STAGE10::sceneFinishalize(void)
	{
	//	�Q�[���V�[���̊J��
		CSCENE_ADVENTURE_GAME::sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_STAGE10::sceneMain
	//
	//	�@�\		�V�[�����C��
	//	�@�\�T�v	�Q�[���V�[���̃��\�b�h���g���A�f�����`��
	//	�X�V		2009/01/27	<�V�K>
	//================================================================================
	SCENE_FLAG
	CSCENE_STAGE10::sceneMain(CINPUT* input)
	{
#ifdef	_DEBUG
		if(	input->checkPress(MYKEY::eF3))
		{
			if(	input->checkPress(MYKEY::eF5))
			{
				if(	input->checkPress(MYKEY::eUP))		lightNearFar.x	+= 50.0f;
				if(	input->checkPress(MYKEY::eDOWN))	lightNearFar.x	-= 50.0f;
			}
			if(	input->checkPress(MYKEY::eF6))
			{
				if(	input->checkPress(MYKEY::eUP))		lightNearFar.y	+= 50.0f;
				if(	input->checkPress(MYKEY::eDOWN))	lightNearFar.y	-= 50.0f;
			}
		}
#endif

	//	�Q�[���t���O�ŏ�������
		switch(gameFlag)
		{
		//	�Q�[���v���C��
			case	eSGFLAG_PLAY:
				sgPlayExecute(input);
				updateExecute();
				sgPlayRender();
				break;
		//	�Q�[���C�x���g��
			case	eSGFLAG_EVENT:
				sgEventExecute(input);
				updateExecute();
				sgEventRender();
				break;
		//	�R���e�B�j���[��
			case	eSGFLAG_CONTINUE:
				sgContinueExecute(input);
				updateExecute();
				sgContinueRender();
				break;
		//	�}�b�v�`�F���W��
			case	eSGFLAG_STAGE_CHANGE:
				sgStageChangeExecute(input);
				updateExecute();
				sgStageChangeRender();
				break;
		//	�ăX�^�[�g��
			case	eSGFLAG_RESET:
				this->sgResetExecute(input);
				this->updateExecute();
				this->sgResetRender();
				break;
		//	�Q�[���N���A��
			case	eSGFLAG_CLEAR:
				sgClearExecute(input);
				updateExecute();
				sgClearRender();
				break;
		//	�Q�[���I����
			case	eSGFLAG_QUIT:
				sgQuitExecute(input);
				updateExecute();
				sgQuitRender();
				break;
		//	�Q�[�����G���[
			case	eSGFLAG_ERROR:
				returnFlag	= eSCENE_ERROR;
				break;
		//	��L�ȊO
			default:
				break;
		}
		return	returnFlag;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_STAGE10::updateExecute
	//
	//	�@�\		�X�V
	//	�X�V		2009/01/27	<�V�K>
	//================================================================================
	void
	CSCENE_STAGE10::updateExecute(void)
	{
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------