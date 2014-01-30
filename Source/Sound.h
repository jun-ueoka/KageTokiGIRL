//-------------------------------------------------------------------------------------------------------------------------------
//ファイル名	:	Sound.h
//作成者		:	志場雅広
//作成日		:	08/04/28
//機能			:	サウンドに関連する処理で共通に使用する情報を記述しています。
//備考			:	
//更新履歴		:	08/04/28	Ver0.01.00	基本機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
//プラグマ指令
#pragma once
#pragma warning(disable:4996)

//ライブラリのリンク
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dsound.lib")

//ヘッダーファイルのインクルード
#include <dsound.h>

//マクロ定義
//プライマリバッファ関連(プライマリバッファは44.1kHz 16bit ステレオで作成する)
#define kCHANNELS 2																	//チャンネル数(ステレオ)
#define kSAMPLESPERSEC 44100														//サンプリングレート(44.1kHz)
#define kBITSPERSAMPLE 16															//サンプルあたりのビット数(16bit)

//解放関連
#define kRELEASE(x) if(x){ x->Release(); x = NULL; }								//xのオブジェクトがNULLでは無い場合は、xを解放する。
#define kCLOSEHANDLE(x) if(x){ CloseHandle(x); x = NULL; }							//ハンドルの開放

//スレッド関連
#define WM_END_THREAD (WM_USER + 1)													//スレッド終了
#define kTIMEOUT 1000																//タイムアウト時間(ミリ秒単位)
#define kEVENTMAX 2																	//イベントの最大数
#define KBLOCKNUM 3																	//通知回数(バッファサイズ = 通知発生間隔 * 通知回数)
//スレッドに送るメッセージ
#define WM_ENDTHREAD WM_USER + 1													//スレッドを終了

//構造体定義
//音声データの読み込みに使用
struct stSoundBuffer {
	VOID *pBuffer;																	//データを格納するバッファ
	DWORD dwBufferSize;																//バッファのサイズ
};

//サウンド名定義
#define	SOUND_STEP				"step1"
#define	SOUND_JUMP_START		"jump_start"
#define	SOUND_JUMP_END			"jump_end"
#define	SOUND_SHOT_SHADOW		"shot_shadow"
#define	SOUND_SHOT_SHINE		"shot_shine"
#define	SOUND_SHOT_ENERGY		"shot_shadow"
#define	SOUND_CURSOR			"cursor1"
#define	SOUND_DECISION1			"decision1"
#define	SOUND_DECISION2			"decision2"
#define	SOUND_BARK1				"shadog_bark1"
#define	SOUND_BARK2				"shadog_bark2"
#define	SOUND_SHADOWB_HIT		"shadow_ball_hit"
#define	SOUND_SHINEB_HIT		"shine_ball_hit"