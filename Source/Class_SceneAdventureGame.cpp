//***********************************************************************************
//!	�t�@�C����		Class_SceneAdventureGame.cpp
//
//	�@�\			�Q�[���V�[���̏������A����A��������A�`�敪�򕔕��̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2008/07/10	<�V�K>
//					2008/11/06	<�ύX>	�S�̓I�ɏ�������
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneAdventureGame.h"
//#include	"Class_BackSky.h"		//!< ���_���E�w�i

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgInitializeGame
	//
	//	�@�\		�Q�[���̊J�n������
	//	�X�V		2008/11/06		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgInitializeGame(void)
	{
	//	�Q�[�����̏������
		this->sgFinishalizeGame();
	//	�Q�[�����̍\�z
	//	���N���X����
		this->gDev			= CGRAPHIC_DEVICE::getInstance();
		this->effectCenter	= C3D_EFFECT_CENTER::getInstance();
		this->effectCenter->finishalize();
		SAVER::CSAVE_RESOURCE::getInstance(gDev->getDevice());
	//	���̑��̏��������K�v�ȃQ�[�����
		this->playerChara	= NULL;
		this->player		= new CADVENT_PLAYER(this->gDev->getDevice(), &this->playerChara, this->ssData);
		this->camera		= new C3D_OBJECT_CAMERA(this->playerChara, TRUE, 0.0f, 0.0f, FALSE);
		this->gameMap		= new CGAME_MAP();
		this->enemyManager	= EM::CADVENT_ENEMY_MANAGER::getInstance(this->gDev->getDevice());

	//	�f�o�b�O����
#ifdef	_DEBUG
		this->fontBase		= CFONT_BASE::getInstance();
	//	�R�����g�p�̃t�H���g����������Ė�����΁A��������
		if(	!this->fontBase->getFontValid(eFONT_DEBUG))
		{
			this->fontBase->createFont(
				gDev->getDevice(),
				eFONT_DEBUG,
				"MS�S�V�b�N",
				16, 8, 10, 0);
		}
#else
#ifdef	PRESENTATION
		this->fontBase		= CFONT_BASE::getInstance();
	//	�R�����g�p�̃t�H���g����������Ė�����΁A��������
		if(	!this->fontBase->getFontValid(eFONT_DEBUG))
		{
			this->fontBase->createFont(
				gDev->getDevice(),
				eFONT_DEBUG,
				"MS�S�V�b�N",
				16, 8, 10, 0);
		}
#endif
#endif

	//	���̑��̏�����
	//	�J�����̃��[�h�ݒ�
		this->camera->setCameraMode(eCAMO_PARALLEL_CHASE);
	//	�v���C���[�Ƀ}�b�v�O���[�v��o�^
		this->player->setViewG(this->gameMap->getViewG());
		this->player->setStageG(this->gameMap->getHitG());
		this->player->setEnemyG(this->gameMap->getEnemyG());
	//	�v���C���[�ɃJ������o�^
		this->player->setCameraPointer(this->camera);

	//	Z�e�N�X�`���V�F�[�_�̐���
		this->shaderZTexture	= CZTEX_SHA::getInstance();
		this->shaderZTexture->initialize(
			this->gDev->getDevice(),
			this->gDev->getScreenSizeX(), 
			this->gDev->getScreenSizeY());

	//	�[�x�o�b�t�@�V�F�[�_�̐���
		this->shaderDepthBuf	= CDEPTH_SHA::getInstance();
		this->shaderDepthBuf->initialize(this->gDev->getDevice());
		this->shaderDepthBuf->setShadowMap(this->shaderZTexture->getZTex());

	//	�R���e�B�j���[�֘A����
		this->effectCenter->addEffects(
			EFFECT_CONTINUE_YES_SMOKE,
			new C3D_EFFECT(this->gDev->getDevice(), EFFECT_CONTINUE_YES_SMOKE, VEC3(0.0f,0.0f,0.0f)));
		this->effectCenter->addEffects(
			EFFECT_CONTINUE_NO_SMOKE,
			new C3D_EFFECT(this->gDev->getDevice(), EFFECT_CONTINUE_NO_SMOKE,  VEC3(0.0f,0.0f,0.0f)));
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgInitializeScript
	//
	//	�@�\		�X�N���v�g�̊J�n������
	//	�X�V		2008/11/06		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgInitializeScript(void)
	{
	//	�X�N���v�g���̏����J��
		this->sgFinishalizeScript();
	//	�X�N���v�g���̍\�z
		this->eventCommentG	= new CCOMMENT_GROUP();
		this->eventInput	= new CDUMMY_INPUT();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgFinishalizeGame
	//
	//	�@�\		�Q�[���̉��
	//	�X�V		2008/11/07		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgFinishalizeGame(void)
	{
	//	�Q�[�����̉��
		SAFE_DELETE(this->player);
		SAFE_DELETE(this->camera);
		SAFE_DELETE(this->gameMap);
		if(	this->enemyManager)
		{	this->enemyManager->releaseEnemy();	}

		SAFE_DELETE(this->back);
		SAFE_DELETE(this->backSound);
		this->stageChangeSwitch.clear();
		if(	this->effectCenter)
		{	this->effectCenter->finishalize();	}

		CZTEX_SHA::releaseInstance();
		CDEPTH_SHA::releaseInstance();
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;

	//	�R���e�B�j���[�֘A����
		SAFE_DELETE(this->effContinue);
		SAFE_DELETE(this->effContinueYes);
		SAFE_DELETE(this->effContinueNo);
		SAFE_DELETE(this->texInterface);

	//	�f�o�b�O����
#ifdef	_DEBUG
		if(	this->fontBase){
			this->fontBase	= NULL;
			CFONT_BASE::releaseInstance();
		}
#endif
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgFinishalizeScript
	//
	//	�@�\		�X�N���v�g�̉��
	//	�X�V		2008/11/07		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgFinishalizeScript(void)
	{
	//	�X�N���v�g���̉��
		if(	this->eventFile){	fclose(this->eventFile);	}
		this->eventStep		= eEVSTEP_FALSE;
		this->eventStartSwitch.clear();
		this->eventObject.clear();
		this->releaseEventExecuter();
		SAFE_DELETE(this->eventCommentG);
		SAFE_DELETE(this->eventInput);
		this->eventWaitKey	= MYKEY::eMAX;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgCreateStage
	//
	//	�@�\		�Q�[���X�e�[�W�̐���
	//	�@�\�T�v	��̃X�e�[�W����ǂݍ���
	//				�E�}�b�v�̐���				�E�����L�����N�^�[�ʒu�̐ݒ�
	//				�E�J�����̐ݒ�				�EBGM�̐ݒ�
	//				�E�C�x���g�X�C�b�`�̐ݒ�	�E�w�i�̐ݒ�
	//				�E���̑��̏�����
	//	����		inStageNum		�X�e�[�W�ԍ�
	//	�X�V		2008/11/06		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgCreateStage(int inStageNum)
	{
	//	�X�e�[�W�ԍ����m��
		this->iStageNum	= inStageNum;

	//-------------------------------------------------
	//	�t�@�C�����J��
	//-------------------------------------------------
		FILE*	stageFile			= NULL;		//!< �X�e�[�W�e�L�X�g�t�@�C���|�C���^�[
		char	szString[MAX_PATH]	= "";		//!< �e�L�X�g���쐬�p

	//	�e�L�X�g���𐶐�
		sprintf_s(szString, sizeof(szString), PATH_GAMESTAGE, this->iStageNum);
	//	�t�@�C�����J��
		fopen_s(&stageFile, szString, "r");

	//	�t�@�C�����J���Ȃ�������
		if(	!stageFile)
		{
		//	�ǂݍ��݂��s�킸�A������Ԃ�
			return;
		}

	//-------------------------------------------------
	//	�ǂݍ��݊J�n
	//-------------------------------------------------
	//	�ǂݍ��ݗp�ϐ��Q
		VEC3	vec3	= VEC3(0.0f,0.0f,0.0f);
		int		num		= 0;

	//-------------------------------------------------
	//	�v���C���[�J�n�ʒu
	//-------------------------------------------------
		fscanf_s(stageFile, "%f, %f, %f, ", &vec3.x, &vec3.y, &vec3.z);
		this->player->resetPlayer(&vec3);

	//-------------------------------------------------
	//	�J�����̏��
	//-------------------------------------------------
	//	�J�����̋ߎ���A������
		fscanf_s(stageFile, "%f, %f, ", &vec3.x, &vec3.y);
		this->camera->setViewNear(vec3.x);
		this->camera->setViewFar(vec3.y);
	//	���_�V�t�g�l
		fscanf_s(stageFile, "%f, %f, %f, ", &vec3.x, &vec3.y, &vec3.z);
		this->camera->setViewShift(&vec3);
	//	�ǐՑΏۂ���̃V�t�g�l
		fscanf_s(stageFile, "%f, %f, %f, ", &vec3.x, &vec3.y, &vec3.z);
		this->camera->setViewTargetShift(&vec3);

	//-------------------------------------------------
	//	���C�g�֘A
	//-------------------------------------------------
	//	���C�g�̃I�t�Z�b�g�l
		fscanf_s(stageFile, "%f, %f,     ", &lightNearFar.x, &lightNearFar.y);
		fscanf_s(stageFile, "%f, %f, %f, ", &lightShift.x,   &lightShift.y,   &lightShift.z);
	//	���C�g�ˉe�s��̐ݒ�
		D3DXMatrixPerspectiveFovLH(&this->lightProj, D3DXToRadian(90), 1.0f, lightNearFar.x, lightNearFar.y);

	//-------------------------------------------------
	//	�}�b�v�̓ǂݍ���
	//-------------------------------------------------
		fscanf_s(stageFile, "%d, ", &num);
		this->gameMap->initialize(this->gDev->getDevice(), num);

	//-------------------------------------------------
	//	�C�x���g�̓ǂݍ���
	//-------------------------------------------------
		fscanf_s(stageFile, "%d, ", &num);
		this->sgLoadEventStartSwitch(num);

	//-------------------------------------------------
	//	�o�b�N�T�E���h�̓ǂݍ���
	//-------------------------------------------------
		fscanf_s(stageFile, "%s ", szString, sizeof(szString));
		SAFE_DELETE(this->backSound);
		this->backSound	= new SSTREAMING(szString);

	//-------------------------------------------------
	//	�w�i�̓ǂݍ���
	//-------------------------------------------------
		fscanf_s(stageFile, "%d, ", &num);
		SAFE_DELETE(this->back);
		switch(num)
		{
		//	���_���E
			case	eSGBACK_SKY:
				this->back = new CBACK_SKY(this->gDev->getDevice(), &this->playerChara->getLoc());
				break;
		}//	default	break;

	//	�t�@�C�������
		fclose(stageFile);

	//-------------------------------------------------
	//	���̑��̋��ʏ�����
	//-------------------------------------------------
	//	�Q�[���v���C�֕ύX
		this->gameFlag	= eSGFLAG_NO;
		this->sgPlayChange();
	//	�V�F�[�_�̐ݒ�
		this->camera->viewTransform(this->gDev->getDevice());
	//	�G�𐶐�����
		this->enemyManager->loadEnemyFromFile(
			this->gDev->getDevice(), this->iStageNum, &this->playerChara,
			this->gameMap->getViewG(), this->gameMap->getHitG(), this->gameMap->getEnemyG());
		this->sgCreateEnemy(this->iStageNum);
	//	�o�b�N�T�E���h�Đ�
		this->backSound->play(0, DSBPLAY_LOOPING);
	//	�Q�[���ڍs���o����
	//	������������
		this->gDev->setCurtainColor(0, 0, 0);
		this->gDev->setCurtainColorAlpha(255);
	//	��������
		this->gDev->curtainMotion(FALSE, 5);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgLoadEventStartSwitch
	//
	//	�@�\		�X�N���v�g�̊J�n������
	//	����		inStageNum		�X�e�[�W�ԍ�
	//	�X�V		2008/11/06		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgLoadEventStartSwitch(int inStageNum)
	{
	//-------------------------------------------------
	//	�t�@�C�����J��
	//-------------------------------------------------
		FILE*	switchFile			= NULL;		//!< �X�e�[�W�e�L�X�g�t�@�C���|�C���^�[
		char	szString[MAX_PATH]	= "";		//!< �e�L�X�g���쐬�p

	//	�e�L�X�g���𐶐�
		sprintf_s(szString, sizeof(szString), PATH_EVENTSWITCH, this->iStageNum);
	//	�t�@�C�����J��
		fopen_s(&switchFile, szString, "r");

	//	�t�@�C�����J���Ȃ�������
		if(	!switchFile)
		{
		//	�ǂݍ��݂��s�킸�A������Ԃ�
			return;
		}

	//	�O�̃f�[�^���폜
		this->eventStartSwitch.clear();
		this->stageChangeSwitch.clear();

	//-------------------------------------------------
	//	�ǂݍ��݊J�n
	//-------------------------------------------------
		int		iEventNum	= 0;
		MY_CUBE	evPosition;

	//	����������������	�C�x���g�J�n�X�C�b�`��ǂݍ��ރ��[�v
		do
		{
			fscanf_s(switchFile, "%d, ", &iEventNum);
		//	�C�x���g�ԍ����s���Ȃ�I��
			if(	iEventNum < 0)	break;
		//	�ʒu��ǂݍ���
			fscanf_s(switchFile, "%f, %f, %f, %f, %f, %f, ",
				&evPosition.left,
				&evPosition.right,
				&evPosition.top,
				&evPosition.bottom,
				&evPosition.front,
				&evPosition.rear);
		//	�ʒu���}�b�v�ɓo�^
			this->eventStartSwitch.insert(pair<int, MY_CUBE>(iEventNum, evPosition));
		}
		while(1);
	//	����������������	�I�����߂�����܂ŁA�J��Ԃ�

	//	����������������	�X�e�[�W�`�F���W�X�C�b�`��ǂݍ��ރ��[�v
		do
		{
			fscanf_s(switchFile, "%d, ", &iEventNum);
		//	�C�x���g�ԍ����s���Ȃ�I��
			if(	iEventNum < 0)	break;
		//	�ʒu��ǂݍ���
			fscanf_s(switchFile, "%f, %f, %f, %f, %f, %f, ",
				&evPosition.left,
				&evPosition.right,
				&evPosition.top,
				&evPosition.bottom,
				&evPosition.front,
				&evPosition.rear);
		//	�ʒu���}�b�v�ɓo�^
			this->stageChangeSwitch.insert(pair<int, MY_CUBE>(iEventNum, evPosition));
		}
		while(1);
	//	����������������	�I�����߂�����܂ŁA�J��Ԃ�

	//	����������������	�X�e�[�W�A�E�g�X�C�b�`��ǂݍ��ރ��[�v
		do
		{
			fscanf_s(switchFile, "%d, ", &iEventNum);
		//	�C�x���g�ԍ����s���Ȃ�I��
			if(	iEventNum < 0)	break;
		//	�ʒu��ǂݍ���
			fscanf_s(switchFile, "%f, %f, %f, %f, %f, %f, ",
				&evPosition.left,
				&evPosition.right,
				&evPosition.top,
				&evPosition.bottom,
				&evPosition.front,
				&evPosition.rear);
		//	�ʒu���}�b�v�ɓo�^
			this->stageOutSwitch.insert(pair<int, MY_CUBE>(iEventNum, evPosition));
		}
		while(1);
	//	����������������	�I�����߂�����܂ŁA�J��Ԃ�

	//	�t�@�C���ԍ���ۑ�
		this->eventStartSwitchNum	= inStageNum;

	//	�t�@�C�������
		fclose(switchFile);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgUpdateLight
	//
	//	�@�\		���C�g�̍X�V
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgUpdateLight(void)
	{
		VEC3	lightEye	= this->camera->getViewPointFinish();
		D3DXMatrixLookAtLH(
			&this->lightView,
			&(lightEye + this->lightShift),
			&lightEye,
			&VEC3(0,1,0));

	//	���C�g�̕����̐ݒ�
		this->shaderDepthBuf->setLight(&(lightEye + this->lightShift));
	//	�V�F�[�_�ɐݒ�
		this->shaderDepthBuf->setLightViewProjMatrix(&(this->lightView * this->lightProj));

	//	Z�o�b�t�@�V�F�[�_�Ƀ��C�g�r���[�ˉe�ϊ��s���o�^
		this->shaderZTexture->setLightViewProjMatrix(&(this->lightView * this->lightProj));
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::CSCENE_ADVENTURE_GAME
	//
	//	�@�\		�R���X�g���N�^
	//	����		inFlag			�Q�[���t���O
	//				inData			�V�[�����L�f�[�^
	//	�X�V		2008/07/10		<�V�K>
	//				2008/11/06		<�ύX>	�S�̓I�ɏ�������
	//================================================================================
	CSCENE_ADVENTURE_GAME::CSCENE_ADVENTURE_GAME(	SCENE_FLAG inFlag,
													SCENE_FLAG inClearFlag,
													SCENE_FLAG inQuitFlag,
													SCENE_SHARE_DATA* inData)	:
	CSCENE(inFlag, inFlag, inData)
	{
	//	�Q�[���֘A��NULL��
		this->gDev				= NULL;
		this->effectCenter		= NULL;
		this->playerChara		= NULL;
		this->player			= NULL;
		this->camera			= NULL;
		this->gameMap			= NULL;
		this->enemyManager		= NULL;
		this->back				= NULL;
		this->backSound			= NULL;
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;

		D3DXMatrixIdentity(&this->lightView);
		D3DXMatrixIdentity(&this->lightProj);
		this->lightNearFar		= VEC2(0.0f, 1000.0f);
		this->lightShift		= VEC3(0.0f, 0.0f, 0.0f);
		this->gameFlag			= eSGFLAG_NO;
		
		this->iStageNum			= 0;
		this->iNextStageNum		= 0;
		this->clearFlag			= inClearFlag;
		this->quitFlag			= inQuitFlag;

	//	�X�N���v�g�֘A��NULL��
		this->eventFile				= NULL;
		this->eventStep				= eEVSTEP_FALSE;
		this->eventStartSwitchNum	= 0;
		this->eventCommentG			= NULL;
		this->eventWaitCnt			= 0;
		this->eventInput			= NULL;
		this->eventWaitKey			= MYKEY::eMAX;

	//	�R���e�B�j���[�֘A����
		this->continueFrameCnt	= 0;
		this->continueChoice	= TRUE;
		this->continueFlag		= eSGCONTI_START;
		this->effContinue		= NULL;
		this->effContinueYes	= NULL;
		this->effContinueNo		= NULL;
		this->texInterface		= NULL;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::~CSCENE_ADVENTURE_GAME
	//
	//	�@�\		�f�X�g���N�^
	//	�X�V		2008/07/10		<�V�K>
	//				2008/11/06		<�ύX>	�S�̓I�ɏ�������
	//================================================================================
	CSCENE_ADVENTURE_GAME::~CSCENE_ADVENTURE_GAME(void)
	{
	//	���
		this->sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sceneInitialize
	//
	//	�@�\		������
	//	����		inNum			�O���f�[�^�ԍ�
	//	�߂�l		TRUE			�ǂݍ��ݐ���
	//				FALSE			�ǂݍ��ݎ��s
	//	�X�V		2008/07/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sceneInitialize(int inStageNum)
	{
	//	�Q�[���̏�����
		this->sgInitializeGame();
	//	�X�N���v�g�̏�����
		this->sgInitializeScript();
	//	����������
		this->sgCreateStage(inStageNum);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sceneFinishalize
	//
	//	�@�\		���
	//	�X�V		2008/07/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sceneFinishalize(void)
	{
	//	�Q�[�����̉��
		this->sgFinishalizeGame();
	//	�X�N���v�g���̉��
		this->sgFinishalizeScript();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sceneMain
	//
	//	�@�\		�V�[�����C��
	//	�X�V		2008/09/19		<�V�K>
	//================================================================================
	SCENE_FLAG
	CSCENE_ADVENTURE_GAME::sceneMain(CINPUT* input)
	{
	//	�Q�[���t���O�ŏ�������
		switch(this->gameFlag)
		{
		//	�Q�[���v���C��
			case	eSGFLAG_PLAY:
				this->sgPlayExecute(input);
				this->sgPlayRender();
				break;
		//	�Q�[���C�x���g��
			case	eSGFLAG_EVENT:
				this->sgEventExecute(input);
				this->sgEventRender();
				break;
		//	�R���e�B�j���[��
			case	eSGFLAG_CONTINUE:
				this->sgContinueExecute(input);
				this->sgContinueRender();
				break;
		//	�}�b�v�`�F���W��
			case	eSGFLAG_STAGE_CHANGE:
				this->sgStageChangeExecute(input);
				this->sgStageChangeRender();
				break;
		//	�ăX�^�[�g��
			case	eSGFLAG_RESET:
				break;
		//	�Q�[���N���A��
			case	eSGFLAG_CLEAR:
				this->sgClearExecute(input);
				this->sgClearRender();
				break;
		//	�Q�[���I����
			case	eSGFLAG_QUIT:
				this->sgQuitExecute(input);
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

#ifdef	_DEBUG
	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::drawDebug
	//
	//	�@�\		�f�o�b�O�p����
	//	�X�V		2009/01/29		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::drawDebug(void)
	{
	//	�t�H���g�ŏ���`��
		char	str[MAX_PATH]	= "";
		SPRITE	sprite			= this->gDev->getSprite();

		sprintf_s(str, sizeof(str), "CharacterPos : %10f, %10f, %10f",
			this->playerChara->getLocX(),
			this->playerChara->getLocY(),
			this->playerChara->getLocZ());

		this->fontBase->drawText(sprite, eFONT_DEBUG, str, 10, 10);
	}
#else
#ifdef	PRESENTATION
	void
	CSCENE_ADVENTURE_GAME::drawDebug(void)
	{
	}
#endif
#endif
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------