#pragma warning(disable:4996)
#include <windows.h>

#include	"Precompiler.h"

//#include "MySystem.h"

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃システムで共有する汎用機能群															　┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
namespace GCom{

	namespace Cnt
	{
		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
		//┃カウンタークラス2初期化																　┃
		//┃　2005/01/18	作成																　┃
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
		cNormal::cNormal( )
		{
			this->counter = 0.0f;
			this->startValue = 0.0f;
			this->endValue = 0.0f;
			this->incValue = 0.0f;
			this->mode = Mode::eNon;
		}
		//カウンターを設定する
		Status::TYPENAME cNormal::init(float inStartValue, float inEndValue, float inIncValue, Mode::TYPENAME inMode)
		{
			//設定内容の整合性を調べる
			if(inMode == Mode::eNon){								return Status::eErr;}//無効フラグ
			//カウンターの増加（減少）量が0の場合
			if(inIncValue == 0.0f){									return Status::eErr;}

			// 初期値　＜　終了値　の関係でカウンターが減少する設定
			if(inStartValue < inEndValue && inIncValue < 0.0f){		return Status::eErr;}
			// 初期値　＞　終了値　の関係でカウンターが増加する設定
			if(inStartValue > inEndValue && inIncValue > 0.0f){		return Status::eErr;}


			//
			this->startValue		= inStartValue;
			this->endValue			= inEndValue;
			this->incValue			= inIncValue;
			if(inStartValue != inEndValue){		this->mode			= inMode;		}
			else{								this->mode			= Mode::eZero;	}
			this->counter			= this->startValue;
			return Status::eNormal;
		}

		//繰り返しモード
		void cNormal::increasedCounter_Mode_RepeatsB(float inAddValue)
		{
			this->counter += this->incValue * inAddValue;

		//初期値・終了値の大小関係によりhige lowを決定
			float hige, low;
			if(this->startValue < this->endValue){
				hige = this->endValue;
				low = this->startValue;
			}else{
				low = this->endValue;
				hige = this->startValue;
			}
		//カウンタを最小から最大の範囲に収める
			while(this->counter < low || this->counter > hige){
				//最小値以下の場合
				if(this->counter < low){
					this->counter = hige + (this->counter - low);
				}
				//最大値以上の場合
				else if(this->counter > hige){
					this->counter = low + (this->counter - hige);
				}
			}
		}
		//繰り返しモード
		void cNormal::increasedCounter_Mode_RepeatsA(float inAddValue)
		{
			this->counter += this->incValue * inAddValue;

		//初期値・終了値の大小関係によりhige lowを決定
			float hige, low;
			if(this->startValue < this->endValue){
				hige = this->endValue;
				low = this->startValue;
			}else{
				low = this->endValue;
				hige = this->startValue;
			}
		//カウンタを最小から最大の範囲に収める
			while(this->counter < low || this->counter > hige){
				//最小値以下の場合
				if(this->counter < low){
					this->counter = hige;
				}
				//最大値以上の場合
				else if(this->counter > hige){
					this->counter = low;
				}
			}
		}
		//復帰モード
		void cNormal::increasedCounter_Mode_Tourn(float inAddValue)
		{
			this->counter += this->incValue * inAddValue;

		//初期値・終了値の大小関係によりhige lowを決定
			float hige, low;
			if(this->startValue < this->endValue){
				hige = this->endValue;
				low = this->startValue;
			}else{
				low = this->endValue;
				hige = this->startValue;
			}
		//カウンタを最小から最大の範囲に収める
			while(this->counter < low || this->counter > hige){
			//カウンタが範囲を超えた場合、増加（減少）値を符号反転する
				this->incValue = -this->incValue;
				//最小値以下の場合
				if(this->counter < low){
					this->counter = low + low - this->counter;
				}
				//最大値以上の場合
				else if(this->counter > hige){
					this->counter =	hige +	hige - this->counter;
				}
			}
		}
		//１回のみモード
		void cNormal::increasedCounter_Mode_Stop(float inAddValue)
		{
			this->counter += this->incValue * inAddValue;

		//初期値・終了値の大小関係によりhige lowを決定
			float hige, low;
			if(this->startValue < this->endValue){
				hige = this->endValue;
				low = this->startValue;
			}else{
				low = this->endValue;
				hige = this->startValue;
			}
		//カウンタを最小から最大の範囲に収める
			while(this->counter < low || this->counter > hige){
				//最小値以下の場合
				if(this->counter < low){
					this->counter = low;
				}
				//最大値以上の場合
				else if(this->counter > hige){
					this->counter = hige;
				}
			}
		}

		//カウンターを進める
		void cNormal::increasedCounter(float inAddValue)
		{
			switch(this->mode){
			case Mode::eRepeatsA:	//繰り返す
				this->increasedCounter_Mode_RepeatsA(inAddValue);
				break;
			case Mode::eRepeatsB:	//繰り返す
				this->increasedCounter_Mode_RepeatsB(inAddValue);
				break;
			case Mode::eTourn:	//復帰する
				this->increasedCounter_Mode_Tourn(inAddValue);
				break;
			case Mode::eStop:		//１回目のみ有効
				this->increasedCounter_Mode_Stop(inAddValue);
				break;
			default:
			case Mode::eNon:		//未使用
				return;
				break;
			}
		}
		//現在値を得る
		float cNormal::getCounter( )
		{
			if(this->mode == Mode::eNon){
				return 0.0f;
			}
			return this->counter;
		}
		double cNormal::getCounterD( )
		{
			if(this->mode == Mode::eNon){
				return 0.0;
			}
			return this->counter;
		}
		int cNormal::getCounterI( )
		{
			if(this->mode == Mode::eNon){
				return 0;
			}
			return (int)this->counter;
		}
		//現在値を変更する（カウンターの範囲外の値を指定した場合無効）
		void cNormal::setCounter(float inCounter)
		{
			float min, max;
			if(this->startValue < this->endValue){
				min = this->startValue;
				max = this->endValue;
			}
			else{
				min = this->endValue;
				max = this->startValue;
			}
			if(inCounter < min || inCounter > max){
				return;
			}
			this->counter = inCounter;
		}

		//カウンターの現在の状態を得る
		Status::TYPENAME cNormal::getStatus( )
		{
			//
			if(this->mode == Mode::eNon){
				return Status::eErr;
			}
			//
			if(this->mode == Mode::eStop && this->counter == this->endValue){
				return Status::eEnd;
			}
			return Status::eNormal;
		}
		//カウンターを初期状態に戻す
		void cNormal::resetCounter( )
		{
			if(this->mode == Mode::eNon){
				return;
			}
			this->counter = this->startValue;
		}
	}

	namespace _3D
	{
	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	//┃立方体																					　┃
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
		cBox::cBox()
		{
			this->s = D3DXVECTOR3(0.0f,0.0f,0.0f);
			this->e = D3DXVECTOR3(0.0f,0.0f,0.0f);
		}
		cBox::cBox(const D3DXVECTOR3* s, const D3DXVECTOR3* e)
		{
			this->set(s, e);
		}
		void cBox::set(const D3DXVECTOR3* s, const D3DXVECTOR3* e)
		{
			this->s = *s;
			this->e = *e;
		}
		void cBox::transform(const D3DXVECTOR3* offset)
		{
			this->s += *offset;
			this->e += *offset;
		}
		BOOL cBox::intersect(const cBox* box)
		{
			//無効判定
			if(this->s.x == this->e.x){return FALSE;}
			if(this->s.y == this->e.y){return FALSE;}
			if(this->s.z == this->e.z){return FALSE;}
			if(box->s.x == box->e.x){return FALSE;}
			if(box->s.y == box->e.y){return FALSE;}
			if(box->s.z == box->e.z){return FALSE;}
			//あたり判定
			if(	this->s.x	<	box->e.x	&&
				box->s.x	<	this->e.x	&&
				this->s.y	<	box->e.y	&&
				box->s.y	<	this->e.y	&&
				this->s.z	<	box->e.z	&&
				box->s.z	<	this->e.z	)
			{
				return TRUE;
			}
			return FALSE;
		}
		//２つのボックスの直行する面の接触を判定する（接触時ＴＲＵＥ）
		BOOL cBox::hitLine(const cBox* inRect)
		{
			//ボックスが接触している状態を確認
			if( this->intersect(inRect) )
			{
				//線上の接触の有無を確認
				if(inRect->s.x < this->s.x && this->s.x < inRect->e.x){	
					return TRUE;
				}	//自身のs.x側平面に相手の矩形が飛び出している
				if(inRect->s.x < this->e.x && this->e.x < inRect->e.x){
					return TRUE;
				}	//自身のe.x側平面に相手の矩形が飛び出している

				if(inRect->s.y < this->s.y && this->s.y < inRect->e.y){
					return TRUE;
				}	//自身のs.y側平面に相手の矩形が飛び出している
				if(inRect->s.y < this->e.y && this->e.y < inRect->e.y){
					return TRUE;
				}	//自身のe.y側平面に相手の矩形が飛び出している

				if(inRect->s.z < this->s.z && this->s.z < inRect->e.z){
					return TRUE;
				}	//自身のs.y側平面に相手の矩形が飛び出している
				if(inRect->s.z < this->e.z && this->e.z < inRect->e.z){
					return TRUE;
				}	//自身のe.y側平面に相手の矩形が飛び出している
			}
			return FALSE;
		}


	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	//┃float型矩形																				　┃
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
		//┃float型で矩形を設定する																　┃
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
		void cFloatRect::set(	float inLeft,
								float inTop, 
								float inRight,
								float inBottom)
		{
			this->left	= inLeft;
			this->top	= inTop;
			this->right	= inRight;
			this->bottom = inBottom;
		}
		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
		//┃矩形を平行移動させる																　┃
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
		void cFloatRect::transform(	float inX,
									float inY)
		{
			this->left	+= inX;
			this->top	+= inY;
			this->right	+= inX;
			this->bottom += inY;
		}
		//=======================================================================
		//２つの矩形の接触を判定する（接触時ＴＲＵＥ）
		BOOL cFloatRect::hit(cFloatRect* inRect)
		{
			if(	this->left	< inRect->right	&&
				this->right	> inRect->left	&&
				this->top	< inRect->bottom &&
				this->bottom > inRect->top	){
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		//=======================================================================
		//２つの矩形の線上の接触を判定する（接触時ＴＲＵＥ）
		BOOL cFloatRect::hitLine(cFloatRect* inRect)
		{
			//矩形が接触している状態を確認
			if( this->hit(inRect) )
			{
				//線上の接触の有無を確認
				if(inRect->left < this->left	&& this->left < inRect->right){	
					return TRUE;
				}	//自身のLeftTop-LeftBottomライン上に相手のLeft-Rightラインがが重なる
				if(inRect->left < this->right	&& this->right < inRect->right){
					return TRUE;
				}	//自身のRightTop-RightBottomライン上に相手のLeft-Rightラインがが重なる

				if(inRect->top < this->top		&& this->top < inRect->bottom){
					return TRUE;
				}	//自身のLeftTop-RightTopライン上に相手のTop-bottomラインがが重なる
				if(inRect->top < this->bottom	&& this->bottom < inRect->bottom){
					return TRUE;
				}	//自身のLeftBottom-RightBottomライン上に相手のTop-bottomラインがが重なる

			}
			return FALSE;
		}
	}
	namespace Color
	{
	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	//┃色設定補助																				　┃
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
		//┃DWORD型で色を設定する。																　┃
		//┃　2005/11/21	作成																　┃
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
		void cARGB::set(DWORD inColor)
		{
			this->color = *((sRGBA* )&inColor);
		}
		void cARGB::set(BYTE a, BYTE r, BYTE g, BYTE b)
		{
			this->color.a = a;
			this->color.r = r;
			this->color.g = g;
			this->color.b = b;
		}
		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
		//┃DWORD型で色を設定する。(コンストラクタ)												　┃
		//┃　2005/11/21	作成																　┃
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
		cARGB::cARGB(DWORD inColor)
		{
			this->color = *((sRGBA* )&inColor);
		}
		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
		//┃コンストラクタ、４色それぞれを個別に色を設定できる、デフォルトは無し				　┃
		//┃　2005/11/21	作成																　┃
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
		cARGB::cARGB(BYTE a, BYTE r, BYTE g, BYTE b)
		{
			this->color.a = a;
			this->color.r = r;
			this->color.g = g;
			this->color.b = b;
		}
		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
		//┃DWORD型で色を返却する。																　┃
		//┃　戻り値：DWORD型色情報																　┃
		//┃　2005/07/28	作成																　┃
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
		DWORD cARGB::get( )
		{
			return *((DWORD* )&this->color);
		}
	}
	namespace Str
	{
		//文字列をＮＥＷしてコピーを作成
		char* copyNew(char* inString)
		{
			char* temp;
			size_t len = strlen(inString);
			temp = new char[len + 1];
			strcpy(temp, inString);
			return temp;
		}
		LPSTR copyNew(LPCSTR inString)
		{
			char* temp;
			size_t len = strlen(inString);
			temp = new char[len + 1];
			strcpy(temp, inString);
			return temp;
		}
		//転送先にメモリが割り当てられている場合、解放後strCopyNewを行う
		char* copyReleaseAndNew(char* *inSet, char* inString)
		{
			if(*inSet)
			{
				delete[] *inSet;
			}
			*inSet = copyNew(inString);
			return *inSet;
		}
		LPSTR copyReleaseAndNew(LPSTR* inSet, LPCSTR inString)
		{
			if(*inSet)
			{
				delete[] *inSet;
			}
			*inSet = copyNew(inString);
			return* inSet;
		}
	}

	namespace Memory
	{
	}
}//namespace GCom