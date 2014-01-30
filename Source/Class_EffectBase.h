//***********************************************************************************
//!	�t�@�C����		Class_EffectBase.h
//
//	�@�\			�G�t�F�N�g�x�[�X�Ɋւ����`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/09/11	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"DirectX.h"				//!< DirectX�g�p

//-----------------------------------------------------------------------
//!	�G�t�F�N�g�x�[�X�̖��O���
//-----------------------------------------------------------------------
namespace EB
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�e�N�X�`�����\�[�X�t�@�C���ւ̃p�X
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_EFFECTDATA		"..//Resources//Effect//Data//%s.eff"
	#define		PATH_EFFECTTEX		"..//Resources//Effect//Texture//%s"

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�Ǘ��}�N��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		MAX_TEXTURE			25

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�G�t�F�N�g�p�̃e�N�X�`���\����
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	EFFECT_TEX_DATA
	{
		TEX				texture;				//!< �e�N�X�`��
		string			name;					//!< �e�N�X�`����
		MY_COLOR_RGBA	myColor;				//!< �J���[���
		int				sizeX;					//!< �e�N�X�`��X�T�C�Y
		int				sizeY;					//!< �e�N�X�`��Y�T�C�Y

		EFFECT_TEX_DATA(void)	{ texture=NULL; sizeX=0; sizeY=0;		}
		~EFFECT_TEX_DATA(void)	{ SAFE_RELEASE(texture);				}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�G�t�F�N�g�e�N�X�`��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	EFFECT_TEXTRUE
	{
		int			iFrame;					//!< ���s�t���[��
		int			iArrayNum;				//!< �e�N�X�`���z��ԍ�

		EFFECT_TEXTRUE(void)	{ iFrame = 0;	iArrayNum = 0;			}
	};
	#define	VEFFE_TEX		vector<EFFECT_TEXTRUE*>
	#define	VEFFE_TEXI		vector<EFFECT_TEXTRUE*>::iterator

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�G�t�F�N�g�T�C�Y			��1�t���[���ɑ�������l
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	EFFECT_SIZE
	{
		int			iFrame;					//!< ���s�t���[��
		VEC2		size;					//!< �T�C�Y

		EFFECT_SIZE(void)		{ iFrame = 0;	size = VEC2(0.0f,0.0f);	}
	};
	#define	VEFFE_SIZE		vector<EFFECT_SIZE*>
	#define	VEFFE_SIZEI		vector<EFFECT_SIZE*>::iterator

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�G�t�F�N�g��]				��1�t���[���ɉ�]����l
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	EFFECT_ROTATO
	{
		int			iFrame;					//!< ���s�t���[��
		VEC3		rotato;					//!< ��]�l

		EFFECT_ROTATO(void)		{ iFrame = 0;	rotato = VEC3(0.0f,0.0f,0.0f);}
	};
	#define	VEFFE_ROTATO	vector<EFFECT_ROTATO*>
	#define	VEFFE_ROTATOI	vector<EFFECT_ROTATO*>::iterator

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�G�t�F�N�g�ړ�				��1�t���[���Ɉړ�����l
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	EFFECT_MOVE
	{
		int			iFrame;					//!< ���s�t���[��
		VEC3		middle;					//!< ���Ԓn�_
		VEC3		goal;					//!< �I���n�_

		EFFECT_MOVE(void)		{ iFrame = 0;
								  middle = VEC3(0.0f,0.0f,0.0f);	
								  goal   = VEC3(0.0f,0.0f,0.0f);	}
	};
	#define	VEFFE_MOVE		vector<EFFECT_MOVE*>
	#define	VEFFE_MOVEI		vector<EFFECT_MOVE*>::iterator

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�G�t�F�N�g�J���[			��1�t���[���̕ϐF�l
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	EFFECT_COLOR
	{
		int			iFrame;					//!< ���s�t���[��
		int			red;					//!< ��
		int			green;					//!< ��
		int			blue;					//!< ��
		int			alpha;					//!< �����x

		EFFECT_COLOR(void)
		{
			iFrame	= 0;
			red		= 0;
			green	= 0;
			blue	= 0;
			alpha	= 0;
		}
	};
	#define	VEFFE_COLOR		vector<EFFECT_COLOR*>
	#define	VEFFE_COLORI	vector<EFFECT_COLOR*>::iterator

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�G�t�F�N�g���
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	EFFECT_DATA
	{
		int					cnt;					//!< �g�p��
		string				name;					//!< �G�t�F�N�g��
		int					frame;					//!< �ő�t���[��
		VEC2				size;					//!< �����T�C�Y
		EFFECT_TEX_DATA*	textures[MAX_TEXTURE];	//!< �e�N�X�`���z��
		VEFFE_TEX			vTex;					//!< �G�t�F�N�g�e�N�X�`��
		VEFFE_SIZE			vSize;					//!< �G�t�F�N�g�T�C�Y
		VEFFE_ROTATO		vRotato;				//!< �G�t�F�N�g��]
		VEFFE_MOVE			vMove;					//!< �G�t�F�N�g�ړ�
		VEFFE_COLOR			vColor;					//!< �G�t�F�N�g�J���[
		DEV					device;					//!< �f�o�C�X�̃|�C���^

	//	���ԉ����̏�����
		EFFECT_DATA(void)
		{
			cnt			= 0;
			name		= "";
			size		= VEC2(0.0f,0.0f);
			device		= NULL;
			INSERT_NULL_ARRAY(textures, MAX_TEXTURE);
		}

	//	���
		~EFFECT_DATA(void)
		{
			SAFE_VECTOR_DELETE(vTex,		EFFECT_TEXTRUE*);
			SAFE_VECTOR_DELETE(vSize,		EFFECT_SIZE*);
			SAFE_VECTOR_DELETE(vRotato,		EFFECT_ROTATO*);
			SAFE_VECTOR_DELETE(vMove,		EFFECT_MOVE*);
			SAFE_VECTOR_DELETE(vColor,		EFFECT_COLOR*);
			SAFE_DELETE_PTR_ARRAY(textures, MAX_TEXTURE);
		}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CEFFECT_BASE
	//
	//	�@�\			�G�t�F�N�g�x�[�X
	//	�X�V			2008/09/11	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CEFFECT_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	���_��
			//==================================================================
		#define	NUM_EFFECT_VERTEX		4

			//==================================================================
			//	�G�t�F�N�g�̐���
			//==================================================================
		static	EFFECT_DATA*	createEffect(	CONST DEV		inDev,
												CONST LPSTR		inName,
												CONST LPSTR		inResource,
												CONST LPSTR		inTexture);

			//==================================================================
			//	�G�t�F�N�g�}�b�v�i�C���X�^���X�ŋ��L�������j
			//==================================================================
		static	map<string, EFFECT_DATA*>*		mEffects;

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
				void	startEffectInitialize(void);

			//==================================================================
			//	�G�t�F�N�g�̏������A���
			//==================================================================
				void	initializeEffect(CONST DEV		inDev,
										 CONST LPSTR	inName,
										 CONST LPSTR	inResource,
										 CONST LPSTR	inTexture);
				void	finishalizeEffect(void);

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	�������̔���p���\�b�h
			//==================================================================
		static	BOOL	taskBreak(int i)	{ return (i >= 10000);	} 
		static	BOOL	taskReturn(int i)	{ return (i <  0);		}

			//==================================================================
			//	�G�t�F�N�g���|�C���^�[
			//==================================================================
				EFFECT_DATA*	ed_ptr;

			//==================================================================
			//	�G�t�F�N�g�̃��[�h�m�F
			//==================================================================
				BOOL			bEffectLoaded;

			//==================================================================
			//	�G�t�F�N�g�Ɋւ��鑮��
			//==================================================================
				VEFFE_TEXI		eiTex;							//!< �G�t�F�N�g�e�N�X�`��
				VEFFE_SIZEI		eiSize;							//!< �G�t�F�N�g�T�C�Y
				VEFFE_ROTATOI	eiRotato;						//!< �G�t�F�N�g��]
				VEFFE_MOVEI		eiMove;							//!< �G�t�F�N�g�ړ�
				VEFFE_COLORI	eiColor;						//!< �G�t�F�N�g�J���[
				VTEX9			vb;								//!< �o�[�e�b�N�X�o�b�t�@
				VEC2			showSize;						//!< �\���T�C�Y
				VEC3			corner[4];						//!< �R�[�i�[�ʒu
				MY_COLOR_RGBA	color;							//!< �J���[

	public:			//-------------------- public --------------------//

				CEFFECT_BASE(CONST DEV		inDev,
							 CONST LPSTR	inName,
							 CONST LPSTR	inResource = PATH_EFFECTDATA,
							 CONST LPSTR	inTexture  = PATH_EFFECTTEX);	//!< �R���X�g���N�^
				CEFFECT_BASE(void);											//!< �R���X�g���N�^(�󏉊���)
				CEFFECT_BASE(CEFFECT_BASE*	inCopy);						//!< �R�s�[
		virtual	~CEFFECT_BASE(void);										//!< �f�X�g���N�^

			//==================================================================
			//	���Z�b�g���s
			//==================================================================
		static	void	resetInstance(CONST DEV inDev);

			//==================================================================
			//	�G�t�F�N�g�̖������𒲂ׂ�
			//==================================================================
				BOOL			effectInvalid(void);

			//==================================================================
			//	access	�G�t�F�N�g���
			//==================================================================
				EFFECT_DATA*	getEffects(void)const	{ return	ed_ptr;										}

			//==================================================================
			//	access	�\���T�C�Y
			//==================================================================
				float	getWidth(void)const				{ return	showSize.x;									}
				float	getHeight(void)const			{ return	showSize.y;									}
				VEC2	getShowSize(void)const			{ return	showSize;									}
				void	setWidth(float inWidth)			{ showSize.x	= inWidth;	updateCorner();				}
				void	setHeight(float inHeight)		{ showSize.y	= inHeight;	updateCorner();				}
				void	setShowSize(VEC2 inShow)		{ showSize		= inShow;	updateCorner();				}
				void	calcShowSize(VEC2 inShow)		{ showSize	   += inShow;	updateCorner();				}

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
				void		setColor(int inRed, int inGreen, int inBlue, int inAlpha);
				void		calcColor(RGBA inRgba);
				void		calcColor(int inRed, int inGreen, int inBlue, int inAlpha);

			//==================================================================
			//	�G�t�F�N�g���s
			//==================================================================
		virtual	BOOL	motionEffect(void)	{ return	FALSE;	}

			//==================================================================
			//	�`��
			//==================================================================
				BOOL	drawEffect(	CONST DEV inDev, MAT16* inMatrix);
				BOOL	drawEffect(	CONST SPRITE inSprite,
									VEC3		 inLoc,
									float		 inAngle	= 0.0f,
									float		 inScale	= 1.0f);
	};
}
//-----------------------------------------------------------------------
//!	�G�t�F�N�g�x�[�X�̖��O���
//-----------------------------------------------------------------------