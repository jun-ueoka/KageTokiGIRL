#pragma warning(disable:4996)
#pragma once
#include <windows.h>
#define DIRECT3D_VERSION	0x0900
#include <d3d9.h>
#include <d3dx9.h>
//=======================================================================
//�J���n�}�N��
#define MAC_Release_One(o)		if(o != NULL){o->Release( );o = NULL;}
#define MAC_release_One(o)		if(o != NULL){o->release( );o = NULL;}
#define MAC_delete_One(o)		if(o != NULL){delete o;o = NULL;}
#define MAC_delete_Table(o)		if(o != NULL){delete[] o;o = NULL;}

//=======================================================================
//�V�X�e���ŋ��L����ėp�@�\�Q
namespace GCom{
	//������������������������������������������������������������������������������������������
	//���J�E���^�[�N���X																	�@��
	//������������������������������������������������������������������������������������������
	namespace Cnt
	{
		//����������������������������������������������������������������������������������������
		namespace Mode{
			enum TYPENAME{
				eNon,			//���g�p
				eRepeatsA,		//�J��Ԃ�A(end��start�̐؂�ւ��̍ہA�덷�����Łj
											//���j���[�J�[�\����
				eRepeatsB,		//�J��Ԃ�B(end��start�̐؂�ւ��̍ہA�덷���c���j
											//��]�╨�̈ړ���
				eTourn,			//���A����
				eStop,			//�P��ڂ̂ݗL��
				eZero			//�����l�ƏI���������ꍇ�ɓK�p�A��ɏ����l��Ԃ�
			};
		}
		//����������������������������������������������������������������������������������������
		namespace Status{
			enum TYPENAME{
				eErr,			//����
				eEnd,			//�J�E���g�I���ieStop���[�h�̂݁j
				eNormal			//�J�E���^�[�L��
			};
		}
		//����������������������������������������������������������������������������������������
		//cCounter�̋@�\���g�������N���X
		//����������������������������������������������������������������������������������������
		class cNormal{
			float counter;			//�J�E���^�[
			float startValue;		//�����l
			float endValue;			//�I���l
			float incValue;			//�����i�����j�l

			Mode::TYPENAME mode;		//���[�h

			void increasedCounter_Mode_RepeatsA(float inAddValue);
			void increasedCounter_Mode_RepeatsB(float inAddValue);
			void increasedCounter_Mode_Tourn(float inAddValue);
			void increasedCounter_Mode_Stop(float inAddValue);
		public:
			cNormal( );
			//�J�E���^�[��ݒ肷��
			Status::TYPENAME init(
				float inStartValue,					//�����l
				float inEndValue,					//�I���l
				float inIncValue,					//�����l
				Mode::TYPENAME inMode);				//���[�h
			//�J�E���^�[��i�߂�
			void increasedCounter(float inAddValue = 1.0f);
			//���ݒl�𓾂�
			float getCounter( );
			//���ݒl��ύX����i�J�E���^�[�͈̔͊O�̒l���w�肵���ꍇ�����j
			void setCounter(float inCounter);
			//���ݒl���C���g�^�ɕϊ����Ď擾����
			int getCounterI( );
			//���ݒl���_�u���^�ɕϊ����Ď擾����
			double getCounterD( );
			//�J�E���^�[�̌��݂̏�Ԃ𓾂�
			Status::TYPENAME getStatus( );
			//�J�E���^�[��������Ԃɖ߂�
			void resetCounter( );
		};
	}
	namespace _3D
	{
		//=======================================================================
		//�R�c�⏕�n
		//=======================================================================
		//������
		class cBox{
		public: D3DXVECTOR3 s;
		public: D3DXVECTOR3 e;
		public: cBox();
		public: cBox(const D3DXVECTOR3* s, const D3DXVECTOR3* e);
		public: void set(const D3DXVECTOR3* s, const D3DXVECTOR3* e);
		public: void transform(const D3DXVECTOR3* offset);
		public: BOOL intersect(const cBox* box);
		//�Q�̃{�b�N�X�̒��s����ʂ̐ڐG�𔻒肷��i�ڐG���s�q�t�d�j
		public: BOOL hitLine(const cBox* box);
		};
		//=======================================================================
		//�R�c�⏕�n
		//=======================================================================
		//�e�����o���Afloat�^��RECT�N���X
		class cFloatRect{
		public:
			float left;				//�����W�i�w�j
			float top;				//����W�i�x�j
			float right;			//�E���W�i�w�j
			float bottom;			//�����W�i�x�j
			//=======================================================================
			//��`���̐ݒ�
			void set(	float inLeft,		//�����W�i�w�j
						float inTop,		//����W�i�x�j
						float inRight,		//�E���W�i�w�j
						float inBottom);	//�����W�i�x�j
			//=======================================================================
			//��`�̕��s�ړ�
			void transform(	float inX,		//�w�������ړ��l
							float inY);		//�x�������ړ��l
			//=======================================================================
			//�Q�̋�`�̐ڐG�𔻒肷��i�ڐG���s�q�t�d�j
			BOOL hit(cFloatRect* inRect);
			//�Q�̋�`�̐���̐ڐG�𔻒肷��i�ڐG���s�q�t�d�j
			BOOL hitLine(cFloatRect* inRect);
		};
	}
	namespace Color
	{
		//=======================================================================
		//�F�ݒ�⏕�n
		//=======================================================================
		//�R�Q�a�����J���[���ȈՓI�Ɉ������߂̍\����
		struct sRGBA{
			BYTE b;		//��
			BYTE g;		//��
			BYTE r;		//��
			BYTE a;		//����
		};
		//=======================================================================
		//�R�Q�a�����J���[���ȈՓI�Ɉ������߂̃N���X
		class cARGB{
		public:
			//=======================================================================
			//�R�Q�a�����J���[���ȈՓI�Ɉ������߂̍\����
			sRGBA color;
			//=======================================================================
			//�R���X�g���N�^�ADWORD�^�ŐF��ݒ�ł���A�f�t�H���g�͍�
			cARGB(DWORD inColor = 0x00000000);	
			//=======================================================================
			//�R���X�g���N�^�A�S�F���ꂼ����ʂɐF��ݒ�ł���A�f�t�H���g�͖���
			cARGB(BYTE a, BYTE r, BYTE g, BYTE b);	
			//=======================================================================
			//DWORD�^�ŐF��ݒ�ł���
			void set(DWORD inColor);		
			//=======================================================================
			//DWORD�^�ŐF��ݒ�ł���
			void set(BYTE a, BYTE r, BYTE g, BYTE b);		
			//=======================================================================
			//�F��DWORD�^�ŕԋp����
			DWORD get( );
		};
	}
	namespace Str
	{
		//��������m�d�v���ăR�s�[���쐬
		char* copyNew(char* inString);
		LPSTR copyNew(LPCSTR inString);
		//�]����Ƀ����������蓖�Ă��Ă���ꍇ�A�����strCopyNew���s��
		char*copyReleaseAndNew(char* *inSet, char* inString);
		LPSTR copyReleaseAndNew(LPSTR* inSet, LPCSTR inString);
	}

	namespace Memory
	{
	}
}	//namespace GCom
