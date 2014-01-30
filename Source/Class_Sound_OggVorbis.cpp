//-------------------------------------------------------------------------------------------------------------------------------
//ファイル名	:	Class_SoundOggVorbis.cpp
//作成者		:	志場雅広
//作成日		:	08/04/28
//機能			:	音声データの再生。
//備考			:	OggVorbisを扱うための操作や処理の提供。
//					CSoundクラスのデータを共有し使用しています。
//更新履歴		:	08/04/28	Ver0.01.00	DirectSoundの初期化処理を実装。
//					08/05/08	Ver0.11.00	Ogg Vorbisに対応。
//-------------------------------------------------------------------------------------------------------------------------------
//ヘッダーファイルのインクルード

#include	"Precompiler.h"

//#include	"Class_Sound_OggVorbis.h"

//-------------------------------------------------------------------------------------------------------------------------------
//	テクスチャリソースファイルへのパス
//-------------------------------------------------------------------------------------------------------------------------------
#define		PATH_SSTATIC_DATA		"..//Resources//Sound//Static//%s.ogg"
#define		PATH_SSTREAMING_DATA	"..//Resources//Sound//Streaming//%s.ogg"

/***スタティックバッファ***/
//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	CSTATICSOUND_OGGVORBIS
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	サウンド関連処理。
//備考			:	CSTATICSOUND_OGGVORBISのコンストラクタです。
//クラス		:	CSTATICSOUND_OGGVORBIS
//引数			:	const LPSTR	szFileName		読み込むファイル名
//戻り値		:	なし
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
CSTATICSOUND_OGGVORBIS::CSTATICSOUND_OGGVORBIS(const LPSTR szFileName)
{
	this->initialize();													//データ初期化

	this->initSoundBuffer(szFileName);
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	~CSTATICSOUND_OGGVORBIS
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	サウンド関連処理。
//備考			:	CSTATICSOUND_OGGVORBISのデストラクタです。
//クラス		:	CSTATICSOUND_OGGVORBIS
//引数			:	なし
//戻り値		:	なし
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
CSTATICSOUND_OGGVORBIS::~CSTATICSOUND_OGGVORBIS()
{
	this->releaseSoundBuffer();											//サウンドバッファの開放
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	initialize
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	サウンド関連処理。
//備考			:	データを初期化します。
//クラス		:	CSTATICSOUND_OGGVORBIS
//引数			:	なし
//戻り値		:	なし
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTATICSOUND_OGGVORBIS::initialize()
{
	//データを初期化
	this->DSBuffer = NULL;												//バッファを初期化
	ZeroMemory(&this->szFileName, sizeof(this->szFileName));			//サウンドファイル名を初期化
	ZeroMemory(&this->dsbd, sizeof(DSBUFFERDESC));						//デバイスデータを初期化
	ZeroMemory(&this->wfex, sizeof(WAVEFORMATEX));						//フォーマット情報を初期化
	this->dataSize = 0;													//データサイズを初期化
	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	initSoundBuffer
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	サウンド関連処理。
//備考			:	OggVorbisを再生するための準備を行います。
//クラス		:	CSTATICSOUND_OGGVORBIS
//引数			:	const LPSTR szFileName	読み込むファイル名
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//					08/05/12	ver1.10.00	OggVorbisの処理を独立。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTATICSOUND_OGGVORBIS::initSoundBuffer(const LPSTR szFileName)
{
//	サウンドパスの添付
	TCHAR			realName[MAX_PATH]	= "";							//!< 名前にパスをつける為

//	パス付きサウンドファイル名を生成
	sprintf_s(realName, sizeof(realName), PATH_SSTATIC_DATA, szFileName);

	//インターフェースが存在するか判定
	if(!this->checkSoundInterface()) {									//インターフェースが存在しない
		return eSOUND_NOTINTERFACE;
	}

	//--------------------------------------------------
	//OggVorbisの読み込み
	//--------------------------------------------------
	//OggVorbisファイルオープン
	if(this->OggVorbis.openOggVorbis(realName) != eOGG_OK) {	//OggVorbisファイルのオープンに失敗
		return eSOUND_OPEN;												//エラー情報を返す
	}

	//読み込むファイル名を設定
	strcpy_s(this->szFileName, sizeof(this->szFileName), szFileName);

	this->wfex = this->OggVorbis.getWaveFormat();						//WAVEフォーマットの取得
	this->dataSize = this->OggVorbis.getDataSize();						//音声データのサイズを取得

	//--------------------------------------------------
	//セカンダリバッファの作成
	//--------------------------------------------------
	//メモリ確保
	LPDIRECTSOUNDBUFFER pDSBuffer;										//DirectSoundバッファ(DirectSoundBuffer8取得に使用)
	stSoundBuffer sBuffer;												//音声データを読み込むためのバッファ
	ZeroMemory(&sBuffer, sizeof(stSoundBuffer));						//バッファの初期化

	//デバイスの設定
	ZeroMemory(&this->dsbd, sizeof(this->dsbd));						//デバイス情報を0クリア

	this->dsbd.dwSize = sizeof(DSBUFFERDESC);							//構造体のサイズを設定
	this->dsbd.dwFlags = DSBCAPS_CTRLPAN								//サウンドバッファの能力を設定
							| DSBCAPS_CTRLVOLUME
							| DSBCAPS_LOCDEFER
							| DSBCAPS_CTRLFX
							| DSBCAPS_GETCURRENTPOSITION2;
	this->dsbd.dwBufferBytes = this->dataSize;							//バッファのサイズの設定
	this->dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;						//3D音源のアルゴリズム
	this->dsbd.lpwfxFormat = &this->wfex;								//フォーマット情報の設定
	
	//バッファの生成
	if(FAILED(this->SoundInterface->CreateSoundBuffer(					//失敗した場合
							&this->dsbd, &pDSBuffer, NULL))) {
       return eSOUND_CREATEBUFFER;
	}
	//DirectSoundBuffer8取得
	if(FAILED(pDSBuffer->QueryInterface(								//失敗した場合
		IID_IDirectSoundBuffer8, (LPVOID*)&this->DSBuffer))) {
		kRELEASE(pDSBuffer);											//バッファを開放
		return eSOUND_CREATEBUFFER;
	}
	kRELEASE(pDSBuffer);												//バッファを開放

	//バッファのロック
	if(FAILED(this->DSBuffer->Lock(0, this->dataSize,					//バッファのロックに失敗
			&sBuffer.pBuffer, &sBuffer.dwBufferSize, NULL, NULL, 0))) {
	   return eSOUND_LOCK;
	}

	//データの読み込み処理
	if(this->OggVorbis.readData(&sBuffer,								//データの読み込み
									sBuffer.dwBufferSize) != eOGG_OK) {
		return eSOUND_READ;
	}
	
	//バッファのアンロック
	if(FAILED(this->DSBuffer->Unlock(sBuffer.pBuffer,					//アンロックに失敗
							sBuffer.dwBufferSize, NULL, 0))) {
		return eSOUND_UNLOCK;											//エラー情報を返す
	}


	//OggVorbisファイルクローズ
	if(this->OggVorbis.closeOggVorbis() != eOGG_OK) {					//OggVorbisファイルのクローズに失敗
		return eSOUND_CLOSE;											//エラー情報を返す
	}
	this->setVolume(-2000);

	return eSOUND_OK;													//正常成功
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	releaseSoundBuffer
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	サウンド関連処理。
//備考			:	解放処理を行います。
//クラス		:	CSTATICSOUND_OGGVORBIS
//引数			:	なし
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTATICSOUND_OGGVORBIS::releaseSoundBuffer()
{
	kRELEASE(this->DSBuffer);											//サウンドバッファの解放
	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	checkSoundBuffer
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	サウンド関連処理。
//備考			:	バッファが正常な状態かチェックします。
//クラス		:	CSTATICSOUND_OGGVORBIS
//引数			:	なし
//戻り値		:	BOOL	TRUE	正常
//							FALSE	異常
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
BOOL CSTATICSOUND_OGGVORBIS::checkSoundBuffer()
{
	//バッファが正常に生成されているか判定
	if(!this->DSBuffer) {												//生成されていない場合
		return FALSE;													//異常
	}
	return TRUE;														//正常
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	play
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	サウンド関連処理。
//備考			:	サウンドを再生します。
//					IDirectSoundBuffer8::Playが基本となっています。
//					引数の優先順位と再生方法を指定するフラグIDirectSoundBuffer8::Playと同じように使用してください。
//クラス		:	CSTATICSOUND_OGGVORBIS
//引数			:	DWORD	dwPriority	優先順位(0が一番低い)
//					DWORD	dwFlag		再生方法を指定するフラグ
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTATICSOUND_OGGVORBIS::play(DWORD dwPriority, DWORD dwFlag)
{
	//バッファが正常に生成されているか判定
	if(!this->checkSoundBuffer()) {										//生成されていない場合
		return eSOUND_NOTBUFFER;
	}
	HRESULT hr;															//再生の戻り値を格納

	hr = this->DSBuffer->Play(NULL, dwPriority, dwFlag);				//サウンド再生

	//デバイスロストの判定
	if(hr == DSERR_BUFFERLOST) {										//デバイスが存在しない
		this->DSBuffer->Restore();										//サウンドバッファの復元を試みる
		this->DSBuffer->Play(NULL, dwPriority, dwFlag);					//サウンド再生
	}
	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	stop
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	サウンド関連処理。
//備考			:	サウンドを停止します。
//クラス		:	CSTATICSOUND_OGGVORBIS
//引数			:	なし
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTATICSOUND_OGGVORBIS::stop()
{
	//バッファが正常に生成されているか判定
	if(!this->checkSoundBuffer()) {										//生成されていない場合
		return eSOUND_NOTBUFFER;
	}

	this->DSBuffer->Stop();												//再生停止

	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	setEffect
//作成者		:	志場雅広
//作成日		:	08/06/26
//機能			:	サウンド関連処理。
//備考			:	エフェクトを設定します。
//					エフェクトを使用するには一度再生を止めてから設定を行う必要があります。
//					この中では一度再生を停止した後、エフェクトの設定を行い指定された方法に従って再び再生を行います。
//エフェクトを指定するフラグ一覧
//					GUID_DSFX_STANDARD_CHORUS		コーラス
//					GUID_DSFX_STANDARD_COMPRESSOR	コンプレッサ 
//					GUID_DSFX_STANDARD_DISTORTION	ディストーション
//					GUID_DSFX_STANDARD_ECHO			エコー
//					GUID_DSFX_STANDARD_FLANGER		フランジ
//					GUID_DSFX_STANDARD_GARGLE		ガーグル
//					GUID_DSFX_STANDARD_I3DL2REVERB	Interactive 3D Level 2 リバーブ
//					GUID_DSFX_STANDARD_PARAMEQ		パラメトリック イコライザ
//					GUID_DSFX_WAVES_REVERB			Waves リバーブ
//クラス		:	CSTATICSOUND_OGGVORBIS
//引数			:	GUID guidEffect		エフェクトの種類を指定するフラグ
//					DWORD	dwPriority	優先順位(0が一番低い)
//					DWORD	dwFlag		再生方法を指定するフラグ
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/06/26	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTATICSOUND_OGGVORBIS::setEffect(GUID guidEffect, DWORD dwPriority, DWORD dwFlag)
{
	HRESULT hr;															//戻り値チェック

	//バッファが正常に生成されているか判定
	if(!this->checkSoundBuffer()) {										//生成されていない場合
		return eSOUND_NOTBUFFER;
	}

	this->stop();														//エフェクトを設定するため再生停止

	//DirectSoundBufferにエフェクトを設定
	DSEFFECTDESC dsEffect[1];
	dsEffect[0].dwSize = sizeof(DSEFFECTDESC);							//サイズ
	dsEffect[0].dwFlags = 0;											//エフェクトをハードウェア生成するかソフトウェア生成するか
	dsEffect[0].guidDSFXClass = guidEffect;								//エフェクトのGUID
	dsEffect[0].dwReserved1 = 0;										//予約済み
	dsEffect[0].dwReserved2 = 0;										//予約済み

	//エフェクトを設定
	DWORD dwResults[1];
	hr = this->DSBuffer->SetFX(1, dsEffect, dwResults);					//エフェクトを設定
	this->play(dwPriority, dwFlag);										//再生を再開
	//エフェクトの設定に成功したか判定
	if(FAILED(hr)) {													//失敗した場合
		return eSOUND_EFFECT;
	}

	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	deleteEffect
//作成者		:	志場雅広
//作成日		:	08/06/26
//機能			:	サウンド関連処理。
//備考			:	すべてのエフェクトを削除します。
//					エフェクトを削除するには一度再生を止めてから設定を行う必要があります。
//					この中では一度再生を停止した後、エフェクトの削除を行い指定された方法に従って再び再生を行います。
//クラス		:	CSTATICSOUND_OGGVORBIS
//引数			:	DWORD	dwPriority	優先順位(0が一番低い)
//					DWORD	dwFlag		再生方法を指定するフラグ
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/06/26	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTATICSOUND_OGGVORBIS::deleteEffect(DWORD dwPriority, DWORD dwFlag)
{
	HRESULT hr;

	//バッファが正常に生成されているか判定
	if(!this->checkSoundBuffer()) {										//生成されていない場合
		return eSOUND_NOTBUFFER;
	}

	this->stop();														//エフェクトを設定するため再生停止

	//バッファのすべてのエフェクトを削除する
	hr = this->DSBuffer->SetFX(0, NULL, NULL);
	//エフェクトの削除に成功したか判定
	if(FAILED(hr)) {													//失敗した場合
		return eSOUND_EFFECT;
	}

	this->play(dwPriority, dwFlag);										//再生を再開

	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	getPlayCur
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	サウンド関連処理。
//備考			:	再生位置を取得します。
//クラス		:	CSTATICSOUND_OGGVORBIS
//引数			:	DWORD	再生位置
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
DWORD CSTATICSOUND_OGGVORBIS::getPlayCur()
{
	//メモリ確保
	DWORD pc, wc;														//カーソル位置

	//再生位置取得
	this->DSBuffer->GetCurrentPosition(&pc, &wc);

	return pc;															//再生位置を返す
}






/***ストリーミングバッファ***/
//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	CSTREAMINGSOUND_OGGVORBIS
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	CSTREAMINGSOUND_OGGVORBISクラスのコンストラクタです。
//備考			:	
//クラス		:	CSTREAMINGSOUND_OGGVORBIS
//引数			:	const LPSTR	szFileName	読み込むファイル名
//戻り値		:	なし
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
CSTREAMINGSOUND_OGGVORBIS::CSTREAMINGSOUND_OGGVORBIS(const LPSTR szFileName)
{
	this->initialize();													//データ初期化
	InitializeCriticalSection(&this->cs);								//クリティカルセクションの初期化

	this->initSoundBuffer(szFileName);									//バッファの生成
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	~CSTREAMINGSOUND_OGGVORBIS
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	CSTREAMINGSOUND_OGGVORBISクラスのデストラクタです。
//備考			:	解放処理を行っています。
//クラス		:	CSTREAMINGSOUND_OGGVORBIS
//引数			:	なし
//戻り値		:	なし
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
CSTREAMINGSOUND_OGGVORBIS::~CSTREAMINGSOUND_OGGVORBIS()
{
	this->releaseSoundBuffer();											//サウンドバッファの解放
	DeleteCriticalSection(&this->cs);									//クリティカルセクションの解放
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	initialize
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	データ初期化。
//備考			:	
//クラス		:	CSTREAMINGSOUND_OGGVORBIS
//引数			:	なし
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::initialize()
{
	//データを初期化
	this->DSBuffer = NULL;												//バッファを初期化
	ZeroMemory(&this->szFileName, sizeof(this->szFileName));			//サウンドファイル名を初期化
	ZeroMemory(&this->dsbd, sizeof(DSBUFFERDESC));						//デバイスデータを初期化
	ZeroMemory(&this->wfex, sizeof(WAVEFORMATEX));						//フォーマット情報を初期化
	this->dataSize = 0;													//データサイズを初期化

	//ストリーミング関連データ
	this->DSNotify = NULL;												//DirectSoundNotify8インターフェースを初期化

	//イベントハンドルを初期化
	for(int i = 0; i < kEVENTMAX; i++) {								//ハンドル数分繰り返す
		this->hNotifyEvent[i] = NULL;									//イベントハンドルを初期化
	}

	this->blockNum = 0;													//通知回数を初期化
	this->blockSize = 0;												//通知発生間隔を初期化

	this->hNotifyThread = NULL;											//スレッドハンドルを初期化
	this->threadId = 0;													//スレッドIDを初期化

	this->bufferSize = 0;												//バッファサイズを初期化
	this->readCur = 0;													//読み込みカーソルを初期化
	this->writeCur = 0;													//書き込みカーソルを初期化
	this->playCnt = 0;													//再生回数を初期化
	this->playCur = 0;													//再生位置を初期化
	this->playFlag = ePLAY_ONCE;										//再生フラグを初期化(一回のみの再生)

	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	initSoundBuffer
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	バッファの生成。
//備考			:	
//クラス		:	CSTREAMINGSOUND_OGGVORBIS
//引数			:	const LPSTR szFileName	読み込むファイル名
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::initSoundBuffer(const LPSTR szFileName)
{
//	サウンドパスの添付
	TCHAR			realName[MAX_PATH]	= "";							//!< 名前にパスをつける為

//	パス付きサウンドファイル名を生成
	sprintf_s(realName, sizeof(realName), PATH_SSTREAMING_DATA, szFileName);

	//インターフェースが存在するか判定
	if(!this->checkSoundInterface()) {									//インターフェースが存在しない
		return eSOUND_NOTINTERFACE;
	}

	//--------------------------------------------------
	//OggVorbisの読み込み
	//--------------------------------------------------
	//OggVorbisファイルオープン
	if(this->OggVorbis.openOggVorbis(realName) != eOGG_OK) {	//OggVorbisファイルのオープンに失敗
		return eSOUND_OPEN;												//エラー情報を返す
	}

	//読み込むファイル名を設定
	strcpy_s(this->szFileName, sizeof(this->szFileName), szFileName);

	this->wfex = this->OggVorbis.getWaveFormat();						//WAVEフォーマットの取得
	this->dataSize = this->OggVorbis.getDataSize();						//音声データのサイズを取得

	//ストリーミング用データを設定
	this->blockNum = KBLOCKNUM;											//通知回数
	this->blockSize = this->wfex.nAvgBytesPerSec;						//通知発生間隔(1秒分)

	//サウンドが3秒以上か判定(3秒以下の場合はストリーミングは行わない)
	if(this->dataSize > this->blockSize * 3) {							//3秒以上の場合
		this->bufferSize = this->blockNum * this->blockSize;			//バッファのサイズを設定
	} else {															//3秒以上の場合
		this->bufferSize = this->dataSize;								//バッファのサイズを設定
	}

	//--------------------------------------------------
	//セカンダリバッファの作成
	//--------------------------------------------------
	//メモリ確保
	LPDIRECTSOUNDBUFFER pDSBuffer;										//DirectSoundバッファ(DirectSoundBuffer8取得に使用)
	stSoundBuffer sBuffer;												//音声データを読み込むためのバッファ
	ZeroMemory(&sBuffer, sizeof(stSoundBuffer));						//バッファの初期化

	//デバイスの設定
	ZeroMemory(&this->dsbd, sizeof(this->dsbd));						//デバイス情報を0クリア

	this->dsbd.dwSize = sizeof(DSBUFFERDESC);							//構造体のサイズを設定
	this->dsbd.dwFlags = DSBCAPS_CTRLPAN								//サウンドバッファの能力を設定
							| DSBCAPS_CTRLVOLUME
							| DSBCAPS_CTRLPOSITIONNOTIFY
							| DSBCAPS_LOCDEFER
							| DSBCAPS_CTRLFX
							| DSBCAPS_GETCURRENTPOSITION2;
	this->dsbd.dwBufferBytes = this->bufferSize;						//バッファのサイズの設定
	this->dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;						//3D音源のアルゴリズム
	this->dsbd.lpwfxFormat = &this->wfex;								//フォーマット情報の設定

	VOID *pBuffer;														//ロックしたバッファの位置
	
	//バッファの生成
	if(FAILED(this->SoundInterface->CreateSoundBuffer(&this->dsbd,		//失敗した場合
											&pDSBuffer, NULL))) {
       return eSOUND_CREATEBUFFER;
	}
	//DirectSoundBuffer8取得
	if(FAILED(pDSBuffer->QueryInterface(								//失敗した場合
		IID_IDirectSoundBuffer8, (LPVOID*)&this->DSBuffer))) {
		kRELEASE(pDSBuffer);											//バッファを開放
		return eSOUND_CREATEBUFFER;
	}
	kRELEASE(pDSBuffer);												//バッファを開放

	//バッファのロック
	if(FAILED(this->DSBuffer->Lock(0, this->bufferSize,					//バッファのロックに失敗
					&sBuffer.pBuffer, &sBuffer.dwBufferSize, &pBuffer, NULL, 0))) {
	   return eSOUND_LOCK;
	}

	//データの読み込み処理
	if(this->OggVorbis.readData(&sBuffer,								//データの読み込み
									sBuffer.dwBufferSize) != eOGG_OK) {
		return eSOUND_READ;
	}
	
	//バッファのアンロック
	if(FAILED(this->DSBuffer->Unlock(sBuffer.pBuffer,					//アンロックに失敗
							sBuffer.dwBufferSize, NULL, 0))) {
		return eSOUND_UNLOCK;											//エラー情報を返す
	}

	//読み込みカーソルと書き込みカーソルの移動
	this->readCur = this->OggVorbis.getReadPos();						//読み込みカーソルを移動
	this->writeCur = 0;													//書き込みカーソルを移動

	this->setVolume(-2000);
	//--------------------------------------------------
	//ストリーミングの設定
	//--------------------------------------------------
	//バッファサイズが3秒以上か判定(3秒以下はストリーミングを行わない)
	if(this->bufferSize >= this->blockSize * 3) {						//3秒以上
		return this->setStreaming();									//ストリーミングの設定を行う
	}
	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	setStreaming
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	設定処理。
//備考			:	ストリーミングを行うための設定を行っています。
//クラス		:	CSTREAMINGSOUND_OGGVORBIS
//引数			:	なし
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/07/03	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::setStreaming()
{
	DSBPOSITIONNOTIFY DsPos[KBLOCKNUM + 1];								//通知位置情報

	//DirectSoundNotifyインターフェースの取得
	if(FAILED(this->DSBuffer->QueryInterface(IID_IDirectSoundNotify8,	//取得に失敗
											(LPVOID*)&this->DSNotify))) {
		return eSOUND_QUERYINTERFACE;									//エラー情報を返す
	}

	//イベントオブジェクトの作成
	for(int i = 0; i < kEVENTMAX; i++) {								//イベント数分繰り返す
		this->hNotifyEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);	//イベントオブジェクトの作成
	}

	//通知位置の設定
	//再生カーソルが指定されたオフセットを過ぎたときにイベントを発生させる
	int iBlock = 1;														//設定するブロック数
	for(int i = 0; i < KBLOCKNUM; i++) {								//最後の1つを除いてイベントを設定
		DsPos[i].dwOffset = this->blockSize * iBlock - 1;				//ブロック数直前
		DsPos[i].hEventNotify = this->hNotifyEvent[0];					//イベントオブジェクトの設定
		iBlock++;														//ブロック数をアップ
	}
	DsPos[KBLOCKNUM].dwOffset = DSBPN_OFFSETSTOP;						//最後の一つは再生停止検出用
	DsPos[KBLOCKNUM].hEventNotify = this->hNotifyEvent[1];				//イベントオブジェクトの設定

	//イベントの設定
	if(FAILED(this->DSNotify->SetNotificationPositions(					//設定に失敗
										KBLOCKNUM + 1, DsPos))){
		return eSOUND_SETNOTIFICATIONPOSITIONS;							//エラー情報を返す
	}

	//スレッドの作成
	this->hNotifyThread = (HANDLE)_beginthreadex(NULL, 0, Notify, 
				reinterpret_cast<void *>(this), 0, &this->threadId);

	//スレッドの優先順位の設定
	SetThreadPriority(this->hNotifyThread, THREAD_PRIORITY_ABOVE_NORMAL);

	return eSOUND_OK;													//正常成功
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	releaseSoundBuffer
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	解放処理。
//備考			:	スレッドやバッファの解放処理を行っています。
//クラス		:	CSTREAMINGSOUND_OGGVORBIS
//引数			:	なし
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::releaseSoundBuffer()
{
	DWORD dwExitCode;													//スレッドの終了コード

	EnterCriticalSection(&this->cs);									//クリティカルセクションの開始

	//終了処理
	this->stop();														//再生停止

	this->OggVorbis.closeOggVorbis();									//OggVorbisファイルのクローズ

	//スレッドが存在するか判定
	if(this->hNotifyThread) {											//スレッドが存在する
		//スレッドを終了
		PostThreadMessage(this->threadId, WM_END_THREAD, 0, 0);			//スレッドに終了メッセージを送る
		/**/
		WaitForSingleObject(this->hNotifyThread, kTIMEOUT);				//スレッドが終了するまで一定時間待機
		GetExitCodeThread(this->hNotifyThread, &dwExitCode);			//スレッドの状態を取得
		//スレッドがまだ動いているか判定
		if(dwExitCode == STILL_ACTIVE) {								//スレッドが稼働中
			//スレッドが自主的に終了されていない場合は強制終了
			if(!TerminateThread(this->hNotifyThread, 0)) {				//強制終了に失敗
				WaitForSingleObject(this->hNotifyThread, kTIMEOUT);		//スレッドが終了するまで待機(もう少し待ってみる)
				TerminateThread(this->hNotifyThread, 0);				//スレッドを強制終了(これで終了しなければ諦める)
			}
		}
		
	}
/**/

	//イベントハンドルのクローズ
	for(int i = 0; i < kEVENTMAX; i++) {								//イベント数分繰り返す
		kCLOSEHANDLE(this->hNotifyEvent[i]);							//イベントハンドルを閉じる
	}

	//スレッドハンドルのクローズ
	if(this->hNotifyThread != NULL) {									//スレッドハンドルが解放されていない
		kCLOSEHANDLE(this->hNotifyThread);								//スレッドハンドルを閉じる
	}

	kRELEASE(this->DSBuffer);											//サウンドバッファの解放

	LeaveCriticalSection(&this->cs);									//クリティカルセクションの終了

	return eSOUND_OK;													//正常終了
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	checkSoundBuffer
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	サウンド関連処理。
//備考			:	バッファが正常な状態かチェックします。
//クラス		:	CSTREAMINGSOUND_OGGVORBIS
//引数			:	なし
//戻り値		:	BOOL	TRUE	正常
//							FALSE	異常
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
BOOL CSTREAMINGSOUND_OGGVORBIS::checkSoundBuffer()
{
	EnterCriticalSection(&this->cs);									//クリティカルセクションの開始

	//バッファが正常に生成されているか判定
	if(!this->DSBuffer) {												//生成されていない場合
		LeaveCriticalSection(&this->cs);								//クリティカルセクションの終了
		return FALSE;													//異常
	}

	LeaveCriticalSection(&this->cs);									//クリティカルセクションの終了
	return TRUE;														//正常
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	play
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	サウンド関連処理。
//備考			:	サウンドを再生します。
//					IDirectSoundBuffer8::Playが基本となっています。
//					引数の優先順位はIDirectSoundBuffer8::Playと同じように使用してください。
//					再生方法を指定するフラグは一回のみの再生(DSBPLAY_LOOPINGを指定しない)を指定することが可能です。
//クラス		:	CSTREAMINGSOUND_OGGVORBIS
//引数			:	DWORD	dwPriority	優先順位(0が一番低い)
//					DWORD	dwFlag		再生方法を指定するフラグ
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::play(DWORD dwPriority, DWORD dwFlag)
{
	EnterCriticalSection(&this->cs);									//クリティカルセクションの開始

	//バッファが正常に生成されているか判定
	if(!this->checkSoundBuffer()) {										//生成されていない場合
		LeaveCriticalSection(&this->cs);								//クリティカルセクションの終了
		return eSOUND_NOTBUFFER;
	}
	HRESULT hr;															//再生の戻り値を格納

	//ループ再生か否か判定
	if(dwFlag & DSBPLAY_LOOPING) {										//ループ再生
		this->playFlag = ePLAY_LOOPING;
		hr = this->DSBuffer->Play(NULL, dwPriority, dwFlag);			//サウンド再生
	} else {															//1回のみ再生
		this->playFlag = ePLAY_ONCE;
		hr = this->DSBuffer->Play(NULL, dwPriority,
									dwFlag | DSBPLAY_LOOPING);			//サウンド再生
	}



	//デバイスロストの判定
	if(hr == DSERR_BUFFERLOST) {										//デバイスが存在しない
		this->DSBuffer->Restore();										//サウンドバッファの復元を試みる
		this->DSBuffer->Play(NULL, dwPriority,							//サウンド再生
								dwFlag | DSBPLAY_LOOPING);
	}
	LeaveCriticalSection(&this->cs);									//クリティカルセクションの終了
	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	stop
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	サウンド関連処理。
//備考			:	サウンドを停止します。
//クラス		:	CSTREAMINGSOUND_OGGVORBIS
//引数			:	なし
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::stop()
{
	EnterCriticalSection(&this->cs);									//クリティカルセクションの開始

	//バッファが正常に生成されているか判定
	if(!this->checkSoundBuffer()) {										//生成されていない場合
		LeaveCriticalSection(&this->cs);								//クリティカルセクションの終了
		return eSOUND_NOTBUFFER;
	}

	this->DSBuffer->Stop();												//再生停止
	LeaveCriticalSection(&this->cs);									//クリティカルセクションの終了

	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	setEffect
//作成者		:	志場雅広
//作成日		:	08/06/26
//機能			:	サウンド関連処理。
//備考			:	エフェクトを設定します。
//					エフェクトを使用するには一度再生を止めてから設定を行う必要があります。
//					この中では一度再生を停止した後、エフェクトの設定を行い指定された方法に従って再び再生を行います。
//エフェクトを指定するフラグ一覧
//					GUID_DSFX_STANDARD_CHORUS		コーラス
//					GUID_DSFX_STANDARD_COMPRESSOR	コンプレッサ 
//					GUID_DSFX_STANDARD_DISTORTION	ディストーション
//					GUID_DSFX_STANDARD_ECHO			エコー
//					GUID_DSFX_STANDARD_FLANGER		フランジ
//					GUID_DSFX_STANDARD_GARGLE		ガーグル
//					GUID_DSFX_STANDARD_I3DL2REVERB	Interactive 3D Level 2 リバーブ
//					GUID_DSFX_STANDARD_PARAMEQ		パラメトリック イコライザ
//					GUID_DSFX_WAVES_REVERB			Waves リバーブ
//クラス		:	CSTREAMINGSOUND_OGGVORBIS
//引数			:	GUID guidEffect		エフェクトの種類を指定するフラグ
//					DWORD	dwPriority	優先順位(0が一番低い)
//					DWORD	dwFlag		再生方法を指定するフラグ
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/06/26	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::setEffect(GUID guidEffect, DWORD dwPriority, DWORD dwFlag)
{
	HRESULT hr;															//戻り値チェック

	EnterCriticalSection(&this->cs);									//クリティカルセクションの開始

	//バッファが正常に生成されているか判定
	if(!this->checkSoundBuffer()) {										//生成されていない場合
		LeaveCriticalSection(&this->cs);								//クリティカルセクションの終了
		return eSOUND_NOTBUFFER;
	}

	this->stop();														//エフェクトを設定するため再生停止

	//DirectSoundBufferにエフェクトを設定
	DSEFFECTDESC dsEffect[1];
	dsEffect[0].dwSize = sizeof(DSEFFECTDESC);							//サイズ
	dsEffect[0].dwFlags = 0;											//エフェクトをハードウェア生成するかソフトウェア生成するか
	dsEffect[0].guidDSFXClass = guidEffect;								//エフェクトのGUID
	dsEffect[0].dwReserved1 = 0;										//予約済み
	dsEffect[0].dwReserved2 = 0;										//予約済み

	//エフェクトを設定
	DWORD dwResults[1];
	hr = this->DSBuffer->SetFX(1, dsEffect, dwResults);					//エフェクトを設定
	this->play(dwPriority, dwFlag);										//再生を再開
	//エフェクトの設定に成功したか判定
	if(FAILED(hr)) {													//失敗した場合
		LeaveCriticalSection(&this->cs);								//クリティカルセクションの終了
		return eSOUND_EFFECT;
	}

	LeaveCriticalSection(&this->cs);									//クリティカルセクションの終了
	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	deleteEffect
//作成者		:	志場雅広
//作成日		:	08/06/26
//機能			:	サウンド関連処理。
//備考			:	すべてのエフェクトを削除します。
//					エフェクトを削除するには一度再生を止めてから設定を行う必要があります。
//					この中では一度再生を停止した後、エフェクトの削除を行い指定された方法に従って再び再生を行います。
//クラス		:	CSTREAMINGSOUND_OGGVORBIS
//引数			:	DWORD	dwPriority	優先順位(0が一番低い)
//					DWORD	dwFlag		再生方法を指定するフラグ
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/06/26	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::deleteEffect(DWORD dwPriority, DWORD dwFlag)
{
	HRESULT hr;

	EnterCriticalSection(&this->cs);									//クリティカルセクションの開始

	//バッファが正常に生成されているか判定
	if(!this->checkSoundBuffer()) {										//生成されていない場合
		LeaveCriticalSection(&this->cs);								//クリティカルセクションの終了
		return eSOUND_NOTBUFFER;
	}

	this->stop();														//エフェクトを設定するため再生停止

	//バッファのすべてのエフェクトを削除する
	hr = this->DSBuffer->SetFX(0, NULL, NULL);
	//エフェクトの削除に成功したか判定
	if(FAILED(hr)) {													//失敗した場合
		LeaveCriticalSection(&this->cs);								//クリティカルセクションの終了
		return eSOUND_EFFECT;
	}

	this->play(dwPriority, dwFlag);										//再生を再開

	LeaveCriticalSection(&this->cs);									//クリティカルセクションの終了
	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	getPlayCur
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	サウンド関連処理。
//備考			:	再生位置を取得します(多少の誤差あり)。
//					再生位置情報にバッファ上の詳細な再生位置情報で補正をして再生位置情報を計算しています。
//クラス		:	CSTREAMINGSOUND_OGGVORBIS
//引数			:	DWORD	再生位置
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
DWORD CSTREAMINGSOUND_OGGVORBIS::getPlayCur()
{
	//メモリ確保
	DWORD pc;															//カーソル位置
	DWORD dwPlayCur;													//再生位置

	EnterCriticalSection(&this->cs);									//クリティカルセクションの開始

	//再生位置取得
	this->DSBuffer->GetCurrentPosition(&pc, NULL);

	dwPlayCur = this->playCur + pc % this->blockSize;
	
	LeaveCriticalSection(&this->cs);									//クリティカルセクションの終了

	return dwPlayCur;													//再生位置を返す
}

//-------------------------------------------------------------------------------------------------------------------------------
//関数名		:	Notify
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	ストリーミングを行うメソッドを呼び出す。
//備考			:	クラスメソッドのストリーミング処理を呼び出す。
//引数			:	LPVOID		lpParam		クラスのポインタ
//戻り値		:	unsigned int	WINAPI
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
unsigned int WINAPI Notify(LPVOID lpParam)
{
	//ストリーミングを行うためのメソッドを呼び出す
	CSTREAMINGSOUND_OGGVORBIS *pCSSound;								//ストリーミング対象のクラス
	pCSSound = (CSTREAMINGSOUND_OGGVORBIS *)lpParam;					//ストリーミング対象のクラスを起動できるようにする

	pCSSound->NotifyProc();												//スレッドを起動

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	NotifyProc
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	ストリーミング処理。
//備考			:	ストリーミングを行う。
//クラス		:	CSTREAMINGSOUND_OGGVORBIS
//引数			:	なし
//戻り値		:	unsigned int	WINAPI
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
unsigned int WINAPI CSTREAMINGSOUND_OGGVORBIS::NotifyProc(void)
{
	DWORD dwResult;														//イベントの状態を受け取る
	MSG msg;															//メッセージを受け取る

//	DWORD dwExitCode;													//スレッドの終了コード

	//データを書き込むループ
	do {
		//再生終了を検知(多少誤差が出る)
		if(this->getPlayCur() >= this->dataSize							//一回のみの再生で再生終了位置に達した場合
				&& this->playFlag == ePLAY_ONCE) {
			this->stop();												//再生を停止
			this->playCur = 0;
		}
		//スレッドにメッセージが来ているか調べる
		if(PeekMessage(&msg, NULL,										//メッセージが来ている
			WM_END_THREAD, WM_END_THREAD, PM_NOREMOVE)) {
			//メッセージを調べる 
			switch(msg.message)
			{
				//スレッドを終了
				case WM_END_THREAD:
					_endthreadex(0);									//スレッドを終了する
					/*
					WaitForSingleObject(this->hNotifyThread, kTIMEOUT);				//スレッドが終了するまで一定時間待機
					GetExitCodeThread(this->hNotifyThread, &dwExitCode);			//スレッドの状態を取得
					//スレッドがまだ動いているか判定
					if(dwExitCode == STILL_ACTIVE) {								//スレッドが稼働中
						//スレッドが自主的に終了されていない場合は強制終了
						if(!TerminateThread(this->hNotifyThread, 0)) {				//強制終了に失敗
							WaitForSingleObject(this->hNotifyThread, kTIMEOUT);		//スレッドが終了するまで待機(もう少し待ってみる)
							TerminateThread(this->hNotifyThread, 0);				//スレッドを強制終了(これで終了しなければ諦める)
						}
					}
					
					//イベントハンドルのクローズ
					for(int i = 0; i < kEVENTMAX; i++) {								//イベント数分繰り返す
						kCLOSEHANDLE(this->hNotifyEvent[i]);							//イベントハンドルを閉じる
					}

					//スレッドハンドルのクローズ
					if(this->hNotifyThread != NULL) {									//スレッドハンドルが解放されていない
						kCLOSEHANDLE(this->hNotifyThread);								//スレッドハンドルを閉じる
					}

					kRELEASE(this->DSBuffer);											//サウンドバッファの解放
					*/
					return 0;
			}
		}

		dwResult = MsgWaitForMultipleObjectsEx(kEVENTMAX,				//イベントを取得
			this->hNotifyEvent,  INFINITE, QS_ALLEVENTS, 0);

		//イベントに応じて処理を振り分ける
		switch(dwResult)
		{
			//バッファの更新
			case WAIT_OBJECT_0:
				this->readStream();										//データを読み込む
				break;
			//再生停止
			case WAIT_OBJECT_0 + 1:
				break;
			//想定外の処理
			default:
				break;
		}
	} while(TRUE);

	_endthreadex(0);													//スレッドを終了する

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	readStream
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	ストリーミング処理。
//備考			:	ストリーミング内での音声データ読み込み処理を行っています。
//クラス		:	CSTREAMINGSOUND_OGGVORBIS
//引数			:	なし
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/05/08	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::readStream()
{
	//メモリ確保
	VOID *pBuffer;														//ロックしたバッファの位置
	stSoundBuffer sBuffer;												//音声データを読み込むためのバッファ
	ZeroMemory(&sBuffer, sizeof(stSoundBuffer));						//バッファの初期化

	//クリティカルセクションを用いた同期処理
	EnterCriticalSection(&this->cs);									//クリティカルセクションの開始

	this->OggVorbis.seekData(this->readCur);							//読み込み位置を移動

	//音声データの最後を検出
	if((this->readCur * (this->wfex.nChannels							//もう音声データの終端
				* this->wfex.wBitsPerSample / 8))
				+ this->blockSize >= this->dataSize) {
		//最初に残りの音声データを読み込む
		DWORD dwDataSize;												//残りのデータ量
		dwDataSize = this->dataSize - (this->readCur
			* (this->wfex.nChannels * this->wfex.wBitsPerSample / 8));

		//バッファのロック
		if(FAILED(this->DSBuffer->Lock(this->writeCur,					//バッファのロックに失敗
						dwDataSize, &sBuffer.pBuffer, 
						&sBuffer.dwBufferSize, &pBuffer, NULL, 0))) {
			LeaveCriticalSection(&this->cs);							//クリティカルセクションの終了
			return eSOUND_LOCK;
		}

		//データの読み込み処理
		if(this->OggVorbis.readData(&sBuffer,							//データの読み込み
									sBuffer.dwBufferSize) != eOGG_OK) {
			LeaveCriticalSection(&this->cs);							//クリティカルセクションの終了
			return eSOUND_READ;
		}
	
		//バッファのアンロック
		if(FAILED(this->DSBuffer->Unlock(sBuffer.pBuffer,				//アンロックに失敗
							sBuffer.dwBufferSize, NULL, 0))) {
			LeaveCriticalSection(&this->cs);							//クリティカルセクションの終了
			return eSOUND_UNLOCK;										//エラー情報を返す
		}

		//読み込みカーソルを修正
		this->readCur = 0;												//カーソルの位置を音声の先頭に戻す
		this->OggVorbis.seekData(this->readCur);						//読み込み位置を移動

		//バッファの残りにデータを書き込む
		//ループ再生なら音声データの先頭から書き込み一回のみの再生なら無音でバッファを埋める
		//バッファのロック
		if(FAILED(this->DSBuffer->Lock(this->writeCur + dwDataSize,		//バッファのロックに失敗
					this->blockSize - dwDataSize, &sBuffer.pBuffer, 
						&sBuffer.dwBufferSize, &pBuffer, NULL, 0))) {
			LeaveCriticalSection(&this->cs);							//クリティカルセクションの終了
			return eSOUND_LOCK;
		}

		//ループ再生か否か判定
		if(this->playFlag == ePLAY_LOOPING) {
			//データの読み込み処理
			if(this->OggVorbis.readData(&sBuffer,						//データの読み込み
					sBuffer.dwBufferSize) != eOGG_OK) {
				LeaveCriticalSection(&this->cs);						//クリティカルセクションの終了
				return eSOUND_READ;
			}
			this->readCur = this->OggVorbis.getReadPos();				//読み込みカーソルを移動
			this->playCur += this->blockSize;							//再生位置の更新
		} else {
			ZeroMemory(sBuffer.pBuffer, sBuffer.dwBufferSize);
			this->playCur += dwDataSize;
		}
	
		//バッファのアンロック
		if(FAILED(this->DSBuffer->Unlock(sBuffer.pBuffer,				//アンロックに失敗
							sBuffer.dwBufferSize, NULL, 0))) {
			LeaveCriticalSection(&this->cs);							//クリティカルセクションの終了
			return eSOUND_UNLOCK;										//エラー情報を返す
		}

		this->playCnt++;												//再生回数アップ
	} else {															//通常の読み込み処理
		//データ読み込み
		//バッファのロック
		if(FAILED(this->DSBuffer->Lock(this->writeCur,					//バッファのロックに失敗
							this->blockSize, &sBuffer.pBuffer, 
							&sBuffer.dwBufferSize, &pBuffer, NULL, 0))) {
			LeaveCriticalSection(&this->cs);							//クリティカルセクションの終了
		   return eSOUND_LOCK;
		}

		//データの読み込み処理
		if(this->OggVorbis.readData(&sBuffer,							//データの読み込み
									sBuffer.dwBufferSize) != eOGG_OK) {
			LeaveCriticalSection(&this->cs);							//クリティカルセクションの終了
			return eSOUND_READ;
		}
	
		//バッファのアンロック
		if(FAILED(this->DSBuffer->Unlock(sBuffer.pBuffer,				//アンロックに失敗
							sBuffer.dwBufferSize, NULL, 0))) {
			LeaveCriticalSection(&this->cs);							//クリティカルセクションの終了
			return eSOUND_UNLOCK;										//エラー情報を返す
		}

		this->readCur = this->OggVorbis.getReadPos();					//読み込みカーソルを移動
		this->playCur += this->blockSize;								//再生位置の更新
	}

	this->writeCur += this->blockSize;									//書き込みカーソルを移動

	//書き込みカーソルを修正
	if(this->writeCur >= this->bufferSize) {							//カーソルの位置がバッファサイズより大きい
		this->writeCur = 0;												//カーソルの位置をバッファの先頭に戻す
	}

	LeaveCriticalSection(&this->cs);									//クリティカルセクションの終了
	return eSOUND_OK;
}