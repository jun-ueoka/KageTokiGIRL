#pragma warning(disable:4996)

#include	"Precompiler.h"
//#include "Class_DirectInput.h"
//#include "MySystem.h"

#pragma comment(lib,"dxguid")
#pragma comment(lib,"dInput8")



//������������������������������������������������������������������������������������������
//�����o�����@���GUID�Ǝg�p��Ԃ��󂯎��\����										�@��
//������������������������������������������������������������������������������������������
struct INPUTDEVICEFLAG{
	GUID rguid;		//GUID
	int active;		//-1NODEVICE	0NOSELECTED	1SERECTED
};
//����������������������������������������������������������������������������������������������
//�����͋@����Ǘ�����N���X																�@��
//����������������������������������������������������������������������������������������������
class CINPUT_OBJECT{
//����������������������������������������������������������������������������������������
private:static CINPUT_OBJECT		*instance;				//�P��I�u�W�F�N�g�̃C���X�^���X
private:LPDIRECTINPUT8			directInputObject;		//DirectInput�I�u�W�F�N�g
//�񋓂����f�o�C�X�̏�Ԃ�ێ�����
private:INPUTDEVICEFLAG deviceFlags_Pad[4];			//�Q�[���p�b�h�f�o�C�X
private:INPUTDEVICEFLAG deviceFlags_Keybd[4];		//�L�[�{�[�h�f�o�C�X
private:int numberOfActiveDevice;					//�g�p���Ă���f�o�C�X�̐�
//����������������������������������������������������������������������������������������
//�C���X�^���X�̎擾
public:static CINPUT_OBJECT* getInstance( );
	//�R���X�g���N�^
	private:CINPUT_OBJECT( );
	//������
	private:MYERROR_DI::TYPENAME init( );
//�C���X�^���X�̉��
public:static void releaseInstance( );
	//���
	private:void release(void);
//����������������������������������������������������������������������������������������
public:INPUTDEVICEFLAG* getDevice_Pad(int inNumber){	return &deviceFlags_Pad[inNumber];}
public:INPUTDEVICEFLAG* getDevice_KeyBd(int inNumber){	return &deviceFlags_Keybd[inNumber];}
//����������������������������������������������������������������������������������������
//�f�o�C�X�̐���
public:MYERROR_DI::TYPENAME createDevice(	HWND inWindowHandle, 
								IDirectInputDevice8** outDirectInputDevice,
								DEVICETYPE::TYPENAME inDeviceType,
								int inID);
public:BOOL addActivetyDeviceCounter(int inAdd);
};
//������������������������������������������������������������������������������������������
//���C���X�^���X�擾																	�@��
//���@2005/11/28	�쐬																�@��
//������������������������������������������������������������������������������������������
CINPUT_OBJECT* CINPUT_OBJECT::instance = NULL;
CINPUT_OBJECT* CINPUT_OBJECT::getInstance( )
{
	if(instance == NULL){
		instance = new CINPUT_OBJECT( );
		if(instance != NULL){
			if(instance->init( ) != MYERROR_DI::eNOTHING){	//DirectInputObject�̐������������Ȃ�����
				instance->releaseInstance( );
			}
		}
	}
	return instance;
}
//������������������������������������������������������������������������������������������
//���C���X�^���X�̊J��																	�@��
//���@2005/11/28	�쐬																�@��
//������������������������������������������������������������������������������������������
void CINPUT_OBJECT::releaseInstance( )
{
	if(instance != NULL){
		delete instance;
		instance = NULL;
	}
}
BOOL CINPUT_OBJECT::addActivetyDeviceCounter(int inAdd)
{
	this->numberOfActiveDevice += inAdd;
	if(this->numberOfActiveDevice <= 0){
		return TRUE;
	}
	return FALSE;
}




//=======================================================================
//�R���X�g���N�^
//�߂�l
//		�F����
//����
//		�F����
//�@�\�T�v
//		�F�ϐ��̏�����
//����
//		�F2005/08/31	�쐬
//=======================================================================
CINPUT_OBJECT::CINPUT_OBJECT( )
{
	this->directInputObject = NULL;
	ZeroMemory(deviceFlags_Pad, sizeof(deviceFlags_Pad));
	ZeroMemory(deviceFlags_Keybd, sizeof(deviceFlags_Keybd));
	this->numberOfActiveDevice = 0;
}

//=======================================================================
//release
//�߂�l
//		�F����
//����
//		�F����
//�@�\�T�v
//		�F�Ăяo���K�{�I
//		�F�J������
//����
//		�F2005/08/31	�쐬
//=======================================================================
void CINPUT_OBJECT::release(void)
{
	ZeroMemory(deviceFlags_Pad, sizeof(deviceFlags_Pad));
	ZeroMemory(deviceFlags_Keybd, sizeof(deviceFlags_Keybd));
	if(this->directInputObject != NULL){
		this->directInputObject->Release( );
		this->directInputObject = NULL;
	}
}

//=======================================================================
//getStage
//�߂�l
//		�FDirectInput�I�u�W�F�N�g�̐�������Ă����TRUE
//����
//		�F����
//�@�\�T�v
//		�FDirectInput�I�u�W�F�N�g�̐������s���Ă��邩�ۂ���߂�
//����
//		�F2005/08/31	�쐬
//=======================================================================
//	BOOL CINPUT_OBJECT::getStage( )
//	{
//		return (this->directInputObject != NULL);
//	}

//=======================================================================
//init
//�߂�l
//		�FDirectInput�I�u�W�F�N�g�̐����̐���
//����
//		�F����
//�@�\�T�v
//		�FDirectInput�I�u�W�F�N�g�̐�������
//����
//		�F2005/08/31	�쐬
//=======================================================================
MYERROR_DI::TYPENAME CINPUT_OBJECT::init( )
{
	//	DirectInput�̏�����
	if(FAILED(DirectInput8Create(	GetModuleHandle(NULL),
									DIRECTINPUT_VERSION,
									IID_IDirectInput8,
									(VOID**)&this->directInputObject,
									NULL))){
		return MYERROR_DI::eDIC;
	}
	return MYERROR_DI::eNOTHING;
}

//=======================================================================
//�Q�[���p�b�h�̕����L�[�ɐ�Ύ��Ȃ������Ύ��p�̐ݒ���s���܂�
BOOL CALLBACK enumObjectsGamePad(	const DIDEVICEOBJECTINSTANCE* inDevice,
									void* inContext);
//�Q�[���p�b�h��񋓂���ꍇ�Ɏg�p����
BOOL CALLBACK diEnumJoysticks(	const DIDEVICEINSTANCE* inDevice,
								void* inContext);
//�L�[�{�[�h��񋓂���ꍇ�Ɏg�p����
BOOL CALLBACK diEnumKeyBord(	const DIDEVICEINSTANCE* inDevice,
								void* inContext);
//=======================================================================
//createDevice
//�߂�l
//		�FDirectInput�I�u�W�F�N�g�̐����̐���
//����
//		�F�E�B���h�E�n���h��
//		�F�����������ʂ��󂯂�DirectInput�f�o�C�X
//		�F�f�o�C�X�̎�ށi�L�[�{�[�h�A�p�b�h�j
//		�F�f�o�C�X�ԍ��i0�`3�������̓}�C�i�X�j
//�@�\�T�v
//		�F���̓f�o�C�X�𐶐�����
//		�F�f�o�C�X�̗񋓂��s��
//����
//		�F2005/08/31	�쐬
//=======================================================================
MYERROR_DI::TYPENAME CINPUT_OBJECT::createDevice(	HWND inWindowHandle, 
										IDirectInputDevice8** outDirectInputDevice,
										DEVICETYPE::TYPENAME inDeviceType,
										int inID)
{
//�f�o�C�X�̗񋓁iWindows�ɃA�^�b�`�i�o�^�j����Ă���R���g���[������T���Ă���j
	int deviceCount = 0;//�񋓂��ꂽ�f�o�C�X�̑������擾����
	//�f�o�C�X�^�C�v�ɉ������t���O�̌���
	DWORD devType;
	switch(inDeviceType){
		case DEVICETYPE::eKEYBOARD:	devType = DI8DEVCLASS_KEYBOARD;		break;		//�@�L�[�{�[�h
		case DEVICETYPE::eGAMEPAD:	devType = DI8DEVCLASS_GAMECTRL;		break;		//�@�Q�[���R���g���[���[
		default:						return MYERROR_DI::eED_UNNONTYPE;	//�@���Ή��̃f�o�C�X
	}
	//�f�o�C�X�^�C�v�ɉ������R�[���o�b�N�֐��̌���
	LPDIENUMDEVICESCALLBACK devCallBack;
	switch(inDeviceType){
		case DEVICETYPE::eKEYBOARD:	devCallBack = diEnumKeyBord;		break;		//�@�L�[�{�[�h
		case DEVICETYPE::eGAMEPAD:	devCallBack = diEnumJoysticks;		break;		//�@�Q�[���R���g���[���[
	}
	//�w�肳�ꂽ�f�o�C�X�̗�
	if(FAILED(this->directInputObject->EnumDevices(
		devType, devCallBack, &deviceCount, DIEDFL_ATTACHEDONLY))){
		return MYERROR_DI::eED;												//�@�f�o�C�X�̗񋓂Ɏ��s
	}
	if(deviceCount < 1){				return MYERROR_DI::eNOTHING;}		//�Ή�����f�o�C�X������

	//DirectInput�f�o�C�X�̐���
	INPUTDEVICEFLAG* inputDeviceWork;
	switch(inDeviceType){
		case DEVICETYPE::eKEYBOARD:	inputDeviceWork = deviceFlags_Keybd;	break;		//�@�L�[�{�[�h
		case DEVICETYPE::eGAMEPAD:	inputDeviceWork = deviceFlags_Pad;		break;		//�@�Q�[���R���g���[���[
	}
	if(inID >= 4 ||			//������f�o�C�X�̓Q�[���p�b�h�A�L�[�{�[�h�Ƃ��ɂS��܂� 
		inID >= deviceCount){	//�񋓂ł����f�o�C�X���𒴂����ԍ����w�肳�ꂽ
		return MYERROR_DI::eCNTOVER;											//�w��ԍ��ɑΉ�����f�o�C�X������
	}
	//�f�o�C�X�ԍ��������Ō��肷��ꍇ
	int cnt;
	if(inID < 0){								//�f�o�C�X�������őI������悤�Ɏw�肳��Ă���
		//�񋓂����f�o�C�X�̒����疢�g�p�̃f�o�C�X�����o����
		for(cnt = 0; cnt < deviceCount; cnt++){
			if(inputDeviceWork[cnt].active == 0){
				break;
			}
		}
		//�g�p�\�ȃf�o�C�X�������ł��Ȃ�����
		if(cnt == deviceCount){
			return MYERROR_DI::eCNTOVER;										//�g����f�o�C�X�̋󂫂�����
		}
		//���g�p�̃f�o�C�X�����蓖�Ă�
		inID = cnt;
	}
	//���̓f�o�C�X�𐶐�����
	if(FAILED(this->directInputObject->CreateDevice(
		inputDeviceWork[inID].rguid, outDirectInputDevice, NULL))){
		return MYERROR_DI::eCD;													//�f�o�C�X�������ł��Ȃ�
	}
	else{
		inputDeviceWork[inID].active = 1;								//�f�o�C�X�̎g�p��Ԃ��u�g�p���v�ɐݒ�
	}
	//���̓f�o�C�X�̃t�H�[�}�b�g��ݒ肷��
	LPCDIDATAFORMAT	deviceFormat;
	switch(inDeviceType){
		case DEVICETYPE::eKEYBOARD:	deviceFormat = &c_dfDIKeyboard;		break;			//�@�L�[�{�[�h
		case DEVICETYPE::eGAMEPAD:	deviceFormat = &c_dfDIJoystick2;	break;			//�@�Q�[���R���g���[���[
	}
	if(FAILED((*outDirectInputDevice)->SetDataFormat(deviceFormat))){	
		return MYERROR_DI::eSDF;												//�@�f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s����
	}
	//���̓f�o�C�X�̋������x����ݒ肷��
	DWORD cooperativeLevel;
	switch(inDeviceType){
		case DEVICETYPE::eKEYBOARD:	cooperativeLevel = DISCL_NONEXCLUSIVE | DISCL_FOREGROUND;	break;	//�@�L�[�{�[�h
		case DEVICETYPE::eGAMEPAD:	cooperativeLevel = DISCL_EXCLUSIVE	| DISCL_FOREGROUND;	break;	//�@�Q�[���R���g���[���[
	}
	if(FAILED((*outDirectInputDevice)->SetCooperativeLevel(inWindowHandle, cooperativeLevel))){		
		return MYERROR_DI::eSCL;												//�@�������x���̐ݒ肪���s����
	}

	//���̓Q�[���p�b�h�̏ꍇ�A�����L�[�ɐ�Ύ��������͑��Ύ����g�p�\�Ȃ��Ǝ����m�F����B
	switch(inDeviceType){
		case DEVICETYPE::eGAMEPAD:
			//��Ύ�����ё��Ύ����g�p�\�ȏꍇ�A���̐ݒ���s��
			if(FAILED((*outDirectInputDevice)->EnumObjects(enumObjectsGamePad, *outDirectInputDevice, DIDFT_AXIS))){
				return MYERROR_DI::eSCL;
			}
			break;
		case DEVICETYPE::eKEYBOARD:
			{
				DIPROPDWORD dipdw;
				dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
				dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
				dipdw.diph.dwObj		= 0;
				dipdw.diph.dwHow		= DIPH_DEVICE;
				dipdw.dwData			= 1;//�����ɔ���ł���L�[�̐��ɂȂ�̂��ȁH
				if(FAILED((*outDirectInputDevice)->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph))){
					return MYERROR_DI::eSCL;
				}
			}			
			break;
	}
	return MYERROR_DI::eNOTHING;

}

//=======================================================================
//DirectInput�̃f�o�C�X����N���X(�g�p����f�o�C�X�̐��ɉ����Đ錾�E�g�p����j
//=======================================================================
//=======================================================================
//�C���X�^���X�擾
//=======================================================================
CINPUT* CINPUT::instance[kinputDeviceMax] = {NULL, NULL, NULL, NULL};
CINPUT* CINPUT::getInstance(int inDeviceNumber)
{
	if(inDeviceNumber < 0 || inDeviceNumber >= kinputDeviceMax){
		return NULL;
	}
	if(instance[inDeviceNumber] == NULL){
		instance[inDeviceNumber] = new CINPUT( );
	}
	return instance[inDeviceNumber];
}
//=======================================================================
//�C���X�^���X�̊J��
void CINPUT::releaseInstance(int inDeviceNumber)
{
	if(inDeviceNumber < 0 || inDeviceNumber >= kinputDeviceMax){
		return;
	}
	if(instance[inDeviceNumber] != NULL){
		instance[inDeviceNumber]->release( );
		//�C���X�^���X�̊J��
		delete instance[inDeviceNumber];
		instance[inDeviceNumber] = NULL;
	}
}
//=======================================================================
//�S�C���X�^���X�̊J��
void CINPUT::releaseInstanceAll( )
{
	int cnt;
	for(cnt = 0; cnt < kinputDeviceMax; cnt++){
		CINPUT::releaseInstance(cnt);
	}
	CINPUT_OBJECT::releaseInstance( );
}

//=======================================================================
//�R���X�g���N�^
//�߂�l
//		�F����
//����
//		�F����
//�@�\�T�v
//		�F�ϐ��̏�����
//����
//		�F2005/08/31	�쐬
//=======================================================================
CINPUT::CINPUT( )
{
	this->device = NULL;
	this->id = -1;
	this->type = DEVICETYPE::eUNNON;
	this->numberOfKey = 0;
	this->informationOfKeys = NULL;
	this->checkKeyFunc = NULL;
	this->valid = FALSE;
}

//=======================================================================
//�L�[�{�[�h�ɂ����o�͂̏����L�[�f�[�^�̏��ɕϐ��Ɋi�[���܂�
void checkKeybd(	IDirectInputDevice8* inDevice,
					int inNumberOfKey,
					sKeyParam ioKeyInformation[ ]);
//�Q�[���p�b�h�ɂ����o�͂̏����L�[�f�[�^�̏��ɕϐ��Ɋi�[���܂�
void checkGamePad(	IDirectInputDevice8* inDevice,
					int inNumberOfKey,
					sKeyParam ioKeyInformation[ ]);
//=======================================================================
//init
//�߂�l
//		�F�f�o�C�X�����̐��ۂ�Ԃ�
//����
//		�F�E�B���h�E�n���h��
//		�F�f�o�C�X�̎��
//		�F�g�p�������{�^���̐�
//		�F�g�p�������{�^���̈ꗗ
//�@�\�T�v
//		�F���̓f�o�C�X���쐬����
//����
//		�F2005/08/31	�쐬
//=======================================================================
MYERROR_DI::TYPENAME CINPUT::init(	HWND inWindowHandle,
								DEVICETYPE::TYPENAME inDeviceType,
								int inNumberOfKey,
								sKeyParam ioKeyInformation[ ])
{		
	if(this->valid==TRUE){
		return MYERROR_DI::eNONRELEASE;								//���łɃf�o�C�X�������ς�
	}
	MYERROR_DI::TYPENAME rtv;
	//InputObject���擾
	CINPUT_OBJECT* inputObj = CINPUT_OBJECT::getInstance( );
	if(inputObj == NULL){
		return MYERROR_DI::eDIC;
	}
	//���̓f�o�C�X�̐���
	CINPUT_OBJECT* input = CINPUT_OBJECT::getInstance( );
	rtv = input->createDevice(	inWindowHandle,
								&this->device,
								inDeviceType,
								this->id);
	if(rtv != MYERROR_DI::eNOTHING){								//���̓f�o�C�X�̐����ɐ������Ȃ�����
		return rtv;												//���̓f�o�C�X�̐����Ɏ��s
	}
	//�L�[����o�^����
	this->numberOfKey = inNumberOfKey;							//�K�v�ȃL�[���̐���ۑ�����
	this->informationOfKeys = new sKeyParam[this->numberOfKey];	//�Ή�����L�[���z��𐶐�
	memcpy(	this->informationOfKeys,
			ioKeyInformation,
			this->numberOfKey * sizeof(sKeyParam));				//�w�肳�ꂽ�L�[����ۑ�����
	//�L�[�̓��̓`�F�b�N�Ɏg�p����֐��̐ݒ�
	switch(inDeviceType){
		case DEVICETYPE::eKEYBOARD:	this->checkKeyFunc = checkKeybd;	break;	//�@�L�[�{�[�h
		case DEVICETYPE::eGAMEPAD:	this->checkKeyFunc = checkGamePad;	break;	//�@�Q�[���R���g���[���[
	}
	//���̓f�o�C�X�̐����ɐ�������
	this->valid = TRUE;
	inputObj->addActivetyDeviceCounter(1);
	return MYERROR_DI::eNOTHING;
}


//=======================================================================
//checkKey
//�߂�l
//		�F����
//����
//		�F����
//�@�\�T�v
//		�F�f�o�C�X�̃L�[�i�{�^���j�̏�Ԃ��m�F����B
//		�F���̋@�\�͊�{�I�ɂP�t���[���P�ʂŌĂяo���A�ŐV�̓��͏�Ԃ�
//		�@�擾����
//����
//		�F2005/08/31	�쐬
//=======================================================================
void CINPUT::checkKey(void)
{
	if(this->valid == FALSE){		return;}	//�f�o�C�X����������Ă��Ȃ�
	if(this->checkKeyFunc == NULL){	return;}	//�L�[���͂𔻒肷��֐����ݒ�ł��Ă��Ȃ�
	//�o�^���Ă���L�[�̓��͏�Ԃ��擾����
	this->checkKeyFunc(	this->device,
						this->numberOfKey,
						this->informationOfKeys);
}

//=======================================================================
//checkKey
//�߂�l
//		�F����
//����
//		�F����
//�@�\�T�v
//		�F�L�[�̌��݂̏�Ԃ��擾����B
//����
//		�F2005/09/06	�쐬
//=======================================================================
MYKEYSTAT::TYPENAME CINPUT::checkStatus(MYKEY::TYPENAME inKeyID)
{
	if(this->valid == FALSE){		return MYKEYSTAT::eFREE;}		//�f�o�C�X����������Ă��Ȃ�
	if(this->checkKeyFunc == NULL){	return MYKEYSTAT::eFREE;}		//�L�[���͂𔻒肷��֐����ݒ�ł��Ă��Ȃ�
	int cnt;
	//�L�[�z��̒�����ΏۂɂȂ�L�[�����o����
	MYKEYSTAT::TYPENAME temp = MYKEYSTAT::eFREE;
	for(cnt = 0; cnt < this->numberOfKey; cnt++){
		if(inKeyID == this->informationOfKeys[cnt].myKey){			//�o�^�ς݂̃L�[�ꗗ����Y������L�[�����o����
			temp = this->informationOfKeys[cnt].keyPressStatus;
			if(temp != MYKEYSTAT::eFREE){
				break;
			}
		}
	}
	if(cnt == this->numberOfKey){return MYKEYSTAT::eNON;}			//�Y������L�[��������Ȃ�����
	//�L�[�̓��͏�Ԃ��m�F
	return temp;
}

//=======================================================================
//checkPress
//�߂�l
//		�F�L�[��������Ă����true��Ԃ�
//����
//		�F��Ԃ�m�肽���L�[��ID
//�@�\�T�v
//		�F�L�[��������Ă��邩�ۂ��m�F����B
//����
//		�F2005/08/31	�쐬
//=======================================================================
bool CINPUT::checkPress(MYKEY::TYPENAME inKeyID)
{
	if(this->valid == FALSE){		return false;}			//�f�o�C�X����������Ă��Ȃ�
	if(this->checkKeyFunc == NULL){	return false;}			//�L�[���͂𔻒肷��֐����ݒ�ł��Ă��Ȃ�
	int cnt;
	//�L�[�z��̒�����ΏۂɂȂ�L�[�����o����
	MYKEYSTAT::TYPENAME temp = MYKEYSTAT::eFREE;
	for(cnt = 0; cnt < this->numberOfKey;cnt++){
		if(inKeyID == this->informationOfKeys[cnt].myKey){		//�o�^�ς݂̃L�[�ꗗ����Y������L�[�����o����
			temp = this->informationOfKeys[cnt].keyPressStatus;
			if(temp != MYKEYSTAT::eFREE){
				break;
			}
		}
	}
	if(cnt == this->numberOfKey){return false;}				//�Y������L�[��������Ȃ�����
	if(temp == MYKEYSTAT::eON	|| temp == MYKEYSTAT::eDOWN){
		return true;
	}
	return false;
}

//=======================================================================
//checkPressOnce
//�߂�l
//		�F�L�[�������ꂽ����ł����true��Ԃ�
//����
//		�F��Ԃ�m�肽���L�[��ID
//�@�\�T�v
//		�F�L�[�������ꂽ���ォ�ۂ��m�F����B
//����
//		�F2005/08/31	�쐬
//=======================================================================
bool CINPUT::checkPressOnce(MYKEY::TYPENAME inKeyID)
{
	if(this->valid == FALSE){		return false;}	//�f�o�C�X����������Ă��Ȃ�
	if(this->checkKeyFunc == NULL){	return false;}	//�L�[���͂𔻒肷��֐����ݒ�ł��Ă��Ȃ�
	int cnt;
	//�L�[�z��̒�����ΏۂɂȂ�L�[�����o����
	MYKEYSTAT::TYPENAME temp = MYKEYSTAT::eFREE;
	for(cnt = 0; cnt < this->numberOfKey; cnt++){
		if(inKeyID == this->informationOfKeys[cnt].myKey){	//�o�^�ς݂̃L�[�ꗗ����Y������L�[�����o����
			temp = this->informationOfKeys[cnt].keyPressStatus;
			if(temp != MYKEYSTAT::eFREE){
				break;
			}
		}
	}
	if(cnt == this->numberOfKey){return false;}			//�Y������L�[��������Ȃ�����
	if(temp == MYKEYSTAT::eON){
		return true;
	}
	return false;
}

//=======================================================================
//checkKeyOnceLock
//�߂�l
//		�F����
//����
//		�F������Ԃ𖳌��ɂ������L�[��ID
//�@�\�T�v
//		�F�������̃L�[�Ɏg�p����ƁA�ēx�L�[�������Ȃ����܂œ��͂��Ă���
//		�@���ɂ��邱�Ƃ��ł���B
//����
//		�F2005/08/31	�쐬
//=======================================================================
void CINPUT::checkKeyOnceLock(MYKEY::TYPENAME inKeyID)
{
	if(this->valid == FALSE){		return;}	//�f�o�C�X����������Ă��Ȃ�
	if(this->checkKeyFunc == NULL){	return;}	//�L�[���͂𔻒肷��֐����ݒ�ł��Ă��Ȃ�
	//�L�[�z��̒�����ΏۂɂȂ�L�[�����o����
	int cnt;
	for(cnt = 0; cnt < this->numberOfKey; cnt++){
		if(inKeyID == this->informationOfKeys[cnt].myKey){	//�o�^�ς݂̃L�[�ꗗ����Y������L�[�����o����
			//�L�[�������Ȃ����܂œ��͂𖳌��ɂ���
			this->informationOfKeys[cnt].keyPressStatus = MYKEYSTAT::eONCELOCK;
		}
	}
}

//=======================================================================
//release
//�߂�l
//		�F����
//����
//		�F����
//�@�\�T�v
//		�F�Ăяo���K�{�I
//		�F���̓f�o�C�X���J������B
//		�F�g�p���̓��̓f�o�C�X�����ׂĊJ�������DirectInput�̃I�u�W�F�N�g
//		�@���J������
//����
//		�F2005/08/31	�쐬
//=======================================================================
void CINPUT::release(void)
{
	//	���͐���f�o�C�X�̉��
	if(this->device != NULL){
		this->device->Unacquire( );
		this->device->Release( );
		this->device = NULL;
	}
	//�L�[���̊J��
	if(this->informationOfKeys != NULL){
		delete[ ] this->informationOfKeys;
		this->informationOfKeys=NULL;
	}
	this->type = DEVICETYPE::eUNNON;
	this->checkKeyFunc = NULL;
	//���ׂẴf�o�C�X���J���������ADirectInput���J������
	CINPUT_OBJECT *input = CINPUT_OBJECT::getInstance( );
	if(this->id != -1){			//��������Ă��Ȃ��f�o�C�X���J���g�p�Ƃ����ꍇ�ւ̑Ή�
		if(input->addActivetyDeviceCounter(-1) == FALSE){
			CINPUT_OBJECT::releaseInstance( );
		}
	}
	this->id = -1;
}
//=======================================================================
//composeKeyStatus
//�߂�l
//		�F����
//����
//		�F��������L�[���̓������N���X
//�@�\�T�v
//		�F�Q�̓��̓f�o�C�X���P�̃f�o�C�X�Ƃ��Ďg�p���邽�߂̏������s���B
//		�F�Ăяo�����̃L�[���Ɉ������̃L�[�����R�s�[����B
//		�F�o���ɋ��ʂ��Ȃ��L�[�Ɋւ��Ă͏������s��Ȃ��B
//����
//		�F2005/09/06	�쐬
//=======================================================================
void CINPUT::composeKeyStatus(CINPUT* inKeyDevice)
{
	MYKEYSTAT::TYPENAME keyStatusMatrix[5][5] = {
		//�f�o�C�X�P
		//���b�N			���				����				����				����,
		{MYKEYSTAT::eONCELOCK, MYKEYSTAT::eONCELOCK,	MYKEYSTAT::eONCELOCK,	MYKEYSTAT::eONCELOCK,	MYKEYSTAT::eONCELOCK	},//���b�N	//�f�o�C�X�Q
		{MYKEYSTAT::eONCELOCK, MYKEYSTAT::eFREE,		MYKEYSTAT::eOFF,		MYKEYSTAT::eON,			MYKEYSTAT::eDOWN		},//���
		{MYKEYSTAT::eONCELOCK, MYKEYSTAT::eOFF,			MYKEYSTAT::eOFF,		MYKEYSTAT::eDOWN,		MYKEYSTAT::eDOWN		},//����
		{MYKEYSTAT::eONCELOCK, MYKEYSTAT::eON,			MYKEYSTAT::eDOWN,		MYKEYSTAT::eON,			MYKEYSTAT::eDOWN		},//����
		{MYKEYSTAT::eONCELOCK, MYKEYSTAT::eDOWN,		MYKEYSTAT::eDOWN,		MYKEYSTAT::eDOWN,		MYKEYSTAT::eDOWN		} //����
	};
	int cnt;
	MYKEYSTAT::TYPENAME tempA, tempB;
	//�Ăяo�����̃f�o�C�X�ɓK�p���Ă���S�ẴL�[�𒲂ׂ�
	for(cnt = 0; cnt < this->numberOfKey; cnt++){
		//�������̃f�o�C�X�ƈ�v����L�[�𒲂ׂ�
		tempB = inKeyDevice->checkStatus(this->informationOfKeys[cnt].myKey);
		if(tempB == MYKEYSTAT::eNON){	continue;	}	//�Ή�����L�[������
		tempA = this->informationOfKeys[cnt].keyPressStatus;
		this->informationOfKeys[cnt].keyPressStatus = keyStatusMatrix[tempA][tempB];
	}
}

//=======================================================================
//diEnumJoysticks
//�߂�l
//		�F����
//����
//		�F����
//�@�\�T�v
//		�FCINPUT_OBJECT::createDevice���\�b�h�̃T�u�֐�
//		�F�Q�[���p�b�h��񋓂���
//����
//		�F2005/08/31	�쐬
//=======================================================================
BOOL CALLBACK diEnumJoysticks(	const DIDEVICEINSTANCE* inDevice,
								void* inContext)
{
	//�񋓂����f�o�C�X�̑������擾
	int* cnt = (int*)inContext;
	CINPUT_OBJECT* input = CINPUT_OBJECT::getInstance( );
	//�S��ڈȏ�͎g�p���Ȃ�
	if((*cnt) < 4){
		input->getDevice_Pad(*cnt)->rguid = inDevice->guidInstance;	//�f�t�h�c��ۑ�
		input->getDevice_Pad(*cnt)->active = 0;						//��Ԃ��u���g�p�v�ɐݒ�
		(*cnt)++;												//�s�䐔�𑝉�������
		return DIENUM_CONTINUE;									//���̃f�o�C�X�̃`�F�b�N�Ɉڂ�
	}
	else{
		return DIENUM_STOP;										//�f�o�C�X�̗񋓂��I������
	}
}

//=======================================================================
//diEnumKeyBord
//�߂�l
//		�F����
//����
//		�F����
//�@�\�T�v
//		�FCINPUT_OBJECT::createDevice���\�b�h�̃T�u�֐�
//		�F�L�[�{�[�h��񋓂���
//����
//		�F2005/08/31	�쐬
//=======================================================================
BOOL CALLBACK diEnumKeyBord(	const DIDEVICEINSTANCE* inDevice,
								void* inContext)
{
	//�񋓂����f�o�C�X�̑������擾
	int* cnt = (int*)inContext;
	//�S��ڈȏ�͎g�p���Ȃ�
	CINPUT_OBJECT* input = CINPUT_OBJECT::getInstance( );
	if((*cnt) < 4){
		input->getDevice_KeyBd(*cnt)->rguid = inDevice->guidInstance;	//�f�t�h�c��ۑ�
		input->getDevice_KeyBd(*cnt)->active = 0;						//��Ԃ��u���g�p�v�ɐݒ�
		(*cnt)++;													//�s�䐔�𑝉�������
		return DIENUM_CONTINUE;										//���̃f�o�C�X�̃`�F�b�N�Ɉڂ�
	}
	else{
		return DIENUM_STOP;											//�f�o�C�X�̗񋓂��I������
	}
}


//=======================================================================
//checkKeybd
//�߂�l
//		�F����
//����
//		�F����
//�@�\�T�v
//		�FCINPUT::init���\�b�h�ŎQ�Ƃ���AcheckKey���\�b�h�ɂ��g�p�����
//		�F�L�[�{�[�h�ɂ����o�͂̏����L�[�f�[�^�̏��ɕϐ��Ɋi�[����
//����
//		�F2005/08/31	�쐬
//=======================================================================
void checkKeybd(	IDirectInputDevice8* inDevice,
					int inNumberOfKey,
					sKeyParam ioKeyInformation[ ])
{
	BYTE keyStatus[256];
	ZeroMemory(keyStatus, 256);
	//�f�o�C�X���g�p�\���m�F����
	int acquireCnt = 0;
	BOOL brFlag = FALSE;
	if(FAILED(inDevice->Poll( ))){
		do{
			if(SUCCEEDED(inDevice->Acquire( ))){
				brFlag = TRUE;
			}
			if(++acquireCnt > 30){
				return;
			}
		}while(brFlag == FALSE);
	}
	//	�L�[�{�[�h���擾
	if(FAILED(inDevice->GetDeviceState(sizeof(BYTE)*  256, &keyStatus))){
		return;
	}
	int cnt;	//	���[�v�J�E���^
	for(cnt = 0; cnt < 256; cnt++){
		if(keyStatus[cnt] != 0){
			keyStatus[cnt] = keyStatus[cnt];
		}
	}
	for(cnt=0; cnt<inNumberOfKey; cnt++){
		if(keyStatus[ioKeyInformation[cnt].vKeyCode] != 0){
			//�L�[���(�����n)
			switch(ioKeyInformation[cnt].keyPressStatus){
				case MYKEYSTAT::eFREE:	//	�����Ă���
				case MYKEYSTAT::eOFF:	//	���������ゾ����
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eON;
					break;
				case MYKEYSTAT::eON:	//	���������ゾ����
				case MYKEYSTAT::eDOWN:	//	�����Ă���
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eDOWN;
					break;
			}
		}
		else{
			//�L�[���(���n)
			switch(ioKeyInformation[cnt].keyPressStatus){
				case MYKEYSTAT::eFREE:	//	�����Ă���
				case MYKEYSTAT::eOFF:	//	���������ゾ����
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eFREE;
					break;
				case MYKEYSTAT::eON:	//	���������ゾ����
				case MYKEYSTAT::eDOWN:	//	�����Ă���
				case MYKEYSTAT::eONCELOCK://���b�N���
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eOFF;
					break;
			}
		}
	}
}

//=======================================================================
//checkGamePad
//�߂�l
//		�F����
//����
//		�F����
//�@�\�T�v
//		�FCINPUT::init���\�b�h�ŎQ�Ƃ���AcheckKey���\�b�h�ɂ��g�p�����
//		�F�Q�[���p�b�h�ɂ����o�͂̏����L�[�f�[�^�̏��ɕϐ��Ɋi�[����
//����
//		�F2005/08/31	�쐬
//=======================================================================
void checkGamePad(	IDirectInputDevice8* inDevice,
					int inNumberOfKey,
					sKeyParam ioKeyInformation[ ])
{
	DIJOYSTATE2 js;	//	�L�[�����i�[����
	//�f�o�C�X���g�p�\���m�F����
	int acquireCnt = 0;
	BOOL brFlag = FALSE;
	if(NULL == inDevice){return;}
	if(FAILED(inDevice->Poll( ))){
		do{
			if(SUCCEEDED(inDevice->Acquire( ))){
				brFlag = TRUE;
			}
			if(++acquireCnt > 30){
				return;
			}
		}while(brFlag == FALSE);
	}
	//	�p�b�h���擾
	if(FAILED(inDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js))){
		return;
	}
	int cnt;	//	���[�v�J�E���^
	BOOL keyFlag;
	for(cnt = 0; cnt < inNumberOfKey; cnt++){
		//�L�[�R�[�h�ɑΉ������f�[�^�ύX�������s��
		keyFlag = FALSE;
		if(		ioKeyInformation[cnt].vKeyCode == MYKEY::eLEFT){	keyFlag = js.lX < -256;}
		else if(ioKeyInformation[cnt].vKeyCode == MYKEY::eRIGHT){	keyFlag = js.lX >	256;}
		else if(ioKeyInformation[cnt].vKeyCode == MYKEY::eUP){		keyFlag = js.lY < -256;}
		else if(ioKeyInformation[cnt].vKeyCode == MYKEY::eDOWN){	keyFlag = js.lY >	256;}
		else if(ioKeyInformation[cnt].vKeyCode >= MYKEY::eSOFT &&	//�@�{�^���O�O�`�P�S
				ioKeyInformation[cnt].vKeyCode <= MYKEY::eQUIT){	
				keyFlag = js.rgbButtons[ioKeyInformation[cnt].vKeyCode - MYKEY::eSOFT];}
		else{	continue;	}//���Ή��̃L�[���ݒ肳��Ă���

		if(keyFlag != FALSE){
			//�L�[���(�����n)
			switch(ioKeyInformation[cnt].keyPressStatus){
				case MYKEYSTAT::eFREE:	//	�����Ă���
				case MYKEYSTAT::eOFF:	//	���������ゾ����
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eON;
					break;
				case MYKEYSTAT::eON:	//	���������ゾ����
				case MYKEYSTAT::eDOWN:	//	�����Ă���
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eDOWN;
					break;
			}
		}
		else{
			//�L�[���(���n)
			switch(ioKeyInformation[cnt].keyPressStatus){
				case MYKEYSTAT::eFREE:	//	�����Ă���
				case MYKEYSTAT::eOFF:	//	���������ゾ����
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eFREE;
					break;
				case MYKEYSTAT::eON:	//	���������ゾ����
				case MYKEYSTAT::eDOWN:	//	�����Ă���
				case MYKEYSTAT::eONCELOCK://���b�N���
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eOFF;
					break;
			}
		}
	}
}

//=======================================================================
//checkGamePad
//�߂�l
//		�F����
//����
//		�F�f�o�C�X
//		�F�f�o�C�X�̔ԍ�
//�@�\�T�v
//		�FCINPUT_OBJECT::createDevice���\�b�h�Ŏg�p�����
//		�F�Q�[���p�b�h�̕����L�[�ɐ�Ύ��Ȃ������Ύ��p�̐ݒ���s��
//����
//		�F2005/08/31	�쐬
//=======================================================================
BOOL CALLBACK enumObjectsGamePad(	const DIDEVICEOBJECTINSTANCE* inDevice,
									void* inContext)
{
	IDirectInputDevice8* directInputDevice=(IDirectInputDevice8*)inContext;
	DIPROPRANGE diprg; 
	//�f�o�C�X�̓��͂͐�Ύ������Ύ��ł���
	if(inDevice->dwType&DIDFT_AXIS){
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = inDevice->dwType;
		diprg.lMin = -1000;
		diprg.lMax = 1000;
		//	�f�o�C�X�̃^�C�v��ݒ肷��
		directInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	}
	else{
		MessageBox(NULL, "���̃^�C�v�̃R���g���[���͑Ή����Ă��܂���", NULL, MB_OK);
	}
	return DIENUM_CONTINUE;
}