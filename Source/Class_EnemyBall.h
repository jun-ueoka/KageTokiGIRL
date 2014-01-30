//***********************************************************************************
//!	�t�@�C����		Class_EnemyBall.h
//
//	�@�\			�G�����ʂ������N���X�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/02/17	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_Mesh.h"					//!< 3D�I�u�W�F�N�g���b�V��
#include	"Class_3DObjectGroup.h"			//!< 3D�I�u�W�F�N�g�O���[�v
#include	"Class_3DEffectCenter.h"		//!< �G�t�F�N�g����

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�G�l�~�[�{�[���̎�ސ���
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	ENEMY_BALL_KIND
	{
		eENEMYB_STRAIGHT,					//!< �꒼���ɔ��ōs��
		eENEMYB_SHADOLL_ARROW,				//!< �|��̗l�ɔ��ōs��(�V���h�[����p)
		eENEMYB_MAX,						//!< ��ނ̍ő吔
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CENEMY_BALL
	//
	//	�@�\			�G�����ʂ������N���X
	//	�X�V			2008/02/17	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CENEMY_BALL : public CMESH
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	�G�l�~�[�{�[���̎�ނɂŁA��������
			//==================================================================
				BOOL							activate;			//!< ���ˊm�F(TRUE = ���ˍς�)
				C3D_OBJECT_GROUP*				stageG;				//!< �X�e�[�W�O���[�v
				CKAGETOKI_ADVENT_CHARACTER**	player;				//!< �v���C���[
				ENEMY_BALL_KIND					ballKind;			//!< �{�[���̎��
				REACTION_MESSAGE				reactMessage;		//!< ���A�N�V�������b�Z�[�W

				char				effectName[EFFECT_NAME_MAX];	//!< �����G�t�F�N�g��
				int					iEffectCnt;						//!< �G�t�F�N�g�̔����Ԋu
				int					iFrame;							//!< �`��t���[��
				D3DCOLOR			effectColor;					//!< �G�t�F�N�g�̐F

	public:			//-------------------- public --------------------//

				CENEMY_BALL(CONST DEV inDev, ENEMY_BALL_KIND inKind, 
							LPSTR inMeshName, LPSTR inEffectName);
				CENEMY_BALL(CENEMY_BALL* inCopy);
		virtual	~CENEMY_BALL(void);

			//==================================================================
			//	access	�Փ˔���m�F
			//==================================================================
				BOOL	getActivate(void)const					{ return	activate;			}

			//==================================================================
			//	access	�����G�t�F�N�g�֘A
			//==================================================================
				LPSTR	getEffectName(void)						{ return	effectName;			}
				int		getEffectCnt(void)const					{ return	iEffectCnt;			}
				void	setEffectCnt(int inValue)				{ iEffectCnt	= inValue;		}

			//==================================================================
			//	access	�G�t�F�N�g�̐F
			//==================================================================
				D3DCOLOR	getEffectColor(void)const			{ return	effectColor;		}
				void		setEffectColor(D3DCOLOR inColor)	{ effectColor	= inColor;		}

			//==================================================================
			//	access	�{�[���̎��
			//==================================================================
				ENEMY_BALL_KIND	getBallKind(void)const				{ return	ballKind;		}
				void			setBallKind(ENEMY_BALL_KIND inKind)	{ ballKind	= inKind;		}

			//==================================================================
			//	access	���b�Z�[�W
			//==================================================================
				REACTION_MESSAGE	getReactMessage(void)const				{ return	reactMessage;		}
				void				setReactMessage(REACTION_MESSAGE inMsg)	{ reactMessage	= inMsg;		}

			//==================================================================
			//	����(����������Ɏg�p)
			//==================================================================
				void	shot(VEC3* inAngle, VEC3* inLoc, C3D_OBJECT_GROUP* inStageG,
							 CKAGETOKI_ADVENT_CHARACTER** inPlayer, int inFrame);

			//==================================================================
			//	�Փ˔���
			//==================================================================
				void	intersectG(void);

			//==================================================================
			//	�{�[���̎�ޖ��̕��򏈗�
			//==================================================================
				void	opeStraight(CONST DEV inDev);		//!< �꒼���ɔ��ł�������
				void	opeShadollArrow(CONST DEV inDev);	//!< �|��̗l�ɔ��ł�������

			//==================================================================
			//	�`��(�T�u�N���X�ŃI�[�o�[���C�h�\)
			//==================================================================
				BOOL	draw(CONST DEV   inDev);
				BOOL	draw(CZTEX_SHA*  inZTexSha)	{ return	TRUE;	}
				BOOL	draw(CDEPTH_SHA* inDepthSha);
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------