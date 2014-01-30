//***********************************************************************************
//!	ファイル名		Class_3DObject.cpp
//
//	機能			このプログラムで使用する三次元の物体に関する実装
//					3Dオブジェクトグループも実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2007/12/08	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_3DObject.h"

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	C3D_OBJECT::initialize
	//
	//	機能		基本的な情報の初期化
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	C3D_OBJECT::initialize(void)
	{
	//	初期化
		rot.xAxis			= VEC3(1.0f, 0.0f, 0.0f);	//!< 座標系Xの軸線
		rot.yAxis			= VEC3(0.0f, 1.0f, 0.0f);	//!< 座標系Yの軸線
		rot.zAxis			= VEC3(0.0f, 0.0f, 1.0f);	//!< 座標系Zの軸線
		rot.rpm				= VEC3(1.0f, 1.0f, 1.0f);	//!< 毎分回転数
		rot.angle			= VEC3(0.0f, 0.0f, 0.0f);	//!< 角度
		loc.line			= LINE();					//!< 進行方向
		loc.prev			= VEC3(0.0f, 0.0f, 0.0f);	//!< 前の位置
		loc.curr			= VEC3(0.0f, 0.0f, 0.0f);	//!< 現在の位置
		loc.next			= VEC3(0.0f, 0.0f, 0.0f);	//!< 次の座標
		bVisible			= TRUE;						//!< 表示
		fScale				= 1.0f;						//!< 標準倍率
		fMoveQuantity		= 1.0f;						//!< 移動量
		fOffsetFromCamera	= 0.0f;						//!< カメラとの距離
		fTemporaryRadius	= 1.0f;						//!< 仮半径
		fDist				= 1000.0f;					//!< 衝突物体間隔
		orderMessage		= eOMSG_NOMSG;				//!< 命令メッセージ
		thisMessage			= eTMSG_NOMSG;				//!< 識別メッセージ

		fDiffuseAlpha		= 0.0f;						//!< デフューズアルファ変動値

		D3DXMatrixIdentity(&matWorldIV);				//!< 逆行列

		update();										//!< ワールド行列更新
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::finalize
	//
	//	機能		後始末
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	C3D_OBJECT::finalize(void)
	{
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::rayFromInversedMatrix
	//
	//	機能		このワールド逆行列のベクトルのトランスフォームを求める
	//	引数		*inFrom			現在地
	//				*inDir			方向
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	C3D_OBJECT::rayFromInversedMatrix(VEC3 *inFrom, VEC3 *inDir)
	{
		MAT16	invMat;		//!< 逆行列格納

	//	現在のワールド行列の逆行列を生成
		D3DXMatrixInverse(&invMat, NULL, &matWorld);

	//	レイの視点はワールド行列の逆行列とする【スケーリングを反映する】
		D3DXVec3TransformCoord(inFrom, inFrom, &invMat);

	//	レイの方向は回転行列の逆行列とする【スケーリングを反映しない】
		D3DXMatrixInverse(&invMat, NULL, &rot.matrix3D);

	//	レイの方向は回転行列の逆行列とする
		D3DXVec3TransformCoord(inDir, inDir, &invMat);
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::move
	//
	//	機能		現在の属性で、位置を更新
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	C3D_OBJECT::move(void)
	{
	//	次の座標を受けとり、現在の座標に入れる
		setLoc(calcNextLoc(fMoveQuantity, &loc.line));
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::moveUpDown
	//
	//	機能		上下方向、移動値指定の移動処理
	//	引数		inUpDown		上下方向
	//				inMoveQuantity	移動値
	//	更新		2009/01/19		<新規>
	//================================================================================
	void
	C3D_OBJECT::moveUpDown(LINE_UPDOWN inUpDown, float inMoveQuantity)
	{
	//	方向を設定
		LINE	inLine	= LINE(inUpDown, eLR_NO, eFR_NO);
	//	次の座標を受けとり、現在の座標に入れる
		setLoc(calcNextLoc(
			(inMoveQuantity >= 0.0f) ? inMoveQuantity : -inMoveQuantity, &inLine));
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::moveLeftRight
	//
	//	機能		左右方向、移動値指定の移動処理
	//	引数		inLeftRight		左右方向
	//				inMoveQuantity	移動値
	//	更新		2009/01/19		<新規>
	//================================================================================
	void
	C3D_OBJECT::moveLeftRight(LINE_LEFTRIGHT inLeftRight, float inMoveQuantity)
	{
	//	方向を設定
		LINE	inLine	= LINE(eUD_NO, inLeftRight, eFR_NO);
	//	次の座標を受けとり、現在の座標に入れる
		setLoc(calcNextLoc(
			(inMoveQuantity >= 0.0f) ? inMoveQuantity : -inMoveQuantity, &inLine));
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::moveFrontRear
	//
	//	機能		前後方向、移動値指定の移動処理
	//	引数		inFrontRear		前後方向
	//				inMoveQuantity	移動値
	//	更新		2009/01/19		<新規>
	//================================================================================
	void
	C3D_OBJECT::moveFrontRear(LINE_FRONTREAR inFrontRear, float inMoveQuantity)
	{
	//	方向を設定
		LINE	inLine	= LINE(eUD_NO, eLR_NO, inFrontRear);
	//	次の座標を受けとり、現在の座標に入れる
		setLoc(calcNextLoc(
			(inMoveQuantity >= 0.0f) ? inMoveQuantity : -inMoveQuantity, &inLine));
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::calcNextLoc
	//
	//	機能		次の位置を受け取る
	//	機能概要	指定された移動量を進む次の位置を受け取る
	//	引数		inMoveQuantity		移動量
	//				inLine				方向
	//	戻り値		移動先の位置のポインター
	//	更新		2008/04/18			<新規>
	//================================================================================
	VEC3*
	C3D_OBJECT::calcNextLoc(float inMoveQuantity, LINE* inLine)
	{
	//	方向が入っていないなら、このインスタンスの方向を使う
		if(	!inLine)	inLine = &loc.line;

		VEC3	vt(0.0f, 0.0f, 0.0f);			//!< 方向

	//	レイの方向を受け取る
		vt		= calcRay(inLine);

	//	移動量を計算
		vt.x   *= inMoveQuantity;
		vt.y   *= inMoveQuantity;
		vt.z   *= inMoveQuantity;

	//	次の移動先を更新
		loc.next	= loc.curr + vt;

	//	次の座標を返す
		return	&loc.next;
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::calcRay
	//
	//	機能		レイの方向を受け取る
	//	機能概要	レイの方向を受け取る
	//	引数		*inLine			方向
	//	戻り値		レイの位置
	//	更新		2008/04/18		<新規>
	//================================================================================
	VEC3
	C3D_OBJECT::calcRay(LINE* inLine)
	{
	//	方向が入っていないなら、このインスタンスの方向を使う
		if(	!inLine)	inLine = &loc.line;

		VEC3	vt(0.0f, 0.0f, 0.0f);			//!< 結果のレイ
		VEC3	vw(	(float)inLine->lr,
					(float)inLine->ud,
					(float)inLine->fr);			//!< 方向を頂点化	

	//	レイの方向を検出
		D3DXVec3TransformCoord(&vt, &vw, getMatrixRotation());

	//	レイの方向を返す
		return	vt;
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::getNextLoc
	//
	//	機能		次の位置を受け取る
	//	機能概要	指定された移動量を進む次の位置を受け取る
	//	引数		inMoveQuantity		移動量
	//				inLine				方向
	//	戻り値		移動先の位置
	//	更新		2008/04/18			<新規>
	//================================================================================
	VEC3
	C3D_OBJECT::getNextLoc(float inMoveQuantity, LINE* inLine)
	{
	//	方向が入っていないなら、このインスタンスの方向を使う
		if(	!inLine)	inLine = &loc.line;

		VEC3	vt(0.0f, 0.0f, 0.0f);			//!< 方向

	//	レイの方向を受け取る
		vt		= calcRay(inLine);

	//	移動量を計算
		vt.x   *= inMoveQuantity;
		vt.y   *= inMoveQuantity;
		vt.z   *= inMoveQuantity;

	//	次の座標を返す
		return	loc.curr + vt;
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::updateScale
	//
	//	機能		スケーリング行列を更新する
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	C3D_OBJECT::updateScale(void)
	{
	//	スケーリング行列を初期化
		D3DXMatrixIdentity(&matScale);
	//	スケーリング行列を作成
		D3DXMatrixScaling(&matScale, fScale, fScale, fScale);
	//	ワールド行列の更新
		updateWorld();
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::updateRotat
	//
	//	機能		回転行列を更新する
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	C3D_OBJECT::updateRotat(void)
	{
	//	回転行列を初期化
		D3DXMatrixIdentity(&rot.matrix3D);
		D3DXMatrixIdentity(&rot.matrixX);
		D3DXMatrixIdentity(&rot.matrixY);
		D3DXMatrixIdentity(&rot.matrixZ);
	//	0 - 359度の範囲に収める
		NORMALIZE_DEGREE(rot.angle.x);
		NORMALIZE_DEGREE(rot.angle.y);
		NORMALIZE_DEGREE(rot.angle.z);
	//	任意の軸に沿って、回転行列を作成
		D3DXMatrixRotationAxis(&rot.matrixX, &rot.xAxis, D3DXToRadian(rot.angle.x));
		D3DXMatrixRotationAxis(&rot.matrixY, &rot.yAxis, D3DXToRadian(rot.angle.y));
		D3DXMatrixRotationAxis(&rot.matrixZ, &rot.zAxis, D3DXToRadian(rot.angle.z));
	//	3D回転行列を作成(順番に注意)
		rot.matrix3D = rot.matrixX * rot.matrixY * rot.matrixZ;
	//	ワールド行列の更新
		updateWorld();
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::updateLoc
	//
	//	機能		座標行列を更新する
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	C3D_OBJECT::updateLoc(void)
	{
	//	単位行列で初期化を行う
		D3DXMatrixIdentity(&loc.matrix);
	//	座標の行列を作成
		D3DXMatrixTranslation(&loc.matrix, loc.curr.x, loc.curr.y, loc.curr.z);
	//	ワールド行列の更新
		updateWorld();
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::updateWorld
	//
	//	機能		ワールド行列を更新する
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	C3D_OBJECT::updateWorld(void)
	{
	//	ワールド行列を作成(順番に注意)
		matWorld = matScale * rot.matrix3D * loc.matrix;
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::update
	//
	//	機能		全体的に行列を更新する
	//	更新		2008/04/18		<新規>
	//================================================================================
	void
	C3D_OBJECT::update(void)
	{
	//	スケーリング行列を初期化
		D3DXMatrixIdentity(&matScale);

	//	スケーリング行列を作成
		D3DXMatrixScaling(&matScale, fScale, fScale, fScale);

	//	回転行列を初期化
		D3DXMatrixIdentity(&rot.matrix3D);
		D3DXMatrixIdentity(&rot.matrixX);
		D3DXMatrixIdentity(&rot.matrixY);
		D3DXMatrixIdentity(&rot.matrixZ);

	//	0 - 359度の範囲に収める
		NORMALIZE_DEGREE(rot.angle.x);
		NORMALIZE_DEGREE(rot.angle.y);
		NORMALIZE_DEGREE(rot.angle.z);

	//	任意の軸に沿って、回転行列を作成
		D3DXMatrixRotationAxis(&rot.matrixX, &rot.xAxis, D3DXToRadian(rot.angle.x));
		D3DXMatrixRotationAxis(&rot.matrixY, &rot.yAxis, D3DXToRadian(rot.angle.y));
		D3DXMatrixRotationAxis(&rot.matrixZ, &rot.zAxis, D3DXToRadian(rot.angle.z));

	//	3D回転行列を作成(順番に注意)
		rot.matrix3D = rot.matrixX * rot.matrixY * rot.matrixZ;

	//	単位行列で初期化を行う
		D3DXMatrixIdentity(&loc.matrix);

	//	座標の行列を作成
		D3DXMatrixTranslation(&loc.matrix, loc.curr.x, loc.curr.y, loc.curr.z);

	//	ワールド行列の更新
		updateWorld();
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::calcDiffuseAlpha
	//
	//	機能		デフューズアルファ変動値の計算
	//	引数		inAlpha			デフューズアルファ変動値の計算値
	//	更新		2008/10/27		<新規>
	//================================================================================
	void
	C3D_OBJECT::calcDiffuseAlpha(float inAlpha)
	{
	//	計算
		fDiffuseAlpha	+= inAlpha;
	//	値補正
		if		(fDiffuseAlpha < -1.0f)	fDiffuseAlpha = -1.0f;
		else if	(fDiffuseAlpha >  1.0f)	fDiffuseAlpha	=  1.0f;
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::intersectRadius
	//
	//	機能		自身とオブジェクトとの衝突を、半径で検出する
	//	引数		inObject		衝突対象物
	//	戻り値		TRUE			衝突している
	//				FALSE			衝突していない
	//	更新		2007/12/08		<新規>
	//================================================================================
	BOOL
	C3D_OBJECT::intersectRadius(C3D_OBJECT *inObject)
	{
	//	どちらかが非表示なら、判定せずに衝突していない事を返す
		if(	(!inObject->getVisible()) || (!getVisible()))
		{	
			return FALSE;
		}
	//	自身とオブジェクトとの座標上の距離を演算
		VEC3	vDistance = getWorldCenter() - inObject->getWorldCenter();

	//	自身とオブジェクトとの距離を計算し、保持
		float	fDistance	= D3DXVec3Length(&vDistance);

	//	自身とオブジェクトとの距離が、ふたつのワールド座標系の半径より、長い時
		if(	fDistance > (getWorldRadius() + inObject->getWorldRadius()))
		{
		//	衝突していない事を返す
			return	FALSE;
		}
	//	上記以外
		else
		{
		//	衝突したオブジェクトとの間隔を保持
			inObject->setDist(
				(getWorldRadius() + inObject->getWorldRadius()) - fDistance);
		//	衝突している事を返
			return	TRUE;
		}
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT::intersectRadius
	//
	//	機能		自身とオブジェクトとの衝突を、半径で検出する(指定半径)
	//	引数		inObject		衝突対象物
	//				inRadius		半径
	//	戻り値		TRUE			衝突している
	//				FALSE			衝突していない
	//	更新		2009/01/19		<新規>
	//================================================================================
	BOOL
	C3D_OBJECT::intersectRadius(C3D_OBJECT *inObject, float inRadius)
	{
	//	自身とオブジェクトとの座標上の距離を演算
		VEC3	vDistance = getWorldCenter() - inObject->getWorldCenter();

	//	自身とオブジェクトとの距離を計算し、保持
		float	fDistance = D3DXVec3Length(&vDistance);

	//	自身とオブジェクトとの距離が、ふたつのワールド座標系の半径より、長い時
		if(	fDistance > (getWorldRadius() + inRadius))
		{
		//	衝突していない事を返す
			return	FALSE;
		}
	//	上記以外
		else
		{
		//	衝突したオブジェクトとの間隔を保持
			inObject->setDist((getWorldRadius() + inRadius) - fDistance);
		//	衝突している事を返
			return	TRUE;
		}
	}


	//================================================================================
	//!	メソッド名	C3D_OBJECT::angleToObject
	//
	//	機能		オブジェクトの方向を向く
	//	引数		inObject		対象物
	//	更新		2008/09/16		<新規>
	//================================================================================
	void
	C3D_OBJECT::angleToObject(C3D_OBJECT *inObject)
	{
		if(	inObject)
		{
			VEC3	vOrigin					= VEC3(0.0f,0.0f,-1.0f);
			float	fRad					= 0.0f;
			VEC3	vInverseCameraDirection	= -(getLoc() - inObject->getLoc());

		//	X-Z面での回転で実現するのに、Y軸の角度は必要ない
			vInverseCameraDirection.y		= 0.0f;

		//	オブジェクトに対しての方向から、自身の方向を決定する
			if(	vInverseCameraDirection.x < 0.0f)
			{
				fRad =  (float)acos(D3DXVec3Dot(&vInverseCameraDirection, &vOrigin)	/
						(D3DXVec3Length(&vOrigin) * D3DXVec3Length(&vInverseCameraDirection)));
			}
			else
			{
				fRad = -(float)acos(D3DXVec3Dot(&vInverseCameraDirection, &vOrigin)	/
						(D3DXVec3Length(&vOrigin) * D3DXVec3Length(&vInverseCameraDirection)));
			}
			setAngleY(D3DXToDegree(fRad));
		}
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------