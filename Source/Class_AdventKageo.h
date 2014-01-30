//***********************************************************************************
//!	�t�@�C����		Class_AdventKageo.h
//
//	�@�\			�A�h�x���`���[�Q�[�����[�h�́u�J�Q�I�v�N���X�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2008/12/09	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_KageTokiAdventCharacter.h"	//!< �A�h�x���`���[�Q�[�����[�h�L�����N�^�[
#include	"Class_KageoBall.h"					//!< �J�Q�I�{�[��
#include	"Class_Sound_OggVorbis.h"			//!< ogg���C�u����(�T�E���h�𔭐��������)

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	��p�}�N��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		MAXFRAME_SHADOW_BALL	15				//!< �V���h�E�{�[�����o��܂ł̃t���[��
	#define		MAXFRAME_SHINE_BALL		15				//!< �V���C���{�[�����o��܂ł̃t���[��

	#define		KAGEO_SHADOW_BALL		MYKEY::eDUST	//!< �Z�u�V���h�E�{�[���v
	#define		KAGEO_SHINE_BALL		MYKEY::eSOFT	//!< �Z�u�V���C���{�[���v

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�J�Q�I�̓��͗L������
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ADVENT_KAGEO_ACTION_LIMIT
	{
		eADKAGEOL_LV1 = 1,								//!< ���x��1(1�{�[�����łĂȂ�)
		eADKAGEOL_LV2,									//!< ���x��2(�V���h�E�{�[�����łĂ�)
		eADKAGEOL_LV3,									//!< ���x��3(�V���C���{�[�����łĂ�)
		eADKAGEOL_LV4,									//!< ���x��4(�������߂���)
		eADKAGEOL_MAX,									//!< ���͗L������̐�(���ł��o����)
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�J�Q�I�̍s���p�^�[��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ADVENT_KAGEO_ACTION
	{
		eADKAGEOA_WAIT,									//!< �ҋ@
		eADKAGEOA_WALK,									//!< ���s
		eADKAGEOA_FLIGHT,								//!< ��s
		eADKAGEOA_SKILL_SHADOW_BALL,					//!< �Z�u�V���h�E�{�[���v
		eADKAGEOA_SKILL_SHINE_BALL,						//!< �Z�u�V���C���{�[���v
		eADKAGEOA_DAMAGE_LV1,							//!< �_���[�WLV1����
		eADKAGEOA_DAMAGE_LV2,							//!< �_���[�WLV2����
		eADKAGEOA_MAX,									//!< �s���̐�
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CADVENT_KAGEO
	//
	//	�@�\			�A�h�x���`���[�Q�[�����[�h�́u�J�Q�I�v�N���X
	//	�X�V			2008/12/09	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CADVENT_KAGEO : public CKAGETOKI_ADVENT_CHARACTER
	{
			//==================================================================
			//	�A�h�x���`���[���[�h�̃J�Q�I��X�t�@�C����
			//==================================================================
				#define	ADVENT_KAGEO				"advent_kageo"
				#define	KAGEO_COMMENT_SHADOWBALL	"kageo_comment_shadowball"
				#define	KAGEO_COMMENT_SHINEBALL		"kageo_comment_shineball"

	private:		//-------------------- private --------------------//

				ADVENT_KAGEO_ACTION_LIMIT	limit;			//!< �J�Q�I�̓��͗L������
				ADVENT_KAGEO_ACTION			action;			//!< �J�Q�I�̏��
				int				iAnimTemp[eADKAGEOA_MAX];	//!< �A�j���[�V�����e���v���[�g
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
				SSTATIC*		sStaticShotShadow;			//!< �V���h�E�{�[�����ˉ�
				SSTATIC*		sStaticShotShine;			//!< �V���C���{�[�����ˉ�

			//==================================================================
			//	�T�E���h����
			//==================================================================
				void			soundStep(void);			//!< �����̔���
				void			soundJumpStart(void);		//!< �W�����v�̔���
				void			soundJumpEnd(void);			//!< ���n��
				void			soundShotShadow(void);		//!< �V���h�E�{�[�����ˉ�
				void			soundShotShine(void);		//!< �V���C���{�[�����ˉ�

			//==================================================================
			//	�u�V���h�E�{�[���v�u�V���C���{�[���v
			//==================================================================
				VEC3			ballAngle;					//!< �{�[���̌���

				CKAGEO_BALL*	originShadowBall;			//!< �I���W�i���u�V���h�E�{�[���v
				float			shadowBallSpeed;			//!< �V���h�E�{�[���̑��x
				int				shadowBallFrame;			//!< �V���h�E�{�[����������܂ł̎���

				CKAGEO_BALL*	originShineBall;			//!< �I���W�i���u�V���C���{�[���v
				float			shineBallSpeed;				//!< �V���C���{�[���̑��x
				int				shineBallFrame;				//!< �V���C���{�[����������܂ł̎���

			//==================================================================
			//	��������̏�������
			//==================================================================
				void	opeWait(CINPUT* input);				//!< �ҋ@��
				void	opeWalk(CINPUT* input);				//!< ���s��
				void	opeFlight(CINPUT* input);			//!< ��s��
				void	opeSkillShadowBall(CINPUT* input);	//!< �Z�u�V���h�E�{�[���v��
				void	opeSkillShineBall(CINPUT* input);	//!< �Z�u�V���C���{�[���v��
				void	opeDamageLv1(CINPUT* input);		//!< �_���[�WLv1������
				void	opeDamageLv2(CINPUT* input);		//!< �_���[�WLv1������

				BOOL	opeSkillCheck(CINPUT* input);		//!< �Z���͔���
				void	opeBallAngle(CINPUT* input);		//!< �{�[���Z�̕�������

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	�Z����
			//==================================================================
				void	skillShadowBall(void);				//!< �V���h�E�{�[��
				void	skillShineBall(void);				//!< �V���C���{�[��

	public:			//-------------------- public --------------------//

				CADVENT_KAGEO(
					CONST DEV	inDev,
					CONST LPSTR	inName     = ADVENT_KAGEO,
					CONST LPSTR	inResource = PATH_SKINMESH,
					CONST LPSTR	inTexture  = PATH_SKINMESHTEX);	//!< �R���X�g���N�^
		virtual	~CADVENT_KAGEO(void);

			//==================================================================
			//	access	���͗L������
			//==================================================================
				int		getLimit(void)	{ return	(int)limit;		}
				void	setLimit(ADVENT_KAGEO_ACTION_LIMIT inLimit)
				{
					if		(inLimit < eADKAGEOL_LV1)	limit = eADKAGEOL_LV1;
					else if	(inLimit > eADKAGEOL_MAX)	limit = eADKAGEOL_MAX;
					else								limit = inLimit;
				}

			//==================================================================
			//	��ԕω�
			//==================================================================
				BOOL	changeAction(ADVENT_KAGEO_ACTION inNewAction);

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