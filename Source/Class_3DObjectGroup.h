//***********************************************************************************
//!	ファイル名		Class_3DObjectGroup.h
//
//	機能			三次元の物体を扱うグループの定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2007/12/08	<新規>
//					2008/07/09	<追加>	全体的にコメントの強化
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_3DObject.h"

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		C3D_OBJECT_GROUP
	//
	//	機能			三次元の物体を扱うグループ
	//	機能概要		解放は各自で行う事
	//	更新			2008/07/09	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class C3D_OBJECT_GROUP
	{
	private:		//-------------------- protected --------------------//

				vector<C3D_OBJECT*>		objects;				//!< オブジェクトグループ
				C3D_OBJECT*				prevHitObject;			//!< 直前の衝突オブジェクト

	public:			//-------------------- public --------------------//

				C3D_OBJECT_GROUP(void);							//!< コンストラクタ
				~C3D_OBJECT_GROUP(void);						//!< デストラクタ

			//==================================================================
			//	グループ操作
			//==================================================================
				void	add(C3D_OBJECT* inObject);				//!< グループにメンバーを追加
				void	addG(C3D_OBJECT_GROUP* inGroup);		//!< グループにグループのメンバーを追加

				void	draw(CONST SPRITE inSprite);			//!< グループのスプライト描画
				void	draw(CONST DEV inDev);					//!< グループのメンバーを全描画
			//	グループのメンバーを見える許容半径範囲内で描画
				void	draw(CONST DEV inDev, VEC3* inViewPoint, float inLength);
			//	グループのメンバーを見える許容立方体範囲内で描画
				void	draw(CONST DEV inDev, VEC3* inViewPoint, VEC3* inRange);

				void	draw(CZTEX_SHA*  inZTexSha);			//!< Zテクスチャシェーダ描画
				void	draw(CDEPTH_SHA* inDepthSha);			//!< 深度バッファシェーダ描画

				void	erase(C3D_OBJECT* eraceObject);			//!< グループからメンバーを削除

			//==================================================================
			//	領域マップ操作
			//==================================================================
			//	void	addMap(C3D_OBJECT* inObject);			//!< 領域マップ
			////	領域マップのメンバーを判定領域で判定しながら描画
			//	void	drawMap(CONST DEV inDev, VEC3* inViewPoint, float inLength);
			//	void	eraseMap(C3D_OBJECT* eraceObject);		//!< 領域マップからメンバーを削除

			//==================================================================
			//	グループからメンバーを削除
			//==================================================================
				vector<C3D_OBJECT*>::iterator	erase(vector<C3D_OBJECT*>::iterator p)
				{
					return	objects.erase(p);
				}

				vector<C3D_OBJECT*>::iterator	getBegin(void)	{ return	objects.begin();	}
				vector<C3D_OBJECT*>::iterator	getEnd(void)	{ return	objects.end();		}

			//==================================================================
			//	access	グループ
			//==================================================================
				vector<C3D_OBJECT*>*	getObjects(void)		{ return	&objects;			}

			//==================================================================
			//	ソート & ソート描画
			//==================================================================
				void	sortByCameraPosition(C3D_OBJECT* camera, BOOL inAngle = FALSE);
				void	drawWhitSortByCameraPosition(CONST DEV inDev, C3D_OBJECT* camera, BOOL inAngle = FALSE);

			//==================================================================
			//	access	直前の衝突オブジェクト
			//==================================================================
				C3D_OBJECT*	getPrevHitObject(void)const			{ return	prevHitObject;		}

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
			//	グループのメンバーを全消去
			//==================================================================
				void	clear(void);

			//==================================================================
			//	グループのメンバーを全解放、全消去
			//==================================================================
				void	releaseClear(void)
				{
					SAFE_VECTOR_DELETE(objects, C3D_OBJECT*);
					clear();
				}
	};
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------