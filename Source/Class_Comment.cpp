//***********************************************************************************
//!	ファイル名		Class_Comment.cpp
//
//	機能			コメントオブジェクトに関する実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/09/10	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_Comment.h"

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	メソッド名	CCOMMENT::コンストラクタ
	//
	//	引数		inDev		Direct3Dデバイス
	//				inPtn		コメントパターン
	//				inReversal	反転確認
	//				inDraw		描画先
	//				inCntDown	カウントダウン
	//				inVoice		音声名
	//	更新		2008/09/10	<新規>
	//================================================================================
	CCOMMENT::CCOMMENT(CONST DEV inDev, int inPtn, BOOL inReversal,
					   VEC2* inLoc, int inCntDown, CONST LPSTR inVoice)		:
	CTEXTURE(	inDev, 
				getCommentTextureName((COMMENT_PATTERN)inPtn),
				VEC2(0.0f, 0.0f))
	{
	//	フォント準備
		this->font	= CFONT_BASE::getInstance();
	//	コメント用のフォントが生成されて無ければ、生成する
		if(	!this->font->getFontValid(eFONT_COMMENT))
		{
			this->font->createFont(
				inDev,
				eFONT_COMMENT,
				"HG創英角ﾎﾟｯﾌﾟ体",
				COMMENT_TEXT_SIZEY,
				COMMENT_TEXT_SIZEX, 2, 0);
		}

	//	属性の初期化
		for(int i=0; i<MAX_LINE; i++)	{	this->string[i]	= NULL;	}
		this->targetSize	= VEC2(COMMENT_TEX_BLANKX, COMMENT_TEX_BLANKY);
		this->commentPtn	= (COMMENT_PATTERN)inPtn;
		this->movePtn		= eCMPN_APPEAR;
		this->iCntDown		= inCntDown;

	//	ボタンの初期化
		this->buttomUp	=
			new CTEXTURE(inDev, TEX_BUTTOM_UP,   VEC2(TEX_BUTTOM_SIZE,TEX_BUTTOM_SIZE));
		this->buttomDown	=
			new CTEXTURE(inDev, TEX_BUTTOM_DOWN, VEC2(TEX_BUTTOM_SIZE,TEX_BUTTOM_SIZE));
		this->iButtomCnt	= 0;

	//	位置の調整
		this->setFromRect(
			(!inReversal ? 0							 : this->texd_ptr->texInfo.Width),
			0,
			(!inReversal ? this->texd_ptr->texInfo.Width : 0							),
			this->texd_ptr->texInfo.Height);
		this->setLoc(&VEC3(inLoc->x, inLoc->y, 0.0f));

	//	サウンドの初期化(特定条件下で)
		this->sStaticDecision	= new SSTATIC(SOUND_DECISION2);

	//	音声名が未設定なら、生成しない
		if(	!strcmp(inVoice, ""))
			this->sStaticVoice	= NULL;
		else
		{
			this->sStaticVoice	= new SSTATIC(inVoice);
			this->sStaticVoice->setVolume(1500);
		}
	}

	//================================================================================
	//!	メソッド名	CCOMMENT::デストラクタ
	//
	//	更新		2008/09/10	<新規>
	//================================================================================
	CCOMMENT::~CCOMMENT(void)
	{
	//	属性の解放
		for(int i=0; i<MAX_LINE; i++)
		{
			SAFE_DELETE(this->string[i]);
		}

	//	ボタンの解放
		SAFE_DELETE(this->buttomUp);
		SAFE_DELETE(this->buttomDown);
		SAFE_DELETE(this->sStaticDecision);
		SAFE_DELETE(this->sStaticVoice);
	}

	//================================================================================
	//!	メソッド名	CCOMMENT::getCommentTextureName
	//
	//	機能		コメントパターンに合った、テクスチャ名の検索
	//	引数		inPtn		コメントパターン
	//	戻り値		テクスチャ名
	//	更新		2008/09/10	<新規>
	//================================================================================
	LPSTR
	CCOMMENT::getCommentTextureName(COMMENT_PATTERN inPtn)
	{
	//	パターン分岐
		switch(inPtn)
		{
			case	eCPTN_NORMAL:		return	COMMENT_NORMAL;			break;
			case	eCPTN_SHOUT:		return	COMMENT_SHOUT;			break;
			case	eCPTN_SKY:			return	COMMENT_SKY;			break;
			case	eCPTN_SYSTEM:		return	COMMENT_SYSTEM;			break;
			case	eCPTN_NARRATION:	return	COMMENT_NARRATION;		break;
			default:					return	NULL;					break;
		}
	}

	//================================================================================
	//!	メソッド名	CCOMMENT::setString
	//
	//	機能		コメント行の挿入
	//	引数		inString	コメント文字列
	//				inLine		行番号
	//	更新		2008/09/10	<新規>
	//================================================================================
	void
	CCOMMENT::setString(LPSTR inString, int inLine, D3DCOLOR inColor)
	{
	//	行番号が不正なら、処理を返す
		if(	(inLine < 0) || (inLine >= MAX_LINE))	return;

	//	指定された行番号の場所にすでにデータがあるなら、それを解放する
		if(	this->string[inLine])	SAFE_DELETE(this->string[inLine]);

	//	フォント文字列の作成
		FONT_STRING*	newString	= new FONT_STRING();	

		newString->font		= eFONT_COMMENT;
		newString->color	= inColor;
		wsprintf(newString->szWord, inString);

	//	コメント行に登録
		this->string[inLine]	= newString;

	//	コメントバルーンの形状変化
		int		iMaxLength	= 0;							//!< 文字列の最大の長さ
		int		iLength		= 0;							//!< 文字列の長さ

		for(int i=0; i<MAX_LINE; i++)
		{
		//	現在見ている行に、コメントがある時
			if(	this->string[i])
			{
			//	コメントのバルーンの縦サイズを変更
				this->targetSize.y	=
					COMMENT_TEX_BLANKY + (COMMENT_TEXT_SIZEY*(float)(i+1));
			//	文字数を受け取る
				iLength	= (int)strlen(this->string[i]->szWord);
			//	最大文字列と比較し、更新されるなら、横サイズを変更
				if(	iMaxLength < iLength)
				{
					iMaxLength = iLength;
					this->targetSize.x	= 
						COMMENT_TEX_BLANKX + (COMMENT_TEXT_SIZEX*(float)(iMaxLength));
				}
			}
		}

	//	フォント位置の修正
		RECT	rcComment	=
		{
			(int)(this->getLocX() - ((this->targetSize.x - COMMENT_TEX_BLANKX) / 2.0f)),
			(int)(this->getLocY() - ((this->targetSize.y - COMMENT_TEX_BLANKY) / 2.0f)),
			1000,
			1000
		};

	//	色判定用
		MY_COLOR_BGRA	myColor;

		for(int i=0; i<MAX_LINE; i++)
		{
		//	現在見ている行に、コメントがある時
			if(	this->string[i])
			{
			//	位置を設定
				this->string[i]->rcPos	= rcComment;

			//	影用のコメントを設定
				this->stringShadow[i]	= *this->string[i];
			//	影の色を設定
				myColor.color	= this->stringShadow[i].color;
				if(	myColor.bgra.red	> 150	||
					myColor.bgra.green	> 150	||
					myColor.bgra.blue	> 150)
				{
					this->stringShadow[i].color	=
						D3DCOLOR_RGBA(0, 0, 0, myColor.bgra.alpha);
				}
				else
				{
					this->stringShadow[i].color	=
						D3DCOLOR_RGBA(200, 200, 200, myColor.bgra.alpha);
				}
			//	影の位置をずらす
				OffsetRect(&stringShadow[i].rcPos, 2, 2);
			}
		//	フォント位置をずらす
			OffsetRect(&rcComment, 0, COMMENT_TEXT_SIZEY);
		}

	//	ボタン位置の調整
		this->buttomUp->setLoc(&VEC3(
			this->getLocX() + (this->targetSize.x / 3.0f), this->getLocY() + (this->targetSize.y / 5.0f), 0.0f));
		this->buttomDown->setLoc(
			&this->buttomUp->getLoc());
	}

	//================================================================================
	//!	メソッド名	CCOMMENT::motionAppear
	//
	//	機能		出現動作
	//	更新		2008/09/10	<新規>
	//================================================================================
	void
	CCOMMENT::motionAppear(void)
	{
	//	拡大終了判定用
		BOOL	bStopX	= FALSE;
		BOOL	bStopY	= FALSE;

	//	コメントサイズを拡大していく
		if(	this->showSize.x +  COMMENT_EXPAND <= this->targetSize.x)
		{
			this->showSize.x += COMMENT_EXPAND;
		}
		else
		{
			this->showSize.x  = this->targetSize.x;
			bStopX	  = TRUE;
		}
		if(	this->showSize.y +  COMMENT_EXPAND <= this->targetSize.y)
		{
			this->showSize.y += COMMENT_EXPAND;
		}
		else
		{
			this->showSize.y  = this->targetSize.y;
			bStopY	  = TRUE;
		}

	//	左右に拡大し切った時
		if(	bStopX && bStopY)
		{
		//	待機動作に移る
			this->movePtn	= eCMPN_WAIT;
		//	音声の発生
			this->soundVoice();
		}
	}

	//================================================================================
	//!	メソッド名	CCOMMENT::motionWait
	//
	//	機能		待機動作
	//	引数		inBoolen	次ステップ判定
	//	更新		2008/09/10	<新規>
	//================================================================================
	void
	CCOMMENT::motionWait(BOOL inBoolen)
	{
	//	カウントダウンが0になったら、強制撤去
		if(	this->iCntDown <  NO_CNT_DOWN)
		{
			this->iCntDown--;
			if(	this->iCntDown <= 0)
			{
				inBoolen = TRUE;
			}
		}

	//	次ステップ判定
		if(	inBoolen)
		{
		//	消滅動作に移る
			this->movePtn	= eCMPN_DISAPPEAR;
		//	決定音
			this->soundDecision();
		}
	}

	//================================================================================
	//!	メソッド名	CCOMMENT::motionDisappear
	//
	//	機能		消滅動作
	//	更新		2008/09/10	<新規>
	//================================================================================
	void
	CCOMMENT::motionDisappear(void)
	{
	//	縮小終了判定用
		BOOL	bStopX	= FALSE;
		BOOL	bStopY	= FALSE;

	//	コメントサイズを縮小していく
		if(	this->showSize.x -  COMMENT_EXPAND >= 0.0f)
		{
			this->showSize.x -= COMMENT_EXPAND;
		}
		else
		{
			this->showSize.x  = 0.0f;
			bStopX	  = TRUE;
		}
		if(	this->showSize.y -  COMMENT_EXPAND >= 0.0f)
		{
			this->showSize.y -= COMMENT_EXPAND;
		}
		else
		{
			this->showSize.y  = 0.0f;
			bStopY	  = TRUE;
		}

	//	左右に縮小し切った時
		if(	bStopX && bStopY)
		{
		//	存在しない
			this->movePtn	= eCMPN_FALSE;
		}
	}

	//================================================================================
	//!	メソッド名	CCOMMENT::motion
	//
	//	機能		コメントの動作
	//	引数		inBoolen	次ステップ判定
	//	戻り値		TRUE		続行命令
	//				FALSE		削除命令
	//	更新		2008/09/10	<新規>
	//================================================================================
	BOOL
	CCOMMENT::motion(BOOL inBoolen)
	{
		switch(this->movePtn)	
		{
		//	出現動作
			case	eCMPN_APPEAR:		this->motionAppear();		return	TRUE;
		//	待機動作
			case	eCMPN_WAIT:			this->motionWait(inBoolen);	return	TRUE;
		//	消滅動作
			case	eCMPN_DISAPPEAR:	this->motionDisappear();	return	TRUE;
		//	その他なら、処理しない
			default:												return	FALSE;
		}
	}

	//================================================================================
	//!	メソッド名	CCOMMENT::drawAppear
	//
	//	機能		出現描画
	//	引数		inSprite	スプライトデバイス
	//	更新		2008/09/10	<新規>
	//================================================================================
	void
	CCOMMENT::drawAppear(CONST SPRITE inSprite)
	{
		CTEXTURE::draw(inSprite);
	}

	//================================================================================
	//!	メソッド名	CCOMMENT::drawWait
	//
	//	機能		待機描画
	//	引数		inSprite	スプライトデバイス
	//	更新		2008/09/10	<新規>
	//================================================================================
	void
	CCOMMENT::drawWait(CONST SPRITE inSprite)
	{
		CTEXTURE::draw(inSprite);			

	//	文字列の描画
		for(int i=0; i<MAX_LINE; i++)
		{
		//	現在見ている行に文字列があるなら、描画
			if(	this->string[i])
			{
				this->font->drawText(inSprite, &this->stringShadow[i]);
				this->font->drawText(inSprite, this->string[i]);
			}
		}

	//	特定条件下で、ボタンの描画
		if(	this->commentPtn != eCPTN_NARRATION)
		{
			this->iButtomCnt++;
		//	20フレームまで、ボタン押されていないテクスチャを表示
			if		(this->iButtomCnt	<= 20)	this->buttomUp->draw(inSprite);
		//	21フレームから、ボタン押されているテクスチャを表示
			else if	(this->iButtomCnt	>  20)	this->buttomDown->draw(inSprite);
		//	30フレームに到達したら、カウンタをリセット
			if(	this->iButtomCnt >= 30)			this->iButtomCnt = 0;
		}
	}

	//================================================================================
	//!	メソッド名	CCOMMENT::drawDisappear
	//
	//	機能		消滅描画
	//	引数		inSprite	スプライトデバイス
	//	更新		2008/09/10	<新規>
	//================================================================================
	void
	CCOMMENT::drawDisappear(CONST SPRITE inSprite)
	{
		CTEXTURE::draw(inSprite);
	}

	//================================================================================
	//!	メソッド名	CCOMMENT::draw
	//
	//	機能		コメントの描画
	//	引数		inSprite	スプライトデバイス
	//	戻り値		TRUE		コメント続行中
	//				FALSE		コメント終了
	//	更新		2008/09/10	<新規>
	//================================================================================
	BOOL
	CCOMMENT::draw(CONST SPRITE inSprite)
	{
	//	拡大比をスプライトに入れる
		switch(this->movePtn)
		{
		//	出現描画
			case	eCMPN_APPEAR:		this->drawAppear(inSprite);		return	TRUE;
		//	待機描画
			case	eCMPN_WAIT:			this->drawWait(inSprite);		return	TRUE;
		//	消滅描画
			case	eCMPN_DISAPPEAR:	this->drawDisappear(inSprite);	return	TRUE;
		//	その他なら、処理しない
			default:													return	FALSE;
		}
	}

	//================================================================================
	//!	メソッド名	CCOMMENT_GROUP::executeComment
	//
	//	機能		コメントの実行
	//	引数		inBoolen	コメントを進めるかの確認
	//	戻り値		TRUE		残りコメント有り
	//				FALSE		残りコメント無し
	//	更新		2008/11/11	<新規>
	//================================================================================
	BOOL
	CCOMMENT_GROUP::executeComment(BOOL inBoolen)
	{
	//	全コメントの動作
		VEC_WHILE(CCOMMENT*, vComment)
		{
		//	動作を実行し、終了したら解放し消去する
			if(	!(*p)->motion(inBoolen))
			{
				SAFE_DELETE(*p);
				p	= this->vComment.erase(p);
			}
		//	終了して無いなら、イテレータを進める
			else
			{
				p++;
			}
		}
	//	コメントがなくなったかを返す
		return	(this->vComment.empty() ? FALSE : TRUE);
	}
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------