//***********************************************************************************
//!	�t�@�C����		Class_SceneAdventureGameData.h
//
//	�@�\			�Q�[���̃V�[���ɕK�v�ȃf�[�^���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2008/07/10	<�V�K>
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_AdventPlayer.h"				//!< �A�h�x���`���[�Q�[���v���C���[
#include	"Class_Back.h"						//!< �w�i
#include	"Class_AdventEnemyManager.h"		//!< �G�l�~�[�}�l�[�W���[

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�X�e�[�W���\�[�X�t�@�C���ւ̃p�X
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_GAMESTAGE		"..//Resources//Data//Stage//GameStage%d.dat"

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�C�x���g�X�C�b�`���\�[�X�t�@�C���ւ̃p�X
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	#define		PATH_EVENTSWITCH	"..//Resources//Data//Event//EventSwitch%d.dat"
	#define		PATH_EVENT			"..//Resources//Data//Event//Event%d.dat"

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�Q�[���V�[���̕���t���O
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	SGAME_FLAG
	{
		eSGFLAG_PLAY,					//!< �Q�[���v���C��
		eSGFLAG_EVENT,					//!< �Q�[���C�x���g��
		eSGFLAG_CONTINUE,				//!< �R���e�B�j���[��
		eSGFLAG_STAGE_CHANGE,			//!< �}�b�v�`�F���W��
		eSGFLAG_STAGE_OUT,				//!< �X�e�[�W�A�E�g��
		eSGFLAG_RESET,					//!< �ăX�^�[�g��
		eSGFLAG_CLEAR,					//!< �Q�[���N���A��
		eSGFLAG_QUIT,					//!< �Q�[���I����
		eSGFLAG_ERROR,					//!< �Q�[�����G���[
		eSGFLAG_NO,						//!< �t���O������
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�Q�[���V�[���̃^�C�~���O�X�e�b�v�t���O
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	SGAME_TIMING_STEP
	{
		eSGTSTEP_END_PROC,				//!< �X�e�b�v����������ς�
		eSGTSTEP_CHANGE_PLAY,			//!< �Q�[���v���C���ֈڍs���ꂽ��
		eSGTSTEP_CHANGE_EVENT,			//!< �Q�[���C�x���g���ֈڍs���ꂽ��
		eSGTSTEP_CHANGE_CONTINUE,		//!< �R���e�B�j���[���ֈڍs���ꂽ��
		eSGTSTEP_CHANGE_STAGE_CHANGE,	//!< �}�b�v�`�F���W���ֈڍs���ꂽ��
		eSGTSTEP_CHANGE_RESET,			//!< �ăX�^�[�g���ֈڍs���ꂽ��
		eSGTSTEP_CHANGE_CLEAR,			//!< �Q�[���N���A���ֈڍs���ꂽ��
		eSGTSTEP_CHANGE_QUIT,			//!< �Q�[���I�����ֈڍs���ꂽ��
		eSGTSTEP_CHANGE_ERROR,			//!< �Q�[�����G���[�ֈڍs���ꂽ��
		eSGTSTEP_NO,					//!< �X�e�b�v�Ȃ�
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�ăX�^�[�g�V�[�����̏�������t���O
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	SGAME_CONTINUE_FLAG
	{
		eSGCONTI_START,					//!< �ăX�^�[�g�����J�n
		eSGCONTI_BLACKOUT,				//!< �u���b�N�A�E�g��
		eSGCONTI_CONTINUE_INIT,			//!< �R���e�B�j���[�I�������ڍs��
		eSGCONTI_CONTINUE,				//!< �R���e�B�j���[�I��������
		eSGCONTI_CHOICE_YES,			//!< �uYES�v�I���㏈����
		eSGCONTI_CHOICE_NO,				//!< �uNO�v�I���㏈����
		eSGCONTI_GAMEOVER,				//!< �����Q�[���I�[�o�[������
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�w�i�̎�ރt���O
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	SGAME_BACK_KIND
	{
		eSGBACK_SKY	= 0,				//!< ��
		eSGBACK_MAX,					//!< �ő��ރt���O
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�C�x���g�X�e�b�v����
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	EVENT_STEP
	{
		eEVSTEP_FALSE,					//!< �C�x���g������
		eEVSTEP_READ,					//!< �ǂݍ��ݒ�
		eEVSTEP_EXECUTE,				//!< ���s��(�ҋ@�Ɠ���)
		eEVSTEP_TALK,					//!< ��b��
		eEVSTEP_INPUTWAIT,				//!< ���͑ҋ@��
		eEVSTEP_ERROR,					//!< �C�x���g�G���[
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�C�x���g���ߎ���
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	EVENT_ORDER
	{
		eEVORDER_TALK,					//!< �R�����g�̐���			//!< TALK____
		eEVORDER_TALK_START,			//!< �R�����g�J�n			//!< TALK_STA
		eEVORDER_OBJ_CREATE,			//!< ���̂̐���				//!< OBJ_CREA
		eEVORDER_OBJ_CHANGE,			//!< ���̂̏�ԕω�			//!< OBJ_CHAN
		eEVORDER_OBJ_MOVE,				//!< ���̈ړ�				//!< OBJ_MOVE
		eEVORDER_OBJ_CLEAR,				//!< �����������̂̏���		//!< OBJ_CLEA
		eEVORDER_EFF_CREATE,			//!< �G�t�F�N�g�̐���		//!< EFF_CREA
		eEVORDER_EFF_OCCU,				//!< �G�t�F�N�g�̔���		//!< EFF_OCCU
		eEVORDER_EFF_CLEAR,				//!< �G�t�F�N�g�̏���		//!< EFF_CLEA
		eEVORDER_INPUT_OPE,				//!< �_�~�[���͑���			//!< INPUTOPE
		eEVORDER_INPUT_WAIT,			//!< ���͑ҋ@				//!< INPUTWAI
		eEVORDER_EXECUTE,				//!< ���s					//!< EXECUTE_
		eEVORDER_GAME_CLEAR,			//!< �Q�[���N���A			//!< GAME_CLE
		eEVORDER_EVENT_QUIT,			//!< �C�x���g�I��			//!< EV_QUIT_
		eEVORDER_MAX,					//!< �ő�C�x���g��
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	���̎���
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	EVENT_OBJECT
	{
		eEVOBJ_PLAYER,					//!< �v���C���[				//!< PLAYER__
		eEVOBJ_CAMERA,					//!< �J����					//!< CAMERA__
		eEVOBJ_EVOBJECT,				//!< �C�x���g���̕�			//!< EVOBJECT
		eEVOBJ_MAX,						//!< �ő啨�̎퐔
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	��ԕω�����
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	EVENT_CHANGE
	{
	//	�v���C���[����
		eEVCHAN_VISIBLE,				//!< �\���m�F				//!< VISIBLE_
		eEVCHAN_ANIMCHANGE,				//!< �A�j���[�V�����ύX		//!< ANIMCHAN
		eEVCHAN_LIMIT,					//!< ���͗L������ύX		//!< LIMIT___
		eEVCHAN_LIMIT_KAGEO,			//!< �J�Q�I���萧��ύX		//!< LIMIT_KO
		eEVCHAN_LIMIT_KAGEMI,			//!< �J�Q�~���萧��ύX		//!< LIMIT_KI
		eEVCHAN_CHARACHANGE_BOOLEN,		//!< �L�����ύX�L���m�F		//!< CHARCHAN
		eEVCHAN_CHARACHANGE_KAGEO,		//!< �J�Q�I�ɃL�����ύX		//!< CHANGEKO
		eEVCHAN_CHARACHANGE_KAGEMI,		//!< �J�Q�~�ɃL�����ύX		//!< CHANGEKI
	//	�J��������
		eEVCHAN_PARALLEL,				//!< ���_���t�ړ����[�h		//!< PARALLEL
		eEVCHAN_PARACHASE,				//!< �J�������t�ړ����[�h	//!< PARACHAS
		eEVCHAN_DOUBLETARGET,			//!< ��d�ǐՃ��[�h			//!< DOUBLETA
		eEVCHAN_MAX,					//!< �ő��ԕω���
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�������@����
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	EVENT_CREATE_OBJ
	{
		eEVCRE_TEXTURE,					//!< �e�N�X�`������			//!< CTEX____
		eEVCRE_MESH,					//!< ���b�V������			//!< CMESH___
		eEVCRE_SKINMESH,				//!< �X�L�����b�V������		//!< CSKIN___
		eEVCRE_EFFECT,					//!< �G�t�F�N�g����			//!< CEFFECT_
		eEVCRE_MAX,						//!< �ő吶�����@��
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�I�u�W�F�N�g�o�^�掯��
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	enum	EVENT_INSERT
	{
		eEVINS_GAMEMAP,					//!< �Q�[���}�b�v			//!< MAP_____
		eEVINS_WAIT_2D,					//!< �ҋ@����2D�`��			//!< WAIT_2D_
		eEVINS_MAX,						//!< �ő�I�u�W�F�N�g�o�^�搔
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CEVENT_JUDGE_TASKER
	//
	//	�@�\			�C�x���g����p�N���X�̒�`
	//	�X�V			2008/11/10	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class	CEVENT_JUDGE_TASKER
	{
	public:			//-------------------- public --------------------//
			//	�C�x���g���ߎ��ʔ���
				EVENT_ORDER			judgeEventOrderToString(LPSTR szString);
			//	���̎��ʔ���
				EVENT_OBJECT		judgeEventObjectToString(LPSTR szString);
			//	��ԕω����ʔ���
				EVENT_CHANGE		judgeEventChangeToString(LPSTR szString);
			//	���̐������ʔ���
				EVENT_CREATE_OBJ	judgeEventCreateObjToString(LPSTR szString);
			//	�I�u�W�F�N�g�o�^�掯�ʔ���
				EVENT_INSERT		judgeEventInsertToString(LPSTR szString);
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CEXECUTER
	//
	//	�@�\			�C�x���g���쏈���N���X
	//	�@�\�T�v		������X�[�p�[�N���X�Ƃ��A�l�X�ȏ�����������
	//					�N���X���`����B
	//	�X�V			2008/11/10	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CEXECUTER
	{
	public:			//-------------------- public --------------------//
			//	�C�x���g���s
		virtual	BOOL	execute(void)	=0;
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CEXE_MOVE_OBJECT
	//
	//	�@�\			3D�I�u�W�F�N�g�𓮂����C�x���g
	//	�@�\�T�v		�ړ��t���[�����œ�����
	//	�X�V			2008/11/10	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CEXE_MOVE_OBJECT : public CEXECUTER
	{
	protected:		//-------------------- protected --------------------//
				C3D_OBJECT*		object;				//!< �ΏۃI�u�W�F�N�g
				int				iMoveFrame;			//!< �ړ��t���[��
				VEC3			moveValue;			//!< �ړ���
	public:			//-------------------- public --------------------//
			//==================================================================
			//	�R���X�g���N�^
			//==================================================================
				CEXE_MOVE_OBJECT(C3D_OBJECT* inObject, int inFrame, VEC3 inValue){
					object		= inObject;
					iMoveFrame	= inFrame;
					moveValue	= inValue;
				}
			//==================================================================
			//	�C�x���g���s
			//==================================================================
				BOOL	execute(void){
					if(	iMoveFrame > 0){
						object->moveValue(&moveValue); iMoveFrame--; return TRUE;
					} else {
						return	FALSE;
					}
				}
	};

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CEXE_DUMMY_INPUT
	//
	//	�@�\			�_�~�[���͂̑���
	//	�X�V			2008/11/11	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CEXE_DUMMY_INPUT : public CEXECUTER
	{
	protected:		//-------------------- protected --------------------//
				CDUMMY_INPUT*	dummyInput;			//!< ����Ώۂ̃_�~�[����
				int				iFrame;				//!< ����t���[��
				DWORD			targetKey;			//!< ���삷��L�[�̎��
	public:			//-------------------- public --------------------//
			//==================================================================
			//	�R���X�g���N�^
			//==================================================================
				CEXE_DUMMY_INPUT(CDUMMY_INPUT* inDummy, int inFrame, DWORD inKey){
					dummyInput	= inDummy;
					iFrame		= inFrame;
					targetKey	= inKey;
				}
			//==================================================================
			//	�C�x���g���s
			//==================================================================
				BOOL	execute(void){
					if(	iFrame > 0){
						dummyInput->setPress((MYKEY::TYPENAME)targetKey, true);
						iFrame--;	return	TRUE;
					} else {
						dummyInput->setPress((MYKEY::TYPENAME)targetKey, false);
						return	FALSE;
					}
				}
	};
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------