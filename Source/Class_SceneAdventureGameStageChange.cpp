//***********************************************************************************
//!	�t�@�C����		Class_SceneAdventureGameStageChange.cpp
//
//	�@�\			�Q�[���V�[���̃}�b�v�ύX���������̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2008/07/10	<�V�K>
//					2008/11/06	<�ύX>	�S�̓I�ɏ�������
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneAdventureGameData.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgStageChange
	//
	//	�@�\		�X�e�[�W�`�F���W�ڍs���̏�����
	//	����		inNewStage		�V�����X�e�[�W�ԍ�
	//	�X�V		2008/11/13		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgStageChange(int inNewStage)
	{
	//	�X�e�[�W�`�F���W���Ƀt���O��ύX�ł�����A��������
		if(	this->flagChange(eSGFLAG_STAGE_CHANGE))
		{
		//	�_�~�[���͏���������
			this->eventInput->resetKey();
		//	���L�����̌����Ă�����Ƀ_�~�[�L�[�����
		//	�������������Ă���
			if(	(this->playerChara->getAngleY() >=  90.0f)	&&
				(this->playerChara->getAngleY() <  270.0f))
			{
				this->eventInput->setPress(MYKEY::eLEFT, true);
			}
		//	�E�����������Ă���
			else
			{
				this->eventInput->setPress(MYKEY::eRIGHT, true);
			}
		//	�J�����̎��_���~
			this->camera->setCameraMode(eCAMO_PARALLEL);
		//	���̃X�e�[�W�ԍ���o�^
			this->iNextStageNum	= inNewStage;
		//	�v���C���[�̃_���[�W����m�F�𖳌��ɂ���
			this->player->setDamageCheck(FALSE);
		//	���̏o��
			this->gDev->curtainMotion(TRUE, 5);
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgStageChangeExecute
	//
	//	�@�\		�X�e�[�W�`�F���W������
	//	����		inInput			���͏��
	//	�X�V		2008/11/13		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgStageChangeExecute(CINPUT* inInput)
	{
	//	�L�����N�^�[���_�~�[���͂ő���
		this->playerChara->operation(this->eventInput);
	//	���̕s�����x���ő�Ȃ�A�X�e�[�W�`�F���W
		if(	this->gDev->getCurtainColorAlpha() >= 255)
		{
		//	�J�����̕��t�ړ����[�h�ɂ���
			this->camera->setCameraMode(eCAMO_PARALLEL_CHASE);
		//	�_�~�[���͏���������
			this->eventInput->resetKey();
		//	�Q�[���̏�Ԃ��Q�[���v���C���ɕύX���Ă���
			this->sgPlayChange();
		//	�����Â����ɃX�e�[�W�ǂݍ���
			this->sgCreateStage(this->iNextStageNum);
		//	�����J���鏈��
			this->gDev->curtainMotion(FALSE, 5);
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgStageChangeRender
	//
	//	�@�\		�X�e�[�W�`�F���W���`��
	//	�X�V		2008/11/13		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgStageChangeRender(void)
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