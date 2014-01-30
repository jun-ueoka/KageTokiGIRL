//-------------------------------------------------------------------------------------------------------------------------------
//ファイル名	:	CSound.h
//作成者		:	志場雅広
//作成日		:	08/04/28
//機能			:	CSound.cppで必要な宣言、定義を行っています。
//備考			:	CSoundクラスから派生して各サウンドバッファが作成されます。
//					又、CSoundクラスで設定されたデータを派生クラスは共有しています。
//更新履歴		:	08/04/28	Ver0.01.00	基本機能を実装。
//					08/05/08	Ver0.11.00	Ogg Vorbisに対応。
//					08/05/12	Ver0.21.00	OggVorbisを扱う処理をクラスとして独立。
//-------------------------------------------------------------------------------------------------------------------------------
//プラグマ指令
#pragma once
#pragma warning(disable:4996)

//ヘッダーファイルのインクルード
#include "Sound.h"

//列挙型定義
//再生フラグ(ストリーミングに使用)
typedef enum ePLAY_FLAG {
	ePLAY_LOOPING,																	//ループ再生
	ePLAY_ONCE,																		//一回のみ再生
} PLAY_FLAG;

//サウンド関連エラー
typedef enum eSOUND_ERROR {
	eSOUND_ERROR,																	//エラー
	eSOUND_OK,																		//正常
	eSOUND_FILENAME,																//ファイル名が設定されていない
	eSOUND_OPEN,																	//ファイルのオープンに失敗
	eSOUND_CLOSE,																	//ファイルのクローズに失敗
	eSOUND_READ,																	//ファイルの読み込みに失敗
	eSOUND_EXISTING,																//既にインターフェースが存在(エラーでは無い)
	eSOUND_CREATEINTERFACE,															//インターフェースの作成に失敗
	eSOUND_COOPERATIVELEVEL,														//強調レベルの設定に失敗
	eSOUND_READFORMAT,																//ファイルフォーマット読み込みに失敗
	eSOUND_DATASIZE,																//データサイズ読み込みに失敗
	eSOUND_NOTINTERFACE,															//インターフェースが存在しない
	eSOUND_CREATEBUFFER,															//バッファの生成に失敗
	eSOUND_LOCK,																	//バッファのロックに失敗
	eSOUND_UNLOCK,																	//バッファのアンロックに失敗
	eSOUND_SETFORMAT,																//フォーマットの設定に失敗
	eSOUND_NOTBUFFER,																//バッファが存在しない
	eSOUND_QUERYINTERFACE,															//DirectSoundNotifyインターフェース取得に失敗
	eSOUND_SETNOTIFICATIONPOSITIONS,												//停止位置の設定に失敗
	eSOUND_CREATETHREAD,															//スレッドの作成に失敗
	eSOUND_EFFECT,																	//エフェクトの設定に失敗
} SOUND_ERROR;

//-------------------------------------------------------------------------------------------------------------------------------
//クラス名		:	CSOUND
//作成者		:	志場雅広
//作成日		:	08/04/28
//機能			:	サウンド関連処理。
//備考			:	サウンドを再生するのに必要なインターフェースです。
//					CSoundを基本クラスとして、CSoundStatic、CSoundStream等の派生クラスが生成されています。
//					このクラスで設定したデータを派生するクラス全てで共有しています。
//					このクラスを使用するためにはウィンドウハンドルを取得する必要があります。
//					又、サウンド関連の処理を行うためにはこのクラスが存在する必要があります。
//更新履歴		:	08/04/28	Ver0.01.00	DirectSoundインターフェースの初期化とプライマリバッファの作成を実装。
//					08/05/01	Ver1.01.00	機能を実装
//-------------------------------------------------------------------------------------------------------------------------------
class CSOUND {
private:
	/***音声の再生に必要なデータ(全ての派生クラスで同一の物を使用)***/
	static HWND hWnd;																//関連付けるウィンドウのハンドル
	/***最初に生成されたクラス以外にはクラス変数を解放できないようにする鍵***/
	static CSOUND *key;																//鍵(自分のポインタを鍵にする)

	//--------------------------------------------------
	//	鍵(コンストラクタとデストラクタで使用)
	//	派生クラスから起動されないようにする
	//--------------------------------------------------
	//機能:鍵を掛ける	引数:自分自身のポインタ		戻り値:なし
	void lock(CSOUND *key) { this->key = key; }										//鍵を掛ける
	//機能:鍵が掛っているか調べる	引数:なし		戻り値:TRUE 掛っている	FALSE 掛っていない
	BOOL checkKey(void) { if(this->key) { return TRUE; } return FALSE; }			//鍵が掛っているか調べる
	//機能:鍵を開ける	引数:自分自身のポインタ		戻り値:なし
	void unlock(CSOUND *key)														//鍵を開ける
	{
		//渡された鍵で錠を開けるか調べる(渡された鍵が掛けられている鍵と一致したら開ける)
		if(this->key == key) {														//鍵が一致
			this->key = NULL;														//鍵を開ける
		}
	}

	/***サウンドの再生に必要な準備***/
	SOUND_ERROR initDirectSound(void);												//DirectSoundを使用可能にする
	SOUND_ERROR initPrimaryBuffer(void);											//PrimaryBufferを使用可能にする

	SOUND_ERROR initialize(void);													//データの初期化

	void release(void);																//データの解放

protected:
	/***音声の再生に必要なデータ(全ての派生クラスで同一の物を使用)***/
	static LPDIRECTSOUND8 SoundInterface;											//DirectSoundのインターフェース
	static LPDIRECTSOUNDBUFFER  PrimaryBuffer;										//DirectSoundのプライマリバッファ

	/***ハンドル操作***/
	//機能:ハンドルの設定	引数:ウィンドウハンドル		戻り値:なし
	void setHandle(HWND hWnd) { this->hWnd = hWnd; }								//ハンドルの設定
	//機能:ハンドルの取得	引数:なし	戻り値:ウィンドウハンドル					//ハンドルの取得
	HWND getHandle(void) { return this->hWnd; }

	/***サウンドインターフェース操作***/
	//機能:サウンドインターフェースの取得	引数:なし	戻り値:サウンドインターフェース
	LPDIRECTSOUND8 getSoundInterface(void) { return this->SoundInterface; }			//サウンドインターフェースの取得
	BOOL checkSoundInterface(void);													//サウンドインターフェースのチェック

public:
	/***コンストラクタとデストラクタ***/
	CSOUND(void) { }																//デフォルトコンストラクタ
	CSOUND(HWND hWnd);																//コンストラクタ
	virtual ~CSOUND(void);															//デストラクタ

	/***サウンド操作***/
	/*
	virtual SOUND_ERROR play(void) { return eSOUND_OK; }							//再生(通常再生初期設定ではループ再生)
	virtual SOUND_ERROR stop(void) { return eSOUND_OK; }							//停止
	*/
};