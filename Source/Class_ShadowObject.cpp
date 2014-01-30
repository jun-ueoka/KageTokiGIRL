//***********************************************************************************
//!	ファイル名		Class_ShadowObject.cpp
//
//	機能			影物体に関する実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/08/29	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_ShadowObject.h"

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//	共有影物体データマップ
	//================================================================================
	map<string, SHADOW_OBJECT_DATA*>*		CSHADOW_OBJECT::mShadowData = NULL;

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::createShadowData
	//
	//	機能		影物体データをロードする。
	//	機能概要	影物体データをロードし、その影物体データを返す
	//				このメソッドはクラスメソッドである。
	//	引数		inDev			Direct3Dデバイス
	//				inName			影物体ファイルの名称
	//				inResource		影物体フォルダ名
	//	戻り値		影物体データのポインター
	//	更新		2008/08/31		<新規>
	//================================================================================
	SHADOW_OBJECT_DATA*
	CSHADOW_OBJECT::createShadowData(CONST DEV	 inDev,
									 CONST LPSTR inName,
									 CONST LPSTR inResource)
	{
	//	新たな影物体情報
		SHADOW_OBJECT_DATA*	newShadowObject		= new SHADOW_OBJECT_DATA();
		char				realName[MAX_PATH]	= "";				//!< テキスト名作成用

	//-------------------------------------------------
	//	ファイルを開く
	//-------------------------------------------------
		FILE*	fp	= NULL;

	//	ファイルを開く
		fopen_s(&fp, inName, "r");

	//	開けなかったら、パスを付けてもう一度
		if(	!fp)
		{
		//	テキスト名を生成
			sprintf_s(realName, sizeof(realName), inResource, inName);
		//	ファイルを開く
			fopen_s(&fp, realName, "r");
		//	ファイルを開けなかった時
			if(	!fp)
			{
			//	読み込みを行わず、NULLを返す処理へ
				DX::errorMsgNoFile(inDev, realName);
				goto error;
			}
		}

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	ファイルを開けたら、読み込み開始
	//-------------------------------------------------
	//	一括読み込み
	//-------------------------------------------------
		char	szShadowObjName[MAX_PATH]	= "";				//!< 影名称
		char	szHitObjName[MAX_PATH]		= "";				//!< 衝突判定用メッシュ名称

	//	一挙に読み込む
		fscanf_s(fp, "%s %s ",
			 szShadowObjName,	sizeof(szShadowObjName),		//!< 影名称
			 szHitObjName,		sizeof(szHitObjName));			//!< 衝突判定用メッシュ名称

	//	影物体メッシュが指定されてたら、生成
		if(	strcmp(szShadowObjName, NONE_OBJECT) != 0)
		{
		//	影物体メッシュの生成に失敗したら、NULLを返す
			newShadowObject->shadowObj	= new CMESH_BASE(inDev, szShadowObjName);
			if(	newShadowObject->shadowObj->meshInvalid())
			{
				DX::errorMsgNoFile(inDev, szShadowObjName);
				goto error;
			}
		}

	//	衝突判定用メッシュが指定されてたら、生成
		if(	strcmp(szHitObjName, NONE_OBJECT) != 0)
		{
		//	衝突判定用メッシュの生成に失敗したら、NULLを返す
			newShadowObject->hitObj		= new CMESH_BASE(inDev, szHitObjName);
			if(	newShadowObject->shadowObj->meshInvalid())
			{
				DX::errorMsgNoFile(inDev, szHitObjName);
				goto error;
			}
		}

		fclose(fp);
	//	作成した影物体の名前を登録する
		newShadowObject->name	= inName;
	//	作成した影物体の情報を返す
		return	newShadowObject;

	//	読み込み終了
	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	//	NULLを返す処理
	error:
		SAFE_DELETE(newShadowObject);	//!< 生成した影物体情報を開放
		if(	fp)fclose(fp);
		return	NULL;					//!< NULLを返す
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::startShadowObjectInitialize
	//
	//	機能		属性の初期化
	//	更新		2008/12/03		<新規>
	//================================================================================
	void
	CSHADOW_OBJECT::startShadowObjectInitialize(void)
	{
	//	スーパークラスの初期化
		C3D_OBJECT::initialize();

		this->so_ptr					= NULL;			//!< 影物体情報
		this->bShadowObjectMeshLoaded	= FALSE;		//!< 影物体ロード確認
		this->shadowObjType				= eSHAO_NONE;	//!< 影物体構成タイプ
		this->shadowObjAction			= eSHAOAC_NONE;	//!< 影物体動作
		this->shadowDiffuse.r			= 1.0f;			//!< ディフューズr
		this->shadowDiffuse.g			= 1.0f;			//!< ディフューズg
		this->shadowDiffuse.b			= 1.0f;			//!< ディフューズb
		this->shadowDiffuse.a			= 1.0f;			//!< ディフューズa
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::initializeShadowObject
	//
	//	機能		影物体の初期化
	//	引数		inDev			Direct3Dデバイス
	//				inType			影物体構成タイプ
	//				inName			影物体ファイルの名称
	//				inResource		影物体フォルダ名
	//	更新		2008/12/03		<新規>
	//================================================================================
	void
	CSHADOW_OBJECT::initializeShadowObject(CONST DEV		inDev,
										   SHADOW_OBJ_TYPE	inType,
										   CONST LPSTR		inName,
										   CONST LPSTR		inResource)
	{
	//	影物体の初期解放
		this->finishalizeShadowObject();

	//	影物体マップが無ければ生成する
		if(	!mShadowData)
		{
			mShadowData = new map<string, SHADOW_OBJECT_DATA*>;
		}

	//	作業用イテレータ
		map<string, SHADOW_OBJECT_DATA*>::iterator	p;
	//	影物体マップに影物体ファイル名を渡し、そのイテレータを受け取る
		p = mShadowData->find(inName);

	//	影物体マップに同じ名前の影物体ファイル名が既に登録してある場合
		if(	p != mShadowData->end())
		{
		//	使用カウンタを増やす
			p->second->cnt++;
		//	影物体情報を受け取る
			this->so_ptr	= (*p).second;
		//	ロード完了
			this->bShadowObjectMeshLoaded	= TRUE;
		}
		else
		{
		//	新たに影物体を生成する
			this->so_ptr	= this->createShadowData(inDev, inName, inResource);

		//	生成に成功した場合
			if(	this->so_ptr)
			{
			//	使用カウンタを増やす
				this->so_ptr->cnt++;
			//	ロード完了
				this->bShadowObjectMeshLoaded	= TRUE;
			//	影物体マップに登録
				mShadowData->insert(
					pair<string, SHADOW_OBJECT_DATA*>(this->so_ptr->name, this->so_ptr));
			}
		//	失敗した場合
			else
			{
			//	ロード未完了
				this->bShadowObjectMeshLoaded	= FALSE;
			}
		}

	//	影物体情報を持っている時
		if(	this->bShadowObjectMeshLoaded)
		{
		//	残りを初期化
			this->setShadowObjType(inType);
		}
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::finishalizeShadowObject
	//
	//	機能		影物体の解放
	//	更新		2008/12/03		<新規>
	//================================================================================
	void
	CSHADOW_OBJECT::finishalizeShadowObject(void)
	{
	//	スーパークラスの初期化
		C3D_OBJECT::finalize();
	//	影物体情報をロードしてないなら、処理を返す
		if(	!this->bShadowObjectMeshLoaded)		return;

	//	影物体カウントを減らす
		this->so_ptr->cnt--;
	//	この影物体がまだ使用されている場合は解放しない
		if(	this->so_ptr->cnt > 0)	return;

	//	影物体マップイテレータ
		map<string, SHADOW_OBJECT_DATA*>::iterator	p;
	//	影物体マップに影物体ファイル名を渡し、そのイテレータを受け取る
		p = mShadowData->find(this->so_ptr->name);
	//	影物体情報の解放
		SAFE_DELETE(p->second);
	//	影物体マップから削除
		mShadowData->erase(p);

	//	影物体マップにメンバーが無ければ、消去する
		if(	mShadowData->begin() == mShadowData->end())
		{
			SAFE_DELETE(mShadowData);
		}
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::コンストラクタ
	//
	//	機能		エフェクトファイルをロードして初期化
	//	引数		inDev			Direct3Dデバイス
	//				inType			影物体構成タイプ
	//				inName			影物体ファイルの名称
	//				inResource		影物体フォルダ名
	//	更新		2008/12/03		<新規>
	//================================================================================
	CSHADOW_OBJECT::CSHADOW_OBJECT(CONST DEV       inDev,
								   SHADOW_OBJ_TYPE inType,
								   CONST LPSTR     inName,
								   CONST LPSTR     inResource)
	{
	//	属性の初期化
		this->startShadowObjectInitialize();
	//	メッシュの初期化
		this->initializeShadowObject(inDev, inType, inName, inResource);
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::コンストラクタ
	//
	//	機能		空初期化
	//	更新		2008/12/03		<新規>
	//================================================================================
	CSHADOW_OBJECT::CSHADOW_OBJECT(void)
	{
	//	属性の初期化
		this->startShadowObjectInitialize();
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::コンストラクタ
	//
	//	機能		コピー
	//	引数		*inCopy			コピー元
	//	更新		2008/12/04		<新規>
	//================================================================================
	CSHADOW_OBJECT::CSHADOW_OBJECT(CSHADOW_OBJECT* inCopy)
	{
	//	属性の初期化
		this->startShadowObjectInitialize();
	//	コピー開始
		this->so_ptr	= inCopy->getShadowObjects();

	//	影物体情報がある時
		if(	this->so_ptr)
		{
		//	残りをコピー
			this->setShadowObjType(inCopy->getShadowObjType());
		//	使用カウンタを増やす
			this->so_ptr->cnt++;
		//	ロード完了
			this->bShadowObjectMeshLoaded	= TRUE;
		}
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::デストラクタ
	//
	//	更新		2008/12/04	<新規>
	//================================================================================
	CSHADOW_OBJECT::~CSHADOW_OBJECT(void)
	{
	//	影物体の解放
		this->finishalizeShadowObject();
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::setShadowObjType
	//
	//	機能		影物体の構成の設定
	//	引数		inType			新しい影構成メッセージ
	//	更新		2008/12/05		<新規>
	//================================================================================
	void
	CSHADOW_OBJECT::setShadowObjType(SHADOW_OBJ_TYPE inType)
	{
	//	影物体の構成を設定
		this->shadowObjType	= inType;
	//	新しい影物体構成によってサブメッセージを決定
		switch(this->shadowObjType)
		{
		//	影も実体もある物体
			case eSHAO_COMPLETE:
			//	影メッシュが生成されていなければ、消滅
				if(	!this->so_ptr->shadowObj)
				{
					this->setThisMessage(eTMSG_NOMSG);
					this->setOrderMessage(eOMSG_DELETE);
				}
				else
				{
					this->setThisMessage(eTMSG_NOMSG);
					this->shadowDiffuse.r	= 1.0f;
					this->shadowDiffuse.g	= 1.0f;
					this->shadowDiffuse.b	= 1.0f;
					this->shadowDiffuse.a	= 1.0f;
				}
				break;
		//	影がない物体
			case eSHAO_HALF_REAL:
			//	影メッシュが生成されていなければ、消滅
				if(	!this->so_ptr->shadowObj)
				{
					this->setThisMessage(eTMSG_NOMSG);
					this->setOrderMessage(eOMSG_DELETE);
				}
				else
				{
					this->setThisMessage(eTMSG_NO_SEE_SHADOW);
					this->shadowDiffuse.r	= 1.0f;
					this->shadowDiffuse.g	= 1.0f;
					this->shadowDiffuse.b	= 1.0f;
					this->shadowDiffuse.a	= 0.5f;
				}
				break;
		//	影だけの物体
			case eSHAO_HALF_SHADOW:
			//	影メッシュが生成されていなければ、消滅
				if(	!this->so_ptr->shadowObj)
				{
					this->setThisMessage(eTMSG_NOMSG);
					this->setOrderMessage(eOMSG_DELETE);
				}
				else
				{
					this->setThisMessage(eTMSG_SEE_SHADOW);
					this->shadowDiffuse.r	= 0.0f;
					this->shadowDiffuse.g	= 0.0f;
					this->shadowDiffuse.b	= 0.0f;
					this->shadowDiffuse.a	= 0.5f;
				}
				break;
		//	実態に影(影魔法)がついた物体
			case eSHAO_INCOMPLETE:
			//	影メッシュが生成されていなければ、消滅
				if(	!this->so_ptr->shadowObj)
				{
					this->setThisMessage(eTMSG_NOMSG);
					this->setOrderMessage(eOMSG_DELETE);
				}
				else
				{
					this->setThisMessage(eTMSG_SEE_SHADOW);
					this->shadowDiffuse.r	= 1.0f;
					this->shadowDiffuse.g	= 1.0f;
					this->shadowDiffuse.b	= 1.0f;
					this->shadowDiffuse.a	= 1.0f;
				}
				break;
		//	影も実態もない状態
			case eSHAO_NONE:
				this->setOrderMessage(eOMSG_DELETE);
				break;
		//	上記以外
			default:
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::reactionMessage
	//
	//	機能		リアクション対応
	//	引数		inMessage		リアクションメッセージ
	//	更新		2008/12/04		<新規>
	//================================================================================
	void
	CSHADOW_OBJECT::reactionMessage(REACTION_MESSAGE inMessage)
	{
	//	影物体構成分岐
		switch(this->shadowObjType)
		{
		//	影がない物体
			case eSHAO_HALF_REAL:
			//	メッセージが「シャドウボールに当たった」なら、
			//	「影が付着していく」動作を実行
				if(	inMessage == eREAMSG_SHADOW_HIT)
				{
					this->shadowObjAction	= eSHAOAC_SHADOW_REVIVAL;
				}
				break;
		//	影だけの物体
			case eSHAO_HALF_SHADOW:
			//	メッセージが「シャインボールに当たった」なら、
			//	「消滅していく」動作を実行
				if(	inMessage == eREAMSG_SHINE_HIT)
				{
					this->shadowObjAction	= eSHAOAC_DELETE;
				}
				break;
		//	実態に影(影魔法)がついた物体
			case eSHAO_INCOMPLETE:
			//	メッセージが「シャインボールに当たった」なら、
			//	「影だけが消滅していく」動作を実行
				if(	inMessage == eREAMSG_SHINE_HIT)
				{
					this->shadowObjAction	= eSHAOAC_SHADOW_DELETE;
				}
				break;
		//	上記以外
			default:
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::actionShadowObj
	//
	//	機能		影物体の動作
	//	更新		2009/01/19		<新規>
	//================================================================================
	void
	CSHADOW_OBJECT::actionShadowObj(void)
	{
	//	影物体動作分岐
		switch(this->shadowObjAction)
		{
		//	影動作なし
			case eSHAOAC_NONE:
				break;
		//	影が付着していく
			case eSHAOAC_SHADOW_REVIVAL:
			//	アルファ値を上げ、限界まで上昇したら
			//	影物体構成を「実体に影がついた物体」に変更
				if((this->shadowDiffuse.a +  CHANGE_VALUE_DIFFUSE) < 1.0f)
					this->shadowDiffuse.a += CHANGE_VALUE_DIFFUSE;
				else	
				{
					this->setShadowObjType(eSHAO_INCOMPLETE);
				//	影動作なしにする
					this->shadowObjAction	= eSHAOAC_NONE;
				}
				break;
		//	影だけが消滅していく
			case eSHAOAC_SHADOW_DELETE:
			//	アルファ値を下げ、限界まで下降したら
			//	影物体構成を「影がない物体」に変更
				if((this->shadowDiffuse.a -  CHANGE_VALUE_DIFFUSE) > 0.5f)
					this->shadowDiffuse.a -= CHANGE_VALUE_DIFFUSE;
				else	
				{
					this->setShadowObjType(eSHAO_HALF_REAL);
				//	影動作なしにする
					this->shadowObjAction	= eSHAOAC_NONE;
				}
				break;
		//	消滅していく
			case eSHAOAC_DELETE:
			//	アルファ値を下げ、限界まで下降したら消滅させる
				if((this->shadowDiffuse.a -  CHANGE_VALUE_DIFFUSE) > 0.0f)
					this->shadowDiffuse.a -= CHANGE_VALUE_DIFFUSE;
				else	
				{
					this->setShadowObjType(eSHAO_NONE);
					this->setOrderMessage(eOMSG_DELETE);
				//	影動作なしにする
					this->shadowObjAction	= eSHAOAC_NONE;
				}
				break;
		//	上記以外
			default:
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::getWorldCenter
	//
	//	機能		ワールド座標系中心点を受け取る
	//	戻り値		ワールド座標系中心点
	//	更新		2008/12/04		<新規>
	//================================================================================
	VEC3
	CSHADOW_OBJECT::getWorldCenter(void)
	{
	//	ワールド座標系中心点
		VEC3	aCenter;
	//	ワールド座標系の演算
		D3DXVec3TransformCoord(
			&aCenter, &so_ptr->shadowObj->getMeshs()->localCenter, this->getWorldMatrix());
	//	ワールド座標系中心点を返す
		return	aCenter;
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::intersectMesh
	//
	//	機能		自身とXメッシュとの衝突を検出する
	//	引数		*inObject		衝突対象物
	//				*inRayLoc		レイの始点
	//				*inLine			自身の進行方向
	//				*inDist			レイの始点から、衝突地点までの距離
	//	戻り値		TRUE			衝突している
	//				FALSE			衝突していない
	//	更新		2008/12/04		<新規>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersectMesh(C3D_OBJECT* inObject, VEC3* inRayLoc, LINE* inLine, float* inDist)
	{
		BOOL	bHit			= FALSE;					//!< 戻り値
		DWORD	dwFaceIndex		= 0;						//!< レイの始点に最も近い面のインデックス
		FLOAT	fU				= 0.0f;						//!< 重心ヒット座標U
		FLOAT	fV				= 0.0f;						//!< 重心ヒット座標V
		VEC3	rayDirection	= inObject->calcRay(inLine);//!< レイの方向
		XMESH	hitMesh			= (this->so_ptr->hitObj ? this->so_ptr->hitObj->getXMesh() :
														  this->so_ptr->shadowObj->getXMesh());

	//	攻撃するレイの位置を攻撃される座標系に合わせる
		this->rayFromInversedMatrix(inRayLoc, &rayDirection);

	//	レイの方向は必ず正規化する
		D3DXVec3Normalize(&rayDirection, &rayDirection);	//!< 正規化(0.0 or 1.0に収束する)

	//	メッシュとレイとの衝突判定
		D3DXIntersect(
			hitMesh,										//!< 衝突対象物
			inRayLoc,										//!< レイの始点座標
			&rayDirection,									//!< レイの方向
			&bHit,											//!< 衝突判定結果格納先
			&dwFaceIndex,									//!< レイの始点に最も近い面のインデックス格納先
			&fU,											//!< 重心ヒット座標U格納先
			&fV,											//!< 重心ヒット座標V格納先
			inDist,											//!< レイの始点から面の交点までの距離格納先
			NULL,											//!< 衝突判定結果の構造体格納先
			NULL);											//!< 配列内のエントリ数格納先

		return	bHit;
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::intersectObject
	//
	//	機能		自身とオブジェクトとの、半径での初期衝突判定
	//	引数		inObject		衝突対象物
	//	戻り値		TRUE			衝突しているが、メッシュとの衝突判定が可能
	//				FALSE			衝突結果が出ている(これ以上判定出来ない)
	//	更新		2008/12/04		<新規>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersectObject(C3D_OBJECT* inObject)
	{
	//	どちらかが非表示なら、判定せずに衝突していない事を返す
		if(	(!inObject->getVisible()) || (!this->getVisible()))
		{	
			return FALSE;
		}
	//	半径が衝突していない時は終了
		if(	!this->intersectRadius(inObject))
		{
			return FALSE;
		}
	//	半径が衝突している
		else
		{
			return TRUE;
		}
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::intersectMsgCheck
	//
	//	機能		メッセージ別衝突判定
	//	引数		inCheckMsg		チェックする識別メッセージ
	//	戻り値		TRUE			衝突判定有効
	//				FALSE			衝突判定無効
	//	更新		2008/12/04		<新規>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersectMsgCheck(THIS_MESSAGE inCheckMsg)
	{
	//	影物体構成分岐
		switch(this->shadowObjType)
		{
		//	影も実体もある物体
			case eSHAO_COMPLETE:
				return	TRUE;
		//	影がない物体
			case eSHAO_HALF_REAL:
			//	メッセージがシャドウボールなら、衝突判定を行う
				if(	inCheckMsg == eTMSG_SHADOW_BALL)
				{
					return	TRUE;
				}
				return	FALSE;
		//	影だけの物体
			case eSHAO_HALF_SHADOW:
				return	TRUE;
		//	実態に影がついた物体
			case eSHAO_INCOMPLETE:
				return	TRUE;
		//	影も実体もない状態
			case eSHAO_NONE:
				return	FALSE;
		//	上記以外
			default:
				return	FALSE;
		}
		return	FALSE;
	}


	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::intersect（全方向版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject		衝突対象物
	//				inLine			自身の進行方向
	//	戻り値		TRUE			衝突している
	//				FALSE			衝突していない
	//	更新		2008/12/04		<新規>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE* inLine)
	{
	//	まずはメッセージで判定
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離

	//	メッシュとの衝突判定し、衝突した時
		if(	this->intersectMesh(inObject, &inObject->getLoc(), inLine, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inObject->getMoveQuantity())
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::intersect（上下版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject		衝突対象物
	//				inUpDown		自身の上下進行方向
	//	戻り値		TRUE			衝突している
	//				FALSE			衝突していない
	//	更新		2008/12/04		<新規>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown)
	{
	//	まずはメッセージで判定
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離
		LINE	line	= LINE(inUpDown, eLR_NO, eFR_NO);		//!< 上下判別専用の方向構造体を作る

	//	メッシュとの衝突判定し、衝突した時
		if(	this->intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inObject->getMoveQuantity())
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::intersect（左右版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject		衝突対象物
	//				inLeftRight		自身の左右進行方向
	//	戻り値		TRUE			衝突している
	//				FALSE			衝突していない
	//	更新		2008/12/04		<新規>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight)
	{
	//	まずはメッセージで判定
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離
		LINE	line	= LINE(eUD_NO, inLeftRight, eFR_NO);	//!< 左右判別専用の方向構造体を作る

	//	メッシュとの衝突判定し、衝突した時
		if(	this->intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inObject->getMoveQuantity())
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::intersect（前後版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject		衝突対象物
	//				inFrontRear		自身の前後進行方向
	//	戻り値		TRUE			衝突している
	//				FALSE			衝突していない
	//	更新		2008/12/04		<新規>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear)
	{
	//	まずはメッセージで判定
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離
		LINE	line	= LINE(eUD_NO, eLR_NO, inFrontRear);	//!< 前後判別専用の方向構造体を作る

	//	メッシュとの衝突判定し、衝突した時
		if(	this->intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inObject->getMoveQuantity())
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::intersect（距離指定、全方向版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject			衝突対象物
	//				inLine				自身の進行方向
	//				inIntersectValue	判定する距離
	//	戻り値		TRUE				衝突している
	//				FALSE				衝突していない
	//	更新		2008/12/04			<新規>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE* inLine, float inIntersectValue)
	{
	//	まずはメッセージで判定
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離

	//	メッシュとの衝突判定し、衝突した時
		if(	this->intersectMesh(inObject, &inObject->getLoc(), inLine, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inIntersectValue)
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::intersect（距離指定、上下版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject			衝突対象物
	//				inUpDown			自身の上下進行方向
	//				inIntersectValue	判定する距離
	//	戻り値		TRUE				衝突している
	//				FALSE				衝突していない
	//	更新		2008/12/04			<新規>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown, float inIntersectValue)
	{
	//	まずはメッセージで判定
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離
		LINE	line	= LINE(inUpDown, eLR_NO, eFR_NO);		//!< 上下判別専用の方向構造体を作る

	//	メッシュとの衝突判定し、衝突した時
		if(	this->intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inIntersectValue)
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::intersect（距離指定、左右版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject			衝突対象物
	//				inLeftRight			自身の左右進行方向
	//				inIntersectValue	判定する距離
	//	戻り値		TRUE				衝突している
	//				FALSE				衝突していない
	//	更新		2008/12/04			<新規>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight, float inIntersectValue)
	{
	//	まずはメッセージで判定
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離
		LINE	line	= LINE(eUD_NO, inLeftRight, eFR_NO);	//!< 左右判別専用の方向構造体を作る

	//	メッシュとの衝突判定し、衝突した時
		if(	this->intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inIntersectValue)
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::intersect（距離指定、前後版）
	//
	//	機能		自身とオブジェクトとの衝突を検出する
	//	引数		inObject			衝突対象物
	//				inFrontRear			自身の前後進行方向
	//				inIntersectValue	判定する距離
	//	戻り値		TRUE				衝突している
	//				FALSE				衝突していない
	//	更新		2008/12/04			<新規>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear, float inIntersectValue)
	{
	//	まずはメッセージで判定
		if(	!this->intersectMsgCheck(inObject->getThisMessage()))	return	FALSE;
	//	半径での初期衝突を確認し、衝突結果が出たら、そこで結果を返す
		if(	!this->intersectObject(inObject))						return	FALSE;

	//	ここまで来たら、メッシュとの判定を行う
		float	fDist	= 0.0f;									//!< レイの始点から面の交点までの距離
		LINE	line	= LINE(eUD_NO, eLR_NO, inFrontRear);	//!< 前後判別専用の方向構造体を作る

	//	メッシュとの衝突判定し、衝突した時
		if(	this->intersectMesh(inObject, &inObject->getLoc(), &line, &fDist))
		{
		//	衝突までの距離が、移動距離より短い時
			if(	fDist < inIntersectValue)
			{
			//	地面からはみ出した距離を格納
				inObject->setDist(fDist);
			//	衝突した事を報告
				return TRUE;
			}
		}

	//	衝突しなかった事を報告
		return FALSE;
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::draw
	//
	//	機能		影物体の描画を行う
	//	引数		inDev			Direct3Dデバイス
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2007/12/08		<新規>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::draw(CONST DEV inDev)
	{
	//	存在しない状態なら、描画しない
		if(	!this->getVisible())	return FALSE;
	//	影物体動作
		this->actionShadowObj();
	//	影物体構成が[影も物体もない状態]だと、描画しない
		if(	this->shadowObjType == eSHAO_NONE)	return FALSE;
	//	モデルを前向きにする
		MAT16	matrix = (*this->getWorldMatrixInversed()) * (*this->getWorldMatrix());

	//	影物体の描画
		return	this->so_ptr->shadowObj->drawMesh(inDev, &matrix, &this->shadowDiffuse);
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::draw
	//
	//	機能		メッシュの描画を行う(Zテクスチャシェーダ使用)
	//	引数		inZTexSha		Zテクスチャシェーダ
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2007/12/14		<新規>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::draw(CZTEX_SHA* inZTexSha)
	{
	//	影物体構成が[影も実体もある物体]以外だと、描画しない
		if(	this->shadowObjType != eSHAO_COMPLETE)	return FALSE;
	//	影物体動作
		this->actionShadowObj();
	//	モデルを前向きにする
		MAT16	matrix = (*this->getWorldMatrixInversed()) * (*this->getWorldMatrix());

	//	ワールド行列を入れる
		inZTexSha->setWorldMatrix(&matrix);
	//	メッシュデータを扱いやすくする
		MESH_DATA*	inData	= this->so_ptr->shadowObj->getMeshs();
	//	マテリアルの数だけ描画を処理
		for(DWORD i = 0; i < inData->numMaterials; i++)
		{
			inZTexSha->beginPass();
			inData->mesh->DrawSubset(i);
			inZTexSha->endPass();
		}

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	CSHADOW_OBJECT::draw
	//
	//	機能		メッシュの描画を行う(深度バッファシェーダ使用)
	//	引数		inDepthSha		深度バッファシェーダ
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2007/12/14		<新規>
	//================================================================================
	BOOL
	CSHADOW_OBJECT::draw(CDEPTH_SHA* inDepthSha)
	{
	//	影物体構成が[影も実体もない状態]以外だと、描画しない
		if(	this->shadowObjType == eSHAO_NONE)	return FALSE;
	//	影物体動作
		this->actionShadowObj();
	//	モデルを前向きにする
		MAT16	matrix = (*this->getWorldMatrixInversed()) * (*this->getWorldMatrix());
	//	影物体構成が[影も物体もない状態]だと、描画しない
		if(	this->shadowObjType == eSHAO_COMPLETE)
		{
		//	メッシュデータを扱いやすくする
			MESH_DATA*	inData	= this->so_ptr->shadowObj->getMeshs();
		//	ディフューズ
			D3DCOLORVALUE 	keepDiffuse;
		//	ワールド行列を入れる
			inDepthSha->setWorldMatrix(&matrix);
		//	パラメータ更新
			inDepthSha->setParamToEffect();
		//	マテリアルの数だけ描画を処理
			for(DWORD i = 0; i < inData->numMaterials; i++)
			{
				inDepthSha->setEffectTexture(inData->texture[i]);
			//	ディフューズの計算
				keepDiffuse	= inData->materials[i].Diffuse;
				inData->materials[i].Diffuse	= this->shadowDiffuse;
				inDepthSha->setMaterialColor(&inData->materials[i].Diffuse);
			//	メッシュを描画
				inDepthSha->beginPass();
				inData->mesh->DrawSubset(i);
				inDepthSha->endPass();
			//	ディフューズを元に戻す
				inData->materials[i].Diffuse	= keepDiffuse;
			}
		}
		else
		{
			inDepthSha->end();
		//	影物体の描画
			this->so_ptr->shadowObj->drawMesh(
				inDepthSha->getDevice(), &matrix, &this->shadowDiffuse);
			inDepthSha->begin();
		}

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------