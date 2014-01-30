//***********************************************************************************
//!	�t�@�C����		Class_SkinMeshBase.h
//
//	�@�\			�X�L�����b�V���x�[�X�Ɋւ����`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/08/30	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_MeshBase.h"				//!< ���b�V���x�[�X

//-----------------------------------------------------------------------
//!	�X�L�����b�V���x�[�X�̖��O���
//-----------------------------------------------------------------------
namespace SMB
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	���O��Ԃ̉��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	using	namespace	MB;			//!< ���b�V���x�[�X

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�A�j���[�V�����Ɋւ����`
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		kIDLE_TRANSITION_TIME	0.125f
	#define		kMOVE_TRANSITION_TIME	0.25f
	#define		kANIM_SPEED				0.016f

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�X�L�����b�V�����\�[�X�t�@�C���ւ̃p�X
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_SKINMESH		"..//Resources//SkinMesh//%s.x"
	#define		PATH_SKINMESHTEX	"..//Resources//SkinMesh//Texture//%s"
	#define		PATH_SKINMESHDATA	"..//Resources//SkinMesh//SkinMeshData//%s.smd"

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�h���t���[���\���� (���ꂼ��̃��b�V���p�̍ŏI���[���h�s���ǉ�����j
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct MYFRAME : public D3DXFRAME
	{
		D3DXMATRIX	CombinedTransformationMatrix;	//!< ���ꂼ��̃��b�V���p�̍ŏI���[���h�s��
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�h�����b�V���R���e�i�[�\����
	//	�R���e�i�[���e�N�X�`���𕡐����Ă�悤�Ƀ|�C���^�[�̃|�C���^�[��ǉ�����j
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct MYMESHCONTAINER : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*	ppTextures;				//!< �e�N�X�`����
		DWORD				dwWeight;				//!< �d�݂̌��i�d�݂Ƃ͒��_�ւ̉e���B�j
		DWORD				dwBoneAmt;				//!< �{�[���̐�
		LPD3DXBUFFER		pBoneBuffer;			//!< �{�[���E�e�[�u��
		D3DXMATRIX**		ppBoneMatrix;			//!< �S�Ẵ{�[���̃��[���h�s��̐擪�|�C���^�[
		D3DXMATRIX*			pBoneOffsetMatrices;	//!< �t���[���Ƃ��Ẵ{�[���̃��[���h�s��̃|�C���^�[
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	X�t�@�C�����̃A�j���[�V�����K�w��ǂ݉����Ă����N���X��h��������
	//	ID3DXAllocateHierarchy�͔h�����邱�Ƒz�肵�Đ݌v����Ă���
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class MY_HIERARCHY: public ID3DXAllocateHierarchy
	{
	private:
	//	�ǂݍ��ރe�N�X�`���̃p�X
		LPSTR		keepTexPath;

	public:
	//	�R���X�g���N�^
		MY_HIERARCHY(CONST LPSTR inPath = NULL)	{ keepTexPath = inPath;		}

	//	�t���[�����\�z����
		STDMETHOD(CreateFrame)			(THIS_ LPCSTR,
										 LPD3DXFRAME*);
	//	���b�V���R���e�i�[���\�z����
		STDMETHOD(CreateMeshContainer)	(THIS_ LPCTSTR,
										 CONST D3DXMESHDATA*,
										 CONST D3DXMATERIAL*, 
										 CONST D3DXEFFECTINSTANCE*,
										 DWORD,
										 CONST DWORD*,
										 LPD3DXSKININFO,
										 LPD3DXMESHCONTAINER*);
	//	�t���[�����������
		STDMETHOD(DestroyFrame)			(THIS_ LPD3DXFRAME);
	//	���b�V���R���e�i�[���������
		STDMETHOD(DestroyMeshContainer)	(THIS_ LPD3DXMESHCONTAINER);	
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	������𔻒f����
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	HRESULT	AllocateName(LPCSTR inName, LPSTR* ioNewName);

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�X�L�����b�V�������i�[����\����
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct SKIN_DATA
	{
		MESH_DATA*					mesh;					//!< ���b�V�����
		int							cnt;					//!< �g�p��
		string						name;					//!< �X�L�����b�V����
		string						rPath;					//!< ���\�[�X�p�X
		string						tPath;					//!< �e�N�X�`���p�X
		LPD3DXFRAME					pFrameRoot;				//!< ���[�g�t���[���i��ԃG�����t���[���j
		LPD3DXANIMATIONCONTROLLER	pAnimController;		//!< �A�j���[�V�����R���g���[���[
		LPD3DXANIMATIONSET*			pAnimSet;				//!< �A�j���[�V�����Z�b�g
		int							iAnimCnt;				//!< �A�j���[�V�����̐�
		int*						iAnimStep;				//!< �A�j���[�V�����X�e�b�v

	//	���ԉ����̏�����
		SKIN_DATA(void)
		{
			mesh			= NULL;
			cnt				= 0;
			name			= "";
			rPath			= "";
			tPath			= "";
			pFrameRoot		= NULL;
			pAnimController	= NULL;
			pAnimSet		= NULL;
			iAnimCnt		= 0;
			iAnimStep		= NULL;
		}

	//	���������
		~SKIN_DATA(void)
		{
			releaseData();
		}

	//	���
		void releaseData(void)
		{
			MY_HIERARCHY ah;
		//	�{�[���̉��
			ah.DestroyFrame(pFrameRoot);
			pFrameRoot = NULL;
		//	�A�j���[�V�����R���g���[���[�̉��
			SAFE_RELEASE(pAnimController);
		//	�A�j���[�V�����Z�b�g�̉��
			for(int i=0; i<iAnimCnt; i++)
			{
				SAFE_RELEASE(pAnimSet[i]);
			}
			SAFE_DELETE_ARRAY(pAnimSet);
		//	���b�V�����̉��
			SAFE_DELETE(mesh);
		//	�A�j���[�V�����X�e�b�v�̉��
			SAFE_DELETE_ARRAY(iAnimStep);
		}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CSKIN_MESH
	//
	//	�@�\			�X�L�����b�V���x�[�X
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSKIN_MESH_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	�X�L�����b�V���ǂݍ��݃��\�b�h
			//==================================================================
		static	BOOL		createSkinInData(CONST DEV   inDev,
											 SKIN_DATA*  inSkinData,
											 CONST LPSTR inName,
											 CONST LPSTR inResource,
											 CONST LPSTR inTexture);
		static	SKIN_DATA*	createSkin(CONST DEV   inDev,
									   CONST LPSTR inName,
									   CONST LPSTR inResource,
									   CONST LPSTR inTexture);

			//==================================================================
			//	�X�L���}�b�v�i�C���X�^���X�ŋ��L�������j
			//==================================================================
		static	map<string, SKIN_DATA*>*		mSkins;

			//==================================================================
			//	�����̏�������
			//==================================================================
				void		startSkinMeshInitialize(void);

			//==================================================================
			//	���b�V���̏������A���
			//==================================================================
				void		initializeSkinMesh(	CONST DEV   inDev,
												CONST LPSTR inName,
												CONST LPSTR inResource = PATH_SKINMESH,
												CONST LPSTR inTexture  = PATH_SKINMESHTEX);
				void		finishalizeSkinMesh(void);

			//==================================================================
			//	�A�j���[�V�����Ɋւ��鑮��
			//==================================================================
				float	animSpeed;							//!< �A�j���[�V�����X�s�[�h�i��t���[�������艽�b�Ԏ��Ԃ��i�ނ̂��j
				float	animTime;							//!< �A�j���[�V�������ԁi���݂ǂ̎��Ԃ̓��������Ă��邩�j

				BOOL	bPlayAnim;							//!< �A�j���[�V�����X�C�b�`�iTRUE�Ȃ�A�j���[�g����j
				int		prevAnimNumber;						//!< �O��̃A�j���[�V�����ԍ�
				int		currAnimNumber;						//!< ���݂̃A�j���[�V�����ԍ�
				int		nextAnimNumber;						//!< ����̃A�j���[�V�����ԍ�
				BOOL	bAnimChange;						//!< �A�j���[�V�����`�F���W�m�F

				LPD3DXANIMATIONCONTROLLER	spAnimCtrl;		//!< ��p�̃A�j���R���g���[���[

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	�X�L�����b�V�����|�C���^�[
			//==================================================================
				SKIN_DATA*	sd_ptr;

			//==================================================================
			//	�X�L�����b�V�����[�h�m�F
			//==================================================================
				BOOL		bSkinLoaded;

	public:			//-------------------- public --------------------//

				CSKIN_MESH_BASE(CONST DEV   inDev,
								CONST LPSTR inName,
								CONST LPSTR inResource = PATH_SKINMESH,
								CONST LPSTR inTexture  = PATH_SKINMESHTEX);	//!< �R���X�g���N�^
				CSKIN_MESH_BASE(void);										//!< �R���X�g���N�^(�󏉊���)
				CSKIN_MESH_BASE(CSKIN_MESH_BASE* inCopy);					//!< �R�s�[
		virtual	~CSKIN_MESH_BASE(void);										//!< �f�X�g���N�^

			//==================================================================
			//	���Z�b�g���s
			//==================================================================
		static	void	resetInstance(CONST DEV inDev);

			//==================================================================
			//	�X�L�����b�V���̖������𒲂ׂ�
			//==================================================================
				BOOL	skinInvalid(void);

			//==================================================================
			//	access	�A�j���[�V�����X�C�b�`
			//==================================================================
				BOOL	getPlayAnim(void)const				{ return	bPlayAnim;				}
				void	setPlayAnim(BOOL inPlayAnim)		{ bPlayAnim = inPlayAnim;			}

			//==================================================================
			//	access	���Z����A�j���[�V�������ԁi�A�j���[�V�����i�s���x�j
			//==================================================================
				float	getAnimSpeed(void)const							{ return	animSpeed;		}
				void	setAnimSpeed(float inAnimSpeed = kANIM_SPEED)	{ animSpeed  = inAnimSpeed;	}
				void	calcAnimSpeed(float inAnimSpeed)				{ animSpeed += inAnimSpeed;	}

			//==================================================================
			//	access	���݂̃A�j���[�V��������
			//==================================================================
				float	getAnimTime(void)const				{ return	animTime;				}
				void	setAnimTime(float inAnimTime)		{ animTime = inAnimTime;			}

			//==================================================================
			//	access	���݂̃A�j���[�V�����̔ԍ�
			//==================================================================
				int		getCurrAnimNumber(void)const		{ return	currAnimNumber;			}
				void	setCurrAnimNumber(int inAnimNumber)	{ prevAnimNumber = currAnimNumber;
															  currAnimNumber = inAnimNumber;	}

			//==================================================================
			//	access	����̃A�j���[�V�����̔ԍ�
			//==================================================================
				int		getNextAnimNumber(void)const		{ return	nextAnimNumber;			}
				void	setNextAnimNumber(int inAnimNumber)	{ nextAnimNumber = inAnimNumber;	}

			//==================================================================
			//	access	�X�L�����b�V�����
			//==================================================================
				SKIN_DATA*	getSkins(void)const				{ return	sd_ptr;					}

			//==================================================================
			//	�{�[���s��̏�����
			//==================================================================
		static	HRESULT	AllocateBoneMatrix(SKIN_DATA*, LPD3DXMESHCONTAINER);
		static	HRESULT	AllocateAllBoneMatrices(SKIN_DATA*,  LPD3DXFRAME);

			//==================================================================
			//	���b�V���R���e�i�[�`��
			//==================================================================
				void	RenderMeshContainer(CONST DEV, MYMESHCONTAINER* ,MYFRAME*);

			//==================================================================
			//	�t���[�����Ƃ̃��[���h�ϊ����s��
			//==================================================================
				void	UpdateFrameMatrices(LPD3DXFRAME, LPD3DXMATRIX);

			//==================================================================
			//	�t���[����`�悷��
			//==================================================================
				void	DrawFrame(CONST DEV, LPD3DXFRAME);

			//==================================================================
			//	�A�j���[�V�����ύX���\�b�h
			//==================================================================
				void	changeAnimationAndBlend(int inNum);

			//==================================================================
			//	�A�j���[�V����
			//==================================================================
				BOOL	animation(void);

			//==================================================================
			//	�`��
			//==================================================================
				BOOL	drawSkin(CONST DEV inDev, MAT16* inMatrix);
	};
}
//-----------------------------------------------------------------------
//!	�X�L�����b�V���x�[�X�̖��O���
//-----------------------------------------------------------------------