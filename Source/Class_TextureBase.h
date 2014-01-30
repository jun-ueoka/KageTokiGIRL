//***********************************************************************************
//!	ファイル名		Class_TextureBase.h
//
//	機能			テクスチャベースに関する定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/08/30	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"DirectX.h"				//!< DirectX使用

//-----------------------------------------------------------------------
//!	テクスチャベースの名前空間
//-----------------------------------------------------------------------
namespace TB
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	テクスチャリソースファイルへのパス
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_TEXTURE		"..//Resources//Texture//%s.dds"
	#define		CLEAR_COLOR			D3DCOLOR_RGBA(255,255,255,255)

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	頂点の位置
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum VERTEX_POSITION
	{
		eVERP_LEFT_UP,   eVERP_UP,     eVERP_RIGHT_UP,			//!< ┏━┓
		eVERP_LEFT,      eVERP_CENTER, eVERP_RIGHT,				//!< ┃╋┃
		eVERP_LEFT_DOWN, eVERP_DOWN,   eVERP_RIGHT_DOWN,		//!< ┗━┛
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	テクスチャ情報
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct TEXTURE_DATA
	{
		int			cnt;			//!< 使用数
		string		name;			//!< テクスチャ名
		string		rPath;			//!< リソースパス
		string		tPath;			//!< テクスチャパス
		TEX			texture;		//!< テクスチャ
		TEXINFO		texInfo;		//!< テクスチャ詳細
		DWORD		color;			//!< カラー情報	
		DEV			device;			//!< デバイスのポインタ

	//	実態化時の初期化
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

	//	解放時処理
		~TEXTURE_DATA(void)
		{
			releaseData();
		}

	//	解放
		void releaseData(void)
		{
		//	テクスチャ解放
			SAFE_RELEASE(texture);
		}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CTEXTURE_BASE
	//
	//	機能			テクスチャベース
	//	更新			2008/08/30	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CTEXTURE_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	頂点数
			//==================================================================
		#define	NUM_TEXTURE_VERTEX		4

			//==================================================================
			//	テクスチャの生成
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
			//	テクスチャマップ（インスタンスで共有される情報）
			//==================================================================
		static	map<string, TEXTURE_DATA*>*		mTextures;

			//==================================================================
			//	初期化、更新
			//==================================================================
				void	initializeVertex(CONST DEV inDev);			//!< 頂点初期化
				void	updateCorner(void);							//!< コーナーの更新
				void	updateUVCoord(void);						//!< UVの更新
				void	updateColor(void);							//!< カラーの更新

			//==================================================================
			//	属性の初期準備
			//==================================================================
				void	startTextureInitialize(void);

			//==================================================================
			//	テクスチャの初期化、解放
			//==================================================================
				void	initializeTexture(	CONST DEV   inDev,
											CONST LPSTR inName,
											VEC2		inSize,
											DWORD		inColor		= CLEAR_COLOR,
											CONST LPSTR inResource	= PATH_TEXTURE);
				void	finishalizeTexture(void);

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	テクスチャ情報ポインター
			//==================================================================
				TEXTURE_DATA*	texd_ptr;

			//==================================================================
			//	テクスチャのロード確認
			//==================================================================
				BOOL			bTextureLoaded;

			//==================================================================
			//	テクスチャに関する属性
			//==================================================================
				VTEX9			vb;						//!< バーテックスバッファ
				VEC2			showSize;				//!< 表示サイズ
				VEC2			texSize;				//!< テクスチャサイズ
				VEC3			corner[4];				//!< コーナー位置
				VEC2			uCoord;					//!< UV座標(右上)
				VEC2			vCoord;					//!< UV座標(左下)
				MY_COLOR_RGBA	color;					//!< カラー
				VERTEX_POSITION	vertexPosition;			//!< 頂点の位置

	public:			//-------------------- public --------------------//

				CTEXTURE_BASE(	CONST DEV   inDev,
								CONST LPSTR inName,
								VEC2		inSize,
								DWORD		inColor		= CLEAR_COLOR,
								CONST LPSTR	inResource = PATH_TEXTURE);	//!< コンストラクタ
				CTEXTURE_BASE(void);									//!< コンストラクタ(空初期化)
				CTEXTURE_BASE(CTEXTURE_BASE* inCopy);					//!< コピー
		virtual	~CTEXTURE_BASE(void);									//!< デストラクタ

			//==================================================================
			//	リセット実行
			//==================================================================
		static	void	resetInstance(CONST DEV inDev);

			//==================================================================
			//	テクスチャの無効性を調べる
			//==================================================================
				BOOL	textureInvalid(void);

			//==================================================================
			//	access	テクスチャ情報
			//==================================================================
				TEXTURE_DATA*	getTextures(void)const			{ return texd_ptr;				}

			//==================================================================
			//	access	コーナー位置
			//==================================================================
				VEC3	getLeftTop(void)			{ return	corner[0];						}	//!	┏
				VEC3	getRightTop(void)			{ return	corner[1];						}	//!	┓
				VEC3	getRightBottom(void)		{ return	corner[2];						}	//!	┛
				VEC3	getLeftBottom(void)			{ return	corner[3];						}	//!	┗

			//==================================================================
			//	access	表示サイズ
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
			//	access	UVサイズ
			//==================================================================
				VEC2	getUCoord(void)const		{ return	uCoord;							}
				VEC2	getVCoord(void)const		{ return	vCoord;							}
				void	setUCoord(VEC2 inU)			{ uCoord	= inU;		updateUVCoord();	}
				void	setVCoord(VEC2 inV)			{ vCoord	= inV;		updateUVCoord();	}
				void	setTextureCoord(RECT* inSrc);
				void	setTextureCoord(VEC2 u, VEC2 v);
				void	setTextureCoord(int inLeft, int inTop, int inRight, int inBottom);

			//==================================================================
			//	access	カラー
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
			//	access	頂点の位置
			//==================================================================
				VERTEX_POSITION	getVertexPosition(void)const	{ return vertexPosition;		}
				void			setVertexPosition(VERTEX_POSITION inVp)
				{
					vertexPosition	= inVp;
					updateCorner();
				}

			//==================================================================
			//	描画
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
	//	構造体名		TEX_SPRITER
	//
	//	機能			テクスチャベースとスプライトデバイスを送ると
	//					情報に基づいて、2D描画する
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct TEX_SPRITER
	{
		RECT		rcFrom;
		RECT		rcTo;
		float		fPosZ;
		VEC2*		rot;
		float		angle;
		D3DCOLOR	color;

	//	実態化時の初期化
		TEX_SPRITER(void)
		{
			SetRect(&rcFrom, 0, 0, 0, 0);
			SetRect(&rcTo,   0, 0, 0, 0);
			fPosZ	= 0.0f;
			rot	= NULL;
			angle	= 0.0f;
			color	= D3DCOLOR_RGBA(255,255,255,255);
		}

	//	解放
		~TEX_SPRITER(void)
		{
		//	テクスチャ解放
			SAFE_DELETE(rot);
		}

	//	描画
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
//!	テクスチャベースの名前空間
//-----------------------------------------------------------------------