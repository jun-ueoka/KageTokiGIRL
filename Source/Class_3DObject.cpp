//***********************************************************************************
//!	�t�@�C����		Class_3DObject.cpp
//
//	�@�\			���̃v���O�����Ŏg�p����O�����̕��̂Ɋւ������
//					3D�I�u�W�F�N�g�O���[�v������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2007/12/08	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_3DObject.h"

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::initialize
	//
	//	�@�\		��{�I�ȏ��̏�����
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	C3D_OBJECT::initialize(void)
	{
	//	������
		rot.xAxis			= VEC3(1.0f, 0.0f, 0.0f);	//!< ���W�nX�̎���
		rot.yAxis			= VEC3(0.0f, 1.0f, 0.0f);	//!< ���W�nY�̎���
		rot.zAxis			= VEC3(0.0f, 0.0f, 1.0f);	//!< ���W�nZ�̎���
		rot.rpm				= VEC3(1.0f, 1.0f, 1.0f);	//!< ������]��
		rot.angle			= VEC3(0.0f, 0.0f, 0.0f);	//!< �p�x
		loc.line			= LINE();					//!< �i�s����
		loc.prev			= VEC3(0.0f, 0.0f, 0.0f);	//!< �O�̈ʒu
		loc.curr			= VEC3(0.0f, 0.0f, 0.0f);	//!< ���݂̈ʒu
		loc.next			= VEC3(0.0f, 0.0f, 0.0f);	//!< ���̍��W
		bVisible			= TRUE;						//!< �\��
		fScale				= 1.0f;						//!< �W���{��
		fMoveQuantity		= 1.0f;						//!< �ړ���
		fOffsetFromCamera	= 0.0f;						//!< �J�����Ƃ̋���
		fTemporaryRadius	= 1.0f;						//!< �����a
		fDist				= 1000.0f;					//!< �Փ˕��̊Ԋu
		orderMessage		= eOMSG_NOMSG;				//!< ���߃��b�Z�[�W
		thisMessage			= eTMSG_NOMSG;				//!< ���ʃ��b�Z�[�W

		fDiffuseAlpha		= 0.0f;						//!< �f�t���[�Y�A���t�@�ϓ��l

		D3DXMatrixIdentity(&matWorldIV);				//!< �t�s��

		update();										//!< ���[���h�s��X�V
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::finalize
	//
	//	�@�\		��n��
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	C3D_OBJECT::finalize(void)
	{
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::rayFromInversedMatrix
	//
	//	�@�\		���̃��[���h�t�s��̃x�N�g���̃g�����X�t�H�[�������߂�
	//	����		*inFrom			���ݒn
	//				*inDir			����
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	C3D_OBJECT::rayFromInversedMatrix(VEC3 *inFrom, VEC3 *inDir)
	{
		MAT16	invMat;		//!< �t�s��i�[

	//	���݂̃��[���h�s��̋t�s��𐶐�
		D3DXMatrixInverse(&invMat, NULL, &matWorld);

	//	���C�̎��_�̓��[���h�s��̋t�s��Ƃ���y�X�P�[�����O�𔽉f����z
		D3DXVec3TransformCoord(inFrom, inFrom, &invMat);

	//	���C�̕����͉�]�s��̋t�s��Ƃ���y�X�P�[�����O�𔽉f���Ȃ��z
		D3DXMatrixInverse(&invMat, NULL, &rot.matrix3D);

	//	���C�̕����͉�]�s��̋t�s��Ƃ���
		D3DXVec3TransformCoord(inDir, inDir, &invMat);
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::move
	//
	//	�@�\		���݂̑����ŁA�ʒu���X�V
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	C3D_OBJECT::move(void)
	{
	//	���̍��W���󂯂Ƃ�A���݂̍��W�ɓ����
		setLoc(calcNextLoc(fMoveQuantity, &loc.line));
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::moveUpDown
	//
	//	�@�\		�㉺�����A�ړ��l�w��̈ړ�����
	//	����		inUpDown		�㉺����
	//				inMoveQuantity	�ړ��l
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	C3D_OBJECT::moveUpDown(LINE_UPDOWN inUpDown, float inMoveQuantity)
	{
	//	������ݒ�
		LINE	inLine	= LINE(inUpDown, eLR_NO, eFR_NO);
	//	���̍��W���󂯂Ƃ�A���݂̍��W�ɓ����
		setLoc(calcNextLoc(
			(inMoveQuantity >= 0.0f) ? inMoveQuantity : -inMoveQuantity, &inLine));
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::moveLeftRight
	//
	//	�@�\		���E�����A�ړ��l�w��̈ړ�����
	//	����		inLeftRight		���E����
	//				inMoveQuantity	�ړ��l
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	C3D_OBJECT::moveLeftRight(LINE_LEFTRIGHT inLeftRight, float inMoveQuantity)
	{
	//	������ݒ�
		LINE	inLine	= LINE(eUD_NO, inLeftRight, eFR_NO);
	//	���̍��W���󂯂Ƃ�A���݂̍��W�ɓ����
		setLoc(calcNextLoc(
			(inMoveQuantity >= 0.0f) ? inMoveQuantity : -inMoveQuantity, &inLine));
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::moveFrontRear
	//
	//	�@�\		�O������A�ړ��l�w��̈ړ�����
	//	����		inFrontRear		�O�����
	//				inMoveQuantity	�ړ��l
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	void
	C3D_OBJECT::moveFrontRear(LINE_FRONTREAR inFrontRear, float inMoveQuantity)
	{
	//	������ݒ�
		LINE	inLine	= LINE(eUD_NO, eLR_NO, inFrontRear);
	//	���̍��W���󂯂Ƃ�A���݂̍��W�ɓ����
		setLoc(calcNextLoc(
			(inMoveQuantity >= 0.0f) ? inMoveQuantity : -inMoveQuantity, &inLine));
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::calcNextLoc
	//
	//	�@�\		���̈ʒu���󂯎��
	//	�@�\�T�v	�w�肳�ꂽ�ړ��ʂ�i�ގ��̈ʒu���󂯎��
	//	����		inMoveQuantity		�ړ���
	//				inLine				����
	//	�߂�l		�ړ���̈ʒu�̃|�C���^�[
	//	�X�V		2008/04/18			<�V�K>
	//================================================================================
	VEC3*
	C3D_OBJECT::calcNextLoc(float inMoveQuantity, LINE* inLine)
	{
	//	�����������Ă��Ȃ��Ȃ�A���̃C���X�^���X�̕������g��
		if(	!inLine)	inLine = &loc.line;

		VEC3	vt(0.0f, 0.0f, 0.0f);			//!< ����

	//	���C�̕������󂯎��
		vt		= calcRay(inLine);

	//	�ړ��ʂ��v�Z
		vt.x   *= inMoveQuantity;
		vt.y   *= inMoveQuantity;
		vt.z   *= inMoveQuantity;

	//	���̈ړ�����X�V
		loc.next	= loc.curr + vt;

	//	���̍��W��Ԃ�
		return	&loc.next;
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::calcRay
	//
	//	�@�\		���C�̕������󂯎��
	//	�@�\�T�v	���C�̕������󂯎��
	//	����		*inLine			����
	//	�߂�l		���C�̈ʒu
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	VEC3
	C3D_OBJECT::calcRay(LINE* inLine)
	{
	//	�����������Ă��Ȃ��Ȃ�A���̃C���X�^���X�̕������g��
		if(	!inLine)	inLine = &loc.line;

		VEC3	vt(0.0f, 0.0f, 0.0f);			//!< ���ʂ̃��C
		VEC3	vw(	(float)inLine->lr,
					(float)inLine->ud,
					(float)inLine->fr);			//!< �����𒸓_��	

	//	���C�̕��������o
		D3DXVec3TransformCoord(&vt, &vw, getMatrixRotation());

	//	���C�̕�����Ԃ�
		return	vt;
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::getNextLoc
	//
	//	�@�\		���̈ʒu���󂯎��
	//	�@�\�T�v	�w�肳�ꂽ�ړ��ʂ�i�ގ��̈ʒu���󂯎��
	//	����		inMoveQuantity		�ړ���
	//				inLine				����
	//	�߂�l		�ړ���̈ʒu
	//	�X�V		2008/04/18			<�V�K>
	//================================================================================
	VEC3
	C3D_OBJECT::getNextLoc(float inMoveQuantity, LINE* inLine)
	{
	//	�����������Ă��Ȃ��Ȃ�A���̃C���X�^���X�̕������g��
		if(	!inLine)	inLine = &loc.line;

		VEC3	vt(0.0f, 0.0f, 0.0f);			//!< ����

	//	���C�̕������󂯎��
		vt		= calcRay(inLine);

	//	�ړ��ʂ��v�Z
		vt.x   *= inMoveQuantity;
		vt.y   *= inMoveQuantity;
		vt.z   *= inMoveQuantity;

	//	���̍��W��Ԃ�
		return	loc.curr + vt;
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::updateScale
	//
	//	�@�\		�X�P�[�����O�s����X�V����
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	C3D_OBJECT::updateScale(void)
	{
	//	�X�P�[�����O�s���������
		D3DXMatrixIdentity(&matScale);
	//	�X�P�[�����O�s����쐬
		D3DXMatrixScaling(&matScale, fScale, fScale, fScale);
	//	���[���h�s��̍X�V
		updateWorld();
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::updateRotat
	//
	//	�@�\		��]�s����X�V����
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	C3D_OBJECT::updateRotat(void)
	{
	//	��]�s���������
		D3DXMatrixIdentity(&rot.matrix3D);
		D3DXMatrixIdentity(&rot.matrixX);
		D3DXMatrixIdentity(&rot.matrixY);
		D3DXMatrixIdentity(&rot.matrixZ);
	//	0 - 359�x�͈̔͂Ɏ��߂�
		NORMALIZE_DEGREE(rot.angle.x);
		NORMALIZE_DEGREE(rot.angle.y);
		NORMALIZE_DEGREE(rot.angle.z);
	//	�C�ӂ̎��ɉ����āA��]�s����쐬
		D3DXMatrixRotationAxis(&rot.matrixX, &rot.xAxis, D3DXToRadian(rot.angle.x));
		D3DXMatrixRotationAxis(&rot.matrixY, &rot.yAxis, D3DXToRadian(rot.angle.y));
		D3DXMatrixRotationAxis(&rot.matrixZ, &rot.zAxis, D3DXToRadian(rot.angle.z));
	//	3D��]�s����쐬(���Ԃɒ���)
		rot.matrix3D = rot.matrixX * rot.matrixY * rot.matrixZ;
	//	���[���h�s��̍X�V
		updateWorld();
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::updateLoc
	//
	//	�@�\		���W�s����X�V����
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	C3D_OBJECT::updateLoc(void)
	{
	//	�P�ʍs��ŏ��������s��
		D3DXMatrixIdentity(&loc.matrix);
	//	���W�̍s����쐬
		D3DXMatrixTranslation(&loc.matrix, loc.curr.x, loc.curr.y, loc.curr.z);
	//	���[���h�s��̍X�V
		updateWorld();
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::updateWorld
	//
	//	�@�\		���[���h�s����X�V����
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	C3D_OBJECT::updateWorld(void)
	{
	//	���[���h�s����쐬(���Ԃɒ���)
		matWorld = matScale * rot.matrix3D * loc.matrix;
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::update
	//
	//	�@�\		�S�̓I�ɍs����X�V����
	//	�X�V		2008/04/18		<�V�K>
	//================================================================================
	void
	C3D_OBJECT::update(void)
	{
	//	�X�P�[�����O�s���������
		D3DXMatrixIdentity(&matScale);

	//	�X�P�[�����O�s����쐬
		D3DXMatrixScaling(&matScale, fScale, fScale, fScale);

	//	��]�s���������
		D3DXMatrixIdentity(&rot.matrix3D);
		D3DXMatrixIdentity(&rot.matrixX);
		D3DXMatrixIdentity(&rot.matrixY);
		D3DXMatrixIdentity(&rot.matrixZ);

	//	0 - 359�x�͈̔͂Ɏ��߂�
		NORMALIZE_DEGREE(rot.angle.x);
		NORMALIZE_DEGREE(rot.angle.y);
		NORMALIZE_DEGREE(rot.angle.z);

	//	�C�ӂ̎��ɉ����āA��]�s����쐬
		D3DXMatrixRotationAxis(&rot.matrixX, &rot.xAxis, D3DXToRadian(rot.angle.x));
		D3DXMatrixRotationAxis(&rot.matrixY, &rot.yAxis, D3DXToRadian(rot.angle.y));
		D3DXMatrixRotationAxis(&rot.matrixZ, &rot.zAxis, D3DXToRadian(rot.angle.z));

	//	3D��]�s����쐬(���Ԃɒ���)
		rot.matrix3D = rot.matrixX * rot.matrixY * rot.matrixZ;

	//	�P�ʍs��ŏ��������s��
		D3DXMatrixIdentity(&loc.matrix);

	//	���W�̍s����쐬
		D3DXMatrixTranslation(&loc.matrix, loc.curr.x, loc.curr.y, loc.curr.z);

	//	���[���h�s��̍X�V
		updateWorld();
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::calcDiffuseAlpha
	//
	//	�@�\		�f�t���[�Y�A���t�@�ϓ��l�̌v�Z
	//	����		inAlpha			�f�t���[�Y�A���t�@�ϓ��l�̌v�Z�l
	//	�X�V		2008/10/27		<�V�K>
	//================================================================================
	void
	C3D_OBJECT::calcDiffuseAlpha(float inAlpha)
	{
	//	�v�Z
		fDiffuseAlpha	+= inAlpha;
	//	�l�␳
		if		(fDiffuseAlpha < -1.0f)	fDiffuseAlpha = -1.0f;
		else if	(fDiffuseAlpha >  1.0f)	fDiffuseAlpha	=  1.0f;
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::intersectRadius
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂��A���a�Ō��o����
	//	����		inObject		�ՓˑΏە�
	//	�߂�l		TRUE			�Փ˂��Ă���
	//				FALSE			�Փ˂��Ă��Ȃ�
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	BOOL
	C3D_OBJECT::intersectRadius(C3D_OBJECT *inObject)
	{
	//	�ǂ��炩����\���Ȃ�A���肹���ɏՓ˂��Ă��Ȃ�����Ԃ�
		if(	(!inObject->getVisible()) || (!getVisible()))
		{	
			return FALSE;
		}
	//	���g�ƃI�u�W�F�N�g�Ƃ̍��W��̋��������Z
		VEC3	vDistance = getWorldCenter() - inObject->getWorldCenter();

	//	���g�ƃI�u�W�F�N�g�Ƃ̋������v�Z���A�ێ�
		float	fDistance	= D3DXVec3Length(&vDistance);

	//	���g�ƃI�u�W�F�N�g�Ƃ̋������A�ӂ��̃��[���h���W�n�̔��a���A������
		if(	fDistance > (getWorldRadius() + inObject->getWorldRadius()))
		{
		//	�Փ˂��Ă��Ȃ�����Ԃ�
			return	FALSE;
		}
	//	��L�ȊO
		else
		{
		//	�Փ˂����I�u�W�F�N�g�Ƃ̊Ԋu��ێ�
			inObject->setDist(
				(getWorldRadius() + inObject->getWorldRadius()) - fDistance);
		//	�Փ˂��Ă��鎖���
			return	TRUE;
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::intersectRadius
	//
	//	�@�\		���g�ƃI�u�W�F�N�g�Ƃ̏Փ˂��A���a�Ō��o����(�w�蔼�a)
	//	����		inObject		�ՓˑΏە�
	//				inRadius		���a
	//	�߂�l		TRUE			�Փ˂��Ă���
	//				FALSE			�Փ˂��Ă��Ȃ�
	//	�X�V		2009/01/19		<�V�K>
	//================================================================================
	BOOL
	C3D_OBJECT::intersectRadius(C3D_OBJECT *inObject, float inRadius)
	{
	//	���g�ƃI�u�W�F�N�g�Ƃ̍��W��̋��������Z
		VEC3	vDistance = getWorldCenter() - inObject->getWorldCenter();

	//	���g�ƃI�u�W�F�N�g�Ƃ̋������v�Z���A�ێ�
		float	fDistance = D3DXVec3Length(&vDistance);

	//	���g�ƃI�u�W�F�N�g�Ƃ̋������A�ӂ��̃��[���h���W�n�̔��a���A������
		if(	fDistance > (getWorldRadius() + inRadius))
		{
		//	�Փ˂��Ă��Ȃ�����Ԃ�
			return	FALSE;
		}
	//	��L�ȊO
		else
		{
		//	�Փ˂����I�u�W�F�N�g�Ƃ̊Ԋu��ێ�
			inObject->setDist((getWorldRadius() + inRadius) - fDistance);
		//	�Փ˂��Ă��鎖���
			return	TRUE;
		}
	}


	//================================================================================
	//!	���\�b�h��	C3D_OBJECT::angleToObject
	//
	//	�@�\		�I�u�W�F�N�g�̕���������
	//	����		inObject		�Ώە�
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	void
	C3D_OBJECT::angleToObject(C3D_OBJECT *inObject)
	{
		if(	inObject)
		{
			VEC3	vOrigin					= VEC3(0.0f,0.0f,-1.0f);
			float	fRad					= 0.0f;
			VEC3	vInverseCameraDirection	= -(getLoc() - inObject->getLoc());

		//	X-Z�ʂł̉�]�Ŏ�������̂ɁAY���̊p�x�͕K�v�Ȃ�
			vInverseCameraDirection.y		= 0.0f;

		//	�I�u�W�F�N�g�ɑ΂��Ă̕�������A���g�̕��������肷��
			if(	vInverseCameraDirection.x < 0.0f)
			{
				fRad =  (float)acos(D3DXVec3Dot(&vInverseCameraDirection, &vOrigin)	/
						(D3DXVec3Length(&vOrigin) * D3DXVec3Length(&vInverseCameraDirection)));
			}
			else
			{
				fRad = -(float)acos(D3DXVec3Dot(&vInverseCameraDirection, &vOrigin)	/
						(D3DXVec3Length(&vOrigin) * D3DXVec3Length(&vInverseCameraDirection)));
			}
			setAngleY(D3DXToDegree(fRad));
		}
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------