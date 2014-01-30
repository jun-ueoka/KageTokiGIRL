//--------------------------------------------------------------------------------------------
//	�t�@�C����	�F	version.h
//	�A�v����	�F	�ėp�I�ȃt�@�C���Ȃ̂ŁA���s�t�@�C���͕s����
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2007.02.09
//	�@�\		�F	VisualStudio2005��2003�̋��������邽�߂̃w�b�_�[�ł��B
//	�g�p���@	�F	VisualStudio2005�ȑO�̊J�������g�p����Ƃ��̓R�����g�A�E�g���܂��B
//	�X�V����	�F	2007.02.09	Ver1.00		�A�R����	�@�\�̎���
//--------------------------------------------------------------------------------------------

#pragma once

//	VisualStudio2005�ȊO�ŃR���p�C������ꍇ�A
//	VS2005�̒�`�̓R�����g�A�E�g���Ă���
#define VS2005

//	�����A�uVS2005�v����`����Ă�����ȉ��̐ݒ���s��
#ifdef VS2005
//	�ȉ��̌x���𖳌��ɂ��܂�

//�u�t�@�C���́A���݂̃R�[�h �y�[�W (932) �ŕ\���ł��Ȃ��������܂�ł��܂��B
//�@�f�[�^�̑�����h�����߂ɁA�t�@�C���� Unicode �`���ŕۑ����Ă��������B�v
#pragma warning (disable: 4819)
//�u�����͊֐��̖{�̕��� 1 �x���Q�Ƃ���܂���v
#pragma warning (disable: 4100)
//�uThis function or variable may be unsafe.�v
#pragma warning (disable: 4996)

#pragma warning	(disable: 4786)

#pragma warning	(disable: 4267)

#endif

//	�Q�ƃJ�E���^�����炷�}�N��
#ifndef kREL
#define kREL(x) if(x){x->Release();x=NULL;}	//	x�̃I�u�W�F�N�g��NULL�ł͖����ꍇ�́Ax���������B
#endif

//#define	PRESENTATION

//	�R�s�[
void myCopyString(char* dest_ptr, int iDestSize, const char* source_ptr);	//	������R�s�[
//	����������֘A
extern void verDelete_Scalar(void** p);	//	�P�̂̉��
extern void verDelete_Vector(void** p);	//	�z��̉��
//�@�n���h����������܂�
extern void verCloseHandle(void** h);	//	�n���h�������