//***********************************************************************************
//!	�t�@�C����		Class_Mesh.cpp
//
//	�@�\			���b�V���I�u�W�F�N�g�Ɋւ������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2007/12/08	<�V�K>
//					2008/06/26	<�ǉ�>	�S�̓I�ɃR�����g�̋���
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_Mesh.h"

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CMESH::enumHitMesh
	//
	//	�@�\		�Փ˔�����s�� '�R�[���o�b�N�̂��߂̂���'
	//	�@�\�T�v	enumerate���\�b�h�̈����Ƃ��ēn�����ƃI�u�W�F�N�g�̐������Ă΂��B
	//				�������ɂ��̗񋓂����I�u�W�F�N�g���n�����B
	//				���b�V���i�U�������j�ƃ��C�i�U������j�̏Փ˔����
	//				���b�V���̃I�u�W�F�N�g���W�n�ōs����B
	//				���b�V���̋t�s����g�p���ă��C�̊J�n�ʒu�����߂�B
	//	����		inDefender		�U�������I�u�W�F�N�g
	//				inAttacker		�U������I�u�W�F�N�g
	//				inRoute			��(1.0)�A��(-1.0)
	//	�߂�l		TRUE			�Փ˂��Ă���
	//				FALSE			�Փ˂��Ă��Ȃ�
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	BOOL
	CMESH::enumHitMesh(CMESH* inDefender, CMESH* inAttacker, LINE* inLine)
	{
		return	inAttacker->intersect(inDefender, inLine);
	}

	//================================================================================
	//!	���\�b�h��	CMESH::�R���X�g���N�^
	//
	//	�@�\		X�t�@�C�������[�h���ď�����
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		���b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	CMESH::CMESH(CONST DEV   inDev,
				 CONST LPSTR inName,
				 CONST LPSTR inResource,
				 CONST LPSTR inTexture)		:
	CMESH_BASE(	inDev, inName, inResource, inTexture)
	{
	//	�X�[�p�[�N���X�̏�����
		C3D_OBJECT::initialize();
	//	�������b�V���̓ǂݍ��ݎ��s���Ă�����A���݊m�F��OFF�ɂ���
		if(	!bMeshLoaded)	setVisible(FALSE);
	}

	//================================================================================
	//!	���\�b�h��	CMESH::�R���X�g���N�^
	//
	//	�@�\		�󏉊���
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	CMESH::CMESH(void)		:
	CMESH_BASE()
	{
	//	�X�[�p�[�N���X�̏�����
		C3D_OBJECT::initialize();
	}

	//================================================================================
	//!	���\�b�h��	CMESH::�R���X�g���N�^
	//
	//	�@�\		�R�s�[
	//	����		inCopy			�R�s�[��
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	CMESH::CMESH(CMESH* inCopy)		:
	CMESH_BASE(inCopy)
	{
	//	�X�[�p�[�N���X�̏�����
		C3D_OBJECT::initialize();
	//	�������b�V���̓ǂݍ��ݎ��s���Ă�����A���݊m�F��OFF�ɂ���
		if(	!bMeshLoaded)	setVisible(FALSE);
	}

	//================================================================================
	//!	���\�b�h��	CMESH::�f�X�g���N�^
	//
	//	�@�\		�S�Ă̎�����������A�I������B
	//				�������A���̃I�u�W�F�N�g�ł��g���Ă����ꍇ�͎c���B
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	CMESH::~CMESH()
	{
	//	�X�[�p�[�N���X�̊J��
		C3D_OBJECT::finalize();
	}

	//================================================================================
	//!	���\�b�h��	CMESH::getWorldCenter
	//
	//	�@�\		���[���h���W�n���S�_���󂯎��
	//	�߂�l		���[���h���W�n���S�_
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	VEC3
	CMESH::getWorldCenter(void)
	{
	//	���[���h���W�n���S�_
		VEC3	aCenter;
	//	���[���h���W�n�̉��Z
		D3DXVec3TransformCoord(&aCenter, &md_ptr->localCenter, getWorldMatrix());
	//	���[���h���W�n���S�_��Ԃ�
		return	aCenter;
	}

	//================================================================================
	//!	���\�b�h��	CMESH::intersectMesh
	//
	//	�@�\		���g��X���b�V���Ƃ̏Փ˂����o����
	//	����		*inObject		�ՓˑΏە�
	//				*inRayLoc		���C�̎n�_
	//				*inLine			���g�̐i�s����
	//				*inDist			���C�̎n�_����A�Փ˒n�_�܂ł̋���
	//	�߂�l		TRUE			�Փ˂��Ă���
	//				FALSE			�Փ˂��Ă��Ȃ�
	//	�X�V		2008/08/24		<�V�K>
	//================================================================================
	BOOL
	CMESH::intersectMesh(C3D_OBJECT* inObject, VEC3* inRayLoc, LINE* inLine, float* inDist)
	{
		BOOL	bHit			= FALSE;					//!< �߂�l
		DWORD	dwFaceIndex		= 0;						//!< ���C�̎n�_�ɍł��߂��ʂ̃C���f�b�N�X
		FLOAT	fU				= 0.0f;						//!< �d�S�q�b�g���WU
		FLOAT	fV				= 0.0f;						//!< �d�S�q�b�g���WV
		VEC3	rayDirection	= inObject->calcRay(inLine);//!< ���C�̕���

	//	�U�����郌�C�̈ʒu���U���������W�n�ɍ��킹��
		rayFromInversedMatrix(inRayLoc, &rayDirection);

	//	���C�̕����͕K�����K������
		D3DXVec3Normalize(&rayDirection, &rayDirection);	//!< ���K��(0.0 or 1.0�Ɏ�������)

	//	���b�V���ƃ��C�Ƃ̏Փ˔���
		D3DXIntersect(
			md_ptr->mesh,									//!< �ՓˑΏە�
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
	//!	���\�b�h��	CMESH::intersect�i�S�����Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject		�ՓˑΏە�
	//				inLine			���g�̐i�s����
	//	�߂�l		TRUE			�Փ˂��Ă���
	//				FALSE			�Փ˂��Ă��Ȃ�
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE* inLine)
	{
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!intersectRadius(inObject))	return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	intersectMesh(inObject, &inObject->getLoc(), inLine, &fDist))
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
	//!	���\�b�h��	CMESH::intersect�i�㉺�Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject		�ՓˑΏە�
	//				inUpDown		���g�̏㉺�i�s����
	//	�߂�l		TRUE			�Փ˂��Ă���
	//				FALSE			�Փ˂��Ă��Ȃ�
	//	�X�V		2008/08/24		<�V�K>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown)
	{
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!intersectRadius(inObject))	return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���
		LINE	line	= LINE(inUpDown, eLR_NO, eFR_NO);		//!< �㉺���ʐ�p�̕����\���̂����

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
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
	//!	���\�b�h��	CMESH::intersect�i���E�Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject		�ՓˑΏە�
	//				inLeftRight		���g�̍��E�i�s����
	//	�߂�l		TRUE			�Փ˂��Ă���
	//				FALSE			�Փ˂��Ă��Ȃ�
	//	�X�V		2008/08/24		<�V�K>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight)
	{
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!intersectRadius(inObject))	return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���
		LINE	line	= LINE(eUD_NO, inLeftRight, eFR_NO);	//!< ���E���ʐ�p�̕����\���̂����

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
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
	//!	���\�b�h��	CMESH::intersect�i�O��Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject		�ՓˑΏە�
	//				inFrontRear		���g�̑O��i�s����
	//	�߂�l		TRUE			�Փ˂��Ă���
	//				FALSE			�Փ˂��Ă��Ȃ�
	//	�X�V		2008/08/24		<�V�K>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear)
	{
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!intersectRadius(inObject))	return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���
		LINE	line	= LINE(eUD_NO, eLR_NO, inFrontRear);	//!< �O�㔻�ʐ�p�̕����\���̂����

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
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
	//!	���\�b�h��	CMESH::intersect�i�����w��A�S�����Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject			�ՓˑΏە�
	//				inLine				���g�̐i�s����
	//				inIntersectValue	���肷�鋗��
	//	�߂�l		TRUE				�Փ˂��Ă���
	//				FALSE				�Փ˂��Ă��Ȃ�
	//	�X�V		2008/08/24			<�V�K>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE* inLine, float inIntersectValue)
	{
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!intersectRadius(inObject))	return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	intersectMesh(inObject, &inObject->getLoc(), inLine, &fDist))
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
	//!	���\�b�h��	CMESH::intersect�i�����w��A�㉺�Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject			�ՓˑΏە�
	//				inUpDown			���g�̏㉺�i�s����
	//				inIntersectValue	���肷�鋗��
	//	�߂�l		TRUE				�Փ˂��Ă���
	//				FALSE				�Փ˂��Ă��Ȃ�
	//	�X�V		2008/08/24			<�V�K>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown, float inIntersectValue)
	{
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!intersectRadius(inObject))	return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���
		LINE	line	= LINE(inUpDown, eLR_NO, eFR_NO);		//!< �㉺���ʐ�p�̕����\���̂����

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
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
	//!	���\�b�h��	CMESH::intersect�i�����w��A���E�Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject			�ՓˑΏە�
	//				inLeftRight			���g�̍��E�i�s����
	//				inIntersectValue	���肷�鋗��
	//	�߂�l		TRUE				�Փ˂��Ă���
	//				FALSE				�Փ˂��Ă��Ȃ�
	//	�X�V		2008/08/24			<�V�K>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight, float inIntersectValue)
	{
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!intersectRadius(inObject))	return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���
		LINE	line	= LINE(eUD_NO, inLeftRight, eFR_NO);	//!< ���E���ʐ�p�̕����\���̂����

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
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
	//!	���\�b�h��	CMESH::intersect�i�����w��A�O��Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject			�ՓˑΏە�
	//				inFrontRear			���g�̑O��i�s����
	//				inIntersectValue	���肷�鋗��
	//	�߂�l		TRUE				�Փ˂��Ă���
	//				FALSE				�Փ˂��Ă��Ȃ�
	//	�X�V		2008/08/24			<�V�K>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear, float inIntersectValue)
	{
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!intersectRadius(inObject))	return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���
		LINE	line	= LINE(eUD_NO, eLR_NO, inFrontRear);	//!< �O�㔻�ʐ�p�̕����\���̂����

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
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
	//!	���\�b�h��	CMESH::intersectPos�i����J�n�ʒu�w��A�����w��A�S�����Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject			�ՓˑΏە�
	//				inLoc				����J�n�ʒu
	//				inLine				���g�̐i�s����
	//				inIntersectValue	���肷�鋗��
	//	�߂�l		TRUE				�Փ˂��Ă���
	//				FALSE				�Փ˂��Ă��Ȃ�
	//	�X�V		2008/08/24			<�V�K>
	//================================================================================
	BOOL
	CMESH::intersectPos(C3D_OBJECT*		inObject,
						VEC3*			inLoc,
						LINE*			inLine,
						float			inIntersectValue)
	{
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!intersectRadius(inObject))	return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	intersectMesh(inObject, inLoc, inLine, &fDist))
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
	//!	���\�b�h��	CMESH::intersectPos�i����J�n�ʒu�w��A�����w��A�㉺�Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject			�ՓˑΏە�
	//				inLoc				����J�n�ʒu
	//				inUpDown			���g�̏㉺�i�s����
	//				inIntersectValue	���肷�鋗��
	//	�߂�l		TRUE				�Փ˂��Ă���
	//				FALSE				�Փ˂��Ă��Ȃ�
	//	�X�V		2008/08/24			<�V�K>
	//================================================================================
	BOOL
	CMESH::intersectPos(C3D_OBJECT*		inObject,
						VEC3*			inLoc,
						LINE_UPDOWN		inUpDown,
						float			inIntersectValue)
	{
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!intersectRadius(inObject))	return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���
		LINE	line	= LINE(inUpDown, eLR_NO, eFR_NO);		//!< �㉺���ʐ�p�̕����\���̂����

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	intersectMesh(inObject, inLoc, &line, &fDist))
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
	//!	���\�b�h��	CMESH::intersectPos�i����J�n�ʒu�w��A�����w��A���E�Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject			�ՓˑΏە�
	//				inLoc				����J�n�ʒu
	//				inLeftRight			���g�̍��E�i�s����
	//				inIntersectValue	���肷�鋗��
	//	�߂�l		TRUE				�Փ˂��Ă���
	//				FALSE				�Փ˂��Ă��Ȃ�
	//	�X�V		2008/08/24			<�V�K>
	//================================================================================
	BOOL
	CMESH::intersectPos(C3D_OBJECT*		inObject,
						VEC3*			inLoc,
						LINE_LEFTRIGHT	inLeftRight,
						float			inIntersectValue)
	{
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!intersectRadius(inObject))	return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���
		LINE	line	= LINE(eUD_NO, inLeftRight, eFR_NO);	//!< ���E���ʐ�p�̕����\���̂����

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	intersectMesh(inObject, inLoc, &line, &fDist))
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
	//!	���\�b�h��	CMESH::intersectPos�i����J�n�ʒu�w��A�����w��A�O��Łj
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂����o����
	//	����		inObject			�ՓˑΏە�
	//				inLoc				����J�n�ʒu
	//				inFrontRear			���g�̑O��i�s����
	//				inIntersectValue	���肷�鋗��
	//	�߂�l		TRUE				�Փ˂��Ă���
	//				FALSE				�Փ˂��Ă��Ȃ�
	//	�X�V		2008/08/24			<�V�K>
	//================================================================================
	BOOL
	CMESH::intersectPos(C3D_OBJECT*		inObject,
						VEC3*			inLoc,
						LINE_FRONTREAR	inFrontRear,
						float			inIntersectValue)
	{
	//	���a�ł̏����Փ˂��m�F���A�Փˌ��ʂ��o����A�����Ō��ʂ�Ԃ�
		if(	!intersectRadius(inObject))	return	FALSE;

	//	�����܂ŗ�����A���b�V���Ƃ̔�����s��
		float	fDist	= 0.0f;									//!< ���C�̎n�_����ʂ̌�_�܂ł̋���
		LINE	line	= LINE(eUD_NO, eLR_NO, inFrontRear);	//!< �O�㔻�ʐ�p�̕����\���̂����

	//	���b�V���Ƃ̏Փ˔��肵�A�Փ˂�����
		if(	intersectMesh(inObject, inLoc, &line, &fDist))
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
	//!	���\�b�h��	CMESH::draw
	//
	//	�@�\		���b�V���̕`����s��
	//	����		inDev			Direct3D�f�o�C�X
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	BOOL
	CMESH::draw(CONST DEV inDev)
	{
	//	���݂��Ȃ���ԂȂ�A�`�悵�Ȃ�
		if(	!getVisible())	return FALSE;

	//	���f����O�����ɂ���
		MAT16	matrix = (*getWorldMatrixInversed()) * (*getWorldMatrix());

	//	���b�V���̕`��
		return	drawMesh(inDev, &matrix, getDiffuseAlpha());
	}

	//================================================================================
	//!	���\�b�h��	CMESH::draw
	//
	//	�@�\		���b�V���̕`����s��(Z�e�N�X�`���V�F�[�_�g�p)
	//	����		inZTexSha		Z�e�N�X�`���V�F�[�_
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	BOOL
	CMESH::draw(CZTEX_SHA* inZTexSha)
	{
	//	���f����O�����ɂ���
		MAT16	matrix = (*getWorldMatrixInversed()) * (*getWorldMatrix());

	//	���[���h�s�������
		inZTexSha->setWorldMatrix(&matrix);
	//	�}�e���A���̐������`�������
		for(DWORD i = 0; i < md_ptr->numMaterials; i++)
		{
			inZTexSha->beginPass();
			md_ptr->mesh->DrawSubset(i);
			inZTexSha->endPass();
		}

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CMESH::draw
	//
	//	�@�\		���b�V���̕`����s��(�[�x�o�b�t�@�V�F�[�_�g�p)
	//	����		inDepthSha		�[�x�o�b�t�@�V�F�[�_
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	BOOL
	CMESH::draw(CDEPTH_SHA* inDepthSha)
	{
	//	���f����O�����ɂ���
		MAT16	matrix = (*getWorldMatrixInversed()) * (*getWorldMatrix());
	//	���[���h�s�������
		inDepthSha->setWorldMatrix(&matrix);
	//	�f�o�C�X�������₷������
		DEV	inDev	= inDepthSha->getDevice();
	//	�p�����[�^�X�V
		inDepthSha->setParamToEffect();
	//	�}�e���A���̐������`�������
		for(DWORD i = 0; i < md_ptr->numMaterials; i++)
		{
		//	�e�N�X�`�����Z�b�g
			inDepthSha->setEffectTexture(md_ptr->texture[i]);
		//	�}�e���A�����Z�b�g
			inDepthSha->setMaterialColor(&md_ptr->materials[i].Diffuse);
			inDepthSha->beginPass();
			md_ptr->mesh->DrawSubset(i);
			inDepthSha->endPass();
		}

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//----------------------------------fcccccc-------------------------------------