//***********************************************************************************
//!	ファイル名		DirectX.h
//
//	機能			DirectXを使用する上で、必要な関数、マクロの記述
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/09/13	<新規>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	エラーコメントの制御
//-----------------------------------------------------------------------
#pragma warning(disable: 4819)
#pragma warning(disable: 4996)

#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9d.lib")

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	<d3d9.h>
#include	<d3dx9.h>
#include	<d3dx9mesh.h>
#include	<iostream>
#include	<vector>
#include	<string>
#include	<map>
#include	<windows.h>

using	namespace	std;

//-----------------------------------------------------------------------
//!	型の再定義
//-----------------------------------------------------------------------
#define VEC2			D3DXVECTOR2
#define VEC3			D3DXVECTOR3
#define VEC4			D3DXVECTOR4
#define MAT				D3DXMATRIX
#define MAT16			D3DXMATRIXA16
#define MTR				D3DMATERIAL9
#define	DEV				LPDIRECT3DDEVICE9
#define TEX				LPDIRECT3DTEXTURE9
#define	SURF			LPDIRECT3DSURFACE9
#define	SPRITE			LPD3DXSPRITE
#define TEXINFO			D3DXIMAGE_INFO
#define XMESH			LPD3DXMESH
#define	D3DHAN			D3DXHANDLE
#define MESHCTN			LPD3DXMESHCONTAINER
#define VTEX9			LPDIRECT3DVERTEXBUFFER9
#define INDEX9			LPDIRECT3DINDEXBUFFER9
#define PLANE			D3DXPLANE
#define D3DPP			D3DPRESENT_PARAMETERS
#define ANIMCTRL		LPD3DXANIMATIONCONTROLLER
#define	EFFECT			LPD3DXEFFECT
#define	FONT			LPD3DXFONT

#define	D3DRGBA			D3DCOLOR_RGBA

//-----------------------------------------------------------------------
//!	安全解放関連
//-----------------------------------------------------------------------
#ifndef	SAFE_DELETE
#define	SAFE_DELETE(p)			{if(p){delete p;		p = NULL;}}
#endif
#ifndef	SAFE_DELETE_ARRAY
#define	SAFE_DELETE_ARRAY(p)	{if(p){delete[] p;		p = NULL;}}
#endif
#ifndef	SAFE_RELEASE
#define	SAFE_RELEASE(p)			{if(p){p->Release();	p = NULL;}}
#endif
#ifndef	SAFE_VECTOR_DELETE
#define	SAFE_VECTOR_DELETE(v, i)	{							\
	vector<i>::iterator	p;										\
	for(p=v.begin(); p!=v.end(); p++){SAFE_DELETE(*p)}			\
	v.clear();													\
}
#endif
#ifndef	SAFE_VECTOR_RELEASE
#define	SAFE_VECTOR_RELEASE(v, i)	{							\
	vector<i>::iterator	p;										\
	for(p=v.begin(); p!=v.end(); p++){SAFE_RELEASE(*p)}			\
	v.clear();													\
}
#endif
#ifndef	SAFE_MAP_DELETE
#define	SAFE_MAP_DELETE(m, i, j)	{							\
	map<i,j>::iterator	p;										\
	for(p=m.begin(); p!=m.end(); p++){SAFE_DELETE(p->second)}	\
	m.clear();													\
}
#endif
#ifndef	SAFE_MAP_RELEASE
#define	SAFE_MAP_RELEASE(m, i, j)	{							\
	map<i,j>::iterator	p;										\
	for(p=m.begin(); p!=m.end(); p++){SAFE_RELEASE(p->second)}	\
	m.clear();													\
}
#endif

//-----------------------------------------------------------------------
//!	あると便利なマクロ
//-----------------------------------------------------------------------
#ifndef	INSERT_NULL_ARRAY
#define	INSERT_NULL_ARRAY(p, m)	{								\
	for(int i=0; i<m; i++){p[i]=NULL;}							\
}
#endif
#ifndef	SAFE_RELEASE_ARRAY
#define	SAFE_RELEASE_ARRAY(p, m)	{							\
	for(int i=0; i<m; i++){SAFE_RELEASE(p[i])}					\
}
#endif
#ifndef	SAFE_DELETE_PTR_ARRAY
#define	SAFE_DELETE_PTR_ARRAY(p, m)	{							\
	for(int i=0; i<m; i++){SAFE_DELETE(p[i])}					\
}
#endif
//	整数ループ
#ifndef	LOOP
#define	LOOP(m)	for(int i=0; i<m; i++)
#endif
//	vectorループ
#ifndef	VEC_LOOP
#define	VEC_LOOP(i, v)		for(vector<i>::iterator	p=v.begin(); p!=v.end(); p++)
#endif
//	vector_whileループ
#ifndef	VEC_WHILE
#define	VEC_WHILE(i, v)		vector<i>::iterator p=v.begin(); while(p!=v.end())
#endif
//	mapループ
#ifndef	MAP_LOOP
#define	MAP_LOOP(i, j, m)	for(map<i, j>::iterator	p=m.begin(); p!=m.end(); p++)
#endif
//-----------------------------------------------------------------------
//!	便利なカラー
//-----------------------------------------------------------------------
#ifndef	MY_COLOR_RGBA
//	一般使用カラー
struct	RGBA							//!< RGBAカラー
{
	BYTE	red;						//!< 赤
	BYTE	green;						//!< 緑
	BYTE	blue;						//!< 青
	BYTE	alpha;						//!< 透明度
};

union	MY_COLOR_RGBA					//!< カラー
{
	RGBA		rgba;					//!< RGBAカラー
	D3DCOLOR	color;					//!< DirectX定義のカラー

	MY_COLOR_RGBA(void)					//!< インスタンス化時の初期化
	{
		rgba.red	= 255;				//!< 赤255
		rgba.green	= 255;				//!< 緑255
		rgba.blue	= 255;				//!< 青255
		rgba.alpha	= 255;				//!< 透255
	}

	void lock(void)						//!< RGBAカラーの範囲補正
	{
		if	   (rgba.red   < 0)		rgba.red   = 0;
		else if(rgba.red   > 255)	rgba.red   = 255;
		if	   (rgba.green < 0)		rgba.green = 0;
		else if(rgba.green > 255)	rgba.green = 255;
		if	   (rgba.blue  < 0)		rgba.blue  = 0;
		else if(rgba.blue  > 255)	rgba.blue  = 255;
		if	   (rgba.alpha < 0)		rgba.alpha = 0;
		else if(rgba.alpha > 255)	rgba.alpha = 255;
	}

	//	カラー計算
	void calcRed(int inRed)
	{
		inRed	= (int)rgba.red		+	inRed;
		if	   (inRed	< 0)	rgba.red	= 0;
		else if(inRed	> 255)	rgba.red	= 255;
		else					rgba.red	= (BYTE)inRed;
	}
	void calcGreen(int inGreen)
	{
		inGreen	= (int)rgba.green	+	inGreen;
		if	   (inGreen	< 0)	rgba.green	= 0;
		else if(inGreen	> 255)	rgba.green	= 255;
		else					rgba.green	= (BYTE)inGreen;
	}
	void calcBlue(int inBlue)
	{
		inBlue	= (int)rgba.blue	+	inBlue;
		if	   (inBlue	< 0)	rgba.blue	= 0;
		else if(inBlue	> 255)	rgba.blue	= 255;
		else					rgba.blue	= (BYTE)inBlue;
	}
	void calcAlpha(int inAlpha)
	{
		inAlpha	= (int)rgba.alpha	+	inAlpha;
		if	   (inAlpha	< 0)	rgba.alpha	= 0;
		else if(inAlpha	> 255)	rgba.alpha	= 255;
		else					rgba.alpha	= (BYTE)inAlpha;
	}
};
//	DWORDカラー変換用
struct	BGRA							//!< RGBAカラー
{
	BYTE	blue;						//!< 青
	BYTE	green;						//!< 緑
	BYTE	red;						//!< 赤
	BYTE	alpha;						//!< 透明度
};

union	MY_COLOR_BGRA					//!< カラー
{
	BGRA		bgra;					//!< BGRAカラー
	D3DCOLOR	color;					//!< DirectX定義のカラー

	MY_COLOR_BGRA(void)						//!< インスタンス化時の初期化
	{
		bgra.blue	= 255;				//!< 青255
		bgra.green	= 255;				//!< 緑255
		bgra.red	= 255;				//!< 赤255
		bgra.alpha	= 255;				//!< 透255
	}

	void lock(void)						//!< RGBAカラーの範囲補正
	{
		if	   (bgra.blue  < 0)		bgra.blue  = 0;
		else if(bgra.blue  > 255)	bgra.blue  = 255;
		if	   (bgra.green < 0)		bgra.green = 0;
		else if(bgra.green > 255)	bgra.green = 255;
		if	   (bgra.red   < 0)		bgra.red   = 0;
		else if(bgra.red   > 255)	bgra.red   = 255;
		if	   (bgra.alpha < 0)		bgra.alpha = 0;
		else if(bgra.alpha > 255)	bgra.alpha = 255;
	}
};
#endif

#ifndef	MY_COLOR_ARGB
struct	ARGB							//!< ARGBカラー
{
	BYTE	alpha;						//!< 透明度
	BYTE	red;						//!< 赤
	BYTE	green;						//!< 緑
	BYTE	blue;						//!< 青
};

union	MY_COLOR_ARGB					//!< カラー
{
	ARGB		argb;					//!< ARGBカラー
	D3DCOLOR	color;					//!< DirectX定義のカラー

	MY_COLOR_ARGB(void)					//!< インスタンス化時の初期化
	{
		argb.alpha	= 255;				//!< 透255
		argb.red	= 255;				//!< 赤255
		argb.green	= 255;				//!< 緑255
		argb.blue	= 255;				//!< 青255
	}

	void lock(void)						//!< RGBAカラーの範囲補正
	{
		if	   (argb.alpha < 0)		argb.alpha = 0;
		else if(argb.alpha > 255)	argb.alpha = 255;
		if	   (argb.red   < 0)		argb.red   = 0;
		else if(argb.red   > 255)	argb.red   = 255;
		if	   (argb.green < 0)		argb.green = 0;
		else if(argb.green > 255)	argb.green = 255;
		if	   (argb.blue  < 0)		argb.blue  = 0;
		else if(argb.blue  > 255)	argb.blue  = 255;
	}
};
#endif
//-----------------------------------------------------------------------
//!	立方体
//-----------------------------------------------------------------------
#ifndef	MY_CUBE
struct	MY_CUBE						//!< 立方体の構造体
{
	float	left;					//!< 左
	float	right;					//!< 右
	float	top;					//!< 上
	float	bottom;					//!< 下
	float	front;					//!< 前
	float	rear;					//!< 後

	MY_CUBE(float inLeft	= 0,
			float inRight	= 0,
			float inTop		= 0,
			float inBottom	= 0,
			float inFront	= 0,
			float inRear	= 0)
	{
		left		= inLeft;
		right		= inRight;
		top			= inTop;
		bottom		= inBottom;
		front		= inFront;
		rear		= inRear;
	}

	//	位置判定
	BOOL intersect(VEC3* inLoc)
	{
		if(	(inLoc->x <= right)	&&	(inLoc->x >=   left)	&&
			(inLoc->y <=   top)	&&	(inLoc->y >= bottom)	&&
			(inLoc->z <= front)	&&	(inLoc->z >=   rear))
			return	TRUE;
		else
			return	FALSE;
	}
};
#endif

//-----------------------------------------------------------------------
//!	FVFの設定
//-----------------------------------------------------------------------
#define D3DFVF_VERTEX		(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define D3DFVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//-----------------------------------------------------------------------
//	頂点一つのデータ型
//-----------------------------------------------------------------------
#ifndef	CUSTOM_VERTEX
struct	CUSTOM_VERTEX
{
	VEC3	pos;						//!< 頂点の位置
	DWORD	color;						//!< カラー
	VEC2	uv;							//!< UVアンラップ
};
#endif	//CUSTOM_VERTEX
#ifndef	CUSTOM_VERTEX_2D
struct	CUSTOM_VERTEX_2D
{
	float x,y,z,rhw;					//!< トランスフォーム済み頂点情報
	DWORD color;						//!< ディフューズ
	float u, v;							//!< UV座標
};
#endif	//CUSTOM_VERTEX_2D
//	セッター関数

//-----------------------------------------------------------------------
//	テクスチャを読み込む形式
//-----------------------------------------------------------------------
#define	D3DTEX_FORMAT		D3DFMT_A8R8G8B8
#define	D3DTEX_POOL			D3DPOOL_MANAGED
#define	D3DTEX_FILTER		D3DX_FILTER_NONE
#define	D3DTEX_MIPFILTER	D3DX_DEFAULT

//-----------------------------------------------------------------------
//	テクスチャを読み込む
//-----------------------------------------------------------------------
#ifndef	D3DX_CREATE_TEXTURE_EX
#define	D3DX_CREATE_TEXTURE_EX(dev,name,color,ti,tex)				\
	D3DXCreateTextureFromFileEx(dev, name, 0, 0, 0, 0,				\
								D3DTEX_FORMAT, D3DTEX_POOL,			\
								D3DTEX_FILTER, D3DTEX_MIPFILTER,	\
								color, ti, NULL, tex)
#endif//D3DX_CREATE_TEXTURE_EX

//-----------------------------------------------------------------------
//	DirectX拡張関数群の名前空間
//-----------------------------------------------------------------------
namespace	DX
{
//	2D頂点の設定
	void setVertex2D(CUSTOM_VERTEX_2D*, float, float, float, float, DWORD, float, float);
//	エラーメッセージ&ウィンドウ破棄
	void errorMsg(CONST DEV inDev, LPSTR inError, LPSTR inTitle = "error! end program");
//	ファイルエラーメッセージ&ウィンドウ破棄
	void errorMsgNoFile(CONST DEV inDev, LPSTR inError, LPSTR inTitle = "no file error! end program");
}
//-----------------------------------------------------------------------
//	DirectX拡張関数群の名前空間
//-----------------------------------------------------------------------