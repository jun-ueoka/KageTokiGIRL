//***********************************************************************************
//!	ファイル名		Class_3DObjectGroup.cpp
//
//	機能			三次元の物体を扱うグループの実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2007/12/08	<新規>
//					2008/07/09	<追加>	全体的にコメントの強化
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

#include	<algorithm>
#include	<functional>
//#include	"Class_3DObjectGroup.h"			//!< 3Dオブジェクトグループ

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::C3D_OBJECT_GROUP
	//
	//	機能		コンストラクタ
	//	更新		2007/12/08			<新規>
	//================================================================================
	C3D_OBJECT_GROUP::C3D_OBJECT_GROUP(void)
	{
	//	初期化
		prevHitObject	= NULL;		//!< 直前の衝突オブジェクト
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::~C3D_OBJECT_GROUP
	//
	//	機能		デストラクタ
	//	更新		2007/12/08			<新規>
	//================================================================================
	C3D_OBJECT_GROUP::~C3D_OBJECT_GROUP(void)
	{
	//	全消去
		clear();
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::add
	//
	//	機能		グループにメンバーを追加
	//	引数		inObject			加えるメンバー
	//	更新		2007/12/08			<新規>
	//================================================================================
	void
	C3D_OBJECT_GROUP::add(C3D_OBJECT* inObject)
	{
	//	NULLなら処理をしない
		if(	inObject == NULL)	return;

	//	作業用イテレータ
		vector<C3D_OBJECT*>::iterator	p;

	//	すでにメンバー加わっているなら、処理を返す
		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
			if(	(*p) == inObject)	return;
		}

	//	ここまで来たら、メンバーに加える
		objects.push_back(inObject);
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::addG
	//
	//	機能		グループにグループのメンバーを追加
	//	引数		inGroup				加えるグループ
	//	更新		2007/12/08			<新規>
	//================================================================================
	void
	C3D_OBJECT_GROUP::addG(C3D_OBJECT_GROUP* inGroup)
	{

	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::draw
	//
	//	機能		グループのメンバーを全描画()
	//	引数		inSprite			スプライトデバイス
	//	更新		2007/12/08			<新規>
	//================================================================================
	void
	C3D_OBJECT_GROUP::draw(CONST SPRITE inSprite)
	{
	//	作業用イテレータ
		vector<C3D_OBJECT*>::iterator	p;

	//	メンバーから探し出して、削除する
		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
			(*p)->draw(inSprite);
		}
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::draw
	//
	//	機能		グループのメンバーを全描画
	//	引数		inDev				Direct3Dデバイス
	//	更新		2007/12/08			<新規>
	//================================================================================
	void
	C3D_OBJECT_GROUP::draw(CONST DEV inDev)
	{
	//	作業用イテレータ
		vector<C3D_OBJECT*>::iterator	p;

	//	メンバーから探し出して、削除する
		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
			(*p)->draw(inDev);
		}
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::draw
	//
	//	機能		グループのメンバーを見える範囲で描画
	//	引数		inDev				Direct3Dデバイス
	//				inViewPoint			視点
	//				inLength			視点からの有効範囲
	//	更新		2008/09/26			<新規>
	//================================================================================
	void
	C3D_OBJECT_GROUP::draw(CONST DEV inDev, VEC3* inViewPoint, float inLength)
	{
	//	作業用イテレータ
		vector<C3D_OBJECT*>::iterator	p;

	//	メンバーから探し出して、削除する
		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
			if(	(D3DXVec3Length(&(*inViewPoint - (*p)->getLoc()))) < inLength)
			{
				(*p)->draw(inDev);
			}
		}
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::draw
	//
	//	機能		グループのメンバーをZテクスチャシェーダで描画
	//	引数		inZTexSha			Zテクスチャシェーダ
	//	更新		2009/02/05			<新規>
	//================================================================================
	void
	C3D_OBJECT_GROUP::draw(CZTEX_SHA* inZTexSha)
	{
	//	作業用イテレータ
		vector<C3D_OBJECT*>::iterator	p;

		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
			(*p)->draw(inZTexSha);
		}
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::draw
	//
	//	機能		グループのメンバーを深度バッファシェーダで描画
	//	引数		inDepthSha			深度バッファシェーダ
	//	更新		2009/02/05			<新規>
	//================================================================================
	void
	C3D_OBJECT_GROUP::draw(CDEPTH_SHA* inDepthSha)
	{
	//	作業用イテレータ
		vector<C3D_OBJECT*>::iterator	p;

		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
			(*p)->draw(inDepthSha);
		}
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::erase
	//
	//	機能		グループからメンバーを削除
	//	引数		eraceObject			削除するメンバー
	//	更新		2007/12/08			<新規>
	//================================================================================
	void
	C3D_OBJECT_GROUP::erase(C3D_OBJECT *eraceObject)
	{
	//	NULLなら処理をしない
		if(	eraceObject == NULL)	return;

	//	作業用イテレータ
		vector<C3D_OBJECT*>::iterator	p;

	//	メンバーから探し出して、削除する
		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
			if(	(*p) == eraceObject)
			{
				objects.erase(p);
				return;
			}
		}
	}

	//================================================================================
	//!	関数名		UDgreater
	//
	//	機能		複数のオブジェクトを並べ替える
	//	引数		inObject1			オブジェクト1
	//				inObject2			オブジェクト2
	//	戻り値		true				1より2の方が近い
	//				false				1より2の方が遠い
	//	更新		2007/12/08			<新規>
	//================================================================================
	bool
	UDgreater(C3D_OBJECT *inObject1, C3D_OBJECT *inObject2)
	{
		float	distance = inObject1->getOffsetFromCamera() - inObject2->getOffsetFromCamera();
		return (distance > 0.0f);
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::sortByCameraPosition
	//
	//	機能		カメラ位置を元に、オブジェクトメンバーを並び替え
	//	引数		camera				カメラオブジェクト
	//				inAngle				角度カメラ方向修正確認
	//	更新		2007/12/08			<新規>
	//================================================================================
	void
	C3D_OBJECT_GROUP::sortByCameraPosition(C3D_OBJECT* camera, BOOL inAngle)
	{
	//	作業用イテレータ
		vector<C3D_OBJECT*>::iterator	p;

	//	オブジェクトにカメラとの距離を設定する
		for(p  = objects.begin();
			p != objects.end();
			p++)
		{
		//	角度を修正する
			if(	inAngle)	(*p)->angleToObject(camera);

		//	カメラまでの距離を計算
			(*p)->setOffsetFromCamera(
				D3DXVec3Length(&VEC3((*p)->getLoc() - camera->getLoc())));
		}

	//	オブジェクトメンバーをソート
		sort(objects.begin(), objects.end(), UDgreater);
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::drawWhitSortByCameraPosition
	//
	//	機能		並び替えと描画を両方実行する
	//	引数		inDev				Direct3Dデバイス
	//				camera				カメラオブジェクト
	//				inAngle				角度カメラ方向修正確認
	//	更新		2007/12/08			<新規>
	//================================================================================
	void
	C3D_OBJECT_GROUP::drawWhitSortByCameraPosition(CONST DEV inDev, C3D_OBJECT *camera, BOOL inAngle)
	{
		sortByCameraPosition(camera, inAngle);		//!< 並び替え
		draw(inDev);								//!< 描画
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::intersect（全方向版）
	//
	//	機能		全方向でグループに対して、衝突判定
	//	引数		inObject			衝突物
	//				inLine				判定方向
	//	更新		2008/08/20			<新規>
	//================================================================================
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE* inLine)
	{
	//	衝突物の存在確認が無ければ、衝突して無い事を返して終了
		if(	!inObject->getVisible())	return	FALSE;

		float	inDist	= 10000.0f;			//!< 距離を確保しておく
		BOOL	inHit	= FALSE;			//!< 衝突結果

	//	メッシュとの距離を初期化しておく
		inObject->setDist(inDist);

	//	作業用イテレータ
		vector<C3D_OBJECT*>::iterator	pi;

	//	オブジェクトにカメラとの距離を設定する
		for(pi  = objects.begin();
			pi != objects.end();
			pi++)
		{
		//	同類のオブジェクトとは当たり判定をしない
			if(	inObject != (C3D_OBJECT*)*pi)
			{
				if(	(*pi)->intersect(inObject, inLine))
				{
				//	前回のオブジェクトより、衝突距離が近い時
					if(	inDist	> inObject->getDist())
					{
						inHit				= TRUE;
					//	直前の衝突オブジェクトを登録
						prevHitObject	= (*pi);
					//	距離をとっておく
						inDist				= inObject->getDist();
					}
				}
			}
		}

	//	衝突していたら、最短距離を入れておく
		if(	inHit)	inObject->setDist(inDist);

		return inHit;
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::intersect（上下、左右、前後版）
	//
	//	機能		全方向版の衝突判定につなげる
	//	更新		2008/08/20			<新規>
	//================================================================================
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown)
	{
		return	intersect(inObject, &LINE(inUpDown, eLR_NO, eFR_NO));
	}
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight)
	{
		return	intersect(inObject, &LINE(eUD_NO, inLeftRight, eFR_NO));
	}
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear)
	{
		return	intersect(inObject, &LINE(eUD_NO, eLR_NO, inFrontRear));
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::intersect（距離指定、全方向版）
	//
	//	機能		全方向でグループに対して、衝突判定
	//	引数		inObject			衝突物
	//				inLine				判定方向
	//				inIntersectValue	判定する距離
	//	更新		2008/08/20			<新規>
	//================================================================================
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE* inLine, float inIntersectValue)
	{
	//	衝突物の存在確認が無ければ、衝突して無い事を返して終了
		if(	!inObject->getVisible())	return	FALSE;

		float	inDist	= 10000.0f;			//!< 距離を確保しておく
		BOOL	inHit	= FALSE;			//!< 衝突結果

	//	メッシュとの距離を初期化しておく
		inObject->setDist(inDist);

	//	作業用イテレータ
		vector<C3D_OBJECT*>::iterator	pi;

	//	オブジェクトにカメラとの距離を設定する
		for(pi  = objects.begin();
			pi != objects.end();
			pi++)
		{
		//	同類のオブジェクトとは当たり判定をしない
			if(inObject != (C3D_OBJECT*)*pi)
			{
				if(	(*pi)->intersect(inObject, inLine, inIntersectValue))
				{
				//	前回のオブジェクトより、衝突距離が近い時
					if(	inDist	> inObject->getDist())
					{
						inHit				= TRUE;
					//	直前の衝突オブジェクトを登録
						prevHitObject	= (*pi);
					//	距離をとっておく
						inDist				= inObject->getDist();
					}
				}
			}
		}

	//	衝突していたら、最短距離を入れておく
		if(	inHit)	inObject->setDist(inDist);

		return	inHit;
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::intersect（距離指定、上下、左右、前後版）
	//
	//	機能		距離指定の全方向版の衝突判定につなげる
	//	更新		2008/08/20			<新規>
	//================================================================================
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown, float inIntersectValue)
	{
		return	intersect(inObject, &LINE(inUpDown, eLR_NO, eFR_NO), inIntersectValue);
	}
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight, float inIntersectValue)
	{
		return	intersect(inObject, &LINE(eUD_NO, inLeftRight, eFR_NO), inIntersectValue);
	}
	BOOL
	C3D_OBJECT_GROUP::intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear, float inIntersectValue)
	{
		return	intersect(inObject, &LINE(eUD_NO, eLR_NO, inFrontRear), inIntersectValue);
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::intersectPos
	//				（判定開始位置指定、距離指定、全方向版）
	//
	//	機能		全方向でグループに対して、衝突判定
	//	引数		inObject			衝突物
	//				inLoc				判定開始位置
	//				inLine				判定方向
	//				inIntersectValue	判定する距離
	//	更新		2009/02/12			<新規>
	//================================================================================
	BOOL
	C3D_OBJECT_GROUP::intersectPos(C3D_OBJECT*	inObject,
								   VEC3*		inLoc,
								   LINE*		inLine,
								   float		inIntersectValue)
	{
	//	衝突物の存在確認が無ければ、衝突して無い事を返して終了
		if(	!inObject->getVisible())	return	FALSE;

		float	inDist	= 10000.0f;			//!< 距離を確保しておく
		BOOL	inHit	= FALSE;			//!< 衝突結果

	//	メッシュとの距離を初期化しておく
		inObject->setDist(inDist);

	//	作業用イテレータ
		vector<C3D_OBJECT*>::iterator	pi;

	//	オブジェクトにカメラとの距離を設定する
		for(pi  = objects.begin();
			pi != objects.end();
			pi++)
		{
		//	同類のオブジェクトとは当たり判定をしない
			if(inObject != (C3D_OBJECT*)*pi)
			{
				if(	(*pi)->intersectPos(inObject, inLoc, inLine, inIntersectValue))
				{
				//	前回のオブジェクトより、衝突距離が近い時
					if(	inDist	> inObject->getDist())
					{
						inHit			= TRUE;
					//	直前の衝突オブジェクトを登録
						prevHitObject	= (*pi);
					//	距離をとっておく
						inDist			= inObject->getDist();
					}
				}
			}
		}

	//	衝突していたら、最短距離を入れておく
		if(	inHit)	inObject->setDist(inDist);

		return	inHit;
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::intersectPos
	//				（判定開始位置指定、距離指定、上下、左右、前後版）
	//
	//	機能		距離指定の全方向版の衝突判定につなげる
	//	更新		2009/02/12			<新規>
	//================================================================================
	BOOL
	C3D_OBJECT_GROUP::intersectPos(
		C3D_OBJECT* inObject, VEC3* inLoc, LINE_UPDOWN inUpDown, float inIntersectValue)
	{
		return	intersectPos(
					inObject, inLoc, &LINE(inUpDown, eLR_NO, eFR_NO), inIntersectValue);
	}
	BOOL
	C3D_OBJECT_GROUP::intersectPos(
		C3D_OBJECT* inObject, VEC3* inLoc, LINE_LEFTRIGHT inLeftRight, float inIntersectValue)
	{
		return	intersectPos(
					inObject, inLoc, &LINE(eUD_NO, inLeftRight, eFR_NO), inIntersectValue);
	}
	BOOL
	C3D_OBJECT_GROUP::intersectPos(
		C3D_OBJECT* inObject, VEC3* inLoc, LINE_FRONTREAR inFrontRear, float inIntersectValue)
	{
		return	intersectPos(
					inObject, inLoc, &LINE(eUD_NO, eLR_NO, inFrontRear), inIntersectValue);
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_GROUP::clear
	//
	//	機能		グループのメンバーを全消去
	//	更新		2009/01/15			<新規>
	//================================================================================
	void
	C3D_OBJECT_GROUP::clear(void)
	{
	//	通常グループを消去
		objects.clear();
	////	領域マップを消去
	//	VEC_LOOP(GROUP_AREA*, areaMap)
	//	{
	//		(*p)->objects.clear();
	//	}
		prevHitObject = NULL;	
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------