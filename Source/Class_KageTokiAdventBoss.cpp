//***********************************************************************************
//!	�t�@�C����		Class_KageTokiAdventBoss.cpp
//
//	�@�\			�A�h�x���`���[�Q�[���̃{�X�x�[�X�N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	[KageTokiGIRL]
//	�X�V			2009/02/19	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_BOSS::�R���X�g���N�^
	//
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				inResource		���b�V���t�H���_��
	//				inTexture		�e�N�X�`���t�H���_��
	//	�X�V		2009/02/19		<�V�K>
	//================================================================================
	CKAGETOKI_ADVENT_BOSS::CKAGETOKI_ADVENT_BOSS(CONST DEV		inDev,
												 CONST LPSTR	inName,
												 CONST LPSTR	inResource,
												 CONST LPSTR	inTexture)	:
	CKAGETOKI_ADVENT_CHARACTER(inDev, inName, inResource, inTexture)
	{
	//	�����̏�����
		this->iMaxLife					= 1;					//!< �ő�̗�
		this->iLife						= this->iMaxLife;		//!< �c��̗�

	//	�t���O�Q
		this->setThisMessage(eTMSG_ENEMY);						//!< �G�\��

	//	�`��f�o�C�X�ɒ�`����Ă���`���o�b�t�@�̔\�͂��擾
		D3DVIEWPORT9	viewPort;
		inDev->GetViewport(&viewPort);

	//	�C���^�[�t�F�[�X�֘A
		this->bDrawInterface	= TRUE;
		this->interfaceTex		= NULL;
	//	����
	//	�̗͊�Ղ̐ݒ�
		DX::setVertex2D(
			&this->lifeBase[0],
			(float)(viewPort.Width  - 20 - 256),
			(float)(viewPort.Height - 20 - 256),
			0.1f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.0f, 0.0f);
		DX::setVertex2D(
			&this->lifeBase[1],
			(float)(viewPort.Width  - 20      ),
			(float)(viewPort.Height - 20 - 256),
			0.1f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.5f, 0.0f);
		DX::setVertex2D(
			&this->lifeBase[2],
			(float)(viewPort.Width  - 20      ),
			(float)(viewPort.Height - 20      ),
			0.1f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.5f, 0.5f);
		DX::setVertex2D(
			&this->lifeBase[3],
			(float)(viewPort.Width  - 20 - 256),
			(float)(viewPort.Height - 20      ),
			0.1f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.0f, 0.5f);

	//	�̗̖͂ڐ��g��Ղ̐ݒ�
		DX::setVertex2D(
			&this->lifeFrameBase[0],
			this->lifeBase[0].x +  35.0f,
			this->lifeBase[0].y + 184.0f,
			0.03f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.1f, 0.51f);
		DX::setVertex2D(
			&this->lifeFrameBase[1],
			this->lifeBase[0].x +  35.0f + 190.0f,
			this->lifeBase[0].y + 184.0f,
			0.03f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.4f, 0.51f);
		DX::setVertex2D(
			&this->lifeFrameBase[2],
			this->lifeBase[0].x +  35.0f + 190.0f,
			this->lifeBase[0].y + 184.0f + 24.0f,
			0.03f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.4f, 0.56f);
		DX::setVertex2D(
			&this->lifeFrameBase[3],
			this->lifeBase[0].x +  35.0f,
			this->lifeBase[0].y + 184.0f + 24.0f,
			0.03f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.1f, 0.56f);

	//	�̗̖͂ڐ��̐ݒ�
		DX::setVertex2D(
			&this->lifeChip[0],
			this->lifeBase[0].x +  35.0f,
			this->lifeBase[0].y + 184.0f,
			0.02f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.1f, 0.59f);
		DX::setVertex2D(
			&this->lifeChip[1],
			this->lifeBase[0].x +  35.0f + (190.0f / EXE_MAXLIFE),
			this->lifeBase[0].y + 184.0f,
			0.02f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.4f, 0.59f);
		DX::setVertex2D(
			&this->lifeChip[2],
			this->lifeBase[0].x +  35.0f + (190.0f / EXE_MAXLIFE),
			this->lifeBase[0].y + 184.0f + 24.0f,
			0.02f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.4f, 0.61f);
		DX::setVertex2D(
			&this->lifeChip[3],
			this->lifeBase[0].x +  35.0f,
			this->lifeBase[0].y + 184.0f + 24.0f,
			0.02f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.1f, 0.61f);

	//	�̗̖͂ڐ��g�̐ݒ�
		DX::setVertex2D(
			&this->lifeFrame[0],
			this->lifeBase[0].x +  30.0f,
			this->lifeBase[0].y + 180.0f,
			0.01f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.015f, 0.63f);
		DX::setVertex2D(
			&this->lifeFrame[1],
			this->lifeBase[0].x +  30.0f + 212.0f,
			this->lifeBase[0].y + 180.0f,
			0.01f, 0.0f, D3DCOLOR_RGBA(255,255,255,255),  0.45f, 0.63f);
		DX::setVertex2D(
			&this->lifeFrame[2],
			this->lifeBase[0].x +  30.0f + 212.0f,
			this->lifeBase[0].y + 180.0f + 32.0f,
			0.01f, 0.0f, D3DCOLOR_RGBA(255,255,255,255),  0.45f, 0.69f);
		DX::setVertex2D(
			&this->lifeFrame[3],
			this->lifeBase[0].x +  30.0f,
			this->lifeBase[0].y + 180.0f + 32.0f,
			0.01f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.015f, 0.69f);
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_BOSS::�f�X�g���N�^
	//
	//	�X�V		2009/02/19		<�V�K>
	//================================================================================
	CKAGETOKI_ADVENT_BOSS::~CKAGETOKI_ADVENT_BOSS(void)
	{
	//	���֘A�̉��
		SAFE_RELEASE(this->interfaceTex);			//!< �C���^�[�t�F�[�X�e�N�X�`��
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_BOSS::opeTurnToPlayer
	//
	//	�@�\		�v���C���[�̕�����������
	//	�X�V		2009/01/13		<�V�K>
	//================================================================================
	void
	CKAGETOKI_ADVENT_BOSS::opeTurnToPlayer(void)
	{
	//	�����̐���
		switch(this->way)
		{
		//	�������
			case eADCW_NO:
			//	�����̌����ɂȂ��Ă���Ȃ�A�������ɐݒ肷��
				if(	(this->getAngleY() >=   0.0f) &&
					(this->getAngleY() <  180.0f))
				{
					this->setAngleY(ADVENT_LEFTANGLE);
					this->way = eADCW_LEFT;
				}
			//	��L�ȊO�́A�E�����ɐݒ肷��
				else
				{
					this->setAngleY(ADVENT_RIGHTANGLE);
					this->way = eADCW_RIGHT;
				}
				break;

		//	���������Ă���
			case eADCW_LEFT:
			//	�v���C���[���E�ɂ���Ȃ�A�E����������(�����)
				if(	this->getLocX() < (*this->player)->getLocX())
				{
					this->setAngleY(ADVENT_RIGHTANGLE);
					this->way = eADCW_RIGHT;
				}
				break;

		//	�E�������Ă���
			case eADCW_RIGHT:
			//	�v���C���[�����ɂ���Ȃ�A������������(�����)
				if(	this->getLocX() > (*this->player)->getLocX())
				{
					this->setAngleY(ADVENT_LEFTANGLE);
					this->way = eADCW_LEFT;
				}
				break;

		//	��L�ȊO�́A���������Ȃ�
			default:	
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_BOSS::resetParameter
	//
	//	�@�\		�p�����[�^�̃��Z�b�g
	//	�X�V		2009/02/18		<�V�K>
	//================================================================================
	void
	CKAGETOKI_ADVENT_BOSS::resetParameter(void)
	{
		this->iLife			= this->iMaxLife;		//!< �c��̗�
	}

	//================================================================================
	//!	���\�b�h��	CKAGETOKI_ADVENT_BOSS::drawInterface
	//
	//	�@�\		�C���^�[�t�F�[�X�̕`��
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2009/02/19		<�V�K>
	//================================================================================
	void
	CKAGETOKI_ADVENT_BOSS::drawInterface(CONST DEV inDev)
	{
	//	�C���^�[�t�F�[�X�`��m�F���L���ł���΁A�`��
		if(	this->bDrawInterface	&&
			this->interfaceTex)
		{
			DWORD		dwFVF=0;		//!< FVF�̃o�b�N�A�b�v�p

			inDev->SetTexture(0, this->interfaceTex);
			inDev->GetFVF(&dwFVF);
			inDev->SetFVF(D3DFVF_VERTEX_2D);
			inDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, this->lifeBase,      sizeof(CUSTOM_VERTEX_2D));
			inDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, this->lifeFrameBase, sizeof(CUSTOM_VERTEX_2D));

			float	lifeDevide	= 190.0f / this->iMaxLife;
			for(int i = 0; i < this->iLife; i++)
			{
				this->lifeChip[0].x	= this->lifeBase[0].x + 35.0f + (lifeDevide * (float) i);
				this->lifeChip[1].x	= this->lifeBase[0].x + 35.0f + (lifeDevide * (float)(i+1));
				this->lifeChip[2].x	= this->lifeBase[0].x + 35.0f + (lifeDevide * (float)(i+1));
				this->lifeChip[3].x	= this->lifeBase[0].x + 35.0f + (lifeDevide * (float) i);
				inDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, this->lifeChip, sizeof(CUSTOM_VERTEX_2D));
			}

			inDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, this->lifeFrame, sizeof(CUSTOM_VERTEX_2D));
			inDev->SetFVF(dwFVF);
		}
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------