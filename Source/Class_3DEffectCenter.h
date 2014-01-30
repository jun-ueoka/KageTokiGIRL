//***********************************************************************************
//!	�t�@�C����		Class_3DEffectCenter.h
//
//	�@�\			3D�Q�[����p�̃G�t�F�N�g�N���X���A
//					��_�ɓZ�߂ĕ`�悷��N���X�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/09/16	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_3DEffect.h"				//!< �G�t�F�N�g�x�[�X
#include	"Class_3DObjectGroup.h"			//!< 3D�I�u�W�F�N�g�O���[�v

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�悭�g����G�t�F�N�g��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define	EFFECT_SMOKE				"smoke"					//!< ��
	#define	EFFECT_SHADOW				"shadow_ball"			//!< �V���h�E�G�t�F�N�g
	#define	EFFECT_SHINE				"shine_ball"			//!< �V���C���G�t�F�N�g
	#define	EFFECT_STARDUST				"star_dust"				//!< ����
	#define	EFFECT_CONTINUE				"continue"				//!< �R���e�B�j���[�G�t�F�N�g��
	#define	EFFECT_CONTINUE_YES			"continue_yes"			//!< �R���e�B�j���[�uYES�v�G�t�F�N�g��
	#define	EFFECT_CONTINUE_NO			"continue_no"			//!< �R���e�B�j���[�uNO�v�G�t�F�N�g��
	#define	EFFECT_CONTINUE_YES_SMOKE	"continue_yes_smoke"	//!< �R���e�B�j���[�uYES�v���o
	#define	EFFECT_CONTINUE_NO_SMOKE	"continue_no_smoke"		//!< �R���e�B�j���[�uNO�v���o

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		C3D_EFFECT_CENTER
	//
	//	�@�\			�O�����̃G�t�F�N�g���A�S�Ĉ����N���X(�G�t�F�N�g�Z���^�[)
	//	�X�V			2008/09/11	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class C3D_EFFECT_CENTER
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	�����C���X�^���X
			//==================================================================
		static	C3D_EFFECT_CENTER*			gUniqueInstance;

			//==================================================================
			//	�G�t�F�N�g�Ǘ�����
			//==================================================================
				C3D_OBJECT_GROUP			effects;			//!< �����G�t�F�N�g�O���[�v
				C3D_OBJECT_GROUP			effects2D;			//!< 2D�`��G�t�F�N�g�O���[�v
				map<string, C3D_OBJECT*>	effectMap;			//!< �G�t�F�N�g�}�b�v

	public:			//-------------------- public --------------------//

				C3D_EFFECT_CENTER(void);
				~C3D_EFFECT_CENTER(void);

		static	C3D_EFFECT_CENTER*	getInstance(void);			//!< �C���X�^���X���擾
		static	void				releaseInstance(void);		//!< �C���X�^���X�����


				void	initialize(void);						//!< ����������
				void	finishalize(void);						//!< �������

			//==================================================================
			//	�C���X�^���X�ɃG�t�F�N�g�o�^�A�����A���
			//==================================================================
		static	BOOL	addEffectsInstance(string inName, C3D_OBJECT* inEffect);
		static	void	occuEffectsInstance(	string		inName,
												VEC3		inLoc,
												float		inSize	= 1.0f,
												VEC3		inAngle	= VEC3(0.0f,0.0f,0.0f),
												D3DCOLOR	inColor	= 0xFFFFFFFF);
		static	void	occu2DEffectsInstance(	string		inName,
												VEC3		inLoc,
												float		inSize	= 1.0f,
												VEC3		inAngle	= VEC3(0.0f,0.0f,0.0f),
												D3DCOLOR	inColor	= 0xFFFFFFFF);
		static	void	releaseEffectInstance(string inName);

			//==================================================================
			//	�G�t�F�N�g�̓o�^�A�����A���
			//==================================================================
				BOOL	addEffects(string inName, C3D_OBJECT* inEffect);
				void	occuEffects(	string		inName,
										VEC3		inLoc,
										float		inSize	= 1.0f,
										VEC3		inAngle	= VEC3(0.0f,0.0f,0.0f),
										D3DCOLOR	inColor	= 0xFFFFFFFF);
				void	occu2DEffects(	string		inName,
										VEC3		inLoc,
										float		inSize	= 1.0f,
										VEC3		inAngle	= VEC3(0.0f,0.0f,0.0f),
										D3DCOLOR	inColor	= 0xFFFFFFFF);
				void	releaseEffect(string inName);

			//==================================================================
			//	���
			//==================================================================
				void	releaseEffects(void)	{ effects.releaseClear();	}
				void	releaseEffects2D(void)	{ effects2D.releaseClear();	}

			//==================================================================
			//	�S�G�t�F�N�g�`��
			//==================================================================
				void	drawEffects(CONST DEV inDev, C3D_OBJECT* inCamera);
				void	drawEffects2D(CONST SPRITE inSprite);
	};
}

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------