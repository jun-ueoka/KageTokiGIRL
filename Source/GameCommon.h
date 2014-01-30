//***********************************************************************************
//!	ファイル名		GameCommon.h
//
//	機能			このゲームで共有するヘッダー、マクロ、列挙体、構造体の宣言群
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/04/17	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_3DObjectCamera.h"		//!< 3Dオブジェクトカメラ
#include	"Class_3DObjectGroup.h"			//!< 3Dオブジェクトグループ
#include	"Class_SkinMesh.h"				//!< 3Dオブジェクトスキンメッシュ
#include	"Class_3DMap.h"					//!< 3Dオブジェクトマップ
#include	"Class_Texture.h"				//!< 3Dオブジェクトテクスチャ
#include	"Class_FontBase.h"				//!< フォントベース
#include	"Class_ShadowObject.h"			//!< 影物体
#include	"Class_GraphicDevice.h"			//!< グラフィックデバイス
#include	"UtilityFunction.h"				//!< 共有関数
#include	"Class_DirectInput.h"			//!< 入力オブジェクト
#include	"Class_3DEffectCenter.h"		//!< エフェクトセンター

#include	"Class_Comment.h"				//!< コメントオブジェクト
#include	"Class_GameMap.h"				//!< マップ

using namespace	O3D;						//!< 3Dオブジェクト
using namespace GD;							//!< グラフィックデバイス
using namespace	SHA;						//!< シェーダ
using namespace UT;							//!< 共有関数