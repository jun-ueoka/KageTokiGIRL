//***********************************************************************************
//!	ファイル名		Class_AdventEnemyManager.cpp
//
//	機能			アドベンチャーゲームの敵キャラクターを管理するクラスの実装
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2009/02/16	<新規>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//-----------------------------------------------------------------------
//!	セーブリソースの名前空間
//-----------------------------------------------------------------------
namespace EM
{
	//================================================================================
	//	インスタンス生成場所
	//================================================================================
	CADVENT_ENEMY_MANAGER*	CADVENT_ENEMY_MANAGER::gUniqueInstance	= NULL;

	//================================================================================
	//!	メソッド名	CADVENT_ENEMY_MANAGER::コンストラクタ
	//
	//	引数		inDev		Direct3Dデバイス
	//	更新		2009/02/16	<新規>
	//================================================================================
	CADVENT_ENEMY_MANAGER::CADVENT_ENEMY_MANAGER(DEV inDev)
	{
	//	生成
		this->shadoll			= NULL;
		this->shadoll_sword		= NULL;
		this->shadog			= NULL;
		this->shadoll_archery	= NULL;

		this->currFileNumber	= -10000;
	}

	//================================================================================
	//!	メソッド名	CADVENT_ENEMY_MANAGER::デストラクタ
	//
	//	更新		2009/02/16	<新規>
	//================================================================================
	CADVENT_ENEMY_MANAGER::~CADVENT_ENEMY_MANAGER(void)
	{
	//	解放
		SAFE_DELETE(this->shadoll);
		SAFE_DELETE(this->shadoll_sword);
		SAFE_DELETE(this->shadog);
		SAFE_DELETE(this->shadoll_archery);
	}

	//================================================================================
	//!	メソッド名	CADVENT_ENEMY_MANAGER::getInstance
	//
	//	機能		インスタンスの取得
	//	引数		inDev		Direct3Dデバイス
	//	戻り値		インスタンスを返す
	//	更新		2009/02/16	<新規>
	//================================================================================
	CADVENT_ENEMY_MANAGER*
	CADVENT_ENEMY_MANAGER::getInstance(DEV inDev)
	{
	//!	生成されていなければ、生成する
		if(	!gUniqueInstance)
		{
		//!	インスタンス生成
			gUniqueInstance = new CADVENT_ENEMY_MANAGER(inDev);
		}
	//!	インスタンスを返す
		return gUniqueInstance;
	}

	//================================================================================
	//!	メソッド名	CADVENT_ENEMY_MANAGER::releaseInstance
	//
	//	機能		インスタンス解放
	//	更新		2009/02/16	<新規>
	//================================================================================
	void
	CADVENT_ENEMY_MANAGER::releaseInstance(void)
	{
	//!	インスタンスの解放
		SAFE_DELETE(gUniqueInstance);
	}

	//================================================================================
	//!	メソッド名	CADVENT_ENEMY_MANAGER::loadEnemyFromFile
	//
	//	機能		ファイルから、敵を読み込む
	//	引数		inFileNumber	読み込むファイルの番号
	//				inPlayer		プレイヤーキャラクター
	//				inViewG			描画グループ
	//				inHitG			衝突判定グループ
	//				inEnemyG		敵グループ
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CADVENT_ENEMY_MANAGER::loadEnemyFromFile(
		DEV								inDev,
		int								inFileNumber,
		CKAGETOKI_ADVENT_CHARACTER**	inPlayer,
		C3D_OBJECT_GROUP*				inViewG,
		C3D_OBJECT_GROUP*				inHitG,
		C3D_OBJECT_GROUP*				inEnemyG)
	{
	//	読み込むファイル番号が前回と等しくないなら、新たに読み込む
		if(	this->currFileNumber != inFileNumber)
		{
			this->releaseEnemy();
			this->currFileNumber  = inFileNumber;
		}
	//	等しいなら、リセットをかける
		else
		{
			this->resetEnemy();
			return;
		}

	//-------------------------------------------------
	//	ファイルを開く
	//-------------------------------------------------
		FILE*	enemyFile			= NULL;		//!< ステージテキストファイルポインター
		char	szString[MAX_PATH]	= "";		//!< テキスト名作成用

	//	テキスト名を生成
		sprintf_s(szString, sizeof(szString), PATH_ENEMYMAP, this->currFileNumber);
	//	ファイルを開く
		fopen_s(&enemyFile, szString, "r");

	//	ファイルを開けなかった時
		if(	!enemyFile)
		{
		//	読み込みを行わず、処理を返す
			return;
		}

	//	読み込み開始
		ENEMY_DATA	enemyData	= ENEMY_DATA();				//!< 新たな敵データ
		VEC3		appearLoc	= VEC3(0.0f, 0.0f, 0.0f);	//!< 出現位置
		MY_CUBE		controlArea	= MY_CUBE();				//!< 制御範囲のオフセット値
		MY_CUBE		actionArea	= MY_CUBE();				//!< 行動可能範囲
		BOOL		bCheckEnd	= TRUE;						//!< 終了確認
		int			iEnemyKind	= 0;						//!< 敵種類番号

	//	敵判別用
		char	enemyJudger[eEKIND_MAX][11]	=
		{
			"SHADOLL___",
			"SHADOLL_SW",
			"SHADOG____",
			"SHADOLL_AR",
		};

	//	終了命令があるまで、ループ
		while(bCheckEnd)
		{
			ZeroMemory(szString, sizeof(szString));
			fscanf_s(enemyFile, "%s, ", szString, sizeof(szString));	//!< 敵の種類

		//	物体生成の種類を、数値として検出
			for(iEnemyKind = 0; iEnemyKind < eEKIND_MAX; iEnemyKind++)
			{
				if(	strcmp(szString, enemyJudger[iEnemyKind]) == 0)	break;
			}

		//	グループに該当しないデータなら、処理を終了
			if(	iEnemyKind >= eEKIND_MAX)	break;

		//	検出した敵種類番号で分岐して、生成
			switch(iEnemyKind)
			{
			//	シャドール
				case eEKIND_SHADOLL:
					enemyData.enemy	= new CENEMY_SHADOLL(inDev);
					if(!this->shadoll){
						this->shadoll	= new CSKIN_MESH_BASE(inDev, "enemy_shadoll");
					}
					break;
			//	シャドールソード
				case eEKIND_SHADOLL_SWORD:
					enemyData.enemy	= new CENEMY_SHADOLL_SWORD(inDev);
					if(!this->shadoll_sword){
						this->shadoll_sword	= new CSKIN_MESH_BASE(inDev, "enemy_shadoll_sword");
					}
					break;
			//	シャドッグ
				case eEKIND_SHADOG:
					enemyData.enemy	= new CENEMY_SHADOG(inDev);
					if(!this->shadog){
						this->shadog	= new CSKIN_MESH_BASE(inDev, "enemy_shadog");
					}
					break;
			//	シャドールアーチェリー
				case eEKIND_SHADOLL_ARCHERY:
					enemyData.enemy	= new CENEMY_SHADOLL_ARCHERY(inDev);
					if(!this->shadoll_archery){
						this->shadoll_archery	= new CSKIN_MESH_BASE(inDev, "enemy_shadoll_archery");
					}
					break;
			//	上記以外
				default:
					bCheckEnd	= false;
					continue;
			}

		//	出現位置
			fscanf_s(enemyFile, "%f, %f, %f, ",
						&appearLoc.x,
						&appearLoc.y,
						&appearLoc.z);
		//	制御範囲のオフセット値
			fscanf_s(enemyFile, "%f, %f, %f, %f, %f, %f, ",
						&controlArea.left,
						&controlArea.right,
						&controlArea.bottom,
						&controlArea.top,
						&controlArea.rear,
						&controlArea.front);
		//	行動可能範囲
			fscanf_s(enemyFile, "%f, %f, %f, %f, %f, %f, ",
						&actionArea.left,
						&actionArea.right,
						&actionArea.bottom,
						&actionArea.top,
						&actionArea.rear,
						&actionArea.front);
		//	情報のセット
			enemyData.appearLoc	= appearLoc;
			enemyData.enemy->setLoc(&enemyData.appearLoc);
			enemyData.enemy->setControlAreaOffset(&controlArea);
			enemyData.enemy->setActionArea(&actionArea);
			enemyData.enemy->setViewG(inViewG);
			enemyData.enemy->setStageG(inHitG);
			enemyData.enemy->setEnemyG(inEnemyG);
			enemyData.enemy->setTargetPlayer(inPlayer);
			enemyData.enemy->resetParameter();
		//	敵グループに登録
			inEnemyG->add(enemyData.enemy);
		//	敵データメンバーに登録
			this->enemyObjects.push_back(enemyData);
		}

	//	ファイルを閉じる
		fclose(enemyFile);
	}

	//================================================================================
	//!	メソッド名	CADVENT_ENEMY_MANAGER::updateEnemy
	//
	//	機能		敵の更新
	//	引数		inPlayerLoc		プレイヤーの位置
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CADVENT_ENEMY_MANAGER::updateEnemy(VEC3* inPlayerLoc)
	{
	//	挙動制御
		VEC_LOOP(ENEMY_DATA, this->enemyObjects)
		{
			if(	p->enemy->getVisible()	&&
				p->enemy->getControlArea()->intersect(inPlayerLoc))
			{
				p->enemy->operation(NULL);
			}
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_ENEMY_MANAGER::resetEnemy
	//
	//	機能		敵のリセット
	//	更新		2009/02/16		<新規>
	//================================================================================
	void
	CADVENT_ENEMY_MANAGER::resetEnemy(void)
	{
		VEC_LOOP(ENEMY_DATA, this->enemyObjects)
		{
			p->enemy->setLoc(&p->appearLoc);
			p->enemy->resetParameter();
		}
	}

	//================================================================================
	//!	メソッド名	CADVENT_ENEMY_MANAGER::releaseEnemy
	//
	//	機能		敵の解放
	//	更新		2009/02/16	<新規>
	//================================================================================
	void
	CADVENT_ENEMY_MANAGER::releaseEnemy(void)
	{
	//	敵データメンバーの解放
		VEC_LOOP(ENEMY_DATA, this->enemyObjects)
		{
			SAFE_DELETE(p->enemy);
		}
		this->enemyObjects.clear();

		this->currFileNumber	= -10000;
	}
}
//-----------------------------------------------------------------------
//!	セーブリソースの名前空間
//-----------------------------------------------------------------------