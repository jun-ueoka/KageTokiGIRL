//***********************************************************************************
//!	�t�@�C����		Class_3DEffectCenter.cpp
//
//	�@�\			3D�Q�[����p�̃G�t�F�N�g�N���X���A
//					��_�ɓZ�߂ĕ`�悷��N���X�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/09/16	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_3DEffectCenter.h"		//!< �G�t�F�N�g�Z���^�[

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//	�C���X�^���X�����ꏊ
	//================================================================================
	C3D_EFFECT_CENTER*	C3D_EFFECT_CENTER::gUniqueInstance	= NULL;

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::�R���X�g���N�^
	//
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	C3D_EFFECT_CENTER::C3D_EFFECT_CENTER(void)	:
	effects()
	{
		initialize();
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::�f�X�g���N�^
	//
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	C3D_EFFECT_CENTER::~C3D_EFFECT_CENTER(void)
	{
		finishalize();
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::getInstance(�C���X�^���X���\�b�h)
	//
	//	�@�\		�C���X�^���X�̐���
	//	�߂�l		�C���X�^���X��Ԃ�
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	C3D_EFFECT_CENTER*
	C3D_EFFECT_CENTER::getInstance(void)
	{
	//!	�������Ă��Ȃ������ꍇ
		if(!gUniqueInstance)
		{
		//!	�C���X�^���X����
			gUniqueInstance = new C3D_EFFECT_CENTER();
		}
	//!	�C���X�^���X��Ԃ�
		return gUniqueInstance;
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::releaseInstance(�C���X�^���X���\�b�h)
	//
	//	�@�\		�C���X�^���X�̉��
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	void
	C3D_EFFECT_CENTER::releaseInstance(void)
	{
		SAFE_DELETE(gUniqueInstance);	//!< �C���X�^���X�̉��
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::initialize
	//
	//	�@�\		����������
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	void
	C3D_EFFECT_CENTER::initialize(void)
	{
	//	�������
		finishalize();
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::finishalize
	//
	//	�@�\		�������
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	void
	C3D_EFFECT_CENTER::finishalize(void)
	{
	//	�����G�t�F�N�g�O���[�v�̉��
		effects.releaseClear();
		effects2D.releaseClear();

	//	�G�t�F�N�g�}�b�v�̉��
 		SAFE_MAP_DELETE(effectMap, string, C3D_OBJECT*);
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::addEffectsInstance(�C���X�^���X���\�b�h)
	//
	//	�@�\		�C���X�^���X�ɃG�t�F�N�g�o�^
	//	����		inName			�o�^��
	//				inEffect		�o�^����G�t�F�N�g�I�u�W�F�N�g
	//	�߂�l		TRUE			�o�^����
	//				FALSE			�o�^���s
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	BOOL
	C3D_EFFECT_CENTER::addEffectsInstance(string inName, C3D_OBJECT* inEffect)
	{
	//	�C���X�^���X�����݂���Ȃ�A�G�t�F�N�g��o�^
		if(	gUniqueInstance)
		{
			return	gUniqueInstance->addEffects(inName, inEffect);
		}
	//	�C���X�^���X�����݂��Ȃ��Ȃ�A�G�t�F�N�g��j��
		else
		{
			SAFE_DELETE(inEffect);
		}
		return	FALSE;
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::occuEffectsInstance(�C���X�^���X���\�b�h)
	//
	//	�@�\		�C���X�^���X�ɃG�t�F�N�g����
	//	����		inName			����������G�t�F�N�g�̓o�^��
	//				inLoc			����������ꏊ
	//				inSize			�T�C�Y�w��(1.0f)
	//				inAngle			�����p�x
	//				inColor			�����J���[
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	void
	C3D_EFFECT_CENTER::occuEffectsInstance(	string		inName,
											VEC3		inLoc,
											float		inSize,
											VEC3		inAngle,
											D3DCOLOR	inColor)
	{
	//	�C���X�^���X�����݂���Ȃ�A�G�t�F�N�g�𔭐�
		if(	gUniqueInstance)
		{
			gUniqueInstance->occuEffects(inName, inLoc, inSize, inAngle, inColor);
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::occu2DEffectsInstance(�C���X�^���X���\�b�h)
	//
	//	�@�\		�C���X�^���X�ɃG�t�F�N�g����
	//	����		inName			����������G�t�F�N�g�̓o�^��
	//				inLoc			����������ꏊ
	//				inSize			�T�C�Y�w��(1.0f)
	//				inAngle			�����p�x
	//				inColor			�����J���[
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	void
	C3D_EFFECT_CENTER::occu2DEffectsInstance(	string		inName,
												VEC3		inLoc,
												float		inSize,
												VEC3		inAngle,
												D3DCOLOR	inColor)
	{
	//	�C���X�^���X�����݂���Ȃ�A�G�t�F�N�g�𔭐�
		if(	gUniqueInstance)
		{
			gUniqueInstance->occu2DEffects(inName, inLoc, inSize, inAngle, inColor);
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::releaseEffectInstance(�C���X�^���X���\�b�h)
	//
	//	�@�\		�C���X�^���X�ɃG�t�F�N�g���
	//	����		inName			���������G�t�F�N�g�̓o�^��
	//	�߂�l		TRUE			�o�^����
	//				FALSE			�o�^���s
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	void
	C3D_EFFECT_CENTER::releaseEffectInstance(string inName)
	{
	//	�C���X�^���X�����݂���Ȃ�A�G�t�F�N�g��o�^
		if(	gUniqueInstance)
		{
			return	gUniqueInstance->releaseEffect(inName);
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::addEffects
	//
	//	�@�\		�G�t�F�N�g�̓o�^
	//	����		inName			�o�^��
	//				inEffect		�o�^����G�t�F�N�g�I�u�W�F�N�g
	//	�߂�l		TRUE			�o�^����
	//				FALSE			�o�^���s
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	BOOL
	C3D_EFFECT_CENTER::addEffects(string inName, C3D_OBJECT* inEffect)
	{
	//	�����G�t�F�N�g�����݂�����A���������Ȃ�
		if(	effectMap.find(inName) != effectMap.end())
		{
			SAFE_DELETE(inEffect);
			return	FALSE;
		}

	//	�����G�t�F�N�g�����݂��Ȃ��Ȃ�A�o�^����
		effectMap.insert(pair<string, C3D_OBJECT*>(inName, inEffect));

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::occuEffects
	//
	//	�@�\		�G�t�F�N�g�̔���(3D)
	//	����		inName			����������G�t�F�N�g�̓o�^��
	//				inLoc			����������ꏊ
	//				inSize			�T�C�Y�w��(1.0f)
	//				inAngle			�����p�x
	//				inColor			�����J���[
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	void
	C3D_EFFECT_CENTER::occuEffects(	string		inName,
									VEC3		inLoc,
									float		inSize,
									VEC3		inAngle,
									D3DCOLOR	inColor)
	{
	//	�o�^������A����������G�t�F�N�g������
		map<string, C3D_OBJECT*>::iterator	mp;
		mp	= effectMap.find(inName);

	//	����������G�t�F�N�g�����݂���Ȃ�A����
		if(	mp != effectMap.end())
		{
			C3D_OBJECT*	inCopy	= NULL;
			mp->second->createCopy(&inCopy, &inLoc, &inAngle);
		//	�ݒ�
			inCopy->setScale(inSize);
			inCopy->setColor(inColor);
			effects.add(inCopy);
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::occu2DEffects
	//
	//	�@�\		�G�t�F�N�g�̔���(2D)
	//	����		inName			����������G�t�F�N�g�̓o�^��
	//				inLoc			����������ꏊ
	//				inSize			�T�C�Y�w��(1.0f)
	//				inAngle			�����p�x
	//				inColor			�����J���[
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	void
	C3D_EFFECT_CENTER::occu2DEffects(	string		inName,
										VEC3		inLoc,
										float		inSize,
										VEC3		inAngle,
										D3DCOLOR	inColor)
	{
	//	�o�^������A����������G�t�F�N�g������
		map<string, C3D_OBJECT*>::iterator	mp;
		mp	= effectMap.find(inName);

	//	����������G�t�F�N�g�����݂���Ȃ�A����
		if(	mp != effectMap.end())
		{
			C3D_OBJECT*	inCopy	= NULL;
			mp->second->createCopy(&inCopy, &inLoc, &inAngle);
		//	�ݒ�
			inCopy->setScale(inSize);
			inCopy->setColor(inColor);
			effects2D.add(inCopy);
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::releaseEffect
	//
	//	�@�\		�G�t�F�N�g�̉��
	//	����		inName			���������G�t�F�N�g�̓o�^��
	//	�X�V		2008/09/24		<�V�K>
	//================================================================================
	void
	C3D_EFFECT_CENTER::releaseEffect(string inName)
	{
	//	�o�^������A���������G�t�F�N�g������
		map<string, C3D_OBJECT*>::iterator	mp;
		mp	= effectMap.find(inName);

	//	���������G�t�F�N�g�����݂���Ȃ�A���
		if(	mp != effectMap.end())
		{
			C3D_OBJECT*	inCopy	= mp->second;
			SAFE_DELETE(inCopy);
			effectMap.erase(mp);
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::drawEffects
	//
	//	�@�\		�S�G�t�F�N�g�`��
	//	����		inDev			Direct3D�f�o�C�X
	//				inCamera		����������ꏊ
	//	�X�V		2008/09/16		<�V�K>
	//================================================================================
	void
	C3D_EFFECT_CENTER::drawEffects(CONST DEV inDev, C3D_OBJECT* inCamera)
	{
	//	�p�x��␳���A���בւ����āA�`��
		effects.sortByCameraPosition(inCamera, TRUE);

		C3D_OBJECT*						releaseObj;	//!< ����p
		vector<C3D_OBJECT*>::iterator	vp;			//!< ��Ɨp�C�e���[�^

	//	�������Ȃ���`�悷�邽�߁A�Ƃ肾���ĕ`��
		vp	= effects.getBegin();
		while(vp != effects.getEnd())
		{
		//	�G�t�F�N�g��`�悵�A�`����I��
			if(	!(*vp)->draw(inDev))
			{
				releaseObj	= (*vp);
				vp			= effects.erase(vp);
				SAFE_DELETE(releaseObj);
			}
		//	�`����I�����Ă��Ȃ��Ȃ�A�C�e���[�^��i�߂�
			else
			{
				vp++;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_EFFECT_CENTER::drawEffects2D
	//
	//	�@�\		�S�G�t�F�N�g�`��
	//	����		inSprite		�X�v���C�g�f�o�C�X
	//	�X�V		2008/09/27		<�V�K>
	//================================================================================
	void
	C3D_EFFECT_CENTER::drawEffects2D(CONST SPRITE inSprite)
	{
		C3D_OBJECT*						releaseObj;	//!< ����p
		vector<C3D_OBJECT*>::iterator	vp;			//!< ��Ɨp�C�e���[�^

	//	�������Ȃ���`�悷�邽�߁A�Ƃ肾���ĕ`��
		vp	= effects2D.getBegin();
		while(vp != effects2D.getEnd())
		{
		//	�G�t�F�N�g��`�悵�A�`����I��
			if(	!(*vp)->draw(inSprite))
			{
				releaseObj	= (*vp);
				vp			= effects2D.erase(vp);
				SAFE_DELETE(releaseObj);
			}
		//	�`����I�����Ă��Ȃ��Ȃ�A�C�e���[�^��i�߂�
			else
			{
				vp++;
			}
		}
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------