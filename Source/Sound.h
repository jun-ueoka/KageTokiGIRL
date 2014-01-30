//-------------------------------------------------------------------------------------------------------------------------------
//�t�@�C����	:	Sound.h
//�쐬��		:	�u���L
//�쐬��		:	08/04/28
//�@�\			:	�T�E���h�Ɋ֘A���鏈���ŋ��ʂɎg�p��������L�q���Ă��܂��B
//���l			:	
//�X�V����		:	08/04/28	Ver0.01.00	��{�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
//�v���O�}�w��
#pragma once
#pragma warning(disable:4996)

//���C�u�����̃����N
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dsound.lib")

//�w�b�_�[�t�@�C���̃C���N���[�h
#include <dsound.h>

//�}�N����`
//�v���C�}���o�b�t�@�֘A(�v���C�}���o�b�t�@��44.1kHz 16bit �X�e���I�ō쐬����)
#define kCHANNELS 2																	//�`�����l����(�X�e���I)
#define kSAMPLESPERSEC 44100														//�T���v�����O���[�g(44.1kHz)
#define kBITSPERSAMPLE 16															//�T���v��������̃r�b�g��(16bit)

//����֘A
#define kRELEASE(x) if(x){ x->Release(); x = NULL; }								//x�̃I�u�W�F�N�g��NULL�ł͖����ꍇ�́Ax���������B
#define kCLOSEHANDLE(x) if(x){ CloseHandle(x); x = NULL; }							//�n���h���̊J��

//�X���b�h�֘A
#define WM_END_THREAD (WM_USER + 1)													//�X���b�h�I��
#define kTIMEOUT 1000																//�^�C���A�E�g����(�~���b�P��)
#define kEVENTMAX 2																	//�C�x���g�̍ő吔
#define KBLOCKNUM 3																	//�ʒm��(�o�b�t�@�T�C�Y = �ʒm�����Ԋu * �ʒm��)
//�X���b�h�ɑ��郁�b�Z�[�W
#define WM_ENDTHREAD WM_USER + 1													//�X���b�h���I��

//�\���̒�`
//�����f�[�^�̓ǂݍ��݂Ɏg�p
struct stSoundBuffer {
	VOID *pBuffer;																	//�f�[�^���i�[����o�b�t�@
	DWORD dwBufferSize;																//�o�b�t�@�̃T�C�Y
};

//�T�E���h����`
#define	SOUND_STEP				"step1"
#define	SOUND_JUMP_START		"jump_start"
#define	SOUND_JUMP_END			"jump_end"
#define	SOUND_SHOT_SHADOW		"shot_shadow"
#define	SOUND_SHOT_SHINE		"shot_shine"
#define	SOUND_SHOT_ENERGY		"shot_shadow"
#define	SOUND_CURSOR			"cursor1"
#define	SOUND_DECISION1			"decision1"
#define	SOUND_DECISION2			"decision2"
#define	SOUND_BARK1				"shadog_bark1"
#define	SOUND_BARK2				"shadog_bark2"
#define	SOUND_SHADOWB_HIT		"shadow_ball_hit"
#define	SOUND_SHINEB_HIT		"shine_ball_hit"