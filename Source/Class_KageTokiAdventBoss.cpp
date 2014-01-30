//***********************************************************************************
//!	ファイル名		Class_KageTokiAdventBoss.cpp
//
//	機能			アドベンチャーゲームのボスベースクラスの実装
//	記述			上岡　純
//	プロジェクト	[KageTokiGIRL]
//	更新			2009/02/19	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_BOSS::コンストラクタ
	//
	//	引数		inDev			Direct3Dデバイス
	//				inName			Xファイルの名称
	//				inResource		メッシュフォルダ名
	//				inTexture		テクスチャフォルダ名
	//	更新		2009/02/19		<新規>
	//================================================================================
	CKAGETOKI_ADVENT_BOSS::CKAGETOKI_ADVENT_BOSS(CONST DEV		inDev,
												 CONST LPSTR	inName,
												 CONST LPSTR	inResource,
												 CONST LPSTR	inTexture)	:
	CKAGETOKI_ADVENT_CHARACTER(inDev, inName, inResource, inTexture)
	{
	//	属性の初期化
		this->iMaxLife					= 1;					//!< 最大体力
		this->iLife						= this->iMaxLife;		//!< 残り体力

	//	フラグ群
		this->setThisMessage(eTMSG_ENEMY);						//!< 敵表示

	//	描画デバイスに定義されている描画先バッファの能力を取得
		D3DVIEWPORT9	viewPort;
		inDev->GetViewport(&viewPort);

	//	インターフェース関連
		this->bDrawInterface	= TRUE;
		this->interfaceTex		= NULL;
	//	共通
	//	体力基盤の設定
		DX::setVertex2D(
			&this->lifeBase[0],
			(float)(viewPort.Width  - 20 - 256),
			(float)(viewPort.Height - 20 - 256),
			0.1f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.0f, 0.0f);
		DX::setVertex2D(
			&this->lifeBase[1],
			(float)(viewPort.Width  - 20      ),
			(float)(viewPort.Height - 20 - 256),
			0.1f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.5f, 0.0f);
		DX::setVertex2D(
			&this->lifeBase[2],
			(float)(viewPort.Width  - 20      ),
			(float)(viewPort.Height - 20      ),
			0.1f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.5f, 0.5f);
		DX::setVertex2D(
			&this->lifeBase[3],
			(float)(viewPort.Width  - 20 - 256),
			(float)(viewPort.Height - 20      ),
			0.1f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.0f, 0.5f);

	//	体力の目盛枠基盤の設定
		DX::setVertex2D(
			&this->lifeFrameBase[0],
			this->lifeBase[0].x +  35.0f,
			this->lifeBase[0].y + 184.0f,
			0.03f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.1f, 0.51f);
		DX::setVertex2D(
			&this->lifeFrameBase[1],
			this->lifeBase[0].x +  35.0f + 190.0f,
			this->lifeBase[0].y + 184.0f,
			0.03f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.4f, 0.51f);
		DX::setVertex2D(
			&this->lifeFrameBase[2],
			this->lifeBase[0].x +  35.0f + 190.0f,
			this->lifeBase[0].y + 184.0f + 24.0f,
			0.03f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.4f, 0.56f);
		DX::setVertex2D(
			&this->lifeFrameBase[3],
			this->lifeBase[0].x +  35.0f,
			this->lifeBase[0].y + 184.0f + 24.0f,
			0.03f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.1f, 0.56f);

	//	体力の目盛の設定
		DX::setVertex2D(
			&this->lifeChip[0],
			this->lifeBase[0].x +  35.0f,
			this->lifeBase[0].y + 184.0f,
			0.02f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.1f, 0.59f);
		DX::setVertex2D(
			&this->lifeChip[1],
			this->lifeBase[0].x +  35.0f + (190.0f / EXE_MAXLIFE),
			this->lifeBase[0].y + 184.0f,
			0.02f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.4f, 0.59f);
		DX::setVertex2D(
			&this->lifeChip[2],
			this->lifeBase[0].x +  35.0f + (190.0f / EXE_MAXLIFE),
			this->lifeBase[0].y + 184.0f + 24.0f,
			0.02f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.4f, 0.61f);
		DX::setVertex2D(
			&this->lifeChip[3],
			this->lifeBase[0].x +  35.0f,
			this->lifeBase[0].y + 184.0f + 24.0f,
			0.02f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.1f, 0.61f);

	//	体力の目盛枠の設定
		DX::setVertex2D(
			&this->lifeFrame[0],
			this->lifeBase[0].x +  30.0f,
			this->lifeBase[0].y + 180.0f,
			0.01f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.015f, 0.63f);
		DX::setVertex2D(
			&this->lifeFrame[1],
			this->lifeBase[0].x +  30.0f + 212.0f,
			this->lifeBase[0].y + 180.0f,
			0.01f, 0.0f, D3DCOLOR_RGBA(255,255,255,255),  0.45f, 0.63f);
		DX::setVertex2D(
			&this->lifeFrame[2],
			this->lifeBase[0].x +  30.0f + 212.0f,
			this->lifeBase[0].y + 180.0f + 32.0f,
			0.01f, 0.0f, D3DCOLOR_RGBA(255,255,255,255),  0.45f, 0.69f);
		DX::setVertex2D(
			&this->lifeFrame[3],
			this->lifeBase[0].x +  30.0f,
			this->lifeBase[0].y + 180.0f + 32.0f,
			0.01f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.015f, 0.69f);
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_BOSS::デストラクタ
	//
	//	更新		2009/02/19		<新規>
	//================================================================================
	CKAGETOKI_ADVENT_BOSS::~CKAGETOKI_ADVENT_BOSS(void)
	{
	//	音関連の解放
		SAFE_RELEASE(this->interfaceTex);			//!< インターフェーステクスチャ
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_BOSS::opeTurnToPlayer
	//
	//	機能		プレイヤーの方を向く操作
	//	更新		2009/01/13		<新規>
	//================================================================================
	void
	CKAGETOKI_ADVENT_BOSS::opeTurnToPlayer(void)
	{
	//	向きの制御
		switch(this->way)
		{
		//	初期状態
			case eADCW_NO:
			//	左寄りの向きになっているなら、左向きに設定する
				if(	(this->getAngleY() >=   0.0f) &&
					(this->getAngleY() <  180.0f))
				{
					this->setAngleY(ADVENT_LEFTANGLE);
					this->way = eADCW_LEFT;
				}
			//	上記以外は、右向きに設定する
				else
				{
					this->setAngleY(ADVENT_RIGHTANGLE);
					this->way = eADCW_RIGHT;
				}
				break;

		//	左を向いている
			case eADCW_LEFT:
			//	プレイヤーが右にいるなら、右を向く動作(無理矢理)
				if(	this->getLocX() < (*this->player)->getLocX())
				{
					this->setAngleY(ADVENT_RIGHTANGLE);
					this->way = eADCW_RIGHT;
				}
				break;

		//	右を向いている
			case eADCW_RIGHT:
			//	プレイヤーが左にいるなら、左を向く動作(無理矢理)
				if(	this->getLocX() > (*this->player)->getLocX())
				{
					this->setAngleY(ADVENT_LEFTANGLE);
					this->way = eADCW_LEFT;
				}
				break;

		//	上記以外は、処理をしない
			default:	
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_BOSS::resetParameter
	//
	//	機能		パラメータのリセット
	//	更新		2009/02/18		<新規>
	//================================================================================
	void
	CKAGETOKI_ADVENT_BOSS::resetParameter(void)
	{
		this->iLife			= this->iMaxLife;		//!< 残り体力
	}

	//================================================================================
	//!	メソッド名	CKAGETOKI_ADVENT_BOSS::drawInterface
	//
	//	機能		インターフェースの描画
	//	引数		inDev			Direct3Dデバイス
	//	更新		2009/02/19		<新規>
	//================================================================================
	void
	CKAGETOKI_ADVENT_BOSS::drawInterface(CONST DEV inDev)
	{
	//	インターフェース描画確認が有効であれば、描画
		if(	this->bDrawInterface	&&
			this->interfaceTex)
		{
			DWORD		dwFVF=0;		//!< FVFのバックアップ用

			inDev->SetTexture(0, this->interfaceTex);
			inDev->GetFVF(&dwFVF);
			inDev->SetFVF(D3DFVF_VERTEX_2D);
			inDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, this->lifeBase,      sizeof(CUSTOM_VERTEX_2D));
			inDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, this->lifeFrameBase, sizeof(CUSTOM_VERTEX_2D));

			float	lifeDevide	= 190.0f / this->iMaxLife;
			for(int i = 0; i < this->iLife; i++)
			{
				this->lifeChip[0].x	= this->lifeBase[0].x + 35.0f + (lifeDevide * (float) i);
				this->lifeChip[1].x	= this->lifeBase[0].x + 35.0f + (lifeDevide * (float)(i+1));
				this->lifeChip[2].x	= this->lifeBase[0].x + 35.0f + (lifeDevide * (float)(i+1));
				this->lifeChip[3].x	= this->lifeBase[0].x + 35.0f + (lifeDevide * (float) i);
				inDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, this->lifeChip, sizeof(CUSTOM_VERTEX_2D));
			}

			inDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, this->lifeFrame, sizeof(CUSTOM_VERTEX_2D));
			inDev->SetFVF(dwFVF);
		}
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------