//***********************************************************************************
//!	ファイル名		Class_3DMap.h
//
//	機能			3Dオブジェクトで形成されたマップの定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/07/20	<新規>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_3DObject.h"				//!< 3Dオブジェクト
#include	"Class_3DObjectGroup.h"			//!< 3Dオブジェクトグループ

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	メッシュリソースファイルへのパス
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_MAP		"..//Resources//Data//Map//Map%d.dat"

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	オブジェクトタイプ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum	GAMEMAP_OBJECT_TYPE
	{
		eGMO_NORMALMESH	= 0,			//!< 通常メッシュ				//!< MESH______ 
		eGMO_NORMALSKINMESH,			//!< 通常スキンメッシュ			//!< SKIN______
		eGMO_NORMALTEXTURE_CENTER,		//!< 通常テクスチャ(頂点が中心)	//!< TEX_CEN___
		eGMO_NORMALTEXTURE_BOTTOM,		//!< 通常テクスチャ(頂点が下)	//!< TEX_BOT___
		eGMO_SHADOWOBJ_COM_SUBSTANCE,	//!< 影オブジェクト(完全実体)	//!< SO_COMP___
		eGMO_SHADOWOBJ_HALF_SHADOW,		//!< 影オブジェクト(半影物体)	//!< SO_HALFSHA
		eGMO_SHADOWOBJ_HALF_REAL,		//!< 影オブジェクト(半実体)		//!< SO_HALFREA
		eGMO_SHADOWOBJ_INCOM_SUBSTANCE,	//!< 影オブジェクト(未完全実体)	//!< SO_INCOMP_
		eGMO_MAX						//!< オブジェクトタイプの最大数

	}	GMO_TYPE;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	通常グループタイプ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum	MAP_GROUP_TYPE
	{
		eMG_VIEW_HIT	= 0,			//!< 描画と衝突					//!< VIEW_HIT__
		eMG_VIEW,						//!< 描画						//!< VIEW______
		eMG_BACK_VIEW_HIT,				//!< 背景描画と衝突				//!< BACK_VIEWH
		eMG_BACK_VIEW,					//!< 背景描画					//!< BACK_VIEW_
		eMG_HIT,						//!< 衝突						//!< HIT_______
		eMG_ENEMY,						//!< 敵							//!< ENEMY_____
		eMG_MAX							//!< タイプの最大数
										//!< データの終端				//!< END_______
	}	MG_TYPE;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	生成オブジェクトデータ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	struct	CREATE_OBJECT_DATA
	{
		char	szData[MAX_PATH];		//!< データタイプ
		char	szName[MAX_PATH];		//!< 外部ファイル名
		VEC3	loc;					//!< 位置
		VEC3	angle;					//!< 角度
		VEC2	scale;					//!< 拡大比

	}	OB_DATA;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		C3D_MAP
	//
	//	機能			3Dオブジェクトで形成されたマップ
	//	機能概要		中心オブジェクトと、範囲を設定して、
	//					範囲の中に入っているオブジェクトを描画し、
	//					入っていないオブジェクトは描画しない
	//	更新			2008/07/20	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class C3D_MAP
	{
	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	描画ステータス
			//==================================================================
				float					fDistance;			//!< 描画許容半径

				C3D_OBJECT_GROUP		releaseG;			//!< 解放グループ
				C3D_OBJECT_GROUP		viewG;				//!< 描画グループ
				C3D_OBJECT_GROUP		backG;				//!< 背景グループ
				C3D_OBJECT_GROUP		hitG;				//!< 衝突グループ

				C3D_OBJECT_GROUP		squeezeViewG;		//!< 絞り込み描画グループ
				C3D_OBJECT_GROUP		squeezeBackG;		//!< 絞り込み背景グループ

			//==================================================================
			//	オブジェクト生成
			//==================================================================
		virtual	BOOL	createObject(CONST DEV	  inDev,
									 C3D_OBJECT** inObj,
									 OB_DATA*	  inData)	{ return FALSE;	}

			//==================================================================
			//	生成したオブジェクトのグループ登録
			//==================================================================
		virtual	BOOL	addObject(C3D_OBJECT* inObject, int inGroupType);

			//==================================================================
			//	オブジェクトのメッセージをチェック
			//==================================================================
		virtual	BOOL	checkMessage(vector<C3D_OBJECT*>::iterator* inObjP);

	public:			//-------------------- public --------------------//

				C3D_MAP(void);								//!< コンストラクタ
		virtual	~C3D_MAP(void);								//!< デストラクタ

			//==================================================================
			//	access	描画ステータス
			//==================================================================
				float		getDistance(void)const			{ return	fDistance;		}
				void		setDistance(float inDistance)	{ fDistance = inDistance;	}

			//==================================================================
			//	access	各グループ
			//==================================================================
				C3D_OBJECT_GROUP*	getViewG(void)					{ return	&viewG;	}
				C3D_OBJECT_GROUP*	getBackG(void)					{ return	&backG;	}
				C3D_OBJECT_GROUP*	getHitG(void)					{ return	&hitG;	}
				void				addViewG(C3D_OBJECT* inObj)		{ viewG.add(inObj);	}
				void				addBackG(C3D_OBJECT* inObj)		{ backG.add(inObj);	}
				void				addHitG(C3D_OBJECT* inObj)		{ hitG.add(inObj);	}

			//==================================================================
			//	準備、解放
			//==================================================================
				void	initialize(CONST DEV inDev,
								   int		 inTxtNum);		//!< 外部読み込み
		virtual	void	finalize(void);						//!< 解放

			//==================================================================
			//	描画
			//==================================================================
				void	draw(CONST DEV inDev, C3D_OBJECT* inCamera);
				void	draw(CONST DEV inDev, C3D_OBJECT* inCamera, VEC3* inViewPoint);
				void	draw(CONST DEV inDev, VEC3* inViewPoint);
				void	draw(CZTEX_SHA*  inZTexSha,  VEC3* inViewPoint);
				void	draw(CDEPTH_SHA* inDepthSha, VEC3* inViewPoint);
				void	drawNotCheckMessage(CDEPTH_SHA* inDepthSha, VEC3* inViewPoint);

			//==================================================================
			//	絞り込み処理
			//==================================================================
				void	squeezeSortByCameraPosition(C3D_OBJECT* inCamera, VEC3* inViewPoint);
				void	squeezeSortByCameraPositionWithDrawZTexSha(
							C3D_OBJECT* inCamera, VEC3* inViewPoint, CZTEX_SHA*  inZTexSha);
				void	squeezeSortByCameraPositionNotCheckMessageWithDrawZTexSha(
							C3D_OBJECT* inCamera, VEC3* inViewPoint, CZTEX_SHA*  inZTexSha);
				void	squeezeDrawWithZTexSha(CZTEX_SHA*  inZTexSha);
				void	squeezeDrawWithDepthSha(CDEPTH_SHA* inDepthSha);
	};
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------