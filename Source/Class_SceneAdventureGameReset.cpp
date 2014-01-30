//***********************************************************************************
//!	�t�@�C����		Class_SceneAdventureGameReset.cpp
//
//	�@�\			�Q�[���V�[���̍ăX�^�[�g���������̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2009/02/03	<�V�K>
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
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgResetChange
	//
	//	�@�\		�ăX�^�[�g�ڍs���̏�����
	//	�X�V		2009/02/03	<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgResetChange(void)
	{
	//	�Q�[���I�����Ƀt���O��ύX�ł�����A��������
		if(	this->flagChange(eSGFLAG_RESET))
		{
		//	�v���C���[�̃_���[�W����m�F�𖳌��ɂ���
			this->player->setDamageCheck(FALSE);
		//	�����o��������
			this->gDev->curtainMotion(TRUE, 5);
			this->continueFrameCnt	= 0;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgResetExecute
	//
	//	�@�\		�ăX�^�[�g������
	//	����		inInput			���͏��
	//	�X�V		2009/02/03		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgResetExecute(CINPUT* inInput)
	{
	//	���̓����x���ő�Ȃ�A�^�C�g����
		if(	this->gDev->getCurtainColorAlpha() >= 255)
		{
		//	�t���[���ɂ���ď�������
			this->continueFrameCnt++;
		//	2�t���[���ڂ܂ŁA�L�����N�^�[�����Z�b�g
			if		(this->continueFrameCnt < 2)
			{
				this->camera->setCameraMode(eCAMO_PARALLEL_CHASE);
				this->player->resetPlayer();
				this->enemyManager->resetEnemy();
				this->sgCreateEnemy(0);
			}
		//	60�t���[���ڂɃQ�[���v���C���ɕύX
			else if	(this->continueFrameCnt > 30)
			{
				this->sgPlayChange();
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgResetRender
	//
	//	�@�\		�ăX�^�[�g���`��
	//	�X�V		2009/02/03		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgResetRender(void)
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
		//this->gDev->spriteBegin();
		//this->gDev->spriteEnd();

	//	���̕`��
		this->gDev->renderCurtainInDev();
	//	�`��I��
		this->gDev->renderEnd();
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------