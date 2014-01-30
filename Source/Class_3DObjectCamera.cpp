//***********************************************************************************
//!	ファイル名		Class_3DObjectCamera.cpp
//
//	機能			三次元の物体を描写するカメラの実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2007/12/08	<新規>
//					2008/07/09	<追加>	全体的にコメントの強化
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_3DObjectCamera.h"

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	C3D_OBJECT_CAMERA::C3D_OBJECT_CAMERA
	//
	//	機能		コンストラクタ(追跡対象あり)
	//	引数		inTarget		操作対象
	//				inChased		追跡確認
	//				inOffsetBack	追跡時の後方位置
	//				inLookHeight	追跡時の高さ
	//				inPan			パン確認
	//	更新		2007/12/08		<新規>
	//================================================================================
	C3D_OBJECT_CAMERA::C3D_OBJECT_CAMERA(C3D_OBJECT*  inTarget,
										 BOOL		  inChased,
										 float		  inOffsetBack,
										 float		  inLookHeight,
										 BOOL		  inPan)
	{
	//	カメラの初期化
		C3D_OBJECT_CAMERA::initialize();

	//	追跡関連の初期化
		bChased		= inChased;			//!< 追跡確認
		bPan		= inPan;			//!< 上下左右動作確認
		fLookHeight	= inLookHeight;		//!< カメラの高さ
		setOffsetBack(inOffsetBack);	//!< 後方距離

	//	位置の初期化
		setLoc(&VEC3(0.0f, fLookHeight, fOffsetBack));

	//	追跡対象の設定
		setTarget(inTarget);
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_CAMERA::C3D_OBJECT_CAMERA
	//
	//	機能		コンストラクタ(追跡対象なし)
	//	引数		inLoc			初期位置
	//				inViewPoint		初期視点
	//	更新		2007/12/08		<新規>
	//================================================================================
	C3D_OBJECT_CAMERA::C3D_OBJECT_CAMERA(VEC3 *inLoc, VEC3 *inViewPoint)
	{
	//	カメラの初期化
		C3D_OBJECT_CAMERA::initialize();

	//	初期化
		viewPoint	= *inViewPoint;	//!< 視点
		setLoc(inLoc);				//!< 位置
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_CAMERA::~C3D_OBJECT_CAMERA
	//
	//	機能		デストラクタ
	//	更新		2007/12/08		<新規>
	//================================================================================
	C3D_OBJECT_CAMERA::~C3D_OBJECT_CAMERA(void)
	{
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_CAMERA::getDoubleTargetLoc
	//
	//	機能		二つの視点対象の間の位置を算出
	//	戻り値		VEC3			位置
	//	更新		2009/02/19		<新規>
	//================================================================================
	VEC3
	C3D_OBJECT_CAMERA::getDoubleTargetLoc(void)
	{
	//	視点対象が設定されていれば、二つの視点対象の間の位置を算出
		if(	this->target && this->secondTarget)
		{
			VEC3	inDist			= this->target->getLoc() - this->secondTarget->getLoc();
			this->fTargetsDist		= D3DXVec3Length(&inDist);
			this->doubleTargetLoc	= this->secondTarget->getLoc() + (inDist * 0.5f);
			return	this->doubleTargetLoc;
		}
	//	設定されてなければ処理をせず、現在の視点対象を返す
		return	this->viewPoint;
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_CAMERA::viewMovement
	//
	//	機能		視点位置変動処理
	//	更新		2009/02/19		<新規>
	//================================================================================
	void
	C3D_OBJECT_CAMERA::viewMovement(void)
	{
	//	視点移動カウンタがある時
		if(	this->iViewMoveCnt   > 0)
		{
			this->iViewMoveCnt--;
		//	視点の移動終着点と現在の視点のまでの距離を計算
			VEC3	moveVec	= ((this->viewMovePoint - this->viewPoint) * 0.15f);
			this->viewPoint += moveVec;
		}
	//	視点移動カウンタが残って無いなら、そのまま視点を更新
		else
		{
			this->viewPoint = this->viewMovePoint;
		}
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_CAMERA::cameraMovement
	//
	//	機能		カメラ位置変動処理
	//	更新		2009/02/19		<新規>
	//================================================================================
	void
	C3D_OBJECT_CAMERA::cameraMovement(void)
	{
	//	カメラ位置移動カウンタがある時
		if(	this->iCameraMoveCnt   > 0)
		{
			this->iCameraMoveCnt--;
		//	カメラの移動終着点と現在のカメラのまでの距離を計算
			VEC3	moveVec	= ((this->cameraMovePoint - this->getLoc()) * 0.15f);
			this->calcLoc(&moveVec);
		}
	//	カメラ移動カウンタが残って無いなら、そのままカメラ位置を更新
		else
		{
			this->setLoc(&cameraMovePoint);
		}
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_CAMERA::viewTargetShiftMovement
	//
	//	機能		追跡対象カメラ位置変動処理
	//	更新		2009/02/19		<新規>
	//================================================================================
	void
	C3D_OBJECT_CAMERA::viewTargetShiftMovement(void)
	{
	//	追跡対象カメラ位置変動確認が有効の時
		if(	this->bViewTargetShiftMove)
		{
		//	追跡対象カメラ位置移動カウンタがある時
			if(	this->iViewTargetShiftMoveCnt   > 0)
			{
				this->iViewTargetShiftMoveCnt--;
			//	追跡対象カメラ位置の移動終着点と現在の追跡対象カメラ位置までの距離を計算
				VEC3	moveVec	= ((this->viewTargetShiftMovePoint - this->viewTargetShift) * 0.15f);
				this->viewTargetShift += moveVec;
			}
		//	追跡対象カメラ位置移動カウンタが残って無いなら、そのまま追跡対象カメラ位置を更新
			else
			{
				this->viewTargetShift		= this->viewTargetShiftMovePoint;
				this->bViewTargetShiftMove	= FALSE;
			}
		}
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_CAMERA::updateCamera
	//
	//	機能		カメラを更新する
	//	更新		2007/12/08		<新規>
	//================================================================================
	void
	C3D_OBJECT_CAMERA::updateCamera(void)
	{
	//	モード分岐
		switch(cameraMode)
		{
		//	視点平衡移動モード
			case	eCAMO_PARALLEL:
				viewPoint		= viewPointShift + getLoc();
				viewPointFinish	= viewPoint;
				break;

		//	カメラ平衡移動モード
			case	eCAMO_PARALLEL_CHASE:
			//	対象が無ければ、処理をしない
				if(	target)
				{
				//	視点位置変動処理
					this->viewMovement();
				//	視点の移動終着点と追跡対象位置が一致しない時
					if(	viewMovePoint != target->getLoc())
					{
					//	新たに視点の移動終着点を設定し、視点移動カウンタ開始
						viewMovePoint  = target->getLoc();
						iViewMoveCnt   = iMaxViewMoveCnt;
					}
				//	追跡対象カメラ位置変動処理
					this->viewTargetShiftMovement();

				//	追跡する時
					if(	bChased)
					{
					//	パンする時
						if(	bPan)
						{
						}
					//	パンしない時
						else
						{
						//	カメラの位置を、ターゲットからのカメラの位置のオフセット値で設定
							setLoc(&(viewPoint + viewTargetShift));
						}
					}
				//	追跡しない時
					else
					{
					//	パンする時
						if(	bPan)
						{
						}
					//	パンしない時
						else
						{
						}
					}
					viewPointFinish = viewPoint	+ viewShift;
				//	視点からのカメラの位置の更新
					viewPointShift = viewPointFinish - getLoc();
				}
				break;

		//	二重追跡モード
			case	eCAMO_DOUBLE_TARGET:
			//	対象が無ければ、処理をしない
				if(	this->target && this->secondTarget)
				{
				//	視点位置変動処理
					this->viewMovement();
				//	視点の移動終着点と追跡対象位置が一致しない時
					if(	this->viewMovePoint != this->getDoubleTargetLoc())
					{
					//	新たに視点の移動終着点を設定し、視点移動カウンタ開始
						this->viewMovePoint  = this->doubleTargetLoc;
						this->iViewMoveCnt   = this->iMaxViewMoveCnt;
					//	追跡対象カメラ位置の変動開始
						this->bViewTargetShiftMove			= TRUE;
						this->viewTargetShiftMovePoint.z	= -(this->fTargetsDist + 1500.0f);
						this->iViewTargetShiftMoveCnt		= this->iMaxViewTargetShiftMoveCnt;
					}
				//	追跡対象カメラ位置変動処理
					this->viewTargetShiftMovement();

				//	追跡する時
					if(	this->bChased)
					{
					//	パンする時
						if(	this->bPan)
						{
						}
					//	パンしない時
						else
						{
						//	カメラの位置を、ターゲットからのカメラの位置のオフセット値で設定
							this->setLoc(&(this->viewPoint + this->viewTargetShift));
						}
					}
				//	追跡しない時
					else
					{
					//	パンする時
						if(	this->bPan)
						{
						}
					//	パンしない時
						else
						{
						}
					}
					this->viewPointFinish = this->viewPoint	+ this->viewShift;
				//	視点からのカメラの位置の更新
					this->viewPointShift = this->viewPointFinish - this->getLoc();
				}
				break;

			default:
			//	追跡対象が存在する時
				if(	this->target)
				{
				//	視点
					this->viewPoint	= this->target->getLoc();
				//	追跡する時
					if(	this->bChased)
					{
					//	追跡するためカメラの位置を更新
						this->setLoc(&this->target->getLoc());
					//	パンする時
						if(	this->bPan)
						{
						//	パン処理
							this->setAngleY(this->target->getAngleY());
						}
					//	後方へ移動
						this->moveRear();
					//	カメラの高さを入れる
						this->setLocY(this->fLookHeight);
					}
				//	視点補正
					this->viewPointFinish = this->viewPoint	+ this->viewShift;
				}
				else
				{	
					this->viewPointFinish = this->viewPoint;
				}
			//	視点からのカメラの位置の更新
				this->viewPointShift	= this->viewPointFinish - this->getLoc();
				break;
		}
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_CAMERA::initialize
	//
	//	機能		カメラの初期化
	//	更新		2007/12/08		<新規>
	//================================================================================
	void
	C3D_OBJECT_CAMERA::initialize(void)
	{
	//	スーパークラスでの初期化
		C3D_OBJECT::initialize();

	//	初期化
		target			= NULL;									//!< 追跡対象
		secondTarget	= NULL;									//!< 二重追跡対象
		fTargetsDist	= 0.0f;									//!< 二つの追跡対象の距離
		bChased			= FALSE;								//!< 追跡確認
		setOffsetBack(0.0f);									//!< 後方距離
		viewPointFinish	= VEC3(0.0f, 0.0f, 0.0f);				//!< 最終的な視点
		viewPoint		= VEC3(0.0f, 0.0f, 0.0f);				//!< 視点

		viewMovePoint	= VEC3(0.0f, 0.0f, 0.0f);				//!< 視点の移動終着点
		iViewMoveCnt	= 0;									//!< 視点移動カウンタ
		iMaxViewMoveCnt	= 60;									//!< 最大視点移動カウンタ

		cameraMovePoint		= VEC3(0.0f, 0.0f, 0.0f);			//!< カメラの移動終着点
		iCameraMoveCnt		= 0;								//!< カメラ移動カウンタ
		iMaxCameraMoveCnt	= 60;								//!< 最大カメラ移動カウンタ

		keepViewTargetShift			= VEC3(0.0f, 0.0f, 0.0f);	//!< 追跡対象カメラ位置保存用
		bViewTargetShiftMove		= FALSE;					//!< 追跡対象カメラ位置変動確認
		viewTargetShiftMovePoint	= VEC3(0.0f, 0.0f, 0.0f);	//!< 追跡対象カメラ位置の移動終着点
		iViewTargetShiftMoveCnt		= 0;						//!< 追跡対象カメラ位置移動カウンタ
		iMaxViewTargetShiftMoveCnt	= 60;						//!< 最大追跡対象カメラ位置移動カウンタ

		doubleTargetLoc	= VEC3(0.0f, 0.0f, 0.0f);				//!< 二つの視点対象の間の位置
		viewShift		= VEC3(0.0f, 0.0f, 0.0f);				//!< 視点補正
		viewPointShift	= VEC3(0.0f, 0.0f, 0.0f);				//!< 視点からのカメラの位置
		viewNear		= 1.0f;									//!< 近視野(適当)
		viewFar			= 1000.0f;								//!< 遠視野(適当)
		currentUp		= VEC3(0.0f, 1.0f, 0.0f);				//!< カレント上方
		bPan			= FALSE;								//!< パン確認
		fLookHeight		= 0.0f;									//!< カメラの高さ
		fFieldOfView	= 60.0f;								//!< 撮影角度
		fAspect			= 4.0f / 3.0f;							//!< 縦横比(平均的値)
		cameraMode		= eCAMO_NO;								//!< カメラモード
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_CAMERA::viewTransform
	//
	//	機能		ワールド変換を施す
	//	機能概要	送られてきたデバイスに、ワールド変換を施す
	//	引数		inDev			Direct3Dデバイス
	//	更新		2007/12/08		<新規>
	//================================================================================
	void
	C3D_OBJECT_CAMERA::viewTransform(CONST DEV inDev)
	{
	//	ビューマトリックスの生成
		D3DXMatrixLookAtLH(
			&matrixView,					//!< ビューマトリックスの格納先
			&getLoc(),						//!< 目
			&viewPointFinish,				//!< 視点
			&currentUp);					//!< カレント上方
	//	プロジェクションマトリックスの生成
		D3DXMatrixPerspectiveFovLH(
			&matrixProjection,				//!< プロジェクションマトリックの格納先
			D3DXToRadian(fFieldOfView),		//!< 撮影する角度
			fAspect,						//!< 縦横比
			viewNear,						//!< 近視野
			viewFar);						//!< 遠視野

	//	ワールドビュー変換
		inDev->SetTransform(D3DTS_VIEW,			&matrixView);
	//	ワールドプロジェクション変換
		inDev->SetTransform(D3DTS_PROJECTION,	&matrixProjection);
	}

	//================================================================================
	//!	メソッド名	C3D_OBJECT_CAMERA::setCameraMode
	//
	//	機能		カメラモードの変更
	//	機能概要	カメラモードの変更を行い、変更後の設定をする
	//	引数		inMode			新たなカメラモード
	//	更新		2009/02/07		<新規>
	//================================================================================
	void
	C3D_OBJECT_CAMERA::setCameraMode(CAMERA_MODE inMode)
	{
	//	今のカメラモードと、新たなカメラモードが等しいなら、処理を返す
		if(	this->cameraMode == inMode)	return;

	//	新規モード分岐
		switch(inMode)
		{
		//	視点平衡移動モード
			case	eCAMO_PARALLEL:
				break;

		//	カメラ平衡移動モード
			case	eCAMO_PARALLEL_CHASE:
				if(	target)
				{
				//	前のモードが二重追跡モードの時
					if(	this->cameraMode == eCAMO_DOUBLE_TARGET)
					{
						bViewTargetShiftMove	 = TRUE;
						viewTargetShiftMovePoint = keepViewTargetShift;
						iViewTargetShiftMoveCnt	 = iMaxViewTargetShiftMoveCnt;
					}
					else
					{
						viewPoint		= viewPoint	- viewShift;
						viewMovePoint	= target->getLoc();
					}
				}
				iViewMoveCnt	= iMaxViewMoveCnt;
				break;

		//	二重追跡モード
			case	eCAMO_DOUBLE_TARGET:
			//	追跡対象のどちらかが欠けてたら、モードを変更しない
				if(	!target	|| !secondTarget)	return;
			//	追跡対象の間の値を、視点移動終着点にセット
				viewMovePoint				= getDoubleTargetLoc();
				iViewMoveCnt				= iMaxCameraMoveCnt;
				viewTargetShiftMovePoint	= viewTargetShift;
				keepViewTargetShift			= viewTargetShift;
				break;

			default:
				break;
		}

		this->cameraMode = inMode;
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------