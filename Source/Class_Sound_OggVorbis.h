//-------------------------------------------------------------------------------------------------------------------------------
//�t�@�C����	:	Class_Sound_OggVorbis.h
//�쐬��		:	�u���L
//�쐬��		:	08/04/28
//�@�\			:	CSound.cpp�ŕK�v�Ȑ錾�A��`���s���Ă��܂��B
//���l			:	CSound�N���X����h�����Ċe�T�E���h�o�b�t�@���쐬����܂��B
//					���ACSound�N���X�Őݒ肳�ꂽ�f�[�^�����L���Ă��܂��B
//�X�V����		:	08/04/28	Ver0.01.00	��{�@�\�������B
//					08/05/08	Ver0.11.00	Ogg Vorbis�ɑΉ��B
//					08/05/12	Ver0.21.00	OggVorbis�������������N���X�Ƃ��ēƗ��B
//					08/06/26	Ver0.31.00	�G�t�F�N�g�ɑΉ��B
//-------------------------------------------------------------------------------------------------------------------------------
//�v���O�}�w��

#pragma once
#pragma warning(disable:4996)

//�w�b�_�[�t�@�C���̃C���N���[�h
#include "Class_Sound.h"
#include "Class_OggVorbis.h"
#include <process.h>

//-------------------------------------------------------------------------------------------------------------------------------
//�N���X��		:	CSTATICSOUND_OGGVORBIS
//�쐬��		:	�u���L
//�쐬��		:	08/05/01
//�@�\			:	�T�E���h�֘A�����B
//���l			:	OggVorbis���X�^�e�B�b�N�o�b�t�@�ōĐ����܂��B
//�X�V����		:	08/05/08	Ver1.01.00	�@�\������
//					08/06/26	Ver1.11.00	�G�t�F�N�g�ɑΉ��B
//-------------------------------------------------------------------------------------------------------------------------------
class CSTATICSOUND_OGGVORBIS : public CSOUND {
private:
	/***�����̍Đ��ɕK�v�ȃf�[�^***/
	LPDIRECTSOUNDBUFFER8 DSBuffer;													//DirectSound�Z�J���_���o�b�t�@(�T�E���h�̃f�[�^)
	COGG_VORBIS OggVorbis;															//OggVorbis�֘A
	char szFileName[MAX_PATH];														//�ǂݍ��ރT�E���h�t�@�C���̃p�X
	DSBUFFERDESC dsbd;																//�T�E���h�f�[�^�̃f�o�C�X���
	WAVEFORMATEX wfex;																//�T�E���h�f�[�^�̃t�H�[�}�b�g
	DWORD dataSize;																	//�T�E���h�f�[�^�̃T�C�Y

	/***DirectSound�֘A***/
	SOUND_ERROR initialize(void);													//�f�[�^������
	SOUND_ERROR initSoundBuffer(const LPSTR szFileName);							//�T�E���h�o�b�t�@�̍쐬
	SOUND_ERROR releaseSoundBuffer(void);											//�T�E���h�o�b�t�@�̉��
	BOOL checkSoundBuffer(void);													//�o�b�t�@�̃`�F�b�N
public:
	/***�R���X�g���N�^�ƃf�X�g���N�^***/
	CSTATICSOUND_OGGVORBIS(void) { this->initialize(); }							//�f�t�H���g�R���X�g���N�^
	CSTATICSOUND_OGGVORBIS(const LPSTR szFileName);									//�R���X�g���N�^
	virtual ~CSTATICSOUND_OGGVORBIS(void);											//�f�X�g���N�^

	/***�f�[�^����***/
	LPSTR getFileName(void) { return this->szFileName; }							//�t�@�C�����擾

	/***�f�[�^�T�C�Y����***/
	//�@�\:�T�E���h�f�[�^�̃T�C�Y�擾	����:�Ȃ�	�߂�l:�T�E���h�f�[�^�̃T�C�Y(byte�P��)
	DWORD getSoundDataSize(void) { return this->dataSize; }						//�T�E���h�f�[�^�̃T�C�Y��Ԃ�(byte�P��)
	//�@�\:�T�E���h�f�[�^�̃T�C�Y�擾	����:�Ȃ�	�߂�l:�T�E���h�f�[�^�̃T�C�Y(�b�P��)
	DWORD getSoundDataSizeSec(void) {												//�T�E���h�f�[�^�̃T�C�Y��Ԃ�(�b�P��)
		return this->getSoundDataSize() / this->wfex.nAvgBytesPerSec;
	}

	/***�Đ��ʒu����***/
	//�@�\:�Đ��ʒu�̎擾	����:�Ȃ�	�߂�l:�Đ��ʒu(byte�P��)
	DWORD getPlayCur(void);															//�Đ��ʒu�̎擾(byte�P��)
	//�@�\:�Đ��ʒu�̎擾	����:�Ȃ�	�߂�l:�Đ��ʒu(�b�P��)
	DWORD getPlayCurSec(void) {														//�Đ��ʒu�̎擾(�b�P��)
		return this->getPlayCur() / this->wfex.nAvgBytesPerSec;						//�Đ����Ԃ�b�P�ʂŕԂ�
	}
	//�@�\:�Đ��ʒu�̐ݒ�	����:�Đ��ʒu(byte�P��)	�߂�l:�Ȃ�
	void setPlayCur(DWORD dwPosition) {												//�Đ��ʒu�̐ݒ�(byte�P��)
		this->DSBuffer->SetCurrentPosition(dwPosition);								//�Đ��ʒu��ύX����
	}
	//�@�\:�Đ��ʒu�̐ݒ�	����:�Đ��ʒu(�b�P��)	�߂�l:�Ȃ�
	void setPlayCurSec(DWORD dwSec) {												//�Đ��ʒu�̐ݒ�(�b�P��)
		this->DSBuffer->SetCurrentPosition(dwSec									//�Đ��ʒu��ύX����
						* this->wfex.nAvgBytesPerSec);
	}

	/***�Đ���Ԃ̊m�F***/
	//�@�\:�Đ���Ԃ̊m�F	����:�Ȃ�	�߂�l:�Đ���Ԃ������l(DSBSTATUS_PLAYING�������Ă��鎞�͍Đ���)
	DWORD getSoundPram(void) { DWORD dwStatus;
	this->DSBuffer->GetStatus(&dwStatus); return dwStatus; }

	/***�T�E���h����***/
	SOUND_ERROR play(DWORD dwPriority, DWORD dwFlag);								//�����Đ�
	SOUND_ERROR stop(void);															//�Đ���~
	SOUND_ERROR setEffect(GUID guidEffect, DWORD dwPriority, DWORD dwFlag);			//�G�t�F�N�g�̐ݒ�
	SOUND_ERROR deleteEffect(DWORD dwPriority, DWORD dwFlag);						//�ݒ肵���G�t�F�N�g���폜����
	//�@�\:�{�����[���̎擾		����:�Ȃ�			�߂�l:�{�����[��
	LONG getVolume(void) { LONG vol; this->DSBuffer->GetVolume(&vol); return vol; }
	//�@�\:�{�����[���̐ݒ�		����:�{�����[��		�߂�l:�Ȃ�
  	void setVolume(LONG vol) { this->DSBuffer->SetVolume(vol); }
	//�@�\:�p���̎擾		����:�Ȃ�			�߂�l:�p��
	LONG getPan(void) { LONG pan; this->DSBuffer->GetPan(&pan); return pan; }
	//�@�\:�p���̐ݒ�		����:�p��	�߂�l:�Ȃ�
	void setPan(LONG pan) { this->DSBuffer->SetPan(pan); }
};

//-------------------------------------------------------------------------------------------------------------------------------
//�N���X��		:	CSTREAMINGSOUND_OGGVORBIS
//�쐬��		:	�u���L
//�쐬��		:	08/05/12
//�@�\			:	�T�E���h�֘A�����B
//���l			:	OggVorbis���X�g���[�~���O�o�b�t�@�ōĐ����܂��B
//					�X�g���[�~���O�̓X���b�h�ɂ���čs���Ă���A�X���b�h�Ԃ̓����ɂ̓N���e�B�J���Z�N�V������p���Ă��܂��B
//�X�V����		:	08/05/12	Ver1.00.00	�@�\������
//					08/06/26	Ver1.11.00	�G�t�F�N�g�ɑΉ��B
//-------------------------------------------------------------------------------------------------------------------------------
class CSTREAMINGSOUND_OGGVORBIS : public CSOUND {
private:
	/***�����̍Đ��ɕK�v�ȃf�[�^***/
	LPDIRECTSOUNDBUFFER8 DSBuffer;													//DirectSound�Z�J���_���o�b�t�@(�T�E���h�̃f�[�^)
	COGG_VORBIS OggVorbis;															//OggVorbis�֘A
	char szFileName[MAX_PATH];														//�ǂݍ��ރT�E���h�t�@�C���̃p�X
	DSBUFFERDESC dsbd;																//�T�E���h�f�[�^�̃f�o�C�X���
	WAVEFORMATEX wfex;																//�T�E���h�f�[�^�̃t�H�[�}�b�g
	DWORD dataSize;																	//�T�E���h�f�[�^�̃T�C�Y

	/***�X�g���[�~���O�p�f�[�^***/
	LPDIRECTSOUNDNOTIFY8 DSNotify;													//DirectSoundNotify8�C���^�[�t�F�[�X

	/***�C�x���g�֘A***/
	HANDLE hNotifyEvent[kEVENTMAX];													//�C�x���g�̃n���h��
	DWORD blockNum;																	//�ʒm��
	DWORD blockSize;																//�ʒm�����Ԋu

	/***�X���b�h�֘A***/
	HANDLE hNotifyThread;															//�X���b�h�̃n���h��
	unsigned int threadId;															//�X���b�hID(���ʎq)

	/***�����֘A***/
	CRITICAL_SECTION cs;															//�N���e�B�J���Z�N�V����

	/***�X�g���[�~���O�Đ��Ɏg�p***/
	DWORD bufferSize;																//�X�g���[�~���O�p�o�b�t�@�̃T�C�Y(byte�P��)
	DWORD readCur;																	//�ǂݍ��݃J�[�\��(�t�@�C���̓ǂݍ��݈ʒu)
	DWORD writeCur;																	//�������݃J�[�\��(�o�b�t�@�ɏ������ވʒu)
	DWORD playCnt;																	//�Đ��񐔂�ۑ�
	DWORD playCur;																	//���݂̍Đ��ʒu(��܂��Ȓl)
	PLAY_FLAG playFlag;																//�Đ��t���O(���[�v�Đ����ǂ���)

	/***DirectSound�֘A***/
	SOUND_ERROR initialize(void);													//�f�[�^������
	SOUND_ERROR initSoundBuffer(const LPSTR szFileName);							//�T�E���h�o�b�t�@�̍쐬
	SOUND_ERROR releaseSoundBuffer(void);											//�T�E���h�o�b�t�@�̉��
	SOUND_ERROR setStreaming(void);													//�X�g���[�~���O���s�����߂̐ݒ������
	BOOL checkSoundBuffer(void);													//�o�b�t�@�̃`�F�b�N	
public:
	/***�R���X�g���N�^�ƃf�X�g���N�^***/
	CSTREAMINGSOUND_OGGVORBIS(void) { this->initialize(); }							//�f�t�H���g�R���X�g���N�^
	CSTREAMINGSOUND_OGGVORBIS(const LPSTR szFileName);								//�R���X�g���N�^
	virtual ~CSTREAMINGSOUND_OGGVORBIS(void);										//�f�X�g���N�^

	/***�f�[�^����***/
	LPSTR getFileName(void) { return this->szFileName; }							//�t�@�C�����擾

	/***�f�[�^�T�C�Y����***/
	//�@�\:�T�E���h�f�[�^�̃T�C�Y�擾	����:�Ȃ�	�߂�l:�T�E���h�f�[�^�̃T�C�Y(byte�P��)
	DWORD getSoundDataSize(void) { return this->dataSize; }							//�T�E���h�f�[�^�̃T�C�Y��Ԃ�(byte�P��)
	//�@�\:�T�E���h�f�[�^�̃T�C�Y�擾	����:�Ȃ�	�߂�l:�T�E���h�f�[�^�̃T�C�Y(�b�P��)
	DWORD getSoundDataSizeSec(void) {												//�T�E���h�f�[�^�̃T�C�Y��Ԃ�(�b�P��)
		return this->getSoundDataSize() / this->wfex.nAvgBytesPerSec;
	}

	/***�Đ��ʒu����***/
	//�@�\:�Đ��ʒu�̎擾	����:�Ȃ�	�߂�l:�Đ��ʒu(byte�P��)
	DWORD getPlayCur(void);															//�Đ��ʒu�̎擾(byte�P��)
	//�@�\:�Đ��ʒu�̎擾	����:�Ȃ�	�߂�l:�Đ��ʒu(�b�P��)
	DWORD getPlayCurSec(void) {														//�Đ��ʒu�̎擾(�b�P��)
		return this->getPlayCur() / this->wfex.nAvgBytesPerSec;						//�Đ����Ԃ�b�P�ʂŕԂ�
	}
	//�@�\:�Đ��ʒu�̐ݒ�	����:�Đ��ʒu(byte�P��)	�߂�l:�Ȃ�
	void setPlayCur(DWORD dwPosition) {												//�Đ��ʒu�̐ݒ�(byte�P��)
		//������
	}
	//�@�\:�Đ��ʒu�̐ݒ�	����:�Đ��ʒu(�b�P��)	�߂�l:�Ȃ�
	void setPlayCurSec(DWORD dwSec) {												//�Đ��ʒu�̐ݒ�(�b�P��)
		//������
	}

	/***�Đ���Ԃ̊m�F***/
	//�@�\:�Đ���Ԃ̊m�F	����:�Ȃ�	�߂�l:�Đ���Ԃ������l(DSBSTATUS_PLAYING�������Ă��鎞�͍Đ���)
	DWORD getSoundPram(void) { DWORD dwStatus;
	this->DSBuffer->GetStatus(&dwStatus); return dwStatus; }

	/***�X�g���[�~���O�֘A***/
	unsigned int WINAPI NotifyProc(void);											//�X�g���[�~���O���s��
	SOUND_ERROR readStream(void);													//�X�g���[�~���O���ł̃f�[�^�ǂݍ��ݏ���

	/***�T�E���h����***/
	SOUND_ERROR play(DWORD dwPriority, DWORD dwFlag);								//�����Đ�
	SOUND_ERROR stop(void);															//�Đ���~
	SOUND_ERROR setEffect(GUID guidEffect, DWORD dwPriority, DWORD dwFlag);			//�G�t�F�N�g�̐ݒ�
	SOUND_ERROR deleteEffect(DWORD dwPriority, DWORD dwFlag);						//�ݒ肵���G�t�F�N�g���폜����
	//�@�\:�{�����[���̎擾		����:�Ȃ�			�߂�l:�{�����[��
	LONG getVolume(void) { LONG vol; this->DSBuffer->GetVolume(&vol); return vol; }
	//�@�\:�{�����[���̐ݒ�		����:�{�����[��		�߂�l:�Ȃ�
	void setVolume(LONG vol) { this->DSBuffer->SetVolume(vol); }
	//�@�\:�p���̎擾		����:�Ȃ�			�߂�l:�p��
	LONG getPan(void) { LONG pan; this->DSBuffer->GetPan(&pan); return pan; }
	//�@�\:�p���̐ݒ�		����:�p��	�߂�l:�Ȃ�
	void setPan(LONG pan) { this->DSBuffer->SetPan(pan); }
};

//�֐��錾
//�N���X���̃X���b�h�𗘗p���邽�߂Ɏg�p
unsigned int WINAPI Notify(LPVOID lpParam);

//�ȗ�
#define	SSTATIC			CSTATICSOUND_OGGVORBIS
#define	SSTREAMING		CSTREAMINGSOUND_OGGVORBIS