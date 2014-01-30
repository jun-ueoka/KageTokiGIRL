//***********************************************************************************
//!	ファイル名		Class_SkinMeshBase.cpp
//
//	機能			スキンメッシュベースに関する実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/08/30	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SkinMeshBase.h"

//-----------------------------------------------------------------------
//!	スキンメッシュベースの名前空間
//-----------------------------------------------------------------------
namespace SMB
{
	//================================================================================
	//	共有スキンマップ
	//================================================================================
	map<string, SKIN_DATA*>*	CSKIN_MESH_BASE::mSkins	= NULL;

	//================================================================================
	//!	メソッド名	MY_HIERARCHY::CreateFrame
	//
	//	機能		フレームの作成
	//	機能概要	新しくフレームを作成する。
	//				フレームとはメッシュ内のオブジェクトを階層ごとに分けたもので、
	//				スキンメッシュの場合はボーンに相当する。
	//	引数		Name			新しく作りたいフレームの名前
	//				ppNewFrame		新しく生成したフレームの格納先へのポインタ
	//	戻り値		HRESULT			各処理のエラー
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	HRESULT
	MY_HIERARCHY::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
	{
		MYFRAME	*pFrame;			//!< ローカルにフレームを作成する為

	//	引数として渡って来た新しいフレームの格納先をNULLにしておく（念のため…）
		*ppNewFrame = NULL;

	//	まずはローカルにフレームを生成
		pFrame = new MYFRAME();

	//	生成できなかったら、メモリー不足を返す
		if(	!pFrame)		return E_OUTOFMEMORY;

	//	続いて生成したフレームに名前を付ける
		pFrame->Name = new TCHAR[lstrlen(Name)+1];					//!< 名前領域を確保

	//	名前領域を確保できなかったら失敗
		if(	!pFrame->Name)	return E_FAIL;

	//	引数として渡って来た新しい名前をチェックし、問題なければ代入する
		AllocateName(Name, &pFrame->Name);

	//	生成したフレームの各要素を初期化する
		D3DXMatrixIdentity(&pFrame->TransformationMatrix);			//!< ワールド変換行列を初期化
		D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);	//!< 最終ワールド変換行列を初期化
		pFrame->pMeshContainer		= NULL;							//!< メッシュコンテナーへのポインタを初期化
		pFrame->pFrameSibling		= NULL;							//!< 兄弟フレームへのポインタを初期化
		pFrame->pFrameFirstChild	= NULL;							//!< 子フレームへのポインタを初期化

	//	新しく作ったフレームを呼び出し元に渡す
		*ppNewFrame	= pFrame;

	//	ここまで来れれば正常終了
		return S_OK;
	}

	//================================================================================
	//!	関数名		AllocateName
	//
	//	機能		文字列の判定
	//	機能概要	（CreateFrameメソッドなどにおいて）異常な文字列はコピーせずに弾く
	//	引数		inName			コピーしたい文字列
	//				ioNewName		コピーする先の文字列
	//	戻り値		HRESULT			各処理のエラー
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	HRESULT
	AllocateName(LPCSTR inName, LPSTR* ioNewName)
	{
	//	コピー元の文字列が異常でないかどうか調べる、異常ではない場合コピーする
		if(inName != NULL)	lstrcpy(*ioNewName, inName);
	//	異常な場合、コピー先にNULLを入れておく
		else				*ioNewName	= NULL;
	//	何もなければ正常終了
		return S_OK;
	}

	//================================================================================
	//!	メソッド名	MY_HIERARCHY::CreateMeshContainer
	//
	//	機能		メッシュコンテナーの作成
	//	機能概要	新たにメッシュコンテナーを作成する。
	//				メッシュコンテナーとはメッシュ情報を格納している構造体のこと
	//	引数		Name				新しく作るメッシュコンテナーの名前
	//				pMeshData			メッシュデータ
	//				pMaterials			マテリアル情報配列
	//				pEffectInstances	エフェクト（シェーダー）インスタンス
	//				NumMaterials		マテリアルの数
	//				pAdjacency			隣接ポリゴンインデックス(あるポリゴンの隣にある三つのポリゴンの番号配列)
	//				pSkinInfo			スキンメッシュのデータ
	//				ppMeshContainer		作成したメッシュコンテナーを渡す領域
	//	戻り値		HRESULT				各処理のエラー
	//	更新		2008/05/30			<追加>	コメント
	//================================================================================
	HRESULT
	MY_HIERARCHY::CreateMeshContainer(LPCSTR						Name,
									  CONST D3DXMESHDATA*			pMeshData,
									  CONST D3DXMATERIAL*			pMaterials,
									  CONST D3DXEFFECTINSTANCE*		pEffectInstances,
									  DWORD							NumMaterials,
									  CONST DWORD					*pAdjacency,
									  LPD3DXSKININFO				pSkinInfo,
									  LPD3DXMESHCONTAINER*			ppMeshContainer) 
	{
		MYMESHCONTAINER		*pMeshContainer	= NULL;	//!< ローカルにメッシュコンテナを生成するための変数
		int					iFacesAmount	= 0;	//!< ポリゴンの数
		LPDIRECT3DDEVICE9	pDevice			= NULL;	//!< Direct3Dデバイス（テクスチャ読み込みなどに必要）
		LPD3DXMESH			pMesh			= NULL;	//!< メッシュインターフェイス（取得用）
		DWORD				dwBoneAmt		= 0;	//!< ボーンの数

	//	引数として渡されてきたメッシュコンテナーの格納先をNULLにしておく（念のため）
		*ppMeshContainer = NULL;

	//	引数からメッシュインターフェイスを取得
		pMesh = pMeshData->pMesh;

	//	まずはローカルに新しくメッシュコンテナーを生成する
		pMeshContainer = new MYMESHCONTAINER();

	//	生成できない場合、メモリー不足
		if(	!pMeshContainer)	return E_OUTOFMEMORY;

	//	生成したメッシュコンテナーを0初期化する
		ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));

	//	初期化したメッシュコンテナーに名前を付ける
		pMeshContainer->Name = new TCHAR[lstrlen(Name) + 1];		//名前領域を確保

	//	名前領域が確保できない場合、失敗
		if(	!pMeshContainer->Name)	return E_FAIL;

	//	引数として渡って来た名前をチェックし、正常なら代入する
		AllocateName(Name, &pMeshContainer->Name);

	//	デバイスを取得（テクスチャ読み込みなどに必要）
		pMesh->GetDevice(&pDevice);

	//	ポリゴン数を取得
		iFacesAmount = pMesh->GetNumFaces();
	   
	//	当該メッシュが法線を持たない場合は法線を追加する
		if(	!(pMesh->GetFVF() && D3DFVF_NORMAL))
		{
		//	通常メッシュだけ扱う
			pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		//	頂点フォーマットコード(FVF)を使用してメッシュのコピーを作成
			if(	FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(),
										   pMesh->GetFVF() | D3DFVF_NORMAL,
										   pDevice,
										   &pMeshContainer->MeshData.pMesh)))
			{
			//	法線の追加に失敗した場合、終了
				return E_FAIL;
			}
			SAFE_RELEASE(pMesh);
			pMesh = pMeshContainer->MeshData.pMesh;
		//	メッシュに含まれる各頂点の法線を計算する
			D3DXComputeNormals(pMesh, NULL);
		}
		else
		{
			pMeshContainer->MeshData.pMesh	= pMesh;
			pMeshContainer->MeshData.Type	= D3DXMESHTYPE_MESH;
		//	必ずメッシュインターフェイスの参照カウンタをひとつ増やす
			pMesh->AddRef();
		}
	 
	//	メッシュのマテリアル設定
		pMeshContainer->NumMaterials = max(1, NumMaterials);								//!< マテリアル数が0の場合、1とする

	//	まずはマテリアル・テクスチャの記憶領域（配列）を確保
		pMeshContainer->pMaterials = new D3DXMATERIAL[NumMaterials];						//!< マテリアル配列確保
		pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];	//!< テクスチャ配列確保
		pMeshContainer->pAdjacency = new DWORD[iFacesAmount*3];								//!< 隣接ポリゴンインデックス

	//	マテリアルか隣接ポリゴンかテクスチャが確保できなかったら失敗
		if(	!(pMeshContainer->pMaterials)	||
			!(pMeshContainer->pAdjacency)	||
			!(pMeshContainer->ppTextures))	return E_FAIL;

	//	隣接ポリゴンインデックスをコピー
		memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount*3);

	//	テクスチャ領域を初期化
		ZeroMemory(pMeshContainer->ppTextures, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	//	マテリアルが存在する場合
		if(NumMaterials > 0)
		{
		//	引数として入って来たマテリアル情報を、メッシュコンテナーにコピーする
			for(UINT i = 0; i < NumMaterials; i++)
			{
			//	四つの属性をコピー
				pMeshContainer->pMaterials[i].MatD3D = pMaterials[i].MatD3D;
			//	元データにテクスチャファイル名が存在する場合、その領域を確保
				if(	pMaterials[i].pTextureFilename)
				{
					pMeshContainer->pMaterials[i].pTextureFilename =
						new TCHAR[strlen(pMaterials[i].pTextureFilename) + 1];
				}
			//	テクスチャファイル名をコピー（もとが存在していない場合は弾かれる）
				AllocateName(pMaterials[i].pTextureFilename, &pMeshContainer->pMaterials[i].pTextureFilename);

			//	アンビエント属性を設定（そのままでは設定されない）
				pMeshContainer->pMaterials[i].MatD3D.Ambient.a = 0.0f;
				pMeshContainer->pMaterials[i].MatD3D.Ambient.r = 0.0f;
				pMeshContainer->pMaterials[i].MatD3D.Ambient.g = 0.0f;
				pMeshContainer->pMaterials[i].MatD3D.Ambient.b = 0.0f;
			}

		//	テクスチャの読み込み
			for(UINT iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
			{
				if(	pMeshContainer->pMaterials[iMaterial].pTextureFilename)
				{
					TCHAR	textureName[MAX_PATH]		= "";
					TCHAR	strTexturePath[MAX_PATH]	= "";
					lstrcpy(textureName,  pMeshContainer->pMaterials[iMaterial].pTextureFilename);
					sprintf_s(strTexturePath, sizeof(strTexturePath), keepTexPath, textureName);

				//	テクスチャの読み込み
					if( FAILED( D3DXCreateTextureFromFile(pDevice,
														  strTexturePath,
														  &pMeshContainer->ppTextures[iMaterial] ) ) )
					{
					//	テクスチャが同じ階層になければ、諦める
						throw textureName;
					}
				}
			}
		}
	//	マテリアルが存在しない（マテリアル数が0だった）場合
		else
		{
		//	マテリアルがひとつだけ存在しているとみなし、設定
			pMeshContainer->pMaterials[0].pTextureFilename	= NULL;						//!< テクスチャ名は無し
			memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));		//!< 0初期化
		//	ディフューズの各色を0.5に設定
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.r	= 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.g	= 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.b	= 0.5f;
		//	スペキュラーとディフューズは同じ値に設定
			pMeshContainer->pMaterials[0].MatD3D.Specular	= pMeshContainer->pMaterials[0].MatD3D.Diffuse;
		}

	//	エフェクトデータの設定
		pMeshContainer->pEffects = new D3DXEFFECTINSTANCE;							//!< メッシュコンテナーにエフェクトの領域を確保

	//	エフェクトファイル名を判定
		if(	pEffectInstances->pEffectFilename)
		{
		//	エフェクトファイル名が存在しているなら、メッシュコンテナーのエフェクト領域にファイル名のための文字配列を確保
			pMeshContainer->pEffects->pEffectFilename = new TCHAR[strlen(pEffectInstances->pEffectFilename) + 1];
		//	名前領域を確保できなかった場合、失敗を返す
			if(	!pMeshContainer->pEffects->pEffectFilename)	return E_FAIL;
		}

	//	名前をメッシュコンテナーのエフェクトにコピー
		AllocateName(pEffectInstances->pEffectFilename, &pMeshContainer->pEffects->pEffectFilename);

	//	デフォルトエフェクトの数をコピー
		pMeshContainer->pEffects->NumDefaults	= pEffectInstances->NumDefaults;

	//	デフォルトエフェクト配列格納領域を確保
		pMeshContainer->pEffects->pDefaults		= new D3DXEFFECTDEFAULT[pEffectInstances->NumDefaults];

		for(UINT i = 0; i < pEffectInstances->NumDefaults; i++)
		{
		//	デフォルトエフェクト内のパラメータ名を示す文字列を、メッシュコンテナー側のエフェクト内にコピーするための領域を確保
			pMeshContainer->pEffects->pDefaults[i].pParamName = new TCHAR[strlen(pEffectInstances->pDefaults[i].pParamName) + 1];

		//	名前領域が確保できなかったら、失敗
			if(!pMeshContainer->pEffects->pDefaults[i].pParamName)	return E_FAIL;

		//	パラメータ名をメッシュコンテナー側のエフェクトにコピー
			AllocateName(pEffectInstances->pDefaults[i].pParamName, &pMeshContainer->pEffects->pDefaults[i].pParamName);

		//	パラメータの大きさをコピー
			pMeshContainer->pEffects->pDefaults[i].NumBytes = pEffectInstances->pDefaults[i].NumBytes;

		//	パラメータの型情報をコピー
			pMeshContainer->pEffects->pDefaults[i].Type = pEffectInstances->pDefaults[i].Type;
			if(	pMeshContainer->pEffects->pDefaults[i].Type < D3DXEDT_DWORD)
				pMeshContainer->pEffects->pDefaults[i].pValue = new DWORD[pEffectInstances->pDefaults[i].NumBytes];
		}

	//	当該メッシュがスキン情報を持っている場合（スキンメッシュ固有の処理）
		if(	pSkinInfo)
		{
			pMeshContainer->pSkinInfo = pSkinInfo;		//!< スキン情報をメッシュコンテナーにコピー
			pSkinInfo->AddRef();						//!< 参照カウンタをひとつ増やす
			dwBoneAmt = pSkinInfo->GetNumBones();		//!< ボーンの数を取得
		//	ボーンの数だけボーンオフセット行列を入れる領域を確保
			pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneAmt];
		//	領域を確保できなかったら失敗
			if(	!pMeshContainer->pBoneOffsetMatrices)	return E_FAIL;
		//	ボーンオフセット行列をコピー
			for(DWORD i= 0; i < dwBoneAmt; i++)
			{
			//	※ボーンオフセット行列とは、頂点のローカル座標をボーン座標に変換する行列
				memcpy(&pMeshContainer->pBoneOffsetMatrices[i],
					pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DMATRIX));	
			}
		//	メッシュを最適化（あるボーンの影響を受ける頂点の番号を通し番号にしておく）
			if(FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
				pMesh, D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
				pMeshContainer->pAdjacency, NULL, NULL, NULL,
				&pMeshContainer->dwWeight,
				&pMeshContainer->dwBoneAmt,
				&pMeshContainer->pBoneBuffer,
				&pMeshContainer->MeshData.pMesh)))
			{
			//	失敗したら終了
				return E_FAIL;
			}
		//	使用したメッシュを解放
			SAFE_RELEASE(pMesh);
		}
	//	ローカルに生成したメッシュコンテナーを呼び出し側に渡す
		*ppMeshContainer = pMeshContainer;
	//	ローカルに取得したデバイスを解放
		SAFE_RELEASE(pDevice);

		return S_OK;
	}

	//================================================================================
	//!	メソッド名	MY_HIERARCHY::DestroyFrame
	//
	//	機能		フレームを破棄する
	//	機能概要	構築したフレームを解放する（再帰関数）
	//	引数		pFrameToFree	解放したいフレーム
	//	戻り値		HRESULT			各処理のエラー
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	HRESULT
	MY_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree)
	{
	//	入力されたフレームが無効だった場合、やめる
		if(	!pFrameToFree)	return E_FAIL;

	//	兄弟フレームが存在しているなら、再びこのメソッドを呼ぶ
		if(	pFrameToFree->pFrameSibling)
		{
			DestroyFrame(pFrameToFree->pFrameSibling);
			pFrameToFree->pFrameSibling = NULL;
		}

	//	子フレームが存在しているなら、再びこのメソッドを呼ぶ
		if(	pFrameToFree->pFrameFirstChild)
		{	
			DestroyFrame(pFrameToFree->pFrameFirstChild);
			pFrameToFree->pFrameFirstChild = NULL;
		}

	//	フレームの名前がNULLでないなら、それを解放
		if(	pFrameToFree->Name)
		{	
			SAFE_DELETE_ARRAY(pFrameToFree->Name);
		}

	//	メッシュコンテナーが存在しているなら、解放
		if(	pFrameToFree->pMeshContainer)
		{
			DestroyMeshContainer(pFrameToFree->pMeshContainer);
		}

	//	ここまで来たら、引数のフレームを解放する
		MYFRAME	*pFrame	= (MYFRAME*)pFrameToFree;
		SAFE_DELETE(pFrame);

		return S_OK; 
	}

	//================================================================================
	//!	メソッド名	MY_HIERARCHY::DestroyFrame
	//
	//	機能		メッシュコンテナーを破棄する
	//	機能概要	生成したメッシュコンテナーを解放する
	//	引数		pMeshContainerBase	解放したいメッシュコンテナー
	//	戻り値		HRESULT				各処理のエラー
	//	更新		2008/05/30			<追加>	コメント
	//================================================================================
	HRESULT
	MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
	{
	//	まずは拡張した部分も参照できるように、拡張メッシュコンテナーにキャストする
		MYMESHCONTAINER* pMeshContainerToFree = (MYMESHCONTAINER*)pMeshContainerBase;

	//	名前を解放
		SAFE_DELETE_ARRAY(pMeshContainerToFree->Name);

	//	メッシュインターフェイスを解放
		SAFE_RELEASE(pMeshContainerToFree->MeshData.pMesh);
		SAFE_RELEASE(pMeshContainerToFree->MeshData.pPatchMesh);
		SAFE_RELEASE(pMeshContainerToFree->MeshData.pPMesh);

	//	マテリアル配列内のテクスチャファイル名を解放
		for(UINT i = 0; i < pMeshContainerToFree->NumMaterials; i++)
		{
			SAFE_DELETE_ARRAY(pMeshContainerToFree->pMaterials[i].pTextureFilename);
		}

	//	マテリアル配列そのものを解放
		SAFE_DELETE_ARRAY(pMeshContainerToFree->pMaterials);

	//	エフェクト情報を解放
		for(UINT i = 0; i < pMeshContainerToFree->pEffects->NumDefaults; i++)
		{
			SAFE_DELETE_ARRAY(pMeshContainerToFree->pEffects->pDefaults[i].pParamName);
			SAFE_DELETE_ARRAY(pMeshContainerToFree->pEffects->pDefaults[i].pValue);
		}

		SAFE_DELETE_ARRAY(pMeshContainerToFree->pEffects->pEffectFilename);
		SAFE_DELETE_ARRAY(pMeshContainerToFree->pEffects->pDefaults);
		SAFE_DELETE(pMeshContainerToFree->pEffects);

	//	隣接ポリゴン配列を解放
		SAFE_DELETE_ARRAY(pMeshContainerToFree->pAdjacency);

	//	スキン情報を解放
		SAFE_RELEASE(pMeshContainerToFree->pSkinInfo);

	//	ここから下は拡張したメッシュコンテナー特有の処理
		if(pMeshContainerToFree->ppTextures)
		{
		//	テクスチャインターフェイスへのポインタ配列を使い、テクスチャインターフェイスを解放
			for(UINT i = 0; i < pMeshContainerToFree->NumMaterials; i++)
			{
				SAFE_RELEASE(pMeshContainerToFree->ppTextures[i]);
			}
			SAFE_DELETE_ARRAY(pMeshContainerToFree->ppTextures);
		}

	//	テクスチャインターフェイスへのポインタ配列そのものを解放
		SAFE_DELETE(pMeshContainerToFree->ppTextures);

	//	ボーンテーブルを解放
		SAFE_RELEASE(pMeshContainerToFree->pBoneBuffer);

	//	全てのボーンのワールド行列配列を解放
		SAFE_DELETE_ARRAY(pMeshContainerToFree->ppBoneMatrix);

	//	ボーンオフセット行列配列を解放
		SAFE_DELETE_ARRAY(pMeshContainerToFree->pBoneOffsetMatrices);

	//	メッシュコンテナーそのものを解放
		SAFE_DELETE(pMeshContainerToFree);
		return S_OK;
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::createSkinInData
	//
	//	機能		Xファイルをロードし、スキンメッシュ情報を作成
	//	機能概要	スキンメッシュ情報をロードし、結果を返す
	//				このメソッドはクラスメソッドである
	//	引数		inDev			Direct3Dデバイス
	//				inSkinData		ロード先スキンメッシュデータ
	//				inName			Xファイルの名称
	//				inResource		スキンメッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	戻り値		TRUE			生成成功
	//				FALSE			生成失敗
	//	更新		2009/03/02		<追加>	コメント
	//================================================================================
	BOOL
	CSKIN_MESH_BASE::createSkinInData(CONST DEV   inDev,
									  SKIN_DATA*  inSkinData,
									  CONST LPSTR inName,
									  CONST LPSTR inResource,
									  CONST LPSTR inTexture)
	{
		MY_HIERARCHY				ah(inTexture);				//!< 読み込みクラス
		LPD3DXANIMATIONCONTROLLER	tempAnimController;			//!< 作業用アニメーションコントローラ

	//	ファイル名を引数から作成（実行ファイルと同じ階層にあるとする）
		TCHAR		realName[MAX_PATH]	= "";					//!< ファイル名にパスを付ける為

	//	スキンメッシュデータを初期解放
		inSkinData->releaseData();

	//	読み込んでみる
	//	D3DXLoadMeshHierarchyFromX(最初のフレーム階層を .x ファイルからロード)
		if(	FAILED(D3DXLoadMeshHierarchyFromX(inName,
											  D3DXMESH_MANAGED,
											  inDev,
											  &ah,
											  NULL,
											  &inSkinData->pFrameRoot,
											  &tempAnimController)))
		{
		//	失敗したら、リソースファイルへのパスをつけてトライ
			sprintf_s(realName, sizeof(realName), inResource, inName);

			if(FAILED(D3DXLoadMeshHierarchyFromX(realName,
												 D3DXMESH_MANAGED,
												 inDev, 
												 &ah,
												 NULL,
												 &inSkinData->pFrameRoot, 
												 &tempAnimController)))
			{
			//	失敗したら、NULLを返す
				goto error;
			}
		}

	//	ボーン初期化
		AllocateAllBoneMatrices(inSkinData, inSkinData->pFrameRoot);

		int	trackNum				= tempAnimController->GetMaxNumTracks();
		int	animNum					= tempAnimController->GetMaxNumAnimationSets();	

	//	アニメーションの数を取得
		inSkinData->iAnimCnt			= animNum;
		inSkinData->pAnimController	= tempAnimController;

	//	アニメーションセットの領域を増やす
		inSkinData->pAnimSet = new LPD3DXANIMATIONSET[inSkinData->iAnimCnt];
	//	アニメーションステップを確保
		inSkinData->iAnimStep= new int[inSkinData->iAnimCnt];

	//	スキンメッシュデータファイルを開く
		FILE*	skinMeshDataFile	= NULL;
		sprintf_s(realName, sizeof(realName), PATH_SKINMESHDATA, inName);
		fopen_s(&skinMeshDataFile, realName, "r");

	//	アニメーショントラックを得る
		for(DWORD i = 0; i < (UINT)inSkinData->iAnimCnt; i++)
		{
			inSkinData->pAnimController->GetAnimationSet(i, &inSkinData->pAnimSet[i]);
		//	アニメーションステップ番号の設定
		//	スキンメッシュデータファイルが開いてあるなら、それから読み込み設定
			if(	skinMeshDataFile)
			{
				fscanf_s(skinMeshDataFile, "%d, ", &inSkinData->iAnimStep[i]);
			}
		//	開かれて無いなら、既存のデータで設定
			else
			{
				inSkinData->iAnimStep[i]	= i;
			}
		}
		if(	skinMeshDataFile){	fclose(skinMeshDataFile);	}

	//	名前をつける
		inSkinData->name	= inName;
		inSkinData->rPath	= inResource;
		inSkinData->tPath	= inTexture;

	//	メッシュとしての情報を生成
		inSkinData->mesh		= new MESH_DATA();

	//	半径を割り出す
		D3DXFrameCalculateBoundingSphere(
			inSkinData->pFrameRoot,
			&inSkinData->mesh->localCenter,
			&inSkinData->mesh->localRadius);

	//	生成したスキンのマップ配列要素番号を返す
		return TRUE;

	//	NULLを返す処理
	error:
		inSkinData->releaseData();	//!< 生成したメッシュ情報を解放
		DX::errorMsgNoFile(inDev, realName);
		return	FALSE;				//!< NULLを返す
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::createSkin
	//
	//	機能		スキンメッシュ情報を生成し、生成したデータを返す
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		スキンメッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	戻り値		スキンメッシュ情報のポインター
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	SKIN_DATA*
	CSKIN_MESH_BASE::createSkin(CONST DEV   inDev,
								CONST LPSTR inName,
								CONST LPSTR inResource,
								CONST LPSTR inTexture)
	{
		SKIN_DATA*	newSkin	= new SKIN_DATA();	//!< 新たなスキンメッシュ情報

	//	スキンメッシュを生成し、失敗したらNULLを返す
		if(	!createSkinInData(inDev, newSkin, inName, inResource, inTexture))
		{
			SAFE_DELETE(newSkin);
			return	NULL;
		}
		return	newSkin;
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::startSkinMeshInitialize
	//
	//	機能		属性の初期化
	//	更新		2008/08/30		<新規>
	//================================================================================
	void
	CSKIN_MESH_BASE::startSkinMeshInitialize(void)
	{
		animSpeed			= kANIM_SPEED;		//!< アニメーションスピード
		animTime			= 0.0f;				//!< アニメーションタイム
		bPlayAnim			= TRUE;				//!< アニメーション実行確認
		prevAnimNumber		= 0;				//!< 前回のアニメーション番号
		currAnimNumber		= -1;				//!< 現在のアニメーション番号
		nextAnimNumber		= 0;				//!< 次回のアニメーション番号
		bAnimChange			= FALSE;			//!< アニメーションチェンジ確認
		spAnimCtrl			= NULL;				//!< 専用のアニメーションコントローラー
		sd_ptr				= NULL;				//!< スキンメッシュ情報
		bSkinLoaded			= FALSE;			//!< スキンメッシュロード確認
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::initializeSkinMesh
	//
	//	機能		スキンメッシュの初期化
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		スキンメッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2007/12/08		<新規>
	//================================================================================
	void
	CSKIN_MESH_BASE::initializeSkinMesh(CONST DEV	inDev,
										CONST LPSTR	inName,
										CONST LPSTR	inResource,
										CONST LPSTR	inTexture)
	{
	//	スキンメッシュの初期解放
		finishalizeSkinMesh();

	//	スキンマップが無ければ生成する
		if(	!mSkins)
		{
			mSkins = new map<string, SKIN_DATA*>;
		}

	//	作業用イテレータ
		map<string, SKIN_DATA*>::iterator	p;
	//	スキンマップにXファイル名を渡し、そのイテレータを受け取る
		p = mSkins->find(inName);

	//	スキンマップに同じ名前のXファイル名が既に登録してある場合
		if(	p != mSkins->end())
		{
		//	使用カウンタを増やす
			p->second->cnt++;
		//	スキンメッシュ情報を取得する
			sd_ptr		= (*p).second;
		//	ロード完了
			bSkinLoaded	= TRUE;
		}
		else
		{
		//	新たにスキンメッシュを生成する
			sd_ptr	= createSkin(inDev, inName, inResource, inTexture);

		//	生成に成功した場合
			if(	sd_ptr)
			{
			//	使用カウンタを増やす
				sd_ptr->cnt++;
			//	ロード完了
				bSkinLoaded	= TRUE;
			//	スキンマップに登録
				mSkins->insert(pair<string, SKIN_DATA*>(sd_ptr->name, sd_ptr));
			}
		//	失敗した場合
			else
			{
			//	ロード未完了
				bSkinLoaded	= FALSE;
			}
		}

	//	スキンメッシュ情報を持っている時
		if(	bSkinLoaded)
		{
		//	専用のアニメーションコントローラーを作成(トラックの数だけ複製)
			sd_ptr->pAnimController->CloneAnimationController(
				sd_ptr->pAnimController->GetMaxNumAnimationOutputs(),
				sd_ptr->pAnimController->GetMaxNumAnimationSets(),
				sd_ptr->iAnimCnt,
				sd_ptr->pAnimController->GetMaxNumEvents(),
				&spAnimCtrl);
		//	アニメコントローラ作動
			spAnimCtrl->ResetTime();
		//	アニメーション再生
			changeAnimationAndBlend(0);
		}
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::finishalizeSkinMesh
	//
	//	機能		スキンメッシュの解放
	//	更新		2007/12/08		<新規>
	//================================================================================
	void
	CSKIN_MESH_BASE::finishalizeSkinMesh(void)
	{
	//	スキンメッシュ情報をロードしてないなら、処理を返す
		if(	!bSkinLoaded)	return;

	//	フラグをもどしておく
		bSkinLoaded	= FALSE;

	//	専用のアニメーションコントローラを解放する
		SAFE_RELEASE(spAnimCtrl);
	//	スキンメッシュカウントを減らす
		sd_ptr->cnt--;
	//	このスキンメッシュがまだ使用されている場合は、データ解放しない
		if(	sd_ptr->cnt > 0)	return;

	//	スキンマップイテレータ
		map<string, SKIN_DATA*>::iterator	p;
	//	スキンマップにXファイル名を渡し、そのイテレータを受け取る
		p = mSkins->find(sd_ptr->name);
	//	スキン情報の解放
		SAFE_DELETE(p->second);
	//	スキンマップから削除
		mSkins->erase(p);

	//	スキンマップにメンバーが無ければ、消去する
		if(	mSkins->begin() == mSkins->end())
		{
			SAFE_DELETE(mSkins);
		}
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::コンストラクタ
	//
	//	機能概要	Xファイルをロードして初期化
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		スキンメッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	CSKIN_MESH_BASE::CSKIN_MESH_BASE(	CONST DEV   inDev,
										CONST LPSTR inName,
										CONST LPSTR inResource,
										CONST LPSTR inTexture)
	{
	//	属性の初期化
		startSkinMeshInitialize();
	//	スーパークラスの初期化
		initializeSkinMesh(inDev, inName, inResource, inTexture);
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::コンストラクタ
	//
	//	機能		空初期化
	//	更新		2007/12/08		<新規>
	//================================================================================
	CSKIN_MESH_BASE::CSKIN_MESH_BASE(void)
	{
	//	属性の初期化
		startSkinMeshInitialize();
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::コンストラクタ
	//
	//	機能概要	コピー
	//	引数		inCopy			コピー元
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	CSKIN_MESH_BASE::CSKIN_MESH_BASE(CSKIN_MESH_BASE* inCopy)
	{
	//	属性の初期化
		startSkinMeshInitialize();
	//	コピー開始
		sd_ptr	= getSkins();		//!< スキンメッシュ情報

	//	スキンメッシュ情報がある時
		if(	sd_ptr)
		{
		//	残りをコピー
			setAnimSpeed		(inCopy->getAnimSpeed());		//!< アニメーションスピード
			setAnimTime			(inCopy->getAnimTime());			//!< アニメーションタイム
			setCurrAnimNumber	(inCopy->getCurrAnimNumber());	//!< 現在のアニメーション番号
			setNextAnimNumber	(inCopy->getNextAnimNumber());	//!< 次回のアニメーション番号
		//	使用カウンタを増やす
			sd_ptr->cnt++;
		//	専用のアニメーションコントローラーを作成(トラックの数だけ複製)
			sd_ptr->pAnimController->CloneAnimationController(
				sd_ptr->pAnimController->GetMaxNumAnimationOutputs(),
				sd_ptr->pAnimController->GetMaxNumAnimationSets(),
				sd_ptr->iAnimCnt,
				sd_ptr->pAnimController->GetMaxNumEvents(),
				&spAnimCtrl);
		//	アニメコントローラ作動
			spAnimCtrl->ResetTime();
		//	ロード完了
			bSkinLoaded	= TRUE;
		//	アニメーション再生
			changeAnimationAndBlend(0);
		}
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::デストラクタ
	//
	//	機能概要	全ての資源を解放し、終了する。
	//				資源が、他のオブジェクトでも使われていた場合は残す。
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	CSKIN_MESH_BASE::~CSKIN_MESH_BASE()
	{
	//	スキンメッシュの解放
		finishalizeSkinMesh();
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::resetInstance
	//
	//	機能		リセット実行
	//	引数		inDev			Direct3Dデバイス
	//	更新		2009/03/02		<新規>
	//================================================================================
	void
	CSKIN_MESH_BASE::resetInstance(CONST DEV inDev)
	{
	//	スキンマップがなければ、リセットしない
		if(	!mSkins)	return;

	//	読み込んだ時の情報を取り出す為
		char	inName[MAX_PATH]	= "";
		char	inRPath[MAX_PATH]	= "";
		char	inTPath[MAX_PATH]	= "";

	//	生成し直す
		MAP_LOOP(string, SKIN_DATA*, (*mSkins))
		{
		//	名称データを保持する
			sprintf_s(inName,   sizeof(inName),  (*p).second->name.c_str());
			sprintf_s(inRPath,  sizeof(inRPath), (*p).second->rPath.c_str());
			sprintf_s(inTPath,  sizeof(inTPath), (*p).second->tPath.c_str());
		//	名称データで読みこみし直す
			createSkinInData(inDev, (*p).second, inName, inRPath, inTPath);
		}
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::skinInvalid
	//
	//	機能		スキンメッシュの無効性を調べる
	//	戻り値		BOOL			TREU	無効
	//								FALSE	有効
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	BOOL
	CSKIN_MESH_BASE::skinInvalid(void)
	{
		if(	(!bSkinLoaded))		//!< スキンメッシュ情報をロードしてない時
		{
			return TRUE;		//!< 無効
		}
		else					//!< 上記に当てはまらないなら
		{
			return FALSE;		//!< 有効
		}
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::animation
	//
	//	機能		アニメーション処理
	//	機能概要	スキンメッシュのアニメーションを行う
	//	戻り値		TRUE			アニメーション成功
	//				FALSE			アニメーション失敗
	//	更新		2009/01/13		<新規>
	//================================================================================
	BOOL
	CSKIN_MESH_BASE::animation(void)
	{
	//	描画の無効性を調べて、無効の時
		if(	skinInvalid())
		{
		//	描画失敗を返す
			return	FALSE;
		}

	//	アニメーションスイッチが入っている時
		if(	bPlayAnim)
		{
		//	値を保持
			float	keepAnimTime	= animTime;
		//	入っているなら時間を進める
			animTime += animSpeed;

		//	アニメーションが終了した時
			if(	animTime > sd_ptr->pAnimSet[currAnimNumber]->GetPeriod())
			{
			//	アニメーションを変更したばかりなら、フラグを戻して終了
			/*	if(	bAnimChange)
				{
					bAnimChange = FALSE;
				}
				else
			*/
			//	現在のアニメーション番号と、次のアニメーションステップ番号が一致した時
				if(	sd_ptr->iAnimStep[currAnimNumber] == currAnimNumber)
				{
					spAnimCtrl->ResetTime();
					animTime = 0.0f;
					spAnimCtrl->AdvanceTime(animSpeed, NULL);
					spAnimCtrl->SetTrackPosition(currAnimNumber, animTime);
				}
			//	次のアニメーションステップ番号が、現在の番号以外の正常なアニメーション番号の時
				else if((sd_ptr->iAnimStep[currAnimNumber] >= 0                     ) &&
						(sd_ptr->iAnimStep[currAnimNumber] <  sd_ptr->iAnimCnt))
				{
					changeAnimationAndBlend(sd_ptr->iAnimStep[currAnimNumber]);
					spAnimCtrl->AdvanceTime(animSpeed, NULL);
				}
			//	上記以外なら、アニメーションをストップ
				else
				{
					animTime	= keepAnimTime;
					spAnimCtrl->SetTrackPosition(currAnimNumber, animTime);
					setPlayAnim(FALSE);
				}
			}
			else
			{
				spAnimCtrl->AdvanceTime(animSpeed, NULL);
				spAnimCtrl->SetTrackPosition(currAnimNumber, animTime);
			}
		}
		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::drawSkin
	//
	//	機能		描画
	//	機能概要	スキンメッシュの描画を行う
	//	引数		inDev			Direct3Dデバイス
	//				inMatrix		ワールドマトリックス
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	BOOL
	CSKIN_MESH_BASE::drawSkin(CONST DEV inDev, MAT16* inMatrix)
	{
	//	描画の無効性を調べて、無効の時
		if(	skinInvalid())
		{
		//	描画失敗を返す
			return	FALSE;
		}

	//	アニメーションスイッチが入っている時
		if(	bPlayAnim)
		{
		//	値を保持
			float	keepAnimTime	= animTime;
		//	入っているなら時間を進める
			animTime += animSpeed;

		//	アニメーションが終了した時
			if(	animTime > sd_ptr->pAnimSet[currAnimNumber]->GetPeriod())
			{
			//	アニメーションを変更したばかりなら、フラグを戻して終了
			/*	if(	bAnimChange)
				{
					bAnimChange = FALSE;
				}
				else
			*/
			//	現在のアニメーション番号と、次のアニメーションステップ番号が一致した時
				if(	sd_ptr->iAnimStep[currAnimNumber] == currAnimNumber)
				{
					spAnimCtrl->ResetTime();
					animTime = 0.0f;
					spAnimCtrl->AdvanceTime(animSpeed, NULL);
					spAnimCtrl->SetTrackPosition(currAnimNumber, animTime);
				}
			//	次のアニメーションステップ番号が、現在の番号以外の正常なアニメーション番号の時
				else if((sd_ptr->iAnimStep[currAnimNumber] >= 0                     ) &&
						(sd_ptr->iAnimStep[currAnimNumber] <  sd_ptr->iAnimCnt))
				{
					changeAnimationAndBlend(sd_ptr->iAnimStep[currAnimNumber]);
					spAnimCtrl->AdvanceTime(animSpeed, NULL);
				}
			//	上記以外なら、アニメーションをストップ
				else
				{
					animTime	= keepAnimTime;
					spAnimCtrl->SetTrackPosition(currAnimNumber, animTime);
					setPlayAnim(FALSE);
				}
			}
			else
			{
				spAnimCtrl->AdvanceTime(animSpeed, NULL);
				spAnimCtrl->SetTrackPosition(currAnimNumber, animTime);
			}
		}
	//	行列更新
		UpdateFrameMatrices(sd_ptr->pFrameRoot, inMatrix);
	//	描画
		DrawFrame(inDev, sd_ptr->pFrameRoot);

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::AllocateBoneMatrix
	//
	//	機能		ボーン行列の初期化
	//	引数		newSkin				スキンメッシュ情報
	//				pMeshContainerBase	メッシュコンテナ
	//	戻り値		HRESULT				各処理のエラー
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	HRESULT
	CSKIN_MESH_BASE::AllocateBoneMatrix(SKIN_DATA* newSkin, LPD3DXMESHCONTAINER pMeshContainerBase)
	{
		MYFRAME*	pFrame	  = NULL;		//!< ボーン情報確認用
		DWORD		dwBoneAmt = 0;			//!< ボーンの数

	//	メッシュコンテナを自作コンテナに入れる
		MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	//	スキンメッシュアニメーションが無いなら終了
		if(	!pMeshContainer->pSkinInfo)	return S_OK;

	//	ボーンの数を取得
		dwBoneAmt = pMeshContainer->pSkinInfo->GetNumBones();

	//	ボーンの数だけ、ボーンワールド行列配列を生成
		pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneAmt];

	//	全てのボーンの検査、及びワールド行列を受け取る
		for (DWORD i = 0; i < dwBoneAmt; i++)
		{
		//	子フレームを検索して受け取る
		//	D3DXFrameFind(ルートフレームの子フレームを検索する)
			pFrame = (MYFRAME*)D3DXFrameFind(newSkin->pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i) );
		//	子フレームを受け取れなかったらエラー
			if(	!pFrame)	return E_FAIL;
		//	受け取った子フレームのワールド行列のポインターを受け取る
			pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;
		}
		return S_OK;
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::AllocateAllBoneMatrices
	//
	//	機能		ボーン行列の初期化(兄弟フレーム、子フレームへ再帰する)
	//	引数		pThing				スキンメッシュ情報
	//				pFrame				スキンメッシュ情報の親フレーム情報
	//	戻り値		HRESULT				各処理のエラー
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	HRESULT
	CSKIN_MESH_BASE::AllocateAllBoneMatrices(SKIN_DATA* newSkin, LPD3DXFRAME pFrame)
	{
	//	フレームにメッシュコンテナが存在する時
		if(	pFrame->pMeshContainer)
		{
		//	ボーン行列の初期化を行う、失敗したらエラー
			if(FAILED(AllocateBoneMatrix(newSkin, pFrame->pMeshContainer)))			return	E_FAIL;
		}
	//	兄弟フレームが存在する時
		if(	pFrame->pFrameSibling)
		{
		//	兄弟フレームの初期化を行う、失敗したらエラー
			if(FAILED(AllocateAllBoneMatrices(newSkin, pFrame->pFrameSibling)))		return E_FAIL;
		}
	//	子フレームが存在する時
		if(	pFrame->pFrameFirstChild)
		{
		//	子フレームの初期化を行い、失敗したらエラー
			if(FAILED(AllocateAllBoneMatrices(newSkin, pFrame->pFrameFirstChild)))	return E_FAIL;
		}
		return S_OK;
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::RenderMeshContainer
	//
	//	機能		フレーム内のそれぞれのメッシュを描画する
	//	引数		inDev				DirectXデバイス
	//				pMeshContainer		描画対象のメッシュコンテナ
	//				pFrame				フレーム情報
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	void
	CSKIN_MESH_BASE::RenderMeshContainer(	CONST DEV			inDev,
											MYMESHCONTAINER*	pMeshContainer,
											MYFRAME*			pFrame)
	{
		DWORD					i, k;				//!< ループカウンタ
		DWORD					dwBlendMatrixAmt;	//!< 重み
		DWORD					dwPrevBoneID;		//!<
		LPD3DXBONECOMBINATION	pBoneCombination;	//!< メッシュサブセット
		UINT					iMatrixIndex;		//!<
		D3DXMATRIX				matStack;			//!< マトリックス一時保管場所

	//	スキンメッシュの場合
		if(	pMeshContainer->pSkinInfo)
		{
		//	属性とボーンの組み合わせが同じメッシュサブセットを受け取る
			pBoneCombination = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneBuffer->GetBufferPointer();
		//
			dwPrevBoneID = UINT_MAX;

		//	全てのボーンを見る
			for(i = 0; i < pMeshContainer->dwBoneAmt; i++)
			{
			//
				dwBlendMatrixAmt = 0;
			//	重みの個数だけ、ボーン配列を見る
				for(k = 0; k < pMeshContainer->dwWeight; k++)
				{
				//	現在見ているボーンの配列の長さが限界で無い時
					if(	pBoneCombination[i].BoneId[k] != UINT_MAX) 
					{
					//	重みの数を更新
						dwBlendMatrixAmt = k;
					}
				}
			//	ジオメトリブレンディングを実行する為に使う行列の数を設定
				inDev->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixAmt);
			//	重みの個数だけ、ボーン配列を見る
				for(k = 0; k < pMeshContainer->dwWeight; k++) 
				{
				//	ボーン配列を受け取る
					iMatrixIndex = pBoneCombination[i].BoneId[k];
				//	ボーンの配列の長さが限界で無い時
					if(	iMatrixIndex != UINT_MAX)
					{
					//	ボーンのワールド行列と、
						matStack = pMeshContainer->pBoneOffsetMatrices[iMatrixIndex] * (*pMeshContainer->ppBoneMatrix[iMatrixIndex]);
						inDev->SetTransform( D3DTS_WORLDMATRIX(k), &matStack );
					}
				}
			//	ディフューズの設定はここで行う
				inDev->SetMaterial(&pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D );
				inDev->SetTexture(0, pMeshContainer->ppTextures[pBoneCombination[i].AttribId] );
				dwPrevBoneID = pBoneCombination[i].AttribId;
				pMeshContainer->MeshData.pMesh->DrawSubset(i);	
			}
		//	レンダリングステートを変更したので、元に戻しておく。これをやらないと一部のPCでバグが出る
			inDev->SetRenderState(D3DRS_VERTEXBLEND, 0);
		}
	//	通常メッシュの場合
		else
		{
			inDev->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);
			for (i = 0; i < pMeshContainer->NumMaterials; i++)
			{
			//	ディフューズの設定はここで行う
				inDev->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D );
				inDev->SetTexture(0, pMeshContainer->ppTextures[i] );
				pMeshContainer->MeshData.pMesh->DrawSubset(i);
			}
		}
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::DrawFrame
	//
	//	機能		フレームを描画する
	//	引数		inDev			DirectXデバイス
	//				pFrameBase		描画するフレーム
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	void
	CSKIN_MESH_BASE::DrawFrame(CONST DEV inDev, LPD3DXFRAME pFrameBase)
	{
	//	作業用ポインターに値を変換
		MYFRAME*			pFrame			= (MYFRAME*)pFrameBase;
		MYMESHCONTAINER*	pMeshContainer	= (MYMESHCONTAINER*)pFrame->pMeshContainer;

	//	メッシュコンテナが存在する
		while(pMeshContainer)
		{
		//	描画
			RenderMeshContainer(inDev, pMeshContainer, pFrame);
		//	メッシュコンテナーを保存して置く
			pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
		}
	//	兄弟フレームが存在する時
		if(	pFrame->pFrameSibling)
		{
		//	兄弟フレームの描画に移る
			DrawFrame(inDev, pFrame->pFrameSibling);
		}
	//	子フレームが存在する時
		if(	pFrame->pFrameFirstChild)
		{
		//	子フレームの描画に移る
			DrawFrame(inDev, pFrame->pFrameFirstChild);
		}
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::UpdateFrameMatrices
	//
	//	機能		フレームのマトリックスを更新
	//	引数		pFrameBase		更新するフレーム
	//				pParentMatrix	更新元のマトリックス
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	void
	CSKIN_MESH_BASE::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
	{
	//	作業用マイフレーム
		MYFRAME *pFrame = (MYFRAME*)pFrameBase;	

	//	更新元のマトリックスが存在する時
		if(	pParentMatrix)
		{
		//	送られてきたマトリックスで更新
			D3DXMatrixMultiply(
				&pFrame->CombinedTransformationMatrix,
				&pFrame->TransformationMatrix,
				pParentMatrix);
		}
	//	更新元のマトリックスが存在しない時
		else
		{
		//	フレームが持つマトリックスで更新
			pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
		}
	//	兄弟フレームが存在する時
		if(	pFrame->pFrameSibling)
		{
		//	兄弟フレームの更新に移る
			UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
		}
	//	子フレームが存在する時
		if(	pFrame->pFrameFirstChild)
		{
		//	子フレームの更新に移る
			UpdateFrameMatrices(
				pFrame->pFrameFirstChild,
				&pFrame->CombinedTransformationMatrix);
		}
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH_BASE::changeAnimationAndBlend
	//
	//	機能		アニメーションの設定
	//	機能概要	指定されたアニメーション番号を使ってアニメーションを変更し
	//				アニメーションをブレンディングする
	//	引数		inAnim			アニメーション番号
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	void
	CSKIN_MESH_BASE::changeAnimationAndBlend(int inAnim)
	{
	//	新たなアニメーションと、現在のアニメーションが一致するなら、処理しない
		if(	currAnimNumber == inAnim)		return;
	//	新たなアニメーション番号が不正なら、処理をしない
		if(	inAnim <  0		||
			inAnim >= sd_ptr->iAnimCnt)	return;
	//	現在のアニメーション番号と前回までのアニメーション番号を保存
		prevAnimNumber = currAnimNumber;

		DWORD dwNewTrack = inAnim;						//!< トラック番号を指定
		LPD3DXANIMATIONCONTROLLER	pAC;				//!< 作業用アニメーションコントローラ
		LPD3DXANIMATIONSET			pAS;				//!< 作業用アニメーションセット

		spAnimCtrl->AddRef();							//!< アニメーションコントローラーの参照カウンタをインクリメントする
		pAC = spAnimCtrl;								//!< 作業用のアニメーションコントローラーで処理をする

		pAC->GetAnimationSet(inAnim, &pAS);				//!< 作業用アニメーションセットを取得
		pAC->SetTrackAnimationSet(dwNewTrack, pAS);		//!< 作業用アニメーションセットを含むようにトラックを設定
		pAS->Release();									//!< 作業用アニメーションセットを開放

		pAC->UnkeyAllTrackEvents(currAnimNumber);		//!< 現在のアニメーションでブレンドを発生
		pAC->UnkeyAllTrackEvents(dwNewTrack);			//!< 次のアニメーションでブレンドを発生

	//	現在使用しているトラックのリセット
	//	指定した時間後にトラックを無効にする
		pAC->KeyTrackEnable	(currAnimNumber, FALSE, animTime + kMOVE_TRANSITION_TIME);
	//	再生スピードのリセット
		pAC->KeyTrackSpeed	(currAnimNumber,  0.0f, animTime, kMOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	//	重みのリセット
		pAC->KeyTrackWeight	(currAnimNumber,  0.0f, animTime, kMOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);

	//	次に使用するトラックのセット
	//	再生スピードのセット
		pAC->KeyTrackSpeed	(dwNewTrack, 1.0f, animTime, kMOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	//	重みのセット
		pAC->KeyTrackWeight	(dwNewTrack, 1.0f, animTime, kMOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	//	トラックを有効にする
		pAC->SetTrackEnable	(dwNewTrack, TRUE);

		currAnimNumber = dwNewTrack;					//!< 次のトラックを現在のトラックにする
		pAC->Release();									//!< 作業用アニメーションコントローラを解放

		spAnimCtrl->ResetTime();						//!< アニメーションをリセット
		animTime		= 0.0f;							//!< アニメーション時間を初期化
		setPlayAnim(TRUE);								//!< アニメーションを再生
		bAnimChange	= TRUE;								//!< アニメーションをブレンドしたて
	}
}
//-----------------------------------------------------------------------
//!	スキンメッシュベースの名前空間
//-----------------------------------------------------------------------