//***********************************************************************************
//!	ファイル名		Class_Texture.h
//
//	機能			テクスチャオブジェクトに関する定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/08/27	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_3DObject.h"				//!< 3Dオブジェクト
#include	"Class_TextureBase.h"			//!< テクスチャベース

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	名前空間の解放
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	using	namespace	TB;	//!< メッシュベース

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CTEXTURE
	//
	//	機能			テクスチャオブジェクト
	//	更新			2008/08/27	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CTEXTURE : public C3D_OBJECT, public CTEXTURE_BASE
	{
	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	スプライト関連の属性
			//==================================================================
				RECT	rcFrom;						//!< 描画先
				RECT	rcTo;						//!< 描画元

	public:			//-------------------- public --------------------//

				CTEXTURE(CONST DEV   inDev,
						 CONST LPSTR inName,
						 VEC2		 inSize,
						 DWORD		 inColor	= CLEAR_COLOR,
						 CONST LPSTR inResource = PATH_TEXTURE);	//!< コンストラクタ
				CTEXTURE(void);										//!< コンストラクタ(空初期化)
				CTEXTURE(CTEXTURE* inCopy);							//!< コピー
		virtual	~CTEXTURE(void);									//!< デストラクタ

			//==================================================================
			//	access	スプライト関連
			//==================================================================
				RECT	getFromRect(void)const		{ return	rcFrom;		}
				RECT	getToRect(void)const		{ return	rcTo;		}
				void	setFromRect(RECT inFrom)	{ rcFrom	= inFrom;	}
				void	setFromRect(int left, int top, int right, int bottom)
				{
					SetRect(&rcFrom, left, top, right, bottom);
				}
				void	setToRect(RECT inTo)		{ rcTo		= inTo;		}
				void	setToRect(VEC2 inSize)
				{
					setShowSize(&inSize);
					SetRect(&rcTo,	int(getLocX() - (inSize.x / 2.0f)),
									int(getLocY() - (inSize.y / 2.0f)),
									int(getLocX() + (inSize.x / 2.0f)),
									int(getLocY() + (inSize.y / 2.0f)));
				}

			//==================================================================
			//	カラーのセット(カラーの設定が出来るサブクラスでオーバーライド)
			//==================================================================
		virtual	void	setColor(D3DCOLOR inColor)	{ color.color	= inColor;		}

			//==================================================================
			//	コピー生成(サブクラスでオーバーライド)
			//==================================================================
		virtual	void	createCopy(C3D_OBJECT** inCopy, VEC3* inLoc)
				{
					*inCopy	= new CTEXTURE(this);
					(*inCopy)->setLoc(inLoc);
				}

			//==================================================================
			//	描画
			//==================================================================
		virtual	BOOL	draw(CONST DEV		inDev);
		virtual	BOOL	draw(CONST SPRITE	inSprite);
	};
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------