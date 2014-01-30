//***********************************************************************************
//!	ファイル名		Precompiler.h
//
//	機能			プリコンパイルヘッダー
//	記述			上岡　純
//	プロジェクト	未定
//***********************************************************************************

#pragma once

#include <TCHAR.h>
#include <stdio.h>
#include <stdlib.h>			// <<<<
#include <windows.h>
#include <time.h>

#include <algorithm>		
#include <functional>      // For greater<int>( )

#include <iostream>
#include <vector>
#include <d3dx9.h>
#include <d3dx9mesh.h>

//-----------------------------------------------------------------------
//!	プリコンパイル列挙
//-----------------------------------------------------------------------
//	環境関連
#include	"DirectX.h"
#include	"Class_GraphicDevice.h"
#include	"Class_OggVorbis.h"
#include	"Class_Sound.h"
#include	"Class_Sound_OggVorbis.h"
#include	"Class_DirectInput.h"
#include	"MySystem.h"
//#include	"Class_ZTextureShader.h"
//#include	"Class_DepthBufShader.h"
#include	"UtilityFunction.h"
#include	"Class_WindowManager.h"
#include	"Class_Direction.h"

//	オブジェクト関連
#include	"Class_3DObject.h"
#include	"Class_3DObjectCamera.h"
#include	"Class_3DEffectCenter.h"
#include	"Class_3DEffect.h"
#include	"Class_3DObjectGroup.h"
#include	"Class_Mesh.h"
#include	"Class_SkinMesh.h"
#include	"Class_Texture.h"
#include	"Class_EffectBase.h"
#include	"Class_FontBase.h"
#include	"Class_MeshBase.h"
#include	"Class_SkinMeshBase.h"
#include	"Class_TextureBase.h"
#include	"Class_Spline.h"
#include	"Class_3DMap.h"
#include	"Class_MovieCamera.h"
#include	"Class_SaveResource.h"

//	ゲーム関連
#include	"GameCommon.h"
#include	"Class_GameApplication.h"
#include	"Class_KageTokiAdventCharacter.h"
#include	"Class_KageTokiAdventEnemy.h"
#include	"Class_KageTokiAdventBoss.h"
#include	"Class_AdventKageo.h"
#include	"Class_AdventKagemi.h"
#include	"Class_KageoBall.h"
#include	"Class_KagemiBall.h"
#include	"Class_AdventPlayer.h"
#include	"Class_EnemyBall.h"
#include	"Class_BossExe.h"
#include	"Class_YamakageWeapon.h"
#include	"Class_BossYamakage.h"
#include	"Class_BossFeather.h"
#include	"Class_BossTima.h"
#include	"Class_BossClime.h"
#include	"Class_EnemyShadoll.h"
#include	"Class_EnemyShadollSword.h"
#include	"Class_EnemyShadog.h"
#include	"Class_EnemyShadollArchery.h"
#include	"Class_AdventEnemyManager.h"

#include	"Class_Comment.h"
#include	"Class_GameMap.h"
#include	"Class_ShadowObject.h"
#include	"Class_BackSky.h"

#include	"Class_Scene.h"
#include	"Class_SceneEvent.h"
#include	"Class_SceneLogo.h"
#include	"Class_SceneDemo.h"
#include	"Class_SceneTitle.h"
#include	"Class_SceneEnding.h"
#include	"Class_SceneGameOver.h"
#include	"Class_SceneMovieBase.h"
#include	"Class_SceneAdventureGame.h"
#include	"Class_SceneAdventureGameData.h"

#include	"Class_SceneMovie1.h"
#include	"Class_SceneMovie2.h"
#include	"Class_SceneMovie3.h"
#include	"Class_SceneMovie4.h"
#include	"Class_SceneMovie5.h"
#include	"Class_SceneMovie6.h"
#include	"Class_SceneMovie7.h"
#include	"Class_SceneMovie8.h"
#include	"Class_SceneMovie9.h"
#include	"Class_SceneMovie10.h"
#include	"Class_SceneMovie11.h"
#include	"Class_SceneMovie12.h"
#include	"Class_SceneStage1.h"
#include	"Class_SceneStage2.h"
#include	"Class_SceneStage3.h"
#include	"Class_SceneStage4.h"
#include	"Class_SceneStage5.h"
#include	"Class_SceneStage6.h"
#include	"Class_SceneStage7.h"
#include	"Class_SceneStage8.h"
#include	"Class_SceneStage9.h"
#include	"Class_SceneStage10.h"
#include	"Class_SceneBoss1.h"
#include	"Class_SceneBoss2.h"
#include	"Class_SceneBoss3.h"
#include	"Class_SceneBoss4.h"
#include	"Class_SceneBoss5.h"

//リソース
#include	"..//Project_Games//resource.h"

// 最後に
// メモリリークを調査するための記述
//┏━【218】「new を DEBUG_NEW に置き換える定義」のないソースファイル>>
#include <crtdbg.h>
#ifdef _DEBUG
#define new					new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s)			_malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define calloc(c, s)		_calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define realloc(p, s)		_realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define _recalloc(p, c, s)	_recalloc_dbg(p, c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define _expand(p, s)		_expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
//┗━【219】「new を DEBUG_NEW に置き換える定義」のないソースファイル　ここまで　━━━━━━━━━━━━━━━━━━━━━━┛