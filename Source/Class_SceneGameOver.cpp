//***********************************************************************************
//!	�t�@�C����		Class_SceneGameOver.cpp
//
//	�@�\			�Q�[���I�[�o�[�V�[���̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2009/02/05	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneGameOver.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_GAMEOVER::CSCENE_GAMEOVER
	//
	//	�@�\		�R���X�g���N�^
	//	����		inData			�V�[�����L�f�[�^
	//	�X�V		2009/02/05		<�V�K>
	//================================================================================
	CSCENE_GAMEOVER::CSCENE_GAMEOVER(SCENE_SHARE_DATA* inData)	:
	CSCENE(eSCENE_GAMEOVER, eSCENE_LOGO, inData)			//!< ��b�V�[���t���O�̐ݒ�
	{
	//	�������̏�����
		gDev			= CGRAPHIC_DEVICE::getInstance();
		font			= CFONT_BASE::getInstance();

	//	���o�����̏�����
		sStreamingBgm	= NULL;
		texGameOver		= NULL;
		iFrameCnt		= 0;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_GAMEOVER::~CSCENE_GAMEOVER
	//
	//	�@�\		�f�X�g���N�^
	//	�X�V		2009/02/05		<�V�K>
	//================================================================================
	CSCENE_GAMEOVER::~CSCENE_GAMEOVER(void)
	{
	//	���
		sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_GAMEOVER::sceneInitialize
	//
	//	�@�\		������
	//	����		inNum			�O���f�[�^�ԍ�
	//	�X�V		2009/02/05		<�V�K>
	//================================================================================
	void
	CSCENE_GAMEOVER::sceneInitialize(int inStageNum)
	{
	//	�������
		sceneFinishalize();

	//	���o�����̏�����
		texGameOver	= new CTEXTURE(gDev->getDevice(), "game_over",
								   VEC2(1512.0f, 756.0f), 0xFFFFFF96);
		texGameOver->setLoc(&VEC3(512.0, 384.0f, 0.5f));
		texGameOver->setColorAlpha(0);
		iFrameCnt	= 0;

	//	���̏�����
	//	������������
		gDev->setCurtainColor(0, 0, 0);
		gDev->setCurtainColorAlpha(0);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_GAMEOVER::sceneFinishalize
	//
	//	�@�\		���
	//	�X�V		2009/02/05		<�V�K>
	//================================================================================
	void
	CSCENE_GAMEOVER::sceneFinishalize(void)
	{
	//	���o�����̉��
		SAFE_DELETE(sStreamingBgm);
		SAFE_DELETE(texGameOver);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_GAMEOVER::sceneMain
	//
	//	�@�\		�V�[�����C��
	//	����		input			���͏��
	//	�߂�l		���݂̃V�[���t���O
	//	�X�V		2009/02/05		<�V�K>
	//================================================================================
	SCENE_FLAG
	CSCENE_GAMEOVER::sceneMain(CINPUT* input)
	{
	//	����
		this->execute(input);
	//	�`��
		this->render();

		return	returnFlag;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_GAMEOVER::execute
	//
	//	�@�\		����
	//	����		input			���͏��
	//	�X�V		2009/02/05		<�V�K>
	//================================================================================
	void
	CSCENE_GAMEOVER::execute(CINPUT* input)
	{
	//	120�t���[���ԁA�Q�[���I�[�o�[�̕������k�����A�A���t�@�l�グ��
		iFrameCnt++;
		if(	iFrameCnt < 200)
		{
			texGameOver->calcShowSize(&VEC2(-5.0f, -2.5f));
			texGameOver->calcColorAlpha(2);
		}
	//	
		else
		if(	iFrameCnt < 320)
		{
		}
		else
		if(	iFrameCnt < 325)
		{
		//	�����o��
			gDev->curtainMotion(TRUE, 5);
		}
		else
		{
		//	���̓����x���ő�Ȃ�A�R���e�B�j���[�I�������ڍs����
			if(	gDev->getCurtainColorAlpha() >= 255)
			{
				returnFlag	= nextFlag;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_GAMEOVER::render
	//
	//	�@�\		�`��
	//	�X�V		2009/02/05		<�V�K>
	//================================================================================
	void
	CSCENE_GAMEOVER::render(void)
	{
	//	�`��J�n
		gDev->renderBegin(D3DCOLOR_RGBA(0,0,0,0));

		gDev->spriteBegin();
		texGameOver->draw(gDev->getSprite());
		gDev->spriteEnd();

	//	���̕`��
		gDev->renderCurtainInDev();
	//	�`��I��
		gDev->renderEnd();
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------