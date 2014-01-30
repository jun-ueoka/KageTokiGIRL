//***********************************************************************************
//!	�t�@�C����		Class_SceneBoss5.h
//
//	�@�\			�Q�[���V�[���N���X�A�{�X�X�e�[�W5�u�N���C���v���`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2009/02/15	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_SceneAdventureGame.h"		//!< �Q�[���V�[��
#include	"Class_BossExe.h"					//!< �{�X�N���X�u�G�N�X�v

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CSCENE_BOSS5
	//
	//	�@�\			�Q�[���V�[���N���X�A�{�X�X�e�[�W5�u�N���C���v
	//	�@�\�T�v		�{�X�X�e�[�W���\������
	//	�X�V			2009/02/15	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_BOSS5 : public CSCENE_ADVENTURE_GAME
	{
	private:		//-------------------- private --------------------//

			//==================================================================
			//	�X�e�[�W�X�e�[�^�X
			//==================================================================
				int			eventStepCnt;						//!< �C�x���g�X�e�b�v�J�E���^
				CBOSS_EXE*	bossExe;							//!< �u�G�N�X�v�N���X

	public:			//-------------------- public --------------------//

				CSCENE_BOSS5(SCENE_SHARE_DATA* inData);			//!< �R���X�g���N�^
		virtual	~CSCENE_BOSS5(void);							//!< �f�X�g���N�^

				void		sceneInitialize(int);				//!< ������
				void		sceneFinishalize(void);				//!< ���

				SCENE_FLAG	sceneMain(CINPUT* input);			//!< �V�[�����C��

			//==================================================================
			//	�{�X�X�e�[�W5��p����
			//==================================================================
				void	updateExecute(void);

			//==================================================================
			//	�G����(�I�[�o�[���C�g)
			//==================================================================
				void	sgCreateEnemy(int inStageNum);
	};
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------