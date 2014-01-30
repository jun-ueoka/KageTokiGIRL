//***********************************************************************************
//!	�t�@�C����		Class_Comment.cpp
//
//	�@�\			�R�����g�I�u�W�F�N�g�Ɋւ������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/09/10	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_Comment.h"

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CCOMMENT::�R���X�g���N�^
	//
	//	����		inDev		Direct3D�f�o�C�X
	//				inPtn		�R�����g�p�^�[��
	//				inReversal	���]�m�F
	//				inDraw		�`���
	//				inCntDown	�J�E���g�_�E��
	//				inVoice		������
	//	�X�V		2008/09/10	<�V�K>
	//================================================================================
	CCOMMENT::CCOMMENT(CONST DEV inDev, int inPtn, BOOL inReversal,
					   VEC2* inLoc, int inCntDown, CONST LPSTR inVoice)		:
	CTEXTURE(	inDev, 
				getCommentTextureName((COMMENT_PATTERN)inPtn),
				VEC2(0.0f, 0.0f))
	{
	//	�t�H���g����
		this->font	= CFONT_BASE::getInstance();
	//	�R�����g�p�̃t�H���g����������Ė�����΁A��������
		if(	!this->font->getFontValid(eFONT_COMMENT))
		{
			this->font->createFont(
				inDev,
				eFONT_COMMENT,
				"HG�n�p�p�߯�ߑ�",
				COMMENT_TEXT_SIZEY,
				COMMENT_TEXT_SIZEX, 2, 0);
		}

	//	�����̏�����
		for(int i=0; i<MAX_LINE; i++)	{	this->string[i]	= NULL;	}
		this->targetSize	= VEC2(COMMENT_TEX_BLANKX, COMMENT_TEX_BLANKY);
		this->commentPtn	= (COMMENT_PATTERN)inPtn;
		this->movePtn		= eCMPN_APPEAR;
		this->iCntDown		= inCntDown;

	//	�{�^���̏�����
		this->buttomUp	=
			new CTEXTURE(inDev, TEX_BUTTOM_UP,   VEC2(TEX_BUTTOM_SIZE,TEX_BUTTOM_SIZE));
		this->buttomDown	=
			new CTEXTURE(inDev, TEX_BUTTOM_DOWN, VEC2(TEX_BUTTOM_SIZE,TEX_BUTTOM_SIZE));
		this->iButtomCnt	= 0;

	//	�ʒu�̒���
		this->setFromRect(
			(!inReversal ? 0							 : this->texd_ptr->texInfo.Width),
			0,
			(!inReversal ? this->texd_ptr->texInfo.Width : 0							),
			this->texd_ptr->texInfo.Height);
		this->setLoc(&VEC3(inLoc->x, inLoc->y, 0.0f));

	//	�T�E���h�̏�����(�����������)
		this->sStaticDecision	= new SSTATIC(SOUND_DECISION2);

	//	�����������ݒ�Ȃ�A�������Ȃ�
		if(	!strcmp(inVoice, ""))
			this->sStaticVoice	= NULL;
		else
		{
			this->sStaticVoice	= new SSTATIC(inVoice);
			this->sStaticVoice->setVolume(1500);
		}
	}

	//================================================================================
	//!	���\�b�h��	CCOMMENT::�f�X�g���N�^
	//
	//	�X�V		2008/09/10	<�V�K>
	//================================================================================
	CCOMMENT::~CCOMMENT(void)
	{
	//	�����̉��
		for(int i=0; i<MAX_LINE; i++)
		{
			SAFE_DELETE(this->string[i]);
		}

	//	�{�^���̉��
		SAFE_DELETE(this->buttomUp);
		SAFE_DELETE(this->buttomDown);
		SAFE_DELETE(this->sStaticDecision);
		SAFE_DELETE(this->sStaticVoice);
	}

	//================================================================================
	//!	���\�b�h��	CCOMMENT::getCommentTextureName
	//
	//	�@�\		�R�����g�p�^�[���ɍ������A�e�N�X�`�����̌���
	//	����		inPtn		�R�����g�p�^�[��
	//	�߂�l		�e�N�X�`����
	//	�X�V		2008/09/10	<�V�K>
	//================================================================================
	LPSTR
	CCOMMENT::getCommentTextureName(COMMENT_PATTERN inPtn)
	{
	//	�p�^�[������
		switch(inPtn)
		{
			case	eCPTN_NORMAL:		return	COMMENT_NORMAL;			break;
			case	eCPTN_SHOUT:		return	COMMENT_SHOUT;			break;
			case	eCPTN_SKY:			return	COMMENT_SKY;			break;
			case	eCPTN_SYSTEM:		return	COMMENT_SYSTEM;			break;
			case	eCPTN_NARRATION:	return	COMMENT_NARRATION;		break;
			default:					return	NULL;					break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CCOMMENT::setString
	//
	//	�@�\		�R�����g�s�̑}��
	//	����		inString	�R�����g������
	//				inLine		�s�ԍ�
	//	�X�V		2008/09/10	<�V�K>
	//================================================================================
	void
	CCOMMENT::setString(LPSTR inString, int inLine, D3DCOLOR inColor)
	{
	//	�s�ԍ����s���Ȃ�A������Ԃ�
		if(	(inLine < 0) || (inLine >= MAX_LINE))	return;

	//	�w�肳�ꂽ�s�ԍ��̏ꏊ�ɂ��łɃf�[�^������Ȃ�A������������
		if(	this->string[inLine])	SAFE_DELETE(this->string[inLine]);

	//	�t�H���g������̍쐬
		FONT_STRING*	newString	= new FONT_STRING();	

		newString->font		= eFONT_COMMENT;
		newString->color	= inColor;
		wsprintf(newString->szWord, inString);

	//	�R�����g�s�ɓo�^
		this->string[inLine]	= newString;

	//	�R�����g�o���[���̌`��ω�
		int		iMaxLength	= 0;							//!< ������̍ő�̒���
		int		iLength		= 0;							//!< ������̒���

		for(int i=0; i<MAX_LINE; i++)
		{
		//	���݌��Ă���s�ɁA�R�����g�����鎞
			if(	this->string[i])
			{
			//	�R�����g�̃o���[���̏c�T�C�Y��ύX
				this->targetSize.y	=
					COMMENT_TEX_BLANKY + (COMMENT_TEXT_SIZEY*(float)(i+1));
			//	���������󂯎��
				iLength	= (int)strlen(this->string[i]->szWord);
			//	�ő啶����Ɣ�r���A�X�V�����Ȃ�A���T�C�Y��ύX
				if(	iMaxLength < iLength)
				{
					iMaxLength = iLength;
					this->targetSize.x	= 
						COMMENT_TEX_BLANKX + (COMMENT_TEXT_SIZEX*(float)(iMaxLength));
				}
			}
		}

	//	�t�H���g�ʒu�̏C��
		RECT	rcComment	=
		{
			(int)(this->getLocX() - ((this->targetSize.x - COMMENT_TEX_BLANKX) / 2.0f)),
			(int)(this->getLocY() - ((this->targetSize.y - COMMENT_TEX_BLANKY) / 2.0f)),
			1000,
			1000
		};

	//	�F����p
		MY_COLOR_BGRA	myColor;

		for(int i=0; i<MAX_LINE; i++)
		{
		//	���݌��Ă���s�ɁA�R�����g�����鎞
			if(	this->string[i])
			{
			//	�ʒu��ݒ�
				this->string[i]->rcPos	= rcComment;

			//	�e�p�̃R�����g��ݒ�
				this->stringShadow[i]	= *this->string[i];
			//	�e�̐F��ݒ�
				myColor.color	= this->stringShadow[i].color;
				if(	myColor.bgra.red	> 150	||
					myColor.bgra.green	> 150	||
					myColor.bgra.blue	> 150)
				{
					this->stringShadow[i].color	=
						D3DCOLOR_RGBA(0, 0, 0, myColor.bgra.alpha);
				}
				else
				{
					this->stringShadow[i].color	=
						D3DCOLOR_RGBA(200, 200, 200, myColor.bgra.alpha);
				}
			//	�e�̈ʒu�����炷
				OffsetRect(&stringShadow[i].rcPos, 2, 2);
			}
		//	�t�H���g�ʒu�����炷
			OffsetRect(&rcComment, 0, COMMENT_TEXT_SIZEY);
		}

	//	�{�^���ʒu�̒���
		this->buttomUp->setLoc(&VEC3(
			this->getLocX() + (this->targetSize.x / 3.0f), this->getLocY() + (this->targetSize.y / 5.0f), 0.0f));
		this->buttomDown->setLoc(
			&this->buttomUp->getLoc());
	}

	//================================================================================
	//!	���\�b�h��	CCOMMENT::motionAppear
	//
	//	�@�\		�o������
	//	�X�V		2008/09/10	<�V�K>
	//================================================================================
	void
	CCOMMENT::motionAppear(void)
	{
	//	�g��I������p
		BOOL	bStopX	= FALSE;
		BOOL	bStopY	= FALSE;

	//	�R�����g�T�C�Y���g�債�Ă���
		if(	this->showSize.x +  COMMENT_EXPAND <= this->targetSize.x)
		{
			this->showSize.x += COMMENT_EXPAND;
		}
		else
		{
			this->showSize.x  = this->targetSize.x;
			bStopX	  = TRUE;
		}
		if(	this->showSize.y +  COMMENT_EXPAND <= this->targetSize.y)
		{
			this->showSize.y += COMMENT_EXPAND;
		}
		else
		{
			this->showSize.y  = this->targetSize.y;
			bStopY	  = TRUE;
		}

	//	���E�Ɋg�債�؂�����
		if(	bStopX && bStopY)
		{
		//	�ҋ@����Ɉڂ�
			this->movePtn	= eCMPN_WAIT;
		//	�����̔���
			this->soundVoice();
		}
	}

	//================================================================================
	//!	���\�b�h��	CCOMMENT::motionWait
	//
	//	�@�\		�ҋ@����
	//	����		inBoolen	���X�e�b�v����
	//	�X�V		2008/09/10	<�V�K>
	//================================================================================
	void
	CCOMMENT::motionWait(BOOL inBoolen)
	{
	//	�J�E���g�_�E����0�ɂȂ�����A�����P��
		if(	this->iCntDown <  NO_CNT_DOWN)
		{
			this->iCntDown--;
			if(	this->iCntDown <= 0)
			{
				inBoolen = TRUE;
			}
		}

	//	���X�e�b�v����
		if(	inBoolen)
		{
		//	���œ���Ɉڂ�
			this->movePtn	= eCMPN_DISAPPEAR;
		//	���艹
			this->soundDecision();
		}
	}

	//================================================================================
	//!	���\�b�h��	CCOMMENT::motionDisappear
	//
	//	�@�\		���œ���
	//	�X�V		2008/09/10	<�V�K>
	//================================================================================
	void
	CCOMMENT::motionDisappear(void)
	{
	//	�k���I������p
		BOOL	bStopX	= FALSE;
		BOOL	bStopY	= FALSE;

	//	�R�����g�T�C�Y���k�����Ă���
		if(	this->showSize.x -  COMMENT_EXPAND >= 0.0f)
		{
			this->showSize.x -= COMMENT_EXPAND;
		}
		else
		{
			this->showSize.x  = 0.0f;
			bStopX	  = TRUE;
		}
		if(	this->showSize.y -  COMMENT_EXPAND >= 0.0f)
		{
			this->showSize.y -= COMMENT_EXPAND;
		}
		else
		{
			this->showSize.y  = 0.0f;
			bStopY	  = TRUE;
		}

	//	���E�ɏk�����؂�����
		if(	bStopX && bStopY)
		{
		//	���݂��Ȃ�
			this->movePtn	= eCMPN_FALSE;
		}
	}

	//================================================================================
	//!	���\�b�h��	CCOMMENT::motion
	//
	//	�@�\		�R�����g�̓���
	//	����		inBoolen	���X�e�b�v����
	//	�߂�l		TRUE		���s����
	//				FALSE		�폜����
	//	�X�V		2008/09/10	<�V�K>
	//================================================================================
	BOOL
	CCOMMENT::motion(BOOL inBoolen)
	{
		switch(this->movePtn)	
		{
		//	�o������
			case	eCMPN_APPEAR:		this->motionAppear();		return	TRUE;
		//	�ҋ@����
			case	eCMPN_WAIT:			this->motionWait(inBoolen);	return	TRUE;
		//	���œ���
			case	eCMPN_DISAPPEAR:	this->motionDisappear();	return	TRUE;
		//	���̑��Ȃ�A�������Ȃ�
			default:												return	FALSE;
		}
	}

	//================================================================================
	//!	���\�b�h��	CCOMMENT::drawAppear
	//
	//	�@�\		�o���`��
	//	����		inSprite	�X�v���C�g�f�o�C�X
	//	�X�V		2008/09/10	<�V�K>
	//================================================================================
	void
	CCOMMENT::drawAppear(CONST SPRITE inSprite)
	{
		CTEXTURE::draw(inSprite);
	}

	//================================================================================
	//!	���\�b�h��	CCOMMENT::drawWait
	//
	//	�@�\		�ҋ@�`��
	//	����		inSprite	�X�v���C�g�f�o�C�X
	//	�X�V		2008/09/10	<�V�K>
	//================================================================================
	void
	CCOMMENT::drawWait(CONST SPRITE inSprite)
	{
		CTEXTURE::draw(inSprite);			

	//	������̕`��
		for(int i=0; i<MAX_LINE; i++)
		{
		//	���݌��Ă���s�ɕ����񂪂���Ȃ�A�`��
			if(	this->string[i])
			{
				this->font->drawText(inSprite, &this->stringShadow[i]);
				this->font->drawText(inSprite, this->string[i]);
			}
		}

	//	����������ŁA�{�^���̕`��
		if(	this->commentPtn != eCPTN_NARRATION)
		{
			this->iButtomCnt++;
		//	20�t���[���܂ŁA�{�^��������Ă��Ȃ��e�N�X�`����\��
			if		(this->iButtomCnt	<= 20)	this->buttomUp->draw(inSprite);
		//	21�t���[������A�{�^��������Ă���e�N�X�`����\��
			else if	(this->iButtomCnt	>  20)	this->buttomDown->draw(inSprite);
		//	30�t���[���ɓ��B������A�J�E���^�����Z�b�g
			if(	this->iButtomCnt >= 30)			this->iButtomCnt = 0;
		}
	}

	//================================================================================
	//!	���\�b�h��	CCOMMENT::drawDisappear
	//
	//	�@�\		���ŕ`��
	//	����		inSprite	�X�v���C�g�f�o�C�X
	//	�X�V		2008/09/10	<�V�K>
	//================================================================================
	void
	CCOMMENT::drawDisappear(CONST SPRITE inSprite)
	{
		CTEXTURE::draw(inSprite);
	}

	//================================================================================
	//!	���\�b�h��	CCOMMENT::draw
	//
	//	�@�\		�R�����g�̕`��
	//	����		inSprite	�X�v���C�g�f�o�C�X
	//	�߂�l		TRUE		�R�����g���s��
	//				FALSE		�R�����g�I��
	//	�X�V		2008/09/10	<�V�K>
	//================================================================================
	BOOL
	CCOMMENT::draw(CONST SPRITE inSprite)
	{
	//	�g�����X�v���C�g�ɓ����
		switch(this->movePtn)
		{
		//	�o���`��
			case	eCMPN_APPEAR:		this->drawAppear(inSprite);		return	TRUE;
		//	�ҋ@�`��
			case	eCMPN_WAIT:			this->drawWait(inSprite);		return	TRUE;
		//	���ŕ`��
			case	eCMPN_DISAPPEAR:	this->drawDisappear(inSprite);	return	TRUE;
		//	���̑��Ȃ�A�������Ȃ�
			default:													return	FALSE;
		}
	}

	//================================================================================
	//!	���\�b�h��	CCOMMENT_GROUP::executeComment
	//
	//	�@�\		�R�����g�̎��s
	//	����		inBoolen	�R�����g��i�߂邩�̊m�F
	//	�߂�l		TRUE		�c��R�����g�L��
	//				FALSE		�c��R�����g����
	//	�X�V		2008/11/11	<�V�K>
	//================================================================================
	BOOL
	CCOMMENT_GROUP::executeComment(BOOL inBoolen)
	{
	//	�S�R�����g�̓���
		VEC_WHILE(CCOMMENT*, vComment)
		{
		//	��������s���A�I��������������������
			if(	!(*p)->motion(inBoolen))
			{
				SAFE_DELETE(*p);
				p	= this->vComment.erase(p);
			}
		//	�I�����Ė����Ȃ�A�C�e���[�^��i�߂�
			else
			{
				p++;
			}
		}
	//	�R�����g���Ȃ��Ȃ�������Ԃ�
		return	(this->vComment.empty() ? FALSE : TRUE);
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------