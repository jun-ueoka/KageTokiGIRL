//***********************************************************************************
//!	ファイル名		Class_GameMap.cpp
//
//	機能			このゲーム専用の3Dマップの実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2008/07/25	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_GameMap.h"				//!< 3Dオブジェクトマップ

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CGAME_MAP::コンストラクタ
	//
	//	引数		inDev		Direct3Dデバイス
	//				inCenter	マップの中心となるオブジェクト
	//				inTxtNum	テキスト番号
	//	更新		2008/07/20	<新規>
	//================================================================================
	CGAME_MAP::CGAME_MAP(void)
	:	C3D_MAP(),
		friendG(),
		enemyG()
	{
		this->fEnemyGDistance	= this->fDistance;
	}

	//================================================================================
	//!	メソッド名	CGAME_MAP::コンストラクタ(全初期化)
	//
	//	引数		inDev		Direct3Dデバイス
	//				inCenter	マップの中心となるオブジェクト
	//				inTxtNum	テキスト番号
	//	更新		2008/07/20	<新規>
	//================================================================================
	CGAME_MAP::CGAME_MAP(CONST DEV inDev, int inTxtNum)
	:	C3D_MAP(),
		friendG(),
		enemyG()
	{
	//	準備
		this->initialize(inDev, inTxtNum);
	}

	//================================================================================
	//!	メソッド名	CGAME_MAP::デストラクタ
	//
	//	更新		2008/07/20	<新規>
	//================================================================================
	CGAME_MAP::~CGAME_MAP(void)
	{
	//	解放
		this->finalize();
	}

	//================================================================================
	//!	メソッド名	CGAME_MAP::finalize
	//
	//	機能		解放
	//	更新		2008/07/20	<新規>
	//================================================================================
	void
	CGAME_MAP::finalize(void)
	{
	//	先にスーパークラスの解放
		C3D_MAP::finalize();

	//	解放グループの解放
		this->friendG.clear();					//!< 味方グループの消去
		this->enemyG.clear();					//!< 敵グループの消去
	}

	//================================================================================
	//!	メソッド名	CGAME_MAP::drawEnemyG
	//
	//	機能		敵グループの描画
	//	引数		inDev			Direct3Dデバイス
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CGAME_MAP::drawEnemyG(CONST DEV inDev, VEC3* inViewPoint)
	{
		vector<C3D_OBJECT*>::iterator	p;			//!< 作業用イテレータ

	//	検査しながら描画するため、とりだして描画
		p	= this->enemyG.getBegin();
		while(p != this->enemyG.getEnd())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	this->checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < this->fEnemyGDistance)
				{
					(*p)->draw(inDev);
				}
				p++;
			}
		}
	}

	//================================================================================
	//!	メソッド名	CGAME_MAP::addObject
	//
	//	機能		生成したオブジェクトのグループ登録
	//	引数		inObject		登録するオブジェクト
	//				inGroupType		登録するグループタイプ
	//	戻り値		TRUE			登録完了
	//				FALSE			登録未完了
	//	更新		2008/07/20		<新規>
	//================================================================================
	BOOL
	CGAME_MAP::addObject(C3D_OBJECT* inObject, int inGroupType)
	{
	//	グループタイプ分岐
		switch(inGroupType)
		{
			case	eMG_ENEMY:								//!< 敵
				this->enemyG.add(inObject);
				return	TRUE;

			default:										//!< 上記以外
				return	C3D_MAP::addObject(inObject, inGroupType);
		}
	}

	//================================================================================
	//!	メソッド名	CGAME_MAP::checkMessage
	//
	//	機能		オブジェクトのメッセージをチェック
	//	引数		*inObjP			オブジェクトイテレータ
	//	更新		2008/07/20		<新規>
	//================================================================================
	BOOL
	CGAME_MAP::checkMessage(vector<C3D_OBJECT*>::iterator* inObjP)
	{
		C3D_OBJECT*	inObj	= (*(*inObjP));					//!< ポインター格納
	//	状態検査
		switch((*(*inObjP))->getOrderMessage())
		{
			case	eOMSG_DELETE_IN:						//!< 消滅準備
				this->releaseG.add((*(*inObjP)));			//!< 消滅グループに入れる
				(*(*inObjP))->setOrderMessage(eOMSG_NOMSG);	//!< メッセージを消す
				return	TRUE;

			case	eOMSG_DELETE:							//!< 消滅命令
				this->releaseG.erase(inObj);				//!< 二重開放を防ぐ為
				this->hitG.erase(inObj);					//!< 衝突グループから削除
				this->enemyG.erase(inObj);					//!< 敵グループから削除
				(*inObjP)	= this->viewG.erase(*inObjP);	//!< 描画グループから消去
				SAFE_DELETE(inObj);							//!< 解放
				return	FALSE;

			default:										//!< 上記以外
				return	TRUE;
		}
	}

	//===============================================================================
	//!	メソッド名	CGAME_MAP::コンストラクタ
	//
	//	引数		inDev		Direct3Dデバイス
	//				inObj		格納先
	//				inData		生成データ
	//	戻り値		TRUE		生成成功
	//				FALSE		生成失敗
	//	更新		2008/07/20	<新規>
	//================================================================================
	BOOL
	CGAME_MAP::createObject(CONST DEV		inDev,
							C3D_OBJECT**	inObj,
							OB_DATA*		inData)
	{
		CTEXTURE*	newTex			= NULL;			//!< テクスチャポインター確保
		int			iObjType		= 0;			//!< オブジェクトタイプ

	//	グループタイプ判定配列
		char	objJudger[eGMO_MAX][11]	=
		{
			"MESH______",		//!< 通常メッシュ
			"SKIN______",		//!< 通常スキンメッシュ
			"TEX_CEN___",		//!< 通常テクスチャ(頂点が中心)
			"TEX_BOT___",		//!< 通常テクスチャ(頂点が下)
			"SO_COMP___",		//!< 影オブジェクト(完全実体)
			"SO_HALFSHA",		//!< 影オブジェクト(半影物体)
			"SO_HALFREA",		//!< 影オブジェクト(半実体)
			"SO_INCOMP_",		//!< 影オブジェクト(未完全実体)
		};

	//	グループ判定
		for(iObjType = 0; iObjType < eGMO_MAX; iObjType++)
		{
			if(	strcmp(inData->szData, objJudger[iObjType]) == 0)	break;
		}

	//	生成オブジェクト分岐
		switch(iObjType)
		{
			case eGMO_NORMALMESH:							//!< 通常メッシュ
				(*inObj)	= new CMESH(inDev, inData->szName);
				(*inObj)->setLoc(&inData->loc);
				(*inObj)->setAngle(&inData->angle);
				(*inObj)->setScale(inData->scale.x);
				return	TRUE;

			case eGMO_NORMALSKINMESH:						//!< 通常スキンメッシュ
				(*inObj)	= new CSKIN_MESH(inDev, inData->szName);
				(*inObj)->setLoc(&inData->loc);
				(*inObj)->setAngle(&inData->angle);
				(*inObj)->setScale(inData->scale.x);
				return	TRUE;

			case eGMO_NORMALTEXTURE_CENTER:					//!< 通常テクスチャ(頂点が中心)
				newTex		= new CTEXTURE(inDev, inData->szName, inData->scale);
				newTex->setLoc(&inData->loc);
				newTex->setAngle(&inData->angle);
				newTex->setVertexPosition(eVERP_CENTER);
				(*inObj)	= newTex;
				return	TRUE;

			case eGMO_NORMALTEXTURE_BOTTOM:					//!< 通常テクスチャ(頂点が下)
				newTex		= new CTEXTURE(inDev, inData->szName, inData->scale);
				newTex->setLoc(&inData->loc);
				newTex->setAngle(&inData->angle);
				newTex->setVertexPosition(eVERP_DOWN);
				(*inObj)	= newTex;
				return	TRUE;

			case eGMO_SHADOWOBJ_COM_SUBSTANCE:
				(*inObj)	= new CSHADOW_OBJECT(inDev, eSHAO_COMPLETE, inData->szName);
				(*inObj)->setLoc(&inData->loc);
				(*inObj)->setAngle(&inData->angle);
				(*inObj)->setScale(inData->scale.x);
				return	TRUE;

			case eGMO_SHADOWOBJ_HALF_SHADOW:
				(*inObj)	= new CSHADOW_OBJECT(inDev, eSHAO_HALF_SHADOW, inData->szName);
				(*inObj)->setLoc(&inData->loc);
				(*inObj)->setAngle(&inData->angle);
				(*inObj)->setScale(inData->scale.x);
				return	TRUE;

			case eGMO_SHADOWOBJ_HALF_REAL:
				(*inObj)	= new CSHADOW_OBJECT(inDev, eSHAO_HALF_REAL, inData->szName);
				(*inObj)->setLoc(&inData->loc);
				(*inObj)->setAngle(&inData->angle);
				(*inObj)->setScale(inData->scale.x);
				return	TRUE;

			case eGMO_SHADOWOBJ_INCOM_SUBSTANCE:
				(*inObj)	= new CSHADOW_OBJECT(inDev, eSHAO_INCOMPLETE, inData->szName);
				(*inObj)->setLoc(&inData->loc);
				(*inObj)->setAngle(&inData->angle);
				(*inObj)->setScale(inData->scale.x);
				return	TRUE;

			default:										//!< 上記以外
				(*inObj)	= NULL;
				return	FALSE;
		}
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------