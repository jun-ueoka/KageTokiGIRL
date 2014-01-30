//***********************************************************************************
//!	ファイル名		Class_MovieCamera.h
//
//	機能			ムービーに使用するカメラの定義
//	記述			小金　智弥
//	プロジェクト	未定
//	更新			2008/12/15	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_3DObjectCamera.h"

namespace MATH
{
	//==================================================================
	//	スプライン曲線の区間の最大値
	//==================================================================
	#define MaxSplineSize 100

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CSPLINE
	//
	//	機能			ムービー用カメラに使用するスプライン曲線の計算
	//	機能概要		
	//	更新			2008/12/15	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class SPLINE
	{
	protected:		//-------------------- protected --------------------//
		//====================================================================
		//	スプライン関連
		//====================================================================
		int		iNumber;				//区間の数
   		float	a[MaxSplineSize+1];		//0次係数(a)
		float	b[MaxSplineSize+1];		//1次係数(b)
		float	c[MaxSplineSize+1];		//2次係数(c)
		float	d[MaxSplineSize+1];		//3次係数(b)
	public:			//-------------------- public --------------------//
		SPLINE();
		//==================================================================
		//	スプラインの情報の設定
		//==================================================================
			void setSplineDate(float *LocDate, int inNumber);
		//==================================================================
		//	スプライン曲線の計算結果を返す
		//==================================================================
			float getSplineCalc(float inMedia);
	};
}


//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//==================================================================
	//	
	//==================================================================
	struct Vec3Spline
	{
		MATH::SPLINE x;
		MATH::SPLINE y;
		MATH::SPLINE z;
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		C3D_MOVIE_CAMERA
	//
	//	機能			三次元物体の描画空間(ムービー用)を生成する
	//	機能概要		
	//	更新			2008/12/15	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class C3D_MOVIE_CAMERA : public C3D_OBJECT_CAMERA
	{
	protected:		//-------------------- protected --------------------//
		//	カメラ位置
			int			iCameraLocVertex;							//!< カメラ位置の動きのラインの頂点数
			Vec3Spline	vCameraLoc;									//!< カメラ位置の動きのライン
			float		fCameraLocMedia;							//!< カメラ位置の動きの媒介変数
			float		fCameraLocSpeed;							//!< カメラ位置の動きの速度
			BOOL		bCameraLocLoop;								//!< カメラ位置の動きのループフラグ
		//	カメラ視点
			int			iCameraViewPointVertex;						//!< カメラ視点の動きのラインの頂点数
			Vec3Spline	vCameraViewPoint;							//!< カメラ視点の動きのライン
			float		fCameraViewPointMedia;						//!< カメラ視点の動きの媒介変数
			float		fCameraViewPointSpeed;						//!< カメラ視点の動きの速度
			BOOL		bCameraViewPointLoop;						//!< カメラ視点の動きのループフラグ
	public:			//-------------------- public --------------------//
			C3D_MOVIE_CAMERA(	C3D_OBJECT*	inTarget,
								BOOL		inChased,
								float		inOffsetBack,
								float		inLookHeight,
								BOOL		inPan);				//!< コンストラクタ(追跡対象あり)
			C3D_MOVIE_CAMERA(VEC3* inLoc, VEC3* inViewPoint);	//!< コンストラクタ(追跡対象なし)
	virtual ~C3D_MOVIE_CAMERA();

	//==================================================================
	//	カメラ位置の動きの設定、動作（スプライン曲線設定）
	//==================================================================
		void	setLocSpline(VEC3 *inLoc, int inVertex, BOOL inLoop);
		BOOL	doLocSpline(void);
		void	setLocSpeed(float inSpeed)									{ fCameraLocSpeed = inSpeed;	}
		float	getLocSpeed(void)											{ return fCameraLocSpeed;		}
		void	setLocMedia(float inMedia)									{ fCameraLocMedia = inMedia;	}
		float	getLocMedia(void)											{ return fCameraLocMedia;		}
		void	setLocLoop(BOOL inLoop)										{ bCameraLocLoop = inLoop;		}
		BOOL	getLocLoop(void)											{ return bCameraLocLoop;		}
	//==================================================================
	//	カメラ視点の動きの設定、動作（スプライン曲線設定）
	//==================================================================
		void	setViewPointSpline(VEC3 *inLoc, int inVertex, BOOL inLoop);
		BOOL	doViewPointSpline(void);
		void	setViewPointSpeed(float inSpeed)							{ fCameraViewPointSpeed = inSpeed;	}
		float	getViewPointSpeed(void)										{ return fCameraViewPointSpeed;		}
		void	setViewPointMedia(float inMedia)							{ fCameraViewPointMedia = inMedia;	}
		float	getViewPointMedia(void)										{ return fCameraViewPointMedia;		}
		void	setViewPointLoop(BOOL inLoop)								{ bCameraViewPointLoop	= inLoop;	}
		BOOL	getViewPointLoop(void)										{ return bCameraViewPointLoop;		}
	};
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------