//***********************************************************************************
//!	�t�@�C����		Class_SceneBoss3.cpp
//
//	�@�\			�Q�[���V�[���N���X�A�{�X�X�e�[�W1�u�G�N�X�v������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2009/02/15	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneBoss3.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS3::CSCENE_BOSS3
	//
	//	�@�\		�R���X�g���N�^
	//	����		inData		�V�[�����L�f�[�^
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	CSCENE_BOSS3::CSCENE_BOSS3(SCENE_SHARE_DATA* inData)	:
	//!	�Q�[���V�[��
	CSCENE_ADVENTURE_GAME(eSCENE_CHAPTER3_BOSS3,
						  eSCENE_CHAPTER3_MOVIE8,
						  eSCENE_GAMEOVER, inData)
	{
		this->eventStepCnt	= 0;
		this->bossFeather	= NULL;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS3::~CSCENE_BOSS3
	//
	//	�@�\		�f�X�g���N�^
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	CSCENE_BOSS3::~CSCENE_BOSS3(void)
	{
	//	���
		this->sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS3::scenecfdInitialize
	//
	//	�@�\		������
	//	����		inStageNum	�O���f�[�^�ԍ�
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	void
	CSCENE_BOSS3::sceneInitialize(int inStageNum)
	{
	//	�f���p�̃X�e�[�W�ԍ����擾
		inStageNum	= 130;

	//	�Q�[���V�[���̏�����
		CSCENE_ADVENTURE_GAME::sceneInitialize(inStageNum);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS3::sceneFinishalize
	//
	//	�@�\		���
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	void
	CSCENE_BOSS3::sceneFinishalize(void)
	{
	//	���
		SAFE_DELETE(this->bossFeather);

	//	�Q�[���V�[���̊J��
		CSCENE_ADVENTURE_GAME::sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_BOSS3::sceneMain
	//
	//	�@�\		�V�[�����C��
	//	�@�\�T�v	�Q�[���V�[���̃��\�b�h���g���A�f�����`��
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	SCENE_FLAG
	CSCENE_BOSS3::sceneMain(CINPUT* input)
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
	//!	���\�b�h��	CSCENE_BOSS3::updateLight
	//
	//	�@�\		���C�g�̍X�V
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	void
	CSCENE_BOSS3::updateExecute(void)
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
					this->bossFeather->operation(NULL);
				//	�{�X�̗̑͂��Ȃ���΁A�N���A
					if(	this->bossFeather->getLife() <= 0)
					{
					//	3002�Ԗڂ̃C�x���g�𔭐�
						this->sgPlayOpenEventFile(3002);
						this->eventStepCnt	= 3002;
					}
				}
			//	�C�x���g�X�e�b�v�J�E���^��3002�̏�Ԃ̎�
				else
				if(	this->eventStepCnt == 3002)
				{
				//	�{�X�̓���
					this->bossFeather->operation(NULL);
				//	3003�Ԗڂ̃C�x���g�𔭐�
					this->sgPlayOpenEventFile(3003);
					this->eventStepCnt	= 3003;
					this->bossFeather->changeAction(eBFEATHERA_DISAPPEAR);
				}
			//	�C�x���g�X�e�b�v�J�E���^��3003�̏�Ԃ̎�
				else
				if(	this->eventStepCnt == 3003)
				{
				//	�{�X�̓���
					this->bossFeather->operation(NULL);
				//	0�Ԗڂ̃C�x���g�𔭐�
					this->sgPlayOpenEventFile(0);
					this->eventStepCnt	= -1;
				}
				break;
		//	�Q�[���C�x���g��
			case	eSGFLAG_EVENT:
			//	�{�X�̓���
				this->bossFeather->operation(NULL);
				break;
		}

	//	�Q�[���X�e�b�v�t���O�Œǉ���������
		switch(this->gameStep)
		{
		//	�Q�[���v���C���Ɉڍs���ꂽ��
			case	eSGTSTEP_CHANGE_PLAY:
				this->bossFeather->changeAction(eBFEATHERA_WAIT);
				this->bossFeather->setDrawInterface(TRUE);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		//	�Q�[���C�x���g���Ɉڍs���ꂽ��
			case	eSGTSTEP_CHANGE_EVENT:
				this->bossFeather->setDrawInterface(FALSE);
				this->gameStep	= eSGTSTEP_END_PROC;
				break;
		//	�R���e�B�j���[���Ɉڍs���ꂽ��
			case	eSGTSTEP_CHANGE_CONTINUE:
				this->bossFeather->setDrawInterface(FALSE);
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
	//!	���\�b�h��	CSCENE_BOSS3::sgCreateEnemy
	//
	//	�@�\		�G����
	//	�@�\�T�v	�G�𐶐����A�}�b�v�ɓo�^����
	//	����		inStageNum		�X�e�[�W�ԍ�
	//	�X�V		2009/01/12		<�V�K>
	//================================================================================
	void
	CSCENE_BOSS3::sgCreateEnemy(int inStageNum)
	{
	//	�G�N�X�𐶐�����
		if(	!this->bossFeather)
		{
			this->bossFeather	= new CBOSS_FEATHER(this->gDev->getDevice());
		}
		
	//	�G�N�X����������Ă�����A�ʒu����������
		if(	this->bossFeather)
		{
			this->bossFeather->setLoc(&VEC3(1500.0f, 4000.0f, 0.0f));
			this->bossFeather->setViewG(this->gameMap->getViewG());
			this->bossFeather->setStageG(this->gameMap->getHitG());
			this->bossFeather->setEnemyG(this->gameMap->getEnemyG());
			this->bossFeather->setTargetPlayer(&this->playerChara);
			this->bossFeather->setVisible(TRUE);
			this->bossFeather->resetParameter();
		}

	//	�G�N�X��G�O���[�v�ɓo�^
		this->gameMap->addEnemyG(bossFeather);
		this->gameMap->setEnemyGDistance(20000.0f);
		this->camera->setSecondTarget(bossFeather);
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------