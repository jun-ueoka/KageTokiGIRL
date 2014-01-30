//***********************************************************************************
//!	�t�@�C����		Class_SceneDemo.cpp
//
//	�@�\			�f���̃V�[��������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2008/09/29	<�V�K>
//					2008/11/17	<�ύX>	�Q�[���V�[�����p��
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneDemo.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_DEMO::CSCENE_DEMO
	//
	//	�@�\		�R���X�g���N�^
	//	����		inData				�V�[�����L�f�[�^
	//	�X�V		2008/09/29	<�V�K>
	//				2008/11/17	<�ύX>	�Q�[���V�[�����p��
	//================================================================================
	CSCENE_DEMO::CSCENE_DEMO(SCENE_SHARE_DATA* inData)	:
	CSCENE_ADVENTURE_GAME(eSCENE_DEMO, eSCENE_TITLE, eSCENE_TITLE, inData)		//!< �Q�[���V�[��
	{
	//	������
		this->effDemo	= NULL;			//!< �f���\���̃G�t�F�N�g
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_DEMO::~CSCENE_DEMO
	//
	//	�@�\		�f�X�g���N�^
	//	�X�V		2008/09/29	<�V�K>
	//				2008/11/17	<�ύX>	�Q�[���V�[�����p��
	//================================================================================
	CSCENE_DEMO::~CSCENE_DEMO(void)
	{
	//	���
		this->sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_DEMO::sdInitialize
	//
	//	�@�\		�f���̏�����
	//	�X�V		2008/11/17		<�V�K>
	//================================================================================
	void
	CSCENE_DEMO::sdInitialize(void)
	{
	//	�����J��
		this->sdFinishalize();
	//	�f���G�t�F�N�g�ԍ��𗐐�����
		int	inDemoEffect	= DEMO_EFFECT_NUM;
	//	�f���\���̃G�t�F�N�g�p�^�[�������擾
		TCHAR	effectName[MAX_PATH]	= "";
	//	�f���G�t�F�N�g���𐶐�
		sprintf_s(effectName, sizeof(effectName), NAME_DEMO, inDemoEffect);
	//	����
		this->effDemo	=
			new C3D_EFFECT(	this->gDev->getDevice(),
							effectName,
							VEC3(float(this->gDev->getScreenSizeX()) / 2.0f,
								 float(this->gDev->getScreenSizeY()) / 2.0f,
								 0.0f));
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_DEMO::sdFinishalize
	//
	//	�@�\		�f���̏�����
	//	�X�V		2008/11/17		<�V�K>
	//================================================================================
	void
	CSCENE_DEMO::sdFinishalize(void)
	{
	//	���
		SAFE_DELETE(this->effDemo);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_DEMO::sceneInitialize
	//
	//	�@�\		������
	//	����		inStageNum		�O���f�[�^�ԍ�
	//	�X�V		2008/11/18		<�V�K>
	//================================================================================
	void
	CSCENE_DEMO::sceneInitialize(int inStageNum)
	{
	//	�f���p�̃X�e�[�W�ԍ����擾
		inStageNum	= DEMO_STAGE_NUM;
	//	�Q�[���V�[���̏�����
		CSCENE_ADVENTURE_GAME::sceneInitialize(inStageNum);
	//	�f���̏�����
		this->sdInitialize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_DEMO::sceneFinishalize
	//
	//	�@�\		���
	//	�X�V		2008/11/18		<�V�K>
	//================================================================================
	void
	CSCENE_DEMO::sceneFinishalize(void)
	{
	//	�f���̉��
		this->sdFinishalize();
	//	�Q�[���V�[���̊J��
		CSCENE_ADVENTURE_GAME::sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_DEMO::sceneMain
	//
	//	�@�\		�V�[�����C��
	//	�@�\�T�v	�Q�[���V�[���̃��\�b�h���g���A�f�����`��
	//	�X�V		2008/09/19		<�V�K>
	//================================================================================
	SCENE_FLAG
	CSCENE_DEMO::sceneMain(CINPUT* input)
	{
	//	�Q�[���t���O�ŏ�������
		switch(this->gameFlag)
		{
		//	�Q�[���C�x���g��
			case	eSGFLAG_EVENT:
				this->sgEventExecute(input);
				this->sgEventRender();
			//	�Q�[���C�x���g���ɃQ�[���v���C�Ɉڂ������A
			//	���̓X�^�[�g�{�^���������ꂽ��A�f���I��
				if(	(this->gameFlag == eSGFLAG_PLAY)	||
					(input->checkPressOnce(MYKEY::eSTART)))
				{
					this->sgQuitChange();
				}
				break;
		//	�Q�[���I����(�f���I��)
			case	eSGFLAG_QUIT:
				this->sgQuitExecute(input);
				this->sgQuitRender();
				break;
		//	�Q�[���G���[
			case	eSGFLAG_ERROR:
				this->returnFlag	= eSCENE_ERROR;
				break;
		//	��L�ȊO
			default:
			//	�����I��
				this->returnFlag	= eSCENE_TITLE;
				break;
		}

		return	this->returnFlag;
	}

	//��������������������������������������������������������������������������
	//	�I�[�o�[���C�h����
	//��������������������������������������������������������������������������
	//================================================================================
	//!	���\�b�h��	CSCENE_DEMO::sgEventRender
	//
	//	�@�\		�Q�[���C�x���g���`��
	//	�X�V		2008/11/27		<�V�K>
	//================================================================================
	void
	CSCENE_DEMO::sgEventRender(void)
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
		this->eventCommentG->drawComment(this->gDev->getSprite());
		this->effectCenter->drawEffects2D(this->gDev->getSprite());
		this->effDemo->draw(this->gDev->getSprite());
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