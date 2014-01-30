//***********************************************************************************
//!	�t�@�C����		Class_3DEffect.cpp
//
//	�@�\			3D�Q�[����p�̃G�t�F�N�g�N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/09/11	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_3DEffect.h"

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	C3D_EFFECT::�R���X�g���N�^
	//
	//	�@�\�T�v	X�t�@�C�������[�h���ď�����
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			�G�t�F�N�g�t�@�C���̖���
	//				inLoc			�J�n�ʒu
	//				inResource		�G�t�F�N�g�t�H���_��
	//				inTexture		�G�t�F�N�g�e�N�X�`���t�H���_��
	//	�X�V		2008/05/30		<�V�K>
	//================================================================================
	C3D_EFFECT::C3D_EFFECT(CONST DEV   inDev,
						   CONST LPSTR inName,
						   VEC3		   inLoc,
						   CONST LPSTR inResource,
						   CONST LPSTR inTexture)	:
	CEFFECT_BASE(inDev, inName, inResource, inTexture),
	spMove(NULL)
	{
	//	�X�[�p�[�N���X�̏�����
		C3D_OBJECT::initialize();
	//	�����̏�����
		iCntFrame	= 0;
		iCntTex		= 0;
		iCntSize	= 0;
		iCntRotato	= 0;
		iCntMove	= 0;
		iCntColor	= 0;
		spMove		= NULL;

		bMotion		= TRUE;
		keepAngle	= VEC3(0.0f,0.0f,0.0f);

	//	�ʒu�̃Z�b�g
		setLoc(&inLoc);
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT::�R���X�g���N�^
	//
	//	�@�\		�󏉊���
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	C3D_EFFECT::C3D_EFFECT(void)	:
	CEFFECT_BASE(),
	spMove(NULL)
	{
	//	�X�[�p�[�N���X�̏�����
		C3D_OBJECT::initialize();
	//	�����̏�����
		iCntFrame	= 0;
		iCntTex		= 0;
		iCntSize	= 0;
		iCntRotato	= 0;
		iCntMove	= 0;
		iCntColor	= 0;
		spMove		= NULL;

		bMotion		= TRUE;
		keepAngle	= VEC3(0.0f,0.0f,0.0f);
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT::�R���X�g���N�^
	//
	//	�@�\�T�v	�R�s�[
	//	����		inCopy			�R�s�[��
	//				inLoc			�J�n�ʒu
	//				inAngle			�J�n�p�x
	//	�X�V		2008/05/30		<�V�K>
	//================================================================================
	C3D_EFFECT::C3D_EFFECT(C3D_EFFECT* inCopy, VEC3 *inLoc, VEC3 *inAngle)	:
	CEFFECT_BASE(inCopy),
	spMove(NULL)
	{
	//	�X�[�p�[�N���X�̏�����
		C3D_OBJECT::initialize();
	//	�����̏�����
		iCntFrame	= 0;
		iCntTex		= 0;
		iCntSize	= 0;
		iCntRotato	= 0;
		iCntMove	= 0;
		iCntColor	= 0;
		spMove		= NULL;

		bMotion		= TRUE;
		keepAngle	= *inAngle;

	//	�ʒu�̃Z�b�g
		setLoc(inLoc);
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT::�f�X�g���N�^
	//
	//	�@�\�T�v	�S�Ă̎�����������A�I������B
	//				�������A���̃I�u�W�F�N�g�ł��g���Ă����ꍇ�͎c���B
	//	�X�V		2008/05/30		<�V�K>
	//================================================================================
	C3D_EFFECT::~C3D_EFFECT()
	{
	//	�X�[�p�[�N���X�̊J��
		C3D_OBJECT::finalize();
	//	�X�v���C���̉��
		SAFE_DELETE(spMove);
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT::createSpline
	//
	//	�@�\		�X�v���C������
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	void
	C3D_EFFECT::createSpline(void)
	{
	//	����
		if(	!taskBreak((*eiMove)->iFrame))
		{
		//	���݊p�x���ێ�
			VEC3	inAngle	= getAngle();

		//	��������Ɛ���
			SAFE_DELETE(spMove);
			spMove	= new CSPLINE();

		//	���Ԓn�_�A�I���n�_�̊m��
			VEC3	vMiddle	= (*eiMove)->middle;
			VEC3	vGoal	= (*eiMove)->goal;

		//	���������ێ��p�x���Z�b�g
			setAngle(&keepAngle);
			D3DXVec3TransformCoord(&vMiddle, &vMiddle, getWorldMatrix());
			D3DXVec3TransformCoord(&vGoal  , &vGoal,   getWorldMatrix());
			spMove->setUp(
				&getLoc(), &vMiddle, &vGoal,
				(*eiMove)->iFrame);

		//	�p�x��߂�
			setAngle(&inAngle);
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT::motionEffect
	//
	//	�@�\		�G�t�F�N�g���s
	//	�@�\�T�v	�G�t�F�N�g�̏������s��
	//	�߂�l		TRUE			���[�V�������s
	//				FALSE			���[�V�����I���i�O��������߁j
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	BOOL
	C3D_EFFECT::motionEffect(void)
	{
	//	�I���`�F�b�N
		BOOL	endChecker	= FALSE;

	//	�G�t�F�N�g�e�N�X�`���̏���
		if(	!taskBreak((*eiTex)->iFrame))
		{
			endChecker		= TRUE;
			iCntTex++;
		//	�t���[�������B������A�C�e���[�^��i�߂�
			if(	(*eiTex)->iFrame	 <= iCntTex)
			{
				iCntTex = 0;
				eiTex++;
			//	�ꉞ�C�e���[�^���`�F�b�N���A�s��������Ȃ�A�O�����
				if(	ed_ptr->vTex.end()	== eiTex)		return	FALSE;
			//	�C�e���[�^��߂�������
				if(	taskReturn((*eiTex)->iFrame))
				{
					eiTex += (*eiTex)->iFrame;
				}
			}
		}

	//	�G�t�F�N�g�T�C�Y�̏���
		if(	!taskBreak((*eiSize)->iFrame))
		{
			endChecker		= TRUE;
			iCntSize++;
		//	�T�C�Y�ω�
			calcShowSize((*eiSize)->size);
		//	�t���[�������B������A�C�e���[�^��i�߂�
			if(	(*eiSize)->iFrame	 <= iCntSize)
			{
				iCntSize = 0;
				eiSize++;
			//	�ꉞ�C�e���[�^���`�F�b�N���A�s��������Ȃ�A�O�����
				if(	ed_ptr->vSize.end()	== eiSize)	return	FALSE;
			//	�C�e���[�^��߂�������
				if(	taskReturn((*eiSize)->iFrame))
				{
					eiSize += (*eiSize)->iFrame;
				}
			}
		}

	//	�G�t�F�N�g��]�̏���
		if(	!taskBreak((*eiRotato)->iFrame))
		{
			endChecker		= TRUE;
			iCntRotato++;
		//	��]�p�x�ω�
			calcAngle(&(*eiRotato)->rotato);
		//	�t���[�������B������A�C�e���[�^��i�߂�
			if(	(*eiRotato)->iFrame	 <= iCntRotato)
			{
				iCntRotato = 0;
				eiRotato++;
			//	�ꉞ�C�e���[�^���`�F�b�N���A�s��������Ȃ�A�O�����
				if(	ed_ptr->vRotato.end()	== eiRotato)	return	FALSE;
			//	�C�e���[�^��߂�������
				if(	taskReturn((*eiRotato)->iFrame))
				{
					eiRotato += (*eiRotato)->iFrame;
				}
			}
		}

	//	�G�t�F�N�g�ړ��̏���
		if(	!taskBreak((*eiMove)->iFrame))
		{
			endChecker		 = TRUE;
		//	�X�v���C������A���݂̈ʒu���擾
			if(	!spMove)	createSpline();
			setLoc(spMove->getPos(iCntMove));
			iCntMove++;
		//	�t���[�������B������A�C�e���[�^��i�߂�
			if(	(*eiMove)->iFrame	 <= iCntMove)
			{
				iCntMove = 0;
				eiMove++;
			//	�ꉞ�C�e���[�^���`�F�b�N���A�s��������Ȃ�A�O�����
				if(	ed_ptr->vMove.end()	== eiMove)	return	FALSE;
			//	�C�e���[�^��߂�������
				if(	taskReturn((*eiMove)->iFrame))
				{
					eiMove += (*eiMove)->iFrame;
				}
			//	�X�v���C���𐶐�
				createSpline();
			}
		//	�t���[�������B���ĂȂ��Ȃ�A�ʒu���X�V
		}

	//	�G�t�F�N�g�J���[�̏���
		if(	!taskBreak((*eiColor)->iFrame))
		{
			endChecker		= TRUE;
			iCntColor++;
		//	�J���[�ω�
			calcColor(
				(*eiColor)->red,
				(*eiColor)->green,
				(*eiColor)->blue,
				(*eiColor)->alpha);
		//	�t���[�������B������A�C�e���[�^��i�߂�
			if(	(*eiColor)->iFrame <= iCntColor)
			{
				iCntColor = 0;
				eiColor++;
			//	�ꉞ�C�e���[�^���`�F�b�N���A�s��������Ȃ�A�O�����
				if(	ed_ptr->vColor.end()	== eiColor)	return	FALSE;
			//	�C�e���[�^��߂�������
				if(	taskReturn((*eiColor)->iFrame))
				{
					eiColor += (*eiColor)->iFrame;
				}
			}
		}

	//	�I�����Ȃ��Ȃ�A�J�E���^��i�߂�
		if(	endChecker)
		{
		//	�I������l�Ȃ�A�������Ȃ�
			if(	!taskBreak(ed_ptr->frame))
			{
				iCntFrame++;
			//	�ő�t���[���܂œ��B������A�O���������
				if(	iCntFrame >= ed_ptr->frame)
				{
					endChecker	= FALSE;
				}
			}
		}
		return	endChecker;
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT::draw
	//
	//	�@�\		�`��(3D)
	//	�@�\�T�v	�G�t�F�N�g�̕`����s��
	//	����		inDev			Direct3D�f�o�C�X
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2008/05/30		<�V�K>
	//================================================================================
	BOOL
	C3D_EFFECT::draw(CONST DEV inDev)
	{
	//	�\���m�F���āA�����̎�
		if(	!getVisible())
		{
		//	�O���������
			setOrderMessage(eOMSG_DELETE);
		//	�`�掸�s��Ԃ�
			return	FALSE;
		}

	//	�X�v���C������
		if(	 (bMotion)	&&
			!(motionEffect()))
		{
		//	�\���𖳌��ɂ��Ă���
			setVisible(FALSE);
		//	�O���������
			setOrderMessage(eOMSG_DELETE);
		//	�`�掸�s��Ԃ�
			return	FALSE;
		}

	//	���[���h�ϊ�
		update();

	//	�`��
		return	drawEffect(inDev, getWorldMatrix());
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT::draw
	//
	//	�@�\		�`��(2D)
	//	�@�\�T�v	�G�t�F�N�g�̕`����s��
	//	����		inSprite		�X�v���C�g�f�o�C�X
	//	�߂�l		TRUE			�`�搬��
	//				FALSE			�`�掸�s
	//	�X�V		2008/09/25		<�V�K>
	//================================================================================
	BOOL
	C3D_EFFECT::draw(CONST SPRITE inSprite)
	{
	//	�\���m�F���āA�����̎�
		if(	!getVisible())
		{
		//	�O���������
			setOrderMessage(eOMSG_DELETE);
		//	�`�掸�s��Ԃ�
			return	FALSE;
		}

	//	�X�v���C������
		if(	 (bMotion)	&&
			!(motionEffect()))
		{
		//	�\���𖳌��ɂ��Ă���
			setVisible(FALSE);
		//	�O���������
			setOrderMessage(eOMSG_DELETE);
		//	�`�掸�s��Ԃ�
			return	FALSE;
		}

	//	�`��
		return	drawEffect(
					inSprite,
					getLoc(),
					D3DXToRadian(getAngleZ()),
					getScale());
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------