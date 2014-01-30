//***********************************************************************************
//!	�t�@�C����		Class_Mesh.h
//
//	�@�\			���b�V���I�u�W�F�N�g�Ɋւ����`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2007/12/08	<�V�K>
//					2008/06/25	<�ǉ�>	�S�̓I�ɃR�����g�̋���
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_3DObject.h"				//!< 3D�I�u�W�F�N�g
#include	"Class_MeshBase.h"				//!< ���b�V���x�[�X

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	���O��Ԃ̉��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	using	namespace	MB;		//!< ���b�V���x�[�X

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CMESH
	//
	//	�@�\			�w�t�@�C���`���̃��b�V��
	//	�X�V			2007/12/08	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CMESH : public C3D_OBJECT, public CMESH_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	FVF�̐ݒ�
			//==================================================================
		#define	D3DFVF_CLONEVERTEX		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

			//==================================================================
			//	���b�V���ƃ��b�V���Ƃ̓�����𒲂ׂ�
			//==================================================================
		static	BOOL		enumHitMesh(CMESH* inDefender,
										CMESH* inAttacker,
										LINE*  inLine);

	public:			//-------------------- public --------------------//

				CMESH(CONST DEV   inDev,
					  CONST LPSTR inName,
					  CONST LPSTR inResource = PATH_MESH,
					  CONST	LPSTR inTexture  = PATH_MESHTEX);		//!< �R���X�g���N�^
				CMESH(void);										//!< �R���X�g���N�^(�󏉊���)
				CMESH(CMESH* inCopy);								//!< �R�s�[
		virtual	~CMESH(void);										//!< �f�X�g���N�^

			//==================================================================
			//	access	���b�V�����
			//==================================================================
				XMESH	getMesh(void)			{ return	md_ptr->mesh;			}

			//==================================================================
			//	���[���h���W�n�̒��S�_
			//==================================================================
				VEC3	getWorldCenter(void);

			//==================================================================
			//	���[���h���W�n�̔��a
			//==================================================================
				float	getWorldRadius(void)	{ return	md_ptr->localRadius * getScale();	}

			//==================================================================
			//	�Ώۂ̂w���b�V���Ƃ̏Փ˔���(�ȈՔ�)
			//==================================================================
				BOOL	intersectMesh(C3D_OBJECT* inObject, VEC3* inRayLoc, LINE* inLine, float* inDist);

			//==================================================================
			//	�S�����A�㉺�A���E�A�O��ŁA�Փ˔���
			//==================================================================
				BOOL	intersect(C3D_OBJECT* inObject, LINE* inLine);
				BOOL	intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown);
				BOOL	intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight);
				BOOL	intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear);

			//==================================================================
			//	�S�����A�㉺�A���E�A�O��ɁA�w�肵�������ŏՓ˔���
			//==================================================================
				BOOL	intersect(C3D_OBJECT* inObject, LINE* inLine, float inIntersectValue);
				BOOL	intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown, float inIntersectValue);
				BOOL	intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight, float inIntersectValue);
				BOOL	intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear, float inIntersectValue);

			//==================================================================
			//	�S�����A�㉺�A���E�A�O��ɁA�����Ɣ���J�n�ʒu���w�肵�āA�����蔻��
			//==================================================================
				BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE* inLine, float inIntersectValue);
				BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE_UPDOWN inUpDown, float inIntersectValue);
				BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE_LEFTRIGHT inLeftRight, float inIntersectValue);
				BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE_FRONTREAR inFrontRear, float inIntersectValue);

			//==================================================================
			//	�R�s�[����(�T�u�N���X�ŃI�[�o�[���C�h)
			//==================================================================
		virtual	void	createCopy(C3D_OBJECT** inCopy, VEC3* inLoc)
				{
					*inCopy	= new CMESH(this);
					(*inCopy)->setLoc(inLoc);
				}

			//==================================================================
			//	�`��(�T�u�N���X�ŃI�[�o�[���C�h�\)
			//==================================================================
		virtual	BOOL	draw(CONST DEV		inDev);
		virtual	BOOL	draw(CZTEX_SHA*		inZTexSha);
		virtual	BOOL	draw(CDEPTH_SHA*	inDepthSha);
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------