//***********************************************************************************
//!	ファイル名		Class_TextureBase.cpp
//
//	機能			テクスチャベースに関する実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/08/30	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_TextureBase.h"

//-----------------------------------------------------------------------
//!	テクスチャベースの名前空間
//-----------------------------------------------------------------------
namespace TB
{
	//================================================================================
	//	共有テクスチャマップ
	//================================================================================
	map<string, TEXTURE_DATA*>*		CTEXTURE_BASE::mTextures = NULL;

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::createTextureInData
	//
	//	機能		テクスチャファイルをロードする。
	//	機能概要	テクスチャファイルをロードし、結果を返す
	//				このメソッドはクラスメソッドである。
	//	引数		inDev			Direct3Dデバイス
	//				inTexData		ロード先テクスチャデータ
	//				inName			Xファイルの名称
	//				dwColor			透明化する色
	//				inResource		テクスチャフォルダ名
	//	戻り値		TRUE			生成成功
	//				FALSE			生成失敗
	//	更新		2009/03/03		<新規>
	//================================================================================
	BOOL
	CTEXTURE_BASE::createTextureInData(	CONST DEV		inDev,
										TEXTURE_DATA*	inTexData,
										CONST LPSTR		inName,
										DWORD			inColor,
										CONST LPSTR		inResource)
	{
		TCHAR			realName[MAX_PATH]	= "";					//!< 名前にパスをつける為

	//	読み込んでみる
		if(	FAILED(D3DX_CREATE_TEXTURE_EX(
				inDev,
				inName,
				inColor,
				&inTexData->texInfo,
				&inTexData->texture)))
		{
		//	失敗したら、リソースファイルへのパスをつけてトライ
			sprintf_s(realName, sizeof(realName), inResource, inName);

			if(	FAILED(D3DX_CREATE_TEXTURE_EX(
					inDev,
					realName,
					inColor,
					&inTexData->texInfo,
					&inTexData->texture)))
			{
			//	失敗したら、NULLを返す
				goto error;
			}
		}

	//	メッシュ名をメッシュ情報に登録
		inTexData->name		= inName;
		inTexData->rPath	= inResource;
	//	カラー情報を確保
		inTexData->color	= inColor;
	//	デバイスを確保
		inTexData->device	= inDev;

	//	作成したテクスチャの配列番号を返す
		return TRUE;

	//	NULLを返す処理
	error:
		inTexData->releaseData();	//!< 生成したメッシュ情報を開放
		DX::errorMsgNoFile(inDev, realName);
		return	FALSE;				//!< NULLを返す
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::createTexture
	//
	//	機能		スキンメッシュ情報を生成し、生成したデータを返す
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				dwColor			透明化する色
	//				inResource		テクスチャフォルダ名
	//	戻り値		テクスチャ情報のポインター
	//	更新		2008/08/27		<新規>
	//================================================================================
	TEXTURE_DATA*
	CTEXTURE_BASE::createTexture(	CONST DEV	inDev,
									CONST LPSTR	inName,
									DWORD		inColor,
									CONST LPSTR	inResource)
	{
		TEXTURE_DATA*	newTex	= new TEXTURE_DATA();	//!< 新たなテクスチャ情報

	//	テクスチャを生成し、失敗したらNULLを返す
		if(	!createTextureInData(inDev, newTex, inName, inColor, inResource))
		{
			SAFE_DELETE(newTex);
			return	NULL;
		}
		return	newTex;
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::initializeVertex
	//
	//	機能		頂点バッファを生成する
	//	引数		inDev		Direct3Dデバイス
	//	更新		2008/08/28	<新規>
	//================================================================================
	void
	CTEXTURE_BASE::initializeVertex(CONST DEV inDev)
	{
	//	属性の初期化
		SAFE_RELEASE(vb);		

	//	頂点バッファの生成
		HRESULT	hr;
		hr = inDev->CreateVertexBuffer(
				NUM_TEXTURE_VERTEX * sizeof(CUSTOM_VERTEX),
				0/*D3DUSAGE_WRITEONLY*/,
				D3DFVF_VERTEX,
				D3DPOOL_MANAGED,
				&vb,
				NULL);
	//	頂点バッファの生成に失敗したなら、例外を発生
		if(	FAILED(hr))	{	throw	hr;	}

	//	コーナーの更新
		updateCorner();

	//	UV座標の更新
		updateUVCoord();

	//	カラーの更新
		updateColor();
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::updateCorner
	//
	//	機能		コーナーの更新
	//	更新		2008/08/28	<新規>
	//================================================================================
	void
	CTEXTURE_BASE::updateCorner(void)
	{
		VEC2	h		= VEC2(0.0f, 0.0f);
		VEC2	v		= VEC2(0.0f, 0.0f);

	//	頂点の位置を決定
		switch(vertexPosition)
		{
			case	eVERP_LEFT_UP:
				h	+=	VEC2(-(             0.0f),  (showSize.x       ));
				v	+=	VEC2( (             0.0f), -(showSize.y       ));
				break;
			case	eVERP_UP:
				h	+=	VEC2(-(showSize.x / 2.0f),  (showSize.x / 2.0f));
				v	+=	VEC2( (             0.0f), -(showSize.y       ));
				break;
			case	eVERP_RIGHT_UP:
				h	+=	VEC2(-(showSize.x       ),  (             0.0f));
				v	+=	VEC2( (             0.0f), -(showSize.y       ));
				break;
			case	eVERP_RIGHT:
				h	+=	VEC2(-(showSize.x       ),  (             0.0f));
				v	+=	VEC2( (showSize.y / 2.0f), -(showSize.y / 2.0f));
				break;
			case	eVERP_RIGHT_DOWN:
				h	+=	VEC2(-(showSize.x       ),  (             0.0f));
				v	+=	VEC2( (showSize.y       ), -(             0.0f));
				break;
			case	eVERP_DOWN:
				h	+=	VEC2(-(showSize.x / 2.0f),  (showSize.x / 2.0f));
				v	+=	VEC2( (showSize.y       ), -(             0.0f));
				break;
			case	eVERP_LEFT_DOWN:
				h	+=	VEC2(-(             0.0f),  (showSize.x       ));
				v	+=	VEC2( (showSize.y       ), -(             0.0f));
				break;
			case	eVERP_LEFT:
				h	+=	VEC2(-(             0.0f),  (showSize.x       ));
				v	+=	VEC2( (showSize.y / 2.0f), -(showSize.y / 2.0f));
				break;
			case	eVERP_CENTER:
				h	+=	VEC2(-(showSize.x / 2.0f),  (showSize.x / 2.0f));
				v	+=	VEC2( (showSize.y / 2.0f), -(showSize.y / 2.0f));
				break;
		}

		corner[0]	= VEC3(h.x, v.x, 0.0f);	//!<左上
		corner[1]	= VEC3(h.y, v.x, 0.0f);	//!<右上
		corner[2]	= VEC3(h.y, v.y, 0.0f);	//!<右下
		corner[3]	= VEC3(h.x, v.y, 0.0f);	//!<左下

		HRESULT	hr;
	//	頂点情報（テクスチャ座標）の更新
		CUSTOM_VERTEX *pV;
		hr = vb->Lock(0, 0, (void**)&pV, 0);
		if(	FAILED(hr))
		{
		//	失敗なら、例外を発生
			throw	hr;
		}

		pV[0].pos		= corner[0];
		pV[1].pos		= corner[1];
		pV[2].pos		= corner[2];
		pV[3].pos		= corner[3];

		VEC3	min = pV[0].pos;
		VEC3	max = pV[0].pos;
		for(int i = 0; i < NUM_TEXTURE_VERTEX; i++)
		{
			min.x	= min(pV[i].pos.x, min.x);
			min.y	= min(pV[i].pos.y, min.y);
			min.z	= min(pV[i].pos.z, min.z);
			max.x	= max(pV[i].pos.x, max.x);
			max.y	= max(pV[i].pos.y, max.y);
			max.z	= max(pV[i].pos.z, max.z);
		}

		vb->Unlock();
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::updateUVCoord
	//
	//	機能		UV座標の更新
	//	更新		2008/08/28	<新規>
	//================================================================================
	void
	CTEXTURE_BASE::updateUVCoord(void)
	{
		HRESULT	hr;
	//	頂点情報（テクスチャ座標）の更新
		CUSTOM_VERTEX *pV;
		hr = vb->Lock(0, 0, (void**)&pV, 0);
		if(	FAILED(hr))
		{
		//	失敗なら、例外を発生
			throw	hr;
		}
		pV[0].uv		= VEC2(uCoord.x, vCoord.x);
		pV[1].uv		= VEC2(uCoord.y, vCoord.x);
		pV[2].uv		= VEC2(uCoord.y, vCoord.y);
		pV[3].uv		= VEC2(uCoord.x, vCoord.y);
		vb->Unlock();
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::updateColor
	//
	//	機能		カラーの更新
	//	更新		2008/09/12	<新規>
	//================================================================================
	void
	CTEXTURE_BASE::updateColor(void)
	{
	//	カラー値の補正
		color.lock();

		HRESULT	hr;
	//	頂点情報（テクスチャ座標）の更新
		CUSTOM_VERTEX *pV;
		hr = vb->Lock(0, 0, (void**)&pV, 0);
		if(	FAILED(hr))
		{
		//	失敗なら、例外を発生
			throw	hr;
		}
	//	全ての頂点バッファの透明度を設定
		pV[0].color =
		pV[1].color =
		pV[2].color =
		pV[3].color = color.color;
		vb->Unlock();
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::startTextureInitialize
	//
	//	機能		属性の初期化
	//	更新		2008/08/30		<新規>
	//================================================================================
	void
	CTEXTURE_BASE::startTextureInitialize(void)
	{
		vb				= NULL;					//!< バーテックスバッファ
		showSize		= VEC2(0.0f, 0.0f);		//!< 表示サイズ
		texSize			= VEC2(0.0f, 0.0f);		//!< テクスチャサイズ
		uCoord			= VEC2(0.00001f, 1.0f);	//!< UV座標(右上)
		vCoord			= VEC2(0.00001f, 1.0f);	//!< UV座標(左下)
		color			= MY_COLOR_RGBA();		//!< カラー
		vertexPosition	= eVERP_CENTER;			//!< 頂点の位置

		texd_ptr		= NULL;					//!< メッシュ情報ポインター
		bTextureLoaded	= FALSE;				//!< メッシュのロード確認
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::initializeTexture
	//
	//	機能		テクスチャの初期化
	//	引数		inDev		Direct3Dデバイス
	//				inName		Xファイルの名称
	//				inSize		サイズ
	//				inColor		透明化される色
	//				inResource	テクスチャフォルダ名
	//	更新		2007/12/08		<新規>
	//================================================================================
	void
	CTEXTURE_BASE::initializeTexture(	CONST DEV	inDev,
										CONST LPSTR	inName,
										VEC2		inSize,
										DWORD		inColor,
										CONST LPSTR	inResource)
	{
	//	テクスチャの初期解放
		finishalizeTexture();

	//	テクスチャマップが無ければ生成する
		if(	!mTextures)
		{
			mTextures = new map<string, TEXTURE_DATA*>;
		}

	//	属性の初期化
		showSize			= inSize;
	//	頂点バッファを生成
		initializeVertex(inDev);

	//	作業用イテレータ
		map<string, TEXTURE_DATA*>::iterator	p;
	//	テクスチャマップにテクスチャファイル名を渡し、そのイテレータを受け取る
		p = mTextures->find(inName);

	//	テクスチャマップに同じ名前のテクスチャファイル名が既に登録してある場合
		if(	p != mTextures->end())
		{
		//	使用カウンタを増やす
			p->second->cnt++;
		//	テクスチャ情報を返す
			texd_ptr			= (*p).second;
		//	ロード完了
			bTextureLoaded	= TRUE;
		}
		else
		{
		//	新たにテクスチャを生成する
			texd_ptr	= createTexture(inDev, inName, inColor, inResource);

		//	生成に成功した場合
			if(	texd_ptr)
			{
			//	使用カウンタを増やす
				texd_ptr->cnt++;
			//	ロード完了
				bTextureLoaded	= TRUE;
			//	テクスチャマップに登録
				mTextures->insert(pair<string, TEXTURE_DATA*>(texd_ptr->name, texd_ptr));
			}
		//	失敗した場合
			else
			{
			//	ロード未完了
				bTextureLoaded	= FALSE;
			}
		}

	//	スキンメッシュ情報がある時
		if(	bTextureLoaded)
		{
		//	テクスチャサイズを保存
			texSize.x	= (float)texd_ptr->texInfo.Width;
			texSize.y	= (float)texd_ptr->texInfo.Height;
		}
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::finishalizeTexture
	//
	//	機能		テクスチャの解放
	//	更新		2007/12/08		<新規>
	//================================================================================
	void
	CTEXTURE_BASE::finishalizeTexture(void)
	{
	//	バーテックスバッファの解放
		SAFE_RELEASE(vb);

	//	テクスチャ情報をロードしてないなら、処理を返す
		if(	!bTextureLoaded)		return;

	//	フラグをもどしておく
		bTextureLoaded	= FALSE;

	//	テクスチャカウントを減らす
		texd_ptr->cnt--;
	//	このテクスチャがまだ使用されている場合は解放しない
		if(	texd_ptr->cnt > 0)	return;

	//	テクスチャマップイテレータ
		map<string, TEXTURE_DATA*>::iterator	p;
	//	テクスチャマップにXファイル名を渡し、そのイテレータを受け取る
		p = mTextures->find(texd_ptr->name);
	//	テクスチャ情報の解放
		SAFE_DELETE(p->second);
	//	テクスチャマップから削除
		mTextures->erase(p);

	//	テクスチャマップにメンバーが無ければ、消去する
		if(	mTextures->begin() == mTextures->end())
		{
			SAFE_DELETE(mTextures);
		}
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::コンストラクタ
	//
	//	機能		テクスチャをロードして初期化
	//	引数		inDev		Direct3Dデバイス
	//				inName		Xファイルの名称
	//				inSize		サイズ
	//				inColor		透明化される色
	//				inResource	テクスチャフォルダ名
	//	更新		2008/08/28	<新規>
	//================================================================================
	CTEXTURE_BASE::CTEXTURE_BASE(	CONST DEV	inDev,
									CONST LPSTR	inName,
									VEC2		inSize,
									DWORD		inColor,
									CONST LPSTR	inResource)
	{
	//	属性の初期化
		startTextureInitialize();
	//	テクスチャの初期化
		initializeTexture(inDev, inName, inSize, inColor, inResource);
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::コンストラクタ
	//
	//	機能		空初期化
	//	更新		2008/08/28	<新規>
	//================================================================================
	CTEXTURE_BASE::CTEXTURE_BASE(void)
	{
	//	属性の初期化
		startTextureInitialize();
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::コンストラクタ
	//
	//	機能		コピー
	//	引数		inCopy		コピー元
	//	更新		2008/08/28	<新規>
	//================================================================================
	CTEXTURE_BASE::CTEXTURE_BASE(CTEXTURE_BASE* inCopy)
	{
	//	属性の初期化
		startTextureInitialize();
	//	コピー開始
		texd_ptr			= inCopy->getTextures();

	//	テクスチャ情報がある時
		if(	texd_ptr)
		{
		//	残りをコピー
			showSize		= inCopy->getShowSize();
			vertexPosition	= inCopy->getVertexPosition();

		//	頂点バッファを生成
			initializeVertex(texd_ptr->device);
			setTextureCoord(inCopy->getUCoord(), inCopy->getVCoord());
			setColor(inCopy->getColorRgba());

		//	使用カウンタを増やす
			texd_ptr->cnt++;
		//	ロード完了
			bTextureLoaded	= TRUE;
		//	テクスチャサイズを保存
			texSize.x	= (float)texd_ptr->texInfo.Width;
			texSize.y	= (float)texd_ptr->texInfo.Height;
		}
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::デストラクタ
	//
	//	更新		2008/05/03	<新規>
	//================================================================================
	CTEXTURE_BASE::~CTEXTURE_BASE(void)
	{
	//	テクスチャの解放
		finishalizeTexture();
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::resetInstance
	//
	//	機能		リセット実行
	//	引数		inDev			Direct3Dデバイス
	//	更新		2009/03/02		<新規>
	//================================================================================
	void
	CTEXTURE_BASE::resetInstance(CONST DEV inDev)
	{
	//	テクスチャマップがなければ、リセットしない
		if(	!mTextures)	return;

	//	読み込んだ時の情報を取り出す為
		char	inName[MAX_PATH]	= "";
		char	inRPath[MAX_PATH]	= "";

	//	生成し直す
		MAP_LOOP(string, TEXTURE_DATA*, (*mTextures))
		{
		//	名称データを保持する
			sprintf_s(inName,   sizeof(inName),  (*p).second->name.c_str());
			sprintf_s(inRPath,  sizeof(inRPath), (*p).second->rPath.c_str());
		//	名称データで読みこみし直す
			createTextureInData(inDev, (*p).second, inName, (*p).second->color, inRPath);
		}
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::setTextureCooard
	//
	//	機能		UV座標の設定(UV版)
	//	引数		u			Uアンラップ
	//				v			Vアンラップ
	//	更新		2008/05/05	<新規>
	//================================================================================
	void
	CTEXTURE_BASE::setTextureCoord(VEC2 u, VEC2 v)
	{
	//	UV範囲を生成
		uCoord.x = u.x;
		uCoord.y = u.y;
		vCoord.x = v.x;
		vCoord.y = v.y;

	//	UV座標の更新
		updateUVCoord();
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::setTextureCooard
	//
	//	機能		UV座標の設定(数値版)
	//	引数		inLeft		左端
	//				inTop		上端
	//				inRight		右端
	//				inBottom	下端
	//	更新		2008/05/05	<新規>
	//================================================================================
	void
	CTEXTURE_BASE::setTextureCoord(int inLeft, int inTop, int inRight, int inBottom)
	{
	//	UV範囲を生成
		uCoord.x = (float)inLeft   / texSize.x;
		uCoord.y = (float)inTop    / texSize.y;
		vCoord.x = (float)inRight  / texSize.x;
		vCoord.y = (float)inBottom / texSize.y;

	//	UV座標の更新
		updateUVCoord();
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::setTextureCooard
	//
	//	機能		UV座標の設定(矩形版)
	//	引数		inSrc		描画元矩形
	//	更新		2008/05/05	<新規>
	//================================================================================
	void
	CTEXTURE_BASE::setTextureCoord(RECT *inSrc)
	{
	//	UV範囲を生成
		uCoord.x = (float)inSrc->left   / texSize.x;
		uCoord.y = (float)inSrc->top    / texSize.y;
		vCoord.x = (float)inSrc->right  / texSize.x;
		vCoord.y = (float)inSrc->bottom / texSize.y;

	//	UV座標の更新
		updateUVCoord();
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::setColor
	//
	//	機能		カラーの設定
	//	引数		inRed		赤成分
	//				inGreen		緑成分
	//				inBlue		青成分
	//				inAlpha		透明度
	//	更新		2008/09/20	<新規>
	//================================================================================
	void
	CTEXTURE_BASE::setColor(int inRed, int inGreen, int inBlue, int inAlpha)
	{
	//	RGBAカラーの設定
		color.rgba.red		= (BYTE)inRed;
		color.rgba.green	= (BYTE)inGreen;
		color.rgba.blue		= (BYTE)inBlue;
		color.rgba.alpha	= (BYTE)inAlpha;

	//	カラーの更新
		updateColor();
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::calcColorRed
	//
	//	機能		RED補正の計算
	//	引数		inRed		RED補正
	//	更新		2009/01/24	<新規>
	//================================================================================
	void
	CTEXTURE_BASE::calcColorRed(int inRed)
	{
	//	RGBAカラーの計算
		inRed	=	(int)color.rgba.red		+ inRed;
		if(	inRed   < 0){ inRed   = 0;	}	else if(inRed   > 255){ inRed   = 255;	}
		color.rgba.red	= (BYTE)inRed;

	//	カラーの更新
		updateColor();
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::calcColorGreen
	//
	//	機能		GREEN補正の計算
	//	引数		inGreen		GREEN補正
	//	更新		2009/01/24	<新規>
	//================================================================================
	void
	CTEXTURE_BASE::calcColorGreen(int inGreen)
	{
	//	RGBAカラーの計算
		inGreen	=	(int)color.rgba.green	+ inGreen;
		if(	inGreen < 0){ inGreen = 0;	}	else if(inGreen > 255){ inGreen = 255;	}
		color.rgba.green	= (BYTE)inGreen;

	//	カラーの更新
		updateColor();
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::calcColorBlue
	//
	//	機能		BLUE補正の計算
	//	引数		inBlue		BLUE補正
	//	更新		2009/01/24	<新規>
	//================================================================================
	void
	CTEXTURE_BASE::calcColorBlue(int inBlue)
	{
	//	RGBAカラーの計算
		inBlue	=	(int)color.rgba.blue	+ inBlue;
		if(	inBlue  < 0){ inBlue  = 0;	}	else if(inBlue  > 255){ inBlue  = 255;	}
		color.rgba.blue		= (BYTE)inBlue;

	//	カラーの更新
		updateColor();
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::calcColorAlpha
	//
	//	機能		ALPHA補正の計算
	//	引数		inAlpha		ALPHA補正
	//	更新		2009/01/24	<新規>
	//================================================================================
	void
	CTEXTURE_BASE::calcColorAlpha(int inAlpha)
	{
	//	RGBAカラーの計算
		inAlpha	=	(int)color.rgba.alpha	+ inAlpha;
		if(	inAlpha < 0){ inAlpha = 0;	}	else if(inAlpha > 255){ inAlpha = 255;	}
		color.rgba.alpha	= (BYTE)inAlpha;

	//	カラーの更新
		updateColor();
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::calcColor
	//
	//	機能		カラーの計算(加算のみ)
	//	引数		inRgba		RGBAカラー
	//	更新		2008/09/20	<新規>
	//================================================================================
	void
	CTEXTURE_BASE::calcColor(RGBA inRgba)
	{
	//	RGBAカラーの計算
		color.rgba.red	   += inRgba.red;
		color.rgba.green   += inRgba.green;
		color.rgba.blue	   += inRgba.blue;
		color.rgba.alpha   += inRgba.alpha;

	//	カラーの更新
		updateColor();
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::calcColor
	//
	//	機能		カラーの計算(加算減算)
	//	引数		inRed		赤成分
	//				inGreen		緑成分
	//				inBlue		青成分
	//				inAlpha		透明度
	//	更新		2008/09/20	<新規>
	//================================================================================
	void
	CTEXTURE_BASE::calcColor(int inRed, int inGreen, int inBlue, int inAlpha)
	{
	//	RGBAカラーの加減算
		inRed	=	(int)color.rgba.red		+ inRed;
		inGreen	=	(int)color.rgba.green	+ inGreen;
		inBlue	=	(int)color.rgba.blue	+ inBlue;
		inAlpha	=	(int)color.rgba.alpha	+ inAlpha;
	//	範囲設定
		if(	inRed   < 0){ inRed   = 0;	}	else if(inRed   > 255){ inRed   = 255;	}
		if(	inGreen < 0){ inGreen = 0;	}	else if(inGreen > 255){ inGreen = 255;	}
		if(	inBlue  < 0){ inBlue  = 0;	}	else if(inBlue  > 255){ inBlue  = 255;	}
		if(	inAlpha < 0){ inAlpha = 0;	}	else if(inAlpha > 255){ inAlpha = 255;	}
	//	改めて設定
		setColor(inRed,inGreen,inBlue,inAlpha);
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::textureInvalid
	//
	//	機能		テクスチャの無効性を検査
	//	戻り値		TRUE		無効
	//				FALSE		有効
	//	更新		2008/05/03	<新規>
	//================================================================================
	BOOL
	CTEXTURE_BASE::textureInvalid(void)
	{
		if(	(!bTextureLoaded)		||	//!< 読み込まれていない時
			(!texd_ptr->texture))		//!< テクスチャが生成されていない時
		{
			return TRUE;					//!< 無効
		}
		else								//!< 上記に当てはまらないなら
		{
			return FALSE;					//!< 有効
		}
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::drawTexture
	//
	//	機能		描画(3D用)
	//	引数		inDev		Direct3Dデバイス
	//				inMatrix	ワールドマトリックス
	//	更新		2008/05/03	<新規>
	//================================================================================
	BOOL
	CTEXTURE_BASE::drawTexture(CONST DEV inDev, MAT16* inMatrix)
	{
	//	描画の無効性があるなら、描画しない
		if(	textureInvalid())	return FALSE;

		inDev->SetTransform(D3DTS_WORLD, inMatrix);

	//	ライティングを無効化
		inDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		inDev->SetTexture(0, texd_ptr->texture);

	//	描画
		inDev->SetFVF(D3DFVF_VERTEX);
		inDev->SetStreamSource(0, vb, 0, sizeof(CUSTOM_VERTEX));
		inDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		inDev->SetTexture(0, NULL);

	//	ライティングモードに復帰
		inDev->SetRenderState(D3DRS_LIGHTING, TRUE);

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CTEXTURE_BASE::drawTexture
	//
	//	機能		描画(2D用)
	//	引数		inSprite	スプライトデバイス
	//				rcFrom		描画元矩形
	//				rcTo		描画先矩形
	//				fPosZ		Zバッファ
	//				*rot		回転位置オフセット
	//				angle		回転角度
	//				color		ディフューズ
	//	更新		2008/05/03	<新規>
	//================================================================================
	BOOL
	CTEXTURE_BASE::drawTexture(	CONST SPRITE	inSprite,
								RECT			rcFrom,
								RECT			rcTo,
								float			fPosZ,
								VEC2*			rot,
								float			angle,
								D3DCOLOR		color)
	{
	//-------------------------------------------------
	//	値の設定
	//-------------------------------------------------
		float	fSrcX	= float(rcFrom.right	- rcFrom.left);
		float	fSrcY	= float(rcFrom.bottom	- rcFrom.top);
		float	fDrawX	= float(rcTo.right		- rcTo.left);
		float	fDrawY	= float(rcTo.bottom		- rcTo.top);
		D3DXMATRIX		matDraw;										//!< 座標変換マトリックスの格納先
		D3DXVECTOR3		center(0.0f, 0.0f, 0.0f);						//!< 描画の基準値の設定
		D3DXVECTOR3		position(0.0f, 0.0f, fPosZ);					//!< 表示する位置を指定
		D3DXVECTOR2		draw(float(rcTo.left), float(rcTo.top));		//!< 描画先座標（演算前）
		D3DXVECTOR2		scale(fDrawX/fSrcX, fDrawY/fSrcY);				//!< スケーリング係数（倍率を指定）
		bool			bAlloc = false;
	//	回転値の確認
		if(	!rot)
		{
			rot		= new D3DXVECTOR2(fDrawX/2.0f, fDrawY/2.0f);
			bAlloc	= true;
		}

	//-------------------------------------------------
	//	スプライトの設定
	//-------------------------------------------------
		D3DXMatrixTransformation2D(&matDraw, NULL, 0.0f, &scale, rot, angle, &draw);
		if(	bAlloc)
		{
			SAFE_DELETE(rot);
		}
		inSprite->SetTransform(&matDraw);

	//-------------------------------------------------
	//	描画
	//-------------------------------------------------
		if(	FAILED(inSprite->Draw(texd_ptr->texture, &rcFrom, &center, &position, color)))
		{
			return	FALSE;
		}
		else
		{
			return	TRUE;
		}
	}
}
//-----------------------------------------------------------------------
//!	テクスチャベースの名前空間
//-----------------------------------------------------------------------