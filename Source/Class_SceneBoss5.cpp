//***********************************************************************************
//!	�t�@�C����		Class_SceneBoss5.cpp
//
//	�@�\			�Q�[���V�[���N���X�A�{�X�X�e�[�W5�u�N���C���v������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2009/02/15	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneBoss5.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS5::CSCENE_BOSS5
	//
	//	�@�\		�R���X�g���N�^
	//	����		inData		�V�[�����L�f�[�^
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	CSCENE_BOSS5::CSCENE_BOSS5(SCENE_SHARE_DATA* inData)	:
	//!	�Q�[���V�[��
	CSCENE_ADVENTURE_GAME(eSCENE_LAST_CHAPTER_LAST_BOSS, eSCENE_LAST_CHAPTER_ENDING_MOVIE,
						  eSCENE_GAMEOVER, inData)
	{
		this->eventStepCnt	= 0;
		this->bossExe		= NULL;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS5::~CSCENE_BOSS5
	//
	//	�@�\		�f�X�g���N�^
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	CSCENE_BOSS5::~CSCENE_BOSS5(void)
	{
	//	���
		this->sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS5::scenecfdInitialize
	//
	//	�@�\		������
	//	����		inStageNum	�O���f�[�^�ԍ�
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	void
	CSCENE_BOSS5::sceneInitialize(int inStageNum)
	{
	//	�f���p�̃X�e�[�W�ԍ����擾
		inStageNum	= 150;

	//	�Q�[���V�[���̏�����
		CSCENE_ADVENTURE_GAME::sceneInitialize(inStageNum);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS5::sceneFinishalize
	//
	//	�@�\		���
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	void
	CSCENE_BOSS5::sceneFinishalize(void)
	{
	//	���
		SAFE_DELETE(this->bossExe);

	//	�Q�[���V�[���̊J��
		CSCENE_ADVENTURE_GAME::sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS5::sceneMain
	//
	//	�@�\		�V�[�����C��
	//	�@�\�T�v	�Q�[���V�[���̃��\�b�h���g���A�f�����`��
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	SCENE_FLAG
	CSCENE_BOSS5::sceneMain(CINPUT* input)
	{
	//	�Q�[���t���O�ŏ�������
		switch(this->gameFlag)
		{
		//	�Q�[���v���C��
			case	eSGFLAG_PLAY:
			//	�C�x���g�X�e�b�v�J�E���^��0�ȉ��̎��͒ʏ퓮��
				if(	this->eventStepCnt <= 0)
				{
				//	�{�X�̓���
					this->bossExe->operation(NULL);
					this->sgPlayExecute(input);
					this->updateExecute();
					this->sgPlayRender();
				//	�{�X�̗̑͂��Ȃ���΁A�N���A
					if(	this->bossExe->getLife() <= 0)
					{
					//	1002�Ԗڂ̃C�x���g�𔭐�
						this->sgPlayOpenEventFile(1002);
						this->eventStepCnt	= 1002;
						this->bossExe->changeAction(eBEXEA_APPEAR);
					}
				}
				else
			//	�C�x���g�X�e�b�v�J�E���^��1002�̏�Ԃł����܂ŗ�����
				if(	this->eventStepCnt == 1002)
				{
				//	�{�X����֔�΂�
					this->bossExe->calcMoveQuantity(10.0f);
					this->bossExe->moveUp();
					this->sgPlayExecute(input);
					this->updateExecute();
					this->sgPlayRender();
				//	�{�X����ѐ؂�����A�S�[��
					if(	this->bossExe->getLocY() > 7000)
					{
					//	0�Ԃ̃C�x���g����
						this->sgPlayOpenEventFile(0);
						this->bossExe->setVisible(FALSE);
					}
				}
				break;
		//	�Q�[���C�x���g��
			case	eSGFLAG_EVENT:
			//	�{�X�̓���
				this->bossExe->operation(NULL);
				this->sgEventExecute(input);
				this->updateExecute();
				this->sgEventRender();
			//	��L�̏����ŃQ�[���v���C���ɕς������A
				if(	this->gameFlag == eSGFLAG_PLAY)
				{
					this->bossExe->changeAction(eBEXEA_WAIT);
				}
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
				this->sgPlayRender();
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
	//!	���\�b�h��	CSCENE_BOSS5::updateLight
	//
	//	�@�\		���C�g�̍X�V
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	void
	CSCENE_BOSS5::updateExecute(void)
	{
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS5::sgCreateEnemy
	//
	//	�@�\		�G����
	//	�@�\�T�v	�G�𐶐����A�}�b�v�ɓo�^����
	//	����		inStageNum		�X�e�[�W�ԍ�
	//	�X�V		2009/02/15		<�V�K>
	//================================================================================
	void
	CSCENE_BOSS5::sgCreateEnemy(int inStageNum)
	{
	//	�G�N�X�𐶐�����
		if(	!this->bossExe)
		{
			this->bossExe	= new CBOSS_EXE(this->gDev->getDevice());
		}
		
	//	�G�N�X����������Ă�����A�ʒu����������
		if(	this->bossExe)
		{
			this->bossExe->setLoc(&VEC3(1500.0f, 4000.0f, 0.0f));
			this->bossExe->setViewG(this->gameMap->getViewG());
			this->bossExe->setStageG(this->gameMap->getHitG());
			this->bossExe->setEnemyG(this->gameMap->getEnemyG());
			this->bossExe->setTargetPlayer(&this->playerChara);
		}

	//	�G�N�X��G�O���[�v�ɓo�^
		this->gameMap->addEnemyG(bossExe);
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------