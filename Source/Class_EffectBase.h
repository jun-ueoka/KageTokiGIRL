//***********************************************************************************
//!	ファイル名		Class_EffectBase.h
//
//	機能			エフェクトベースに関する定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/09/11	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"DirectX.h"				//!< DirectX使用

//-----------------------------------------------------------------------
//!	エフェクトベースの名前空間
//-----------------------------------------------------------------------
namespace EB
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	テクスチャリソースファイルへのパス
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_EFFECTDATA		"..//Resources//Effect//Data//%s.eff"
	#define		PATH_EFFECTTEX		"..//Resources//Effect//Texture//%s"

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	管理マクロ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		MAX_TEXTURE			25

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	エフェクト用のテクスチャ構造体
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	EFFECT_TEX_DATA
	{
		TEX				texture;				//!< テクスチャ
		string			name;					//!< テクスチャ名
		MY_COLOR_RGBA	myColor;				//!< カラー情報
		int				sizeX;					//!< テクスチャXサイズ
		int				sizeY;					//!< テクスチャYサイズ

		EFFECT_TEX_DATA(void)	{ texture=NULL; sizeX=0; sizeY=0;		}
		~EFFECT_TEX_DATA(void)	{ SAFE_RELEASE(texture);				}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	エフェクトテクスチャ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	EFFECT_TEXTRUE
	{
		int			iFrame;					//!< 実行フレーム
		int			iArrayNum;				//!< テクスチャ配列番号

		EFFECT_TEXTRUE(void)	{ iFrame = 0;	iArrayNum = 0;			}
	};
	#define	VEFFE_TEX		vector<EFFECT_TEXTRUE*>
	#define	VEFFE_TEXI		vector<EFFECT_TEXTRUE*>::iterator

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	エフェクトサイズ			※1フレームに増減する値
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	EFFECT_SIZE
	{
		int			iFrame;					//!< 実行フレーム
		VEC2		size;					//!< サイズ

		EFFECT_SIZE(void)		{ iFrame = 0;	size = VEC2(0.0f,0.0f);	}
	};
	#define	VEFFE_SIZE		vector<EFFECT_SIZE*>
	#define	VEFFE_SIZEI		vector<EFFECT_SIZE*>::iterator

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	エフェクト回転				※1フレームに回転する値
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	EFFECT_ROTATO
	{
		int			iFrame;					//!< 実行フレーム
		VEC3		rotato;					//!< 回転値

		EFFECT_ROTATO(void)		{ iFrame = 0;	rotato = VEC3(0.0f,0.0f,0.0f);}
	};
	#define	VEFFE_ROTATO	vector<EFFECT_ROTATO*>
	#define	VEFFE_ROTATOI	vector<EFFECT_ROTATO*>::iterator

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	エフェクト移動				※1フレームに移動する値
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	EFFECT_MOVE
	{
		int			iFrame;					//!< 実行フレーム
		VEC3		middle;					//!< 中間地点
		VEC3		goal;					//!< 終了地点

		EFFECT_MOVE(void)		{ iFrame = 0;
								  middle = VEC3(0.0f,0.0f,0.0f);	
								  goal   = VEC3(0.0f,0.0f,0.0f);	}
	};
	#define	VEFFE_MOVE		vector<EFFECT_MOVE*>
	#define	VEFFE_MOVEI		vector<EFFECT_MOVE*>::iterator

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	エフェクトカラー			※1フレームの変色値
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	EFFECT_COLOR
	{
		int			iFrame;					//!< 実行フレーム
		int			red;					//!< 赤
		int			green;					//!< 緑
		int			blue;					//!< 青
		int			alpha;					//!< 透明度

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
	//	エフェクト情報
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	EFFECT_DATA
	{
		int					cnt;					//!< 使用数
		string				name;					//!< エフェクト名
		int					frame;					//!< 最大フレーム
		VEC2				size;					//!< 初期サイズ
		EFFECT_TEX_DATA*	textures[MAX_TEXTURE];	//!< テクスチャ配列
		VEFFE_TEX			vTex;					//!< エフェクトテクスチャ
		VEFFE_SIZE			vSize;					//!< エフェクトサイズ
		VEFFE_ROTATO		vRotato;				//!< エフェクト回転
		VEFFE_MOVE			vMove;					//!< エフェクト移動
		VEFFE_COLOR			vColor;					//!< エフェクトカラー
		DEV					device;					//!< デバイスのポインタ

	//	実態化時の初期化
		EFFECT_DATA(void)
		{
			cnt			= 0;
			name		= "";
			size		= VEC2(0.0f,0.0f);
			device		= NULL;
			INSERT_NULL_ARRAY(textures, MAX_TEXTURE);
		}

	//	解放
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
	//!	クラス名		CEFFECT_BASE
	//
	//	機能			エフェクトベース
	//	更新			2008/09/11	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CEFFECT_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	頂点数
			//==================================================================
		#define	NUM_EFFECT_VERTEX		4

			//==================================================================
			//	エフェクトの生成
			//==================================================================
		static	EFFECT_DATA*	createEffect(	CONST DEV		inDev,
												CONST LPSTR		inName,
												CONST LPSTR		inResource,
												CONST LPSTR		inTexture);

			//==================================================================
			//	エフェクトマップ（インスタンスで共有される情報）
			//==================================================================
		static	map<string, EFFECT_DATA*>*		mEffects;

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
				void	startEffectInitialize(void);

			//==================================================================
			//	エフェクトの初期化、解放
			//==================================================================
				void	initializeEffect(CONST DEV		inDev,
										 CONST LPSTR	inName,
										 CONST LPSTR	inResource,
										 CONST LPSTR	inTexture);
				void	finishalizeEffect(void);

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	処理時の判定用メソッド
			//==================================================================
		static	BOOL	taskBreak(int i)	{ return (i >= 10000);	} 
		static	BOOL	taskReturn(int i)	{ return (i <  0);		}

			//==================================================================
			//	エフェクト情報ポインター
			//==================================================================
				EFFECT_DATA*	ed_ptr;

			//==================================================================
			//	エフェクトのロード確認
			//==================================================================
				BOOL			bEffectLoaded;

			//==================================================================
			//	エフェクトに関する属性
			//==================================================================
				VEFFE_TEXI		eiTex;							//!< エフェクトテクスチャ
				VEFFE_SIZEI		eiSize;							//!< エフェクトサイズ
				VEFFE_ROTATOI	eiRotato;						//!< エフェクト回転
				VEFFE_MOVEI		eiMove;							//!< エフェクト移動
				VEFFE_COLORI	eiColor;						//!< エフェクトカラー
				VTEX9			vb;								//!< バーテックスバッファ
				VEC2			showSize;						//!< 表示サイズ
				VEC3			corner[4];						//!< コーナー位置
				MY_COLOR_RGBA	color;							//!< カラー

	public:			//-------------------- public --------------------//

				CEFFECT_BASE(CONST DEV		inDev,
							 CONST LPSTR	inName,
							 CONST LPSTR	inResource = PATH_EFFECTDATA,
							 CONST LPSTR	inTexture  = PATH_EFFECTTEX);	//!< コンストラクタ
				CEFFECT_BASE(void);											//!< コンストラクタ(空初期化)
				CEFFECT_BASE(CEFFECT_BASE*	inCopy);						//!< コピー
		virtual	~CEFFECT_BASE(void);										//!< デストラクタ

			//==================================================================
			//	リセット実行
			//==================================================================
		static	void	resetInstance(CONST DEV inDev);

			//==================================================================
			//	エフェクトの無効性を調べる
			//==================================================================
				BOOL			effectInvalid(void);

			//==================================================================
			//	access	エフェクト情報
			//==================================================================
				EFFECT_DATA*	getEffects(void)const	{ return	ed_ptr;										}

			//==================================================================
			//	access	表示サイズ
			//==================================================================
				float	getWidth(void)const				{ return	showSize.x;									}
				float	getHeight(void)const			{ return	showSize.y;									}
				VEC2	getShowSize(void)const			{ return	showSize;									}
				void	setWidth(float inWidth)			{ showSize.x	= inWidth;	updateCorner();				}
				void	setHeight(float inHeight)		{ showSize.y	= inHeight;	updateCorner();				}
				void	setShowSize(VEC2 inShow)		{ showSize		= inShow;	updateCorner();				}
				void	calcShowSize(VEC2 inShow)		{ showSize	   += inShow;	updateCorner();				}

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
				void		setColor(int inRed, int inGreen, int inBlue, int inAlpha);
				void		calcColor(RGBA inRgba);
				void		calcColor(int inRed, int inGreen, int inBlue, int inAlpha);

			//==================================================================
			//	エフェクト実行
			//==================================================================
		virtual	BOOL	motionEffect(void)	{ return	FALSE;	}

			//==================================================================
			//	描画
			//==================================================================
				BOOL	drawEffect(	CONST DEV inDev, MAT16* inMatrix);
				BOOL	drawEffect(	CONST SPRITE inSprite,
									VEC3		 inLoc,
									float		 inAngle	= 0.0f,
									float		 inScale	= 1.0f);
	};
}
//-----------------------------------------------------------------------
//!	エフェクトベースの名前空間
//-----------------------------------------------------------------------