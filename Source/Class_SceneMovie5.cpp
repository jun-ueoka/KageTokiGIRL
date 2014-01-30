//***********************************************************************************
//!	�t�@�C����		Class_SceneMovie5.cpp
//
//	�@�\			���[�r�[�V�[���N���X�A���[�r�[5[�U��؂�����c]�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2009/02/10	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneMovie5.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE5::�R���X�g���N�^
	//
	//	����		inFlag		���݂̃V�[���t���O
	//				inNextFlag	���̃V�[���t���O
	//				inData		�V�[�����L�f�[�^
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	CSCENE_MOVIE5::CSCENE_MOVIE5(SCENE_SHARE_DATA* inData)	:
	//!	�Q�[���V�[��
	CSCENE_MOVIE_BASE(eSCENE_CHAPTER2_MOVIE5, eSCENE_CHAPTER2_STAGE5, inData)
	{
	//	������
		this->camera	= NULL;
		this->kageo		= NULL;
		this->kagemi	= NULL;
		this->sky		= NULL;
		this->objMap	= NULL;

		this->backSound			= NULL;
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE5::�f�X�g���N�^
	//
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	CSCENE_MOVIE5::~CSCENE_MOVIE5(void)
	{
	//	���
		sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE5::sceneInitialize
	//
	//	�@�\		������
	//	����		inStageNum	�O���f�[�^�ԍ�
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE5::sceneInitialize(int inStageNum)
	{
	//	���[�r�[�̏�����
		this->initializeMovie(1);

	//	�������
		this->sceneFinishalize();

	//	������

	//	�J����
		this->camera	= new C3D_OBJECT_CAMERA(&VEC3(0.0f,2000.0f,-5000.0f), &VEC3(0.0f,0.0f,0.0f));
		this->camera->setViewFar(20000.0f);
		this->camera->setViewShift(&VEC3(100.0f, -500.0f,  2000.0f));

	//	�J�Q�I
		this->kageo		= new CSKIN_MESH(gDev->getDevice(), "movie_kageo");
		this->kageo->setLoc(&VEC3(400.0f, 0.0f, 0.0f));
		this->kageo->changeAnimationAndBlend(0);

	//	�J�Q�~
		this->kagemi	= new CSKIN_MESH(gDev->getDevice(), "movie_kagemi");
		this->kagemi->setLoc(&VEC3(-400.0f, 0.0f, 0.0f));
		this->kagemi->changeAnimationAndBlend(0);

	//	��
		this->sky	= new CTEXTURE(gDev->getDevice(), "sky", 
							   VEC2((float)gDev->getScreenSizeX(), (float)gDev->getScreenSizeY()));
		this->sky->setLoc(&VEC3(float(gDev->getScreenSizeX()/2), float(gDev->getScreenSizeY()/2), 1.0f));

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
		this->lightShift	= VEC3(2000.0f,  5000.0f, -6000.0f);

	//	���C�g�ˉe�s��̐ݒ�
		D3DXMatrixPerspectiveFovLH(&this->lightProj, D3DXToRadian(70), 1.0f, lightNearFar.x, lightNearFar.y);
	//	------------------------------------------------------

	//	3D�}�b�v
		this->objMap	= new CGAME_MAP();
		this->objMap->initialize(this->gDev->getDevice(), 1);

	//	���[�r�[�̊J�n
		this->movieStartProc();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE5::sceneFinishalize
	//
	//	�@�\		���
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE5::sceneFinishalize(void)
	{
	//	���
		SAFE_DELETE(this->camera);
		SAFE_DELETE(this->kageo);
		SAFE_DELETE(this->kagemi);
		SAFE_DELETE(this->sky);

		SAFE_DELETE(this->objMap);
		SAFE_DELETE(this->backSound);

		CZTEX_SHA::releaseInstance();
		CDEPTH_SHA::releaseInstance();
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE5::sceneMain
	//
	//	�@�\		�V�[�����C��
	//	�@�\�T�v	�Q�[���V�[�����C��
	//	����		input		���͏��
	//	�߂�l		���̃V�[��
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	SCENE_FLAG
	CSCENE_MOVIE5::sceneMain(CINPUT* input)
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
	//!	���\�b�h��	CSCENE_MOVIE5::executeInstantChengeMovieSpeed
	//
	//	�@�\		���[�r�[�������x�ύX������
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE5::executeInstantChengeMovieSpeed(void)
	{
		this->kageo->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->kagemi->setAnimSpeed(0.016f * float(this->movieSpeed));
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE5::executeProcOnce
	//
	//	�@�\		�����������s
	//	����		inProc		�����ԍ�
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE5::executeProcOnce(int inProc)
	{
	//	�����ԍ��ŕ���
		switch(inProc)
		{
		//	�����ݒ�
			case 0:
				this->kageo->setLoc (&VEC3(-8400.0f, 0.0f,  50.0f));
				this->kageo->setAngleY(270.0f);
				this->kageo->changeAnimationAndBlend(5);
				this->kagemi->setLoc(&VEC3(-8400.0f, 0.0f,-150.0f));
				this->kagemi->setAngleY(270.0f);
				this->kagemi->changeAnimationAndBlend(5);
				this->camera->setCameraMode(eCAMO_PARALLEL);
				this->camera->setLoc(&VEC3(0.0f, 5000.0f, -1500.0f));
				this->camera->setViewPointShift(&VEC3(0.0f, -500.0f, 1500.0f));
				this->gDev->createCurtainTex("Loading_KageoKagemiB");
				break;

		//
			case 1:
				this->camera->setCameraMode(eCAMO_NO);
				break;
		//
			case 2:
				this->camera->setCameraMode(eCAMO_PARALLEL);
				break;
		//
			case 3:
				this->kageo->changeAnimationAndBlend(0);
				this->kagemi->changeAnimationAndBlend(0);
				break;
		//
			case 4:
				this->kagemi->changeAnimationAndBlend(6);
				break;
		//
			case 5:
				this->kageo->changeAnimationAndBlend(1);
				break;
		//
			case 6:
				this->kagemi->changeAnimationAndBlend(2);
				break;
		//
			case 7:
				this->kagemi->changeAnimationAndBlend(3);
				break;
		//
			case 8:
				this->kageo->changeAnimationAndBlend(2);
				break;
		//
			case 9:
				this->kagemi->changeAnimationAndBlend(0);
				break;
		//
			case 10:
				this->kagemi->changeAnimationAndBlend(5);
				break;
		//
			case 11:
				this->kageo->changeAnimationAndBlend(0);
				break;
		//
			case 12:
				this->kageo->changeAnimationAndBlend(5);
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE5::executeProc
	//
	//	�@�\		�������s
	//	����		inProc		�����ԍ�
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE5::executeProc(int inProc)
	{
	//	�����ԍ��ŕ���
		switch(inProc)
		{
		//	�����ݒ�
			case 0:
				this->camera->calcLocY(-4.0f);
				break;
		//	
			case 1:
				this->camera->calcLocX(-4.0f);
				this->kageo->calcLocX(3.0f);
				this->kagemi->calcLocX(3.0f);
				break;
		//	
			case 2:
				this->kageo->calcLocX(3.0f);
				this->kagemi->calcLocX(3.0f);
				break;
		//	
			case 3:
				this->camera->calcViewPointX(-1.1f);
				this->camera->calcViewPointY(-2.0f);
				this->camera->calcLocY(-2.5f);
				this->camera->calcLocZ(2.0f);
				this->kageo->calcLocX(3.0f);
				this->kagemi->calcLocX(3.0f);
				break;
		//	
			case 4:
				this->camera->calcLocX(3.0f);
				this->kageo->calcLocX(3.0f);
				this->kagemi->calcLocX(3.0f);
				break;
		//	
			case 5:
				this->kageo->calcLocX(3.0f);
				this->kagemi->calcAngleY(-1.0f);
				break;
		//	
			case 6:
				this->kageo->calcAngleY(2.3f);
				break;
		//	
			case 7:
				this->kagemi->calcAngleY(1.5f);
				break;
		//	
			case 8:
				this->kagemi->calcLocX(6.0f);
				break;
		//	
			case 9:
				this->camera->calcLocY(3.0f);
				this->kageo->calcAngleY(-3.45f);
				this->kageo->calcLocZ(-2.0f);
				this->kagemi->calcLocX(6.0f);
				break;
		//	
			case 10:
				this->camera->calcLocY(3.0f);
				this->kageo->calcLocX(7.0f);
				this->kagemi->calcLocX(6.0f);
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE5::updateLight
	//
	//	�@�\		���C�g�̍X�V
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE5::updateLight(void)
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
	//!	���\�b�h��	CSCENE_MOVIE5::execute
	//
	//	�@�\		����
	//	����		input		���͏��
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE5::execute(CINPUT* input)
	{
	//	���������f�[�^�̍X�V����
		this->checkProcOnce();
	//	�����f�[�^�̍X�V����
		this->checkProc();

	//	3�{������
#ifdef	_DEBUG
		if(	input->checkPress(MYKEY::eSOFT))
		{
			this->iMovieFrame++;
			this->checkProcOnce();
			this->checkProc();
			this->iMovieFrame++;
			this->checkProcOnce();
			this->checkProc();
			this->iMovieFrame++;
			this->checkProcOnce();
			this->checkProc();
			this->iMovieFrame++;
			this->checkProcOnce();
			this->checkProc();

		//	�X�L�����b�V���̑�����
			this->kageo->setAnimSpeed(0.080f);
			this->kagemi->setAnimSpeed(0.080f);
		}
#else
		if(	input->checkPress(MYKEY::eSOFT))
		{
			this->iMovieFrame++;
			this->checkProcOnce();
			this->checkProc();
			this->iMovieFrame++;
			this->checkProcOnce();
			this->checkProc();

		//	�X�L�����b�V���̑�����
			this->kageo->setAnimSpeed(0.048f);
			this->kagemi->setAnimSpeed(0.048f);
		}
#endif
	//	1�{������
		else
		{
			this->kageo->setAnimSpeed();
			this->kagemi->setAnimSpeed();
		}

	//	���݂̃t���[�����I���t���[���ɓ��B�������A
	//	�I�����߂��ł���A���[�r�[�I��������
		if(	(iMovieFrame >= iFinishFrame)	||
			input->checkPressOnce(MYKEY::eSTART))
		{
			this->movieEndProc();
		}
		else
		{
			this->iMovieFrame++;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE5::render
	//
	//	�@�\		�`��
	//	�X�V		2009/02/10	<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE5::render(void)
	{
	//	�`��J�n
		gDev->renderBegin(D3DCOLOR_RGBA(255,255,255,0));

		DEV	inDev	= this->gDev->getDevice();

	//	�I�u�W�F�N�g�̃\�[�g
		this->camera->updateCamera();
		this->objMap->squeezeSortByCameraPosition(
			this->camera, &this->camera->getViewPointFinish());

	//	���C�g�̍X�V
		this->updateLight();

	//	Z�e�N�X�`���V�F�[�_�Ńe�N�X�`���ɐ[�x�l������������
		this->shaderZTexture->begin();
		this->objMap->squeezeDrawWithZTexSha(this->shaderZTexture);
		this->shaderZTexture->end();

		this->gDev->bufferClear(D3DCOLOR_RGBA(255,255,255,255));

	//	��̕`��
		gDev->spriteBegin();
		sky->draw(gDev->getSprite());
		gDev->spriteEnd();

	//	�J�����Ńf�o�C�X�X�V
		this->camera->viewTransform(inDev);

	//	�[�x�o�b�t�@�V�F�[�_�ŕ`��
		this->shaderDepthBuf->setCameraViewProjMatrix(
			&((*this->camera->getMatrixView()) * (*this->camera->getMatrixProjection())));

		kageo->draw(inDev);
		kagemi->draw(inDev);

		this->shaderDepthBuf->begin();
		this->objMap->squeezeDrawWithDepthSha(this->shaderDepthBuf);
		this->shaderDepthBuf->end();

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