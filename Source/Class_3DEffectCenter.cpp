//***********************************************************************************
//!	ファイル名		Class_3DEffectCenter.cpp
//
//	機能			3Dゲーム専用のエフェクトクラスを、
//					一点に纏めて描画するクラスの実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/09/16	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_3DEffectCenter.h"		//!< エフェクトセンター

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//	インスタンス生成場所
	//================================================================================
	C3D_EFFECT_CENTER*	C3D_EFFECT_CENTER::gUniqueInstance	= NULL;

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::コンストラクタ
	//
	//	更新		2008/09/16		<新規>
	//================================================================================
	C3D_EFFECT_CENTER::C3D_EFFECT_CENTER(void)	:
	effects()
	{
		initialize();
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::デストラクタ
	//
	//	更新		2008/09/16		<新規>
	//================================================================================
	C3D_EFFECT_CENTER::~C3D_EFFECT_CENTER(void)
	{
		finishalize();
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::getInstance(インスタンスメソッド)
	//
	//	機能		インスタンスの生成
	//	戻り値		インスタンスを返す
	//	更新		2008/09/16		<新規>
	//================================================================================
	C3D_EFFECT_CENTER*
	C3D_EFFECT_CENTER::getInstance(void)
	{
	//!	生成していなかった場合
		if(!gUniqueInstance)
		{
		//!	インスタンス生成
			gUniqueInstance = new C3D_EFFECT_CENTER();
		}
	//!	インスタンスを返す
		return gUniqueInstance;
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::releaseInstance(インスタンスメソッド)
	//
	//	機能		インスタンスの解放
	//	更新		2008/09/16		<新規>
	//================================================================================
	void
	C3D_EFFECT_CENTER::releaseInstance(void)
	{
		SAFE_DELETE(gUniqueInstance);	//!< インスタンスの解放
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::initialize
	//
	//	機能		初期化処理
	//	更新		2008/09/16		<新規>
	//================================================================================
	void
	C3D_EFFECT_CENTER::initialize(void)
	{
	//	初期解放
		finishalize();
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::finishalize
	//
	//	機能		解放処理
	//	更新		2008/09/16		<新規>
	//================================================================================
	void
	C3D_EFFECT_CENTER::finishalize(void)
	{
	//	発生エフェクトグループの解放
		effects.releaseClear();
		effects2D.releaseClear();

	//	エフェクトマップの解放
 		SAFE_MAP_DELETE(effectMap, string, C3D_OBJECT*);
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::addEffectsInstance(インスタンスメソッド)
	//
	//	機能		インスタンスにエフェクト登録
	//	引数		inName			登録名
	//				inEffect		登録するエフェクトオブジェクト
	//	戻り値		TRUE			登録成功
	//				FALSE			登録失敗
	//	更新		2008/09/16		<新規>
	//================================================================================
	BOOL
	C3D_EFFECT_CENTER::addEffectsInstance(string inName, C3D_OBJECT* inEffect)
	{
	//	インスタンスが存在するなら、エフェクトを登録
		if(	gUniqueInstance)
		{
			return	gUniqueInstance->addEffects(inName, inEffect);
		}
	//	インスタンスが存在しないなら、エフェクトを破棄
		else
		{
			SAFE_DELETE(inEffect);
		}
		return	FALSE;
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::occuEffectsInstance(インスタンスメソッド)
	//
	//	機能		インスタンスにエフェクト発生
	//	引数		inName			発生させるエフェクトの登録名
	//				inLoc			発生させる場所
	//				inSize			サイズ指定(1.0f)
	//				inAngle			初期角度
	//				inColor			初期カラー
	//	更新		2008/09/16		<新規>
	//================================================================================
	void
	C3D_EFFECT_CENTER::occuEffectsInstance(	string		inName,
											VEC3		inLoc,
											float		inSize,
											VEC3		inAngle,
											D3DCOLOR	inColor)
	{
	//	インスタンスが存在するなら、エフェクトを発生
		if(	gUniqueInstance)
		{
			gUniqueInstance->occuEffects(inName, inLoc, inSize, inAngle, inColor);
		}
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::occu2DEffectsInstance(インスタンスメソッド)
	//
	//	機能		インスタンスにエフェクト発生
	//	引数		inName			発生させるエフェクトの登録名
	//				inLoc			発生させる場所
	//				inSize			サイズ指定(1.0f)
	//				inAngle			初期角度
	//				inColor			初期カラー
	//	更新		2008/09/16		<新規>
	//================================================================================
	void
	C3D_EFFECT_CENTER::occu2DEffectsInstance(	string		inName,
												VEC3		inLoc,
												float		inSize,
												VEC3		inAngle,
												D3DCOLOR	inColor)
	{
	//	インスタンスが存在するなら、エフェクトを発生
		if(	gUniqueInstance)
		{
			gUniqueInstance->occu2DEffects(inName, inLoc, inSize, inAngle, inColor);
		}
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::releaseEffectInstance(インスタンスメソッド)
	//
	//	機能		インスタンスにエフェクト解放
	//	引数		inName			解放させるエフェクトの登録名
	//	戻り値		TRUE			登録成功
	//				FALSE			登録失敗
	//	更新		2008/09/16		<新規>
	//================================================================================
	void
	C3D_EFFECT_CENTER::releaseEffectInstance(string inName)
	{
	//	インスタンスが存在するなら、エフェクトを登録
		if(	gUniqueInstance)
		{
			return	gUniqueInstance->releaseEffect(inName);
		}
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::addEffects
	//
	//	機能		エフェクトの登録
	//	引数		inName			登録名
	//				inEffect		登録するエフェクトオブジェクト
	//	戻り値		TRUE			登録成功
	//				FALSE			登録失敗
	//	更新		2008/09/16		<新規>
	//================================================================================
	BOOL
	C3D_EFFECT_CENTER::addEffects(string inName, C3D_OBJECT* inEffect)
	{
	//	同じエフェクトが存在したら、処理をしない
		if(	effectMap.find(inName) != effectMap.end())
		{
			SAFE_DELETE(inEffect);
			return	FALSE;
		}

	//	同じエフェクトが存在しないなら、登録する
		effectMap.insert(pair<string, C3D_OBJECT*>(inName, inEffect));

		return	TRUE;
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::occuEffects
	//
	//	機能		エフェクトの発生(3D)
	//	引数		inName			発生させるエフェクトの登録名
	//				inLoc			発生させる場所
	//				inSize			サイズ指定(1.0f)
	//				inAngle			初期角度
	//				inColor			初期カラー
	//	更新		2008/09/16		<新規>
	//================================================================================
	void
	C3D_EFFECT_CENTER::occuEffects(	string		inName,
									VEC3		inLoc,
									float		inSize,
									VEC3		inAngle,
									D3DCOLOR	inColor)
	{
	//	登録名から、発生させるエフェクトを検索
		map<string, C3D_OBJECT*>::iterator	mp;
		mp	= effectMap.find(inName);

	//	発生させるエフェクトが存在するなら、発生
		if(	mp != effectMap.end())
		{
			C3D_OBJECT*	inCopy	= NULL;
			mp->second->createCopy(&inCopy, &inLoc, &inAngle);
		//	設定
			inCopy->setScale(inSize);
			inCopy->setColor(inColor);
			effects.add(inCopy);
		}
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::occu2DEffects
	//
	//	機能		エフェクトの発生(2D)
	//	引数		inName			発生させるエフェクトの登録名
	//				inLoc			発生させる場所
	//				inSize			サイズ指定(1.0f)
	//				inAngle			初期角度
	//				inColor			初期カラー
	//	更新		2008/09/16		<新規>
	//================================================================================
	void
	C3D_EFFECT_CENTER::occu2DEffects(	string		inName,
										VEC3		inLoc,
										float		inSize,
										VEC3		inAngle,
										D3DCOLOR	inColor)
	{
	//	登録名から、発生させるエフェクトを検索
		map<string, C3D_OBJECT*>::iterator	mp;
		mp	= effectMap.find(inName);

	//	発生させるエフェクトが存在するなら、発生
		if(	mp != effectMap.end())
		{
			C3D_OBJECT*	inCopy	= NULL;
			mp->second->createCopy(&inCopy, &inLoc, &inAngle);
		//	設定
			inCopy->setScale(inSize);
			inCopy->setColor(inColor);
			effects2D.add(inCopy);
		}
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::releaseEffect
	//
	//	機能		エフェクトの解放
	//	引数		inName			解放させるエフェクトの登録名
	//	更新		2008/09/24		<新規>
	//================================================================================
	void
	C3D_EFFECT_CENTER::releaseEffect(string inName)
	{
	//	登録名から、解放させるエフェクトを検索
		map<string, C3D_OBJECT*>::iterator	mp;
		mp	= effectMap.find(inName);

	//	解放させるエフェクトが存在するなら、解放
		if(	mp != effectMap.end())
		{
			C3D_OBJECT*	inCopy	= mp->second;
			SAFE_DELETE(inCopy);
			effectMap.erase(mp);
		}
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::drawEffects
	//
	//	機能		全エフェクト描画
	//	引数		inDev			Direct3Dデバイス
	//				inCamera		発生させる場所
	//	更新		2008/09/16		<新規>
	//================================================================================
	void
	C3D_EFFECT_CENTER::drawEffects(CONST DEV inDev, C3D_OBJECT* inCamera)
	{
	//	角度を補正し、並べ替えして、描画
		effects.sortByCameraPosition(inCamera, TRUE);

		C3D_OBJECT*						releaseObj;	//!< 解放用
		vector<C3D_OBJECT*>::iterator	vp;			//!< 作業用イテレータ

	//	検査しながら描画するため、とりだして描画
		vp	= effects.getBegin();
		while(vp != effects.getEnd())
		{
		//	エフェクトを描画し、描画を終了
			if(	!(*vp)->draw(inDev))
			{
				releaseObj	= (*vp);
				vp			= effects.erase(vp);
				SAFE_DELETE(releaseObj);
			}
		//	描画を終了していないなら、イテレータを進める
			else
			{
				vp++;
			}
		}
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT_CENTER::drawEffects2D
	//
	//	機能		全エフェクト描画
	//	引数		inSprite		スプライトデバイス
	//	更新		2008/09/27		<新規>
	//================================================================================
	void
	C3D_EFFECT_CENTER::drawEffects2D(CONST SPRITE inSprite)
	{
		C3D_OBJECT*						releaseObj;	//!< 解放用
		vector<C3D_OBJECT*>::iterator	vp;			//!< 作業用イテレータ

	//	検査しながら描画するため、とりだして描画
		vp	= effects2D.getBegin();
		while(vp != effects2D.getEnd())
		{
		//	エフェクトを描画し、描画を終了
			if(	!(*vp)->draw(inSprite))
			{
				releaseObj	= (*vp);
				vp			= effects2D.erase(vp);
				SAFE_DELETE(releaseObj);
			}
		//	描画を終了していないなら、イテレータを進める
			else
			{
				vp++;
			}
		}
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------