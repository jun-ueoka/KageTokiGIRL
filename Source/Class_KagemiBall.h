//***********************************************************************************
//!	�t�@�C����		Class_KagemiBall.h
//
//	�@�\			�J�Q�~�����A�ʂ������N���X�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/12/11	<�V�K>
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
	//	�J�Q�~�{�[���̎�ސ���
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	KAGEMI_BALL_KIND
	{
		eKMIBALL_ENERGY,					//!< �G�i�W�[�{�[��
		eKMIBALL_MAX,						//!< ��ނ̍ő吔
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CKAGEMI_BALL
	//
	//	�@�\			�J�Q�~�����A�ʂ������N���X
	//	�@�\�T�v		���̋ʂɓ����������̃��b�Z�[�W��ύX����
	//					[judgMessage]�Ɠ������b�Z�[�W�������ɏՓ˂���ƁA
	//					���̕���[reactMessage]�𑗂�
	//	�X�V			2008/12/11	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CKAGEMI_BALL : public CMESH
	{
				#define		ENERGY_BALL			"energy_ball"
				#define		EFFECT_ENERGY		"energy_ball"
				#define		EFFECT_NAME_MAX		15

	private:		//-------------------- private --------------------//

			//==================================================================
			//	�J�Q�~�{�[���̎�ނɂŁA��������
			//==================================================================
				LPSTR				getBallTextureName(KAGEMI_BALL_KIND inKind);

				BOOL				activate;						//!< ���ˊm�F(TRUE = ���ˍς�)
				THIS_MESSAGE		judgMessage;					//!< �������ʃ��b�Z�[�W
				REACTION_MESSAGE	reactMessage;					//!< ���A�N�V�������b�Z�[�W
				C3D_OBJECT_GROUP*	stageG;							//!< �X�e�[�W�O���[�v
				C3D_OBJECT_GROUP*	enemyG;							//!< �G�O���[�v
				char				effectName[EFFECT_NAME_MAX];	//!< �����G�t�F�N�g��
				int					iEffectCnt;						//!< �G�t�F�N�g�̔����Ԋu
				int					iFrame;							//!< �`��t���[��
				D3DCOLOR			smokeColor;						//!< ���̐F

	public:			//-------------------- public --------------------//

				CKAGEMI_BALL(CONST DEV inDev, KAGEMI_BALL_KIND inKind);
				CKAGEMI_BALL(CKAGEMI_BALL* inCopy);
		virtual	~CKAGEMI_BALL(void);

			//==================================================================
			//	access	�Փ˔���m�F
			//==================================================================
				BOOL	getActivate(void)const					{ return	activate;			}

			//==================================================================
			//	access	���b�Z�[�W
			//==================================================================
				THIS_MESSAGE		getJudgMessage(void)const				{ return	judgMessage;		}
				REACTION_MESSAGE	getReactMessage(void)const				{ return	reactMessage;		}
				void				setJudgMessage(THIS_MESSAGE inMsg)		{ judgMessage	= inMsg;		}
				void				setReactMessage(REACTION_MESSAGE inMsg)	{ reactMessage	= inMsg;		}

			//==================================================================
			//	access	�����G�t�F�N�g�֘A
			//==================================================================
				LPSTR	getEffectName(void)						{ return	effectName;			}
				int		getEffectCnt(void)const					{ return	iEffectCnt;			}

			//==================================================================
			//	access	���̐F
			//==================================================================
				D3DCOLOR	getSmokeColor(void)const			{ return	smokeColor;			}
				void		getSmokeColor(D3DCOLOR inColor)		{ smokeColor	= inColor;		}

			//==================================================================
			//	����(����������Ɏg�p)
			//==================================================================
				void	shot(VEC3* inAngle, VEC3* inLoc, C3D_OBJECT_GROUP* inStageG,
							 C3D_OBJECT_GROUP* inEnemyG, int inFrame);

			//==================================================================
			//	�Փ˔���
			//==================================================================
				void	intersectG(void);

			//==================================================================
			//	�`��(�T�u�N���X�ŃI�[�o�[���C�h�\)
			//==================================================================
		virtual	BOOL	draw(CONST DEV   inDev);
		virtual	BOOL	draw(CZTEX_SHA*  inZTexSha)	{ return	TRUE;	}
		virtual	BOOL	draw(CDEPTH_SHA* inDepthSha);
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------