//***********************************************************************************
//!	ファイル名		Class_MovieCamera.cpp
//
//	機能			ムービーに使用するカメラの実装
//	記述			小金　智弥
//	プロジェクト	未定
//	更新			2008/12/15	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace MATH
{
	//================================================================================
	//!	メソッド名	SPLINE::SPLINE
	//
	//	機能		コンストラクタ
	//	引数		なし
	//	更新		2007/12/08		<新規>
	//================================================================================
	SPLINE::SPLINE(void)
	{
	//	変数の初期化
		this->iNumber = 0;
	}

	//================================================================================
	//!	メソッド名	SPLINE::setSplineDate
	//
	//	機能		スプライン情報を設定
	//	引数		float	*LocDate 頂点配列
	//				int		inNumber 頂点数	
	//	更新		2007/12/08		<新規>
	//================================================================================
	void SPLINE::setSplineDate(float *LocDate, int inNumber)
	{
		float	tmp; 
		float	w[MaxSplineSize+1];
		int		iCnt;										//カウント用

		this->iNumber = inNumber - 1;						//区間の数を記録

		// ３次多項式の0次係数(a)を設定
		for(iCnt = 0; iCnt <= inNumber; ++iCnt) 
		{
       		this->a[iCnt] = LocDate[iCnt];
		}

		// ３次多項式の2次係数(c)を計算
		c[0] = c[this->iNumber] = 0.0f;
		for(iCnt = 1; iCnt < this->iNumber; ++iCnt)
		{
			this->c[iCnt] = 3.0f * (this->a[iCnt-1] - 2.0f * this->a[iCnt] + this->a[iCnt+1]);
		}

	//-----------------処理の意味がまだわからない-------------------------
   		// 左下を消す
  		w[0]=0.0f;
		for(iCnt = 1; iCnt < inNumber; ++iCnt) 
		{
			tmp = 4.0f - w[iCnt-1];
			this->c[iCnt] = (this->c[iCnt] - this->c[iCnt-1]) / tmp;
			w[iCnt] = 1.0f / tmp;
		}
		// 右上を消す
		for(iCnt = this->iNumber - 1; iCnt > 0; --iCnt) 
		{
			this->c[iCnt] = this->c[iCnt] - this->c[iCnt+1] * w[iCnt];
		}
	//-----------------処理の意味がまだわからない-------------------------

		// ３次多項式の1次係数(b)と3次係数(b)を計算
		this->b[inNumber] = this->d[inNumber] =0.0f;
		for(iCnt = 0; iCnt < inNumber; ++iCnt) 
		{
			this->d[iCnt] = ( this->c[iCnt+1] - this->c[iCnt]) / 3.0f;
			this->b[iCnt] = this->a[iCnt+1] - this->a[iCnt] - this->c[iCnt] - this->d[iCnt];
		}
	}

	//================================================================================
	//!	メソッド名	SPLINE::getSplineCalc
	//
	//	機能		スプライン情報を設定
	//	引数		float	inMedia 媒介変数
	//	戻り値		float	計算結果
	//	更新		2007/12/08		<新規>
	//================================================================================
	float SPLINE::getSplineCalc(float inMedia)
	{
		int		iSection;								//区間を求めるのに使用
		float	fMedia;									//修正を加えた媒介変数の記録に使用

		iSection = (int)floor(inMedia);					// 小数点以下切捨て

		//誤差修正
		if(iSection < 0)
		{
			iSection = 0; 
		}
		else if (iSection >= this->iNumber)
		{
			iSection = this->iNumber = 1;
		}
		fMedia = inMedia - (float)iSection;

		//求めた値を返す
		float Test = this->a[iSection] + ( this->b[iSection] + (this->c[iSection] + this->d[iSection] * fMedia) * fMedia ) * fMedia;
		return Test;
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	C3D_MOVIE_CAMERA::C3D_MOVIE_CAMERA
	//
	//	機能		コンストラクタ(追跡対象あり)
	//	引数		inTarget		操作対象
	//				inChased		追跡確認
	//				inOffsetBack	追跡時の後方位置
	//				inLookHeight	追跡時の高さ
	//				inPan			パン確認
	//	更新		2007/12/08		<新規>
	//================================================================================
	C3D_MOVIE_CAMERA::C3D_MOVIE_CAMERA(	C3D_OBJECT*	inTarget, 
										BOOL inChased, 
										float inOffsetBack, 
										float inLookHeight, 
										BOOL inPan)	
	: C3D_OBJECT_CAMERA(inTarget, inChased, inOffsetBack, inLookHeight, inPan)	
	{
	//	値の初期化
		this->iCameraLocVertex			= 0;
		this->fCameraLocMedia			= 0.0f;
		this->fCameraLocSpeed			= 0.0f;
		this->bCameraLocLoop			= FALSE;
		this->iCameraViewPointVertex	= 0;
		this->fCameraViewPointMedia		= 0.0f;
		this->fCameraViewPointSpeed		= 0.0f;
		this->bCameraViewPointLoop		= FALSE;
	}

	//================================================================================
	//!	メソッド名	C3D_MOVIE_CAMERA::C3D_MOVIE_CAMERA
	//
	//	機能		コンストラクタ(追跡対象なし)
	//	引数		inLoc			操作対象
	//				inViewPoint		追跡確認
	//	更新		2007/12/08		<新規>
	//================================================================================
	C3D_MOVIE_CAMERA::C3D_MOVIE_CAMERA(	VEC3* inLoc,
										VEC3* inViewPoint)
	: C3D_OBJECT_CAMERA(inLoc, inViewPoint)
	{
	//	値の初期化
		this->iCameraLocVertex			= 0;
		this->fCameraLocMedia			= 0.0f;
		this->fCameraLocSpeed			= 0.0f;
		this->bCameraLocLoop			= FALSE;
		this->iCameraViewPointVertex	= 0;
		this->fCameraViewPointMedia		= 0.0f;
		this->fCameraViewPointSpeed		= 0.0f;
		this->bCameraViewPointLoop		= FALSE;
	}

	//================================================================================
	//!	メソッド名	C3D_MOVIE_CAMERA::~C3D_MOVIE_CAMERA
	//
	//	機能		デストラクタ
	//	引数		なし
	//	更新		2008/12/08		<新規>
	//================================================================================
	C3D_MOVIE_CAMERA::~C3D_MOVIE_CAMERA()
	{
	}

	//================================================================================
	//!	メソッド名	C3D_MOVIE_CAMERA::setLocSpline
	//
	//	機能		カメラ位置の動きの設定(スプライン曲線せってい)
	//	引数		*inLoc		頂点配列
	//				inVertex	頂点数
	//				inLoop		ループフラグ
	//	更新		2007/12/08		<新規>
	//================================================================================
	void C3D_MOVIE_CAMERA::setLocSpline(D3DXVECTOR3 *inLoc, int inVertex, BOOL inLoop)
	{
		float fLocX[MaxSplineSize];
		float fLocY[MaxSplineSize];
		float fLocZ[MaxSplineSize];

		for(int i = 0; i < inVertex; ++i)
		{
			fLocX[i] = inLoc[i].x;
			fLocY[i] = inLoc[i].y;
			fLocZ[i] = inLoc[i].z;
		}

	//	スプラインの設定
		this->vCameraLoc.x.setSplineDate(fLocX, inVertex);	//頂点X
		this->vCameraLoc.y.setSplineDate(fLocY, inVertex);	//頂点Y
		this->vCameraLoc.z.setSplineDate(fLocZ, inVertex);	//頂点Z
		this->bCameraLocLoop = inLoop;						//ループフラグの記録
		this->iCameraLocVertex = inVertex;					//頂点数
	}

	//================================================================================
	//!	メソッド名	C3D_MOVIE_CAMERA::doLocSpline
	//
	//	機能		カメラ位置の動きの動作
	//	引数		なし
	//	戻り値		終了フラグ　TRUE　終了　FALSE　継続
	//	更新		2007/12/08		<新規>
	//================================================================================
	BOOL C3D_MOVIE_CAMERA::doLocSpline(void)
	{
	//	終了判定
		//	まだ終了していない場合
		if(this->fCameraLocMedia < (float)(this->iCameraLocVertex - 1))
		{
			VEC3 vLoc;	//更新する頂点位置の保存用

		//	スプライン計算結果を取得する
			vLoc.x = this->vCameraLoc.x.getSplineCalc(this->fCameraLocMedia);
			vLoc.y = this->vCameraLoc.y.getSplineCalc(this->fCameraLocMedia);
			vLoc.z = this->vCameraLoc.z.getSplineCalc(this->fCameraLocMedia);

		//	位置を更新する
			this->setLoc(&vLoc);

		//	媒介変数を増やす
			this->fCameraLocMedia += this->fCameraLocSpeed;
			
			return FALSE;
		}
		//	終了している場合
		else
		{
		//	ループ判定
			//	ループを行う
			if(this->bCameraLocLoop)
			{
				this->fCameraLocMedia = 0.0f;
				return FALSE;
			}
			//	ループを行わない
			else
			{
				return TRUE;
			}
		}
	}

	//================================================================================
	//!	メソッド名	C3D_MOVIE_CAMERA::setViewPointSpline
	//
	//	機能		カメラ視点の動きの設定(スプライン曲線せってい)
	//	引数		*inLoc		頂点配列
	//				inVertex	頂点数
	//				inLoop		ループフラグ
	//	更新		2007/12/08		<新規>
	//================================================================================
	void C3D_MOVIE_CAMERA::setViewPointSpline(D3DXVECTOR3 *inLoc, int inVertex, BOOL inLoop)
	{
		float fLocX[MaxSplineSize];
		float fLocY[MaxSplineSize];
		float fLocZ[MaxSplineSize];

		for(int i = 0; i < inVertex; ++i)
		{
			fLocX[i] = inLoc[i].x;
			fLocY[i] = inLoc[i].y;
			fLocZ[i] = inLoc[i].z;
		}

	//	スプラインの設定
		this->vCameraViewPoint.x.setSplineDate(fLocX, inVertex);	//頂点X
		this->vCameraViewPoint.y.setSplineDate(fLocY, inVertex);	//頂点Y
		this->vCameraViewPoint.z.setSplineDate(fLocZ, inVertex);	//頂点Z
		this->bCameraViewPointLoop = inLoop;						//ループフラグの記録
		this->iCameraViewPointVertex = inVertex;					//頂点数
	}

	//================================================================================
	//!	メソッド名	C3D_MOVIE_CAMERA::setViewPointSpline
	//
	//	機能		カメラ視点の動きの設定(スプライン曲線せってい)
	//	引数		*inLoc		頂点配列
	//				inVertex	頂点数
	//				inLoop		ループフラグ
	//	更新		2007/12/08		<新規>
	//================================================================================
	BOOL C3D_MOVIE_CAMERA::doViewPointSpline(void)
	{
		//	終了判定
		//	まだ終了していない場合
		if(this->fCameraViewPointMedia < (float)(this->iCameraViewPointVertex - 1))
		{
			VEC3 vLoc;	//更新する頂点位置の保存用

		//	スプライン計算結果を取得する
			vLoc.x = this->vCameraViewPoint.x.getSplineCalc(this->fCameraViewPointMedia);
			vLoc.y = this->vCameraViewPoint.y.getSplineCalc(this->fCameraViewPointMedia);
			vLoc.z = this->vCameraViewPoint.z.getSplineCalc(this->fCameraViewPointMedia);

		//	位置を更新する
			this->setViewPoint(&vLoc);

		//	媒介変数を増やす
			this->fCameraViewPointMedia += this->fCameraViewPointSpeed;
			
			return FALSE;
		}
		//	終了している場合
		else
		{
		//	ループ判定
			//	ループを行う
			if(this->bCameraViewPointLoop)
			{
				this->fCameraViewPointMedia = 0.0f;
				return FALSE;
			}
			//	ループを行わない
			else
			{
				return TRUE;
			}
		}
	}
}