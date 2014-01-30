//***********************************************************************************
//!	�t�@�C����		Class_GraphicDevice.h
//
//	�@�\			�O���t�B�b�N�E�`��֌W�̊Ǘ����s���N���X�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/04/17	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"DirectX.h"				//!< DirectX�g�p

//-----------------------------------------------------------------------
//!	�O���t�B�b�N�f�o�C�X�̖��O���
//-----------------------------------------------------------------------
namespace GD
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�G���[�Ǘ�
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	GD_ERROR				//!< �G���[�����o�[
	{
		eGDERR_CREATE3D,			//!< Direct3D�̐����Ɏ��s
		eGDERR_CREATE3DDEVICE,		//!< Direct3D�f�o�C�X�̐����Ɏ��s
		eGDERR_RESETDEVICE,			//!< �f�o�C�X�̍ċN���Ɏ��s
		eGDERR_BUFFERCLEAR,			//!< �o�b�t�@���N���A����Ȃ�
		eGDERR_BEGINSCENE,			//!< �V�[�����J�n�ł��Ȃ�
		eGDERR_BEGINSPRITE,			//!< �X�v���C�g���J�n�ł��Ȃ�
		eGDERR_ENDSPRITE,			//!< �X�v���C�g���I���ł��Ȃ�
		eGDERR_ENDSCENE,			//!< �V�[�����I���ł��Ȃ�
		eGDERR_NOGETDISPLAYMODE,	//!< �f�B�X�v���C�̏�Ԃ��擾�ł��Ȃ�
		eGDERR_NON,					//!< �G���[�Ȃ�
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�w�i
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define	BACK_BUFFER		D3DCOLOR_RGBA(255,255,255,255)

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CGRAPHIC_DEVICE
	//
	//	�@�\			�O���t�B�b�N�f�o�C�X
	//	�X�V			2008/04/17	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CGRAPHIC_DEVICE
	{
			//==================================================================
			//!	�w�i
			//==================================================================
				#define	PATH_CURTAIN	"gd_curtain//gd_curtain.dds"

	private:		//-------------------- private --------------------//
		
			//==================================================================
			//	�C���X�^���X����x�������������
			//==================================================================
		static	CGRAPHIC_DEVICE*	gUniqueInstance;

			//==================================================================
			//	DirectX�֘A
			//==================================================================
				LPDIRECT3D9				d3D9;				//!< Direct3D
				DEV						dev;				//!< Direct3D�f�o�C�X
				SPRITE					d3dsp;				//!< �X�v���C�g�f�[�^
				D3DPP					d3dpp;				//!< �v���[���e�[�V�����p�����[�^
				D3DCAPS9				caps;				//!< Caps�f�o�C�X
				LPDIRECT3DSURFACE9		surface;			//!< �T�[�t�F�X

			//==================================================================
			//	�\�����
			//==================================================================
				HWND		hWnd;							//!< �^�[�Q�b�g�E�B���h�E�n���h��
				int			iScreenSizeX;					//!< �X�N���[���T�C�Y����
				int			iScreenSizeY;					//!< �X�N���[���T�C�Y�c��
				int			iRefRate;						//!< ���t���b�V�����[�g
				BOOL		bFullScreen;					//!< �t���X�N���[���m�F�t���O
				DWORD		dwWindowStyle;					//!< �E�B���h�E�X�^�C��

			//==================================================================
			//	�����
			//==================================================================
				BOOL			bCurtainAction;				//!< ���̓���m�F
				BOOL			bCurtainAppear;				//!< ���̏o���m�F
				int				iAlphaSpeed;				//!< ���̓����x�ϓ��l
				MY_COLOR_RGBA	curtainColor;				//!< ���̃J���[
				TEX				curtainTex;					//!< ���̃e�N�X�`��
				float			curtainZLoc;				//!< ����Z�ʒu

			//==================================================================
			//	�������֘A
			//==================================================================
				void		setParam(void);					//!< �p�����[�^�̐ݒ�
				BOOL		createDevice(void);				//!< �f�o�C�X�̐���
				void		setState(void);					//!< �X�e�[�^�X�̐ݒ�
				void		createManageClass(void);		//!< �e��Ǘ�����N���X�𐶐�����

			//==================================================================
			//	�G���[��
			//==================================================================
				void		errMessage(GD_ERROR err);

				CGRAPHIC_DEVICE(void);
				~CGRAPHIC_DEVICE(void);

	public:			//-------------------- public --------------------//

		static	CGRAPHIC_DEVICE*	getInstance(void);		//!< �C���X�^���X���擾
		static	void				releaseInstance(void);	//!< �C���X�^���X�����

				void	initialize(void);					//!< ����������
				void	finishalize(void);					//!< �������

			//==================================================================
			//!	�f�o�C�X�̐ݒ�
			//==================================================================
				BOOL	initDevice(HWND inWnd, int inScreenSizeX, int inScreenSizeY, BOOL inFullScreen = FALSE);

			//==================================================================
			//!	�X�N���[���̍X�V
			//==================================================================
				BOOL	renewalScreenMode(HWND inWnd, BOOL inFullScreen, int inScreenSizeX = 0, int inScreenSizeY = 0);

			//==================================================================
			//!	�����_�����O�֌W
			//==================================================================
				HRESULT	bufferClear(DWORD dwColor = BACK_BUFFER);				//!< �o�b�t�@�N���A
				HRESULT	renderBegin(DWORD dwColor = BACK_BUFFER);				//!< �`�揀��
				HRESULT	renderEnd(void);										//!< �`��X�V
				HRESULT	spriteBegin(void);										//!< �X�v���C�g�J�n
				HRESULT	spriteEnd(void);										//!< �X�v���C�g�I��

			//==================================================================
			//	access	���t���b�V�����[�g
			//==================================================================
				int		getRefRate(void)const		{ return iRefRate;		}
				void	setRefRate(int inRefRate)	{ iRefRate = inRefRate;	}

			//==================================================================
			//	access	�X�N���[���T�C�Y
			//==================================================================
				int		getScreenSizeX(void)const	{ return iScreenSizeX;	}
				int		getScreenSizeY(void)const	{ return iScreenSizeY;	}

			//==================================================================
			//	access	���J���[
			//==================================================================
				void	setCurtainColorAlpha(int inAlpha)	{ curtainColor.rgba.alpha = (BYTE)inAlpha;	}
				void	setCurtainColor(int inRed, int inGreen, int inBlue);
				void	calcCurtainColor(int inRed, int inGreen, int inBlue);
				int		getCurtainColorRed(void)			{ return (int)curtainColor.rgba.red;	}
				int		getCurtainColorGreen(void)			{ return (int)curtainColor.rgba.green;	}
				int		getCurtainColorBlue(void)			{ return (int)curtainColor.rgba.blue;	}
				int		getCurtainColorAlpha(void)			{ return (int)curtainColor.rgba.alpha;	}

				void	setCurtainAction(BOOL inBoolen)		{ bCurtainAction	= inBoolen;			}

				void	setCurtainZLoc(float inLoc)			{ curtainZLoc		= inLoc;			}
				float	getCurtainZLoc(void)const			{ return	curtainZLoc;				}

			//==================================================================
			//	get	access
			//==================================================================
				DEV			getDevice(void)const	{ return dev;			}	//!< �f�o�C�X���󂯎��
				D3DCAPS9	getCaps(void)const		{ return caps;			}	//!< Caps�f�o�C�X���󂯎��
				SPRITE		getSprite(void)const	{ return d3dsp;			}	//!< �X�v���C�g�f�o�C�X���󂯎��

				void	setLight(bool bLight, int inStage, VEC3 inLoc);			//!< ���C�g�̐ݒ�
				void	setFog(void);											//!< �t�H�O�̐ݒ�
				void	createCurtainTex(LPSTR inTexName = NULL);				//!< ���e�N�X�`���̐���
				void	renderCurtain(TEX inTex, DWORD inColor);				//!< �h��Ԃ��`��
				void	renderCurtainInDev(void);								//!< �h��Ԃ��`��(�f�o�C�X�X�e�[�^�X)
				void	curtainMotionExecute(void);								//!< ���̓��쏈��
				void	curtainMotion(BOOL inBoolen, int inAlpha);				//!< ���̓���
				BOOL	resetDevice(void);										//!< �f�o�C�X�̃��Z�b�g
				void	setFogEnable(const BOOL bEnable);						//!< �t�H�O�̎g�p�E�s�g�p�ݒ�
	};
}
//-----------------------------------------------------------------------
//!	�O���t�B�b�N�f�o�C�X�̖��O���
//-----------------------------------------------------------------------