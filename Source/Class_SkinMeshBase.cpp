//***********************************************************************************
//!	�t�@�C����		Class_SkinMeshBase.cpp
//
//	�@�\			�X�L�����b�V���x�[�X�Ɋւ������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/08/30	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SkinMeshBase.h"

//-----------------------------------------------------------------------
//!	�X�L�����b�V���x�[�X�̖��O���
//-----------------------------------------------------------------------
namespace SMB
{
	//================================================================================
	//	���L�X�L���}�b�v
	//================================================================================
	map<string, SKIN_DATA*>*	CSKIN_MESH_BASE::mSkins	= NULL;

	//================================================================================
	//!	���\�b�h��	MY_HIERARCHY::CreateFrame
	//
	//	�@�\		�t���[���̍쐬
	//	�@�\�T�v	�V�����t���[�����쐬����B
	//				�t���[���Ƃ̓��b�V�����̃I�u�W�F�N�g���K�w���Ƃɕ��������̂ŁA
	//				�X�L�����b�V���̏ꍇ�̓{�[���ɑ�������B
	//	����		Name			�V������肽���t���[���̖��O
	//				ppNewFrame		�V�������������t���[���̊i�[��ւ̃|�C���^
	//	�߂�l		HRESULT			�e�����̃G���[
	//	�X�V		2008/05/30		<�ǉ�>	�R�����g
	//================================================================================
	HRESULT
	MY_HIERARCHY::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
	{
		MYFRAME	*pFrame;			//!< ���[�J���Ƀt���[�����쐬�����

	//	�����Ƃ��ēn���ė����V�����t���[���̊i�[���NULL�ɂ��Ă����i�O�̂��߁c�j
		*ppNewFrame = NULL;

	//	�܂��̓��[�J���Ƀt���[���𐶐�
		pFrame = new MYFRAME();

	//	�����ł��Ȃ�������A�������[�s����Ԃ�
		if(	!pFrame)		return E_OUTOFMEMORY;

	//	�����Đ��������t���[���ɖ��O��t����
		pFrame->Name = new TCHAR[lstrlen(Name)+1];					//!< ���O�̈���m��

	//	���O�̈���m�ۂł��Ȃ������玸�s
		if(	!pFrame->Name)	return E_FAIL;

	//	�����Ƃ��ēn���ė����V�������O���`�F�b�N���A���Ȃ���Α������
		AllocateName(Name, &pFrame->Name);

	//	���������t���[���̊e�v�f������������
		D3DXMatrixIdentity(&pFrame->TransformationMatrix);			//!< ���[���h�ϊ��s���������
		D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);	//!< �ŏI���[���h�ϊ��s���������
		pFrame->pMeshContainer		= NULL;							//!< ���b�V���R���e�i�[�ւ̃|�C���^��������
		pFrame->pFrameSibling		= NULL;							//!< �Z��t���[���ւ̃|�C���^��������
		pFrame->pFrameFirstChild	= NULL;							//!< �q�t���[���ւ̃|�C���^��������

	//	�V����������t���[�����Ăяo�����ɓn��
		*ppNewFrame	= pFrame;

	//	�����܂ŗ����ΐ���I��
		return S_OK;
	}

	//================================================================================
	//!	�֐���		AllocateName
	//
	//	�@�\		������̔���
	//	�@�\�T�v	�iCreateFrame���\�b�h�Ȃǂɂ����āj�ُ�ȕ�����̓R�s�[�����ɒe��
	//	����		inName			�R�s�[������������
	//				ioNewName		�R�s�[�����̕�����
	//	�߂�l		HRESULT			�e�����̃G���[
	//	�X�V		2008/05/30		<�ǉ�>	�R�����g
	//================================================================================
	HRESULT
	AllocateName(LPCSTR inName, LPSTR* ioNewName)
	{
	//	�R�s�[���̕����񂪈ُ�łȂ����ǂ������ׂ�A�ُ�ł͂Ȃ��ꍇ�R�s�[����
		if(inName != NULL)	lstrcpy(*ioNewName, inName);
	//	�ُ�ȏꍇ�A�R�s�[���NULL�����Ă���
		else				*ioNewName	= NULL;
	//	�����Ȃ���ΐ���I��
		return S_OK;
	}

	//================================================================================
	//!	���\�b�h��	MY_HIERARCHY::CreateMeshContainer
	//
	//	�@�\		���b�V���R���e�i�[�̍쐬
	//	�@�\�T�v	�V���Ƀ��b�V���R���e�i�[���쐬����B
	//				���b�V���R���e�i�[�Ƃ̓��b�V�������i�[���Ă���\���̂̂���
	//	����		Name				�V������郁�b�V���R���e�i�[�̖��O
	//				pMeshData			���b�V���f�[�^
	//				pMaterials			�}�e���A�����z��
	//				pEffectInstances	�G�t�F�N�g�i�V�F�[�_�[�j�C���X�^���X
	//				NumMaterials		�}�e���A���̐�
	//				pAdjacency			�אڃ|���S���C���f�b�N�X(����|���S���ׂ̗ɂ���O�̃|���S���̔ԍ��z��)
	//				pSkinInfo			�X�L�����b�V���̃f�[�^
	//				ppMeshContainer		�쐬�������b�V���R���e�i�[��n���̈�
	//	�߂�l		HRESULT				�e�����̃G���[
	//	�X�V		2008/05/30			<�ǉ�>	�R�����g
	//================================================================================
	HRESULT
	MY_HIERARCHY::CreateMeshContainer(LPCSTR						Name,
									  CONST D3DXMESHDATA*			pMeshData,
									  CONST D3DXMATERIAL*			pMaterials,
									  CONST D3DXEFFECTINSTANCE*		pEffectInstances,
									  DWORD							NumMaterials,
									  CONST DWORD					*pAdjacency,
									  LPD3DXSKININFO				pSkinInfo,
									  LPD3DXMESHCONTAINER*			ppMeshContainer) 
	{
		MYMESHCONTAINER		*pMeshContainer	= NULL;	//!< ���[�J���Ƀ��b�V���R���e�i�𐶐����邽�߂̕ϐ�
		int					iFacesAmount	= 0;	//!< �|���S���̐�
		LPDIRECT3DDEVICE9	pDevice			= NULL;	//!< Direct3D�f�o�C�X�i�e�N�X�`���ǂݍ��݂ȂǂɕK�v�j
		LPD3DXMESH			pMesh			= NULL;	//!< ���b�V���C���^�[�t�F�C�X�i�擾�p�j
		DWORD				dwBoneAmt		= 0;	//!< �{�[���̐�

	//	�����Ƃ��ēn����Ă������b�V���R���e�i�[�̊i�[���NULL�ɂ��Ă����i�O�̂��߁j
		*ppMeshContainer = NULL;

	//	�������烁�b�V���C���^�[�t�F�C�X���擾
		pMesh = pMeshData->pMesh;

	//	�܂��̓��[�J���ɐV�������b�V���R���e�i�[�𐶐�����
		pMeshContainer = new MYMESHCONTAINER();

	//	�����ł��Ȃ��ꍇ�A�������[�s��
		if(	!pMeshContainer)	return E_OUTOFMEMORY;

	//	�����������b�V���R���e�i�[��0����������
		ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));

	//	�������������b�V���R���e�i�[�ɖ��O��t����
		pMeshContainer->Name = new TCHAR[lstrlen(Name) + 1];		//���O�̈���m��

	//	���O�̈悪�m�ۂł��Ȃ��ꍇ�A���s
		if(	!pMeshContainer->Name)	return E_FAIL;

	//	�����Ƃ��ēn���ė������O���`�F�b�N���A����Ȃ�������
		AllocateName(Name, &pMeshContainer->Name);

	//	�f�o�C�X���擾�i�e�N�X�`���ǂݍ��݂ȂǂɕK�v�j
		pMesh->GetDevice(&pDevice);

	//	�|���S�������擾
		iFacesAmount = pMesh->GetNumFaces();
	   
	//	���Y���b�V�����@���������Ȃ��ꍇ�͖@����ǉ�����
		if(	!(pMesh->GetFVF() && D3DFVF_NORMAL))
		{
		//	�ʏ탁�b�V����������
			pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		//	���_�t�H�[�}�b�g�R�[�h(FVF)���g�p���ă��b�V���̃R�s�[���쐬
			if(	FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(),
										   pMesh->GetFVF() | D3DFVF_NORMAL,
										   pDevice,
										   &pMeshContainer->MeshData.pMesh)))
			{
			//	�@���̒ǉ��Ɏ��s�����ꍇ�A�I��
				return E_FAIL;
			}
			SAFE_RELEASE(pMesh);
			pMesh = pMeshContainer->MeshData.pMesh;
		//	���b�V���Ɋ܂܂��e���_�̖@�����v�Z����
			D3DXComputeNormals(pMesh, NULL);
		}
		else
		{
			pMeshContainer->MeshData.pMesh	= pMesh;
			pMeshContainer->MeshData.Type	= D3DXMESHTYPE_MESH;
		//	�K�����b�V���C���^�[�t�F�C�X�̎Q�ƃJ�E���^���ЂƂ��₷
			pMesh->AddRef();
		}
	 
	//	���b�V���̃}�e���A���ݒ�
		pMeshContainer->NumMaterials = max(1, NumMaterials);								//!< �}�e���A������0�̏ꍇ�A1�Ƃ���

	//	�܂��̓}�e���A���E�e�N�X�`���̋L���̈�i�z��j���m��
		pMeshContainer->pMaterials = new D3DXMATERIAL[NumMaterials];						//!< �}�e���A���z��m��
		pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];	//!< �e�N�X�`���z��m��
		pMeshContainer->pAdjacency = new DWORD[iFacesAmount*3];								//!< �אڃ|���S���C���f�b�N�X

	//	�}�e���A�����אڃ|���S�����e�N�X�`�����m�ۂł��Ȃ������玸�s
		if(	!(pMeshContainer->pMaterials)	||
			!(pMeshContainer->pAdjacency)	||
			!(pMeshContainer->ppTextures))	return E_FAIL;

	//	�אڃ|���S���C���f�b�N�X���R�s�[
		memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount*3);

	//	�e�N�X�`���̈��������
		ZeroMemory(pMeshContainer->ppTextures, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	//	�}�e���A�������݂���ꍇ
		if(NumMaterials > 0)
		{
		//	�����Ƃ��ē����ė����}�e���A�������A���b�V���R���e�i�[�ɃR�s�[����
			for(UINT i = 0; i < NumMaterials; i++)
			{
			//	�l�̑������R�s�[
				pMeshContainer->pMaterials[i].MatD3D = pMaterials[i].MatD3D;
			//	���f�[�^�Ƀe�N�X�`���t�@�C���������݂���ꍇ�A���̗̈���m��
				if(	pMaterials[i].pTextureFilename)
				{
					pMeshContainer->pMaterials[i].pTextureFilename =
						new TCHAR[strlen(pMaterials[i].pTextureFilename) + 1];
				}
			//	�e�N�X�`���t�@�C�������R�s�[�i���Ƃ����݂��Ă��Ȃ��ꍇ�͒e�����j
				AllocateName(pMaterials[i].pTextureFilename, &pMeshContainer->pMaterials[i].pTextureFilename);

			//	�A���r�G���g������ݒ�i���̂܂܂ł͐ݒ肳��Ȃ��j
				pMeshContainer->pMaterials[i].MatD3D.Ambient.a = 0.0f;
				pMeshContainer->pMaterials[i].MatD3D.Ambient.r = 0.0f;
				pMeshContainer->pMaterials[i].MatD3D.Ambient.g = 0.0f;
				pMeshContainer->pMaterials[i].MatD3D.Ambient.b = 0.0f;
			}

		//	�e�N�X�`���̓ǂݍ���
			for(UINT iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
			{
				if(	pMeshContainer->pMaterials[iMaterial].pTextureFilename)
				{
					TCHAR	textureName[MAX_PATH]		= "";
					TCHAR	strTexturePath[MAX_PATH]	= "";
					lstrcpy(textureName,  pMeshContainer->pMaterials[iMaterial].pTextureFilename);
					sprintf_s(strTexturePath, sizeof(strTexturePath), keepTexPath, textureName);

				//	�e�N�X�`���̓ǂݍ���
					if( FAILED( D3DXCreateTextureFromFile(pDevice,
														  strTexturePath,
														  &pMeshContainer->ppTextures[iMaterial] ) ) )
					{
					//	�e�N�X�`���������K�w�ɂȂ���΁A���߂�
						throw textureName;
					}
				}
			}
		}
	//	�}�e���A�������݂��Ȃ��i�}�e���A������0�������j�ꍇ
		else
		{
		//	�}�e���A�����ЂƂ������݂��Ă���Ƃ݂Ȃ��A�ݒ�
			pMeshContainer->pMaterials[0].pTextureFilename	= NULL;						//!< �e�N�X�`�����͖���
			memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));		//!< 0������
		//	�f�B�t���[�Y�̊e�F��0.5�ɐݒ�
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.r	= 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.g	= 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.b	= 0.5f;
		//	�X�y�L�����[�ƃf�B�t���[�Y�͓����l�ɐݒ�
			pMeshContainer->pMaterials[0].MatD3D.Specular	= pMeshContainer->pMaterials[0].MatD3D.Diffuse;
		}

	//	�G�t�F�N�g�f�[�^�̐ݒ�
		pMeshContainer->pEffects = new D3DXEFFECTINSTANCE;							//!< ���b�V���R���e�i�[�ɃG�t�F�N�g�̗̈���m��

	//	�G�t�F�N�g�t�@�C�����𔻒�
		if(	pEffectInstances->pEffectFilename)
		{
		//	�G�t�F�N�g�t�@�C���������݂��Ă���Ȃ�A���b�V���R���e�i�[�̃G�t�F�N�g�̈�Ƀt�@�C�����̂��߂̕����z����m��
			pMeshContainer->pEffects->pEffectFilename = new TCHAR[strlen(pEffectInstances->pEffectFilename) + 1];
		//	���O�̈���m�ۂł��Ȃ������ꍇ�A���s��Ԃ�
			if(	!pMeshContainer->pEffects->pEffectFilename)	return E_FAIL;
		}

	//	���O�����b�V���R���e�i�[�̃G�t�F�N�g�ɃR�s�[
		AllocateName(pEffectInstances->pEffectFilename, &pMeshContainer->pEffects->pEffectFilename);

	//	�f�t�H���g�G�t�F�N�g�̐����R�s�[
		pMeshContainer->pEffects->NumDefaults	= pEffectInstances->NumDefaults;

	//	�f�t�H���g�G�t�F�N�g�z��i�[�̈���m��
		pMeshContainer->pEffects->pDefaults		= new D3DXEFFECTDEFAULT[pEffectInstances->NumDefaults];

		for(UINT i = 0; i < pEffectInstances->NumDefaults; i++)
		{
		//	�f�t�H���g�G�t�F�N�g���̃p�����[�^����������������A���b�V���R���e�i�[���̃G�t�F�N�g���ɃR�s�[���邽�߂̗̈���m��
			pMeshContainer->pEffects->pDefaults[i].pParamName = new TCHAR[strlen(pEffectInstances->pDefaults[i].pParamName) + 1];

		//	���O�̈悪�m�ۂł��Ȃ�������A���s
			if(!pMeshContainer->pEffects->pDefaults[i].pParamName)	return E_FAIL;

		//	�p�����[�^�������b�V���R���e�i�[���̃G�t�F�N�g�ɃR�s�[
			AllocateName(pEffectInstances->pDefaults[i].pParamName, &pMeshContainer->pEffects->pDefaults[i].pParamName);

		//	�p�����[�^�̑傫�����R�s�[
			pMeshContainer->pEffects->pDefaults[i].NumBytes = pEffectInstances->pDefaults[i].NumBytes;

		//	�p�����[�^�̌^�����R�s�[
			pMeshContainer->pEffects->pDefaults[i].Type = pEffectInstances->pDefaults[i].Type;
			if(	pMeshContainer->pEffects->pDefaults[i].Type < D3DXEDT_DWORD)
				pMeshContainer->pEffects->pDefaults[i].pValue = new DWORD[pEffectInstances->pDefaults[i].NumBytes];
		}

	//	���Y���b�V�����X�L�����������Ă���ꍇ�i�X�L�����b�V���ŗL�̏����j
		if(	pSkinInfo)
		{
			pMeshContainer->pSkinInfo = pSkinInfo;		//!< �X�L���������b�V���R���e�i�[�ɃR�s�[
			pSkinInfo->AddRef();						//!< �Q�ƃJ�E���^���ЂƂ��₷
			dwBoneAmt = pSkinInfo->GetNumBones();		//!< �{�[���̐����擾
		//	�{�[���̐������{�[���I�t�Z�b�g�s�������̈���m��
			pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneAmt];
		//	�̈���m�ۂł��Ȃ������玸�s
			if(	!pMeshContainer->pBoneOffsetMatrices)	return E_FAIL;
		//	�{�[���I�t�Z�b�g�s����R�s�[
			for(DWORD i= 0; i < dwBoneAmt; i++)
			{
			//	���{�[���I�t�Z�b�g�s��Ƃ́A���_�̃��[�J�����W���{�[�����W�ɕϊ�����s��
				memcpy(&pMeshContainer->pBoneOffsetMatrices[i],
					pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DMATRIX));	
			}
		//	���b�V�����œK���i����{�[���̉e�����󂯂钸�_�̔ԍ���ʂ��ԍ��ɂ��Ă����j
			if(FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
				pMesh, D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
				pMeshContainer->pAdjacency, NULL, NULL, NULL,
				&pMeshContainer->dwWeight,
				&pMeshContainer->dwBoneAmt,
				&pMeshContainer->pBoneBuffer,
				&pMeshContainer->MeshData.pMesh)))
			{
			//	���s������I��
				return E_FAIL;
			}
		//	�g�p�������b�V�������
			SAFE_RELEASE(pMesh);
		}
	//	���[�J���ɐ����������b�V���R���e�i�[���Ăяo�����ɓn��
		*ppMeshContainer = pMeshContainer;
	//	���[�J���Ɏ擾�����f�o�C�X�����
		SAFE_RELEASE(pDevice);

		return S_OK;
	}

	//================================================================================
	//!	���\�b�h��	MY_HIERARCHY::DestroyFrame
	//
	//	�@�\		�t���[����j������
	//	�@�\�T�v	�\�z�����t���[�����������i�ċA�֐��j
	//	����		pFrameToFree	����������t���[��
	//	�߂�l		HRESULT			�e�����̃G���[
	//	�X�V		2008/05/30		<�ǉ�>	�R�����g
	//================================================================================
	HRESULT
	MY_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree)
	{
	//	���͂��ꂽ�t���[���������������ꍇ�A��߂�
		if(	!pFrameToFree)	return E_FAIL;

	//	�Z��t���[�������݂��Ă���Ȃ�A�Ăт��̃��\�b�h���Ă�
		if(	pFrameToFree->pFrameSibling)
		{
			DestroyFrame(pFrameToFree->pFrameSibling);
			pFrameToFree->pFrameSibling = NULL;
		}

	//	�q�t���[�������݂��Ă���Ȃ�A�Ăт��̃��\�b�h���Ă�
		if(	pFrameToFree->pFrameFirstChild)
		{	
			DestroyFrame(pFrameToFree->pFrameFirstChild);
			pFrameToFree->pFrameFirstChild = NULL;
		}

	//	�t���[���̖��O��NULL�łȂ��Ȃ�A��������
		if(	pFrameToFree->Name)
		{	
			SAFE_DELETE_ARRAY(pFrameToFree->Name);
		}

	//	���b�V���R���e�i�[�����݂��Ă���Ȃ�A���
		if(	pFrameToFree->pMeshContainer)
		{
			DestroyMeshContainer(pFrameToFree->pMeshContainer);
		}

	//	�����܂ŗ�����A�����̃t���[�����������
		MYFRAME	*pFrame	= (MYFRAME*)pFrameToFree;
		SAFE_DELETE(pFrame);

		return S_OK; 
	}

	//================================================================================
	//!	���\�b�h��	MY_HIERARCHY::DestroyFrame
	//
	//	�@�\		���b�V���R���e�i�[��j������
	//	�@�\�T�v	�����������b�V���R���e�i�[���������
	//	����		pMeshContainerBase	������������b�V���R���e�i�[
	//	�߂�l		HRESULT				�e�����̃G���[
	//	�X�V		2008/05/30			<�ǉ�>	�R�����g
	//================================================================================
	HRESULT
	MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
	{
	//	�܂��͊g�������������Q�Ƃł���悤�ɁA�g�����b�V���R���e�i�[�ɃL���X�g����
		MYMESHCONTAINER* pMeshContainerToFree = (MYMESHCONTAINER*)pMeshContainerBase;

	//	���O�����
		SAFE_DELETE_ARRAY(pMeshContainerToFree->Name);

	//	���b�V���C���^�[�t�F�C�X�����
		SAFE_RELEASE(pMeshContainerToFree->MeshData.pMesh);
		SAFE_RELEASE(pMeshContainerToFree->MeshData.pPatchMesh);
		SAFE_RELEASE(pMeshContainerToFree->MeshData.pPMesh);

	//	�}�e���A���z����̃e�N�X�`���t�@�C���������
		for(UINT i = 0; i < pMeshContainerToFree->NumMaterials; i++)
		{
			SAFE_DELETE_ARRAY(pMeshContainerToFree->pMaterials[i].pTextureFilename);
		}

	//	�}�e���A���z�񂻂̂��̂����
		SAFE_DELETE_ARRAY(pMeshContainerToFree->pMaterials);

	//	�G�t�F�N�g�������
		for(UINT i = 0; i < pMeshContainerToFree->pEffects->NumDefaults; i++)
		{
			SAFE_DELETE_ARRAY(pMeshContainerToFree->pEffects->pDefaults[i].pParamName);
			SAFE_DELETE_ARRAY(pMeshContainerToFree->pEffects->pDefaults[i].pValue);
		}

		SAFE_DELETE_ARRAY(pMeshContainerToFree->pEffects->pEffectFilename);
		SAFE_DELETE_ARRAY(pMeshContainerToFree->pEffects->pDefaults);
		SAFE_DELETE(pMeshContainerToFree->pEffects);

	//	�אڃ|���S���z������
		SAFE_DELETE_ARRAY(pMeshContainerToFree->pAdjacency);

	//	�X�L���������
		SAFE_RELEASE(pMeshContainerToFree->pSkinInfo);

	//	�������牺�͊g���������b�V���R���e�i�[���L�̏���
		if(pMeshContainerToFree->ppTextures)
		{
		//	�e�N�X�`���C���^�[�t�F�C�X�ւ̃|�C���^�z����g���A�e�N�X�`���C���^�[�t�F�C�X�����
			for(UINT i = 0; i < pMeshContainerToFree->NumMaterials; i++)
			{
				SAFE_RELEASE(pMeshContainerToFree->ppTextures[i]);
			}
			SAFE_DELETE_ARRAY(pMeshContainerToFree->ppTextures);
		}

	//	�e�N�X�`���C���^�[�t�F�C�X�ւ̃|�C���^�z�񂻂̂��̂����
		SAFE_DELETE(pMeshContainerToFree->ppTextures);

	//	�{�[���e�[�u�������
		SAFE_RELEASE(pMeshContainerToFree->pBoneBuffer);

	//	�S�Ẵ{�[���̃��[���h�s��z������
		SAFE_DELETE_ARRAY(pMeshContainerToFree->ppBoneMatrix);

	//	�{�[���I�t�Z�b�g�s��z������
		SAFE_DELETE_ARRAY(pMeshContainerToFree->pBoneOffsetMatrices);

	//	���b�V���R���e�i�[���̂��̂����
		SAFE_DELETE(pMeshContainerToFree);
		return S_OK;
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::createSkinInData
	//
	//	�@�\		X�t�@�C�������[�h���A�X�L�����b�V�������쐬
	//	�@�\�T�v	�X�L�����b�V���������[�h���A���ʂ�Ԃ�
	//				���̃��\�b�h�̓N���X���\�b�h�ł���
	//	����		inDev			Direct3D�f�o�C�X
	//				inSkinData		���[�h��X�L�����b�V���f�[�^
	//				inName			X�t�@�C���̖���
	//				inResource		�X�L�����b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�߂�l		TRUE			��������
	//				FALSE			�������s
	//	�X�V		2009/03/02		<�ǉ�>	�R�����g
	//================================================================================
	BOOL
	CSKIN_MESH_BASE::createSkinInData(CONST DEV   inDev,
									  SKIN_DATA*  inSkinData,
									  CONST LPSTR inName,
									  CONST LPSTR inResource,
									  CONST LPSTR inTexture)
	{
		MY_HIERARCHY				ah(inTexture);				//!< �ǂݍ��݃N���X
		LPD3DXANIMATIONCONTROLLER	tempAnimController;			//!< ��Ɨp�A�j���[�V�����R���g���[��

	//	�t�@�C��������������쐬�i���s�t�@�C���Ɠ����K�w�ɂ���Ƃ���j
		TCHAR		realName[MAX_PATH]	= "";					//!< �t�@�C�����Ƀp�X��t�����

	//	�X�L�����b�V���f�[�^���������
		inSkinData->releaseData();

	//	�ǂݍ���ł݂�
	//	D3DXLoadMeshHierarchyFromX(�ŏ��̃t���[���K�w�� .x �t�@�C�����烍�[�h)
		if(	FAILED(D3DXLoadMeshHierarchyFromX(inName,
											  D3DXMESH_MANAGED,
											  inDev,
											  &ah,
											  NULL,
											  &inSkinData->pFrameRoot,
											  &tempAnimController)))
		{
		//	���s������A���\�[�X�t�@�C���ւ̃p�X�����ăg���C
			sprintf_s(realName, sizeof(realName), inResource, inName);

			if(FAILED(D3DXLoadMeshHierarchyFromX(realName,
												 D3DXMESH_MANAGED,
												 inDev, 
												 &ah,
												 NULL,
												 &inSkinData->pFrameRoot, 
												 &tempAnimController)))
			{
			//	���s������ANULL��Ԃ�
				goto error;
			}
		}

	//	�{�[��������
		AllocateAllBoneMatrices(inSkinData, inSkinData->pFrameRoot);

		int	trackNum				= tempAnimController->GetMaxNumTracks();
		int	animNum					= tempAnimController->GetMaxNumAnimationSets();	

	//	�A�j���[�V�����̐����擾
		inSkinData->iAnimCnt			= animNum;
		inSkinData->pAnimController	= tempAnimController;

	//	�A�j���[�V�����Z�b�g�̗̈�𑝂₷
		inSkinData->pAnimSet = new LPD3DXANIMATIONSET[inSkinData->iAnimCnt];
	//	�A�j���[�V�����X�e�b�v���m��
		inSkinData->iAnimStep= new int[inSkinData->iAnimCnt];

	//	�X�L�����b�V���f�[�^�t�@�C�����J��
		FILE*	skinMeshDataFile	= NULL;
		sprintf_s(realName, sizeof(realName), PATH_SKINMESHDATA, inName);
		fopen_s(&skinMeshDataFile, realName, "r");

	//	�A�j���[�V�����g���b�N�𓾂�
		for(DWORD i = 0; i < (UINT)inSkinData->iAnimCnt; i++)
		{
			inSkinData->pAnimController->GetAnimationSet(i, &inSkinData->pAnimSet[i]);
		//	�A�j���[�V�����X�e�b�v�ԍ��̐ݒ�
		//	�X�L�����b�V���f�[�^�t�@�C�����J���Ă���Ȃ�A���ꂩ��ǂݍ��ݐݒ�
			if(	skinMeshDataFile)
			{
				fscanf_s(skinMeshDataFile, "%d, ", &inSkinData->iAnimStep[i]);
			}
		//	�J����Ė����Ȃ�A�����̃f�[�^�Őݒ�
			else
			{
				inSkinData->iAnimStep[i]	= i;
			}
		}
		if(	skinMeshDataFile){	fclose(skinMeshDataFile);	}

	//	���O������
		inSkinData->name	= inName;
		inSkinData->rPath	= inResource;
		inSkinData->tPath	= inTexture;

	//	���b�V���Ƃ��Ă̏��𐶐�
		inSkinData->mesh		= new MESH_DATA();

	//	���a������o��
		D3DXFrameCalculateBoundingSphere(
			inSkinData->pFrameRoot,
			&inSkinData->mesh->localCenter,
			&inSkinData->mesh->localRadius);

	//	���������X�L���̃}�b�v�z��v�f�ԍ���Ԃ�
		return TRUE;

	//	NULL��Ԃ�����
	error:
		inSkinData->releaseData();	//!< �����������b�V���������
		DX::errorMsgNoFile(inDev, realName);
		return	FALSE;				//!< NULL��Ԃ�
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::createSkin
	//
	//	�@�\		�X�L�����b�V�����𐶐����A���������f�[�^��Ԃ�
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		�X�L�����b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�߂�l		�X�L�����b�V�����̃|�C���^�[
	//	�X�V		2008/05/30		<�ǉ�>	�R�����g
	//================================================================================
	SKIN_DATA*
	CSKIN_MESH_BASE::createSkin(CONST DEV   inDev,
								CONST LPSTR inName,
								CONST LPSTR inResource,
								CONST LPSTR inTexture)
	{
		SKIN_DATA*	newSkin	= new SKIN_DATA();	//!< �V���ȃX�L�����b�V�����

	//	�X�L�����b�V���𐶐����A���s������NULL��Ԃ�
		if(	!createSkinInData(inDev, newSkin, inName, inResource, inTexture))
		{
			SAFE_DELETE(newSkin);
			return	NULL;
		}
		return	newSkin;
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::startSkinMeshInitialize
	//
	//	�@�\		�����̏�����
	//	�X�V		2008/08/30		<�V�K>
	//================================================================================
	void
	CSKIN_MESH_BASE::startSkinMeshInitialize(void)
	{
		animSpeed			= kANIM_SPEED;		//!< �A�j���[�V�����X�s�[�h
		animTime			= 0.0f;				//!< �A�j���[�V�����^�C��
		bPlayAnim			= TRUE;				//!< �A�j���[�V�������s�m�F
		prevAnimNumber		= 0;				//!< �O��̃A�j���[�V�����ԍ�
		currAnimNumber		= -1;				//!< ���݂̃A�j���[�V�����ԍ�
		nextAnimNumber		= 0;				//!< ����̃A�j���[�V�����ԍ�
		bAnimChange			= FALSE;			//!< �A�j���[�V�����`�F���W�m�F
		spAnimCtrl			= NULL;				//!< ��p�̃A�j���[�V�����R���g���[���[
		sd_ptr				= NULL;				//!< �X�L�����b�V�����
		bSkinLoaded			= FALSE;			//!< �X�L�����b�V�����[�h�m�F
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::initializeSkinMesh
	//
	//	�@�\		�X�L�����b�V���̏�����
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		�X�L�����b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	void
	CSKIN_MESH_BASE::initializeSkinMesh(CONST DEV	inDev,
										CONST LPSTR	inName,
										CONST LPSTR	inResource,
										CONST LPSTR	inTexture)
	{
	//	�X�L�����b�V���̏������
		finishalizeSkinMesh();

	//	�X�L���}�b�v��������ΐ�������
		if(	!mSkins)
		{
			mSkins = new map<string, SKIN_DATA*>;
		}

	//	��Ɨp�C�e���[�^
		map<string, SKIN_DATA*>::iterator	p;
	//	�X�L���}�b�v��X�t�@�C������n���A���̃C�e���[�^���󂯎��
		p = mSkins->find(inName);

	//	�X�L���}�b�v�ɓ������O��X�t�@�C���������ɓo�^���Ă���ꍇ
		if(	p != mSkins->end())
		{
		//	�g�p�J�E���^�𑝂₷
			p->second->cnt++;
		//	�X�L�����b�V�������擾����
			sd_ptr		= (*p).second;
		//	���[�h����
			bSkinLoaded	= TRUE;
		}
		else
		{
		//	�V���ɃX�L�����b�V���𐶐�����
			sd_ptr	= createSkin(inDev, inName, inResource, inTexture);

		//	�����ɐ��������ꍇ
			if(	sd_ptr)
			{
			//	�g�p�J�E���^�𑝂₷
				sd_ptr->cnt++;
			//	���[�h����
				bSkinLoaded	= TRUE;
			//	�X�L���}�b�v�ɓo�^
				mSkins->insert(pair<string, SKIN_DATA*>(sd_ptr->name, sd_ptr));
			}
		//	���s�����ꍇ
			else
			{
			//	���[�h������
				bSkinLoaded	= FALSE;
			}
		}

	//	�X�L�����b�V�����������Ă��鎞
		if(	bSkinLoaded)
		{
		//	��p�̃A�j���[�V�����R���g���[���[���쐬(�g���b�N�̐���������)
			sd_ptr->pAnimController->CloneAnimationController(
				sd_ptr->pAnimController->GetMaxNumAnimationOutputs(),
				sd_ptr->pAnimController->GetMaxNumAnimationSets(),
				sd_ptr->iAnimCnt,
				sd_ptr->pAnimController->GetMaxNumEvents(),
				&spAnimCtrl);
		//	�A�j���R���g���[���쓮
			spAnimCtrl->ResetTime();
		//	�A�j���[�V�����Đ�
			changeAnimationAndBlend(0);
		}
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::finishalizeSkinMesh
	//
	//	�@�\		�X�L�����b�V���̉��
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	void
	CSKIN_MESH_BASE::finishalizeSkinMesh(void)
	{
	//	�X�L�����b�V���������[�h���ĂȂ��Ȃ�A������Ԃ�
		if(	!bSkinLoaded)	return;

	//	�t���O�����ǂ��Ă���
		bSkinLoaded	= FALSE;

	//	��p�̃A�j���[�V�����R���g���[�����������
		SAFE_RELEASE(spAnimCtrl);
	//	�X�L�����b�V���J�E���g�����炷
		sd_ptr->cnt--;
	//	���̃X�L�����b�V�����܂��g�p����Ă���ꍇ�́A�f�[�^������Ȃ�
		if(	sd_ptr->cnt > 0)	return;

	//	�X�L���}�b�v�C�e���[�^
		map<string, SKIN_DATA*>::iterator	p;
	//	�X�L���}�b�v��X�t�@�C������n���A���̃C�e���[�^���󂯎��
		p = mSkins->find(sd_ptr->name);
	//	�X�L�����̉��
		SAFE_DELETE(p->second);
	//	�X�L���}�b�v����폜
		mSkins->erase(p);

	//	�X�L���}�b�v�Ƀ����o�[��������΁A��������
		if(	mSkins->begin() == mSkins->end())
		{
			SAFE_DELETE(mSkins);
		}
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::�R���X�g���N�^
	//
	//	�@�\�T�v	X�t�@�C�������[�h���ď�����
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		�X�L�����b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2008/05/30		<�ǉ�>	�R�����g
	//================================================================================
	CSKIN_MESH_BASE::CSKIN_MESH_BASE(	CONST DEV   inDev,
										CONST LPSTR inName,
										CONST LPSTR inResource,
										CONST LPSTR inTexture)
	{
	//	�����̏�����
		startSkinMeshInitialize();
	//	�X�[�p�[�N���X�̏�����
		initializeSkinMesh(inDev, inName, inResource, inTexture);
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::�R���X�g���N�^
	//
	//	�@�\		�󏉊���
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	CSKIN_MESH_BASE::CSKIN_MESH_BASE(void)
	{
	//	�����̏�����
		startSkinMeshInitialize();
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::�R���X�g���N�^
	//
	//	�@�\�T�v	�R�s�[
	//	����		inCopy			�R�s�[��
	//	�X�V		2008/05/30		<�ǉ�>	�R�����g
	//================================================================================
	CSKIN_MESH_BASE::CSKIN_MESH_BASE(CSKIN_MESH_BASE* inCopy)
	{
	//	�����̏�����
		startSkinMeshInitialize();
	//	�R�s�[�J�n
		sd_ptr	= getSkins();		//!< �X�L�����b�V�����

	//	�X�L�����b�V����񂪂��鎞
		if(	sd_ptr)
		{
		//	�c����R�s�[
			setAnimSpeed		(inCopy->getAnimSpeed());		//!< �A�j���[�V�����X�s�[�h
			setAnimTime			(inCopy->getAnimTime());			//!< �A�j���[�V�����^�C��
			setCurrAnimNumber	(inCopy->getCurrAnimNumber());	//!< ���݂̃A�j���[�V�����ԍ�
			setNextAnimNumber	(inCopy->getNextAnimNumber());	//!< ����̃A�j���[�V�����ԍ�
		//	�g�p�J�E���^�𑝂₷
			sd_ptr->cnt++;
		//	��p�̃A�j���[�V�����R���g���[���[���쐬(�g���b�N�̐���������)
			sd_ptr->pAnimController->CloneAnimationController(
				sd_ptr->pAnimController->GetMaxNumAnimationOutputs(),
				sd_ptr->pAnimController->GetMaxNumAnimationSets(),
				sd_ptr->iAnimCnt,
				sd_ptr->pAnimController->GetMaxNumEvents(),
				&spAnimCtrl);
		//	�A�j���R���g���[���쓮
			spAnimCtrl->ResetTime();
		//	���[�h����
			bSkinLoaded	= TRUE;
		//	�A�j���[�V�����Đ�
			changeAnimationAndBlend(0);
		}
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::�f�X�g���N�^
	//
	//	�@�\�T�v	�S�Ă̎�����������A�I������B
	//				�������A���̃I�u�W�F�N�g�ł��g���Ă����ꍇ�͎c���B
	//	�X�V		2008/05/30		<�ǉ�>	�R�����g
	//================================================================================
	CSKIN_MESH_BASE::~CSKIN_MESH_BASE()
	{
	//	�X�L�����b�V���̉��
		finishalizeSkinMesh();
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::resetInstance
	//
	//	�@�\		���Z�b�g���s
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2009/03/02		<�V�K>
	//================================================================================
	void
	CSKIN_MESH_BASE::resetInstance(CONST DEV inDev)
	{
	//	�X�L���}�b�v���Ȃ���΁A���Z�b�g���Ȃ�
		if(	!mSkins)	return;

	//	�ǂݍ��񂾎��̏������o����
		char	inName[MAX_PATH]	= "";
		char	inRPath[MAX_PATH]	= "";
		char	inTPath[MAX_PATH]	= "";

	//	����������
		MAP_LOOP(string, SKIN_DATA*, (*mSkins))
		{
		//	���̃f�[�^��ێ�����
			sprintf_s(inName,   sizeof(inName),  (*p).second->name.c_str());
			sprintf_s(inRPath,  sizeof(inRPath), (*p).second->rPath.c_str());
			sprintf_s(inTPath,  sizeof(inTPath), (*p).second->tPath.c_str());
		//	���̃f�[�^�œǂ݂��݂�����
			createSkinInData(inDev, (*p).second, inName, inRPath, inTPath);
		}
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::skinInvalid
	//
	//	�@�\		�X�L�����b�V���̖������𒲂ׂ�
	//	�߂�l		BOOL			TREU	����
	//								FALSE	�L��
	//	�X�V		2008/05/30		<�ǉ�>	�R�����g
	//================================================================================
	BOOL
	CSKIN_MESH_BASE::skinInvalid(void)
	{
		if(	(!bSkinLoaded))		//!< �X�L�����b�V���������[�h���ĂȂ���
		{
			return TRUE;		//!< ����
		}
		else					//!< ��L�ɓ��Ă͂܂�Ȃ��Ȃ�
		{
			return FALSE;		//!< �L��
		}
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::animation
	//
	//	�@�\		�A�j���[�V��������
	//	�@�\�T�v	�X�L�����b�V���̃A�j���[�V�������s��
	//	�߂�l		TRUE			�A�j���[�V��������
	//				FALSE			�A�j���[�V�������s
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	BOOL
	CSKIN_MESH_BASE::animation(void)
	{
	//	�`��̖������𒲂ׂāA�����̎�
		if(	skinInvalid())
		{
		//	�`�掸�s��Ԃ�
			return	FALSE;
		}

	//	�A�j���[�V�����X�C�b�`�������Ă��鎞
		if(	bPlayAnim)
		{
		//	�l��ێ�
			float	keepAnimTime	= animTime;
		//	�����Ă���Ȃ玞�Ԃ�i�߂�
			animTime += animSpeed;

		//	�A�j���[�V�������I��������
			if(	animTime > sd_ptr->pAnimSet[currAnimNumber]->GetPeriod())
			{
			//	�A�j���[�V������ύX�����΂���Ȃ�A�t���O��߂��ďI��
			/*	if(	bAnimChange)
				{
					bAnimChange = FALSE;
				}
				else
			*/
			//	���݂̃A�j���[�V�����ԍ��ƁA���̃A�j���[�V�����X�e�b�v�ԍ�����v������
				if(	sd_ptr->iAnimStep[currAnimNumber] == currAnimNumber)
				{
					spAnimCtrl->ResetTime();
					animTime = 0.0f;
					spAnimCtrl->AdvanceTime(animSpeed, NULL);
					spAnimCtrl->SetTrackPosition(currAnimNumber, animTime);
				}
			//	���̃A�j���[�V�����X�e�b�v�ԍ����A���݂̔ԍ��ȊO�̐���ȃA�j���[�V�����ԍ��̎�
				else if((sd_ptr->iAnimStep[currAnimNumber] >= 0                     ) &&
						(sd_ptr->iAnimStep[currAnimNumber] <  sd_ptr->iAnimCnt))
				{
					changeAnimationAndBlend(sd_ptr->iAnimStep[currAnimNumber]);
					spAnimCtrl->AdvanceTime(animSpeed, NULL);
				}
			//	��L�ȊO�Ȃ�A�A�j���[�V�������X�g�b�v
				else
				{
					animTime	= keepAnimTime;
					spAnimCtrl->SetTrackPosition(currAnimNumber, animTime);
					setPlayAnim(FALSE);
				}
			}
			else
			{
				spAnimCtrl->AdvanceTime(animSpeed, NULL);
				spAnimCtrl->SetTrackPosition(currAnimNumber, animTime);
			}
		}
		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::drawSkin
	//
	//	�@�\		�`��
	//	�@�\�T�v	�X�L�����b�V���̕`����s��
	//	����		inDev			Direct3D�f�o�C�X
	//				inMatrix		���[���h�}�g���b�N�X
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2008/05/30		<�ǉ�>	�R�����g
	//================================================================================
	BOOL
	CSKIN_MESH_BASE::drawSkin(CONST DEV inDev, MAT16* inMatrix)
	{
	//	�`��̖������𒲂ׂāA�����̎�
		if(	skinInvalid())
		{
		//	�`�掸�s��Ԃ�
			return	FALSE;
		}

	//	�A�j���[�V�����X�C�b�`�������Ă��鎞
		if(	bPlayAnim)
		{
		//	�l��ێ�
			float	keepAnimTime	= animTime;
		//	�����Ă���Ȃ玞�Ԃ�i�߂�
			animTime += animSpeed;

		//	�A�j���[�V�������I��������
			if(	animTime > sd_ptr->pAnimSet[currAnimNumber]->GetPeriod())
			{
			//	�A�j���[�V������ύX�����΂���Ȃ�A�t���O��߂��ďI��
			/*	if(	bAnimChange)
				{
					bAnimChange = FALSE;
				}
				else
			*/
			//	���݂̃A�j���[�V�����ԍ��ƁA���̃A�j���[�V�����X�e�b�v�ԍ�����v������
				if(	sd_ptr->iAnimStep[currAnimNumber] == currAnimNumber)
				{
					spAnimCtrl->ResetTime();
					animTime = 0.0f;
					spAnimCtrl->AdvanceTime(animSpeed, NULL);
					spAnimCtrl->SetTrackPosition(currAnimNumber, animTime);
				}
			//	���̃A�j���[�V�����X�e�b�v�ԍ����A���݂̔ԍ��ȊO�̐���ȃA�j���[�V�����ԍ��̎�
				else if((sd_ptr->iAnimStep[currAnimNumber] >= 0                     ) &&
						(sd_ptr->iAnimStep[currAnimNumber] <  sd_ptr->iAnimCnt))
				{
					changeAnimationAndBlend(sd_ptr->iAnimStep[currAnimNumber]);
					spAnimCtrl->AdvanceTime(animSpeed, NULL);
				}
			//	��L�ȊO�Ȃ�A�A�j���[�V�������X�g�b�v
				else
				{
					animTime	= keepAnimTime;
					spAnimCtrl->SetTrackPosition(currAnimNumber, animTime);
					setPlayAnim(FALSE);
				}
			}
			else
			{
				spAnimCtrl->AdvanceTime(animSpeed, NULL);
				spAnimCtrl->SetTrackPosition(currAnimNumber, animTime);
			}
		}
	//	�s��X�V
		UpdateFrameMatrices(sd_ptr->pFrameRoot, inMatrix);
	//	�`��
		DrawFrame(inDev, sd_ptr->pFrameRoot);

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::AllocateBoneMatrix
	//
	//	�@�\		�{�[���s��̏�����
	//	����		newSkin				�X�L�����b�V�����
	//				pMeshContainerBase	���b�V���R���e�i
	//	�߂�l		HRESULT				�e�����̃G���[
	//	�X�V		2008/05/30		<�ǉ�>	�R�����g
	//================================================================================
	HRESULT
	CSKIN_MESH_BASE::AllocateBoneMatrix(SKIN_DATA* newSkin, LPD3DXMESHCONTAINER pMeshContainerBase)
	{
		MYFRAME*	pFrame	  = NULL;		//!< �{�[�����m�F�p
		DWORD		dwBoneAmt = 0;			//!< �{�[���̐�

	//	���b�V���R���e�i������R���e�i�ɓ����
		MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	//	�X�L�����b�V���A�j���[�V�����������Ȃ�I��
		if(	!pMeshContainer->pSkinInfo)	return S_OK;

	//	�{�[���̐����擾
		dwBoneAmt = pMeshContainer->pSkinInfo->GetNumBones();

	//	�{�[���̐������A�{�[�����[���h�s��z��𐶐�
		pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneAmt];

	//	�S�Ẵ{�[���̌����A�y�у��[���h�s����󂯎��
		for (DWORD i = 0; i < dwBoneAmt; i++)
		{
		//	�q�t���[�����������Ď󂯎��
		//	D3DXFrameFind(���[�g�t���[���̎q�t���[������������)
			pFrame = (MYFRAME*)D3DXFrameFind(newSkin->pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i) );
		//	�q�t���[�����󂯎��Ȃ�������G���[
			if(	!pFrame)	return E_FAIL;
		//	�󂯎�����q�t���[���̃��[���h�s��̃|�C���^�[���󂯎��
			pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;
		}
		return S_OK;
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::AllocateAllBoneMatrices
	//
	//	�@�\		�{�[���s��̏�����(�Z��t���[���A�q�t���[���֍ċA����)
	//	����		pThing				�X�L�����b�V�����
	//				pFrame				�X�L�����b�V�����̐e�t���[�����
	//	�߂�l		HRESULT				�e�����̃G���[
	//	�X�V		2008/05/30		<�ǉ�>	�R�����g
	//================================================================================
	HRESULT
	CSKIN_MESH_BASE::AllocateAllBoneMatrices(SKIN_DATA* newSkin, LPD3DXFRAME pFrame)
	{
	//	�t���[���Ƀ��b�V���R���e�i�����݂��鎞
		if(	pFrame->pMeshContainer)
		{
		//	�{�[���s��̏��������s���A���s������G���[
			if(FAILED(AllocateBoneMatrix(newSkin, pFrame->pMeshContainer)))			return	E_FAIL;
		}
	//	�Z��t���[�������݂��鎞
		if(	pFrame->pFrameSibling)
		{
		//	�Z��t���[���̏��������s���A���s������G���[
			if(FAILED(AllocateAllBoneMatrices(newSkin, pFrame->pFrameSibling)))		return E_FAIL;
		}
	//	�q�t���[�������݂��鎞
		if(	pFrame->pFrameFirstChild)
		{
		//	�q�t���[���̏��������s���A���s������G���[
			if(FAILED(AllocateAllBoneMatrices(newSkin, pFrame->pFrameFirstChild)))	return E_FAIL;
		}
		return S_OK;
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::RenderMeshContainer
	//
	//	�@�\		�t���[�����̂��ꂼ��̃��b�V����`�悷��
	//	����		inDev				DirectX�f�o�C�X
	//				pMeshContainer		�`��Ώۂ̃��b�V���R���e�i
	//				pFrame				�t���[�����
	//	�X�V		2008/05/30		<�ǉ�>	�R�����g
	//================================================================================
	void
	CSKIN_MESH_BASE::RenderMeshContainer(	CONST DEV			inDev,
											MYMESHCONTAINER*	pMeshContainer,
											MYFRAME*			pFrame)
	{
		DWORD					i, k;				//!< ���[�v�J�E���^
		DWORD					dwBlendMatrixAmt;	//!< �d��
		DWORD					dwPrevBoneID;		//!<
		LPD3DXBONECOMBINATION	pBoneCombination;	//!< ���b�V���T�u�Z�b�g
		UINT					iMatrixIndex;		//!<
		D3DXMATRIX				matStack;			//!< �}�g���b�N�X�ꎞ�ۊǏꏊ

	//	�X�L�����b�V���̏ꍇ
		if(	pMeshContainer->pSkinInfo)
		{
		//	�����ƃ{�[���̑g�ݍ��킹���������b�V���T�u�Z�b�g���󂯎��
			pBoneCombination = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneBuffer->GetBufferPointer();
		//
			dwPrevBoneID = UINT_MAX;

		//	�S�Ẵ{�[��������
			for(i = 0; i < pMeshContainer->dwBoneAmt; i++)
			{
			//
				dwBlendMatrixAmt = 0;
			//	�d�݂̌������A�{�[���z�������
				for(k = 0; k < pMeshContainer->dwWeight; k++)
				{
				//	���݌��Ă���{�[���̔z��̒��������E�Ŗ�����
					if(	pBoneCombination[i].BoneId[k] != UINT_MAX) 
					{
					//	�d�݂̐����X�V
						dwBlendMatrixAmt = k;
					}
				}
			//	�W�I���g���u�����f�B���O�����s����ׂɎg���s��̐���ݒ�
				inDev->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixAmt);
			//	�d�݂̌������A�{�[���z�������
				for(k = 0; k < pMeshContainer->dwWeight; k++) 
				{
				//	�{�[���z����󂯎��
					iMatrixIndex = pBoneCombination[i].BoneId[k];
				//	�{�[���̔z��̒��������E�Ŗ�����
					if(	iMatrixIndex != UINT_MAX)
					{
					//	�{�[���̃��[���h�s��ƁA
						matStack = pMeshContainer->pBoneOffsetMatrices[iMatrixIndex] * (*pMeshContainer->ppBoneMatrix[iMatrixIndex]);
						inDev->SetTransform( D3DTS_WORLDMATRIX(k), &matStack );
					}
				}
			//	�f�B�t���[�Y�̐ݒ�͂����ōs��
				inDev->SetMaterial(&pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D );
				inDev->SetTexture(0, pMeshContainer->ppTextures[pBoneCombination[i].AttribId] );
				dwPrevBoneID = pBoneCombination[i].AttribId;
				pMeshContainer->MeshData.pMesh->DrawSubset(i);	
			}
		//	�����_�����O�X�e�[�g��ύX�����̂ŁA���ɖ߂��Ă����B��������Ȃ��ƈꕔ��PC�Ńo�O���o��
			inDev->SetRenderState(D3DRS_VERTEXBLEND, 0);
		}
	//	�ʏ탁�b�V���̏ꍇ
		else
		{
			inDev->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);
			for (i = 0; i < pMeshContainer->NumMaterials; i++)
			{
			//	�f�B�t���[�Y�̐ݒ�͂����ōs��
				inDev->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D );
				inDev->SetTexture(0, pMeshContainer->ppTextures[i] );
				pMeshContainer->MeshData.pMesh->DrawSubset(i);
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::DrawFrame
	//
	//	�@�\		�t���[����`�悷��
	//	����		inDev			DirectX�f�o�C�X
	//				pFrameBase		�`�悷��t���[��
	//	�X�V		2008/05/30		<�ǉ�>	�R�����g
	//================================================================================
	void
	CSKIN_MESH_BASE::DrawFrame(CONST DEV inDev, LPD3DXFRAME pFrameBase)
	{
	//	��Ɨp�|�C���^�[�ɒl��ϊ�
		MYFRAME*			pFrame			= (MYFRAME*)pFrameBase;
		MYMESHCONTAINER*	pMeshContainer	= (MYMESHCONTAINER*)pFrame->pMeshContainer;

	//	���b�V���R���e�i�����݂���
		while(pMeshContainer)
		{
		//	�`��
			RenderMeshContainer(inDev, pMeshContainer, pFrame);
		//	���b�V���R���e�i�[��ۑ����Ēu��
			pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
		}
	//	�Z��t���[�������݂��鎞
		if(	pFrame->pFrameSibling)
		{
		//	�Z��t���[���̕`��Ɉڂ�
			DrawFrame(inDev, pFrame->pFrameSibling);
		}
	//	�q�t���[�������݂��鎞
		if(	pFrame->pFrameFirstChild)
		{
		//	�q�t���[���̕`��Ɉڂ�
			DrawFrame(inDev, pFrame->pFrameFirstChild);
		}
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::UpdateFrameMatrices
	//
	//	�@�\		�t���[���̃}�g���b�N�X���X�V
	//	����		pFrameBase		�X�V����t���[��
	//				pParentMatrix	�X�V���̃}�g���b�N�X
	//	�X�V		2008/05/30		<�ǉ�>	�R�����g
	//================================================================================
	void
	CSKIN_MESH_BASE::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
	{
	//	��Ɨp�}�C�t���[��
		MYFRAME *pFrame = (MYFRAME*)pFrameBase;	

	//	�X�V���̃}�g���b�N�X�����݂��鎞
		if(	pParentMatrix)
		{
		//	�����Ă����}�g���b�N�X�ōX�V
			D3DXMatrixMultiply(
				&pFrame->CombinedTransformationMatrix,
				&pFrame->TransformationMatrix,
				pParentMatrix);
		}
	//	�X�V���̃}�g���b�N�X�����݂��Ȃ���
		else
		{
		//	�t���[�������}�g���b�N�X�ōX�V
			pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
		}
	//	�Z��t���[�������݂��鎞
		if(	pFrame->pFrameSibling)
		{
		//	�Z��t���[���̍X�V�Ɉڂ�
			UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
		}
	//	�q�t���[�������݂��鎞
		if(	pFrame->pFrameFirstChild)
		{
		//	�q�t���[���̍X�V�Ɉڂ�
			UpdateFrameMatrices(
				pFrame->pFrameFirstChild,
				&pFrame->CombinedTransformationMatrix);
		}
	}

	//================================================================================
	//!	���\�b�h��	CSKIN_MESH_BASE::changeAnimationAndBlend
	//
	//	�@�\		�A�j���[�V�����̐ݒ�
	//	�@�\�T�v	�w�肳�ꂽ�A�j���[�V�����ԍ����g���ăA�j���[�V������ύX��
	//				�A�j���[�V�������u�����f�B���O����
	//	����		inAnim			�A�j���[�V�����ԍ�
	//	�X�V		2008/05/30		<�ǉ�>	�R�����g
	//================================================================================
	void
	CSKIN_MESH_BASE::changeAnimationAndBlend(int inAnim)
	{
	//	�V���ȃA�j���[�V�����ƁA���݂̃A�j���[�V��������v����Ȃ�A�������Ȃ�
		if(	currAnimNumber == inAnim)		return;
	//	�V���ȃA�j���[�V�����ԍ����s���Ȃ�A���������Ȃ�
		if(	inAnim <  0		||
			inAnim >= sd_ptr->iAnimCnt)	return;
	//	���݂̃A�j���[�V�����ԍ��ƑO��܂ł̃A�j���[�V�����ԍ���ۑ�
		prevAnimNumber = currAnimNumber;

		DWORD dwNewTrack = inAnim;						//!< �g���b�N�ԍ����w��
		LPD3DXANIMATIONCONTROLLER	pAC;				//!< ��Ɨp�A�j���[�V�����R���g���[��
		LPD3DXANIMATIONSET			pAS;				//!< ��Ɨp�A�j���[�V�����Z�b�g

		spAnimCtrl->AddRef();							//!< �A�j���[�V�����R���g���[���[�̎Q�ƃJ�E���^���C���N�������g����
		pAC = spAnimCtrl;								//!< ��Ɨp�̃A�j���[�V�����R���g���[���[�ŏ���������

		pAC->GetAnimationSet(inAnim, &pAS);				//!< ��Ɨp�A�j���[�V�����Z�b�g���擾
		pAC->SetTrackAnimationSet(dwNewTrack, pAS);		//!< ��Ɨp�A�j���[�V�����Z�b�g���܂ނ悤�Ƀg���b�N��ݒ�
		pAS->Release();									//!< ��Ɨp�A�j���[�V�����Z�b�g���J��

		pAC->UnkeyAllTrackEvents(currAnimNumber);		//!< ���݂̃A�j���[�V�����Ńu�����h�𔭐�
		pAC->UnkeyAllTrackEvents(dwNewTrack);			//!< ���̃A�j���[�V�����Ńu�����h�𔭐�

	//	���ݎg�p���Ă���g���b�N�̃��Z�b�g
	//	�w�肵�����Ԍ�Ƀg���b�N�𖳌��ɂ���
		pAC->KeyTrackEnable	(currAnimNumber, FALSE, animTime + kMOVE_TRANSITION_TIME);
	//	�Đ��X�s�[�h�̃��Z�b�g
		pAC->KeyTrackSpeed	(currAnimNumber,  0.0f, animTime, kMOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	//	�d�݂̃��Z�b�g
		pAC->KeyTrackWeight	(currAnimNumber,  0.0f, animTime, kMOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);

	//	���Ɏg�p����g���b�N�̃Z�b�g
	//	�Đ��X�s�[�h�̃Z�b�g
		pAC->KeyTrackSpeed	(dwNewTrack, 1.0f, animTime, kMOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	//	�d�݂̃Z�b�g
		pAC->KeyTrackWeight	(dwNewTrack, 1.0f, animTime, kMOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	//	�g���b�N��L���ɂ���
		pAC->SetTrackEnable	(dwNewTrack, TRUE);

		currAnimNumber = dwNewTrack;					//!< ���̃g���b�N�����݂̃g���b�N�ɂ���
		pAC->Release();									//!< ��Ɨp�A�j���[�V�����R���g���[�������

		spAnimCtrl->ResetTime();						//!< �A�j���[�V���������Z�b�g
		animTime		= 0.0f;							//!< �A�j���[�V�������Ԃ�������
		setPlayAnim(TRUE);								//!< �A�j���[�V�������Đ�
		bAnimChange	= TRUE;								//!< �A�j���[�V�������u�����h������
	}
}
//-----------------------------------------------------------------------
//!	�X�L�����b�V���x�[�X�̖��O���
//-----------------------------------------------------------------------