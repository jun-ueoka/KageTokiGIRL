//***********************************************************************************
//!	�t�@�C����		Class_SceneAdventureGamePlay.cpp
//
//	�@�\			�Q�[���V�[���̃v���C�����̎���
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

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgPlayChange
	//
	//	�@�\		�Q�[���v���C�ڍs���̏�����
	//	�X�V		2008/11/06		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgPlayChange(void)
	{
	//	�Q�[���v���C���Ƀt���O��ύX�ł�����A��������
		if(	this->flagChange(eSGFLAG_PLAY))
		{
		//	���������C�x���g�`�F�b�N
			this->sgPlayEventCheck(&this->player->getPlayChara()->getLoc());
		//	�v���C���[�̃_���[�W����m�F��L���ɂ���
			this->player->setDamageCheck(TRUE);
		//	��������
			this->gDev->curtainMotion(FALSE, 5);
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgPlayEventCheck
	//
	//	�@�\		�C�x���g�����`�F�b�N
	//	����		inLoc			����ʒu
	//	�X�V		2008/11/06		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgPlayEventCheck(VEC3* inLoc)
	{
	//	�C�x���g��������ԂȂ�A�`�F�b�N����
		if(	this->eventStep != eEVSTEP_FALSE)	return;

		map<int, MY_CUBE>::iterator	p;

	//	�C�x���g�X�C�b�`����
		for(p  = this->eventStartSwitch.begin();
			p != this->eventStartSwitch.end();
			p++)
		{
		//	�C�x���g�����ʒu�ɓ�������A�C�x���g����
			if(	p->second.intersect(inLoc))
			{
			//	�C�x���g�t�@�C�����J��
				this->sgPlayOpenEventFile(p->first);
			//	�C�x���g�������ς݂Ȃ̂ŁA����
				this->eventStartSwitch.erase(p);
				return;
			}
		}

	//	�X�e�[�W�`�F���W�����X�C�b�`����
		for(p  = this->stageChangeSwitch.begin();
			p != this->stageChangeSwitch.end();
			p++)
		{
		//	�X�e�[�W�`�F���W���������ʒu�ɓ�������A�C�x���g����
			if(	p->second.intersect(inLoc))
			{
			//	�X�e�[�W�`�F���W���ɕύX
				this->sgStageChange(p->first);
				return;
			}
		}

	//	�X�e�[�W�A�E�g�����X�C�b�`����
		for(p  = this->stageOutSwitch.begin();
			p != this->stageOutSwitch.end();
			p++)
		{
		//	�X�e�[�W�`�F���W���������ʒu�ɓ�������A�C�x���g����
			if(	p->second.intersect(inLoc))
			{
			//	�X�e�[�W�`�F���W���ɕύX
				this->sgStageOutChange();
				return;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgPlayOpenEventFile
	//
	//	�@�\		�C�x���g�t�@�C�����J��
	//	����		inEventNum		�J���C�x���g�t�@�C���ԍ�
	//	�X�V		2008/11/06		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgPlayOpenEventFile(int inEventNum)
	{
	//-------------------------------------------------
	//	�t�@�C�����J��
	//-------------------------------------------------
		char	textName[MAX_PATH]	= "";		//!< �e�L�X�g���쐬�p

	//	�e�L�X�g���𐶐�
		sprintf_s(textName, sizeof(textName), PATH_EVENT, inEventNum);
	//	�t�@�C�����J��
		fopen_s(&eventFile, textName, "r");

	//	�t�@�C�����J���Ȃ�������A�C�x���g�G���[�̏�Ԃɂ���
		if(	!eventFile)
		{
			this->eventStep	= eEVSTEP_ERROR;
		}
	//	�J������A�ǂݍ��ݒ��̏�Ԃɂ���
		else
		{
		//	�Q�[���C�x���g���ɐ؂�ւ�
			this->sgEventChange();
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgPlayExecute
	//
	//	�@�\		�Q�[���v���C������
	//	����		inInput			���͏��
	//	�X�V		2008/11/06		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgPlayExecute(CINPUT* inInput)
	{
#ifdef	_DEBUG
		if(	inInput->checkPress(MYKEY::eF2))
		{
			if(	inInput->checkPress(MYKEY::eDUST))
			{
				this->camera->calcViewTargetShiftZ(30.0f);
				this->camera->calcViewShiftZ(30.0f);
			}
			if(	inInput->checkPress(MYKEY::eSOFT))
			{
				this->camera->calcViewTargetShiftZ(-30.0f);
				this->camera->calcViewShiftZ(-30.0f);
			}
		}

		if(	inInput->checkPress(MYKEY::eF4))
		{
			if(	inInput->checkPress(MYKEY::eUP))
			{
				this->camera->calcViewTargetShiftY(30.0f);
				this->camera->calcViewShiftY(30.0f);
			}
			if(	inInput->checkPress(MYKEY::eDOWN))
			{
				this->camera->calcViewTargetShiftY(-30.0f);
				this->camera->calcViewShiftY(-30.0f);
			}
			if(	inInput->checkPress(MYKEY::eLEFT))
			{
				this->camera->calcViewTargetShiftX(-30.0f);
				this->camera->calcViewShiftX(-30.0f);
			}
			if(	inInput->checkPress(MYKEY::eRIGHT))
			{
				this->camera->calcViewTargetShiftX(30.0f);
				this->camera->calcViewShiftX(30.0f);
			}
		}
		else
		{
		//	�v���C���[�L�����N�^�[�̑���
			this->playerChara->operation(inInput);
		}
	//	�G�̍X�V
		this->enemyManager->updateEnemy(&this->camera->getViewPointFinish());
	//	�v���C���[�̑���
		this->player->operation(inInput);
	//	�C�x���g�����`�F�b�N
		this->sgPlayEventCheck(&this->playerChara->getLoc());
	//	���S����
		if(	this->player->getCharacterOver())	this->sgContinueChange();

#else
#ifdef	PRESENTATION
		if(	inInput->checkPress(MYKEY::eF2))
		{
			if(	inInput->checkPress(MYKEY::eDUST))
			{
				this->camera->calcViewTargetShiftZ(30.0f);
				this->camera->calcViewShiftZ(30.0f);
			}
			if(	inInput->checkPress(MYKEY::eSOFT))
			{
				this->camera->calcViewTargetShiftZ(-30.0f);
				this->camera->calcViewShiftZ(-30.0f);
			}
		}

		if(	inInput->checkPress(MYKEY::eF4))
		{
			if(	inInput->checkPress(MYKEY::eUP))
			{
				this->camera->calcViewTargetShiftY(30.0f);
				this->camera->calcViewShiftY(30.0f);
			}
			if(	inInput->checkPress(MYKEY::eDOWN))
			{
				this->camera->calcViewTargetShiftY(-30.0f);
				this->camera->calcViewShiftY(-30.0f);
			}
			if(	inInput->checkPress(MYKEY::eLEFT))
			{
				this->camera->calcViewTargetShiftX(-30.0f);
				this->camera->calcViewShiftX(-30.0f);
			}
			if(	inInput->checkPress(MYKEY::eRIGHT))
			{
				this->camera->calcViewTargetShiftX(30.0f);
				this->camera->calcViewShiftX(30.0f);
			}
		}
		else
		{
		//	�v���C���[�L�����N�^�[�̑���
			this->playerChara->operation(inInput);
		}
	//	�G�̍X�V
		this->enemyManager->updateEnemy(&this->playerChara->getLoc());
	//	�v���C���[�̑���
		this->player->operation(inInput);
	//	�C�x���g�����`�F�b�N
		this->sgPlayEventCheck(this->playerChara->getLoc());
	//	���S����
		if(	this->player->getCharacterOver())	this->sgContinueChange();
#else
	//	�v���C���[�L�����N�^�[�̑���
		this->playerChara->operation(inInput);
	//	�G�̍X�V
		this->enemyManager->updateEnemy(&this->playerChara->getLoc());
	//	�v���C���[�̑���
		this->player->operation(inInput);
	//	�C�x���g�����`�F�b�N
		this->sgPlayEventCheck(&this->playerChara->getLoc());
	//	���S����
		if(	this->player->getCharacterOver())	this->sgContinueChange();
#endif
#endif
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgPlayRender
	//
	//	�@�\		�Q�[���v���C���`��
	//	�X�V		2008/11/06		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgPlayRender(void)
	{
	//	�f�o�C�X���m��
		DEV	device	= this->gDev->getDevice();
	//	�`��J�n
		this->gDev->renderBegin();

	//	�J�����̍X�V
		this->camera->updateCamera();
	//	���C�g�̍X�V
		this->sgUpdateLight();

	//-------------------------------------------------
	//	3D���̂̕`��
	//-------------------------------------------------
	//	Z�e�N�X�`���V�F�[�_�Ńe�N�X�`���ɐ[�x�l������������
		this->shaderZTexture->begin();
		this->gameMap->squeezeSortByCameraPositionWithDrawZTexSha(
			this->camera, &this->camera->getViewPointFinish(), this->shaderZTexture);
		this->shaderZTexture->end();

		this->gDev->bufferClear(D3DCOLOR_RGBA(255,255,255,255));

	//	�J�����Ńf�o�C�X�X�V
		this->camera->viewTransform(device);

	//	�w�i�̕`��
		this->back->motion(&this->camera->getViewPointFinish());
		this->back->drawBack(device, this->gDev->getSprite());

	//	�[�x�o�b�t�@�V�F�[�_�ŕ`��
		this->shaderDepthBuf->setCameraViewProjMatrix(
			&((*this->camera->getMatrixView()) * (*this->camera->getMatrixProjection())));

		this->playerChara->draw(device);
		this->gameMap->drawEnemyG(device, &this->camera->getViewPointFinish());

	//	���C�e�B���O�𖳌�
		this->shaderDepthBuf->begin();
		this->gameMap->squeezeDrawWithDepthSha(this->shaderDepthBuf);
		this->shaderDepthBuf->end();

		this->effectCenter->drawEffects(device, this->camera);

	//-------------------------------------------------
	//	2D���̂̕`��
	//-------------------------------------------------
		this->gDev->spriteBegin();
		this->player->drawInterface(this->gDev->getSprite());
#ifdef	_DEBUG
		this->drawDebug();
#else
#ifdef PRESENTATION
		this->drawDebug();
#endif
#endif
		this->gDev->spriteEnd();

	//	���̕`��
		this->gDev->renderCurtainInDev();
	//	�`��I��
		this->gDev->renderEnd();
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------