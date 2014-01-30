/*CInput.cpp
 *�W���C�X�e�B�b�N����̓��͏����A�ϊ����s��
 *
 */

#include "CInput.h"

//initialize
void CInput::initialize(HWND inWnd, CTRLID inID)
{
	//������
	ctrlID = inID;
	handle = inWnd;
}
//finalize
void CInput::finalize()
{
	//�����Ȃ�
}
//
PLAYKEYINFO CInput::checkPadState(DiClassEx* device)
{
	PLAYKEYINFO result = ePLAYKEY_NONE;			//�߂�l�p
	
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
	//��L�[�̓��̓`�F�b�N
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
	//���L�[�̓��̓`�F�b�N
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
//�p�b�h�̂S�{�^���̏�Ԋi�[
	resS = device->getPadPressed(ctrlID, eKEY_SOFT);
	resM = device->getPadPressed(ctrlID, eKEY_MIDDLE);
	resH = device->getPadPressed(ctrlID, eKEY_HARD);
	resD = device->getPadPressed(ctrlID, eKEY_DUST);
//�{�^���L�[�`�F�b�N
	//�S��
	if(resS && resM && resH && resD) { return ePLAYKEY_ALLBUTTON; }
	//�R��
	if(resS && resM && resH) { return ePLAYKEY_ABC; }
	if(resS && resM && resD) { return ePLAYKEY_ABD; }
	if(resS && resH && resD) { return ePLAYKEY_ACD; }
	if(resM && resH && resD) { return ePLAYKEY_BCD; }
	//�Q��
	if(resS && resM) { return ePLAYKEY_AB; }
	if(resS && resH) { return ePLAYKEY_AC; }
	if(resS && resD) { return ePLAYKEY_AD; }
	if(resM && resH) { return ePLAYKEY_BC; }
	if(resM && resD) { return ePLAYKEY_BD; }
	if(resH && resD) { return ePLAYKEY_CD; }
	//�P��
	if(resS) { return ePLAYKEY_A; }
	if(resM) { return ePLAYKEY_B; }
	if(resH) { return ePLAYKEY_C; }
	if(resD) { return ePLAYKEY_D; }
//���̂ق��̃L�[
	if(device->getPadPressed(ctrlID, eKEY_L1)) { return ePLAYKEY_L1; }
	if(device->getPadPressed(ctrlID, eKEY_L2)) { return ePLAYKEY_L2; }
	if(device->getPadPressed(ctrlID, eKEY_R1)) { return ePLAYKEY_R1; }
	if(device->getPadPressed(ctrlID, eKEY_R2)) { return ePLAYKEY_R2; }
	return ePLAYKEY_NONE;
}