//--------------------------------------------------------------------------------------------
//	ファイル名	：	version.h
//	アプリ名	：	汎用的なファイルなので、実行ファイルは不特定
//	作成者		：	植山沙欧
//	作成日		：	2007.02.09
//	機能		：	VisualStudio2005と2003の共存をするためのヘッダーです。
//	使用方法	：	VisualStudio2005以前の開発環境を使用するときはコメントアウトします。
//	更新履歴	：	2007.02.09	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------

#pragma once

//	VisualStudio2005以外でコンパイルする場合、
//	VS2005の定義はコメントアウトしておく
#define VS2005

//	もし、「VS2005」が定義されていたら以下の設定を行う
#ifdef VS2005
//	以下の警告を無効にします

//「ファイルは、現在のコード ページ (932) で表示できない文字を含んでいます。
//　データの損失を防ぐために、ファイルを Unicode 形式で保存してください。」
#pragma warning (disable: 4819)
//「引数は関数の本体部で 1 度も参照されません」
#pragma warning (disable: 4100)
//「This function or variable may be unsafe.」
#pragma warning (disable: 4996)

#pragma warning	(disable: 4786)

#pragma warning	(disable: 4267)

#endif

//	参照カウンタを減らすマクロ
#ifndef kREL
#define kREL(x) if(x){x->Release();x=NULL;}	//	xのオブジェクトがNULLでは無い場合は、xを解放する。
#endif

//#define	PRESENTATION

//	コピー
void myCopyString(char* dest_ptr, int iDestSize, const char* source_ptr);	//	文字列コピー
//	メモリ解放関連
extern void verDelete_Scalar(void** p);	//	単体の解放
extern void verDelete_Vector(void** p);	//	配列の解放
//　ハンドルを解放します
extern void verCloseHandle(void** h);	//	ハンドルを閉じる