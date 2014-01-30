//***********************************************************************************
//!	�t�@�C����		Class_3DMap.cpp
//
//	�@�\			3D�I�u�W�F�N�g�Ō`�����ꂽ�}�b�v�̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	����
//	�X�V			2008/07/20	<�V�K>
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_3DMap.h"					//!< 3D�I�u�W�F�N�g�}�b�v

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//================================================================================
	//!	���\�b�h��	C3D_MAP::�R���X�g���N�^
	//
	//	����		inTxtNum	�e�L�X�g�ԍ�
	//	�X�V		2008/07/20	<�V�K>
	//================================================================================
	C3D_MAP::C3D_MAP(void)
	:	releaseG(),						//!< ����O���[�v
		viewG(),						//!< �`��O���[�v
		backG(),						//!< �w�i�`��O���[�v
		hitG(),							//!< �Փ˃O���[�v
		squeezeViewG(),					//!< �i�荞�ݕ`��O���[�v
		squeezeBackG()					//!< �i�荞�ݔw�i�O���[�v
	{
		fDistance	= 5000.0f;			//!< ���a
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::�f�X�g���N�^
	//
	//	�X�V		2008/07/20	<�V�K>
	//================================================================================
	C3D_MAP::~C3D_MAP(void)
	{
	//	���
		finalize();
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::addObject
	//
	//	�@�\		���������I�u�W�F�N�g�̃O���[�v�o�^
	//	����		inObject		�o�^����I�u�W�F�N�g
	//				inGroupType		�o�^����O���[�v�^�C�v
	//	�߂�l		TRUE			�o�^����
	//				FALSE			�o�^������
	//	�X�V		2008/07/20		<�V�K>
	//================================================================================
	BOOL
	C3D_MAP::addObject(C3D_OBJECT* inObject, int inGroupType)
	{
	//	�O���[�v�^�C�v����
		switch(inGroupType)
		{
			case eMG_VIEW_HIT:							//!< �`��A�Փ�
				viewG.add(inObject);
				hitG.add(inObject);
				return	TRUE;

			case eMG_VIEW:								//!< �`��
				viewG.add(inObject);
				return	TRUE;

			case eMG_BACK_VIEW_HIT:						//!< �w�i�`��A�Փ�
				backG.add(inObject);
				hitG.add(inObject);
				return	TRUE;

			case eMG_BACK_VIEW:							//!< �w�i�`��
				backG.add(inObject);
				return	TRUE;

			case eMG_HIT:								//!< �Փ�
				hitG.add(inObject);
				return	TRUE;

			default:									//!< ��L�ȊO
				return	FALSE;
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::checkMessage
	//
	//	�@�\		�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N
	//	����		*inObjP			�I�u�W�F�N�g�C�e���[�^
	//	�߂�l		TRUE			�`��n�j
	//				FALSE			�`��֎~
	//	�X�V		2008/07/20		<�V�K>
	//================================================================================
	BOOL
	C3D_MAP::checkMessage(vector<C3D_OBJECT*>::iterator* inObjP)
	{
		C3D_OBJECT*	inObj	= (*(*inObjP));					//!< �|�C���^�[�i�[
	//	��Ԍ���
		switch((*(*inObjP))->getOrderMessage())
		{
			case	eOMSG_DELETE_IN:						//!< ���ŏ���
				releaseG.add((*(*inObjP)));			//!< ���ŃO���[�v�ɓ����
				(*(*inObjP))->setOrderMessage(eOMSG_NOMSG);	//!< ���b�Z�[�W������
				return	TRUE;

			case	eOMSG_DELETE:							//!< ���Ŗ���
				releaseG.erase(inObj);				//!< ��d�J����h����
				hitG.erase(inObj);					//!< ��d�J����h����
				(*inObjP)	= viewG.erase(*inObjP);	//!< �`��O���[�v�������
				SAFE_DELETE(inObj);							//!< ���
				return	FALSE;

			default:										//!< ��L�ȊO
				return	TRUE;
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::initialize
	//
	//	�@�\		�O���ǂݍ���
	//	����		inTxtNum	�e�L�X�g�ԍ�
	//	�X�V		2008/07/20	<�V�K>
	//================================================================================
	void
	C3D_MAP::initialize(CONST DEV inDev, int inTxtNum)
	{
	//	�ǂݍ��ݏ���
		FILE*	fp;								//!< �t�@�C���|�C���^�[
		char	textName[MAX_PATH]		= "";	//!< �e�L�X�g���쐬�p

	//	�e�L�X�g���𐶐�
		sprintf_s(textName, sizeof(textName), PATH_MAP, inTxtNum);
	//	�t�@�C�����J��
		fopen_s(&fp, textName, "r");

	//	�t�@�C�����J���Ȃ�������
		if(	!fp)
		{
		//	�ǂݍ��݂��s�킸�A������Ԃ�
			return;
		}

	//	�������
		finalize();

	//	�ǂݍ��݊J�n
		char				szGroupType[MAX_PATH]	= "";			//!< �O���[�v�^�C�v���蕶����
		int					iGroupType				= 0;			//!< �O���[�v�^�C�v
		C3D_OBJECT*			newObject				= NULL;			//!< �I�u�W�F�N�g�i�[
		CREATE_OBJECT_DATA	newObjData;								//!< �f�[�^�i�[
		BOOL				bCheckEnd;								//!< �I���m�F
		float				fDiffuse				= 0.0f;			//!< �f�B�t���[�Y�̌v�Z����

	//	�O���[�v�^�C�v����z��
		char	groupJudger[eMG_MAX][11]	=
		{
			"VIEW_HIT__",		//!< �`��ƏՓ�
			"VIEW______",		//!< �`��
			"BACK_VIEWH",		//!< �w�i�`��ƏՓ�
			"BACK_VIEW_",		//!< �w�i�`��
			"HIT_______",		//!< �Փ�
			"ENEMY_____",		//!< �G
		};

		ZeroMemory(&newObjData, sizeof(CREATE_OBJECT_DATA));		//!< �f�[�^��������

	//	�I���̖��߂�����܂ŁA���[�v
		do
		{
			fscanf_s(fp, "%s, ", szGroupType, sizeof(szGroupType));	//!< �O���[�v�^�C�v

		//	�O���[�v����
			for(iGroupType = 0; iGroupType < eMG_MAX; iGroupType++)
			{
				if(	strcmp(szGroupType, groupJudger[iGroupType]) == 0)	break;
			}

		//	�O���[�v�ɊY�����Ȃ��f�[�^�Ȃ�A�������I��
			if(	iGroupType >= eMG_MAX)	break;

			fscanf_s(fp, "%s %s %f, %f, %f, %f, %f, %f, %f, %f, ",
				 newObjData.szData,	sizeof(newObjData.szData),		//!< �I�u�W�F�N�g�^�C�v
				 newObjData.szName, sizeof(newObjData.szName),		//!< �O���t�@�C����
				&newObjData.loc.x,									//!< X�ʒu
				&newObjData.loc.y,									//!< Y�ʒu
				&newObjData.loc.z,									//!< Z�ʒu
				&newObjData.angle.x,								//!< �p�xX
				&newObjData.angle.y,								//!< �p�xY
				&newObjData.angle.z,								//!< �p�xZ
				&newObjData.scale.x,								//!< �g���X
				&newObjData.scale.y);								//!< �g���Y

		//	�I�u�W�F�N�g����
			bCheckEnd	= createObject(inDev, &newObject, &newObjData);

		//	�����ɐ���������A�O���[�v�o�^
			if(	bCheckEnd)
			{
			//	�f�B�t���[�Y�̐ݒ�
			////--------------------------------------------------------
			////	�������������C���̗]�n����
			//	if(	newObject->getLocZ() > 0)
			//	{
			//		fDiffuse	= -(newObject->getLocZ() / 3000.0f);
			//		newObject->setDiffuseAlpha(fDiffuse);
			//	}
			////--------------------------------------------------------
			//	��ɕK������O���[�v�ɓo�^
				releaseG.add(newObject);
			//	�O���[�v�ɓo�^���āA�������Ȃ�s����������̂ŁA�����ŏ����I��
				if(	!addObject(newObject, iGroupType))	break;
			}
			ZeroMemory(&newObjData, sizeof(CREATE_OBJECT_DATA));	//!< �f�[�^��������
		}
	//	�����Ɏ��s���Ă�����A���[�v�I��
		while(bCheckEnd);

	//	�t�@�C�������
		fclose(fp);
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::finalize
	//
	//	�@�\		���
	//	�X�V		2008/07/20	<�V�K>
	//================================================================================
	void
	C3D_MAP::finalize(void)
	{
	//	����O���[�v�̉��
		vector<C3D_OBJECT*>::iterator	p;		//!< ��Ɨp
		for(p  = releaseG.getBegin();
			p != releaseG.getEnd();
			p++)
		{
			SAFE_DELETE(*p);
		}
		releaseG.clear();

		viewG.clear();					//!< �`��O���[�v�̏���
		backG.clear();					//!< �w�i�`��O���[�v�̏���
		hitG.clear();						//!< �Փ˃O���[�v�̏���

		squeezeViewG.clear();				//!< �i�荞�ݕ`��O���[�v�̏���
		squeezeBackG.clear();				//!< �i�荞�ݔw�i�O���[�v�̏���
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::draw
	//
	//	�@�\		�`��
	//	����		inDev			Direct3D�f�o�C�X
	//				inCamera		�J�����I�u�W�F�N�g
	//	�X�V		2008/07/20		<�V�K>
	//================================================================================
	void
	C3D_MAP::draw(CONST DEV inDev, C3D_OBJECT* inCamera)
	{
	//	���בւ�
		backG.sortByCameraPosition(inCamera);
		viewG.sortByCameraPosition(inCamera);

		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vector�|�C���^�[
		vector<C3D_OBJECT*>::iterator	p;			//!< ��Ɨp�C�e���[�^

	//	�������Ȃ���`�悷�邽�߁A�Ƃ肾���ĕ`��
	//	�w�i�`��
		c3dObjvec	= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	checkMessage(&p))
			{
				(*p)->draw(inDev);
				p++;
			}
		}
	//	�ʏ�`��
		c3dObjvec	= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	checkMessage(&p))
			{
				(*p)->draw(inDev);
				p++;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::draw
	//
	//	�@�\		�`��(�����Ȃ����́A�`�悵�Ȃ�)
	//	����		inDev			Direct3D�f�o�C�X
	//				inCamera		�J�����I�u�W�F�N�g
	//				inViewPoint		�J�������_
	//	�X�V		2008/07/20		<�V�K>
	//================================================================================
	void
	C3D_MAP::draw(CONST DEV inDev, C3D_OBJECT* inCamera, VEC3* inViewPoint)
	{
	//	���בւ�
		backG.sortByCameraPosition(inCamera);
		viewG.sortByCameraPosition(inCamera);

		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vector�|�C���^�[
		vector<C3D_OBJECT*>::iterator	p;			//!< ��Ɨp�C�e���[�^

	//	�������Ȃ���`�悷�邽�߁A�Ƃ肾���ĕ`��
	//	�w�i�`��
		c3dObjvec	= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					(*p)->draw(inDev);
				}
				p++;
			}
		}
	//	�ʏ�`��
		c3dObjvec	= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					(*p)->draw(inDev);
				}
				p++;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::draw
	//
	//	�@�\		�`��(�����Ȃ����́A�`�悵�Ȃ�)(�\�[�g���Ȃ�)
	//	����		inDev			Direct3D�f�o�C�X
	//				inViewPoint		�J�������_
	//	�X�V		2008/07/20		<�V�K>
	//================================================================================
	void
	C3D_MAP::draw(CONST DEV inDev, VEC3* inViewPoint)
	{
		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vector�|�C���^�[
		vector<C3D_OBJECT*>::iterator	p;			//!< ��Ɨp�C�e���[�^

	//	�������Ȃ���`�悷�邽�߁A�Ƃ肾���ĕ`��
	//	�w�i�`��
		c3dObjvec	= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					(*p)->draw(inDev);
				}
				p++;
			}
		}
	//	�ʏ�`��
		c3dObjvec	= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					(*p)->draw(inDev);
				}
				p++;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::draw
	//
	//	�@�\		�`��(�����Ȃ����́A�`�悵�Ȃ�)(�\�[�g���Ȃ�)(Z�e�N�X�`���V�F�[�_�g�p)
	//	����		inZTexSha		Z�e�N�X�`���V�F�[�_
	//				inViewPoint		�J�������_
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	C3D_MAP::draw(CZTEX_SHA* inZTexSha, VEC3* inViewPoint)
	{
		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vector�|�C���^�[
		vector<C3D_OBJECT*>::iterator	p;			//!< ��Ɨp�C�e���[�^

	//	�������Ȃ���`�悷�邽�߁A�Ƃ肾���ĕ`��
	//	�w�i�`��
		c3dObjvec	= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
			{
				(*p)->draw(inZTexSha);
			}
			p++;
		}
	//	�ʏ�`��
		c3dObjvec	= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
			{
				(*p)->draw(inZTexSha);
			}
			p++;
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::draw
	//
	//	�@�\		�`��(�����Ȃ����́A�`�悵�Ȃ�)(�\�[�g���Ȃ�)(�[�x�o�b�t�@�V�F�[�_�g�p)
	//	����		inDepthSha		�[�x�o�b�t�@�V�F�[�_
	//				inViewPoint		�J�������_
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	C3D_MAP::draw(CDEPTH_SHA* inDepthSha, VEC3* inViewPoint)
	{
		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vector�|�C���^�[
		vector<C3D_OBJECT*>::iterator	p;			//!< ��Ɨp�C�e���[�^

	//	�������Ȃ���`�悷�邽�߁A�Ƃ肾���ĕ`��
	//	�w�i�`��
		c3dObjvec	= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
			{
				(*p)->draw(inDepthSha);
			}
			p++;
		}
	//	�ʏ�`��
		c3dObjvec	= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					(*p)->draw(inDepthSha);
				}
				p++;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::drawNotCheckMessage
	//
	//	�@�\		�`��(�����Ȃ����́A�`�悵�Ȃ�)(�\�[�g���Ȃ�)(�[�x�o�b�t�@�V�F�[�_�g�p)
	//				���b�Z�[�W�`�F�b�N�����Ȃ�
	//	����		inDepthSha		�[�x�o�b�t�@�V�F�[�_
	//				inViewPoint		�J�������_
	//	�X�V		2009/02/11		<�V�K>
	//================================================================================
	void
	C3D_MAP::drawNotCheckMessage(CDEPTH_SHA* inDepthSha, VEC3* inViewPoint)
	{
		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vector�|�C���^�[
		vector<C3D_OBJECT*>::iterator	p;			//!< ��Ɨp�C�e���[�^

	//	�������Ȃ���`�悷�邽�߁A�Ƃ肾���ĕ`��
	//	�w�i�`��
		c3dObjvec	= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
			{
				(*p)->draw(inDepthSha);
			}
			p++;
		}
	//	�ʏ�`��
		c3dObjvec	= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
			{
				(*p)->draw(inDepthSha);
			}
			p++;
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::squeezeSortByCameraPosition
	//
	//	�@�\		�i�荞�݃\�[�g
	//	����		inCamera		�J����
	//				inViewPoint		�J�������_
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	C3D_MAP::squeezeSortByCameraPosition(C3D_OBJECT* inCamera, VEC3* inViewPoint)
	{
		squeezeBackG.clear();					//!< �i�荞�ݔw�i�O���[�v�̏���
		squeezeViewG.clear();					//!< �i�荞�ݕ`��O���[�v�̏���

		vector<C3D_OBJECT*>*	c3dObjvec;			//!< vector�|�C���^�[
		vector<C3D_OBJECT*>*	c3dObjvecSqueeze;	//!< �i�荞�ݔw�ivector�|�C���^�[
		vector<C3D_OBJECT*>::iterator	p;			//!< ��Ɨp�C�e���[�^


	//	�w�i
		c3dObjvecSqueeze	= squeezeBackG.getObjects();
		c3dObjvec			= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					c3dObjvecSqueeze->push_back(*p);
				}
				p++;
			}
		}

	//	�ʏ�
		c3dObjvecSqueeze	= squeezeViewG.getObjects();
		c3dObjvec			= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					c3dObjvecSqueeze->push_back(*p);
				}
				p++;
			}
		}

	//	�\�[�g
		squeezeBackG.sortByCameraPosition(inCamera);
		squeezeViewG.sortByCameraPosition(inCamera);
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::squeezeSortByCameraPositionWithDrawZTexSha
	//
	//	�@�\		�i�荞�݂��Ȃ���Z�e�N�X�`���ŕ`������A�\�[�g����
	//	����		inCamera		�J����
	//				inViewPoint		�J�������_
	//				inZTexSha		Z�e�N�X�`���V�F�[�_
	//	�X�V		2009/01/31		<�V�K>
	//================================================================================
	void
	C3D_MAP::squeezeSortByCameraPositionWithDrawZTexSha(
				C3D_OBJECT* inCamera, VEC3* inViewPoint, CZTEX_SHA*  inZTexSha)
	{
		squeezeBackG.clear();					//!< �i�荞�ݔw�i�O���[�v�̏���
		squeezeViewG.clear();					//!< �i�荞�ݕ`��O���[�v�̏���

		vector<C3D_OBJECT*>*	c3dObjvec;			//!< vector�|�C���^�[
		vector<C3D_OBJECT*>*	c3dObjvecSqueeze;	//!< �i�荞�ݔw�ivector�|�C���^�[
		vector<C3D_OBJECT*>::iterator	p;			//!< ��Ɨp�C�e���[�^

	//	�w�i
		c3dObjvecSqueeze	= squeezeBackG.getObjects();
		c3dObjvec			= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					(*p)->draw(inZTexSha);
					c3dObjvecSqueeze->push_back(*p);
				}
				p++;
			}
		}

	//	�ʏ�
		c3dObjvecSqueeze	= squeezeViewG.getObjects();
		c3dObjvec			= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			if(	checkMessage(&p))
			{
				if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
				{
					(*p)->draw(inZTexSha);
					c3dObjvecSqueeze->push_back(*p);
				}
				p++;
			}
		}

	//	�\�[�g
		squeezeBackG.sortByCameraPosition(inCamera);
		squeezeViewG.sortByCameraPosition(inCamera);
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::squeezeSortByCameraPositionNotCheckMessageWithDrawZTexSha
	//
	//	�@�\		�i�荞�݂��Ȃ���Z�e�N�X�`���ŕ`������A�\�[�g����
	//				���b�Z�[�W�`�F�b�N�͂��Ȃ�
	//	����		inCamera		�J����
	//				inViewPoint		�J�������_
	//				inZTexSha		Z�e�N�X�`���V�F�[�_
	//	�X�V		2009/01/31		<�V�K>
	//================================================================================
	void
	C3D_MAP::squeezeSortByCameraPositionNotCheckMessageWithDrawZTexSha(
				C3D_OBJECT* inCamera, VEC3* inViewPoint, CZTEX_SHA*  inZTexSha)
	{
		squeezeBackG.clear();						//!< �i�荞�ݔw�i�O���[�v�̏���
		squeezeViewG.clear();						//!< �i�荞�ݕ`��O���[�v�̏���

		vector<C3D_OBJECT*>*	c3dObjvec;			//!< vector�|�C���^�[
		vector<C3D_OBJECT*>*	c3dObjvecSqueeze;	//!< �i�荞�ݔw�ivector�|�C���^�[
		vector<C3D_OBJECT*>::iterator	p;			//!< ��Ɨp�C�e���[�^

	//	�w�i
		c3dObjvecSqueeze	= squeezeBackG.getObjects();
		c3dObjvec			= backG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
			if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
			{
				(*p)->draw(inZTexSha);
				c3dObjvecSqueeze->push_back(*p);
			}
			p++;
		}

	//	�ʏ�
		c3dObjvecSqueeze	= squeezeViewG.getObjects();
		c3dObjvec			= viewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
			if(	D3DXVec3Length(&(*inViewPoint - (*p)->getLoc())) < fDistance)
			{
				(*p)->draw(inZTexSha);
				c3dObjvecSqueeze->push_back(*p);
			}
			p++;
		}

	//	�\�[�g
		squeezeBackG.sortByCameraPosition(inCamera);
		squeezeViewG.sortByCameraPosition(inCamera);
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::squeezeDrawWithZTexSha
	//
	//	�@�\		�i�荞�ݕ`��(Z�e�N�X�`���V�F�[�_�g�p)
	//	����		inZTexSha		Z�e�N�X�`���V�F�[�_
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	C3D_MAP::squeezeDrawWithZTexSha(CZTEX_SHA*  inZTexSha)
	{
		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vector�|�C���^�[
		vector<C3D_OBJECT*>::iterator	p;			//!< ��Ɨp�C�e���[�^

	//	�������Ȃ���`�悷�邽�߁A�Ƃ肾���ĕ`��
	//	�w�i�`��
		c3dObjvec	= squeezeBackG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
			(*p)->draw(inZTexSha);
			p++;
		}
	//	�ʏ�`��
		c3dObjvec	= squeezeViewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			(*p)->draw(inZTexSha);
			p++;
		}
	}

	//================================================================================
	//!	���\�b�h��	C3D_MAP::squeezeDrawWithDepthSha
	//
	//	�@�\		�i�荞�ݕ`��(�[�x�o�b�t�@�V�F�[�_�g�p)
	//	����		inDepthSha		�[�x�o�b�t�@�V�F�[�_
	//	�X�V		2008/12/14		<�V�K>
	//================================================================================
	void
	C3D_MAP::squeezeDrawWithDepthSha(CDEPTH_SHA* inDepthSha)
	{
		vector<C3D_OBJECT*>*			c3dObjvec;	//!< vector�|�C���^�[
		vector<C3D_OBJECT*>::iterator	p;			//!< ��Ɨp�C�e���[�^

	//	�������Ȃ���`�悷�邽�߁A�Ƃ肾���ĕ`��
	//	�w�i�`��
		c3dObjvec	= squeezeBackG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
			(*p)->draw(inDepthSha);
			p++;
		}
	//	�ʏ�`��
		c3dObjvec	= squeezeViewG.getObjects();
		p	= c3dObjvec->begin();
		while(p != c3dObjvec->end())
		{
		//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N���āA�`�攻��
			(*p)->draw(inDepthSha);
			p++;
		}
	}
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------