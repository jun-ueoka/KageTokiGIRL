#pragma warning(disable:4996)

#include	"Precompiler.h"
//#include "Class_DirectInput.h"
//#include "MySystem.h"

#pragma comment(lib,"dxguid")
#pragma comment(lib,"dInput8")



//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃検出した機器のGUIDと使用状態を受け取る構造体										　┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
struct INPUTDEVICEFLAG{
	GUID rguid;		//GUID
	int active;		//-1NODEVICE	0NOSELECTED	1SERECTED
};
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃入力機器を管理するクラス																　┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
class CINPUT_OBJECT{
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
private:static CINPUT_OBJECT		*instance;				//単一オブジェクトのインスタンス
private:LPDIRECTINPUT8			directInputObject;		//DirectInputオブジェクト
//列挙したデバイスの状態を保持する
private:INPUTDEVICEFLAG deviceFlags_Pad[4];			//ゲームパッドデバイス
private:INPUTDEVICEFLAG deviceFlags_Keybd[4];		//キーボードデバイス
private:int numberOfActiveDevice;					//使用しているデバイスの数
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//インスタンスの取得
public:static CINPUT_OBJECT* getInstance( );
	//コンストラクタ
	private:CINPUT_OBJECT( );
	//初期化
	private:MYERROR_DI::TYPENAME init( );
//インスタンスの解放
public:static void releaseInstance( );
	//解放
	private:void release(void);
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
public:INPUTDEVICEFLAG* getDevice_Pad(int inNumber){	return &deviceFlags_Pad[inNumber];}
public:INPUTDEVICEFLAG* getDevice_KeyBd(int inNumber){	return &deviceFlags_Keybd[inNumber];}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//デバイスの生成
public:MYERROR_DI::TYPENAME createDevice(	HWND inWindowHandle, 
								IDirectInputDevice8** outDirectInputDevice,
								DEVICETYPE::TYPENAME inDeviceType,
								int inID);
public:BOOL addActivetyDeviceCounter(int inAdd);
};
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃インスタンス取得																	　┃
//┃　2005/11/28	作成																　┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
CINPUT_OBJECT* CINPUT_OBJECT::instance = NULL;
CINPUT_OBJECT* CINPUT_OBJECT::getInstance( )
{
	if(instance == NULL){
		instance = new CINPUT_OBJECT( );
		if(instance != NULL){
			if(instance->init( ) != MYERROR_DI::eNOTHING){	//DirectInputObjectの生成が成功しなかった
				instance->releaseInstance( );
			}
		}
	}
	return instance;
}
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃インスタンスの開放																	　┃
//┃　2005/11/28	作成																　┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
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
//コンストラクタ
//戻り値
//		：無し
//引数
//		：無し
//機能概要
//		：変数の初期化
//履歴
//		：2005/08/31	作成
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
//戻り値
//		：無し
//引数
//		：無し
//機能概要
//		：呼び出し必須！
//		：開放処理
//履歴
//		：2005/08/31	作成
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
//戻り値
//		：DirectInputオブジェクトの生成されていればTRUE
//引数
//		：無し
//機能概要
//		：DirectInputオブジェクトの生成が行われているか否かを戻す
//履歴
//		：2005/08/31	作成
//=======================================================================
//	BOOL CINPUT_OBJECT::getStage( )
//	{
//		return (this->directInputObject != NULL);
//	}

//=======================================================================
//init
//戻り値
//		：DirectInputオブジェクトの生成の成否
//引数
//		：無し
//機能概要
//		：DirectInputオブジェクトの生成する
//履歴
//		：2005/08/31	作成
//=======================================================================
MYERROR_DI::TYPENAME CINPUT_OBJECT::init( )
{
	//	DirectInputの初期化
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
//ゲームパッドの方向キーに絶対軸ないし相対軸用の設定を行います
BOOL CALLBACK enumObjectsGamePad(	const DIDEVICEOBJECTINSTANCE* inDevice,
									void* inContext);
//ゲームパッドを列挙する場合に使用する
BOOL CALLBACK diEnumJoysticks(	const DIDEVICEINSTANCE* inDevice,
								void* inContext);
//キーボードを列挙する場合に使用する
BOOL CALLBACK diEnumKeyBord(	const DIDEVICEINSTANCE* inDevice,
								void* inContext);
//=======================================================================
//createDevice
//戻り値
//		：DirectInputオブジェクトの生成の成否
//引数
//		：ウィンドウハンドル
//		：生成した結果を受けるDirectInputデバイス
//		：デバイスの種類（キーボード、パッド）
//		：デバイス番号（0～3もしくはマイナス）
//機能概要
//		：入力デバイスを生成する
//		：デバイスの列挙を行う
//履歴
//		：2005/08/31	作成
//=======================================================================
MYERROR_DI::TYPENAME CINPUT_OBJECT::createDevice(	HWND inWindowHandle, 
										IDirectInputDevice8** outDirectInputDevice,
										DEVICETYPE::TYPENAME inDeviceType,
										int inID)
{
//デバイスの列挙（Windowsにアタッチ（登録）されているコントローラ等を探してくる）
	int deviceCount = 0;//列挙されたデバイスの総数を取得する
	//デバイスタイプに応じたフラグの決定
	DWORD devType;
	switch(inDeviceType){
		case DEVICETYPE::eKEYBOARD:	devType = DI8DEVCLASS_KEYBOARD;		break;		//　キーボード
		case DEVICETYPE::eGAMEPAD:	devType = DI8DEVCLASS_GAMECTRL;		break;		//　ゲームコントローラー
		default:						return MYERROR_DI::eED_UNNONTYPE;	//　未対応のデバイス
	}
	//デバイスタイプに応じたコールバック関数の決定
	LPDIENUMDEVICESCALLBACK devCallBack;
	switch(inDeviceType){
		case DEVICETYPE::eKEYBOARD:	devCallBack = diEnumKeyBord;		break;		//　キーボード
		case DEVICETYPE::eGAMEPAD:	devCallBack = diEnumJoysticks;		break;		//　ゲームコントローラー
	}
	//指定されたデバイスの列挙
	if(FAILED(this->directInputObject->EnumDevices(
		devType, devCallBack, &deviceCount, DIEDFL_ATTACHEDONLY))){
		return MYERROR_DI::eED;												//　デバイスの列挙に失敗
	}
	if(deviceCount < 1){				return MYERROR_DI::eNOTHING;}		//対応するデバイスが無い

	//DirectInputデバイスの生成
	INPUTDEVICEFLAG* inputDeviceWork;
	switch(inDeviceType){
		case DEVICETYPE::eKEYBOARD:	inputDeviceWork = deviceFlags_Keybd;	break;		//　キーボード
		case DEVICETYPE::eGAMEPAD:	inputDeviceWork = deviceFlags_Pad;		break;		//　ゲームコントローラー
	}
	if(inID >= 4 ||			//許可するデバイスはゲームパッド、キーボードともに４台まで 
		inID >= deviceCount){	//列挙できたデバイス数を超えた番号を指定された
		return MYERROR_DI::eCNTOVER;											//指定番号に対応するデバイスが無い
	}
	//デバイス番号を自動で決定する場合
	int cnt;
	if(inID < 0){								//デバイスを自動で選択するように指定されている
		//列挙したデバイスの中から未使用のデバイスを検出する
		for(cnt = 0; cnt < deviceCount; cnt++){
			if(inputDeviceWork[cnt].active == 0){
				break;
			}
		}
		//使用可能なデバイスが発見できなかった
		if(cnt == deviceCount){
			return MYERROR_DI::eCNTOVER;										//使えるデバイスの空きが無い
		}
		//未使用のデバイスを割り当てる
		inID = cnt;
	}
	//入力デバイスを生成する
	if(FAILED(this->directInputObject->CreateDevice(
		inputDeviceWork[inID].rguid, outDirectInputDevice, NULL))){
		return MYERROR_DI::eCD;													//デバイスが生成できない
	}
	else{
		inputDeviceWork[inID].active = 1;								//デバイスの使用状態を「使用中」に設定
	}
	//入力デバイスのフォーマットを設定する
	LPCDIDATAFORMAT	deviceFormat;
	switch(inDeviceType){
		case DEVICETYPE::eKEYBOARD:	deviceFormat = &c_dfDIKeyboard;		break;			//　キーボード
		case DEVICETYPE::eGAMEPAD:	deviceFormat = &c_dfDIJoystick2;	break;			//　ゲームコントローラー
	}
	if(FAILED((*outDirectInputDevice)->SetDataFormat(deviceFormat))){	
		return MYERROR_DI::eSDF;												//　データフォーマットの設定に失敗した
	}
	//入力デバイスの協調レベルを設定する
	DWORD cooperativeLevel;
	switch(inDeviceType){
		case DEVICETYPE::eKEYBOARD:	cooperativeLevel = DISCL_NONEXCLUSIVE | DISCL_FOREGROUND;	break;	//　キーボード
		case DEVICETYPE::eGAMEPAD:	cooperativeLevel = DISCL_EXCLUSIVE	| DISCL_FOREGROUND;	break;	//　ゲームコントローラー
	}
	if(FAILED((*outDirectInputDevice)->SetCooperativeLevel(inWindowHandle, cooperativeLevel))){		
		return MYERROR_DI::eSCL;												//　協調レベルの設定が失敗した
	}

	//入力ゲームパッドの場合、方向キーに絶対軸もしくは相対軸が使用可能なこと事を確認する。
	switch(inDeviceType){
		case DEVICETYPE::eGAMEPAD:
			//絶対軸および相対軸が使用可能な場合、軸の設定を行う
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
				dipdw.dwData			= 1;//同時に判定できるキーの数になるのかな？
				if(FAILED((*outDirectInputDevice)->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph))){
					return MYERROR_DI::eSCL;
				}
			}			
			break;
	}
	return MYERROR_DI::eNOTHING;

}

//=======================================================================
//DirectInputのデバイス制御クラス(使用するデバイスの数に応じて宣言・使用する）
//=======================================================================
//=======================================================================
//インスタンス取得
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
//インスタンスの開放
void CINPUT::releaseInstance(int inDeviceNumber)
{
	if(inDeviceNumber < 0 || inDeviceNumber >= kinputDeviceMax){
		return;
	}
	if(instance[inDeviceNumber] != NULL){
		instance[inDeviceNumber]->release( );
		//インスタンスの開放
		delete instance[inDeviceNumber];
		instance[inDeviceNumber] = NULL;
	}
}
//=======================================================================
//全インスタンスの開放
void CINPUT::releaseInstanceAll( )
{
	int cnt;
	for(cnt = 0; cnt < kinputDeviceMax; cnt++){
		CINPUT::releaseInstance(cnt);
	}
	CINPUT_OBJECT::releaseInstance( );
}

//=======================================================================
//コンストラクタ
//戻り値
//		：無し
//引数
//		：無し
//機能概要
//		：変数の初期化
//履歴
//		：2005/08/31	作成
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
//キーボードによる入出力の情報をキーデータの情報に変数に格納します
void checkKeybd(	IDirectInputDevice8* inDevice,
					int inNumberOfKey,
					sKeyParam ioKeyInformation[ ]);
//ゲームパッドによる入出力の情報をキーデータの情報に変数に格納します
void checkGamePad(	IDirectInputDevice8* inDevice,
					int inNumberOfKey,
					sKeyParam ioKeyInformation[ ]);
//=======================================================================
//init
//戻り値
//		：デバイス生成の成否を返す
//引数
//		：ウィンドウハンドル
//		：デバイスの種類
//		：使用したいボタンの数
//		：使用したいボタンの一覧
//機能概要
//		：入力デバイスを作成する
//履歴
//		：2005/08/31	作成
//=======================================================================
MYERROR_DI::TYPENAME CINPUT::init(	HWND inWindowHandle,
								DEVICETYPE::TYPENAME inDeviceType,
								int inNumberOfKey,
								sKeyParam ioKeyInformation[ ])
{		
	if(this->valid==TRUE){
		return MYERROR_DI::eNONRELEASE;								//すでにデバイスが生成済み
	}
	MYERROR_DI::TYPENAME rtv;
	//InputObjectを取得
	CINPUT_OBJECT* inputObj = CINPUT_OBJECT::getInstance( );
	if(inputObj == NULL){
		return MYERROR_DI::eDIC;
	}
	//入力デバイスの生成
	CINPUT_OBJECT* input = CINPUT_OBJECT::getInstance( );
	rtv = input->createDevice(	inWindowHandle,
								&this->device,
								inDeviceType,
								this->id);
	if(rtv != MYERROR_DI::eNOTHING){								//入力デバイスの生成に成功しなかった
		return rtv;												//入力デバイスの生成に失敗
	}
	//キー情報を登録する
	this->numberOfKey = inNumberOfKey;							//必要なキー情報の数を保存する
	this->informationOfKeys = new sKeyParam[this->numberOfKey];	//対応するキー情報配列を生成
	memcpy(	this->informationOfKeys,
			ioKeyInformation,
			this->numberOfKey * sizeof(sKeyParam));				//指定されたキー情報を保存する
	//キーの入力チェックに使用する関数の設定
	switch(inDeviceType){
		case DEVICETYPE::eKEYBOARD:	this->checkKeyFunc = checkKeybd;	break;	//　キーボード
		case DEVICETYPE::eGAMEPAD:	this->checkKeyFunc = checkGamePad;	break;	//　ゲームコントローラー
	}
	//入力デバイスの生成に成功した
	this->valid = TRUE;
	inputObj->addActivetyDeviceCounter(1);
	return MYERROR_DI::eNOTHING;
}


//=======================================================================
//checkKey
//戻り値
//		：無し
//引数
//		：無し
//機能概要
//		：デバイスのキー（ボタン）の状態を確認する。
//		：この機能は基本的に１フレーム単位で呼び出し、最新の入力状態を
//		　取得する
//履歴
//		：2005/08/31	作成
//=======================================================================
void CINPUT::checkKey(void)
{
	if(this->valid == FALSE){		return;}	//デバイスが生成されていない
	if(this->checkKeyFunc == NULL){	return;}	//キー入力を判定する関数が設定できていない
	//登録してあるキーの入力状態を取得する
	this->checkKeyFunc(	this->device,
						this->numberOfKey,
						this->informationOfKeys);
}

//=======================================================================
//checkKey
//戻り値
//		：無し
//引数
//		：無し
//機能概要
//		：キーの現在の状態を取得する。
//履歴
//		：2005/09/06	作成
//=======================================================================
MYKEYSTAT::TYPENAME CINPUT::checkStatus(MYKEY::TYPENAME inKeyID)
{
	if(this->valid == FALSE){		return MYKEYSTAT::eFREE;}		//デバイスが生成されていない
	if(this->checkKeyFunc == NULL){	return MYKEYSTAT::eFREE;}		//キー入力を判定する関数が設定できていない
	int cnt;
	//キー配列の中から対象になるキーを検出する
	MYKEYSTAT::TYPENAME temp = MYKEYSTAT::eFREE;
	for(cnt = 0; cnt < this->numberOfKey; cnt++){
		if(inKeyID == this->informationOfKeys[cnt].myKey){			//登録済みのキー一覧から該当するキーを検出した
			temp = this->informationOfKeys[cnt].keyPressStatus;
			if(temp != MYKEYSTAT::eFREE){
				break;
			}
		}
	}
	if(cnt == this->numberOfKey){return MYKEYSTAT::eNON;}			//該当するキーが見つからなかった
	//キーの入力状態を確認
	return temp;
}

//=======================================================================
//checkPress
//戻り値
//		：キーが押されていればtrueを返す
//引数
//		：状態を知りたいキーのID
//機能概要
//		：キーが押されているか否か確認する。
//履歴
//		：2005/08/31	作成
//=======================================================================
bool CINPUT::checkPress(MYKEY::TYPENAME inKeyID)
{
	if(this->valid == FALSE){		return false;}			//デバイスが生成されていない
	if(this->checkKeyFunc == NULL){	return false;}			//キー入力を判定する関数が設定できていない
	int cnt;
	//キー配列の中から対象になるキーを検出する
	MYKEYSTAT::TYPENAME temp = MYKEYSTAT::eFREE;
	for(cnt = 0; cnt < this->numberOfKey;cnt++){
		if(inKeyID == this->informationOfKeys[cnt].myKey){		//登録済みのキー一覧から該当するキーを検出した
			temp = this->informationOfKeys[cnt].keyPressStatus;
			if(temp != MYKEYSTAT::eFREE){
				break;
			}
		}
	}
	if(cnt == this->numberOfKey){return false;}				//該当するキーが見つからなかった
	if(temp == MYKEYSTAT::eON	|| temp == MYKEYSTAT::eDOWN){
		return true;
	}
	return false;
}

//=======================================================================
//checkPressOnce
//戻り値
//		：キーが押された直後であればtrueを返す
//引数
//		：状態を知りたいキーのID
//機能概要
//		：キーが押された直後か否か確認する。
//履歴
//		：2005/08/31	作成
//=======================================================================
bool CINPUT::checkPressOnce(MYKEY::TYPENAME inKeyID)
{
	if(this->valid == FALSE){		return false;}	//デバイスが生成されていない
	if(this->checkKeyFunc == NULL){	return false;}	//キー入力を判定する関数が設定できていない
	int cnt;
	//キー配列の中から対象になるキーを検出する
	MYKEYSTAT::TYPENAME temp = MYKEYSTAT::eFREE;
	for(cnt = 0; cnt < this->numberOfKey; cnt++){
		if(inKeyID == this->informationOfKeys[cnt].myKey){	//登録済みのキー一覧から該当するキーを検出した
			temp = this->informationOfKeys[cnt].keyPressStatus;
			if(temp != MYKEYSTAT::eFREE){
				break;
			}
		}
	}
	if(cnt == this->numberOfKey){return false;}			//該当するキーが見つからなかった
	if(temp == MYKEYSTAT::eON){
		return true;
	}
	return false;
}

//=======================================================================
//checkKeyOnceLock
//戻り値
//		：無し
//引数
//		：押下状態を無効にしたいキーのID
//機能概要
//		：押下中のキーに使用すると、再度キーを押しなおすまで入力していな
//		　事にすることができる。
//履歴
//		：2005/08/31	作成
//=======================================================================
void CINPUT::checkKeyOnceLock(MYKEY::TYPENAME inKeyID)
{
	if(this->valid == FALSE){		return;}	//デバイスが生成されていない
	if(this->checkKeyFunc == NULL){	return;}	//キー入力を判定する関数が設定できていない
	//キー配列の中から対象になるキーを検出する
	int cnt;
	for(cnt = 0; cnt < this->numberOfKey; cnt++){
		if(inKeyID == this->informationOfKeys[cnt].myKey){	//登録済みのキー一覧から該当するキーを検出した
			//キーを押しなおすまで入力を無効にする
			this->informationOfKeys[cnt].keyPressStatus = MYKEYSTAT::eONCELOCK;
		}
	}
}

//=======================================================================
//release
//戻り値
//		：無し
//引数
//		：無し
//機能概要
//		：呼び出し必須！
//		：入力デバイスを開放する。
//		：使用中の入力デバイスをすべて開放するとDirectInputのオブジェクト
//		　を開放する
//履歴
//		：2005/08/31	作成
//=======================================================================
void CINPUT::release(void)
{
	//	入力制御デバイスの解放
	if(this->device != NULL){
		this->device->Unacquire( );
		this->device->Release( );
		this->device = NULL;
	}
	//キー情報の開放
	if(this->informationOfKeys != NULL){
		delete[ ] this->informationOfKeys;
		this->informationOfKeys=NULL;
	}
	this->type = DEVICETYPE::eUNNON;
	this->checkKeyFunc = NULL;
	//すべてのデバイスを開放した時、DirectInputも開放する
	CINPUT_OBJECT *input = CINPUT_OBJECT::getInstance( );
	if(this->id != -1){			//生成されていないデバイスを開放使用とした場合への対応
		if(input->addActivetyDeviceCounter(-1) == FALSE){
			CINPUT_OBJECT::releaseInstance( );
		}
	}
	this->id = -1;
}
//=======================================================================
//composeKeyStatus
//戻り値
//		：無し
//引数
//		：合成するキー情報の入ったクラス
//機能概要
//		：２つの入力デバイスを１つのデバイスとして使用するための処理を行う。
//		：呼び出し側のキー情報に引数側のキー情報をコピーする。
//		：双方に共通しないキーに関しては処理を行わない。
//履歴
//		：2005/09/06	作成
//=======================================================================
void CINPUT::composeKeyStatus(CINPUT* inKeyDevice)
{
	MYKEYSTAT::TYPENAME keyStatusMatrix[5][5] = {
		//デバイス１
		//ロック			解放				解除				入力				押下,
		{MYKEYSTAT::eONCELOCK, MYKEYSTAT::eONCELOCK,	MYKEYSTAT::eONCELOCK,	MYKEYSTAT::eONCELOCK,	MYKEYSTAT::eONCELOCK	},//ロック	//デバイス２
		{MYKEYSTAT::eONCELOCK, MYKEYSTAT::eFREE,		MYKEYSTAT::eOFF,		MYKEYSTAT::eON,			MYKEYSTAT::eDOWN		},//解放
		{MYKEYSTAT::eONCELOCK, MYKEYSTAT::eOFF,			MYKEYSTAT::eOFF,		MYKEYSTAT::eDOWN,		MYKEYSTAT::eDOWN		},//解除
		{MYKEYSTAT::eONCELOCK, MYKEYSTAT::eON,			MYKEYSTAT::eDOWN,		MYKEYSTAT::eON,			MYKEYSTAT::eDOWN		},//入力
		{MYKEYSTAT::eONCELOCK, MYKEYSTAT::eDOWN,		MYKEYSTAT::eDOWN,		MYKEYSTAT::eDOWN,		MYKEYSTAT::eDOWN		} //押下
	};
	int cnt;
	MYKEYSTAT::TYPENAME tempA, tempB;
	//呼び出し側のデバイスに適用している全てのキーを調べる
	for(cnt = 0; cnt < this->numberOfKey; cnt++){
		//合成元のデバイスと一致するキーを調べる
		tempB = inKeyDevice->checkStatus(this->informationOfKeys[cnt].myKey);
		if(tempB == MYKEYSTAT::eNON){	continue;	}	//対応するキーが無い
		tempA = this->informationOfKeys[cnt].keyPressStatus;
		this->informationOfKeys[cnt].keyPressStatus = keyStatusMatrix[tempA][tempB];
	}
}

//=======================================================================
//diEnumJoysticks
//戻り値
//		：無し
//引数
//		：無し
//機能概要
//		：CINPUT_OBJECT::createDeviceメソッドのサブ関数
//		：ゲームパッドを列挙する
//履歴
//		：2005/08/31	作成
//=======================================================================
BOOL CALLBACK diEnumJoysticks(	const DIDEVICEINSTANCE* inDevice,
								void* inContext)
{
	//列挙したデバイスの総数を取得
	int* cnt = (int*)inContext;
	CINPUT_OBJECT* input = CINPUT_OBJECT::getInstance( );
	//４台目以上は使用しない
	if((*cnt) < 4){
		input->getDevice_Pad(*cnt)->rguid = inDevice->guidInstance;	//ＧＵＩＤを保存
		input->getDevice_Pad(*cnt)->active = 0;						//状態を「未使用」に設定
		(*cnt)++;												//壮台数を増加させる
		return DIENUM_CONTINUE;									//次のデバイスのチェックに移る
	}
	else{
		return DIENUM_STOP;										//デバイスの列挙を終了する
	}
}

//=======================================================================
//diEnumKeyBord
//戻り値
//		：無し
//引数
//		：無し
//機能概要
//		：CINPUT_OBJECT::createDeviceメソッドのサブ関数
//		：キーボードを列挙する
//履歴
//		：2005/08/31	作成
//=======================================================================
BOOL CALLBACK diEnumKeyBord(	const DIDEVICEINSTANCE* inDevice,
								void* inContext)
{
	//列挙したデバイスの総数を取得
	int* cnt = (int*)inContext;
	//４台目以上は使用しない
	CINPUT_OBJECT* input = CINPUT_OBJECT::getInstance( );
	if((*cnt) < 4){
		input->getDevice_KeyBd(*cnt)->rguid = inDevice->guidInstance;	//ＧＵＩＤを保存
		input->getDevice_KeyBd(*cnt)->active = 0;						//状態を「未使用」に設定
		(*cnt)++;													//壮台数を増加させる
		return DIENUM_CONTINUE;										//次のデバイスのチェックに移る
	}
	else{
		return DIENUM_STOP;											//デバイスの列挙を終了する
	}
}


//=======================================================================
//checkKeybd
//戻り値
//		：無し
//引数
//		：無し
//機能概要
//		：CINPUT::initメソッドで参照され、checkKeyメソッドにより使用される
//		：キーボードによる入出力の情報をキーデータの情報に変数に格納する
//履歴
//		：2005/08/31	作成
//=======================================================================
void checkKeybd(	IDirectInputDevice8* inDevice,
					int inNumberOfKey,
					sKeyParam ioKeyInformation[ ])
{
	BYTE keyStatus[256];
	ZeroMemory(keyStatus, 256);
	//デバイスが使用可能か確認する
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
	//	キーボード情報取得
	if(FAILED(inDevice->GetDeviceState(sizeof(BYTE)*  256, &keyStatus))){
		return;
	}
	int cnt;	//	ループカウンタ
	for(cnt = 0; cnt < 256; cnt++){
		if(keyStatus[cnt] != 0){
			keyStatus[cnt] = keyStatus[cnt];
		}
	}
	for(cnt=0; cnt<inNumberOfKey; cnt++){
		if(keyStatus[ioKeyInformation[cnt].vKeyCode] != 0){
			//キー状態(押下系)
			switch(ioKeyInformation[cnt].keyPressStatus){
				case MYKEYSTAT::eFREE:	//	離していた
				case MYKEYSTAT::eOFF:	//	離した直後だった
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eON;
					break;
				case MYKEYSTAT::eON:	//	押した直後だった
				case MYKEYSTAT::eDOWN:	//	押していた
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eDOWN;
					break;
			}
		}
		else{
			//キー状態(離系)
			switch(ioKeyInformation[cnt].keyPressStatus){
				case MYKEYSTAT::eFREE:	//	離していた
				case MYKEYSTAT::eOFF:	//	離した直後だった
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eFREE;
					break;
				case MYKEYSTAT::eON:	//	押した直後だった
				case MYKEYSTAT::eDOWN:	//	押していた
				case MYKEYSTAT::eONCELOCK://ロック状態
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eOFF;
					break;
			}
		}
	}
}

//=======================================================================
//checkGamePad
//戻り値
//		：無し
//引数
//		：無し
//機能概要
//		：CINPUT::initメソッドで参照され、checkKeyメソッドにより使用される
//		：ゲームパッドによる入出力の情報をキーデータの情報に変数に格納する
//履歴
//		：2005/08/31	作成
//=======================================================================
void checkGamePad(	IDirectInputDevice8* inDevice,
					int inNumberOfKey,
					sKeyParam ioKeyInformation[ ])
{
	DIJOYSTATE2 js;	//	キー情報を格納する
	//デバイスが使用可能か確認する
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
	//	パッド情報取得
	if(FAILED(inDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js))){
		return;
	}
	int cnt;	//	ループカウンタ
	BOOL keyFlag;
	for(cnt = 0; cnt < inNumberOfKey; cnt++){
		//キーコードに対応したデータ変更処理を行う
		keyFlag = FALSE;
		if(		ioKeyInformation[cnt].vKeyCode == MYKEY::eLEFT){	keyFlag = js.lX < -256;}
		else if(ioKeyInformation[cnt].vKeyCode == MYKEY::eRIGHT){	keyFlag = js.lX >	256;}
		else if(ioKeyInformation[cnt].vKeyCode == MYKEY::eUP){		keyFlag = js.lY < -256;}
		else if(ioKeyInformation[cnt].vKeyCode == MYKEY::eDOWN){	keyFlag = js.lY >	256;}
		else if(ioKeyInformation[cnt].vKeyCode >= MYKEY::eSOFT &&	//　ボタン００～１４
				ioKeyInformation[cnt].vKeyCode <= MYKEY::eQUIT){	
				keyFlag = js.rgbButtons[ioKeyInformation[cnt].vKeyCode - MYKEY::eSOFT];}
		else{	continue;	}//未対応のキーが設定されている

		if(keyFlag != FALSE){
			//キー状態(押下系)
			switch(ioKeyInformation[cnt].keyPressStatus){
				case MYKEYSTAT::eFREE:	//	離していた
				case MYKEYSTAT::eOFF:	//	離した直後だった
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eON;
					break;
				case MYKEYSTAT::eON:	//	押した直後だった
				case MYKEYSTAT::eDOWN:	//	押していた
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eDOWN;
					break;
			}
		}
		else{
			//キー状態(離系)
			switch(ioKeyInformation[cnt].keyPressStatus){
				case MYKEYSTAT::eFREE:	//	離していた
				case MYKEYSTAT::eOFF:	//	離した直後だった
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eFREE;
					break;
				case MYKEYSTAT::eON:	//	押した直後だった
				case MYKEYSTAT::eDOWN:	//	押していた
				case MYKEYSTAT::eONCELOCK://ロック状態
					ioKeyInformation[cnt].keyPressStatus = MYKEYSTAT::eOFF;
					break;
			}
		}
	}
}

//=======================================================================
//checkGamePad
//戻り値
//		：無し
//引数
//		：デバイス
//		：デバイスの番号
//機能概要
//		：CINPUT_OBJECT::createDeviceメソッドで使用される
//		：ゲームパッドの方向キーに絶対軸ないし相対軸用の設定を行う
//履歴
//		：2005/08/31	作成
//=======================================================================
BOOL CALLBACK enumObjectsGamePad(	const DIDEVICEOBJECTINSTANCE* inDevice,
									void* inContext)
{
	IDirectInputDevice8* directInputDevice=(IDirectInputDevice8*)inContext;
	DIPROPRANGE diprg; 
	//デバイスの入力は絶対軸か相対軸である
	if(inDevice->dwType&DIDFT_AXIS){
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = inDevice->dwType;
		diprg.lMin = -1000;
		diprg.lMax = 1000;
		//	デバイスのタイプを設定する
		directInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	}
	else{
		MessageBox(NULL, "このタイプのコントローラは対応していません", NULL, MB_OK);
	}
	return DIENUM_CONTINUE;
}