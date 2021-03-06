//***********************************************************************************
//!	ファイル名		Class_Spline.h
//
//	機能			スプライン計算をするクラスの定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/06/04	<新規>
//					2008/09/15	<修正>	コメント
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"DirectX.h"				//!< DirectX使用

//-----------------------------------------------------------------------
//!	スプラインの名前空間
//-----------------------------------------------------------------------
namespace SPL
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CSPLINE
	//
	//	機能			スプライン（ベジェ曲線）
	//	更新			2008/09/15	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CSPLINE
	{
	private:		//-------------------- private --------------------//

				int				iMaxCnt;		//!< 三点間の頂点数
				D3DXVECTOR3		*vSpline;		//!< スプライン頂点配列

	public:			//-------------------- public --------------------//

				CSPLINE() : iMaxCnt(0), vSpline(NULL)	{					}
		virtual	~CSPLINE()								{ releaseSpline();	}

			//==================================================================
			//	最大数をセット
			//==================================================================
				void	setMax(DWORD inMax);

			//==================================================================
			//	スプライン計算(最大数を再設定可能)
			//==================================================================
				void	setUp(VEC3* inPosFirst, VEC3* inPosSecond, VEC3* inPosThird, DWORD inMax = 0);

			//==================================================================
			//	配列要素数を指定して頂点を受け取る
			//==================================================================
				VEC3*	getPos(DWORD inTarget);

			//==================================================================
			//	全ての頂点を受け取る
			//==================================================================
				VEC3*	getAllPos(void)			{ return	vSpline;			}

			//==================================================================
			//	スプライン頂点の解放
			//==================================================================
				void	releaseSpline(void)		{ iMaxCnt	= 0;
												  SAFE_DELETE_ARRAY(vSpline);	}
	};
}
//-----------------------------------------------------------------------
//!	スプラインの名前空間
//-----------------------------------------------------------------------