//***********************************************************************************
//!	�t�@�C����		Class_3DMap.h
//
//	�@�\			3D�I�u�W�F�N�g�Ō`�����ꂽ�}�b�v�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/07/20	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_3DObject.h"				//!< 3D�I�u�W�F�N�g
#include	"Class_3DObjectGroup.h"			//!< 3D�I�u�W�F�N�g�O���[�v

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	���b�V�����\�[�X�t�@�C���ւ̃p�X
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_MAP		"..//Resources//Data//Map//Map%d.dat"

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�I�u�W�F�N�g�^�C�v
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum	GAMEMAP_OBJECT_TYPE
	{
		eGMO_NORMALMESH	= 0,			//!< �ʏ탁�b�V��				//!< MESH______ 
		eGMO_NORMALSKINMESH,			//!< �ʏ�X�L�����b�V��			//!< SKIN______
		eGMO_NORMALTEXTURE_CENTER,		//!< �ʏ�e�N�X�`��(���_�����S)	//!< TEX_CEN___
		eGMO_NORMALTEXTURE_BOTTOM,		//!< �ʏ�e�N�X�`��(���_����)	//!< TEX_BOT___
		eGMO_SHADOWOBJ_COM_SUBSTANCE,	//!< �e�I�u�W�F�N�g(���S����)	//!< SO_COMP___
		eGMO_SHADOWOBJ_HALF_SHADOW,		//!< �e�I�u�W�F�N�g(���e����)	//!< SO_HALFSHA
		eGMO_SHADOWOBJ_HALF_REAL,		//!< �e�I�u�W�F�N�g(������)		//!< SO_HALFREA
		eGMO_SHADOWOBJ_INCOM_SUBSTANCE,	//!< �e�I�u�W�F�N�g(�����S����)	//!< SO_INCOMP_
		eGMO_MAX						//!< �I�u�W�F�N�g�^�C�v�̍ő吔

	}	GMO_TYPE;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�ʏ�O���[�v�^�C�v
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum	MAP_GROUP_TYPE
	{
		eMG_VIEW_HIT	= 0,			//!< �`��ƏՓ�					//!< VIEW_HIT__
		eMG_VIEW,						//!< �`��						//!< VIEW______
		eMG_BACK_VIEW_HIT,				//!< �w�i�`��ƏՓ�				//!< BACK_VIEWH
		eMG_BACK_VIEW,					//!< �w�i�`��					//!< BACK_VIEW_
		eMG_HIT,						//!< �Փ�						//!< HIT_______
		eMG_ENEMY,						//!< �G							//!< ENEMY_____
		eMG_MAX							//!< �^�C�v�̍ő吔
										//!< �f�[�^�̏I�[				//!< END_______
	}	MG_TYPE;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�����I�u�W�F�N�g�f�[�^
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	struct	CREATE_OBJECT_DATA
	{
		char	szData[MAX_PATH];		//!< �f�[�^�^�C�v
		char	szName[MAX_PATH];		//!< �O���t�@�C����
		VEC3	loc;					//!< �ʒu
		VEC3	angle;					//!< �p�x
		VEC2	scale;					//!< �g���

	}	OB_DATA;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		C3D_MAP
	//
	//	�@�\			3D�I�u�W�F�N�g�Ō`�����ꂽ�}�b�v
	//	�@�\�T�v		���S�I�u�W�F�N�g�ƁA�͈͂�ݒ肵�āA
	//					�͈͂̒��ɓ����Ă���I�u�W�F�N�g��`�悵�A
	//					�����Ă��Ȃ��I�u�W�F�N�g�͕`�悵�Ȃ�
	//	�X�V			2008/07/20	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class C3D_MAP
	{
	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	�`��X�e�[�^�X
			//==================================================================
				float					fDistance;			//!< �`�拖�e���a

				C3D_OBJECT_GROUP		releaseG;			//!< ����O���[�v
				C3D_OBJECT_GROUP		viewG;				//!< �`��O���[�v
				C3D_OBJECT_GROUP		backG;				//!< �w�i�O���[�v
				C3D_OBJECT_GROUP		hitG;				//!< �Փ˃O���[�v

				C3D_OBJECT_GROUP		squeezeViewG;		//!< �i�荞�ݕ`��O���[�v
				C3D_OBJECT_GROUP		squeezeBackG;		//!< �i�荞�ݔw�i�O���[�v

			//==================================================================
			//	�I�u�W�F�N�g����
			//==================================================================
		virtual	BOOL	createObject(CONST DEV	  inDev,
									 C3D_OBJECT** inObj,
									 OB_DATA*	  inData)	{ return FALSE;	}

			//==================================================================
			//	���������I�u�W�F�N�g�̃O���[�v�o�^
			//==================================================================
		virtual	BOOL	addObject(C3D_OBJECT* inObject, int inGroupType);

			//==================================================================
			//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N
			//==================================================================
		virtual	BOOL	checkMessage(vector<C3D_OBJECT*>::iterator* inObjP);

	public:			//-------------------- public --------------------//

				C3D_MAP(void);								//!< �R���X�g���N�^
		virtual	~C3D_MAP(void);								//!< �f�X�g���N�^

			//==================================================================
			//	access	�`��X�e�[�^�X
			//==================================================================
				float		getDistance(void)const			{ return	fDistance;		}
				void		setDistance(float inDistance)	{ fDistance = inDistance;	}

			//==================================================================
			//	access	�e�O���[�v
			//==================================================================
				C3D_OBJECT_GROUP*	getViewG(void)					{ return	&viewG;	}
				C3D_OBJECT_GROUP*	getBackG(void)					{ return	&backG;	}
				C3D_OBJECT_GROUP*	getHitG(void)					{ return	&hitG;	}
				void				addViewG(C3D_OBJECT* inObj)		{ viewG.add(inObj);	}
				void				addBackG(C3D_OBJECT* inObj)		{ backG.add(inObj);	}
				void				addHitG(C3D_OBJECT* inObj)		{ hitG.add(inObj);	}

			//==================================================================
			//	�����A���
			//==================================================================
				void	initialize(CONST DEV inDev,
								   int		 inTxtNum);		//!< �O���ǂݍ���
		virtual	void	finalize(void);						//!< ���

			//==================================================================
			//	�`��
			//==================================================================
				void	draw(CONST DEV inDev, C3D_OBJECT* inCamera);
				void	draw(CONST DEV inDev, C3D_OBJECT* inCamera, VEC3* inViewPoint);
				void	draw(CONST DEV inDev, VEC3* inViewPoint);
				void	draw(CZTEX_SHA*  inZTexSha,  VEC3* inViewPoint);
				void	draw(CDEPTH_SHA* inDepthSha, VEC3* inViewPoint);
				void	drawNotCheckMessage(CDEPTH_SHA* inDepthSha, VEC3* inViewPoint);

			//==================================================================
			//	�i�荞�ݏ���
			//==================================================================
				void	squeezeSortByCameraPosition(C3D_OBJECT* inCamera, VEC3* inViewPoint);
				void	squeezeSortByCameraPositionWithDrawZTexSha(
							C3D_OBJECT* inCamera, VEC3* inViewPoint, CZTEX_SHA*  inZTexSha);
				void	squeezeSortByCameraPositionNotCheckMessageWithDrawZTexSha(
							C3D_OBJECT* inCamera, VEC3* inViewPoint, CZTEX_SHA*  inZTexSha);
				void	squeezeDrawWithZTexSha(CZTEX_SHA*  inZTexSha);
				void	squeezeDrawWithDepthSha(CDEPTH_SHA* inDepthSha);
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------