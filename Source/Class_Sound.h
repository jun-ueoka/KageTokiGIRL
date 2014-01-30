//-------------------------------------------------------------------------------------------------------------------------------
//�t�@�C����	:	CSound.h
//�쐬��		:	�u���L
//�쐬��		:	08/04/28
//�@�\			:	CSound.cpp�ŕK�v�Ȑ錾�A��`���s���Ă��܂��B
//���l			:	CSound�N���X����h�����Ċe�T�E���h�o�b�t�@���쐬����܂��B
//					���ACSound�N���X�Őݒ肳�ꂽ�f�[�^��h���N���X�͋��L���Ă��܂��B
//�X�V����		:	08/04/28	Ver0.01.00	��{�@�\�������B
//					08/05/08	Ver0.11.00	Ogg Vorbis�ɑΉ��B
//					08/05/12	Ver0.21.00	OggVorbis�������������N���X�Ƃ��ēƗ��B
//-------------------------------------------------------------------------------------------------------------------------------
//�v���O�}�w��
#pragma once
#pragma warning(disable:4996)

//�w�b�_�[�t�@�C���̃C���N���[�h
#include "Sound.h"

//�񋓌^��`
//�Đ��t���O(�X�g���[�~���O�Ɏg�p)
typedef enum ePLAY_FLAG {
	ePLAY_LOOPING,																	//���[�v�Đ�
	ePLAY_ONCE,																		//���̂ݍĐ�
} PLAY_FLAG;

//�T�E���h�֘A�G���[
typedef enum eSOUND_ERROR {
	eSOUND_ERROR,																	//�G���[
	eSOUND_OK,																		//����
	eSOUND_FILENAME,																//�t�@�C�������ݒ肳��Ă��Ȃ�
	eSOUND_OPEN,																	//�t�@�C���̃I�[�v���Ɏ��s
	eSOUND_CLOSE,																	//�t�@�C���̃N���[�Y�Ɏ��s
	eSOUND_READ,																	//�t�@�C���̓ǂݍ��݂Ɏ��s
	eSOUND_EXISTING,																//���ɃC���^�[�t�F�[�X������(�G���[�ł͖���)
	eSOUND_CREATEINTERFACE,															//�C���^�[�t�F�[�X�̍쐬�Ɏ��s
	eSOUND_COOPERATIVELEVEL,														//�������x���̐ݒ�Ɏ��s
	eSOUND_READFORMAT,																//�t�@�C���t�H�[�}�b�g�ǂݍ��݂Ɏ��s
	eSOUND_DATASIZE,																//�f�[�^�T�C�Y�ǂݍ��݂Ɏ��s
	eSOUND_NOTINTERFACE,															//�C���^�[�t�F�[�X�����݂��Ȃ�
	eSOUND_CREATEBUFFER,															//�o�b�t�@�̐����Ɏ��s
	eSOUND_LOCK,																	//�o�b�t�@�̃��b�N�Ɏ��s
	eSOUND_UNLOCK,																	//�o�b�t�@�̃A�����b�N�Ɏ��s
	eSOUND_SETFORMAT,																//�t�H�[�}�b�g�̐ݒ�Ɏ��s
	eSOUND_NOTBUFFER,																//�o�b�t�@�����݂��Ȃ�
	eSOUND_QUERYINTERFACE,															//DirectSoundNotify�C���^�[�t�F�[�X�擾�Ɏ��s
	eSOUND_SETNOTIFICATIONPOSITIONS,												//��~�ʒu�̐ݒ�Ɏ��s
	eSOUND_CREATETHREAD,															//�X���b�h�̍쐬�Ɏ��s
	eSOUND_EFFECT,																	//�G�t�F�N�g�̐ݒ�Ɏ��s
} SOUND_ERROR;

//-------------------------------------------------------------------------------------------------------------------------------
//�N���X��		:	CSOUND
//�쐬��		:	�u���L
//�쐬��		:	08/04/28
//�@�\			:	�T�E���h�֘A�����B
//���l			:	�T�E���h���Đ�����̂ɕK�v�ȃC���^�[�t�F�[�X�ł��B
//					CSound����{�N���X�Ƃ��āACSoundStatic�ACSoundStream���̔h���N���X����������Ă��܂��B
//					���̃N���X�Őݒ肵���f�[�^��h������N���X�S�Ăŋ��L���Ă��܂��B
//					���̃N���X���g�p���邽�߂ɂ̓E�B���h�E�n���h�����擾����K�v������܂��B
//					���A�T�E���h�֘A�̏������s�����߂ɂ͂��̃N���X�����݂���K�v������܂��B
//�X�V����		:	08/04/28	Ver0.01.00	DirectSound�C���^�[�t�F�[�X�̏������ƃv���C�}���o�b�t�@�̍쐬�������B
//					08/05/01	Ver1.01.00	�@�\������
//-------------------------------------------------------------------------------------------------------------------------------
class CSOUND {
private:
	/***�����̍Đ��ɕK�v�ȃf�[�^(�S�Ă̔h���N���X�œ���̕����g�p)***/
	static HWND hWnd;																//�֘A�t����E�B���h�E�̃n���h��
	/***�ŏ��ɐ������ꂽ�N���X�ȊO�ɂ̓N���X�ϐ�������ł��Ȃ��悤�ɂ��錮***/
	static CSOUND *key;																//��(�����̃|�C���^�����ɂ���)

	//--------------------------------------------------
	//	��(�R���X�g���N�^�ƃf�X�g���N�^�Ŏg�p)
	//	�h���N���X����N������Ȃ��悤�ɂ���
	//--------------------------------------------------
	//�@�\:�����|����	����:�������g�̃|�C���^		�߂�l:�Ȃ�
	void lock(CSOUND *key) { this->key = key; }										//�����|����
	//�@�\:�����|���Ă��邩���ׂ�	����:�Ȃ�		�߂�l:TRUE �|���Ă���	FALSE �|���Ă��Ȃ�
	BOOL checkKey(void) { if(this->key) { return TRUE; } return FALSE; }			//�����|���Ă��邩���ׂ�
	//�@�\:�����J����	����:�������g�̃|�C���^		�߂�l:�Ȃ�
	void unlock(CSOUND *key)														//�����J����
	{
		//�n���ꂽ���ŏ����J���邩���ׂ�(�n���ꂽ�����|�����Ă��錮�ƈ�v������J����)
		if(this->key == key) {														//������v
			this->key = NULL;														//�����J����
		}
	}

	/***�T�E���h�̍Đ��ɕK�v�ȏ���***/
	SOUND_ERROR initDirectSound(void);												//DirectSound���g�p�\�ɂ���
	SOUND_ERROR initPrimaryBuffer(void);											//PrimaryBuffer���g�p�\�ɂ���

	SOUND_ERROR initialize(void);													//�f�[�^�̏�����

	void release(void);																//�f�[�^�̉��

protected:
	/***�����̍Đ��ɕK�v�ȃf�[�^(�S�Ă̔h���N���X�œ���̕����g�p)***/
	static LPDIRECTSOUND8 SoundInterface;											//DirectSound�̃C���^�[�t�F�[�X
	static LPDIRECTSOUNDBUFFER  PrimaryBuffer;										//DirectSound�̃v���C�}���o�b�t�@

	/***�n���h������***/
	//�@�\:�n���h���̐ݒ�	����:�E�B���h�E�n���h��		�߂�l:�Ȃ�
	void setHandle(HWND hWnd) { this->hWnd = hWnd; }								//�n���h���̐ݒ�
	//�@�\:�n���h���̎擾	����:�Ȃ�	�߂�l:�E�B���h�E�n���h��					//�n���h���̎擾
	HWND getHandle(void) { return this->hWnd; }

	/***�T�E���h�C���^�[�t�F�[�X����***/
	//�@�\:�T�E���h�C���^�[�t�F�[�X�̎擾	����:�Ȃ�	�߂�l:�T�E���h�C���^�[�t�F�[�X
	LPDIRECTSOUND8 getSoundInterface(void) { return this->SoundInterface; }			//�T�E���h�C���^�[�t�F�[�X�̎擾
	BOOL checkSoundInterface(void);													//�T�E���h�C���^�[�t�F�[�X�̃`�F�b�N

public:
	/***�R���X�g���N�^�ƃf�X�g���N�^***/
	CSOUND(void) { }																//�f�t�H���g�R���X�g���N�^
	CSOUND(HWND hWnd);																//�R���X�g���N�^
	virtual ~CSOUND(void);															//�f�X�g���N�^

	/***�T�E���h����***/
	/*
	virtual SOUND_ERROR play(void) { return eSOUND_OK; }							//�Đ�(�ʏ�Đ������ݒ�ł̓��[�v�Đ�)
	virtual SOUND_ERROR stop(void) { return eSOUND_OK; }							//��~
	*/
};