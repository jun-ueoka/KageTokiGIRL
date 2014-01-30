//***********************************************************************************
//!	�t�@�C����		Class_SceneAdventureGameContinue.cpp
//
//	�@�\			�Q�[���V�[���̃R���e�B�j���[���������̎���
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
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgContinueChange
	//
	//	�@�\		�R���e�B�j���[�ڍs���̏�����
	//	�X�V		2008/11/12		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgContinueChange(void)
	{
	//	�R���e�B�j���[���Ƀt���O��ύX�ł�����A��������
		if(	this->flagChange(eSGFLAG_CONTINUE))
		{
		//	�g�p����ϐ��̏�����
			this->continueFrameCnt	= 0;
			this->continueFlag		= eSGCONTI_START;
			this->continueChoice	= TRUE;

		//	�v���C���[�����ŏ���
			this->player->setAllVisible(FALSE);
			C3D_EFFECT_CENTER::occuEffectsInstance(
				EFFECT_SMOKE, this->playerChara->getLoc(), 3.0f);

		//	�R���e�B�j���[�֘A����
			SAFE_DELETE(this->effContinue);
			SAFE_DELETE(this->effContinueYes);
			SAFE_DELETE(this->effContinueNo);
			SAFE_DELETE(this->texInterface);
			this->effContinue	 =
				new C3D_EFFECT(this->gDev->getDevice(), EFFECT_CONTINUE,     VEC3(512.0f, 200.0f, 0.0f));
			this->effContinueYes =
				new C3D_EFFECT(this->gDev->getDevice(), EFFECT_CONTINUE_YES, VEC3(312.0f, 400.0f, 0.0f));
			this->effContinueNo	 =
				new C3D_EFFECT(this->gDev->getDevice(), EFFECT_CONTINUE_NO,  VEC3(712.0f, 400.0f, 0.0f));
			this->texInterface	 =
				new CTEXTURE_BASE(this->gDev->getDevice(), "game_interface_player", VEC2(1.0f, 1.0f), 0xFFFFFF96);

		//	�R���e�B�j���[������������
			this->effContinue->setVisible(FALSE);
			this->effContinueYes->setVisible(FALSE);
			this->effContinueNo->setVisible(FALSE);

		//	�v���C���[�̃_���[�W����m�F�𖳌��ɂ���
			this->player->setDamageCheck(FALSE);

		//	�c�C���Ȃ���΁A�Q�[���I�[�o�[
			if(	this->ssData->dwPlayerLife <= 0)
			{
				this->continueFlag	= eSGCONTI_GAMEOVER;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgContinueExecute
	//
	//	�@�\		�R���e�B�j���[������
	//	����		inInput			���͏��
	//	�X�V		2008/11/12		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgContinueExecute(CINPUT* inInput)
	{
	//	�v���C���[�̑���
		this->player->operation(this->eventInput);
	//	�v���C���[�L�����N�^�[�̑���
		this->playerChara->operation(this->eventInput);

	//	�R���e�B�j���[�����t���O����
		switch(this->continueFlag)
		{
		//	�R���e�B�j���[�����J�n
			case eSGCONTI_START:
			//	�t���[���J�E���^������̃t���[�����܂ŏオ������A
			//	�u���b�N�A�E�g
				this->continueFrameCnt++;
				if(	this->continueFrameCnt > 60)
				{
					this->continueFlag		= eSGCONTI_BLACKOUT;
					this->continueFrameCnt	= 0;
					this->gDev->setCurtainColor(0, 0, 0);
					this->gDev->setCurtainColorAlpha(0);
					this->gDev->curtainMotion(TRUE, 5);
				}
				break;
		//	�u���b�N�A�E�g��
			case eSGCONTI_BLACKOUT:
			//	���̓����x��150�ȏ�Ȃ�A�R���e�B�j���[�I�������ڍs����
				if(	this->gDev->getCurtainColorAlpha() >= 150)
				{
					this->gDev->setCurtainAction(FALSE);
					this->gDev->setCurtainZLoc(0.1f);
					this->continueFlag	= eSGCONTI_CONTINUE_INIT;
					this->effContinue->setVisible(TRUE);
					C3D_EFFECT_CENTER::occu2DEffectsInstance(
						EFFECT_SMOKE, this->effContinue->getLoc());
				}
				break;
		//	�R���e�B�j���[�I�������ڍs��
			case eSGCONTI_CONTINUE_INIT:
			//	�t���[���J�E���^������̃t���[�����܂ŏオ������A
			//	�R���e�B�j���[�I����������
				this->continueFrameCnt++;
				if(	this->continueFrameCnt > 60)
				{
					this->continueFlag		= eSGCONTI_CONTINUE;
					this->continueFrameCnt	= 0;
					this->effContinueYes->setVisible(TRUE);
					this->effContinueNo->setVisible(TRUE);
					C3D_EFFECT_CENTER::occu2DEffectsInstance(
						EFFECT_CONTINUE_YES_SMOKE, this->effContinueYes->getLoc());
					C3D_EFFECT_CENTER::occu2DEffectsInstance(
						EFFECT_CONTINUE_NO_SMOKE,  this->effContinueNo->getLoc());
				}
				break;
		//	�R���e�B�j���[�I��������
			case eSGCONTI_CONTINUE:
			//	�uYES�v�I��
				if(	this->continueChoice)
				{
				//	�uYES�v�͓����A�uNO�v�͒�~��
					this->effContinueYes->setMotionBool(TRUE);
					this->effContinueYes->setShowSize(VEC2(400.0f, 200.0f));
					this->effContinueNo->setMotionBool(FALSE);
					this->effContinueNo->setShowSize(VEC2(200.0f, 100.0f));
				//	�E�������ꂽ��A�uNO�v��
					if(	inInput->checkPressOnce(MYKEY::eRIGHT))
					{
						this->continueChoice	= FALSE;
						C3D_EFFECT_CENTER::occu2DEffectsInstance(
							EFFECT_CONTINUE_NO_SMOKE,  this->effContinueNo->getLoc());
					}
				//	����L�[�������ꂽ��A�uYES�v�I���㏈������
					if(	inInput->checkPressOnce(MYKEY::eSTART))
					{
						this->continueFrameCnt	= 0;
						this->continueFlag		= eSGCONTI_CHOICE_YES;
						C3D_EFFECT_CENTER::occu2DEffectsInstance(
							EFFECT_CONTINUE_YES_SMOKE,  this->effContinueYes->getLoc(), 1.5f);
						C3D_EFFECT_CENTER::occu2DEffectsInstance(
							EFFECT_SMOKE,
							VEC3(float(300+(100*(this->ssData->dwPlayerLife-1))+(200/2)), 600.0f, 0.0f));
						this->ssData->dwPlayerLife--;
					}
				}
			//	�uNO�v�I��
				else
				{
				//	�uNO�v�͓����A�uYES�v�͒�~��
					this->effContinueYes->setMotionBool(FALSE);
					this->effContinueYes->setShowSize(VEC2(200.0f, 100.0f));
					this->effContinueNo->setMotionBool(TRUE);
					this->effContinueNo->setShowSize(VEC2(400.0f, 200.0f));
				//	���������ꂽ��A�uYES�v��
					if(	inInput->checkPressOnce(MYKEY::eLEFT))
					{
						this->continueChoice	= TRUE;
						C3D_EFFECT_CENTER::occu2DEffectsInstance(
							EFFECT_CONTINUE_YES_SMOKE, this->effContinueYes->getLoc());
					}
				//	����L�[�������ꂽ��A�uNO�v�I���㏈������
					if(	inInput->checkPressOnce(MYKEY::eSTART))
					{
						this->continueFrameCnt	= 0;
						this->continueFlag		= eSGCONTI_CHOICE_NO;
						C3D_EFFECT_CENTER::occu2DEffectsInstance(
							EFFECT_CONTINUE_NO_SMOKE,  this->effContinueNo->getLoc(), 1.5f);
					}
				}
				break;
		//	�uYES�v�I���㏈����
			case eSGCONTI_CHOICE_YES:
			//	�t���[���J�E���^������̃t���[�����܂ŏオ������A
			//	���Z�b�g��
				this->continueFrameCnt++;
				if(	this->continueFrameCnt > 60)
				{
					this->gDev->setCurtainZLoc(0.0f);
					this->sgResetChange();
				}
				break;
		//	�uNO�v�I���㏈����
			case eSGCONTI_CHOICE_NO:
			//	�t���[���J�E���^������̃t���[�����܂ŏオ������A
			//	�Q�[���I�[�o�[��
				this->continueFrameCnt++;
				if(	this->continueFrameCnt > 60)
				{
					this->gDev->setCurtainZLoc(0.0f);
					this->sgQuitChange();
				}
				break;
		//	�����Q�[���I�[�o�[������
			case eSGCONTI_GAMEOVER:
			//	�t���[���J�E���^������̃t���[�����܂ŏオ������A
			//	�Q�[���I�[�o�[��
				this->continueFrameCnt++;
				if(	this->continueFrameCnt > 60)	
				{
					this->sgQuitChange();
				}
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgContinueRender
	//
	//	�@�\		�R���e�B�j���[���`��
	//	�X�V		2008/11/12		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgContinueRender(void)
	{
	//	�f�o�C�X���m��
		DEV		device	= this->gDev->getDevice();
		SPRITE	sprite	= this->gDev->getSprite();

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

	//	�R���e�B�j���[�I�������ڍs���̕`��
		if(	this->continueFlag == eSGCONTI_CONTINUE_INIT)
		{
			this->effContinue->draw(sprite);
			this->effectCenter->drawEffects2D(sprite);
		}
	//	�R���e�B�j���[�I���������̕`��
		else
		if(	(this->continueFlag == eSGCONTI_CONTINUE)	||
			(this->continueFlag == eSGCONTI_CHOICE_YES)	||
			(this->continueFlag == eSGCONTI_CHOICE_NO))
		{
			this->effContinue->draw(sprite);
			this->effContinueYes->draw(sprite);
			this->effContinueNo->draw(sprite);
			this->effectCenter->drawEffects2D(sprite);

		//	�c�C�̕`��
			RECT	rcFrom	= {192,  0,256, 64};
			RECT	rcTo	= {  0,  0,  0,  0};
			LOOP((int)this->ssData->dwPlayerLife)
			{
				SetRect(&rcTo, 300+(100*i), 500, 300+200+(100*i), 500+200);
				this->texInterface->drawTexture(sprite, rcFrom, rcTo);
			}
		}
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