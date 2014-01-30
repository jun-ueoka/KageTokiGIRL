#pragma warning(disable:4996)
#include <windows.h>

#include	"Precompiler.h"

//#include "MySystem.h"

//����������������������������������������������������������������������������������������������
//���V�X�e���ŋ��L����ėp�@�\�Q															�@��
//����������������������������������������������������������������������������������������������
namespace GCom{

	namespace Cnt
	{
		//������������������������������������������������������������������������������������������
		//���J�E���^�[�N���X2������																�@��
		//���@2005/01/18	�쐬																�@��
		//������������������������������������������������������������������������������������������
		cNormal::cNormal( )
		{
			this->counter = 0.0f;
			this->startValue = 0.0f;
			this->endValue = 0.0f;
			this->incValue = 0.0f;
			this->mode = Mode::eNon;
		}
		//�J�E���^�[��ݒ肷��
		Status::TYPENAME cNormal::init(float inStartValue, float inEndValue, float inIncValue, Mode::TYPENAME inMode)
		{
			//�ݒ���e�̐������𒲂ׂ�
			if(inMode == Mode::eNon){								return Status::eErr;}//�����t���O
			//�J�E���^�[�̑����i�����j�ʂ�0�̏ꍇ
			if(inIncValue == 0.0f){									return Status::eErr;}

			// �����l�@���@�I���l�@�̊֌W�ŃJ�E���^�[����������ݒ�
			if(inStartValue < inEndValue && inIncValue < 0.0f){		return Status::eErr;}
			// �����l�@���@�I���l�@�̊֌W�ŃJ�E���^�[����������ݒ�
			if(inStartValue > inEndValue && inIncValue > 0.0f){		return Status::eErr;}


			//
			this->startValue		= inStartValue;
			this->endValue			= inEndValue;
			this->incValue			= inIncValue;
			if(inStartValue != inEndValue){		this->mode			= inMode;		}
			else{								this->mode			= Mode::eZero;	}
			this->counter			= this->startValue;
			return Status::eNormal;
		}

		//�J��Ԃ����[�h
		void cNormal::increasedCounter_Mode_RepeatsB(float inAddValue)
		{
			this->counter += this->incValue * inAddValue;

		//�����l�E�I���l�̑召�֌W�ɂ��hige low������
			float hige, low;
			if(this->startValue < this->endValue){
				hige = this->endValue;
				low = this->startValue;
			}else{
				low = this->endValue;
				hige = this->startValue;
			}
		//�J�E���^���ŏ�����ő�͈̔͂Ɏ��߂�
			while(this->counter < low || this->counter > hige){
				//�ŏ��l�ȉ��̏ꍇ
				if(this->counter < low){
					this->counter = hige + (this->counter - low);
				}
				//�ő�l�ȏ�̏ꍇ
				else if(this->counter > hige){
					this->counter = low + (this->counter - hige);
				}
			}
		}
		//�J��Ԃ����[�h
		void cNormal::increasedCounter_Mode_RepeatsA(float inAddValue)
		{
			this->counter += this->incValue * inAddValue;

		//�����l�E�I���l�̑召�֌W�ɂ��hige low������
			float hige, low;
			if(this->startValue < this->endValue){
				hige = this->endValue;
				low = this->startValue;
			}else{
				low = this->endValue;
				hige = this->startValue;
			}
		//�J�E���^���ŏ�����ő�͈̔͂Ɏ��߂�
			while(this->counter < low || this->counter > hige){
				//�ŏ��l�ȉ��̏ꍇ
				if(this->counter < low){
					this->counter = hige;
				}
				//�ő�l�ȏ�̏ꍇ
				else if(this->counter > hige){
					this->counter = low;
				}
			}
		}
		//���A���[�h
		void cNormal::increasedCounter_Mode_Tourn(float inAddValue)
		{
			this->counter += this->incValue * inAddValue;

		//�����l�E�I���l�̑召�֌W�ɂ��hige low������
			float hige, low;
			if(this->startValue < this->endValue){
				hige = this->endValue;
				low = this->startValue;
			}else{
				low = this->endValue;
				hige = this->startValue;
			}
		//�J�E���^���ŏ�����ő�͈̔͂Ɏ��߂�
			while(this->counter < low || this->counter > hige){
			//�J�E���^���͈͂𒴂����ꍇ�A�����i�����j�l�𕄍����]����
				this->incValue = -this->incValue;
				//�ŏ��l�ȉ��̏ꍇ
				if(this->counter < low){
					this->counter = low + low - this->counter;
				}
				//�ő�l�ȏ�̏ꍇ
				else if(this->counter > hige){
					this->counter =	hige +	hige - this->counter;
				}
			}
		}
		//�P��̂݃��[�h
		void cNormal::increasedCounter_Mode_Stop(float inAddValue)
		{
			this->counter += this->incValue * inAddValue;

		//�����l�E�I���l�̑召�֌W�ɂ��hige low������
			float hige, low;
			if(this->startValue < this->endValue){
				hige = this->endValue;
				low = this->startValue;
			}else{
				low = this->endValue;
				hige = this->startValue;
			}
		//�J�E���^���ŏ�����ő�͈̔͂Ɏ��߂�
			while(this->counter < low || this->counter > hige){
				//�ŏ��l�ȉ��̏ꍇ
				if(this->counter < low){
					this->counter = low;
				}
				//�ő�l�ȏ�̏ꍇ
				else if(this->counter > hige){
					this->counter = hige;
				}
			}
		}

		//�J�E���^�[��i�߂�
		void cNormal::increasedCounter(float inAddValue)
		{
			switch(this->mode){
			case Mode::eRepeatsA:	//�J��Ԃ�
				this->increasedCounter_Mode_RepeatsA(inAddValue);
				break;
			case Mode::eRepeatsB:	//�J��Ԃ�
				this->increasedCounter_Mode_RepeatsB(inAddValue);
				break;
			case Mode::eTourn:	//���A����
				this->increasedCounter_Mode_Tourn(inAddValue);
				break;
			case Mode::eStop:		//�P��ڂ̂ݗL��
				this->increasedCounter_Mode_Stop(inAddValue);
				break;
			default:
			case Mode::eNon:		//���g�p
				return;
				break;
			}
		}
		//���ݒl�𓾂�
		float cNormal::getCounter( )
		{
			if(this->mode == Mode::eNon){
				return 0.0f;
			}
			return this->counter;
		}
		double cNormal::getCounterD( )
		{
			if(this->mode == Mode::eNon){
				return 0.0;
			}
			return this->counter;
		}
		int cNormal::getCounterI( )
		{
			if(this->mode == Mode::eNon){
				return 0;
			}
			return (int)this->counter;
		}
		//���ݒl��ύX����i�J�E���^�[�͈̔͊O�̒l���w�肵���ꍇ�����j
		void cNormal::setCounter(float inCounter)
		{
			float min, max;
			if(this->startValue < this->endValue){
				min = this->startValue;
				max = this->endValue;
			}
			else{
				min = this->endValue;
				max = this->startValue;
			}
			if(inCounter < min || inCounter > max){
				return;
			}
			this->counter = inCounter;
		}

		//�J�E���^�[�̌��݂̏�Ԃ𓾂�
		Status::TYPENAME cNormal::getStatus( )
		{
			//
			if(this->mode == Mode::eNon){
				return Status::eErr;
			}
			//
			if(this->mode == Mode::eStop && this->counter == this->endValue){
				return Status::eEnd;
			}
			return Status::eNormal;
		}
		//�J�E���^�[��������Ԃɖ߂�
		void cNormal::resetCounter( )
		{
			if(this->mode == Mode::eNon){
				return;
			}
			this->counter = this->startValue;
		}
	}

	namespace _3D
	{
	//����������������������������������������������������������������������������������������������
	//��������																					�@��
	//����������������������������������������������������������������������������������������������
		cBox::cBox()
		{
			this->s = D3DXVECTOR3(0.0f,0.0f,0.0f);
			this->e = D3DXVECTOR3(0.0f,0.0f,0.0f);
		}
		cBox::cBox(const D3DXVECTOR3* s, const D3DXVECTOR3* e)
		{
			this->set(s, e);
		}
		void cBox::set(const D3DXVECTOR3* s, const D3DXVECTOR3* e)
		{
			this->s = *s;
			this->e = *e;
		}
		void cBox::transform(const D3DXVECTOR3* offset)
		{
			this->s += *offset;
			this->e += *offset;
		}
		BOOL cBox::intersect(const cBox* box)
		{
			//��������
			if(this->s.x == this->e.x){return FALSE;}
			if(this->s.y == this->e.y){return FALSE;}
			if(this->s.z == this->e.z){return FALSE;}
			if(box->s.x == box->e.x){return FALSE;}
			if(box->s.y == box->e.y){return FALSE;}
			if(box->s.z == box->e.z){return FALSE;}
			//�����蔻��
			if(	this->s.x	<	box->e.x	&&
				box->s.x	<	this->e.x	&&
				this->s.y	<	box->e.y	&&
				box->s.y	<	this->e.y	&&
				this->s.z	<	box->e.z	&&
				box->s.z	<	this->e.z	)
			{
				return TRUE;
			}
			return FALSE;
		}
		//�Q�̃{�b�N�X�̒��s����ʂ̐ڐG�𔻒肷��i�ڐG���s�q�t�d�j
		BOOL cBox::hitLine(const cBox* inRect)
		{
			//�{�b�N�X���ڐG���Ă����Ԃ��m�F
			if( this->intersect(inRect) )
			{
				//����̐ڐG�̗L�����m�F
				if(inRect->s.x < this->s.x && this->s.x < inRect->e.x){	
					return TRUE;
				}	//���g��s.x�����ʂɑ���̋�`����яo���Ă���
				if(inRect->s.x < this->e.x && this->e.x < inRect->e.x){
					return TRUE;
				}	//���g��e.x�����ʂɑ���̋�`����яo���Ă���

				if(inRect->s.y < this->s.y && this->s.y < inRect->e.y){
					return TRUE;
				}	//���g��s.y�����ʂɑ���̋�`����яo���Ă���
				if(inRect->s.y < this->e.y && this->e.y < inRect->e.y){
					return TRUE;
				}	//���g��e.y�����ʂɑ���̋�`����яo���Ă���

				if(inRect->s.z < this->s.z && this->s.z < inRect->e.z){
					return TRUE;
				}	//���g��s.y�����ʂɑ���̋�`����яo���Ă���
				if(inRect->s.z < this->e.z && this->e.z < inRect->e.z){
					return TRUE;
				}	//���g��e.y�����ʂɑ���̋�`����яo���Ă���
			}
			return FALSE;
		}


	//����������������������������������������������������������������������������������������������
	//��float�^��`																				�@��
	//����������������������������������������������������������������������������������������������
		//������������������������������������������������������������������������������������������
		//��float�^�ŋ�`��ݒ肷��																�@��
		//������������������������������������������������������������������������������������������
		void cFloatRect::set(	float inLeft,
								float inTop, 
								float inRight,
								float inBottom)
		{
			this->left	= inLeft;
			this->top	= inTop;
			this->right	= inRight;
			this->bottom = inBottom;
		}
		//������������������������������������������������������������������������������������������
		//����`�𕽍s�ړ�������																�@��
		//������������������������������������������������������������������������������������������
		void cFloatRect::transform(	float inX,
									float inY)
		{
			this->left	+= inX;
			this->top	+= inY;
			this->right	+= inX;
			this->bottom += inY;
		}
		//=======================================================================
		//�Q�̋�`�̐ڐG�𔻒肷��i�ڐG���s�q�t�d�j
		BOOL cFloatRect::hit(cFloatRect* inRect)
		{
			if(	this->left	< inRect->right	&&
				this->right	> inRect->left	&&
				this->top	< inRect->bottom &&
				this->bottom > inRect->top	){
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		//=======================================================================
		//�Q�̋�`�̐���̐ڐG�𔻒肷��i�ڐG���s�q�t�d�j
		BOOL cFloatRect::hitLine(cFloatRect* inRect)
		{
			//��`���ڐG���Ă����Ԃ��m�F
			if( this->hit(inRect) )
			{
				//����̐ڐG�̗L�����m�F
				if(inRect->left < this->left	&& this->left < inRect->right){	
					return TRUE;
				}	//���g��LeftTop-LeftBottom���C����ɑ����Left-Right���C�������d�Ȃ�
				if(inRect->left < this->right	&& this->right < inRect->right){
					return TRUE;
				}	//���g��RightTop-RightBottom���C����ɑ����Left-Right���C�������d�Ȃ�

				if(inRect->top < this->top		&& this->top < inRect->bottom){
					return TRUE;
				}	//���g��LeftTop-RightTop���C����ɑ����Top-bottom���C�������d�Ȃ�
				if(inRect->top < this->bottom	&& this->bottom < inRect->bottom){
					return TRUE;
				}	//���g��LeftBottom-RightBottom���C����ɑ����Top-bottom���C�������d�Ȃ�

			}
			return FALSE;
		}
	}
	namespace Color
	{
	//����������������������������������������������������������������������������������������������
	//���F�ݒ�⏕																				�@��
	//����������������������������������������������������������������������������������������������
		//������������������������������������������������������������������������������������������
		//��DWORD�^�ŐF��ݒ肷��B																�@��
		//���@2005/11/21	�쐬																�@��
		//������������������������������������������������������������������������������������������
		void cARGB::set(DWORD inColor)
		{
			this->color = *((sRGBA* )&inColor);
		}
		void cARGB::set(BYTE a, BYTE r, BYTE g, BYTE b)
		{
			this->color.a = a;
			this->color.r = r;
			this->color.g = g;
			this->color.b = b;
		}
		//������������������������������������������������������������������������������������������
		//��DWORD�^�ŐF��ݒ肷��B(�R���X�g���N�^)												�@��
		//���@2005/11/21	�쐬																�@��
		//������������������������������������������������������������������������������������������
		cARGB::cARGB(DWORD inColor)
		{
			this->color = *((sRGBA* )&inColor);
		}
		//������������������������������������������������������������������������������������������
		//���R���X�g���N�^�A�S�F���ꂼ����ʂɐF��ݒ�ł���A�f�t�H���g�͖���				�@��
		//���@2005/11/21	�쐬																�@��
		//������������������������������������������������������������������������������������������
		cARGB::cARGB(BYTE a, BYTE r, BYTE g, BYTE b)
		{
			this->color.a = a;
			this->color.r = r;
			this->color.g = g;
			this->color.b = b;
		}
		//������������������������������������������������������������������������������������������
		//��DWORD�^�ŐF��ԋp����B																�@��
		//���@�߂�l�FDWORD�^�F���																�@��
		//���@2005/07/28	�쐬																�@��
		//������������������������������������������������������������������������������������������
		DWORD cARGB::get( )
		{
			return *((DWORD* )&this->color);
		}
	}
	namespace Str
	{
		//��������m�d�v���ăR�s�[���쐬
		char* copyNew(char* inString)
		{
			char* temp;
			size_t len = strlen(inString);
			temp = new char[len + 1];
			strcpy(temp, inString);
			return temp;
		}
		LPSTR copyNew(LPCSTR inString)
		{
			char* temp;
			size_t len = strlen(inString);
			temp = new char[len + 1];
			strcpy(temp, inString);
			return temp;
		}
		//�]����Ƀ����������蓖�Ă��Ă���ꍇ�A�����strCopyNew���s��
		char* copyReleaseAndNew(char* *inSet, char* inString)
		{
			if(*inSet)
			{
				delete[] *inSet;
			}
			*inSet = copyNew(inString);
			return *inSet;
		}
		LPSTR copyReleaseAndNew(LPSTR* inSet, LPCSTR inString)
		{
			if(*inSet)
			{
				delete[] *inSet;
			}
			*inSet = copyNew(inString);
			return* inSet;
		}
	}

	namespace Memory
	{
	}
}//namespace GCom