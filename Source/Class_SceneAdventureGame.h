//***********************************************************************************
//!	�t�@�C����		Class_SceneAdventureGame.h
//
//	�@�\			�Q�[���V�[�����`
//	�@�\�T�v		�R���ɃQ�[���̎厲���������āA�h��������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2008/07/10	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_SceneAdventureGameData.h"		//!< �Q�[���f�[�^
#include	"Class_Scene.h"							//!< �V�[��

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CSCENE_ADVENTURE_GAME
	//
	//	�@�\			�Q�[���V�[���̃N���X
	//	�X�V			2008/11/06	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CSCENE_ADVENTURE_GAME : public CSCENE, protected CEVENT_JUDGE_TASKER
	{
			//==================================================================
			//	�萔�Q
			//==================================================================
				#define	READ_SOUND_NONE		"S_NONE"				//!< �ǂݍ��݃T�E���h�����m�F

	protected:		//-------------------- protected --------------------//

			//==================================================================
			//	���N���X
			//==================================================================
				CGRAPHIC_DEVICE*			gDev;					//!< �O���t�B�b�N�f�o�C�X
				C3D_EFFECT_CENTER*			effectCenter;			//!< �G�t�F�N�g�Z���^�[
				SGAME_FLAG					gameFlag;				//!< �Q�[���t���O
				SGAME_TIMING_STEP			gameStep;				//!< �Q�[���X�e�b�v�t���O

			//==================================================================
			//	�Q�[���֘A
			//==================================================================
			//	�Q�[�����\�����镔���֌W
				CKAGETOKI_ADVENT_CHARACTER*	playerChara;			//!< �v���C���[�L�����N�^�[
				CADVENT_PLAYER*				player;					//!< �v���C���[
				C3D_OBJECT_CAMERA*			camera;					//!< �J����
				CGAME_MAP*					gameMap;				//!< �Q�[���}�b�v
				EM::CADVENT_ENEMY_MANAGER*	enemyManager;			//!< �G�Ǘ�
				CBACK*						back;					//!< �w�i
				SSTREAMING*					backSound;				//!< �X�e�[�WBGM(����Ύ����I�ɍĐ�)
			//	�Q�[�����o�N���X
				CZTEX_SHA*					shaderZTexture;			//!< Z�e�N�X�`���V�F�[�_
				CDEPTH_SHA*					shaderDepthBuf;			//!< �[�x�o�b�t�@�V�F�[�_
				MAT							lightView;				//!< ���C�g�r���[�s��
				MAT							lightProj;				//!< ���C�g�ˉe�s��
				VEC3						lightShift;				//!< ���_����̃��C�g�r���[�̈ʒu
				VEC2						lightNearFar;			//!< ���C�g�̉����ߎ�
			//	�Q�[�����̂̃X�e�[�^�X�֌W
				int							iStageNum;				//!< �X�e�[�W�ԍ�
				SCENE_FLAG					clearFlag;				//!< �Q�[���N���A���ɕԂ��t���O
				SCENE_FLAG					quitFlag;				//!< �Q�[���I�����ɕԂ��t���O
			//	�X�e�[�W�`�F���W�֌W
				int							iNextStageNum;			//!< ���̃X�e�[�W�ԍ�
				map<int, MY_CUBE>			stageChangeSwitch;		//!< �X�e�[�W�`�F���W�X�C�b�`�}�b�v
				map<int, MY_CUBE>			stageOutSwitch;			//!< �X�e�[�W�A�E�g�X�C�b�`�}�b�v

			//	�f�o�b�O����
#ifdef	_DEBUG
				CFONT_BASE*					fontBase;				//!< �t�H���g�x�[�X
				void						drawDebug(void);		//!< �f�o�b�O����
#else
#ifdef	PRESENTATION
				CFONT_BASE*					fontBase;				//!< �t�H���g�x�[�X
				void						drawDebug(void);		//!< �f�o�b�O����
#endif
#endif

			//==================================================================
			//	�C�x���g�X�N���v�g�֘A
			//==================================================================
				FILE*						eventFile;				//!< �C�x���g�t�@�C���|�C���^
				EVENT_STEP					eventStep;				//!< �C�x���g�X�e�b�v�t���O
				int							eventStartSwitchNum;	//!< �C�x���g�J�n�X�C�b�`�}�b�v�̔ԍ�
				map<int, MY_CUBE>			eventStartSwitch;		//!< �C�x���g�J�n�X�C�b�`�}�b�v
				map<int, C3D_OBJECT*>		eventObject;			//!< �C�x���g�I�u�W�F�N�g�}�b�v
				vector<CEXECUTER*>			eventExecuter;			//!< �C�x���g���쏈���O���[�v
				CCOMMENT_GROUP*				eventCommentG;			//!< �R�����g�O���[�v
				int							eventWaitCnt;			//!< �ҋ@�t���[���J�E���^
				CDUMMY_INPUT*				eventInput;				//!< �_�~�[���͏��
				MYKEY::TYPENAME				eventWaitKey;			//!< ���͑ҋ@�̃L�[

			//==================================================================
			//	�ăX�^�[�g�֘A
			//==================================================================
				int							continueFrameCnt;		//!< �t���[���J�E���^
				BOOL						continueChoice;			//!< �I���m�F(TRUE=�uYES�v NO=�uNO�v)
				SGAME_CONTINUE_FLAG			continueFlag;			//!< �R���e�B�j���[�t���O
				C3D_EFFECT*					effContinue;			//!< �R���e�B�j���[�G�t�F�N�g
				C3D_EFFECT*					effContinueYes;			//!< �R���e�B�j���[�uYES�v�G�t�F�N�g
				C3D_EFFECT*					effContinueNo;			//!< �R���e�B�j���[�uNo�v�G�t�F�N�g
				CTEXTURE_BASE*				texInterface;			//!< �C���^�[�t�F�[�X�e�N�X�`��

	private:		//-------------------- private --------------------//

			//==================================================================
			//	��L�f�[�^�̉������
			//==================================================================
			//	�C�x���g���쏈���̉��
				void	releaseEventExecuter(void)		{
					SAFE_VECTOR_DELETE(eventExecuter, CEXECUTER*);
				}

			//==================================================================
			//	�t���O�ύX�m�F
			//==================================================================
				BOOL	flagChange(SGAME_FLAG inFlag)	{
					if(	gameFlag != inFlag)
					{
						gameFlag = inFlag;
						switch(gameFlag)
						{
						case eSGFLAG_PLAY:			gameStep = eSGTSTEP_CHANGE_PLAY;		break;
						case eSGFLAG_EVENT:			gameStep = eSGTSTEP_CHANGE_EVENT;		break;
						case eSGFLAG_CONTINUE:		gameStep = eSGTSTEP_CHANGE_CONTINUE;	break;
						case eSGFLAG_STAGE_CHANGE:	gameStep = eSGTSTEP_CHANGE_STAGE_CHANGE;break;
						case eSGFLAG_RESET:			gameStep = eSGTSTEP_CHANGE_RESET;		break;
						case eSGFLAG_CLEAR:			gameStep = eSGTSTEP_CHANGE_CLEAR;		break;
						case eSGFLAG_QUIT:			gameStep = eSGTSTEP_CHANGE_QUIT;		break;
						case eSGFLAG_ERROR:			gameStep = eSGTSTEP_CHANGE_ERROR;		break;
						}
						return	TRUE;
					}
					else					{						return	FALSE;	}
				}

		//��������������������������������������������������������������������������
		//	Class_SceneAdventureGame.cpp
		//��������������������������������������������������������������������������
	private:		//-------------------- private --------------------//

				void	sgInitializeGame(void);						//!< �Q�[���̊J�n������
				void	sgInitializeScript(void);					//!< �X�N���v�g�̊J�n������

				void	sgFinishalizeGame(void);					//!< �Q�[���̉��
				void	sgFinishalizeScript(void);					//!< �X�N���v�g�̉��

	protected:		//-------------------- protected --------------------//

				void	sgCreateStage(int inStageNum);				//!< �Q�[���X�e�[�W����
				void	sgLoadEventStartSwitch(int inStageNum);		//!< �C�x���g�X�C�b�`�̓ǂݍ���
		virtual	void	sgCreateEnemy(int inStageNum)	{}			//!< �G����
				void	sgUpdateLight(void);						//!< ���C�g�̍X�V

	public:			//-------------------- public --------------------//

				CSCENE_ADVENTURE_GAME(	SCENE_FLAG inFlag,
										SCENE_FLAG inClearFlag,
										SCENE_FLAG inQuitFlag,
										SCENE_SHARE_DATA* inData);	//!< �R���X�g���N�^
		virtual	~CSCENE_ADVENTURE_GAME(void);						//!< �f�X�g���N�^

		virtual	void	sceneInitialize(int);						//!< ������
		virtual	void	sceneFinishalize(void);						//!< ���

		virtual	SCENE_FLAG	sceneMain(CINPUT* input);				//!< �V�[�����C��

		//��������������������������������������������������������������������������
		//	Class_SceneAdventureGamePlay.cpp
		//��������������������������������������������������������������������������
	protected:		//-------------------- protected --------------------//

				void	sgPlayEventCheck(VEC3* inLoc);				//!< �C�x���g�����`�F�b�N
				void	sgPlayOpenEventFile(int inEventNum);		//!< �C�x���g�t�@�C�����J��

		virtual	void	sgPlayChange(void);							//!< �Q�[���v���C�ڍs���̏�����
		virtual	void	sgPlayExecute(CINPUT* inInput);				//!< �Q�[���v���C������
		virtual	void	sgPlayRender(void);							//!< �Q�[���v���C���`��

		//��������������������������������������������������������������������������
		//	Class_SceneAdventureGameEvent.cpp
		//��������������������������������������������������������������������������
	private:		//-------------------- private --------------------//

				void	sgEventProc(CINPUT* input);					//!< �C�x���g����(�ċA���\�b�h)

				void	sgEventStepRead(void);						//!< �C�x���g�t�@�C����ǂ�
				void	sgEventStepExecute(void);					//!< �C�x���g���s
				void	sgEventStepTalk(CINPUT* input);				//!< �C�x���g��b��
				void	sgEventStepInputWait(CINPUT* input);		//!< �C�x���g���͑ҋ@��

				void	sgEventExecuter(void);						//!< �C�x���g���쏈���̎��s

				void	sgEventProcTalk(void);						//!< �R�����g����
				void	sgEventProcTalkStart(void);					//!< �R�����g�J�n
				void	sgEventProcObjCreate(void);					//!< ���̂̐���
				void	sgEventProcObjChange(void);					//!< ���̂̏�ԕω�
				void	sgEventProcObjMove(void);					//!< ���̈ړ�
				void	sgEventProcObjClear(void);					//!< ���̏���
				void	sgEventProcEffCreate(void);					//!< �G�t�F�N�g�̐���
				void	sgEventProcEffOccu(void);					//!< �G�t�F�N�g�̔���
				void	sgEventProcEffClear(void);					//!< �G�t�F�N�g�̏���
				void	sgEventProcInputOpe(void);					//!< �_�~�[���͂̑���
				void	sgEventProcInputWait(void);					//!< ���͑ҋ@�ڍs
				void	sgEventProcExecute(void);					//!< �ҋ@�ڍs
				void	sgEventProcGameClear(void);					//!< �Q�[���N���A�ڍs
				void	sgEventProcEventQuit(void);					//!< �C�x���g�I��

	protected:		//-------------------- protected --------------------//

		virtual	void	sgEventChange(void);						//!< �Q�[���C�x���g�ڍs���̏�����
		virtual	void	sgEventExecute(CINPUT* inInput);			//!< �Q�[���C�x���g������
		virtual	void	sgEventRender(void);						//!< �Q�[���C�x���g���`��

		//��������������������������������������������������������������������������
		//	Class_SceneAdventureGameContinue.cpp
		//��������������������������������������������������������������������������
	protected:		//-------------------- protected --------------------//

		virtual	void	sgContinueChange(void);						//!< �R���e�B�j���[�ڍs���̏�����
		virtual	void	sgContinueExecute(CINPUT* inInput);			//!< �R���e�B�j���[������
		virtual	void	sgContinueRender(void);						//!< �R���e�B�j���[���`��

		//��������������������������������������������������������������������������
		//	Class_SceneAdventureGameStageChange.cpp
		//��������������������������������������������������������������������������
	protected:		//-------------------- protected --------------------//

		virtual	void	sgStageChange(int inNewStage);				//!< �X�e�[�W�`�F���W�ڍs���̏�����
		virtual	void	sgStageChangeExecute(CINPUT* inInput);		//!< �X�e�[�W�`�F���W������
		virtual	void	sgStageChangeRender(void);					//!< �X�e�[�W�`�F���W���`��

		//��������������������������������������������������������������������������
		//	Class_SceneAdventureGameStageOut.cpp
		//��������������������������������������������������������������������������
	protected:		//-------------------- protected --------------------//

		virtual	void	sgStageOutChange(void);						//!< �X�e�[�W�A�E�g�ڍs���̏�����
		virtual	void	sgStageOutExecute(CINPUT* inInput);			//!< �X�e�[�W�A�E�g������
		virtual	void	sgStageOutRender(void);						//!< �X�e�[�W�A�E�g���`��

		//��������������������������������������������������������������������������
		//	Class_SceneAdventureGameReset.cpp
		//��������������������������������������������������������������������������
	protected:		//-------------------- protected --------------------//

		virtual	void	sgResetChange(void);						//!< �ăX�^�[�g�ڍs���̏�����
		virtual	void	sgResetExecute(CINPUT* inInput);			//!< �ăX�^�[�g������
		virtual	void	sgResetRender(void);						//!< �ăX�^�[�g���`��

		//��������������������������������������������������������������������������
		//	Class_SceneAdventureGameClear.cpp
		//��������������������������������������������������������������������������
	protected:		//-------------------- protected --------------------//

		virtual	void	sgClearChange(void);						//!< �Q�[���N���A�ڍs���̏�����
		virtual	void	sgClearExecute(CINPUT* inInput);			//!< �Q�[���N���A������
		virtual	void	sgClearRender(void);						//!< �Q�[���N���A���`��

		//��������������������������������������������������������������������������
		//	Class_SceneAdventureGameQuit.cpp
		//��������������������������������������������������������������������������
	protected:		//-------------------- protected --------------------//

		virtual	void	sgQuitChange(void);							//!< �Q�[���I���ڍs���̏�����
		virtual	void	sgQuitExecute(CINPUT* inInput);				//!< �Q�[���I��������
		virtual	void	sgQuitRender(void);							//!< �Q�[���I�����`��
	};
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------