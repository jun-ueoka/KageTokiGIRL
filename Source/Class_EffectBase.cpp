//***********************************************************************************
//!	�t�@�C����		Class_EffectBase.cpp
//
//	�@�\			�G�t�F�N�g�x�[�X�Ɋւ������
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/09/12	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_EffectBase.h"

//-----------------------------------------------------------------------
//!	�G�t�F�N�g�x�[�X�̖��O���
//-----------------------------------------------------------------------
namespace EB
{
	//================================================================================
	//	���L�G�t�F�N�g�}�b�v
	//================================================================================
	map<string, EFFECT_DATA*>*		CEFFECT_BASE::mEffects	= NULL;

	//================================================================================
	//!	���\�b�h��	CEFFECT_BASE::createEffect
	//
	//	�@�\		�G�t�F�N�g�t�@�C�������[�h����B
	//	�@�\�T�v	�G�t�F�N�g�t�@�C�������[�h���A���̃G�t�F�N�g����Ԃ�
	//				���̃��\�b�h�̓N���X���\�b�h�ł���B
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			�G�t�F�N�g�t�@�C���̖���
	//				inResource		�G�t�F�N�g�t�H���_��
	//				inTexture		�G�t�F�N�g�e�N�X�`���t�H���_��
	//	�߂�l		�G�t�F�N�g���̃|�C���^�[
	//	�X�V		2008/09/12		<�V�K>
	//================================================================================
	EFFECT_DATA*
	CEFFECT_BASE::createEffect(	CONST DEV	inDev,
								CONST LPSTR inName,
								CONST LPSTR inResource,
								CONST LPSTR inTexture)
	{
		EFFECT_DATA*	newEffect	= new EFFECT_DATA();	//!< �V���ȃe�N�X�`�����
		TCHAR			realName[MAX_PATH]	= "";			//!< ���O�Ƀp�X�������

	//-------------------------------------------------
	//	�t�@�C�����J��
	//-------------------------------------------------
		FILE*	fp	= NULL;

	//	�t�@�C�����J��
		fopen_s(&fp, inName, "r");

	//	�t�@�C�����J���Ȃ�������
		if(	!fp)
		{
		//	�p�X�t���G�t�F�N�g�t�@�C�����𐶐�
			sprintf_s(realName, sizeof(realName), inResource, inName);
		//	�t�@�C�����J��
			fopen_s(&fp, realName, "r");

		//	�t�@�C�����J���Ȃ�������
			if(	!fp)
			{
			//	�ǂݍ��݂��s�킸�ANULL��Ԃ�������
				goto error;
			}
		}

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	�t�@�C�����J������A�ǂݍ��݊J�n
	//-------------------------------------------------
	//	�����T�C�Y��ǂݍ���
	//-------------------------------------------------
		fscanf_s(fp, "%f, %f, ", &newEffect->size.x, &newEffect->size.y);

	//-------------------------------------------------
	//	�e�N�X�`���z���ǂݍ���
	//-------------------------------------------------
		int		r=0, g=0, b=0, a=0;						//!< RGBA
		int		iArrayNum				= 0;			//!< �z��ԍ�
		TCHAR	texName[MAX_PATH]		= "";			//!< �e�N�X�`�����ǂݎ��
		TEXINFO	texInfo;								//!< �e�N�X�`�����

		while(1)
		{
		//	�ǂݍ��ݐ�z��ԍ���ǂݍ��݁A�I�����߁A���͕s���Ȓl���o����I��
			fscanf_s(fp, "%d, ", &iArrayNum);
			if(	taskBreak(iArrayNum))	{ break; }

			ZeroMemory(&texInfo, sizeof(texInfo));

		//	�ǂݍ��ݐ�z��ԍ��̃e�N�X�`���f�[�^���������
			SAFE_DELETE(newEffect->textures[iArrayNum]);
		//	�e�N�X�`���f�[�^�𐶐�
			newEffect->textures[iArrayNum]	= new EFFECT_TEX_DATA();

		//	�e�N�X�`������ǂݍ���
			fscanf_s(fp, "%s  ", texName, sizeof(texName));
			fscanf_s(fp, "%d, %d, %d, %d, ", &r, &g, &b, &a);

		//	�e�N�X�`����ǂݍ���ł݂�
			if(	FAILED(D3DX_CREATE_TEXTURE_EX(
					inDev, texName, D3DCOLOR_RGBA(r,g,b,a),
					&texInfo, &newEffect->textures[iArrayNum]->texture)))
			{
			//	���s������A�e�N�X�`���t�@�C���ւ̃p�X�����ăg���C
				sprintf_s(realName, sizeof(realName), inTexture, texName);

				if(	FAILED(D3DX_CREATE_TEXTURE_EX(
						inDev, realName, D3DCOLOR_RGBA(r,g,b,a),
						&texInfo, &newEffect->textures[iArrayNum]->texture)))
				{
				//	���s������ANULL��Ԃ�
					goto error;
				}
			}
		//	�e�N�X�`�������m��
			newEffect->textures[iArrayNum]->name	= realName;
		//	�e�N�X�`���T�C�Y���m��
			newEffect->textures[iArrayNum]->sizeX	= texInfo.Width;
			newEffect->textures[iArrayNum]->sizeY	= texInfo.Height;
		//	�J���[���
			newEffect->textures[iArrayNum]->myColor.rgba.red	= BYTE(r);
			newEffect->textures[iArrayNum]->myColor.rgba.green	= BYTE(g);
			newEffect->textures[iArrayNum]->myColor.rgba.blue	= BYTE(b);
			newEffect->textures[iArrayNum]->myColor.rgba.alpha	= BYTE(a);
		}

	//-------------------------------------------------
	//	�e�N�X�`����ǂݍ���
	//-------------------------------------------------
		EFFECT_TEXTRUE*		newEffTex	= NULL;			//!< �G�t�F�N�g�e�N�X�`���m�ې�

		while(1)
		{
		//	�V���ɃG�t�F�N�g�e�N�X�`���𐶐�
			newEffTex	= new EFFECT_TEXTRUE();

		//	��ɓo�^
			newEffect->vTex.push_back(newEffTex);

		//	���s�t���[����ǂݍ��݁A�I�����߂��o����I��
			fscanf_s(fp, "%d, ", &newEffTex->iFrame);
			if(	taskBreak(newEffTex->iFrame))	{ break; }
			if(	taskReturn(newEffTex->iFrame))	{ break; }

		//	�e�N�X�`������ǂݍ���
			fscanf_s(fp, "%d, ", &newEffTex->iArrayNum);
		}

	//-------------------------------------------------
	//	�T�C�Y��ǂݍ���
	//-------------------------------------------------
		EFFECT_SIZE*		newEffSize	= NULL;			//!< �G�t�F�N�g�T�C�Y�m�ې�

		while(1)
		{
		//	�V���ɃG�t�F�N�g�T�C�Y�𐶐�
			newEffSize	= new EFFECT_SIZE();

		//	��ɓo�^
			newEffect->vSize.push_back(newEffSize);

		//	���s�t���[����ǂݍ��݁A�I�����߂��o����I��
			fscanf_s(fp, "%d, ", &newEffSize->iFrame);
			if(	taskBreak(newEffSize->iFrame))	{ break; }
			if(	taskReturn(newEffSize->iFrame))	{ break; }

		//	�T�C�Y����ǂݍ���
			fscanf_s(fp, "%f, %f, ", &newEffSize->size.x, &newEffSize->size.y);
		}

	//-------------------------------------------------
	//	��]�l��ǂݍ���
	//-------------------------------------------------
		EFFECT_ROTATO*		newEffRotato	= NULL;			//!< �G�t�F�N�g��]�m�ې�

		while(1)
		{
		//	�V���ɃG�t�F�N�g��]�𐶐�
			newEffRotato	= new EFFECT_ROTATO();

		//	��ɓo�^
			newEffect->vRotato.push_back(newEffRotato);

		//	���s�t���[����ǂݍ��݁A�I�����߂��o����I��
			fscanf_s(fp, "%d, ", &newEffRotato->iFrame);
			if(	taskBreak(newEffRotato->iFrame))	{ break; }
			if(	taskReturn(newEffRotato->iFrame))	{ break; }

		//	��]�l����ǂݍ���
			fscanf_s(fp, "%f, %f, %f, ",
				&newEffRotato->rotato.x,
				&newEffRotato->rotato.y,
				&newEffRotato->rotato.z);
		}

	//-------------------------------------------------
	//	�ړ��ʂ�ǂݍ���
	//-------------------------------------------------
		EFFECT_MOVE*		newEffMove	= NULL;			//!< �G�t�F�N�g�ړ��m�ې�

		while(1)
		{
		//	�V���ɃG�t�F�N�g�ړ��𐶐�
			newEffMove	= new EFFECT_MOVE();

		//	��ɓo�^
			newEffect->vMove.push_back(newEffMove);

		//	���s�t���[����ǂݍ��݁A�I�����߂��o����I��
			fscanf_s(fp, "%d, ", &newEffMove->iFrame);
			if(	taskBreak(newEffMove->iFrame))	{ break; }
			if(	taskReturn(newEffMove->iFrame))	{ break; }

		//	�ړ�����ǂݍ���
			fscanf_s(fp, "%f, %f, %f, ",
				&newEffMove->middle.x,
				&newEffMove->middle.y,
				&newEffMove->middle.z);
			fscanf_s(fp, "%f, %f, %f, ",
				&newEffMove->goal.x,
				&newEffMove->goal.y,
				&newEffMove->goal.z);
		}

	//-------------------------------------------------
	//	�ϐF�l��ǂݍ���
	//-------------------------------------------------
		EFFECT_COLOR*		newEffColor	= NULL;			//!< �G�t�F�N�g�J���[�m�ې�

		while(1)
		{
		//	�V���ɃG�t�F�N�g�J���[�𐶐�
			newEffColor	= new EFFECT_COLOR();

		//	��ɓo�^
			newEffect->vColor.push_back(newEffColor);

		//	���s�t���[����ǂݍ��݁A�I�����߂��o����I��
			fscanf_s(fp, "%d, ", &newEffColor->iFrame);
			if(	taskBreak(newEffColor->iFrame))	{ break; }
			if(	taskReturn(newEffColor->iFrame)){ break; }

		//	�ϐF�l����ǂݍ���
			fscanf_s(fp, "%d, %d, %d, %d, ",
				&newEffColor->red,
				&newEffColor->green,
				&newEffColor->blue,
				&newEffColor->alpha);
		}

	//	�ő�t���[�����̓ǂݍ���
		fscanf_s(fp, "%d, ", &newEffect->frame);

	//	�f�o�C�X�̃|�C���^��ێ�
		newEffect->device	= inDev;

	//	�G�t�F�N�g����ۑ�
		newEffect->name		= inName;

		fclose(fp);
	//	�쐬�����G�t�F�N�g�̏���Ԃ�
		return newEffect;

	//	�ǂݍ��ݏI��
	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	//	NULL��Ԃ�����
	error:
		SAFE_DELETE(newEffect);				//!< �����������b�V�������J��
		DX::errorMsgNoFile(inDev, realName);
		return	NULL;						//!< NULL��Ԃ�
	}

	//================================================================================
	//!	���\�b�h��	CEFFECT_BASE::initializeVertex
	//
	//	�@�\		���_�o�b�t�@�𐶐�����
	//	����		inDev		Direct3D�f�o�C�X
	//	�X�V		2008/08/28	<�V�K>
	//================================================================================
	void
	CEFFECT_BASE::initializeVertex(CONST DEV inDev)
	{
	//	�����̏�����
		SAFE_RELEASE(vb);		

	//	���_�o�b�t�@�̐���
		HRESULT	hr;
		hr = inDev->CreateVertexBuffer(
				NUM_EFFECT_VERTEX * sizeof(CUSTOM_VERTEX),
				D3DUSAGE_WRITEONLY,
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
	//!	���\�b�h��	CEFFECT_BASE::updateCorner
	//
	//	�@�\		�R�[�i�[�̍X�V
	//	�X�V		2008/09/12	<�V�K>
	//================================================================================
	void
	CEFFECT_BASE::updateCorner(void)
	{
		VEC2	h	= VEC2(-(showSize.x / 2.0f),  (showSize.x / 2.0f));
		VEC2	v	= VEC2( (showSize.y / 2.0f), -(showSize.y / 2.0f));

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
		for(int i = 0; i < NUM_EFFECT_VERTEX; i++)
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
	//!	���\�b�h��	CEFFECT_BASE::updateColor
	//
	//	�@�\		�J���[�̍X�V
	//	�X�V		2008/09/12	<�V�K>
	//================================================================================
	void
	CEFFECT_BASE::updateColor(void)
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
	//!	���\�b�h��	CEFFECT_BASE::updateUVCoord
	//
	//	�@�\		UV���W�̍X�V
	//	�X�V		2008/09/12	<�V�K>
	//================================================================================
	void
	CEFFECT_BASE::updateUVCoord(void)
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
		pV[0].uv		= VEC2(0.00001f, 0.00001f);
		pV[1].uv		= VEC2(1.0f,     0.00001f);
		pV[2].uv		= VEC2(1.0f,     1.0f);
		pV[3].uv		= VEC2(0.00001f, 1.0f);
		vb->Unlock();
	}

	//================================================================================
	//!	���\�b�h��	CEFFECT_BASE::startEffectInitialize
	//
	//	�@�\		�����̏�����
	//	�X�V		2008/09/12		<�V�K>
	//================================================================================
	void
	CEFFECT_BASE::startEffectInitialize(void)
	{
		vb				= NULL;					//!< �o�[�e�b�N�X�o�b�t�@
		showSize		= VEC2(0.0f, 0.0f);		//!< �\���T�C�Y
		color			= MY_COLOR_RGBA();		//!< �F

		ed_ptr			= NULL;					//!< �G�t�F�N�g���
		bEffectLoaded	= FALSE;				//!< �G�t�F�N�g���[�h�m�F

	}

	//================================================================================
	//!	���\�b�h��	CEFFECT_BASE::initializeEffect
	//
	//	�@�\		�G�t�F�N�g�̏�����
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			�G�t�F�N�g�t�@�C���̖���
	//				inResource		�G�t�F�N�g�t�H���_��
	//				inTexture		�G�t�F�N�g�e�N�X�`���t�H���_��
	//	�X�V		2008/09/12		<�V�K>
	//================================================================================
	void
	CEFFECT_BASE::initializeEffect(	CONST DEV	inDev,
									CONST LPSTR inName,
									CONST LPSTR inResource,
									CONST LPSTR inTexture)
	{
	//	�G�t�F�N�g�̏������
		finishalizeEffect();

	//	�G�t�F�N�g�}�b�v��������ΐ�������
		if(	!mEffects)
		{
			mEffects = new map<string, EFFECT_DATA*>;
		}

	//	��Ɨp�C�e���[�^
		map<string, EFFECT_DATA*>::iterator	p;
	//	�G�t�F�N�g�}�b�v�ɃG�t�F�N�g�t�@�C������n���A���̃C�e���[�^���󂯎��
		p = mEffects->find(inName);

	//	�G�t�F�N�g�}�b�v�ɓ������O�̃G�t�F�N�g�t�@�C���������ɓo�^���Ă���ꍇ
		if(	p != mEffects->end())
		{
		//	�g�p�J�E���^�𑝂₷
			p->second->cnt++;
		//	�G�t�F�N�g�����󂯎��
			ed_ptr	= (*p).second;
		//	���[�h����
			bEffectLoaded	= TRUE;
		}
		else
		{
		//	�V���ɃG�t�F�N�g�𐶐�����
			ed_ptr	= createEffect(inDev, inName, inResource, inTexture);

		//	�����ɐ��������ꍇ
			if(	ed_ptr)
			{
			//	�g�p�J�E���^�𑝂₷
				ed_ptr->cnt++;
			//	���[�h����
				bEffectLoaded	= TRUE;
			//	�G�t�F�N�g�}�b�v�ɓo�^
				mEffects->insert(pair<string, EFFECT_DATA*>(ed_ptr->name, ed_ptr));
			}
		//	���s�����ꍇ
			else
			{
			//	���[�h������
				bEffectLoaded	= FALSE;
			}
		}

	//	�G�t�F�N�g���������Ă��鎞
		if(	bEffectLoaded)
		{
		//	�c����R�s�[
			showSize	= ed_ptr->size;

		//	���_�o�b�t�@�𐶐�
			initializeVertex(ed_ptr->device);

		//	�e�|�C���^�[���擾
			eiTex		= ed_ptr->vTex.begin();
			eiSize		= ed_ptr->vSize.begin();
			eiRotato	= ed_ptr->vRotato.begin();
			eiMove		= ed_ptr->vMove.begin();
			eiColor		= ed_ptr->vColor.begin();
		}
	}

	//================================================================================
	//!	���\�b�h��	CEFFECT_BASE::finishalizeEffect
	//
	//	�@�\		�G�t�F�N�g�̉��
	//	�X�V		2008/09/12		<�V�K>
	//================================================================================
	void
	CEFFECT_BASE::finishalizeEffect(void)
	{
	//	�G�t�F�N�g�������[�h���ĂȂ��Ȃ�A������Ԃ�
		if(	!bEffectLoaded)		return;

	//	�G�t�F�N�g�J�E���g�����炷
		ed_ptr->cnt--;
	//	���̃G�t�F�N�g���܂��g�p����Ă���ꍇ�͉�����Ȃ�
		if(	ed_ptr->cnt > 0)	return;

	//	�G�t�F�N�g�}�b�v�C�e���[�^
		map<string, EFFECT_DATA*>::iterator	p;
	//	�G�t�F�N�g�}�b�v�ɃG�t�F�N�g�t�@�C������n���A���̃C�e���[�^���󂯎��
		p = mEffects->find(ed_ptr->name);
	//	�G�t�F�N�g���̉��
		SAFE_DELETE(p->second);
	//	�G�t�F�N�g�}�b�v����폜
		mEffects->erase(p);

	//	�G�t�F�N�g�}�b�v�Ƀ����o�[��������΁A��������
		if(	mEffects->begin() == mEffects->end())
		{
			SAFE_DELETE(mEffects);
		}
	}

	//================================================================================
	//!	���\�b�h��	CEFFECT_BASE::�R���X�g���N�^
	//
	//	�@�\		�G�t�F�N�g�t�@�C�������[�h���ď�����
	//	����		inDev			Direct3D�f�o�C�X
	//				inName			�G�t�F�N�g�t�@�C���̖���
	//				inResource		�G�t�F�N�g�t�H���_��
	//				inTexture		�G�t�F�N�g�e�N�X�`���t�H���_��
	//	�X�V		2008/09/12		<�V�K>
	//================================================================================
	CEFFECT_BASE::CEFFECT_BASE(	CONST DEV	inDev,
								CONST LPSTR inName,
								CONST LPSTR inResource,
								CONST LPSTR inTexture)
	{
	//	�����̏�����
		startEffectInitialize();
	//	���b�V���̏�����
		initializeEffect(inDev, inName, inResource, inTexture);
	}

	//================================================================================
	//!	���\�b�h��	CEFFECT_BASE::�R���X�g���N�^
	//
	//	�@�\		�󏉊���
	//	�X�V		2008/09/12		<�V�K>
	//================================================================================
	CEFFECT_BASE::CEFFECT_BASE(void)
	{
	//	�����̏�����
		startEffectInitialize();
	}

	//================================================================================
	//!	���\�b�h��	CEFFECT_BASE::�R���X�g���N�^
	//
	//	�@�\		�R�s�[
	//	����		inCopy			�R�s�[��
	//	�X�V		2008/09/12		<�V�K>
	//================================================================================
	CEFFECT_BASE::CEFFECT_BASE(CEFFECT_BASE* inCopy)
	{
	//	�����̏�����
		startEffectInitialize();
	//	�R�s�[�J�n
		ed_ptr	= inCopy->getEffects();

	//	�G�t�F�N�g��񂪂��鎞
		if(	ed_ptr)
		{
		//	�c����R�s�[
			showSize	= ed_ptr->size;

		//	���_�o�b�t�@�𐶐�
			initializeVertex(ed_ptr->device);

		//	�e�|�C���^�[���擾
			eiTex		= ed_ptr->vTex.begin();
			eiSize		= ed_ptr->vSize.begin();
			eiRotato	= ed_ptr->vRotato.begin();
			eiMove		= ed_ptr->vMove.begin();
			eiColor		= ed_ptr->vColor.begin();

		//	�g�p�J�E���^�𑝂₷
			ed_ptr->cnt++;

		//	���[�h����
			bEffectLoaded	= TRUE;
		}
	}

	//================================================================================
	//!	���\�b�h��	CEFFECT_BASE::�f�X�g���N�^
	//
	//	�X�V		2008/09/12	<�V�K>
	//================================================================================
	CEFFECT_BASE::~CEFFECT_BASE(void)
	{
	//	�G�t�F�N�g�̉��
		finishalizeEffect();
	}

	//================================================================================
	//!	���\�b�h��	CEFFECT_BASE::resetInstance
	//
	//	�@�\		���Z�b�g���s
	//	����		inDev			Direct3D�f�o�C�X
	//	�X�V		2009/03/02		<�V�K>
	//================================================================================
	void
	CEFFECT_BASE::resetInstance(CONST DEV inDev)
	{
	//	�G�t�F�N�g�}�b�v���Ȃ���΁A���Z�b�g���Ȃ�
		if(	!mEffects)	return;

	//	�������ꂽ�G�t�F�N�g���̃e�N�X�`���������Z�b�g
		MAP_LOOP(string, EFFECT_DATA*, (*mEffects))
		{
			LOOP(MAX_TEXTURE)
			{
				if(	(*p).second->textures[i])
				{
				//	��Ƀe�N�X�`���������
					SAFE_RELEASE((*p).second->textures[i]->texture);
				//	�m�ۂ���Ă��e�N�X�`�����ōēx�ǂݍ��݊J�n
					if(	FAILED(D3DX_CREATE_TEXTURE_EX(
							inDev,
							(*p).second->textures[i]->name.c_str(),
							(*p).second->textures[i]->myColor.color,
							NULL,
							&(*p).second->textures[i]->texture)))
					{
					}
				}
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CEFFECT_BASE::effectInvalid
	//
	//	�@�\		�G�t�F�N�g�̖�����������
	//	�߂�l		TRUE		����
	//				FALSE		�L��
	//	�X�V		2008/09/12	<�V�K>
	//================================================================================
	BOOL
	CEFFECT_BASE::effectInvalid(void)
	{
	//	�ǂݍ��܂�Ă��Ȃ���
	//	�G�t�F�N�g����������Ă��Ȃ���
	//	���݂̃e�N�X�`����������
		if(	(!bEffectLoaded)						||
			(!ed_ptr)								||
			(!ed_ptr->textures[(*eiTex)->iArrayNum]))
		{
			return TRUE;					//!< ����
		}
		else								//!< ��L�ɓ��Ă͂܂�Ȃ��Ȃ�
		{
			return FALSE;					//!< �L��
		}
	}

	//================================================================================
	//!	���\�b�h��	CEFFECT_BASE::setColor
	//
	//	�@�\		�J���[�̐ݒ�
	//	����		inRed		�Ԑ���
	//				inGreen		�ΐ���
	//				inBlue		����
	//				inAlpha		�����x
	//	�X�V		2008/09/20	<�V�K>
	//================================================================================
	void
	CEFFECT_BASE::setColor(int inRed, int inGreen, int inBlue, int inAlpha)
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
	//!	���\�b�h��	CEFFECT_BASE::calcColor
	//
	//	�@�\		�J���[�̌v�Z(���Z�̂�)
	//	����		inRgba		RGBA�J���[
	//	�X�V		2008/09/20	<�V�K>
	//================================================================================
	void
	CEFFECT_BASE::calcColor(RGBA inRgba)
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
	//!	���\�b�h��	CEFFECT_BASE::calcColor
	//
	//	�@�\		�J���[�̌v�Z(���Z���Z)
	//	����		inRed		�Ԑ���
	//				inGreen		�ΐ���
	//				inBlue		����
	//				inAlpha		�����x
	//	�X�V		2008/09/20	<�V�K>
	//================================================================================
	void
	CEFFECT_BASE::calcColor(int inRed, int inGreen, int inBlue, int inAlpha)
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
	//!	���\�b�h��	CEFFECT_BASE::drawEffect
	//
	//	�@�\		�`��(3D)
	//	����		inDev		Direct3D�f�o�C�X
	//				inMatrix	���[���h�}�g���b�N�X
	//	�X�V		2008/09/12	<�V�K>
	//================================================================================
	BOOL
	CEFFECT_BASE::drawEffect(CONST DEV inDev, MAT16* inMatrix)
	{
	//	�`��̖�����������Ȃ�A�`�悵�Ȃ�
		if(	effectInvalid())	return FALSE;

		inDev->SetTransform(D3DTS_WORLD, inMatrix);

		inDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		inDev->SetTexture(0, ed_ptr->textures[(*eiTex)->iArrayNum]->texture);

		inDev->SetFVF(D3DFVF_VERTEX);
		inDev->SetStreamSource(0, vb, 0, sizeof(CUSTOM_VERTEX));
		inDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		inDev->SetTexture(0, NULL);

	//	���C�e�B���O���[�h�ɕ��A
		inDev->SetRenderState(D3DRS_LIGHTING, TRUE);

		return	TRUE;
	}

	//================================================================================
	//!	���\�b�h��	CEFFECT_BASE::drawEffect
	//
	//	�@�\		�`��(2D)
	//	����		inSprite	�X�v���C�g�f�o�C�X
	//				inLoc		�`���
	//				inAngle		Z���̊p�x
	//				inScale		�g���
	//	�X�V		2008/09/25	<�V�K>
	//================================================================================
	BOOL
	CEFFECT_BASE::drawEffect(CONST SPRITE inSprite, VEC3 inLoc, float inAngle, float inScale)
	{
	//	�`��̖�����������Ȃ�A�`�悵�Ȃ�
		if(	effectInvalid())	return FALSE;

	//-------------------------------------------------
	//	�l�̐ݒ�
	//-------------------------------------------------
	//	�`�挳��`
		RECT			rcFrom	=
		{
			0,
			0,
			ed_ptr->textures[(*eiTex)->iArrayNum]->sizeX,
			ed_ptr->textures[(*eiTex)->iArrayNum]->sizeY
		};
	//	�\���T�C�Y���X�P�[�����O
		D3DXVECTOR2		scaleSize(showSize.x * inScale, showSize.y * inScale);
	//	���W�ϊ��}�g���b�N�X�̊i�[��
		D3DXMATRIX		matDraw;
	//	�`��̊�l�̐ݒ�
		D3DXVECTOR3		center(0.0f, 0.0f, 0.0f);
	//	�\������ʒu���w��
		D3DXVECTOR3		position(0.0f, 0.0f, inLoc.z);
	//	�`�����W�i���Z�O�j
		D3DXVECTOR2		draw(inLoc.x-(scaleSize.x/2.0f), inLoc.y-(scaleSize.y/2.0f));
	//	�X�P�[�����O�W���i�{�����w��j
		D3DXVECTOR2		scale(scaleSize.x/(float)rcFrom.right, scaleSize.y/(float)rcFrom.bottom);
	//	��]���̐ݒ�
		D3DXVECTOR2		rot(scaleSize.x/2.0f, scaleSize.y/2.0f);

	//-------------------------------------------------
	//	�X�v���C�g�̐ݒ�
	//-------------------------------------------------
		D3DXMatrixTransformation2D(&matDraw, NULL, 0.0f, &scale, &rot, inAngle, &draw);
		inSprite->SetTransform(&matDraw);

	//-------------------------------------------------
	//	�`��
	//-------------------------------------------------
		if(	FAILED(inSprite->Draw(
				ed_ptr->textures[(*eiTex)->iArrayNum]->texture,
				&rcFrom, &center, &position, color.color)))
		{
			return	FALSE;
		}
		else
		{
			return	TRUE;
		}

		return	TRUE;
	}
}
//-----------------------------------------------------------------------
//!	�G�t�F�N�g�x�[�X�̖��O���
//-----------------------------------------------------------------------