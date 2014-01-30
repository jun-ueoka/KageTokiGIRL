//***********************************************************************************
//!	ファイル名		Class_3DObject.h
//
//	機能			このプログラムで使用する三次元の物体に関する定義
//	記述			上岡　純
//	プロジェクト	未定
//	更新			2007/12/08	<新規>
//					2008/06/24	<追加>	全体的にコメントの強化
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	インターフェイスヘッダー
//-----------------------------------------------------------------------
#include	"DirectX.h"					//!< DirectX使用
#include	"Class_ZTextureShader.h"	//!< Zテクスチャシェーダ
#include	"Class_DepthBufShader.h"	//!< 深度バッファシェーダ

//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	角度を範囲内に収める
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#ifndef	NORMALIZE_DEGREE
		#define	NORMALIZE_DEGREE(angle)				\
		{											\
			if(angle	>=	360.0f)					\
			   angle	-=	360.0f;					\
			else if(angle	<	0.0f)				\
					angle	+=	360.0f;				\
		}
	#endif	//!	NORMALIZE_DEGREE

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	上下
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum LINE_UPDOWN
	{
		eUD_UP		=  1,	//!< 上
		eUD_NO		=  0,	//!< なし
		eUD_DOWN	= -1	//!< 下
	};
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	左右
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum LINE_LEFTRIGHT
	{
		eLR_RIGHT	=  1,	//!< 右
		eLR_NO		=  0,	//!< なし
		eLR_LEFT	= -1	//!< 左
	};
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	前後
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum LINE_FRONTREAR
	{
		eFR_FRONT	= -1,	//!< 前
		eFR_NO		=  0,	//!< なし
		eFR_REAR	=  1	//!< 後
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	サブメッセージ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum ORDER_MESSAGE
	{
		eOMSG_NOMSG,			//!< メッセージなし
		eOMSG_DELETE_IN,		//!< 消滅グループに入れる
		eOMSG_DELETE,			//!< 消滅命令
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	識別メッセージ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum THIS_MESSAGE
	{
		eTMSG_NOMSG,			//!< メッセージなし
		eTMSG_ENEMY,			//!< 敵
		eTMSG_NO_SEE_SHADOW,	//!< 見えない影
		eTMSG_SEE_SHADOW,		//!< 見える影
		eTMSG_SHADOW_BALL,		//!< シャドウボール
		eTMSG_SHINE_BALL,		//!< シャインボール
		eTMSG_ENERGY_BALL,		//!< エナジーボール
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	リアクションメッセージ
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum REACTION_MESSAGE
	{
		eREAMSG_NOMSG,				//!< メッセージなし
		eREAMSG_SHADOW_HIT,			//!< シャドウボールに当たった
		eREAMSG_SHINE_HIT,			//!< シャインボールに当たった
		eREAMSG_ENERGY_HIT,			//!< エナジーボールに当たった
		eREAMSG_ENEMY_ATTACK_LV1,	//!< 敵の攻撃Lv1
		eREAMSG_ENEMY_ATTACK_LV2,	//!< 敵の攻撃Lv2
		eREAMSG_ENEMY_ATTACK_LV3,	//!< 敵の攻撃Lv3
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	方向
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	LINE
	{
		LINE_UPDOWN		ud;
		LINE_LEFTRIGHT	lr;
		LINE_FRONTREAR	fr;

	//	実体化処理
		LINE(	LINE_UPDOWN		inUD = eUD_NO,
				LINE_LEFTRIGHT	inLR = eLR_NO,
				LINE_FRONTREAR	inFR = eFR_NO)
		{
			ud	= inUD;
			lr	= inLR;
			fr	= inFR;
		}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	角度に扱う構造体
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	ROTATION
	{
		VEC3			xAxis;		//!< 座標系Xの軸線
		VEC3			yAxis;		//!< 座標系Yの軸線
		VEC3			zAxis;		//!< 座標系Zの軸線
		VEC3			rpm;		//!< 毎分回転数
		VEC3			angle;		//!< 角度
		MAT16			matrix3D;	//!< 3D回転行列
		MAT16			matrixX;	//!< X軸回転行列
		MAT16			matrixY;	//!< Y軸回転行列
		MAT16			matrixZ;	//!< Z軸回転行列
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	座標を扱う構造体
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	TRANSLATION
	{
		LINE			line;		//!< 方向頂点
		VEC3			prev;		//!< 前の座標
		VEC3			curr;		//!< 現在の座標
		VEC3			next;		//!< 次の座標
		MAT16			matrix;		//!< 行列
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	クラス名		C3D_OBJECT
	//
	//	機能			三次元の物体
	//					三次元の物体の共通の性質を定義している。（抽象クラス）
	//	更新			2008/06/24	<新規>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	C3D_OBJECT
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	オブジェクトステータス
			//==================================================================
				MAT16			matWorld;				//!< ワールド行列
				MAT16			matWorldIV;				//!< ワールド逆行列
				ROTATION		rot;					//!< 角度情報
				TRANSLATION		loc;					//!< 座標情報
				BOOL			bVisible;				//!< 表示確認
				float			fScale;					//!< 倍率
				MAT16			matScale;				//!< 大きさの行列
				float			fMoveQuantity;			//!< 移動量
				float			fOffsetFromCamera;		//!< カメラとの距離
				float			fTemporaryRadius;		//!< 当たり判定用半径
				float			fDist;					//!< 衝突物体間隔
				ORDER_MESSAGE	orderMessage;			//!< 命令メッセージ(必要な時)
				THIS_MESSAGE	thisMessage;			//!< 識別メッセージ(必要な時)

			//==================================================================
			//	描画ステータス
			//==================================================================
				float			fDiffuseAlpha;			//!< デフューズアルファ変動値

			//==================================================================
			//	各行列を更新する
			//==================================================================
				void	updateScale(void);				//!< スケーリング行列
				void	updateRotat(void);				//!< 回転行列
				void	updateLoc(void);				//!< 座標行列
				void	updateWorld(void);				//!< ワールド行列

	public:			//-------------------- public --------------------//

				C3D_OBJECT(void)	{ initialize();	}	//!< コンストラクタ
		virtual	~C3D_OBJECT(void)	{ finalize();	}	//!< デストラクタ
				void	initialize(void);				//!< 初期化
				void	finalize(void);					//!< 後始末

			//==================================================================
			//	access	表示確認
			//==================================================================
				BOOL	getVisible(void)const				{ return	bVisible;					}
				void	setVisible(BOOL	inVisible)			{ bVisible = inVisible;					}

			//==================================================================
			//	access	回転行列
			//==================================================================
				MAT16*	getMatrixRotation(void)				{ return	&rot.matrix3D;				}
				MAT16*	getMatrixRotationX(void)			{ return	&rot.matrixX;				}
				MAT16*	getMatrixRotationY(void)			{ return	&rot.matrixY;				}
				MAT16*	getMatrixRotationZ(void)			{ return	&rot.matrixZ;				}
				void	setMatrixRotationX(MAT16* inX)		{ rot.matrixX = *inX;	updateRotat();	}
				void	setMatrixRotationY(MAT16* inY)		{ rot.matrixY = *inY;	updateRotat();	}
				void	setMatrixRotationZ(MAT16* inZ)		{ rot.matrixZ = *inZ;	updateRotat();	}

			//==================================================================
			//	access	ワールド行列
			//==================================================================
				MAT16*	getWorldMatrix(void)				{ return	&matWorld;					}
				MAT16*	getWorldMatrixInversed(void)		{ return	&matWorldIV;				}

			//==================================================================
			//	このワールド逆行列のベクトルのトランスフォームを求める
			//==================================================================
				void	rayFromInversedMatrix(VEC3* inFrom, VEC3* inDir);

			//==================================================================
			//	access	倍率
			//==================================================================
				float	getScale(void)const					{ return	fScale;						}
				void	setScale(float inScale)				{ fScale = inScale;	updateScale();		}
				MAT16*	getMatrixScaling(void)				{ return	&matScale;					}

			//==================================================================
			//	access	移動量
			//==================================================================
				float	getMoveQuantity(void)const			{ return	fMoveQuantity;				}
				void	setMoveQuantity(float inMoveQuantity)
				{
				//	移動量は、負の数であってはならない
					fMoveQuantity = (inMoveQuantity >= 0.0f ? inMoveQuantity : -inMoveQuantity);
				}
				void	calcMoveQuantity(float inMoveQuantity)
				{
					fMoveQuantity +=  inMoveQuantity;
				//	移動量は、負の数であってはならない
					fMoveQuantity  = (fMoveQuantity >= 0.0f ? fMoveQuantity : 0.0f);
				}

			//==================================================================
			//	access	位置
			//==================================================================
		virtual	VEC3	getLoc(void)						{ return	loc.curr;											}
				float	getLocX(void)const					{ return	loc.curr.x;											}
				float	getLocY(void)const					{ return	loc.curr.y;											}
				float	getLocZ(void)const					{ return	loc.curr.z;											}
				void	setLoc(VEC3* inLoc)					{ loc.prev   = loc.curr;   loc.curr    = *inLoc;	updateLoc();}
				void	setLocX(float inX)					{ loc.prev.x = loc.curr.x; loc.curr.x  = inX;		updateLoc();}
				void	setLocY(float inY)					{ loc.prev.y = loc.curr.y; loc.curr.y  = inY;		updateLoc();}
				void	setLocZ(float inZ)					{ loc.prev.z = loc.curr.z; loc.curr.z  = inZ;		updateLoc();}
				void	calcLoc(VEC3* inLoc)				{ loc.prev   = loc.curr;   loc.curr   += *inLoc;	updateLoc();}
				void	calcLocX(float inX)					{ loc.prev.x = loc.curr.x; loc.curr.x += inX;		updateLoc();}
				void	calcLocY(float inY)					{ loc.prev.y = loc.curr.y; loc.curr.y += inY;		updateLoc();}
				void	calcLocZ(float inZ)					{ loc.prev.z = loc.curr.z; loc.curr.z += inZ;		updateLoc();}
				MAT16*	getMatrixLocation(void)				{ return	&loc.matrix;										}

			//==================================================================
			//	移動の動作
			//==================================================================
				void	move(void);
				void	move(LINE inLine)					{ loc.line	= inLine;								move();		}
				void	moveUp(void)						{ loc.line	= LINE(eUD_UP,   eLR_NO,    eFR_NO);	move();		}
				void	moveDown(void)						{ loc.line	= LINE(eUD_DOWN, eLR_NO,    eFR_NO);	move();		}
				void	moveRight(void)						{ loc.line	= LINE(eUD_NO,   eLR_RIGHT, eFR_NO);	move();		}
				void	moveLeft(void)						{ loc.line	= LINE(eUD_NO,   eLR_LEFT,  eFR_NO);	move();		}
				void	moveFront(void)						{ loc.line	= LINE(eUD_NO,   eLR_NO,    eFR_FRONT);	move();		}
				void	moveRear(void)						{ loc.line	= LINE(eUD_NO,   eLR_NO,    eFR_REAR);	move();		}
				void	moveAt(VEC3	*inLoc)					{ setLoc(inLoc);												}
				void	moveValue(VEC3 *inValue)			{ loc.prev  = loc.curr;   loc.curr += *inValue;	updateLoc();	}

				void	moveUpDown(LINE_UPDOWN, float inMoveQuantity);				//!< 上下方向、移動値指定の移動処理
				void	moveLeftRight(LINE_LEFTRIGHT, float inMoveQuantity);		//!< 左右方向、移動値指定の移動処理
				void	moveFrontRear(LINE_FRONTREAR, float inMoveQuantity);		//!< 前後方向、移動値指定の移動処理

				VEC3*	calcNextLoc(float inMoveQuantity, LINE* inLine = NULL);		//!< 次の移動位置を受け取る
				VEC3	calcRay(LINE* inLine = NULL);								//!< レイの方向を受け取る

				VEC3	getNextLoc(float inMoveQuantity, LINE* inLine = NULL);
				VEC3	getNextLocUp(float inMoveQuantity)		{ return getNextLoc(inMoveQuantity, &LINE(eUD_UP,   eLR_NO,    eFR_NO));	}
				VEC3	getNextLocDown(float inMoveQuantity)	{ return getNextLoc(inMoveQuantity, &LINE(eUD_DOWN, eLR_NO,    eFR_NO));	}
				VEC3	getNextLocRight(float inMoveQuantity)	{ return getNextLoc(inMoveQuantity, &LINE(eUD_NO,   eLR_RIGHT, eFR_NO));	}
				VEC3	getNextLocLeft(float inMoveQuantity)	{ return getNextLoc(inMoveQuantity, &LINE(eUD_NO,   eLR_LEFT,  eFR_NO));	}
				VEC3	getNextLocFront(float inMoveQuantity)	{ return getNextLoc(inMoveQuantity, &LINE(eUD_NO,   eLR_NO,    eFR_FRONT));	}
				VEC3	getNextLocRear(float inMoveQuantity)	{ return getNextLoc(inMoveQuantity, &LINE(eUD_NO,   eLR_NO,    eFR_REAR));	}

			//==================================================================
			//	access	移動位置
			//==================================================================
				LINE_UPDOWN		getUpDown(void)const				{ return	loc.line.ud;			}
				LINE_LEFTRIGHT	getLeftRight(void)const				{ return	loc.line.lr;			}
				LINE_FRONTREAR	getFrontRear(void)const				{ return	loc.line.fr;			}
				LINE*			getLine(void)						{ return	&loc.line;				}
				void			setLine(LINE_UPDOWN inUD)			{ loc.line.ud	= inUD;				}
				void			setLine(LINE_LEFTRIGHT inLR)		{ loc.line.lr	= inLR;				}
				void			setLine(LINE_FRONTREAR inFR)		{ loc.line.fr	= inFR;				}
				void			setLine(LINE* inLine)				{ loc.line		= *inLine;			}

			//==================================================================
			//	access	角度(x,y,z)
			//==================================================================
				VEC3	getAngle(void)const					{ return	rot.angle;						}
				float	getAngleX(void)const				{ return	rot.angle.x;					}
				float	getAngleY(void)const				{ return	rot.angle.y;					}
				float	getAngleZ(void)const				{ return	rot.angle.z;					}
				void	setAngle(VEC3* inAngle)				{ rot.angle    = *inAngle;	updateRotat();	}
				void	setAngleX(float inX)				{ rot.angle.x  = inX;		updateRotat();	}
				void	setAngleY(float inY)				{ rot.angle.y  = inY;		updateRotat();	}
				void	setAngleZ(float inZ)				{ rot.angle.z  = inZ;		updateRotat();	}
				void	calcAngle(VEC3* inAngle)			{ rot.angle   += *inAngle;	updateRotat();	}
				void	calcAngleX(float inX)				{ rot.angle.x += inX;		updateRotat();	}
				void	calcAngleY(float inY)				{ rot.angle.y += inY;		updateRotat();	}
				void	calcAngleZ(float inZ)				{ rot.angle.z += inZ;		updateRotat();	}

			//==================================================================
			//	access	毎分回転数
			//==================================================================
				VEC3	getRpm(void)const					{ return	rot.rpm;						}
				float	getRpmX(void)const					{ return	rot.rpm.x;						}
				float	getRpmY(void)const					{ return	rot.rpm.y;						}
				float	getRpmZ(void)const					{ return	rot.rpm.z;						}
				void	setRpm(VEC3* inRpm)					{ rot.rpm    = *inRpm;						}
				void	setRpmX(float inX)					{ rot.rpm.x  = inX;							}
				void	setRpmY(float inY)					{ rot.rpm.y  = inY;							}
				void	setRpmZ(float inZ)					{ rot.rpm.z  = inZ;							}
				void	calcRpm(VEC3* inRpm)				{ rot.rpm   += *inRpm;						}
				void	calcRpmX(float inX)					{ rot.rpm.x += inX;							}
				void	calcRpmY(float inY)					{ rot.rpm.y += inY;							}
				void	calcRpmZ(float inZ)					{ rot.rpm.z += inZ;							}

			//==================================================================
			//	motion	角度
			//==================================================================
				void	lookAtX(float inX)					{ rot.angle.x  = inX;		updateRotat();	}
				void	lookAtY(float inY)					{ rot.angle.y  = inY;		updateRotat();	}
				void	lookAtZ(float inZ)					{ rot.angle.z  = inZ;		updateRotat();	}
				void	rotateUp(void)						{ rot.angle.x += rot.rpm.x;	updateRotat();	}	//!< 上を向く
				void	rotateDown(void)					{ rot.angle.x -= rot.rpm.x;	updateRotat();	}	//!< 下を向く
				void	rotateLeft(void)					{ rot.angle.y += rot.rpm.y;	updateRotat();	}	//!< 左を向く
				void	rotateRight(void)					{ rot.angle.y -= rot.rpm.y;	updateRotat();	}	//!< 右を向く
				void	rotateRHanded(void)					{ rot.angle.z += rot.rpm.z;	updateRotat();	}	//!< 右回転
				void	rotateLHanded(void)					{ rot.angle.z -= rot.rpm.z;	updateRotat();	}	//!< 左回転

			//==================================================================
			//	行列全てを更新する
			//==================================================================
				void	update(void);

			//==================================================================
			//	反転（モデルの正面を反転させる）
			//==================================================================
				void	zReverse(void)						{ D3DXMatrixRotationY(&matWorldIV, D3DXToRadian(180.0f));	}
		
			//==================================================================
			//	access	カメラとの距離
			//==================================================================
				float	getOffsetFromCamera(void)			{ return	fOffsetFromCamera;				}
				void	setOffsetFromCamera(float inDist)	{ fOffsetFromCamera = inDist;				}

			//==================================================================
			//	access	仮半径
			//==================================================================
				float	getTemporaryRadius(void)			{ return	fTemporaryRadius;				}
				void	setTemporaryRadius(float inRad)		{ fTemporaryRadius = inRad;					}

			//==================================================================
			//	access	衝突物体間隔
			//==================================================================
				float	getDist(void)						{ return	fDist;							}
				void	setDist(float inDist)				{ fDist	= inDist;							}

			//==================================================================
			//	access	メッセージ
			//==================================================================
				ORDER_MESSAGE	getOrderMessage(void)					{ return	orderMessage;		}
				THIS_MESSAGE	getThisMessage(void)					{ return	thisMessage;		}
				void			setOrderMessage(ORDER_MESSAGE inMessage){ orderMessage	= inMessage;	}
				void			setThisMessage(THIS_MESSAGE inMessage)	{ thisMessage	= inMessage;	}

			//==================================================================
			//	リアクション(正式なアクションをするサブクラスでオーバーライド)
			//==================================================================
		virtual	void	reactionMessage(REACTION_MESSAGE inMessage)	{ }

			//==================================================================
			//	ワールド座標の中心を返す(正式な中心を生成するサブクラスでオーバーライド)
			//==================================================================
		virtual	VEC3	getWorldCenter(void)				{ return	loc.curr;						}

			//==================================================================
			//	ワールド座標の半径を返す(正式な半径を生成するサブクラスでオーバーライド)
			//==================================================================
		virtual	float	getWorldRadius(void)				{ return	fTemporaryRadius * getScale();	}

			//==================================================================
			//	カラーのセット(カラーの設定が出来るサブクラスでオーバーライド)
			//==================================================================
		virtual	void	setColor(D3DCOLOR inColor)			{ }
		
			//==================================================================
			//	access	デフューズアルファ変動値
			//==================================================================
				float	getDiffuseAlpha(void)const			{ return	fDiffuseAlpha;					}
				void	setDiffuseAlpha(float inAlpha)		{ fDiffuseAlpha	= inAlpha;					}
				void	calcDiffuseAlpha(float inAlpha);

			//==================================================================
			//	半径で当たり判定
			//==================================================================
				BOOL	intersectRadius(C3D_OBJECT* inObject);
				BOOL	intersectRadius(C3D_OBJECT* inObject, float inRadius);

			//==================================================================
			//	オブジェクトの方向を向く
			//==================================================================
				void	angleToObject(C3D_OBJECT* inObject);

			//==================================================================
			//	全方向、上下、左右、前後で、当たり判定(サブクラスでオーバーライド)
			//==================================================================
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE* inLine)				{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown)		{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight)	{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear)	{ return	intersectRadius(inObject);	}

			//==================================================================
			//	全方向、上下、左右、前後に、指定した距離で当たり判定(サブクラスでオーバーライド)
			//==================================================================
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE* inLine, float inIntersectValue)				{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown, float inIntersectValue)		{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight, float inIntersectValue)	{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear, float inIntersectValue)	{ return	intersectRadius(inObject);	}

			//==================================================================
			//	全方向、上下、左右、前後に、距離と判定開始位置を指定して、当たり判定(サブクラスでオーバーライド)
			//==================================================================
		virtual	BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE* inLine, float inIntersectValue)				{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE_UPDOWN inUpDown, float inIntersectValue)		{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE_LEFTRIGHT inLeftRight, float inIntersectValue)	{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE_FRONTREAR inFrontRear, float inIntersectValue)	{ return	intersectRadius(inObject);	}

			//==================================================================
			//	コピー生成(サブクラスでオーバーライド)
			//==================================================================
		virtual	void	createCopy(C3D_OBJECT** inCopy, VEC3* inLoc)				{ *inCopy	= NULL;	}
		virtual	void	createCopy(C3D_OBJECT** inCopy, VEC3* inLoc, VEC3* inAngle)	{ *inCopy	= NULL;	}

			//==================================================================
			//	描画(サブクラスでオーバーライド)
			//==================================================================
		virtual	BOOL	draw(CONST DEV		inDev)		{ return	FALSE;	}
		virtual	BOOL	draw(CZTEX_SHA*		inZTexSha)	{ return	FALSE;	}
		virtual	BOOL	draw(CDEPTH_SHA*	inDepthSha)	{ return	FALSE;	}
		virtual	BOOL	draw(CONST SPRITE	inSprite)	{ return	FALSE;	}
	};
}
//-----------------------------------------------------------------------
//!	3Dオブジェクトの名前空間
//-----------------------------------------------------------------------