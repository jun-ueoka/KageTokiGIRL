//***********************************************************************************
//!	�t�@�C����		Class_SceneTitle.h
//
//	�@�\			�^�C�g���V�[���̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2008/09/01	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"GameCommon.h"					//!< �Q�[���R����
#include	"Class_Scene.h"					//!< �V�[��
#include	"Class_Sound_OggVorbis.h"		//!< ogg���C�u����(�T�E���h�𔭐��������)

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CSCENE_TITLE
	//
	//	�@�\			�^�C�g���V�[���̃N���X
	//	�X�V			2008/09/01	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_TITLE : public CSCENE
	{
			//==================================================================
			//	�J�[�\���̈ʒu�t���O
			//==================================================================
				enum	CURSOR_POS
				{
					eCURP_START		= 0,					//!< �uSTART�v�Ɉʒu����
					eCURP_GO_START,							//!< �uSTART�v�Ɍ�����
					eCURP_OPTION,							//!< �uOPTION�v�Ɉʒu����
					eCURP_GO_OPTION_FROM_START,				//!< �uSTART�v����uOPTION�v�Ɍ�����
					eCURP_GO_OPTION_FROM_END,				//!< �uEND�v����uOPTION�v�Ɍ�����
					eCURP_END,								//!< �uEND�v�Ɉʒu����
					eCURP_GO_END,							//!< �uEND�v�Ɍ�����
				};

			//==================================================================
			//	�^�C�g���V�[���̕���
			//==================================================================
				enum	STITLE_FLAG
				{
					eST_INIT_TITLE,							//!< �^�C�g����ʈڍs���o
					eST_TITLE,								//!< �^�C�g�����
					eST_START_SWITCH,						//!< �Q�[���X�^�[�g���o
					eST_END_SWITCH,							//!< �Q�[���I�����o
					eST_DEMO,								//!< �f���ڍs���o
				};

			//==================================================================
			//	�}�N���Q
			//==================================================================
				#define	CURSOR_START_POSY		530.0f		//!< �uSTART�v��Y�ʒu
				#define	CURSOR_OPTION_POSY		600.0f		//!< �uOPTION�v��Y�ʒu
				#define	CURSOR_END_POSY			670.0f		//!< �uEND�v��Y�ʒu
				#define	CURSOR_MOVE_SPEED		 10.0f		//!< �J�[�\���ړ����x

				#define	STARTSWITCH_EXPAND		60			//!< �X�^�[�g�g��
				#define	STARTSWITCH_CLEAR_S		100			//!< �������J�n
				#define	STARTSWITCH_CLEAR_E		102			//!< �������I��
				#define	STARTSWITCH_END			200			//!< �Q�[���X�^�[�g���o�I��

				#define	ENDSWITCH_EXPAND		60			//!< �X�^�[�g�g��
				#define	ENDSWITCH_CLEAR_S		100			//!< �������J�n
				#define	ENDSWITCH_CLEAR_E		102			//!< �������I��
				#define	ENDSWITCH_END			200			//!< �Q�[���X�^�[�g���o�I��

				#define	DEMO_STEP_TIME			1800		//!< �f���ڍs�t���[��
				#define	DEMO_CLEAR_S			0			//!< �������J�n
				#define	DEMO_CLEAR_E			100			//!< �������I��
				#define	DEMO_END				100			//!< �f���ڍs���o�I��

				#define	EFF_OCCU_SPACE			5				//!< �G�t�F�N�g�����Ԋu
				#define	NAME_SHADOW				"shadow_ball"	//!< �e�G�t�F�N�g��
				#define	NAME_SHINE				"shine_ball"	//!< ���G�t�F�N�g��

	private:		//-------------------- private --------------------//

			//==================================================================
			//	�t���O����惁�\�b�h�Q
			//==================================================================
				void	motionInitTitle(CINPUT* input);		//!< �^�C�g����ʈڍs���o����
				void	motionTitle(CINPUT* input);			//!< �^�C�g����ʏ���
				void	motionStartSwitch(CINPUT* input);	//!< �Q�[���X�^�[�g���o����
				void	motionEndSwitch(CINPUT* input);		//!< �Q�[���I�����o����
				void	motionDemo(CINPUT* input);			//!< �f���ڍs���o����

				void	renderInitTitle(void);				//!< �^�C�g����ʈڍs���o�`��
				void	renderTitle(void);					//!< �^�C�g����ʕ`��
				void	renderStartSwitch(void);			//!< �Q�[���X�^�[�g���o�`��
				void	renderEndSwitch(void);				//!< �Q�[���I�����o�`��
				void	renderDemo(void);					//!< �f���ڍs���o�`��

				void	initInitTitle(void);				//!< �^�C�g����ʈڍs���o����
				void	initTitle(void);					//!< �^�C�g����ʖ���
				void	initStartSwitch(void);				//!< �Q�[���X�^�[�g���o����
				void	initEndSwitch(void);				//!< �Q�[���I�����o����
				void	initDemo(void);						//!< �f���ڍs���o����

			//==================================================================
			//	�T�E���h�f�[�^
			//==================================================================
				SSTATIC*	sStaticCursor;					//!< �J�[�\����
				SSTATIC*	sStaticDecision;				//!< ���艹
				SSTREAMING*	sStreamingBgm;					//!< �w�i��

			//==================================================================
			//	�T�E���h����
			//==================================================================
				void	soundCursor(void){
					if(sStaticCursor)
					{
						this->sStaticCursor->setPlayCur(0);
						this->sStaticCursor->play(0, NULL);
					}
				}
				void	soundDecision(void){
					if(sStaticDecision)
					{
						this->sStaticDecision->setPlayCur(0);
						this->sStaticDecision->play(0, NULL);
					}
				}

			//==================================================================
			//	���o����
			//==================================================================
				CTEXTURE*	titleBack;						//!< �^�C�g���w�i
				CTEXTURE*	titleTex;						//!< �^�C�g��
				CTEXTURE*	start;							//!< �uSTART�v
				CTEXTURE*	option;							//!< �uOPTION�v
				CTEXTURE*	end;							//!< �uEND�v
				CTEXTURE*	cursor;							//!< �J�[�\��
				CFONT_BASE*	font;							//!< �t�H���g�x�[�X
				int			iStack;							//!< �X�^�b�N�J�E���^
				BOOL		bStack;							//!< �X�^�b�N�m�F
				int			iTimeCnt;						//!< �^�C���J�E���^

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	���N���X
			//==================================================================
				CGRAPHIC_DEVICE*	gDev;					//!< �O���t�B�b�N�f�o�C�X
				C3D_EFFECT_CENTER*	effect;					//!< �G�t�F�N�g�Z���^�[

			//==================================================================
			//	�t���O�Q
			//==================================================================
				STITLE_FLAG	titleFlag;						//!< ����t���O
				CURSOR_POS	cursorPos;						//!< �J�[�\���ʒu
				SCENE_FLAG	startFlag;						//!< �X�^�[�g�ڍs�t���O
				SCENE_FLAG	endFlag;						//!< �G���h�ڍs�t���O
				SCENE_FLAG	loopFlag;						//!< ���[�v�t���O(�����Ēu���ƈڍs����)

	public:			//-------------------- public --------------------//

				CSCENE_TITLE(SCENE_SHARE_DATA* inData);
		virtual	~CSCENE_TITLE(void);

				void	sceneInitialize(int inStageNum);	//!< ������
				void	sceneFinishalize(void);				//!< �J��

				SCENE_FLAG	sceneMain(CINPUT* input);		//!< �V�[�����C��
	};
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------