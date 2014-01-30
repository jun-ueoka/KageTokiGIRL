//***********************************************************************************
//!	ファイル名		Class_3DEffect.cpp
//
//	機能			3Dゲーム専用のエフェクトクラスの実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/09/11	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_3DEffect.h"

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	C3D_EFFECT::コンストラクタ
	//
	//	機能概要	Xファイルをロードして初期化
	//	引数		inDev			Direct3Dデバイス
	//				inName			エフェクトファイルの名称
	//				inLoc			開始位置
	//				inResource		エフェクトフォルダ名
	//				inTexture		エフェクトテクスチャフォルダ名
	//	更新		2008/05/30		<新規>
	//================================================================================
	C3D_EFFECT::C3D_EFFECT(CONST DEV   inDev,
						   CONST LPSTR inName,
						   VEC3		   inLoc,
						   CONST LPSTR inResource,
						   CONST LPSTR inTexture)	:
	CEFFECT_BASE(inDev, inName, inResource, inTexture),
	spMove(NULL)
	{
	//	スーパークラスの初期化
		C3D_OBJECT::initialize();
	//	属性の初期化
		iCntFrame	= 0;
		iCntTex		= 0;
		iCntSize	= 0;
		iCntRotato	= 0;
		iCntMove	= 0;
		iCntColor	= 0;
		spMove		= NULL;

		bMotion		= TRUE;
		keepAngle	= VEC3(0.0f,0.0f,0.0f);

	//	位置のセット
		setLoc(&inLoc);
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT::コンストラクタ
	//
	//	機能		空初期化
	//	更新		2007/12/08		<新規>
	//================================================================================
	C3D_EFFECT::C3D_EFFECT(void)	:
	CEFFECT_BASE(),
	spMove(NULL)
	{
	//	スーパークラスの初期化
		C3D_OBJECT::initialize();
	//	属性の初期化
		iCntFrame	= 0;
		iCntTex		= 0;
		iCntSize	= 0;
		iCntRotato	= 0;
		iCntMove	= 0;
		iCntColor	= 0;
		spMove		= NULL;

		bMotion		= TRUE;
		keepAngle	= VEC3(0.0f,0.0f,0.0f);
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT::コンストラクタ
	//
	//	機能概要	コピー
	//	引数		inCopy			コピー元
	//				inLoc			開始位置
	//				inAngle			開始角度
	//	更新		2008/05/30		<新規>
	//================================================================================
	C3D_EFFECT::C3D_EFFECT(C3D_EFFECT* inCopy, VEC3 *inLoc, VEC3 *inAngle)	:
	CEFFECT_BASE(inCopy),
	spMove(NULL)
	{
	//	スーパークラスの初期化
		C3D_OBJECT::initialize();
	//	属性の初期化
		iCntFrame	= 0;
		iCntTex		= 0;
		iCntSize	= 0;
		iCntRotato	= 0;
		iCntMove	= 0;
		iCntColor	= 0;
		spMove		= NULL;

		bMotion		= TRUE;
		keepAngle	= *inAngle;

	//	位置のセット
		setLoc(inLoc);
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT::デストラクタ
	//
	//	機能概要	全ての資源を解放し、終了する。
	//				資源が、他のオブジェクトでも使われていた場合は残す。
	//	更新		2008/05/30		<新規>
	//================================================================================
	C3D_EFFECT::~C3D_EFFECT()
	{
	//	スーパークラスの開放
		C3D_OBJECT::finalize();
	//	スプラインの解放
		SAFE_DELETE(spMove);
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT::createSpline
	//
	//	機能		スプライン生成
	//	更新		2008/09/16		<新規>
	//================================================================================
	void
	C3D_EFFECT::createSpline(void)
	{
	//	生成
		if(	!taskBreak((*eiMove)->iFrame))
		{
		//	現在角度を維持
			VEC3	inAngle	= getAngle();

		//	初期解放と生成
			SAFE_DELETE(spMove);
			spMove	= new CSPLINE();

		//	中間地点、終了地点の確保
			VEC3	vMiddle	= (*eiMove)->middle;
			VEC3	vGoal	= (*eiMove)->goal;

		//	初期化時維持角度をセット
			setAngle(&keepAngle);
			D3DXVec3TransformCoord(&vMiddle, &vMiddle, getWorldMatrix());
			D3DXVec3TransformCoord(&vGoal  , &vGoal,   getWorldMatrix());
			spMove->setUp(
				&getLoc(), &vMiddle, &vGoal,
				(*eiMove)->iFrame);

		//	角度を戻す
			setAngle(&inAngle);
		}
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT::motionEffect
	//
	//	機能		エフェクト実行
	//	機能概要	エフェクトの処理を行う
	//	戻り値		TRUE			モーション続行
	//				FALSE			モーション終了（外部解放命令）
	//	更新		2008/09/16		<新規>
	//================================================================================
	BOOL
	C3D_EFFECT::motionEffect(void)
	{
	//	終了チェック
		BOOL	endChecker	= FALSE;

	//	エフェクトテクスチャの処理
		if(	!taskBreak((*eiTex)->iFrame))
		{
			endChecker		= TRUE;
			iCntTex++;
		//	フレームが到達したら、イテレータを進める
			if(	(*eiTex)->iFrame	 <= iCntTex)
			{
				iCntTex = 0;
				eiTex++;
			//	一応イテレータをチェックし、不正があるなら、外部解放
				if(	ed_ptr->vTex.end()	== eiTex)		return	FALSE;
			//	イテレータを戻すか判定
				if(	taskReturn((*eiTex)->iFrame))
				{
					eiTex += (*eiTex)->iFrame;
				}
			}
		}

	//	エフェクトサイズの処理
		if(	!taskBreak((*eiSize)->iFrame))
		{
			endChecker		= TRUE;
			iCntSize++;
		//	サイズ変化
			calcShowSize((*eiSize)->size);
		//	フレームが到達したら、イテレータを進める
			if(	(*eiSize)->iFrame	 <= iCntSize)
			{
				iCntSize = 0;
				eiSize++;
			//	一応イテレータをチェックし、不正があるなら、外部解放
				if(	ed_ptr->vSize.end()	== eiSize)	return	FALSE;
			//	イテレータを戻すか判定
				if(	taskReturn((*eiSize)->iFrame))
				{
					eiSize += (*eiSize)->iFrame;
				}
			}
		}

	//	エフェクト回転の処理
		if(	!taskBreak((*eiRotato)->iFrame))
		{
			endChecker		= TRUE;
			iCntRotato++;
		//	回転角度変化
			calcAngle(&(*eiRotato)->rotato);
		//	フレームが到達したら、イテレータを進める
			if(	(*eiRotato)->iFrame	 <= iCntRotato)
			{
				iCntRotato = 0;
				eiRotato++;
			//	一応イテレータをチェックし、不正があるなら、外部解放
				if(	ed_ptr->vRotato.end()	== eiRotato)	return	FALSE;
			//	イテレータを戻すか判定
				if(	taskReturn((*eiRotato)->iFrame))
				{
					eiRotato += (*eiRotato)->iFrame;
				}
			}
		}

	//	エフェクト移動の処理
		if(	!taskBreak((*eiMove)->iFrame))
		{
			endChecker		 = TRUE;
		//	スプラインから、現在の位置を取得
			if(	!spMove)	createSpline();
			setLoc(spMove->getPos(iCntMove));
			iCntMove++;
		//	フレームが到達したら、イテレータを進める
			if(	(*eiMove)->iFrame	 <= iCntMove)
			{
				iCntMove = 0;
				eiMove++;
			//	一応イテレータをチェックし、不正があるなら、外部解放
				if(	ed_ptr->vMove.end()	== eiMove)	return	FALSE;
			//	イテレータを戻すか判定
				if(	taskReturn((*eiMove)->iFrame))
				{
					eiMove += (*eiMove)->iFrame;
				}
			//	スプラインを生成
				createSpline();
			}
		//	フレームが到達してないなら、位置を更新
		}

	//	エフェクトカラーの処理
		if(	!taskBreak((*eiColor)->iFrame))
		{
			endChecker		= TRUE;
			iCntColor++;
		//	カラー変化
			calcColor(
				(*eiColor)->red,
				(*eiColor)->green,
				(*eiColor)->blue,
				(*eiColor)->alpha);
		//	フレームが到達したら、イテレータを進める
			if(	(*eiColor)->iFrame <= iCntColor)
			{
				iCntColor = 0;
				eiColor++;
			//	一応イテレータをチェックし、不正があるなら、外部解放
				if(	ed_ptr->vColor.end()	== eiColor)	return	FALSE;
			//	イテレータを戻すか判定
				if(	taskReturn((*eiColor)->iFrame))
				{
					eiColor += (*eiColor)->iFrame;
				}
			}
		}

	//	終了しないなら、カウンタを進める
		if(	endChecker)
		{
		//	終了する値なら、処理しない
			if(	!taskBreak(ed_ptr->frame))
			{
				iCntFrame++;
			//	最大フレームまで到達したら、外部解放命令
				if(	iCntFrame >= ed_ptr->frame)
				{
					endChecker	= FALSE;
				}
			}
		}
		return	endChecker;
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT::draw
	//
	//	機能		描画(3D)
	//	機能概要	エフェクトの描画を行う
	//	引数		inDev			Direct3Dデバイス
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2008/05/30		<新規>
	//================================================================================
	BOOL
	C3D_EFFECT::draw(CONST DEV inDev)
	{
	//	表示確認して、無効の時
		if(	!getVisible())
		{
		//	外部解放命令
			setOrderMessage(eOMSG_DELETE);
		//	描画失敗を返す
			return	FALSE;
		}

	//	スプライン動作
		if(	 (bMotion)	&&
			!(motionEffect()))
		{
		//	表示を無効にしておく
			setVisible(FALSE);
		//	外部解放命令
			setOrderMessage(eOMSG_DELETE);
		//	描画失敗を返す
			return	FALSE;
		}

	//	ワールド変換
		update();

	//	描画
		return	drawEffect(inDev, getWorldMatrix());
	}

	//================================================================================
	//!	メソッド名	C3D_EFFECT::draw
	//
	//	機能		描画(2D)
	//	機能概要	エフェクトの描画を行う
	//	引数		inSprite		スプライトデバイス
	//	戻り値		TRUE			描画成功
	//				FALSE			描画失敗
	//	更新		2008/09/25		<新規>
	//================================================================================
	BOOL
	C3D_EFFECT::draw(CONST SPRITE inSprite)
	{
	//	表示確認して、無効の時
		if(	!getVisible())
		{
		//	外部解放命令
			setOrderMessage(eOMSG_DELETE);
		//	描画失敗を返す
			return	FALSE;
		}

	//	スプライン動作
		if(	 (bMotion)	&&
			!(motionEffect()))
		{
		//	表示を無効にしておく
			setVisible(FALSE);
		//	外部解放命令
			setOrderMessage(eOMSG_DELETE);
		//	描画失敗を返す
			return	FALSE;
		}

	//	描画
		return	drawEffect(
					inSprite,
					getLoc(),
					D3DXToRadian(getAngleZ()),
					getScale());
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------