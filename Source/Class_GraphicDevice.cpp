//***********************************************************************************
//!	�t�@�C����		Class_GraphicDevice.cpp
//
//	�@�\			�O���t�B�b�N�E�`��֌W�̋@�\�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/04/17	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_GraphicDevice.h"
//#include	"Class_ZTextureShader.h"
//#include	"Class_DepthBufShader.h"

//-----------------------------------------------------------------------
//!	�O���t�B�b�N�f�o�C�X�̖��O���
//-----------------------------------------------------------------------
namespace GD
{
	//================================================================================
	//	�C���X�^���X�����ꏊ
	//================================================================================
	CGRAPHIC_DEVICE* CGRAPHIC_DEVICE::gUniqueInstance = NULL;		

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::�R���X�g���N�^
	//
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	CGRAPHIC_DEVICE::CGRAPHIC_DEVICE(void)
	:	d3D9(NULL),				//!< Direct3D
		dev(NULL),				//!< Direct3D�f�o�C�X
		d3dsp(NULL),			//!< �X�v���C�g�f�[�^
		surface(NULL),			//!< �T�[�t�F�X
		curtainTex(NULL)		//!< ���̃e�N�X�`��
	{
		initialize();
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::�f�X�g���N�^
	//
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	CGRAPHIC_DEVICE::~CGRAPHIC_DEVICE(void)
	{
		finishalize();
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::getInstance
	//
	//	�@�\		�C���X�^���X�̐���
	//	�߂�l		�C���X�^���X��Ԃ�
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	CGRAPHIC_DEVICE*
	CGRAPHIC_DEVICE::getInstance(void)
	{
	//!	�������Ă��Ȃ������ꍇ
		if(!gUniqueInstance)
		{
		//!	�C���X�^���X����
			gUniqueInstance = new CGRAPHIC_DEVICE();
		}
	//!	�C���X�^���X��Ԃ�
		return gUniqueInstance;
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::releaseInstance
	//
	//	�@�\		�C���X�^���X�̉��
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::releaseInstance(void)
	{
		SAFE_DELETE(gUniqueInstance);	//!< �C���X�^���X�̉��
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::initialize
	//
	//	�@�\		����������
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::initialize(void)
	{
	//	��ɉ��
		finishalize();

	//	�e�p�����[�^�̏�����
		ZeroMemory(&d3dpp,	sizeof(D3DPP));			//!< �v���[���e�[�V�����p�����[�^
		ZeroMemory(&caps,	sizeof(D3DCAPS9));		//!< Caps���

		hWnd			= NULL;						//!< �^�[�Q�b�g�E�B���h�E�n���h��
		iScreenSizeX	= 0;						//!< �X�N���[���T�C�Y����
		iScreenSizeY	= 0;						//!< �X�N���[���T�C�Y�c��
		iRefRate		= 60;						//!< ���t���b�V�����[�g
		bFullScreen		= FALSE;					//!< �t���X�N���[���m�F�t���O
		dwWindowStyle	= 0;						//!< �E�B���h�E�X�^�C�����
		bCurtainAction	= FALSE;					//!< ���̓���m�F
		bCurtainAppear	= FALSE;					//!< ���̓����m�F
		curtainColor	= MY_COLOR_RGBA();			//!< ���̃J���[
		curtainTex		= NULL;						//!< ���̃e�N�X�`��
		curtainZLoc		= 0.0f;						//!< ���̃v���C�I���e�B
		surface			= NULL;						//!< �T�[�t�F�X
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::finishalize
	//
	//	�@�\		�������
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::finishalize(void)
	{
	//	�e�p�����[�^���
		SAFE_RELEASE(surface);			//!< �T�[�t�F�X���
		SAFE_RELEASE(d3dsp);			//!< �X�v���C�g�f�o�C�X���
		SAFE_RELEASE(dev);				//!< Direct3D�f�o�C�X���
		SAFE_RELEASE(d3D9);				//!< DirectX���
		SAFE_RELEASE(curtainTex);		//!< ���̃e�N�X�`��
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::initDevice
	//
	//	�@�\		�f�o�C�X�̏���
	//	����		inWnd			�E�B���h�E�n���h��
	//				inScreenSizeX	�X�N���[���T�C�Y����
	//				inScreenSizeY	�X�N���[���T�C�Y�c��
	//				inFullScreen	�t���X�N���[���m�F�t���O
	//	�߂�l		TRUE			����
	//				FALSE			���s
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	BOOL
	CGRAPHIC_DEVICE::initDevice(HWND inWnd, int inScreenSizeX, int inScreenSizeY, BOOL inFullScreen)
	{
	//	�e����̊m��
		hWnd			= inWnd;
		iScreenSizeX	= inScreenSizeX;
		iScreenSizeY	= inScreenSizeY;
		bFullScreen		= inFullScreen;

	//	Direct3D�I�u�W�F�N�g�̐���
		d3D9 = Direct3DCreate9(D3D_SDK_VERSION);
		if(	d3D9 == NULL)
		{
		//	���s������G���[��
			errMessage(eGDERR_CREATE3D);
			return FALSE;		//!< ���s��Ԃ�
		}
	//	�f�o�C�X�̔\�͂��m�F
		d3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	//	�⑫�E�B���h�E�������̎w�肳�ꂽ�I�t�Z�b�g�ʒu��32�r�b�g�l��ݒ�
		dwWindowStyle	= GetWindowLong(hWnd, GWL_STYLE);
	//	�v���[���e�[�V�����p�����[�^�̐ݒ�(Present�֐��ɉe��)
		setParam();

	//	�f�o�C�X�̐���
		if(	!createDevice())
		{
		//	���s������G���[��
			errMessage(eGDERR_CREATE3DDEVICE);
			return FALSE;		//!< ���s��Ԃ�
		}
	//	�X�e�[�^�X�̐ݒ�
		setState();
	//	�e��Ǘ��N���X�𐶐�����
		createManageClass();

	//	�ݒ萳��I��
		return TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::setParam
	//
	//	�@�\		�v���[���e�[�V�����p�����[�^��ݒ�
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::setParam(void)
	{
	//	�f�B�X�v���C�̏�Ԃ��m��
		D3DDISPLAYMODE	dispMode;
		if(	FAILED(d3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dispMode)))
		{
		//	���s������G���[��
			errMessage(eGDERR_NOGETDISPLAYMODE);
		}

		ZeroMemory(&d3dpp,	sizeof(D3DPP));
	//	�E�B���h�E�X�^�C���ƃT�C�Y�̐ݒ�
		if(	bFullScreen)
		{
		//	���t���b�V�����[�g���t���X�N���[���p�ɐݒ�
			d3dpp.Windowed						= FALSE;				//!< �E�B���h�E���[�h�ł͂Ȃ�
			d3dpp.BackBufferFormat				= dispMode.Format;		//!< �o�b�N�o�b�t�@�ݒ�
		//	d3dpp.FullScreen_RefreshRateInHz	= 0;
		//	d3dpp.PresentationInterval			= 1;
		}
		else
		{
		//	��Ɨp �V���ȃE�B���h�E�̈ʒu
			RECT rcWindowSize =
			{
				(GetSystemMetrics(SM_CXSCREEN) / 2) - ((iScreenSizeX	+ 4)  / 2),	//!< �E�B���h�E�ʒuX
				(GetSystemMetrics(SM_CYSCREEN) / 2) - ((iScreenSizeY	+ 29) / 2),	//!< �E�B���h�E�ʒuY
				iScreenSizeX ,														//!< �E�B���h�E��
				iScreenSizeY,														//!< �E�B���h�E����
			};
		//	�E�B���h�E�̈ʒu,Z�I�[�_�[��ݒ�
			SetWindowPos(hWnd, HWND_NOTOPMOST, 
				rcWindowSize.left,
				rcWindowSize.top,
				rcWindowSize.right,
				rcWindowSize.bottom,
				SWP_SHOWWINDOW);
		//	���t���b�V�����[�g���E�B���h�E���[�h�p�ɐݒ�
			d3dpp.Windowed				= TRUE;							//!< �E�B���h�E���[�h
			d3dpp.BackBufferFormat		= dispMode.Format;				//!< �o�b�N�o�b�t�@�͉�ʈˑ�
		}

		d3dpp.BackBufferWidth			= iScreenSizeX;					//!< �o�b�N�o�b�t�@�̉���
		d3dpp.BackBufferHeight			= iScreenSizeY;					//!< �o�b�N�o�b�t�@�̏c��
		d3dpp.BackBufferCount			= 1;							//!< �o�b�N�o�b�t�@�̐�
		d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;		//!< �X���b�v�`�F�[���̊Ǘ��t���O
		d3dpp.EnableAutoDepthStencil	= TRUE;							//!< �[�x�o�b�t�@���Ǘ�����
		d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;					//!< 16�r�b�g�t�H�[�}�b�g
		d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;			//!< �}���`�T���v���^�C�v

		d3dpp.Flags	= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::createDevice
	//
	//	�@�\		�f�o�C�X�̐���
	//	�߂�l		TRUE			����
	//				FALSE			���s
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	BOOL
	CGRAPHIC_DEVICE::createDevice(void)
	{
	//!	HAL�Ő����i�n�[�h�E�F�A���\�t�g�E�F�A�ɂ�钸�_�����j
		if(	!dev)
		{
			if(FAILED(	d3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
						D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &dev)))
			{
			//!	HAL�Ő����i�n�[�h�E�F�A�ɂ�钸�_�����j
				if(FAILED(	d3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
							D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &dev)))
				{
				//!	HAL�Ő����i�\�t�g�E�F�A�ɂ�钸�_�����j
					if(FAILED(	d3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
								D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &dev)))
					{
					//!	REF�Ő���
						if(FAILED(	d3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &dev)))
						{
						//!	�������s
							return FALSE;
						}
					}
				}
			}
		}
		else
		{
		//	���Ƀf�o�C�X�����鎞�̓��Z�b�g
			if(	FAILED(dev->Reset(&d3dpp)))
			{
				return	FALSE;
			}
		}
	//	��������
		return TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::setState
	//
	//	�@�\		�X�e�[�^�X��ݒ�
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::setState(void)
	{
	//	�W���X�e�[�^�X
		dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			//!< ���|���S����\�����Ȃ�
		dev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);				//!< Z�o�b�t�@�𗘗p����
		dev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				//!< �[�x�o�b�t�@�ւ̏������݂�L���ɂ���
		dev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);		//!< �O���[�V�F�[�f�B���O���[�h
	//	�A���t�@�u�����f�B���O�̐ݒ�
		dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//	�A���t�@�e�X�g�̐ݒ�
		dev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//!< �A���t�@�e�X�g���s��
		dev->SetRenderState(D3DRS_ALPHAREF, 0x00000001);			//!< �A���t�@�l�̐ݒ�
		dev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	//!< �A���t�@�ݒ�
	//	�A���t�@�����̐ݒ�
		dev->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	//	���C�g�̐ݒ�
		dev->SetRenderState(D3DRS_LIGHTING, TRUE);
		dev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);			//!< ���_�@���̎������K����L��
		dev->SetRenderState(D3DRS_DITHERENABLE, TRUE);				//!< �f�B�U�����O���s���i���i���`��j
	//	�`��֘A�X�e�[�g
		dev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	//!< �W���M���
	//	dev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//!< �W���M���
		dev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);	//!< �W���M���
		dev->SetRenderState(D3DRS_COLORVERTEX, TRUE);				//!< ���_�J���[�̐ݒ�
		dev->SetRenderState(D3DRS_LASTPIXEL, FALSE);				//!< �Ō��1�s�N�Z����`�悵�Ȃ�

		//dev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
		//dev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		//dev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

		dev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRRORONCE);
		dev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRRORONCE);

		setLight(true, 0, VEC3( 1.0f,  1.0f,  1.0f));			//!< ���C�g�̐ݒ�(�S����)
		setLight(true, 1, VEC3(-1.0f,  1.0f, -1.0f));			//!< ���C�g�̐ݒ�
		setLight(true, 2, VEC3( 1.0f, -1.0f, -1.0f));			//!< ���C�g�̐ݒ�
		setLight(true, 3, VEC3(-1.0f, -1.0f,  1.0f));			//!< ���C�g�̐ݒ�

	//	�L���b�v�X�̃X�e�[�^�X�ݒ�
		caps.RasterCaps	= D3DPRASTERCAPS_WFOG;
	//	�X�v���C�g�f�o�C�X�Ɉ�����������s��
		SAFE_RELEASE(d3dsp);
	//	�X�v���C�g�����_�����O�I�u�W�F�N�g�̐���
		D3DXCreateSprite(dev, &d3dsp);
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::setCurtainColor
	//
	//	�@�\		���̃J���[�̐ݒ�
	//	����		inRed			�Ԑ���
	//				inGreen			�ΐ���
	//				inBlue			����
	//	�X�V		2008/09/30		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::setCurtainColor(int inRed, int inGreen, int inBlue)
	{
		curtainColor.rgba.red	= BYTE(inRed);
		curtainColor.rgba.green	= BYTE(inGreen);
		curtainColor.rgba.blue	= BYTE(inBlue);
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::calcCurtainColor
	//
	//	�@�\		���̃J���[�̉��Z
	//	����		inRed			�Ԑ���
	//				inGreen			�ΐ���
	//				inBlue			����
	//	�X�V		2008/09/30		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::calcCurtainColor(int inRed, int inGreen, int inBlue)
	{
	//	RGBA�J���[�̉����Z
		inRed	=	(int)curtainColor.rgba.red		+ inRed;
		inGreen	=	(int)curtainColor.rgba.green	+ inGreen;
		inBlue	=	(int)curtainColor.rgba.blue		+ inBlue;
	//	�͈͐ݒ�
		if(	inRed   < 0){ inRed   = 0;	}	else if(inRed   > 255){ inRed   = 255;	}
		if(	inGreen < 0){ inGreen = 0;	}	else if(inGreen > 255){ inGreen = 255;	}
		if(	inBlue  < 0){ inBlue  = 0;	}	else if(inBlue  > 255){ inBlue  = 255;	}
	//	���߂Đݒ�
		setCurtainColor(inRed,inGreen,inBlue);
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::setState
	//
	//	�@�\		���C�g�̐ݒ���s��
	//	����		bLight			���C�g���g�p���邩�ǂ����̃t���O
	//				inStage			�w��X�e�[�W(�z���̈ʒu)
	//				inLoc			�ʒu
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::setLight(bool bLight, int inStage, VEC3 inLoc)
	{
	//	���C�g�����
		if(	bLight)
		{
			D3DLIGHT9		light;										//!< ���C�g���\����
			D3DCOLORVALUE	diffuse		= {1.0f, 1.0f, 1.0f, 1.0f};		//!< �F
			D3DCOLORVALUE	specular	= {0.5f, 0.5f, 0.5f, 0.5f};		//!< ���ˊp

			ZeroMemory(&light, sizeof(D3DLIGHT9));						//!< ���C�g���̏�����

		//	�f�B�N�V���i�����C�g
			light.Type			= D3DLIGHT_DIRECTIONAL;		//!< �|�C���g���C�g
			light.Diffuse		= diffuse;					//!< �F��ݒ�
			light.Direction		= inLoc;					//!< �ʒu��ݒ�
			light.Specular		= specular;					//!< ���ˊp��ݒ�

			dev->SetLight(inStage, &light);					//!< �w�肵���X�e�[�W�Ƀ��C�g��ݒ�
			dev->LightEnable(inStage, TRUE);				//!< �w�肵���X�e�[�W�̃��C�g���I��
		}
	//	���C�g������
		else
		{
			dev->SetLight(inStage, NULL);					//!< �w�肵���X�e�[�W�̃��C�g������
			dev->LightEnable(inStage, FALSE);				//!< �w�肵���X�e�[�W�̃��C�g���I�t
		}
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::createManageClass
	//
	//	�@�\		�e��Ǘ�����N���X�𐶐�����
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::createManageClass(void)
	{
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::setFog
	//
	//	�@�\		�t�H�O�̐ݒ������
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::setFog(void)
	{
		const DWORD	dwFogColor	= D3DCOLOR_ARGB(255,255,255,255);			//!< �t�H�O�̐F
		const float fFogStart	= 0.0f;										//!< �t�H�O�J�n�ʒu
		const float fFogEnd		= 10000.0f;									//!< �t�H�O�ŏI�ʒu
	//! ��D3DRS_FOGVERTEXMODE	���@���_�t�H�O
	//! ��D3DRS_FOGTABLEMODE	���@�s�N�Z���t�H�O
		const D3DRENDERSTATETYPE fogType	= D3DRS_FOGTABLEMODE;			//!< �t�H�O�̃^�C�v

		dev->SetRenderState(D3DRS_FOGENABLE,	TRUE);						//!< �t�H�O���g�p����
		dev->SetRenderState(D3DRS_FOGCOLOR,		dwFogColor);				//!< �t�H�O�̐F��ݒ�
		dev->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_EXP);				//!< �t�H�O�̃^�C�v�ݒ�
		dev->SetRenderState(fogType,			D3DFOG_LINEAR);				//!< �t�H�O�̃^�C�v�ݒ�
		dev->SetRenderState(D3DRS_FOGSTART,		*(DWORD*)(&fFogStart));		//!< �t�H�O�̊J�n�ʒu�ݒ�
		dev->SetRenderState(D3DRS_FOGEND,		*(DWORD*)(&fFogEnd));		//!< �t�H�O�̍ŏI�ʒu�ݒ�
		dev->SetRenderState(D3DRS_AMBIENT,		D3DCOLOR_XRGB(255, 255, 255));
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::createCurtainTex
	//
	//	�@�\		���e�N�X�`���𐶐�����
	//	����		inTexName		���[�h��ʂɎg���e�N�X�`����
	//	�X�V		2008/12/11		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::createCurtainTex(LPSTR inTexName)
	{
	//	���e�N�X�`���̉��
		SAFE_RELEASE(curtainTex);
	//	�e�N�X�`�����w�肳��Ă���΁A���e�N�X�`���𐶐�
		if(	inTexName)
		{
		//	�e�N�X�`�����Ƀp�X������
			char	realName[MAX_PATH]	= "";
		//	���s������A���\�[�X�t�@�C���ւ̃p�X�����ăg���C
			sprintf_s(realName, sizeof(realName), "..//Resources//LoadingTexture//%s.dds", inTexName);

		//	�e�N�X�`����ǂݍ���ł݂�
			if(	FAILED(D3DX_CREATE_TEXTURE_EX(
					dev, realName, D3DCOLOR_RGBA(255,255,255,100),
					NULL, &curtainTex)))
			{
			//	���s������ANULL������
				curtainTex	= NULL;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::renderCurtain
	//
	//	�@�\		�h��Ԃ��`��
	//	����		inColor
	//	�X�V		2008/09/26		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::renderCurtain(TEX inTex, DWORD inColor)
	{
	//	�S�̕`��p���_
		CUSTOM_VERTEX_2D	surface[4]	=
		{
			{			   -1.0f,				-1.0f, curtainZLoc, 0.0f, inColor, 0.0f, 0.0f },
			{(float)iScreenSizeX,				-1.0f, curtainZLoc, 0.0f, inColor, 1.0f, 0.0f },
			{(float)iScreenSizeX, (float)iScreenSizeY, curtainZLoc, 0.0f, inColor, 1.0f, 0.75f},
			{			   -1.0f, (float)iScreenSizeY, curtainZLoc, 0.0f, inColor, 0.0f, 0.75f}
		};
		DWORD	dwFVF=0;	//!< FVF�̃o�b�N�A�b�v�p

	//	�|���S���`��
		dev->SetTexture(0, inTex);
		dev->GetFVF(&dwFVF);
		dev->SetFVF(D3DFVF_VERTEX_2D);
		dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, surface, sizeof(CUSTOM_VERTEX_2D));
		dev->SetFVF(dwFVF);
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::renderCurtainInDev
	//
	//	�@�\		�h��Ԃ��`��
	//	�@�\�T�v	�X�v���C�g�N�����͈Ӗ�������
	//	����		inColor
	//	�X�V		2008/09/26		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::renderCurtainInDev(void)
	{
	//!	���̓��쏈��
		this->curtainMotionExecute();

	//	���̃A���t�@��255�ȏ�Ȃ�A�e�N�X�`���Ŗ���`��
		if(	curtainColor.rgba.alpha >= 255	&&
			(this->curtainTex))
		{
			renderCurtain(curtainTex, D3DCOLOR_RGBA(255, 255, 255, 255));
		}
	//	���̃A���t�@��0����Ȃ�A����`��
		else
		if(	curtainColor.rgba.alpha > 0)
		{
			renderCurtain(NULL, curtainColor.color);
		}
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::curtainMotionExecute
	//
	//	�@�\		���̓��쏈��
	//	�X�V		2009/02/12		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::curtainMotionExecute(void)
	{
	//!	���̓���m�F
 		if(	bCurtainAction)
		{
			int	inAlpha	 = (int)curtainColor.rgba.alpha;
		//	���̊m�F���L���Ȃ�A�o��
			if(	bCurtainAppear)
			{
				inAlpha	+= iAlphaSpeed;
			//	�A���t�@���ő�ɂȂ�����A����m�F�𖳌�
				if(	inAlpha	> 255)
				{
					bCurtainAction	= FALSE;
					inAlpha			= 255;
				}
			}
		//	�����Ȃ�A������
			else
			{
				inAlpha	-= iAlphaSpeed;
			//	�A���t�@���ŏ��ɂȂ�����A����m�F�𖳌�
				if(	inAlpha < 0)
				{
					bCurtainAction	= FALSE;
					inAlpha			= 0;
				}
			}
		//	�A���t�@�̃Z�b�g
			curtainColor.rgba.alpha	= BYTE(inAlpha);
		}
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::curtainMotion
	//
	//	�@�\		���̓���
	//	�@�\�T�v	���𒣂邩�A�������ł�����
	//	����		inBoolen		TRUE(���𒣂�) : FALSE(��������)
	//				inAlpha			�A���t�@�l�ϓ��l
	//	�X�V		2008/09/26		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::curtainMotion(BOOL inBoolen, int inAlpha)
	{
	//	���̓���m�F��L��
		bCurtainAction	= TRUE;
	//	���̏o���m�F���m��
		bCurtainAppear	= inBoolen;
	//	���̓����x�ϓ��l���m��
		iAlphaSpeed		= inAlpha;
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::errMessage
	//
	//	�@�\		�G���[��
	//	����		err				�G���[�̎��
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::errMessage(GD_ERROR err)
	{
		char	szErr[MAX_PATH] = "";
	//	�G���[���𐶐�
		switch(err){
			case eGDERR_CREATE3D:			wsprintf(szErr, "Direct3D�̐����Ɏ��s");				break;
			case eGDERR_CREATE3DDEVICE:		wsprintf(szErr, "Direct3D�f�o�C�X�̐����Ɏ��s");		break;
			case eGDERR_RESETDEVICE:		wsprintf(szErr, "�f�o�C�X�̍ċN���Ɏ��s");				break;
			case eGDERR_BUFFERCLEAR:		wsprintf(szErr, "�o�b�t�@���N���A����Ȃ�");			break;
			case eGDERR_BEGINSCENE:			wsprintf(szErr, "�V�[�����J�n�ł��Ȃ�");				break;
			case eGDERR_BEGINSPRITE:		wsprintf(szErr, "�X�v���C�g���J�n�ł��Ȃ�");			break;
			case eGDERR_ENDSPRITE:			wsprintf(szErr, "�X�v���C�g���I���ł��Ȃ�");			break;
			case eGDERR_ENDSCENE:			wsprintf(szErr, "�V�[�����I���ł��Ȃ�");				break;
			case eGDERR_NOGETDISPLAYMODE:	wsprintf(szErr, "�f�B�X�v���C�̏�Ԃ��擾�ł��Ȃ�");	break;
			default:						wsprintf(szErr, "�G���[�̎�ނ����m��");				break;
		}
	//	���b�Z�[�W�{�b�N�X�̕\��
		MessageBox(hWnd, szErr, "GraphicDevice�G���[", MB_OK);
		//!	�v���O�����I����
		SendMessage(hWnd, WM_DESTROY, 0, 0);
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::renewalScreenMode
	//
	//	�@�\		�X�N���[���̍X�V
	//	����		inWnd			�E�B���h�E�n���h��
	//				inFullScreen	�t���X�N���[���m�F�t���O
	//				inScreenSizeX	�X�N���[���T�C�Y����
	//				inScreenSizeY	�X�N���[���T�C�Y�c��
	//	�߂�l		TRUE			����
	//				FALSE			���s
	//	�X�V		2008/04/19		<�V�K>
	//================================================================================
	BOOL
	CGRAPHIC_DEVICE::renewalScreenMode(HWND inWnd, BOOL inFullScreen, int inScreenSizeX, int inScreenSizeY)
	{
	//	�e����̊m��
		hWnd	= inWnd;
	//	�E�B���h�E�T�C�Y��ύX���邩���肷��
		if(	inScreenSizeX != 0 && inScreenSizeY != 0)
		{
			iScreenSizeX	= inScreenSizeX;
			iScreenSizeY	= inScreenSizeY;
		}
	//	�T�C�Y�ύX�łȂ��ꍇ�̓t���X�N���[���ύX�Ɣ��f
		else
		{
			bFullScreen	= inFullScreen;
		}
	//	�p�����[�^�̍Đݒ�
		setParam();
	//	�f�o�C�X�����Z�b�g
		if(	createDevice())
		{
		//	����������X�e�[�^�X�̏�����
			setState();
			return	TRUE;
		}
		else
		{
		//	���s������X���[�v
			Sleep(1);
			return	FALSE;
		}
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::bufferClear
	//
	//	�@�\		�o�b�t�@�̃N���A���s��
	//	����		dwColor			�o�b�t�@�N���A���ɓh��Ԃ��F
	//	�߂�l		S_OK			����
	//				E_FAIL			���s
	//	�X�V		2008/04/19		<�V�K>
	//================================================================================
	HRESULT
	CGRAPHIC_DEVICE::bufferClear(DWORD dwColor)
	{
	//!	�o�b�t�@�̃N���A
		if(FAILED(dev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, dwColor, 1.0f, 0)))
		{
		//	���s������G���[��
			errMessage(eGDERR_BUFFERCLEAR);
			return E_FAIL;		//!	���s��Ԃ�
		}
		return S_OK;			//!< ������Ԃ�
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::renderBegin
	//
	//	�@�\		�����_�����O�̏������s��
	//	����		dwColor			�o�b�t�@�N���A���ɓh��Ԃ��F
	//	�߂�l		S_OK			����
	//				E_FAIL			���s
	//	�X�V		2008/04/19		<�V�K>
	//================================================================================
	HRESULT
	CGRAPHIC_DEVICE::renderBegin(DWORD dwColor)
	{
	//!	�o�b�t�@�̃N���A
		if(FAILED(dev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, dwColor, 1.0f, 0)))
		{
		//	���s������G���[��
			errMessage(eGDERR_BUFFERCLEAR);
			return E_FAIL;		//!	���s��Ԃ�
		}
	//!	�V�[���̊J�n
		if(FAILED(dev->BeginScene()))
		{
		//	���s������G���[��
			errMessage(eGDERR_BEGINSCENE);
			return E_FAIL;		//!< ���s��Ԃ�
		}
		return S_OK;			//!< ������Ԃ�
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::renderEnd
	//
	//	�@�\		�����_�����O�̍X�V���s��
	//	�߂�l		S_OK			����
	//				E_FAIL			���s
	//	�X�V		2008/04/19		<�V�K>
	//================================================================================
	HRESULT
	CGRAPHIC_DEVICE::renderEnd(void)
	{
	//!	�V�[���̏I��
		if(FAILED(dev->EndScene()))
		{
		//	���s������G���[��
			errMessage(eGDERR_ENDSCENE);
			return E_FAIL;				//!< ���s��Ԃ�
		}
	//!	�o�b�N�o�b�t�@����]��
		if(FAILED(dev->Present(NULL, NULL, NULL, NULL)))
		{
			resetDevice();				//!< �f�o�C�X�����Z�b�g����
			return E_FAIL;				//!< ���s��Ԃ�
		}
		return S_OK;					//!< ������Ԃ�
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::spriteBegin
	//
	//	�@�\		�X�v���C�g���J�n
	//	�߂�l		S_OK			����
	//				E_FAIL			���s
	//	�X�V		2008/04/19		<�V�K>
	//================================================================================
	HRESULT
	CGRAPHIC_DEVICE::spriteBegin(void)
	{
	//!	�X�v���C�g�̕`�������
		if(FAILED(d3dsp->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK)))
		{
		//	���s������G���[��
			errMessage(eGDERR_BEGINSPRITE);
			return E_FAIL;				//!< ���s��Ԃ�
		}
		return S_OK;					//!< ������Ԃ�
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::spriteBegin
	//
	//	�@�\		�X�v���C�g���I��
	//	�߂�l		S_OK			����
	//				E_FAIL			���s
	//	�X�V		2008/04/19		<�V�K>
	//================================================================================
	HRESULT
	CGRAPHIC_DEVICE::spriteEnd(void)
	{
	//!	�X�v���C�g�`��I��
		if(FAILED(d3dsp->End()))
		{
		//	���s������G���[��
			errMessage(eGDERR_ENDSPRITE);
			return E_FAIL;				//!< ���s��Ԃ�
		}
		return S_OK;					//!< ������Ԃ�
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::setFogEnable
	//
	//	�@�\		�t�H�O�̎g�p�E�s�g�p�ݒ�
	//	����		bEnable			�t�H�O�̎g�p�E�s�g�p�̕ύX
	//	�X�V		2008/04/19		<�V�K>
	//================================================================================
	void
	CGRAPHIC_DEVICE::setFogEnable(const BOOL bEnable)
	{
		dev->SetRenderState(D3DRS_FOGENABLE,	  bEnable);	//!< �t�H�O���g�p�E�s�g�p�ύX
		dev->SetRenderState(D3DRS_RANGEFOGENABLE, bEnable);	//!< �t�H�O��L���E�����ύX
	}

	//================================================================================
	//!	���\�b�h��	CGRAPHIC_DEVICE::resetDevice
	//
	//	�@�\		�f�o�C�X�����Z�b�g����
	//	�߂�l		TRUE			����
	//				FALSE			���s
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	BOOL
	CGRAPHIC_DEVICE::resetDevice(void)
	{
		FILE*	fp	= NULL;
	//	�f�o�C�X���������Ă�����č\�z����
		switch(dev->TestCooperativeLevel())
		{
			case D3DERR_DEVICELOST:			//!< ���Z�b�g�o���Ȃ�
				Sleep(50);
				return	FALSE;
			case D3DERR_DEVICENOTRESET:		//!< ���Z�b�g�o����
			//	���̑����Z�b�g
				setParam();					//!< �p�����[�^�̍Đݒ�
				if(	createDevice())			//!< �f�o�C�X�̍Đ���(���Z�b�g)
				{
					CZTEX_SHA::resetInstance();
					CDEPTH_SHA::resetInstance();
				//	Z�e�N�X�`�������L����
					CDEPTH_SHA::getInstance()->setEffectTexture(
						CZTEX_SHA::getInstance()->getZTex());
				//	����������X�e�[�^�X�̃��Z�b�g
					setState();
					return	TRUE;
				}
				break;
		}
		return	FALSE;
	}
}
//-----------------------------------------------------------------------
//!	�O���t�B�b�N�f�o�C�X�̖��O���
//-----------------------------------------------------------------------