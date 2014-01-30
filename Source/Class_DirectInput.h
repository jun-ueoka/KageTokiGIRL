#pragma warning(disable:4996)
#pragma once
#include <windows.h>
#define DIRECTINPUT_VERSION	0x0800
#include <dInput.h>
#include	"Version.h"

//������������������������������������������������������������������������������������������
//�����͋@��̎��																		�@��
//������������������������������������������������������������������������������������������
namespace DEVICETYPE{
	enum TYPENAME{
		eKEYBOARD,			//�@�L�[�{�[�h
		eGAMEPAD,			//�@�W���C�X�e�B�b�N
		eUNNON
	};
}
//������������������������������������������������������������������������������������������
//���L�[�̓��͏�ԁi���Ԃ���ђl�̕ύX�s�j											�@��
//������������������������������������������������������������������������������������������
namespace MYKEYSTAT{
	enum TYPENAME{
		eONCELOCK,			//	�������̃L�[���P�x�����܂Ŗ���������
		eFREE,				//	�����Ă����
		eOFF,				//	����������
		eON,				//	����������
		eDOWN,				//	�����Ă����
		eNON				//	checkStatus( )�@�Ή�����L�[�������ꍇ
	};
}
//������������������������������������������������������������������������������������������
//���v���O�������Ŏg�p���鉼�z�L�[���́i���Ԃ���ђl�̕ύX�s�j						�@��
//������������������������������������������������������������������������������������������
namespace MYKEY{
	enum TYPENAME{	//	�L�[�̖��O
	//	�L�[�p�b�h���L�[�{�[�h
		eUP,		//	��L�[				//!< ��
		eDOWN,		//	���L�[				//!< ��
		eLEFT,		//	���L�[				//!< ��
		eRIGHT,		//	�E�L�[				//!< ��
		eSOFT,		//�@�{�^��00			//!< �E�{�^��
		eMIDDLE,	//�@�{�^��01			//!< ���{�^��
		eHARD,		//�@�{�^��02			//!< ��{�^��
		eDUST,		//�@�{�^��03			//!< ���{�^��
		eL1,		//�@�{�^��04			//!< 
		eL2,		//�@�{�^��05			//!< 
		eR1,		//�@�{�^��10			//!< 
		eR2,		//�@�{�^��11			//!< 
		eSELECT,	//�@�{�^��12			//!< �I���{�^��
		eSTART,		//�@�{�^��13			//!< ����{�^��
		eQUIT,		//�@�{�^��14			//!< �I���{�^��

#ifdef	_DEBUG
	//	�L�[�{�[�h��p
		eF1,		//	�t�@���N1			//!< ���d�V���{����ʂ�
		eF2,		//	�t�@���N2			//!< �L�����N�^�[�̈ʒu��ύX
		eF3,		//	�t�@���N3			//!< �J�Q�I�L��
		eF4,		//	�t�@���N4			//!< �J�Q�I����
		eF5,		//	�t�@���N5			//!< �J�Q�I��������
		eF6,		//	�t�@���N6			//!< �C�x���g�X�L�b�v
		eF7,		//	�t�@���N7			//!< �Q�[���N���A
		eF8,		//	�t�@���N8			//!<
		eF9,		//	�t�@���N9			//!<
		eF10,		//	�t�@���N10			//!<
#else
#ifdef PRESENTATION
	//	�L�[�{�[�h��p
		eF1,		//	�t�@���N1			//!< ���d�V���{����ʂ�
		eF2,		//	�t�@���N2			//!< �L�����N�^�[�̈ʒu��ύX
		eF3,		//	�t�@���N3			//!< �J�Q�I�L��
		eF4,		//	�t�@���N4			//!< �J�Q�I����
		eF5,		//	�t�@���N5			//!< �J�Q�I��������
		eF6,		//	�t�@���N6			//!< �C�x���g�X�L�b�v
		eF7,		//	�t�@���N7			//!< �Q�[���N���A
		eF8,		//	�t�@���N8			//!<
		eF9,		//	�t�@���N9			//!<
		eF10,		//	�t�@���N10			//!<
		e1,			//
		e2,			//
		e3,			//
		e4,			//
		e5,			//
		e6,			//
		e7,			//
		e8,			//
		e9,			//
		e0,			//
#endif
#endif
		eMAX		//�@�{�^������			//!< 
	};
}
//������������������������������������������������������������������������������������������
//�����͋@��ƃv���O�����𒇉��L�[���\����										�@��
//������������������������������������������������������������������������������������������
struct sKeyParam{				
	DWORD vKeyCode;				//	�o�[�`�����L�[�R�[�h(�L�[�{�[�h�̏ꍇ�ݒ肷��j�i�Q�[���p�b�h�ł�MYKEY�^�̂����ꂩ�̒l������j
	MYKEY::TYPENAME myKey;				//	�Ή��L�[(MYKEY�^�̂����ꂩ�̒l������j
	MYKEYSTAT::TYPENAME keyPressStatus;	//	�L�[���͏��(�����Ă���ԁA����������A����������A�����Ă����)
};
//������������������������������������������������������������������������������������������
//�����͊��ł�����G���[���															�@��
//������������������������������������������������������������������������������������������
namespace MYERROR_DI{
	enum TYPENAME{
		eDIC,			//	DirectInput8Create�����s
		eED,			//	�f�o�C�X�񋓂Ɏ��s
		eED_UNNONTYPE,	//	���Ή��̃f�o�C�X�@
		eCD,			//	�f�o�C�X�����Ɏ��s
		eSDF,			//	�t�H�[�}�b�g�ݒ�Ɏ��s
		eSCL,			//	�������x���̐ݒ�Ɏ��s
		eNOINIT,		//	�I�u�W�F�N�g������������Ă��Ȃ�
		eCNTOVER,		//	�J�E���^�I�[�o�[
		eGDS,			//	�f�o�C�X���擾�Ɏ��s
		eUSEKEYBOARD,	//	�L�[�{�[�h���g�p���Ă���
		eNONRELEASE,	//	�J�����Ă��Ȃ���ԂōĐ������悤�Ƃ��Ă���
		eNOTHING		//	�G���[�͂Ȃ�
	};
}
//����������������������������������������������������������������������������������������������
//��DirectInput�̃f�o�C�X����N���X(�g�p����f�o�C�X�̐��ɉ����Đ錾�E�g�p����j			�@��
//����������������������������������������������������������������������������������������������
#define kinputDeviceMax 4	//���o����f�o�C�X�̍ő吔

//����������������������������������������������������������������������������������������
class CINPUT{
//����������������������������������������������������������������������������������������
private:static CINPUT			*instance[kinputDeviceMax];		//�@�S�̃f�o�C�X�̂ݎg�p�\			
private:IDirectInputDevice8*	device;							//	DI�f�o�C�X
private:DWORD					type;							//	�L�[�{�[�h��Q�[���p�b�h�Ȃǂ����ʂ���
private:int						id;								//�@�f�o�C�X�Ɋ��蓖�Ă�ꂽID�ԍ�
private:int						numberOfKey;					//�@���̓`�F�b�N����L�[�̍ő吔
private:sKeyParam				*informationOfKeys;				//�@�`�F�b�N�Ώۂ̃L�[���
private:BOOL					valid;							//�@�����������S�ɍs���Ă���ꍇTRUE
//���͋@��̃{�^���𒲂ׂ�A���͋@�했�ɐݒ肷��֐��|�C���^
private:void (*checkKeyFunc)(	IDirectInputDevice8* inDevice,	//DInput�f�o�C�X
								int inNumberOfKey,				//�`�F�b�N����L�[�̐�
								sKeyParam inKeys[ ]);			//�L�[���
//����������������������������������������������������������������������������������������
private:void release(void);
private:MYKEYSTAT::TYPENAME checkStatus(MYKEY::TYPENAME inKeyID);
//����������������������������������������������������������������������������������������
public:CINPUT( );
//�C���X�^���X
//�擾
public:static CINPUT* getInstance(int inDeviceNumber);	//�f�o�C�X�ԍ�
//�J��
public:static void releaseInstance(int inDeviceNumber);			//�f�o�C�X�ԍ�
//�S�J��
public:static void releaseInstanceAll( );
//����������������������������������������������������������������������������������������
//���̓f�o�C�X
//����
public:MYERROR_DI::TYPENAME init(	HWND inWindowHandle,			//�E�B���h�E�n���h��
						DEVICETYPE::TYPENAME inDeviceType,		//�f�o�C�X���
						int inNumberOfKey,				//�L�[�i�{�^���j��
						sKeyParam ioKeyInformation[ ]);	//�e�L�[�����{�^���ɑΉ����邩
														//�ݒ肵�Ă��邱��
//����������������������������������������������������������������������������������������
//���͊m�F
//�n�[�h�E�F�A�����Ԏ擾
public:void		checkKey(void);
//�����m�F
public:virtual	bool checkPress(MYKEY::TYPENAME inKeyID);
//��������m�F
public:virtual	bool checkPressOnce(MYKEY::TYPENAME inKeyID);
//����������������������������������������������������������������������������������������
//�X�e�[�^�X�ύX
//�L�[���ꎞ�I�ɉ����Ă��Ȃ���Ԃɕς���i�������̃L�[�𖳌��ɂ���j
public:void checkKeyOnceLock(MYKEY::TYPENAME inKeyID);
//����������������������������������������������������������������������������������������
//�f�o�C�X����
//�ʂ̃f�o�C�X�̓��͏�����荞��
public:void composeKeyStatus(CINPUT* inKeyDevice);
};

//�_�~�[����
//����������������������������������������������������������������������������������������
class CDUMMY_INPUT : public CINPUT{
//����������������������������������������������������������������������������������������
protected:	bool	dummyKey[MYKEY::eMAX];			//!< ����
//����������������������������������������������������������������������������������������
public:		CDUMMY_INPUT( )	: CINPUT( ) { resetKey();	}
//���̓L�[���Z�b�g
public:void		resetKey(void)
	   { for(int i=0; i<MYKEY::eMAX; i++){ dummyKey[i]	= false;}	}
//��������
public:void		setPress(MYKEY::TYPENAME inKeyID, bool inBool)
	   { dummyKey[inKeyID] = inBool;	}
//���͊m�F
//�����m�F
public:bool		checkPress(MYKEY::TYPENAME inKeyID)
	   { return	dummyKey[inKeyID];		}
//��������m�F
public:bool		checkPressOnce(MYKEY::TYPENAME inKeyID)
	   { return	dummyKey[inKeyID];		}
};

//�^�C�}�[�t���_�~�[����
//����������������������������������������������������������������������������������������
class CDUMMY_INPUT_TIMER : public CDUMMY_INPUT{
//����������������������������������������������������������������������������������������
protected:	int		iKeyTimer[MYKEY::eMAX];			//!< �^�C�}�[
//����������������������������������������������������������������������������������������
public:		CDUMMY_INPUT_TIMER( )	: CDUMMY_INPUT( )
		{ for(int i=0; i<MYKEY::eMAX; i++){ iKeyTimer[i]	= 0;}}
//�^�C�}�[�t����������
public:void		setPressWithTimer(MYKEY::TYPENAME inKeyID, bool inBool, int inTime)
	   { dummyKey[inKeyID] = inBool;	iKeyTimer[inKeyID] = inTime;	}
//�^�C�}�[�m�F
public:void		checkTimer(void)
	   { for(int i=0; i<MYKEY::eMAX; i++)
	     { if(dummyKey[i])
	       { if(iKeyTimer[i] > 0)	iKeyTimer[i]--;
		     else					dummyKey[i]	= false;
		   }
	     }
	   }
};