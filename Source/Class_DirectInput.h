#pragma warning(disable:4996)
#pragma once
#include <windows.h>
#define DIRECTINPUT_VERSION	0x0800
#include <dInput.h>
#include	"Version.h"

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃入力機器の種類																		　┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
namespace DEVICETYPE{
	enum TYPENAME{
		eKEYBOARD,			//　キーボード
		eGAMEPAD,			//　ジョイスティック
		eUNNON
	};
}
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃キーの入力状態（順番および値の変更不可）											　┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
namespace MYKEYSTAT{
	enum TYPENAME{
		eONCELOCK,			//	押下中のキーを１度離すまで無効化する
		eFREE,				//	離している間
		eOFF,				//	離した直後
		eON,				//	押した直後
		eDOWN,				//	押している間
		eNON				//	checkStatus( )　対応するキーが無い場合
	};
}
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃プログラム中で使用する仮想キー名称（順番および値の変更不可）						　┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
namespace MYKEY{
	enum TYPENAME{	//	キーの名前
	//	キーパッド＆キーボード
		eUP,		//	上キー				//!< ↑
		eDOWN,		//	下キー				//!< ↓
		eLEFT,		//	左キー				//!< ←
		eRIGHT,		//	右キー				//!< →
		eSOFT,		//　ボタン00			//!< 右ボタン
		eMIDDLE,	//　ボタン01			//!< 下ボタン
		eHARD,		//　ボタン02			//!< 上ボタン
		eDUST,		//　ボタン03			//!< 左ボタン
		eL1,		//　ボタン04			//!< 
		eL2,		//　ボタン05			//!< 
		eR1,		//　ボタン10			//!< 
		eR2,		//　ボタン11			//!< 
		eSELECT,	//　ボタン12			//!< 選択ボタン
		eSTART,		//　ボタン13			//!< 決定ボタン
		eQUIT,		//　ボタン14			//!< 終了ボタン

#ifdef	_DEBUG
	//	キーボード専用
		eF1,		//	ファンク1			//!< 日電シンボル画面へ
		eF2,		//	ファンク2			//!< キャラクターの位置を変更
		eF3,		//	ファンク3			//!< カゲオ有効
		eF4,		//	ファンク4			//!< カゲオ無効
		eF5,		//	ファンク5			//!< カゲオ制限解除
		eF6,		//	ファンク6			//!< イベントスキップ
		eF7,		//	ファンク7			//!< ゲームクリア
		eF8,		//	ファンク8			//!<
		eF9,		//	ファンク9			//!<
		eF10,		//	ファンク10			//!<
#else
#ifdef PRESENTATION
	//	キーボード専用
		eF1,		//	ファンク1			//!< 日電シンボル画面へ
		eF2,		//	ファンク2			//!< キャラクターの位置を変更
		eF3,		//	ファンク3			//!< カゲオ有効
		eF4,		//	ファンク4			//!< カゲオ無効
		eF5,		//	ファンク5			//!< カゲオ制限解除
		eF6,		//	ファンク6			//!< イベントスキップ
		eF7,		//	ファンク7			//!< ゲームクリア
		eF8,		//	ファンク8			//!<
		eF9,		//	ファンク9			//!<
		eF10,		//	ファンク10			//!<
		e1,			//
		e2,			//
		e3,			//
		e4,			//
		e5,			//
		e6,			//
		e7,			//
		e8,			//
		e9,			//
		e0,			//
#endif
#endif
		eMAX		//　ボタン総数			//!< 
	};
}
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃入力機器とプログラムを仲介するキー情報構造体										　┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
struct sKeyParam{				
	DWORD vKeyCode;				//	バーチャルキーコード(キーボードの場合設定する）（ゲームパッドではMYKEY型のいずれかの値を入れる）
	MYKEY::TYPENAME myKey;				//	対応キー(MYKEY型のいずれかの値を入れる）
	MYKEYSTAT::TYPENAME keyPressStatus;	//	キー入力状態(離している間、離した直後、押した直後、押している間)
};
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃入力環境でおこるエラー情報															　┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
namespace MYERROR_DI{
	enum TYPENAME{
		eDIC,			//	DirectInput8Createが失敗
		eED,			//	デバイス列挙に失敗
		eED_UNNONTYPE,	//	未対応のデバイス　
		eCD,			//	デバイス生成に失敗
		eSDF,			//	フォーマット設定に失敗
		eSCL,			//	協調レベルの設定に失敗
		eNOINIT,		//	オブジェクトが初期化されていない
		eCNTOVER,		//	カウンタオーバー
		eGDS,			//	デバイス情報取得に失敗
		eUSEKEYBOARD,	//	キーボードを使用している
		eNONRELEASE,	//	開放していない状態で再生成しようとしている
		eNOTHING		//	エラーはなし
	};
}
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃DirectInputのデバイス制御クラス(使用するデバイスの数に応じて宣言・使用する）			　┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
#define kinputDeviceMax 4	//検出するデバイスの最大数

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CINPUT{
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
private:static CINPUT			*instance[kinputDeviceMax];		//　４つのデバイスのみ使用可能			
private:IDirectInputDevice8*	device;							//	DIデバイス
private:DWORD					type;							//	キーボードやゲームパッドなどを識別する
private:int						id;								//　デバイスに割り当てられたID番号
private:int						numberOfKey;					//　入力チェックするキーの最大数
private:sKeyParam				*informationOfKeys;				//　チェック対象のキー情報
private:BOOL					valid;							//　初期化が完全に行われている場合TRUE
//入力機器のボタンを調べる、入力機器毎に設定する関数ポインタ
private:void (*checkKeyFunc)(	IDirectInputDevice8* inDevice,	//DInputデバイス
								int inNumberOfKey,				//チェックするキーの数
								sKeyParam inKeys[ ]);			//キー情報
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
private:void release(void);
private:MYKEYSTAT::TYPENAME checkStatus(MYKEY::TYPENAME inKeyID);
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
public:CINPUT( );
//インスタンス
//取得
public:static CINPUT* getInstance(int inDeviceNumber);	//デバイス番号
//開放
public:static void releaseInstance(int inDeviceNumber);			//デバイス番号
//全開放
public:static void releaseInstanceAll( );
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//入力デバイス
//生成
public:MYERROR_DI::TYPENAME init(	HWND inWindowHandle,			//ウィンドウハンドル
						DEVICETYPE::TYPENAME inDeviceType,		//デバイス種類
						int inNumberOfKey,				//キー（ボタン）数
						sKeyParam ioKeyInformation[ ]);	//各キーが何ボタンに対応するか
														//設定してあること
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//入力確認
//ハードウェアから状態取得
public:void		checkKey(void);
//押下確認
public:virtual	bool checkPress(MYKEY::TYPENAME inKeyID);
//押下直後確認
public:virtual	bool checkPressOnce(MYKEY::TYPENAME inKeyID);
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ステータス変更
//キーを一時的に押していない状態に変える（押下中のキーを無効にする）
public:void checkKeyOnceLock(MYKEY::TYPENAME inKeyID);
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//デバイス合成
//別のデバイスの入力情報を取り込む
public:void composeKeyStatus(CINPUT* inKeyDevice);
};

//ダミー入力
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CDUMMY_INPUT : public CINPUT{
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
protected:	bool	dummyKey[MYKEY::eMAX];			//!< 入力
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
public:		CDUMMY_INPUT( )	: CINPUT( ) { resetKey();	}
//入力キーリセット
public:void		resetKey(void)
	   { for(int i=0; i<MYKEY::eMAX; i++){ dummyKey[i]	= false;}	}
//押下操作
public:void		setPress(MYKEY::TYPENAME inKeyID, bool inBool)
	   { dummyKey[inKeyID] = inBool;	}
//入力確認
//押下確認
public:bool		checkPress(MYKEY::TYPENAME inKeyID)
	   { return	dummyKey[inKeyID];		}
//押下直後確認
public:bool		checkPressOnce(MYKEY::TYPENAME inKeyID)
	   { return	dummyKey[inKeyID];		}
};

//タイマー付きダミー入力
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CDUMMY_INPUT_TIMER : public CDUMMY_INPUT{
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
protected:	int		iKeyTimer[MYKEY::eMAX];			//!< タイマー
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
public:		CDUMMY_INPUT_TIMER( )	: CDUMMY_INPUT( )
		{ for(int i=0; i<MYKEY::eMAX; i++){ iKeyTimer[i]	= 0;}}
//タイマー付き押下操作
public:void		setPressWithTimer(MYKEY::TYPENAME inKeyID, bool inBool, int inTime)
	   { dummyKey[inKeyID] = inBool;	iKeyTimer[inKeyID] = inTime;	}
//タイマー確認
public:void		checkTimer(void)
	   { for(int i=0; i<MYKEY::eMAX; i++)
	     { if(dummyKey[i])
	       { if(iKeyTimer[i] > 0)	iKeyTimer[i]--;
		     else					dummyKey[i]	= false;
		   }
	     }
	   }
};