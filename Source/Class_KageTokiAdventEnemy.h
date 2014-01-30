//***********************************************************************************
//!	ファイル名		Class_KageTokiAdventEnemy.h
//
//	機能			アドベンチャーゲームモードの敵キャラクタークラス
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2009/01/16	<新規>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_KageTokiAdventCharacter.h"	//!< アドベントキャラクター

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CKAGETOKI_ADVENT_ENEMY
	//
	//	機能			アドベンチャーゲームモードの敵キャラクタークラス
	//	更新			2009/01/16	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CKAGETOKI_ADVENT_ENEMY : public CKAGETOKI_ADVENT_CHARACTER
	{
	protected:		//-------------------- protected --------------------//

				int					iMaxLife;					//!< 最大体力
				int					iLife;						//!< 残り体力
				MY_CUBE				controlAreaOffset;			//!< 制御領域のオフセット
				MY_CUBE				controlArea;				//!< 制御領域
				MY_CUBE				actionArea;					//!< 行動領域

			//==================================================================
			//	外部ポインター
			//==================================================================
				CKAGETOKI_ADVENT_CHARACTER**	player;			//!< プレイヤー

				void	opeTurn(void);							//!< 方向転換を向く

	public:			//-------------------- public --------------------//

				CKAGETOKI_ADVENT_ENEMY	(
					CONST DEV	inDev,
					CONST LPSTR	inName,
					CONST LPSTR	inResource = PATH_SKINMESH,
					CONST LPSTR	inTexture  = PATH_SKINMESHTEX);	//!< コンストラクタ
		virtual	~CKAGETOKI_ADVENT_ENEMY(void);					//!< デストラクタ

			//==================================================================
			//	access	プレイヤーの外部ポインター
			//==================================================================
				void	setTargetPlayer(CKAGETOKI_ADVENT_CHARACTER** inPlayer)
				{
					this->player	= inPlayer;
				}

			//==================================================================
			//	access	体力
			//==================================================================
				int		getMaxLife(void)const	{ return	iMaxLife;	}
				int		getLife(void)const		{ return	iLife;		}

			//==================================================================
			//	access	制御領域
			//==================================================================
				void		setControlAreaOffset(float inLeft,   float inRight,
												 float inBottom, float inTop,
												 float inRear,   float inFront);
				void		setControlAreaOffset(MY_CUBE* inCube)
				{	controlAreaOffset	= *inCube;	}
				MY_CUBE*	getControlArea(void);

			//==================================================================
			//	access	行動領域
			//==================================================================
				void	setActionArea(VEC3* inLoc, VEC3* inSize);
				void	setActionArea(float inLeft,   float inRight,
									  float inBottom, float inTop,
									  float inRear,   float inFront);
				void	setActionArea(MY_CUBE* inCube)
				{	actionArea	= *inCube;	}

			//==================================================================
			//	敵の処理
			//==================================================================
				void	resetParameter(void);
	};
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------