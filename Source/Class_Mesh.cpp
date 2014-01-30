//***********************************************************************************
//!	ファイル名		Class_Mesh.cpp
//
//	機能			メッシュオブジェクトに関する実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2007/12/08	<新規>
//					2008/06/26	<追加>	全体的にコメントの強化
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_Mesh.h"

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CMESH::enumHitMesh
	//
	//	機能		衝突判定を行う 'コールバックのためのもの'
	//	機能概要	enumerateメソッドの引数として渡されるとオブジェクトの数だけ呼ばれる。
	//				第一引数にその列挙されるオブジェクトが渡される。
	//				メッシュ（攻撃される）とレイ（攻撃する）の衝突判定は
	//				メッシュのオブジェクト座標系で行われる。
	//				メッシュの逆行列を使用してレイの開始位置を求める。
	//	引数		inDefender		攻撃されるオブジェクト
	//				inAttacker		攻撃するオブジェクト
	//				inRoute			正(1.0)、負(-1.0)
	//	戻り値		TRUE			衝突している
	//				FALSE			衝突していない
	//	更新		2007/12/08		<新規>
	//================================================================================
	BOOL
	CMESH::enumHitMesh(CMESH* inDefender, CMESH* inAttacker, LINE* inLine)
	{
		return	inAttacker->intersect(inDefender, inLine);
	}

	//================================================================================
	//!	メソッド名	CMESH::コンストラクタ
	//
	//	機能		Xファイルをロードして初期化
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		メッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2007/12/08		<新規>
	//================================================================================
	CMESH::CMESH(CONST DEV   inDev,
				 CONST LPSTR inName,
				 CONST LPSTR inResource,
				 CONST LPSTR inTexture)		:
	CMESH_BASE(	inDev, inName, inResource, inTexture)
	{
	//	スーパークラスの初期化
		C3D_OBJECT::initialize();
	//	もしメッシュの読み込み失敗していたら、存在確認をOFFにする
		if(	!bMeshLoaded)	setVisible(FALSE);
	}

	//================================================================================
	//!	メソッド名	CMESH::コンストラクタ
	//
	//	機能		空初期化
	//	更新		2007/12/08		<新規>
	//================================================================================
	CMESH::CMESH(void)		:
	CMESH_BASE()
	{
	//	スーパークラスの初期化
		C3D_OBJECT::initialize();
	}

	//================================================================================
	//!	メソッド名	CMESH::コンストラクタ
	//
	//	機能		コピー
	//	引数		inCopy			コピー元
	//	更新		2007/12/08		<新規>
	//================================================================================
	CMESH::CMESH(CMESH* inCopy)		:
	CMESH_BASE(inCopy)
	{
	//	スーパークラスの初期化
		C3D_OBJECT::initialize();
	//	もしメッシュの読み込み失敗していたら、存在確認をOFFにする
		if(	!bMeshLoaded)	setVisible(FALSE);
	}

	//================================================================================
	//!	メソッド名	CMESH::デストラクタ
	//
	//	機能		全ての資源を解放し、終了する。
	//				資源が、他のオブジェクトでも使われていた場合は残す。
	//	更新		2007/12/08		<新規>
	//================================================================================
	CMESH::~CMESH()
	{
	//	スーパークラスの開放
		C3D_OBJECT::finalize();
	}

	//================================================================================
	//!	メソッド名	CMESH::getWorldCenter
	//
	//	機能		ワールド座標系中心点を受け取る
	//	戻り値		ワールド座標系中心点
	//	更新		2007/12/08		<新規>
	//================================================================================
	VEC3
	CMESH::getWorldCenter(void)
	{
	//	ワールド座標系中心点
		VEC3	aCenter;
	//	ワールド座標系の演算
		D3DXVec3TransformCoord(&aCenter, &md_ptr->localCenter, getWorldMatrix());
	//	ワールド座標系中心点を返す
		return	aCenter;
	}

	//================================================================================
	//!	メソッド名	CMESH::intersectMesh
	//
	//	機能		自身とXメッシュとの衝突を検出する
	//	引数		*inObject		衝突対象物
	//				*inRayLoc		レイの始点
	//				*inLine			自身の進行方向
	//				*inDist			レイの始点から、衝突地点までの距離
	//	戻り値		TRUE			衝突している
	//				FALSE			衝突していない
	//	更新		2008/08/24		<新規>
	//================================================================================
	BOOL
	CMESH::intersectMesh(C3D_OBJECT* inObject, VEC3* inRayLoc, LINE* inLine, float* inDist)
	{
		BOOL	bHit			= FALSE;					//!< 戻り値
		DWORD	dwFaceIndex		= 0;						//!< レイの始点に最も近い面のインデックス
		FLOAT	fU				= 0.0f;						//!< 重心ヒット座標U
		FLOAT	fV				= 0.0f;						//!< 重心ヒット座標V
		VEC3	rayDirection	= inObject->calcRay(inLine);//!< レイの方向

	//	攻撃するレイの位置を攻撃される座標系に合わせる
		rayFromInversedMatrix(inRayLoc, &rayDirection);

	//	レイの方向は必ず正規化する
		D3DXVec3Normalize(&rayDirection, &rayDirection);	//!< 正規化(0.0 or 1.0に収束する)

	//	メッシュとレイとの衝突判定
		D3DXIntersect(
			md_ptr->mesh,									//!< 衝突対象物
			inRayLoc,										//!< レイの始点座標
			&rayDirection,									//!< レイの方向
			&bHit,											//!< 衝突判定結果格納先
			&dwFaceIndex,									//!< レイの始点に最も近い面のインデックス格納先
			&fU,											//!< 重心ヒット座標U格納先
			&fV,											//!< 重心ヒット座標V格納先
			inDist,											//!< レイの始点から面の交点までの距離格納先
			NULL,											//!< 衝突判定結果の構造体格納先
			NULL);											//!< 配列内のエントリ数格納先

		return	bHit;
	}

	//================================================================================
	//!	メソッド名	CMESH::intersect（全方向版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject		衝突対象物
	//				inLine			自身の進行方向
	//	戻り値		TRUE			衝突している
	//				FALSE			衝突していない
	//	更新		2007/12/08		<新規>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE* inLine)
	{
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!intersectRadius(inObject))	return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離

	//	メッシュとの衝突判定し、衝突した時
		if(	intersectMesh(inObject, &inObject->getLoc(), inLine, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inObject->getMoveQuantity())
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CMESH::intersect（上下版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject		衝突対象物
	//				inUpDown		自身の上下進行方向
	//	戻り値		TRUE			衝突している
	//				FALSE			衝突していない
	//	更新		2008/08/24		<新規>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown)
	{
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!intersectRadius(inObject))	return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離
		LINE	line	= LINE(inUpDown, eLR_NO, eFR_NO);		//!< 上下判別専用の方向構造体を作る

	//	メッシュとの衝突判定し、衝突した時
		if(	intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inObject->getMoveQuantity())
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CMESH::intersect（左右版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject		衝突対象物
	//				inLeftRight		自身の左右進行方向
	//	戻り値		TRUE			衝突している
	//				FALSE			衝突していない
	//	更新		2008/08/24		<新規>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight)
	{
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!intersectRadius(inObject))	return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離
		LINE	line	= LINE(eUD_NO, inLeftRight, eFR_NO);	//!< 左右判別専用の方向構造体を作る

	//	メッシュとの衝突判定し、衝突した時
		if(	intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inObject->getMoveQuantity())
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CMESH::intersect（前後版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject		衝突対象物
	//				inFrontRear		自身の前後進行方向
	//	戻り値		TRUE			衝突している
	//				FALSE			衝突していない
	//	更新		2008/08/24		<新規>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear)
	{
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!intersectRadius(inObject))	return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離
		LINE	line	= LINE(eUD_NO, eLR_NO, inFrontRear);	//!< 前後判別専用の方向構造体を作る

	//	メッシュとの衝突判定し、衝突した時
		if(	intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inObject->getMoveQuantity())
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CMESH::intersect（距離指定、全方向版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject			衝突対象物
	//				inLine				自身の進行方向
	//				inIntersectValue	判定する距離
	//	戻り値		TRUE				衝突している
	//				FALSE				衝突していない
	//	更新		2008/08/24			<新規>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE* inLine, float inIntersectValue)
	{
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!intersectRadius(inObject))	return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離

	//	メッシュとの衝突判定し、衝突した時
		if(	intersectMesh(inObject, &inObject->getLoc(), inLine, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inIntersectValue)
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CMESH::intersect（距離指定、上下版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject			衝突対象物
	//				inUpDown			自身の上下進行方向
	//				inIntersectValue	判定する距離
	//	戻り値		TRUE				衝突している
	//				FALSE				衝突していない
	//	更新		2008/08/24			<新規>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown, float inIntersectValue)
	{
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!intersectRadius(inObject))	return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離
		LINE	line	= LINE(inUpDown, eLR_NO, eFR_NO);		//!< 上下判別専用の方向構造体を作る

	//	メッシュとの衝突判定し、衝突した時
		if(	intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inIntersectValue)
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CMESH::intersect（距離指定、左右版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject			衝突対象物
	//				inLeftRight			自身の左右進行方向
	//				inIntersectValue	判定する距離
	//	戻り値		TRUE				衝突している
	//				FALSE				衝突していない
	//	更新		2008/08/24			<新規>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight, float inIntersectValue)
	{
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!intersectRadius(inObject))	return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離
		LINE	line	= LINE(eUD_NO, inLeftRight, eFR_NO);	//!< 左右判別専用の方向構造体を作る

	//	メッシュとの衝突判定し、衝突した時
		if(	intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inIntersectValue)
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CMESH::intersect（距離指定、前後版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject			衝突対象物
	//				inFrontRear			自身の前後進行方向
	//				inIntersectValue	判定する距離
	//	戻り値		TRUE				衝突している
	//				FALSE				衝突していない
	//	更新		2008/08/24			<新規>
	//================================================================================
	BOOL
	CMESH::intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear, float inIntersectValue)
	{
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!intersectRadius(inObject))	return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離
		LINE	line	= LINE(eUD_NO, eLR_NO, inFrontRear);	//!< 前後判別専用の方向構造体を作る

	//	メッシュとの衝突判定し、衝突した時
		if(	intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inIntersectValue)
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CMESH::intersectPos（判定開始位置指定、距離指定、全方向版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject			衝突対象物
	//				inLoc				判定開始位置
	//				inLine				自身の進行方向
	//				inIntersectValue	判定する距離
	//	戻り値		TRUE				衝突している
	//				FALSE				衝突していない
	//	更新		2008/08/24			<新規>
	//================================================================================
	BOOL
	CMESH::intersectPos(C3D_OBJECT*		inObject,
						VEC3*			inLoc,
						LINE*			inLine,
						float			inIntersectValue)
	{
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!intersectRadius(inObject))	return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離

	//	メッシュとの衝突判定し、衝突した時
		if(	intersectMesh(inObject, inLoc, inLine, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inIntersectValue)
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CMESH::intersectPos（判定開始位置指定、距離指定、上下版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject			衝突対象物
	//				inLoc				判定開始位置
	//				inUpDown			自身の上下進行方向
	//				inIntersectValue	判定する距離
	//	戻り値		TRUE				衝突している
	//				FALSE				衝突していない
	//	更新		2008/08/24			<新規>
	//================================================================================
	BOOL
	CMESH::intersectPos(C3D_OBJECT*		inObject,
						VEC3*			inLoc,
						LINE_UPDOWN		inUpDown,
						float			inIntersectValue)
	{
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!intersectRadius(inObject))	return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離
		LINE	line	= LINE(inUpDown, eLR_NO, eFR_NO);		//!< 上下判別専用の方向構造体を作る

	//	メッシュとの衝突判定し、衝突した時
		if(	intersectMesh(inObject, inLoc, &line, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inIntersectValue)
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CMESH::intersectPos（判定開始位置指定、距離指定、左右版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject			衝突対象物
	//				inLoc				判定開始位置
	//				inLeftRight			自身の左右進行方向
	//				inIntersectValue	判定する距離
	//	戻り値		TRUE				衝突している
	//				FALSE				衝突していない
	//	更新		2008/08/24			<新規>
	//================================================================================
	BOOL
	CMESH::intersectPos(C3D_OBJECT*		inObject,
						VEC3*			inLoc,
						LINE_LEFTRIGHT	inLeftRight,
						float			inIntersectValue)
	{
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!intersectRadius(inObject))	return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離
		LINE	line	= LINE(eUD_NO, inLeftRight, eFR_NO);	//!< 左右判別専用の方向構造体を作る

	//	メッシュとの衝突判定し、衝突した時
		if(	intersectMesh(inObject, inLoc, &line, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inIntersectValue)
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CMESH::intersectPos（判定開始位置指定、距離指定、前後版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject			衝突対象物
	//				inLoc				判定開始位置
	//				inFrontRear			自身の前後進行方向
	//				inIntersectValue	判定する距離
	//	戻り値		TRUE				衝突している
	//				FALSE				衝突していない
	//	更新		2008/08/24			<新規>
	//================================================================================
	BOOL
	CMESH::intersectPos(C3D_OBJECT*		inObject,
						VEC3*			inLoc,
						LINE_FRONTREAR	inFrontRear,
						float			inIntersectValue)
	{
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!intersectRadius(inObject))	return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離
		LINE	line	= LINE(eUD_NO, eLR_NO, inFrontRear);	//!< 前後判別専用の方向構造体を作る

	//	メッシュとの衝突判定し、衝突した時
		if(	intersectMesh(inObject, inLoc, &line, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inIntersectValue)
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CMESH::draw
	//
	//	機能		メッシュの描画を行う
	//	引数		inDev			Direct3Dデバイス
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2007/12/08		<新規>
	//================================================================================
	BOOL
	CMESH::draw(CONST DEV inDev)
	{
	//	存在しない状態なら、描画しない
		if(	!getVisible())	return FALSE;

	//	モデルを前向きにする
		MAT16	matrix = (*getWorldMatrixInversed()) * (*getWorldMatrix());

	//	メッシュの描画
		return	drawMesh(inDev, &matrix, getDiffuseAlpha());
	}

	//================================================================================
	//!	メソッド名	CMESH::draw
	//
	//	機能		メッシュの描画を行う(Zテクスチャシェーダ使用)
	//	引数		inZTexSha		Zテクスチャシェーダ
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2008/12/14		<新規>
	//================================================================================
	BOOL
	CMESH::draw(CZTEX_SHA* inZTexSha)
	{
	//	モデルを前向きにする
		MAT16	matrix = (*getWorldMatrixInversed()) * (*getWorldMatrix());

	//	ワールド行列を入れる
		inZTexSha->setWorldMatrix(&matrix);
	//	マテリアルの数だけ描画を処理
		for(DWORD i = 0; i < md_ptr->numMaterials; i++)
		{
			inZTexSha->beginPass();
			md_ptr->mesh->DrawSubset(i);
			inZTexSha->endPass();
		}

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CMESH::draw
	//
	//	機能		メッシュの描画を行う(深度バッファシェーダ使用)
	//	引数		inDepthSha		深度バッファシェーダ
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2008/12/14		<新規>
	//================================================================================
	BOOL
	CMESH::draw(CDEPTH_SHA* inDepthSha)
	{
	//	モデルを前向きにする
		MAT16	matrix = (*getWorldMatrixInversed()) * (*getWorldMatrix());
	//	ワールド行列を入れる
		inDepthSha->setWorldMatrix(&matrix);
	//	デバイスを扱いやすくする
		DEV	inDev	= inDepthSha->getDevice();
	//	パラメータ更新
		inDepthSha->setParamToEffect();
	//	マテリアルの数だけ描画を処理
		for(DWORD i = 0; i < md_ptr->numMaterials; i++)
		{
		//	テクスチャをセット
			inDepthSha->setEffectTexture(md_ptr->texture[i]);
		//	マテリアルをセット
			inDepthSha->setMaterialColor(&md_ptr->materials[i].Diffuse);
			inDepthSha->beginPass();
			md_ptr->mesh->DrawSubset(i);
			inDepthSha->endPass();
		}

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//----------------------------------fcccccc-------------------------------------