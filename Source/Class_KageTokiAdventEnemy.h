//***********************************************************************************
//!	�t�@�C����		Class_KageTokiAdventEnemy.h
//
//	�@�\			�A�h�x���`���[�Q�[�����[�h�̓G�L�����N�^�[�N���X
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2009/01/16	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_KageTokiAdventCharacter.h"	//!< �A�h�x���g�L�����N�^�[

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CKAGETOKI_ADVENT_ENEMY
	//
	//	�@�\			�A�h�x���`���[�Q�[�����[�h�̓G�L�����N�^�[�N���X
	//	�X�V			2009/01/16	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CKAGETOKI_ADVENT_ENEMY : public CKAGETOKI_ADVENT_CHARACTER
	{
	protected:		//-------------------- protected --------------------//

				int					iMaxLife;					//!< �ő�̗�
				int					iLife;						//!< �c��̗�
				MY_CUBE				controlAreaOffset;			//!< ����̈�̃I�t�Z�b�g
				MY_CUBE				controlArea;				//!< ����̈�
				MY_CUBE				actionArea;					//!< �s���̈�

			//==================================================================
			//	�O���|�C���^�[
			//==================================================================
				CKAGETOKI_ADVENT_CHARACTER**	player;			//!< �v���C���[

				void	opeTurn(void);							//!< �����]��������

	public:			//-------------------- public --------------------//

				CKAGETOKI_ADVENT_ENEMY	(
					CONST DEV	inDev,
					CONST LPSTR	inName,
					CONST LPSTR	inResource = PATH_SKINMESH,
					CONST LPSTR	inTexture  = PATH_SKINMESHTEX);	//!< �R���X�g���N�^
		virtual	~CKAGETOKI_ADVENT_ENEMY(void);					//!< �f�X�g���N�^

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
				int		getLife(void)const		{ return	iLife;		}

			//==================================================================
			//	access	����̈�
			//==================================================================
				void		setControlAreaOffset(float inLeft,   float inRight,
												 float inBottom, float inTop,
												 float inRear,   float inFront);
				void		setControlAreaOffset(MY_CUBE* inCube)
				{	controlAreaOffset	= *inCube;	}
				MY_CUBE*	getControlArea(void);

			//==================================================================
			//	access	�s���̈�
			//==================================================================
				void	setActionArea(VEC3* inLoc, VEC3* inSize);
				void	setActionArea(float inLeft,   float inRight,
									  float inBottom, float inTop,
									  float inRear,   float inFront);
				void	setActionArea(MY_CUBE* inCube)
				{	actionArea	= *inCube;	}

			//==================================================================
			//	�G�̏���
			//==================================================================
				void	resetParameter(void);
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------