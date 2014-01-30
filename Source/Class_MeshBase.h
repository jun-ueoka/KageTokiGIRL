//***********************************************************************************
//!	�t�@�C����		Class_MeshBase.h
//
//	�@�\			���b�V���x�[�X�Ɋւ����`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/08/30	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"DirectX.h"				//!< DirectX�g�p

//-----------------------------------------------------------------------
//!	���b�V���x�[�X�̖��O���
//-----------------------------------------------------------------------
namespace MB
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	���b�V�����\�[�X�t�@�C���ւ̃p�X
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_MESH		"..//Resources//Mesh//%s.x"
	#define		PATH_MESHTEX	"..//Resources//Mesh//Texture//%s"

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	���b�V�����
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	MESH_DATA
	{
		int		cnt;				//!< �g�p��
		string	name;				//!< ���b�V����
		string	rPath;				//!< ���\�[�X�p�X
		string	tPath;				//!< �e�N�X�`���p�X
		XMESH	mesh;				//!< ���b�V��
		VTEX9	meshVB;				//!< ���_�o�b�t�@
		DWORD	numMaterials;		//!< �}�e���A����
		MTR*	materials;			//!< �}�e���A��
		TEX*	texture;			//!< �e�N�X�`��
		VEC3	localCenter;		//!< ���S�_
		float	localRadius;		//!< ���a

	//	���ԉ����̏�����
		MESH_DATA(void)
		{
			cnt				= 0;
			name			= "";
			rPath			= "";
			tPath			= "";
			mesh			= NULL;
			meshVB			= NULL;
			numMaterials	= 0;
			materials		= NULL;
			texture			= NULL;
			localCenter		= VEC3(0.0f, 0.0f, 0.0f);
			localRadius		= 0.0f;
		};

	//	���������
		~MESH_DATA(void)
		{
			releaseData();
		};

	//	���
		void releaseData(void)
		{
		//	�e�N�X�`�����X�g����e�N�X�`���̃N���A
			if(	texture)
			{
				for(DWORD i = 0; i < numMaterials; i++)
				{
					SAFE_RELEASE(texture[i]);
				}
				SAFE_DELETE_ARRAY(texture);
			}
		//	�}�e���A�����X�g����}�e���A���̃N���A
			SAFE_DELETE_ARRAY(materials);
		//	���b�V���̃N���A
			SAFE_RELEASE(mesh);
		}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CMESH_BASE
	//
	//	�@�\			���b�V���x�[�X
	//	�X�V			2007/12/08	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CMESH_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	���b�V���̐���
			//==================================================================
		static	BOOL		createMeshInData(CONST DEV   inDev,
											 MESH_DATA*  inMeshData,
											 CONST LPSTR inName,
											 CONST LPSTR inResource,
											 CONST LPSTR inTexture);
		static	MESH_DATA*	createMesh(CONST DEV   inDev,
									   CONST LPSTR inName,
									   CONST LPSTR inResource,
									   CONST LPSTR inTexture);

			//==================================================================
			//	���b�V���}�b�v�i�C���X�^���X�ŋ��L�������j
			//==================================================================
		static	map<string, MESH_DATA*>*		mMeshs;

			//==================================================================
			//	�����̏�������
			//==================================================================
				void		startMeshInitialize(void);

			//==================================================================
			//	���b�V���̏������A���
			//==================================================================
				void		initializeMesh(	CONST DEV   inDev,
											CONST LPSTR inName,
											CONST LPSTR inResource = PATH_MESH,
											CONST LPSTR inTexture  = PATH_MESHTEX);
				void		finishalizeMesh(void);


	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	���b�V�����|�C���^�[
			//==================================================================
				MESH_DATA*	md_ptr;

			//==================================================================
			//	���b�V���̃��[�h�m�F
			//==================================================================
				BOOL		bMeshLoaded;

	public:			//-------------------- public --------------------//

				CMESH_BASE(	CONST DEV	inDev,
							CONST LPSTR	inName,
							CONST LPSTR	inResource = PATH_MESH,
							CONST LPSTR	inTexture  = PATH_MESHTEX);		//!< �R���X�g���N�^
				CMESH_BASE(void);										//!< �R���X�g���N�^(�󏉊���)
				CMESH_BASE(CMESH_BASE* inCopy);							//!< �R�s�[
		virtual	~CMESH_BASE(void);										//!< �f�X�g���N�^

			//==================================================================
			//	���Z�b�g���s
			//==================================================================
		static	void	resetInstance(CONST DEV inDev);

			//==================================================================
			//	���b�V���̖������𒲂ׂ�
			//==================================================================
				BOOL		meshInvalid(void);

			//==================================================================
			//	access	���b�V�����
			//==================================================================
				XMESH		getXMesh(void)			{ return	md_ptr->mesh;			}
				VEC3		getLocalCenter(void)	{ return	md_ptr->localCenter;	}
				float		getLocalRadius(void)	{ return	md_ptr->localRadius;	}

			//==================================================================
			//	access	���b�V�����
			//==================================================================
				MESH_DATA*	getMeshs(void)const		{ return	md_ptr;					}

			//==================================================================
			//	�`��
			//==================================================================
				BOOL	drawMesh(CONST DEV inDev, MAT16* inMatrix, float inCalcDiffuse = 0.0f);
				BOOL	drawMesh(CONST DEV inDev, MAT16* inMatrix, D3DCOLORVALUE* inDiffuse);
	};
}
//-----------------------------------------------------------------------
//!	���b�V���x�[�X�̖��O���
//-----------------------------------------------------------------------