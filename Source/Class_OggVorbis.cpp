//-------------------------------------------------------------------------------------------------------------------------------
//�t�@�C����	:	Class_OggVorbis.cpp
//�쐬��		:	�u���L
//�쐬��		:	08/05/12
//�@�\			:	OggVorbis�ɑ΂��鑀��⏈���B
//���l			:	OggVorbis���������߂̑���⏈���̒񋟁B
//					CSound�֘A�N���X�ň������Ƃ�z�肵�Ă��܂��B
//�X�V����		:	08/05/12	Ver0.01.00	��{�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
//�w�b�_�[�t�@�C���̃C���N���[�h

#include	"Precompiler.h"

//#include	"Class_OggVorbis.h"

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	COGG_VORBIS
//�쐬��		:	�u���L
//�쐬��		:	08/05/12
//�@�\			:	COGG_VORBIS�N���X�̃R���X�g���N�^�ł��B
//���l			:	�f�[�^�̏������Ɠǂݍ���OggVorbis�t�@�C���̐ݒ���s���܂��B
//�N���X		:	COGG_VORBIS
//����			:	LPSTR szFileName	�ǂݍ��ރt�@�C����
//�߂�l		:	�Ȃ�
//�X�V����		:	08/05/12	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
COGG_VORBIS::COGG_VORBIS(LPSTR szFileName)
{
	this->initialize();													//�f�[�^�̏�����
	this->szFileName = szFileName;										//�ǂݍ��ރt�@�C������ݒ�
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	~COGG_VORBIS
//�쐬��		:	�u���L
//�쐬��		:	08/05/12
//�@�\			:	COGG_VORBIS�N���X�̃f�X�g���N�^�ł��B
//���l			:	
//�N���X		:	COGG_VORBIS
//����			:	�Ȃ�
//�߂�l		:	�Ȃ�
//�X�V����		:	08/05/12	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
COGG_VORBIS::~COGG_VORBIS()
{
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	initialize
//�쐬��		:	�u���L
//�쐬��		:	08/05/12
//�@�\			:	�K�v�ȃf�[�^�̐ݒ�Ə��������s���Ă��܂��B
//���l			:	�Ȃ��B
//�N���X		:	COGG_VORBIS
//����			:	�Ȃ�
//�߂�l		:	OGGVORBIS_ERROR	�G���[���
//�X�V����		:	08/05/12	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
OGGVORBIS_ERROR COGG_VORBIS::initialize(void)
{
	//�f�[�^�̏�����
	ZeroMemory(this, sizeof(COGG_VORBIS));								//�f�[�^�̏�����

	return eOGG_OK;														//����I��
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	openOggVorbis
//�쐬��		:	�u���L
//�쐬��		:	08/05/12
//�@�\			:	OggVorbis�֘A�����B
//���l			:	OggVorbis�t�@�C�����J���ăA�N�Z�X�\�ȏ�Ԃɂ��܂��BcloseOggVorbis�Ƒ΂ɂȂ��Ă��܂��B
//					OggVorbis�ւ̃A�N�Z�X���K�v�Ȃ��Ȃ�����AcloseOggVorbis���Ăяo���ďI���������s���Ă��������B
//�N���X		:	COGG_VORBIS
//����			:	�Ȃ�
//�߂�l		:	OGGVORBIS_ERROR	�G���[���
//�X�V����		:	08/05/12	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
OGGVORBIS_ERROR COGG_VORBIS::openOggVorbis(LPSTR szFileName)
{
	//�ǂݍ��ރt�@�C������ݒ�
	this->szFileName = szFileName;

	//OggVorbis�t�@�C�����J��
	if(!this->szFileName) {												//�ǂݍ��ރt�@�C�������ݒ肳��Ă��Ȃ�
		return eOGG_FILENAME;											//�G���[����Ԃ�
	}

	this->fp = fopen(this->szFileName, "rb");							//�Ώۂ�OggVorbis�t�@�C�����I�[�v��
	//�t�@�C���I�[�v���G���[�`�F�b�N
	if(!this->fp) {														//�t�@�C���I�[�v���Ɏ��s
		return eOGG_OPENFILE;											//�G���[����Ԃ�
	}

	//OggVorbis�̏�����
	if(ov_open(this->fp, &this->ovf, NULL, 0) < 0) {					//�������Ɏ��s
		fclose(this->fp);												//�t�@�C���|�C���^�����
		return eOGG_OGGFILE;											//�G���[����Ԃ�
	}

	//OggVorbis�̏����擾
	this->vi = ov_info(&this->ovf, -1);									//OggVorbis�̏����擾
	//�擾�ɐ�������������
	if(this->vi == NULL) {												//�擾�Ɏ��s
		this->closeOggVorbis();											//OggVorbis�Ɋւ��鏈�����I��
		return eOGG_INFO;
	}

	//WAVE�t�@�C���t�H�[�}�b�g�̐ݒ�(�ʎq���r�b�g����16bit�Ƃ���)
	//���̃f�[�^�����Ƃ�WAVE�Ƀf�R�[�h����B
	this->wfex.cbSize = sizeof(WAVEFORMATEX);							//�f�[�^�T�C�Y
	this->wfex.wFormatTag = WAVE_FORMAT_PCM;							//WAVE�t�H�[�}�b�g�̎��
	this->wfex.nChannels = this->vi->channels;							//�`�����l����
	this->wfex.nSamplesPerSec = this->vi->rate;							//�T���v�����O���g��
	this->wfex.wBitsPerSample = kBITSPERSAMPLE;							//�ʎq���r�b�g��(16bit�ɐݒ�)
	this->wfex.nBlockAlign = this->wfex.nChannels						//�u���b�N�A���C�����g
								* this->wfex.wBitsPerSample / 8;
	this->wfex.nAvgBytesPerSec = this->wfex.nSamplesPerSec				//1�b�Ԃɓ]������o�C�g��
									* this->wfex.nBlockAlign;

	//WAVE�f�[�^�T�C�Y�̐ݒ�
	this->DataSize = (DWORD)(this->wfex.nChannels						//WAVE�f�[�^�T�C�Y�̌v�Z
								* this->wfex.nSamplesPerSec
								* ov_time_total(&this->ovf, -1)
								* this->wfex.wBitsPerSample / 8);

	return eOGG_OK;														//����I��
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	closeOggVorbis
//�쐬��		:	�u���L
//�쐬��		:	08/05/12
//�@�\			:	OggVorbis�֘A�����B
//���l			:	OggVorbis�t�@�C������ăA�N�Z�X���I�����܂��BopenOggVorbis�Ƒ΂ɂȂ��Ă��܂��B
//�N���X		:	COGG_VORBIS
//����			:	�Ȃ�
//�߂�l		:	OGGVORBIS_ERROR	�G���[���
//�X�V����		:	08/05/12	Ver1.00.00	�@�\�������B
//-------------------------------------------------------------------------------------------------------------------------------
OGGVORBIS_ERROR COGG_VORBIS::closeOggVorbis()
{
	//�I������
	ov_clear(&this->ovf);												//OggVorbis�Ɋւ��鏈�����I��

	//OggVorbis�t�@�C������������
	//�����I�ɉ�����Ȃ��Ă�ov_clear�ɂ���ĉ������邪�A�|�C���^�̏�񂪎c�邽�ߏ���������
	this->vi = NULL;													//OggVorbis�t�@�C������������

	return eOGG_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------
//���\�b�h��	:	readData
//�쐬��		:	�u���L
//�쐬��		:	08/05/12
//�@�\			:	OggVorbis�̉����f�[�^��PCM�f�[�^�Ƀf�R�[�h���ēǂݍ��݂܂��B
//���l			:	�w�肳�ꂽ�o�b�t�@�Ƀo�b�t�@�T�C�Y�����������f�[�^��ǂݍ��݂܂��B
//�N���X		:	COGG_VORBIS
//����			:	stSoundBuffer	*pstSoundBuffer		�Ǎ���̃o�b�t�@���
//					DWORD			dwSize				�ǂݍ��ރf�[�^��
//�߂�l		:	OGGVORBIS_ERROR	�G���[���
//�X�V����		:	08/05/12	Ver1.00.00	�@�\�������B
//					08/06/05	Ver1.01.00	�f�[�^�ǂݍ��ݎ��̈��S���������B
//-------------------------------------------------------------------------------------------------------------------------------
OGGVORBIS_ERROR COGG_VORBIS::readData(stSoundBuffer *pstSoundBuffer, DWORD dwSize)
{
	//�o�b�t�@�[�ɉ��f�[�^����������
	BOOL bEndFlag = TRUE;												//�ǂݍ��݂̃t���O(TRUE:�p��,FALSE:�I��)
	DWORD dwReadCursor = 0;												//�ǂݍ��݃J�[�\��
	DWORD ret;															//�ǂݍ��݂̖߂�l

	//�o�b�t�@�ɓǂݍ��߂�f�[�^�ʂ����ׂ�
	if(pstSoundBuffer->dwBufferSize < dwSize) {							//�o�b�t�@���ǂݍ��ރf�[�^�ʂ�����
		return eOGG_READ;												//�G���[����Ԃ�
	}

	//�Z�J���_���o�b�t�@��OggVorbis�̃f�[�^��ǂݍ���
	//��x�ɂ��ׂẴf�[�^�͓ǂݍ��߂Ȃ��̂ŏ������f�[�^��ǂݍ���
	//�G���[����������
	do {
		//OggVorbis�̉����f�[�^���f�R�[�h���Ȃ���o�b�t�@�ɓǂݍ���
		ret = ov_read(&this->ovf,										//OggVorbis_File�\����
			(char *)pstSoundBuffer->pBuffer + dwReadCursor,				//�Ǎ���̃o�b�t�@
			pstSoundBuffer->dwBufferSize - dwReadCursor,				//�ǂݍ��ރf�[�^�T�C�Y
			0,															//�G���f�B�A���l�X�̎w��
			this->wfex.wBitsPerSample / 8,								//�ʎq���o�C�g��
			1,															//�������f�[�^��?(���������1)
			NULL);														//?(NULL�ő��v�݂����E�E�E)

		//���[�v���p�����邩�̔���
		if(ret == 0) {													//�f�[�^�̍Ō�ɒB����
			bEndFlag = FALSE;											//�ǂݍ��݂��I������
		} else if(ret < 0) {											//���炩�̃G���[����������
			this->closeOggVorbis();										//OggVorbis�Ɋւ��鏈�����I��
			return eOGG_READ;											//�G���[����Ԃ�
		} else {														//�ʏ�̓ǂݍ��ݏ���(�܂��A�f�[�^���ǂݏI����Ă��Ȃ�)
			//�f�[�^�̓ǂݍ��݂��I�����������
			//���ꂪ�����Ɠǂݍ��ݏI������͂��̃f�[�^��ǂݍ��ރo�O���������邱�Ƃ�����
			if(dwReadCursor < pstSoundBuffer->dwBufferSize) {			//�ǂݍ��݂��I�����Ă��Ȃ�
				dwReadCursor += ret;									//�o�b�t�@�̏������݈ʒu���X�V
			} else {													//�ǂݍ��݂��I������
				bEndFlag = FALSE;										//�ǂݍ��ݏ������I��
			}
		}
	}while(bEndFlag);													//���ׂẴf�[�^���ǂݏI���܂ŌJ��Ԃ�

	return eOGG_OK;														//����
}