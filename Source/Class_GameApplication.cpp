//***********************************************************************************
/*!	\file		Class_GameApplication.cpp
 *
 *	\brief		�Q�[���A�v���P�[�V�����̎���.
 *
 *�@���̃t�@�C����Class_GameApplication.cpp�ł�.
 *
 *	\author		�㉪�@��
 *	\version	KageTokiGIRL
 *	\date		2008/04/17	<�V�K>
 */
//***********************************************************************************

#include	"Precompiler.h"		//!< �v���R���p�C���[

//-----------------------------------------------------------------------
namespace GAPP		//!< \namespace �Q�[���A�v���P�[�V�����̖��O���
//-----------------------------------------------------------------------
{
	//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
	/*!	\class	CGAME_APPLICATION
	 *
	 *	\brief	�Q�[���A�v���P�[�V����
	 */
	//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[

	//	�V�[���̖��O��Ԃ����
	using	namespace	SCE;

	//	�C���X�^���X�����ꏊ
	CGAME_APPLICATION*	CGAME_APPLICATION::gUniqueInstance	= NULL;

	//================================================================================
	/*!	\method	�R���X�g���N�^
	 *
	 *	\brief	�������̏������s��
	 *
	 *	\param	inWnd		[in]�E�B���h�E�n���h��
	 *
	 *	\date	2008/04/17	<�V�K>
	 */
	CGAME_APPLICATION::CGAME_APPLICATION(HWND inWnd)
	{
	//	NULL��
		this->scene					= NULL;
		this->inputKeyBoard			= NULL;
		this->inputGamePad			= NULL;
		this->effectCenterInstance	= NULL;
		this->soundInstance			= NULL;

		this->scenePrevFlag			= eSCENE_NO;
		this->sceneCurrFlag			= eSCENE_NO;

		this->initialize(inWnd);
	}

	//================================================================================
	/*!	\method	�f�X�g���N�^
	 *
	 *	\brief	������̏������s��
	 *
	 *	\date	2008/04/17	<�V�K>
	 */
	CGAME_APPLICATION::~CGAME_APPLICATION(void)
	{
		this->finishalize();
	}

	//================================================================================
	/*!	\method	getInstance(�N���X���\�b�h)
	 *
	 *	\brief	��x�����C���X�^���X�𐶐����A�C���X�^���X��Ԃ�
	 *
	 *	\param	inWnd		[in]�E�B���h�E�n���h��
	 *
	 *	\return	�������ꂽ�C���X�^���X
	 *
	 *	\date	2008/04/17	<�V�K>
	 */
	CGAME_APPLICATION*
	CGAME_APPLICATION::getInstance(HWND inWnd)
	{
	//	��������Ă��Ȃ���΁A��������
		if(	!gUniqueInstance)
		{
		//	�C���X�^���X����
			gUniqueInstance = new CGAME_APPLICATION(inWnd);
		}
	//	�C���X�^���X��Ԃ�
		return gUniqueInstance;
	}

	//================================================================================
	/*!	\method	releaseInstance(�N���X���\�b�h)
	 *
	 *	\brief	�C���X�^���X���������
	 *
	 *	\date	2008/04/17	<�V�K>
	 */
	void
	CGAME_APPLICATION::releaseInstance(void)
	{
	//	�C���X�^���X�̉��
		SAFE_DELETE(gUniqueInstance);
	}

	//================================================================================
	/*!	\method	initialize
	 *
	 *	\brief	�������������s��
	 *
	 *	\param	inWnd		[in]�E�B���h�E�n���h��
	 *
	 *	\date	2008/04/17	<�V�K>
	 *	\date	2008/04/24	<�ǉ�>	�V�[���̏�����
	 */
	void
	CGAME_APPLICATION::initialize(HWND inWnd)
	{
		this->finishalize();					//	���
		this->hWnd			= inWnd;			//	�E�B���h�E�n���h���̏�����
		this->scenePrevFlag	= eSCENE_NO;		//	�O��̃Q�[���t���O�̏�����
		this->sceneCurrFlag	= eSCENE_NO;		//	���݂̃Q�[���t���O�̏�����

	//	�V�[�����L�f�[�^�̏�����
		ZeroMemory(&this->sShareData, sizeof(this->sShareData));
		this->sShareData.dwPlayerLife	= 3;

	//	����������
		srand((unsigned)time(NULL));

	//	���͋@��̐ݒ�
		MYERROR_DI::TYPENAME	rtvInputDevice;
	//	----------------------------------------------------------------------------------------------						
	//	�L�[�{�[�h��DirectInput�@��Ƃ��Đݒ肷��
	//	�C���X�^���X�擾	���̓f�o�C�X0�@�i�L�[�{�[�h�j
		this->inputKeyBoard	= CINPUT::getInstance(0);

		if(	this->inputKeyBoard)
		{
		//	�g�p����L�[�ƑΉ������鉼�z�{�^����ݒ�
			sKeyParam	keyBoardKeys[]={
			//	�L�[�{�[�h�̃L�[�i���L�[�j		���z�Q�[���p�b�h�̃{�^��		�L�[�̏������
				{DIK_UP,						MYKEY::eUP,						MYKEYSTAT::eFREE},		//��L�[
				{DIK_DOWN,						MYKEY::eDOWN,					MYKEYSTAT::eFREE},		//���L�[
				{DIK_LEFT,						MYKEY::eLEFT,					MYKEYSTAT::eFREE},		//���L�[
				{DIK_RIGHT,						MYKEY::eRIGHT,					MYKEYSTAT::eFREE},		//�E�L�[
				{DIK_C,							MYKEY::eSOFT,					MYKEYSTAT::eFREE},		//�b�L�[
				{DIK_X,							MYKEY::eMIDDLE,					MYKEYSTAT::eFREE},		//�w�L�[
				{DIK_B,							MYKEY::eHARD,					MYKEYSTAT::eFREE},		//�a�L�[
				{DIK_Z,							MYKEY::eDUST,					MYKEYSTAT::eFREE},		//�y�L�[
				{DIK_Q,							MYKEY::eL1,						MYKEYSTAT::eFREE},		//�p�L�[
				{DIK_E,							MYKEY::eL2,						MYKEYSTAT::eFREE},		//�d�L�[
				{DIK_R,							MYKEY::eR1,						MYKEYSTAT::eFREE},		//�q�L�[
				{DIK_F,							MYKEY::eR2,						MYKEYSTAT::eFREE},		//�e�L�[
				{DIK_RETURN,					MYKEY::eSELECT,					MYKEYSTAT::eFREE},		//Enter
				{DIK_SPACE,						MYKEY::eSTART,					MYKEYSTAT::eFREE},		//Space
				{DIK_ESCAPE,					MYKEY::eQUIT,					MYKEYSTAT::eFREE},		//Esc
#ifdef	_DEBUG
				{DIK_F1,						MYKEY::eF1,						MYKEYSTAT::eFREE},		//F1
				{DIK_F2,						MYKEY::eF2,						MYKEYSTAT::eFREE},		//F2
				{DIK_F3,						MYKEY::eF3,						MYKEYSTAT::eFREE},		//F3
				{DIK_F4,						MYKEY::eF4,						MYKEYSTAT::eFREE},		//F4
				{DIK_F5,						MYKEY::eF5,						MYKEYSTAT::eFREE},		//F5
				{DIK_F6,						MYKEY::eF6,						MYKEYSTAT::eFREE},		//F6
				{DIK_F7,						MYKEY::eF7,						MYKEYSTAT::eFREE},		//F7
				{DIK_F8,						MYKEY::eF8,						MYKEYSTAT::eFREE},		//F8
				{DIK_F9,						MYKEY::eF9,						MYKEYSTAT::eFREE},		//F9
				{DIK_F10,						MYKEY::eF10,					MYKEYSTAT::eFREE},		//F10
#else 
#ifdef	PRESENTATION
				{DIK_F1,						MYKEY::eF1,						MYKEYSTAT::eFREE},		//F1
				{DIK_F2,						MYKEY::eF2,						MYKEYSTAT::eFREE},		//F2
				{DIK_F3,						MYKEY::eF3,						MYKEYSTAT::eFREE},		//F3
				{DIK_F4,						MYKEY::eF4,						MYKEYSTAT::eFREE},		//F4
				{DIK_F5,						MYKEY::eF5,						MYKEYSTAT::eFREE},		//F5
				{DIK_F6,						MYKEY::eF6,						MYKEYSTAT::eFREE},		//F6
				{DIK_F7,						MYKEY::eF7,						MYKEYSTAT::eFREE},		//F7
				{DIK_F8,						MYKEY::eF8,						MYKEYSTAT::eFREE},		//F8
				{DIK_F9,						MYKEY::eF9,						MYKEYSTAT::eFREE},		//F9
				{DIK_F10,						MYKEY::eF10,					MYKEYSTAT::eFREE},		//F10
				{DIK_1,							MYKEY::e1,						MYKEYSTAT::eFREE},		//1
				{DIK_2,							MYKEY::e2,						MYKEYSTAT::eFREE},		//2
				{DIK_3,							MYKEY::e3,						MYKEYSTAT::eFREE},		//3
				{DIK_4,							MYKEY::e4,						MYKEYSTAT::eFREE},		//4
				{DIK_5,							MYKEY::e5,						MYKEYSTAT::eFREE},		//5
				{DIK_6,							MYKEY::e6,						MYKEYSTAT::eFREE},		//6
				{DIK_7,							MYKEY::e7,						MYKEYSTAT::eFREE},		//7
				{DIK_8,							MYKEY::e8,						MYKEYSTAT::eFREE},		//8
				{DIK_9,							MYKEY::e9,						MYKEYSTAT::eFREE},		//9
				{DIK_0,							MYKEY::e0,						MYKEYSTAT::eFREE},		//10
#endif
#endif
			};
		//	���̓f�o�C�X���g�p����L�[��ݒ�
			rtvInputDevice = this->inputKeyBoard->init(
								inWnd,										//!< �E�B���h�E�n���h��
								DEVICETYPE::eKEYBOARD,						//�f�o�C�X�^�C�v�F�L�[�{�[�h
								sizeof(keyBoardKeys) / sizeof(sKeyParam),	//���͎�ސ�
								keyBoardKeys);
		//	�f�o�C�X�̐����`�F�b�N�i���s�ꍇ�J�������ցj
			if(rtvInputDevice != MYERROR_DI::eNOTHING)
			{
				CINPUT::releaseInstance(0);
			}
		}
	//	----------------------------------------------------------------------------------------------						
	//	�Q�[���p�b�h��DirectInput�@��Ƃ��Đݒ肷��
	//	�C���X�^���X�擾	���̓f�o�C�X1�@�i�Q�[���pbr�b�h�P�ԁj
		this->inputGamePad	= CINPUT::getInstance(1);

		if(	this->inputGamePad)
		{
		//	�g�p����{�^���ƑΉ������鉼�z�{�^����ݒ�
			sKeyParam	gemePadKeys[]={
			//	�Q�[���p�b�g�̃{�^���i���{�^���j���z�Q�[���p�b�h�̃{�^��		�L�[�̏������
				{MYKEY::eUP,					MYKEY::eUP,						MYKEYSTAT::eFREE},		//��L�[
				{MYKEY::eDOWN,					MYKEY::eDOWN,					MYKEYSTAT::eFREE},		//���L�[
				{MYKEY::eLEFT,					MYKEY::eLEFT,					MYKEYSTAT::eFREE},		//���L�[
				{MYKEY::eRIGHT,					MYKEY::eRIGHT,					MYKEYSTAT::eFREE},		//�E�L�[
				{MYKEY::eSOFT,					MYKEY::eSOFT,					MYKEYSTAT::eFREE},		//�E�{�^��
				{MYKEY::eMIDDLE,				MYKEY::eMIDDLE,					MYKEYSTAT::eFREE},		//���{�^��
				{MYKEY::eHARD,					MYKEY::eHARD,					MYKEYSTAT::eFREE},		//��{�^��
				{MYKEY::eDUST,					MYKEY::eDUST,					MYKEYSTAT::eFREE},		//���{�^��
				{MYKEY::eL1,					MYKEY::eL1,						MYKEYSTAT::eFREE},		//�k�P
				{MYKEY::eL2,					MYKEY::eL2,						MYKEYSTAT::eFREE},		//�k�Q
				{MYKEY::eR1,					MYKEY::eR1,						MYKEYSTAT::eFREE},		//�q�P
				{MYKEY::eR2,					MYKEY::eR2,						MYKEYSTAT::eFREE},		//�q�Q
				{MYKEY::eSELECT,				MYKEY::eSELECT,					MYKEYSTAT::eFREE},		//�X�^�[�g
				{MYKEY::eSTART,					MYKEY::eSTART,					MYKEYSTAT::eFREE}		//�Z���N�g
			};
		//	���̓f�o�C�X���g�p����L�[��ݒ�
			rtvInputDevice = this->inputGamePad->init(
								inWnd,										//!< �E�B���h�E�n���h��
								DEVICETYPE::eGAMEPAD,						//�f�o�C�X�^�C�v�F�L�[�{�[�h
								sizeof(gemePadKeys) / sizeof(sKeyParam),	//���͎�ސ�
								gemePadKeys);
		//	�f�o�C�X�̐����`�F�b�N�i���s�ꍇ�J�������ցj
			if(rtvInputDevice != MYERROR_DI::eNOTHING)
			{
				CINPUT::releaseInstance(0);
			}
		}

		this->effectCenterInstance	= C3D_EFFECT_CENTER::getInstance();	//!< �G�t�F�N�g�Z���^�[
		this->soundInstance			= new CSOUND(inWnd);				//!< �T�E���h

	//	�O���[�o���ɔz�u����t�H���g���\�[�X�̐���
		AddFontResource(NAME_FONT1);
		AddFontResource(NAME_FONT2);
	}

	//================================================================================
	//!	���\�b�h��	CGAME_APPLICATION::finishalize
	//
	//	�@�\		�������
	//	�X�V		2008/04/17	<�V�K>
	//================================================================================
	void
	CGAME_APPLICATION::finishalize(void)
	{
		SAFE_DELETE(this->scene);							//!< �V�[���̉��
		this->inputKeyBoard			= NULL;					//!< �L�[�{�[�h���̓A�N�Z�X�Ւf
		this->inputGamePad			= NULL;					//!< �Q�[���p�b�h���̓A�N�Z�X�Ւf
		this->effectCenterInstance	= NULL;					//!< �G�t�F�N�g�Z���^�[�A�N�Z�X�Ւf

		EM::CADVENT_ENEMY_MANAGER::releaseInstance();		//!< �G�l�~�[�}�l�[�W���[�̉��
		SAVER::CSAVE_RESOURCE::releaseInstance();			//!< �Z�[�u���\�[�X�̉��
		CINPUT::releaseInstanceAll();						//!< �L�[�{�[�h���͂̉��
		C3D_EFFECT_CENTER::releaseInstance();				//!< �G�t�F�N�g�Z���^�[�̉��
		CFONT_BASE::releaseInstance();						//!< �t�H���g�Ǘ�
		SAFE_DELETE(this->soundInstance);					//!< �T�E���h

	//	�O���[�o���ɔz�u����t�H���g���\�[�X�̉��
		RemoveFontResource(NAME_FONT1);
		RemoveFontResource(NAME_FONT2);
	}

	//================================================================================
	//!	���\�b�h��	CGAME_APPLICATION::main
	//
	//	�@�\		���C������
	//	�߂�l		�A�v�����C������̃��b�Z�[�W
	//	�X�V		2008/04/17	<�V�K>
	//				2008/04/24	<�ǉ�>	�t���O�������
	//================================================================================
	HRESULT
	CGAME_APPLICATION::main(void)
	{
	//	��O���󂯎��
		try
		{
		//	�V�[�������݂��鎞
			if(	this->scene)
			{
				CINPUT*	input_ptr	= NULL;
			//	�L�[�{�[�h���͍X�V
				if(	this->inputKeyBoard)
				{
					this->inputKeyBoard->checkKey();
					input_ptr	= inputKeyBoard;
				}
			//	�Q�[���p�b�h���͍X�V
				if(	this->inputGamePad)
				{
					this->inputGamePad->checkKey();
				//	���ɓ��͂�����Ȃ�A���͓���
					if(	input_ptr)
						input_ptr->composeKeyStatus(this->inputGamePad);
					else
						input_ptr	= inputGamePad;
				}
			//	���̓f�o�C�X��������΁A�G���[��
				if(	!input_ptr)	{ throw	"���̓f�o�C�X������܂���";	}

			#ifdef	_DEBUG
			//	F1�������ꂽ��A���S�V�[���ɖ߂�
				if(	input_ptr->checkPressOnce(MYKEY::eF1))
				{
					this->flagChack(eSCENE_LOGO);
				}
			#else
			#ifdef	PRESENTATION
				if(	input_ptr->checkPressOnce(MYKEY::e1))	this->flagChack(eSCENE_CHAPTER1_STAGE1);
				if(	input_ptr->checkPressOnce(MYKEY::e2))	this->flagChack(eSCENE_CHAPTER1_STAGE2);
				if(	input_ptr->checkPressOnce(MYKEY::e3))	this->flagChack(eSCENE_CHAPTER1_STAGE3);
				if(	input_ptr->checkPressOnce(MYKEY::e4))	this->flagChack(eSCENE_CHAPTER2_STAGE4);
				if(	input_ptr->checkPressOnce(MYKEY::e5))	this->flagChack(eSCENE_CHAPTER2_STAGE5);
				if(	input_ptr->checkPressOnce(MYKEY::e6))	this->flagChack(eSCENE_CHAPTER2_STAGE6);
				if(	input_ptr->checkPressOnce(MYKEY::e7))	this->flagChack(eSCENE_CHAPTER3_STAGE7);
				if(	input_ptr->checkPressOnce(MYKEY::e8))	this->flagChack(eSCENE_CHAPTER3_STAGE8);
				if(	input_ptr->checkPressOnce(MYKEY::e9))	this->flagChack(eSCENE_CHAPTER3_STAGE9);
				if(	input_ptr->checkPressOnce(MYKEY::e0))	this->flagChack(eSCENE_LAST_CHAPTER_STAGE10);
			#endif
			#endif

			//	���ł������I��
				if(	GetAsyncKeyState(VK_ESCAPE))
				{
					throw 0;
				}

			//	�V�[�������s���A�A���ė����t���O������
				this->flagChack(this->scene->sceneMain(input_ptr));
			}
		//	�V�[����������΁A�G���[��
			else
			{
				throw	"�V�[���G���[";
			}
		}
	//!	������̗�O���擾
		catch(char szString[MAX_PATH])
		{
			char	szErr[MAX_PATH] = "";
		//	�G���[���b�Z�[�W�̍쐬
			my_sprintf(szErr, sizeof(szErr), "�u%s�v���s���ł�", szString);
		//	���b�Z�[�W�{�b�N�X�̕\��
			MessageBox(this->hWnd, szErr, "���\�[�X�G���[", MB_OK);
		//!	�v���O�����I����
			SendMessage(this->hWnd, WM_DESTROY, 0, 0);
		}
	//!	�V�[���G���[��
		catch(APP_SCENE_FLAG flag)
		{
		//	�G���[�񍐕�
			char	err[eSCENE_MAX][MAX_PATH]	=
			{
				"�𖾕s�\",
				"���S",
				"�f��",
				"�^�C�g��",
				"���[�r�[",
				"�Q�[�����C��",
				"�Q�[���N���A",
				"�I�v�V����",
				"�G���f�B���O",
				"�Q�[���I��",
				"�𖾕s�\",
			};

		//	�G���[��
			char	szErr[MAX_PATH] = "";
		//	�G���[���b�Z�[�W�̍쐬
			my_sprintf(szErr, sizeof(szErr), "%s���̃G���[�ł��B", err[flag]);
		//	���b�Z�[�W�{�b�N�X�̕\��
			MessageBox(this->hWnd, szErr, "�Q�[���G���[", MB_OK);
		//!	�v���O�����I����
			SendMessage(this->hWnd, WM_DESTROY, 0, 0);
			
		}
	//!	��L�ȊO�̗�O���擾
		catch(...)
		{
		//	������ŁA�E�B���h�E��j��
			SendMessage(this->hWnd, WM_DESTROY, 0, 0);
		}
		return 0;
	}

	//================================================================================
	//!	���\�b�h��	CGAME_APPLICATION::flagChack
	//
	//	�@�\		�󂯎�����t���O���`�F�b�N
	//	����		inGFlag		�V�����t���O
	//	�X�V		2008/04/21	<�V�K>
	//================================================================================
	void
	CGAME_APPLICATION::flagChack(SCENE_FLAG inGFlag)
	{
	//	�O�̃t���O�Ɠ������Ȃ�A�`�F�b�N���Ȃ�
		if(	this->sceneCurrFlag == inGFlag)	return;
	//	�O��̃t���O���m��
		this->scenePrevFlag	= this->sceneCurrFlag;
	//	�V�����t���O���m��
		this->sceneCurrFlag	= inGFlag;
	//	�O�̃V�[�������
		SAFE_DELETE(this->scene);
	//	�t���O���򏈗�
		switch(this->sceneCurrFlag)
		{
		//-------------------------------------
		//	�I�[�v�j���O�t���[
		//-------------------------------------
		//	�Q�[�����S
			case	eSCENE_LOGO:
				this->scene = new CSCENE_LOGO(&this->sShareData);
				break;
		//	�f��
			case	eSCENE_DEMO:
				this->scene = new CSCENE_DEMO(&this->sShareData);
				break;
		//	�^�C�g��
			case	eSCENE_TITLE:
				this->sShareData.dwPlayerLife	= 3;
				this->scene = new CSCENE_TITLE(&this->sShareData);
				break;
		//	�I�v�V����
			case	eSCENE_OPTION:
				throw	0;
				break;

		//-------------------------------------
		//	�`���v�^�[1[��̏W�c]
		//-------------------------------------
		//	���[�r�[1[�f�[�g]
			case	eSCENE_CHAPTER1_MOVIE1:
				this->scene = new CSCENE_MOVIE1(&this->sShareData);
				break;
		//	�X�e�[�W1[����]
			case	eSCENE_CHAPTER1_STAGE1:
				this->scene = new CSCENE_STAGE1(&this->sShareData);
				break;
		//	���[�r�[2[�V���h�E�N���~�i���A����]
			case	eSCENE_CHAPTER1_MOVIE2:
				this->scene = new CSCENE_MOVIE2(&this->sShareData);
				break;
		//	�X�e�[�W2[�����o��]
			case	eSCENE_CHAPTER1_STAGE2:
				this->scene = new CSCENE_STAGE2(&this->sShareData);
				break;
		//	���[�r�[3[�V���h�E�N���~�i���A�W�g]
			case	eSCENE_CHAPTER1_MOVIE3:
				this->scene = new CSCENE_MOVIE3(&this->sShareData);
				break;
		//	�X�e�[�W3[��]
			case	eSCENE_CHAPTER1_STAGE3:
				this->scene = new CSCENE_STAGE3(&this->sShareData);
				break;
		//	�{�X1[�G�N�X]
			case	eSCENE_CHAPTER1_BOSS1:
				this->scene = new CSCENE_BOSS1(&this->sShareData);
				break;

		//-------------------------------------
		//	�`���v�^�[2[�V���h�E�N���~�i����ǂ�!]
		//-------------------------------------
		//	���[�r�[4[�G��ǂ��āc]
			case	eSCENE_CHAPTER2_MOVIE4:
				this->scene = new CSCENE_MOVIE4(&this->sShareData);
				break;
		//	�X�e�[�W4[�r��]
			case	eSCENE_CHAPTER2_STAGE4:
				this->scene = new CSCENE_STAGE4(&this->sShareData);
				break;
		//	���[�r�[5[�R����c�J����c]
			case	eSCENE_CHAPTER2_MOVIE5:
				this->scene = new CSCENE_MOVIE5(&this->sShareData);
				break;
		//	�X�e�[�W5[�R]
			case	eSCENE_CHAPTER2_STAGE5:
				this->scene = new CSCENE_STAGE5(&this->sShareData);
				break;
		//	���[�r�[6[�A�W�g?]
			case	eSCENE_CHAPTER2_MOVIE6:
				this->scene = new CSCENE_MOVIE6(&this->sShareData);
				break;
		//	�X�e�[�W6[���A]
			case	eSCENE_CHAPTER2_STAGE6:
				this->scene = new CSCENE_STAGE6(&this->sShareData);
				break;
		//	�{�X2[�J�Q���}]
			case	eSCENE_CHAPTER2_BOSS2:
				this->scene = new CSCENE_BOSS2(&this->sShareData);
				break;

		//-------------------------------------
		//	�`���v�^�[3[�A�W�g�}��]
		//-------------------------------------
		//	���[�r�[7[��������i]
			case	eSCENE_CHAPTER3_MOVIE7:
				this->scene = new CSCENE_MOVIE7(&this->sShareData);
				break;
		//	�X�e�[�W7[���]
			case	eSCENE_CHAPTER3_STAGE7:
				this->scene = new CSCENE_STAGE7(&this->sShareData);
				break;
		//	�{�X3[�t�F�U�[]
			case	eSCENE_CHAPTER3_BOSS3:
				this->scene = new CSCENE_BOSS3(&this->sShareData);
				break;
		//	���[�r�[8[�{���n����]
			case	eSCENE_CHAPTER3_MOVIE8:
				this->scene = new CSCENE_MOVIE8(&this->sShareData);
				break;
		//	�X�e�[�W8[�鉺��]
			case	eSCENE_CHAPTER3_STAGE8:
				this->scene = new CSCENE_STAGE8(&this->sShareData);
				break;
		//	���[�r�[9[��͒@�����A�󂵂Ă�!]
			case	eSCENE_CHAPTER3_MOVIE9:
				this->scene = new CSCENE_MOVIE9(&this->sShareData);
				break;
		//	�X�e�[�W9[�����]
			case	eSCENE_CHAPTER3_STAGE9:
				this->scene = new CSCENE_STAGE9(&this->sShareData);
				break;

		//-------------------------------------
		//	���X�g�`���v�^�[[�N���C�}�b�N�X]
		//-------------------------------------
		//	���[�r�[10[�ǂ��l�߂���!]
			case	eSCENE_LAST_CHAPTER_MOVIE10:
				this->scene = new CSCENE_MOVIE10(&this->sShareData);
				break;
		//	�X�e�[�W10[����n�����{]
			case	eSCENE_LAST_CHAPTER_STAGE10:
				this->scene = new CSCENE_STAGE10(&this->sShareData);
				break;
		//	�{�X4[�e�B�}]
			case	eSCENE_LAST_CHAPTER_BOSS4:
				this->scene = new CSCENE_BOSS4(&this->sShareData);
				break;
		//	���[�r�[11[�ŏI����!]
			case	eSCENE_LAST_CHAPTER_MOVIE11:
				this->scene = new CSCENE_MOVIE11(&this->sShareData);
				break;
		//	���X�{�X[�N���C��]
			case	eSCENE_LAST_CHAPTER_LAST_BOSS:
				this->scene = new CSCENE_BOSS5(&this->sShareData);
				break;
		//	�G���f�B���O���[�r�[[���a�̖K��]
			case	eSCENE_LAST_CHAPTER_ENDING_MOVIE:
				this->scene = new CSCENE_MOVIE12(&this->sShareData);
				break;

		//-------------------------------------
		//	�G���f�B���O�t���[
		//-------------------------------------
		//	�G���f�B���O
			case	eSCENE_ENDING:
				this->scene = new CSCENE_ENDING(&this->sShareData);
				break;
		//	�Q�[���I�[�o�[
			case	eSCENE_GAMEOVER:
				this->scene = new CSCENE_GAMEOVER(&this->sShareData);
				break;

		//	�Q�[���I��
			case	eSCENE_QUIT:
				throw	0;
				break;
		//	�G���[
			case	eSCENE_ERROR:
				throw	this->scenePrevFlag;
				break;
		//	���߂Ȃ�
			case	eSCENE_NO:
			//	�Ȃɂ������ɏI��
				return;
		//	���̑��͉������Ȃ�
			default:
				break;
		}
	//	�V�[������������Ă���A������
		if(	this->scene){ this->scene->sceneInitialize(0);	}
	}
//-----------------------------------------------------------------------
}								//!	�Q�[���A�v���P�[�V�����̖��O���
//-----------------------------------------------------------------------