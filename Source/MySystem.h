#pragma warning(disable:4996)
#pragma once
#include <windows.h>
#define DIRECT3D_VERSION	0x0900
#include <d3d9.h>
#include <d3dx9.h>
//=======================================================================
//開放系マクロ
#define MAC_Release_One(o)		if(o != NULL){o->Release( );o = NULL;}
#define MAC_release_One(o)		if(o != NULL){o->release( );o = NULL;}
#define MAC_delete_One(o)		if(o != NULL){delete o;o = NULL;}
#define MAC_delete_Table(o)		if(o != NULL){delete[] o;o = NULL;}

//=======================================================================
//システムで共有する汎用機能群
namespace GCom{
	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	//┃カウンタークラス																	　┃
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	namespace Cnt
	{
		//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		namespace Mode{
			enum TYPENAME{
				eNon,			//未使用
				eRepeatsA,		//繰り返すA(endとstartの切り替わりの際、誤差が消滅）
											//メニューカーソル等
				eRepeatsB,		//繰り返すB(endとstartの切り替わりの際、誤差を残す）
											//回転や物体移動等
				eTourn,			//復帰する
				eStop,			//１回目のみ有効
				eZero			//初期値と終了が同じ場合に適用、常に初期値を返す
			};
		}
		//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		namespace Status{
			enum TYPENAME{
				eErr,			//無効
				eEnd,			//カウント終了（eStopモードのみ）
				eNormal			//カウンター有効
			};
		}
		//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		//cCounterの機能を拡張したクラス
		//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		class cNormal{
			float counter;			//カウンター
			float startValue;		//初期値
			float endValue;			//終了値
			float incValue;			//増加（減少）値

			Mode::TYPENAME mode;		//モード

			void increasedCounter_Mode_RepeatsA(float inAddValue);
			void increasedCounter_Mode_RepeatsB(float inAddValue);
			void increasedCounter_Mode_Tourn(float inAddValue);
			void increasedCounter_Mode_Stop(float inAddValue);
		public:
			cNormal( );
			//カウンターを設定する
			Status::TYPENAME init(
				float inStartValue,					//初期値
				float inEndValue,					//終了値
				float inIncValue,					//増加値
				Mode::TYPENAME inMode);				//モード
			//カウンターを進める
			void increasedCounter(float inAddValue = 1.0f);
			//現在値を得る
			float getCounter( );
			//現在値を変更する（カウンターの範囲外の値を指定した場合無効）
			void setCounter(float inCounter);
			//現在値をイント型に変換して取得する
			int getCounterI( );
			//現在値をダブル型に変換して取得する
			double getCounterD( );
			//カウンターの現在の状態を得る
			Status::TYPENAME getStatus( );
			//カウンターを初期状態に戻す
			void resetCounter( );
		};
	}
	namespace _3D
	{
		//=======================================================================
		//３Ｄ補助系
		//=======================================================================
		//立方体
		class cBox{
		public: D3DXVECTOR3 s;
		public: D3DXVECTOR3 e;
		public: cBox();
		public: cBox(const D3DXVECTOR3* s, const D3DXVECTOR3* e);
		public: void set(const D3DXVECTOR3* s, const D3DXVECTOR3* e);
		public: void transform(const D3DXVECTOR3* offset);
		public: BOOL intersect(const cBox* box);
		//２つのボックスの直行する面の接触を判定する（接触時ＴＲＵＥ）
		public: BOOL hitLine(const cBox* box);
		};
		//=======================================================================
		//３Ｄ補助系
		//=======================================================================
		//各メンバが、float型のRECTクラス
		class cFloatRect{
		public:
			float left;				//左座標（Ｘ）
			float top;				//上座標（Ｙ）
			float right;			//右座標（Ｘ）
			float bottom;			//下座標（Ｙ）
			//=======================================================================
			//矩形情報の設定
			void set(	float inLeft,		//左座標（Ｘ）
						float inTop,		//上座標（Ｙ）
						float inRight,		//右座標（Ｘ）
						float inBottom);	//下座標（Ｙ）
			//=======================================================================
			//矩形の平行移動
			void transform(	float inX,		//Ｘ軸方向移動値
							float inY);		//Ｙ軸方向移動値
			//=======================================================================
			//２つの矩形の接触を判定する（接触時ＴＲＵＥ）
			BOOL hit(cFloatRect* inRect);
			//２つの矩形の線上の接触を判定する（接触時ＴＲＵＥ）
			BOOL hitLine(cFloatRect* inRect);
		};
	}
	namespace Color
	{
		//=======================================================================
		//色設定補助系
		//=======================================================================
		//３２Ｂｉｔカラーを簡易的に扱うための構造体
		struct sRGBA{
			BYTE b;		//青
			BYTE g;		//緑
			BYTE r;		//赤
			BYTE a;		//透過
		};
		//=======================================================================
		//３２Ｂｉｔカラーを簡易的に扱うためのクラス
		class cARGB{
		public:
			//=======================================================================
			//３２Ｂｉｔカラーを簡易的に扱うための構造体
			sRGBA color;
			//=======================================================================
			//コンストラクタ、DWORD型で色を設定できる、デフォルトは黒
			cARGB(DWORD inColor = 0x00000000);	
			//=======================================================================
			//コンストラクタ、４色それぞれを個別に色を設定できる、デフォルトは無し
			cARGB(BYTE a, BYTE r, BYTE g, BYTE b);	
			//=======================================================================
			//DWORD型で色を設定できる
			void set(DWORD inColor);		
			//=======================================================================
			//DWORD型で色を設定できる
			void set(BYTE a, BYTE r, BYTE g, BYTE b);		
			//=======================================================================
			//色をDWORD型で返却する
			DWORD get( );
		};
	}
	namespace Str
	{
		//文字列をＮＥＷしてコピーを作成
		char* copyNew(char* inString);
		LPSTR copyNew(LPCSTR inString);
		//転送先にメモリが割り当てられている場合、解放後strCopyNewを行う
		char*copyReleaseAndNew(char* *inSet, char* inString);
		LPSTR copyReleaseAndNew(LPSTR* inSet, LPCSTR inString);
	}

	namespace Memory
	{
	}
}	//namespace GCom
