//***********************************************************************************
//!	�t�@�C����		Class_SceneEnding.cpp
//
//	�@�\			�G���f�B���O�V�[���̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2008/9/30	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneEnding.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_ENDING::CSCENE_ENDING
	//
	//	�@�\		�R���X�g���N�^
	//	����		inFlag			���݂̃V�[���t���O
	//				inNextFlag		���̃V�[���t���O
	//				inData			�V�[�����L�f�[�^
	//	�X�V		2008/10/05		<�V�K>
	//================================================================================
	CSCENE_ENDING::CSCENE_ENDING(SCENE_SHARE_DATA* inData)	:
	CSCENE_MOVIE_BASE(eSCENE_ENDING, eSCENE_LOGO, inData)	//!< ��b�V�[���t���O�̐ݒ�
	{
	//	���o�����̏�����
		this->camera	= NULL;
		this->kageo		= NULL;
		this->kagemi	= NULL;
		this->drawMap	= NULL;
		this->objMap1	= NULL;
		this->objMap2	= NULL;
		this->objMap3	= NULL;
		this->objMap4	= NULL;
		this->drawBack	= NULL;
		this->back1		= NULL;

		this->iRoll		= 0;

		this->sStreamingBgm		= NULL;
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ENDING::~CSCENE_ENDING
	//
	//	�@�\		�f�X�g���N�^
	//	�X�V		2008/10/05		<�V�K>
	//================================================================================
	CSCENE_ENDING::~CSCENE_ENDING(void)
	{
	//	���
		sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ENDING::sceneInitialize
	//
	//	�@�\		������
	//	����		inNum			�O���f�[�^�ԍ�
	//	�X�V		2008/10/05		<�V�K>
	//================================================================================
	void
	CSCENE_ENDING::sceneInitialize(int inStageNum)
	{
	//	���[�r�[�̏�����
		this->initializeMovie(1000);

	//	�������
		sceneFinishalize();

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


	//	���o�����̏�����
		sStreamingBgm	= new SSTREAMING("ainoaisatu");
		this->sStreamingBgm->play(0, DSBPLAY_LOOPING);

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
		this->objMap1	= new CGAME_MAP();
		this->objMap1->initialize(this->gDev->getDevice(), 201);
		this->objMap2	= new CGAME_MAP();
		this->objMap2->initialize(this->gDev->getDevice(), 10);
		this->objMap3	= new CGAME_MAP();
		this->objMap3->initialize(this->gDev->getDevice(), 20);
		this->objMap4	= new CGAME_MAP();
		this->objMap4->initialize(this->gDev->getDevice(), 30);
		this->drawMap	= this->objMap1;

	//	�w�i
		this->back1		= new CBACK_SKY(this->gDev->getDevice(), &VEC3(0.0f,0.0f,0.0f));
		this->drawBack	= this->back1;

	//	�G���f�B���O�p�̃t�H���g�𐶐�
		this->font->createFont(
			this->gDev->getDevice(),
			eFONT_STAFFROLL,
			"HG�n�p�p�߯�ߑ�",
			40, 20, 3, 0);

	//	���[��������̓o�^
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "REOJECT KageToki",
			300,   900, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "Staff",
			450,  1800, 0xFF000000));


		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "Programmer Leader",
			200,  2600, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "�㉪ ��",
			250,  2800, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "(�������� �����)",
			500,  2850, 0xFF000000));


		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "Designer Leader",
			200,  3650, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "���� �D��",
			250,  3850, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "(���܂� �䂤��)",
			500,  3900, 0xFF000000));


		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "Programmer",
			200,  4700, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "���� �q��",
			250,  4900, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "(������ �Ƃ���)",
			500,  4950, 0xFF000000));


		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "Designer",
			200,  5750, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "�쌴 �l",
			250,  5950, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "(����͂� ������)",
			500,  6000, 0xFF000000));


		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "���R �L��",
			250,  6400, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "(�Ђ��� �Ђ��)",
			500,  6450, 0xFF000000));

		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "�y�� ���q",
			250,  6800, 0xFF000000));
		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "(�������� ������)",
			500,  6850, 0xFF000000));

		this->staffRoll.push_back(
			FONT_STRING(eFONT_STAFFROLL, "�����",
			450,  8000, 0xFF000000));

	//	���[�r�[�̊J�n
		this->movieStartProc();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ENDING::sceneFinishalize
	//
	//	�@�\		���
	//	�X�V		2008/10/05		<�V�K>
	//================================================================================
	void
	CSCENE_ENDING::sceneFinishalize(void)
	{
	//	���o�����̉��
		SAFE_DELETE(this->camera);
		SAFE_DELETE(this->kageo);
		SAFE_DELETE(this->kagemi);
		SAFE_DELETE(this->objMap1);
		SAFE_DELETE(this->objMap2);
		SAFE_DELETE(this->objMap3);
		SAFE_DELETE(this->objMap4);
		SAFE_DELETE(this->back1);
		this->drawMap	= NULL;
		this->drawBack	= NULL;
		SAFE_DELETE(this->sStreamingBgm);

		CZTEX_SHA::releaseInstance();
		CDEPTH_SHA::releaseInstance();
		this->shaderZTexture	= NULL;
		this->shaderDepthBuf	= NULL;

		this->staffRoll.clear();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ENDING::sceneMain
	//
	//	�@�\		�V�[�����C��
	//	����		input			���͏��
	//	�߂�l		���݂̃V�[���t���O
	//	�X�V		2008/10/05		<�V�K>
	//================================================================================
	SCENE_FLAG
	CSCENE_ENDING::sceneMain(CINPUT* input)
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
	//!	���\�b�h��	CSCENE_ENDING::motionStaffRoll
	//
	//	�@�\		���[������
	//	�X�V		2009/02/23		<�V�K>
	//================================================================================
	void
	CSCENE_ENDING::motionStaffRoll(void)
	{
		this->iRoll	+= ROLL_SPEED;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ENDING::renderStaffRoll
	//
	//	�@�\		�X�^�b�t���[����ʕ`��
	//	�X�V		2008/02/23		<�V�K>
	//================================================================================
	void
	CSCENE_ENDING::renderStaffRoll(void)
	{
	//	�X�^�b�t���[����������A���[���l�������炵�āA�`��
		VEC_LOOP(FONT_STRING, this->staffRoll)
		{
			OffsetRect(&(p->rcPos), 0, -this->iRoll);
			if(	((p->rcPos).top			< 768)	&&
				((p->rcPos).top + 100	> 0))
			{
				this->font->drawText(this->gDev->getSprite(), &(*p));
			}
		}
	//	���[���l��������
		this->iRoll	= 0;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ENDING::executeInstantChengeMovieSpeed
	//
	//	�@�\		���[�r�[�������x�ύX������
	//	�X�V		2009/02/15	<�V�K>
	//================================================================================
	void
	CSCENE_ENDING::executeInstantChengeMovieSpeed(void)
	{
		this->kageo->setAnimSpeed(0.016f * float(this->movieSpeed));
		this->kagemi->setAnimSpeed(0.016f * float(this->movieSpeed));
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ENDING::executeProcOnce
	//
	//	�@�\		�����������s
	//	����		inProc		�����ԍ�
	//	�X�V		2009/01/27	<�V�K>
	//================================================================================
	void
	CSCENE_ENDING::executeProcOnce(int inProc)
	{
	//	�����ԍ��ŕ���
		switch(inProc)
		{
		//	�����ݒ�
			case 0:
				this->kageo->setLoc (&VEC3(-5000.0f, 0.0f,  50.0f));
				this->kageo->setAngleY(270.0f);
				this->kageo->changeAnimationAndBlend(5);
				this->kagemi->setLoc(&VEC3(-5000.0f, 0.0f,-150.0f));
				this->kagemi->setAngleY(270.0f);
				this->kagemi->changeAnimationAndBlend(5);
				this->camera->setCameraMode(eCAMO_PARALLEL);
				this->camera->setLoc(&VEC3(-4000.0f, 500.0f, -1500.0f));
				this->camera->setViewPointShift(&VEC3(0.0f, -300.0f, 1500.0f));
				this->curtainColor.color	= 0x00000000;
				break;
		//
			case 1:
				this->kageo->setVisible(FALSE);
				this->kagemi->setVisible(FALSE);
				break;
		//
			case 2:
				this->drawMap	= this->objMap2;
				this->camera->setLoc(&VEC3(1000.0f, 500.0f, -1500.0f));
				this->camera->setViewPointShift(&VEC3(0.0f, -300.0f, 1500.0f));
				this->executeProcOnce(6);
				break;
		//
			case 3:
				this->drawMap	= this->objMap3;
				this->camera->setLoc(&VEC3(5000.0f, 1500.0f, -3000.0f));
				this->camera->setViewPointShift(&VEC3(0.0f, -300.0f, 3000.0f));
				this->executeProcOnce(6);
				break;
		//
			case 4:
				this->drawMap	= this->objMap4;
				this->camera->setLoc(&VEC3(1000.0f, 1000.0f, -1500.0f));
				this->camera->setViewPointShift(&VEC3(0.0f, -300.0f, 1500.0f));
				this->executeProcOnce(6);
				break;
		//
			case 5:
				this->gDev->curtainMotion(TRUE, 6);
				this->gDev->createCurtainTex();
				break;
		//
			case 6:
				this->gDev->curtainMotion(FALSE, 6);
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ENDING::executeProc
	//
	//	�@�\		�������s
	//	����		inProc		�����ԍ�
	//	�X�V		2009/01/27	<�V�K>
	//================================================================================
	void
	CSCENE_ENDING::executeProc(int inProc)
	{
	//	�����ԍ��ŕ���
		switch(inProc)
		{
		//
			case 0:
				this->camera->calcLocX(2.5f);
				this->kageo->calcLocX(3.5f);
				this->kagemi->calcLocX(8.0f);
				break;
		//
			case 1:
				this->camera->calcLocX(2.5f);
				this->kageo->calcLocX(8.0f);
				this->kagemi->calcLocX(8.0f);
				break;
		//
			case 2:
				this->camera->calcLocX(2.5f);
				break;
		//
			case 3:
				this->camera->calcLocX(2.5f);
				this->motionStaffRoll();
				break;
		//
			case 4:
				this->camera->calcLocX(2.5f);
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ENDING::updateLight
	//
	//	�@�\		���C�g�̍X�V
	//	�X�V		2009/02/05	<�V�K>
	//================================================================================
	void
	CSCENE_ENDING::updateLight(void)
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
	//!	���\�b�h��	CSCENE_ENDING::execute
	//
	//	�@�\		����
	//	����		input		���͏��
	//	�X�V		2009/01/27	<�V�K>
	//================================================================================
	void
	CSCENE_ENDING::execute(CINPUT* input)
	{
		this->checkMovieInput(input);
		this->checkProcFollowMovieSpeed();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ENDING::render
	//
	//	�@�\		�`��
	//	�X�V		2009/01/26		<�V�K>
	//================================================================================
	void
	CSCENE_ENDING::render(void)
	{
	//	�`��J�n
		gDev->renderBegin(D3DCOLOR_RGBA(0,0,0,0));

		DEV	inDev	= this->gDev->getDevice();

	//	�J�����̍X�V
		this->camera->updateCamera();

	//	���C�g�̍X�V
		this->updateLight();

	//	Z�e�N�X�`���V�F�[�_�Ńe�N�X�`���ɐ[�x�l������������
		this->shaderZTexture->begin();
		this->drawMap->squeezeSortByCameraPositionNotCheckMessageWithDrawZTexSha(
			this->camera, &this->camera->getViewPointFinish(), this->shaderZTexture);
		this->shaderZTexture->end();

		this->gDev->bufferClear(D3DCOLOR_RGBA(255,255,255,255));

		gDev->spriteBegin();
		gDev->spriteEnd();

	//	�J�����Ńf�o�C�X�X�V
		this->camera->viewTransform(inDev);

	//	�w�i�̕`��
		this->drawBack->motion(&this->camera->getViewPointFinish());
		this->drawBack->drawBack(inDev, this->gDev->getSprite());

	//	�[�x�o�b�t�@�V�F�[�_�ŕ`��
		this->shaderDepthBuf->setCameraViewProjMatrix(
			&((*this->camera->getMatrixView()) * (*this->camera->getMatrixProjection())));

		this->shaderDepthBuf->begin();
		this->drawMap->squeezeDrawWithDepthSha(this->shaderDepthBuf);
		this->shaderDepthBuf->end();

		kageo->draw(inDev);
		kagemi->draw(inDev);

	//	�V�l�}�X�R�[�v�̕`��
		direction->directCinemaScopePaintOut();

	//	�t�H���g�̕`��
		gDev->spriteBegin();
		renderStaffRoll();
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