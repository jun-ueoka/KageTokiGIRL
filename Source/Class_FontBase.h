//***********************************************************************************
//!	�t�@�C����		Class_FontBase.h
//
//	�@�\			�t�H���g�x�[�X�Ɋւ����`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/09/09	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"DirectX.h"				//!< DirectX�g�p
#include	"UtilityFunction.h"		//!< ���[�e�B���e�B

//-----------------------------------------------------------------------
//!	�t�H���g�x�[�X�̖��O���
//-----------------------------------------------------------------------
namespace FB
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�}�N����`
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		FONT_COLOR			D3DCOLOR_RGBA(  0,  0,  0,255)

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�t�H���g�̗�
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	FONT_KIND
	{
		eFONT_PTN1,					//!< �|�b�v��
		eFONT_PTN2,					//!< ���[�r�[�̎���
		eFONT_COMMENT,				//!< �R�����g�p
		eFONT_STAFFROLL,			//!< �X�^�b�t���[���p
		eFONT_DEBUG,				//!< �f�o�b�O�p
		eFONT_MAX					//!< �t�H���g�ő吔
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�t�H���g���
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	FONT_DATA
	{
		LPD3DXFONT		font;			//!< �t�H���g
		D3DXFONT_DESC	fontData;		//!< �ǂݍ��񂾃t�H���g�̃f�[�^

	//	���ԉ����̏�����
		FONT_DATA(void)
		{
			font		= NULL;
			ZeroMemory(&fontData, sizeof(fontData));
		}

	//	���
		~FONT_DATA(void)
		{
		//	�e�N�X�`�����
			SAFE_RELEASE(font);
		}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�t�H���g��������
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef struct	FONT_STRING
	{
		FONT_KIND		font;			//!< �t�H���g�ԍ�
		char			szWord[256];	//!< �`�悷�镶����
		D3DCOLOR		color;			//!< �F
		RECT			rcPos;			//!< �ʒu
		float			fPriority;		//!< �v���C�I���e�B
		int				iTextCnt;		//!< �e�L�X�g�J�E���^(�\������o�C�g��)
		DWORD			dFormat;		//!< �`����@

	//	���ԉ����̏�����(�l�w��)
		FONT_STRING(FONT_KIND	inFont	= eFONT_PTN1,
					LPSTR		inWord	= "",
					int			left	= 0,
					int			top		= 0,
					D3DCOLOR	inColor	= D3DCOLOR_RGBA(0,0,0,255),
					float		inPriority= 0.0f,
					int			inTextCnt = -1,
					DWORD		inFormat  = DT_NOCLIP)
		{
			font		= inFont;
			ZeroMemory(szWord, sizeof(szWord));
			strcpy_s(szWord, sizeof(szWord), inWord);
			SetRect(&rcPos, left, top, 1500, 1500);
			color		= inColor;
			fPriority	= inPriority;
			iTextCnt	= inTextCnt;
			dFormat		= inFormat;
		}
	}FONT_STR;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	���X�ɕ\������t�H���g��������
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef struct	FONT_GRADUALLY_STRING : public FONT_STRING
	{
		int	iTextGraduallySpeed;			//!< ���X�\�����鑬�x
		int	iTextGraduallyCnt;				//!< ���X�\������t���[���J�E���^

	//	���ԉ����̏�����(�l�w��)
		FONT_GRADUALLY_STRING(
			FONT_KIND	inFont	= eFONT_PTN1,
			LPSTR		inWord	= "",
			int			left	= 0,
			int			top		= 0,
			D3DCOLOR	inColor	= D3DCOLOR_RGBA(0,0,0,255),
			float		inPriority= 0.0f,
			DWORD		inFormat  = DT_NOCLIP,
			int			inTextCnt = -1,
			int			inTextGraduallySpeed = 10)	:
		FONT_STRING(inFont, inWord, left, top, inColor, inPriority, inTextCnt, inFormat)
		{
			iTextGraduallySpeed	= inTextGraduallySpeed;
			iTextGraduallyCnt	= 0;
		}

	//	���X�ɕ\�����鏈��
		void step(void)
		{
		//	���݌��Ă��镶����NULL�łȂ���΁A�\���J�E���^��i�߂�
			if(	szWord[iTextCnt] != '\0')
			{
				iTextGraduallyCnt++;
				iTextCnt = (iTextGraduallyCnt / iTextGraduallySpeed);
			}
		}

	}FONT_GRASTR;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CFONT_BASE
	//
	//	�@�\			�t�H���g�x�[�X
	//	�X�V			2008/09/09	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CFONT_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	�I�t�Z�b�g�}�N��
			//==================================================================
		#define	DEFAULT_SIZE		16					//!< �f�t�H���g�̃T�C�Y
		#define	DEFAULT_WEIGHT		1					//!< �f�t�H���g�̕�
		#define	DEFAULT_RCSIZE		1000				//!< ��`�T�C�Y

			//==================================================================
			//	���L�C���X�^���X
			//==================================================================
		static	CFONT_BASE*				gUniqueInstance;

			//==================================================================
			//	��{����
			//==================================================================
				FONT_DATA*				fonts[eFONT_MAX];	//!< �t�H���g�z��
				vector<FONT_STRING*>	strings;			//!< �o�^������

	public:			//-------------------- public --------------------//

				CFONT_BASE(void);							//!< �R���X�g���N�^
		virtual	~CFONT_BASE(void);							//!< �f�X�g���N�^

		static	CFONT_BASE*	getInstance(void);				//!< ���L�C���X�^���X����
		static	void		releaseInstance(void);			//!< ���L�C���X�^���X���
		static	void		resetInstance(CONST DEV inDev);	//!< ���L�C���X�^���X���Z�b�g

			//==================================================================
			//	���Z�b�g���s
			//==================================================================
				void	resetExecute(CONST DEV inDev);

			//==================================================================
			//	�t�H���g�̐���
			//==================================================================
				BOOL	createFont(	CONST DEV	inDev,
									FONT_KIND	inKind,
									LPSTR		inFontStyle,
									int			inHeight	= DEFAULT_SIZE,
									int			inWidth		= DEFAULT_SIZE,
									int			inWeight	= DEFAULT_WEIGHT,
									BOOL		inItalic	= FALSE);

			//==================================================================
			//	������̓o�^
			//==================================================================
				void	registerText(	LPSTR		inWord,
										FONT_KIND	inKind,
										int			inPosX,
										int			inPosY,
										float		inPosZ,
										D3DCOLOR	inColor,
										int			inSizeX	= DEFAULT_RCSIZE,
										int			inSizeY	= DEFAULT_RCSIZE,
										int			inTextCnt = -1,
										DWORD		inFormat  = DT_NOCLIP);

			//==================================================================
			//	�t�H���g�̓o�^�m�F
			//==================================================================
				BOOL	getFontValid(FONT_KIND inKind);

			//==================================================================
			//	�`��i�o�^������̈�ĕ`��j
			//==================================================================
				void	drawText(CONST SPRITE inSprite);
				void	drawText(CONST SPRITE inSprite, FONT_STR* inString);
				void	drawText(CONST SPRITE inSprite, FONT_GRASTR* inGraString);
				void	drawText(CONST SPRITE inSprite, FONT_KIND inKind, LPSTR inString,
								 int inLeft, int inTop, DWORD inColor = 0xFF000000,
								 DWORD inFormat = DT_NOCLIP);

			//==================================================================
			//	�o�^������̈�ĉ��
			//==================================================================
				void	releaseText(void);
	};
}
//-----------------------------------------------------------------------
//!	�e�N�X�`���x�[�X�̖��O���
//-----------------------------------------------------------------------