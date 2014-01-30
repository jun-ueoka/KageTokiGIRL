//***********************************************************************************
//!	�t�@�C����		Class_SceneMovieBase.h
//
//	�@�\			���[�r�[�X�^�b�N���Ǘ�����N���X�̒�`
//	�@�\�T�v		�f���⃀�[�r�[�Ɏg����N���X
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2008/12/01	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_Scene.h"					//!< �V�[���N���X
#include	"Class_GraphicDevice.h"			//!< �O���t�B�b�N�f�o�C�X
#include	"Class_FontBase.h"				//!< �t�H���g
#include	"Class_Direction.h"				//!< DirectX���o�N���X
#include	"Class_Sound_OggVorbis.h"		//!< ogg���C�u����(�T�E���h�𔭐��������)

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	#define	PATH_MOVIEFILE		"..//Resources//Data//MovieBase//MovieFile%d.mdat"
	#define	CINEMASCOPE_TOP		50		//!	�V�l�}�X�R�[�v�̏�ӕ�
	#define	CINEMASCOPE_BOTTOM	80		//!	�V�l�}�X�R�[�v�̉��ӕ�

	//----------------
	//!	���[�r�[�������x�t���O
	enum	MOVIE_SPEED
	{
		eMSPEED_STOP		= 0,
		eMSPEED_NORMAL		= 1,
		eMSPEED_3DIAMETER	= 3,
		eMSPEED_5DIAMETER	= 5,
		eMSPEED_10DIAMETER	= 10,
		eMSPEED_20DIAMETER	= 20,
	};

	//--------------------
	//!	�����f�[�^�̍\����
	struct	MOVIE_TELOP_DATA
	{
		int			iStart;		//!< �J�n�t���[��
		int			iEnd;		//!< �I���t���[��
		FONT_GRASTR	telop;		//!< �`�敶����f�[�^
		SSTATIC*	voice;		//!< �����f�[�^
		BOOL		bVoice;		//!< �����f�[�^�Đ��m�F

		MOVIE_TELOP_DATA(void)
		{
			iStart	= 0;
			iEnd	= 0;
			voice	= NULL;
			bVoice	= FALSE;
		}
		~MOVIE_TELOP_DATA(void)
		{
			SAFE_DELETE(voice);
		}
	};

	//------------------------
	//!	���������f�[�^�̍\����
	struct	MOVIE_PROC_DATA_ONCE
	{
		int	iFrame;				//!< ���������t���[��
		int	iProc;				//!< ���������ԍ�

		MOVIE_PROC_DATA_ONCE(void)
		{
			iFrame	= 0;
			iProc	= 0;
		}
	};

	//--------------------
	//!	�����f�[�^�̍\����
	struct	MOVIE_PROC_DATA
	{
		int	iStart;				//!< �����J�n�t���[��
		int	iEnd;				//!< �����I���t���[��
		int	iProc;				//!< �����ԍ�

		MOVIE_PROC_DATA(void)
		{
			iStart	= 0;
			iEnd	= 0;
			iProc	= 0;
		}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CSCENE_MOVIE_BASE
	//
	//	�@�\			���[�r�[��ՃN���X
	//	�X�V			2008/12/01	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_MOVIE_BASE : public CSCENE
	{
	private:		//-------------------- private --------------------//

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	������
			//==================================================================
				CGRAPHIC_DEVICE*	gDev;				//!< �O���t�B�b�N�f�o�C�X
				C3D_EFFECT_CENTER*	effectCenter;		//!< �G�t�F�N�g�Z���^�[
				CFONT_BASE*			font;				//!< �t�H���g�x�[�X
				DX::CDIRECTION*		direction;			//!< DirectX���o

			//==================================================================
			//	���o����
			//==================================================================
				vector<MOVIE_TELOP_DATA*>				telop;		//!< �����f�[�^
				vector<MOVIE_TELOP_DATA*>::iterator		telopI;		//!< �����f�[�^�C�e���[�^
				vector<MOVIE_PROC_DATA_ONCE>			procOnce;	//!< ���������f�[�^
				vector<MOVIE_PROC_DATA_ONCE>::iterator	procOnceI;	//!< ���������f�[�^�C�e���[�^
				vector<MOVIE_PROC_DATA>					proc;		//!< �����f�[�^
				vector<MOVIE_PROC_DATA>::iterator		procI;		//!< �����f�[�^�C�e���[�^
				MOVIE_SPEED		movieSpeed;							//!< ���[�r�[�������x�t���O
				int				iMovieFrame;						//!< ���[�r�[�����t���[��
				int				iFinishFrame;						//!< ���[�r�[�I���t���[��
				BOOL			bEndMovie;							//!< ���[�r�[�I���m�F

			//==================================================================
			//	������
			//==================================================================
				void	initializeMovie(int inFileNum);				//!< ���[�r�[������
				void	finishalizeMovie(void);						//!< ���[�r�[���

				void	movieStartProc(void);						//!< ���[�r�[�J�n
				void	movieEndProc(void);							//!< ���[�r�[�I������
				void	movieEndCheck(void);						//!< ���[�r�[�I���`�F�b�N

				void	executeDrawTelop(void);						//!< �����̕`�揈��
				void	checkMovieInput(CINPUT* input);				//!< ���[�r�[�������x�̍X�V����
				void	checkProcOnce(void);						//!< ���������f�[�^�̍X�V����
				void	checkProc(void);							//!< �����f�[�^�̍X�V����
				void	checkProcFollowMovieSpeed(void);			//!< ���x�ɏ]���������f�[�^�̍X�V

		virtual	void	executeInstantChengeMovieSpeed(void){}		//!< ���[�r�[�������x�ύX������
		virtual	void	executeProcOnce(int inProc)			{}		//!< �����������s
		virtual	void	executeProc(int inProc)				{}		//!< �������s

	public:			//-------------------- public --------------------//

				CSCENE_MOVIE_BASE(SCENE_FLAG inFlag, SCENE_FLAG inNextFlag, SCENE_SHARE_DATA* inData);
		virtual	~CSCENE_MOVIE_BASE(void);
	};
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------