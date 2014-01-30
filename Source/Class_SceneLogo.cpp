//***********************************************************************************
//!	�t�@�C����		Class_SceneLogo.cpp
//
//	�@�\			���S�V�[���̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2008/9/30	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneLogo.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_LOGO::CSCENE_LOGO
	//
	//	�@�\		�R���X�g���N�^
	//	����		inData			�V�[�����L�f�[�^
	//	�X�V		2008/09/30		<�V�K>
	//				2009/01/24		<�ύX>��������
	//================================================================================
	CSCENE_LOGO::CSCENE_LOGO(SCENE_SHARE_DATA* inData)	:
	CSCENE(eSCENE_LOGO, eSCENE_TITLE, inData)		//!< ��b�V�[���t���O�̐ݒ�
	{
	//	������
		this->gDev			= CGRAPHIC_DEVICE::getInstance();
		this->effectCenter	= C3D_EFFECT_CENTER::getInstance();
		this->effectCenter->finishalize();
		this->iFrameCnt		= 0;
		this->iStepCnt		= 0;
		this->logoJec		= NULL;
		this->logoProject	= NULL;
		this->sStaLogo		= NULL;
		this->bEndCheck		= FALSE;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_LOGO::~CSCENE_LOGO
	//
	//	�@�\		�f�X�g���N�^
	//	�X�V		2008/09/30		<�V�K>
	//				2009/01/24		<�ύX>��������
	//================================================================================
	CSCENE_LOGO::~CSCENE_LOGO(void)
	{
	//	���
		sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_LOGO::sceneInitialize
	//
	//	�@�\		������
	//	����		inNum			�O���f�[�^�ԍ�
	//	�X�V		2008/09/30		<�V�K>
	//				2009/01/24		<�ύX>��������
	//================================================================================
	void
	CSCENE_LOGO::sceneInitialize(int inStageNum)
	{
	//	�������
		sceneFinishalize();

	//	���{�d�q���w�Z���S�̏���
		this->logoJec		=
			new CTEXTURE(gDev->getDevice(), "logo_jec", VEC2(512.0f, 512.0f));
		this->logoJec->setLoc(&VEC3(512.0f, 384.0f, 0.0f));

	//	�v���W�F�N�g���S�̏���
		this->logoProject	=
			new C3D_EFFECT(gDev->getDevice(), "logo_project", VEC3(512.0f, 384.0f, 0.0f));

	//	���S�o�����̏���
		this->sStaLogo		= new SSTATIC("logo");

	//	�g�p����G�t�F�N�g�̐���
		this->effectCenter->addEffects(
			EFFECT_SHINE, new C3D_EFFECT(gDev->getDevice(), EFFECT_SHINE, VEC3(0.0f,0.0f,0.0f)));

	//	��������
		gDev->setCurtainColor(0,0,0);
		gDev->setCurtainColorAlpha(255);
		gDev->curtainMotion(FALSE, 10);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_LOGO::sceneFinishalize
	//
	//	�@�\		���
	//	�X�V		2008/09/30		<�V�K>
	//				2009/01/24		<�ύX>��������
	//================================================================================
	void
	CSCENE_LOGO::sceneFinishalize(void)
	{
	//	���
		SAFE_DELETE(this->logoJec);
		SAFE_DELETE(this->logoProject);
		SAFE_DELETE(this->sStaLogo);
		this->effectCenter->finishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_LOGO::initEndCheck
	//
	//	�@�\		�I������
	//	�X�V		2009/02/24		<�V�K>
	//================================================================================
	void
	CSCENE_LOGO::initEndCheck(void)
	{
	//	�I������
		if(	!this->bEndCheck)
		{
			this->bEndCheck	= TRUE;
		//	�����o��������
			gDev->curtainMotion(TRUE, 10);
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_LOGO::endCheck
	//
	//	�@�\		�I���m�F����
	//	�X�V		2009/02/24		<�V�K>
	//================================================================================
	void
	CSCENE_LOGO::endCheck(void)
	{
	//	�I������
		if(	this->bEndCheck)
		{
		//	�{�����[����������
			this->sStaLogo->setVolume(this->sStaLogo->getVolume() - 100);
			if(	gDev->getCurtainColorAlpha() >= 255)
			{
				returnFlag	= nextFlag;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_LOGO::sceneMain
	//
	//	�@�\		�V�[�����C��
	//	����		input			���͏��
	//	�X�V		2008/09/30		<�V�K>
	//				2009/01/24		<�ύX>��������
	//================================================================================
	SCENE_FLAG
	CSCENE_LOGO::sceneMain(CINPUT* input)
	{
	//	����
		execute(input);
	//	�`��
		render();
	//	�I���m�F����
		endCheck();

		return	returnFlag;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_LOGO::execute
	//
	//	�@�\		����
	//	����		input			���͏��
	//	�X�V		2009/01/24		<�V�K>
	//				2009/02/24		<�X�V>���o�ύX
	//================================================================================
	void
	CSCENE_LOGO::execute(CINPUT* input)
	{
	//	�X�e�b�v�J�E���^�ɂ��A�X�L�b�v����
		if(	(this->iStepCnt >= 0)	&&
			(this->iStepCnt <= 3))
		{
			if(	input->checkPressOnce(MYKEY::eDUST)		||
				input->checkPressOnce(MYKEY::eHARD)		||
				input->checkPressOnce(MYKEY::eMIDDLE)	||
				input->checkPressOnce(MYKEY::eSOFT)		||
				input->checkPressOnce(MYKEY::eSTART))
			{
				this->iStepCnt	= 4;
			}
		}
		else
		if(	(this->iStepCnt >= 4))
		{
			if(	input->checkPressOnce(MYKEY::eDUST)		||
				input->checkPressOnce(MYKEY::eHARD)		||
				input->checkPressOnce(MYKEY::eMIDDLE)	||
				input->checkPressOnce(MYKEY::eSOFT)		||
				input->checkPressOnce(MYKEY::eSTART))
			{
				this->initEndCheck();
			}
		}
	//	�X�e�b�v�ɂ��A�������s��
		switch(iStepCnt)
		{
		//	�v���W�F�N�g���S���B���A���{�d�q���w�Z�̃��S�𓧖��ɂ���
			case 0:
				logoProject->setVisible(FALSE);
				logoJec->setColorAlpha(0);
				iStepCnt	= 1;
				break;

		//	���{�d�q���w�Z�̃��S���������o��������
			case 1:
				logoJec->calcColorAlpha(4);
				iFrameCnt++;
				if(	iFrameCnt >= 60)	{ iStepCnt = 2; iFrameCnt = 0;	}
				break;

		//	60�t���[���ҋ@
			case 2:
				iFrameCnt++;
				if(	iFrameCnt >= 60)	{ iStepCnt = 3; iFrameCnt = 0;	}
				break;

		//	���{�d�q���w�Z�̃��S�����������ł�����
			case 3:
				logoJec->calcColorAlpha(-10);
				iFrameCnt++;
				if(	iFrameCnt >= 50)	{ iStepCnt = 4; iFrameCnt = 0;	}
				break;

		//	���{�d�q���w�Z�̃��S�̑��݊m�F�𖳌��ɂ��A
		//	���S�o�����𔭐�
			case 4:
				logoJec->setVisible(FALSE);
				this->sStaLogo->play(0, NULL);
				iStepCnt	= 5;
				iFrameCnt	= 0;
				break;

		//	���t���[���ҋ@���A
		//	�v���W�F�N�g���S�̑��݊m�F��L��
			case 5:
				iFrameCnt++;
				if(	iFrameCnt >= 30)
				{ 
					iStepCnt	= 6;
					iFrameCnt	= 0;
					logoProject->setVisible(TRUE);
				}
				break;

		//	120�t���[���ҋ@
			case 6:
			//	3�t���[�����ɃG�t�F�N�g���ɃG�t�F�N�g����
				if(	!(iFrameCnt % 3))
				{
				//	�ʒu����
					VEC3	inLoc	= logoProject->getLoc();
					inLoc.x	+= float((rand() % (int)logoProject->getWidth())  - int(logoProject->getWidth()  * 0.5f));
					inLoc.y	+= float((rand() % (int)logoProject->getHeight()) - int(logoProject->getHeight() * 0.5f));
				//	�X�P�[������
					float	inScale	= logoProject->getWidth() / 400.0f;
					this->effectCenter->occu2DEffects(
						EFFECT_SHINE, inLoc, inScale, VEC3(0.0f,0.0f,0.0f), D3DRGBA(255,255,255,100));
				}
				iFrameCnt++;
				if(	iFrameCnt >= 360)
				{ 
					iStepCnt	= 7;
					iFrameCnt	= 0;
				//	�I�������ֈڍs
					this->initEndCheck();
				}
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_LOGO::render
	//
	//	�@�\		�`��
	//	�X�V		2009/01/24		<�V�K>
	//================================================================================
	void
	CSCENE_LOGO::render(void)
	{
	//	�`��J�n
		gDev->renderBegin(D3DCOLOR_RGBA(255,255,255,0));
		gDev->spriteBegin();

		logoJec->draw(gDev->getSprite());					//!< ���{�d�q���w�Z���S�̕`��
		logoProject->draw(gDev->getSprite());				//!< �v���W�F�N�g���S�̕`��
		effectCenter->drawEffects2D(gDev->getSprite());		//!< �G�t�F�N�g2D�̕`��

		gDev->spriteEnd();
	//	���̕`��
		gDev->renderCurtainInDev();
		gDev->renderEnd();
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------