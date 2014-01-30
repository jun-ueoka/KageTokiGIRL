//***********************************************************************************
//!	�t�@�C����		Class_AdventEnemyManager.h
//
//	�@�\			�A�h�x���`���[�Q�[���̓G�L�����N�^�[���Ǘ�����N���X�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2009/02/16	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_EnemyShadoll.h"				//!< �A�h�x���`���[�G�l�~�[�u�V���h�[���v
#include	"Class_EnemyShadollSword.h"			//!< �A�h�x���`���[�G�l�~�[�u�V���h�[���\�[�h�v
#include	"Class_EnemyShadog.h"				//!< �A�h�x���`���[�G�l�~�[�u�V���h�b�O�v
#include	"Class_EnemyShadollArchery.h"		//!< �A�h�x���`���[�G�l�~�[�u�V���h�[���A�[�`�F���[�v

//-----------------------------------------------------------------------
//!	�Z�[�u���\�[�X�̖��O���
//-----------------------------------------------------------------------
namespace EM
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�G�z�u�}�b�v�t�@�C���ւ̃p�X
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_ENEMYMAP		"..//Resources//Data//Enemy//EnemyMap%d.dat"

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�G�̎��ʃt���O
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ENEMY_KIND
	{
		eEKIND_SHADOLL,			//!< �V���h�[��					//!< SHADOLL___
		eEKIND_SHADOLL_SWORD,	//!< �V���h�[���\�[�h			//!< SHADOLL_SW
		eEKIND_SHADOG,			//!< �V���h�b�O					//!< SHADOG____
		eEKIND_SHADOLL_ARCHERY,	//!< �V���h�[���A�[�`�F���[		//!< SHADOLL_AR
		eEKIND_MAX,				//!< �G�̍ő吔
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�ǂݍ��񂾓G�̃f�[�^
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	ENEMY_DATA
	{
		VEC3						appearLoc;
		CKAGETOKI_ADVENT_ENEMY*		enemy;

	//	�R���X�g���N�^
		ENEMY_DATA(void)
		{
			this->appearLoc	= VEC3(0.0f, 0.0f, 0.0f);
			this->enemy		= NULL;
		}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CADVENT_ENEMY_MANAGER
	//
	//	�@�\			�A�h�x���`���[�Q�[���̓G�L�����N�^�[���Ǘ�����N���X
	//	�X�V			2009/02/16	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CADVENT_ENEMY_MANAGER
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	���\�[�X�u����
			//==================================================================
				CSKIN_MESH_BASE*	shadoll;
				CSKIN_MESH_BASE*	shadoll_sword;
				CSKIN_MESH_BASE*	shadog;
				CSKIN_MESH_BASE*	shadoll_archery;

				int					currFileNumber;			//!< ���݂̃t�@�C���ԍ�
				vector<ENEMY_DATA>	enemyObjects;			//!< ���䂷��G�f�[�^�����o�[

			//==================================================================
			//	�����C���X�^���X
			//==================================================================
		static	CADVENT_ENEMY_MANAGER*		gUniqueInstance;

				CADVENT_ENEMY_MANAGER(DEV inDev);			//!< �R���X�g���N�^
				~CADVENT_ENEMY_MANAGER(void);				//!< �f�X�g���N�^

	public:			//-------------------- public --------------------//

		static	CADVENT_ENEMY_MANAGER*	getInstance(DEV inDev);			//!< �C���X�^���X�擾
		static	void					releaseInstance(void);			//!< �C���X�^���X���

				void	loadEnemyFromFile(
							DEV inDev, int inFileNumber,
							CKAGETOKI_ADVENT_CHARACTER**	inPlayer,
							C3D_OBJECT_GROUP*	inViewG,
							C3D_OBJECT_GROUP*	inHitG,
							C3D_OBJECT_GROUP*	inEnemyG);	//!< �t�@�C���ǂݍ���
				void	updateEnemy(VEC3* inPlayerLoc);		//!< �G�X�V
				void	resetEnemy(void);					//!< �G���Z�b�g
				void	releaseEnemy(void);					//!< �G���
	};
}
//-----------------------------------------------------------------------
//!	�Z�[�u���\�[�X�̖��O���
//-----------------------------------------------------------------------