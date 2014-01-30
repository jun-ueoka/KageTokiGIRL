//***********************************************************************************
//!	�t�@�C����		Class_EnemyShadollArchery.h
//
//	�@�\			�G�L�����N�^�[�N���X�u�V���h�[���A�[�`�F���[�v�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2009/02/20	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_KageTokiAdventEnemy.h"		//!< �A�h�x���`���[�Q�[�����[�h�G�L�����N�^�[
#include	"Class_Sound_OggVorbis.h"			//!< ogg���C�u����(�T�E���h�𔭐��������)

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�V���h�[���A�[�`�F���[�̍s���p�^�[��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ENEMYSHADOLLARCHERY_ACTION
	{
		eESHADOLLARCHERYA_WAIT,							//!< �ҋ@��
		eESHADOLLARCHERYA_ARROWINIT,					//!< ��̏���
		eESHADOLLARCHERYA_ARROW,						//!< ����˂�
		eESHADOLLARCHERYA_MAX,							//!< �s���̍ő吔
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CENEMY_SHADOLL_ARCHERY
	//
	//	�@�\			�A�h�x���`���[�Q�[�����[�h�́u�V���h�[���A�[�`�F���[�v�N���X
	//	�X�V			2009/02/20	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CENEMY_SHADOLL_ARCHERY : public CKAGETOKI_ADVENT_ENEMY
	{
			//==================================================================
			//	�G�u�V���h�[���A�[�`�F���[�v�}�N��
			//==================================================================
				#define	NAME_SHADOLLARCHERY		"enemy_shadoll_archery"

	private:		//-------------------- private --------------------//

				ENEMYSHADOLLARCHERY_ACTION	action;					//!< �V���h�[���A�[�`�F���[�̏��
				int				iAnimTemp[eESHADOLLARCHERYA_MAX];	//!< �A�j���[�V�����e���v���[�g

				int				iHitFrame;					//!< �Փ˓_�Ńt���[��

			//==================================================================
			//	��̃I���W�i��
			//==================================================================
				CENEMY_BALL*			originArrow;		//!< ��̃I���W�i��
				SSTATIC*				ssArrowShot;		//!< ��ˉ�

			//==================================================================
			//	�T�E���h�f�[�^
			//==================================================================

			//==================================================================
			//	��������̏�������
			//==================================================================
				void	opeWait(void);						//!< �ҋ@��
				void	opeArrowInit(void);					//!< ��̏���
				void	opeArrow(void);						//!< ����˂�

				void	skillArrowShot(void);				//!< �A���[�V���b�g

	public:			//-------------------- public --------------------//

				CENEMY_SHADOLL_ARCHERY(
					CONST DEV	inDev,
					CONST LPSTR	inName     = NAME_SHADOLLARCHERY,
					CONST LPSTR	inResource = PATH_SKINMESH,
					CONST LPSTR	inTexture  = PATH_SKINMESHTEX);	//!< �R���X�g���N�^
		virtual	~CENEMY_SHADOLL_ARCHERY(void);

			//==================================================================
			//	��ԕω�
			//==================================================================
				BOOL	changeAction(ENEMYSHADOLLARCHERY_ACTION inNewAction);

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