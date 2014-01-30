//***********************************************************************************
//!	�t�@�C����		Class_GameMap.cpp
//
//	�@�\			���̃Q�[����p��3D�}�b�v�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2008/07/25	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_GameMap.h"				//!< 3D�I�u�W�F�N�g�}�b�v

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CGAME_MAP::�R���X�g���N�^
	//
	//	����		inDev		Direct3D�f�o�C�X
	//				inCenter	�}�b�v�̒��S�ƂȂ�I�u�W�F�N�g
	//				inTxtNum	�e�L�X�g�ԍ�
	//	�X�V		2008/07/20	<�V�K>
	//================================================================================
	CGAME_MAP::CGAME_MAP(void)
	:	C3D_MAP(),
		friendG(),
		enemyG()
	{
		this->fEnemyGDistance	= this->fDistance;
	}

	//================================================================================
	//!	���\�b�h��	CGAME_MAP::�R���X�g���N�^(�S������)
	//
	//	����		inDev		Direct3D�f�o�C�X
	//				inCenter	�}�b�v�̒��S�ƂȂ�I�u�W�F�N�g
	//				inTxtNum	�e�L�X�g�ԍ�
	//	�X�V		2008/07/20	<�V�K>
	//================================================================================
	CGAME_MAP::CGAME_MAP(CONST DEV inDev, int inTxtNum)
	:	C3D_MAP(),
		friendG(),
		enemyG()
	{
	//	����
		this->initialize(inDev, inTxtNum);
	}

	//================================================================================
	//!	���\�b�h��	CGAME_MAP::�f�X�g���N�^
	//
	//	�X�V		2008/07/20	<�V�K>
	//================================================================================
	CGAME_MAP::~CGAME_MAP(void)
	{
	//	���
		this->finalize();
	}

	//================================================================================
	//!	���\�b�h��	CGAME_MAP::finalize
	//
	//	�@�\		���
	//	�X�V		2008/07/20	<�V�K>
	//================================================================================
	void
	CGAME_MAP::finalize(void)
	{
	//	��ɃX�[�p�[�N���X�̉��
		C3D_MAP::finalize();

	//	����O���[�v�̉��
		this->friendG.clear();					//!< �����O���[�v�̏���
		this->enemyG.clear();					//!< �G�O���[�v�̏���
	}

	//================================================================================
	//!	���\�b�h��	CGAME_MAP::drawEnemyG
	//
	//	�@�\		�G�O���[�v�̕`��
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CGAME_MAP::drawEnemyG(CONST DEV inDev, VEC3* inViewPoint)
	{
		vector<C3D_OBJECT*>::iterator	p;			//!< ��Ɨp�C�e���[�^

	//	�������Ȃ���`�悷�邽�߁A�Ƃ肾���ĕ`��
		p	= this->enemyG.getBegin();
		while(p != this->enemyG.getEnd())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	this->checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < this->fEnemyGDistance)
				{
					(*p)->draw(inDev);
				}
				p++;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CGAME_MAP::addObject
	//
	//	�@�\		���������I�u�W�F�N�g�̃O���[�v�o�^
	//	����		inObject		�o�^����I�u�W�F�N�g
	//				inGroupType		�o�^����O���[�v�^�C�v
	//	�߂�l		TRUE			�o�^����
	//				FALSE			�o�^������
	//	�X�V		2008/07/20		<�V�K>
	//================================================================================
	BOOL
	CGAME_MAP::addObject(C3D_OBJECT* inObject, int inGroupType)
	{
	//	�O���[�v�^�C�v����
		switch(inGroupType)
		{
			case	eMG_ENEMY:								//!< �G
				this->enemyG.add(inObject);
				return	TRUE;

			default:										//!< ��L�ȊO
				return	C3D_MAP::addObject(inObject, inGroupType);
		}
	}

	//================================================================================
	//!	���\�b�h��	CGAME_MAP::checkMessage
	//
	//	�@�\		�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N
	//	����		*inObjP			�I�u�W�F�N�g�C�e���[�^
	//	�X�V		2008/07/20		<�V�K>
	//================================================================================
	BOOL
	CGAME_MAP::checkMessage(vector<C3D_OBJECT*>::iterator* inObjP)
	{
		C3D_OBJECT*	inObj	= (*(*inObjP));					//!< �|�C���^�[�i�[
	//	��Ԍ���
		switch((*(*inObjP))->getOrderMessage())
		{
			case	eOMSG_DELETE_IN:						//!< ���ŏ���
				this->releaseG.add((*(*inObjP)));			//!< ���ŃO���[�v�ɓ����
				(*(*inObjP))->setOrderMessage(eOMSG_NOMSG);	//!< ���b�Z�[�W������
				return	TRUE;

			case	eOMSG_DELETE:							//!< ���Ŗ���
				this->releaseG.erase(inObj);				//!< ��d�J����h����
				this->hitG.erase(inObj);					//!< �Փ˃O���[�v����폜
				this->enemyG.erase(inObj);					//!< �G�O���[�v����폜
				(*inObjP)	= this->viewG.erase(*inObjP);	//!< �`��O���[�v�������
				SAFE_DELETE(inObj);							//!< ���
				return	FALSE;

			default:										//!< ��L�ȊO
				return	TRUE;
		}
	}

	//===============================================================================
	//!	���\�b�h��	CGAME_MAP::�R���X�g���N�^
	//
	//	����		inDev		Direct3D�f�o�C�X
	//				inObj		�i�[��
	//				inData		�����f�[�^
	//	�߂�l		TRUE		��������
	//				FALSE		�������s
	//	�X�V		2008/07/20	<�V�K>
	//================================================================================
	BOOL
	CGAME_MAP::createObject(CONST DEV		inDev,
							C3D_OBJECT**	inObj,
							OB_DATA*		inData)
	{
		CTEXTURE*	newTex			= NULL;			//!< �e�N�X�`���|�C���^�[�m��
		int			iObjType		= 0;			//!< �I�u�W�F�N�g�^�C�v

	//	�O���[�v�^�C�v����z��
		char	objJudger[eGMO_MAX][11]	=
		{
			"MESH______",		//!< �ʏ탁�b�V��
			"SKIN______",		//!< �ʏ�X�L�����b�V��
			"TEX_CEN___",		//!< �ʏ�e�N�X�`��(���_�����S)
			"TEX_BOT___",		//!< �ʏ�e�N�X�`��(���_����)
			"SO_COMP___",		//!< �e�I�u�W�F�N�g(���S����)
			"SO_HALFSHA",		//!< �e�I�u�W�F�N�g(���e����)
			"SO_HALFREA",		//!< �e�I�u�W�F�N�g(������)
			"SO_INCOMP_",		//!< �e�I�u�W�F�N�g(�����S����)
		};

	//	�O���[�v����
		for(iObjType = 0; iObjType < eGMO_MAX; iObjType++)
		{
			if(	strcmp(inData->szData, objJudger[iObjType]) == 0)	break;
		}

	//	�����I�u�W�F�N�g����
		switch(iObjType)
		{
			case eGMO_NORMALMESH:							//!< �ʏ탁�b�V��
				(*inObj)	= new CMESH(inDev, inData->szName);
				(*inObj)->setLoc(&inData->loc);
				(*inObj)->setAngle(&inData->angle);
				(*inObj)->setScale(inData->scale.x);
				return	TRUE;

			case eGMO_NORMALSKINMESH:						//!< �ʏ�X�L�����b�V��
				(*inObj)	= new CSKIN_MESH(inDev, inData->szName);
				(*inObj)->setLoc(&inData->loc);
				(*inObj)->setAngle(&inData->angle);
				(*inObj)->setScale(inData->scale.x);
				return	TRUE;

			case eGMO_NORMALTEXTURE_CENTER:					//!< �ʏ�e�N�X�`��(���_�����S)
				newTex		= new CTEXTURE(inDev, inData->szName, inData->scale);
				newTex->setLoc(&inData->loc);
				newTex->setAngle(&inData->angle);
				newTex->setVertexPosition(eVERP_CENTER);
				(*inObj)	= newTex;
				return	TRUE;

			case eGMO_NORMALTEXTURE_BOTTOM:					//!< �ʏ�e�N�X�`��(���_����)
				newTex		= new CTEXTURE(inDev, inData->szName, inData->scale);
				newTex->setLoc(&inData->loc);
				newTex->setAngle(&inData->angle);
				newTex->setVertexPosition(eVERP_DOWN);
				(*inObj)	= newTex;
				return	TRUE;

			case eGMO_SHADOWOBJ_COM_SUBSTANCE:
				(*inObj)	= new CSHADOW_OBJECT(inDev, eSHAO_COMPLETE, inData->szName);
				(*inObj)->setLoc(&inData->loc);
				(*inObj)->setAngle(&inData->angle);
				(*inObj)->setScale(inData->scale.x);
				return	TRUE;

			case eGMO_SHADOWOBJ_HALF_SHADOW:
				(*inObj)	= new CSHADOW_OBJECT(inDev, eSHAO_HALF_SHADOW, inData->szName);
				(*inObj)->setLoc(&inData->loc);
				(*inObj)->setAngle(&inData->angle);
				(*inObj)->setScale(inData->scale.x);
				return	TRUE;

			case eGMO_SHADOWOBJ_HALF_REAL:
				(*inObj)	= new CSHADOW_OBJECT(inDev, eSHAO_HALF_REAL, inData->szName);
				(*inObj)->setLoc(&inData->loc);
				(*inObj)->setAngle(&inData->angle);
				(*inObj)->setScale(inData->scale.x);
				return	TRUE;

			case eGMO_SHADOWOBJ_INCOM_SUBSTANCE:
				(*inObj)	= new CSHADOW_OBJECT(inDev, eSHAO_INCOMPLETE, inData->szName);
				(*inObj)->setLoc(&inData->loc);
				(*inObj)->setAngle(&inData->angle);
				(*inObj)->setScale(inData->scale.x);
				return	TRUE;

			default:										//!< ��L�ȊO
				(*inObj)	= NULL;
				return	FALSE;
		}
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------