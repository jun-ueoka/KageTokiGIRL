//***********************************************************************************
//!	ファイル名		Class_3DMap.cpp
//
//	機能			3Dオブジェクトで形成されたマップの実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/07/20	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_3DMap.h"					//!< 3Dオブジェクトマップ

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	C3D_MAP::コンストラクタ
	//
	//	引数		inTxtNum	テキスト番号
	//	更新		2008/07/20	<新規>
	//================================================================================
	C3D_MAP::C3D_MAP(void)
	:	releaseG(),						//!< 解放グループ
		viewG(),						//!< 描画グループ
		backG(),						//!< 背景描画グループ
		hitG(),							//!< 衝突グループ
		squeezeViewG(),					//!< 絞り込み描画グループ
		squeezeBackG()					//!< 絞り込み背景グループ
	{
		fDistance	= 5000.0f;			//!< 半径
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::デストラクタ
	//
	//	更新		2008/07/20	<新規>
	//================================================================================
	C3D_MAP::~C3D_MAP(void)
	{
	//	解放
		finalize();
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::addObject
	//
	//	機能		生成したオブジェクトのグループ登録
	//	引数		inObject		登録するオブジェクト
	//				inGroupType		登録するグループタイプ
	//	戻り値		TRUE			登録完了
	//				FALSE			登録未完了
	//	更新		2008/07/20		<新規>
	//================================================================================
	BOOL
	C3D_MAP::addObject(C3D_OBJECT* inObject, int inGroupType)
	{
	//	グループタイプ分岐
		switch(inGroupType)
		{
			case eMG_VIEW_HIT:							//!< 描画、衝突
				viewG.add(inObject);
				hitG.add(inObject);
				return	TRUE;

			case eMG_VIEW:								//!< 描画
				viewG.add(inObject);
				return	TRUE;

			case eMG_BACK_VIEW_HIT:						//!< 背景描画、衝突
				backG.add(inObject);
				hitG.add(inObject);
				return	TRUE;

			case eMG_BACK_VIEW:							//!< 背景描画
				backG.add(inObject);
				return	TRUE;

			case eMG_HIT:								//!< 衝突
				hitG.add(inObject);
				return	TRUE;

			default:									//!< 上記以外
				return	FALSE;
		}
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::checkMessage
	//
	//	機能		オブジェクトのメッセージをチェック
	//	引数		*inObjP			オブジェクトイテレータ
	//	戻り値		TRUE			描画ＯＫ
	//				FALSE			描画禁止
	//	更新		2008/07/20		<新規>
	//================================================================================
	BOOL
	C3D_MAP::checkMessage(vector<C3D_OBJECT*>::iterator* inObjP)
	{
		C3D_OBJECT*	inObj	= (*(*inObjP));					//!< ポインター格納
	//	状態検査
		switch((*(*inObjP))->getOrderMessage())
		{
			case	eOMSG_DELETE_IN:						//!< 消滅準備
				releaseG.add((*(*inObjP)));			//!< 消滅グループに入れる
				(*(*inObjP))->setOrderMessage(eOMSG_NOMSG);	//!< メッセージを消す
				return	TRUE;

			case	eOMSG_DELETE:							//!< 消滅命令
				releaseG.erase(inObj);				//!< 二重開放を防ぐ為
				hitG.erase(inObj);					//!< 二重開放を防ぐ為
				(*inObjP)	= viewG.erase(*inObjP);	//!< 描画グループから消去
				SAFE_DELETE(inObj);							//!< 解放
				return	FALSE;

			default:										//!< 上記以外
				return	TRUE;
		}
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::initialize
	//
	//	機能		外部読み込み
	//	引数		inTxtNum	テキスト番号
	//	更新		2008/07/20	<新規>
	//================================================================================
	void
	C3D_MAP::initialize(CONST DEV inDev, int inTxtNum)
	{
	//	読み込み処理
		FILE*	fp;								//!< ファイルポインター
		char	textName[MAX_PATH]		= "";	//!< テキスト名作成用

	//	テキスト名を生成
		sprintf_s(textName, sizeof(textName), PATH_MAP, inTxtNum);
	//	ファイルを開く
		fopen_s(&fp, textName, "r");

	//	ファイルを開けなかった時
		if(	!fp)
		{
		//	読み込みを行わず、処理を返す
			return;
		}

	//	初期解放
		finalize();

	//	読み込み開始
		char				szGroupType[MAX_PATH]	= "";			//!< グループタイプ判定文字列
		int					iGroupType				= 0;			//!< グループタイプ
		C3D_OBJECT*			newObject				= NULL;			//!< オブジェクト格納
		CREATE_OBJECT_DATA	newObjData;								//!< データ格納
		BOOL				bCheckEnd;								//!< 終了確認
		float				fDiffuse				= 0.0f;			//!< ディフューズの計算結果

	//	グループタイプ判定配列
		char	groupJudger[eMG_MAX][11]	=
		{
			"VIEW_HIT__",		//!< 描画と衝突
			"VIEW______",		//!< 描画
			"BACK_VIEWH",		//!< 背景描画と衝突
			"BACK_VIEW_",		//!< 背景描画
			"HIT_______",		//!< 衝突
			"ENEMY_____",		//!< 敵
		};

		ZeroMemory(&newObjData, sizeof(CREATE_OBJECT_DATA));		//!< データを初期化

	//	終了の命令があるまで、ループ
		do
		{
			fscanf_s(fp, "%s, ", szGroupType, sizeof(szGroupType));	//!< グループタイプ

		//	グループ判定
			for(iGroupType = 0; iGroupType < eMG_MAX; iGroupType++)
			{
				if(	strcmp(szGroupType, groupJudger[iGroupType]) == 0)	break;
			}

		//	グループに該当しないデータなら、処理を終了
			if(	iGroupType >= eMG_MAX)	break;

			fscanf_s(fp, "%s %s %f, %f, %f, %f, %f, %f, %f, %f, ",
				 newObjData.szData,	sizeof(newObjData.szData),		//!< オブジェクトタイプ
				 newObjData.szName, sizeof(newObjData.szName),		//!< 外部ファイル名
				&newObjData.loc.x,									//!< X位置
				&newObjData.loc.y,									//!< Y位置
				&newObjData.loc.z,									//!< Z位置
				&newObjData.angle.x,								//!< 角度X
				&newObjData.angle.y,								//!< 角度Y
				&newObjData.angle.z,								//!< 角度Z
				&newObjData.scale.x,								//!< 拡大比X
				&newObjData.scale.y);								//!< 拡大比Y

		//	オブジェクト生成
			bCheckEnd	= createObject(inDev, &newObject, &newObjData);

		//	生成に成功したら、グループ登録
			if(	bCheckEnd)
			{
			//	ディフューズの設定
			////--------------------------------------------------------
			////	超無理矢理だから修正の余地あり
			//	if(	newObject->getLocZ() > 0)
			//	{
			//		fDiffuse	= -(newObject->getLocZ() / 3000.0f);
			//		newObject->setDiffuseAlpha(fDiffuse);
			//	}
			////--------------------------------------------------------
			//	先に必ず解放グループに登録
				releaseG.add(newObject);
			//	グループに登録して、未完了なら不具合が生じたので、そこで処理終了
				if(	!addObject(newObject, iGroupType))	break;
			}
			ZeroMemory(&newObjData, sizeof(CREATE_OBJECT_DATA));	//!< データを初期化
		}
	//	生成に失敗していたら、ループ終了
		while(bCheckEnd);

	//	ファイルを閉じる
		fclose(fp);
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::finalize
	//
	//	機能		解放
	//	更新		2008/07/20	<新規>
	//================================================================================
	void
	C3D_MAP::finalize(void)
	{
	//	解放グループの解放
		vector<C3D_OBJECT*>::iterator	p;		//!< 作業用
		for(p  = releaseG.getBegin();
			p != releaseG.getEnd();
			p++)
		{
			SAFE_DELETE(*p);
		}
		releaseG.clear();

		viewG.clear();					//!< 描画グループの消去
		backG.clear();					//!< 背景描画グループの消去
		hitG.clear();						//!< 衝突グループの消去

		squeezeViewG.clear();				//!< 絞り込み描画グループの消去
		squeezeBackG.clear();				//!< 絞り込み背景グループの消去
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::draw
	//
	//	機能		描画
	//	引数		inDev			Direct3Dデバイス
	//				inCamera		カメラオブジェクト
	//	更新		2008/07/20		<新規>
	//================================================================================
	void
	C3D_MAP::draw(CONST DEV inDev, C3D_OBJECT* inCamera)
	{
	//	並べ替え
		backG.sortByCameraPosition(inCamera);
		viewG.sortByCameraPosition(inCamera);

		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vectorポインター
		vector<C3D_OBJECT*>::iterator	p;			//!< 作業用イテレータ

	//	検査しながら描画するため、とりだして描画
	//	背景描画
		c3dObjvec	= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	checkMessage(&p))
			{
				(*p)->draw(inDev);
				p++;
			}
		}
	//	通常描画
		c3dObjvec	= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	checkMessage(&p))
			{
				(*p)->draw(inDev);
				p++;
			}
		}
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::draw
	//
	//	機能		描画(見えない物は、描画しない)
	//	引数		inDev			Direct3Dデバイス
	//				inCamera		カメラオブジェクト
	//				inViewPoint		カメラ視点
	//	更新		2008/07/20		<新規>
	//================================================================================
	void
	C3D_MAP::draw(CONST DEV inDev, C3D_OBJECT* inCamera, VEC3* inViewPoint)
	{
	//	並べ替え
		backG.sortByCameraPosition(inCamera);
		viewG.sortByCameraPosition(inCamera);

		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vectorポインター
		vector<C3D_OBJECT*>::iterator	p;			//!< 作業用イテレータ

	//	検査しながら描画するため、とりだして描画
	//	背景描画
		c3dObjvec	= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					(*p)->draw(inDev);
				}
				p++;
			}
		}
	//	通常描画
		c3dObjvec	= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					(*p)->draw(inDev);
				}
				p++;
			}
		}
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::draw
	//
	//	機能		描画(見えない物は、描画しない)(ソートしない)
	//	引数		inDev			Direct3Dデバイス
	//				inViewPoint		カメラ視点
	//	更新		2008/07/20		<新規>
	//================================================================================
	void
	C3D_MAP::draw(CONST DEV inDev, VEC3* inViewPoint)
	{
		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vectorポインター
		vector<C3D_OBJECT*>::iterator	p;			//!< 作業用イテレータ

	//	検査しながら描画するため、とりだして描画
	//	背景描画
		c3dObjvec	= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					(*p)->draw(inDev);
				}
				p++;
			}
		}
	//	通常描画
		c3dObjvec	= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					(*p)->draw(inDev);
				}
				p++;
			}
		}
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::draw
	//
	//	機能		描画(見えない物は、描画しない)(ソートしない)(Zテクスチャシェーダ使用)
	//	引数		inZTexSha		Zテクスチャシェーダ
	//				inViewPoint		カメラ視点
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	C3D_MAP::draw(CZTEX_SHA* inZTexSha, VEC3* inViewPoint)
	{
		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vectorポインター
		vector<C3D_OBJECT*>::iterator	p;			//!< 作業用イテレータ

	//	検査しながら描画するため、とりだして描画
	//	背景描画
		c3dObjvec	= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
			{
				(*p)->draw(inZTexSha);
			}
			p++;
		}
	//	通常描画
		c3dObjvec	= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
			{
				(*p)->draw(inZTexSha);
			}
			p++;
		}
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::draw
	//
	//	機能		描画(見えない物は、描画しない)(ソートしない)(深度バッファシェーダ使用)
	//	引数		inDepthSha		深度バッファシェーダ
	//				inViewPoint		カメラ視点
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	C3D_MAP::draw(CDEPTH_SHA* inDepthSha, VEC3* inViewPoint)
	{
		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vectorポインター
		vector<C3D_OBJECT*>::iterator	p;			//!< 作業用イテレータ

	//	検査しながら描画するため、とりだして描画
	//	背景描画
		c3dObjvec	= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
			{
				(*p)->draw(inDepthSha);
			}
			p++;
		}
	//	通常描画
		c3dObjvec	= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					(*p)->draw(inDepthSha);
				}
				p++;
			}
		}
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::drawNotCheckMessage
	//
	//	機能		描画(見えない物は、描画しない)(ソートしない)(深度バッファシェーダ使用)
	//				メッセージチェックをしない
	//	引数		inDepthSha		深度バッファシェーダ
	//				inViewPoint		カメラ視点
	//	更新		2009/02/11		<新規>
	//================================================================================
	void
	C3D_MAP::drawNotCheckMessage(CDEPTH_SHA* inDepthSha, VEC3* inViewPoint)
	{
		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vectorポインター
		vector<C3D_OBJECT*>::iterator	p;			//!< 作業用イテレータ

	//	検査しながら描画するため、とりだして描画
	//	背景描画
		c3dObjvec	= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
			{
				(*p)->draw(inDepthSha);
			}
			p++;
		}
	//	通常描画
		c3dObjvec	= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
			{
				(*p)->draw(inDepthSha);
			}
			p++;
		}
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::squeezeSortByCameraPosition
	//
	//	機能		絞り込みソート
	//	引数		inCamera		カメラ
	//				inViewPoint		カメラ視点
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	C3D_MAP::squeezeSortByCameraPosition(C3D_OBJECT* inCamera, VEC3* inViewPoint)
	{
		squeezeBackG.clear();					//!< 絞り込み背景グループの消去
		squeezeViewG.clear();					//!< 絞り込み描画グループの消去

		vector<C3D_OBJECT*>*	c3dObjvec;			//!< vectorポインター
		vector<C3D_OBJECT*>*	c3dObjvecSqueeze;	//!< 絞り込み背景vectorポインター
		vector<C3D_OBJECT*>::iterator	p;			//!< 作業用イテレータ


	//	背景
		c3dObjvecSqueeze	= squeezeBackG.getObjects();
		c3dObjvec			= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					c3dObjvecSqueeze->push_back(*p);
				}
				p++;
			}
		}

	//	通常
		c3dObjvecSqueeze	= squeezeViewG.getObjects();
		c3dObjvec			= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					c3dObjvecSqueeze->push_back(*p);
				}
				p++;
			}
		}

	//	ソート
		squeezeBackG.sortByCameraPosition(inCamera);
		squeezeViewG.sortByCameraPosition(inCamera);
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::squeezeSortByCameraPositionWithDrawZTexSha
	//
	//	機能		絞り込みしながらZテクスチャで描画をし、ソートする
	//	引数		inCamera		カメラ
	//				inViewPoint		カメラ視点
	//				inZTexSha		Zテクスチャシェーダ
	//	更新		2009/01/31		<新規>
	//================================================================================
	void
	C3D_MAP::squeezeSortByCameraPositionWithDrawZTexSha(
				C3D_OBJECT* inCamera, VEC3* inViewPoint, CZTEX_SHA*  inZTexSha)
	{
		squeezeBackG.clear();					//!< 絞り込み背景グループの消去
		squeezeViewG.clear();					//!< 絞り込み描画グループの消去

		vector<C3D_OBJECT*>*	c3dObjvec;			//!< vectorポインター
		vector<C3D_OBJECT*>*	c3dObjvecSqueeze;	//!< 絞り込み背景vectorポインター
		vector<C3D_OBJECT*>::iterator	p;			//!< 作業用イテレータ

	//	背景
		c3dObjvecSqueeze	= squeezeBackG.getObjects();
		c3dObjvec			= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					(*p)->draw(inZTexSha);
					c3dObjvecSqueeze->push_back(*p);
				}
				p++;
			}
		}

	//	通常
		c3dObjvecSqueeze	= squeezeViewG.getObjects();
		c3dObjvec			= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					(*p)->draw(inZTexSha);
					c3dObjvecSqueeze->push_back(*p);
				}
				p++;
			}
		}

	//	ソート
		squeezeBackG.sortByCameraPosition(inCamera);
		squeezeViewG.sortByCameraPosition(inCamera);
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::squeezeSortByCameraPositionNotCheckMessageWithDrawZTexSha
	//
	//	機能		絞り込みしながらZテクスチャで描画をし、ソートする
	//				メッセージチェックはしない
	//	引数		inCamera		カメラ
	//				inViewPoint		カメラ視点
	//				inZTexSha		Zテクスチャシェーダ
	//	更新		2009/01/31		<新規>
	//================================================================================
	void
	C3D_MAP::squeezeSortByCameraPositionNotCheckMessageWithDrawZTexSha(
				C3D_OBJECT* inCamera, VEC3* inViewPoint, CZTEX_SHA*  inZTexSha)
	{
		squeezeBackG.clear();						//!< 絞り込み背景グループの消去
		squeezeViewG.clear();						//!< 絞り込み描画グループの消去

		vector<C3D_OBJECT*>*	c3dObjvec;			//!< vectorポインター
		vector<C3D_OBJECT*>*	c3dObjvecSqueeze;	//!< 絞り込み背景vectorポインター
		vector<C3D_OBJECT*>::iterator	p;			//!< 作業用イテレータ

	//	背景
		c3dObjvecSqueeze	= squeezeBackG.getObjects();
		c3dObjvec			= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
			if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
			{
				(*p)->draw(inZTexSha);
				c3dObjvecSqueeze->push_back(*p);
			}
			p++;
		}

	//	通常
		c3dObjvecSqueeze	= squeezeViewG.getObjects();
		c3dObjvec			= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
			if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
			{
				(*p)->draw(inZTexSha);
				c3dObjvecSqueeze->push_back(*p);
			}
			p++;
		}

	//	ソート
		squeezeBackG.sortByCameraPosition(inCamera);
		squeezeViewG.sortByCameraPosition(inCamera);
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::squeezeDrawWithZTexSha
	//
	//	機能		絞り込み描画(Zテクスチャシェーダ使用)
	//	引数		inZTexSha		Zテクスチャシェーダ
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	C3D_MAP::squeezeDrawWithZTexSha(CZTEX_SHA*  inZTexSha)
	{
		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vectorポインター
		vector<C3D_OBJECT*>::iterator	p;			//!< 作業用イテレータ

	//	検査しながら描画するため、とりだして描画
	//	背景描画
		c3dObjvec	= squeezeBackG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
			(*p)->draw(inZTexSha);
			p++;
		}
	//	通常描画
		c3dObjvec	= squeezeViewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			(*p)->draw(inZTexSha);
			p++;
		}
	}

	//================================================================================
	//!	メソッド名	C3D_MAP::squeezeDrawWithDepthSha
	//
	//	機能		絞り込み描画(深度バッファシェーダ使用)
	//	引数		inDepthSha		深度バッファシェーダ
	//	更新		2008/12/14		<新規>
	//================================================================================
	void
	C3D_MAP::squeezeDrawWithDepthSha(CDEPTH_SHA* inDepthSha)
	{
		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vectorポインター
		vector<C3D_OBJECT*>::iterator	p;			//!< 作業用イテレータ

	//	検査しながら描画するため、とりだして描画
	//	背景描画
		c3dObjvec	= squeezeBackG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
			(*p)->draw(inDepthSha);
			p++;
		}
	//	通常描画
		c3dObjvec	= squeezeViewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	オブジェクトのメッセージをチェックして、描画判定
			(*p)->draw(inDepthSha);
			p++;
		}
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------