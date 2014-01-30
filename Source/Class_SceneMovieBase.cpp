//***********************************************************************************
//!	�t�@�C����		Class_SceneMovieBase.h
//
//	�@�\			���[�r�[�X�^�b�N���Ǘ�����N���X�̒�`
//	�@�\�T�v		�f���⃀�[�r�[�Ɏg����N���X
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2008/12/01	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneMovieBase.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE_BASE::�R���X�g���N�^
	//
	//	����		inFlag			���݂̃V�[���t���O
	//				inNextFlag		���̃V�[���t���O
	//				inData			�V�[�����L�f�[�^
	//	�X�V		2008/12/02		<�V�K>
	//================================================================================
	CSCENE_MOVIE_BASE::CSCENE_MOVIE_BASE(SCENE_FLAG inFlag,
										 SCENE_FLAG inNextFlag,
										 SCENE_SHARE_DATA* inData)	:
	CSCENE(inFlag, inNextFlag, inData)
	{
	//	�������̏�����
		this->gDev			= CGRAPHIC_DEVICE::getInstance();
		this->effectCenter	= C3D_EFFECT_CENTER::getInstance();
		this->effectCenter->finishalize();
		this->font			= CFONT_BASE::getInstance();
		this->direction		= new DX::CDIRECTION(gDev->getDevice(),
										 gDev->getScreenSizeX(),
										 gDev->getScreenSizeY());
	//	�t�H���g�̐ݒ�
		this->font->createFont(gDev->getDevice(), eFONT_PTN2, "HG�n�p�p�߯�ߑ�", 20, 10, 1, 0);
	//	�V�l�}�X�R�[�v�̐ݒ�
		this->direction->initCinemaScopePaintOut(
			CINEMASCOPE_TOP, CINEMASCOPE_BOTTOM, 0.1f, 0xFF000000); 

	//	���o�����̏�����
		this->movieSpeed	= eMSPEED_NORMAL;
		this->iMovieFrame	= 0;
		this->iFinishFrame	= 0;
		this->bEndMovie		= FALSE;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE_BASE::�f�X�g���N�^
	//
	//	�X�V		2008/12/02		<�V�K>
	//================================================================================
	CSCENE_MOVIE_BASE::~CSCENE_MOVIE_BASE(void)
	{
	//	���
		this->finishalizeMovie();
	
	//	�������̉��
		this->effectCenter->finishalize();
		this->font	= NULL;
		CFONT_BASE::releaseInstance();
		SAFE_DELETE(direction);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE_BASE::initializeMovie
	//
	//	�@�\		���[�r�[�̏���
	//	����		inFileNum		�O���t�@�C���ԍ�
	//	�X�V		2009/01/26		<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::initializeMovie(int inFileNum)
	{
	//	�ǂݍ��ݏ���
		FILE*	fp;								//!< �t�@�C���|�C���^�[
		char	szString[MAX_PATH]		= "";	//!< ���l������

	//	�e�L�X�g���𐶐�
		sprintf_s(szString, sizeof(szString), PATH_MOVIEFILE, inFileNum);
	//	�t�@�C�����J��
		fopen_s(&fp, szString, "r");

	//	�t�@�C�����J���Ȃ�������
		if(	!fp)
		{
		//	�ǂݍ��݂��s�킸�A������Ԃ�
			return;
		}

	//	�������
		finishalizeMovie();

	//	�����f�[�^��ǂݍ���
		MOVIE_TELOP_DATA*	newTelop = NULL;
		int	inStart  = 0;
		int	inEnd    = 0;
		int	r = 0, g = 0, b = 0, a = 0;

		do
		{
		//	�J�n�t���[���ƏI���t���[����ǂݍ���
			fscanf_s(fp, "%d, %d, ", &inStart, &inEnd);

		//	�J�n�t���[���ƏI���t���[���̒l����v������A�����f�[�^�̓ǂݍ��݂��I��
			if(	inStart == inEnd)	break;

		//	�����܂ŗ�����A�V���Ȏ����f�[�^�𐶐�
			newTelop = new MOVIE_TELOP_DATA();
			newTelop->iStart = inStart;
			newTelop->iEnd   = inEnd;

		//	�`�敶����f�[�^��ǂݍ��݁A�ݒ肷��
			fscanf_s(fp, "%s ", szString, sizeof(szString));
			UT::uf_StrCpy1(newTelop->telop.szWord, (int)sizeof(newTelop->telop.szWord),
						   szString, (int)sizeof(szString));
			fscanf_s(fp, "%d, %d, %d, %d, ", &r, &g, &b, &a);
			newTelop->telop.font				= eFONT_PTN2;
			newTelop->telop.color				= D3DCOLOR_RGBA(r,g,b,a);
			SetRect(&newTelop->telop.rcPos,
				0, gDev->getScreenSizeY() - CINEMASCOPE_BOTTOM,
				gDev->getScreenSizeX(), gDev->getScreenSizeY());
			newTelop->telop.fPriority			= 0.05f;
			newTelop->telop.iTextCnt			= 0;
			newTelop->telop.dFormat				= DT_CENTER;
			newTelop->telop.iTextGraduallySpeed	= 1;

		//	�T�E���h�f�[�^��ǂݍ��݁A�ݒ肷��
			fscanf_s(fp, "%s ", szString, sizeof(szString));
			if(	strcmp(szString, "S_NONE"))		//!< �_�~�[�T�E���h���ȊO�Ȃ琶��
			{
				newTelop->voice	= new SSTATIC(szString);
			}
			else								//!< ��L�ȊO�Ȃ�NULL��
			{
				newTelop->voice	= NULL;
			}

		//	�������ꂽ�f�[�^��o�^
			telop.push_back(newTelop);
		}
		while(1);

	//	���������f�[�^��ǂݍ���
		MOVIE_PROC_DATA_ONCE	newProcOnce;
		do
		{
		//	���������t���[����ǂݍ���
			fscanf_s(fp, "%d, ", &newProcOnce.iFrame);

		//	�ǂݍ��񂾑��������t���[���̒l�����̐��Ȃ�A���������f�[�^�̓ǂݍ��݂��I��
			if(	newProcOnce.iFrame < 0)	break;

		//	�c���ǂݍ���
			fscanf_s(fp, "%d, ", &newProcOnce.iProc);

		//	�������ꂽ�f�[�^��o�^
			procOnce.push_back(newProcOnce);
		}
		while(1);

	//	�����f�[�^��ǂݍ���
		MOVIE_PROC_DATA		newProc;
		do
		{
		//	�����J�n�t���[����ǂݍ���
			fscanf_s(fp, "%d, ", &newProc.iStart);

		//	�ǂݍ��񂾏����J�n�t���[���̒l�����̐��Ȃ�A�����J�n�f�[�^�̓ǂݍ��݂��I��
			if(	newProc.iStart < 0)	break;

		//	�c���ǂݍ���
			fscanf_s(fp, "%d, ", &newProc.iEnd);
			fscanf_s(fp, "%d, ", &newProc.iProc);

		//	�������ꂽ�f�[�^��o�^
			proc.push_back(newProc);
		}
		while(1);

	//	���[�r�[�I���t���[����ǂݍ���
		fscanf_s(fp, "%d, ", &iFinishFrame);

	//	�t�@�C�������
		fclose(fp);

	//	�e�f�[�^�̃C�e���[�^���m��
		telopI		= telop.begin();
		procOnceI	= procOnce.begin();
		procI		= proc.begin();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE_BASE::finishalizeMovie
	//
	//	�@�\		���[�r�[�̉��
	//	�X�V		2009/01/26		<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::finishalizeMovie(void)
	{
	//	���o�����̉��
		SAFE_VECTOR_DELETE(telop, MOVIE_TELOP_DATA*);
		procOnce.clear();
		proc.clear();

		iMovieFrame		= 0;
		iFinishFrame	= 0;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE_BASE::movieStartProc
	//
	//	�@�\		���[�r�[�̊J�n
	//	�X�V		2009/02/07		<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::movieStartProc(void)
	{
	//	�������ł���
		this->gDev->setCurtainColor(0, 0, 0);
		this->gDev->curtainMotion(FALSE, 10);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE_BASE::movieEndProc
	//
	//	�@�\		���[�r�[�I������
	//	�X�V		2009/02/07		<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::movieEndProc(void)
	{
		if(	!this->bEndMovie)
		{
		//	�����o������
			this->gDev->curtainMotion(TRUE, 5);
		//	���[�r�[�I���m�F��L��
			this->bEndMovie	= TRUE;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE_BASE::movieEndCheck
	//
	//	�@�\		���[�r�[�I���`�F�b�N
	//	�X�V		2009/02/07		<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::movieEndCheck(void)
	{
	//	���[�r�[�I���m�F���L���ŁA
	//	���̃A���t�@���ő�Ȃ�A���̏�����
		if(	this->bEndMovie	&&
			(this->gDev->getCurtainColorAlpha() >= 255))
		{
			this->returnFlag	= this->nextFlag;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE_BASE::executeDrawTelop
	//
	//	�@�\		�����̕`�揈��
	//	�X�V		2009/01/26		<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::executeDrawTelop(void)
	{
#ifdef	_DEBUG
		char	szString[20]	= "";
		sprintf_s(szString, sizeof(szString), "%d flame", this->iMovieFrame);
		font->drawText(gDev->getSprite(), eFONT_PTN2, szString, 0, 0, 0xFFFFFFFF);
#else
		char	szString1[30]	= "�������� : Ѱ�ް ���خ�";
		char	szString2[30]	= "������   : 3�޲ �Ե��";
		font->drawText(gDev->getSprite(), eFONT_PTN2, szString1, 0,  0, 0xFFFFFFFF);
		font->drawText(gDev->getSprite(), eFONT_PTN2, szString2, 0, 25, 0xFFFFFFFF);
#endif

	//	���݂̎����f�[�^�̃C�e���[�^���I�����w���Ă���Ȃ�A�������Ȃ�
		if(	telopI == telop.end())	return;

	//	���݂̎����f�[�^���`�悳���t���[����ԓ��ɖ������Ă��邩�m�F
		if(	(*telopI)->iStart <= iMovieFrame)
		{
			if(	(*telopI)->iEnd >= iMovieFrame)
			{
			//	���[�r�[�������x�ɂ��킹�āA���X�`�揈�����s��
				LOOP(this->movieSpeed - 1)
				{
					(*telopI)->telop.step();
				}
			//	�������Ă���Ȃ�`��
				font->drawText(gDev->getSprite(), &(*telopI)->telop);
			//	�����f�[�^������A�Đ��ς݂Ŗ����Ȃ�A�Đ�������
				if(	 (*telopI)->voice	&&
					!(*telopI)->bVoice)
				{
					(*telopI)->voice->play(0, NULL);
					(*telopI)->bVoice	= TRUE;
				}
			}
		//	���݂̎����f�[�^���`��I��������
			else
			{
			//	���̎����f�[�^��
				telopI++;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE_BASE::checkMovieInput
	//
	//	�@�\		���������f�[�^�̍X�V����
	//	����		input			���͏��
	//	�X�V		2009/02/15		<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::checkMovieInput(CINPUT* input)
	{
	//	�E����̉��{�^����������Ă���3�{��
		if(	input->checkPress(MYKEY::eMIDDLE))
		{
			if(	this->movieSpeed != eMSPEED_3DIAMETER)
			{
				this->movieSpeed  = eMSPEED_3DIAMETER;
				this->executeInstantChengeMovieSpeed();
			}
		}
	#ifdef	_DEBUG
	//	�E����̍��{�^����������Ă���5�{��
		else
		if(	input->checkPress(MYKEY::eDUST))
		{
			if(	this->movieSpeed != eMSPEED_5DIAMETER)
			{
				this->movieSpeed  = eMSPEED_5DIAMETER;
				this->executeInstantChengeMovieSpeed();
			}
		}
	//	�E����̉E�{�^����������Ă���10�{��
		else
		if(	input->checkPress(MYKEY::eSOFT))
		{
			if(	this->movieSpeed != eMSPEED_10DIAMETER)
			{
				this->movieSpeed  = eMSPEED_10DIAMETER;
				this->executeInstantChengeMovieSpeed();
			}
		}
	//	�E����̏�{�^����������Ă���15�{��
		else
		if(	input->checkPress(MYKEY::eHARD))
		{
			if(	this->movieSpeed != eMSPEED_20DIAMETER)
			{
				this->movieSpeed  = eMSPEED_20DIAMETER;
				this->executeInstantChengeMovieSpeed();
			}
		}
	//	�E����̏�{�^����������Ă���0�{��
		else
		if(	input->checkPress(MYKEY::eSELECT))
		{
			if(	this->movieSpeed != eMSPEED_STOP)
			{
				this->movieSpeed  = eMSPEED_STOP;
				this->executeInstantChengeMovieSpeed();
			}
		}
	#endif
	//	����������ĂȂ���΁A�ʏ푬�x
		else
		{
			if(	this->movieSpeed != eMSPEED_NORMAL)
			{
				this->movieSpeed  = eMSPEED_NORMAL;
				this->executeInstantChengeMovieSpeed();
			}
		}
	//	�X�^�[�g��������Ă�����A���[�r�[�I��������
		if(	input->checkPressOnce(MYKEY::eSTART))
		{
			this->movieEndProc();
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE_BASE::checkProcOnce
	//
	//	�@�\		���������f�[�^�̍X�V����
	//	�X�V		2009/01/26		<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::checkProcOnce(void)
	{
	//	���݂̑��������f�[�^�̃C�e���[�^���I�����w���Ă���Ȃ�A�������Ȃ�
		if(	procOnceI == procOnce.end())	return;

	//	���݂̑��������f�[�^�����s�����t���[����ԓ��ɖ������Ă��邩�m�F
		if(	procOnceI->iFrame <= iMovieFrame)
		{
		//	�����������s
			executeProcOnce(procOnceI->iProc);
		//	���̑��������f�[�^��
			procOnceI++;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE_BASE::checkProc
	//
	//	�@�\		�����f�[�^�̍X�V����
	//	�X�V		2009/01/26		<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::checkProc(void)
	{
	//	���݂̏����f�[�^�̃C�e���[�^���I�����w���Ă���Ȃ�A�������Ȃ�
		if(	procI == proc.end())	return;

	//	���݂̏����f�[�^�����s�����t���[����ԓ��ɖ������Ă��邩�m�F
		if(	procI->iStart <= iMovieFrame)
		{
			if(	procI->iEnd >= iMovieFrame)
			{
			//	�������Ă���Ȃ���s
				executeProc(procI->iProc);
			}
		//	���݂̏����f�[�^�����s�I��������
			if(	procI->iEnd <= iMovieFrame)
			{
			//	���̎����f�[�^��
				procI++;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_MOVIE_BASE::checkProcFollowMovieSpeed
	//
	//	�@�\		���x�ɏ]���������f�[�^�̍X�V
	//	�X�V		2009/02/15		<�V�K>
	//================================================================================
	void
	CSCENE_MOVIE_BASE::checkProcFollowMovieSpeed(void)
	{
	//	���[�r�[�������x�������[�v
		LOOP(this->movieSpeed)
		{
			this->checkProcOnce();
			this->checkProc();
			this->iMovieFrame++;
			if(	i > 0){	this->gDev->curtainMotionExecute();	}
		}
	//	���݂̃t���[�����I���t���[���ɓ��B������A���[�r�[�I��������
		if(	iMovieFrame >= iFinishFrame)
		{
			this->movieEndProc();
		}
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------