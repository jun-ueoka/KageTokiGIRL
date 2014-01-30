//***********************************************************************************
//!	�t�@�C����		Class_MeshBase.cpp
//
//	�@�\			���b�V���x�[�X�Ɋւ����`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/08/30	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_MeshBase.h"				//!< ���b�V���x�[�X

//-----------------------------------------------------------------------
//!	���b�V���x�[�X�̖��O���
//-----------------------------------------------------------------------
namespace MB
{
	//================================================================================
	//	���L���b�V���}�b�v
	//================================================================================
	map<string, MESH_DATA*>*	CMESH_BASE::mMeshs = NULL;

	//================================================================================
	//!	���\�b�h��	CMESH_BASE::createMeshInData
	//
	//	�@�\		X�t�@�C�������[�h���A���b�V�����𐶐�
	//	�@�\�T�v	���b�V���������[�h���A���ʂ�Ԃ�
	//				���̃��\�b�h�̓N���X���\�b�h�ł���
	//	����		inDev			Direct3D�f�o�C�X
	//				inMeshData		���[�h�惁�b�V���f�[�^
	//				inName			X�t�@�C���̖���
	//				inResource		���b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�߂�l		TRUE			��������
	//				FALSE			�������s
	//	�X�V		2009/03/02		<�V�K>
	//================================================================================
	BOOL
	CMESH_BASE::createMeshInData(CONST DEV		inDev,
								 MESH_DATA*		inMeshData,
								 CONST LPSTR	inName,
								 CONST LPSTR	inResource,
								 CONST LPSTR	inTexture)
	{
		LPD3DXBUFFER	mtrlBuffer			= NULL;				//!< �}�e���A���o�b�t�@
		LPD3DXBUFFER	adjacencyBuffer		= NULL;				//!< �אڃo�b�t�@
		D3DXMATERIAL	*materials			= NULL;				//!< �}�e���A��

		TCHAR		realName[MAX_PATH]		= "";				//!< ���O�Ƀp�X�������

	//	���b�V���f�[�^���������
		inMeshData->releaseData();

	//	�ǂݍ���ł݂�
		if(	FAILED(D3DXLoadMeshFromX(
				inName,
				D3DXMESH_SYSTEMMEM,
				inDev,
				&adjacencyBuffer,
				&mtrlBuffer,
				NULL,
				&inMeshData->numMaterials,
				&inMeshData->mesh)))
		{
		//	���s������A���\�[�X�t�@�C���ւ̃p�X�����ăg���C
			sprintf_s(realName, sizeof(realName), inResource, inName);

			if(	FAILED(D3DXLoadMeshFromX(
					realName,
					D3DXMESH_SYSTEMMEM,
					inDev,
					&adjacencyBuffer,
					&mtrlBuffer,
					NULL,
					&inMeshData->numMaterials,
					&inMeshData->mesh)))
			{
			//	���s������ANULL��Ԃ�
				goto error;
			}
		}

	//	���b�V���̍œK��
		if(	FAILED(inMeshData->mesh->OptimizeInplace(
						D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
						(DWORD*)adjacencyBuffer->GetBufferPointer(),
						NULL,
						NULL,
						NULL)))
		{
		//	���b�V���̍œK�����ł��Ȃ���΁A�I��
			SAFE_RELEASE(adjacencyBuffer);	//!< �אڃo�b�t�@���J��
			SAFE_RELEASE(mtrlBuffer);		//!< �}�e���A���o�b�t�@
			goto error;						//!< NULL�Ԃ�
		}

	//	�@�����̊m�F
		DWORD currentFVF = 0, newFVF = 0, normalFVFV = D3DFVF_NORMAL;

	//	���݂�FVF���擾����
		currentFVF = inMeshData->mesh->GetFVF();

	//	�@����񂪖���������A���b�V�����쐬���Ȃ���
		if(	!(currentFVF && D3DFVF_NORMAL))
		{
			XMESH cloneMesh = NULL;		//!< �V���ɍ쐬�������b�V���̊i�[��
		//	���݂�FVF�ɁAFVF�̏����ݒ���|�����킹��
			newFVF = currentFVF | D3DFVF_CLONEVERTEX;
		//	�V����FVF�ŁA�V���ȃ��b�V���̍쐬
			inMeshData->mesh->CloneMeshFVF(
				inMeshData->mesh->GetOptions(), newFVF, inDev, &cloneMesh);
		//	�O�̃��b�V����j��
			SAFE_RELEASE(inMeshData->mesh);
		//	�V���ȃ��b�V�����i�[
			inMeshData->mesh = cloneMesh;
		}

	//	�@�����v�Z����
	//	3DCG�c�[���ɂ���Ă͖@�����������Ȃ��ꍇ�����邽��.
		D3DXComputeNormals(inMeshData->mesh, NULL);

	//	�}�e���A���o�b�t�@�|�C���^�[���擾
		materials = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

	//	���b�V���̃}�e���A���̐������A�}�e���A���ƃe�N�X�`�����쐬
		inMeshData->materials	= new MTR[inMeshData->numMaterials];
		inMeshData->texture	= new TEX[inMeshData->numMaterials];

	//	���ׂẴ}�e���A���̐ݒ�
		for(DWORD i = 0; i < inMeshData->numMaterials; i++)
		{
		//	�}�e���A���̃v���p�f�B���i�[
			inMeshData->materials[i]			= materials[i].MatD3D;
		//	�A���r�G���g�F�ƃf�B�t���[�Y�F�𓯊�
			inMeshData->materials[i].Ambient	= inMeshData->materials[i].Diffuse;
		//	�}�e���A���Ƀe�N�X�`���������ꍇ
			if(	materials[i].pTextureFilename == NULL)
			{
			//	���݌��Ă���}�e���A���ƁA�����z��v�f�ԍ������e�N�X�`����NULL�����āA���̃��[�v��
				inMeshData->texture[i] = NULL;
				continue;
			}
		//	�e�N�X�`���𐶐�
			if(	FAILED(D3DXCreateTextureFromFile(inDev,
												 materials[i].pTextureFilename,
												 &inMeshData->texture[i])))
			{
			//	���s������A���b�V�����\�[�X�t�@�C���ւ̃p�X�����ăg���C
				sprintf_s(realName, sizeof(realName), inTexture, materials[i].pTextureFilename);

				if(	FAILED(D3DXCreateTextureFromFile(inDev,
													 realName,
													 &inMeshData->texture[i])))
				{
				//	���s������ANULL��Ԃ�
					goto error;
				}
			}
		}
		SAFE_RELEASE(adjacencyBuffer);	//!< �אڃo�b�t�@���J��
		SAFE_RELEASE(mtrlBuffer);		//!< �}�e���A���o�b�t�@

	//	���_�o�b�t�@�𐶐�����
		if(	FAILED(inMeshData->mesh->GetVertexBuffer(&inMeshData->meshVB)))
		{
		//	���s������ANULL��Ԃ�
			goto error;
		}

	//	�o�E���f�B���O�E�X�t�B�A�����߂�
		BYTE* pv = NULL;
	//	���_�o�b�t�@�������ւ̃|�C���^���擾
		if(	inMeshData->mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pv) == D3D_OK)
		{
		//	���b�V���̋��E�����Z�o����
			D3DXComputeBoundingSphere(
				(VEC3*)pv,											//!< �ŏ��̈ʒu�ւ̃|�C���^
				inMeshData->mesh->GetNumVertices(),					//!< ���_�̐�
				D3DXGetFVFVertexSize(inMeshData->mesh->GetFVF()),	//!< ���_�̃T�C�Y
				&inMeshData->localCenter,							//!< �Ԃ���鋫�E���̍��W�̒��S
				&inMeshData->localRadius);							//!< �Ԃ���鋫�E���̔��a
		//	���_���A�����b�N
			inMeshData->mesh->UnlockVertexBuffer();
		}

	//	���b�V���������b�V�����ɓo�^
		inMeshData->name	= inName;
		inMeshData->rPath	= inResource;
		inMeshData->tPath	= inTexture;

		return	TRUE;

	//	NULL��Ԃ�����
	error:
		inMeshData->releaseData();
		DX::errorMsgNoFile(inDev, realName);
		return	FALSE;				//!< NULL��Ԃ�
	}

	//================================================================================
	//!	���\�b�h��	CMESH_BASE::createMesh
	//
	//	�@�\		���b�V�����𐶐����A���������f�[�^��Ԃ�
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		���b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�߂�l		���b�V�����̃|�C���^�[
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	MESH_DATA*
	CMESH_BASE::createMesh(	CONST DEV	inDev,
							CONST LPSTR	inName,
							CONST LPSTR	inResource,
							CONST LPSTR	inTexture)
	{
		MESH_DATA*	newMesh	= new MESH_DATA();	//!< �V���ȃ��b�V�����

	//	���b�V���𐶐����A���s������NULL��Ԃ�
		if(	!createMeshInData(inDev, newMesh, inName, inResource, inTexture))
		{
			SAFE_DELETE(newMesh);
			return	NULL;
		}
		return	newMesh;
	}

	//================================================================================
	//!	���\�b�h��	CMESH_BASE::startMeshInitialize
	//
	//	�@�\		�����̏�����
	//	�X�V		2008/08/30		<�V�K>
	//================================================================================
	void
	CMESH_BASE::startMeshInitialize(void)
	{
		md_ptr		= NULL;			//!< ���b�V�����
		bMeshLoaded	= FALSE;		//!< ���b�V�����[�h�m�F
	}

	//================================================================================
	//!	���\�b�h��	CMESH_BASE::initializeMesh
	//
	//	�@�\		���b�V���̏�����
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		���b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	void
	CMESH_BASE::initializeMesh(	CONST DEV	inDev,
								CONST LPSTR inName,
								CONST LPSTR	inResource,
								CONST LPSTR	inTexture)
	{
	//	���b�V���̏������
		finishalizeMesh();

	//	���b�V���}�b�v��������ΐ�������
		if(	!mMeshs)
		{
			mMeshs = new map<string, MESH_DATA*>;
		}

	//	��Ɨp�C�e���[�^
		map<string, MESH_DATA*>::iterator	p;
	//	���b�V���}�b�v��X�t�@�C������n���A���̃C�e���[�^���󂯎��
		p = mMeshs->find(inName);

	//	���b�V���}�b�v�ɓ������O��X�t�@�C���������ɓo�^���Ă���ꍇ
		if(	p != mMeshs->end())
		{
		//	�g�p�J�E���^�𑝂₷
			p->second->cnt++;
		//	���b�V������Ԃ�
			md_ptr		= (*p).second;
		//	���[�h����
			bMeshLoaded	= TRUE;
		}
		else
		{
		//	�V���Ƀ��b�V���𐶐�����
			md_ptr	= createMesh(inDev, inName, inResource, inTexture);

		//	�����ɐ��������ꍇ
			if(	md_ptr)
			{
			//	�g�p�J�E���^�𑝂₷
				md_ptr->cnt++;
			//	���[�h����
				bMeshLoaded	= TRUE;
			//	���b�V���}�b�v�ɓo�^
				mMeshs->insert(pair<string, MESH_DATA*>(md_ptr->name, md_ptr));
			}
		//	���s�����ꍇ
			else
			{
			//	���[�h������
				bMeshLoaded	= FALSE;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CMESH_BASE::finishalizeMesh
	//
	//	�@�\		���b�V���̉��
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	void
	CMESH_BASE::finishalizeMesh(void)
	{
	//	���b�V���������[�h���ĂȂ��Ȃ�A������Ԃ�
		if(	!bMeshLoaded)	return;

	//	�t���O�����ǂ��Ă���
		bMeshLoaded	= FALSE;

	//	���b�V���J�E���g�����炷
		md_ptr->cnt--;
	//	���̃��b�V�����܂��g�p����Ă���ꍇ�͉�����Ȃ�
		if(	md_ptr->cnt > 0)	return;

	//	���b�V���}�b�v�C�e���[�^
		map<string, MESH_DATA*>::iterator	p;
	//	���b�V���}�b�v��X�t�@�C������n���A���̃C�e���[�^���󂯎��
		p = mMeshs->find(md_ptr->name);
	//	���b�V�����̉��
		SAFE_DELETE(p->second);
	//	���b�V���}�b�v����폜
		mMeshs->erase(p);

	//	���b�V���}�b�v�Ƀ����o�[��������΁A��������
		if(	mMeshs->begin() == mMeshs->end())
		{
			SAFE_DELETE(mMeshs);
		}
	}

	//================================================================================
	//!	���\�b�h��	CMESH_BASE::�R���X�g���N�^
	//
	//	�@�\		X�t�@�C�������[�h���ď�����
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		���b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	CMESH_BASE::CMESH_BASE(	CONST DEV	inDev,
							CONST LPSTR	inName,
							CONST LPSTR	inResource,
							CONST LPSTR	inTexture)
	{
	//	�����̏�����
		startMeshInitialize();
	//	���b�V���̏�����
		initializeMesh(inDev, inName, inResource, inTexture);
	}

	//================================================================================
	//!	���\�b�h��	CMESH_BASE::�R���X�g���N�^
	//
	//	�@�\		�󏉊���
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	CMESH_BASE::CMESH_BASE(void)
	{
	//	�����̏�����
		startMeshInitialize();
	}

	//================================================================================
	//!	���\�b�h��	CMESH_BASE::�R���X�g���N�^
	//
	//	�@�\		�R�s�[
	//	����		inCopy			�R�s�[��
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	CMESH_BASE::CMESH_BASE(CMESH_BASE* inCopy)
	{
	//	�����̏�����
		startMeshInitialize();
	//	�R�s�[�J�n
		md_ptr	= inCopy->getMeshs();

	//	���b�V����񂪂��鎞
		if(	md_ptr)
		{
		//	�g�p�J�E���^�𑝂₷
			md_ptr->cnt++;
		//	���[�h����
			bMeshLoaded	= TRUE;
		}
	}

	//================================================================================
	//!	���\�b�h��	CMESH_BASE::�f�X�g���N�^
	//
	//	�@�\		�S�Ă̎�����������A�I������B
	//				�������A���̃I�u�W�F�N�g�ł��g���Ă����ꍇ�͎c���B
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	CMESH_BASE::~CMESH_BASE()
	{
	//	���b�V���̉��
		finishalizeMesh();
	}

	//================================================================================
	//!	���\�b�h��	CMESH_BASE::resetInstance
	//
	//	�@�\		���Z�b�g���s
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2009/03/02		<�V�K>
	//================================================================================
	void
	CMESH_BASE::resetInstance(CONST DEV inDev)
	{
	//	���b�V���}�b�v���Ȃ���΁A���Z�b�g���Ȃ�
		if(	!mMeshs)	return;

	//	�ǂݍ��񂾎��̏������o����
		char	inName[MAX_PATH]	= "";
		char	inRPath[MAX_PATH]	= "";
		char	inTPath[MAX_PATH]	= "";

	//	����������
		MAP_LOOP(string, MESH_DATA*, (*mMeshs))
		{
		//	���̃f�[�^��ێ�����
			sprintf_s(inName,   sizeof(inName),  (*p).second->name.c_str());
			sprintf_s(inRPath,  sizeof(inRPath), (*p).second->rPath.c_str());
			sprintf_s(inTPath,  sizeof(inTPath), (*p).second->tPath.c_str());
		//	���̃f�[�^�œǂ݂��݂�����
			createMeshInData(inDev, (*p).second, inName, inRPath, inTPath);
		}
	}

	//================================================================================
	//!	���\�b�h��	CMESH_BASE::meshInvalid
	//
	//	�@�\		���b�V���̖������𒲂ׂ�
	//	�߂�l		TRUE			����
	//				FALSE			�L��
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	BOOL
	CMESH_BASE::meshInvalid(void)
	{
		if(	(!bMeshLoaded)	||	//!< ���b�V���������[�h���ĂȂ���
			(!md_ptr->mesh))		//!< ���́A���b�V�������݂��Ȃ���
		{
			return TRUE;				//!< ����
		}
		else							//!< ��L�ɓ��Ă͂܂�Ȃ��Ȃ�
		{
			return FALSE;				//!< �L��
		}
	}

	//================================================================================
	//!	���\�b�h��	CMESH_BASE::drawMesh
	//
	//	�@�\		���b�V���̕`����s��(�A���t�@���Z)
	//	����		inDev			Direct3D�f�o�C�X
	//				inMatrix		���[���h�}�g���b�N�X
	//				inCalcDiffuse	�����f�B�t���[�Y�l
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	BOOL
	CMESH_BASE::drawMesh(CONST DEV inDev, MAT16* inMatrix, float inCalcDiffuse)
	{
	//	�`��̖�����������Ȃ�A�`�悵�Ȃ�
		if(	meshInvalid())	return	FALSE;

	//	�f�B�t���[�Y
		float	fKeepDiffuse	= 0.0f;
	//	���[���h�s�������
		inDev->SetTransform(D3DTS_WORLD, inMatrix);
	//	�}�e���A���̐������`�������
		for(DWORD i = 0; i < md_ptr->numMaterials; i++)
		{
		//	�e�N�X�`�����Z�b�g
			inDev->SetTexture(0, md_ptr->texture[i]);
		//	�}�e���A�����Z�b�g
		//	�f�B�t���[�Y�̌v�Z
			fKeepDiffuse	= md_ptr->materials[i].Diffuse.a;
			md_ptr->materials[i].Diffuse.a += inCalcDiffuse;
			inDev->SetMaterial(&md_ptr->materials[i]);
		//	���b�V����`��
			md_ptr->mesh->DrawSubset(i);
		//	�f�B�t���[�Y�����ɖ߂�
			md_ptr->materials[i].Diffuse.a  = fKeepDiffuse;
		}

	//	�e�N�X�`���̃|�C���^�[��'NULL'�ŏ㏑��
		inDev->SetTexture(0, NULL);

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CMESH_BASE::drawMesh
	//
	//	�@�\		���b�V���̕`����s��(�f�B�t���[�Y�ݒ�)
	//	����		inDev			Direct3D�f�o�C�X
	//				inMatrix		���[���h�}�g���b�N�X
	//				inDiffuse		�����f�B�t���[�Y�l
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2008/12/08		<�V�K>
	//================================================================================
	BOOL
	CMESH_BASE::drawMesh(CONST DEV inDev, MAT16* inMatrix, D3DCOLORVALUE* inDiffuse)
	{
	//	�`��̖�����������Ȃ�A�`�悵�Ȃ�
		if(	meshInvalid())	return	FALSE;

	//	�f�B�t���[�Y
		D3DCOLORVALUE 	keepDiffuse;
	//	���[���h�s�������
		inDev->SetTransform(D3DTS_WORLD, inMatrix);
	//	�}�e���A���̐������`�������
		for(DWORD i = 0; i < md_ptr->numMaterials; i++)
		{
		//	�e�N�X�`�����Z�b�g
			inDev->SetTexture(0, md_ptr->texture[i]);
		//	�}�e���A�����Z�b�g
		//	�f�B�t���[�Y�̌v�Z
			keepDiffuse	= md_ptr->materials[i].Diffuse;
			md_ptr->materials[i].Diffuse	= *inDiffuse;
			inDev->SetMaterial(&md_ptr->materials[i]);
		//	���b�V����`��
			md_ptr->mesh->DrawSubset(i);
		//	�f�B�t���[�Y�����ɖ߂�
			md_ptr->materials[i].Diffuse	= keepDiffuse;
		}

	//	�e�N�X�`���̃|�C���^�[��'NULL'�ŏ㏑��
		inDev->SetTexture(0, NULL);

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	���b�V���x�[�X�̖��O���
//-----------------------------------------------------------------------