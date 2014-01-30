//***********************************************************************************
//!	�t�@�C����		Class_AdventPlayer.h
//
//	�@�\			�A�h�x���`���[�Q�[�����[�h�̃v���C���[�N���X�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2008/12/11	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_AdventKageo.h"		//!< �A�h�x���`���[�Q�[�����[�h�u�J�Q�I�v
#include	"Class_AdventKagemi.h"		//!< �A�h�x���`���[�Q�[�����[�h�u�J�Q�~�v
#include	"Class_3DObjectCamera.h"	//!< 3D�I�u�W�F�N�g�J����
#include	"Class_Scene.h"				//!< �V�[��

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�A�h�x���`���[�v���C���[�I���t���O
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ADVENT_CHOICE_CHARACTER
	{
		eADCC_KAGEO = 0,							//!< �J�Q�I
		eADCC_KAGEMI,								//!< �J�Q�~
		eADCC_MAX,									//!< �ő吔
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�v���C���[�G�t�F�N�g�t���O
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ADVENT_PLAYER_EFFECT
	{
		eADPEFF_ACHE,								//!< �ɂ�
		eADPEFF_ANGER,								//!< �{��
		eADPEFF_HASTE,								//!< �ł�
		eADPEFF_DATHE,								//!< ��
		eADPEFF_MAX,								//!< �ő吔
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CADVENT_PLAYER
	//
	//	�@�\			�A�h�x���`���[�Q�[�����[�h�̃v���C���[�N���X
	//	�X�V			2008/12/11	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CADVENT_PLAYER
	{
			//==================================================================
			//	�Q�[���C���^�[�t�F�[�X�֘A
			//==================================================================
				#define	NAME_INTERFACE			"game_interface_player"

			//==================================================================
			//	�����o���֘A
			//==================================================================
				#define	KAGEO_COMMENT_ACHE		"kageo_comment_ache"
				#define	KAGEO_COMMENT_ANGER		"kageo_comment_anger"
				#define	KAGEO_COMMENT_HASTE		"kageo_comment_haste"
				#define	KAGEO_COMMENT_DATHE		"kageo_comment_uwaaaa"
				#define	KAGEMI_COMMENT_ACHE		"kagemi_comment_ache"
				#define	KAGEMI_COMMENT_ANGER	"kagemi_comment_anger"
				#define	KAGEMI_COMMENT_HASTE	"kagemi_comment_haste"
				#define	KAGEMI_COMMENT_DATHE	"kagemi_comment_kyaaaa"

	private:		//-------------------- private --------------------//

			//==================================================================
			//	�A�h�x���`���[�J�Q�I&�J�Q�~
			//==================================================================
				CADVENT_KAGEO*		adventKageo;
				CADVENT_KAGEMI*		adventKagemi;

			//==================================================================
			//	�Q�[���C���^�[�t�F�[�X�֘A
			//==================================================================
				CTEXTURE*			texKageoLife;		//!< �J�Q�I���C�t�̓y��
				CTEXTURE*			texKagemiLife;		//!< �J�Q�~���C�t�̓y��
				BOOL				bDamageCheck;		//!< �_���[�W����m�F
				int					iMaxLife;			//!< �ő�̗�
				int					iLife;				//!< �c��̗�
				VEC3				keepStartLoc;		//!< �����ʒu

			//==================================================================
			//	�O���|�C���^�[
			//==================================================================
				CKAGETOKI_ADVENT_CHARACTER**	target;
				C3D_OBJECT_CAMERA*				camera;
				SCENE_SHARE_DATA*				share;

			//==================================================================
			//	�t���O�Q
			//==================================================================
				ADVENT_CHOICE_CHARACTER			choiceChara;
				BOOL							bChoiceCharaChange;

	public:			//-------------------- public --------------------//

				CADVENT_PLAYER(CONST DEV inDev, CKAGETOKI_ADVENT_CHARACTER** inTarget,
							   SCENE_SHARE_DATA* inShare);
		virtual	~CADVENT_PLAYER(void);

			//==================================================================
			//	�O���|�C���^�[�֘A
			//==================================================================
				void	setCameraPointer(C3D_OBJECT_CAMERA* inCamera)	{ camera = inCamera;	}
				void	setViewG(C3D_OBJECT_GROUP* inGroup);
				void	setStageG(C3D_OBJECT_GROUP* inGroup);
				void	setEnemyG(C3D_OBJECT_GROUP* inGroup);

			//==================================================================
			//	�L�����N�^�[����֘A
			//==================================================================
				void	setAllLoc(VEC3* inLoc);
				void	setAllAngle(VEC3* inAngle);
				void	setAllVisible(BOOL inVisible);
				void	setAllLimit(int inLimit);
				void	resetAllWay(void);
				void	resetPlayer(VEC3* inStartLoc = NULL);

				ADVENT_CHOICE_CHARACTER	getChoiceCharacter(void)const	{ return	choiceChara;			}
				void	setChoiceCharacter(ADVENT_CHOICE_CHARACTER inChara);
				void	setChoiceCharaChangeBoolen(BOOL inBoolen)		{ bChoiceCharaChange = inBoolen;	}

			//==================================================================
			//	access	�L�����N�^�[
			//==================================================================
				CKAGETOKI_ADVENT_CHARACTER*	getPlayChara(void);
				CADVENT_KAGEO*				getKageo(void)const		{ return	adventKageo;	}
				CADVENT_KAGEMI*				getKagemi(void)const	{ return	adventKagemi;	}

			//==================================================================
			//	access	�_���[�W����m�F
			//==================================================================
				BOOL	getDamageCheck(void)const		{ return	bDamageCheck;				}
				void	setDamageCheck(BOOL inBoolen)	{ bDamageCheck	= inBoolen;				}

			//==================================================================
			//	�v���C���[�G�t�F�N�g����
			//==================================================================
				void	occuPlayerEffect(ADVENT_PLAYER_EFFECT inEffectFlag);

			//==================================================================
			//	�L�����N�^�[���S�m�F
			//==================================================================
				BOOL	getCharacterOver(void)	{ return (iLife <= 0) ? TRUE : FALSE;	}

			//==================================================================
			//	�v���C���[����
			//==================================================================
				void	operation(CINPUT* input);

			//==================================================================
			//	�v���C���[�C���^�[�t�F�[�X�̕`��
			//==================================================================
				void	drawInterface(SPRITE inSprite);
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------