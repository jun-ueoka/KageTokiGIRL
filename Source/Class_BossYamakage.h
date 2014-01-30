//***********************************************************************************
//!	�t�@�C����		Class_BossYamakage.h
//
//	�@�\			�{�X�L�����N�^�[�N���X�u���}�J�Q�v�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2009/02/16	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_KageTokiAdventBoss.h"	//!< �A�h�x���`���[�Q�[���{�X�x�[�X
#include	"Class_Sound_OggVorbis.h"		//!< ogg���C�u����(�T�E���h�𔭐��������)
#include	"Class_YamakageWeapon.h"		//!< ���}�J�Q�̕���

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	���}�J�Q�̍s���p�^�[��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	BOSSYAMAKAGE_ACTION
	{
		eBYAMAKAGEA_APPEAR,						//!< �o����
		eBYAMAKAGEA_APPEAR_WAIT,				//!< �o���ҋ@��
		eBYAMAKAGEA_WAIT,						//!< �ҋ@��
		eBYAMAKAGEA_DAMAGE,						//!< �_���[�W������
		eBYAMAKAGEA_WEAPON_TAKEOUT,				//!< ����̎��o��
		eBYAMAKAGEA_WEAPON_ATTACK,				//!< ����̍U��
		eBYAMAKAGEA_ATTACK_ANDMOVE,				//!< �U�����Ȃ���ړ�
		eBYAMAKAGEA_DISAPPEAR_WAIT,				//!< ���őҋ@��
		eBYAMAKAGEA_DISAPPEAR,					//!< ���Ŏ�
		eBYAMAKAGEA_MAX,						//!< �s���̍ő吔
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CBOSS_YAMAKAGE
	//
	//	�@�\			�A�h�x���`���[�Q�[�����[�h�́u���}�J�Q�v�N���X
	//	�X�V			2009/02/16	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CBOSS_YAMAKAGE : public CKAGETOKI_ADVENT_BOSS
	{
			//==================================================================
			//	�{�X�u���}�J�Q�v�}�N��
			//==================================================================
				#define	NAME_YAMAKAGE			"boss_yamakage"
				#define	YAMAKAGE_MAXLIFE		12

	private:		//-------------------- private --------------------//

				BOSSYAMAKAGE_ACTION	action;					//!< ���}�J�Q�̏��
				int					iAnimTemp[eBYAMAKAGEA_MAX];	//!< �A�j���[�V�����e���v���[�g
				int					iHitFrame;					//!< �Փ˓_�Ńt���[��

				CYAMAKAGE_WEAPON*	yamakageWeaponA;		//!< ���}�J�Q����A
				CYAMAKAGE_WEAPON*	yamakageWeaponB;		//!< ���}�J�Q����B
				CYAMAKAGE_WEAPON*	yamakageWeaponC;		//!< ���}�J�Q����C
				CYAMAKAGE_WEAPON*	yamakageWeaponD;		//!< ���}�J�Q����D
				int					iAttackCnt;				//!< �U���񐔐���p
				int					iMoveCnt;				//!< �ړ�����p

			//==================================================================
			//	�T�E���h�f�[�^
			//==================================================================
				SSTATIC*		sStaticJumpStart;			//!< �W�����v��
				SSTATIC*		sStaticJumpEnd;				//!< ���n��

				void	soundJumpStart(void);				//!< �W�����v�̔���
				void	soundJumpEnd(void);					//!< ���n��

			//==================================================================
			//	��������̏�������
			//==================================================================
				void	opeAppear(void);					//!< �o����
				void	opeAppearWait(void);				//!< �o���ҋ@��
				void	opeWait(void);						//!< �ҋ@��
				void	opeDamage(void);					//!< �_���[�W������
				void	opeWeaponTakeOut(void);				//!< ������o����
				void	opeWeaponAttack(void);				//!< ����U��
				void	opeAttackAndMove(void);				//!< �U�����Ȃ���ړ�
				void	opeDisappearWait(void);				//!< ���őҋ@��
				void	opeDisappear(void);					//!< ���Ŏ�

	protected:		//-------------------- protected --------------------//

	public:			//-------------------- public --------------------//

				CBOSS_YAMAKAGE(
					CONST DEV	inDev,
					CONST LPSTR	inName     = NAME_YAMAKAGE,
					CONST LPSTR	inResource = PATH_SKINMESH,
					CONST LPSTR	inTexture  = PATH_SKINMESHTEX);	//!< �R���X�g���N�^
		virtual	~CBOSS_YAMAKAGE(void);

			//==================================================================
			//	��ԕω�
			//==================================================================
				BOOL	changeAction(BOSSYAMAKAGE_ACTION inNewAction);

			//==================================================================
			//	���A�N�V����(�����ȃA�N�V����������T�u�N���X�ŃI�[�o�[���C�h)
			//==================================================================
				void	reactionMessage(REACTION_MESSAGE inMessage);

			//==================================================================
			//	���Z�b�g
			//==================================================================
				void	resetParameter(void);

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