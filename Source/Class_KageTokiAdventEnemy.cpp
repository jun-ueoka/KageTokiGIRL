//***********************************************************************************
//!	ファイル名		Class_KageTokiAdventEnemy.cpp
//
//	機能			アドベンチャーゲームモードの敵キャラクタークラス
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2009/01/16	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_KageTokiAdventEnemy.h"

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_ENEMY::コンストラクタ
	//
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		スキンメッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2009/01/16		<新規>
	//================================================================================
	CKAGETOKI_ADVENT_ENEMY::CKAGETOKI_ADVENT_ENEMY(
		CONST DEV	inDev,
		CONST LPSTR	inName,
		CONST LPSTR	inResource,
		CONST LPSTR	inTexture)	:
	CKAGETOKI_ADVENT_CHARACTER(inDev, inName, inResource, inTexture)
	{
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_ENEMY::デストラクタ
	//
	//	更新		2009/01/16		<新規>
	//================================================================================
	CKAGETOKI_ADVENT_ENEMY::~CKAGETOKI_ADVENT_ENEMY(void)
	{
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_ENEMY::opeTurn
	//
	//	機能		方向転換処理
	//	更新		2009/01/16		<新規>
	//================================================================================
	void
	CKAGETOKI_ADVENT_ENEMY::opeTurn(void)
	{
	//	向きの制御
		switch(this->way)
		{
		//	左を向く途中
			case eADCW_LEFT_TURN:
				this->setAngleY(ADVENT_LEFTANGLE);
				this->way = eADCW_LEFT;
				break;

		//	右を向く途中
			case eADCW_RIGHT_TURN:
				this->setAngleY(ADVENT_RIGHTANGLE);
				this->way = eADCW_RIGHT;
				break;

		//	初期状態
			case eADCW_NO:
			//	左寄りの向きになっているなら、左向きに設定する
				if(	(this->getAngleY() >=   0.0f) &&
					(this->getAngleY() <  180.0f))
				{
					this->setAngleY(ADVENT_LEFTANGLE);
					this->way = eADCW_LEFT;
				}
			//	上記以外は、右向きに設定する
				else
				{
					this->setAngleY(ADVENT_RIGHTANGLE);
					this->way = eADCW_RIGHT;
				}
				break;

		//	上記以外は、処理をしない
			default:	
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_ENEMY::setControlAreaOffset
	//
	//	機能		制御領域のオフセットを設定
	//	引数		左右上下前後の領域幅
	//	更新		2009/01/16		<新規>
	//================================================================================
	void
	CKAGETOKI_ADVENT_ENEMY::setControlAreaOffset(
		float inLeft,   float inRight,
		float inBottom, float inTop,
		float inRear,   float inFront)
	{
		controlAreaOffset.left   = inLeft;
		controlAreaOffset.right  = inRight;
		controlAreaOffset.bottom = inBottom; 
		controlAreaOffset.top    = inTop;
		controlAreaOffset.rear   = inRear;
		controlAreaOffset.front  = inFront;
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_ENEMY::getControlArea
	//
	//	機能		制御領域のオフセットを受け取る
	//	戻り値		制御領域を補正した領域ポインター
	//	更新		2009/01/16		<新規>
	//================================================================================
	MY_CUBE*
	CKAGETOKI_ADVENT_ENEMY::getControlArea(void)
	{
		controlArea.left	= getLocX() + controlAreaOffset.left;
		controlArea.right	= getLocX() + controlAreaOffset.right;
		controlArea.bottom	= getLocY() + controlAreaOffset.bottom; 
		controlArea.top		= getLocY() + controlAreaOffset.top;
		controlArea.rear	= getLocZ() + controlAreaOffset.rear;
		controlArea.front	= getLocZ() + controlAreaOffset.front;
		return	&controlArea;
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_ENEMY::setActionArea
	//
	//	機能		行動領域を設定する
	//	更新		2009/01/16		<新規>
	//================================================================================
	void
	CKAGETOKI_ADVENT_ENEMY::setActionArea(VEC3* inLoc, VEC3* inSize)
	{
		actionArea.left		= inLoc->x - inSize->x;
		actionArea.right	= inLoc->x + inSize->x;
		actionArea.bottom	= inLoc->y - inSize->y; 
		actionArea.top		= inLoc->y + inSize->y;
		actionArea.rear		= inLoc->z - inSize->z;
		actionArea.front	= inLoc->z + inSize->z;
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_ENEMY::setActionArea
	//
	//	機能		行動領域を設定する(完全指定)
	//	更新		2009/01/16		<新規>
	//================================================================================
	void
	CKAGETOKI_ADVENT_ENEMY::setActionArea(
		float inLeft,   float inRight,
		float inBottom, float inTop,
		float inRear,   float inFront)
	{
		actionArea.left   = inLeft;
		actionArea.right  = inRight;
		actionArea.bottom = inBottom; 
		actionArea.top    = inTop;
		actionArea.rear   = inRear;
		actionArea.front  = inFront;
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_ENEMY::resetParameter
	//
	//	機能		行動領域を設定する(完全指定)
	//	更新		2009/02/10		<新規>
	//================================================================================
	void
	CKAGETOKI_ADVENT_ENEMY::resetParameter(void)
	{
		this->iLife	= this->iMaxLife;
		this->setVisible(TRUE);
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------