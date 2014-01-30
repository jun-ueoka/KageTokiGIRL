//***********************************************************************************
//!	ファイル名		Class_MeshBase.cpp
//
//	機能			メッシュベースに関する定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/08/30	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_MeshBase.h"				//!< メッシュベース

//-----------------------------------------------------------------------
//!	メッシュベースの名前空間
//-----------------------------------------------------------------------
namespace MB
{
	//================================================================================
	//	共有メッシュマップ
	//================================================================================
	map<string, MESH_DATA*>*	CMESH_BASE::mMeshs = NULL;

	//================================================================================
	//!	メソッド名	CMESH_BASE::createMeshInData
	//
	//	機能		Xファイルをロードし、メッシュ情報を生成
	//	機能概要	メッシュ情報をロードし、結果を返す
	//				このメソッドはクラスメソッドである
	//	引数		inDev			Direct3Dデバイス
	//				inMeshData		ロード先メッシュデータ
	//				inName			Xファイルの名称
	//				inResource		メッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	戻り値		TRUE			生成成功
	//				FALSE			生成失敗
	//	更新		2009/03/02		<新規>
	//================================================================================
	BOOL
	CMESH_BASE::createMeshInData(CONST DEV		inDev,
								 MESH_DATA*		inMeshData,
								 CONST LPSTR	inName,
								 CONST LPSTR	inResource,
								 CONST LPSTR	inTexture)
	{
		LPD3DXBUFFER	mtrlBuffer			= NULL;				//!< マテリアルバッファ
		LPD3DXBUFFER	adjacencyBuffer		= NULL;				//!< 隣接バッファ
		D3DXMATERIAL	*materials			= NULL;				//!< マテリアル

		TCHAR		realName[MAX_PATH]		= "";				//!< 名前にパスをつける為

	//	メッシュデータを初期解放
		inMeshData->releaseData();

	//	読み込んでみる
		if(	FAILED(D3DXLoadMeshFromX(
				inName,
				D3DXMESH_SYSTEMMEM,
				inDev,
				&adjacencyBuffer,
				&mtrlBuffer,
				NULL,
				&inMeshData->numMaterials,
				&inMeshData->mesh)))
		{
		//	失敗したら、リソースファイルへのパスをつけてトライ
			sprintf_s(realName, sizeof(realName), inResource, inName);

			if(	FAILED(D3DXLoadMeshFromX(
					realName,
					D3DXMESH_SYSTEMMEM,
					inDev,
					&adjacencyBuffer,
					&mtrlBuffer,
					NULL,
					&inMeshData->numMaterials,
					&inMeshData->mesh)))
			{
			//	失敗したら、NULLを返す
				goto error;
			}
		}

	//	メッシュの最適化
		if(	FAILED(inMeshData->mesh->OptimizeInplace(
						D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
						(DWORD*)adjacencyBuffer->GetBufferPointer(),
						NULL,
						NULL,
						NULL)))
		{
		//	メッシュの最適化ができなければ、終了
			SAFE_RELEASE(adjacencyBuffer);	//!< 隣接バッファを開放
			SAFE_RELEASE(mtrlBuffer);		//!< マテリアルバッファ
			goto error;						//!< NULL返す
		}

	//	法線情報の確認
		DWORD currentFVF = 0, newFVF = 0, normalFVFV = D3DFVF_NORMAL;

	//	現在のFVFを取得する
		currentFVF = inMeshData->mesh->GetFVF();

	//	法線情報が無かったら、メッシュを作成しなおす
		if(	!(currentFVF && D3DFVF_NORMAL))
		{
			XMESH cloneMesh = NULL;		//!< 新たに作成したメッシュの格納先
		//	現在のFVFに、FVFの初期設定を掛け合わせる
			newFVF = currentFVF | D3DFVF_CLONEVERTEX;
		//	新たなFVFで、新たなメッシュの作成
			inMeshData->mesh->CloneMeshFVF(
				inMeshData->mesh->GetOptions(), newFVF, inDev, &cloneMesh);
		//	前のメッシュを破棄
			SAFE_RELEASE(inMeshData->mesh);
		//	新たなメッシュを格納
			inMeshData->mesh = cloneMesh;
		}

	//	法線を計算する
	//	3DCGツールによっては法線が正しくない場合があるため.
		D3DXComputeNormals(inMeshData->mesh, NULL);

	//	マテリアルバッファポインターを取得
		materials = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

	//	メッシュのマテリアルの数だけ、マテリアルとテクスチャを作成
		inMeshData->materials	= new MTR[inMeshData->numMaterials];
		inMeshData->texture	= new TEX[inMeshData->numMaterials];

	//	すべてのマテリアルの設定
		for(DWORD i = 0; i < inMeshData->numMaterials; i++)
		{
		//	マテリアルのプロパディを格納
			inMeshData->materials[i]			= materials[i].MatD3D;
		//	アンビエント色とディフューズ色を同期
			inMeshData->materials[i].Ambient	= inMeshData->materials[i].Diffuse;
		//	マテリアルにテクスチャが無い場合
			if(	materials[i].pTextureFilename == NULL)
			{
			//	現在見ているマテリアルと、同じ配列要素番号を持つテクスチャにNULLを入れて、次のループへ
				inMeshData->texture[i] = NULL;
				continue;
			}
		//	テクスチャを生成
			if(	FAILED(D3DXCreateTextureFromFile(inDev,
												 materials[i].pTextureFilename,
												 &inMeshData->texture[i])))
			{
			//	失敗したら、メッシュリソースファイルへのパスをつけてトライ
				sprintf_s(realName, sizeof(realName), inTexture, materials[i].pTextureFilename);

				if(	FAILED(D3DXCreateTextureFromFile(inDev,
													 realName,
													 &inMeshData->texture[i])))
				{
				//	失敗したら、NULLを返す
					goto error;
				}
			}
		}
		SAFE_RELEASE(adjacencyBuffer);	//!< 隣接バッファを開放
		SAFE_RELEASE(mtrlBuffer);		//!< マテリアルバッファ

	//	頂点バッファを生成する
		if(	FAILED(inMeshData->mesh->GetVertexBuffer(&inMeshData->meshVB)))
		{
		//	失敗したら、NULLを返す
			goto error;
		}

	//	バウンディング・スフィアを求める
		BYTE* pv = NULL;
	//	頂点バッファメモリへのポインタを取得
		if(	inMeshData->mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pv) == D3D_OK)
		{
		//	メッシュの境界球を算出する
			D3DXComputeBoundingSphere(
				(VEC3*)pv,											//!< 最初の位置へのポインタ
				inMeshData->mesh->GetNumVertices(),					//!< 頂点の数
				D3DXGetFVFVertexSize(inMeshData->mesh->GetFVF()),	//!< 頂点のサイズ
				&inMeshData->localCenter,							//!< 返される境界球の座標の中心
				&inMeshData->localRadius);							//!< 返される境界球の半径
		//	頂点をアンロック
			inMeshData->mesh->UnlockVertexBuffer();
		}

	//	メッシュ名をメッシュ情報に登録
		inMeshData->name	= inName;
		inMeshData->rPath	= inResource;
		inMeshData->tPath	= inTexture;

		return	TRUE;

	//	NULLを返す処理
	error:
		inMeshData->releaseData();
		DX::errorMsgNoFile(inDev, realName);
		return	FALSE;				//!< NULLを返す
	}

	//================================================================================
	//!	メソッド名	CMESH_BASE::createMesh
	//
	//	機能		メッシュ情報を生成し、生成したデータを返す
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		メッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	戻り値		メッシュ情報のポインター
	//	更新		2007/12/08		<新規>
	//================================================================================
	MESH_DATA*
	CMESH_BASE::createMesh(	CONST DEV	inDev,
							CONST LPSTR	inName,
							CONST LPSTR	inResource,
							CONST LPSTR	inTexture)
	{
		MESH_DATA*	newMesh	= new MESH_DATA();	//!< 新たなメッシュ情報

	//	メッシュを生成し、失敗したらNULLを返す
		if(	!createMeshInData(inDev, newMesh, inName, inResource, inTexture))
		{
			SAFE_DELETE(newMesh);
			return	NULL;
		}
		return	newMesh;
	}

	//================================================================================
	//!	メソッド名	CMESH_BASE::startMeshInitialize
	//
	//	機能		属性の初期化
	//	更新		2008/08/30		<新規>
	//================================================================================
	void
	CMESH_BASE::startMeshInitialize(void)
	{
		md_ptr		= NULL;			//!< メッシュ情報
		bMeshLoaded	= FALSE;		//!< メッシュロード確認
	}

	//================================================================================
	//!	メソッド名	CMESH_BASE::initializeMesh
	//
	//	機能		メッシュの初期化
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		メッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2007/12/08		<新規>
	//================================================================================
	void
	CMESH_BASE::initializeMesh(	CONST DEV	inDev,
								CONST LPSTR inName,
								CONST LPSTR	inResource,
								CONST LPSTR	inTexture)
	{
	//	メッシュの初期解放
		finishalizeMesh();

	//	メッシュマップが無ければ生成する
		if(	!mMeshs)
		{
			mMeshs = new map<string, MESH_DATA*>;
		}

	//	作業用イテレータ
		map<string, MESH_DATA*>::iterator	p;
	//	メッシュマップにXファイル名を渡し、そのイテレータを受け取る
		p = mMeshs->find(inName);

	//	メッシュマップに同じ名前のXファイル名が既に登録してある場合
		if(	p != mMeshs->end())
		{
		//	使用カウンタを増やす
			p->second->cnt++;
		//	メッシュ情報を返す
			md_ptr		= (*p).second;
		//	ロード完了
			bMeshLoaded	= TRUE;
		}
		else
		{
		//	新たにメッシュを生成する
			md_ptr	= createMesh(inDev, inName, inResource, inTexture);

		//	生成に成功した場合
			if(	md_ptr)
			{
			//	使用カウンタを増やす
				md_ptr->cnt++;
			//	ロード完了
				bMeshLoaded	= TRUE;
			//	メッシュマップに登録
				mMeshs->insert(pair<string, MESH_DATA*>(md_ptr->name, md_ptr));
			}
		//	失敗した場合
			else
			{
			//	ロード未完了
				bMeshLoaded	= FALSE;
			}
		}
	}

	//================================================================================
	//!	メソッド名	CMESH_BASE::finishalizeMesh
	//
	//	機能		メッシュの解放
	//	更新		2007/12/08		<新規>
	//================================================================================
	void
	CMESH_BASE::finishalizeMesh(void)
	{
	//	メッシュ情報をロードしてないなら、処理を返す
		if(	!bMeshLoaded)	return;

	//	フラグをもどしておく
		bMeshLoaded	= FALSE;

	//	メッシュカウントを減らす
		md_ptr->cnt--;
	//	このメッシュがまだ使用されている場合は解放しない
		if(	md_ptr->cnt > 0)	return;

	//	メッシュマップイテレータ
		map<string, MESH_DATA*>::iterator	p;
	//	メッシュマップにXファイル名を渡し、そのイテレータを受け取る
		p = mMeshs->find(md_ptr->name);
	//	メッシュ情報の解放
		SAFE_DELETE(p->second);
	//	メッシュマップから削除
		mMeshs->erase(p);

	//	メッシュマップにメンバーが無ければ、消去する
		if(	mMeshs->begin() == mMeshs->end())
		{
			SAFE_DELETE(mMeshs);
		}
	}

	//================================================================================
	//!	メソッド名	CMESH_BASE::コンストラクタ
	//
	//	機能		Xファイルをロードして初期化
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		メッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2007/12/08		<新規>
	//================================================================================
	CMESH_BASE::CMESH_BASE(	CONST DEV	inDev,
							CONST LPSTR	inName,
							CONST LPSTR	inResource,
							CONST LPSTR	inTexture)
	{
	//	属性の初期化
		startMeshInitialize();
	//	メッシュの初期化
		initializeMesh(inDev, inName, inResource, inTexture);
	}

	//================================================================================
	//!	メソッド名	CMESH_BASE::コンストラクタ
	//
	//	機能		空初期化
	//	更新		2007/12/08		<新規>
	//================================================================================
	CMESH_BASE::CMESH_BASE(void)
	{
	//	属性の初期化
		startMeshInitialize();
	}

	//================================================================================
	//!	メソッド名	CMESH_BASE::コンストラクタ
	//
	//	機能		コピー
	//	引数		inCopy			コピー元
	//	更新		2007/12/08		<新規>
	//================================================================================
	CMESH_BASE::CMESH_BASE(CMESH_BASE* inCopy)
	{
	//	属性の初期化
		startMeshInitialize();
	//	コピー開始
		md_ptr	= inCopy->getMeshs();

	//	メッシュ情報がある時
		if(	md_ptr)
		{
		//	使用カウンタを増やす
			md_ptr->cnt++;
		//	ロード完了
			bMeshLoaded	= TRUE;
		}
	}

	//================================================================================
	//!	メソッド名	CMESH_BASE::デストラクタ
	//
	//	機能		全ての資源を解放し、終了する。
	//				資源が、他のオブジェクトでも使われていた場合は残す。
	//	更新		2007/12/08		<新規>
	//================================================================================
	CMESH_BASE::~CMESH_BASE()
	{
	//	メッシュの解放
		finishalizeMesh();
	}

	//================================================================================
	//!	メソッド名	CMESH_BASE::resetInstance
	//
	//	機能		リセット実行
	//	引数		inDev			Direct3Dデバイス
	//	更新		2009/03/02		<新規>
	//================================================================================
	void
	CMESH_BASE::resetInstance(CONST DEV inDev)
	{
	//	メッシュマップがなければ、リセットしない
		if(	!mMeshs)	return;

	//	読み込んだ時の情報を取り出す為
		char	inName[MAX_PATH]	= "";
		char	inRPath[MAX_PATH]	= "";
		char	inTPath[MAX_PATH]	= "";

	//	生成し直す
		MAP_LOOP(string, MESH_DATA*, (*mMeshs))
		{
		//	名称データを保持する
			sprintf_s(inName,   sizeof(inName),  (*p).second->name.c_str());
			sprintf_s(inRPath,  sizeof(inRPath), (*p).second->rPath.c_str());
			sprintf_s(inTPath,  sizeof(inTPath), (*p).second->tPath.c_str());
		//	名称データで読みこみし直す
			createMeshInData(inDev, (*p).second, inName, inRPath, inTPath);
		}
	}

	//================================================================================
	//!	メソッド名	CMESH_BASE::meshInvalid
	//
	//	機能		メッシュの無効性を調べる
	//	戻り値		TRUE			無効
	//				FALSE			有効
	//	更新		2007/12/08		<新規>
	//================================================================================
	BOOL
	CMESH_BASE::meshInvalid(void)
	{
		if(	(!bMeshLoaded)	||	//!< メッシュ情報をロードしてない時
			(!md_ptr->mesh))		//!< 又は、メッシュが存在しない時
		{
			return TRUE;				//!< 無効
		}
		else							//!< 上記に当てはまらないなら
		{
			return FALSE;				//!< 有効
		}
	}

	//================================================================================
	//!	メソッド名	CMESH_BASE::drawMesh
	//
	//	機能		メッシュの描画を行う(アルファ演算)
	//	引数		inDev			Direct3Dデバイス
	//				inMatrix		ワールドマトリックス
	//				inCalcDiffuse	増減ディフューズ値
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2007/12/08		<新規>
	//================================================================================
	BOOL
	CMESH_BASE::drawMesh(CONST DEV inDev, MAT16* inMatrix, float inCalcDiffuse)
	{
	//	描画の無効性があるなら、描画しない
		if(	meshInvalid())	return	FALSE;

	//	ディフューズ
		float	fKeepDiffuse	= 0.0f;
	//	ワールド行列を入れる
		inDev->SetTransform(D3DTS_WORLD, inMatrix);
	//	マテリアルの数だけ描画を処理
		for(DWORD i = 0; i < md_ptr->numMaterials; i++)
		{
		//	テクスチャをセット
			inDev->SetTexture(0, md_ptr->texture[i]);
		//	マテリアルをセット
		//	ディフューズの計算
			fKeepDiffuse	= md_ptr->materials[i].Diffuse.a;
			md_ptr->materials[i].Diffuse.a += inCalcDiffuse;
			inDev->SetMaterial(&md_ptr->materials[i]);
		//	メッシュを描画
			md_ptr->mesh->DrawSubset(i);
		//	ディフューズを元に戻す
			md_ptr->materials[i].Diffuse.a  = fKeepDiffuse;
		}

	//	テクスチャのポインターを'NULL'で上書き
		inDev->SetTexture(0, NULL);

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CMESH_BASE::drawMesh
	//
	//	機能		メッシュの描画を行う(ディフューズ設定)
	//	引数		inDev			Direct3Dデバイス
	//				inMatrix		ワールドマトリックス
	//				inDiffuse		増減ディフューズ値
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2008/12/08		<新規>
	//================================================================================
	BOOL
	CMESH_BASE::drawMesh(CONST DEV inDev, MAT16* inMatrix, D3DCOLORVALUE* inDiffuse)
	{
	//	描画の無効性があるなら、描画しない
		if(	meshInvalid())	return	FALSE;

	//	ディフューズ
		D3DCOLORVALUE 	keepDiffuse;
	//	ワールド行列を入れる
		inDev->SetTransform(D3DTS_WORLD, inMatrix);
	//	マテリアルの数だけ描画を処理
		for(DWORD i = 0; i < md_ptr->numMaterials; i++)
		{
		//	テクスチャをセット
			inDev->SetTexture(0, md_ptr->texture[i]);
		//	マテリアルをセット
		//	ディフューズの計算
			keepDiffuse	= md_ptr->materials[i].Diffuse;
			md_ptr->materials[i].Diffuse	= *inDiffuse;
			inDev->SetMaterial(&md_ptr->materials[i]);
		//	メッシュを描画
			md_ptr->mesh->DrawSubset(i);
		//	ディフューズを元に戻す
			md_ptr->materials[i].Diffuse	= keepDiffuse;
		}

	//	テクスチャのポインターを'NULL'で上書き
		inDev->SetTexture(0, NULL);

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	メッシュベースの名前空間
//-----------------------------------------------------------------------