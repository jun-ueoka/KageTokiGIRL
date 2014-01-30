//***********************************************************************************
//!	ファイル名		Class_Mesh.h
//
//	機能			メッシュオブジェクトに関する定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2007/12/08	<新規>
//					2008/06/25	<追加>	全体的にコメントの強化
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_3DObject.h"				//!< 3Dオブジェクト
#include	"Class_MeshBase.h"				//!< メッシュベース

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	名前空間の解放
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	using	namespace	MB;		//!< メッシュベース

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CMESH
	//
	//	機能			Ｘファイル形式のメッシュ
	//	更新			2007/12/08	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CMESH : public C3D_OBJECT, public CMESH_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	FVFの設定
			//==================================================================
		#define	D3DFVF_CLONEVERTEX		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

			//==================================================================
			//	メッシュとメッシュとの当たりを調べる
			//==================================================================
		static	BOOL		enumHitMesh(CMESH* inDefender,
										CMESH* inAttacker,
										LINE*  inLine);

	public:			//-------------------- public --------------------//

				CMESH(CONST DEV   inDev,
					  CONST LPSTR inName,
					  CONST LPSTR inResource = PATH_MESH,
					  CONST	LPSTR inTexture  = PATH_MESHTEX);		//!< コンストラクタ
				CMESH(void);										//!< コンストラクタ(空初期化)
				CMESH(CMESH* inCopy);								//!< コピー
		virtual	~CMESH(void);										//!< デストラクタ

			//==================================================================
			//	access	メッシュ情報
			//==================================================================
				XMESH	getMesh(void)			{ return	md_ptr->mesh;			}

			//==================================================================
			//	ワールド座標系の中心点
			//==================================================================
				VEC3	getWorldCenter(void);

			//==================================================================
			//	ワールド座標系の半径
			//==================================================================
				float	getWorldRadius(void)	{ return	md_ptr->localRadius * getScale();	}

			//==================================================================
			//	対象のＸメッシュとの衝突判定(簡易版)
			//==================================================================
				BOOL	intersectMesh(C3D_OBJECT* inObject, VEC3* inRayLoc, LINE* inLine, float* inDist);

			//==================================================================
			//	全方向、上下、左右、前後で、衝突判定
			//==================================================================
				BOOL	intersect(C3D_OBJECT* inObject, LINE* inLine);
				BOOL	intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown);
				BOOL	intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight);
				BOOL	intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear);

			//==================================================================
			//	全方向、上下、左右、前後に、指定した距離で衝突判定
			//==================================================================
				BOOL	intersect(C3D_OBJECT* inObject, LINE* inLine, float inIntersectValue);
				BOOL	intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown, float inIntersectValue);
				BOOL	intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight, float inIntersectValue);
				BOOL	intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear, float inIntersectValue);

			//==================================================================
			//	全方向、上下、左右、前後に、距離と判定開始位置を指定して、当たり判定
			//==================================================================
				BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE* inLine, float inIntersectValue);
				BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE_UPDOWN inUpDown, float inIntersectValue);
				BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE_LEFTRIGHT inLeftRight, float inIntersectValue);
				BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE_FRONTREAR inFrontRear, float inIntersectValue);

			//==================================================================
			//	コピー生成(サブクラスでオーバーライド)
			//==================================================================
		virtual	void	createCopy(C3D_OBJECT** inCopy, VEC3* inLoc)
				{
					*inCopy	= new CMESH(this);
					(*inCopy)->setLoc(inLoc);
				}

			//==================================================================
			//	描画(サブクラスでオーバーライド可能)
			//==================================================================
		virtual	BOOL	draw(CONST DEV		inDev);
		virtual	BOOL	draw(CZTEX_SHA*		inZTexSha);
		virtual	BOOL	draw(CDEPTH_SHA*	inDepthSha);
	};
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------