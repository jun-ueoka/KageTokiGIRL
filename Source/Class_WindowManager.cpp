//***********************************************************************************
//!	�t�@�C����		Class_WindowMamager.cpp
//
//	�@�\			�E�B���h�E�𐶐����鎞�̏����̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/04/16	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_WindowManager.h"
//#include	"UtilityFunction.h"
//#include	"resource.h"

//-----------------------------------------------------------------------
//	�E�B���h�E�|���b�N
//-----------------------------------------------------------------------
extern	LRESULT CALLBACK WndProc(HWND inWnd, UINT inMessage, WPARAM inWParam, LPARAM inLParam);

//-----------------------------------------------------------------------
//!	�E�B���h�E�}�l�[�W���̖��O���
//-----------------------------------------------------------------------
namespace WM
{
	//================================================================================
	//	�C���X�^���X�����ꏊ
	//================================================================================
	CWINDOW_MANAGER*	CWINDOW_MANAGER::gUniqueInstance = NULL;

	//================================================================================
	//!	���\�b�h��	CWINDOW_MANAGER::�R���X�g���N�^
	//
	//	����		inInstance		�v���O�����Ɋ��蓖�Ă�ꂽ�C���X�^���X�n���h��
	//				inShowCmd		�E�B���h�E�\�����@
	//	�X�V		2008/04/16		<�V�K>
	//================================================================================
	CWINDOW_MANAGER::CWINDOW_MANAGER(HINSTANCE inInstance, int inShowCmd)
	{
	//!	�e�����o������
		initialize();

	//	�X�N���[�����[�h�̐ݒ�
		int	iCheck	= 0;
	//	�f�o�b�O���[�h�̏ꍇ�́A�\���`����I������
		iCheck	= MessageBox(
					hWnd,
					"�t���X�N���[���ɂ��܂����H",
					"�X�N���[���ݒ�",
					MB_YESNOCANCEL);
	//	�I�����ʂ𔻒�
		switch(iCheck)
		{
		//	�u�͂��v
			case	IDYES:		bFullScreen	= TRUE;				break;
		//	�u�������v
			case	IDNO:		bFullScreen	= FALSE;			break;
		//	��L�ȊO
			default:			bGenerationFailure	= TRUE;		return;
		}

	//!	�v���O�������N�����Ă��Ȃ�
		if(	!findOperationWindow())
		{
			bGenerationFailure	= TRUE;
		//!	��������
		//	�E�B���h�E�N���X�̓o�^�Ɏ��s
			if(	!registerClass(inInstance))
			{
			//!	�G���[��
				MessageBox(hWnd, "�E�B���h�E�̓o�^�Ɏ��s", "�G���[", MB_OK);
			//!	�v���O�����I����
				SendMessage(hWnd, WM_DESTROY, 0, 0);
				return;
			}
		//	�E�B���h�E�̐����Ɏ��s
			if(	!generationWindow(inInstance))
			{
			//!	�G���[��
				MessageBox(hWnd, "�E�B���h�E�̐����Ɏ��s", "�G���[", MB_OK);
			//!	�v���O�����I����
				SendMessage(hWnd, WM_DESTROY, 0, 0);
				return;
			}
		//	�A�v���P�[�V���������Ɏ��s
			if(	!initApplication(inInstance))
			{
			//!	�G���[��
				MessageBox(hWnd, "�A�v���P�[�V�����̏������Ɏ��s", "�G���[", MB_OK);
			//!	�v���O�����I����
				SendMessage(hWnd, WM_DESTROY, 0, 0);
				return;
			}
		//!	�����܂ŗ�����A����
			bGenerationFailure	= FALSE;
		//!	�E�B���h�E���X�V�����[�v�̏���
			initLoop(inShowCmd);
		}
	//!	���Ƀv���O�������N�����Ă���
		else
		{
		//!	�N�����Ă��鎖����
			bOperation = TRUE;
		}
	}

	//================================================================================
	//!	���\�b�h��	CWINDOW_MANAGER::�f�X�g���N�^
	//
	//	�X�V		2008/04/17		<�V�K>
	//================================================================================
	CWINDOW_MANAGER::~CWINDOW_MANAGER(void)
	{
	//!	�������
		finishalize();
	}

	//================================================================================
	//!	���\�b�h��	CWINDOW_MANAGER::getInstance
	//
	//	�@�\		�C���X�^���X�̐���
	//	����		inInstance		�v���O�����Ɋ��蓖�Ă�ꂽ�C���X�^���X�n���h��
	//				inShowCmd		�E�B���h�E�\�����@
	//	�߂�l		�C���X�^���X��Ԃ�
	//	�X�V		2008/04/17		<�V�K>
	//================================================================================
	CWINDOW_MANAGER*
	CWINDOW_MANAGER::getInstance(HINSTANCE inInstance, int inShowCmd)
	{
	//!	��������Ă��Ȃ���΁A��������
		if(	!gUniqueInstance)
		{
		//!	�C���X�^���X����
			gUniqueInstance = new CWINDOW_MANAGER(inInstance, inShowCmd);
		}
	//!	�C���X�^���X��Ԃ�
		return gUniqueInstance;
	}


	//================================================================================
	//!	���\�b�h��	CWINDOW_MANAGER::releaseInstance
	//
	//	�@�\		�C���X�^���X�̉��
	//	�X�V		2008/04/17		<�V�K>
	//================================================================================
	void
	CWINDOW_MANAGER::releaseInstance(void)
	{
	//!	�C���X�^���X�̉��
		SAFE_DELETE(gUniqueInstance);
	}

	//================================================================================
	//!	���\�b�h��	CWINDOW_MANAGER::initialize
	//
	//	�@�\		�����o�̏��������s��
	//	�X�V		2008/04/17		<�V�K>
	//================================================================================
	void
	CWINDOW_MANAGER::initialize(void)
	{
		hWnd				= NULL;					//!< �E�B���h�E�n���h��������
		bGenerationFailure	= FALSE;				//!< �������s�m�F�t���O������
		bFullScreen			= FALSE;				//!< �t���X�N���[�����[�h�m�F�t���O�������i�E�B���h�E���[�h�j
		bResetDevice		= FALSE;				//!< �f�o�C�X�m�F�t���O��������
		bOperation			= FALSE;				//!< �N����Ԋm�F�t���O������
		dwTime				= 0;					//!< �Q�[�����[�v�p���Ԋi�[�ϐ�������
		dwFrames			= 0;					//!< �t���[�����i�[�ϐ�������
		dwLast				= timeGetTime();		//!< ���݂̎��Ԃ��i�[
		app					= NULL;					//!< �A�v���P�[�V�����Ǘ��N���X�|�C���^������
		gDev				= NULL;					//!< �O���t�B�b�N�f�o�C�X�|�C���^������
		pScreenSize.x		= kWIN_WIDTH;			//!< �E�B���h�E�̉����̏�����
		pScreenSize.y		= kWIN_HEIGHT;			//!< �E�B���h�E�̏c���̏�����
		my_sprintf(szFps, sizeof(szFps), "\0");		//!< FPS�\�������񏉊���
	}

	//================================================================================
	//!	���\�b�h��	CWINDOW_MANAGER::finishalize
	//
	//	�@�\		�������
	//	�X�V		2008/04/17		<�V�K>
	//================================================================================
	void
	CWINDOW_MANAGER::finishalize(void)
	{
		ShowCursor(TRUE);								//!< �J�[�\����\����Ԃɂ���
		timeEndPeriod(1);								//!< �^�C�}�[�X�V���Ԃ�߂�
		GAPP::CGAME_APPLICATION	::releaseInstance();	//!< �Q�[�����������
		GD	::CGRAPHIC_DEVICE	::releaseInstance();	//!< �O���t�B�b�N���̏�����
	}

	//================================================================================
	//!	���\�b�h��	CWINDOW_MANAGER::findOperationWindow
	//
	//	�@�\		���̃v���O���������ɋN�����Ă��邩���ׂ�
	//	�߂�l		TRUE			�N�����Ă���
	//				FALSE			�N�����Ă��Ȃ�
	//	�X�V		2008/04/17		<�V�K>
	//================================================================================
	BOOL
	CWINDOW_MANAGER::findOperationWindow(void)
	{
	//!	�N���X������E�B���h�E������
		HWND hWnd(FindWindow(kWINCLASS, NULL));
	//!	���ɋN�����Ă���ꍇ
		if(	hWnd)
		{
			SetForegroundWindow(hWnd);					//!< ���ɋN�����Ă�������őO�ʂɂ���
			return TRUE;								//!< �N�����Ă��邱�Ƃ�Ԃ�
		}
		return FALSE;									//!< �N�����Ă��Ȃ����Ƃ�Ԃ�
	}

	//================================================================================
	//!	���\�b�h��	CWINDOW_MANAGER::registerClass
	//
	//	�@�\		�E�B���h�E��o�^����
	//	����		inInstance		�C���X�^���X�n���h��
	//	�߂�l		TRUE			����
	//				FALSE			���s
	//	�X�V		2008/04/17		<�V�K>
	//================================================================================
	BOOL
	CWINDOW_MANAGER::registerClass(HINSTANCE inInstance)
	{
		WNDCLASSEX wcex;		//!< ��`�p�\����

		ZeroMemory(&wcex, sizeof(WNDCLASSEX));						//!< �\���̏�����
		wcex.cbSize			= sizeof(WNDCLASSEX);					//!< WNDCLASSEX�\���̂̃T�C�Y
		wcex.hInstance		= inInstance;							//!< �C���X�^���X
		wcex.lpfnWndProc	= (WNDPROC)WndProc;						//!< �E�B���h�E�v���V�[�W��
		wcex.style			= CS_HREDRAW | CS_VREDRAW;				//!< �N���X�X�^�C��
		wcex.lpszMenuName	= NULL;									//!< ���j���[
		wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);	//!< �w�i�F
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);			//!< �J�[�\��
	//!< �A�C�R��
		wcex.hIcon			= LoadIcon(inInstance, MAKEINTRESOURCE(ID_KAGETOKI_ICON));
		wcex.hIconSm		= NULL;									//!< �������A�C�R��
		wcex.lpszClassName	= kWINCLASS;							//!< �E�B���h�E�N���X
		wcex.cbClsExtra		= 0;									//!< �⑫�������̃u���b�N
		wcex.cbWndExtra		= 0;									//!< �⑫�������̃T�C�Y

	//!	Windows�ɓo�^
		if(	!RegisterClassEx(&wcex))
		{
			return FALSE;		//!< ���s�������Ƃ�Ԃ�
		}
		return TRUE;			//!< �����������Ƃ�Ԃ�
	}

	//================================================================================
	//!	���\�b�h��	CWINDOW_MANAGER::generationWindow
	//
	//	�@�\		�E�B���h�E�𐶐�����
	//	����		inInstance		�C���X�^���X�n���h��
	//	�߂�l		TRUE			����
	//				FALSE			���s
	//	�X�V		2008/04/17		<�V�K>
	//================================================================================
	BOOL
	CWINDOW_MANAGER::generationWindow(HINSTANCE inInstance)
	{
	//!	�t���X�N���[�����[�h
		if(	bFullScreen)
		{
			hWnd	=
				CreateWindowEx(
					WS_EX_APPWINDOW,				//!< �g���E�B���h�E�X�^�C��
					kWINCLASS,						//!< �E�B���h�E�N���X��
					kGAMETITLE,						//!< �E�B���h�E��
					WS_POPUP,						//!< �E�B���h�E�X�^�C��
					0,								//!< �E�B���h�E�ʒuX
					0,								//!< �E�B���h�E�ʒuY
					pScreenSize.x,					//!< �E�B���h�E��
					pScreenSize.y,					//!< �E�B���h�E����
					NULL,							//!< �e�E�B���h�E�̃n���h��
					NULL,							//!< ���j���[�̃n���h��
					inInstance,						//!< �A�v���P�[�V�����̃C���X�^���X
					NULL);							//!< �E�B���h�E�쐬�f�[�^
		}
	//!	�E�B���h�E���[�h
		else
		{
			hWnd	=
				CreateWindowEx(
					WS_EX_APPWINDOW,				//!< �g���E�B���h�E�X�^�C��
					kWINCLASS,						//!< �E�B���h�E�N���X��
					kGAMETITLE,						//!< �E�B���h�E��
					WS_CAPTION | WS_SYSMENU,		//!< �E�B���h�E�X�^�C��
					(GetSystemMetrics(SM_CXSCREEN) / 2) - ((pScreenSize.x	+ 4)  / 2),	//!< �E�B���h�E�ʒuX
					(GetSystemMetrics(SM_CYSCREEN) / 2) - ((pScreenSize.y	+ 29) / 2),	//!< �E�B���h�E�ʒuY
					pScreenSize.x + 4,				//!< �E�B���h�E��
					pScreenSize.y + 29,				//!< �E�B���h�E����
					NULL,							//!< �e�E�B���h�E�̃n���h��
					NULL,							//!< ���j���[�̃n���h��
					inInstance,						//!< �A�v���P�[�V�����̃C���X�^���X
					NULL);							//!< �E�B���h�E�쐬�f�[�^
		}

	//!	�E�B���h�E�������s�m�F
		if(	!hWnd)
		{
			return FALSE;							//!< �������s��Ԃ�
		}

		ImmAssociateContext(hWnd, NULL);		//!< ���{����͂𖳌��ɂ���
		return TRUE;								//!< ����������Ԃ�
	}

	//================================================================================
	//!	���\�b�h��	CWINDOW_MANAGER::initApplication
	//
	//	�@�\		�A�v���P�[�V�����̐����Ə��������s��
	//	����		inInstance		�C���X�^���X�n���h��
	//	�߂�l		TRUE			����
	//				FALSE			���s
	//	�X�V		2008/04/17		<�V�K>
	//				2008/04/24		<�ǉ�>	�i�����̐���
	//================================================================================
	BOOL
	CWINDOW_MANAGER::initApplication(HINSTANCE inInstance)
	{
	//!	�Q�[���C���X�^���X���擾����
		app	= GAPP::CGAME_APPLICATION::getInstance(hWnd);
	//!	�������s
		if(	!app)
		{
			return	FALSE;	//!< �������s��Ԃ�
		}

	//!	�O���t�B�b�N�f�o�C�X�𐶐�
		gDev	= GD::CGRAPHIC_DEVICE::getInstance();
	//!	�f�o�C�X�̐���
		if(	!gDev->initDevice(hWnd, kWIN_WIDTH, kWIN_HEIGHT, bFullScreen))
		{
			return	FALSE;	//!	�������s
		}

	//!	�����V�[���̐���
		APP_SCENE_FLAG	inFlag	= eSCENE_LOGO;
#ifdef	_DEBUG
		inFlag	= DEBUG_FLAG;
#endif
#ifdef	PRESENTATION
		inFlag	= PRESENTATION_FLAG;
#endif

		app->flagChack(inFlag);
		return TRUE;		//!< ����������Ԃ�
	}

	//================================================================================
	//!	���\�b�h��	CWINDOW_MANAGER::initLoop
	//
	//	�@�\		�E�B���h�E���X�V���A���[�v�̏���������
	//	����		inShowCmd		�E�B���h�E�\�����@
	//	�X�V		2008/04/17		<�V�K>
	//================================================================================
	void
	CWINDOW_MANAGER::initLoop(int inShowCmd)
	{
		CoInitialize(NULL);
		ShowWindow(hWnd, inShowCmd);		//!< �\����Ԑݒ�
		UpdateWindow(hWnd);				//!< �E�B���h�E���X�V����
		timeBeginPeriod(1);						//!< �^�C�}�[�X�V���Ԃ�1000����1�b�ɐݒ�
		ShowCursor(FALSE);						//!< �J�[�\�����\���ɂ���
		dwTime = timeGetTime();			//!< ���Ԃ̎擾
	}

	//================================================================================
	//!	���\�b�h��	CWINDOW_MANAGER::messageLoop
	//
	//	�@�\		���b�Z�[�W���[�v
	//	�X�V		2008/04/17		<�V�K>
	//================================================================================
	int
	CWINDOW_MANAGER::messageLoop(void)
	{
	//!	���Ƀv���O�������N�����Ă���ꍇ�A���[�v�ɓ��炸�I��
		if(bOperation)			return 0;
	//!	�o�^�E�����Ɏ��s���Ă���ꍇ�A���[�v�ɓ��炸�I��
		if(bGenerationFailure)	return 0;

	//!	���b�Z�[�W���[�v
		while(TRUE)
		{
		//!	���b�Z�[�W�����Ă��邩
			if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
			//!	���b�Z�[�W�̗񋓂��I�����
				if(!GetMessage(&msg, NULL, 0, 0))
				{
					break;
				}
				TranslateMessage(&msg);		//!< ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�����
				DispatchMessage(&msg);		//!< ���b�Z�[�W���E�B���h�E�v���V�[�W���ɓ]������
			}
			else
			{
			//!	���b�Z�[�W�������A�E�B���h�E���A�N�e�B�u��Ԃ̎�
				if(GetActiveWindow() == hWnd)
				{
				//!	�f�o�C�X�����Z�b�g����K�v�����鎞
					if(	bResetDevice)
					{
					//!	�f�o�C�X�����Z�b�g
						if(	gDev->resetDevice())
						{
						//	�f�o�C�X���Z�b�g������������A�t���O��߂�
							bResetDevice	= FALSE;
						}
					}
				//!	�t���[������
					else if(timeGetTime() > dwTime + 15)
					{
					//! �������Ԃ��X�V����
						dwTime = timeGetTime();				
					//!	�E�B���h�E���[�h�̏ꍇ
						if(!bFullScreen)
						{
						//!	FPS���v�Z���\������
							calclateFPS();
						}
					//!	�Q�[�����������āA���b�Z�[�W�𔻒�
						app->main();
					}
				//!	��L�ɖ������Ȃ��ꍇ�AOS�ɏ�����Ԃ�
					else
					{
						Sleep(1);
					}
				}
			//!	�E�B���h�E����A�N�e�B�u�̎�
				else
				{
				//	�t���X�N���[���̎�
					if(	!bResetDevice	&&
						 bFullScreen)
					{
					//	�f�o�C�X�𕜋A����K�v������
						bResetDevice	= TRUE;
						CDEPTH_SHA::lostInstance();
						CZTEX_SHA::lostInstance();
					}
				//!	OS�ɐ����n��
					Sleep(1);
				}
			}
		}
		return 0;
	}

	//================================================================================
	//!	���\�b�h��	CWINDOW_MANAGER::calclateFPS
	//
	//	�@�\		1�b�Ԃ�����̃t���[���������߂�
	//	�X�V		2008/04/17		<�V�K>
	//================================================================================
	void
	CWINDOW_MANAGER::calclateFPS(void)
	{
		DWORD	dwCurrent(timeGetTime());		//!< ���݂̎��Ԃ��i�[
		dwFrames++;								//!< �t���[�����X�V

	//!	FPS�X�V
		if(500 <= dwCurrent - dwLast)
		{
			float	dt((float)(dwCurrent - dwLast) / 1000.0f);				//!< ���Ԋu���v�Z
			float	fps((float)dwFrames / dt);								//!< fps���v�Z
			dwLast		= dwCurrent;										//!< ���݂̎��Ԃ��i�[
			dwFrames	= 0;												//!< �t���[���J�E���^������
			sprintf_s(szFps, sizeof(szFps), kGAMETITLE, fps);				//!< �^�C�g�����̍X�V
			SetWindowText(hWnd, szFps);										//!< �^�C�g���̍X�V
		}
	}
}
//-----------------------------------------------------------------------
//!	�E�B���h�E�}�l�[�W���̖��O���
//-----------------------------------------------------------------------