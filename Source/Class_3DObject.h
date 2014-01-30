//***********************************************************************************
//!	�t�@�C����		Class_3DObject.h
//
//	�@�\			���̃v���O�����Ŏg�p����O�����̕��̂Ɋւ����`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2007/12/08	<�V�K>
//					2008/06/24	<�ǉ�>	�S�̓I�ɃR�����g�̋���
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"DirectX.h"					//!< DirectX�g�p
#include	"Class_ZTextureShader.h"	//!< Z�e�N�X�`���V�F�[�_
#include	"Class_DepthBufShader.h"	//!< �[�x�o�b�t�@�V�F�[�_

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�p�x��͈͓��Ɏ��߂�
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
	//!	�㉺
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum LINE_UPDOWN
	{
		eUD_UP		=  1,	//!< ��
		eUD_NO		=  0,	//!< �Ȃ�
		eUD_DOWN	= -1	//!< ��
	};
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	���E
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum LINE_LEFTRIGHT
	{
		eLR_RIGHT	=  1,	//!< �E
		eLR_NO		=  0,	//!< �Ȃ�
		eLR_LEFT	= -1	//!< ��
	};
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�O��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum LINE_FRONTREAR
	{
		eFR_FRONT	= -1,	//!< �O
		eFR_NO		=  0,	//!< �Ȃ�
		eFR_REAR	=  1	//!< ��
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�T�u���b�Z�[�W
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum ORDER_MESSAGE
	{
		eOMSG_NOMSG,			//!< ���b�Z�[�W�Ȃ�
		eOMSG_DELETE_IN,		//!< ���ŃO���[�v�ɓ����
		eOMSG_DELETE,			//!< ���Ŗ���
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	���ʃ��b�Z�[�W
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum THIS_MESSAGE
	{
		eTMSG_NOMSG,			//!< ���b�Z�[�W�Ȃ�
		eTMSG_ENEMY,			//!< �G
		eTMSG_NO_SEE_SHADOW,	//!< �����Ȃ��e
		eTMSG_SEE_SHADOW,		//!< ������e
		eTMSG_SHADOW_BALL,		//!< �V���h�E�{�[��
		eTMSG_SHINE_BALL,		//!< �V���C���{�[��
		eTMSG_ENERGY_BALL,		//!< �G�i�W�[�{�[��
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	���A�N�V�������b�Z�[�W
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	typedef	enum REACTION_MESSAGE
	{
		eREAMSG_NOMSG,				//!< ���b�Z�[�W�Ȃ�
		eREAMSG_SHADOW_HIT,			//!< �V���h�E�{�[���ɓ�������
		eREAMSG_SHINE_HIT,			//!< �V���C���{�[���ɓ�������
		eREAMSG_ENERGY_HIT,			//!< �G�i�W�[�{�[���ɓ�������
		eREAMSG_ENEMY_ATTACK_LV1,	//!< �G�̍U��Lv1
		eREAMSG_ENEMY_ATTACK_LV2,	//!< �G�̍U��Lv2
		eREAMSG_ENEMY_ATTACK_LV3,	//!< �G�̍U��Lv3
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	����
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	LINE
	{
		LINE_UPDOWN		ud;
		LINE_LEFTRIGHT	lr;
		LINE_FRONTREAR	fr;

	//	���̉�����
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
	//!	�p�x�Ɉ����\����
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	ROTATION
	{
		VEC3			xAxis;		//!< ���W�nX�̎���
		VEC3			yAxis;		//!< ���W�nY�̎���
		VEC3			zAxis;		//!< ���W�nZ�̎���
		VEC3			rpm;		//!< ������]��
		VEC3			angle;		//!< �p�x
		MAT16			matrix3D;	//!< 3D��]�s��
		MAT16			matrixX;	//!< X����]�s��
		MAT16			matrixY;	//!< Y����]�s��
		MAT16			matrixZ;	//!< Z����]�s��
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	���W�������\����
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct	TRANSLATION
	{
		LINE			line;		//!< �������_
		VEC3			prev;		//!< �O�̍��W
		VEC3			curr;		//!< ���݂̍��W
		VEC3			next;		//!< ���̍��W
		MAT16			matrix;		//!< �s��
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		C3D_OBJECT
	//
	//	�@�\			�O�����̕���
	//					�O�����̕��̂̋��ʂ̐������`���Ă���B�i���ۃN���X�j
	//	�X�V			2008/06/24	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	C3D_OBJECT
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	�I�u�W�F�N�g�X�e�[�^�X
			//==================================================================
				MAT16			matWorld;				//!< ���[���h�s��
				MAT16			matWorldIV;				//!< ���[���h�t�s��
				ROTATION		rot;					//!< �p�x���
				TRANSLATION		loc;					//!< ���W���
				BOOL			bVisible;				//!< �\���m�F
				float			fScale;					//!< �{��
				MAT16			matScale;				//!< �傫���̍s��
				float			fMoveQuantity;			//!< �ړ���
				float			fOffsetFromCamera;		//!< �J�����Ƃ̋���
				float			fTemporaryRadius;		//!< �����蔻��p���a
				float			fDist;					//!< �Փ˕��̊Ԋu
				ORDER_MESSAGE	orderMessage;			//!< ���߃��b�Z�[�W(�K�v�Ȏ�)
				THIS_MESSAGE	thisMessage;			//!< ���ʃ��b�Z�[�W(�K�v�Ȏ�)

			//==================================================================
			//	�`��X�e�[�^�X
			//==================================================================
				float			fDiffuseAlpha;			//!< �f�t���[�Y�A���t�@�ϓ��l

			//==================================================================
			//	�e�s����X�V����
			//==================================================================
				void	updateScale(void);				//!< �X�P�[�����O�s��
				void	updateRotat(void);				//!< ��]�s��
				void	updateLoc(void);				//!< ���W�s��
				void	updateWorld(void);				//!< ���[���h�s��

	public:			//-------------------- public --------------------//

				C3D_OBJECT(void)	{ initialize();	}	//!< �R���X�g���N�^
		virtual	~C3D_OBJECT(void)	{ finalize();	}	//!< �f�X�g���N�^
				void	initialize(void);				//!< ������
				void	finalize(void);					//!< ��n��

			//==================================================================
			//	access	�\���m�F
			//==================================================================
				BOOL	getVisible(void)const				{ return	bVisible;					}
				void	setVisible(BOOL	inVisible)			{ bVisible = inVisible;					}

			//==================================================================
			//	access	��]�s��
			//==================================================================
				MAT16*	getMatrixRotation(void)				{ return	&rot.matrix3D;				}
				MAT16*	getMatrixRotationX(void)			{ return	&rot.matrixX;				}
				MAT16*	getMatrixRotationY(void)			{ return	&rot.matrixY;				}
				MAT16*	getMatrixRotationZ(void)			{ return	&rot.matrixZ;				}
				void	setMatrixRotationX(MAT16* inX)		{ rot.matrixX = *inX;	updateRotat();	}
				void	setMatrixRotationY(MAT16* inY)		{ rot.matrixY = *inY;	updateRotat();	}
				void	setMatrixRotationZ(MAT16* inZ)		{ rot.matrixZ = *inZ;	updateRotat();	}

			//==================================================================
			//	access	���[���h�s��
			//==================================================================
				MAT16*	getWorldMatrix(void)				{ return	&matWorld;					}
				MAT16*	getWorldMatrixInversed(void)		{ return	&matWorldIV;				}

			//==================================================================
			//	���̃��[���h�t�s��̃x�N�g���̃g�����X�t�H�[�������߂�
			//==================================================================
				void	rayFromInversedMatrix(VEC3* inFrom, VEC3* inDir);

			//==================================================================
			//	access	�{��
			//==================================================================
				float	getScale(void)const					{ return	fScale;						}
				void	setScale(float inScale)				{ fScale = inScale;	updateScale();		}
				MAT16*	getMatrixScaling(void)				{ return	&matScale;					}

			//==================================================================
			//	access	�ړ���
			//==================================================================
				float	getMoveQuantity(void)const			{ return	fMoveQuantity;				}
				void	setMoveQuantity(float inMoveQuantity)
				{
				//	�ړ��ʂ́A���̐��ł����Ă͂Ȃ�Ȃ�
					fMoveQuantity = (inMoveQuantity >= 0.0f ? inMoveQuantity : -inMoveQuantity);
				}
				void	calcMoveQuantity(float inMoveQuantity)
				{
					fMoveQuantity +=  inMoveQuantity;
				//	�ړ��ʂ́A���̐��ł����Ă͂Ȃ�Ȃ�
					fMoveQuantity  = (fMoveQuantity >= 0.0f ? fMoveQuantity : 0.0f);
				}

			//==================================================================
			//	access	�ʒu
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
			//	�ړ��̓���
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

				void	moveUpDown(LINE_UPDOWN, float inMoveQuantity);				//!< �㉺�����A�ړ��l�w��̈ړ�����
				void	moveLeftRight(LINE_LEFTRIGHT, float inMoveQuantity);		//!< ���E�����A�ړ��l�w��̈ړ�����
				void	moveFrontRear(LINE_FRONTREAR, float inMoveQuantity);		//!< �O������A�ړ��l�w��̈ړ�����

				VEC3*	calcNextLoc(float inMoveQuantity, LINE* inLine = NULL);		//!< ���̈ړ��ʒu���󂯎��
				VEC3	calcRay(LINE* inLine = NULL);								//!< ���C�̕������󂯎��

				VEC3	getNextLoc(float inMoveQuantity, LINE* inLine = NULL);
				VEC3	getNextLocUp(float inMoveQuantity)		{ return getNextLoc(inMoveQuantity, &LINE(eUD_UP,   eLR_NO,    eFR_NO));	}
				VEC3	getNextLocDown(float inMoveQuantity)	{ return getNextLoc(inMoveQuantity, &LINE(eUD_DOWN, eLR_NO,    eFR_NO));	}
				VEC3	getNextLocRight(float inMoveQuantity)	{ return getNextLoc(inMoveQuantity, &LINE(eUD_NO,   eLR_RIGHT, eFR_NO));	}
				VEC3	getNextLocLeft(float inMoveQuantity)	{ return getNextLoc(inMoveQuantity, &LINE(eUD_NO,   eLR_LEFT,  eFR_NO));	}
				VEC3	getNextLocFront(float inMoveQuantity)	{ return getNextLoc(inMoveQuantity, &LINE(eUD_NO,   eLR_NO,    eFR_FRONT));	}
				VEC3	getNextLocRear(float inMoveQuantity)	{ return getNextLoc(inMoveQuantity, &LINE(eUD_NO,   eLR_NO,    eFR_REAR));	}

			//==================================================================
			//	access	�ړ��ʒu
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
			//	access	�p�x(x,y,z)
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
			//	access	������]��
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
			//	motion	�p�x
			//==================================================================
				void	lookAtX(float inX)					{ rot.angle.x  = inX;		updateRotat();	}
				void	lookAtY(float inY)					{ rot.angle.y  = inY;		updateRotat();	}
				void	lookAtZ(float inZ)					{ rot.angle.z  = inZ;		updateRotat();	}
				void	rotateUp(void)						{ rot.angle.x += rot.rpm.x;	updateRotat();	}	//!< �������
				void	rotateDown(void)					{ rot.angle.x -= rot.rpm.x;	updateRotat();	}	//!< ��������
				void	rotateLeft(void)					{ rot.angle.y += rot.rpm.y;	updateRotat();	}	//!< ��������
				void	rotateRight(void)					{ rot.angle.y -= rot.rpm.y;	updateRotat();	}	//!< �E������
				void	rotateRHanded(void)					{ rot.angle.z += rot.rpm.z;	updateRotat();	}	//!< �E��]
				void	rotateLHanded(void)					{ rot.angle.z -= rot.rpm.z;	updateRotat();	}	//!< ����]

			//==================================================================
			//	�s��S�Ă��X�V����
			//==================================================================
				void	update(void);

			//==================================================================
			//	���]�i���f���̐��ʂ𔽓]������j
			//==================================================================
				void	zReverse(void)						{ D3DXMatrixRotationY(&matWorldIV, D3DXToRadian(180.0f));	}
		
			//==================================================================
			//	access	�J�����Ƃ̋���
			//==================================================================
				float	getOffsetFromCamera(void)			{ return	fOffsetFromCamera;				}
				void	setOffsetFromCamera(float inDist)	{ fOffsetFromCamera = inDist;				}

			//==================================================================
			//	access	�����a
			//==================================================================
				float	getTemporaryRadius(void)			{ return	fTemporaryRadius;				}
				void	setTemporaryRadius(float inRad)		{ fTemporaryRadius = inRad;					}

			//==================================================================
			//	access	�Փ˕��̊Ԋu
			//==================================================================
				float	getDist(void)						{ return	fDist;							}
				void	setDist(float inDist)				{ fDist	= inDist;							}

			//==================================================================
			//	access	���b�Z�[�W
			//==================================================================
				ORDER_MESSAGE	getOrderMessage(void)					{ return	orderMessage;		}
				THIS_MESSAGE	getThisMessage(void)					{ return	thisMessage;		}
				void			setOrderMessage(ORDER_MESSAGE inMessage){ orderMessage	= inMessage;	}
				void			setThisMessage(THIS_MESSAGE inMessage)	{ thisMessage	= inMessage;	}

			//==================================================================
			//	���A�N�V����(�����ȃA�N�V����������T�u�N���X�ŃI�[�o�[���C�h)
			//==================================================================
		virtual	void	reactionMessage(REACTION_MESSAGE inMessage)	{ }

			//==================================================================
			//	���[���h���W�̒��S��Ԃ�(�����Ȓ��S�𐶐�����T�u�N���X�ŃI�[�o�[���C�h)
			//==================================================================
		virtual	VEC3	getWorldCenter(void)				{ return	loc.curr;						}

			//==================================================================
			//	���[���h���W�̔��a��Ԃ�(�����Ȕ��a�𐶐�����T�u�N���X�ŃI�[�o�[���C�h)
			//==================================================================
		virtual	float	getWorldRadius(void)				{ return	fTemporaryRadius * getScale();	}

			//==================================================================
			//	�J���[�̃Z�b�g(�J���[�̐ݒ肪�o����T�u�N���X�ŃI�[�o�[���C�h)
			//==================================================================
		virtual	void	setColor(D3DCOLOR inColor)			{ }
		
			//==================================================================
			//	access	�f�t���[�Y�A���t�@�ϓ��l
			//==================================================================
				float	getDiffuseAlpha(void)const			{ return	fDiffuseAlpha;					}
				void	setDiffuseAlpha(float inAlpha)		{ fDiffuseAlpha	= inAlpha;					}
				void	calcDiffuseAlpha(float inAlpha);

			//==================================================================
			//	���a�œ����蔻��
			//==================================================================
				BOOL	intersectRadius(C3D_OBJECT* inObject);
				BOOL	intersectRadius(C3D_OBJECT* inObject, float inRadius);

			//==================================================================
			//	�I�u�W�F�N�g�̕���������
			//==================================================================
				void	angleToObject(C3D_OBJECT* inObject);

			//==================================================================
			//	�S�����A�㉺�A���E�A�O��ŁA�����蔻��(�T�u�N���X�ŃI�[�o�[���C�h)
			//==================================================================
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE* inLine)				{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown)		{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight)	{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear)	{ return	intersectRadius(inObject);	}

			//==================================================================
			//	�S�����A�㉺�A���E�A�O��ɁA�w�肵�������œ����蔻��(�T�u�N���X�ŃI�[�o�[���C�h)
			//==================================================================
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE* inLine, float inIntersectValue)				{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_UPDOWN inUpDown, float inIntersectValue)		{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_LEFTRIGHT inLeftRight, float inIntersectValue)	{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersect(C3D_OBJECT* inObject, LINE_FRONTREAR inFrontRear, float inIntersectValue)	{ return	intersectRadius(inObject);	}

			//==================================================================
			//	�S�����A�㉺�A���E�A�O��ɁA�����Ɣ���J�n�ʒu���w�肵�āA�����蔻��(�T�u�N���X�ŃI�[�o�[���C�h)
			//==================================================================
		virtual	BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE* inLine, float inIntersectValue)				{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE_UPDOWN inUpDown, float inIntersectValue)		{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE_LEFTRIGHT inLeftRight, float inIntersectValue)	{ return	intersectRadius(inObject);	}
		virtual	BOOL	intersectPos(C3D_OBJECT* inObject, VEC3* inLoc, LINE_FRONTREAR inFrontRear, float inIntersectValue)	{ return	intersectRadius(inObject);	}

			//==================================================================
			//	�R�s�[����(�T�u�N���X�ŃI�[�o�[���C�h)
			//==================================================================
		virtual	void	createCopy(C3D_OBJECT** inCopy, VEC3* inLoc)				{ *inCopy	= NULL;	}
		virtual	void	createCopy(C3D_OBJECT** inCopy, VEC3* inLoc, VEC3* inAngle)	{ *inCopy	= NULL;	}

			//==================================================================
			//	�`��(�T�u�N���X�ŃI�[�o�[���C�h)
			//==================================================================
		virtual	BOOL	draw(CONST DEV		inDev)		{ return	FALSE;	}
		virtual	BOOL	draw(CZTEX_SHA*		inZTexSha)	{ return	FALSE;	}
		virtual	BOOL	draw(CDEPTH_SHA*	inDepthSha)	{ return	FALSE;	}
		virtual	BOOL	draw(CONST SPRITE	inSprite)	{ return	FALSE;	}
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------