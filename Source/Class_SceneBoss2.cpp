//***********************************************************************************
//!	�t�@�C����		Class_SceneBoss2.cpp
//
//	�@�\			�Q�[���V�[���N���X�A�{�X�X�e�[�W2�u���}�J�Q�v������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2009/02/15	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneBoss2.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS2::CSCENE_BOSS2
	//
	//	�@�\		�R���X�g���N�^
	//	����		inData		�V�[�����L�f�[�^
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	CSCENE_BOSS2::CSCENE_BOSS2(SCENE_SHARE_DATA* inData)	:
	//!	�Q�[���V�[��
	CSCENE_ADVENTURE_GAME(eSCENE_CHAPTER2_BOSS2,
						  eSCENE_CHAPTER3_MOVIE7,
						  eSCENE_GAMEOVER, inData)
	{
		this->eventStepCnt	= 0;
		this->bossYamakage	= NULL;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS2::~CSCENE_BOSS2
	//
	//	�@�\		�f�X�g���N�^
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	CSCENE_BOSS2::~CSCENE_BOSS2(void)
	{
	//	���
		this->sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS2::scenecfdInitialize
	//
	//	�@�\		������
	//	����		inStageNum	�O���f�[�^�ԍ�
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	void
	CSCENE_BOSS2::sceneInitialize(int inStageNum)
	{
	//	�f���p�̃X�e�[�W�ԍ����擾
		inStageNum	= 120;

	//	�Q�[���V�[���̏�����
		CSCENE_ADVENTURE_GAME::sceneInitialize(inStageNum);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS2::sceneFinishalize
	//
	//	�@�\		���
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	void
	CSCENE_BOSS2::sceneFinishalize(void)
	{
	//	���
		SAFE_DELETE(this->bossYamakage);

	//	�Q�[���V�[���̊J��
		CSCENE_ADVENTURE_GAME::sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS2::sceneMain
	//
	//	�@�\		�V�[�����C��
	//	�@�\�T�v	�Q�[���V�[���̃��\�b�h���g���A�f�����`��
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	SCENE_FLAG
	CSCENE_BOSS2::sceneMain(CINPUT* input)
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
	//!	���\�b�h��	CSCENE_BOSS2::updateLight
	//
	//	�@�\		���C�g�̍X�V
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	void
	CSCENE_BOSS2::updateExecute(void)
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
					this->bossYamakage->operation(NULL);
				//	�{�X�̗̑͂��Ȃ���΁A�N���A
					if(	this->bossYamakage->getLife() <= 0)
					{
					//	2002�Ԗڂ̃C�x���g�𔭐�
						this->sgPlayOpenEventFile(2002);
						this->eventStepCnt	= 2002;
					}
				}
			//	�C�x���g�X�e�b�v�J�E���^��2002�̏�Ԃ̎�
				else
				if(	this->eventStepCnt == 2002)
				{
				//	�{�X�̓���
					this->bossYamakage->operation(NULL);
				//	2003�Ԗڂ̃C�x���g�𔭐�
					this->sgPlayOpenEventFile(2003);
					this->eventStepCnt	= 2003;
					this->bossYamakage->changeAction(eBYAMAKAGEA_DISAPPEAR);
				}
			//	�C�x���g�X�e�b�v�J�E���^��2003�̏�Ԃ̎�
				else
				if(	this->eventStepCnt == 2003)
				{
				//	�{�X�̓���
					this->bossYamakage->operation(NULL);
				//	0�Ԗڂ̃C�x���g�𔭐�
					this->sgPlayOpenEventFile(0);
					this->eventStepCnt	= -1;
				}
				break;
		//	�Q�[���C�x���g��
			case	eSGFLAG_EVENT:
			//	�{�X�̓���
				this->bossYamakage->operation(NULL);
				break;
		}

	//	�Q�[���X�e�b�v�t���O�Œǉ���������
		switch(this->gameStep)
		{
		//	�Q�[���v���C���Ɉڍs���ꂽ��
			case	eSGTSTEP_CHANGE_PLAY:
				this->bossYamakage->changeAction(eBYAMAKAGEA_WAIT);
				this->bossYamakage->setDrawInterface(TRUE);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		//	�Q�[���C�x���g���Ɉڍs���ꂽ��
			case	eSGTSTEP_CHANGE_EVENT:
				this->bossYamakage->setDrawInterface(FALSE);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		//	�R���e�B�j���[���Ɉڍs���ꂽ��
			case	eSGTSTEP_CHANGE_CONTINUE:
				this->bossYamakage->setDrawInterface(FALSE);
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
	//!	���\�b�h��	CSCENE_BOSS2::sgCreateEnemy
	//
	//	�@�\		�G����
	//	�@�\�T�v	�G�𐶐����A�}�b�v�ɓo�^����
	//	����		inStageNum		�X�e�[�W�ԍ�
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	void
	CSCENE_BOSS2::sgCreateEnemy(int inStageNum)
	{
	//	���}�J�Q�𐶐�����
		if(	!this->bossYamakage)
		{
			this->bossYamakage	= new CBOSS_YAMAKAGE(this->gDev->getDevice());
		}

	//	���}�J�Q����������Ă�����A�ʒu����������
		if(	this->bossYamakage)
		{
			this->bossYamakage->setLoc(&VEC3(1500.0f, 4000.0f, 0.0f));
			this->bossYamakage->changeAction(eBYAMAKAGEA_APPEAR);
			this->bossYamakage->setViewG(this->gameMap->getViewG());
			this->bossYamakage->setStageG(this->gameMap->getHitG());
			this->bossYamakage->setEnemyG(this->gameMap->getEnemyG());
			this->bossYamakage->setTargetPlayer(&this->playerChara);
			this->bossYamakage->setVisible(TRUE);
			this->bossYamakage->resetParameter();
		}

	//	�G�N�X��G�O���[�v�ɓo�^
		this->gameMap->addEnemyG(bossYamakage);
		this->gameMap->setEnemyGDistance(20000.0f);
		this->camera->setSecondTarget(bossYamakage);
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------