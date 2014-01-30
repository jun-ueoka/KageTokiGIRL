//***********************************************************************************
//!	�t�@�C����		Class_SceneAdventureGameClear.cpp
//
//	�@�\			�Q�[���V�[���̃Q�[���N���A���������̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2008/11/13	<�V�K>
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
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgClearChange
	//
	//	�@�\		�Q�[���N���A�ڍs���̏�����
	//	�X�V		2008/11/13		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgClearChange(void)
	{
	//	�Q�[���N���A���Ƀt���O��ύX�ł�����A��������
		if(	this->flagChange(eSGFLAG_CLEAR))
		{
		//	�v���C���[�̃_���[�W����m�F�𖳌��ɂ���
			this->player->setDamageCheck(FALSE);
		//	�����o��������
			this->gDev->curtainMotion(TRUE, 5);
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgClearExecute
	//
	//	�@�\		�Q�[���N���A������
	//	����		inInput			���͏��
	//	�X�V		2008/11/13		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgClearExecute(CINPUT* inInput)
	{
	//	���̓����x���ő�Ȃ�A�^�C�g����
		if(	this->gDev->getCurtainColorAlpha() >= 255)
		{
			this->returnFlag	= this->clearFlag;
		}
	//	�v���C���[�̃C�x���g����
		this->player->operation(this->eventInput);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgClearRender
	//
	//	�@�\		�Q�[���N���A���`��
	//	�X�V		2008/11/13		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgClearRender(void)
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