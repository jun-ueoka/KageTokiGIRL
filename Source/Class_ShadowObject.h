//***********************************************************************************
//!	�t�@�C����		Class_ShadowObject.h
//
//	�@�\			�e���̂Ɋւ����`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/08/29	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_Mesh.h"					//!< 3D�I�u�W�F�N�g���b�V��
#include	"Class_TextureBase.h"			//!< �e�N�X�`���x�[�X

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�e���̃t�@�C���ւ̃p�X
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_SHADOW_OBJECT		"..//Resources//Data//ShadowObject//%s.sob"

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�e���̍\���^�C�v
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum	SHADOW_OBJ_TYPE
	{
		eSHAO_COMPLETE,			//!< �e�����̂����镨��(���S����)
		eSHAO_HALF_REAL,		//!< �e���Ȃ�����(������)
		eSHAO_HALF_SHADOW,		//!< �e�����̕���(���e����)
		eSHAO_INCOMPLETE,		//!< ���̂ɉe(�e���@)����������(�����S����)

		eSHAO_NONE,				//!< �e�����̂��Ȃ����
		eSHAO_MAX				//!< �e���̍\���^�C�v�̍ő吔

	}	SHAO_TYPE;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�e���̓���
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum	SHADOW_OBJ_ACTION
	{
		eSHAOAC_SHADOW_REVIVAL,	//!< �e���t�����Ă���
		eSHAOAC_SHADOW_DELETE,	//!< �e���������ł��Ă���
		eSHAOAC_DELETE,			//!< ���ł��Ă���

		eSHAOAC_NONE,			//!< �e���̓���Ȃ�
		eSHAOAC_MAX				//!< �e���̃��A�N�V�����̍ő吔

	}	SHAOAC_TYPE;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�e���̃f�[�^
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct SHADOW_OBJECT_DATA
	{
		int				cnt;			//!< �g�p��
		string			name;			//!< �e���̃f�[�^
		CMESH_BASE*		shadowObj;		//!< �e���b�V��
		CMESH_BASE*		hitObj;			//!< �Փ˔���p���b�V��

	//	���ԉ����̏�����
		SHADOW_OBJECT_DATA(void)
		{
			this->cnt		= 0;
			this->name		= "";
			this->shadowObj	= NULL;
			this->hitObj	= NULL;
		}
	//	���
		~SHADOW_OBJECT_DATA(void)
		{
			SAFE_DELETE(this->shadowObj);
			SAFE_DELETE(this->hitObj);
		}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CSHADOW_OBJECT
	//
	//	�@�\			�e����(���b�V����)
	//	�X�V			2008/08/29	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CSHADOW_OBJECT : public C3D_OBJECT
	{
			//==================================================================
			//	�������ە�����
			//==================================================================
				#define	NONE_OBJECT				"NO_OBJECT"

			//==================================================================
			//	�I�u�W�F�N�g�}�N��
			//==================================================================
				#define	CHANGE_VALUE_DIFFUSE	0.02f	//!< �f�B�t���[�Y�ϓ��l

	private:		//-------------------- private --------------------//

			//==================================================================
			//	�e���̃}�b�v�i�C���X�^���X�ŋ��L�������j
			//==================================================================
		static	map<string, SHADOW_OBJECT_DATA*>*		mShadowData;

			//==================================================================
			//	�e���̃f�[�^�̐���
			//==================================================================
		static	SHADOW_OBJECT_DATA*	createShadowData(CONST DEV   inDev,
													 CONST LPSTR inName,
													 CONST LPSTR inResource = PATH_SHADOW_OBJECT);

			//==================================================================
			//	�����̏�������
			//==================================================================
				void	startShadowObjectInitialize(void);

			//==================================================================
			//	�G�t�F�N�g�̏������A���
			//==================================================================
				void	initializeShadowObject(CONST DEV		inDev,
											   SHADOW_OBJ_TYPE	inType,
											   CONST LPSTR		inName,
											   CONST LPSTR		inResource);
				void	finishalizeShadowObject(void);

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	�e���̏��|�C���^�[
			//==================================================================
				SHADOW_OBJECT_DATA*		so_ptr;

			//==================================================================
			//	�e���̂̃��[�h�m�F
			//==================================================================
				BOOL					bShadowObjectMeshLoaded;

			//==================================================================
			//	�e���̂̃X�e�[�^�X
			//==================================================================
				SHADOW_OBJ_TYPE			shadowObjType;				//!< �e���̍\���^�C�v
				SHADOW_OBJ_ACTION		shadowObjAction;			//!< �e���̓���

			//==================================================================
			//	�`��f�B�t���[�Y
			//==================================================================
				D3DCOLORVALUE			shadowDiffuse;

	public:			//-------------------- public --------------------//

				CSHADOW_OBJECT(CONST DEV       inDev,
							   SHADOW_OBJ_TYPE inType,
							   CONST LPSTR     inName,
							   CONST LPSTR     inResource	= PATH_SHADOW_OBJECT);
				CSHADOW_OBJECT(void);								//!< �R���X�g���N�^(�󏉊���)
				CSHADOW_OBJECT(CSHADOW_OBJECT* inCopy);				//!< �R�s�[
		virtual	~CSHADOW_OBJECT(void);

			//==================================================================
			//	access	�e���̏��
			//==================================================================
				SHADOW_OBJECT_DATA*	getShadowObjects(void)const			{ return	so_ptr;			}

			//==================================================================
			//	access	�e���̂̍\��
			//==================================================================
				SHADOW_OBJ_TYPE	getShadowObjType(void)const				{ return	shadowObjType;	}
				void			setShadowObjType(SHADOW_OBJ_TYPE inType);

			//==================================================================
			//	���A�N�V����
			//==================================================================
				void	reactionMessage(REACTION_MESSAGE inMessage);

			//==================================================================
			//	�e���̂̓���
			//==================================================================
				void	actionShadowObj(void);

			//==================================================================
			//	���[���h���W�n�̒��S�_
			//==================================================================
				VEC3	getWorldCenter(void);

			//==================================================================
			//	���[���h���W�n�̔��a
			//==================================================================
				float	getWorldRadius(void)
				{
					if(so_ptr)	{ return so_ptr->shadowObj->getMeshs()->localRadius * getScale();	}
					else		{ return C3D_OBJECT::getWorldRadius();								}
				}

			//==================================================================
			//	�Ώۂ̂w���b�V���Ƃ̏Փ˔���(�ȈՔ�)
			//==================================================================
				BOOL	intersectMesh(C3D_OBJECT* inObject, VEC3* inRayLoc, LINE* inLine, float* inDist);

			//==================================================================
			//	���g�ƃI�u�W�F�N�g�Ƃ́A���a�ł̏����Փ˔���
			//==================================================================
				BOOL	intersectObject(C3D_OBJECT* inObject);

			//==================================================================
			//	���b�Z�[�W�ʏՓ˔���
			//==================================================================
				BOOL	intersectMsgCheck(THIS_MESSAGE inCheckMsg);

			//==================================================================
			//	�S�����A�㉺�A���E�A�O��ŁA�Փ˔���(�T�u�N���X�ŃI�[�o�[���C�h�\)
			//==================================================================
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE* inLine);
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown);
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight);
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear);

			//==================================================================
			//	�S�����A�㉺�A���E�A�O��ɁA�w�肵�������ŏՓ˔���(�T�u�N���X�ŃI�[�o�[���C�h�\)
			//==================================================================
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE* inLine, float inIntersectValue);
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown, float inIntersectValue);
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight, float inIntersectValue);
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear, float inIntersectValue);

			//==================================================================
			//	�`��
			//==================================================================
		virtual	BOOL	draw(CONST DEV		inDev);
		virtual	BOOL	draw(CZTEX_SHA*		inZTexSha);
		virtual	BOOL	draw(CDEPTH_SHA*	inDepthSha);
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------