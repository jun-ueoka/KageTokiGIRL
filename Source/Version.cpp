//--------------------------------------------------------------------------------------------
//	�t�@�C����	�F	version.h
//	�A�v����	�F	�ėp�I�ȃt�@�C���Ȃ̂ŁA���s�t�@�C���͕s����
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2007.02.09
//	�@�\		�F	VisualStudio2005��2003�̋��������邽�߂̃w�b�_�[�ł��B
//	�g�p���@	�F	VisualStudio2005�ȑO�̊J�������g�p����Ƃ��̓R�����g�A�E�g���܂��B
//	�X�V����	�F	2007.02.09	Ver1.00		�A�R����	�@�\�̎���
//--------------------------------------------------------------------------------------------
#include <windows.h>

#include	"Precompiler.h"

//#include	"Version.h"

//--------------------------------------------------------------------------------------------
//	�b��Łi�Ƃ肠����������R�s�[�j
//--------------------------------------------------------------------------------------------
void myCopyString(char* dest_ptr, int iDestSize, const char* source_ptr)
{
#ifdef VS2005 
	strcpy_s(dest_ptr, iDestSize, source_ptr);
#else
	strcpy(dest_ptr, source_ptr);
#endif
}

//--------------------------------------------------------------------------------------------
//	Scalar�^�Ŋm�ۂ��ꂽ�������̉��
//--------------------------------------------------------------------------------------------
void verDelete_Scalar(void** p)
{
	if(*p)
	{
		delete *p;
		*p = NULL;
	}
}

//--------------------------------------------------------------------------------------------
//	Vector�^�Ŋm�ۂ��ꂽ�������̉��
//--------------------------------------------------------------------------------------------
void verDelete_Vector(void** p)
{
	if(*p)
	{
		delete[] *p;
		*p = NULL;
	}
}

//--------------------------------------------------------------------------------------------
//	�g�p�����n���h������܂�	
//--------------------------------------------------------------------------------------------
extern void verCloseHandle(void** h)
{
	if(*h)
	{
		CloseHandle(*h);
		*h = NULL;
	}
}