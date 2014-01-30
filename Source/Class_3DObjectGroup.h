//***********************************************************************************
//!	�t�@�C����		Class_3DObjectGroup.h
//
//	�@�\			�O�����̕��̂������O���[�v�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2007/12/08	<�V�K>
//					2008/07/09	<�ǉ�>	�S�̓I�ɃR�����g�̋���
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_3DObject.h"

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		C3D_OBJECT_GROUP
	//
	//	�@�\			�O�����̕��̂������O���[�v
	//	�@�\�T�v		����͊e���ōs����
	//	�X�V			2008/07/09	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class C3D_OBJECT_GROUP
	{
	private:		//-------------------- protected --------------------//

				vector<C3D_OBJECT*>		objects;				//!< �I�u�W�F�N�g�O���[�v
				C3D_OBJECT*				prevHitObject;			//!< ���O�̏Փ˃I�u�W�F�N�g

	public:			//-------------------- public --------------------//

				C3D_OBJECT_GROUP(void);							//!< �R���X�g���N�^
				~C3D_OBJECT_GROUP(void);						//!< �f�X�g���N�^

			//==================================================================
			//	�O���[�v����
			//==================================================================
				void	add(C3D_OBJECT* inObject);				//!< �O���[�v�Ƀ����o�[��ǉ�
				void	addG(C3D_OBJECT_GROUP* inGroup);		//!< �O���[�v�ɃO���[�v�̃����o�[��ǉ�

				void	draw(CONST SPRITE inSprite);			//!< �O���[�v�̃X�v���C�g�`��
				void	draw(CONST DEV inDev);					//!< �O���[�v�̃����o�[��S�`��
			//	�O���[�v�̃����o�[�������鋖�e���a�͈͓��ŕ`��
				void	draw(CONST DEV inDev, VEC3* inViewPoint, float inLength);
			//	�O���[�v�̃����o�[�������鋖�e�����͈͓̔��ŕ`��
				void	draw(CONST DEV inDev, VEC3* inViewPoint, VEC3* inRange);

				void	draw(CZTEX_SHA*  inZTexSha);			//!< Z�e�N�X�`���V�F�[�_�`��
				void	draw(CDEPTH_SHA* inDepthSha);			//!< �[�x�o�b�t�@�V�F�[�_�`��

				void	erase(C3D_OBJECT* eraceObject);			//!< �O���[�v���烁���o�[���폜

			//==================================================================
			//	�̈�}�b�v����
			//==================================================================
			//	void	addMap(C3D_OBJECT* inObject);			//!< �̈�}�b�v
			////	�̈�}�b�v�̃����o�[�𔻒�̈�Ŕ��肵�Ȃ���`��
			//	void	drawMap(CONST DEV inDev, VEC3* inViewPoint, float inLength);
			//	void	eraseMap(C3D_OBJECT* eraceObject);		//!< �̈�}�b�v���烁���o�[���폜

			//==================================================================
			//	�O���[�v���烁���o�[���폜
			//==================================================================
				vector<C3D_OBJECT*>::iterator	erase(vector<C3D_OBJECT*>::iterator p)
				{
					return	objects.erase(p);
				}

				vector<C3D_OBJECT*>::iterator	getBegin(void)	{ return	objects.begin();	}
				vector<C3D_OBJECT*>::iterator	getEnd(void)	{ return	objects.end();		}

			//==================================================================
			//	access	�O���[�v
			//==================================================================
				vector<C3D_OBJECT*>*	getObjects(void)		{ return	&objects;			}

			//==================================================================
			//	�\�[�g & �\�[�g�`��
			//==================================================================
				void	sortByCameraPosition(C3D_OBJECT* camera, BOOL inAngle = FALSE);
				void	drawWhitSortByCameraPosition(CONST DEV inDev, C3D_OBJECT* camera, BOOL inAngle = FALSE);

			//==================================================================
			//	access	���O�̏Փ˃I�u�W�F�N�g
			//==================================================================
				C3D_OBJECT*	getPrevHitObject(void)const			{ return	prevHitObject;		}

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
			//	�O���[�v�̃����o�[��S����
			//==================================================================
				void	clear(void);

			//==================================================================
			//	�O���[�v�̃����o�[��S����A�S����
			//==================================================================
				void	releaseClear(void)
				{
					SAFE_VECTOR_DELETE(objects, C3D_OBJECT*);
					clear();
				}
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------