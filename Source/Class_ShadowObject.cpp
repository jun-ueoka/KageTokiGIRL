//***********************************************************************************
//!	�t�@�C����		Class_ShadowObject.cpp
//
//	�@�\			�e���̂Ɋւ������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/08/29	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_ShadowObject.h"

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//	���L�e���̃f�[�^�}�b�v
	//================================================================================
	map<string, SHADOW_OBJECT_DATA*>*		CSHADOW_OBJECT::mShadowData = NULL;

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::createShadowData
	//
	//	�@�\		�e���̃f�[�^�����[�h����B
	//	�@�\�T�v	�e���̃f�[�^�����[�h���A���̉e���̃f�[�^��Ԃ�
	//				���̃��\�b�h�̓N���X���\�b�h�ł���B
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			�e���̃t�@�C���̖���
	//				inResource		�e���̃t�H���_��
	//	�߂�l		�e���̃f�[�^�̃|�C���^�[
	//	�X�V		2008/08/31		<�V�K>
	//================================================================================
	SHADOW_OBJECT_DATA*
	CSHADOW_OBJECT::createShadowData(CONST DEV	 inDev,
									 CONST LPSTR inName,
									 CONST LPSTR inResource)
	{
	//	�V���ȉe���̏��
		SHADOW_OBJECT_DATA*	newShadowObject		= new SHADOW_OBJECT_DATA();
		char				realName[MAX_PATH]	= "";				//!< �e�L�X�g���쐬�p

	//-------------------------------------------------
	//	�t�@�C�����J��
	//-------------------------------------------------
		FILE*	fp	= NULL;

	//	�t�@�C�����J��
		fopen_s(&fp, inName, "r");

	//	�J���Ȃ�������A�p�X��t���Ă�����x
		if(	!fp)
		{
		//	�e�L�X�g���𐶐�
			sprintf_s(realName, sizeof(realName), inResource, inName);
		//	�t�@�C�����J��
			fopen_s(&fp, realName, "r");
		//	�t�@�C�����J���Ȃ�������
			if(	!fp)
			{
			//	�ǂݍ��݂��s�킸�ANULL��Ԃ�������
				DX::errorMsgNoFile(inDev, realName);
				goto error;
			}
		}

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�t�@�C�����J������A�ǂݍ��݊J�n
	//-------------------------------------------------
	//	�ꊇ�ǂݍ���
	//-------------------------------------------------
		char	szShadowObjName[MAX_PATH]	= "";				//!< �e����
		char	szHitObjName[MAX_PATH]		= "";				//!< �Փ˔���p���b�V������

	//	�ꋓ�ɓǂݍ���
		fscanf_s(fp, "%s %s ",
			 szShadowObjName,	sizeof(szShadowObjName),		//!< �e����
			 szHitObjName,		sizeof(szHitObjName));			//!< �Փ˔���p���b�V������

	//	�e���̃��b�V�����w�肳��Ă���A����
		if(	strcmp(szShadowObjName, NONE_OBJECT) != 0)
		{
		//	�e���̃��b�V���̐����Ɏ��s������ANULL��Ԃ�
			newShadowObject->shadowObj	= new CMESH_BASE(inDev, szShadowObjName);
			if(	newShadowObject->shadowObj->meshInvalid())
			{
				DX::errorMsgNoFile(inDev, szShadowObjName);
				goto error;
			}
		}

	//	�Փ˔���p���b�V�����w�肳��Ă���A����
		if(	strcmp(szHitObjName, NONE_OBJECT) != 0)
		{
		//	�Փ˔���p���b�V���̐����Ɏ��s������ANULL��Ԃ�
			newShadowObject->hitObj		= new CMESH_BASE(inDev, szHitObjName);
			if(	newShadowObject->shadowObj->meshInvalid())
			{
				DX::errorMsgNoFile(inDev, szHitObjName);
				goto error;
			}
		}

		fclose(fp);
	//	�쐬�����e���̖̂��O��o�^����
		newShadowObject->name	= inName;
	//	�쐬�����e���̂̏���Ԃ�
		return	newShadowObject;

	//	�ǂݍ��ݏI��
	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	//	NULL��Ԃ�����
	error:
		SAFE_DELETE(newShadowObject);	//!< ���������e���̏����J��
		if(	fp)fclose(fp);
		return	NULL;					//!< NULL��Ԃ�
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::startShadowObjectInitialize
	//
	//	�@�\		�����̏�����
	//	�X�V		2008/12/03		<�V�K>
	//================================================================================
	void
	CSHADOW_OBJECT::startShadowObjectInitialize(void)
	{
	//	�X�[�p�[�N���X�̏�����
		C3D_OBJECT::initialize();

		this->so_ptr					= NULL;			//!< �e���̏��
		this->bShadowObjectMeshLoaded	= FALSE;		//!< �e���̃��[�h�m�F
		this->shadowObjType				= eSHAO_NONE;	//!< �e���̍\���^�C�v
		this->shadowObjAction			= eSHAOAC_NONE;	//!< �e���̓���
		this->shadowDiffuse.r			= 1.0f;			//!< �f�B�t���[�Yr
		this->shadowDiffuse.g			= 1.0f;			//!< �f�B�t���[�Yg
		this->shadowDiffuse.b			= 1.0f;			//!< �f�B�t���[�Yb
		this->shadowDiffuse.a			= 1.0f;			//!< �f�B�t���[�Ya
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::initializeShadowObject
	//
	//	�@�\		�e���̂̏�����
	//	����		inDev			Direct3D�f�o�C�X
	//				inType			�e���̍\���^�C�v
	//				inName			�e���̃t�@�C���̖���
	//				inResource		�e���̃t�H���_��
	//	�X�V		2008/12/03		<�V�K>
	//================================================================================
	void
	CSHADOW_OBJECT::initializeShadowObject(CONST DEV		inDev,
										   SHADOW_OBJ_TYPE	inType,
										   CONST LPSTR		inName,
										   CONST LPSTR		inResource)
	{
	//	�e���̂̏������
		this->finishalizeShadowObject();

	//	�e���̃}�b�v��������ΐ�������
		if(	!mShadowData)
		{
			mShadowData = new map<string, SHADOW_OBJECT_DATA*>;
		}

	//	��Ɨp�C�e���[�^
		map<string, SHADOW_OBJECT_DATA*>::iterator	p;
	//	�e���̃}�b�v�ɉe���̃t�@�C������n���A���̃C�e���[�^���󂯎��
		p = mShadowData->find(inName);

	//	�e���̃}�b�v�ɓ������O�̉e���̃t�@�C���������ɓo�^���Ă���ꍇ
		if(	p != mShadowData->end())
		{
		//	�g�p�J�E���^�𑝂₷
			p->second->cnt++;
		//	�e���̏����󂯎��
			this->so_ptr	= (*p).second;
		//	���[�h����
			this->bShadowObjectMeshLoaded	= TRUE;
		}
		else
		{
		//	�V���ɉe���̂𐶐�����
			this->so_ptr	= this->createShadowData(inDev, inName, inResource);

		//	�����ɐ��������ꍇ
			if(	this->so_ptr)
			{
			//	�g�p�J�E���^�𑝂₷
				this->so_ptr->cnt++;
			//	���[�h����
				this->bShadowObjectMeshLoaded	= TRUE;
			//	�e���̃}�b�v�ɓo�^
				mShadowData->insert(
					pair<string, SHADOW_OBJECT_DATA*>(this->so_ptr->name, this->so_ptr));
			}
		//	���s�����ꍇ
			else
			{
			//	���[�h������
				this->bShadowObjectMeshLoaded	= FALSE;
			}
		}

	//	�e���̏��������Ă��鎞
		if(	this->bShadowObjectMeshLoaded)
		{
		//	�c���������
			this->setShadowObjType(inType);
		}
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::finishalizeShadowObject
	//
	//	�@�\		�e���̂̉��
	//	�X�V		2008/12/03		<�V�K>
	//================================================================================
	void
	CSHADOW_OBJECT::finishalizeShadowObject(void)
	{
	//	�X�[�p�[�N���X�̏�����
		C3D_OBJECT::finalize();
	//	�e���̏������[�h���ĂȂ��Ȃ�A������Ԃ�
		if(	!this->bShadowObjectMeshLoaded)		return;

	//	�e���̃J�E���g�����炷
		this->so_ptr->cnt--;
	//	���̉e���̂��܂��g�p����Ă���ꍇ�͉�����Ȃ�
		if(	this->so_ptr->cnt > 0)	return;

	//	�e���̃}�b�v�C�e���[�^
		map<string, SHADOW_OBJECT_DATA*>::iterator	p;
	//	�e���̃}�b�v�ɉe���̃t�@�C������n���A���̃C�e���[�^���󂯎��
		p = mShadowData->find(this->so_ptr->name);
	//	�e���̏��̉��
		SAFE_DELETE(p->second);
	//	�e���̃}�b�v����폜
		mShadowData->erase(p);

	//	�e���̃}�b�v�Ƀ����o�[��������΁A��������
		if(	mShadowData->begin() == mShadowData->end())
		{
			SAFE_DELETE(mShadowData);
		}
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::�R���X�g���N�^
	//
	//	�@�\		�G�t�F�N�g�t�@�C�������[�h���ď�����
	//	����		inDev			Direct3D�f�o�C�X
	//				inType			�e���̍\���^�C�v
	//				inName			�e���̃t�@�C���̖���
	//				inResource		�e���̃t�H���_��
	//	�X�V		2008/12/03		<�V�K>
	//================================================================================
	CSHADOW_OBJECT::CSHADOW_OBJECT(CONST DEV       inDev,
								   SHADOW_OBJ_TYPE inType,
								   CONST LPSTR     inName,
								   CONST LPSTR     inResource)
	{
	//	�����̏�����
		this->startShadowObjectInitialize();
	//	���b�V���̏�����
		this->initializeShadowObject(inDev, inType, inName, inResource);
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::�R���X�g���N�^
	//
	//	�@�\		�󏉊���
	//	�X�V		2008/12/03		<�V�K>
	//================================================================================
	CSHADOW_OBJECT::CSHADOW_OBJECT(void)
	{
	//	�����̏�����
		this->startShadowObjectInitialize();
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::�R���X�g���N�^
	//
	//	�@�\		�R�s�[
	//	����		*inCopy			�R�s�[��
	//	�X�V		2008/12/04		<�V�K>
	//================================================================================
	CSHADOW_OBJECT::CSHADOW_OBJECT(CSHADOW_OBJECT* inCopy)
	{
	//	�����̏�����
		this->startShadowObjectInitialize();
	//	�R�s�[�J�n
		this->so_ptr	= inCopy->getShadowObjects();

	//	�e���̏�񂪂��鎞
		if(	this->so_ptr)
		{
		//	�c����R�s�[
			this->setShadowObjType(inCopy->getShadowObjType());
		//	�g�p�J�E���^�𑝂₷
			this->so_ptr->cnt++;
		//	���[�h����
			this->bShadowObjectMeshLoaded	= TRUE;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::�f�X�g���N�^
	//
	//	�X�V		2008/12/04	<�V�K>
	//================================================================================
	CSHADOW_OBJECT::~CSHADOW_OBJECT(void)
	{
	//	�e���̂̉��
		this->finishalizeShadowObject();
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::setShadowObjType
	//
	//	�@�\		�e���̂̍\���̐ݒ�
	//	����		inType			�V�����e�\�����b�Z�[�W
	//	�X�V		2008/12/05		<�V�K>
	//================================================================================
	void
	CSHADOW_OBJECT::setShadowObjType(SHADOW_OBJ_TYPE inType)
	{
	//	�e���̂̍\����ݒ�
		this->shadowObjType	= inType;
	//	�V�����e���̍\���ɂ���ăT�u���b�Z�[�W������
		switch(this->shadowObjType)
		{
		//	�e�����̂����镨��
			case eSHAO_COMPLETE:
			//	�e���b�V������������Ă��Ȃ���΁A����
				if(	!this->so_ptr->shadowObj)
				{
					this->setThisMessage(eTMSG_NOMSG);
					this->setOrderMessage(eOMSG_DELETE);
				}
				else
				{
					this->setThisMessage(eTMSG_NOMSG);
					this->shadowDiffuse.r	= 1.0f;
					this->shadowDiffuse.g	= 1.0f;
					this->shadowDiffuse.b	= 1.0f;
					this->shadowDiffuse.a	= 1.0f;
				}
				break;
		//	�e���Ȃ�����
			case eSHAO_HALF_REAL:
			//	�e���b�V������������Ă��Ȃ���΁A����
				if(	!this->so_ptr->shadowObj)
				{
					this->setThisMessage(eTMSG_NOMSG);
					this->setOrderMessage(eOMSG_DELETE);
				}
				else
				{
					this->setThisMessage(eTMSG_NO_SEE_SHADOW);
					this->shadowDiffuse.r	= 1.0f;
					this->shadowDiffuse.g	= 1.0f;
					this->shadowDiffuse.b	= 1.0f;
					this->shadowDiffuse.a	= 0.5f;
				}
				break;
		//	�e�����̕���
			case eSHAO_HALF_SHADOW:
			//	�e���b�V������������Ă��Ȃ���΁A����
				if(	!this->so_ptr->shadowObj)
				{
					this->setThisMessage(eTMSG_NOMSG);
					this->setOrderMessage(eOMSG_DELETE);
				}
				else
				{
					this->setThisMessage(eTMSG_SEE_SHADOW);
					this->shadowDiffuse.r	= 0.0f;
					this->shadowDiffuse.g	= 0.0f;
					this->shadowDiffuse.b	= 0.0f;
					this->shadowDiffuse.a	= 0.5f;
				}
				break;
		//	���Ԃɉe(�e���@)����������
			case eSHAO_INCOMPLETE:
			//	�e���b�V������������Ă��Ȃ���΁A����
				if(	!this->so_ptr->shadowObj)
				{
					this->setThisMessage(eTMSG_NOMSG);
					this->setOrderMessage(eOMSG_DELETE);
				}
				else
				{
					this->setThisMessage(eTMSG_SEE_SHADOW);
					this->shadowDiffuse.r	= 1.0f;
					this->shadowDiffuse.g	= 1.0f;
					this->shadowDiffuse.b	= 1.0f;
					this->shadowDiffuse.a	= 1.0f;
				}
				break;
		//	�e�����Ԃ��Ȃ����
			case eSHAO_NONE:
				this->setOrderMessage(eOMSG_DELETE);
				break;
		//	��L�ȊO
			default:
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::reactionMessage
	//
	//	�@�\		���A�N�V�����Ή�
	//	����		inMessage		���A�N�V�������b�Z�[�W
	//	�X�V		2008/12/04		<�V�K>
	//================================================================================
	void
	CSHADOW_OBJECT::reactionMessage(REACTION_MESSAGE inMessage)
	{
	//	�e���̍\������
		switch(this->shadowObjType)
		{
		//	�e���Ȃ�����
			case eSHAO_HALF_REAL:
			//	���b�Z�[�W���u�V���h�E�{�[���ɓ��������v�Ȃ�A
			//	�u�e���t�����Ă����v��������s
				if(	inMessage == eREAMSG_SHADOW_HIT)
				{
					this->shadowObjAction	= eSHAOAC_SHADOW_REVIVAL;
				}
				break;
		//	�e�����̕���
			case eSHAO_HALF_SHADOW:
			//	���b�Z�[�W���u�V���C���{�[���ɓ��������v�Ȃ�A
			//	�u���ł��Ă����v��������s
				if(	inMessage == eREAMSG_SHINE_HIT)
				{
					this->shadowObjAction	= eSHAOAC_DELETE;
				}
				break;
		//	���Ԃɉe(�e���@)����������
			case eSHAO_INCOMPLETE:
			//	���b�Z�[�W���u�V���C���{�[���ɓ��������v�Ȃ�A
			//	�u�e���������ł��Ă����v��������s
				if(	inMessage == eREAMSG_SHINE_HIT)
				{
					this->shadowObjAction	= eSHAOAC_SHADOW_DELETE;
				}
				break;
		//	��L�ȊO
			default:
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::actionShadowObj
	//
	//	�@�\		�e���̂̓���
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	CSHADOW_OBJECT::actionShadowObj(void)
	{
	//	�e���̓��앪��
		switch(this->shadowObjAction)
		{
		//	�e����Ȃ�
			case eSHAOAC_NONE:
				break;
		//	�e���t�����Ă���
			case eSHAOAC_SHADOW_REVIVAL:
			//	�A���t�@�l���グ�A���E�܂ŏ㏸������
			//	�e���̍\�����u���̂ɉe���������́v�ɕύX
				if((this->shadowDiffuse.a +  CHANGE_VALUE_DIFFUSE) < 1.0f)
					this->shadowDiffuse.a += CHANGE_VALUE_DIFFUSE;
				else	
				{
					this->setShadowObjType(eSHAO_INCOMPLETE);
				//	�e����Ȃ��ɂ���
					this->shadowObjAction	= eSHAOAC_NONE;
				}
				break;
		//	�e���������ł��Ă���
			case eSHAOAC_SHADOW_DELETE:
			//	�A���t�@�l�������A���E�܂ŉ��~������
			//	�e���̍\�����u�e���Ȃ����́v�ɕύX
				if((this->shadowDiffuse.a -  CHANGE_VALUE_DIFFUSE) > 0.5f)
					this->shadowDiffuse.a -= CHANGE_VALUE_DIFFUSE;
				else	
				{
					this->setShadowObjType(eSHAO_HALF_REAL);
				//	�e����Ȃ��ɂ���
					this->shadowObjAction	= eSHAOAC_NONE;
				}
				break;
		//	���ł��Ă���
			case eSHAOAC_DELETE:
			//	�A���t�@�l�������A���E�܂ŉ��~��������ł�����
				if((this->shadowDiffuse.a -  CHANGE_VALUE_DIFFUSE) > 0.0f)
					this->shadowDiffuse.a -= CHANGE_VALUE_DIFFUSE;
				else	
				{
					this->setShadowObjType(eSHAO_NONE);
					this->setOrderMessage(eOMSG_DELETE);
				//	�e����Ȃ��ɂ���
					this->shadowObjAction	= eSHAOAC_NONE;
				}
				break;
		//	��L�ȊO
			default:
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::getWorldCenter
	//
	//	�@�\		���[���h���W�n���S�_���󂯎��
	//	�߂�l		���[���h���W�n���S�_
	//	�X�V		2008/12/04		<�V�K>
	//================================================================================
	VEC3
	CSHADOW_OBJECT::getWorldCenter(void)
	{
	//	���[���h���W�n���S�_
		VEC3	aCenter;
	//	���[���h���W�n�̉��Z
		D3DXVec3TransformCoord(
			&aCenter, &so_ptr->shadowObj->getMeshs()->localCenter, this->getWorldMatrix());
	//	���[���h���W�n���S�_��Ԃ�
		return	aCenter;
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::intersectMesh
	//
	//	�@�\		���g��X���b�V���Ƃ̏Փ˂����o����
	//	����		*inObject		�ՓˑΏە�
	//				*inRayLoc		���C�̎n�_
	//				*inLine			���g�̐i�s����
	//				*inDist			���C�̎n�_����A�Փ˒n�_�܂ł̋���
	//	�߂�l		TRUE			�Փ˂��Ă���
	//				FALSE			�Փ˂��Ă��Ȃ�
	//	�X�V		2008/12/04		<�V�K>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersectMesh(C3D_OBJECT* inObject, VEC3* inRayLoc, LINE* inLine, float* inDist)
	{
		BOOL	bHit			= FALSE;					//!< �߂�l
		DWORD	dwFaceIndex		= 0;						//!< ���C�̎n�_�ɍł��߂��ʂ̃C���f�b�N�X
		FLOAT	fU				= 0.0f;						//!< �d�S�q�b�g���WU
		FLOAT	fV				= 0.0f;						//!< �d�S�q�b�g���WV
		VEC3	rayDirection	= inObject->calcRay(inLine);//!< ���C�̕���
		XMESH	hitMesh			= (this->so_ptr->hitObj ? this->so_ptr->hitObj->getXMesh() :
														  this->so_ptr->shadowObj->getXMesh());

	//	�U�����郌�C�̈ʒu���U���������W�n�ɍ��킹��
		this->rayFromInversedMatrix(inRayLoc, &rayDirection);

	//	���C�̕����͕K�����K������
		D3DXVec3Normalize(&rayDirection, &rayDirection);	//!< ���K��(0.0 or 1.0�Ɏ�������)

	//	���b�V���ƃ��C�Ƃ̏Փ˔���
		D3DXIntersect(
			hitMesh,										//!< �ՓˑΏە�
			inRayLoc,										//!< ���C�̎n�_���W
			&rayDirection,									//!< ���C�̕���
			&bHit,											//!< �Փ˔��茋�ʊi�[��
			&dwFaceIndex,									//!< ���C�̎n�_�ɍł��߂��ʂ̃C���f�b�N�X�i�[��
			&fU,											//!< �d�S�q�b�g���WU�i�[��
			&fV,											//!< �d�S�q�b�g���WV�i�[��
			inDist,											//!< ���C�̎n�_����ʂ̌�_�܂ł̋����i�[��
			NULL,											//!< �Փ˔��茋�ʂ̍\���̊i�[��
			NULL);											//!< �z����̃G���g�����i�[��

		return	bHit;
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::intersectObject
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ́A���a�ł̏����Փ˔���
	//	����		inObject		�ՓˑΏە�
	//	�߂�l		TRUE			�Փ˂��Ă��邪�A���b�V���Ƃ̏Փ˔��肪�\
	//				FALSE			�Փˌ��ʂ��o�Ă���(����ȏ㔻��o���Ȃ�)
	//	�X�V		2008/12/04		<�V�K>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersectObject(C3D_OBJECT* inObject)
	{
	//	�ǂ��炩����\���Ȃ�A���肹���ɏՓ˂��Ă��Ȃ�����Ԃ�
		if(	(!inObject->getVisible()) || (!this->getVisible()))
		{	
			return FALSE;
		}
	//	���a���Փ˂��Ă��Ȃ����͏I��
		if(	!this->intersectRadius(inObject))
		{
			return FALSE;
		}
	//	���a���Փ˂��Ă���
		else
		{
			return TRUE;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::intersectMsgCheck
	//
	//	�@�\		���b�Z�[�W�ʏՓ˔���
	//	����		inCheckMsg		�`�F�b�N���鎯�ʃ��b�Z�[�W
	//	�߂�l		TRUE			�Փ˔���L��
	//				FALSE			�Փ˔��薳��
	//	�X�V		2008/12/04		<�V�K>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersectMsgCheck(THIS_MESSAGE inCheckMsg)
	{
	//	�e���̍\������
		switch(this->shadowObjType)
		{
		//	�e�����̂����镨��
			case eSHAO_COMPLETE:
				return	TRUE;
		//	�e���Ȃ�����
			case eSHAO_HALF_REAL:
			//	���b�Z�[�W���V���h�E�{�[���Ȃ�A�Փ˔�����s��
				if(	inCheckMsg == eTMSG_SHADOW_BALL)
				{
					return	TRUE;
				}
				return	FALSE;
		//	�e�����̕���
			case eSHAO_HALF_SHADOW:
				return	TRUE;
		//	���Ԃɉe����������
			case eSHAO_INCOMPLETE:
				return	TRUE;
		//	�e�����̂��Ȃ����
			case eSHAO_NONE:
				return	FALSE;
		//	��L�ȊO
			default:
				return	FALSE;
		}
		return	FALSE;
	}


	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::intersect�i�S�����Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject		�ՓˑΏە�
	//				inLine			���g�̐i�s����
	//	�߂�l		TRUE			�Փ˂��Ă���
	//				FALSE			�Փ˂��Ă��Ȃ�
	//	�X�V		2008/12/04		<�V�K>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE* inLine)
	{
	//	�܂��̓��b�Z�[�W�Ŕ���
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	this->intersectMesh(inObject, &inObject->getLoc(), inLine, &fDist))
		{
		//	�Փ˂܂ł̋������A�ړ��������Z����
			if(	fDist < inObject->getMoveQuantity())
			{
			//	�n�ʂ���͂ݏo�����������i�[
				inObject->setDist(fDist);
			//	�Փ˂��������
				return TRUE;
			}
		}

	//	�Փ˂��Ȃ����������
		return FALSE;
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::intersect�i�㉺�Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject		�ՓˑΏە�
	//				inUpDown		���g�̏㉺�i�s����
	//	�߂�l		TRUE			�Փ˂��Ă���
	//				FALSE			�Փ˂��Ă��Ȃ�
	//	�X�V		2008/12/04		<�V�K>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown)
	{
	//	�܂��̓��b�Z�[�W�Ŕ���
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���
		LINE	line	= LINE(inUpDown, eLR_NO, eFR_NO);		//!< �㉺���ʐ�p�̕����\���̂����

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	this->intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	�Փ˂܂ł̋������A�ړ��������Z����
			if(	fDist < inObject->getMoveQuantity())
			{
			//	�n�ʂ���͂ݏo�����������i�[
				inObject->setDist(fDist);
			//	�Փ˂��������
				return TRUE;
			}
		}

	//	�Փ˂��Ȃ����������
		return FALSE;
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::intersect�i���E�Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject		�ՓˑΏە�
	//				inLeftRight		���g�̍��E�i�s����
	//	�߂�l		TRUE			�Փ˂��Ă���
	//				FALSE			�Փ˂��Ă��Ȃ�
	//	�X�V		2008/12/04		<�V�K>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight)
	{
	//	�܂��̓��b�Z�[�W�Ŕ���
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���
		LINE	line	= LINE(eUD_NO, inLeftRight, eFR_NO);	//!< ���E���ʐ�p�̕����\���̂����

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	this->intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	�Փ˂܂ł̋������A�ړ��������Z����
			if(	fDist < inObject->getMoveQuantity())
			{
			//	�n�ʂ���͂ݏo�����������i�[
				inObject->setDist(fDist);
			//	�Փ˂��������
				return TRUE;
			}
		}

	//	�Փ˂��Ȃ����������
		return FALSE;
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::intersect�i�O��Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject		�ՓˑΏە�
	//				inFrontRear		���g�̑O��i�s����
	//	�߂�l		TRUE			�Փ˂��Ă���
	//				FALSE			�Փ˂��Ă��Ȃ�
	//	�X�V		2008/12/04		<�V�K>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear)
	{
	//	�܂��̓��b�Z�[�W�Ŕ���
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���
		LINE	line	= LINE(eUD_NO, eLR_NO, inFrontRear);	//!< �O�㔻�ʐ�p�̕����\���̂����

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	this->intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	�Փ˂܂ł̋������A�ړ��������Z����
			if(	fDist < inObject->getMoveQuantity())
			{
			//	�n�ʂ���͂ݏo�����������i�[
				inObject->setDist(fDist);
			//	�Փ˂��������
				return TRUE;
			}
		}

	//	�Փ˂��Ȃ����������
		return FALSE;
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::intersect�i�����w��A�S�����Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject			�ՓˑΏە�
	//				inLine				���g�̐i�s����
	//				inIntersectValue	���肷�鋗��
	//	�߂�l		TRUE				�Փ˂��Ă���
	//				FALSE				�Փ˂��Ă��Ȃ�
	//	�X�V		2008/12/04			<�V�K>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE* inLine, float inIntersectValue)
	{
	//	�܂��̓��b�Z�[�W�Ŕ���
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	this->intersectMesh(inObject, &inObject->getLoc(), inLine, &fDist))
		{
		//	�Փ˂܂ł̋������A�ړ��������Z����
			if(	fDist < inIntersectValue)
			{
			//	�n�ʂ���͂ݏo�����������i�[
				inObject->setDist(fDist);
			//	�Փ˂��������
				return TRUE;
			}
		}

	//	�Փ˂��Ȃ����������
		return FALSE;
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::intersect�i�����w��A�㉺�Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject			�ՓˑΏە�
	//				inUpDown			���g�̏㉺�i�s����
	//				inIntersectValue	���肷�鋗��
	//	�߂�l		TRUE				�Փ˂��Ă���
	//				FALSE				�Փ˂��Ă��Ȃ�
	//	�X�V		2008/12/04			<�V�K>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown, float inIntersectValue)
	{
	//	�܂��̓��b�Z�[�W�Ŕ���
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���
		LINE	line	= LINE(inUpDown, eLR_NO, eFR_NO);		//!< �㉺���ʐ�p�̕����\���̂����

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	this->intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	�Փ˂܂ł̋������A�ړ��������Z����
			if(	fDist < inIntersectValue)
			{
			//	�n�ʂ���͂ݏo�����������i�[
				inObject->setDist(fDist);
			//	�Փ˂��������
				return TRUE;
			}
		}

	//	�Փ˂��Ȃ����������
		return FALSE;
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::intersect�i�����w��A���E�Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject			�ՓˑΏە�
	//				inLeftRight			���g�̍��E�i�s����
	//				inIntersectValue	���肷�鋗��
	//	�߂�l		TRUE				�Փ˂��Ă���
	//				FALSE				�Փ˂��Ă��Ȃ�
	//	�X�V		2008/12/04			<�V�K>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight, float inIntersectValue)
	{
	//	�܂��̓��b�Z�[�W�Ŕ���
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���
		LINE	line	= LINE(eUD_NO, inLeftRight, eFR_NO);	//!< ���E���ʐ�p�̕����\���̂����

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	this->intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	�Փ˂܂ł̋������A�ړ��������Z����
			if(	fDist < inIntersectValue)
			{
			//	�n�ʂ���͂ݏo�����������i�[
				inObject->setDist(fDist);
			//	�Փ˂��������
				return TRUE;
			}
		}

	//	�Փ˂��Ȃ����������
		return FALSE;
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::intersect�i�����w��A�O��Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject			�ՓˑΏە�
	//				inFrontRear			���g�̑O��i�s����
	//				inIntersectValue	���肷�鋗��
	//	�߂�l		TRUE				�Փ˂��Ă���
	//				FALSE				�Փ˂��Ă��Ȃ�
	//	�X�V		2008/12/04			<�V�K>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear, float inIntersectValue)
	{
	//	�܂��̓��b�Z�[�W�Ŕ���
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���
		LINE	line	= LINE(eUD_NO, eLR_NO, inFrontRear);	//!< �O�㔻�ʐ�p�̕����\���̂����

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	this->intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	�Փ˂܂ł̋������A�ړ��������Z����
			if(	fDist < inIntersectValue)
			{
			//	�n�ʂ���͂ݏo�����������i�[
				inObject->setDist(fDist);
			//	�Փ˂��������
				return TRUE;
			}
		}

	//	�Փ˂��Ȃ����������
		return FALSE;
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::draw
	//
	//	�@�\		�e���̂̕`����s��
	//	����		inDev			Direct3D�f�o�C�X
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::draw(CONST DEV inDev)
	{
	//	���݂��Ȃ���ԂȂ�A�`�悵�Ȃ�
		if(	!this->getVisible())	return FALSE;
	//	�e���̓���
		this->actionShadowObj();
	//	�e���̍\����[�e�����̂��Ȃ����]���ƁA�`�悵�Ȃ�
		if(	this->shadowObjType == eSHAO_NONE)	return FALSE;
	//	���f����O�����ɂ���
		MAT16	matrix = (*this->getWorldMatrixInversed()) * (*this->getWorldMatrix());

	//	�e���̂̕`��
		return	this->so_ptr->shadowObj->drawMesh(inDev, &matrix, &this->shadowDiffuse);
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::draw
	//
	//	�@�\		���b�V���̕`����s��(Z�e�N�X�`���V�F�[�_�g�p)
	//	����		inZTexSha		Z�e�N�X�`���V�F�[�_
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2007/12/14		<�V�K>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::draw(CZTEX_SHA* inZTexSha)
	{
	//	�e���̍\����[�e�����̂����镨��]�ȊO���ƁA�`�悵�Ȃ�
		if(	this->shadowObjType != eSHAO_COMPLETE)	return FALSE;
	//	�e���̓���
		this->actionShadowObj();
	//	���f����O�����ɂ���
		MAT16	matrix = (*this->getWorldMatrixInversed()) * (*this->getWorldMatrix());

	//	���[���h�s�������
		inZTexSha->setWorldMatrix(&matrix);
	//	���b�V���f�[�^�������₷������
		MESH_DATA*	inData	= this->so_ptr->shadowObj->getMeshs();
	//	�}�e���A���̐������`�������
		for(DWORD i = 0; i < inData->numMaterials; i++)
		{
			inZTexSha->beginPass();
			inData->mesh->DrawSubset(i);
			inZTexSha->endPass();
		}

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CSHADOW_OBJECT::draw
	//
	//	�@�\		���b�V���̕`����s��(�[�x�o�b�t�@�V�F�[�_�g�p)
	//	����		inDepthSha		�[�x�o�b�t�@�V�F�[�_
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2007/12/14		<�V�K>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::draw(CDEPTH_SHA* inDepthSha)
	{
	//	�e���̍\����[�e�����̂��Ȃ����]�ȊO���ƁA�`�悵�Ȃ�
		if(	this->shadowObjType == eSHAO_NONE)	return FALSE;
	//	�e���̓���
		this->actionShadowObj();
	//	���f����O�����ɂ���
		MAT16	matrix = (*this->getWorldMatrixInversed()) * (*this->getWorldMatrix());
	//	�e���̍\����[�e�����̂��Ȃ����]���ƁA�`�悵�Ȃ�
		if(	this->shadowObjType == eSHAO_COMPLETE)
		{
		//	���b�V���f�[�^�������₷������
			MESH_DATA*	inData	= this->so_ptr->shadowObj->getMeshs();
		//	�f�B�t���[�Y
			D3DCOLORVALUE 	keepDiffuse;
		//	���[���h�s�������
			inDepthSha->setWorldMatrix(&matrix);
		//	�p�����[�^�X�V
			inDepthSha->setParamToEffect();
		//	�}�e���A���̐������`�������
			for(DWORD i = 0; i < inData->numMaterials; i++)
			{
				inDepthSha->setEffectTexture(inData->texture[i]);
			//	�f�B�t���[�Y�̌v�Z
				keepDiffuse	= inData->materials[i].Diffuse;
				inData->materials[i].Diffuse	= this->shadowDiffuse;
				inDepthSha->setMaterialColor(&inData->materials[i].Diffuse);
			//	���b�V����`��
				inDepthSha->beginPass();
				inData->mesh->DrawSubset(i);
				inDepthSha->endPass();
			//	�f�B�t���[�Y�����ɖ߂�
				inData->materials[i].Diffuse	= keepDiffuse;
			}
		}
		else
		{
			inDepthSha->end();
		//	�e���̂̕`��
			this->so_ptr->shadowObj->drawMesh(
				inDepthSha->getDevice(), &matrix, &this->shadowDiffuse);
			inDepthSha->begin();
		}

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------