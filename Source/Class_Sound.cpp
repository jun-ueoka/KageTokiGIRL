//-------------------------------------------------------------------------------------------------------------------------------
//ファイル名	:	Class_Sound.cpp
//作成者		:	志場雅広
//作成日		:	08/04/28
//機能			:	音声データの再生。
//備考			:	サウンドを扱うための操作や処理の提供。
//					CSoundクラスのデータを派生クラスは共有し使用しています。
//更新履歴		:	08/04/28	Ver0.01.00	DirectSoundの初期化処理を実装。
//					08/05/08	Ver0.11.00	Ogg Vorbisに対応。
//-------------------------------------------------------------------------------------------------------------------------------
//ヘッダーファイルのインクルード

#include	"Precompiler.h"

//#include	"Class_Sound.h"

//クラス変数
//サウンド関連クラスで共有
HWND CSOUND::hWnd = NULL;												//ウィンドウハンドル
LPDIRECTSOUND8 CSOUND::SoundInterface = NULL;							//DirectSoundのインターフェース(絶対に必要)
LPDIRECTSOUNDBUFFER  CSOUND::PrimaryBuffer = NULL;						//DirectSoundのプライマリバッファ
CSOUND* CSOUND::key = NULL;												//デストラクタの鍵(自分のポインタを鍵にする)

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	CSOUND
//作成者		:	志場雅広
//作成日		:	08/04/28
//機能			:	CSOUNDクラスのコンストラクタです。
//備考			:	一番最初に起動され、以降全ての派生クラスはこのクラスで設定されたデータを基にサウンドの再生を行います。
//					このクラスは、共有データが勝手に解放されないように鍵を掛けてます。
//					これによって、このクラス以外からはデータを解放できないようにしています。
//クラス		:	CSOUND
//引数			:	HWND	hWnd	ウィンドウハンドル
//戻り値		:	なし
//更新履歴		:	08/04/28	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
CSOUND::CSOUND(HWND hWnd)
{	
	//鍵が掛かっているか判定
	if(!this->checkKey()) {												//掛かっていない
		this->lock(this);												//鍵を掛ける
		this->setHandle(hWnd);											//ウィンドウハンドルの設定
		this->initialize();												//データの初期化
		//DirectSoundの使用準備
		this->initDirectSound();										//DirectSoundのインターフェースを作成
		this->initPrimaryBuffer();										//プライマリバッファの作成
	}
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	~CSOUND
//作成者		:	志場雅広
//作成日		:	08/04/28
//機能			:	CSOUNDクラスのデストラクタです。
//備考			:	このクラスのデータを解放できる権限を持つクラスから呼び出されたときにのみデータの解放を行います。
//クラス		:	CSOUND
//引数			:	なし
//戻り値		:	なし
//更新履歴		:	08/04/28	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
CSOUND::~CSOUND()
{
	//クラス変数を解放する権限があるクラスのみ解放処理を行える
	this->unlock(this);													//鍵を開ける

	//鍵が開いたか調べる
	if(!this->checkKey()) {												//鍵が開いた
		this->release();												//データの解放
	}
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	initialize
//作成者		:	志場雅広
//作成日		:	08/04/28
//機能			:	必要なデータの設定、初期化を行います。
//備考			:	DirectSound8の作成と設定および、プライマリバッファの作成と設定を行っています。
//クラス		:	CSOUND
//引数			:	なし
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/04/28	Ver0.01.00	仮機能を実装。
//					08/05/01	Ver1.01.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSOUND::initialize()
{
	return eSOUND_OK;													//正常終了
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	initDirectSound
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	DirectSoundの準備。
//備考			:	DirectSoundを使用できるようにします。
//クラス		:	CSOUND
//引数			:	なし
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/05/08	Ver1.01.00	機能を実装。
//					08/06/26	Ver1.11.00	COMに対応。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSOUND::initDirectSound()
{
	//メモリを確保
	HRESULT hr;															//戻り値チェック用

	//--------------------------------------------------
	//サウンドインターフェースの生成
	//--------------------------------------------------
	hr = DirectSoundCreate8(NULL, &this->SoundInterface, NULL);			//インターフェースを作成
	//正常にインターフェースが生成されたか判定
	if(FAILED(hr)) {													//生成に失敗
		return eSOUND_CREATEINTERFACE;									//エラー情報を返す
	}

	//協調レベルを設定
	hr = this->SoundInterface											//優先協調レベルを指定
			->SetCooperativeLevel(this->hWnd, DSSCL_PRIORITY);
	//正常に協調レベルが設定されたか判定
	if(FAILED(hr)) {													//協調レベルの設定に失敗
		return eSOUND_COOPERATIVELEVEL;									//エラー情報を返す
	}

	//--------------------------------------------------
	//COMを初期化
	//--------------------------------------------------
	CoInitialize(NULL);

	return eSOUND_OK;													//正常終了
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	initPrimaryBuffer
//作成者		:	志場雅広
//作成日		:	08/05/08
//機能			:	DirectSoundの準備。
//備考			:	プライマリバッファを作成します。
//クラス		:	CSOUND
//引数			:	なし
//戻り値		:	SOUND_ERROR	エラー情報
//更新履歴		:	08/05/08	Ver1.01.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSOUND::initPrimaryBuffer()
{
	//メモリを確保
	HRESULT hr;															//戻り値チェック用

	//--------------------------------------------------
	//プライマリバッファの作成
	//--------------------------------------------------
	//デバイスの設定
	DSBUFFERDESC dsbd;													//デバイス情報
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));							//データの初期化
	dsbd.dwSize = sizeof(DSBUFFERDESC);									//構造体のサイズを設定
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME									//バッファの能力を設定
					| DSBCAPS_CTRLPAN
					| DSBCAPS_GETCURRENTPOSITION2
					| DSBCAPS_PRIMARYBUFFER;
	dsbd.dwBufferBytes = 0;												//バッファのサイズの設定
	dsbd.lpwfxFormat = NULL;											//フォーマット情報の設定

	//バッファの作成
	hr = this->SoundInterface											//プライマリバッファの作成
			->CreateSoundBuffer(&dsbd, &this->PrimaryBuffer, NULL);
	//バッファの作成に成功したか判定
	if(FAILED(hr)) {													//バッファの作成に失敗
		return eSOUND_CREATEBUFFER;										//エラー情報を返す
	}

	//プライマリバッファのフォーマットを設定
	WAVEFORMATEX wfex;
	ZeroMemory(&wfex, sizeof(WAVEFORMATEX));
	wfex.wFormatTag = (WORD)WAVE_FORMAT_PCM;							//フォーマットタグ
	wfex.nChannels = (WORD)kCHANNELS;									//チャンネル数
	wfex.nSamplesPerSec = (DWORD)kSAMPLESPERSEC;						//サンプリングレート
	wfex.wBitsPerSample = (WORD)kBITSPERSAMPLE;							//サンプルあたりのビット数
	wfex.nBlockAlign = (WORD)(wfex.wBitsPerSample						//1サンプルあたりのバイト数
										/ 8 * wfex.nChannels);
	wfex.nAvgBytesPerSec = (DWORD)(wfex.nSamplesPerSec					//サンプルあたりのビット数
										* wfex.nBlockAlign);

	//フォーマットの設定
	hr = this->PrimaryBuffer->SetFormat(&wfex);
	//フォーマットの設定に成功したか判定
	if(FAILED(hr)) {													//フォーマットの設定に失敗
		kRELEASE(this->PrimaryBuffer);									//プライマリバッファの解放
		return eSOUND_SETFORMAT;										//エラー情報を返す
	}

	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	release
//作成者		:	志場雅広
//作成日		:	08/04/28
//機能			:	データの解放を行います。
//備考			:	
//クラス		:	CSOUND
//引数			:	なし
//戻り値		:	なし
//更新履歴		:	08/04/28	Ver1.00.00	機能を実装。
//					08/06/26	Ver1.11.00	COMに対応。
//-------------------------------------------------------------------------------------------------------------------------------
void CSOUND::release()
{
	//解放処理
	kRELEASE(this->PrimaryBuffer);										//プライマリバッファの解放
	kRELEASE(this->SoundInterface);										//サウンドインターフェースの解放

	CoUninitialize();													//COMを終了
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	checkSoundInterface
//作成者		:	志場雅広
//作成日		:	08/04/28
//機能			:	サウンド関連処理。
//備考			:	サウンドのインターフェースが正常に作成されているかを調べます。
//クラス		:	CSOUND
//引数			:	なし
//戻り値		:	BOOL		TRUE		正常
//								FALSE		異常
//更新履歴		:	08/04/28	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
BOOL CSOUND::checkSoundInterface(void)
{
	//インターフェースが正常に作成されているかを判定
	if(!this->SoundInterface) {											//インターフェースが正常に作成されていない場合
		return FALSE;													//異常
	}

	return TRUE;														//正常
}