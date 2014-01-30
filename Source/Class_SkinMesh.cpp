//***********************************************************************************
//!	ファイル名		Class_SkinMesh.cpp
//
//	機能			スキンメッシュを扱うクラスの実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/05/30	<追加>	コメント
//					2008/06/25	<追加>	全体的にコメントの強化
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SkinMesh.h"

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CSKIN_MESH::コンストラクタ
	//
	//	機能概要	Xファイルをロードして初期化
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		スキンメッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	CSKIN_MESH::CSKIN_MESH(CONST DEV   inDev,
						   CONST LPSTR inName,
						   CONST LPSTR inResource,
						   CONST LPSTR inTexture)	:
	CSKIN_MESH_BASE(inDev, inName, inResource, inTexture)
	{
	//	スーパークラスの初期化
		C3D_OBJECT::initialize();
	//	もしスキンメッシュの読み込み失敗していたら、存在確認をOFFにする
		if(	!bSkinLoaded)	setVisible(FALSE);
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH::コンストラクタ
	//
	//	機能		空初期化
	//	更新		2007/12/08		<新規>
	//================================================================================
	CSKIN_MESH::CSKIN_MESH(void)	:
	CSKIN_MESH_BASE()
	{
	//	スーパークラスの初期化
		C3D_OBJECT::initialize();
	}


	//================================================================================
	//!	メソッド名	CSKIN_MESH::コンストラクタ
	//
	//	機能概要	コピー
	//	引数		inCopy			コピー元
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	CSKIN_MESH::CSKIN_MESH(CSKIN_MESH* inCopy)	:
	CSKIN_MESH_BASE(inCopy)
	{
	//	スーパークラスの初期化
		C3D_OBJECT::initialize();
	//	もしスキンメッシュの読み込み失敗していたら、存在確認をOFFにする
		if(	!bSkinLoaded)	setVisible(FALSE);
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH::デストラクタ
	//
	//	機能概要	全ての資源を解放し、終了する。
	//				資源が、他のオブジェクトでも使われていた場合は残す。
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	CSKIN_MESH::~CSKIN_MESH()
	{
	//	スーパークラスの解放
		C3D_OBJECT::finalize();
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH::getWorldCenter
	//
	//	機能		ワールド座標系中心点を受け取る
	//	戻り値		ワールド座標系中心点
	//	更新		2007/12/08		<新規>
	//================================================================================
	VEC3
	CSKIN_MESH::getWorldCenter(void)
	{
	//	ワールド座標系中心点
		VEC3	aCenter;
	//	ワールド座標系の演算
		D3DXVec3TransformCoord(&aCenter, &sd_ptr->mesh->localCenter, getWorldMatrix());
	//	ワールド座標系中心点を返す
		return	aCenter;
	}

	//================================================================================
	//!	メソッド名	CSKIN_MESH::draw
	//
	//	機能		描画
	//	機能概要	スキンメッシュの描画を行う
	//	引数		inDev			Direct3Dデバイス
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2008/05/30		<追加>	コメント
	//================================================================================
	BOOL
	CSKIN_MESH::draw(CONST DEV inDev)
	{
	//	描画の無効性を調べて、無効の時
		if(	!getVisible())
		{
		//	描画失敗を返す
			return	FALSE;
		}

	//	モデルを前向きにする
		MAT16	matrix = (*getWorldMatrixInversed()) * (*getWorldMatrix());

		return	drawSkin(inDev, &matrix);
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------