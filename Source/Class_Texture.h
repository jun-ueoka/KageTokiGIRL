//***********************************************************************************
//!	�t�@�C����		Class_Texture.h
//
//	�@�\			�e�N�X�`���I�u�W�F�N�g�Ɋւ����`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/08/27	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_3DObject.h"				//!< 3D�I�u�W�F�N�g
#include	"Class_TextureBase.h"			//!< �e�N�X�`���x�[�X

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	���O��Ԃ̉��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	using	namespace	TB;	//!< ���b�V���x�[�X

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CTEXTURE
	//
	//	�@�\			�e�N�X�`���I�u�W�F�N�g
	//	�X�V			2008/08/27	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CTEXTURE : public C3D_OBJECT, public CTEXTURE_BASE
	{
	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	�X�v���C�g�֘A�̑���
			//==================================================================
				RECT	rcFrom;						//!< �`���
				RECT	rcTo;						//!< �`�挳

	public:			//-------------------- public --------------------//

				CTEXTURE(CONST DEV   inDev,
						 CONST LPSTR inName,
						 VEC2		 inSize,
						 DWORD		 inColor	= CLEAR_COLOR,
						 CONST LPSTR inResource = PATH_TEXTURE);	//!< �R���X�g���N�^
				CTEXTURE(void);										//!< �R���X�g���N�^(�󏉊���)
				CTEXTURE(CTEXTURE* inCopy);							//!< �R�s�[
		virtual	~CTEXTURE(void);									//!< �f�X�g���N�^

			//==================================================================
			//	access	�X�v���C�g�֘A
			//==================================================================
				RECT	getFromRect(void)const		{ return	rcFrom;		}
				RECT	getToRect(void)const		{ return	rcTo;		}
				void	setFromRect(RECT inFrom)	{ rcFrom	= inFrom;	}
				void	setFromRect(int left, int top, int right, int bottom)
				{
					SetRect(&rcFrom, left, top, right, bottom);
				}
				void	setToRect(RECT inTo)		{ rcTo		= inTo;		}
				void	setToRect(VEC2 inSize)
				{
					setShowSize(&inSize);
					SetRect(&rcTo,	int(getLocX() - (inSize.x / 2.0f)),
									int(getLocY() - (inSize.y / 2.0f)),
									int(getLocX() + (inSize.x / 2.0f)),
									int(getLocY() + (inSize.y / 2.0f)));
				}

			//==================================================================
			//	�J���[�̃Z�b�g(�J���[�̐ݒ肪�o����T�u�N���X�ŃI�[�o�[���C�h)
			//==================================================================
		virtual	void	setColor(D3DCOLOR inColor)	{ color.color	= inColor;		}

			//==================================================================
			//	�R�s�[����(�T�u�N���X�ŃI�[�o�[���C�h)
			//==================================================================
		virtual	void	createCopy(C3D_OBJECT** inCopy, VEC3* inLoc)
				{
					*inCopy	= new CTEXTURE(this);
					(*inCopy)->setLoc(inLoc);
				}

			//==================================================================
			//	�`��
			//==================================================================
		virtual	BOOL	draw(CONST DEV		inDev);
		virtual	BOOL	draw(CONST SPRITE	inSprite);
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------