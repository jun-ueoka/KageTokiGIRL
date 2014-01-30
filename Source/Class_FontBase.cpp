//***********************************************************************************
//!	�t�@�C����		Class_FontBase.cpp
//
//	�@�\			�t�H���g�x�[�X�Ɋւ����`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/09/09	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_FontBase.h"				//!< �t�H���g�x�[�X

//-----------------------------------------------------------------------
//!	�t�H���g�x�[�X�̖��O���
//-----------------------------------------------------------------------
namespace FB
{
	//================================================================================
	//	���L�C���X�^���X
	//================================================================================
	CFONT_BASE*		CFONT_BASE::gUniqueInstance	= NULL;

	//================================================================================
	//!	���\�b�h��	CFONT_BASE::getInstance
	//
	//	�@�\		���L�C���X�^���X����
	//	�߂�l		�������ꂽ�C���X�^���X
	//	�X�V		2008/09/09		<�V�K>
	//================================================================================
	CFONT_BASE*
	CFONT_BASE::getInstance(void)
	{
	//	��������Ă��Ȃ���΁A��������
		if(!gUniqueInstance)
		{
			gUniqueInstance	= new CFONT_BASE();
		}
	//	�C���X�^���X��Ԃ�
		return	gUniqueInstance;
	}

	//================================================================================
	//!	���\�b�h��	CFONT_BASE::releaseInstance
	//
	//	�@�\		���L�C���X�^���X���
	//	�X�V		2008/09/09		<�V�K>
	//================================================================================
	void
	CFONT_BASE::releaseInstance(void)
	{
	//	���
		SAFE_DELETE(gUniqueInstance);
	}

	//================================================================================
	//!	���\�b�h��	CFONT_BASE::resetInstance
	//
	//	�@�\		���L�C���X�^���X���Z�b�g
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2009/03/02		<�V�K>
	//================================================================================
	void
	CFONT_BASE::resetInstance(CONST DEV inDev)
	{
	//	��������Ă��Ȃ���΁A��������
		if(!gUniqueInstance)
		{
			gUniqueInstance->resetExecute(inDev);
		}
	}

	//================================================================================
	//!	���\�b�h��	CFONT_BASE::resetExecute
	//
	//	�@�\		���Z�b�g���s
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2009/03/02		<�V�K>
	//================================================================================
	void
	CFONT_BASE::resetExecute(CONST DEV inDev)
	{
	//	�t�H���g����������Ă���z�񂾂����Z�b�g
		for(int i=0; i<eFONT_MAX; i++)
		{
			if(	fonts[i])
			{
			//	��ɉ��
				SAFE_RELEASE(fonts[i]->font);
			//	����Ă������t�H���g�f�[�^�ŁA�t�H���g�𕜌�
				if(	FAILED(D3DXCreateFontIndirect(inDev, &fonts[i]->fontData, &fonts[i]->font)))
				{
				//	�����f�[�^��j��
					SAFE_DELETE(fonts[i]);
					DX::errorMsgNoFile(inDev, fonts[i]->fontData.FaceName);
				}
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CFONT_BASE::�R���X�g���N�^
	//
	//	�X�V		2008/09/09		<�V�K>
	//================================================================================
	CFONT_BASE::CFONT_BASE(void)
	{
	//	�t�H���g�z��̏�����
		for(int i=0; i<eFONT_MAX; i++)
		{
			fonts[i]	= NULL;
		}
	}

	//================================================================================
	//!	���\�b�h��	CFONT_BASE::�f�X�g���N�^
	//
	//	�X�V		2008/09/09		<�V�K>
	//================================================================================
	CFONT_BASE::~CFONT_BASE(void)
	{
	//	�t�H���g�z��̉��
		for(int i=0; i<eFONT_MAX; i++)
		{
			SAFE_DELETE(fonts[i]);
		}
	//	�o�^������̉��
		releaseText();
	}

	//================================================================================
	//!	���\�b�h��	CFONT_BASE::createFont
	//
	//	�@�\		�t�H���g�̐���
	//	����		inDev			Direct3D�f�o�C�X
	//				inKind			�t�H���g�̎��
	//				inFontStyle		�t�H���g�̖���
	//				inHeight		�t�H���g�̏c�T�C�Y
	//				inWidth			�t�H���g�̉��T�C�Y
	//				inWeight		�t�H���g�̉���
	//				inItalic		�C�^���b�N�m�F
	//	�߂�l		TRUE			��������
	//				FALSE			�������s
	//	�X�V		2008/09/09		<�V�K>
	//================================================================================
	BOOL
	CFONT_BASE::createFont(	CONST DEV	inDev,
							FONT_KIND	inKind,
							LPSTR		inFontStyle,
							int			inHeight,
							int			inWidth,
							int			inWeight,
							BOOL		inItalic)
	{
	//	�����Ă������ɕs��������Ȃ�A�����������Ɏ��s��Ԃ�
		if(	(inKind < 0) && (inKind >= eFONT_MAX))	return	FALSE;

	//	�V���ȃt�H���g�f�[�^�𐶐�
		FONT_DATA*	newFont		= new FONT_DATA();
		
	//	�t�H���g�̐ݒ�
		sprintf_s(	newFont->fontData.FaceName,
					sizeof(newFont->fontData.FaceName),
					inFontStyle);											//!< �t�H���g�̐ݒ�
		newFont->fontData.Height			= inHeight;						//!< �t�H���g�̍���
		newFont->fontData.Width				= inWidth;						//!< �t�H���g�̕�
		newFont->fontData.Weight			= inWeight;						//!< �t�H���g�̑���
		newFont->fontData.MipLevels			= 0;							//!< �t�H���g�̃~�b�v���x��
		newFont->fontData.Italic			= inItalic;						//!< �Α�
		newFont->fontData.CharSet			= SHIFTJIS_CHARSET;				//!< �t�H���g�̕����Z�b�g(�f�t�H���g�̂���)
		newFont->fontData.OutputPrecision	= OUT_DEFAULT_PRECIS;			//!< �o�͐��x
		newFont->fontData.Quality			= ANTIALIASED_QUALITY;			//!< �o�͕i��
		newFont->fontData.PitchAndFamily	= DEFAULT_PITCH | FF_DONTCARE;	//!< �s�b�`�ƃt�@�~��

	//	�t�H���g�̐����Ɏ��s������
		if(	FAILED(D3DXCreateFontIndirect(inDev, &newFont->fontData, &newFont->font)))
		{
		//	�����f�[�^��j��
			SAFE_DELETE(newFont);
			DX::errorMsgNoFile(inDev, inFontStyle);
		//	�������s��Ԃ�
			return FALSE;
		}

	//	�t�H���g�̐����ɐ����������A�O�̃f�[�^���㏑��
		SAFE_DELETE(fonts[inKind]);
		fonts[inKind]	= newFont;

	//	����������Ԃ�
		return TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CFONT_BASE::registerText
	//
	//	�@�\		������̓o�^
	//	����		inWord			������
	//				inKind			�g�p����t�H���g
	//				inPosX			X���W�ʒu
	//				inPosY			Y���W�ʒu
	//				inPosZ			Z���W�ʒu
	//				inColor			�`��J���[
	//				inSizeX			��`�T�C�YX
	//				inSizeY			��`�T�C�YY
	//				inTextCnt		�e�L�X�g�J�E���^(�\�����镶����)
	//	�X�V		2008/09/09		<�V�K>
	//================================================================================
	void
	CFONT_BASE::registerText(LPSTR		inWord,
							FONT_KIND	inKind,
							int			inPosX,
							int			inPosY,
							float		inPosZ,
							D3DCOLOR	inColor,
							int			inSizeX,
							int			inSizeY,
							int			inTextCnt,
							DWORD		inFormat)
	{
	//	�����Ă������ɕs��������Ȃ�A�����������ɏI��
		if(	(inKind < 0) && (inKind >= eFONT_MAX))	return;

	//	�V���ȕ�����f�[�^�𐶐�
		FONT_STRING*	newString	= new FONT_STRING();

	//	�`��ʒu�̐ݒ�
		RECT	 rcDraw		= {	inPosX, inPosY, inSizeX, inSizeY};

	//	������̐ݒ�
		sprintf_s(newString->szWord, sizeof(newString->szWord), inWord);	//!< �\��������ݒ�
		newString->font			= inKind;									//!< �g�p�t�H���g�ݒ�
		newString->rcPos		= rcDraw;									//!< �\���ʒu�ݒ�
		newString->fPriority	= inPosZ;									//!< �v���C�I���e�B
		newString->color		= inColor;									//!< �F�̐ݒ�
		newString->iTextCnt		= inTextCnt;								//!< �\�����镶����
		newString->dFormat		= inFormat;									//!< �`����@

	//	�\��������ꗗ�ɓo�^
		strings.push_back(newString);
	}

	//================================================================================
	//!	���\�b�h��	CFONT_BASE::getFontValid
	//
	//	�@�\		�t�H���g�̓o�^�m�F
	//	����		inKind			�m�F������
	//	�߂�l		TRUE			�o�^�ς�
	//				FALSE			���o�^
	//	�X�V		2008/09/09		<�V�K>
	//================================================================================
	BOOL
	CFONT_BASE::getFontValid(FONT_KIND inKind)
	{
	//	�m�F�����ނ��s���Ȃ�A���o�^��Ԃ�
		if(	(inKind < 0) || (inKind >= eFONT_MAX))	return	FALSE;

	//	�o�^�m�F���A���ʂ�Ԃ�
		return	((fonts[inKind]) ? TRUE : FALSE);
	}

	//================================================================================
	//!	���\�b�h��	CFONT_BASE::drawText
	//
	//	�@�\		�o�^������̈�ĕ`��
	//	����		inSprite		�X�v���C�g�f�o�C�X
	//	�X�V		2008/09/09		<�V�K>
	//================================================================================
	void
	CFONT_BASE::drawText(CONST SPRITE inSprite)
	{
	//	�o�^����Ă��镶���񂪖�����΁A���������Ȃ��ŏI��
		if(	strings.empty())	return;

	//	�`��J�n
		vector<FONT_STRING*>::iterator	p;
		for(p  = strings.begin();
			p != strings.end();
			p++)
		{
			drawText(inSprite, *p);
		}
	}

	//================================================================================
	//!	���\�b�h��	CFONT_BASE::drawText
	//
	//	�@�\		�o�^������̈�ĕ`��
	//	����		inSprite		�X�v���C�g�f�o�C�X
	//				inString		�`�敶����
	//	�X�V		2008/09/09		<�V�K>
	//================================================================================
	void
	CFONT_BASE::drawText(CONST SPRITE inSprite, FONT_STR* inString)
	{
	//	�X�v���C�g��������΁A���������Ȃ��ŏI��
		if(	!inSprite)			return;

	//	�t�H���g�z��ɁA�w��t�H���g������Ε`��
		if(	fonts[inString->font])
		{
		//	�v���C�I���e�B�ݒ�p�s��
			MAT		mat;
		//	�v���C�I���e�B���s��ɐݒ�
			D3DXMatrixTranslation(&mat, 0.0f, 0.0f, inString->fPriority);
		//	�v���C�I���e�B�𔽉f������
			inSprite->SetTransform(&mat);

		//	������̕`��
			fonts[inString->font]->font->DrawText(
				inSprite,
				inString->szWord,
				inString->iTextCnt,
				&inString->rcPos,
				inString->dFormat,
				inString->color);
		}
	}

	//================================================================================
	//!	���\�b�h��	CFONT_BASE::drawText
	//
	//	�@�\		�o�^������̈�ĕ`��(�i�X�\��)
	//	����		inSprite		�X�v���C�g�f�o�C�X
	//				inGraString		�i�X�\������`�敶����
	//	�X�V		2008/09/09		<�V�K>
	//================================================================================
	void
	CFONT_BASE::drawText(CONST SPRITE inSprite, FONT_GRASTR* inGraString)
	{
	//	�X�v���C�g��������΁A���������Ȃ��ŏI��
		if(	!inSprite)			return;

	//	�t�H���g�z��ɁA�w��t�H���g������Ε`��
		if(	fonts[inGraString->font])
		{
		//	���݌��Ă��镶����NULL�łȂ���΁A�\���J�E���^��i�߂�
			if(	inGraString->szWord[inGraString->iTextCnt] != '\0')
			{
				inGraString->iTextGraduallyCnt++;
				inGraString->iTextCnt = (inGraString->iTextGraduallyCnt /
										 inGraString->iTextGraduallySpeed);
			}
		//	�v���C�I���e�B�ݒ�p�s��
			MAT		mat;
		//	�v���C�I���e�B���s��ɐݒ�
			D3DXMatrixTranslation(&mat, 0.0f, 0.0f, inGraString->fPriority);
		//	�v���C�I���e�B�𔽉f������
			inSprite->SetTransform(&mat);

		//	������̕`��
			fonts[inGraString->font]->font->DrawText(
				inSprite,
				inGraString->szWord,
				inGraString->iTextCnt,
				&inGraString->rcPos,
				inGraString->dFormat,
				inGraString->color);
		}
	}

	//================================================================================
	//!	���\�b�h��	CFONT_BASE::drawText
	//
	//	�@�\		�o�^������̈�ĕ`��(�i�X�\��)
	//	����		inSprite		�X�v���C�g�f�o�C�X
	//				inKind			�񋓔ԍ�
	//				inString		�`�悷�镶����
	//	�X�V		2008/09/09		<�V�K>
	//================================================================================
	void
	CFONT_BASE::drawText(CONST SPRITE inSprite, FONT_KIND inKind, LPSTR inString,
						 int inLeft, int inTop, DWORD inColor, DWORD inFormat)
	{
	//	�X�v���C�g��������΁A���������Ȃ��ŏI��
		if(	!inSprite)			return;

	//	�t�H���g�z��ɁA�w��t�H���g������Ε`��
		if(	fonts[inKind])
		{
		//	�v���C�I���e�B�ݒ�p�s��
			MAT		mat;
		//	�v���C�I���e�B���s��ɐݒ�
			D3DXMatrixTranslation(&mat, 0.0f, 0.0f, 0.0f);
		//	�v���C�I���e�B�𔽉f������
			inSprite->SetTransform(&mat);

		//	�`��͈�
			RECT	rcDraw	= { inLeft, inTop, 2000, 2000 };

		//	������̕`��
			fonts[inKind]->font->DrawText(
				inSprite, inString, (UINT)strlen(inString),
				&rcDraw, inFormat, inColor);
		}
	}

	//================================================================================
	//!	���\�b�h��	CFONT_BASE::releaseText
	//
	//	�@�\		�o�^������̈�ĉ��
	//	�X�V		2008/09/09		<�V�K>
	//================================================================================
	void
	CFONT_BASE::releaseText(void)
	{
	//	����J�n
		vector<FONT_STRING*>::iterator	p;
		for(p  = strings.begin();
			p != strings.end();
			p++)
		{
			SAFE_DELETE(*p);
		}
		strings.clear();
	}
}
//-----------------------------------------------------------------------
//!	�t�H���g�x�[�X�̖��O���
//-----------------------------------------------------------------------