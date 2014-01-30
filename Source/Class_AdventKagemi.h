//***********************************************************************************
//!	�t�@�C����		Class_AdventKagemi.h
//
//	�@�\			�A�h�x���`���[�Q�[�����[�h�́u�J�Q�~�v�N���X�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2008/12/11		<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_KageTokiAdventCharacter.h"			//!< �A�h�x���`���[�Q�[�����[�h�L�����N�^�[
#include	"Class_KagemiBall.h"						//!< �J�Q�~�{�[��
#include	"Class_Sound_OggVorbis.h"					//!< ogg���C�u����(�T�E���h�𔭐��������)

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	��p�}�N��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		MAXFRAME_ENERGY_BALL			30		//!< �G�i�W�[�{�[�����o��܂ł̃t���[��
	#define		MAXFRAME_ENERGY_BALL_OPERATION	330		//!< �G�i�W�[�{�[���𑀍�o����t���[��
	#define		MAXFRAME_ENERGY_SHOOT			100		//!< �G�i�W�[�V���[�g���o��܂ł̃t���[��

	#define		KAGEMI_ENERGY_BALL		MYKEY::eSOFT	//!< �Z�u�G�i�W�[�{�[���v
	#define		KAGEMI_ENERGY_SHOOT		MYKEY::eDUST	//!< �Z�u�G�i�W�[�V���[�g�v

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�J�Q�~�̓��͗L������
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ADVENT_KAGEMI_ACTION_LIMIT
	{
		eADKAGEMIL_LV1 = 1,								//!< ���x��1(�{�[�����łĂȂ�)
		eADKAGEMIL_LV2,									//!< ���x��2(�G�i�W�[�{�[�����łĂ�)
		eADKAGEMIL_LV3,									//!< ���x��3(�G�i�W�[�V���[�g���łĂ�)
		eADKAGEMIL_MAX,									//!< ���͗L������̐�(���ł��o����)
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�J�Q�~�̍s���p�^�[��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ADVENT_KAGEMI_ACTION
	{
		eADKAGEMIA_WAIT,								//!< �ҋ@
		eADKAGEMIA_WALK,								//!< ���s
		eADKAGEMIA_FLIGHT,								//!< ��s
		eADKAGEMIA_SKILL_ENERGY_BALL_SHOT,				//!< �Z�u�G�i�W�[�{�[�����ˁv
		eADKAGEMIA_SKILL_ENERGY_BALL_OPERATION,			//!< �Z�u�G�i�W�[�{�[������v
		eADKAGEMIA_SKILL_ENERGY_SHOOT,					//!< �Z�u�G�i�W�[�V���[�g�v
		eADKAGEMIA_DAMAGE_LV1,							//!< �_���[�WLV1����
		eADKAGEMIA_DAMAGE_LV2,							//!< �_���[�WLV2����
		eADKAGEMIA_MAX,									//!< �s���̐�
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CADVENT_KAGEMI
	//
	//	�@�\			�A�h�x���`���[�Q�[�����[�h�́u�J�Q�~�v�N���X
	//	�X�V			2008/12/11		<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CADVENT_KAGEMI : public CKAGETOKI_ADVENT_CHARACTER
	{
			//==================================================================
			//	�A�h�x���`���[���[�h�̃J�Q�~��X�t�@�C����
			//==================================================================
				#define	ADVENT_KAGEMI				"advent_kagemi"
				#define	KAGEMI_COMMENT_ENERGYBALL	"kagemi_comment_energyball"
				#define	KAGEMI_COMMENT_ENERGYSHOOT	"kagemi_comment_energyshoot"

	private:		//-------------------- private --------------------//

				ADVENT_KAGEMI_ACTION_LIMIT	limit;			//!< �J�Q�~�̓��͗L������
				ADVENT_KAGEMI_ACTION		action;			//!< �J�Q�~�̏��
				int				iAnimTemp[eADKAGEMIA_MAX];	//!< �A�j���[�V�����e���v���[�g
				int				iHitFrame;					//!< �Փ˓_�Ńt���[��
				BOOL			bJunpInput;					//!< ���ē��͊m�F
				CTEXTURE*		texArrow;					//!< ���

			//==================================================================
			//	�T�E���h�f�[�^
			//==================================================================
				int				stepCnt;					//!< �X�e�b�v�J�E���^
				SSTATIC*		sStaticStep;				//!< ����
				SSTATIC*		sStaticJumpStart;			//!< �W�����v��
				SSTATIC*		sStaticJumpEnd;				//!< ���n��
				SSTATIC*		sStaticShotEnergy;			//!< �G�i�W�[�{�[�����ˉ�

			//==================================================================
			//	�T�E���h����
			//==================================================================
				void			soundStep(void);			//!< �����̔���
				void			soundJumpStart(void);		//!< �W�����v�̔���
				void			soundJumpEnd(void);			//!< ���n��
				void			soundShotEnergy(void);		//!< �G�i�W�[�{�[�����ˉ�

			//==================================================================
			//	�u�G�i�W�[�{�[���v�u�G�i�W�[�V���[�g�v
			//==================================================================
				VEC3			ballAngle;					//!< �{�[���̌���

				CKAGEMI_BALL*	originEnergyBall;			//!< �I���W�i���u�G�i�W�[�{�[���v
				CKAGEMI_BALL*	opeEnergyBall;				//!< ���쒆�G�i�W�[�{�[��
				float			energyBallSpeed;			//!< �G�i�W�[�{�[���̑��x
				int				energyBallFrame;			//!< �G�i�W�[�{�[����������܂ł̎���
				float			energyShootSpeed;			//!< �G�i�W�[�V���[�g�̑��x
				int				energyShootFrame;			//!< �G�i�W�[�V���[�g��������܂ł̎���

			//==================================================================
			//	��������̏�������
			//==================================================================
				void	opeWait(CINPUT* input);						//!< �ҋ@��
				void	opeWalk(CINPUT* input);						//!< ���s��
				void	opeFlight(CINPUT* input);					//!< ��s��
				void	opeSkillEnergyBallShot(CINPUT* input);		//!< �Z�u�G�i�W�[�{�[�����ˁv��
				void	opeSkillEnergyBallOperation(CINPUT* input);	//!< �Z�u�G�i�W�[�{�[������v��
				void	opeSkillEnergyShoot(CINPUT* input);			//!< �Z�u�G�i�W�[�V���[�g�v��
				void	opeDamageLv1(CINPUT* input);				//!< �_���[�WLv1������
				void	opeDamageLv2(CINPUT* input);				//!< �_���[�WLv2������

				BOOL	opeSkillCheck(CINPUT* input);				//!< �Z���͔���
				void	opeBallAngle(CINPUT* input);				//!< �{�[���Z�̕�������

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	�Z����
			//==================================================================
				void	skillEnergyBall(void);						//!< �G�i�W�[�{�[��
				void	skillEnergyShoot(void);						//!< �G�i�W�[�V���[�g

	public:			//-------------------- public --------------------//

				CADVENT_KAGEMI(
					CONST DEV	inDev,
					CONST LPSTR	inName     = ADVENT_KAGEMI,
					CONST LPSTR	inResource = PATH_SKINMESH,
					CONST LPSTR	inTexture  = PATH_SKINMESHTEX);		//!< �R���X�g���N�^
		virtual	~CADVENT_KAGEMI(void);

			//==================================================================
			//	access	���͗L������
			//==================================================================
				int		getLimit(void)	{ return	(int)limit;	}
				void	setLimit(ADVENT_KAGEMI_ACTION_LIMIT inLimit)
				{
					if		(inLimit < eADKAGEOL_LV1)	limit = eADKAGEMIL_LV1;
					else if	(inLimit > eADKAGEOL_MAX)	limit = eADKAGEMIL_MAX;
					else								limit = inLimit;
				}

			//==================================================================
			//	��ԕω�
			//==================================================================
				BOOL	changeAction(ADVENT_KAGEMI_ACTION inNewAction);
				ADVENT_KAGEMI_ACTION	getAction(void)const			{ return	action;		}
				void	safeResetChangeToWait(void);			//!< ���S�ɑҋ@�Ɉڍs����

			//==================================================================
			//	access	���쒆�̃G�i�W�[�{�[��
			//==================================================================
				CKAGEMI_BALL*	getOpeEnergyBall(void)					{ return	opeEnergyBall;	}

			//==================================================================
			//	���A�N�V����(�����ȃA�N�V����������T�u�N���X�ŃI�[�o�[���C�h)
			//==================================================================
				void	reactionMessage(REACTION_MESSAGE inMessage);

			//==================================================================
			//	��{����
			//==================================================================
				void	operation(CINPUT* input);

			//==================================================================
			//	�`��
			//==================================================================
				BOOL	draw(CONST DEV inDev);
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------