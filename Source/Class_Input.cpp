/*CInput.cpp
 *ジョイスティックからの入力処理、変換を行う
 *
 */

#include "CInput.h"

//initialize
void CInput::initialize(HWND inWnd, CTRLID inID)
{
	//初期化
	ctrlID = inID;
	handle = inWnd;
}
//finalize
void CInput::finalize()
{
	//処理なし
}
//
PLAYKEYINFO CInput::checkPadState(DiClassEx* device)
{
	PLAYKEYINFO result = ePLAYKEY_NONE;			//戻り値用
	
	if((result = getPressedButton(device)) != ePLAYKEY_NONE)
	{
		return result;
	}
	result = getPressedCrossKey(device);
	return result;
}

//
PLAYKEYINFO CInput :: getPressedCrossKey(DiClassEx* device)
{
	//上キーの入力チェック
	if(device->getPadPressed(ctrlID, eKEY_UP))
	{
		if(device->getPadPressed(ctrlID, eKEY_LEFT))
		{
			return ePLAYKEY_UPLEFT;
		}
		else if(device->getPadPressed(ctrlID, eKEY_RIGHT))
		{
			return ePLAYKEY_UPRIGHT;
		}
		else
		{
			return ePLAYKEY_UP;
		}
	}
	//下キーの入力チェック
	if(device->getPadPressed(ctrlID, eKEY_DOWN))
	{
		if(device->getPadPressed(ctrlID, eKEY_LEFT))
		{
			return ePLAYKEY_DOWNLEFT;
		}
		else if(device->getPadPressed(ctrlID, eKEY_RIGHT))
		{
			return ePLAYKEY_DOWNRIGHT;
		}
		else
		{
			return ePLAYKEY_DOWN;
		}
	}
	if(device->getPadPressed(ctrlID, eKEY_LEFT))
	{
		return ePLAYKEY_LEFT;
	}
	else if(device->getPadPressed(ctrlID, eKEY_RIGHT))
	{
		return ePLAYKEY_RIGHT;
	}
	return ePLAYKEY_NONE;
}

PLAYKEYINFO CInput::getPressedButton(DiClassEx *device)
{
	bool resS, resM, resH, resD;
//パッドの４ボタンの状態格納
	resS = device->getPadPressed(ctrlID, eKEY_SOFT);
	resM = device->getPadPressed(ctrlID, eKEY_MIDDLE);
	resH = device->getPadPressed(ctrlID, eKEY_HARD);
	resD = device->getPadPressed(ctrlID, eKEY_DUST);
//ボタンキーチェック
	//４つ
	if(resS && resM && resH && resD) { return ePLAYKEY_ALLBUTTON; }
	//３つ
	if(resS && resM && resH) { return ePLAYKEY_ABC; }
	if(resS && resM && resD) { return ePLAYKEY_ABD; }
	if(resS && resH && resD) { return ePLAYKEY_ACD; }
	if(resM && resH && resD) { return ePLAYKEY_BCD; }
	//２つ
	if(resS && resM) { return ePLAYKEY_AB; }
	if(resS && resH) { return ePLAYKEY_AC; }
	if(resS && resD) { return ePLAYKEY_AD; }
	if(resM && resH) { return ePLAYKEY_BC; }
	if(resM && resD) { return ePLAYKEY_BD; }
	if(resH && resD) { return ePLAYKEY_CD; }
	//１つ
	if(resS) { return ePLAYKEY_A; }
	if(resM) { return ePLAYKEY_B; }
	if(resH) { return ePLAYKEY_C; }
	if(resD) { return ePLAYKEY_D; }
//そのほかのキー
	if(device->getPadPressed(ctrlID, eKEY_L1)) { return ePLAYKEY_L1; }
	if(device->getPadPressed(ctrlID, eKEY_L2)) { return ePLAYKEY_L2; }
	if(device->getPadPressed(ctrlID, eKEY_R1)) { return ePLAYKEY_R1; }
	if(device->getPadPressed(ctrlID, eKEY_R2)) { return ePLAYKEY_R2; }
	return ePLAYKEY_NONE;
}