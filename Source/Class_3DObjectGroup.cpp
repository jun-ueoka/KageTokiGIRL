//***********************************************************************************
//!	�t�@�C����		Class_3DObjectGroup.cpp
//
//	�@�\			�O�����̕��̂������O���[�v�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2007/12/08	<�V�K>
//					2008/07/09	<�ǉ�>	�S�̓I�ɃR�����g�̋���
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

#include	<algorithm>
#include	<functional>
//#include	"Class_3DObjectGroup.h"			//!< 3D�I�u�W�F�N�g�O���[�v

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::C3D_OBJECT_GROUP
	//
	//	�@�\		�R���X�g���N�^
	//	�X�V		2007/12/08			<�V�K>
	//================================================================================
	C3D_OBJECT_GROUP::C3D_OBJECT_GROUP(void)
	{
	//	������
		prevHitObject	= NULL;		//!< ���O�̏Փ˃I�u�W�F�N�g
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::~C3D_OBJECT_GROUP
	//
	//	�@�\		�f�X�g���N�^
	//	�X�V		2007/12/08			<�V�K>
	//================================================================================
	C3D_OBJECT_GROUP::~C3D_OBJECT_GROUP(void)
	{
	//	�S����
		clear();
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::add
	//
	//	�@�\		�O���[�v�Ƀ����o�[��ǉ�
	//	����		inObject			�����郁���o�[
	//	�X�V		2007/12/08			<�V�K>
	//================================================================================
	void
	C3D_OBJECT_GROUP::add(C3D_OBJECT* inObject)
	{
	//	NULL�Ȃ珈�������Ȃ�
		if(	inObject == NULL)	return;

	//	��Ɨp�C�e���[�^
		vector<C3D_OBJECT*>::iterator	p;

	//	���łɃ����o�[������Ă���Ȃ�A������Ԃ�
		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
			if(	(*p) == inObject)	return;
		}

	//	�����܂ŗ�����A�����o�[�ɉ�����
		objects.push_back(inObject);
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::addG
	//
	//	�@�\		�O���[�v�ɃO���[�v�̃����o�[��ǉ�
	//	����		inGroup				������O���[�v
	//	�X�V		2007/12/08			<�V�K>
	//================================================================================
	void
	C3D_OBJECT_GROUP::addG(C3D_OBJECT_GROUP* inGroup)
	{

	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::draw
	//
	//	�@�\		�O���[�v�̃����o�[��S�`��()
	//	����		inSprite			�X�v���C�g�f�o�C�X
	//	�X�V		2007/12/08			<�V�K>
	//================================================================================
	void
	C3D_OBJECT_GROUP::draw(CONST SPRITE inSprite)
	{
	//	��Ɨp�C�e���[�^
		vector<C3D_OBJECT*>::iterator	p;

	//	�����o�[����T���o���āA�폜����
		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
			(*p)->draw(inSprite);
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::draw
	//
	//	�@�\		�O���[�v�̃����o�[��S�`��
	//	����		inDev				Direct3D�f�o�C�X
	//	�X�V		2007/12/08			<�V�K>
	//================================================================================
	void
	C3D_OBJECT_GROUP::draw(CONST DEV inDev)
	{
	//	��Ɨp�C�e���[�^
		vector<C3D_OBJECT*>::iterator	p;

	//	�����o�[����T���o���āA�폜����
		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
			(*p)->draw(inDev);
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::draw
	//
	//	�@�\		�O���[�v�̃����o�[��������͈͂ŕ`��
	//	����		inDev				Direct3D�f�o�C�X
	//				inViewPoint			���_
	//				inLength			���_����̗L���͈�
	//	�X�V		2008/09/26			<�V�K>
	//================================================================================
	void
	C3D_OBJECT_GROUP::draw(CONST DEV inDev, VEC3* inViewPoint, float inLength)
	{
	//	��Ɨp�C�e���[�^
		vector<C3D_OBJECT*>::iterator	p;

	//	�����o�[����T���o���āA�폜����
		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
			if(	(D3DXVec3Length(&(*inViewPoint - (*p)->getLoc()))) < inLength)
			{
				(*p)->draw(inDev);
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::draw
	//
	//	�@�\		�O���[�v�̃����o�[��Z�e�N�X�`���V�F�[�_�ŕ`��
	//	����		inZTexSha			Z�e�N�X�`���V�F�[�_
	//	�X�V		2009/02/05			<�V�K>
	//================================================================================
	void
	C3D_OBJECT_GROUP::draw(CZTEX_SHA* inZTexSha)
	{
	//	��Ɨp�C�e���[�^
		vector<C3D_OBJECT*>::iterator	p;

		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
			(*p)->draw(inZTexSha);
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::draw
	//
	//	�@�\		�O���[�v�̃����o�[��[�x�o�b�t�@�V�F�[�_�ŕ`��
	//	����		inDepthSha			�[�x�o�b�t�@�V�F�[�_
	//	�X�V		2009/02/05			<�V�K>
	//================================================================================
	void
	C3D_OBJECT_GROUP::draw(CDEPTH_SHA* inDepthSha)
	{
	//	��Ɨp�C�e���[�^
		vector<C3D_OBJECT*>::iterator	p;

		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
			(*p)->draw(inDepthSha);
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::erase
	//
	//	�@�\		�O���[�v���烁���o�[���폜
	//	����		eraceObject			�폜���郁���o�[
	//	�X�V		2007/12/08			<�V�K>
	//================================================================================
	void
	C3D_OBJECT_GROUP::erase(C3D_OBJECT *eraceObject)
	{
	//	NULL�Ȃ珈�������Ȃ�
		if(	eraceObject == NULL)	return;

	//	��Ɨp�C�e���[�^
		vector<C3D_OBJECT*>::iterator	p;

	//	�����o�[����T���o���āA�폜����
		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
			if(	(*p) == eraceObject)
			{
				objects.erase(p);
				return;
			}
		}
	}

	//================================================================================
	//!	�֐���		UDgreater
	//
	//	�@�\		�����̃I�u�W�F�N�g����בւ���
	//	����		inObject1			�I�u�W�F�N�g1
	//				inObject2			�I�u�W�F�N�g2
	//	�߂�l		true				1���2�̕����߂�
	//				false				1���2�̕�������
	//	�X�V		2007/12/08			<�V�K>
	//================================================================================
	bool
	UDgreater(C3D_OBJECT *inObject1, C3D_OBJECT *inObject2)
	{
		float	distance = inObject1->getOffsetFromCamera() - inObject2->getOffsetFromCamera();
		return (distance > 0.0f);
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::sortByCameraPosition
	//
	//	�@�\		�J�����ʒu�����ɁA�I�u�W�F�N�g�����o�[����ёւ�
	//	����		camera				�J�����I�u�W�F�N�g
	//				inAngle				�p�x�J���������C���m�F
	//	�X�V		2007/12/08			<�V�K>
	//================================================================================
	void
	C3D_OBJECT_GROUP::sortByCameraPosition(C3D_OBJECT* camera, BOOL inAngle)
	{
	//	��Ɨp�C�e���[�^
		vector<C3D_OBJECT*>::iterator	p;

	//	�I�u�W�F�N�g�ɃJ�����Ƃ̋�����ݒ肷��
		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
		//	�p�x���C������
			if(	inAngle)	(*p)->angleToObject(camera);

		//	�J�����܂ł̋������v�Z
			(*p)->setOffsetFromCamera(
				D3DXVec3Length(&VEC3((*p)->getLoc() - camera->getLoc())));
		}

	//	�I�u�W�F�N�g�����o�[���\�[�g
		sort(objects.begin(), objects.end(), UDgreater);
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::drawWhitSortByCameraPosition
	//
	//	�@�\		���ёւ��ƕ`��𗼕����s����
	//	����		inDev				Direct3D�f�o�C�X
	//				camera				�J�����I�u�W�F�N�g
	//				inAngle				�p�x�J���������C���m�F
	//	�X�V		2007/12/08			<�V�K>
	//================================================================================
	void
	C3D_OBJECT_GROUP::drawWhitSortByCameraPosition(CONST DEV inDev, C3D_OBJECT *camera, BOOL inAngle)
	{
		sortByCameraPosition(camera, inAngle);		//!< ���ёւ�
		draw(inDev);								//!< �`��
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::intersect�i�S�����Łj
	//
	//	�@�\		�S�����ŃO���[�v�ɑ΂��āA�Փ˔���
	//	����		inObject			�Փ˕�
	//				inLine				�������
	//	�X�V		2008/08/20			<�V�K>
	//================================================================================
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE* inLine)
	{
	//	�Փ˕��̑��݊m�F��������΁A�Փ˂��Ė�������Ԃ��ďI��
		if(	!inObject->getVisible())	return	FALSE;

		float	inDist	= 10000.0f;			//!< �������m�ۂ��Ă���
		BOOL	inHit	= FALSE;			//!< �Փˌ���

	//	���b�V���Ƃ̋��������������Ă���
		inObject->setDist(inDist);

	//	��Ɨp�C�e���[�^
		vector<C3D_OBJECT*>::iterator	pi;

	//	�I�u�W�F�N�g�ɃJ�����Ƃ̋�����ݒ肷��
		for(pi  = objects.begin();
			pi != objects.end();
			pi++)
		{
		//	���ނ̃I�u�W�F�N�g�Ƃ͓����蔻������Ȃ�
			if(	inObject != (C3D_OBJECT*)*pi)
			{
				if(	(*pi)->intersect(inObject, inLine))
				{
				//	�O��̃I�u�W�F�N�g���A�Փˋ������߂���
					if(	inDist	> inObject->getDist())
					{
						inHit				= TRUE;
					//	���O�̏Փ˃I�u�W�F�N�g��o�^
						prevHitObject	= (*pi);
					//	�������Ƃ��Ă���
						inDist				= inObject->getDist();
					}
				}
			}
		}

	//	�Փ˂��Ă�����A�ŒZ���������Ă���
		if(	inHit)	inObject->setDist(inDist);

		return inHit;
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::intersect�i�㉺�A���E�A�O��Łj
	//
	//	�@�\		�S�����ł̏Փ˔���ɂȂ���
	//	�X�V		2008/08/20			<�V�K>
	//================================================================================
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown)
	{
		return	intersect(inObject, &LINE(inUpDown, eLR_NO, eFR_NO));
	}
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight)
	{
		return	intersect(inObject, &LINE(eUD_NO, inLeftRight, eFR_NO));
	}
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear)
	{
		return	intersect(inObject, &LINE(eUD_NO, eLR_NO, inFrontRear));
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::intersect�i�����w��A�S�����Łj
	//
	//	�@�\		�S�����ŃO���[�v�ɑ΂��āA�Փ˔���
	//	����		inObject			�Փ˕�
	//				inLine				�������
	//				inIntersectValue	���肷�鋗��
	//	�X�V		2008/08/20			<�V�K>
	//================================================================================
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE* inLine, float inIntersectValue)
	{
	//	�Փ˕��̑��݊m�F��������΁A�Փ˂��Ė�������Ԃ��ďI��
		if(	!inObject->getVisible())	return	FALSE;

		float	inDist	= 10000.0f;			//!< �������m�ۂ��Ă���
		BOOL	inHit	= FALSE;			//!< �Փˌ���

	//	���b�V���Ƃ̋��������������Ă���
		inObject->setDist(inDist);

	//	��Ɨp�C�e���[�^
		vector<C3D_OBJECT*>::iterator	pi;

	//	�I�u�W�F�N�g�ɃJ�����Ƃ̋�����ݒ肷��
		for(pi  = objects.begin();
			pi != objects.end();
			pi++)
		{
		//	���ނ̃I�u�W�F�N�g�Ƃ͓����蔻������Ȃ�
			if(inObject != (C3D_OBJECT*)*pi)
			{
				if(	(*pi)->intersect(inObject, inLine, inIntersectValue))
				{
				//	�O��̃I�u�W�F�N�g���A�Փˋ������߂���
					if(	inDist	> inObject->getDist())
					{
						inHit				= TRUE;
					//	���O�̏Փ˃I�u�W�F�N�g��o�^
						prevHitObject	= (*pi);
					//	�������Ƃ��Ă���
						inDist				= inObject->getDist();
					}
				}
			}
		}

	//	�Փ˂��Ă�����A�ŒZ���������Ă���
		if(	inHit)	inObject->setDist(inDist);

		return	inHit;
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::intersect�i�����w��A�㉺�A���E�A�O��Łj
	//
	//	�@�\		�����w��̑S�����ł̏Փ˔���ɂȂ���
	//	�X�V		2008/08/20			<�V�K>
	//================================================================================
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown, float inIntersectValue)
	{
		return	intersect(inObject, &LINE(inUpDown, eLR_NO, eFR_NO), inIntersectValue);
	}
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight, float inIntersectValue)
	{
		return	intersect(inObject, &LINE(eUD_NO, inLeftRight, eFR_NO), inIntersectValue);
	}
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear, float inIntersectValue)
	{
		return	intersect(inObject, &LINE(eUD_NO, eLR_NO, inFrontRear), inIntersectValue);
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::intersectPos
	//				�i����J�n�ʒu�w��A�����w��A�S�����Łj
	//
	//	�@�\		�S�����ŃO���[�v�ɑ΂��āA�Փ˔���
	//	����		inObject			�Փ˕�
	//				inLoc				����J�n�ʒu
	//				inLine				�������
	//				inIntersectValue	���肷�鋗��
	//	�X�V		2009/02/12			<�V�K>
	//================================================================================
	BOOL
	C3D_OBJECT_GROUP::intersectPos(C3D_OBJECT*	inObject,
								   VEC3*		inLoc,
								   LINE*		inLine,
								   float		inIntersectValue)
	{
	//	�Փ˕��̑��݊m�F��������΁A�Փ˂��Ė�������Ԃ��ďI��
		if(	!inObject->getVisible())	return	FALSE;

		float	inDist	= 10000.0f;			//!< �������m�ۂ��Ă���
		BOOL	inHit	= FALSE;			//!< �Փˌ���

	//	���b�V���Ƃ̋��������������Ă���
		inObject->setDist(inDist);

	//	��Ɨp�C�e���[�^
		vector<C3D_OBJECT*>::iterator	pi;

	//	�I�u�W�F�N�g�ɃJ�����Ƃ̋�����ݒ肷��
		for(pi  = objects.begin();
			pi != objects.end();
			pi++)
		{
		//	���ނ̃I�u�W�F�N�g�Ƃ͓����蔻������Ȃ�
			if(inObject != (C3D_OBJECT*)*pi)
			{
				if(	(*pi)->intersectPos(inObject, inLoc, inLine, inIntersectValue))
				{
				//	�O��̃I�u�W�F�N�g���A�Փˋ������߂���
					if(	inDist	> inObject->getDist())
					{
						inHit			= TRUE;
					//	���O�̏Փ˃I�u�W�F�N�g��o�^
						prevHitObject	= (*pi);
					//	�������Ƃ��Ă���
						inDist			= inObject->getDist();
					}
				}
			}
		}

	//	�Փ˂��Ă�����A�ŒZ���������Ă���
		if(	inHit)	inObject->setDist(inDist);

		return	inHit;
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::intersectPos
	//				�i����J�n�ʒu�w��A�����w��A�㉺�A���E�A�O��Łj
	//
	//	�@�\		�����w��̑S�����ł̏Փ˔���ɂȂ���
	//	�X�V		2009/02/12			<�V�K>
	//================================================================================
	BOOL
	C3D_OBJECT_GROUP::intersectPos(
		C3D_OBJECT* inObject, VEC3* inLoc, LINE_UPDOWN inUpDown, float inIntersectValue)
	{
		return	intersectPos(
					inObject, inLoc, &LINE(inUpDown, eLR_NO, eFR_NO), inIntersectValue);
	}
	BOOL
	C3D_OBJECT_GROUP::intersectPos(
		C3D_OBJECT* inObject, VEC3* inLoc, LINE_LEFTRIGHT inLeftRight, float inIntersectValue)
	{
		return	intersectPos(
					inObject, inLoc, &LINE(eUD_NO, inLeftRight, eFR_NO), inIntersectValue);
	}
	BOOL
	C3D_OBJECT_GROUP::intersectPos(
		C3D_OBJECT* inObject, VEC3* inLoc, LINE_FRONTREAR inFrontRear, float inIntersectValue)
	{
		return	intersectPos(
					inObject, inLoc, &LINE(eUD_NO, eLR_NO, inFrontRear), inIntersectValue);
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT_GROUP::clear
	//
	//	�@�\		�O���[�v�̃����o�[��S����
	//	�X�V		2009/01/15			<�V�K>
	//================================================================================
	void
	C3D_OBJECT_GROUP::clear(void)
	{
	//	�ʏ�O���[�v������
		objects.clear();
	////	�̈�}�b�v������
	//	VEC_LOOP(GROUP_AREA*, areaMap)
	//	{
	//		(*p)->objects.clear();
	//	}
		prevHitObject = NULL;	
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------