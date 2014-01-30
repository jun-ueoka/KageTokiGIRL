//***********************************************************************************
//!	�t�@�C����		Class_SceneMovie2.cpp
//
//	�@�\			���[�r�[�V�[���N���X�A���[�r�[2[�V���h�E�N���~�i���A����]�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2009/02/10	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneMovie2.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE2::�R���X�g���N�^
	//
	//	����		inFlag		���݂̃V�[���t���O
	//				inNextFlag	���̃V�[���t���O
	//				inData		�V�[�����L�f�[�^
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	CSCENE_MOVIE2::CSCENE_MOVIE2(SCENE_SHARE_DATA* inData)	:
	//!	�Q�[���V�[��
	CSCENE_MOVIE_BASE(eSCENE_CHAPTER1_MOVIE2, eSCENE_CHAPTER1_STAGE2, inData)
	{
	//	������
		this->camera	= NULL;
		this->kageo		= NULL;
		this->kagemi	= NULL;
		this->exe		= NULL;
		this->tima		= NULL;
		LOOP(3){ this->shabird[i] = NULL; }
		this->back		= NULL;
		this->objMap	= NULL;

		this->backSound			= NULL;
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE2::�f�X�g���N�^
	//
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	CSCENE_MOVIE2::~CSCENE_MOVIE2(void)
	{
	//	���
		sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE2::sceneInitialize
	//
	//	�@�\		������
	//	����		inStageNum	�O���f�[�^�ԍ�
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE2::sceneInitialize(int inStageNum)
	{
	//	���[�r�[�̏�����
		this->initializeMovie(2);

	//	�������
		this->sceneFinishalize();

	//	������

	//	�J����
		this->camera	= new C3D_OBJECT_CAMERA(&VEC3(0.0f,2000.0f,-5000.0f), &VEC3(0.0f,0.0f,0.0f));
		this->camera->setViewFar(20000.0f);
		this->camera->setViewShift(&VEC3(100.0f, -500.0f,  2000.0f));

	//	�J�Q�I
		this->kageo		= new CSKIN_MESH(gDev->getDevice(), "movie_kageo");

	//	�J�Q�~
		this->kagemi	= new CSKIN_MESH(gDev->getDevice(), "movie_kagemi");

	//	�G�N�X
		this->exe		= new CSKIN_MESH(gDev->getDevice(), "movie_exe");

	//	�e�B�}
		this->tima		= new CSKIN_MESH(gDev->getDevice(), "movie_tima");

	//	�V���o�[�h
		LOOP(3)
		{
			this->shabird[i] = new CSKIN_MESH(gDev->getDevice(), "enemy_shabird");
			this->shabird[i]->setVisible(FALSE);
		}

	//	��w�i
		this->back		= new CBACK_SKY(this->gDev->getDevice(), &VEC3(0.0f,0.0f,0.0f));

	//	�g�p����G�t�F�N�g�̐���
		this->effectCenter->addEffects(
			EFFECT_SMOKE, new C3D_EFFECT(gDev->getDevice(), EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));

	//	BGM
		this->backSound	= new SSTREAMING("ainoaisatu");
		this->backSound->play(0, DSBPLAY_LOOPING);

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

	//	���C�g�̐ݒ�------------------------------------------
		D3DXMatrixIdentity(&this->lightView);
		D3DXMatrixIdentity(&this->lightProj);

	//	���C�g�r���[�̃I�t�Z�b�g�l��ݒ�
		this->lightNearFar	= VEC2(1000.0f, 12000.0f);
		this->lightShift	= VEC3(5000.0f,  5000.0f, -6000.0f);

	//	���C�g�ˉe�s��̐ݒ�
		D3DXMatrixPerspectiveFovLH(&this->lightProj, D3DXToRadian(70), 1.0f, lightNearFar.x, lightNearFar.y);
	//	------------------------------------------------------

	//	3D�}�b�v
		this->objMap	= new CGAME_MAP();
		this->objMap->initialize(this->gDev->getDevice(), 202);
		this->objMap->setDistance(5000.0f);

	//	���[�r�[�̊J�n
		this->movieStartProc();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE2::sceneFinishalize
	//
	//	�@�\		���
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE2::sceneFinishalize(void)
	{
	//	���
		SAFE_DELETE(this->camera);
		SAFE_DELETE(this->kageo);
		SAFE_DELETE(this->kagemi);
		SAFE_DELETE(this->exe);
		SAFE_DELETE(this->tima);
		LOOP(3){ SAFE_DELETE(this->shabird[i]);	}
		SAFE_DELETE(this->back);

		SAFE_DELETE(this->objMap);
		SAFE_DELETE(this->backSound);

		CZTEX_SHA::releaseInstance();
		CDEPTH_SHA::releaseInstance();
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE2::sceneMain
	//
	//	�@�\		�V�[�����C��
	//	�@�\�T�v	�Q�[���V�[�����C��
	//	����		input		���͏��
	//	�߂�l		���̃V�[��
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	SCENE_FLAG
	CSCENE_MOVIE2::sceneMain(CINPUT* input)
	{
	//	����
		this->execute(input);
	//	�`��
		this->render();
	//	���[�r�[�̏I���`�F�b�N
		this->movieEndCheck();

		return	this->returnFlag;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE2::executeInstantChengeMovieSpeed
	//
	//	�@�\		���[�r�[�������x�ύX������
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE2::executeInstantChengeMovieSpeed(void)
	{
		this->kageo->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->kagemi->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->exe->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->tima->setAnimSpeed(0.016f * float(this->movieSpeed));
		LOOP(3){ this->shabird[i]->setAnimSpeed(0.016f * float(this->movieSpeed)); }
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE2::executeProcOnce
	//
	//	�@�\		�����������s
	//	����		inProc		�����ԍ�
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE2::executeProcOnce(int inProc)
	{
	//	�����ԍ��ŕ���
		switch(inProc)
		{
		//	�����ݒ�
			case 0:
				this->kageo->setLoc(&VEC3(-4400.0f, 0.0f,  50.0f));
				this->kageo->setAngleY(270.0f);
				this->kageo->changeAnimationAndBlend(5);
				this->kagemi->setLoc(&VEC3(-4400.0f, 0.0f,-150.0f));
				this->kagemi->setAngleY(270.0f);
				this->kagemi->changeAnimationAndBlend(5);
				this->exe->setLoc(&VEC3(-1300.0f, 0.0f, 3000.0));
				this->exe->setAngleY(270.0f);
				this->exe->changeAnimationAndBlend(0);
				this->tima->setLoc(&VEC3(-700.0f, 0.0f, 3000.0));
				this->tima->setAngleY(90.0f);
				this->tima->changeAnimationAndBlend(0);
				this->camera->setCameraMode(eCAMO_PARALLEL);
				this->camera->setLoc(&VEC3(-4700.0f, 500.0f, -1000.0f));
				this->camera->setViewPointShift(&VEC3(300.0f, -500.0f, 1000.0f));
				this->gDev->createCurtainTex("Loading_KageoKagemiC");
				break;
			case 1:
				this->kageo->changeAnimationAndBlend(6);
				break;
			case 2:
				this->kagemi->changeAnimationAndBlend(0);
				break;
			case 3:
				this->kagemi->changeAnimationAndBlend(6);
				break;
			case 4:
				this->kageo->changeAnimationAndBlend(0);
				this->kageo->setMoveQuantity(3.0f);
				this->kagemi->changeAnimationAndBlend(0);
				this->kagemi->setMoveQuantity(5.5f);
				break;
			case 5:
				this->camera->setCameraMode(eCAMO_NO);
				break;
			case 6:
				this->camera->setLoc(&VEC3(-500.0f, 300.0f, 2000.0f));
				this->camera->setViewPoint(&VEC3(-1500.0f, 100.0f, 3000.0f));
				this->kageo->setVisible(FALSE);
				this->kagemi->setVisible(FALSE);
				break;
			case 7:
				this->tima->changeAnimationAndBlend(1);
				break;
			case 8:
				this->tima->changeAnimationAndBlend(0);
				break;
			case 9:
				this->exe->changeAnimationAndBlend(5);
				this->camera->setLoc(&VEC3(-1000.0f, 200.0f, 2500.0f));
				this->camera->setViewPoint(&VEC3(-1300.0f, 150.0f, 3000.0f));
				break;
			case 10:
				this->tima->changeAnimationAndBlend(3);
				this->camera->setLoc(&VEC3(-1000.0f, 200.0f, 2000.0f));
				this->camera->setViewPoint(&VEC3(-700.0f, 150.0f, 3000.0f));
				break;
			case 11:
				this->tima->changeAnimationAndBlend(4);
				break;
			case 12:
				this->shabird[0]->setLoc(&VEC3(-1000.0f, 700.0f, 2900.0f)); 
				this->shabird[0]->setAngle(&VEC3(0.0f, 90.0f, 5.0f)); 
				this->shabird[0]->setVisible(TRUE);
				this->shabird[0]->setMoveQuantity(2.0f);
			//	���G�t�F�N�g����
				this->effectCenter->occuEffects(
					EFFECT_SMOKE, this->shabird[0]->getLoc(), 1.0f);
				break;
			case 13:
				this->shabird[1]->setLoc(&VEC3(-500.0f, 600.0f, 3000.0f)); 
				this->shabird[1]->setAngle(&VEC3(0.0f, 180.0f, -3.0f)); 
				this->shabird[1]->setVisible(TRUE);
				this->shabird[1]->setMoveQuantity(2.0f);
			//	���G�t�F�N�g����
				this->effectCenter->occuEffects(
					EFFECT_SMOKE, this->shabird[1]->getLoc(), 1.0f);
				break;
			case 14:
				this->shabird[2]->setLoc(&VEC3(-1500.0f, 500.0f, 3100.0f)); 
				this->shabird[2]->setAngle(&VEC3(2.0f, 250.0f, -4.0f)); 
				this->shabird[2]->setVisible(TRUE);
				this->shabird[2]->setMoveQuantity(2.0f);
			//	���G�t�F�N�g����
				this->effectCenter->occuEffects(
					EFFECT_SMOKE, this->shabird[2]->getLoc(), 1.0f);
				break;
			case 15:
				LOOP(3){ this->shabird[i]->setMoveQuantity(8.0f);	}
				this->tima->changeAnimationAndBlend(0);
				break;
			case 16:
				LOOP(3){ this->shabird[i]->setVisible(FALSE);	}
				break;
			case 17:
				this->exe->changeAnimationAndBlend(1);
				this->exe->setMoveQuantity(3.0f);
				this->tima->changeAnimationAndBlend(2);
				this->tima->setMoveQuantity(3.0f);
				break;
			case 18:
				this->exe->setMoveQuantity(10.0f);
				this->tima->setMoveQuantity(10.0f);
				break;
			case 19:
				this->kageo->setVisible(TRUE);
				this->kageo->setAngleY(270.0f);
				this->kageo->setMoveQuantity(4.0f);
				this->kageo->changeAnimationAndBlend(0);
				this->kagemi->setVisible(TRUE);
				this->kagemi->setAngleY(90.0f);
				this->kagemi->setMoveQuantity(4.0f);
				this->kagemi->changeAnimationAndBlend(0);
				this->exe->setVisible(FALSE);
				this->tima->setVisible(FALSE);
				break;
			case 20:
				this->kageo->changeAnimationAndBlend(6);
				this->kagemi->changeAnimationAndBlend(6);
				break;
			case 21:
				this->camera->setLoc(&VEC3(-900.0f, 300.0f, -500.0f));
				this->camera->setViewPoint(&VEC3(-1100.0f, 150.0f, 300.0f));
				this->kageo->changeAnimationAndBlend(1);
				break;
			case 22:
				this->camera->setLoc(&VEC3(-1100.0f, 300.0f, -500.0f));
				this->camera->setViewPoint(&VEC3(-900.0f, 150.0f, 300.0f));
				this->kagemi->changeAnimationAndBlend(1);
				break;
			case 23:
				this->kagemi->changeAnimationAndBlend(3);
				break;
			case 24:
				this->camera->setLoc(&VEC3(-900.0f, 300.0f, -500.0f));
				this->camera->setViewPoint(&VEC3(-1100.0f, 150.0f, 300.0f));
				this->kageo->changeAnimationAndBlend(4);
				break;
			case 25:
				this->camera->setLoc(&VEC3(-1000.0f, 200.0f, -700.0f));
				this->camera->setViewPoint(&VEC3(-1000.0f, 100.0f, 2000.0f));
				this->kagemi->changeAnimationAndBlend(5);
				break;
			case 26:
				this->kageo->changeAnimationAndBlend(5);
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE2::executeProc
	//
	//	�@�\		�������s
	//	����		inProc		�����ԍ�
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE2::executeProc(int inProc)
	{
	//	�����ԍ��ŕ���
		switch(inProc)
		{
			case 0:
				this->camera->calcLocX(4.0f);
				this->camera->calcViewPointShiftX(-1.0f);
				this->kageo->calcLocX(3.0f);
				this->kagemi->calcLocX(3.0f);
				break;
			case 1:
				this->camera->calcLocX(3.5f);
				this->camera->calcViewPointShiftX(-1.0f);
				this->kagemi->calcLocX(3.0f);
				break;
			case 2:
				this->kagemi->calcAngleY(-2.5f);
				break;
			case 3:
				this->camera->calcViewPointShiftX(3.5f);
				this->camera->calcViewPointShiftY(0.5f);
				this->camera->calcViewPointShiftZ(10.0f);
				this->kageo->calcAngleY(-1.5f);
				this->kagemi->calcAngleY(4.0f);
				this->kagemi->moveFront();
				break;
			case 4:
				this->camera->calcViewPointShiftX(3.0f);
				this->camera->calcViewPointShiftY(0.5f);
				this->camera->calcViewPointShiftZ(10.0f);
				this->kageo->moveFront();
				this->kagemi->calcAngleY(2.6f);
				this->kagemi->moveFront();
				break;
			case 5:
				this->camera->calcLocY(-1.0f);
				this->camera->calcLocZ(3.0f);
				this->camera->calcViewPointShiftX(3.0f);
				this->camera->calcViewPointShiftY(1.0f);
				this->camera->calcViewPointShiftZ(10.0f);
				this->kageo->moveFront();
				this->kagemi->moveFront();
				break;
			case 6:
				this->camera->calcLocY(-1.0f);
				this->camera->calcViewPointShiftX(2.0f);
				this->camera->calcViewPointShiftY(1.0f);
				this->camera->calcViewPointShiftZ(10.0f);
				this->kagemi->moveFront();
				break;
			case 7:
				this->kagemi->calcAngleY(-3.5f);
				break;
			case 8:
				this->camera->calcLocZ(4.0f);
				this->camera->calcViewPointZ(3.0f);
				break;
			case 9:
				this->camera->calcLocX(-5.0f);
				this->camera->calcViewPointX(5.0f);
				break;
			case 10:
				this->camera->calcLocY(0.5f);
				this->camera->calcLocZ(-4.0f);
				this->camera->calcViewPointX(-1.0f);
				this->camera->calcViewPointY(1.5f);
				break;
			case 11:
				this->camera->calcLocY(0.5f);
				this->camera->calcLocZ(-4.0f);
				this->camera->calcViewPointX(-2.0f);
				this->camera->calcViewPointY(2.5f);
				this->shabird[0]->calcAngleY(2.5f);
				this->shabird[0]->moveFront();
				break;
			case 12:
				this->camera->calcLocY(0.5f);
				this->camera->calcLocZ(-4.0f);
				this->camera->calcViewPointX(-2.0f);
				this->camera->calcViewPointY(2.5f);
				this->shabird[0]->calcAngleY(2.5f);
				this->shabird[0]->moveFront();
				this->shabird[1]->calcAngleY(2.0f);
				this->shabird[1]->moveFront();
				break;
			case 13:
				this->camera->calcLocY(0.5f);
				this->camera->calcLocZ(-4.0f);
				this->camera->calcViewPointX(-2.0f);
				this->camera->calcViewPointY(2.5f);
				this->shabird[0]->calcAngleY(2.5f);
				this->shabird[0]->moveFront();
				this->shabird[1]->calcAngleY(2.0f);
				this->shabird[1]->moveFront();
				this->shabird[2]->calcAngleY(2.0f);
				this->shabird[2]->moveFront();
				break;
			case 14:
				this->shabird[0]->calcAngleY(2.65f);
				this->shabird[0]->moveFront();
				this->shabird[1]->calcAngleY(2.7f);
				this->shabird[1]->moveFront();
				this->shabird[2]->calcAngleY(2.8f);
				this->shabird[2]->moveFront();
				break;
			case 15:
				this->camera->calcViewPointY(-0.5f);
				this->shabird[0]->moveFront();
				this->shabird[1]->moveFront();
				this->shabird[2]->moveFront();
				break;
			case 16:
				this->camera->calcLocZ(1.0f);
				this->camera->calcViewPointY(-0.5f);
				break;
			case 17:
				this->camera->calcLocY(-2.0f);
				this->exe->calcAngleY(-3.0f);
				this->exe->moveFront();
				this->tima->calcAngleY(3.0f);
				this->tima->moveFront();
				break;
			case 18:
				this->camera->calcLocZ(-3.0f);
				this->exe->moveFront();
				this->tima->moveFront();
				break;
			case 19:
				this->camera->calcLocZ(-3.0f);
				this->kageo->moveFront();
				this->kagemi->moveFront();
				break;
			case 20:
				this->kagemi->calcAngleY(3.0f);
				break;
			case 21:
				this->camera->calcViewPointY(0.7f);
				this->kagemi->moveFront();
				break;
			case 22:
				this->camera->calcViewPointY(0.7f);
				this->kageo->calcAngleY(-3.0f);
				this->kagemi->moveFront();
				break;
			case 23:
				this->camera->calcViewPointY(0.7f);
				this->kageo->moveFront();
				this->kagemi->moveFront();
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE2::updateLight
	//
	//	�@�\		���C�g�̍X�V
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE2::updateLight(void)
	{
	//	���C�g�̍X�V
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
	//!	���\�b�h��	CSCENE_MOVIE2::execute
	//
	//	�@�\		����
	//	����		input		���͏��
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE2::execute(CINPUT* input)
	{
		this->checkMovieInput(input);
		this->checkProcFollowMovieSpeed();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE2::render
	//
	//	�@�\		�`��
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE2::render(void)
	{
	//	�`��J�n
		gDev->renderBegin(D3DCOLOR_RGBA(255,255,255,0));

		DEV	inDev	= this->gDev->getDevice();

	//	�I�u�W�F�N�g�̃\�[�g
		this->camera->updateCamera();

	//	���C�g�̍X�V
		this->updateLight();

	//	Z�e�N�X�`���V�F�[�_�Ńe�N�X�`���ɐ[�x�l������������
		this->shaderZTexture->begin();
		this->objMap->squeezeSortByCameraPositionNotCheckMessageWithDrawZTexSha(
			this->camera, &this->camera->getViewPointFinish(), this->shaderZTexture);
		this->shaderZTexture->end();

		this->gDev->bufferClear(D3DCOLOR_RGBA(255,255,255,255));

	//	�J�����Ńf�o�C�X�X�V
		this->camera->viewTransform(inDev);

	//	��w�i�̕`��
		this->back->motion(&this->camera->getViewPointFinish());
		this->back->drawBack(inDev, this->gDev->getSprite());

	//	�[�x�o�b�t�@�V�F�[�_�ŕ`��
		this->shaderDepthBuf->setCameraViewProjMatrix(
			&((*this->camera->getMatrixView()) * (*this->camera->getMatrixProjection())));

		this->shaderDepthBuf->begin();
		this->objMap->squeezeDrawWithDepthSha(this->shaderDepthBuf);
		this->shaderDepthBuf->end();

		this->kageo->draw(inDev);
		this->kagemi->draw(inDev);
		this->exe->draw(inDev);
		this->tima->draw(inDev);
		LOOP(3){ this->shabird[i]->draw(inDev); }

		this->effectCenter->drawEffects(inDev, this->camera);

	//	�V�l�}�X�R�[�v�̕`��
		direction->directCinemaScopePaintOut();

	//	�t�H���g�̕`��
		gDev->spriteBegin();
		executeDrawTelop();
		gDev->spriteEnd();

	//	���̕`��
		gDev->renderCurtainInDev();
		gDev->renderEnd();
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------