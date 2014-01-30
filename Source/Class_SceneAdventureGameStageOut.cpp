//***********************************************************************************
//!	�t�@�C����		Class_SceneAdventureGameStageOut.cpp
//
//	�@�\			�Q�[���V�[���̃X�e�[�W�A�E�g���������̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2009/02/27	<�V�K>
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
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgStageOutChange
	//
	//	�@�\		�X�e�[�W�A�E�g�ڍs���̏�����
	//	�X�V		2009/02/27	<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgStageOutChange(void)
	{
	//	�Q�[���I�����Ƀt���O��ύX�ł�����A��������
		if(	this->flagChange(eSGFLAG_STAGE_OUT))
		{
		//	�v���C���[�̃_���[�W����m�F�𖳌��ɂ���
			this->player->setDamageCheck(FALSE);
		//	�J�����̃��[�h��ύX
			this->camera->setCameraMode(eCAMO_PARALLEL);
		//	�R���e�B�j���[�t���[����������
			this->continueFrameCnt	= 0;
		//	���S�G�t�F�N�g����
			this->player->occuPlayerEffect(eADPEFF_DATHE);
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgStageOutExecute
	//
	//	�@�\		�X�e�[�W�A�E�g������
	//	����		inInput			���͏��
	//	�X�V		2009/02/27		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgStageOutExecute(CINPUT* inInput)
	{
	//	�v���C���[�̑���
		this->playerChara->operation(this->eventInput);
	//	�t���[���ɂ���ď�������
		this->continueFrameCnt++;
	//	60�t���[���ڂɃQ�[���v���C���ɕύX
		if(	this->continueFrameCnt > 60)
		{
			this->sgContinueChange();
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgStageOutRender
	//
	//	�@�\		�X�e�[�W�A�E�g���`��
	//	�X�V		2009/02/27		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgStageOutRender(void)
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