//-------------------------------------------------------------------------------------------------------------------------------
//�t�@�C����	:	COGG_VORBIS.h
//�쐬��		:	�u���L
//�쐬��		:	08/05/12
//�@�\			:	COGG_VORBIS.cpp�ŕK�v�Ȑ錾�A��`���s���Ă��܂��B
//���l			:	OggVorbis���������߂̑���⏈���̒񋟁B
//					CSound�֘A�N���X�ň������Ƃ�O���ɂ����č쐬���Ă��܂��B
//�X�V����		:	08/05/12	Ver1.01.00	��{�@�\�������B
//					08/05/26	Ver1.11.00	oggvorbis-win32sdk���ŐV�o�[�W�����ɕύX�B
//											oggvorbis-win32sdk-1.0.1����libogg-1.1.3
//																		libvorbis-1.2.0�֕ύX�B
//-------------------------------------------------------------------------------------------------------------------------------
//�v���O�}�w��
#pragma once
#pragma warning(disable:4996)

//���C�u�����̃����N

#pragma comment(lib,"../source/oggvorbis-win32sdk/lib/ogg_static.lib")
#pragma comment(lib,"../source/oggvorbis-win32sdk/lib/vorbis_static.lib")
#pragma comment(lib,"../source/oggvorbis-win32sdk/lib/vorbisfile_static.lib")

#pragma comment(linker, "/nodefaultlib:LIBCMT")

//�w�b�_�[�t�@�C���̃C���N���[�h
#include "Sound.h"

#include "oggvorbis-win32sdk/include/vorbis/codec.h"
#include "oggvorbis-win32sdk/include/vorbis/vorbisfile.h"

//�񋓌^��`
//�T�E���h�֘A�G���[
typedef enum eOGGVORBIS_ERROR {
	eOGG_ERROR,																		//�G���[
	eOGG_OK,																		//����
	eOGG_FILENAME,																	//�t�@�C�������w�肳��Ă��Ȃ�
	eOGG_OPENFILE,																	//�t�@�C�����J���Ȃ�
	eOGG_OGGFILE,																	//OggVorbis�̏������Ɏ��s
	eOGG_INFO,																		//OggVorbis�̏��擾�Ɏ��s
	eOGG_READ,																		//OggVorbis�̓ǂݍ��݂Ɏ��s
} OGGVORBIS_ERROR;

//-------------------------------------------------------------------------------------------------------------------------------
//�N���X��		:	COGG_VORBIS
//�쐬��		:	�u���L
//�쐬��		:	08/05/12
//�@�\			:	OggVorbis�֘A�����B
//���l			:	OggVorbis��CSound�֘A�N���X�ň������߂̋@�\��񋟂��Ă��܂��B
//�X�V����		:	08/05/12	Ver1.01.00	�@�\������
//-------------------------------------------------------------------------------------------------------------------------------
class COGG_VORBIS {
private:
	/***OggVorbis���g�p����̂ɕK�v�ȃf�[�^***/
	FILE *fp;																		//OggVorbis�̃t�@�C���|�C���^
	LPSTR szFileName;																//�ǂݍ���OggVorbis�t�@�C����
	OggVorbis_File ovf;																//OggVorbis�̃t�@�C�����
	vorbis_info *vi;																//OggVorbis�̏��
	WAVEFORMATEX wfex;																//�T�E���h�f�[�^�̃t�H�[�}�b�g
	DWORD DataSize;																	//�����f�[�^�̃T�C�Y
public:
	/***�R���X�g���N�^�ƃf�X�g���N�^***/
	COGG_VORBIS(void) { this->initialize(); }										//�f�t�H���g�R���X�g���N�^
	COGG_VORBIS(LPSTR szFileName);													//�R���X�g���N�^
	~COGG_VORBIS(void);																//�f�X�g���N�^

	OGGVORBIS_ERROR initialize(void);												//������

	/***OggVorbis�֘A����***/
	OGGVORBIS_ERROR openOggVorbis(LPSTR szFileName);								//OggVorbis�t�@�C�����J���Ďg�p�\�ɂ���
	OGGVORBIS_ERROR closeOggVorbis(void);											//OggVorbis�t�@�C�������

	/***�f�[�^����***/
	WAVEFORMATEX getWaveFormat(void) { return this->wfex; }							//WAVE�t�H�[�}�b�g�����擾
	DWORD getDataSize(void) { return this->DataSize; }								//�����f�[�^�̃T�C�Y�̎擾

	/***�T�E���h�f�[�^�ǂݍ��݊֘A����***/
	//�@�\:���݂̓ǂݍ��݈ʒu�̎擾		����:�Ȃ�	�߂�l:�ǂݍ��݈ʒu
	DWORD getReadPos(void) { return (DWORD)ov_pcm_tell(&this->ovf); } //�ǂݍ��݈ʒu�̎擾
	//�@�\:�ǂݍ��݈ʒu�̃V�[�N		����:�V�[�N��̈ʒu(�ʒu / (�`�����l�� * �ʎq���o�C�g��)		�߂�l:�G���[���
	//<�g�p��>this->seekData(readCursor / (this->wfex.nChannels * kBITSPERSAMPLE / 8));
	OGGVORBIS_ERROR seekData(DWORD pos) {											//PCM�Ƀf�R�[�h��̒l�ŃV�[�N����
		ov_pcm_seek(&this->ovf, pos); return eOGG_OK;
	}

	/***�T�E���h�f�[�^�ǂݍ���***/
	OGGVORBIS_ERROR readData(stSoundBuffer *pstSoundBuffer, DWORD dwSize);			//�����f�[�^��PCM�Ƀf�R�[�h���ēǂݍ���
};