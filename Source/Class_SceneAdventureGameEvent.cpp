//***********************************************************************************
//!	ファイル名		Class_SceneAdventureGameEvent.cpp
//
//	機能			ゲームシーンのイベント処理部分の実装
//	記述			上岡　純
//	プロジェクト	「」
//	更新			2008/07/10	<新規>
//					2008/11/06	<変更>	全体的に書き直し
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneAdventureGameData.h"

//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventChange
	//
	//	機能		ゲームイベント移行時の初期化
	//	更新		2008/11/07		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventChange(void)
	{
	//	ゲームイベント中にフラグを変更できたら、準備する
		if(	this->flagChange(eSGFLAG_EVENT))
		{
		//	プレイヤーのダメージ判定確認を無効にする
			this->player->setDamageCheck(FALSE);
		//	イベントファイルの読み込みに移る
			this->eventStep	= eEVSTEP_READ;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventProc
	//
	//	機能		イベント処理(再帰メソッド)
	//	引数		input			入力情報
	//	更新		2008/11/07		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProc(CINPUT* input)
	{
	//-------------------------------------------------
	//	イベントステップ分岐
	//-------------------------------------------------
		switch(this->eventStep)
		{
		//	読み込み中
			case	eEVSTEP_READ:
				this->sgEventStepRead();
				this->sgEventProc(input);
				break;
		//	実行中
			case	eEVSTEP_EXECUTE:
				this->sgEventExecuter();
				this->sgEventStepExecute();
				break;
		//	会話中
			case	eEVSTEP_TALK:
				this->sgEventExecuter();
				this->sgEventStepTalk(input);
				break;
		//	入力待機中
			case	eEVSTEP_INPUTWAIT:
				this->sgEventExecuter();
				this->sgEventStepInputWait(input);
				break;
		//	イベントが無い
			case	eEVSTEP_FALSE:
				return;
		//	イベントエラー
			case	eEVSTEP_ERROR:
				this->gameFlag	= eSGFLAG_ERROR;
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventStepRead
	//
	//	機能		イベントファイルを読む
	//	更新		2008/11/07		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventStepRead(void)
	{
	//	ファイル開かれいないなら、イベント終了
		if(	!this->eventFile)
		{
			this->gameFlag	= eSGFLAG_PLAY;
			return;
		}

	//	読み込み用
		char	reader[9]	= "";

	//	イベントの種類を読み込む
		fscanf_s(this->eventFile, "%s ", reader, sizeof(reader));

	//	イベントの種類で分岐
		switch(this->judgeEventOrderToString(reader))
		{
		//	コメント生成
 		case eEVORDER_TALK:			this->sgEventProcTalk();		break;
		//	コメント開始
		case eEVORDER_TALK_START:	this->sgEventProcTalkStart();	break;
		//	物体の生成
		case eEVORDER_OBJ_CREATE:	this->sgEventProcObjCreate();	break;
		//	物体の状態変化
		case eEVORDER_OBJ_CHANGE:	this->sgEventProcObjChange();	break;
		//	物体移動
		case eEVORDER_OBJ_MOVE:		this->sgEventProcObjMove();		break;
		//	物体消滅
		case eEVORDER_OBJ_CLEAR:	this->sgEventProcObjClear();	break;
		//	エフェクトの生成
		case eEVORDER_EFF_CREATE:	this->sgEventProcEffCreate();	break;
		//	エフェクトの発生
		case eEVORDER_EFF_OCCU:		this->sgEventProcEffOccu();		break;
		//	エフェクトの消滅
		case eEVORDER_EFF_CLEAR:	this->sgEventProcEffClear();	break;
		//	ダミー入力の操作
		case eEVORDER_INPUT_OPE:	this->sgEventProcInputOpe();	break;
		//	入力待機の操作
		case eEVORDER_INPUT_WAIT:	this->sgEventProcInputWait();	break;
		//	待機
		case eEVORDER_EXECUTE:		this->sgEventProcExecute();		break;
		//	ゲームクリア
		case eEVORDER_GAME_CLEAR:	this->sgEventProcGameClear();	break;
		//	イベント終了
		case eEVORDER_EVENT_QUIT:	this->sgEventProcEventQuit();	break;
		//	上記以外
		default:	this->eventStep	= eEVSTEP_ERROR;				break;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventStepExecute
	//
	//	機能		イベント実行
	//	更新		2008/11/11		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventStepExecute(void)
	{
	//	カウントが有れば、待機
		if(	this->eventWaitCnt > 0)
		{
		//	コメント実行
			this->eventCommentG->executeComment(FALSE);
			this->eventWaitCnt--;
		}
	//	無ければ、待機終了
		else
		{
		//	コメント自動消滅
			this->eventCommentG->executeComment(TRUE);
		//	待機時に2D描画される物体の解放
			this->effectCenter->releaseEffects2D();
			this->eventStep = eEVSTEP_READ;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventStepTalk
	//
	//	機能		イベント会話中
	//	引数		input			入力情報
	//	更新		2008/11/11		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventStepTalk(CINPUT* input)
	{
	//	入力の有無をチェック
		BOOL	bInput	= FALSE;
		if(	input->checkPressOnce(MYKEY::eSTART)	||
			input->checkPressOnce(MYKEY::eSOFT)		||
			input->checkPressOnce(MYKEY::eMIDDLE)	||
			input->checkPressOnce(MYKEY::eHARD)		||
			input->checkPressOnce(MYKEY::eDUST))
		{
			bInput	= TRUE;
		}

	//	コメント実行し、コメントデータが無くなれば、読み込み処理に戻る
		if(	!this->eventCommentG->executeComment(bInput))
		{
		//	待機時に2D描画される物体の解放
			this->effectCenter->releaseEffects2D();
			this->eventStep	= eEVSTEP_READ;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventStepInputWait
	//
	//	機能		イベント入力待機中
	//	引数		input			入力情報
	//	更新		2008/11/11		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventStepInputWait(CINPUT* input)
	{
	//	入力の有無をチェック
		BOOL	bInput	= FALSE;
		if(	input->checkPressOnce(this->eventWaitKey))
		{
		//	待機時に2D描画される物体の解放
			this->effectCenter->releaseEffects2D();
			this->eventStep	= eEVSTEP_READ;
			bInput			= TRUE;
		}

	//	コメント実行
		this->eventCommentG->executeComment(bInput);
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventExecuter
	//
	//	機能		イベント動作処理の実行
	//	更新		2008/11/11		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventExecuter(void)
	{
	//	実行中イベントの処理
		VEC_WHILE(CEXECUTER*, eventExecuter)
		{
		//	イベントを実行し、終了の合図が出たら、イベントを破棄する
			if(	!(*p)->execute())
			{
				SAFE_DELETE(*p);
				p	= this->eventExecuter.erase(p);
			}
		//	終了の合図が出てないなら、イテレータを進める
			else
			{
				p++;
			}
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventProcTalk
	//
	//	機能		コメント生成
	//	更新		2008/11/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcTalk(void)
	{
	//	コメント読み込み
		CCOMMENT*	newComment				= NULL;

		char		commentReader[MAX_PATH]	= "";
		VEC2		loc						= VEC2(0.0f, 0.0f);
		int			iPtn					= 0;
		int			iReversal				= 0;
		int			iLine					= 0;
		char		soundReader[MAX_PATH]	= "";
		int			r=0, g=0, b=0, a=0;

	//	データを読み込む
		fscanf_s(this->eventFile, "%f, %f, ",	&loc.x, &loc.y);
		fscanf_s(this->eventFile, "%d, ",		&iPtn);
		fscanf_s(this->eventFile, "%d, ",		&iReversal);
		fscanf_s(this->eventFile, "%d, ",		&iLine);
		fscanf_s(this->eventFile, "%s  ",		soundReader, sizeof(soundReader));

	//	読み込んだサウンド名をチェック
		if(	!strcmp(soundReader, READ_SOUND_NONE))
		{
		//	ダミーのサウンド名だったら、NULLを入れる
			strcpy_s(soundReader, sizeof(soundReader), "");
		}

	//	コメント生成
		newComment	=
			new CCOMMENT(this->gDev->getDevice(),
						 iPtn,
						 BOOL(iReversal),
						 &loc,
						 1000,
						 soundReader);

	//	文字列読み込み
		for(int i=0; i<iLine; i++)
		{
			fscanf_s(this->eventFile, "%s ", commentReader, sizeof(commentReader));
			fscanf_s(this->eventFile, "%d, %d, %d, %d, ", &r, &g, &b, &a);
			newComment->setString(commentReader, i, D3DCOLOR_RGBA(r,g,b,a));
		}

	//	コメントの登録
		this->eventCommentG->setComment(newComment);
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventProcTalkStart
	//
	//	機能		コメント開始
	//	更新		2008/11/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcTalkStart(void)
	{
	//	会話中に変更
		this->eventStep	= eEVSTEP_TALK;
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventProcObjCreate
	//
	//	機能		物体の生成
	//	更新		2008/11/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcObjCreate(void)
	{
	//	物体生成対象読み込み
		C3D_OBJECT*	newObject				= NULL;

		int			iEntry					= 0;
		char		objReader[9]			= "";
		char		nameReader[MAX_PATH]	= "";
		char		insertReader[9]			= "";
		VEC3		pos						= VEC3(0.0f, 0.0f, 0.0f);
		VEC3		angle					= VEC3(0.0f, 0.0f, 0.0f);
		VEC2		size					= VEC2(0.0f, 0.0f);
		int			r=0, g=0, b=0, a=0;
		MY_COLOR_RGBA	color;

	//	データを読み込む
		fscanf_s(this->eventFile, "%s ",	objReader,  sizeof(objReader));
		fscanf_s(this->eventFile, "%s ",	nameReader, sizeof(nameReader));
		fscanf_s(this->eventFile, "%d, ",	&iEntry);
		fscanf_s(this->eventFile, "%f, %f, %f, %f, %f, %f, %f, %f, ",
			&pos.x,   &pos.y,   &pos.z,
			&angle.x, &angle.y, &angle.z,
			&size.x,  &size.y);
		fscanf_s(this->eventFile, "%s ",	insertReader, sizeof(insertReader));

	//	物体生成対象の種類で分岐
		switch(this->judgeEventCreateObjToString(objReader))
		{
		//	テクスチャ
			case	eEVCRE_TEXTURE:
				fscanf_s(this->eventFile, "%d, %d, %d, %d, ", &r, &g, &b, &a);
				color.rgba.red		= (BYTE)r;
				color.rgba.green	= (BYTE)g;
				color.rgba.blue		= (BYTE)b;
				color.rgba.alpha	= (BYTE)a;
				newObject	=
					new CTEXTURE(gDev->getDevice(), nameReader, size, color.color);
				newObject->setLoc(&pos);
				newObject->setAngle(&angle);
				break;
		//	メッシュ
			case	eEVCRE_MESH:
				newObject	=
					new CMESH(gDev->getDevice(), nameReader);
				newObject->setLoc(&pos);
				newObject->setAngle(&angle);
				newObject->setScale(size.x);
				break;
		//	スキンメッシュ
			case	eEVCRE_SKINMESH:
				newObject	=
					new CSKIN_MESH(gDev->getDevice(), nameReader);
				newObject->setLoc(&pos);
				newObject->setAngle(&angle);
				newObject->setScale(size.x);
				break;
		//	エフェクト
			case	eEVCRE_EFFECT:
				newObject	=
					new C3D_EFFECT(gDev->getDevice(), nameReader, pos);
				newObject->setAngle(&angle);
				newObject->setScale(size.x);
				break;
		//	上記以外
			default:
				break;
		}

	//	オブジェクト登録先の種類で分岐
		switch(this->judgeEventInsertToString(insertReader))
		{
		//	ゲームマップ
			case	eEVINS_GAMEMAP:
			//	全オブジェクトは、いつでもマップ上で消去出来るようにする
				newObject->setOrderMessage(eOMSG_DELETE_IN);
			//	イベントマップに登録
				this->eventObject.insert(pair<int, C3D_OBJECT*>(iEntry, newObject));
			//	全体マップに登録
				this->gameMap->addViewG(newObject);
				break;
		//	待機中の2D描画
			case	eEVINS_WAIT_2D:
				this->effectCenter->addEffects(nameReader, newObject);
				this->effectCenter->occu2DEffects(nameReader, pos, size.x, angle);
				break;
		//	上記以外
			default:
				SAFE_DELETE(newObject);
				return;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventProcObjChange
	//
	//	機能		物体の状態変化
	//	更新		2008/11/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcObjChange(void)
	{
	//	状態変化対象読み込み
		char	objReader[9]	= "";
		char	chaReader[9]	= "";
		int		iNum			= 0;

	//	データを読み込む
		fscanf_s(this->eventFile, "%s ",	objReader, sizeof(objReader));
		fscanf_s(this->eventFile, "%s ",	chaReader, sizeof(chaReader));

	//	状態変化対象の種類で分岐
		switch(this->judgeEventObjectToString(objReader))
		{
		//	物体移動
			case	eEVOBJ_PLAYER:
			//	状態変化種類で分岐
				switch(this->judgeEventChangeToString(chaReader))
				{
				//	表示確認
					case	eEVCHAN_VISIBLE:
						fscanf_s(this->eventFile, "%d ",	&iNum);
						this->player->setAllVisible((!iNum ? FALSE : TRUE));
						break;
				//	アニメーション変更
					case	eEVCHAN_ANIMCHANGE:
						fscanf_s(this->eventFile, "%d ",	&iNum);
						this->playerChara->changeAnimationAndBlend(iNum);
						break;
				//	入力有効制御変更
					case	eEVCHAN_LIMIT:
						fscanf_s(this->eventFile, "%d ",	&iNum);
						this->player->setAllLimit(iNum);
						break;
				//	カゲオ限定制御変更
					case	eEVCHAN_LIMIT_KAGEO:
						fscanf_s(this->eventFile, "%d ",	&iNum);
						this->player->getKageo()->setLimit((ADVENT_KAGEO_ACTION_LIMIT)iNum);
						break;
				//	カゲミ限定制御変更
					case	eEVCHAN_LIMIT_KAGEMI:
						fscanf_s(this->eventFile, "%d ",	&iNum);
						this->player->getKagemi()->setLimit((ADVENT_KAGEMI_ACTION_LIMIT)iNum);
						break;
				//	キャラ変更有効確認
					case	eEVCHAN_CHARACHANGE_BOOLEN:
						fscanf_s(this->eventFile, "%d ",	&iNum);
						this->player->setChoiceCharaChangeBoolen((BOOL)iNum);
						break;
				//	カゲオにキャラ変更
					case	eEVCHAN_CHARACHANGE_KAGEO:
						this->player->setChoiceCharacter(eADCC_KAGEO);
						break;
				//	カゲミにキャラ変更
					case	eEVCHAN_CHARACHANGE_KAGEMI:
						this->player->setChoiceCharacter(eADCC_KAGEMI);
						break;
				}
				break;
		//	カメラ
			case	eEVOBJ_CAMERA:
			//	状態変化種類で分岐
				switch(this->judgeEventChangeToString(chaReader))
				{
				//	視点平衡移動モード
					case	eEVCHAN_PARALLEL:
						this->camera->setCameraMode(eCAMO_PARALLEL);
						break;
				//	カメラ平衡移動モード
					case	eEVCHAN_PARACHASE://
						this->camera->setCameraMode(eCAMO_PARALLEL_CHASE);
						break;
				//	二重追跡モード
					case	eEVCHAN_DOUBLETARGET://
						this->camera->setCameraMode(eCAMO_DOUBLE_TARGET);
						break;
				}
				break;
		//	ゲームクリア
			case	eEVOBJ_EVOBJECT:
				break;
		//	上記以外
			default:
				break;
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventProcObjMove
	//
	//	機能		物体移動
	//	更新		2008/11/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcObjMove(void)
	{
	//	移動対象読み込み
		char	reader[9]	= "";
		int		inFrame		= 0;
		VEC3	inEndPoint(0.0f,0.0f,0.0f);

	//	データを読み込む
		fscanf_s(this->eventFile, "%s ",	reader, sizeof(reader));
		fscanf_s(this->eventFile, "%d, ",	&inFrame);
		fscanf_s(this->eventFile, "%f, %f, %f, ",
			&inEndPoint.x, &inEndPoint.y, &inEndPoint.z);

	//	移動対象格納先
		C3D_OBJECT*	inTarget	= NULL;
	//	移動対象の種類で分岐
		switch(this->judgeEventObjectToString(reader))
		{
		//	物体移動
			case	eEVOBJ_PLAYER:
				inTarget	= this->playerChara;
				break;
		//	カメラ
			case	eEVOBJ_CAMERA:
				inTarget	= this->camera;
				break;
		//	ゲームクリア
			case	eEVOBJ_EVOBJECT:
				break;
		//	上記以外
			default:
				this->eventStep	= eEVSTEP_ERROR;
				return;
		}
	//	3Dオブジェクトを動かすイベント
		CEXECUTER*	newExecuter
			= new CEXE_MOVE_OBJECT(inTarget, inFrame, inEndPoint);
	//	イベント動作処理グループに登録
		this->eventExecuter.push_back(newExecuter);
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventProcObjClear
	//
	//	機能		物体消滅
	//	更新		2008/11/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcObjClear(void)
	{
		int		iEntry	= 0;
	//	読み込み
		fscanf_s(this->eventFile, "%d, ", &iEntry);
	//	物体マップから、データを取り出す
		map<int, C3D_OBJECT*>::iterator	p;
		p	= this->eventObject.find(iEntry);

	//	取り出したデータが有効なら、消去命令を出す
		if(	p	!= this->eventObject.end())
		{
			(*p).second->setOrderMessage(eOMSG_DELETE);
			this->eventObject.erase(p);
		}
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventProcEffCreate
	//
	//	機能		エフェクトの生成
	//	更新		2008/11/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcEffCreate(void)
	{
		char	nameReader[MAX_PATH]	= "";
	//	読み込み
		fscanf_s(this->eventFile, "%s ",	nameReader, sizeof(nameReader));
	//	エフェクトの生成
		C3D_EFFECT_CENTER::addEffectsInstance(
			nameReader,
			new C3D_EFFECT(this->gDev->getDevice(), nameReader, VEC3(0.0f,0.0f,0.0f)));
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventProcEffOccu
	//
	//	機能		エフェクトの発生
	//	更新		2008/11/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcEffOccu(void)
	{
		char	nameReader[MAX_PATH]	= "";
		VEC3	pos						= VEC3(0.0f, 0.0f, 0.0f);
		float	scale					= 1.0f;
	//	読み込み
		fscanf_s(this->eventFile, "%s ",		  nameReader, sizeof(nameReader));
		fscanf_s(this->eventFile, "%f, %f, %f, ", &pos.x,   &pos.y,   &pos.z);
		fscanf_s(this->eventFile, "%f, ",		  &scale);
	//	エフェクトの発生
		C3D_EFFECT_CENTER::occuEffectsInstance(nameReader, pos, scale);
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventProcEffClear
	//
	//	機能		エフェクトの消滅
	//	更新		2008/11/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcEffClear(void)
	{
		char	nameReader[MAX_PATH]	= "";
	//	読み込み
		fscanf_s(this->eventFile, "%s ", nameReader, sizeof(nameReader));
	//	エフェクトの生成
		C3D_EFFECT_CENTER::releaseEffectInstance(nameReader);
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventProcInputOpe
	//
	//	機能		ダミー入力の操作
	//	更新		2008/11/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcInputOpe(void)
	{
		int		inKey		= 0;
		int		inFrame		= 0;
	//	読み込み
		fscanf_s(this->eventFile, "%d, ", &inFrame);
		fscanf_s(this->eventFile, "%d, ", &inKey);

	//	ダミー入力情報を操作する
		CEXECUTER*	newExecuter
			= new CEXE_DUMMY_INPUT(this->eventInput, inFrame, inKey);

		this->eventExecuter.push_back(newExecuter);
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventProcInputWait
	//
	//	機能		入力待機移行
	//	更新		2008/11/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcInputWait(void)
	{
		int		inKey		= 0;
	//	読み込み
		fscanf_s(this->eventFile, "%d, ", &inKey);
	//	待機するキー
		this->eventWaitKey	= (MYKEY::TYPENAME)inKey;
	//	入力待機中に変更
		this->eventStep		= eEVSTEP_INPUTWAIT;
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventProcExecute
	//
	//	機能		待機移行
	//	更新		2008/11/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcExecute(void)
	{
	//	読み込み
		fscanf_s(this->eventFile, "%d, ", &this->eventWaitCnt);
	//	実行へ移る
		this->eventStep		= eEVSTEP_EXECUTE;
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventProcGameClear
	//
	//	機能		ゲームクリア移行
	//	更新		2008/11/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcGameClear(void)
	{
	//	イベント終了
		this->eventStep	= eEVSTEP_FALSE;
	//	ゲームクリアの状態にする
		this->sgClearChange();
	//	イベント動作処理の解放
		this->releaseEventExecuter();
	//	コメントグループの中身を開放
		this->eventCommentG->releaseComment();
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventProcEventQuit
	//
	//	機能		イベント終了
	//	更新		2008/11/10		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcEventQuit(void)
	{
	//	イベント終了
		this->eventStep	= eEVSTEP_FALSE;
	//	ゲームプレイの状態にする
		this->sgPlayChange();
	//	イベント動作処理の解放
		this->releaseEventExecuter();
	//	コメントグループの中身を開放
		this->eventCommentG->releaseComment();
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventExecute
	//
	//	機能		ゲームイベント中処理
	//	引数		inInput			入力情報
	//	更新		2008/11/07		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventExecute(CINPUT* inInput)
	{
	//	イベント実行
		this->sgEventProc(inInput);;
	//	プレイヤーの操作
		this->player->operation(this->eventInput);
	//	プレイヤーキャラクターの操作
		this->playerChara->operation(this->eventInput);
	}

	//================================================================================
	//!	メソッド名	CSCENE_ADVENTURE_GAME::sgEventRender
	//
	//	機能		ゲームイベント中描画
	//	更新		2008/11/07		<新規>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventRender(void)
	{
	//	デバイスを確保
		DEV	device	= this->gDev->getDevice();
	//	描画開始
		this->gDev->renderBegin();

	//	カメラの更新
		this->camera->updateCamera();
	//	ライトの更新
		this->sgUpdateLight();

	//-------------------------------------------------
	//	3D物体の描画
	//-------------------------------------------------
	//	Zテクスチャシェーダでテクスチャに深度値情報を書き込む
		this->shaderZTexture->begin();
		this->gameMap->squeezeSortByCameraPositionWithDrawZTexSha(
			this->camera, &this->camera->getViewPointFinish(), this->shaderZTexture);
		this->shaderZTexture->end();

		this->gDev->bufferClear(D3DCOLOR_RGBA(255,255,255,255));

	//	カメラでデバイス更新
		this->camera->viewTransform(device);

	//	背景の描画
		this->back->motion(&this->camera->getViewPointFinish());
		this->back->drawBack(device, this->gDev->getSprite());

	//	深度バッファシェーダで描画
		this->shaderDepthBuf->setCameraViewProjMatrix(
			&((*this->camera->getMatrixView()) * (*this->camera->getMatrixProjection())));

		this->playerChara->draw(device);
		this->gameMap->drawEnemyG(device, &this->camera->getViewPointFinish());

	//	ライティングを無効
		this->shaderDepthBuf->begin();
		this->gameMap->squeezeDrawWithDepthSha(this->shaderDepthBuf);
		this->shaderDepthBuf->end();

		this->effectCenter->drawEffects(device, this->camera);

	//-------------------------------------------------
	//	2D物体の描画
	//-------------------------------------------------
		this->gDev->spriteBegin();
		this->eventCommentG->drawComment(this->gDev->getSprite());
		this->effectCenter->drawEffects2D(this->gDev->getSprite());
		this->gDev->spriteEnd();

	//	幕の描画
		this->gDev->renderCurtainInDev();
	//	描画終了
		this->gDev->renderEnd();
	}
}
//-----------------------------------------------------------------------
//!	シーンの名前空間
//-----------------------------------------------------------------------