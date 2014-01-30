//***********************************************************************************
//!	�t�@�C����		Class_AdventEnemyManager.cpp
//
//	�@�\			�A�h�x���`���[�Q�[���̓G�L�����N�^�[���Ǘ�����N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2009/02/16	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//-----------------------------------------------------------------------
//!	�Z�[�u���\�[�X�̖��O���
//-----------------------------------------------------------------------
namespace EM
{
	//================================================================================
	//	�C���X�^���X�����ꏊ
	//================================================================================
	CADVENT_ENEMY_MANAGER*	CADVENT_ENEMY_MANAGER::gUniqueInstance	= NULL;

	//================================================================================
	//!	���\�b�h��	CADVENT_ENEMY_MANAGER::�R���X�g���N�^
	//
	//	����		inDev		Direct3D�f�o�C�X
	//	�X�V		2009/02/16	<�V�K>
	//================================================================================
	CADVENT_ENEMY_MANAGER::CADVENT_ENEMY_MANAGER(DEV inDev)
	{
	//	����
		this->shadoll			= NULL;
		this->shadoll_sword		= NULL;
		this->shadog			= NULL;
		this->shadoll_archery	= NULL;

		this->currFileNumber	= -10000;
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_ENEMY_MANAGER::�f�X�g���N�^
	//
	//	�X�V		2009/02/16	<�V�K>
	//================================================================================
	CADVENT_ENEMY_MANAGER::~CADVENT_ENEMY_MANAGER(void)
	{
	//	���
		SAFE_DELETE(this->shadoll);
		SAFE_DELETE(this->shadoll_sword);
		SAFE_DELETE(this->shadog);
		SAFE_DELETE(this->shadoll_archery);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_ENEMY_MANAGER::getInstance
	//
	//	�@�\		�C���X�^���X�̎擾
	//	����		inDev		Direct3D�f�o�C�X
	//	�߂�l		�C���X�^���X��Ԃ�
	//	�X�V		2009/02/16	<�V�K>
	//================================================================================
	CADVENT_ENEMY_MANAGER*
	CADVENT_ENEMY_MANAGER::getInstance(DEV inDev)
	{
	//!	��������Ă��Ȃ���΁A��������
		if(	!gUniqueInstance)
		{
		//!	�C���X�^���X����
			gUniqueInstance = new CADVENT_ENEMY_MANAGER(inDev);
		}
	//!	�C���X�^���X��Ԃ�
		return gUniqueInstance;
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_ENEMY_MANAGER::releaseInstance
	//
	//	�@�\		�C���X�^���X���
	//	�X�V		2009/02/16	<�V�K>
	//================================================================================
	void
	CADVENT_ENEMY_MANAGER::releaseInstance(void)
	{
	//!	�C���X�^���X�̉��
		SAFE_DELETE(gUniqueInstance);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_ENEMY_MANAGER::loadEnemyFromFile
	//
	//	�@�\		�t�@�C������A�G��ǂݍ���
	//	����		inFileNumber	�ǂݍ��ރt�@�C���̔ԍ�
	//				inPlayer		�v���C���[�L�����N�^�[
	//				inViewG			�`��O���[�v
	//				inHitG			�Փ˔���O���[�v
	//				inEnemyG		�G�O���[�v
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CADVENT_ENEMY_MANAGER::loadEnemyFromFile(
		DEV								inDev,
		int								inFileNumber,
		CKAGETOKI_ADVENT_CHARACTER**	inPlayer,
		C3D_OBJECT_GROUP*				inViewG,
		C3D_OBJECT_GROUP*				inHitG,
		C3D_OBJECT_GROUP*				inEnemyG)
	{
	//	�ǂݍ��ރt�@�C���ԍ����O��Ɠ������Ȃ��Ȃ�A�V���ɓǂݍ���
		if(	this->currFileNumber != inFileNumber)
		{
			this->releaseEnemy();
			this->currFileNumber  = inFileNumber;
		}
	//	�������Ȃ�A���Z�b�g��������
		else
		{
			this->resetEnemy();
			return;
		}

	//-------------------------------------------------
	//	�t�@�C�����J��
	//-------------------------------------------------
		FILE*	enemyFile			= NULL;		//!< �X�e�[�W�e�L�X�g�t�@�C���|�C���^�[
		char	szString[MAX_PATH]	= "";		//!< �e�L�X�g���쐬�p

	//	�e�L�X�g���𐶐�
		sprintf_s(szString, sizeof(szString), PATH_ENEMYMAP, this->currFileNumber);
	//	�t�@�C�����J��
		fopen_s(&enemyFile, szString, "r");

	//	�t�@�C�����J���Ȃ�������
		if(	!enemyFile)
		{
		//	�ǂݍ��݂��s�킸�A������Ԃ�
			return;
		}

	//	�ǂݍ��݊J�n
		ENEMY_DATA	enemyData	= ENEMY_DATA();				//!< �V���ȓG�f�[�^
		VEC3		appearLoc	= VEC3(0.0f, 0.0f, 0.0f);	//!< �o���ʒu
		MY_CUBE		controlArea	= MY_CUBE();				//!< ����͈͂̃I�t�Z�b�g�l
		MY_CUBE		actionArea	= MY_CUBE();				//!< �s���\�͈�
		BOOL		bCheckEnd	= TRUE;						//!< �I���m�F
		int			iEnemyKind	= 0;						//!< �G��ޔԍ�

	//	�G���ʗp
		char	enemyJudger[eEKIND_MAX][11]	=
		{
			"SHADOLL___",
			"SHADOLL_SW",
			"SHADOG____",
			"SHADOLL_AR",
		};

	//	�I�����߂�����܂ŁA���[�v
		while(bCheckEnd)
		{
			ZeroMemory(szString, sizeof(szString));
			fscanf_s(enemyFile, "%s, ", szString, sizeof(szString));	//!< �G�̎��

		//	���̐����̎�ނ��A���l�Ƃ��Č��o
			for(iEnemyKind = 0; iEnemyKind < eEKIND_MAX; iEnemyKind++)
			{
				if(	strcmp(szString, enemyJudger[iEnemyKind]) == 0)	break;
			}

		//	�O���[�v�ɊY�����Ȃ��f�[�^�Ȃ�A�������I��
			if(	iEnemyKind >= eEKIND_MAX)	break;

		//	���o�����G��ޔԍ��ŕ��򂵂āA����
			switch(iEnemyKind)
			{
			//	�V���h�[��
				case eEKIND_SHADOLL:
					enemyData.enemy	= new CENEMY_SHADOLL(inDev);
					if(!this->shadoll){
						this->shadoll	= new CSKIN_MESH_BASE(inDev, "enemy_shadoll");
					}
					break;
			//	�V���h�[���\�[�h
				case eEKIND_SHADOLL_SWORD:
					enemyData.enemy	= new CENEMY_SHADOLL_SWORD(inDev);
					if(!this->shadoll_sword){
						this->shadoll_sword	= new CSKIN_MESH_BASE(inDev, "enemy_shadoll_sword");
					}
					break;
			//	�V���h�b�O
				case eEKIND_SHADOG:
					enemyData.enemy	= new CENEMY_SHADOG(inDev);
					if(!this->shadog){
						this->shadog	= new CSKIN_MESH_BASE(inDev, "enemy_shadog");
					}
					break;
			//	�V���h�[���A�[�`�F���[
				case eEKIND_SHADOLL_ARCHERY:
					enemyData.enemy	= new CENEMY_SHADOLL_ARCHERY(inDev);
					if(!this->shadoll_archery){
						this->shadoll_archery	= new CSKIN_MESH_BASE(inDev, "enemy_shadoll_archery");
					}
					break;
			//	��L�ȊO
				default:
					bCheckEnd	= false;
					continue;
			}

		//	�o���ʒu
			fscanf_s(enemyFile, "%f, %f, %f, ",
						&appearLoc.x,
						&appearLoc.y,
						&appearLoc.z);
		//	����͈͂̃I�t�Z�b�g�l
			fscanf_s(enemyFile, "%f, %f, %f, %f, %f, %f, ",
						&controlArea.left,
						&controlArea.right,
						&controlArea.bottom,
						&controlArea.top,
						&controlArea.rear,
						&controlArea.front);
		//	�s���\�͈�
			fscanf_s(enemyFile, "%f, %f, %f, %f, %f, %f, ",
						&actionArea.left,
						&actionArea.right,
						&actionArea.bottom,
						&actionArea.top,
						&actionArea.rear,
						&actionArea.front);
		//	���̃Z�b�g
			enemyData.appearLoc	= appearLoc;
			enemyData.enemy->setLoc(&enemyData.appearLoc);
			enemyData.enemy->setControlAreaOffset(&controlArea);
			enemyData.enemy->setActionArea(&actionArea);
			enemyData.enemy->setViewG(inViewG);
			enemyData.enemy->setStageG(inHitG);
			enemyData.enemy->setEnemyG(inEnemyG);
			enemyData.enemy->setTargetPlayer(inPlayer);
			enemyData.enemy->resetParameter();
		//	�G�O���[�v�ɓo�^
			inEnemyG->add(enemyData.enemy);
		//	�G�f�[�^�����o�[�ɓo�^
			this->enemyObjects.push_back(enemyData);
		}

	//	�t�@�C�������
		fclose(enemyFile);
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_ENEMY_MANAGER::updateEnemy
	//
	//	�@�\		�G�̍X�V
	//	����		inPlayerLoc		�v���C���[�̈ʒu
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CADVENT_ENEMY_MANAGER::updateEnemy(VEC3* inPlayerLoc)
	{
	//	��������
		VEC_LOOP(ENEMY_DATA, this->enemyObjects)
		{
			if(	p->enemy->getVisible()	&&
				p->enemy->getControlArea()->intersect(inPlayerLoc))
			{
				p->enemy->operation(NULL);
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_ENEMY_MANAGER::resetEnemy
	//
	//	�@�\		�G�̃��Z�b�g
	//	�X�V		2009/02/16		<�V�K>
	//================================================================================
	void
	CADVENT_ENEMY_MANAGER::resetEnemy(void)
	{
		VEC_LOOP(ENEMY_DATA, this->enemyObjects)
		{
			p->enemy->setLoc(&p->appearLoc);
			p->enemy->resetParameter();
		}
	}

	//================================================================================
	//!	���\�b�h��	CADVENT_ENEMY_MANAGER::releaseEnemy
	//
	//	�@�\		�G�̉��
	//	�X�V		2009/02/16	<�V�K>
	//================================================================================
	void
	CADVENT_ENEMY_MANAGER::releaseEnemy(void)
	{
	//	�G�f�[�^�����o�[�̉��
		VEC_LOOP(ENEMY_DATA, this->enemyObjects)
		{
			SAFE_DELETE(p->enemy);
		}
		this->enemyObjects.clear();

		this->currFileNumber	= -10000;
	}
}
//-----------------------------------------------------------------------
//!	�Z�[�u���\�[�X�̖��O���
//-----------------------------------------------------------------------