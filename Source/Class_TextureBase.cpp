//***********************************************************************************
//!	�t�@�C����		Class_TextureBase.cpp
//
//	�@�\			�e�N�X�`���x�[�X�Ɋւ������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/08/30	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_TextureBase.h"

//-----------------------------------------------------------------------
//!	�e�N�X�`���x�[�X�̖��O���
//-----------------------------------------------------------------------
namespace TB
{
	//================================================================================
	//	���L�e�N�X�`���}�b�v
	//================================================================================
	map<string, TEXTURE_DATA*>*		CTEXTURE_BASE::mTextures = NULL;

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::createTextureInData
	//
	//	�@�\		�e�N�X�`���t�@�C�������[�h����B
	//	�@�\�T�v	�e�N�X�`���t�@�C�������[�h���A���ʂ�Ԃ�
	//				���̃��\�b�h�̓N���X���\�b�h�ł���B
	//	����		inDev			Direct3D�f�o�C�X
	//				inTexData		���[�h��e�N�X�`���f�[�^
	//				inName			X�t�@�C���̖���
	//				dwColor			����������F
	//				inResource		�e�N�X�`���t�H���_��
	//	�߂�l		TRUE			��������
	//				FALSE			�������s
	//	�X�V		2009/03/03		<�V�K>
	//================================================================================
	BOOL
	CTEXTURE_BASE::createTextureInData(	CONST DEV		inDev,
										TEXTURE_DATA*	inTexData,
										CONST LPSTR		inName,
										DWORD			inColor,
										CONST LPSTR		inResource)
	{
		TCHAR			realName[MAX_PATH]	= "";					//!< ���O�Ƀp�X�������

	//	�ǂݍ���ł݂�
		if(	FAILED(D3DX_CREATE_TEXTURE_EX(
				inDev,
				inName,
				inColor,
				&inTexData->texInfo,
				&inTexData->texture)))
		{
		//	���s������A���\�[�X�t�@�C���ւ̃p�X�����ăg���C
			sprintf_s(realName, sizeof(realName), inResource, inName);

			if(	FAILED(D3DX_CREATE_TEXTURE_EX(
					inDev,
					realName,
					inColor,
					&inTexData->texInfo,
					&inTexData->texture)))
			{
			//	���s������ANULL��Ԃ�
				goto error;
			}
		}

	//	���b�V���������b�V�����ɓo�^
		inTexData->name		= inName;
		inTexData->rPath	= inResource;
	//	�J���[�����m��
		inTexData->color	= inColor;
	//	�f�o�C�X���m��
		inTexData->device	= inDev;

	//	�쐬�����e�N�X�`���̔z��ԍ���Ԃ�
		return TRUE;

	//	NULL��Ԃ�����
	error:
		inTexData->releaseData();	//!< �����������b�V�������J��
		DX::errorMsgNoFile(inDev, realName);
		return	FALSE;				//!< NULL��Ԃ�
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::createTexture
	//
	//	�@�\		�X�L�����b�V�����𐶐����A���������f�[�^��Ԃ�
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			X�t�@�C���̖���
	//				dwColor			����������F
	//				inResource		�e�N�X�`���t�H���_��
	//	�߂�l		�e�N�X�`�����̃|�C���^�[
	//	�X�V		2008/08/27		<�V�K>
	//================================================================================
	TEXTURE_DATA*
	CTEXTURE_BASE::createTexture(	CONST DEV	inDev,
									CONST LPSTR	inName,
									DWORD		inColor,
									CONST LPSTR	inResource)
	{
		TEXTURE_DATA*	newTex	= new TEXTURE_DATA();	//!< �V���ȃe�N�X�`�����

	//	�e�N�X�`���𐶐����A���s������NULL��Ԃ�
		if(	!createTextureInData(inDev, newTex, inName, inColor, inResource))
		{
			SAFE_DELETE(newTex);
			return	NULL;
		}
		return	newTex;
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::initializeVertex
	//
	//	�@�\		���_�o�b�t�@�𐶐�����
	//	����		inDev		Direct3D�f�o�C�X
	//	�X�V		2008/08/28	<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::initializeVertex(CONST DEV inDev)
	{
	//	�����̏�����
		SAFE_RELEASE(vb);		

	//	���_�o�b�t�@�̐���
		HRESULT	hr;
		hr = inDev->CreateVertexBuffer(
				NUM_TEXTURE_VERTEX * sizeof(CUSTOM_VERTEX),
				0/*D3DUSAGE_WRITEONLY*/,
				D3DFVF_VERTEX,
				D3DPOOL_MANAGED,
				&vb,
				NULL);
	//	���_�o�b�t�@�̐����Ɏ��s�����Ȃ�A��O�𔭐�
		if(	FAILED(hr))	{	throw	hr;	}

	//	�R�[�i�[�̍X�V
		updateCorner();

	//	UV���W�̍X�V
		updateUVCoord();

	//	�J���[�̍X�V
		updateColor();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::updateCorner
	//
	//	�@�\		�R�[�i�[�̍X�V
	//	�X�V		2008/08/28	<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::updateCorner(void)
	{
		VEC2	h		= VEC2(0.0f, 0.0f);
		VEC2	v		= VEC2(0.0f, 0.0f);

	//	���_�̈ʒu������
		switch(vertexPosition)
		{
			case	eVERP_LEFT_UP:
				h	+=	VEC2(-(             0.0f),  (showSize.x       ));
				v	+=	VEC2( (             0.0f), -(showSize.y       ));
				break;
			case	eVERP_UP:
				h	+=	VEC2(-(showSize.x / 2.0f),  (showSize.x / 2.0f));
				v	+=	VEC2( (             0.0f), -(showSize.y       ));
				break;
			case	eVERP_RIGHT_UP:
				h	+=	VEC2(-(showSize.x       ),  (             0.0f));
				v	+=	VEC2( (             0.0f), -(showSize.y       ));
				break;
			case	eVERP_RIGHT:
				h	+=	VEC2(-(showSize.x       ),  (             0.0f));
				v	+=	VEC2( (showSize.y / 2.0f), -(showSize.y / 2.0f));
				break;
			case	eVERP_RIGHT_DOWN:
				h	+=	VEC2(-(showSize.x       ),  (             0.0f));
				v	+=	VEC2( (showSize.y       ), -(             0.0f));
				break;
			case	eVERP_DOWN:
				h	+=	VEC2(-(showSize.x / 2.0f),  (showSize.x / 2.0f));
				v	+=	VEC2( (showSize.y       ), -(             0.0f));
				break;
			case	eVERP_LEFT_DOWN:
				h	+=	VEC2(-(             0.0f),  (showSize.x       ));
				v	+=	VEC2( (showSize.y       ), -(             0.0f));
				break;
			case	eVERP_LEFT:
				h	+=	VEC2(-(             0.0f),  (showSize.x       ));
				v	+=	VEC2( (showSize.y / 2.0f), -(showSize.y / 2.0f));
				break;
			case	eVERP_CENTER:
				h	+=	VEC2(-(showSize.x / 2.0f),  (showSize.x / 2.0f));
				v	+=	VEC2( (showSize.y / 2.0f), -(showSize.y / 2.0f));
				break;
		}

		corner[0]	= VEC3(h.x, v.x, 0.0f);	//!<����
		corner[1]	= VEC3(h.y, v.x, 0.0f);	//!<�E��
		corner[2]	= VEC3(h.y, v.y, 0.0f);	//!<�E��
		corner[3]	= VEC3(h.x, v.y, 0.0f);	//!<����

		HRESULT	hr;
	//	���_���i�e�N�X�`�����W�j�̍X�V
		CUSTOM_VERTEX *pV;
		hr = vb->Lock(0, 0, (void**)&pV, 0);
		if(	FAILED(hr))
		{
		//	���s�Ȃ�A��O�𔭐�
			throw	hr;
		}

		pV[0].pos		= corner[0];
		pV[1].pos		= corner[1];
		pV[2].pos		= corner[2];
		pV[3].pos		= corner[3];

		VEC3	min = pV[0].pos;
		VEC3	max = pV[0].pos;
		for(int i = 0; i < NUM_TEXTURE_VERTEX; i++)
		{
			min.x	= min(pV[i].pos.x, min.x);
			min.y	= min(pV[i].pos.y, min.y);
			min.z	= min(pV[i].pos.z, min.z);
			max.x	= max(pV[i].pos.x, max.x);
			max.y	= max(pV[i].pos.y, max.y);
			max.z	= max(pV[i].pos.z, max.z);
		}

		vb->Unlock();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::updateUVCoord
	//
	//	�@�\		UV���W�̍X�V
	//	�X�V		2008/08/28	<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::updateUVCoord(void)
	{
		HRESULT	hr;
	//	���_���i�e�N�X�`�����W�j�̍X�V
		CUSTOM_VERTEX *pV;
		hr = vb->Lock(0, 0, (void**)&pV, 0);
		if(	FAILED(hr))
		{
		//	���s�Ȃ�A��O�𔭐�
			throw	hr;
		}
		pV[0].uv		= VEC2(uCoord.x, vCoord.x);
		pV[1].uv		= VEC2(uCoord.y, vCoord.x);
		pV[2].uv		= VEC2(uCoord.y, vCoord.y);
		pV[3].uv		= VEC2(uCoord.x, vCoord.y);
		vb->Unlock();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::updateColor
	//
	//	�@�\		�J���[�̍X�V
	//	�X�V		2008/09/12	<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::updateColor(void)
	{
	//	�J���[�l�̕␳
		color.lock();

		HRESULT	hr;
	//	���_���i�e�N�X�`�����W�j�̍X�V
		CUSTOM_VERTEX *pV;
		hr = vb->Lock(0, 0, (void**)&pV, 0);
		if(	FAILED(hr))
		{
		//	���s�Ȃ�A��O�𔭐�
			throw	hr;
		}
	//	�S�Ă̒��_�o�b�t�@�̓����x��ݒ�
		pV[0].color =
		pV[1].color =
		pV[2].color =
		pV[3].color = color.color;
		vb->Unlock();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::startTextureInitialize
	//
	//	�@�\		�����̏�����
	//	�X�V		2008/08/30		<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::startTextureInitialize(void)
	{
		vb				= NULL;					//!< �o�[�e�b�N�X�o�b�t�@
		showSize		= VEC2(0.0f, 0.0f);		//!< �\���T�C�Y
		texSize			= VEC2(0.0f, 0.0f);		//!< �e�N�X�`���T�C�Y
		uCoord			= VEC2(0.00001f, 1.0f);	//!< UV���W(�E��)
		vCoord			= VEC2(0.00001f, 1.0f);	//!< UV���W(����)
		color			= MY_COLOR_RGBA();		//!< �J���[
		vertexPosition	= eVERP_CENTER;			//!< ���_�̈ʒu

		texd_ptr		= NULL;					//!< ���b�V�����|�C���^�[
		bTextureLoaded	= FALSE;				//!< ���b�V���̃��[�h�m�F
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::initializeTexture
	//
	//	�@�\		�e�N�X�`���̏�����
	//	����		inDev		Direct3D�f�o�C�X
	//				inName		X�t�@�C���̖���
	//				inSize		�T�C�Y
	//				inColor		�����������F
	//				inResource	�e�N�X�`���t�H���_��
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::initializeTexture(	CONST DEV	inDev,
										CONST LPSTR	inName,
										VEC2		inSize,
										DWORD		inColor,
										CONST LPSTR	inResource)
	{
	//	�e�N�X�`���̏������
		finishalizeTexture();

	//	�e�N�X�`���}�b�v��������ΐ�������
		if(	!mTextures)
		{
			mTextures = new map<string, TEXTURE_DATA*>;
		}

	//	�����̏�����
		showSize			= inSize;
	//	���_�o�b�t�@�𐶐�
		initializeVertex(inDev);

	//	��Ɨp�C�e���[�^
		map<string, TEXTURE_DATA*>::iterator	p;
	//	�e�N�X�`���}�b�v�Ƀe�N�X�`���t�@�C������n���A���̃C�e���[�^���󂯎��
		p = mTextures->find(inName);

	//	�e�N�X�`���}�b�v�ɓ������O�̃e�N�X�`���t�@�C���������ɓo�^���Ă���ꍇ
		if(	p != mTextures->end())
		{
		//	�g�p�J�E���^�𑝂₷
			p->second->cnt++;
		//	�e�N�X�`������Ԃ�
			texd_ptr			= (*p).second;
		//	���[�h����
			bTextureLoaded	= TRUE;
		}
		else
		{
		//	�V���Ƀe�N�X�`���𐶐�����
			texd_ptr	= createTexture(inDev, inName, inColor, inResource);

		//	�����ɐ��������ꍇ
			if(	texd_ptr)
			{
			//	�g�p�J�E���^�𑝂₷
				texd_ptr->cnt++;
			//	���[�h����
				bTextureLoaded	= TRUE;
			//	�e�N�X�`���}�b�v�ɓo�^
				mTextures->insert(pair<string, TEXTURE_DATA*>(texd_ptr->name, texd_ptr));
			}
		//	���s�����ꍇ
			else
			{
			//	���[�h������
				bTextureLoaded	= FALSE;
			}
		}

	//	�X�L�����b�V����񂪂��鎞
		if(	bTextureLoaded)
		{
		//	�e�N�X�`���T�C�Y��ۑ�
			texSize.x	= (float)texd_ptr->texInfo.Width;
			texSize.y	= (float)texd_ptr->texInfo.Height;
		}
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::finishalizeTexture
	//
	//	�@�\		�e�N�X�`���̉��
	//	�X�V		2007/12/08		<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::finishalizeTexture(void)
	{
	//	�o�[�e�b�N�X�o�b�t�@�̉��
		SAFE_RELEASE(vb);

	//	�e�N�X�`���������[�h���ĂȂ��Ȃ�A������Ԃ�
		if(	!bTextureLoaded)		return;

	//	�t���O�����ǂ��Ă���
		bTextureLoaded	= FALSE;

	//	�e�N�X�`���J�E���g�����炷
		texd_ptr->cnt--;
	//	���̃e�N�X�`�����܂��g�p����Ă���ꍇ�͉�����Ȃ�
		if(	texd_ptr->cnt > 0)	return;

	//	�e�N�X�`���}�b�v�C�e���[�^
		map<string, TEXTURE_DATA*>::iterator	p;
	//	�e�N�X�`���}�b�v��X�t�@�C������n���A���̃C�e���[�^���󂯎��
		p = mTextures->find(texd_ptr->name);
	//	�e�N�X�`�����̉��
		SAFE_DELETE(p->second);
	//	�e�N�X�`���}�b�v����폜
		mTextures->erase(p);

	//	�e�N�X�`���}�b�v�Ƀ����o�[��������΁A��������
		if(	mTextures->begin() == mTextures->end())
		{
			SAFE_DELETE(mTextures);
		}
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::�R���X�g���N�^
	//
	//	�@�\		�e�N�X�`�������[�h���ď�����
	//	����		inDev		Direct3D�f�o�C�X
	//				inName		X�t�@�C���̖���
	//				inSize		�T�C�Y
	//				inColor		�����������F
	//				inResource	�e�N�X�`���t�H���_��
	//	�X�V		2008/08/28	<�V�K>
	//================================================================================
	CTEXTURE_BASE::CTEXTURE_BASE(	CONST DEV	inDev,
									CONST LPSTR	inName,
									VEC2		inSize,
									DWORD		inColor,
									CONST LPSTR	inResource)
	{
	//	�����̏�����
		startTextureInitialize();
	//	�e�N�X�`���̏�����
		initializeTexture(inDev, inName, inSize, inColor, inResource);
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::�R���X�g���N�^
	//
	//	�@�\		�󏉊���
	//	�X�V		2008/08/28	<�V�K>
	//================================================================================
	CTEXTURE_BASE::CTEXTURE_BASE(void)
	{
	//	�����̏�����
		startTextureInitialize();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::�R���X�g���N�^
	//
	//	�@�\		�R�s�[
	//	����		inCopy		�R�s�[��
	//	�X�V		2008/08/28	<�V�K>
	//================================================================================
	CTEXTURE_BASE::CTEXTURE_BASE(CTEXTURE_BASE* inCopy)
	{
	//	�����̏�����
		startTextureInitialize();
	//	�R�s�[�J�n
		texd_ptr			= inCopy->getTextures();

	//	�e�N�X�`����񂪂��鎞
		if(	texd_ptr)
		{
		//	�c����R�s�[
			showSize		= inCopy->getShowSize();
			vertexPosition	= inCopy->getVertexPosition();

		//	���_�o�b�t�@�𐶐�
			initializeVertex(texd_ptr->device);
			setTextureCoord(inCopy->getUCoord(), inCopy->getVCoord());
			setColor(inCopy->getColorRgba());

		//	�g�p�J�E���^�𑝂₷
			texd_ptr->cnt++;
		//	���[�h����
			bTextureLoaded	= TRUE;
		//	�e�N�X�`���T�C�Y��ۑ�
			texSize.x	= (float)texd_ptr->texInfo.Width;
			texSize.y	= (float)texd_ptr->texInfo.Height;
		}
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::�f�X�g���N�^
	//
	//	�X�V		2008/05/03	<�V�K>
	//================================================================================
	CTEXTURE_BASE::~CTEXTURE_BASE(void)
	{
	//	�e�N�X�`���̉��
		finishalizeTexture();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::resetInstance
	//
	//	�@�\		���Z�b�g���s
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2009/03/02		<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::resetInstance(CONST DEV inDev)
	{
	//	�e�N�X�`���}�b�v���Ȃ���΁A���Z�b�g���Ȃ�
		if(	!mTextures)	return;

	//	�ǂݍ��񂾎��̏������o����
		char	inName[MAX_PATH]	= "";
		char	inRPath[MAX_PATH]	= "";

	//	����������
		MAP_LOOP(string, TEXTURE_DATA*, (*mTextures))
		{
		//	���̃f�[�^��ێ�����
			sprintf_s(inName,   sizeof(inName),  (*p).second->name.c_str());
			sprintf_s(inRPath,  sizeof(inRPath), (*p).second->rPath.c_str());
		//	���̃f�[�^�œǂ݂��݂�����
			createTextureInData(inDev, (*p).second, inName, (*p).second->color, inRPath);
		}
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::setTextureCooard
	//
	//	�@�\		UV���W�̐ݒ�(UV��)
	//	����		u			U�A�����b�v
	//				v			V�A�����b�v
	//	�X�V		2008/05/05	<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::setTextureCoord(VEC2 u, VEC2 v)
	{
	//	UV�͈͂𐶐�
		uCoord.x = u.x;
		uCoord.y = u.y;
		vCoord.x = v.x;
		vCoord.y = v.y;

	//	UV���W�̍X�V
		updateUVCoord();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::setTextureCooard
	//
	//	�@�\		UV���W�̐ݒ�(���l��)
	//	����		inLeft		���[
	//				inTop		��[
	//				inRight		�E�[
	//				inBottom	���[
	//	�X�V		2008/05/05	<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::setTextureCoord(int inLeft, int inTop, int inRight, int inBottom)
	{
	//	UV�͈͂𐶐�
		uCoord.x = (float)inLeft   / texSize.x;
		uCoord.y = (float)inTop    / texSize.y;
		vCoord.x = (float)inRight  / texSize.x;
		vCoord.y = (float)inBottom / texSize.y;

	//	UV���W�̍X�V
		updateUVCoord();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::setTextureCooard
	//
	//	�@�\		UV���W�̐ݒ�(��`��)
	//	����		inSrc		�`�挳��`
	//	�X�V		2008/05/05	<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::setTextureCoord(RECT *inSrc)
	{
	//	UV�͈͂𐶐�
		uCoord.x = (float)inSrc->left   / texSize.x;
		uCoord.y = (float)inSrc->top    / texSize.y;
		vCoord.x = (float)inSrc->right  / texSize.x;
		vCoord.y = (float)inSrc->bottom / texSize.y;

	//	UV���W�̍X�V
		updateUVCoord();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::setColor
	//
	//	�@�\		�J���[�̐ݒ�
	//	����		inRed		�Ԑ���
	//				inGreen		�ΐ���
	//				inBlue		����
	//				inAlpha		�����x
	//	�X�V		2008/09/20	<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::setColor(int inRed, int inGreen, int inBlue, int inAlpha)
	{
	//	RGBA�J���[�̐ݒ�
		color.rgba.red		= (BYTE)inRed;
		color.rgba.green	= (BYTE)inGreen;
		color.rgba.blue		= (BYTE)inBlue;
		color.rgba.alpha	= (BYTE)inAlpha;

	//	�J���[�̍X�V
		updateColor();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::calcColorRed
	//
	//	�@�\		RED�␳�̌v�Z
	//	����		inRed		RED�␳
	//	�X�V		2009/01/24	<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::calcColorRed(int inRed)
	{
	//	RGBA�J���[�̌v�Z
		inRed	=	(int)color.rgba.red		+ inRed;
		if(	inRed   < 0){ inRed   = 0;	}	else if(inRed   > 255){ inRed   = 255;	}
		color.rgba.red	= (BYTE)inRed;

	//	�J���[�̍X�V
		updateColor();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::calcColorGreen
	//
	//	�@�\		GREEN�␳�̌v�Z
	//	����		inGreen		GREEN�␳
	//	�X�V		2009/01/24	<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::calcColorGreen(int inGreen)
	{
	//	RGBA�J���[�̌v�Z
		inGreen	=	(int)color.rgba.green	+ inGreen;
		if(	inGreen < 0){ inGreen = 0;	}	else if(inGreen > 255){ inGreen = 255;	}
		color.rgba.green	= (BYTE)inGreen;

	//	�J���[�̍X�V
		updateColor();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::calcColorBlue
	//
	//	�@�\		BLUE�␳�̌v�Z
	//	����		inBlue		BLUE�␳
	//	�X�V		2009/01/24	<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::calcColorBlue(int inBlue)
	{
	//	RGBA�J���[�̌v�Z
		inBlue	=	(int)color.rgba.blue	+ inBlue;
		if(	inBlue  < 0){ inBlue  = 0;	}	else if(inBlue  > 255){ inBlue  = 255;	}
		color.rgba.blue		= (BYTE)inBlue;

	//	�J���[�̍X�V
		updateColor();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::calcColorAlpha
	//
	//	�@�\		ALPHA�␳�̌v�Z
	//	����		inAlpha		ALPHA�␳
	//	�X�V		2009/01/24	<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::calcColorAlpha(int inAlpha)
	{
	//	RGBA�J���[�̌v�Z
		inAlpha	=	(int)color.rgba.alpha	+ inAlpha;
		if(	inAlpha < 0){ inAlpha = 0;	}	else if(inAlpha > 255){ inAlpha = 255;	}
		color.rgba.alpha	= (BYTE)inAlpha;

	//	�J���[�̍X�V
		updateColor();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::calcColor
	//
	//	�@�\		�J���[�̌v�Z(���Z�̂�)
	//	����		inRgba		RGBA�J���[
	//	�X�V		2008/09/20	<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::calcColor(RGBA inRgba)
	{
	//	RGBA�J���[�̌v�Z
		color.rgba.red	   += inRgba.red;
		color.rgba.green   += inRgba.green;
		color.rgba.blue	   += inRgba.blue;
		color.rgba.alpha   += inRgba.alpha;

	//	�J���[�̍X�V
		updateColor();
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::calcColor
	//
	//	�@�\		�J���[�̌v�Z(���Z���Z)
	//	����		inRed		�Ԑ���
	//				inGreen		�ΐ���
	//				inBlue		����
	//				inAlpha		�����x
	//	�X�V		2008/09/20	<�V�K>
	//================================================================================
	void
	CTEXTURE_BASE::calcColor(int inRed, int inGreen, int inBlue, int inAlpha)
	{
	//	RGBA�J���[�̉����Z
		inRed	=	(int)color.rgba.red		+ inRed;
		inGreen	=	(int)color.rgba.green	+ inGreen;
		inBlue	=	(int)color.rgba.blue	+ inBlue;
		inAlpha	=	(int)color.rgba.alpha	+ inAlpha;
	//	�͈͐ݒ�
		if(	inRed   < 0){ inRed   = 0;	}	else if(inRed   > 255){ inRed   = 255;	}
		if(	inGreen < 0){ inGreen = 0;	}	else if(inGreen > 255){ inGreen = 255;	}
		if(	inBlue  < 0){ inBlue  = 0;	}	else if(inBlue  > 255){ inBlue  = 255;	}
		if(	inAlpha < 0){ inAlpha = 0;	}	else if(inAlpha > 255){ inAlpha = 255;	}
	//	���߂Đݒ�
		setColor(inRed,inGreen,inBlue,inAlpha);
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::textureInvalid
	//
	//	�@�\		�e�N�X�`���̖�����������
	//	�߂�l		TRUE		����
	//				FALSE		�L��
	//	�X�V		2008/05/03	<�V�K>
	//================================================================================
	BOOL
	CTEXTURE_BASE::textureInvalid(void)
	{
		if(	(!bTextureLoaded)		||	//!< �ǂݍ��܂�Ă��Ȃ���
			(!texd_ptr->texture))		//!< �e�N�X�`������������Ă��Ȃ���
		{
			return TRUE;					//!< ����
		}
		else								//!< ��L�ɓ��Ă͂܂�Ȃ��Ȃ�
		{
			return FALSE;					//!< �L��
		}
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::drawTexture
	//
	//	�@�\		�`��(3D�p)
	//	����		inDev		Direct3D�f�o�C�X
	//				inMatrix	���[���h�}�g���b�N�X
	//	�X�V		2008/05/03	<�V�K>
	//================================================================================
	BOOL
	CTEXTURE_BASE::drawTexture(CONST DEV inDev, MAT16* inMatrix)
	{
	//	�`��̖�����������Ȃ�A�`�悵�Ȃ�
		if(	textureInvalid())	return FALSE;

		inDev->SetTransform(D3DTS_WORLD, inMatrix);

	//	���C�e�B���O�𖳌���
		inDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		inDev->SetTexture(0, texd_ptr->texture);

	//	�`��
		inDev->SetFVF(D3DFVF_VERTEX);
		inDev->SetStreamSource(0, vb, 0, sizeof(CUSTOM_VERTEX));
		inDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		inDev->SetTexture(0, NULL);

	//	���C�e�B���O���[�h�ɕ��A
		inDev->SetRenderState(D3DRS_LIGHTING, TRUE);

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CTEXTURE_BASE::drawTexture
	//
	//	�@�\		�`��(2D�p)
	//	����		inSprite	�X�v���C�g�f�o�C�X
	//				rcFrom		�`�挳��`
	//				rcTo		�`����`
	//				fPosZ		Z�o�b�t�@
	//				*rot		��]�ʒu�I�t�Z�b�g
	//				angle		��]�p�x
	//				color		�f�B�t���[�Y
	//	�X�V		2008/05/03	<�V�K>
	//================================================================================
	BOOL
	CTEXTURE_BASE::drawTexture(	CONST SPRITE	inSprite,
								RECT			rcFrom,
								RECT			rcTo,
								float			fPosZ,
								VEC2*			rot,
								float			angle,
								D3DCOLOR		color)
	{
	//-------------------------------------------------
	//	�l�̐ݒ�
	//-------------------------------------------------
		float	fSrcX	= float(rcFrom.right	- rcFrom.left);
		float	fSrcY	= float(rcFrom.bottom	- rcFrom.top);
		float	fDrawX	= float(rcTo.right		- rcTo.left);
		float	fDrawY	= float(rcTo.bottom		- rcTo.top);
		D3DXMATRIX		matDraw;										//!< ���W�ϊ��}�g���b�N�X�̊i�[��
		D3DXVECTOR3		center(0.0f, 0.0f, 0.0f);						//!< �`��̊�l�̐ݒ�
		D3DXVECTOR3		position(0.0f, 0.0f, fPosZ);					//!< �\������ʒu���w��
		D3DXVECTOR2		draw(float(rcTo.left), float(rcTo.top));		//!< �`�����W�i���Z�O�j
		D3DXVECTOR2		scale(fDrawX/fSrcX, fDrawY/fSrcY);				//!< �X�P�[�����O�W���i�{�����w��j
		bool			bAlloc = false;
	//	��]�l�̊m�F
		if(	!rot)
		{
			rot		= new D3DXVECTOR2(fDrawX/2.0f, fDrawY/2.0f);
			bAlloc	= true;
		}

	//-------------------------------------------------
	//	�X�v���C�g�̐ݒ�
	//-------------------------------------------------
		D3DXMatrixTransformation2D(&matDraw, NULL, 0.0f, &scale, rot, angle, &draw);
		if(	bAlloc)
		{
			SAFE_DELETE(rot);
		}
		inSprite->SetTransform(&matDraw);

	//-------------------------------------------------
	//	�`��
	//-------------------------------------------------
		if(	FAILED(inSprite->Draw(texd_ptr->texture, &rcFrom, &center, &position, color)))
		{
			return	FALSE;
		}
		else
		{
			return	TRUE;
		}
	}
}
//-----------------------------------------------------------------------
//!	�e�N�X�`���x�[�X�̖��O���
//-----------------------------------------------------------------------