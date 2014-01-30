//***********************************************************************************
//!	�t�@�C����		Class_SceneTitle.cpp
//
//	�@�\			�^�C�g���V�[���̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2008/9/01	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneTitle.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::CSCENE_TITLE
	//
	//	�@�\		�R���X�g���N�^
	//	����		inData			�V�[�����L�f�[�^
	//	�X�V		2008/09/01		<�V�K>
	//================================================================================
	CSCENE_TITLE::CSCENE_TITLE(SCENE_SHARE_DATA* inData)	:
	CSCENE(eSCENE_TITLE, eSCENE_TITLE, inData)		//!< ��b�V�[���t���O�̐ݒ�
	{
	//	������
		this->titleFlag			= eST_INIT_TITLE;			//!< ����t���O(�Q�[���v���C��)
		this->startFlag			= eSCENE_CHAPTER1_MOVIE1;	//!< �X�^�[�g�ڍs�t���O
		this->endFlag			= eSCENE_QUIT;				//!< �G���h�ڍs�t���O
		this->loopFlag			= eSCENE_LOGO;				//!< ���[�v�t���O

		this->cursorPos			= eCURP_START;						//!< �J�[�\���ʒu
		this->gDev				= CGRAPHIC_DEVICE::getInstance();	//!< �O���t�B�b�N�f�o�C�X
		this->effect			= C3D_EFFECT_CENTER::getInstance();	//!< �G�t�F�N�g�Z���^�[

		this->titleBack			= NULL;				//!< �^�C�g���w�i
		this->titleTex			= NULL;				//!< �^�C�g��
		this->start				= NULL;				//!< �uSTART�v
		this->end				= NULL;				//!< �uEND�v
		this->option			= NULL;				//!< �uOPTION�v
		this->cursor			= NULL;				//!< �J�[�\��
		this->font				= NULL;				//!< �t�H���g

		this->sStaticCursor		= NULL;				//!< �J�[�\����
		this->sStaticDecision	= NULL;				//!< ���艹
		this->sStreamingBgm		= NULL;				//!< �w�i��

		this->iStack			= 0;				//!< �X�^�b�N�J�E���^
		this->bStack			= FALSE;			//!< �X�^�b�N�m�F
		this->iTimeCnt			= 0;				//!< �^�C���J�E���^

	//	���̐F��ݒ�
		this->gDev->setCurtainColor(0, 0, 0);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::~CSCENE_TITLE
	//
	//	�@�\		�f�X�g���N�^
	//	�X�V		2008/09/01		<�V�K>
	//================================================================================
	CSCENE_TITLE::~CSCENE_TITLE(void)
	{
	//	���
		this->sceneFinishalize();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::motionInitTitle
	//
	//	�@�\		�^�C�g����ʈڍs���o����
	//	����		input			���͏��
	//	�X�V		2008/09/01		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::motionInitTitle(CINPUT* input)
	{
	//	�X�^�b�N�t���[�����J�E���g
		this->iStack++;
	//	�^�C�g���o��
		if(	this->iStack > 100	&&
			this->iStack < 200)
		{
			if(	this->iStack < 150)
			{
				this->titleTex->calcColor(0, 0, 0, 5);
			}
			else
			{
				this->titleTex->calcColor(5, 5, 5, 5);
			}
		}
	//	�X�^�[�g�o��
		if(	this->iStack > 130	&&
			this->iStack < 230)
		{
			if(	this->iStack < 180)
			{
				this->start->calcColor(0, 0, 0, 5);
			}
			else
			{
				this->start->calcColor(5, 5, 5, 5);
			}
		}
	//	�I�v�V�����o��
		if(	this->iStack > 160	&&
			this->iStack < 260)
		{
			if(	this->iStack < 210)
			{
				this->option->calcColor(0, 0, 0, 5);
			}
			else
			{
				this->option->calcColor(5, 5, 5, 5);
			}
		}
	//	�I�v�V�����o��
		if(	this->iStack > 190	&&
			this->iStack < 290)
		{
			if(	this->iStack < 240)
			{
				this->end->calcColor(0, 0, 0, 5);
			}
			else
			{
				this->end->calcColor(5, 5, 5, 5);
			}
		}

	//	�X�y�[�X�L�[�������ꂽ��A�^�C�g���Ɉڂ�
		if(	input->checkPress(MYKEY::eSTART)	||
			this->iStack > 350)
		{
			this->initTitle();
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::motionTitle
	//
	//	�@�\		�^�C�g����ʏ���
	//	����		input			���͏��
	//	�X�V		2008/09/01		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::motionTitle(CINPUT* input)
	{
	//-------------------------------------------------
	//	�C���^�[�t�F�[�X�̓���
	//-------------------------------------------------
	//	�J�[�\���ʒu�ɂ��A��������
		switch(this->cursorPos)
		{
		//	�uSTART�v�Ɉʒu����
			case eCURP_START:
			//	�J�[�\�������Ɉړ�
				if(	input->checkPressOnce(MYKEY::eDOWN))
				{
					this->cursorPos	= eCURP_GO_OPTION_FROM_START;
				//	�J�[�\����
					this->soundCursor();
				}
			//	����L�[�������ꂽ��A�Q�[���X�^�[�g���o�Ɉڂ�
				if(	input->checkPressOnce(MYKEY::eSTART))
				{
					this->initStartSwitch();
				//	���艹
					this->soundDecision();
				}
				break;

		//	�uSTART�v�Ɍ�����
			case eCURP_GO_START:
			//	�J�[�\������Ɉړ����āA�uSTART�v�ʒu�ɂ��Ă��Ȃ���
				if((this->cursor->getLocY() - CURSOR_MOVE_SPEED) >=
					CURSOR_START_POSY)
				{
					this->cursor->calcLocY(-CURSOR_MOVE_SPEED);
				}
			//	�J�[�\���ʒu���A�uSTART�v�ɕt������
				else
				{
					this->cursorPos	= eCURP_START;
					this->cursor->setLocY(CURSOR_START_POSY);
				}
				break;

		//	�uOPTION�v�Ɉʒu����
			case eCURP_OPTION:
			//	�J�[�\������Ɉړ�
				if(	input->checkPressOnce(MYKEY::eUP))
				{
					this->cursorPos	= eCURP_GO_START;
				//	�J�[�\����
					this->soundCursor();
				}
			//	�J�[�\�������Ɉړ�
				if(	input->checkPressOnce(MYKEY::eDOWN))
				{
					this->cursorPos	= eCURP_GO_END;
				//	�J�[�\����
					this->soundCursor();
				}
			//	����L�[�������ꂽ��A�Q�[���I�����o�Ɉڂ�
				if(	input->checkPressOnce(MYKEY::eSTART))
				{
				//	this->initEndSwitch();
				////	���艹
				//	this->soundDecision();
				}
				break;

		//	�uSTART�v����uOPTION�v�Ɍ�����
			case eCURP_GO_OPTION_FROM_START:
			//	�J�[�\�������Ɉړ����āA�uOPTION�v�ʒu�ɂ��Ă��Ȃ���
				if(	(this->cursor->getLocY() + CURSOR_MOVE_SPEED) <=
					CURSOR_OPTION_POSY)
				{
					this->cursor->calcLocY(CURSOR_MOVE_SPEED);
				}
			//	�J�[�\���ʒu���A�uOPTION�v�ɕt������
				else
				{
					this->cursorPos	= eCURP_OPTION;
					this->cursor->setLocY(CURSOR_OPTION_POSY);
				}
				break;

		//	�uEND�v����uOPTION�v�Ɍ�����
			case eCURP_GO_OPTION_FROM_END:
			//	�J�[�\������Ɉړ����āA�uOPTION�v�ʒu�ɂ��Ă��Ȃ���
				if(	(this->cursor->getLocY() - CURSOR_MOVE_SPEED) >=
					CURSOR_OPTION_POSY)
				{
					this->cursor->calcLocY(-CURSOR_MOVE_SPEED);
				}
			//	�J�[�\���ʒu���A�uOPTION�v�ɕt������
				else
				{
					this->cursorPos	= eCURP_OPTION;
					this->cursor->setLocY(CURSOR_OPTION_POSY);
				}
				break;


		//	�uEND�v�Ɉʒu����
			case eCURP_END:
			//	�J�[�\������Ɉړ�
				if(	input->checkPressOnce(MYKEY::eUP))
				{
					this->cursorPos	= eCURP_GO_OPTION_FROM_END;
				//	�J�[�\����
					this->soundCursor();
				}
			//	����L�[�������ꂽ��A�Q�[���I�����o�Ɉڂ�
				if(	input->checkPressOnce(MYKEY::eSTART))
				{
					this->initEndSwitch();
				//	���艹
					this->soundDecision();
				}
				break;

		//	�uEMD�v�Ɍ�����
			case eCURP_GO_END:
			//	�J�[�\������Ɉړ����āA�uEND�v�ʒu�ɂ��Ă��Ȃ���
				if(	(this->cursor->getLocY() + CURSOR_MOVE_SPEED) <=
					CURSOR_END_POSY)
				{
					this->cursor->calcLocY(CURSOR_MOVE_SPEED);
				}
			//	�J�[�\���ʒu���A�uEND�v�ɕt������
				else
				{
					this->cursorPos	= eCURP_END;
					this->cursor->setLocY(CURSOR_END_POSY);
				}
				break;
		}

	//	�^�C���`�F�b�N���A�f���ڍs�t���[���܂ŗ�����A�f���Ɉڍs
		if(	(++this->iTimeCnt) > DEMO_STEP_TIME)
		{
			this->initDemo();
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::motionStartSwitch
	//
	//	�@�\		�Q�[���X�^�[�g���o����
	//	����		input			���͏��
	//	�X�V		2008/09/20		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::motionStartSwitch(CINPUT* input)
	{
	//	�uSTART�v�g��
		if(	(this->iStack >  0) &&
			(this->iStack <= STARTSWITCH_EXPAND))
		{
			this->start->calcShowSize(&VEC2(2.3f, 1.0f));
		}

	//	���̏o��
		if(	(this->iStack >= STARTSWITCH_CLEAR_S)	&&
			(this->iStack <= STARTSWITCH_CLEAR_E))
		{
			this->gDev->curtainMotion(TRUE, 10);
		}

	//	�X�^�b�N�t���[�����J�E���g
		this->iStack++;

	//	�X�^�b�N�����l�܂ōs������A���[�r�[�V�[����
		if(	this->iStack > STARTSWITCH_END)
		{
			this->returnFlag	= this->startFlag;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::motionEndSwitch
	//
	//	�@�\		�Q�[���I�����o����
	//	����		input			���͏��
	//	�X�V		2008/09/20		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::motionEndSwitch(CINPUT* input)
	{
	//	�uEND�v�g��
		if(	(this->iStack >  0) &&
			(this->iStack <= ENDSWITCH_EXPAND))
		{
			this->end->calcShowSize(&VEC2(2.3f, 1.0f));
		}

	//	���̏o��
		if(	(this->iStack >= ENDSWITCH_CLEAR_S)	&&
			(this->iStack <= ENDSWITCH_CLEAR_E))
		{
			this->gDev->curtainMotion(TRUE, 5);
		}

	//	�X�^�b�N�t���[�����J�E���g
		this->iStack++;

	//	�X�^�b�N�����l�܂ōs������A�I����
		if(	this->iStack > ENDSWITCH_END)
		{
		//	TGS�p����
			this->returnFlag	= this->endFlag;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::motionDemo
	//
	//	�@�\		�f���ڍs���o����
	//	����		input			���͏��
	//	�X�V		2008/09/30		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::motionDemo(CINPUT* input)
	{
	//	���̏o��
		if(	(this->iStack >= DEMO_CLEAR_S)	&&
			(this->iStack <= DEMO_CLEAR_E))
		{
			this->gDev->curtainMotion(TRUE, 5);
		}

	//	�X�^�b�N�t���[�����J�E���g
		this->iStack++;

	//	�X�^�b�N�����l�܂ōs������A�f����
		if(	this->iStack > DEMO_END)
		{
			this->returnFlag	= this->loopFlag;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::renderInitTitle
	//
	//	�@�\		�^�C�g����ʈڍs���o�`��
	//	�X�V		2008/09/01		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::renderInitTitle(void)
	{
	//	�C���^�[�t�F�[�X�̕`��
		this->titleTex->draw(this->gDev->getSprite());
		this->start->draw(this->gDev->getSprite());
		this->option->draw(this->gDev->getSprite());
		this->end->draw(this->gDev->getSprite());
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::renderTitle
	//
	//	�@�\		�^�C�g����ʕ`��
	//	�X�V		2008/09/01		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::renderTitle(void)
	{
	//	�C���^�[�t�F�[�X�̕`��
		this->titleTex->draw(this->gDev->getSprite());
		this->start->draw(this->gDev->getSprite());
		this->option->draw(this->gDev->getSprite());
		this->end->draw(this->gDev->getSprite());
		this->cursor->draw(this->gDev->getSprite());
		this->font->drawText(this->gDev->getSprite());
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::renderStartSwitch
	//
	//	�@�\		�Q�[���X�^�[�g���o�`��
	//	�X�V		2008/09/20		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::renderStartSwitch(void)
	{
	//	�C���^�[�t�F�[�X�̕`��
		this->titleTex->draw(this->gDev->getSprite());
		this->start->draw(this->gDev->getSprite());
		this->option->draw(this->gDev->getSprite());
		this->end->draw(this->gDev->getSprite());
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::renderEndSwitch
	//
	//	�@�\		�Q�[���I�����o�`��
	//	�X�V		2008/09/20		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::renderEndSwitch(void)
	{
	//	�C���^�[�t�F�[�X�̕`��
		this->titleTex->draw(this->gDev->getSprite());
		this->start->draw(this->gDev->getSprite());
		this->option->draw(this->gDev->getSprite());
		this->end->draw(this->gDev->getSprite());
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::renderDemo
	//
	//	�@�\		�f���ڍs���o�`��
	//	�X�V		2008/09/30		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::renderDemo(void)
	{
	//	�C���^�[�t�F�[�X�̕`��
		this->titleTex->draw(this->gDev->getSprite());
		this->start->draw(this->gDev->getSprite());
		this->option->draw(this->gDev->getSprite());
		this->end->draw(this->gDev->getSprite());
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::initInitTitle
	//
	//	�@�\		�^�C�g����ʈڍs���o����
	//	�X�V		2008/09/20		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::initInitTitle(void)
	{
	//	�^�C�g����ʈڍs���o�ɕύX
		this->titleFlag	= eST_INIT_TITLE;
	//	���̑��̏���
		iStack			= 0;
		this->titleTex->setColor(0x00000000);
		this->start->setColor(0x00000000);
		this->option->setColor(0x00000000);
		this->end->setColor(0x00000000);
	//	���̏���
		this->gDev->setCurtainColor(0,0,0);
		this->gDev->setCurtainColorAlpha(255);
		this->gDev->curtainMotion(FALSE, 5);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::initTitle
	//
	//	�@�\		�^�C�g����ʖ���
	//	�X�V		2008/09/20		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::initTitle(void)
	{
	//	�^�C�g����ʂɕύX
		this->titleFlag	= eST_TITLE;
	//	���̑��̏���
		iStack			= 0;
		this->titleTex->setColor(0xFFFFFFFF);
		this->start->setColor(0xFFFFFFFF);
		this->option->setColor(0xFFFFFFFF);
		this->end->setColor(0xFFFFFFFF);
		this->gDev->setCurtainColorAlpha(0);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::initStartSwitch
	//
	//	�@�\		�Q�[���X�^�[�g���o����
	//	�X�V		2008/09/20		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::initStartSwitch(void)
	{
	//	�Q�[���X�^�[�g���o�ɕύX
		this->titleFlag	= eST_START_SWITCH;
	//	���̑��̏���
		iStack			= 0;
		this->start->setLocZ(0.0f);
		this->titleTex->setColor(0xFFFFFFFF);
		this->start->setColor(0xFFFFFFFF);
		this->option->setColor(0xFFFFFFFF);
		this->end->setColor(0xFFFFFFFF);
		this->bStack	= FALSE;

	//	���e�N�X�`���𐶐�
		this->gDev->setCurtainColor(0, 0, 0);
		this->gDev->createCurtainTex("Loading_KageoKagemiA");
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::initEndSwitch
	//
	//	�@�\		�Q�[���I�����o����
	//	�X�V		2008/09/20		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::initEndSwitch(void)
	{
	//	�Q�[���I�����o�ɕύX
		this->titleFlag	= eST_END_SWITCH;
	//	���̑��̏���
		iStack			= 0;
		this->end->setLocZ(0.0f);
		this->titleTex->setColor(0xFFFFFFFF);
		this->start->setColor(0xFFFFFFFF);
		this->option->setColor(0xFFFFFFFF);
		this->end->setColor(0xFFFFFFFF);
		this->bStack	= FALSE;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::initDemo
	//
	//	�@�\		�f���ڍs���o����
	//	�X�V		2008/09/30		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::initDemo(void)
	{
	//	�Q�[���I�����o�ɕύX
		this->titleFlag	= eST_DEMO;
	//	���̑��̏���
		iStack			= 0;
		this->titleTex->setColorAlpha(255);
		this->start->setColorAlpha(255);
		this->option->setColorAlpha(255);
		this->end->setColorAlpha(255);
		this->bStack	= FALSE;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::sceneInitialize
	//
	//	�@�\		������
	//	����		inNum			�O���f�[�^�ԍ�
	//	�X�V		2008/09/01		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::sceneInitialize(int inStageNum)
	{
	//	�������
		this->sceneFinishalize();

	//-------------------------------------------------
	//	�^�C�g���̃C���^�[�t�F�[�X��ǂݍ���
	//-------------------------------------------------
	//	�^�C�g���w�i����
		this->titleBack		=
			new CTEXTURE(this->gDev->getDevice(),
						 "title_interface1",
						 VEC2(1024.0f,768.0f),
						 D3DCOLOR_RGBA(0, 0, 0, 100));
		this->titleBack->setFromRect(0, 0, 1024, 768);
		this->titleBack->setLoc(&VEC3((float)this->gDev->getScreenSizeX() / 2.0f,
									  (float)this->gDev->getScreenSizeY() / 2.0f,
									  1.0f));
	//	�^�C�g������
		this->titleTex		=
			new CTEXTURE(this->gDev->getDevice(),
						 "title_interface2",
						 VEC2(640.0f,512.0f),
						 D3DCOLOR_RGBA(0, 0, 0, 100));
		this->titleTex->setFromRect(0, 0, 640, 512);
		this->titleTex->setLoc(&VEC3((float)this->gDev->getScreenSizeX() / 2.0f, 200.0f, 0.5f));

	//	�uSTART�v����
		this->start		=	new CTEXTURE(this->titleTex);
		this->start->setToRect(VEC2(300.0f, 90.0f));
		this->start->setFromRect(640, 0, 1024, 128);
		this->start->setLoc(&VEC3(700.0f, CURSOR_START_POSY, 0.5f));

	//	�uOPTION�v����
		this->option	=	new CTEXTURE(this->titleTex);
		this->option->setToRect(VEC2(300.0f, 90.0f));
		this->option->setFromRect(640, 256, 1024, 384);
		this->option->setLoc(&VEC3(700.0f, CURSOR_OPTION_POSY, 0.5f));

	//	�uEND�v����
		this->end		=	new CTEXTURE(this->titleTex);
		this->end->setToRect(VEC2(300.0f, 90.0f));
		this->end->setFromRect(640, 128, 1024, 256);
		this->end->setLoc(&VEC3(700.0f, CURSOR_END_POSY, 0.5f));

	//	�J�[�\������
		this->cursor	=	new CTEXTURE(this->titleTex);
		this->cursor->setToRect(VEC2(128.0f, 128.0f));
		this->cursor->setFromRect(0, 512, 128, 640);
		this->cursor->setLoc(&VEC3(550.0f, CURSOR_START_POSY, 0.5f));

	//	�R�����g����
		this->font	= CFONT_BASE::getInstance();
	//	�R�����g�p�̃t�H���g����������Ė�����΁A��������
		if(	!this->font->getFontValid(eFONT_PTN1))
		{
			this->font->createFont(
				this->gDev->getDevice(),
				eFONT_PTN1,
				"HG�n�p�p�߯�ߑ�",
				40, 20, 2, 0);
		}
	//	������̓o�^
		this->font->registerText(
			"PUSH START BUTTOM",
			eFONT_PTN1,
			540, 450, 0.0f,
			D3DCOLOR_RGBA(255,255,255,255));
		this->font->registerText(
			"COPYRIGHT @ PROJECT KAGETOKI",
			eFONT_PTN1,
			450, 720, 0.0f,
			D3DCOLOR_RGBA(255,255,255,255));

	//	�T�E���h�̏���
		this->sStaticCursor		= new SSTATIC(SOUND_CURSOR);
		this->sStaticDecision	= new SSTATIC(SOUND_DECISION1);
		this->sStaticDecision->setVolume(-1500);

	//	�����Ŏg�p����G�t�F�N�g�̐���
		this->effect->addEffects(
			NAME_SHADOW, new C3D_EFFECT(this->gDev->getDevice(), NAME_SHADOW, VEC3(0.0f,0.0f,0.0f)));
		this->effect->addEffects(
			NAME_SHINE,  new C3D_EFFECT(this->gDev->getDevice(), NAME_SHINE,  VEC3(0.0f,0.0f,0.0f)));

	//	�^�C�g����ʈڍs���o�̎��s
		this->initInitTitle();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::sceneFinishalize
	//
	//	�@�\		���
	//	�X�V		2008/09/01		<�V�K>
	//================================================================================
	void
	CSCENE_TITLE::sceneFinishalize(void)
	{
	//	�������
		SAFE_DELETE(this->titleBack);
		SAFE_DELETE(this->titleTex);
		SAFE_DELETE(this->start);
		SAFE_DELETE(this->option);
		SAFE_DELETE(this->end);
		SAFE_DELETE(this->cursor);

	//	�e�L�X�g�̉��
		if(	this->font)	this->font->releaseText();

	//	�T�E���h�f�[�^�̉��
		SAFE_DELETE(this->sStaticCursor);
		SAFE_DELETE(this->sStaticDecision);
		SAFE_DELETE(this->sStreamingBgm);

	//	�����Ő������ꂽ�G�t�F�N�g������
		this->effect->finishalize();

	}

	//================================================================================
	//!	���\�b�h��	CSCENE_TITLE::sceneMain
	//
	//	�@�\		�V�[�����C��
	//	�X�V		2008/09/01		<�V�K>
	//================================================================================
	SCENE_FLAG
	CSCENE_TITLE::sceneMain(CINPUT* input)
	{
	//	�`��J�n
		this->gDev->renderBegin(D3DCOLOR_RGBA(0,0,0,255));
		this->gDev->spriteBegin();

	//	�^�C�g���w�i�`��
		this->titleBack->draw(this->gDev->getSprite());
	//	�t���O����
		switch(this->titleFlag)
		{
		//	�^�C�g����ʈڍs���o
			case	eST_INIT_TITLE:
				this->motionInitTitle(input);
				this->renderInitTitle();
				break;
		//	�^�C�g�����
			case	eST_TITLE:
				this->motionTitle(input);
				this->renderTitle();
				break;	
		//	�Q�[���X�^�[�g���o
			case	eST_START_SWITCH:
				this->motionStartSwitch(input);
				this->renderStartSwitch();
				break;		
		//	�Q�[���I�����o
			case	eST_END_SWITCH:
				this->motionEndSwitch(input);
				this->renderEndSwitch();
				break;
		//	�f���ڍs���o
			case	eST_DEMO:
				this->motionDemo(input);
				this->renderDemo();
				break;
			default:
				break;
		}

	//	�G�t�F�N�g�`��
		this->effect->drawEffects2D(this->gDev->getSprite());

	//	�`��I��
		this->gDev->spriteEnd();

	//	���̕`��
		this->gDev->renderCurtainInDev();
		this->gDev->renderEnd();

		return	this->returnFlag;
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------