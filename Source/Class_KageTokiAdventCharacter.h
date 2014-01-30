//***********************************************************************************
//!	�t�@�C����		Class_KageTokiAdventCharacter.h
//
//	�@�\			�A�h�x���`���[�Q�[�����[�h�̃L�����N�^�[�N���X
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2008/12/09	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_DirectInput.h"			//!< ���͏��
#include	"Class_SkinMesh.h"				//!< 3D�I�u�W�F�N�g�X�L�����b�V��
#include	"Class_3DObjectGroup.h"			//!< 3D�I�u�W�F�N�g�O���[�v

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	��p�}�N��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		ADVENT_LEFT				MYKEY::eLEFT	//!< ���ړ�
	#define		ADVENT_RIGHT			MYKEY::eRIGHT	//!< �E�ړ�
	#define		ADVENT_TOP				MYKEY::eUP		//!< ���@�����
	#define		ADVENT_BOTTOM			MYKEY::eDOWN	//!< ���@������
	#define		ADVENT_JUMP				MYKEY::eMIDDLE	//!< �W�����v
	#define		ADVENT_JUMP_HEIGHT		55.0f			//!< �W�����v�̍���

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CKAGETOKI_ADVENT_CHARACTER
	//
	//	�@�\			�A�h�x���`���[�Q�[�����[�h�̃L�����N�^�[�N���X
	//	�X�V			2008/12/09	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CKAGETOKI_ADVENT_CHARACTER : public CSKIN_MESH
	{
			//==================================================================
			//	�L�����N�^�[�̌����̋�������
			//==================================================================
				#define	ADVENT_LEFTANGLE	 90.0f				//!< �������̊p�x
				#define	ADVENT_RIGHTANGLE	270.0f				//!< �E�����̊p�x
				#define	ADVENT_FRONTHIT		eFR_FRONT			//!< �O���ւ̏Փ˔������
				#define	ADVENT_REARHIT		eFR_REAR			//!< ����ւ̏Փ˔������
				#define	ADVENT_FRONTMOVE	this->moveFront()	//!< �O���ړ����̖���
				#define	ADVENT_REALMOVE		this->moveRear()	//!< �O���ړ����̖���

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	�L�����N�^�[�̌����̗�
			//==================================================================
				enum	ADVENT_CHARA_WAY
				{
					eADCW_LEFT,								//!< ������
					eADCW_LEFT_TURN,						//!< ���Ɍ����r��
					eADCW_RIGHT,							//!< �E����
					eADCW_RIGHT_TURN,						//!< �E�Ɍ����r��
					eADCW_NO,								//!< �������
				};

			//==================================================================
			//	�ړ����
			//==================================================================
				float	fUpDownSpeed;						//!< �㉺�ړ����x
				float	fMaxUpSpeed;						//!< �ő�㏸���x
				float	fMaxDownSpeed;						//!< �ő剺�~���x
				float	fUpDownAccelerate;					//!< �㉺�ړ������l
				float	fUpDownSlowDown;					//!< �㉺�ړ������l

				LINE	UDLine;								//!< ����Ώۂ̏㉺�������
				LINE	LRLine;								//!< ����Ώۂ̍��E�������
				LINE	FRLine;								//!< ����Ώۂ̑O�㔻�����

				float	fFloorSpace;						//!< ���Ƃ̊Ԋu
				float	fWallSpace;							//!< �ǂƂ̊Ԋu
				bool	bFlighting;							//!< �󒆊m�F
				bool	bHeightValid;						//!< �����L���m�F
				bool	bWidthValid;						//!< �����L���m�F

				float	fHeightValue;						//!< ����
				float	fWidthValue;						//!< ����

				ADVENT_CHARA_WAY	way;					//!< �L�����N�^�[�̌���
				int		iFrameCnt;							//!< �t���[���J�E���^(���l)

				float	fJumpHeight;						//!< �W�����v���x
				float	fLeftRightMoveSpeed;				//!< ���E�ړ��X�s�[�h
				float	fLeftRightMoveMaxSpeed;				//!< ���E�ړ��ő�X�s�[�h
				float	fLeftRightMoveAccele;				//!< ���E�ړ������l

				BOOL	bDamage;							//!< �_���[�W�m�F
				int		iDamage;							//!< �_���[�W�l

			//==================================================================
			//	����O���[�v(�����͑S�ĊO���̕�)
			//==================================================================
				C3D_OBJECT_GROUP*	viewG;					//!< �`��I�u�W�F�N�g�O���[�v�|�C���^�[
				C3D_OBJECT_GROUP*	stageG;					//!< �X�e�[�W�I�u�W�F�N�g�O���[�v�|�C���^�[
				C3D_OBJECT_GROUP*	enemyG;					//!< �G�I�u�W�F�N�g�O���[�v�|�C���^�[

	public:			//-------------------- public --------------------//

				CKAGETOKI_ADVENT_CHARACTER(
					CONST DEV	inDev,
					CONST LPSTR	inName,
					CONST LPSTR	inResource = PATH_SKINMESH,
					CONST LPSTR	inTexture  = PATH_SKINMESHTEX);	//!< �R���X�g���N�^
		virtual	~CKAGETOKI_ADVENT_CHARACTER(void);				//!< �f�X�g���N�^
		
			//==================================================================
			//	��􏈗�
			//==================================================================
				void	moveJumping(float inValue);

			//==================================================================
			//	access	�����L���m�F
			//==================================================================
				void	setHeightValid(bool inBoolen)			{ bHeightValid	= inBoolen;		}

			//==================================================================
			//	access	�����L���m�F
			//==================================================================
				void	setWidthValid(bool inBoolen)			{ bWidthValid	= inBoolen;		}

			//==================================================================
			//	access	�ړ����x
			//==================================================================
				float	getUpDownSpeed(void)const				{ return	fUpDownSpeed;		}
				float	getIntegralDownSpeed(void)				//!< ���������ēn����
				{
					return	(fUpDownSpeed > 0.0f ? fUpDownSpeed : -fUpDownSpeed);
				}

			//==================================================================
			//	access	�㉺�ړ����
			//==================================================================
				float	getMaxUpSpeed(void)const				{ return	fMaxUpSpeed;		}
				float	getMaxDownSpeed(void)const				{ return	fMaxDownSpeed;		}
				float	getUpAccelerate(void)const				{ return	fUpDownAccelerate;	}
				float	getUpSlowDown(void)const				{ return	fUpDownSlowDown;	}
				void	setMaxUpSpeed(float inValue)			{ fMaxUpSpeed		= inValue;	}
				void	setMaxDownSpeed(float inValue)			{ fMaxDownSpeed		= inValue;	}
				void	setUpAccelerate(float inValue)			{ fUpDownAccelerate	= inValue;	}
				void	setUpSlowDown(float inValue)			{ fUpDownSlowDown	= inValue;	}

			//==================================================================
			//	access	���֌W
			//==================================================================
				float	getFloorSpace(void)const				{ return	fFloorSpace;		}
				void	setFloorSpace(float inValue)			{ fFloorSpace	= inValue;		}

			//==================================================================
			//	access	�Ǌ֌W
			//==================================================================
				float	getWallSpace(void)const					{ return	fWallSpace;			}
				void	setWallSpace(float inValue)				{ fWallSpace	= inValue;		}

			//==================================================================
			//	access	��s�m�F
			//==================================================================
				bool	getFlighting(void)const					{ return	bFlighting;			}

			//==================================================================
			//	access	�������
			//==================================================================
				float	getHeightValue(void)const				{ return	fHeightValue;		}
				void	setHeightValue(float inValue)			{ fHeightValue	= inValue;		}

			//==================================================================
			//	access	�������
			//==================================================================
				float	getWidthValue(void)const				{ return	fWidthValue;		}
				void	setWidthValue(float inValue)			{ fWidthValue	= inValue;		}

			//==================================================================
			//	access	�_���[�W�m�F
			//==================================================================
				BOOL	getDamageBoolen(void)const				{ return	bDamage;			}
				void	setDamageBoolen(BOOL inBoolen)			{ bDamage	= inBoolen;			}
				int		getDamageValue(void)const				{ return	iDamage;			}

			//==================================================================
			//	access	�O���[�v
			//==================================================================
				void	setViewG(C3D_OBJECT_GROUP* inG)			{ viewG		= inG;				}
				void	setStageG(C3D_OBJECT_GROUP* inG)		{ stageG	= inG;				}
				void	setEnemyG(C3D_OBJECT_GROUP* inG)		{ enemyG	= inG;				}

			//==================================================================
			//	����
			//==================================================================
				void	opeAdvent(CINPUT* input);				//!< �A�h�x���`���[����
				void	opeEffectGravity(void);					//!< �d�͑���
				void	opeLocShift(void);						//!< �ʒu���킹����

			//==================================================================
			//	�����̏�����	
			//==================================================================
				void	resetWay(void)							{ way	= eADCW_NO;				}

			//==================================================================
			//	��{����(�T�u�N���X�ŃI�[�o�[���C�h)
			//==================================================================
		virtual	void	operation(CINPUT* input);
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------