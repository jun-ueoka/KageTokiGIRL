//***********************************************************************************
//!	�t�@�C����		Class_SceneStage4.cpp
//
//	�@�\			�Q�[���V�[���N���X�A�X�e�[�W4�u���v������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2009/01/27	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneStage4.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_STAGE4::CSCENE_STAGE4
	//
	//	�@�\		�R���X�g���N�^
	//	����		inData		�V�[�����L�f�[�^
	//	�X�V		2009/01/27	<�V�K>
	//================================================================================
	CSCENE_STAGE4::CSCENE_STAGE4(SCENE_SHARE_DATA* inData)	:
	//!	�Q�[���V�[��
	CSCENE_ADVENTURE_GAME(eSCENE_CHAPTER2_STAGE4, eSCENE_CHAPTER2_MOVIE5,
						  eSCENE_GAMEOVER, inData)
	{
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_STAGE4::~CSCENE_STAGE4
	//
	//	�@�\		�f�X�g���N�^
	//	�X�V		2009/01/27	<�V�K>
	//================================================================================
	CSCENE_STAGE4::~CSCENE_STAGE4(void)
	{
	//	���
		this->sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_STAGE4::scenecfdInitialize
	//
	//	�@�\		������
	//	����		inStageNum	�O���f�[�^�ԍ�
	//	�X�V		2009/01/27	<�V�K>
	//================================================================================
	void
	CSCENE_STAGE4::sceneInitialize(int inStageNum)
	{
	//	�f���p�̃X�e�[�W�ԍ����擾
		inStageNum	= 40;

	//	�Q�[���V�[���̏�����
		CSCENE_ADVENTURE_GAME::sceneInitialize(inStageNum);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_STAGE4::sceneFinishalize
	//
	//	�@�\		���
	//	�X�V		2009/01/27	<�V�K>
	//================================================================================
	void
	CSCENE_STAGE4::sceneFinishalize(void)
	{
	//	�Q�[���V�[���̊J��
		CSCENE_ADVENTURE_GAME::sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_STAGE4::sceneMain
	//
	//	�@�\		�V�[�����C��
	//	�@�\�T�v	�Q�[���V�[���̃��\�b�h���g���A�f�����`��
	//	�X�V		2009/01/27	<�V�K>
	//================================================================================
	SCENE_FLAG
	CSCENE_STAGE4::sceneMain(CINPUT* input)
	{
#ifdef	_DEBUG
		if(	input->checkPress(MYKEY::eF3))
		{
			if(	input->checkPress(MYKEY::eF5))
			{
				if(	input->checkPress(MYKEY::eUP))		this->lightNearFar.x	+= 50.0f;
				if(	input->checkPress(MYKEY::eDOWN))	this->lightNearFar.x	-= 50.0f;
			}
			if(	input->checkPress(MYKEY::eF6))
			{
				if(	input->checkPress(MYKEY::eUP))		this->lightNearFar.y	+= 50.0f;
				if(	input->checkPress(MYKEY::eDOWN))	this->lightNearFar.y	-= 50.0f;
			}
		}
#endif

	//	�Q�[���t���O�ŏ�������
		switch(this->gameFlag)
		{
		//	�Q�[���v���C��
			case	eSGFLAG_PLAY:
				this->sgPlayExecute(input);
				this->updateExecute();
				this->sgPlayRender();
				break;
		//	�Q�[���C�x���g��
			case	eSGFLAG_EVENT:
				this->sgEventExecute(input);
				this->updateExecute();
				this->sgEventRender();
				break;
		//	�R���e�B�j���[��
			case	eSGFLAG_CONTINUE:
				this->sgContinueExecute(input);
				this->updateExecute();
				this->sgContinueRender();
				break;
		//	�}�b�v�`�F���W��
			case	eSGFLAG_STAGE_CHANGE:
				this->sgStageChangeExecute(input);
				this->updateExecute();
				this->sgStageChangeRender();
				break;
		//	�ăX�^�[�g��
			case	eSGFLAG_RESET:
				this->sgResetExecute(input);
				this->updateExecute();
				this->sgResetRender();
				break;
		//	�Q�[���N���A��
			case	eSGFLAG_CLEAR:
				this->sgClearExecute(input);
				this->updateExecute();
				this->sgClearRender();
				break;
		//	�Q�[���I����
			case	eSGFLAG_QUIT:
				this->sgQuitExecute(input);
				this->updateExecute();
				this->sgQuitRender();
				break;
		//	�Q�[�����G���[
			case	eSGFLAG_ERROR:
				this->returnFlag	= eSCENE_ERROR;
				break;
		//	��L�ȊO
			default:
				break;
		}
		return	this->returnFlag;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_STAGE4::updateExecute
	//
	//	�@�\		�X�V
	//	�X�V		2009/01/27	<�V�K>
	//================================================================================
	void
	CSCENE_STAGE4::updateExecute(void)
	{
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------