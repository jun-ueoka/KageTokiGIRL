//-------------------------------------------------------------------------------------------------------------------------------
//ファイル名	:	COGG_VORBIS.h
//作成者		:	志場雅広
//作成日		:	08/05/12
//機能			:	COGG_VORBIS.cppで必要な宣言、定義を行っています。
//備考			:	OggVorbisを扱うための操作や処理の提供。
//					CSound関連クラスで扱うことを念頭において作成しています。
//更新履歴		:	08/05/12	Ver1.01.00	基本機能を実装。
//					08/05/26	Ver1.11.00	oggvorbis-win32sdkを最新バージョンに変更。
//											oggvorbis-win32sdk-1.0.1からlibogg-1.1.3
//																		libvorbis-1.2.0へ変更。
//-------------------------------------------------------------------------------------------------------------------------------
//プラグマ指令
#pragma once
#pragma warning(disable:4996)

//ライブラリのリンク

#pragma comment(lib,"../source/oggvorbis-win32sdk/lib/ogg_static.lib")
#pragma comment(lib,"../source/oggvorbis-win32sdk/lib/vorbis_static.lib")
#pragma comment(lib,"../source/oggvorbis-win32sdk/lib/vorbisfile_static.lib")

#pragma comment(linker, "/nodefaultlib:LIBCMT")

//ヘッダーファイルのインクルード
#include "Sound.h"

#include "oggvorbis-win32sdk/include/vorbis/codec.h"
#include "oggvorbis-win32sdk/include/vorbis/vorbisfile.h"

//列挙型定義
//サウンド関連エラー
typedef enum eOGGVORBIS_ERROR {
	eOGG_ERROR,																		//エラー
	eOGG_OK,																		//正常
	eOGG_FILENAME,																	//ファイル名が指定されていない
	eOGG_OPENFILE,																	//ファイルが開けない
	eOGG_OGGFILE,																	//OggVorbisの初期化に失敗
	eOGG_INFO,																		//OggVorbisの情報取得に失敗
	eOGG_READ,																		//OggVorbisの読み込みに失敗
} OGGVORBIS_ERROR;

//-------------------------------------------------------------------------------------------------------------------------------
//クラス名		:	COGG_VORBIS
//作成者		:	志場雅広
//作成日		:	08/05/12
//機能			:	OggVorbis関連処理。
//備考			:	OggVorbisをCSound関連クラスで扱うための機能を提供しています。
//更新履歴		:	08/05/12	Ver1.01.00	機能を実装
//-------------------------------------------------------------------------------------------------------------------------------
class COGG_VORBIS {
private:
	/***OggVorbisを使用するのに必要なデータ***/
	FILE *fp;																		//OggVorbisのファイルポインタ
	LPSTR szFileName;																//読み込むOggVorbisファイル名
	OggVorbis_File ovf;																//OggVorbisのファイル情報
	vorbis_info *vi;																//OggVorbisの情報
	WAVEFORMATEX wfex;																//サウンドデータのフォーマット
	DWORD DataSize;																	//音声データのサイズ
public:
	/***コンストラクタとデストラクタ***/
	COGG_VORBIS(void) { this->initialize(); }										//デフォルトコンストラクタ
	COGG_VORBIS(LPSTR szFileName);													//コンストラクタ
	~COGG_VORBIS(void);																//デストラクタ

	OGGVORBIS_ERROR initialize(void);												//初期化

	/***OggVorbis関連処理***/
	OGGVORBIS_ERROR openOggVorbis(LPSTR szFileName);								//OggVorbisファイルを開いて使用可能にする
	OGGVORBIS_ERROR closeOggVorbis(void);											//OggVorbisファイルを閉じる

	/***データ操作***/
	WAVEFORMATEX getWaveFormat(void) { return this->wfex; }							//WAVEフォーマット情報を取得
	DWORD getDataSize(void) { return this->DataSize; }								//音声データのサイズの取得

	/***サウンドデータ読み込み関連操作***/
	//機能:現在の読み込み位置の取得		引数:なし	戻り値:読み込み位置
	DWORD getReadPos(void) { return (DWORD)ov_pcm_tell(&this->ovf); } //読み込み位置の取得
	//機能:読み込み位置のシーク		引数:シーク先の位置(位置 / (チャンネル * 量子化バイト数)		戻り値:エラー情報
	//<使用例>this->seekData(readCursor / (this->wfex.nChannels * kBITSPERSAMPLE / 8));
	OGGVORBIS_ERROR seekData(DWORD pos) {											//PCMにデコード後の値でシークする
		ov_pcm_seek(&this->ovf, pos); return eOGG_OK;
	}

	/***サウンドデータ読み込み***/
	OGGVORBIS_ERROR readData(stSoundBuffer *pstSoundBuffer, DWORD dwSize);			//音声データをPCMにデコードして読み込む
};