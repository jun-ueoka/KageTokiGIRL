//***********************************************************************************
//!	�t�@�C����		Class_TextureBase.h
//
//	�@�\			�e�N�X�`���x�[�X�Ɋւ����`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/08/30	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"DirectX.h"				//!< DirectX�g�p

//-----------------------------------------------------------------------
//!	�e�N�X�`���x�[�X�̖��O���
//-----------------------------------------------------------------------
namespace TB
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�e�N�X�`�����\�[�X�t�@�C���ւ̃p�X
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_TEXTURE		"..//Resources//Texture//%s.dds"
	#define		CLEAR_COLOR			D3DCOLOR_RGBA(255,255,255,255)

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	���_�̈ʒu
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum VERTEX_POSITION
	{
		eVERP_LEFT_UP,   eVERP_UP,     eVERP_RIGHT_UP,			//!< ������
		eVERP_LEFT,      eVERP_CENTER, eVERP_RIGHT,				//!< ������
		eVERP_LEFT_DOWN, eVERP_DOWN,   eVERP_RIGHT_DOWN,		//!< ������
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�e�N�X�`�����
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct TEXTURE_DATA
	{
		int			cnt;			//!< �g�p��
		string		name;			//!< �e�N�X�`����
		string		rPath;			//!< ���\�[�X�p�X
		string		tPath;			//!< �e�N�X�`���p�X
		TEX			texture;		//!< �e�N�X�`��
		TEXINFO		texInfo;		//!< �e�N�X�`���ڍ�
		DWORD		color;			//!< �J���[���	
		DEV			device;			//!< �f�o�C�X�̃|�C���^

	//	���ԉ����̏�����
		TEXTURE_DATA(void)
		{
			cnt			= 0;
			name		= "";
			rPath		= "";
			tPath		= "";
			texture		= NULL;
			device		= NULL;
			ZeroMemory(&texInfo, sizeof(texInfo));
		}

	//	���������
		~TEXTURE_DATA(void)
		{
			releaseData();
		}

	//	���
		void releaseData(void)
		{
		//	�e�N�X�`�����
			SAFE_RELEASE(texture);
		}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CTEXTURE_BASE
	//
	//	�@�\			�e�N�X�`���x�[�X
	//	�X�V			2008/08/30	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CTEXTURE_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	���_��
			//==================================================================
		#define	NUM_TEXTURE_VERTEX		4

			//==================================================================
			//	�e�N�X�`���̐���
			//==================================================================
		static	BOOL			createTextureInData(CONST DEV		inDev,
													TEXTURE_DATA*	inTexData,
													CONST LPSTR		inName,
													DWORD			inColor,
													CONST LPSTR		inTexture);
		static	TEXTURE_DATA*	createTexture(CONST DEV   inDev,
											  CONST LPSTR inName,
											  DWORD		  inColor,
											  CONST LPSTR inResource);

			//==================================================================
			//	�e�N�X�`���}�b�v�i�C���X�^���X�ŋ��L�������j
			//==================================================================
		static	map<string, TEXTURE_DATA*>*		mTextures;

			//==================================================================
			//	�������A�X�V
			//==================================================================
				void	initializeVertex(CONST DEV inDev);			//!< ���_������
				void	updateCorner(void);							//!< �R�[�i�[�̍X�V
				void	updateUVCoord(void);						//!< UV�̍X�V
				void	updateColor(void);							//!< �J���[�̍X�V

			//==================================================================
			//	�����̏�������
			//==================================================================
				void	startTextureInitialize(void);

			//==================================================================
			//	�e�N�X�`���̏������A���
			//==================================================================
				void	initializeTexture(	CONST DEV   inDev,
											CONST LPSTR inName,
											VEC2		inSize,
											DWORD		inColor		= CLEAR_COLOR,
											CONST LPSTR inResource	= PATH_TEXTURE);
				void	finishalizeTexture(void);

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	�e�N�X�`�����|�C���^�[
			//==================================================================
				TEXTURE_DATA*	texd_ptr;

			//==================================================================
			//	�e�N�X�`���̃��[�h�m�F
			//==================================================================
				BOOL			bTextureLoaded;

			//==================================================================
			//	�e�N�X�`���Ɋւ��鑮��
			//==================================================================
				VTEX9			vb;						//!< �o�[�e�b�N�X�o�b�t�@
				VEC2			showSize;				//!< �\���T�C�Y
				VEC2			texSize;				//!< �e�N�X�`���T�C�Y
				VEC3			corner[4];				//!< �R�[�i�[�ʒu
				VEC2			uCoord;					//!< UV���W(�E��)
				VEC2			vCoord;					//!< UV���W(����)
				MY_COLOR_RGBA	color;					//!< �J���[
				VERTEX_POSITION	vertexPosition;			//!< ���_�̈ʒu

	public:			//-------------------- public --------------------//

				CTEXTURE_BASE(	CONST DEV   inDev,
								CONST LPSTR inName,
								VEC2		inSize,
								DWORD		inColor		= CLEAR_COLOR,
								CONST LPSTR	inResource = PATH_TEXTURE);	//!< �R���X�g���N�^
				CTEXTURE_BASE(void);									//!< �R���X�g���N�^(�󏉊���)
				CTEXTURE_BASE(CTEXTURE_BASE* inCopy);					//!< �R�s�[
		virtual	~CTEXTURE_BASE(void);									//!< �f�X�g���N�^

			//==================================================================
			//	���Z�b�g���s
			//==================================================================
		static	void	resetInstance(CONST DEV inDev);

			//==================================================================
			//	�e�N�X�`���̖������𒲂ׂ�
			//==================================================================
				BOOL	textureInvalid(void);

			//==================================================================
			//	access	�e�N�X�`�����
			//==================================================================
				TEXTURE_DATA*	getTextures(void)const			{ return texd_ptr;				}

			//==================================================================
			//	access	�R�[�i�[�ʒu
			//==================================================================
				VEC3	getLeftTop(void)			{ return	corner[0];						}	//!	��
				VEC3	getRightTop(void)			{ return	corner[1];						}	//!	��
				VEC3	getRightBottom(void)		{ return	corner[2];						}	//!	��
				VEC3	getLeftBottom(void)			{ return	corner[3];						}	//!	��

			//==================================================================
			//	access	�\���T�C�Y
			//==================================================================
				float	getWidth(void)const			{ return	showSize.x;						}
				float	getHeight(void)const		{ return	showSize.y;						}
				VEC2	getShowSize(void)const		{ return	showSize;						}
				void	setWidth(float inWidth)		{ showSize.x  = inWidth;	updateCorner();	}
				void	setHeight(float inHeight)	{ showSize.y  = inHeight;	updateCorner();	}
				void	setShowSize(VEC2* inShow)	{ showSize	  = *inShow;	updateCorner();	}
				void	calcWidth(float inWidth)	{ showSize.x += inWidth;	updateCorner();	}
				void	calcHeight(float inHeight)	{ showSize.y += inHeight;	updateCorner();	}
				void	calcShowSize(VEC2* inShow)	{ showSize	 += *inShow;	updateCorner();	}

			//==================================================================
			//	access	UV�T�C�Y
			//==================================================================
				VEC2	getUCoord(void)const		{ return	uCoord;							}
				VEC2	getVCoord(void)const		{ return	vCoord;							}
				void	setUCoord(VEC2 inU)			{ uCoord	= inU;		updateUVCoord();	}
				void	setVCoord(VEC2 inV)			{ vCoord	= inV;		updateUVCoord();	}
				void	setTextureCoord(RECT* inSrc);
				void	setTextureCoord(VEC2 u, VEC2 v);
				void	setTextureCoord(int inLeft, int inTop, int inRight, int inBottom);

			//==================================================================
			//	access	�J���[
			//==================================================================
				int			getColorRed(void)			{ return	(int)color.rgba.red;								}
				int			getColorGreen(void)			{ return	(int)color.rgba.green;								}
				int			getColorBlue(void)			{ return	(int)color.rgba.blue;								}
				int			getColorAlpha(void)			{ return	(int)color.rgba.alpha;								}
				RGBA		getColorRgba(void)			{ return	color.rgba;											}
				D3DCOLOR	getColor(void)const			{ return	D3DCOLOR_RGBA(color.rgba.red,  color.rgba.green,
																				  color.rgba.blue, color.rgba.alpha);	}
				void		setColorRed(int inRed)		{ color.rgba.red	= (BYTE)inRed;		updateColor();			}
				void		setColorGreen(int inGreen)	{ color.rgba.green	= (BYTE)inGreen;	updateColor();			}
				void		setColorBlue(int inBlue)	{ color.rgba.blue	= (BYTE)inBlue;		updateColor();			}
				void		setColorAlpha(int inAlpha)	{ color.rgba.alpha	= (BYTE)inAlpha;	updateColor();			}
				void		setColor(RGBA inRgba)		{ color.rgba		= inRgba;			updateColor();			}
				void		setColor(int inRed, int inGreen, int inBlue, int inAlpha);

				void		calcColorRed(int inRed);
				void		calcColorGreen(int inGreen);
				void		calcColorBlue(int inBlue);
				void		calcColorAlpha(int inAlpha);
				void		calcColor(RGBA inRgba);
				void		calcColor(int inRed, int inGreen, int inBlue, int inAlpha);

			//==================================================================
			//	access	���_�̈ʒu
			//==================================================================
				VERTEX_POSITION	getVertexPosition(void)const	{ return vertexPosition;		}
				void			setVertexPosition(VERTEX_POSITION inVp)
				{
					vertexPosition	= inVp;
					updateCorner();
				}

			//==================================================================
			//	�`��
			//==================================================================
				BOOL	drawTexture(CONST DEV inDev, MAT16* inMatrix);
				BOOL	drawTexture(CONST SPRITE	inSprite,
									RECT			rcFrom,
									RECT			rcTo,
									float			fPosZ	= 0.5f,
									VEC2*			rot		= NULL,
									float			angle	= 0.0f,
									D3DCOLOR		color	= D3DCOLOR_RGBA(255, 255, 255, 255));
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�\���̖�		TEX_SPRITER
	//
	//	�@�\			�e�N�X�`���x�[�X�ƃX�v���C�g�f�o�C�X�𑗂��
	//					���Ɋ�Â��āA2D�`�悷��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct TEX_SPRITER
	{
		RECT		rcFrom;
		RECT		rcTo;
		float		fPosZ;
		VEC2*		rot;
		float		angle;
		D3DCOLOR	color;

	//	���ԉ����̏�����
		TEX_SPRITER(void)
		{
			SetRect(&rcFrom, 0, 0, 0, 0);
			SetRect(&rcTo,   0, 0, 0, 0);
			fPosZ	= 0.0f;
			rot	= NULL;
			angle	= 0.0f;
			color	= D3DCOLOR_RGBA(255,255,255,255);
		}

	//	���
		~TEX_SPRITER(void)
		{
		//	�e�N�X�`�����
			SAFE_DELETE(rot);
		}

	//	�`��
		BOOL	draw(CONST SPRITE inSprite, CTEXTURE_BASE* texBase)
		{
			return	texBase->drawTexture(
				inSprite,
				rcFrom,
				rcTo,
				fPosZ,
				rot,
				angle,
				color);
		}
	};
}
//-----------------------------------------------------------------------
//!	�e�N�X�`���x�[�X�̖��O���
//-----------------------------------------------------------------------