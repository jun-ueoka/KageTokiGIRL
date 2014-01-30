//***********************************************************************************
//!	ファイル名		Class_EffectBase.cpp
//
//	機能			エフェクトベースに関する実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/09/12	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_EffectBase.h"

//-----------------------------------------------------------------------
//!	エフェクトベースの名前空間
//-----------------------------------------------------------------------
namespace EB
{
	//================================================================================
	//	共有エフェクトマップ
	//================================================================================
	map<string, EFFECT_DATA*>*		CEFFECT_BASE::mEffects	= NULL;

	//================================================================================
	//!	メソッド名	CEFFECT_BASE::createEffect
	//
	//	機能		エフェクトファイルをロードする。
	//	機能概要	エフェクトファイルをロードし、そのエフェクト情報を返す
	//				このメソッドはクラスメソッドである。
	//	引数		inDev			Direct3Dデバイス
	//				inName			エフェクトファイルの名称
	//				inResource		エフェクトフォルダ名
	//				inTexture		エフェクトテクスチャフォルダ名
	//	戻り値		エフェクト情報のポインター
	//	更新		2008/09/12		<新規>
	//================================================================================
	EFFECT_DATA*
	CEFFECT_BASE::createEffect(	CONST DEV	inDev,
								CONST LPSTR inName,
								CONST LPSTR inResource,
								CONST LPSTR inTexture)
	{
		EFFECT_DATA*	newEffect	= new EFFECT_DATA();	//!< 新たなテクスチャ情報
		TCHAR			realName[MAX_PATH]	= "";			//!< 名前にパスをつける為

	//-------------------------------------------------
	//	ファイルを開く
	//-------------------------------------------------
		FILE*	fp	= NULL;

	//	ファイルを開く
		fopen_s(&fp, inName, "r");

	//	ファイルを開けなかった時
		if(	!fp)
		{
		//	パス付きエフェクトファイル名を生成
			sprintf_s(realName, sizeof(realName), inResource, inName);
		//	ファイルを開く
			fopen_s(&fp, realName, "r");

		//	ファイルを開けなかった時
			if(	!fp)
			{
			//	読み込みを行わず、NULLを返す処理へ
				goto error;
			}
		}

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	ファイルを開けたら、読み込み開始
	//-------------------------------------------------
	//	初期サイズを読み込む
	//-------------------------------------------------
		fscanf_s(fp, "%f, %f, ", &newEffect->size.x, &newEffect->size.y);

	//-------------------------------------------------
	//	テクスチャ配列を読み込む
	//-------------------------------------------------
		int		r=0, g=0, b=0, a=0;						//!< RGBA
		int		iArrayNum				= 0;			//!< 配列番号
		TCHAR	texName[MAX_PATH]		= "";			//!< テクスチャ名読み取り
		TEXINFO	texInfo;								//!< テクスチャ情報

		while(1)
		{
		//	読み込み先配列番号を読み込み、終了命令、又は不正な値が出たら終了
			fscanf_s(fp, "%d, ", &iArrayNum);
			if(	taskBreak(iArrayNum))	{ break; }

			ZeroMemory(&texInfo, sizeof(texInfo));

		//	読み込み先配列番号のテクスチャデータを初期解放
			SAFE_DELETE(newEffect->textures[iArrayNum]);
		//	テクスチャデータを生成
			newEffect->textures[iArrayNum]	= new EFFECT_TEX_DATA();

		//	テクスチャ情報を読み込む
			fscanf_s(fp, "%s  ", texName, sizeof(texName));
			fscanf_s(fp, "%d, %d, %d, %d, ", &r, &g, &b, &a);

		//	テクスチャを読み込んでみる
			if(	FAILED(D3DX_CREATE_TEXTURE_EX(
					inDev, texName, D3DCOLOR_RGBA(r,g,b,a),
					&texInfo, &newEffect->textures[iArrayNum]->texture)))
			{
			//	失敗したら、テクスチャファイルへのパスをつけてトライ
				sprintf_s(realName, sizeof(realName), inTexture, texName);

				if(	FAILED(D3DX_CREATE_TEXTURE_EX(
						inDev, realName, D3DCOLOR_RGBA(r,g,b,a),
						&texInfo, &newEffect->textures[iArrayNum]->texture)))
				{
				//	失敗したら、NULLを返す
					goto error;
				}
			}
		//	テクスチャ名を確保
			newEffect->textures[iArrayNum]->name	= realName;
		//	テクスチャサイズを確保
			newEffect->textures[iArrayNum]->sizeX	= texInfo.Width;
			newEffect->textures[iArrayNum]->sizeY	= texInfo.Height;
		//	カラー情報
			newEffect->textures[iArrayNum]->myColor.rgba.red	= BYTE(r);
			newEffect->textures[iArrayNum]->myColor.rgba.green	= BYTE(g);
			newEffect->textures[iArrayNum]->myColor.rgba.blue	= BYTE(b);
			newEffect->textures[iArrayNum]->myColor.rgba.alpha	= BYTE(a);
		}

	//-------------------------------------------------
	//	テクスチャを読み込む
	//-------------------------------------------------
		EFFECT_TEXTRUE*		newEffTex	= NULL;			//!< エフェクトテクスチャ確保先

		while(1)
		{
		//	新たにエフェクトテクスチャを生成
			newEffTex	= new EFFECT_TEXTRUE();

		//	先に登録
			newEffect->vTex.push_back(newEffTex);

		//	実行フレームを読み込み、終了命令が出たら終了
			fscanf_s(fp, "%d, ", &newEffTex->iFrame);
			if(	taskBreak(newEffTex->iFrame))	{ break; }
			if(	taskReturn(newEffTex->iFrame))	{ break; }

		//	テクスチャ情報を読み込む
			fscanf_s(fp, "%d, ", &newEffTex->iArrayNum);
		}

	//-------------------------------------------------
	//	サイズを読み込む
	//-------------------------------------------------
		EFFECT_SIZE*		newEffSize	= NULL;			//!< エフェクトサイズ確保先

		while(1)
		{
		//	新たにエフェクトサイズを生成
			newEffSize	= new EFFECT_SIZE();

		//	先に登録
			newEffect->vSize.push_back(newEffSize);

		//	実行フレームを読み込み、終了命令が出たら終了
			fscanf_s(fp, "%d, ", &newEffSize->iFrame);
			if(	taskBreak(newEffSize->iFrame))	{ break; }
			if(	taskReturn(newEffSize->iFrame))	{ break; }

		//	サイズ情報を読み込む
			fscanf_s(fp, "%f, %f, ", &newEffSize->size.x, &newEffSize->size.y);
		}

	//-------------------------------------------------
	//	回転値を読み込む
	//-------------------------------------------------
		EFFECT_ROTATO*		newEffRotato	= NULL;			//!< エフェクト回転確保先

		while(1)
		{
		//	新たにエフェクト回転を生成
			newEffRotato	= new EFFECT_ROTATO();

		//	先に登録
			newEffect->vRotato.push_back(newEffRotato);

		//	実行フレームを読み込み、終了命令が出たら終了
			fscanf_s(fp, "%d, ", &newEffRotato->iFrame);
			if(	taskBreak(newEffRotato->iFrame))	{ break; }
			if(	taskReturn(newEffRotato->iFrame))	{ break; }

		//	回転値情報を読み込む
			fscanf_s(fp, "%f, %f, %f, ",
				&newEffRotato->rotato.x,
				&newEffRotato->rotato.y,
				&newEffRotato->rotato.z);
		}

	//-------------------------------------------------
	//	移動量を読み込む
	//-------------------------------------------------
		EFFECT_MOVE*		newEffMove	= NULL;			//!< エフェクト移動確保先

		while(1)
		{
		//	新たにエフェクト移動を生成
			newEffMove	= new EFFECT_MOVE();

		//	先に登録
			newEffect->vMove.push_back(newEffMove);

		//	実行フレームを読み込み、終了命令が出たら終了
			fscanf_s(fp, "%d, ", &newEffMove->iFrame);
			if(	taskBreak(newEffMove->iFrame))	{ break; }
			if(	taskReturn(newEffMove->iFrame))	{ break; }

		//	移動情報を読み込む
			fscanf_s(fp, "%f, %f, %f, ",
				&newEffMove->middle.x,
				&newEffMove->middle.y,
				&newEffMove->middle.z);
			fscanf_s(fp, "%f, %f, %f, ",
				&newEffMove->goal.x,
				&newEffMove->goal.y,
				&newEffMove->goal.z);
		}

	//-------------------------------------------------
	//	変色値を読み込む
	//-------------------------------------------------
		EFFECT_COLOR*		newEffColor	= NULL;			//!< エフェクトカラー確保先

		while(1)
		{
		//	新たにエフェクトカラーを生成
			newEffColor	= new EFFECT_COLOR();

		//	先に登録
			newEffect->vColor.push_back(newEffColor);

		//	実行フレームを読み込み、終了命令が出たら終了
			fscanf_s(fp, "%d, ", &newEffColor->iFrame);
			if(	taskBreak(newEffColor->iFrame))	{ break; }
			if(	taskReturn(newEffColor->iFrame)){ break; }

		//	変色値情報を読み込む
			fscanf_s(fp, "%d, %d, %d, %d, ",
				&newEffColor->red,
				&newEffColor->green,
				&newEffColor->blue,
				&newEffColor->alpha);
		}

	//	最大フレーム数の読み込み
		fscanf_s(fp, "%d, ", &newEffect->frame);

	//	デバイスのポインタを保持
		newEffect->device	= inDev;

	//	エフェクト名を保存
		newEffect->name		= inName;

		fclose(fp);
	//	作成したエフェクトの情報を返す
		return newEffect;

	//	読み込み終了
	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	//	NULLを返す処理
	error:
		SAFE_DELETE(newEffect);				//!< 生成したメッシュ情報を開放
		DX::errorMsgNoFile(inDev, realName);
		return	NULL;						//!< NULLを返す
	}

	//================================================================================
	//!	メソッド名	CEFFECT_BASE::initializeVertex
	//
	//	機能		頂点バッファを生成する
	//	引数		inDev		Direct3Dデバイス
	//	更新		2008/08/28	<新規>
	//================================================================================
	void
	CEFFECT_BASE::initializeVertex(CONST DEV inDev)
	{
	//	属性の初期化
		SAFE_RELEASE(vb);		

	//	頂点バッファの生成
		HRESULT	hr;
		hr = inDev->CreateVertexBuffer(
				NUM_EFFECT_VERTEX * sizeof(CUSTOM_VERTEX),
				D3DUSAGE_WRITEONLY,
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
	//!	メソッド名	CEFFECT_BASE::updateCorner
	//
	//	機能		コーナーの更新
	//	更新		2008/09/12	<新規>
	//================================================================================
	void
	CEFFECT_BASE::updateCorner(void)
	{
		VEC2	h	= VEC2(-(showSize.x / 2.0f),  (showSize.x / 2.0f));
		VEC2	v	= VEC2( (showSize.y / 2.0f), -(showSize.y / 2.0f));

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
		for(int i = 0; i < NUM_EFFECT_VERTEX; i++)
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
	//!	メソッド名	CEFFECT_BASE::updateColor
	//
	//	機能		カラーの更新
	//	更新		2008/09/12	<新規>
	//================================================================================
	void
	CEFFECT_BASE::updateColor(void)
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
	//!	メソッド名	CEFFECT_BASE::updateUVCoord
	//
	//	機能		UV座標の更新
	//	更新		2008/09/12	<新規>
	//================================================================================
	void
	CEFFECT_BASE::updateUVCoord(void)
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
		pV[0].uv		= VEC2(0.00001f, 0.00001f);
		pV[1].uv		= VEC2(1.0f,     0.00001f);
		pV[2].uv		= VEC2(1.0f,     1.0f);
		pV[3].uv		= VEC2(0.00001f, 1.0f);
		vb->Unlock();
	}

	//================================================================================
	//!	メソッド名	CEFFECT_BASE::startEffectInitialize
	//
	//	機能		属性の初期化
	//	更新		2008/09/12		<新規>
	//================================================================================
	void
	CEFFECT_BASE::startEffectInitialize(void)
	{
		vb				= NULL;					//!< バーテックスバッファ
		showSize		= VEC2(0.0f, 0.0f);		//!< 表示サイズ
		color			= MY_COLOR_RGBA();		//!< 色

		ed_ptr			= NULL;					//!< エフェクト情報
		bEffectLoaded	= FALSE;				//!< エフェクトロード確認

	}

	//================================================================================
	//!	メソッド名	CEFFECT_BASE::initializeEffect
	//
	//	機能		エフェクトの初期化
	//	引数		inDev			Direct3Dデバイス
	//				inName			エフェクトファイルの名称
	//				inResource		エフェクトフォルダ名
	//				inTexture		エフェクトテクスチャフォルダ名
	//	更新		2008/09/12		<新規>
	//================================================================================
	void
	CEFFECT_BASE::initializeEffect(	CONST DEV	inDev,
									CONST LPSTR inName,
									CONST LPSTR inResource,
									CONST LPSTR inTexture)
	{
	//	エフェクトの初期解放
		finishalizeEffect();

	//	エフェクトマップが無ければ生成する
		if(	!mEffects)
		{
			mEffects = new map<string, EFFECT_DATA*>;
		}

	//	作業用イテレータ
		map<string, EFFECT_DATA*>::iterator	p;
	//	エフェクトマップにエフェクトファイル名を渡し、そのイテレータを受け取る
		p = mEffects->find(inName);

	//	エフェクトマップに同じ名前のエフェクトファイル名が既に登録してある場合
		if(	p != mEffects->end())
		{
		//	使用カウンタを増やす
			p->second->cnt++;
		//	エフェクト情報を受け取る
			ed_ptr	= (*p).second;
		//	ロード完了
			bEffectLoaded	= TRUE;
		}
		else
		{
		//	新たにエフェクトを生成する
			ed_ptr	= createEffect(inDev, inName, inResource, inTexture);

		//	生成に成功した場合
			if(	ed_ptr)
			{
			//	使用カウンタを増やす
				ed_ptr->cnt++;
			//	ロード完了
				bEffectLoaded	= TRUE;
			//	エフェクトマップに登録
				mEffects->insert(pair<string, EFFECT_DATA*>(ed_ptr->name, ed_ptr));
			}
		//	失敗した場合
			else
			{
			//	ロード未完了
				bEffectLoaded	= FALSE;
			}
		}

	//	エフェクト情報を持っている時
		if(	bEffectLoaded)
		{
		//	残りをコピー
			showSize	= ed_ptr->size;

		//	頂点バッファを生成
			initializeVertex(ed_ptr->device);

		//	各ポインターを取得
			eiTex		= ed_ptr->vTex.begin();
			eiSize		= ed_ptr->vSize.begin();
			eiRotato	= ed_ptr->vRotato.begin();
			eiMove		= ed_ptr->vMove.begin();
			eiColor		= ed_ptr->vColor.begin();
		}
	}

	//================================================================================
	//!	メソッド名	CEFFECT_BASE::finishalizeEffect
	//
	//	機能		エフェクトの解放
	//	更新		2008/09/12		<新規>
	//================================================================================
	void
	CEFFECT_BASE::finishalizeEffect(void)
	{
	//	エフェクト情報をロードしてないなら、処理を返す
		if(	!bEffectLoaded)		return;

	//	エフェクトカウントを減らす
		ed_ptr->cnt--;
	//	このエフェクトがまだ使用されている場合は解放しない
		if(	ed_ptr->cnt > 0)	return;

	//	エフェクトマップイテレータ
		map<string, EFFECT_DATA*>::iterator	p;
	//	エフェクトマップにエフェクトファイル名を渡し、そのイテレータを受け取る
		p = mEffects->find(ed_ptr->name);
	//	エフェクト情報の解放
		SAFE_DELETE(p->second);
	//	エフェクトマップから削除
		mEffects->erase(p);

	//	エフェクトマップにメンバーが無ければ、消去する
		if(	mEffects->begin() == mEffects->end())
		{
			SAFE_DELETE(mEffects);
		}
	}

	//================================================================================
	//!	メソッド名	CEFFECT_BASE::コンストラクタ
	//
	//	機能		エフェクトファイルをロードして初期化
	//	引数		inDev			Direct3Dデバイス
	//				inName			エフェクトファイルの名称
	//				inResource		エフェクトフォルダ名
	//				inTexture		エフェクトテクスチャフォルダ名
	//	更新		2008/09/12		<新規>
	//================================================================================
	CEFFECT_BASE::CEFFECT_BASE(	CONST DEV	inDev,
								CONST LPSTR inName,
								CONST LPSTR inResource,
								CONST LPSTR inTexture)
	{
	//	属性の初期化
		startEffectInitialize();
	//	メッシュの初期化
		initializeEffect(inDev, inName, inResource, inTexture);
	}

	//================================================================================
	//!	メソッド名	CEFFECT_BASE::コンストラクタ
	//
	//	機能		空初期化
	//	更新		2008/09/12		<新規>
	//================================================================================
	CEFFECT_BASE::CEFFECT_BASE(void)
	{
	//	属性の初期化
		startEffectInitialize();
	}

	//================================================================================
	//!	メソッド名	CEFFECT_BASE::コンストラクタ
	//
	//	機能		コピー
	//	引数		inCopy			コピー元
	//	更新		2008/09/12		<新規>
	//================================================================================
	CEFFECT_BASE::CEFFECT_BASE(CEFFECT_BASE* inCopy)
	{
	//	属性の初期化
		startEffectInitialize();
	//	コピー開始
		ed_ptr	= inCopy->getEffects();

	//	エフェクト情報がある時
		if(	ed_ptr)
		{
		//	残りをコピー
			showSize	= ed_ptr->size;

		//	頂点バッファを生成
			initializeVertex(ed_ptr->device);

		//	各ポインターを取得
			eiTex		= ed_ptr->vTex.begin();
			eiSize		= ed_ptr->vSize.begin();
			eiRotato	= ed_ptr->vRotato.begin();
			eiMove		= ed_ptr->vMove.begin();
			eiColor		= ed_ptr->vColor.begin();

		//	使用カウンタを増やす
			ed_ptr->cnt++;

		//	ロード完了
			bEffectLoaded	= TRUE;
		}
	}

	//================================================================================
	//!	メソッド名	CEFFECT_BASE::デストラクタ
	//
	//	更新		2008/09/12	<新規>
	//================================================================================
	CEFFECT_BASE::~CEFFECT_BASE(void)
	{
	//	エフェクトの解放
		finishalizeEffect();
	}

	//================================================================================
	//!	メソッド名	CEFFECT_BASE::resetInstance
	//
	//	機能		リセット実行
	//	引数		inDev			Direct3Dデバイス
	//	更新		2009/03/02		<新規>
	//================================================================================
	void
	CEFFECT_BASE::resetInstance(CONST DEV inDev)
	{
	//	エフェクトマップがなければ、リセットしない
		if(	!mEffects)	return;

	//	生成されたエフェクト情報のテクスチャだけリセット
		MAP_LOOP(string, EFFECT_DATA*, (*mEffects))
		{
			LOOP(MAX_TEXTURE)
			{
				if(	(*p).second->textures[i])
				{
				//	先にテクスチャだけ解放
					SAFE_RELEASE((*p).second->textures[i]->texture);
				//	確保されてたテクスチャ名で再度読み込み開始
					if(	FAILED(D3DX_CREATE_TEXTURE_EX(
							inDev,
							(*p).second->textures[i]->name.c_str(),
							(*p).second->textures[i]->myColor.color,
							NULL,
							&(*p).second->textures[i]->texture)))
					{
					}
				}
			}
		}
	}

	//================================================================================
	//!	メソッド名	CEFFECT_BASE::effectInvalid
	//
	//	機能		エフェクトの無効性を検査
	//	戻り値		TRUE		無効
	//				FALSE		有効
	//	更新		2008/09/12	<新規>
	//================================================================================
	BOOL
	CEFFECT_BASE::effectInvalid(void)
	{
	//	読み込まれていない時
	//	エフェクトが生成されていない時
	//	現在のテクスチャが無い時
		if(	(!bEffectLoaded)						||
			(!ed_ptr)								||
			(!ed_ptr->textures[(*eiTex)->iArrayNum]))
		{
			return TRUE;					//!< 無効
		}
		else								//!< 上記に当てはまらないなら
		{
			return FALSE;					//!< 有効
		}
	}

	//================================================================================
	//!	メソッド名	CEFFECT_BASE::setColor
	//
	//	機能		カラーの設定
	//	引数		inRed		赤成分
	//				inGreen		緑成分
	//				inBlue		青成分
	//				inAlpha		透明度
	//	更新		2008/09/20	<新規>
	//================================================================================
	void
	CEFFECT_BASE::setColor(int inRed, int inGreen, int inBlue, int inAlpha)
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
	//!	メソッド名	CEFFECT_BASE::calcColor
	//
	//	機能		カラーの計算(加算のみ)
	//	引数		inRgba		RGBAカラー
	//	更新		2008/09/20	<新規>
	//================================================================================
	void
	CEFFECT_BASE::calcColor(RGBA inRgba)
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
	//!	メソッド名	CEFFECT_BASE::calcColor
	//
	//	機能		カラーの計算(加算減算)
	//	引数		inRed		赤成分
	//				inGreen		緑成分
	//				inBlue		青成分
	//				inAlpha		透明度
	//	更新		2008/09/20	<新規>
	//================================================================================
	void
	CEFFECT_BASE::calcColor(int inRed, int inGreen, int inBlue, int inAlpha)
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
	//!	メソッド名	CEFFECT_BASE::drawEffect
	//
	//	機能		描画(3D)
	//	引数		inDev		Direct3Dデバイス
	//				inMatrix	ワールドマトリックス
	//	更新		2008/09/12	<新規>
	//================================================================================
	BOOL
	CEFFECT_BASE::drawEffect(CONST DEV inDev, MAT16* inMatrix)
	{
	//	描画の無効性があるなら、描画しない
		if(	effectInvalid())	return FALSE;

		inDev->SetTransform(D3DTS_WORLD, inMatrix);

		inDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		inDev->SetTexture(0, ed_ptr->textures[(*eiTex)->iArrayNum]->texture);

		inDev->SetFVF(D3DFVF_VERTEX);
		inDev->SetStreamSource(0, vb, 0, sizeof(CUSTOM_VERTEX));
		inDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		inDev->SetTexture(0, NULL);

	//	ライティングモードに復帰
		inDev->SetRenderState(D3DRS_LIGHTING, TRUE);

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CEFFECT_BASE::drawEffect
	//
	//	機能		描画(2D)
	//	引数		inSprite	スプライトデバイス
	//				inLoc		描画先
	//				inAngle		Z軸の角度
	//				inScale		拡大比
	//	更新		2008/09/25	<新規>
	//================================================================================
	BOOL
	CEFFECT_BASE::drawEffect(CONST SPRITE inSprite, VEC3 inLoc, float inAngle, float inScale)
	{
	//	描画の無効性があるなら、描画しない
		if(	effectInvalid())	return FALSE;

	//-------------------------------------------------
	//	値の設定
	//-------------------------------------------------
	//	描画元矩形
		RECT			rcFrom	=
		{
			0,
			0,
			ed_ptr->textures[(*eiTex)->iArrayNum]->sizeX,
			ed_ptr->textures[(*eiTex)->iArrayNum]->sizeY
		};
	//	表示サイズをスケーリング
		D3DXVECTOR2		scaleSize(showSize.x * inScale, showSize.y * inScale);
	//	座標変換マトリックスの格納先
		D3DXMATRIX		matDraw;
	//	描画の基準値の設定
		D3DXVECTOR3		center(0.0f, 0.0f, 0.0f);
	//	表示する位置を指定
		D3DXVECTOR3		position(0.0f, 0.0f, inLoc.z);
	//	描画先座標（演算前）
		D3DXVECTOR2		draw(inLoc.x-(scaleSize.x/2.0f), inLoc.y-(scaleSize.y/2.0f));
	//	スケーリング係数（倍率を指定）
		D3DXVECTOR2		scale(scaleSize.x/(float)rcFrom.right, scaleSize.y/(float)rcFrom.bottom);
	//	回転軸の設定
		D3DXVECTOR2		rot(scaleSize.x/2.0f, scaleSize.y/2.0f);

	//-------------------------------------------------
	//	スプライトの設定
	//-------------------------------------------------
		D3DXMatrixTransformation2D(&matDraw, NULL, 0.0f, &scale, &rot, inAngle, &draw);
		inSprite->SetTransform(&matDraw);

	//-------------------------------------------------
	//	描画
	//-------------------------------------------------
		if(	FAILED(inSprite->Draw(
				ed_ptr->textures[(*eiTex)->iArrayNum]->texture,
				&rcFrom, &center, &position, color.color)))
		{
			return	FALSE;
		}
		else
		{
			return	TRUE;
		}

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	エフェクトベースの名前空間
//-----------------------------------------------------------------------