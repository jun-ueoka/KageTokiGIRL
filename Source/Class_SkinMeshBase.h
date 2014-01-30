//***********************************************************************************
//!	ファイル名		Class_SkinMeshBase.h
//
//	機能			スキンメッシュベースに関する定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/08/30	<新規>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"Class_MeshBase.h"				//!< メッシュベース

//-----------------------------------------------------------------------
//!	スキンメッシュベースの名前空間
//-----------------------------------------------------------------------
namespace SMB
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	名前空間の解放
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	using	namespace	MB;			//!< メッシュベース

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	アニメーションに関する定義
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		kIDLE_TRANSITION_TIME	0.125f
	#define		kMOVE_TRANSITION_TIME	0.25f
	#define		kANIM_SPEED				0.016f

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	スキンメッシュリソースファイルへのパス
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_SKINMESH		"..//Resources//SkinMesh//%s.x"
	#define		PATH_SKINMESHTEX	"..//Resources//SkinMesh//Texture//%s"
	#define		PATH_SKINMESHDATA	"..//Resources//SkinMesh//SkinMeshData//%s.smd"

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	派生フレーム構造体 (それぞれのメッシュ用の最終ワールド行列を追加する）
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct MYFRAME : public D3DXFRAME
	{
		D3DXMATRIX	CombinedTransformationMatrix;	//!< それぞれのメッシュ用の最終ワールド行列
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	派生メッシュコンテナー構造体
	//	コンテナーがテクスチャを複数持てるようにポインターのポインターを追加する）
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct MYMESHCONTAINER : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*	ppTextures;				//!< テクスチャの
		DWORD				dwWeight;				//!< 重みの個数（重みとは頂点への影響。）
		DWORD				dwBoneAmt;				//!< ボーンの数
		LPD3DXBUFFER		pBoneBuffer;			//!< ボーン・テーブル
		D3DXMATRIX**		ppBoneMatrix;			//!< 全てのボーンのワールド行列の先頭ポインター
		D3DXMATRIX*			pBoneOffsetMatrices;	//!< フレームとしてのボーンのワールド行列のポインター
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	Xファイル内のアニメーション階層を読み下してくれるクラスを派生させる
	//	ID3DXAllocateHierarchyは派生すること想定して設計されている
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class MY_HIERARCHY: public ID3DXAllocateHierarchy
	{
	private:
	//	読み込むテクスチャのパス
		LPSTR		keepTexPath;

	public:
	//	コンストラクタ
		MY_HIERARCHY(CONST LPSTR inPath = NULL)	{ keepTexPath = inPath;		}

	//	フレームを構築する
		STDMETHOD(CreateFrame)			(THIS_ LPCSTR,
										 LPD3DXFRAME*);
	//	メッシュコンテナーを構築する
		STDMETHOD(CreateMeshContainer)	(THIS_ LPCTSTR,
										 CONST D3DXMESHDATA*,
										 CONST D3DXMATERIAL*, 
										 CONST D3DXEFFECTINSTANCE*,
										 DWORD,
										 CONST DWORD*,
										 LPD3DXSKININFO,
										 LPD3DXMESHCONTAINER*);
	//	フレームを解放する
		STDMETHOD(DestroyFrame)			(THIS_ LPD3DXFRAME);
	//	メッシュコンテナーを解放する
		STDMETHOD(DestroyMeshContainer)	(THIS_ LPD3DXMESHCONTAINER);	
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	文字列を判断する
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	HRESULT	AllocateName(LPCSTR inName, LPSTR* ioNewName);

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	スキンメッシュ情報を格納する構造体
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct SKIN_DATA
	{
		MESH_DATA*					mesh;					//!< メッシュ情報
		int							cnt;					//!< 使用数
		string						name;					//!< スキンメッシュ名
		string						rPath;					//!< リソースパス
		string						tPath;					//!< テクスチャパス
		LPD3DXFRAME					pFrameRoot;				//!< ルートフレーム（一番エラいフレーム）
		LPD3DXANIMATIONCONTROLLER	pAnimController;		//!< アニメーションコントローラー
		LPD3DXANIMATIONSET*			pAnimSet;				//!< アニメーションセット
		int							iAnimCnt;				//!< アニメーションの数
		int*						iAnimStep;				//!< アニメーションステップ

	//	実態化時の初期化
		SKIN_DATA(void)
		{
			mesh			= NULL;
			cnt				= 0;
			name			= "";
			rPath			= "";
			tPath			= "";
			pFrameRoot		= NULL;
			pAnimController	= NULL;
			pAnimSet		= NULL;
			iAnimCnt		= 0;
			iAnimStep		= NULL;
		}

	//	解放時処理
		~SKIN_DATA(void)
		{
			releaseData();
		}

	//	解放
		void releaseData(void)
		{
			MY_HIERARCHY ah;
		//	ボーンの解放
			ah.DestroyFrame(pFrameRoot);
			pFrameRoot = NULL;
		//	アニメーションコントローラーの解放
			SAFE_RELEASE(pAnimController);
		//	アニメーションセットの解放
			for(int i=0; i<iAnimCnt; i++)
			{
				SAFE_RELEASE(pAnimSet[i]);
			}
			SAFE_DELETE_ARRAY(pAnimSet);
		//	メッシュ情報の解放
			SAFE_DELETE(mesh);
		//	アニメーションステップの解放
			SAFE_DELETE_ARRAY(iAnimStep);
		}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CSKIN_MESH
	//
	//	機能			スキンメッシュベース
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSKIN_MESH_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	スキンメッシュ読み込みメソッド
			//==================================================================
		static	BOOL		createSkinInData(CONST DEV   inDev,
											 SKIN_DATA*  inSkinData,
											 CONST LPSTR inName,
											 CONST LPSTR inResource,
											 CONST LPSTR inTexture);
		static	SKIN_DATA*	createSkin(CONST DEV   inDev,
									   CONST LPSTR inName,
									   CONST LPSTR inResource,
									   CONST LPSTR inTexture);

			//==================================================================
			//	スキンマップ（インスタンスで共有される情報）
			//==================================================================
		static	map<string, SKIN_DATA*>*		mSkins;

			//==================================================================
			//	属性の初期準備
			//==================================================================
				void		startSkinMeshInitialize(void);

			//==================================================================
			//	メッシュの初期化、解放
			//==================================================================
				void		initializeSkinMesh(	CONST DEV   inDev,
												CONST LPSTR inName,
												CONST LPSTR inResource = PATH_SKINMESH,
												CONST LPSTR inTexture  = PATH_SKINMESHTEX);
				void		finishalizeSkinMesh(void);

			//==================================================================
			//	アニメーションに関する属性
			//==================================================================
				float	animSpeed;							//!< アニメーションスピード（一フレームあたり何秒間時間が進むのか）
				float	animTime;							//!< アニメーション時間（現在どの時間の動きをしているか）

				BOOL	bPlayAnim;							//!< アニメーションスイッチ（TRUEならアニメートする）
				int		prevAnimNumber;						//!< 前回のアニメーション番号
				int		currAnimNumber;						//!< 現在のアニメーション番号
				int		nextAnimNumber;						//!< 次回のアニメーション番号
				BOOL	bAnimChange;						//!< アニメーションチェンジ確認

				LPD3DXANIMATIONCONTROLLER	spAnimCtrl;		//!< 専用のアニメコントローラー

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	スキンメッシュ情報ポインター
			//==================================================================
				SKIN_DATA*	sd_ptr;

			//==================================================================
			//	スキンメッシュロード確認
			//==================================================================
				BOOL		bSkinLoaded;

	public:			//-------------------- public --------------------//

				CSKIN_MESH_BASE(CONST DEV   inDev,
								CONST LPSTR inName,
								CONST LPSTR inResource = PATH_SKINMESH,
								CONST LPSTR inTexture  = PATH_SKINMESHTEX);	//!< コンストラクタ
				CSKIN_MESH_BASE(void);										//!< コンストラクタ(空初期化)
				CSKIN_MESH_BASE(CSKIN_MESH_BASE* inCopy);					//!< コピー
		virtual	~CSKIN_MESH_BASE(void);										//!< デストラクタ

			//==================================================================
			//	リセット実行
			//==================================================================
		static	void	resetInstance(CONST DEV inDev);

			//==================================================================
			//	スキンメッシュの無効性を調べる
			//==================================================================
				BOOL	skinInvalid(void);

			//==================================================================
			//	access	アニメーションスイッチ
			//==================================================================
				BOOL	getPlayAnim(void)const				{ return	bPlayAnim;				}
				void	setPlayAnim(BOOL inPlayAnim)		{ bPlayAnim = inPlayAnim;			}

			//==================================================================
			//	access	加算するアニメーション時間（アニメーション進行速度）
			//==================================================================
				float	getAnimSpeed(void)const							{ return	animSpeed;		}
				void	setAnimSpeed(float inAnimSpeed = kANIM_SPEED)	{ animSpeed  = inAnimSpeed;	}
				void	calcAnimSpeed(float inAnimSpeed)				{ animSpeed += inAnimSpeed;	}

			//==================================================================
			//	access	現在のアニメーション時間
			//==================================================================
				float	getAnimTime(void)const				{ return	animTime;				}
				void	setAnimTime(float inAnimTime)		{ animTime = inAnimTime;			}

			//==================================================================
			//	access	現在のアニメーションの番号
			//==================================================================
				int		getCurrAnimNumber(void)const		{ return	currAnimNumber;			}
				void	setCurrAnimNumber(int inAnimNumber)	{ prevAnimNumber = currAnimNumber;
															  currAnimNumber = inAnimNumber;	}

			//==================================================================
			//	access	次回のアニメーションの番号
			//==================================================================
				int		getNextAnimNumber(void)const		{ return	nextAnimNumber;			}
				void	setNextAnimNumber(int inAnimNumber)	{ nextAnimNumber = inAnimNumber;	}

			//==================================================================
			//	access	スキンメッシュ情報
			//==================================================================
				SKIN_DATA*	getSkins(void)const				{ return	sd_ptr;					}

			//==================================================================
			//	ボーン行列の初期化
			//==================================================================
		static	HRESULT	AllocateBoneMatrix(SKIN_DATA*, LPD3DXMESHCONTAINER);
		static	HRESULT	AllocateAllBoneMatrices(SKIN_DATA*,  LPD3DXFRAME);

			//==================================================================
			//	メッシュコンテナー描画
			//==================================================================
				void	RenderMeshContainer(CONST DEV, MYMESHCONTAINER* ,MYFRAME*);

			//==================================================================
			//	フレームごとのワールド変換を行う
			//==================================================================
				void	UpdateFrameMatrices(LPD3DXFRAME, LPD3DXMATRIX);

			//==================================================================
			//	フレームを描画する
			//==================================================================
				void	DrawFrame(CONST DEV, LPD3DXFRAME);

			//==================================================================
			//	アニメーション変更メソッド
			//==================================================================
				void	changeAnimationAndBlend(int inNum);

			//==================================================================
			//	アニメーション
			//==================================================================
				BOOL	animation(void);

			//==================================================================
			//	描画
			//==================================================================
				BOOL	drawSkin(CONST DEV inDev, MAT16* inMatrix);
	};
}
//-----------------------------------------------------------------------
//!	スキンメッシュベースの名前空間
//-----------------------------------------------------------------------