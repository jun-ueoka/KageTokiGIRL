//***********************************************************************************
//!	ファイル名		Class_MeshBase.h
//
//	機能			メッシュベースに関する定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2008/08/30	<新規>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"DirectX.h"				//!< DirectX使用

//-----------------------------------------------------------------------
//!	メッシュベースの名前空間
//-----------------------------------------------------------------------
namespace MB
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	メッシュリソースファイルへのパス
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_MESH		"..//Resources//Mesh//%s.x"
	#define		PATH_MESHTEX	"..//Resources//Mesh//Texture//%s"

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	メッシュ情報
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	MESH_DATA
	{
		int		cnt;				//!< 使用数
		string	name;				//!< メッシュ名
		string	rPath;				//!< リソースパス
		string	tPath;				//!< テクスチャパス
		XMESH	mesh;				//!< メッシュ
		VTEX9	meshVB;				//!< 頂点バッファ
		DWORD	numMaterials;		//!< マテリアル数
		MTR*	materials;			//!< マテリアル
		TEX*	texture;			//!< テクスチャ
		VEC3	localCenter;		//!< 中心点
		float	localRadius;		//!< 半径

	//	実態化時の初期化
		MESH_DATA(void)
		{
			cnt				= 0;
			name			= "";
			rPath			= "";
			tPath			= "";
			mesh			= NULL;
			meshVB			= NULL;
			numMaterials	= 0;
			materials		= NULL;
			texture			= NULL;
			localCenter		= VEC3(0.0f, 0.0f, 0.0f);
			localRadius		= 0.0f;
		};

	//	解放時処理
		~MESH_DATA(void)
		{
			releaseData();
		};

	//	解放
		void releaseData(void)
		{
		//	テクスチャリストからテクスチャのクリア
			if(	texture)
			{
				for(DWORD i = 0; i < numMaterials; i++)
				{
					SAFE_RELEASE(texture[i]);
				}
				SAFE_DELETE_ARRAY(texture);
			}
		//	マテリアルリストからマテリアルのクリア
			SAFE_DELETE_ARRAY(materials);
		//	メッシュのクリア
			SAFE_RELEASE(mesh);
		}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		CMESH_BASE
	//
	//	機能			メッシュベース
	//	更新			2007/12/08	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CMESH_BASE
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	メッシュの生成
			//==================================================================
		static	BOOL		createMeshInData(CONST DEV   inDev,
											 MESH_DATA*  inMeshData,
											 CONST LPSTR inName,
											 CONST LPSTR inResource,
											 CONST LPSTR inTexture);
		static	MESH_DATA*	createMesh(CONST DEV   inDev,
									   CONST LPSTR inName,
									   CONST LPSTR inResource,
									   CONST LPSTR inTexture);

			//==================================================================
			//	メッシュマップ（インスタンスで共有される情報）
			//==================================================================
		static	map<string, MESH_DATA*>*		mMeshs;

			//==================================================================
			//	属性の初期準備
			//==================================================================
				void		startMeshInitialize(void);

			//==================================================================
			//	メッシュの初期化、解放
			//==================================================================
				void		initializeMesh(	CONST DEV   inDev,
											CONST LPSTR inName,
											CONST LPSTR inResource = PATH_MESH,
											CONST LPSTR inTexture  = PATH_MESHTEX);
				void		finishalizeMesh(void);


	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	メッシュ情報ポインター
			//==================================================================
				MESH_DATA*	md_ptr;

			//==================================================================
			//	メッシュのロード確認
			//==================================================================
				BOOL		bMeshLoaded;

	public:			//-------------------- public --------------------//

				CMESH_BASE(	CONST DEV	inDev,
							CONST LPSTR	inName,
							CONST LPSTR	inResource = PATH_MESH,
							CONST LPSTR	inTexture  = PATH_MESHTEX);		//!< コンストラクタ
				CMESH_BASE(void);										//!< コンストラクタ(空初期化)
				CMESH_BASE(CMESH_BASE* inCopy);							//!< コピー
		virtual	~CMESH_BASE(void);										//!< デストラクタ

			//==================================================================
			//	リセット実行
			//==================================================================
		static	void	resetInstance(CONST DEV inDev);

			//==================================================================
			//	メッシュの無効性を調べる
			//==================================================================
				BOOL		meshInvalid(void);

			//==================================================================
			//	access	メッシュ情報
			//==================================================================
				XMESH		getXMesh(void)			{ return	md_ptr->mesh;			}
				VEC3		getLocalCenter(void)	{ return	md_ptr->localCenter;	}
				float		getLocalRadius(void)	{ return	md_ptr->localRadius;	}

			//==================================================================
			//	access	メッシュ情報
			//==================================================================
				MESH_DATA*	getMeshs(void)const		{ return	md_ptr;					}

			//==================================================================
			//	描画
			//==================================================================
				BOOL	drawMesh(CONST DEV inDev, MAT16* inMatrix, float inCalcDiffuse = 0.0f);
				BOOL	drawMesh(CONST DEV inDev, MAT16* inMatrix, D3DCOLORVALUE* inDiffuse);
	};
}
//-----------------------------------------------------------------------
//!	メッシュベースの名前空間
//-----------------------------------------------------------------------