//***********************************************************************************
//!	�t�@�C����		Class_SceneMovie3.cpp
//
//	�@�\			���[�r�[�V�[���N���X�A���[�r�[3[�V���h�E�N���~�i���A�W�g]�̎���
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
	//!	���\�b�h��	CSCENE_MOVIE3::�R���X�g���N�^
	//
	//	����		inFlag		���݂̃V�[���t���O
	//				inNextFlag	���̃V�[���t���O
	//				inData		�V�[�����L�f�[�^
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	CSCENE_MOVIE3::CSCENE_MOVIE3(SCENE_SHARE_DATA* inData)	:
	//!	�Q�[���V�[��
	CSCENE_MOVIE_BASE(eSCENE_CHAPTER1_MOVIE3, eSCENE_CHAPTER1_STAGE3, inData)
	{
	//	������
		this->camera			= NULL;
		this->cameraTarget		= NULL;

		this->kageo				= NULL;
		this->kagemi			= NULL;
		this->shabird			= NULL;
		this->clime				= NULL;
		this->tima				= NULL;
		this->feather			= NULL;
		this->yamakage			= NULL;
		this->exe				= NULL;

		this->objMapForest		= NULL;
		this->objMapCave		= NULL;
		this->back				= NULL;
		this->cave				= NULL;

		this->movieFlag			= eM3F_FOREST;

		this->backSound			= NULL;
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;

		D3DXMatrixIdentity(&this->lightView);
		D3DXMatrixIdentity(&this->lightProj);

		this->lightShift		= VEC3(0.0f, 0.0f, 0.0f);
		this->lightNearFar		= VEC2(0.0f, 0.0f);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE3::�f�X�g���N�^
	//
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	CSCENE_MOVIE3::~CSCENE_MOVIE3(void)
	{
	//	���
		sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE3::sceneInitialize
	//
	//	�@�\		������
	//	����		inStageNum	�O���f�[�^�ԍ�
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE3::sceneInitialize(int inStageNum)
	{
	//	���[�r�[�̏�����
		this->initializeMovie(3);

	//	�������
		this->sceneFinishalize();

	//	������

	//	�J����
		this->camera	= new C3D_OBJECT_CAMERA(&VEC3(0.0f,0.0f,0.0f), &VEC3(0.0f,0.0f,0.0f));
		this->camera->setViewFar(20000.0f);
		this->camera->setViewShift(&VEC3(100.0f, -500.0f,  2000.0f));
	//	�J�����^�[�Q�b�g
		this->cameraTarget	= new C3D_OBJECT();

	//	�J�Q�I
		this->kageo		= new CSKIN_MESH(gDev->getDevice(), "movie_kageo");
	//	�J�Q�~
		this->kagemi	= new CSKIN_MESH(gDev->getDevice(), "movie_kagemi");
	//	�V���o�[�h
		this->shabird	= new CSKIN_MESH(gDev->getDevice(), "enemy_shabird");
	//	�N���C��
		this->clime		= new CSKIN_MESH(gDev->getDevice(), "movie_clime");
	//	�e�B�}
		this->tima		= new CSKIN_MESH(gDev->getDevice(), "movie_tima");
	//	�t�F�U�[
		this->feather	= new CSKIN_MESH(gDev->getDevice(), "movie_feather");
	//	���}�J�Q
		this->yamakage	= new CSKIN_MESH(gDev->getDevice(), "movie_yamakage");
	//	�G�N�X
		this->exe		= new CSKIN_MESH(gDev->getDevice(), "movie_exe");

	//	��w�i
		this->back		= new CBACK_SKY(this->gDev->getDevice(), &VEC3(0.0f,0.0f,0.0f));

	//	�g�p����G�t�F�N�g�̐���
		this->effectCenter->addEffects(
			EFFECT_SMOKE, new C3D_EFFECT(gDev->getDevice(), EFFECT_SMOKE, VEC3(0.0f,0.0f,0.0f)));

	//	���A
		this->cave	= new CMESH(gDev->getDevice(), "kage_stage_doom2");

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
	//	���C�g�r���[�̃I�t�Z�b�g�l��ݒ�
		this->lightNearFar	= VEC2(1000.0f, 12000.0f);
		this->lightShift	= VEC3(5000.0f,  5000.0f, -6000.0f);

	//	���C�g�ˉe�s��̐ݒ�
		D3DXMatrixPerspectiveFovLH(&this->lightProj, D3DXToRadian(70), 1.0f, lightNearFar.x, lightNearFar.y);
	//	------------------------------------------------------

	//	3D�}�b�v(�X)
		this->objMapForest	= new CGAME_MAP();
		this->objMapForest->initialize(this->gDev->getDevice(), 203);

	//	3D�}�b�v(�V���h�E�N���~�i���A�W�g���A)
		this->objMapCave	= new CGAME_MAP();
		this->objMapCave->initialize(this->gDev->getDevice(), 204);

		this->movieFlag		= eM3F_FOREST;

	//	���[�r�[�̊J�n
		this->movieStartProc();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE3::sceneFinishalize
	//
	//	�@�\		���
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE3::sceneFinishalize(void)
	{
	//	���
		SAFE_DELETE(this->camera);
		SAFE_DELETE(this->cameraTarget);

		SAFE_DELETE(this->kageo);
		SAFE_DELETE(this->kagemi);
		SAFE_DELETE(this->shabird);
		SAFE_DELETE(this->clime);
		SAFE_DELETE(this->tima);
		SAFE_DELETE(this->feather);
		SAFE_DELETE(this->yamakage);
		SAFE_DELETE(this->exe);

		SAFE_DELETE(this->objMapForest);
		SAFE_DELETE(this->objMapCave);
		SAFE_DELETE(this->backSound);

		SAFE_DELETE(this->back);
		SAFE_DELETE(this->cave);

		CZTEX_SHA::releaseInstance();
		CDEPTH_SHA::releaseInstance();
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE3::sceneMain
	//
	//	�@�\		�V�[�����C��
	//	�@�\�T�v	�Q�[���V�[�����C��
	//	����		input		���͏��
	//	�߂�l		���̃V�[��
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	SCENE_FLAG
	CSCENE_MOVIE3::sceneMain(CINPUT* input)
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
	//!	���\�b�h��	CSCENE_MOVIE3::executeInstantChengeMovieSpeed
	//
	//	�@�\		���[�r�[�������x�ύX������
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE3::executeInstantChengeMovieSpeed(void)
	{
		this->kageo->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->kagemi->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->shabird->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->clime->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->tima->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->feather->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->yamakage->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->exe->setAnimSpeed(0.016f * float(this->movieSpeed));
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE3::executeProcOnce
	//
	//	�@�\		�����������s
	//	����		inProc		�����ԍ�
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE3::executeProcOnce(int inProc)
	{
	//	���[�r�[�t���O���X�Ȃ�A�ȉ�������
		if(	this->movieFlag == eM3F_FOREST)
		{
		//	�����ԍ��ŕ���
			switch(inProc)
			{
				case 0:
					this->kageo->setLoc (&VEC3(-3000.0f, 0.0f,  50.0f));
					this->kageo->setAngleY(270.0f);
					this->kageo->setMoveQuantity(3.0f);
					this->kageo->changeAnimationAndBlend(0);
					this->kagemi->setLoc(&VEC3(-3000.0f, 0.0f,-150.0f));
					this->kagemi->setAngleY(270.0f);
					this->kagemi->setMoveQuantity(3.0f);
					this->kagemi->changeAnimationAndBlend(0);
					this->shabird->setLoc(&VEC3(800.0f, 1800.0f, 500.0f));
					this->shabird->setMoveQuantity(3.0f);
					this->camera->setCameraMode(eCAMO_PARALLEL);
					this->camera->setLoc(&VEC3(-3000.0f, 700.0f, -1500.0f));
					this->camera->setViewPointShift(&VEC3(0.0f, -500.0f, 1500.0f));
					break;
				case 1:
					this->kagemi->setMoveQuantity(5.0f);
					this->kagemi->changeAnimationAndBlend(5);
					break;
				case 2:
					this->kageo->setMoveQuantity(5.0f);
					this->kageo->changeAnimationAndBlend(5);
					break;
				case 3:
					this->shabird->setMoveQuantity(10.0f);
					break;
				case 4:
					this->gDev->curtainMotion(TRUE, 4);
					this->gDev->createCurtainTex();
					break;
				case 5:
					this->movieFlag		= eM3F_CAVE;
					break;
			}
		}
	//	���[�r�[�t���O�����A�Ȃ�A�ȉ�������	
		else
		{
		//	�����ԍ��ŕ���
			switch(inProc)
			{
				case 6:
					this->camera->setCameraMode(eCAMO_NO);
					this->camera->setChase(FALSE);
					this->camera->setTarget(this->shabird);
					this->camera->setLoc(&VEC3(0.0f, 4500.0f, 0.0f));
					this->camera->setViewShift(&VEC3(0.0f, 0.0f, 0.0f));
					this->shabird->setLoc(&VEC3(2500.0f, 5000.0f, -5000.0f));
					this->shabird->setAngleX(-16.0f);
					this->shabird->setAngleY(180.0f);
					this->shabird->setMoveQuantity(10.0f);
					this->clime->setLoc(&VEC3(0.0f, 3000.0f, 2000.0f));
					this->clime->setAngle(&VEC3(0.0f, 0.0f, 0.0f));
					this->tima->setLoc(&VEC3(2000.0f, 3000.0f, 1000.0f));
					this->tima->setAngle(&VEC3(0.0f, 72.0f, 0.0f));
					this->feather->setLoc(&VEC3(-2000.0f, 3000.0f, 1000.0f));
					this->feather->setAngle(&VEC3(0.0f, 288.0f, 0.0f));
					this->yamakage->setLoc(&VEC3(-1000.0f, 3000.0f, -2000.0f));
					this->yamakage->setAngle(&VEC3(0.0f, 216.0f, 0.0f));
					this->exe->setLoc(&VEC3(1000.0f, 3000.0f, -2000.0f));
					this->exe->setAngle(&VEC3(0.0f, 144.0f, 0.0f));
					this->gDev->curtainMotion(FALSE, 4);
					break;
				case 7:
					this->camera->setCameraMode(eCAMO_PARALLEL);
					this->camera->setLocY(3100.0f);
					this->camera->setViewPointShift(&VEC3(2000.0f, 250.0f, 1000.0f));
					this->camera->setTarget(NULL);
					this->shabird->setAngleX(0.0f);
					break;
				case 8:
					this->camera->setCameraMode(eCAMO_PARALLEL_CHASE);
					this->camera->setTarget(this->cameraTarget);
					this->camera->setMaxViewMoveCnt(100);
					this->cameraTarget->setLoc(&VEC3(1000.0f, 3250.0f, -2000.0f));
					break;
				case 9:
					this->cameraTarget->setLoc(&VEC3(-1000.0f, 3250.0f, -2000.0f));
					break;
				case 10:
					this->cameraTarget->setLoc(&VEC3(-2000.0f, 3250.0f, 1000.0f));
					break;
				case 11:
					this->cameraTarget->setLoc(&VEC3(0.0f, 3250.0f, 2000.0f));
					break;
				case 12:
					this->tima->setAngle(&VEC3(0.0f, 100.0f, 0.0f));
					this->cameraTarget->setLoc(&VEC3(2000.0f, 3250.0f, 1000.0f));
					break;
				case 13:
					this->tima->changeAnimationAndBlend(4);
					break;
				case 14:
					this->effectCenter->occuEffects(
						EFFECT_SMOKE, this->shabird->getLoc(), 1.0f);
					this->shabird->setVisible(FALSE);
					this->tima->changeAnimationAndBlend(0);
					break;
				case 15:
					this->camera->setCameraMode(eCAMO_NO);
					this->camera->setTarget(NULL);
					this->camera->setLoc(&VEC3(-1000.0f, 3300.0f, 1000.0f));
					this->camera->setViewPoint(&VEC3(1000.0f, 3250.0f, 2000.0f));
					break;
				case 16:
					this->tima->changeAnimationAndBlend(1);
					break;
				case 17:
					this->tima->changeAnimationAndBlend(0);
					break;
				case 18:
					this->camera->setCameraMode(eCAMO_PARALLEL_CHASE);
					this->camera->setTarget(this->cameraTarget);
					this->cameraTarget->setLoc(&VEC3(0.0f, 3250.0f, 2000.0f));
					break;
				case 19:
					this->cameraTarget->setLoc(&VEC3(1000.0f, 3250.0f, -2000.0f));
					break;
				case 20:
					this->exe->changeAnimationAndBlend(3);
					break;
				case 21:
					this->camera->setCameraMode(eCAMO_NO);
					this->camera->setTarget(this->exe);
					this->exe->setMoveQuantity(90.0f);
					this->exe->changeAnimationAndBlend(2);
					this->effectCenter->occuEffects(
						EFFECT_SMOKE, this->exe->getLoc(), 1.0f);
					break;
				case 22:
					this->camera->setTarget(NULL);
					break;
				case 23:
					this->camera->setViewPoint(&VEC3(0.0f, 3250.0f, 2000.0f));
					this->camera->setLoc(&VEC3(0.0f, 3100.0f, 1700.0f));
					this->clime->setAngleY(0.0f);
					break;
				case 24:
					this->clime->changeAnimationAndBlend(4);
					break;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE3::executeProc
	//
	//	�@�\		�������s
	//	����		inProc		�����ԍ�
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE3::executeProc(int inProc)
	{
	//	���[�r�[�t���O���X�Ȃ�A�ȉ�������
		if(	this->movieFlag == eM3F_FOREST)
		{
		//	�����ԍ��ŕ���
			switch(inProc)
			{
				case 0:
					this->camera->calcLocX(2.5f);
					this->camera->calcViewPointShiftX(0.5f);
					this->kageo->moveFront();
					this->kagemi->moveFront();
					break;
				case 1:
					this->camera->calcLocY(1.0f);
					this->camera->calcLocZ(0.5f);
					this->camera->calcViewPointShiftX(3.0f);
					this->kageo->moveFront();
					this->kagemi->moveFront();
					break;
				case 2:
					this->camera->calcLocY(1.0f);
					this->camera->calcViewPointShiftX(-5.0f);
					this->camera->calcViewPointShiftY(4.0f);
					this->camera->calcViewPointShiftZ(3.0f);
					this->kageo->moveFront();
					this->kagemi->moveFront();
					break;
				case 3:
					this->camera->calcLocY(1.0f);
					this->camera->calcViewPointShiftX(-5.0f);
					this->camera->calcViewPointShiftY(4.0f);
					this->camera->calcViewPointShiftZ(3.0f);
					this->kageo->moveFront();
					this->kagemi->moveFront();
					this->shabird->moveFront();
					break;
				case 4:
					this->camera->calcLocY(1.0f);
					this->camera->calcViewPointShiftX(-5.0f);
					this->camera->calcViewPointShiftY(4.0f);
					this->camera->calcViewPointShiftZ(3.0f);
					this->shabird->calcAngleX(0.2f);
					this->shabird->calcAngleY(1.0f);
					this->shabird->moveFront();
					break;
				case 5:
					this->camera->calcLocY(1.0f);
					this->camera->calcViewPointShiftX(-5.0f);
					this->camera->calcViewPointShiftY(4.0f);
					this->camera->calcViewPointShiftZ(3.0f);
					this->shabird->moveFront();
					break;
			}
		}
	//	���[�r�[�t���O�����A�Ȃ�A�ȉ�������	
		else
		{
		//	�����ԍ��ŕ���
			switch(inProc)
			{
				case 6:
					this->camera->calcLocY(-1.0f);
					this->shabird->calcAngleY(-0.01f);
					this->shabird->moveFront();
					break;
				case 7:
					this->camera->calcViewPointShiftY(-0.25f);
					this->camera->calcFieldOfView(-0.1f);
					this->shabird->calcAngleY(-1.0f);
					this->shabird->moveFront();
					break;
				case 8:
					this->shabird->calcAngleY(-1.0f);
					this->shabird->moveFront();
					break;
				case 9:
					this->camera->calcFieldOfView(0.2f);
					this->camera->calcLocZ(6.0f);
					this->camera->calcLocX(-2.0f);
					break;
				case 10:
					this->shabird->calcAngleY(-1.0f);
					this->shabird->moveFront();
					this->camera->calcLocZ(-1.0f);
					this->camera->calcLocX(4.0f);
					break;
				case 11:
					this->clime->calcAngleY(-2.0f);
					break;
				case 12:
					this->camera->calcLocX(2.0f);
					this->camera->calcLocZ(-5.0f);
					this->clime->calcAngleY(0.02f);
					break;
				case 13:
					this->camera->calcLocX(1.0f);
					this->camera->calcLocZ(-5.0f);
					break;
				case 14:
					this->exe->calcAngleY(7.0f);
					break;
				case 15:
					this->camera->calcLocX(-20.0f);
					this->camera->calcLocY(20.0f);
					this->exe->calcAngleX(-8.0f);
					this->exe->moveUp();
					break;
				case 16:
					this->exe->moveUp();
					break;
				case 17:
					this->camera->calcLocZ(-1.0f);
					break;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE3::updateLight
	//
	//	�@�\		���C�g�̍X�V
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE3::updateLight(void)
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
	//!	���\�b�h��	CSCENE_MOVIE3::execute
	//
	//	�@�\		����
	//	����		input		���͏��
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE3::execute(CINPUT* input)
	{
		this->checkMovieInput(input);
		this->checkProcFollowMovieSpeed();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE3::render
	//
	//	�@�\		�`��
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE3::render(void)
	{
	//	���[�r�[�t���O���X�Ȃ�A�ȉ�������
		if(	this->movieFlag == eM3F_FOREST)
		{
		//	�`��J�n
			gDev->renderBegin(D3DCOLOR_RGBA(255,255,255,0));

			DEV	inDev	= this->gDev->getDevice();

		//	�J�����̍X�V
			this->camera->updateCamera();

		//	���C�g�̍X�V
			this->updateLight();

		//	Z�e�N�X�`���V�F�[�_�Ńe�N�X�`���ɐ[�x�l������������
			this->shaderZTexture->begin();
			this->objMapForest->squeezeSortByCameraPositionNotCheckMessageWithDrawZTexSha(
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

		//	�X�̕`��
			this->shaderDepthBuf->begin();
			this->objMapForest->squeezeDrawWithDepthSha(this->shaderDepthBuf);
			this->shaderDepthBuf->end();

		//	�L�����N�^�[�̕`��
			this->kageo->draw(inDev);
			this->kagemi->draw(inDev);
			this->shabird->draw(inDev);

		//	�G�t�F�N�g�̕`��
			this->effectCenter->drawEffects(inDev, this->camera);

		//	�V�l�}�X�R�[�v�̕`��
			this->direction->directCinemaScopePaintOut();

		//	�t�H���g�̕`��
			this->gDev->spriteBegin();
			this->executeDrawTelop();
			this->gDev->spriteEnd();

		//	���̕`��
			this->gDev->renderCurtainInDev();
			this->gDev->renderEnd();
		}
	//	���[�r�[�t���O�����A�Ȃ�A�ȉ�������
		else
		{
		//	�`��J�n
			gDev->renderBegin(D3DCOLOR_RGBA(255,255,255,0));

			DEV	inDev	= this->gDev->getDevice();

		//	�J�����̍X�V
			this->camera->updateCamera();

		//	���C�g�̍X�V
			this->updateLight();

		//	Z�e�N�X�`���V�F�[�_�Ńe�N�X�`���ɐ[�x�l������������
			this->shaderZTexture->begin();
			this->objMapCave->squeezeSortByCameraPositionNotCheckMessageWithDrawZTexSha(
				this->camera, &this->camera->getViewPointFinish(), this->shaderZTexture);
			this->shaderZTexture->end();

			this->gDev->bufferClear(D3DCOLOR_RGBA(255,255,255,255));

		//	�J�����Ńf�o�C�X�X�V
			this->camera->viewTransform(inDev);

		//	���A�\�ʂ̕`��
			this->cave->draw(inDev);

		//	�[�x�o�b�t�@�V�F�[�_�ŕ`��
			this->shaderDepthBuf->setCameraViewProjMatrix(
				&((*this->camera->getMatrixView()) * (*this->camera->getMatrixProjection())));

		//	���A�̕`��
			this->shaderDepthBuf->begin();
			this->objMapCave->squeezeDrawWithDepthSha(this->shaderDepthBuf);
			this->shaderDepthBuf->end();

		//	�L�����N�^�[�̕`��
			this->shabird->draw(inDev);
			this->clime->draw(inDev);
			this->tima->draw(inDev);
			this->feather->draw(inDev);
			this->yamakage->draw(inDev);
			this->exe->draw(inDev);

		//	�G�t�F�N�g�̕`��
			this->effectCenter->drawEffects(inDev, this->camera);

		//	�V�l�}�X�R�[�v�̕`��
			this->direction->directCinemaScopePaintOut();

		//	�t�H���g�̕`��
			this->gDev->spriteBegin();
			this->executeDrawTelop();
			this->gDev->spriteEnd();

		//	���̕`��
			this->gDev->renderCurtainInDev();
			this->gDev->renderEnd();
		}
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------