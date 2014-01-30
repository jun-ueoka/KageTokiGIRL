//-------------------------------------------------------------------------------------------------------------------------------
//�t�@�C����	:	Class_SoundOggVorbis.cpp
//�쐬��		:	�u���L
//�쐬��		:	08/04/28
//�@�\			:	�����f�[�^�̍Đ��B
//���l			:	OggVorbis���������߂̑���⏈���̒񋟁B
//					CSound�N���X�̃f�[�^�����L���g�p���Ă��܂��B
//�X�V����		:	08/04/28	Ver0.01.00	DirectSound�̏����������������B
//					08/05/08	Ver0.11.00	Ogg Vorbis�ɑΉ��B
//-------------------------------------------------------------------------------------------------------------------------------
//�w�b�_�[�t�@�C���̃C���N���[�h

#include	"Precompiler.h"

//#include	"Class_Sound_OggVorbis.h"

//-------------------------------------------------------------------------------------------------------------------------------
//	�e�N�X�`�����\�[�X�t�@�C���ւ̃p�X
//-------------------------------------------------------------------------------------------------------------------------------
#define		PATH_SSTATIC_DATA		"..//Resources//Sound//Static//%s.ogg"
#define		PATH_SSTREAMING_DATA	"..//Resources//Sound//Streaming//%s.ogg"

/***�X�^�e�B�b�N�o�b�t�@***/
//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	CSTATICSOUND_OGGVORBIS
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�T�E���h�֘A�����B
//���l			:	CSTATICSOUND_OGGVORBIS�̃R���X�g���N�^�ł��B
//�N���X		:	CSTATICSOUND_OGGVORBIS
//����			:	const LPSTR	szFileName		�ǂݍ��ރt�@�C����
//�߂�l		:	�Ȃ�
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
CSTATICSOUND_OGGVORBIS::CSTATICSOUND_OGGVORBIS(const LPSTR szFileName)
{
	this->initialize();													//�f�[�^������

	this->initSoundBuffer(szFileName);
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	~CSTATICSOUND_OGGVORBIS
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�T�E���h�֘A�����B
//���l			:	CSTATICSOUND_OGGVORBIS�̃f�X�g���N�^�ł��B
//�N���X		:	CSTATICSOUND_OGGVORBIS
//����			:	�Ȃ�
//�߂�l		:	�Ȃ�
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
CSTATICSOUND_OGGVORBIS::~CSTATICSOUND_OGGVORBIS()
{
	this->releaseSoundBuffer();											//�T�E���h�o�b�t�@�̊J��
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	initialize
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�T�E���h�֘A�����B
//���l			:	�f�[�^�����������܂��B
//�N���X		:	CSTATICSOUND_OGGVORBIS
//����			:	�Ȃ�
//�߂�l		:	�Ȃ�
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTATICSOUND_OGGVORBIS::initialize()
{
	//�f�[�^��������
	this->DSBuffer = NULL;												//�o�b�t�@��������
	ZeroMemory(&this->szFileName, sizeof(this->szFileName));			//�T�E���h�t�@�C������������
	ZeroMemory(&this->dsbd, sizeof(DSBUFFERDESC));						//�f�o�C�X�f�[�^��������
	ZeroMemory(&this->wfex, sizeof(WAVEFORMATEX));						//�t�H�[�}�b�g����������
	this->dataSize = 0;													//�f�[�^�T�C�Y��������
	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	initSoundBuffer
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�T�E���h�֘A�����B
//���l			:	OggVorbis���Đ����邽�߂̏������s���܂��B
//�N���X		:	CSTATICSOUND_OGGVORBIS
//����			:	const LPSTR szFileName	�ǂݍ��ރt�@�C����
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//					08/05/12	ver1.10.00	OggVorbis�̏�����Ɨ��B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTATICSOUND_OGGVORBIS::initSoundBuffer(const LPSTR szFileName)
{
//	�T�E���h�p�X�̓Y�t
	TCHAR			realName[MAX_PATH]	= "";							//!< ���O�Ƀp�X�������

//	�p�X�t���T�E���h�t�@�C�����𐶐�
	sprintf_s(realName, sizeof(realName), PATH_SSTATIC_DATA, szFileName);

	//�C���^�[�t�F�[�X�����݂��邩����
	if(!this->checkSoundInterface()) {									//�C���^�[�t�F�[�X�����݂��Ȃ�
		return eSOUND_NOTINTERFACE;
	}

	//--------------------------------------------------
	//OggVorbis�̓ǂݍ���
	//--------------------------------------------------
	//OggVorbis�t�@�C���I�[�v��
	if(this->OggVorbis.openOggVorbis(realName) != eOGG_OK) {	//OggVorbis�t�@�C���̃I�[�v���Ɏ��s
		return eSOUND_OPEN;												//�G���[����Ԃ�
	}

	//�ǂݍ��ރt�@�C������ݒ�
	strcpy_s(this->szFileName, sizeof(this->szFileName), szFileName);

	this->wfex = this->OggVorbis.getWaveFormat();						//WAVE�t�H�[�}�b�g�̎擾
	this->dataSize = this->OggVorbis.getDataSize();						//�����f�[�^�̃T�C�Y���擾

	//--------------------------------------------------
	//�Z�J���_���o�b�t�@�̍쐬
	//--------------------------------------------------
	//�������m��
	LPDIRECTSOUNDBUFFER pDSBuffer;										//DirectSound�o�b�t�@(DirectSoundBuffer8�擾�Ɏg�p)
	stSoundBuffer sBuffer;												//�����f�[�^��ǂݍ��ނ��߂̃o�b�t�@
	ZeroMemory(&sBuffer, sizeof(stSoundBuffer));						//�o�b�t�@�̏�����

	//�f�o�C�X�̐ݒ�
	ZeroMemory(&this->dsbd, sizeof(this->dsbd));						//�f�o�C�X����0�N���A

	this->dsbd.dwSize = sizeof(DSBUFFERDESC);							//�\���̂̃T�C�Y��ݒ�
	this->dsbd.dwFlags = DSBCAPS_CTRLPAN								//�T�E���h�o�b�t�@�̔\�͂�ݒ�
							| DSBCAPS_CTRLVOLUME
							| DSBCAPS_LOCDEFER
							| DSBCAPS_CTRLFX
							| DSBCAPS_GETCURRENTPOSITION2;
	this->dsbd.dwBufferBytes = this->dataSize;							//�o�b�t�@�̃T�C�Y�̐ݒ�
	this->dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;						//3D�����̃A���S���Y��
	this->dsbd.lpwfxFormat = &this->wfex;								//�t�H�[�}�b�g���̐ݒ�
	
	//�o�b�t�@�̐���
	if(FAILED(this->SoundInterface->CreateSoundBuffer(					//���s�����ꍇ
							&this->dsbd, &pDSBuffer, NULL))) {
       return eSOUND_CREATEBUFFER;
	}
	//DirectSoundBuffer8�擾
	if(FAILED(pDSBuffer->QueryInterface(								//���s�����ꍇ
		IID_IDirectSoundBuffer8, (LPVOID*)&this->DSBuffer))) {
		kRELEASE(pDSBuffer);											//�o�b�t�@���J��
		return eSOUND_CREATEBUFFER;
	}
	kRELEASE(pDSBuffer);												//�o�b�t�@���J��

	//�o�b�t�@�̃��b�N
	if(FAILED(this->DSBuffer->Lock(0, this->dataSize,					//�o�b�t�@�̃��b�N�Ɏ��s
			&sBuffer.pBuffer, &sBuffer.dwBufferSize, NULL, NULL, 0))) {
	   return eSOUND_LOCK;
	}

	//�f�[�^�̓ǂݍ��ݏ���
	if(this->OggVorbis.readData(&sBuffer,								//�f�[�^�̓ǂݍ���
									sBuffer.dwBufferSize) != eOGG_OK) {
		return eSOUND_READ;
	}
	
	//�o�b�t�@�̃A�����b�N
	if(FAILED(this->DSBuffer->Unlock(sBuffer.pBuffer,					//�A�����b�N�Ɏ��s
							sBuffer.dwBufferSize, NULL, 0))) {
		return eSOUND_UNLOCK;											//�G���[����Ԃ�
	}


	//OggVorbis�t�@�C���N���[�Y
	if(this->OggVorbis.closeOggVorbis() != eOGG_OK) {					//OggVorbis�t�@�C���̃N���[�Y�Ɏ��s
		return eSOUND_CLOSE;											//�G���[����Ԃ�
	}
	this->setVolume(-2000);

	return eSOUND_OK;													//���퐬��
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	releaseSoundBuffer
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�T�E���h�֘A�����B
//���l			:	����������s���܂��B
//�N���X		:	CSTATICSOUND_OGGVORBIS
//����			:	�Ȃ�
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTATICSOUND_OGGVORBIS::releaseSoundBuffer()
{
	kRELEASE(this->DSBuffer);											//�T�E���h�o�b�t�@�̉��
	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	checkSoundBuffer
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�T�E���h�֘A�����B
//���l			:	�o�b�t�@������ȏ�Ԃ��`�F�b�N���܂��B
//�N���X		:	CSTATICSOUND_OGGVORBIS
//����			:	�Ȃ�
//�߂�l		:	BOOL	TRUE	����
//							FALSE	�ُ�
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
BOOL CSTATICSOUND_OGGVORBIS::checkSoundBuffer()
{
	//�o�b�t�@������ɐ�������Ă��邩����
	if(!this->DSBuffer) {												//��������Ă��Ȃ��ꍇ
		return FALSE;													//�ُ�
	}
	return TRUE;														//����
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	play
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�T�E���h�֘A�����B
//���l			:	�T�E���h���Đ����܂��B
//					IDirectSoundBuffer8::Play����{�ƂȂ��Ă��܂��B
//					�����̗D�揇�ʂƍĐ����@���w�肷��t���OIDirectSoundBuffer8::Play�Ɠ����悤�Ɏg�p���Ă��������B
//�N���X		:	CSTATICSOUND_OGGVORBIS
//����			:	DWORD	dwPriority	�D�揇��(0����ԒႢ)
//					DWORD	dwFlag		�Đ����@���w�肷��t���O
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTATICSOUND_OGGVORBIS::play(DWORD dwPriority, DWORD dwFlag)
{
	//�o�b�t�@������ɐ�������Ă��邩����
	if(!this->checkSoundBuffer()) {										//��������Ă��Ȃ��ꍇ
		return eSOUND_NOTBUFFER;
	}
	HRESULT hr;															//�Đ��̖߂�l���i�[

	hr = this->DSBuffer->Play(NULL, dwPriority, dwFlag);				//�T�E���h�Đ�

	//�f�o�C�X���X�g�̔���
	if(hr == DSERR_BUFFERLOST) {										//�f�o�C�X�����݂��Ȃ�
		this->DSBuffer->Restore();										//�T�E���h�o�b�t�@�̕��������݂�
		this->DSBuffer->Play(NULL, dwPriority, dwFlag);					//�T�E���h�Đ�
	}
	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	stop
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�T�E���h�֘A�����B
//���l			:	�T�E���h���~���܂��B
//�N���X		:	CSTATICSOUND_OGGVORBIS
//����			:	�Ȃ�
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTATICSOUND_OGGVORBIS::stop()
{
	//�o�b�t�@������ɐ�������Ă��邩����
	if(!this->checkSoundBuffer()) {										//��������Ă��Ȃ��ꍇ
		return eSOUND_NOTBUFFER;
	}

	this->DSBuffer->Stop();												//�Đ���~

	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	setEffect
//�쐬��		:	�u���L
//�쐬��		:	08/06/26
//�@�\			:	�T�E���h�֘A�����B
//���l			:	�G�t�F�N�g��ݒ肵�܂��B
//					�G�t�F�N�g���g�p����ɂ͈�x�Đ����~�߂Ă���ݒ���s���K�v������܂��B
//					���̒��ł͈�x�Đ����~������A�G�t�F�N�g�̐ݒ���s���w�肳�ꂽ���@�ɏ]���čĂэĐ����s���܂��B
//�G�t�F�N�g���w�肷��t���O�ꗗ
//					GUID_DSFX_STANDARD_CHORUS		�R�[���X
//					GUID_DSFX_STANDARD_COMPRESSOR	�R���v���b�T 
//					GUID_DSFX_STANDARD_DISTORTION	�f�B�X�g�[�V����
//					GUID_DSFX_STANDARD_ECHO			�G�R�[
//					GUID_DSFX_STANDARD_FLANGER		�t�����W
//					GUID_DSFX_STANDARD_GARGLE		�K�[�O��
//					GUID_DSFX_STANDARD_I3DL2REVERB	Interactive 3D Level 2 ���o�[�u
//					GUID_DSFX_STANDARD_PARAMEQ		�p�����g���b�N �C�R���C�U
//					GUID_DSFX_WAVES_REVERB			Waves ���o�[�u
//�N���X		:	CSTATICSOUND_OGGVORBIS
//����			:	GUID guidEffect		�G�t�F�N�g�̎�ނ��w�肷��t���O
//					DWORD	dwPriority	�D�揇��(0����ԒႢ)
//					DWORD	dwFlag		�Đ����@���w�肷��t���O
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/06/26	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTATICSOUND_OGGVORBIS::setEffect(GUID guidEffect, DWORD dwPriority, DWORD dwFlag)
{
	HRESULT hr;															//�߂�l�`�F�b�N

	//�o�b�t�@������ɐ�������Ă��邩����
	if(!this->checkSoundBuffer()) {										//��������Ă��Ȃ��ꍇ
		return eSOUND_NOTBUFFER;
	}

	this->stop();														//�G�t�F�N�g��ݒ肷�邽�ߍĐ���~

	//DirectSoundBuffer�ɃG�t�F�N�g��ݒ�
	DSEFFECTDESC dsEffect[1];
	dsEffect[0].dwSize = sizeof(DSEFFECTDESC);							//�T�C�Y
	dsEffect[0].dwFlags = 0;											//�G�t�F�N�g���n�[�h�E�F�A�������邩�\�t�g�E�F�A�������邩
	dsEffect[0].guidDSFXClass = guidEffect;								//�G�t�F�N�g��GUID
	dsEffect[0].dwReserved1 = 0;										//�\��ς�
	dsEffect[0].dwReserved2 = 0;										//�\��ς�

	//�G�t�F�N�g��ݒ�
	DWORD dwResults[1];
	hr = this->DSBuffer->SetFX(1, dsEffect, dwResults);					//�G�t�F�N�g��ݒ�
	this->play(dwPriority, dwFlag);										//�Đ����ĊJ
	//�G�t�F�N�g�̐ݒ�ɐ�������������
	if(FAILED(hr)) {													//���s�����ꍇ
		return eSOUND_EFFECT;
	}

	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	deleteEffect
//�쐬��		:	�u���L
//�쐬��		:	08/06/26
//�@�\			:	�T�E���h�֘A�����B
//���l			:	���ׂẴG�t�F�N�g���폜���܂��B
//					�G�t�F�N�g���폜����ɂ͈�x�Đ����~�߂Ă���ݒ���s���K�v������܂��B
//					���̒��ł͈�x�Đ����~������A�G�t�F�N�g�̍폜���s���w�肳�ꂽ���@�ɏ]���čĂэĐ����s���܂��B
//�N���X		:	CSTATICSOUND_OGGVORBIS
//����			:	DWORD	dwPriority	�D�揇��(0����ԒႢ)
//					DWORD	dwFlag		�Đ����@���w�肷��t���O
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/06/26	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTATICSOUND_OGGVORBIS::deleteEffect(DWORD dwPriority, DWORD dwFlag)
{
	HRESULT hr;

	//�o�b�t�@������ɐ�������Ă��邩����
	if(!this->checkSoundBuffer()) {										//��������Ă��Ȃ��ꍇ
		return eSOUND_NOTBUFFER;
	}

	this->stop();														//�G�t�F�N�g��ݒ肷�邽�ߍĐ���~

	//�o�b�t�@�̂��ׂẴG�t�F�N�g���폜����
	hr = this->DSBuffer->SetFX(0, NULL, NULL);
	//�G�t�F�N�g�̍폜�ɐ�������������
	if(FAILED(hr)) {													//���s�����ꍇ
		return eSOUND_EFFECT;
	}

	this->play(dwPriority, dwFlag);										//�Đ����ĊJ

	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	getPlayCur
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�T�E���h�֘A�����B
//���l			:	�Đ��ʒu���擾���܂��B
//�N���X		:	CSTATICSOUND_OGGVORBIS
//����			:	DWORD	�Đ��ʒu
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
DWORD CSTATICSOUND_OGGVORBIS::getPlayCur()
{
	//�������m��
	DWORD pc, wc;														//�J�[�\���ʒu

	//�Đ��ʒu�擾
	this->DSBuffer->GetCurrentPosition(&pc, &wc);

	return pc;															//�Đ��ʒu��Ԃ�
}






/***�X�g���[�~���O�o�b�t�@***/
//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	CSTREAMINGSOUND_OGGVORBIS
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	CSTREAMINGSOUND_OGGVORBIS�N���X�̃R���X�g���N�^�ł��B
//���l			:	
//�N���X		:	CSTREAMINGSOUND_OGGVORBIS
//����			:	const LPSTR	szFileName	�ǂݍ��ރt�@�C����
//�߂�l		:	�Ȃ�
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
CSTREAMINGSOUND_OGGVORBIS::CSTREAMINGSOUND_OGGVORBIS(const LPSTR szFileName)
{
	this->initialize();													//�f�[�^������
	InitializeCriticalSection(&this->cs);								//�N���e�B�J���Z�N�V�����̏�����

	this->initSoundBuffer(szFileName);									//�o�b�t�@�̐���
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	~CSTREAMINGSOUND_OGGVORBIS
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	CSTREAMINGSOUND_OGGVORBIS�N���X�̃f�X�g���N�^�ł��B
//���l			:	����������s���Ă��܂��B
//�N���X		:	CSTREAMINGSOUND_OGGVORBIS
//����			:	�Ȃ�
//�߂�l		:	�Ȃ�
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
CSTREAMINGSOUND_OGGVORBIS::~CSTREAMINGSOUND_OGGVORBIS()
{
	this->releaseSoundBuffer();											//�T�E���h�o�b�t�@�̉��
	DeleteCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̉��
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	initialize
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�f�[�^�������B
//���l			:	
//�N���X		:	CSTREAMINGSOUND_OGGVORBIS
//����			:	�Ȃ�
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::initialize()
{
	//�f�[�^��������
	this->DSBuffer = NULL;												//�o�b�t�@��������
	ZeroMemory(&this->szFileName, sizeof(this->szFileName));			//�T�E���h�t�@�C������������
	ZeroMemory(&this->dsbd, sizeof(DSBUFFERDESC));						//�f�o�C�X�f�[�^��������
	ZeroMemory(&this->wfex, sizeof(WAVEFORMATEX));						//�t�H�[�}�b�g����������
	this->dataSize = 0;													//�f�[�^�T�C�Y��������

	//�X�g���[�~���O�֘A�f�[�^
	this->DSNotify = NULL;												//DirectSoundNotify8�C���^�[�t�F�[�X��������

	//�C�x���g�n���h����������
	for(int i = 0; i < kEVENTMAX; i++) {								//�n���h�������J��Ԃ�
		this->hNotifyEvent[i] = NULL;									//�C�x���g�n���h����������
	}

	this->blockNum = 0;													//�ʒm�񐔂�������
	this->blockSize = 0;												//�ʒm�����Ԋu��������

	this->hNotifyThread = NULL;											//�X���b�h�n���h����������
	this->threadId = 0;													//�X���b�hID��������

	this->bufferSize = 0;												//�o�b�t�@�T�C�Y��������
	this->readCur = 0;													//�ǂݍ��݃J�[�\����������
	this->writeCur = 0;													//�������݃J�[�\����������
	this->playCnt = 0;													//�Đ��񐔂�������
	this->playCur = 0;													//�Đ��ʒu��������
	this->playFlag = ePLAY_ONCE;										//�Đ��t���O��������(���݂̂̍Đ�)

	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	initSoundBuffer
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�o�b�t�@�̐����B
//���l			:	
//�N���X		:	CSTREAMINGSOUND_OGGVORBIS
//����			:	const LPSTR szFileName	�ǂݍ��ރt�@�C����
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::initSoundBuffer(const LPSTR szFileName)
{
//	�T�E���h�p�X�̓Y�t
	TCHAR			realName[MAX_PATH]	= "";							//!< ���O�Ƀp�X�������

//	�p�X�t���T�E���h�t�@�C�����𐶐�
	sprintf_s(realName, sizeof(realName), PATH_SSTREAMING_DATA, szFileName);

	//�C���^�[�t�F�[�X�����݂��邩����
	if(!this->checkSoundInterface()) {									//�C���^�[�t�F�[�X�����݂��Ȃ�
		return eSOUND_NOTINTERFACE;
	}

	//--------------------------------------------------
	//OggVorbis�̓ǂݍ���
	//--------------------------------------------------
	//OggVorbis�t�@�C���I�[�v��
	if(this->OggVorbis.openOggVorbis(realName) != eOGG_OK) {	//OggVorbis�t�@�C���̃I�[�v���Ɏ��s
		return eSOUND_OPEN;												//�G���[����Ԃ�
	}

	//�ǂݍ��ރt�@�C������ݒ�
	strcpy_s(this->szFileName, sizeof(this->szFileName), szFileName);

	this->wfex = this->OggVorbis.getWaveFormat();						//WAVE�t�H�[�}�b�g�̎擾
	this->dataSize = this->OggVorbis.getDataSize();						//�����f�[�^�̃T�C�Y���擾

	//�X�g���[�~���O�p�f�[�^��ݒ�
	this->blockNum = KBLOCKNUM;											//�ʒm��
	this->blockSize = this->wfex.nAvgBytesPerSec;						//�ʒm�����Ԋu(1�b��)

	//�T�E���h��3�b�ȏォ����(3�b�ȉ��̏ꍇ�̓X�g���[�~���O�͍s��Ȃ�)
	if(this->dataSize > this->blockSize * 3) {							//3�b�ȏ�̏ꍇ
		this->bufferSize = this->blockNum * this->blockSize;			//�o�b�t�@�̃T�C�Y��ݒ�
	} else {															//3�b�ȏ�̏ꍇ
		this->bufferSize = this->dataSize;								//�o�b�t�@�̃T�C�Y��ݒ�
	}

	//--------------------------------------------------
	//�Z�J���_���o�b�t�@�̍쐬
	//--------------------------------------------------
	//�������m��
	LPDIRECTSOUNDBUFFER pDSBuffer;										//DirectSound�o�b�t�@(DirectSoundBuffer8�擾�Ɏg�p)
	stSoundBuffer sBuffer;												//�����f�[�^��ǂݍ��ނ��߂̃o�b�t�@
	ZeroMemory(&sBuffer, sizeof(stSoundBuffer));						//�o�b�t�@�̏�����

	//�f�o�C�X�̐ݒ�
	ZeroMemory(&this->dsbd, sizeof(this->dsbd));						//�f�o�C�X����0�N���A

	this->dsbd.dwSize = sizeof(DSBUFFERDESC);							//�\���̂̃T�C�Y��ݒ�
	this->dsbd.dwFlags = DSBCAPS_CTRLPAN								//�T�E���h�o�b�t�@�̔\�͂�ݒ�
							| DSBCAPS_CTRLVOLUME
							| DSBCAPS_CTRLPOSITIONNOTIFY
							| DSBCAPS_LOCDEFER
							| DSBCAPS_CTRLFX
							| DSBCAPS_GETCURRENTPOSITION2;
	this->dsbd.dwBufferBytes = this->bufferSize;						//�o�b�t�@�̃T�C�Y�̐ݒ�
	this->dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;						//3D�����̃A���S���Y��
	this->dsbd.lpwfxFormat = &this->wfex;								//�t�H�[�}�b�g���̐ݒ�

	VOID *pBuffer;														//���b�N�����o�b�t�@�̈ʒu
	
	//�o�b�t�@�̐���
	if(FAILED(this->SoundInterface->CreateSoundBuffer(&this->dsbd,		//���s�����ꍇ
											&pDSBuffer, NULL))) {
       return eSOUND_CREATEBUFFER;
	}
	//DirectSoundBuffer8�擾
	if(FAILED(pDSBuffer->QueryInterface(								//���s�����ꍇ
		IID_IDirectSoundBuffer8, (LPVOID*)&this->DSBuffer))) {
		kRELEASE(pDSBuffer);											//�o�b�t�@���J��
		return eSOUND_CREATEBUFFER;
	}
	kRELEASE(pDSBuffer);												//�o�b�t�@���J��

	//�o�b�t�@�̃��b�N
	if(FAILED(this->DSBuffer->Lock(0, this->bufferSize,					//�o�b�t�@�̃��b�N�Ɏ��s
					&sBuffer.pBuffer, &sBuffer.dwBufferSize, &pBuffer, NULL, 0))) {
	   return eSOUND_LOCK;
	}

	//�f�[�^�̓ǂݍ��ݏ���
	if(this->OggVorbis.readData(&sBuffer,								//�f�[�^�̓ǂݍ���
									sBuffer.dwBufferSize) != eOGG_OK) {
		return eSOUND_READ;
	}
	
	//�o�b�t�@�̃A�����b�N
	if(FAILED(this->DSBuffer->Unlock(sBuffer.pBuffer,					//�A�����b�N�Ɏ��s
							sBuffer.dwBufferSize, NULL, 0))) {
		return eSOUND_UNLOCK;											//�G���[����Ԃ�
	}

	//�ǂݍ��݃J�[�\���Ə������݃J�[�\���̈ړ�
	this->readCur = this->OggVorbis.getReadPos();						//�ǂݍ��݃J�[�\�����ړ�
	this->writeCur = 0;													//�������݃J�[�\�����ړ�

	this->setVolume(-2000);
	//--------------------------------------------------
	//�X�g���[�~���O�̐ݒ�
	//--------------------------------------------------
	//�o�b�t�@�T�C�Y��3�b�ȏォ����(3�b�ȉ��̓X�g���[�~���O���s��Ȃ�)
	if(this->bufferSize >= this->blockSize * 3) {						//3�b�ȏ�
		return this->setStreaming();									//�X�g���[�~���O�̐ݒ���s��
	}
	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	setStreaming
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�ݒ菈���B
//���l			:	�X�g���[�~���O���s�����߂̐ݒ���s���Ă��܂��B
//�N���X		:	CSTREAMINGSOUND_OGGVORBIS
//����			:	�Ȃ�
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/07/03	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::setStreaming()
{
	DSBPOSITIONNOTIFY DsPos[KBLOCKNUM + 1];								//�ʒm�ʒu���

	//DirectSoundNotify�C���^�[�t�F�[�X�̎擾
	if(FAILED(this->DSBuffer->QueryInterface(IID_IDirectSoundNotify8,	//�擾�Ɏ��s
											(LPVOID*)&this->DSNotify))) {
		return eSOUND_QUERYINTERFACE;									//�G���[����Ԃ�
	}

	//�C�x���g�I�u�W�F�N�g�̍쐬
	for(int i = 0; i < kEVENTMAX; i++) {								//�C�x���g�����J��Ԃ�
		this->hNotifyEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);	//�C�x���g�I�u�W�F�N�g�̍쐬
	}

	//�ʒm�ʒu�̐ݒ�
	//�Đ��J�[�\�����w�肳�ꂽ�I�t�Z�b�g���߂����Ƃ��ɃC�x���g�𔭐�������
	int iBlock = 1;														//�ݒ肷��u���b�N��
	for(int i = 0; i < KBLOCKNUM; i++) {								//�Ō��1�������ăC�x���g��ݒ�
		DsPos[i].dwOffset = this->blockSize * iBlock - 1;				//�u���b�N�����O
		DsPos[i].hEventNotify = this->hNotifyEvent[0];					//�C�x���g�I�u�W�F�N�g�̐ݒ�
		iBlock++;														//�u���b�N�����A�b�v
	}
	DsPos[KBLOCKNUM].dwOffset = DSBPN_OFFSETSTOP;						//�Ō�̈�͍Đ���~���o�p
	DsPos[KBLOCKNUM].hEventNotify = this->hNotifyEvent[1];				//�C�x���g�I�u�W�F�N�g�̐ݒ�

	//�C�x���g�̐ݒ�
	if(FAILED(this->DSNotify->SetNotificationPositions(					//�ݒ�Ɏ��s
										KBLOCKNUM + 1, DsPos))){
		return eSOUND_SETNOTIFICATIONPOSITIONS;							//�G���[����Ԃ�
	}

	//�X���b�h�̍쐬
	this->hNotifyThread = (HANDLE)_beginthreadex(NULL, 0, Notify, 
				reinterpret_cast<void *>(this), 0, &this->threadId);

	//�X���b�h�̗D�揇�ʂ̐ݒ�
	SetThreadPriority(this->hNotifyThread, THREAD_PRIORITY_ABOVE_NORMAL);

	return eSOUND_OK;													//���퐬��
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	releaseSoundBuffer
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	��������B
//���l			:	�X���b�h��o�b�t�@�̉���������s���Ă��܂��B
//�N���X		:	CSTREAMINGSOUND_OGGVORBIS
//����			:	�Ȃ�
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::releaseSoundBuffer()
{
	DWORD dwExitCode;													//�X���b�h�̏I���R�[�h

	EnterCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̊J�n

	//�I������
	this->stop();														//�Đ���~

	this->OggVorbis.closeOggVorbis();									//OggVorbis�t�@�C���̃N���[�Y

	//�X���b�h�����݂��邩����
	if(this->hNotifyThread) {											//�X���b�h�����݂���
		//�X���b�h���I��
		PostThreadMessage(this->threadId, WM_END_THREAD, 0, 0);			//�X���b�h�ɏI�����b�Z�[�W�𑗂�
		/**/
		WaitForSingleObject(this->hNotifyThread, kTIMEOUT);				//�X���b�h���I������܂ň�莞�ԑҋ@
		GetExitCodeThread(this->hNotifyThread, &dwExitCode);			//�X���b�h�̏�Ԃ��擾
		//�X���b�h���܂������Ă��邩����
		if(dwExitCode == STILL_ACTIVE) {								//�X���b�h���ғ���
			//�X���b�h������I�ɏI������Ă��Ȃ��ꍇ�͋����I��
			if(!TerminateThread(this->hNotifyThread, 0)) {				//�����I���Ɏ��s
				WaitForSingleObject(this->hNotifyThread, kTIMEOUT);		//�X���b�h���I������܂őҋ@(���������҂��Ă݂�)
				TerminateThread(this->hNotifyThread, 0);				//�X���b�h�������I��(����ŏI�����Ȃ���Β��߂�)
			}
		}
		
	}
/**/

	//�C�x���g�n���h���̃N���[�Y
	for(int i = 0; i < kEVENTMAX; i++) {								//�C�x���g�����J��Ԃ�
		kCLOSEHANDLE(this->hNotifyEvent[i]);							//�C�x���g�n���h�������
	}

	//�X���b�h�n���h���̃N���[�Y
	if(this->hNotifyThread != NULL) {									//�X���b�h�n���h�����������Ă��Ȃ�
		kCLOSEHANDLE(this->hNotifyThread);								//�X���b�h�n���h�������
	}

	kRELEASE(this->DSBuffer);											//�T�E���h�o�b�t�@�̉��

	LeaveCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̏I��

	return eSOUND_OK;													//����I��
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	checkSoundBuffer
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�T�E���h�֘A�����B
//���l			:	�o�b�t�@������ȏ�Ԃ��`�F�b�N���܂��B
//�N���X		:	CSTREAMINGSOUND_OGGVORBIS
//����			:	�Ȃ�
//�߂�l		:	BOOL	TRUE	����
//							FALSE	�ُ�
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
BOOL CSTREAMINGSOUND_OGGVORBIS::checkSoundBuffer()
{
	EnterCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̊J�n

	//�o�b�t�@������ɐ�������Ă��邩����
	if(!this->DSBuffer) {												//��������Ă��Ȃ��ꍇ
		LeaveCriticalSection(&this->cs);								//�N���e�B�J���Z�N�V�����̏I��
		return FALSE;													//�ُ�
	}

	LeaveCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̏I��
	return TRUE;														//����
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	play
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�T�E���h�֘A�����B
//���l			:	�T�E���h���Đ����܂��B
//					IDirectSoundBuffer8::Play����{�ƂȂ��Ă��܂��B
//					�����̗D�揇�ʂ�IDirectSoundBuffer8::Play�Ɠ����悤�Ɏg�p���Ă��������B
//					�Đ����@���w�肷��t���O�͈��݂̂̍Đ�(DSBPLAY_LOOPING���w�肵�Ȃ�)���w�肷�邱�Ƃ��\�ł��B
//�N���X		:	CSTREAMINGSOUND_OGGVORBIS
//����			:	DWORD	dwPriority	�D�揇��(0����ԒႢ)
//					DWORD	dwFlag		�Đ����@���w�肷��t���O
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::play(DWORD dwPriority, DWORD dwFlag)
{
	EnterCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̊J�n

	//�o�b�t�@������ɐ�������Ă��邩����
	if(!this->checkSoundBuffer()) {										//��������Ă��Ȃ��ꍇ
		LeaveCriticalSection(&this->cs);								//�N���e�B�J���Z�N�V�����̏I��
		return eSOUND_NOTBUFFER;
	}
	HRESULT hr;															//�Đ��̖߂�l���i�[

	//���[�v�Đ����ۂ�����
	if(dwFlag & DSBPLAY_LOOPING) {										//���[�v�Đ�
		this->playFlag = ePLAY_LOOPING;
		hr = this->DSBuffer->Play(NULL, dwPriority, dwFlag);			//�T�E���h�Đ�
	} else {															//1��̂ݍĐ�
		this->playFlag = ePLAY_ONCE;
		hr = this->DSBuffer->Play(NULL, dwPriority,
									dwFlag | DSBPLAY_LOOPING);			//�T�E���h�Đ�
	}



	//�f�o�C�X���X�g�̔���
	if(hr == DSERR_BUFFERLOST) {										//�f�o�C�X�����݂��Ȃ�
		this->DSBuffer->Restore();										//�T�E���h�o�b�t�@�̕��������݂�
		this->DSBuffer->Play(NULL, dwPriority,							//�T�E���h�Đ�
								dwFlag | DSBPLAY_LOOPING);
	}
	LeaveCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̏I��
	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	stop
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�T�E���h�֘A�����B
//���l			:	�T�E���h���~���܂��B
//�N���X		:	CSTREAMINGSOUND_OGGVORBIS
//����			:	�Ȃ�
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::stop()
{
	EnterCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̊J�n

	//�o�b�t�@������ɐ�������Ă��邩����
	if(!this->checkSoundBuffer()) {										//��������Ă��Ȃ��ꍇ
		LeaveCriticalSection(&this->cs);								//�N���e�B�J���Z�N�V�����̏I��
		return eSOUND_NOTBUFFER;
	}

	this->DSBuffer->Stop();												//�Đ���~
	LeaveCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̏I��

	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	setEffect
//�쐬��		:	�u���L
//�쐬��		:	08/06/26
//�@�\			:	�T�E���h�֘A�����B
//���l			:	�G�t�F�N�g��ݒ肵�܂��B
//					�G�t�F�N�g���g�p����ɂ͈�x�Đ����~�߂Ă���ݒ���s���K�v������܂��B
//					���̒��ł͈�x�Đ����~������A�G�t�F�N�g�̐ݒ���s���w�肳�ꂽ���@�ɏ]���čĂэĐ����s���܂��B
//�G�t�F�N�g���w�肷��t���O�ꗗ
//					GUID_DSFX_STANDARD_CHORUS		�R�[���X
//					GUID_DSFX_STANDARD_COMPRESSOR	�R���v���b�T 
//					GUID_DSFX_STANDARD_DISTORTION	�f�B�X�g�[�V����
//					GUID_DSFX_STANDARD_ECHO			�G�R�[
//					GUID_DSFX_STANDARD_FLANGER		�t�����W
//					GUID_DSFX_STANDARD_GARGLE		�K�[�O��
//					GUID_DSFX_STANDARD_I3DL2REVERB	Interactive 3D Level 2 ���o�[�u
//					GUID_DSFX_STANDARD_PARAMEQ		�p�����g���b�N �C�R���C�U
//					GUID_DSFX_WAVES_REVERB			Waves ���o�[�u
//�N���X		:	CSTREAMINGSOUND_OGGVORBIS
//����			:	GUID guidEffect		�G�t�F�N�g�̎�ނ��w�肷��t���O
//					DWORD	dwPriority	�D�揇��(0����ԒႢ)
//					DWORD	dwFlag		�Đ����@���w�肷��t���O
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/06/26	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::setEffect(GUID guidEffect, DWORD dwPriority, DWORD dwFlag)
{
	HRESULT hr;															//�߂�l�`�F�b�N

	EnterCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̊J�n

	//�o�b�t�@������ɐ�������Ă��邩����
	if(!this->checkSoundBuffer()) {										//��������Ă��Ȃ��ꍇ
		LeaveCriticalSection(&this->cs);								//�N���e�B�J���Z�N�V�����̏I��
		return eSOUND_NOTBUFFER;
	}

	this->stop();														//�G�t�F�N�g��ݒ肷�邽�ߍĐ���~

	//DirectSoundBuffer�ɃG�t�F�N�g��ݒ�
	DSEFFECTDESC dsEffect[1];
	dsEffect[0].dwSize = sizeof(DSEFFECTDESC);							//�T�C�Y
	dsEffect[0].dwFlags = 0;											//�G�t�F�N�g���n�[�h�E�F�A�������邩�\�t�g�E�F�A�������邩
	dsEffect[0].guidDSFXClass = guidEffect;								//�G�t�F�N�g��GUID
	dsEffect[0].dwReserved1 = 0;										//�\��ς�
	dsEffect[0].dwReserved2 = 0;										//�\��ς�

	//�G�t�F�N�g��ݒ�
	DWORD dwResults[1];
	hr = this->DSBuffer->SetFX(1, dsEffect, dwResults);					//�G�t�F�N�g��ݒ�
	this->play(dwPriority, dwFlag);										//�Đ����ĊJ
	//�G�t�F�N�g�̐ݒ�ɐ�������������
	if(FAILED(hr)) {													//���s�����ꍇ
		LeaveCriticalSection(&this->cs);								//�N���e�B�J���Z�N�V�����̏I��
		return eSOUND_EFFECT;
	}

	LeaveCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̏I��
	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	deleteEffect
//�쐬��		:	�u���L
//�쐬��		:	08/06/26
//�@�\			:	�T�E���h�֘A�����B
//���l			:	���ׂẴG�t�F�N�g���폜���܂��B
//					�G�t�F�N�g���폜����ɂ͈�x�Đ����~�߂Ă���ݒ���s���K�v������܂��B
//					���̒��ł͈�x�Đ����~������A�G�t�F�N�g�̍폜���s���w�肳�ꂽ���@�ɏ]���čĂэĐ����s���܂��B
//�N���X		:	CSTREAMINGSOUND_OGGVORBIS
//����			:	DWORD	dwPriority	�D�揇��(0����ԒႢ)
//					DWORD	dwFlag		�Đ����@���w�肷��t���O
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/06/26	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::deleteEffect(DWORD dwPriority, DWORD dwFlag)
{
	HRESULT hr;

	EnterCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̊J�n

	//�o�b�t�@������ɐ�������Ă��邩����
	if(!this->checkSoundBuffer()) {										//��������Ă��Ȃ��ꍇ
		LeaveCriticalSection(&this->cs);								//�N���e�B�J���Z�N�V�����̏I��
		return eSOUND_NOTBUFFER;
	}

	this->stop();														//�G�t�F�N�g��ݒ肷�邽�ߍĐ���~

	//�o�b�t�@�̂��ׂẴG�t�F�N�g���폜����
	hr = this->DSBuffer->SetFX(0, NULL, NULL);
	//�G�t�F�N�g�̍폜�ɐ�������������
	if(FAILED(hr)) {													//���s�����ꍇ
		LeaveCriticalSection(&this->cs);								//�N���e�B�J���Z�N�V�����̏I��
		return eSOUND_EFFECT;
	}

	this->play(dwPriority, dwFlag);										//�Đ����ĊJ

	LeaveCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̏I��
	return eSOUND_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	getPlayCur
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�T�E���h�֘A�����B
//���l			:	�Đ��ʒu���擾���܂�(�����̌덷����)�B
//					�Đ��ʒu���Ƀo�b�t�@��̏ڍׂȍĐ��ʒu���ŕ␳�����čĐ��ʒu�����v�Z���Ă��܂��B
//�N���X		:	CSTREAMINGSOUND_OGGVORBIS
//����			:	DWORD	�Đ��ʒu
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
DWORD CSTREAMINGSOUND_OGGVORBIS::getPlayCur()
{
	//�������m��
	DWORD pc;															//�J�[�\���ʒu
	DWORD dwPlayCur;													//�Đ��ʒu

	EnterCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̊J�n

	//�Đ��ʒu�擾
	this->DSBuffer->GetCurrentPosition(&pc, NULL);

	dwPlayCur = this->playCur + pc % this->blockSize;
	
	LeaveCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̏I��

	return dwPlayCur;													//�Đ��ʒu��Ԃ�
}

//-------------------------------------------------------------------------------------------------------------------------------
//�֐���		:	Notify
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�X�g���[�~���O���s�����\�b�h���Ăяo���B
//���l			:	�N���X���\�b�h�̃X�g���[�~���O�������Ăяo���B
//����			:	LPVOID		lpParam		�N���X�̃|�C���^
//�߂�l		:	unsigned int	WINAPI
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
unsigned int WINAPI Notify(LPVOID lpParam)
{
	//�X�g���[�~���O���s�����߂̃��\�b�h���Ăяo��
	CSTREAMINGSOUND_OGGVORBIS *pCSSound;								//�X�g���[�~���O�Ώۂ̃N���X
	pCSSound = (CSTREAMINGSOUND_OGGVORBIS *)lpParam;					//�X�g���[�~���O�Ώۂ̃N���X���N���ł���悤�ɂ���

	pCSSound->NotifyProc();												//�X���b�h���N��

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	NotifyProc
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�X�g���[�~���O�����B
//���l			:	�X�g���[�~���O���s���B
//�N���X		:	CSTREAMINGSOUND_OGGVORBIS
//����			:	�Ȃ�
//�߂�l		:	unsigned int	WINAPI
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
unsigned int WINAPI CSTREAMINGSOUND_OGGVORBIS::NotifyProc(void)
{
	DWORD dwResult;														//�C�x���g�̏�Ԃ��󂯎��
	MSG msg;															//���b�Z�[�W���󂯎��

//	DWORD dwExitCode;													//�X���b�h�̏I���R�[�h

	//�f�[�^���������ރ��[�v
	do {
		//�Đ��I�������m(�����덷���o��)
		if(this->getPlayCur() >= this->dataSize							//���݂̂̍Đ��ōĐ��I���ʒu�ɒB�����ꍇ
				&& this->playFlag == ePLAY_ONCE) {
			this->stop();												//�Đ����~
			this->playCur = 0;
		}
		//�X���b�h�Ƀ��b�Z�[�W�����Ă��邩���ׂ�
		if(PeekMessage(&msg, NULL,										//���b�Z�[�W�����Ă���
			WM_END_THREAD, WM_END_THREAD, PM_NOREMOVE)) {
			//���b�Z�[�W�𒲂ׂ� 
			switch(msg.message)
			{
				//�X���b�h���I��
				case WM_END_THREAD:
					_endthreadex(0);									//�X���b�h���I������
					/*
					WaitForSingleObject(this->hNotifyThread, kTIMEOUT);				//�X���b�h���I������܂ň�莞�ԑҋ@
					GetExitCodeThread(this->hNotifyThread, &dwExitCode);			//�X���b�h�̏�Ԃ��擾
					//�X���b�h���܂������Ă��邩����
					if(dwExitCode == STILL_ACTIVE) {								//�X���b�h���ғ���
						//�X���b�h������I�ɏI������Ă��Ȃ��ꍇ�͋����I��
						if(!TerminateThread(this->hNotifyThread, 0)) {				//�����I���Ɏ��s
							WaitForSingleObject(this->hNotifyThread, kTIMEOUT);		//�X���b�h���I������܂őҋ@(���������҂��Ă݂�)
							TerminateThread(this->hNotifyThread, 0);				//�X���b�h�������I��(����ŏI�����Ȃ���Β��߂�)
						}
					}
					
					//�C�x���g�n���h���̃N���[�Y
					for(int i = 0; i < kEVENTMAX; i++) {								//�C�x���g�����J��Ԃ�
						kCLOSEHANDLE(this->hNotifyEvent[i]);							//�C�x���g�n���h�������
					}

					//�X���b�h�n���h���̃N���[�Y
					if(this->hNotifyThread != NULL) {									//�X���b�h�n���h�����������Ă��Ȃ�
						kCLOSEHANDLE(this->hNotifyThread);								//�X���b�h�n���h�������
					}

					kRELEASE(this->DSBuffer);											//�T�E���h�o�b�t�@�̉��
					*/
					return 0;
			}
		}

		dwResult = MsgWaitForMultipleObjectsEx(kEVENTMAX,				//�C�x���g���擾
			this->hNotifyEvent,  INFINITE, QS_ALLEVENTS, 0);

		//�C�x���g�ɉ����ď�����U�蕪����
		switch(dwResult)
		{
			//�o�b�t�@�̍X�V
			case WAIT_OBJECT_0:
				this->readStream();										//�f�[�^��ǂݍ���
				break;
			//�Đ���~
			case WAIT_OBJECT_0 + 1:
				break;
			//�z��O�̏���
			default:
				break;
		}
	} while(TRUE);

	_endthreadex(0);													//�X���b�h���I������

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	readStream
//�쐬��		:	�u���L
//�쐬��		:	08/05/08
//�@�\			:	�X�g���[�~���O�����B
//���l			:	�X�g���[�~���O���ł̉����f�[�^�ǂݍ��ݏ������s���Ă��܂��B
//�N���X		:	CSTREAMINGSOUND_OGGVORBIS
//����			:	�Ȃ�
//�߂�l		:	SOUND_ERROR	�G���[���
//�X�V����		:	08/05/08	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
SOUND_ERROR CSTREAMINGSOUND_OGGVORBIS::readStream()
{
	//�������m��
	VOID *pBuffer;														//���b�N�����o�b�t�@�̈ʒu
	stSoundBuffer sBuffer;												//�����f�[�^��ǂݍ��ނ��߂̃o�b�t�@
	ZeroMemory(&sBuffer, sizeof(stSoundBuffer));						//�o�b�t�@�̏�����

	//�N���e�B�J���Z�N�V������p������������
	EnterCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̊J�n

	this->OggVorbis.seekData(this->readCur);							//�ǂݍ��݈ʒu���ړ�

	//�����f�[�^�̍Ō�����o
	if((this->readCur * (this->wfex.nChannels							//���������f�[�^�̏I�[
				* this->wfex.wBitsPerSample / 8))
				+ this->blockSize >= this->dataSize) {
		//�ŏ��Ɏc��̉����f�[�^��ǂݍ���
		DWORD dwDataSize;												//�c��̃f�[�^��
		dwDataSize = this->dataSize - (this->readCur
			* (this->wfex.nChannels * this->wfex.wBitsPerSample / 8));

		//�o�b�t�@�̃��b�N
		if(FAILED(this->DSBuffer->Lock(this->writeCur,					//�o�b�t�@�̃��b�N�Ɏ��s
						dwDataSize, &sBuffer.pBuffer, 
						&sBuffer.dwBufferSize, &pBuffer, NULL, 0))) {
			LeaveCriticalSection(&this->cs);							//�N���e�B�J���Z�N�V�����̏I��
			return eSOUND_LOCK;
		}

		//�f�[�^�̓ǂݍ��ݏ���
		if(this->OggVorbis.readData(&sBuffer,							//�f�[�^�̓ǂݍ���
									sBuffer.dwBufferSize) != eOGG_OK) {
			LeaveCriticalSection(&this->cs);							//�N���e�B�J���Z�N�V�����̏I��
			return eSOUND_READ;
		}
	
		//�o�b�t�@�̃A�����b�N
		if(FAILED(this->DSBuffer->Unlock(sBuffer.pBuffer,				//�A�����b�N�Ɏ��s
							sBuffer.dwBufferSize, NULL, 0))) {
			LeaveCriticalSection(&this->cs);							//�N���e�B�J���Z�N�V�����̏I��
			return eSOUND_UNLOCK;										//�G���[����Ԃ�
		}

		//�ǂݍ��݃J�[�\�����C��
		this->readCur = 0;												//�J�[�\���̈ʒu�������̐擪�ɖ߂�
		this->OggVorbis.seekData(this->readCur);						//�ǂݍ��݈ʒu���ړ�

		//�o�b�t�@�̎c��Ƀf�[�^����������
		//���[�v�Đ��Ȃ特���f�[�^�̐擪���珑�����݈��݂̂̍Đ��Ȃ疳���Ńo�b�t�@�𖄂߂�
		//�o�b�t�@�̃��b�N
		if(FAILED(this->DSBuffer->Lock(this->writeCur + dwDataSize,		//�o�b�t�@�̃��b�N�Ɏ��s
					this->blockSize - dwDataSize, &sBuffer.pBuffer, 
						&sBuffer.dwBufferSize, &pBuffer, NULL, 0))) {
			LeaveCriticalSection(&this->cs);							//�N���e�B�J���Z�N�V�����̏I��
			return eSOUND_LOCK;
		}

		//���[�v�Đ����ۂ�����
		if(this->playFlag == ePLAY_LOOPING) {
			//�f�[�^�̓ǂݍ��ݏ���
			if(this->OggVorbis.readData(&sBuffer,						//�f�[�^�̓ǂݍ���
					sBuffer.dwBufferSize) != eOGG_OK) {
				LeaveCriticalSection(&this->cs);						//�N���e�B�J���Z�N�V�����̏I��
				return eSOUND_READ;
			}
			this->readCur = this->OggVorbis.getReadPos();				//�ǂݍ��݃J�[�\�����ړ�
			this->playCur += this->blockSize;							//�Đ��ʒu�̍X�V
		} else {
			ZeroMemory(sBuffer.pBuffer, sBuffer.dwBufferSize);
			this->playCur += dwDataSize;
		}
	
		//�o�b�t�@�̃A�����b�N
		if(FAILED(this->DSBuffer->Unlock(sBuffer.pBuffer,				//�A�����b�N�Ɏ��s
							sBuffer.dwBufferSize, NULL, 0))) {
			LeaveCriticalSection(&this->cs);							//�N���e�B�J���Z�N�V�����̏I��
			return eSOUND_UNLOCK;										//�G���[����Ԃ�
		}

		this->playCnt++;												//�Đ��񐔃A�b�v
	} else {															//�ʏ�̓ǂݍ��ݏ���
		//�f�[�^�ǂݍ���
		//�o�b�t�@�̃��b�N
		if(FAILED(this->DSBuffer->Lock(this->writeCur,					//�o�b�t�@�̃��b�N�Ɏ��s
							this->blockSize, &sBuffer.pBuffer, 
							&sBuffer.dwBufferSize, &pBuffer, NULL, 0))) {
			LeaveCriticalSection(&this->cs);							//�N���e�B�J���Z�N�V�����̏I��
		   return eSOUND_LOCK;
		}

		//�f�[�^�̓ǂݍ��ݏ���
		if(this->OggVorbis.readData(&sBuffer,							//�f�[�^�̓ǂݍ���
									sBuffer.dwBufferSize) != eOGG_OK) {
			LeaveCriticalSection(&this->cs);							//�N���e�B�J���Z�N�V�����̏I��
			return eSOUND_READ;
		}
	
		//�o�b�t�@�̃A�����b�N
		if(FAILED(this->DSBuffer->Unlock(sBuffer.pBuffer,				//�A�����b�N�Ɏ��s
							sBuffer.dwBufferSize, NULL, 0))) {
			LeaveCriticalSection(&this->cs);							//�N���e�B�J���Z�N�V�����̏I��
			return eSOUND_UNLOCK;										//�G���[����Ԃ�
		}

		this->readCur = this->OggVorbis.getReadPos();					//�ǂݍ��݃J�[�\�����ړ�
		this->playCur += this->blockSize;								//�Đ��ʒu�̍X�V
	}

	this->writeCur += this->blockSize;									//�������݃J�[�\�����ړ�

	//�������݃J�[�\�����C��
	if(this->writeCur >= this->bufferSize) {							//�J�[�\���̈ʒu���o�b�t�@�T�C�Y���傫��
		this->writeCur = 0;												//�J�[�\���̈ʒu���o�b�t�@�̐擪�ɖ߂�
	}

	LeaveCriticalSection(&this->cs);									//�N���e�B�J���Z�N�V�����̏I��
	return eSOUND_OK;
}