//***********************************************************************************
//!	ファイル名		Class_3DObjectCamera.h
//
//	機能			三次元の物体を描写するカメラの定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2007/12/08	<新規>
//					2008/07/09	<追加>	全体的にコメントの強化
//***********************************************************************************

#pragma once

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
	//	カメラのモード
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	CAMERA_MODE
	{
		eCAMO_PARALLEL,				//!< 視点平衡移動モード
		eCAMO_PARALLEL_CHASE,		//!< カメラ平衡移動モード
		eCAMO_DOUBLE_TARGET,		//!< 二重追跡モード(2D視点)
		eCAMO_NO					//!< カメラモードなし
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		C3D_OBJECT_CAMERA
	//
	//	機能			三次元物体の描画空間を生成する
	//	機能概要		このカメラにDirect3Dデバイスを送ると、
	//					3D空間を自動生成してくれます
	//					かなりKageToki仕様なので、汎用できない場合がある
	//	更新			2008/07/09	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	C3D_OBJECT_CAMERA : public C3D_OBJECT
	{
	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	追跡関連
			//==================================================================
				C3D_OBJECT*		target;					//!< 追跡対象
				BOOL			bChased;				//!< 追跡確認
				float			fOffsetBack;			//!< 追跡対象からの後方距離

				C3D_OBJECT*		secondTarget;			//!< 二重追跡対象
				float			fTargetsDist;			//!< 二つの追跡対象の距離

			//==================================================================
			//	カメラ情報
			//==================================================================
				VEC3	viewPointFinish;				//!< 最終的な視点
				VEC3	viewPoint;						//!< 視点

				VEC3	viewMovePoint;					//!< 視点の移動終着点
				int		iViewMoveCnt;					//!< 視点移動カウンタ
				int		iMaxViewMoveCnt;				//!< 最大視点移動カウンタ

				VEC3	cameraMovePoint;				//!< カメラの移動終着点
				int		iCameraMoveCnt;					//!< カメラ移動カウンタ
				int		iMaxCameraMoveCnt;				//!< 最大カメラ移動カウンタ

				VEC3	keepViewTargetShift;			//!< 追跡対象カメラ位置保存用
				BOOL	bViewTargetShiftMove;			//!< 追跡対象カメラ位置変動確認
				VEC3	viewTargetShiftMovePoint;		//!< 追跡対象カメラ位置の移動終着点
				int		iViewTargetShiftMoveCnt;		//!< 追跡対象カメラ位置移動カウンタ
				int		iMaxViewTargetShiftMoveCnt;		//!< 最大追跡対象カメラ位置移動カウンタ

				VEC3		viewShift;					//!< 視点補正
				VEC3		viewPointShift;				//!< カメラの視点からの位置
				VEC3		viewTargetShift;			//!< 追跡対象からの、カメラの位置
				float		viewNear;					//!< 近視野
				float		viewFar;					//!< 遠視野
				VEC3		currentUp;					//!< カレント上方
				BOOL		bPan;						//!< パン確認
				float		fLookHeight;				//!< カメラの高さ
				float		fFieldOfView;				//!< 撮影する角度
				float		fAspect;					//!< 縦横比
				CAMERA_MODE	cameraMode;					//!< カメラのモード

				MAT16			matrixView;				//!< ビューマトリックス
				MAT16			matrixProjection;		//!< プロジェクションマトリックス

				VEC3	doubleTargetLoc;				//!< 二つの視点対象の間の位置
				VEC3	getDoubleTargetLoc(void);		//!< 二つの視点対象の間の位置を算出
				void	viewMovement(void);				//!< 視点位置変動処理
				void	cameraMovement(void);			//!< カメラ位置変動処理
				void	viewTargetShiftMovement(void);	//!< 追跡対象カメラ位置変動処理

	public:			//-------------------- public --------------------//

				C3D_OBJECT_CAMERA(C3D_OBJECT*	inTarget,
								  BOOL			inChased,
								  float			inOffsetBack,
								  float			inLookHeight,
								  BOOL			inPan);				//!< コンストラクタ(追跡対象なし)
				C3D_OBJECT_CAMERA(VEC3* inLoc, VEC3* inViewPoint);	//!< コンストラクタ(追跡対象あり)
		virtual	~C3D_OBJECT_CAMERA();								//!< デストラクタ
		
	public:			//-------------------- public --------------------//

				void	updateCamera(void);							//!< カメラを更新する
				void	initialize(void);							//!< 初期化
		virtual	void	viewTransform(CONST DEV inDev);				//!< ワールド変換を施す

			//==================================================================
			//	access	追跡対象
			//==================================================================
				C3D_OBJECT*	getTarget(void)const					{ return	target;			}
				void		setTarget(C3D_OBJECT* inTarget)			{ target = inTarget;		}
				C3D_OBJECT*	getSecondTarget(void)const				{ return	secondTarget;	}
				void		setSecondTarget(C3D_OBJECT* inTarget)	{ secondTarget	= inTarget;	}

			//==================================================================
			//	access	追跡確認
			//==================================================================
				BOOL	getChase(void)const						{ return	bChased;			}
				void	setChase(BOOL inChased)					{ bChased = inChased;			}

			//==================================================================
			//	access	後方距離
			//==================================================================
				float	getOffsetBack(void)const				{ return	fOffsetBack;		}
				void	setOffsetBack(float inOffsetBack)		{ fOffsetBack = inOffsetBack;
																  setMoveQuantity(fOffsetBack);	}

			//==================================================================
			//	access	最終的な視点
			//==================================================================
				VEC3	getViewPointFinish(void)const			{ return	viewPointFinish;		}
				void	setViewPointFinish(VEC3* inViewPoint)	{ viewPointFinish = *inViewPoint;	}

			//==================================================================
			//	access	視点
			//==================================================================
				VEC3	getViewPoint(void)const				{ return	viewPoint;			}
				float	getViewPointX(void)const			{ return	viewPoint.x;		}
				float	getViewPointY(void)const			{ return	viewPoint.y;		}
				float	getViewPointZ(void)const			{ return	viewPoint.z;		}
				void	setViewPoint(VEC3* inViewPoint)		{ viewPoint		= *inViewPoint;	}
				void	setViewPointX(float inViewPointX)	{ viewPoint.x	= inViewPointX;	}
				void	setViewPointY(float inViewPointY)	{ viewPoint.y	= inViewPointY;	}
				void	setViewPointZ(float inViewPointZ)	{ viewPoint.z	= inViewPointZ;	}
				void	calcViewPoint(VEC3* inViewPoint)	{ viewPoint	   += *inViewPoint;	}
				void	calcViewPointX(float inViewPointX)	{ viewPoint.x  += inViewPointX;	}
				void	calcViewPointY(float inViewPointY)	{ viewPoint.y  += inViewPointY;	}
				void	calcViewPointZ(float inViewPointZ)	{ viewPoint.z  += inViewPointZ;	}

			//==================================================================
			//	access	最大視点移動カウンタ
			//==================================================================
				int		getMaxViewMoveCnt(void)const		{ return	iMaxViewMoveCnt;	}
				void	setMaxViewMoveCnt(int inValue)		{ iMaxViewMoveCnt = inValue;	}

			//==================================================================
			//	access	最大カメラ移動カウンタ
			//==================================================================
				int		getMaxCameraMoveCnt(void)const		{ return	iMaxCameraMoveCnt;	}
				void	setMaxCameraMoveCnt(int inValue)	{ iMaxCameraMoveCnt = inValue;	}

			//==================================================================
			//	access	視点補正
			//==================================================================
				VEC3	getViewShift(void)const				{ return	viewShift;			}
				float	getViewShiftX(void)const			{ return	viewShift.x;		}
				float	getViewShiftY(void)const			{ return	viewShift.y;		}
				float	getViewShiftZ(void)const			{ return	viewShift.z;		}
				void	setViewShift(VEC3* inViewShift)		{ viewShift		= *inViewShift;	}
				void	setViewShiftX(float inViewShiftX)	{ viewShift.x	= inViewShiftX;	}
				void	setViewShiftY(float inViewShiftY)	{ viewShift.y	= inViewShiftY;	}
				void	setViewShiftZ(float inViewShiftZ)	{ viewShift.z	= inViewShiftZ;	}
				void	calcViewShift(VEC3* inViewShift)	{ viewShift	   += *inViewShift;	}
				void	calcViewShiftX(float inViewShiftX)	{ viewShift.x  += inViewShiftX;	}
				void	calcViewShiftY(float inViewShiftY)	{ viewShift.y  += inViewShiftY;	}
				void	calcViewShiftZ(float inViewShiftZ)	{ viewShift.z  += inViewShiftZ;	}

			//==================================================================
			//	access	カメラからの、追跡対象の位置
			//==================================================================
				VEC3	getViewPointShift(void)const					{ return	viewPointShift;					}
				float	getViewPointShiftX(void)const					{ return	viewPointShift.x;				}
				float	getViewPointShiftY(void)const					{ return	viewPointShift.y;				}
				float	getViewPointShiftZ(void)const					{ return	viewPointShift.z;				}
				void	setViewPointShift(VEC3* inViewPointShift)		{ viewPointShift	= *inViewPointShift;	}
				void	setViewPointShiftX(float inViewPointShiftX)		{ viewPointShift.x	= inViewPointShiftX;	}
				void	setViewPointShiftY(float inViewPointShiftY)		{ viewPointShift.y	= inViewPointShiftY;	}
				void	setViewPointShiftZ(float inViewPointShiftZ)		{ viewPointShift.z	= inViewPointShiftZ;	}
				void	calcViewPointShift(VEC3* inViewPointShift)		{ viewPointShift   += *inViewPointShift;	}
				void	calcViewPointShiftX(float inViewPointShiftX)	{ viewPointShift.x  += inViewPointShiftX;	}
				void	calcViewPointShiftY(float inViewPointShiftY)	{ viewPointShift.y  += inViewPointShiftY;	}
				void	calcViewPointShiftZ(float inViewPointShiftZ)	{ viewPointShift.z  += inViewPointShiftZ;	}

			//==================================================================
			//	access	追跡対象からの、カメラの位置
			//==================================================================
				VEC3	getViewTargetShift(void)const					{ return	viewTargetShift;				}
				float	getViewTargetShiftX(void)const					{ return	viewTargetShift.x;				}
				float	getViewTargetShiftY(void)const					{ return	viewTargetShift.y;				}
				float	getViewTargetShiftZ(void)const					{ return	viewTargetShift.z;				}
				void	setViewTargetShift(VEC3* inViewTargetShift)		{ viewTargetShift	 = *inViewTargetShift;	}
				void	setViewTargetShiftX(float inViewTargetShiftX)	{ viewTargetShift.x	 = inViewTargetShiftX;	}
				void	setViewTargetShiftY(float inViewTargetShiftY)	{ viewTargetShift.y	 = inViewTargetShiftY;	}
				void	setViewTargetShiftZ(float inViewTargetShiftZ)	{ viewTargetShift.z	 = inViewTargetShiftZ;	}
				void	calcViewTargetShift(VEC3* inViewTargetShift)	{ viewTargetShift	+= *inViewTargetShift;	}
				void	calcViewTargetShiftX(float inViewTargetShiftX)	{ viewTargetShift.x	+= inViewTargetShiftX;	}
				void	calcViewTargetShiftY(float inViewTargetShiftY)	{ viewTargetShift.y	+= inViewTargetShiftY;	}
				void	calcViewTargetShiftZ(float inViewTargetShiftZ)	{ viewTargetShift.z	+= inViewTargetShiftZ;	}

			//==================================================================
			//	access	近視野,遠視野
			//==================================================================
				float	getViewNear(void)const				{ return	viewNear;			}
				float	getViewFar(void)const				{ return	viewFar;			}
				void	setViewNear(float inViewNear)		{ viewNear = inViewNear;		}
				void	setViewFar(float inViewFar)			{ viewFar = inViewFar;			}

			//==================================================================
			//	access	カレント上方
			//==================================================================
				VEC3	getCurrentUp(void)const				{ return	currentUp;			}
				void	setCurrentUp(VEC3* inCurrentUp)		{ currentUp = *inCurrentUp;		}

			//==================================================================
			//	access	パン
			//==================================================================
				BOOL	getPan(void)const					{ return	bPan;				}
				void	setPan(BOOL inPan)					{ bPan = inPan;	updateCamera();	}

			//==================================================================
			//	access	視点の高さ
			//==================================================================
				float	getLookHeight(void)const			{ return	fLookHeight;		}
				void	setLookHeight(float inLookHeight)	{ fLookHeight = inLookHeight;	}

			//==================================================================
			//	access	撮影する角度
			//==================================================================
				float	getFieldOfView(void)const			{ return	fFieldOfView;		}
				void	setFieldOfView(float inFieldOfView)	{ fFieldOfView  = inFieldOfView;}
				void	calcFieldOfView(float inFieldOfView){ fFieldOfView += inFieldOfView;}

			//==================================================================
			//	access	撮影する角度
			//==================================================================
				float	getAspect(void)const				{ return	fAspect;			}
				void	setAspect(FLOAT inAspect)			{ fAspect = inAspect;			}

			//==================================================================
			//	access	カメラモード
			//==================================================================
				CAMERA_MODE	getCameraMode(void)const		{ return	cameraMode;			}
				void		setCameraMode(CAMERA_MODE inMode);

			//==================================================================
			////	access	マトリックス
			//==================================================================
				MAT16*	getMatrixView(void)					{ return	&matrixView;		}
				MAT16*	getMatrixProjection(void)			{ return	&matrixProjection;	}

			//==================================================================
			//	仮想メソッド回避
			//==================================================================
				BOOL	draw(CONST DEV inDev)				{ return	FALSE;				}
	};
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------