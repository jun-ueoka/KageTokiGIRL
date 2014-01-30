//-------------------------------------------------------------------------------------------------------------------------------
//ファイル名	:	Class_OggVorbis.cpp
//作成者		:	志場雅広
//作成日		:	08/05/12
//機能			:	OggVorbisに対する操作や処理。
//備考			:	OggVorbisを扱うための操作や処理の提供。
//					CSound関連クラスで扱うことを想定しています。
//更新履歴		:	08/05/12	Ver0.01.00	基本機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
//ヘッダーファイルのインクルード

#include	"Precompiler.h"

//#include	"Class_OggVorbis.h"

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	COGG_VORBIS
//作成者		:	志場雅広
//作成日		:	08/05/12
//機能			:	COGG_VORBISクラスのコンストラクタです。
//備考			:	データの初期化と読み込むOggVorbisファイルの設定を行います。
//クラス		:	COGG_VORBIS
//引数			:	LPSTR szFileName	読み込むファイル名
//戻り値		:	なし
//更新履歴		:	08/05/12	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
COGG_VORBIS::COGG_VORBIS(LPSTR szFileName)
{
	this->initialize();													//データの初期化
	this->szFileName = szFileName;										//読み込むファイル名を設定
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	~COGG_VORBIS
//作成者		:	志場雅広
//作成日		:	08/05/12
//機能			:	COGG_VORBISクラスのデストラクタです。
//備考			:	
//クラス		:	COGG_VORBIS
//引数			:	なし
//戻り値		:	なし
//更新履歴		:	08/05/12	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
COGG_VORBIS::~COGG_VORBIS()
{
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	initialize
//作成者		:	志場雅広
//作成日		:	08/05/12
//機能			:	必要なデータの設定と初期化を行っています。
//備考			:	なし。
//クラス		:	COGG_VORBIS
//引数			:	なし
//戻り値		:	OGGVORBIS_ERROR	エラー情報
//更新履歴		:	08/05/12	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
OGGVORBIS_ERROR COGG_VORBIS::initialize(void)
{
	//データの初期化
	ZeroMemory(this, sizeof(COGG_VORBIS));								//データの初期化

	return eOGG_OK;														//正常終了
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	openOggVorbis
//作成者		:	志場雅広
//作成日		:	08/05/12
//機能			:	OggVorbis関連処理。
//備考			:	OggVorbisファイルを開いてアクセス可能な状態にします。closeOggVorbisと対になっています。
//					OggVorbisへのアクセスが必要なくなったら、closeOggVorbisを呼び出して終了処理を行ってください。
//クラス		:	COGG_VORBIS
//引数			:	なし
//戻り値		:	OGGVORBIS_ERROR	エラー情報
//更新履歴		:	08/05/12	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
OGGVORBIS_ERROR COGG_VORBIS::openOggVorbis(LPSTR szFileName)
{
	//読み込むファイル名を設定
	this->szFileName = szFileName;

	//OggVorbisファイルを開く
	if(!this->szFileName) {												//読み込むファイル名が設定されていない
		return eOGG_FILENAME;											//エラー情報を返す
	}

	this->fp = fopen(this->szFileName, "rb");							//対象のOggVorbisファイルをオープン
	//ファイルオープンエラーチェック
	if(!this->fp) {														//ファイルオープンに失敗
		return eOGG_OPENFILE;											//エラー情報を返す
	}

	//OggVorbisの初期化
	if(ov_open(this->fp, &this->ovf, NULL, 0) < 0) {					//初期化に失敗
		fclose(this->fp);												//ファイルポインタを閉じる
		return eOGG_OGGFILE;											//エラー情報を返す
	}

	//OggVorbisの情報を取得
	this->vi = ov_info(&this->ovf, -1);									//OggVorbisの情報を取得
	//取得に成功したか判定
	if(this->vi == NULL) {												//取得に失敗
		this->closeOggVorbis();											//OggVorbisに関する処理を終了
		return eOGG_INFO;
	}

	//WAVEファイルフォーマットの設定(量子化ビット数は16bitとする)
	//このデータをもとにWAVEにデコードする。
	this->wfex.cbSize = sizeof(WAVEFORMATEX);							//データサイズ
	this->wfex.wFormatTag = WAVE_FORMAT_PCM;							//WAVEフォーマットの種類
	this->wfex.nChannels = this->vi->channels;							//チャンネル数
	this->wfex.nSamplesPerSec = this->vi->rate;							//サンプリング周波数
	this->wfex.wBitsPerSample = kBITSPERSAMPLE;							//量子化ビット数(16bitに設定)
	this->wfex.nBlockAlign = this->wfex.nChannels						//ブロックアライメント
								* this->wfex.wBitsPerSample / 8;
	this->wfex.nAvgBytesPerSec = this->wfex.nSamplesPerSec				//1秒間に転送するバイト数
									* this->wfex.nBlockAlign;

	//WAVEデータサイズの設定
	this->DataSize = (DWORD)(this->wfex.nChannels						//WAVEデータサイズの計算
								* this->wfex.nSamplesPerSec
								* ov_time_total(&this->ovf, -1)
								* this->wfex.wBitsPerSample / 8);

	return eOGG_OK;														//正常終了
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	closeOggVorbis
//作成者		:	志場雅広
//作成日		:	08/05/12
//機能			:	OggVorbis関連処理。
//備考			:	OggVorbisファイルを閉じてアクセスを終了します。openOggVorbisと対になっています。
//クラス		:	COGG_VORBIS
//引数			:	なし
//戻り値		:	OGGVORBIS_ERROR	エラー情報
//更新履歴		:	08/05/12	Ver1.00.00	機能を実装。
//-------------------------------------------------------------------------------------------------------------------------------
OGGVORBIS_ERROR COGG_VORBIS::closeOggVorbis()
{
	//終了処理
	ov_clear(&this->ovf);												//OggVorbisに関する処理を終了

	//OggVorbisファイル情報を初期化
	//明示的に解放しなくてもov_clearによって解放されるが、ポインタの情報が残るため初期化する
	this->vi = NULL;													//OggVorbisファイル情報を初期化

	return eOGG_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//メソッド名	:	readData
//作成者		:	志場雅広
//作成日		:	08/05/12
//機能			:	OggVorbisの音声データをPCMデータにデコードして読み込みます。
//備考			:	指定されたバッファにバッファサイズ分だけ音声データを読み込みます。
//クラス		:	COGG_VORBIS
//引数			:	stSoundBuffer	*pstSoundBuffer		読込先のバッファ情報
//					DWORD			dwSize				読み込むデータ量
//戻り値		:	OGGVORBIS_ERROR	エラー情報
//更新履歴		:	08/05/12	Ver1.00.00	機能を実装。
//					08/06/05	Ver1.01.00	データ読み込み時の安全性を強化。
//-------------------------------------------------------------------------------------------------------------------------------
OGGVORBIS_ERROR COGG_VORBIS::readData(stSoundBuffer *pstSoundBuffer, DWORD dwSize)
{
	//バッファーに音データを書き込む
	BOOL bEndFlag = TRUE;												//読み込みのフラグ(TRUE:継続,FALSE:終了)
	DWORD dwReadCursor = 0;												//読み込みカーソル
	DWORD ret;															//読み込みの戻り値

	//バッファに読み込めるデータ量か調べる
	if(pstSoundBuffer->dwBufferSize < dwSize) {							//バッファより読み込むデータ量が多い
		return eOGG_READ;												//エラー情報を返す
	}

	//セカンダリバッファにOggVorbisのデータを読み込む
	//一度にすべてのデータは読み込めないので少しずつデータを読み込む
	//エラー処理未実装
	do {
		//OggVorbisの音声データをデコードしながらバッファに読み込む
		ret = ov_read(&this->ovf,										//OggVorbis_File構造体
			(char *)pstSoundBuffer->pBuffer + dwReadCursor,				//読込先のバッファ
			pstSoundBuffer->dwBufferSize - dwReadCursor,				//読み込むデータサイズ
			0,															//エンディアンネスの指定
			this->wfex.wBitsPerSample / 8,								//量子化バイト数
			1,															//符号化データか?(符号ありは1)
			NULL);														//?(NULLで大丈夫みたい・・・)

		//ループを継続するかの判定
		if(ret == 0) {													//データの最後に達した
			bEndFlag = FALSE;											//読み込みを終了する
		} else if(ret < 0) {											//何らかのエラーが発生した
			this->closeOggVorbis();										//OggVorbisに関する処理を終了
			return eOGG_READ;											//エラー情報を返す
		} else {														//通常の読み込み処理(まだ、データが読み終わっていない)
			//データの読み込みが終わったか判定
			//これが無いと読み込み終わったはずのデータを読み込むバグが発生することがある
			if(dwReadCursor < pstSoundBuffer->dwBufferSize) {			//読み込みが終了していない
				dwReadCursor += ret;									//バッファの書き込み位置を更新
			} else {													//読み込みが終了した
				bEndFlag = FALSE;										//読み込み処理を終了
			}
		}
	}while(bEndFlag);													//すべてのデータが読み終わるまで繰り返す

	return eOGG_OK;														//正常
}