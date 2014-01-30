//***********************************************************************************
//!	�t�@�C����		Class_SceneAdventureGameData.cpp
//
//	�@�\			�Q�[���̃V�[���ɕK�v�ȃf�[�^������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			22008/11/06	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneAdventureGameData.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CEVENT_JUDGE_TASKER::judgeEventOrderToString
	//
	//	�@�\		�C�x���g���ߎ��ʔ���
	//	����		szString		���蕶����
	//	�߂�l		�C�x���g���ߎ��ʔ��茋��
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	EVENT_ORDER
	CEVENT_JUDGE_TASKER::judgeEventOrderToString(LPSTR szString)
	{
	//	�C�x���g����p�����z��
		char	orderJudger[eEVORDER_MAX][9]	=
		{
			"TALK____",
			"TALK_STA",
			"OBJ_CREA",
			"OBJ_CHAN",
			"OBJ_MOVE",
			"OBJ_CLEA",
			"EFF_CREA",
			"EFF_OCCU",
			"EFF_CLEA",
			"INPUTOPE",
			"INPUTWAI",
			"EXECUTE_",
			"GAME_CLE",
			"EV_QUIT_"
		};
	//	�C�x���g�̎�ނ��A���l�Ƃ��Č��o
		int	iFlag;
		for(iFlag=0; iFlag<eEVORDER_MAX; iFlag++)
		{
			if(	strcmp(szString, orderJudger[iFlag]) == 0)	break;
		}
	//	�C�x���g�̎�ނ�Ԃ�
		return	EVENT_ORDER(iFlag);
	}

	//================================================================================
	//!	���\�b�h��	CEVENT_JUDGE_TASKER::judgeEventObjectToString
	//
	//	�@�\		���̎��ʔ���
	//	����		szString		���蕶����
	//	�߂�l		���̎��ʔ��茋��
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	EVENT_OBJECT
	CEVENT_JUDGE_TASKER::judgeEventObjectToString(LPSTR szString)
	{
	//	���̔���p�����z��
		char	objectJudger[eEVOBJ_MAX][9]	=
		{
			"PLAYER__",
			"CAMERA__",
			"EVOBJECT"
		};
	//	���̂̎�ނ��A���l�Ƃ��Č��o
		int	iFlag;
		for(iFlag=0; iFlag<eEVOBJ_MAX; iFlag++)
		{
			if(	strcmp(szString, objectJudger[iFlag]) == 0)	break;
		}
	//	���̂̎�ނ�Ԃ�
		return	EVENT_OBJECT(iFlag);
	}

	//================================================================================
	//!	���\�b�h��	CEVENT_JUDGE_TASKER::judgeEventChangeToString
	//
	//	�@�\		��ԕω����ʔ���
	//	����		szString		���蕶����
	//	�߂�l		��ԕω����ʔ��茋��
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	EVENT_CHANGE
	CEVENT_JUDGE_TASKER::judgeEventChangeToString(LPSTR szString)
	{
	//	��ԕω�����p�����z��
		char	changeJudger[eEVCHAN_MAX][9]	=
		{
			"VISIBLE_",
			"ANIMCHAN",
			"LIMIT___",
			"LIMIT_KO",
			"LIMIT_KI",
			"CHARCHAN",
			"CHANGEKO",
			"CHANGEKI",
			"PARALLEL",
			"PARACHAS",
			"DOUBLETA"
		};
	//	��ԕω��̎�ނ��A���l�Ƃ��Č��o
		int	iFlag;
		for(iFlag=0; iFlag<eEVCHAN_MAX; iFlag++)
		{
			if(	strcmp(szString, changeJudger[iFlag]) == 0)	break;
		}
	//	��ԕω��̎�ނ�Ԃ�
		return	EVENT_CHANGE(iFlag);
	}

	//================================================================================
	//!	���\�b�h��	CEVENT_JUDGE_TASKER::judgeEventCreateObjToString
	//
	//	�@�\		���̐������ʔ���
	//	����		szString		���蕶����
	//	�߂�l		���̐������ʔ��茋��
	//	�X�V		2008/11/10		<�V�K>
	//====================================================b============================
	EVENT_CREATE_OBJ
	CEVENT_JUDGE_TASKER::judgeEventCreateObjToString(LPSTR szString)
	{
	//	���̐�������p�����z��
		char	createObjJudger[eEVCRE_MAX][9]	=
		{
			"CTEX____",
			"CMESH___",
			"CSKIN___",
			"CEFFECT_"
		};
	//	���̐����̎�ނ��A���l�Ƃ��Č��o
		int	iFlag;
		for(iFlag=0; iFlag<eEVCRE_MAX; iFlag++)
		{
			if(	strcmp(szString, createObjJudger[iFlag]) == 0)	break;
		}
	//	���̐����̎�ނ�Ԃ�
		return	EVENT_CREATE_OBJ(iFlag);
	}

	//================================================================================
	//!	���\�b�h��	CEVENT_JUDGE_TASKER::judgeEventInsertToString
	//
	//	�@�\		�I�u�W�F�N�g�o�^�掯�ʔ���
	//	����		szString		���蕶����
	//	�߂�l		�I�u�W�F�N�g�o�^�掯�ʔ��茋��
	//	�X�V		2008/11/10		<�V�K>
	//====================================================b============================
	EVENT_INSERT
	CEVENT_JUDGE_TASKER::judgeEventInsertToString(LPSTR szString)
	{
	//	�I�u�W�F�N�g�o�^�攻��p�����z��
		char	insertJudger[eEVINS_MAX][9]	=
		{
			"MAP_____",
			"WAIT_2D_"
		};
	//	�I�u�W�F�N�g�o�^��̎�ނ��A���l�Ƃ��Č��o
		int	iFlag;
		for(iFlag=0; iFlag<eEVINS_MAX; iFlag++)
		{
			if(	strcmp(szString, insertJudger[iFlag]) == 0)	break;
		}
	//	�I�u�W�F�N�g�o�^��̎�ނ�Ԃ�
		return	EVENT_INSERT(iFlag);
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------