//-------------------------------------------------------------------------------------------------------------------------------
//ファイル名	:	Class_Sound_OggVorbis.h
//作成者		:	志場雅広
//作成日		:	08/04/28
//機能			:	CSound.cppで必要な宣言、定義を行っています。
//備考			:	CSoundクラスから派生して各サウンドバッファが作成されます。
//					又、CSoundクラスで設定されたデータを共有しています。
//更新履歴		:	08/04/28	Ver0.01.00	基本機能を実装。
//					08/05/08	Ver0.11.00	Ogg Vorbisに対応。
//					08/05/12	Ver0.21.00	OggVorbisを扱う処理をクラスとして独立。
//					08/06/26	Ver0.31.00	エフェクトに対応。
//-------------------------------------------------------------------------------------------------------------------------------
//プラグマ指令

#pragma once
#pragma warning(disable:4996)

//ヘッダーファイルのインクルード
#include "Class_Sound.h"
#include "Class_OggVorbis.h"
#include <process.h>

//-------------------------------------------------------------------------------------------------------------------------------
//クラス名		:	CSTATICSOUND_OGGVORBIS
//作成者		:	志場雅広
//作成日		:	08/05/01
//機能			:	サウンド関連処理。
//備考			:	OggVorbisをスタティックバッファで再生します。
//更新履歴		:	08/05/08	Ver1.01.00	機能を実装
//					08/06/26	Ver1.11.00	エフェクトに対応。
//-------------------------------------------------------------------------------------------------------------------------------
class CSTATICSOUND_OGGVORBIS : public CSOUND {
private:
	/***音声の再生に必要なデータ***/
	LPDIRECTSOUNDBUFFER8 DSBuffer;													//DirectSoundセカンダリバッファ(サウンドのデータ)
	COGG_VORBIS OggVorbis;															//OggVorbis関連
	char szFileName[MAX_PATH];														//読み込むサウンドファイルのパス
	DSBUFFERDESC dsbd;																//サウンドデータのデバイス情報
	WAVEFORMATEX wfex;																//サウンドデータのフォーマット
	DWORD dataSize;																	//サウンドデータのサイズ

	/***DirectSound関連***/
	SOUND_ERROR initialize(void);													//データ初期化
	SOUND_ERROR initSoundBuffer(const LPSTR szFileName);							//サウンドバッファの作成
	SOUND_ERROR releaseSoundBuffer(void);											//サウンドバッファの解放
	BOOL checkSoundBuffer(void);													//バッファのチェック
public:
	/***コンストラクタとデストラクタ***/
	CSTATICSOUND_OGGVORBIS(void) { this->initialize(); }							//デフォルトコンストラクタ
	CSTATICSOUND_OGGVORBIS(const LPSTR szFileName);									//コンストラクタ
	virtual ~CSTATICSOUND_OGGVORBIS(void);											//デストラクタ

	/***データ操作***/
	LPSTR getFileName(void) { return this->szFileName; }							//ファイル名取得

	/***データサイズ操作***/
	//機能:サウンドデータのサイズ取得	引数:なし	戻り値:サウンドデータのサイズ(byte単位)
	DWORD getSoundDataSize(void) { return this->dataSize; }						//サウンドデータのサイズを返す(byte単位)
	//機能:サウンドデータのサイズ取得	引数:なし	戻り値:サウンドデータのサイズ(秒単位)
	DWORD getSoundDataSizeSec(void) {												//サウンドデータのサイズを返す(秒単位)
		return this->getSoundDataSize() / this->wfex.nAvgBytesPerSec;
	}

	/***再生位置操作***/
	//機能:再生位置の取得	引数:なし	戻り値:再生位置(byte単位)
	DWORD getPlayCur(void);															//再生位置の取得(byte単位)
	//機能:再生位置の取得	引数:なし	戻り値:再生位置(秒単位)
	DWORD getPlayCurSec(void) {														//再生位置の取得(秒単位)
		return this->getPlayCur() / this->wfex.nAvgBytesPerSec;						//再生時間を秒単位で返す
	}
	//機能:再生位置の設定	引数:再生位置(byte単位)	戻り値:なし
	void setPlayCur(DWORD dwPosition) {												//再生位置の設定(byte単位)
		this->DSBuffer->SetCurrentPosition(dwPosition);								//再生位置を変更する
	}
	//機能:再生位置の設定	引数:再生位置(秒単位)	戻り値:なし
	void setPlayCurSec(DWORD dwSec) {												//再生位置の設定(秒単位)
		this->DSBuffer->SetCurrentPosition(dwSec									//再生位置を変更する
						* this->wfex.nAvgBytesPerSec);
	}

	/***再生状態の確認***/
	//機能:再生状態の確認	引数:なし	戻り値:再生状態を示す値(DSBSTATUS_PLAYINGが入っている時は再生中)
	DWORD getSoundPram(void) { DWORD dwStatus;
	this->DSBuffer->GetStatus(&dwStatus); return dwStatus; }

	/***サウンド操作***/
	SOUND_ERROR play(DWORD dwPriority, DWORD dwFlag);								//音声再生
	SOUND_ERROR stop(void);															//再生停止
	SOUND_ERROR setEffect(GUID guidEffect, DWORD dwPriority, DWORD dwFlag);			//エフェクトの設定
	SOUND_ERROR deleteEffect(DWORD dwPriority, DWORD dwFlag);						//設定したエフェクトを削除する
	//機能:ボリュームの取得		引数:なし			戻り値:ボリューム
	LONG getVolume(void) { LONG vol; this->DSBuffer->GetVolume(&vol); return vol; }
	//機能:ボリュームの設定		引数:ボリューム		戻り値:なし
  	void setVolume(LONG vol) { this->DSBuffer->SetVolume(vol); }
	//機能:パンの取得		引数:なし			戻り値:パン
	LONG getPan(void) { LONG pan; this->DSBuffer->GetPan(&pan); return pan; }
	//機能:パンの設定		引数:パン	戻り値:なし
	void setPan(LONG pan) { this->DSBuffer->SetPan(pan); }
};

//-------------------------------------------------------------------------------------------------------------------------------
//クラス名		:	CSTREAMINGSOUND_OGGVORBIS
//作成者		:	志場雅広
//作成日		:	08/05/12
//機能			:	サウンド関連処理。
//備考			:	OggVorbisをストリーミングバッファで再生します。
//					ストリーミングはスレッドによって行っており、スレッド間の同期にはクリティカルセクションを用いています。
//更新履歴		:	08/05/12	Ver1.00.00	機能を実装
//					08/06/26	Ver1.11.00	エフェクトに対応。
//-------------------------------------------------------------------------------------------------------------------------------
class CSTREAMINGSOUND_OGGVORBIS : public CSOUND {
private:
	/***音声の再生に必要なデータ***/
	LPDIRECTSOUNDBUFFER8 DSBuffer;													//DirectSoundセカンダリバッファ(サウンドのデータ)
	COGG_VORBIS OggVorbis;															//OggVorbis関連
	char szFileName[MAX_PATH];														//読み込むサウンドファイルのパス
	DSBUFFERDESC dsbd;																//サウンドデータのデバイス情報
	WAVEFORMATEX wfex;																//サウンドデータのフォーマット
	DWORD dataSize;																	//サウンドデータのサイズ

	/***ストリーミング用データ***/
	LPDIRECTSOUNDNOTIFY8 DSNotify;													//DirectSoundNotify8インターフェース

	/***イベント関連***/
	HANDLE hNotifyEvent[kEVENTMAX];													//イベントのハンドル
	DWORD blockNum;																	//通知回数
	DWORD blockSize;																//通知発生間隔

	/***スレッド関連***/
	HANDLE hNotifyThread;															//スレッドのハンドル
	unsigned int threadId;															//スレッドID(識別子)

	/***同期関連***/
	CRITICAL_SECTION cs;															//クリティカルセクション

	/***ストリーミング再生に使用***/
	DWORD bufferSize;																//ストリーミング用バッファのサイズ(byte単位)
	DWORD readCur;																	//読み込みカーソル(ファイルの読み込み位置)
	DWORD writeCur;																	//書き込みカーソル(バッファに書き込む位置)
	DWORD playCnt;																	//再生回数を保存
	DWORD playCur;																	//現在の再生位置(大まかな値)
	PLAY_FLAG playFlag;																//再生フラグ(ループ再生かどうか)

	/***DirectSound関連***/
	SOUND_ERROR initialize(void);													//データ初期化
	SOUND_ERROR initSoundBuffer(const LPSTR szFileName);							//サウンドバッファの作成
	SOUND_ERROR releaseSoundBuffer(void);											//サウンドバッファの解放
	SOUND_ERROR setStreaming(void);													//ストリーミングを行うための設定をする
	BOOL checkSoundBuffer(void);													//バッファのチェック	
public:
	/***コンストラクタとデストラクタ***/
	CSTREAMINGSOUND_OGGVORBIS(void) { this->initialize(); }							//デフォルトコンストラクタ
	CSTREAMINGSOUND_OGGVORBIS(const LPSTR szFileName);								//コンストラクタ
	virtual ~CSTREAMINGSOUND_OGGVORBIS(void);										//デストラクタ

	/***データ操作***/
	LPSTR getFileName(void) { return this->szFileName; }							//ファイル名取得

	/***データサイズ操作***/
	//機能:サウンドデータのサイズ取得	引数:なし	戻り値:サウンドデータのサイズ(byte単位)
	DWORD getSoundDataSize(void) { return this->dataSize; }							//サウンドデータのサイズを返す(byte単位)
	//機能:サウンドデータのサイズ取得	引数:なし	戻り値:サウンドデータのサイズ(秒単位)
	DWORD getSoundDataSizeSec(void) {												//サウンドデータのサイズを返す(秒単位)
		return this->getSoundDataSize() / this->wfex.nAvgBytesPerSec;
	}

	/***再生位置操作***/
	//機能:再生位置の取得	引数:なし	戻り値:再生位置(byte単位)
	DWORD getPlayCur(void);															//再生位置の取得(byte単位)
	//機能:再生位置の取得	引数:なし	戻り値:再生位置(秒単位)
	DWORD getPlayCurSec(void) {														//再生位置の取得(秒単位)
		return this->getPlayCur() / this->wfex.nAvgBytesPerSec;						//再生時間を秒単位で返す
	}
	//機能:再生位置の設定	引数:再生位置(byte単位)	戻り値:なし
	void setPlayCur(DWORD dwPosition) {												//再生位置の設定(byte単位)
		//未実装
	}
	//機能:再生位置の設定	引数:再生位置(秒単位)	戻り値:なし
	void setPlayCurSec(DWORD dwSec) {												//再生位置の設定(秒単位)
		//未実装
	}

	/***再生状態の確認***/
	//機能:再生状態の確認	引数:なし	戻り値:再生状態を示す値(DSBSTATUS_PLAYINGが入っている時は再生中)
	DWORD getSoundPram(void) { DWORD dwStatus;
	this->DSBuffer->GetStatus(&dwStatus); return dwStatus; }

	/***ストリーミング関連***/
	unsigned int WINAPI NotifyProc(void);											//ストリーミングを行う
	SOUND_ERROR readStream(void);													//ストリーミング内でのデータ読み込み処理

	/***サウンド操作***/
	SOUND_ERROR play(DWORD dwPriority, DWORD dwFlag);								//音声再生
	SOUND_ERROR stop(void);															//再生停止
	SOUND_ERROR setEffect(GUID guidEffect, DWORD dwPriority, DWORD dwFlag);			//エフェクトの設定
	SOUND_ERROR deleteEffect(DWORD dwPriority, DWORD dwFlag);						//設定したエフェクトを削除する
	//機能:ボリュームの取得		引数:なし			戻り値:ボリューム
	LONG getVolume(void) { LONG vol; this->DSBuffer->GetVolume(&vol); return vol; }
	//機能:ボリュームの設定		引数:ボリューム		戻り値:なし
	void setVolume(LONG vol) { this->DSBuffer->SetVolume(vol); }
	//機能:パンの取得		引数:なし			戻り値:パン
	LONG getPan(void) { LONG pan; this->DSBuffer->GetPan(&pan); return pan; }
	//機能:パンの設定		引数:パン	戻り値:なし
	void setPan(LONG pan) { this->DSBuffer->SetPan(pan); }
};

//関数宣言
//クラス内のスレッドを利用するために使用
unsigned int WINAPI Notify(LPVOID lpParam);

//省略
#define	SSTATIC			CSTATICSOUND_OGGVORBIS
#define	SSTREAMING		CSTREAMINGSOUND_OGGVORBIS