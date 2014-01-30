//***********************************************************************************
//!	�t�@�C����		Class_SceneBoss1.cpp
//
//	�@�\			�Q�[���V�[���N���X�A�{�X�X�e�[�W1�u�G�N�X�v������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2009/01/12	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneBoss1.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS1::CSCENE_BOSS1
	//
	//	�@�\		�R���X�g���N�^
	//	����		inData		�V�[�����L�f�[�^
	//	�X�V		2009/01/12	<�V�K>
	//================================================================================
	CSCENE_BOSS1::CSCENE_BOSS1(SCENE_SHARE_DATA* inData)	:
	//!	�Q�[���V�[��
	CSCENE_ADVENTURE_GAME(eSCENE_CHAPTER1_BOSS1,
						  eSCENE_ENDING,
						  eSCENE_GAMEOVER, inData)
	{
		this->eventStepCnt	= 0;
		this->bossExe		= NULL;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS1::~CSCENE_BOSS1
	//
	//	�@�\		�f�X�g���N�^
	//	�X�V		2009/01/12	<�V�K>
	//================================================================================
	CSCENE_BOSS1::~CSCENE_BOSS1(void)
	{
	//	���
		this->sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS1::scenecfdInitialize
	//
	//	�@�\		������
	//	����		inStageNum	�O���f�[�^�ԍ�
	//	�X�V		2009/01/12	<�V�K>
	//================================================================================
	void
	CSCENE_BOSS1::sceneInitialize(int inStageNum)
	{
	//	�f���p�̃X�e�[�W�ԍ����擾
		inStageNum	= 110;

	//	�Q�[���V�[���̏�����
		CSCENE_ADVENTURE_GAME::sceneInitialize(inStageNum);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS1::sceneFinishalize
	//
	//	�@�\		���
	//	�X�V		2009/01/12	<�V�K>
	//================================================================================
	void
	CSCENE_BOSS1::sceneFinishalize(void)
	{
	//	���
		SAFE_DELETE(this->bossExe);

	//	�Q�[���V�[���̊J��
		CSCENE_ADVENTURE_GAME::sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS1::sceneMain
	//
	//	�@�\		�V�[�����C��
	//	�@�\�T�v	�Q�[���V�[���̃��\�b�h���g���A�f�����`��
	//	�X�V		2009/01/12	<�V�K>
	//================================================================================
	SCENE_FLAG
	CSCENE_BOSS1::sceneMain(CINPUT* input)
	{
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
	//!	���\�b�h��	CSCENE_BOSS1::updateLight
	//
	//	�@�\		���C�g�̍X�V
	//	�X�V		2009/01/12	<�V�K>
	//================================================================================
	void
	CSCENE_BOSS1::updateExecute(void)
	{
	//	�Q�[���t���O�Œǉ���������
		switch(this->gameFlag)
		{
		//	�Q�[���v���C��
			case	eSGFLAG_PLAY:
			//	�C�x���g�X�e�b�v�J�E���^��0�ȉ��̎��͒ʏ퓮��
				if(	this->eventStepCnt <= 0)
				{
				//	�{�X�̓���
					this->bossExe->operation(NULL);
				//	�{�X�̗̑͂��Ȃ���΁A�N���A
					if(	this->bossExe->getLife() <= 0)
					{
					//	1002�Ԗڂ̃C�x���g�𔭐�
						this->sgPlayOpenEventFile(1002);
						this->eventStepCnt	= 1002;
					}
				}
			//	�C�x���g�X�e�b�v�J�E���^��1002�̏�Ԃ̎�
				else
				if(	this->eventStepCnt == 1002)
				{
				//	�{�X�̓���
					this->bossExe->operation(NULL);
				//	1003�Ԗڂ̃C�x���g�𔭐�
					this->sgPlayOpenEventFile(1003);
					this->eventStepCnt	= 1003;
					this->bossExe->changeAction(eBEXEA_DISAPPEAR);
				}
			//	�C�x���g�X�e�b�v�J�E���^��1003�̏�Ԃ̎�
				else
				if(	this->eventStepCnt == 1003)
				{
				//	�{�X�̓���
					this->bossExe->operation(NULL);
				//	0�Ԗڂ̃C�x���g�𔭐�
					this->sgPlayOpenEventFile(0);
					this->eventStepCnt	= -1;
				}
				break;
		//	�Q�[���C�x���g��
			case	eSGFLAG_EVENT:
			//	�{�X�̓���
				this->bossExe->operation(NULL);
				break;
		}

	//	�Q�[���X�e�b�v�t���O�Œǉ���������
		switch(this->gameStep)
		{
		//	�Q�[���v���C���Ɉڍs���ꂽ��
			case	eSGTSTEP_CHANGE_PLAY:
				this->bossExe->changeAction(eBEXEA_WAIT);
				this->bossExe->setDrawInterface(TRUE);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		//	�Q�[���C�x���g���Ɉڍs���ꂽ��
			case	eSGTSTEP_CHANGE_EVENT:
				this->bossExe->setDrawInterface(FALSE);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		//	�R���e�B�j���[���Ɉڍs���ꂽ��
			case	eSGTSTEP_CHANGE_CONTINUE:
				this->bossExe->setDrawInterface(FALSE);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		//	�ăX�^�[�g���Ɉڍs���ꂽ��
			case	eSGTSTEP_CHANGE_RESET:
				this->sgLoadEventStartSwitch(this->eventStartSwitchNum);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS1::sgCreateEnemy
	//
	//	�@�\		�G����
	//	�@�\�T�v	�G�𐶐����A�}�b�v�ɓo�^����
	//	����		inStageNum		�X�e�[�W�ԍ�
	//	�X�V		2009/01/12		<�V�K>
	//================================================================================
	void
	CSCENE_BOSS1::sgCreateEnemy(int inStageNum)
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
			this->bossExe->changeAction(eBEXEA_APPEAR);
			this->bossExe->setViewG(this->gameMap->getViewG());
			this->bossExe->setStageG(this->gameMap->getHitG());
			this->bossExe->setEnemyG(this->gameMap->getEnemyG());
			this->bossExe->setTargetPlayer(&this->playerChara);
			this->bossExe->setVisible(TRUE);
			this->bossExe->resetParameter();
		}

	//	�G�N�X��G�O���[�v�ɓo�^
		this->gameMap->addEnemyG(bossExe);
		this->gameMap->setEnemyGDistance(20000.0f);
		this->camera->setSecondTarget(bossExe);
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------