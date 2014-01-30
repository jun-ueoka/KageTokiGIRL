//***********************************************************************************
//!	ファイル名		Class_GraphicDevice.h
//
//	機能			グラフィック・描画関係の管理を行うクラスの定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/04/17	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"DirectX.h"				//!< DirectX使用

//-----------------------------------------------------------------------
//!	グラフィックデバイスの名前空間
//-----------------------------------------------------------------------
namespace GD
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	エラー管理
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	GD_ERROR				//!< エラーメンバー
	{
		eGDERR_CREATE3D,			//!< Direct3Dの生成に失敗
		eGDERR_CREATE3DDEVICE,		//!< Direct3Dデバイスの生成に失敗
		eGDERR_RESETDEVICE,			//!< デバイスの再起動に失敗
		eGDERR_BUFFERCLEAR,			//!< バッファがクリアされない
		eGDERR_BEGINSCENE,			//!< シーンを開始できない
		eGDERR_BEGINSPRITE,			//!< スプライトを開始できない
		eGDERR_ENDSPRITE,			//!< スプライトを終了できない
		eGDERR_ENDSCENE,			//!< シーンを終了できない
		eGDERR_NOGETDISPLAYMODE,	//!< ディスプレイの状態を取得できない
		eGDERR_NON,					//!< エラーなし
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	背景
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define	BACK_BUFFER		D3DCOLOR_RGBA(255,255,255,255)

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CGRAPHIC_DEVICE
	//
	//	機能			グラフィックデバイス
	//	更新			2008/04/17	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CGRAPHIC_DEVICE
	{
			//==================================================================
			//!	背景
			//==================================================================
				#define	PATH_CURTAIN	"gd_curtain//gd_curtain.dds"

	private:		//-------------------- private --------------------//
		
			//==================================================================
			//	インスタンスを一度だけ生成する為
			//==================================================================
		static	CGRAPHIC_DEVICE*	gUniqueInstance;

			//==================================================================
			//	DirectX関連
			//==================================================================
				LPDIRECT3D9				d3D9;				//!< Direct3D
				DEV						dev;				//!< Direct3Dデバイス
				SPRITE					d3dsp;				//!< スプライトデータ
				D3DPP					d3dpp;				//!< プレゼンテーションパラメータ
				D3DCAPS9				caps;				//!< Capsデバイス
				LPDIRECT3DSURFACE9		surface;			//!< サーフェス

			//==================================================================
			//	表示状態
			//==================================================================
				HWND		hWnd;							//!< ターゲットウィンドウハンドル
				int			iScreenSizeX;					//!< スクリーンサイズ横幅
				int			iScreenSizeY;					//!< スクリーンサイズ縦幅
				int			iRefRate;						//!< リフレッシュレート
				BOOL		bFullScreen;					//!< フルスクリーン確認フラグ
				DWORD		dwWindowStyle;					//!< ウィンドウスタイル

			//==================================================================
			//	幕状態
			//==================================================================
				BOOL			bCurtainAction;				//!< 幕の動作確認
				BOOL			bCurtainAppear;				//!< 幕の出現確認
				int				iAlphaSpeed;				//!< 幕の透明度変動値
				MY_COLOR_RGBA	curtainColor;				//!< 幕のカラー
				TEX				curtainTex;					//!< 幕のテクスチャ
				float			curtainZLoc;				//!< 幕のZ位置

			//==================================================================
			//	初期化関連
			//==================================================================
				void		setParam(void);					//!< パラメータの設定
				BOOL		createDevice(void);				//!< デバイスの生成
				void		setState(void);					//!< ステータスの設定
				void		createManageClass(void);		//!< 各種管理するクラスを生成する

			//==================================================================
			//	エラー報告
			//==================================================================
				void		errMessage(GD_ERROR err);

				CGRAPHIC_DEVICE(void);
				~CGRAPHIC_DEVICE(void);

	public:			//-------------------- public --------------------//

		static	CGRAPHIC_DEVICE*	getInstance(void);		//!< インスタンスを取得
		static	void				releaseInstance(void);	//!< インスタンスを解放

				void	initialize(void);					//!< 初期化処理
				void	finishalize(void);					//!< 解放処理

			//==================================================================
			//!	デバイスの設定
			//==================================================================
				BOOL	initDevice(HWND inWnd, int inScreenSizeX, int inScreenSizeY, BOOL inFullScreen = FALSE);

			//==================================================================
			//!	スクリーンの更新
			//==================================================================
				BOOL	renewalScreenMode(HWND inWnd, BOOL inFullScreen, int inScreenSizeX = 0, int inScreenSizeY = 0);

			//==================================================================
			//!	レンダリング関係
			//==================================================================
				HRESULT	bufferClear(DWORD dwColor = BACK_BUFFER);				//!< バッファクリア
				HRESULT	renderBegin(DWORD dwColor = BACK_BUFFER);				//!< 描画準備
				HRESULT	renderEnd(void);										//!< 描画更新
				HRESULT	spriteBegin(void);										//!< スプライト開始
				HRESULT	spriteEnd(void);										//!< スプライト終了

			//==================================================================
			//	access	リフレッシュレート
			//==================================================================
				int		getRefRate(void)const		{ return iRefRate;		}
				void	setRefRate(int inRefRate)	{ iRefRate = inRefRate;	}

			//==================================================================
			//	access	スクリーンサイズ
			//==================================================================
				int		getScreenSizeX(void)const	{ return iScreenSizeX;	}
				int		getScreenSizeY(void)const	{ return iScreenSizeY;	}

			//==================================================================
			//	access	幕カラー
			//==================================================================
				void	setCurtainColorAlpha(int inAlpha)	{ curtainColor.rgba.alpha = (BYTE)inAlpha;	}
				void	setCurtainColor(int inRed, int inGreen, int inBlue);
				void	calcCurtainColor(int inRed, int inGreen, int inBlue);
				int		getCurtainColorRed(void)			{ return (int)curtainColor.rgba.red;	}
				int		getCurtainColorGreen(void)			{ return (int)curtainColor.rgba.green;	}
				int		getCurtainColorBlue(void)			{ return (int)curtainColor.rgba.blue;	}
				int		getCurtainColorAlpha(void)			{ return (int)curtainColor.rgba.alpha;	}

				void	setCurtainAction(BOOL inBoolen)		{ bCurtainAction	= inBoolen;			}

				void	setCurtainZLoc(float inLoc)			{ curtainZLoc		= inLoc;			}
				float	getCurtainZLoc(void)const			{ return	curtainZLoc;				}

			//==================================================================
			//	get	access
			//==================================================================
				DEV			getDevice(void)const	{ return dev;			}	//!< デバイスを受け取る
				D3DCAPS9	getCaps(void)const		{ return caps;			}	//!< Capsデバイスを受け取る
				SPRITE		getSprite(void)const	{ return d3dsp;			}	//!< スプライトデバイスを受け取る

				void	setLight(bool bLight, int inStage, VEC3 inLoc);			//!< ライトの設定
				void	setFog(void);											//!< フォグの設定
				void	createCurtainTex(LPSTR inTexName = NULL);				//!< 幕テクスチャの生成
				void	renderCurtain(TEX inTex, DWORD inColor);				//!< 塗りつぶし描画
				void	renderCurtainInDev(void);								//!< 塗りつぶし描画(デバイスステータス)
				void	curtainMotionExecute(void);								//!< 幕の動作処理
				void	curtainMotion(BOOL inBoolen, int inAlpha);				//!< 幕の動作
				BOOL	resetDevice(void);										//!< デバイスのリセット
				void	setFogEnable(const BOOL bEnable);						//!< フォグの使用・不使用設定
	};
}
//-----------------------------------------------------------------------
//!	グラフィックデバイスの名前空間
//-----------------------------------------------------------------------