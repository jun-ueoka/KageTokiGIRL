//***********************************************************************************
//!	�t�@�C����		Class_Texture.cpp
//
//	�@�\			�e�N�X�`���I�u�W�F�N�g�Ɋւ������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/08/27	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_Texture.h"

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CTEXTURE::�R���X�g���N�^
	//
	//	�@�\		�e�N�X�`�������[�h���ď�����
	//	����		inDev		Direct3D�f�o�C�X
	//				inName		X�t�@�C���̖���
	//				inSize		�T�C�Y
	//				inColor		�����������F
	//				inResource	�e�N�X�`���t�H���_��
	//	�X�V		2008/08/28	<�V�K>
	//================================================================================
	CTEXTURE::CTEXTURE(CONST DEV	inDev,
					   CONST LPSTR	inName,
					   VEC2			inSize,
					   DWORD		inColor,
					   CONST LPSTR	inResource)		:
	CTEXTURE_BASE(inDev, inName, inSize, inColor, inResource)
	{
	//	�X�[�p�[�N���X�̏�����
		C3D_OBJECT::initialize();
	//	�����e�N�X�`���̓ǂݍ��ݎ��s���Ă�����A���݊m�F��OFF�ɂ���
		if(	!bTextureLoaded)	{ setVisible(FALSE); return; }
	//	�`�挳�A�`���̏�����
		setFromRect(
			0, 0, texd_ptr->texInfo.Width, texd_ptr->texInfo.Height);
		setToRect(inSize);
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE::�R���X�g���N�^
	//
	//	�@�\		�󏉊���
	//	�X�V		2008/08/28	<�V�K>
	//================================================================================
	CTEXTURE::CTEXTURE(void)	:
	CTEXTURE_BASE()
	{
	//	�X�[�p�[�N���X�̏�����
		C3D_OBJECT::initialize();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE::�R���X�g���N�^
	//
	//	�@�\		�R�s�[
	//	����		inCopy		�R�s�[��
	//	�X�V		2008/08/28	<�V�K>
	//================================================================================
	CTEXTURE::CTEXTURE(CTEXTURE* inCopy)	:
	CTEXTURE_BASE(inCopy)
	{
	//	�X�[�p�[�N���X�̏�����
		C3D_OBJECT::initialize();
	//	�����e�N�X�`���̓ǂݍ��ݎ��s���Ă�����A���݊m�F��OFF�ɂ���
		if(	!bTextureLoaded)	{ setVisible(FALSE); return; }
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE::�f�X�g���N�^
	//
	//	�X�V		2008/05/03	<�V�K>
	//================================================================================
	CTEXTURE::~CTEXTURE(void)
	{
	//	�X�[�p�[�N���X�̊J��
		C3D_OBJECT::finalize();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE::draw
	//
	//	�@�\		�`��(3D�p)
	//	����		inDev		Direct3D�f�o�C�X
	//	�X�V		2008/05/03	<�V�K>
	//================================================================================
	BOOL
	CTEXTURE::draw(CONST DEV inDev)
	{
	//	���݂��Ȃ���ԂȂ�A�`�悵�Ȃ�
		if(	!getVisible())	return FALSE;

	/*	if(	getDiffuseAlpha() < 0.0f)
		{
			setColorAlpha(255 - int(255.0f * (-getDiffuseAlpha())));
		}*/

		return	drawTexture(inDev, getWorldMatrix());
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE::draw
	//
	//	�@�\		�`��(2D�p)
	//	����		inSprite	�X�v���C�g�f�o�C�X
	//	�X�V		2008/05/03	<�V�K>
	//================================================================================
	BOOL
	CTEXTURE::draw(CONST SPRITE inSprite)
	{
	//	���݂��Ȃ���ԂȂ�A�`�悵�Ȃ�
		if(	!getVisible())	return FALSE;

	//	�`���X�V
		setToRect(getShowSize());

		return	drawTexture(
					inSprite,
					rcFrom,
					rcTo,
					getLocZ(),
					NULL,
					D3DXToRadian(getAngleZ()),
					getColor());
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------