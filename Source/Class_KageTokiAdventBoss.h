//***********************************************************************************
//!	�t�@�C����		Class_KageTokiAdventBoss.h
//
//	�@�\			�A�h�x���`���[�Q�[���̃{�X�x�[�X�N���X�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2009/02/19	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_KageTokiAdventCharacter.h"	//!< �A�h�x���`���[�Q�[�����[�h�L�����N�^�[
#include	"Class_Sound_OggVorbis.h"			//!< ogg���C�u����(�T�E���h�𔭐��������)

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CKAGETOKI_ADVENT_BOSS
	//
	//	�@�\			�A�h�x���`���[�Q�[���̃{�X�x�[�X�N���X
	//	�X�V			2009/02/19	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CKAGETOKI_ADVENT_BOSS : public CKAGETOKI_ADVENT_CHARACTER
	{
	//private:		//-------------------- private --------------------//
	protected:		//-------------------- protected --------------------//

				int				iMaxLife;					//!< �ő�̗�
				int				iLife;						//!< �c��̗�

				BOOL				bDrawInterface;			//!< �C���^�[�t�F�[�X�`��m�F
				TEX					interfaceTex;			//!< �C���^�[�t�F�[�X�e�N�X�`��
				CUSTOM_VERTEX_2D	lifeBase[4];			//!< �̗͊��
				CUSTOM_VERTEX_2D	lifeFrameBase[4];		//!< �̗̖͂ڐ��̘g���
				CUSTOM_VERTEX_2D	lifeFrame[4];			//!< �̗̖͂ڐ��̘g
				CUSTOM_VERTEX_2D	lifeChip[4];			//!< �̗̖͂ڐ�

			//==================================================================
			//	�O���|�C���^�[
			//==================================================================
				CKAGETOKI_ADVENT_CHARACTER**	player;		//!< �v���C���[

				void	opeTurnToPlayer(void);				//!< �v���C���[�̕�������

	public:			//-------------------- public --------------------//

				CKAGETOKI_ADVENT_BOSS(
					CONST DEV	inDev,
					CONST LPSTR	inName,
					CONST LPSTR	inResource = PATH_SKINMESH,
					CONST LPSTR	inTexture  = PATH_SKINMESHTEX);	//!< �R���X�g���N�^
		virtual	~CKAGETOKI_ADVENT_BOSS(void);					//!< �f�X�g���N�^

			//==================================================================
			//	access	�v���C���[�̊O���|�C���^�[
			//==================================================================
				void	setTargetPlayer(CKAGETOKI_ADVENT_CHARACTER** inPlayer)
				{
					this->player	= inPlayer;
				}

			//==================================================================
			//	access	�̗�
			//==================================================================
				int		getMaxLife(void)const	{ return	iMaxLife;	}
				void	setMaxLife(int inValue)	{ iMaxLife	= inValue;	}
				int		getLife(void)const		{ return	iLife;		}
				void	setLife(int inValue)	{ iLife		= inValue;	}

			//==================================================================
			//	access	�C���^�[�t�F�[�X�`��m�F
			//==================================================================
				BOOL	getDrawInterface(void)const		{ return	bDrawInterface;		}
				void	setDrawInterface(BOOL inBoolen)	{ bDrawInterface = inBoolen;	}

			//==================================================================
			//	���Z�b�g
			//==================================================================
		virtual	void	resetParameter(void);

			//==================================================================
			//	��{����
			//==================================================================
		virtual	void	operation(CINPUT* input){}

			//==================================================================
			//	�C���^�[�t�F�[�X�̕`��
			//==================================================================
				void	drawInterface(CONST DEV inDev);
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------