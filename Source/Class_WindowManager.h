//***********************************************************************************
//!	�t�@�C����		Class_WindowMamager.h
//
//	�@�\			�E�B���h�E�̏������Ǘ�����N���X�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/04/16	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_GameApplication.h"
#include	"Class_GraphicDevice.h"

//-----------------------------------------------------------------------
//!	�E�B���h�E�}�l�[�W���̖��O���
//-----------------------------------------------------------------------
namespace WM
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�Ǘ��}�N��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define	kWINCLASS			"KageTokiGIRL"				//!< WinClass��
	#define	kGAMETITLE			"KageTokiGIRL [%.2f fps]"	//!< �Q�[���^�C�g��

	#define	kWIN_WIDTH			1024						//!< �E�B���h�E����
	#define	kWIN_HEIGHT			768							//!< �E�B���h�E�c��

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CWINDOW_MANAGER
	//
	//	�@�\			DirectX�f�o�C�X���������E�B���h�E���A�Ǘ�����N���X
	//	�X�V			2008/04/16	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CWINDOW_MANAGER
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	�C���X�^���X����x������������ׂɕK�v
			//==================================================================
		static	CWINDOW_MANAGER*	gUniqueInstance;

				GAPP	::	CGAME_APPLICATION*	app;			//!< �Q�[���A�v���P�[�V����
				GD		::	CGRAPHIC_DEVICE*	gDev;			//!< �O���t�B�b�N�f�o�C�X
				HWND							hWnd;			//!< �E�B���h�E�n���h��
				MSG								msg;			//!< ���b�Z�[�W�L���[����̃��b�Z�[�W�����i�[

			//==================================================================
			//	��Ԋm�F
			//==================================================================
				BOOL			bGenerationFailure;				//!< �������s�m�F�t���O
				BOOL			bFullScreen;					//!< �t���X�N���[�����[�h�m�F�t���O
				BOOL			bResetDevice;					//!< �f�o�C�X���Z�b�g�m�F�t���O
				BOOL			bOperation;						//!< �N����Ԋm�F�t���O
				DWORD			dwTime;							//!< �擾���Ԋi�[�i�Q�[�����[�v�p�j

			//==================================================================
			//	��ʃX�e�[�^�X
			//==================================================================
				POINT			pScreenSize;					//!< �X�N���[���T�C�Y
				POINT			pScreenMulti;					//!< �ʏ��Ԃ���݂��X�N���[���̔䗦

			//==================================================================
			//	fps�֘A
			//==================================================================
				DWORD			dwFrames;						//!< �t���[����
				DWORD			dwLast;							//!< �擾���Ԋi�[�iFPS�p�j
				char			szFps[256];						//!< �^�C�g���\���p

				CWINDOW_MANAGER(HINSTANCE inInstance, int inShowCmd);					//!< �R���X�g���N�^
				~CWINDOW_MANAGER(void);													//!< �f�X�g���N�^

	public:		//-------------------- public --------------------//

		static	CWINDOW_MANAGER*	getInstance(HINSTANCE inInstance, int inShowCmd);	//!< �C���X�^���X�擾
		static	void				releaseInstance(void);								//!< �C���X�^���X���

				void	initialize(void);						//!< ����������
				void	finishalize(void);						//!< �������

				BOOL	findOperationWindow(void);				//!< ���̃v���O���������ɍ쓮���Ă��邩���ׂ�
				BOOL	registerClass(HINSTANCE inInstance);	//!< �E�B���h�E��o�^����
				BOOL	generationWindow(HINSTANCE inInstance);	//!< �E�B���h�E�𐶐�����
				BOOL	initApplication(HINSTANCE inInstance);	//!< �A�v���P�[�V�����̐����E������
				void	initLoop(int inShowCmd);				//!< �E�B���h�E���X�V�����[�v�̏���������
				int		messageLoop(void);						//!< ���b�Z�[�W���[�v
				void	calclateFPS(void);						//!< �t���[�������v�Z����
	};
}
//-----------------------------------------------------------------------
//!	�E�B���h�E�}�l�[�W���̖��O���
//-----------------------------------------------------------------------