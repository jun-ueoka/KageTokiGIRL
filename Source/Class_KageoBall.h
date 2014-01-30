//***********************************************************************************
//!	�t�@�C����		Class_KageoBall.h
//
//	�@�\			�J�Q�I�����A�ʂ������N���X�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/08/25	<�V�K>
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
	//	�J�Q�I�{�[���̎�ސ���
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	KAGEO_BALL_KIND
	{
		eKOBALL_SHADOW,						//!< �V���h�E�{�[��
		eKOBALL_SHINE,						//!< �V���C���{�[��
		eKOBALL_MAX,						//!< ��ނ̍ő吔
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CKAGEO_BALL
	//
	//	�@�\			�J�Q�I�����A�ʂ������N���X
	//	�@�\�T�v		���̋ʂɓ����������̃��b�Z�[�W��ύX����
	//					[judgMessage]�Ɠ������b�Z�[�W�������ɏՓ˂���ƁA
	//					���̕���[reactMessage]�𑗂�
	//	�X�V			2008/08/25	<�V�K>
	//					2008/08/27	<�ύX>	�g���񂵂������p�ɂ���
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CKAGEO_BALL : public CMESH
	{
				#define		SHADOW_BALL			"shadow_ball"
				#define		SHINE_BALL			"shine_ball"
				#define		EFFECT_SHADOW		"shadow_ball"
				#define		EFFECT_SHINE		"shine_ball"
				#define		EFFECT_NAME_MAX		15

	private:		//-------------------- private --------------------//

			//==================================================================
			//	�J�Q�I�{�[���̎�ނɂŁA��������
			//==================================================================
				LPSTR				getBallTextureName(KAGEO_BALL_KIND inKind);

				BOOL				activate;						//!< ���ˊm�F(TRUE = ���ˍς�)
				THIS_MESSAGE		judgMessage;					//!< �������ʃ��b�Z�[�W
				REACTION_MESSAGE	reactMessage;					//!< ���A�N�V�������b�Z�[�W
				C3D_OBJECT_GROUP*	hitG;							//!< �Փ˔���O���[�v
				char				effectName[EFFECT_NAME_MAX];	//!< �����G�t�F�N�g��
				int					iEffectCnt;						//!< �G�t�F�N�g�̔����Ԋu
				int					iFrame;							//!< �`��t���[��
				D3DCOLOR			smokeColor;						//!< ���̐F

			//==================================================================
			//	������
			//==================================================================
				SSTATIC*	sStaticHit;								//!< �Փˉ�
				void		soundHit(void)							//!< �Փˉ�����
				{	if(	sStaticHit){
						sStaticHit->setPlayCur(0);
						sStaticHit->play(0, NULL);	}	}
				LPSTR		getHitSoundName(void)					//!< �Փˉ������擾
				{	if(	sStaticHit)	{	return	sStaticHit->getFileName();	}
					else			{	return	NULL;						}	}

	public:			//-------------------- public --------------------//

				CKAGEO_BALL(CONST DEV inDev, KAGEO_BALL_KIND inKind);
				CKAGEO_BALL(CKAGEO_BALL* inCopy);
		virtual	~CKAGEO_BALL(void);

			//==================================================================
			//	access	�Փ˔���m�F
			//==================================================================
				BOOL	getActivate(void)const					{ return	activate;			}

			//==================================================================
			//	access	�Փ˔���O���[�v
			//==================================================================
				C3D_OBJECT_GROUP*	getHitGroup(void)const					{ return	hitG;	}
				void				setHitGroup(C3D_OBJECT_GROUP* inHitG)	{ hitG	= inHitG;	}

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
				void	shot(VEC3* inAngle, VEC3* inLoc, C3D_OBJECT_GROUP* inHitG, int inFrame);

			//==================================================================
			//	�Փ˔���
			//==================================================================
				void	intersectG(void);

			//==================================================================
			//	�`��(�T�u�N���X�ŃI�[�o�[���C�h�\)
			//==================================================================
		virtual	BOOL	draw(CONST DEV inDev);
		virtual	BOOL	draw(CZTEX_SHA*  inZTexSha)	{ return	FALSE;	}
		virtual	BOOL	draw(CDEPTH_SHA* inDepthSha);
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------