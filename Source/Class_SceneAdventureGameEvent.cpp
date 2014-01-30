//***********************************************************************************
//!	�t�@�C����		Class_SceneAdventureGameEvent.cpp
//
//	�@�\			�Q�[���V�[���̃C�x���g���������̎���
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2008/07/10	<�V�K>
//					2008/11/06	<�ύX>	�S�̓I�ɏ�������
//***********************************************************************************

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Precompiler.h"

//#include	"Class_SceneAdventureGameData.h"

//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------
namespace SCE
{
	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventChange
	//
	//	�@�\		�Q�[���C�x���g�ڍs���̏�����
	//	�X�V		2008/11/07		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventChange(void)
	{
	//	�Q�[���C�x���g���Ƀt���O��ύX�ł�����A��������
		if(	this->flagChange(eSGFLAG_EVENT))
		{
		//	�v���C���[�̃_���[�W����m�F�𖳌��ɂ���
			this->player->setDamageCheck(FALSE);
		//	�C�x���g�t�@�C���̓ǂݍ��݂Ɉڂ�
			this->eventStep	= eEVSTEP_READ;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventProc
	//
	//	�@�\		�C�x���g����(�ċA���\�b�h)
	//	����		input			���͏��
	//	�X�V		2008/11/07		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProc(CINPUT* input)
	{
	//-------------------------------------------------
	//	�C�x���g�X�e�b�v����
	//-------------------------------------------------
		switch(this->eventStep)
		{
		//	�ǂݍ��ݒ�
			case	eEVSTEP_READ:
				this->sgEventStepRead();
				this->sgEventProc(input);
				break;
		//	���s��
			case	eEVSTEP_EXECUTE:
				this->sgEventExecuter();
				this->sgEventStepExecute();
				break;
		//	��b��
			case	eEVSTEP_TALK:
				this->sgEventExecuter();
				this->sgEventStepTalk(input);
				break;
		//	���͑ҋ@��
			case	eEVSTEP_INPUTWAIT:
				this->sgEventExecuter();
				this->sgEventStepInputWait(input);
				break;
		//	�C�x���g������
			case	eEVSTEP_FALSE:
				return;
		//	�C�x���g�G���[
			case	eEVSTEP_ERROR:
				this->gameFlag	= eSGFLAG_ERROR;
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventStepRead
	//
	//	�@�\		�C�x���g�t�@�C����ǂ�
	//	�X�V		2008/11/07		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventStepRead(void)
	{
	//	�t�@�C���J���ꂢ�Ȃ��Ȃ�A�C�x���g�I��
		if(	!this->eventFile)
		{
			this->gameFlag	= eSGFLAG_PLAY;
			return;
		}

	//	�ǂݍ��ݗp
		char	reader[9]	= "";

	//	�C�x���g�̎�ނ�ǂݍ���
		fscanf_s(this->eventFile, "%s ", reader, sizeof(reader));

	//	�C�x���g�̎�ނŕ���
		switch(this->judgeEventOrderToString(reader))
		{
		//	�R�����g����
 		case eEVORDER_TALK:			this->sgEventProcTalk();		break;
		//	�R�����g�J�n
		case eEVORDER_TALK_START:	this->sgEventProcTalkStart();	break;
		//	���̂̐���
		case eEVORDER_OBJ_CREATE:	this->sgEventProcObjCreate();	break;
		//	���̂̏�ԕω�
		case eEVORDER_OBJ_CHANGE:	this->sgEventProcObjChange();	break;
		//	���̈ړ�
		case eEVORDER_OBJ_MOVE:		this->sgEventProcObjMove();		break;
		//	���̏���
		case eEVORDER_OBJ_CLEAR:	this->sgEventProcObjClear();	break;
		//	�G�t�F�N�g�̐���
		case eEVORDER_EFF_CREATE:	this->sgEventProcEffCreate();	break;
		//	�G�t�F�N�g�̔���
		case eEVORDER_EFF_OCCU:		this->sgEventProcEffOccu();		break;
		//	�G�t�F�N�g�̏���
		case eEVORDER_EFF_CLEAR:	this->sgEventProcEffClear();	break;
		//	�_�~�[���͂̑���
		case eEVORDER_INPUT_OPE:	this->sgEventProcInputOpe();	break;
		//	���͑ҋ@�̑���
		case eEVORDER_INPUT_WAIT:	this->sgEventProcInputWait();	break;
		//	�ҋ@
		case eEVORDER_EXECUTE:		this->sgEventProcExecute();		break;
		//	�Q�[���N���A
		case eEVORDER_GAME_CLEAR:	this->sgEventProcGameClear();	break;
		//	�C�x���g�I��
		case eEVORDER_EVENT_QUIT:	this->sgEventProcEventQuit();	break;
		//	��L�ȊO
		default:	this->eventStep	= eEVSTEP_ERROR;				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventStepExecute
	//
	//	�@�\		�C�x���g���s
	//	�X�V		2008/11/11		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventStepExecute(void)
	{
	//	�J�E���g���L��΁A�ҋ@
		if(	this->eventWaitCnt > 0)
		{
		//	�R�����g���s
			this->eventCommentG->executeComment(FALSE);
			this->eventWaitCnt--;
		}
	//	������΁A�ҋ@�I��
		else
		{
		//	�R�����g��������
			this->eventCommentG->executeComment(TRUE);
		//	�ҋ@����2D�`�悳��镨�̂̉��
			this->effectCenter->releaseEffects2D();
			this->eventStep = eEVSTEP_READ;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventStepTalk
	//
	//	�@�\		�C�x���g��b��
	//	����		input			���͏��
	//	�X�V		2008/11/11		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventStepTalk(CINPUT* input)
	{
	//	���̗͂L�����`�F�b�N
		BOOL	bInput	= FALSE;
		if(	input->checkPressOnce(MYKEY::eSTART)	||
			input->checkPressOnce(MYKEY::eSOFT)		||
			input->checkPressOnce(MYKEY::eMIDDLE)	||
			input->checkPressOnce(MYKEY::eHARD)		||
			input->checkPressOnce(MYKEY::eDUST))
		{
			bInput	= TRUE;
		}

	//	�R�����g���s���A�R�����g�f�[�^�������Ȃ�΁A�ǂݍ��ݏ����ɖ߂�
		if(	!this->eventCommentG->executeComment(bInput))
		{
		//	�ҋ@����2D�`�悳��镨�̂̉��
			this->effectCenter->releaseEffects2D();
			this->eventStep	= eEVSTEP_READ;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventStepInputWait
	//
	//	�@�\		�C�x���g���͑ҋ@��
	//	����		input			���͏��
	//	�X�V		2008/11/11		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventStepInputWait(CINPUT* input)
	{
	//	���̗͂L�����`�F�b�N
		BOOL	bInput	= FALSE;
		if(	input->checkPressOnce(this->eventWaitKey))
		{
		//	�ҋ@����2D�`�悳��镨�̂̉��
			this->effectCenter->releaseEffects2D();
			this->eventStep	= eEVSTEP_READ;
			bInput			= TRUE;
		}

	//	�R�����g���s
		this->eventCommentG->executeComment(bInput);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventExecuter
	//
	//	�@�\		�C�x���g���쏈���̎��s
	//	�X�V		2008/11/11		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventExecuter(void)
	{
	//	���s���C�x���g�̏���
		VEC_WHILE(CEXECUTER*, eventExecuter)
		{
		//	�C�x���g�����s���A�I���̍��}���o����A�C�x���g��j������
			if(	!(*p)->execute())
			{
				SAFE_DELETE(*p);
				p	= this->eventExecuter.erase(p);
			}
		//	�I���̍��}���o�ĂȂ��Ȃ�A�C�e���[�^��i�߂�
			else
			{
				p++;
			}
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventProcTalk
	//
	//	�@�\		�R�����g����
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcTalk(void)
	{
	//	�R�����g�ǂݍ���
		CCOMMENT*	newComment				= NULL;

		char		commentReader[MAX_PATH]	= "";
		VEC2		loc						= VEC2(0.0f, 0.0f);
		int			iPtn					= 0;
		int			iReversal				= 0;
		int			iLine					= 0;
		char		soundReader[MAX_PATH]	= "";
		int			r=0, g=0, b=0, a=0;

	//	�f�[�^��ǂݍ���
		fscanf_s(this->eventFile, "%f, %f, ",	&loc.x, &loc.y);
		fscanf_s(this->eventFile, "%d, ",		&iPtn);
		fscanf_s(this->eventFile, "%d, ",		&iReversal);
		fscanf_s(this->eventFile, "%d, ",		&iLine);
		fscanf_s(this->eventFile, "%s  ",		soundReader, sizeof(soundReader));

	//	�ǂݍ��񂾃T�E���h�����`�F�b�N
		if(	!strcmp(soundReader, READ_SOUND_NONE))
		{
		//	�_�~�[�̃T�E���h����������ANULL������
			strcpy_s(soundReader, sizeof(soundReader), "");
		}

	//	�R�����g����
		newComment	=
			new CCOMMENT(this->gDev->getDevice(),
						 iPtn,
						 BOOL(iReversal),
						 &loc,
						 1000,
						 soundReader);

	//	������ǂݍ���
		for(int i=0; i<iLine; i++)
		{
			fscanf_s(this->eventFile, "%s ", commentReader, sizeof(commentReader));
			fscanf_s(this->eventFile, "%d, %d, %d, %d, ", &r, &g, &b, &a);
			newComment->setString(commentReader, i, D3DCOLOR_RGBA(r,g,b,a));
		}

	//	�R�����g�̓o�^
		this->eventCommentG->setComment(newComment);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventProcTalkStart
	//
	//	�@�\		�R�����g�J�n
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcTalkStart(void)
	{
	//	��b���ɕύX
		this->eventStep	= eEVSTEP_TALK;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventProcObjCreate
	//
	//	�@�\		���̂̐���
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcObjCreate(void)
	{
	//	���̐����Ώۓǂݍ���
		C3D_OBJECT*	newObject				= NULL;

		int			iEntry					= 0;
		char		objReader[9]			= "";
		char		nameReader[MAX_PATH]	= "";
		char		insertReader[9]			= "";
		VEC3		pos						= VEC3(0.0f, 0.0f, 0.0f);
		VEC3		angle					= VEC3(0.0f, 0.0f, 0.0f);
		VEC2		size					= VEC2(0.0f, 0.0f);
		int			r=0, g=0, b=0, a=0;
		MY_COLOR_RGBA	color;

	//	�f�[�^��ǂݍ���
		fscanf_s(this->eventFile, "%s ",	objReader,  sizeof(objReader));
		fscanf_s(this->eventFile, "%s ",	nameReader, sizeof(nameReader));
		fscanf_s(this->eventFile, "%d, ",	&iEntry);
		fscanf_s(this->eventFile, "%f, %f, %f, %f, %f, %f, %f, %f, ",
			&pos.x,   &pos.y,   &pos.z,
			&angle.x, &angle.y, &angle.z,
			&size.x,  &size.y);
		fscanf_s(this->eventFile, "%s ",	insertReader, sizeof(insertReader));

	//	���̐����Ώۂ̎�ނŕ���
		switch(this->judgeEventCreateObjToString(objReader))
		{
		//	�e�N�X�`��
			case	eEVCRE_TEXTURE:
				fscanf_s(this->eventFile, "%d, %d, %d, %d, ", &r, &g, &b, &a);
				color.rgba.red		= (BYTE)r;
				color.rgba.green	= (BYTE)g;
				color.rgba.blue		= (BYTE)b;
				color.rgba.alpha	= (BYTE)a;
				newObject	=
					new CTEXTURE(gDev->getDevice(), nameReader, size, color.color);
				newObject->setLoc(&pos);
				newObject->setAngle(&angle);
				break;
		//	���b�V��
			case	eEVCRE_MESH:
				newObject	=
					new CMESH(gDev->getDevice(), nameReader);
				newObject->setLoc(&pos);
				newObject->setAngle(&angle);
				newObject->setScale(size.x);
				break;
		//	�X�L�����b�V��
			case	eEVCRE_SKINMESH:
				newObject	=
					new CSKIN_MESH(gDev->getDevice(), nameReader);
				newObject->setLoc(&pos);
				newObject->setAngle(&angle);
				newObject->setScale(size.x);
				break;
		//	�G�t�F�N�g
			case	eEVCRE_EFFECT:
				newObject	=
					new C3D_EFFECT(gDev->getDevice(), nameReader, pos);
				newObject->setAngle(&angle);
				newObject->setScale(size.x);
				break;
		//	��L�ȊO
			default:
				break;
		}

	//	�I�u�W�F�N�g�o�^��̎�ނŕ���
		switch(this->judgeEventInsertToString(insertReader))
		{
		//	�Q�[���}�b�v
			case	eEVINS_GAMEMAP:
			//	�S�I�u�W�F�N�g�́A���ł��}�b�v��ŏ����o����悤�ɂ���
				newObject->setOrderMessage(eOMSG_DELETE_IN);
			//	�C�x���g�}�b�v�ɓo�^
				this->eventObject.insert(pair<int, C3D_OBJECT*>(iEntry, newObject));
			//	�S�̃}�b�v�ɓo�^
				this->gameMap->addViewG(newObject);
				break;
		//	�ҋ@����2D�`��
			case	eEVINS_WAIT_2D:
				this->effectCenter->addEffects(nameReader, newObject);
				this->effectCenter->occu2DEffects(nameReader, pos, size.x, angle);
				break;
		//	��L�ȊO
			default:
				SAFE_DELETE(newObject);
				return;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventProcObjChange
	//
	//	�@�\		���̂̏�ԕω�
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcObjChange(void)
	{
	//	��ԕω��Ώۓǂݍ���
		char	objReader[9]	= "";
		char	chaReader[9]	= "";
		int		iNum			= 0;

	//	�f�[�^��ǂݍ���
		fscanf_s(this->eventFile, "%s ",	objReader, sizeof(objReader));
		fscanf_s(this->eventFile, "%s ",	chaReader, sizeof(chaReader));

	//	��ԕω��Ώۂ̎�ނŕ���
		switch(this->judgeEventObjectToString(objReader))
		{
		//	���̈ړ�
			case	eEVOBJ_PLAYER:
			//	��ԕω���ނŕ���
				switch(this->judgeEventChangeToString(chaReader))
				{
				//	�\���m�F
					case	eEVCHAN_VISIBLE:
						fscanf_s(this->eventFile, "%d ",	&iNum);
						this->player->setAllVisible((!iNum ? FALSE : TRUE));
						break;
				//	�A�j���[�V�����ύX
					case	eEVCHAN_ANIMCHANGE:
						fscanf_s(this->eventFile, "%d ",	&iNum);
						this->playerChara->changeAnimationAndBlend(iNum);
						break;
				//	���͗L������ύX
					case	eEVCHAN_LIMIT:
						fscanf_s(this->eventFile, "%d ",	&iNum);
						this->player->setAllLimit(iNum);
						break;
				//	�J�Q�I���萧��ύX
					case	eEVCHAN_LIMIT_KAGEO:
						fscanf_s(this->eventFile, "%d ",	&iNum);
						this->player->getKageo()->setLimit((ADVENT_KAGEO_ACTION_LIMIT)iNum);
						break;
				//	�J�Q�~���萧��ύX
					case	eEVCHAN_LIMIT_KAGEMI:
						fscanf_s(this->eventFile, "%d ",	&iNum);
						this->player->getKagemi()->setLimit((ADVENT_KAGEMI_ACTION_LIMIT)iNum);
						break;
				//	�L�����ύX�L���m�F
					case	eEVCHAN_CHARACHANGE_BOOLEN:
						fscanf_s(this->eventFile, "%d ",	&iNum);
						this->player->setChoiceCharaChangeBoolen((BOOL)iNum);
						break;
				//	�J�Q�I�ɃL�����ύX
					case	eEVCHAN_CHARACHANGE_KAGEO:
						this->player->setChoiceCharacter(eADCC_KAGEO);
						break;
				//	�J�Q�~�ɃL�����ύX
					case	eEVCHAN_CHARACHANGE_KAGEMI:
						this->player->setChoiceCharacter(eADCC_KAGEMI);
						break;
				}
				break;
		//	�J����
			case	eEVOBJ_CAMERA:
			//	��ԕω���ނŕ���
				switch(this->judgeEventChangeToString(chaReader))
				{
				//	���_���t�ړ����[�h
					case	eEVCHAN_PARALLEL:
						this->camera->setCameraMode(eCAMO_PARALLEL);
						break;
				//	�J�������t�ړ����[�h
					case	eEVCHAN_PARACHASE://
						this->camera->setCameraMode(eCAMO_PARALLEL_CHASE);
						break;
				//	��d�ǐՃ��[�h
					case	eEVCHAN_DOUBLETARGET://
						this->camera->setCameraMode(eCAMO_DOUBLE_TARGET);
						break;
				}
				break;
		//	�Q�[���N���A
			case	eEVOBJ_EVOBJECT:
				break;
		//	��L�ȊO
			default:
				break;
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventProcObjMove
	//
	//	�@�\		���̈ړ�
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcObjMove(void)
	{
	//	�ړ��Ώۓǂݍ���
		char	reader[9]	= "";
		int		inFrame		= 0;
		VEC3	inEndPoint(0.0f,0.0f,0.0f);

	//	�f�[�^��ǂݍ���
		fscanf_s(this->eventFile, "%s ",	reader, sizeof(reader));
		fscanf_s(this->eventFile, "%d, ",	&inFrame);
		fscanf_s(this->eventFile, "%f, %f, %f, ",
			&inEndPoint.x, &inEndPoint.y, &inEndPoint.z);

	//	�ړ��Ώۊi�[��
		C3D_OBJECT*	inTarget	= NULL;
	//	�ړ��Ώۂ̎�ނŕ���
		switch(this->judgeEventObjectToString(reader))
		{
		//	���̈ړ�
			case	eEVOBJ_PLAYER:
				inTarget	= this->playerChara;
				break;
		//	�J����
			case	eEVOBJ_CAMERA:
				inTarget	= this->camera;
				break;
		//	�Q�[���N���A
			case	eEVOBJ_EVOBJECT:
				break;
		//	��L�ȊO
			default:
				this->eventStep	= eEVSTEP_ERROR;
				return;
		}
	//	3D�I�u�W�F�N�g�𓮂����C�x���g
		CEXECUTER*	newExecuter
			= new CEXE_MOVE_OBJECT(inTarget, inFrame, inEndPoint);
	//	�C�x���g���쏈���O���[�v�ɓo�^
		this->eventExecuter.push_back(newExecuter);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventProcObjClear
	//
	//	�@�\		���̏���
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcObjClear(void)
	{
		int		iEntry	= 0;
	//	�ǂݍ���
		fscanf_s(this->eventFile, "%d, ", &iEntry);
	//	���̃}�b�v����A�f�[�^�����o��
		map<int, C3D_OBJECT*>::iterator	p;
		p	= this->eventObject.find(iEntry);

	//	���o�����f�[�^���L���Ȃ�A�������߂��o��
		if(	p	!= this->eventObject.end())
		{
			(*p).second->setOrderMessage(eOMSG_DELETE);
			this->eventObject.erase(p);
		}
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventProcEffCreate
	//
	//	�@�\		�G�t�F�N�g�̐���
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcEffCreate(void)
	{
		char	nameReader[MAX_PATH]	= "";
	//	�ǂݍ���
		fscanf_s(this->eventFile, "%s ",	nameReader, sizeof(nameReader));
	//	�G�t�F�N�g�̐���
		C3D_EFFECT_CENTER::addEffectsInstance(
			nameReader,
			new C3D_EFFECT(this->gDev->getDevice(), nameReader, VEC3(0.0f,0.0f,0.0f)));
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventProcEffOccu
	//
	//	�@�\		�G�t�F�N�g�̔���
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcEffOccu(void)
	{
		char	nameReader[MAX_PATH]	= "";
		VEC3	pos						= VEC3(0.0f, 0.0f, 0.0f);
		float	scale					= 1.0f;
	//	�ǂݍ���
		fscanf_s(this->eventFile, "%s ",		  nameReader, sizeof(nameReader));
		fscanf_s(this->eventFile, "%f, %f, %f, ", &pos.x,   &pos.y,   &pos.z);
		fscanf_s(this->eventFile, "%f, ",		  &scale);
	//	�G�t�F�N�g�̔���
		C3D_EFFECT_CENTER::occuEffectsInstance(nameReader, pos, scale);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventProcEffClear
	//
	//	�@�\		�G�t�F�N�g�̏���
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcEffClear(void)
	{
		char	nameReader[MAX_PATH]	= "";
	//	�ǂݍ���
		fscanf_s(this->eventFile, "%s ", nameReader, sizeof(nameReader));
	//	�G�t�F�N�g�̐���
		C3D_EFFECT_CENTER::releaseEffectInstance(nameReader);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventProcInputOpe
	//
	//	�@�\		�_�~�[���͂̑���
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcInputOpe(void)
	{
		int		inKey		= 0;
		int		inFrame		= 0;
	//	�ǂݍ���
		fscanf_s(this->eventFile, "%d, ", &inFrame);
		fscanf_s(this->eventFile, "%d, ", &inKey);

	//	�_�~�[���͏��𑀍삷��
		CEXECUTER*	newExecuter
			= new CEXE_DUMMY_INPUT(this->eventInput, inFrame, inKey);

		this->eventExecuter.push_back(newExecuter);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventProcInputWait
	//
	//	�@�\		���͑ҋ@�ڍs
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcInputWait(void)
	{
		int		inKey		= 0;
	//	�ǂݍ���
		fscanf_s(this->eventFile, "%d, ", &inKey);
	//	�ҋ@����L�[
		this->eventWaitKey	= (MYKEY::TYPENAME)inKey;
	//	���͑ҋ@���ɕύX
		this->eventStep		= eEVSTEP_INPUTWAIT;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventProcExecute
	//
	//	�@�\		�ҋ@�ڍs
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcExecute(void)
	{
	//	�ǂݍ���
		fscanf_s(this->eventFile, "%d, ", &this->eventWaitCnt);
	//	���s�ֈڂ�
		this->eventStep		= eEVSTEP_EXECUTE;
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventProcGameClear
	//
	//	�@�\		�Q�[���N���A�ڍs
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcGameClear(void)
	{
	//	�C�x���g�I��
		this->eventStep	= eEVSTEP_FALSE;
	//	�Q�[���N���A�̏�Ԃɂ���
		this->sgClearChange();
	//	�C�x���g���쏈���̉��
		this->releaseEventExecuter();
	//	�R�����g�O���[�v�̒��g���J��
		this->eventCommentG->releaseComment();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventProcEventQuit
	//
	//	�@�\		�C�x���g�I��
	//	�X�V		2008/11/10		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventProcEventQuit(void)
	{
	//	�C�x���g�I��
		this->eventStep	= eEVSTEP_FALSE;
	//	�Q�[���v���C�̏�Ԃɂ���
		this->sgPlayChange();
	//	�C�x���g���쏈���̉��
		this->releaseEventExecuter();
	//	�R�����g�O���[�v�̒��g���J��
		this->eventCommentG->releaseComment();
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventExecute
	//
	//	�@�\		�Q�[���C�x���g������
	//	����		inInput			���͏��
	//	�X�V		2008/11/07		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventExecute(CINPUT* inInput)
	{
	//	�C�x���g���s
		this->sgEventProc(inInput);;
	//	�v���C���[�̑���
		this->player->operation(this->eventInput);
	//	�v���C���[�L�����N�^�[�̑���
		this->playerChara->operation(this->eventInput);
	}

	//================================================================================
	//!	���\�b�h��	CSCENE_ADVENTURE_GAME::sgEventRender
	//
	//	�@�\		�Q�[���C�x���g���`��
	//	�X�V		2008/11/07		<�V�K>
	//================================================================================
	void
	CSCENE_ADVENTURE_GAME::sgEventRender(void)
	{
	//	�f�o�C�X���m��
		DEV	device	= this->gDev->getDevice();
	//	�`��J�n
		this->gDev->renderBegin();

	//	�J�����̍X�V
		this->camera->updateCamera();
	//	���C�g�̍X�V
		this->sgUpdateLight();

	//-------------------------------------------------
	//	3D���̂̕`��
	//-------------------------------------------------
	//	Z�e�N�X�`���V�F�[�_�Ńe�N�X�`���ɐ[�x�l������������
		this->shaderZTexture->begin();
		this->gameMap->squeezeSortByCameraPositionWithDrawZTexSha(
			this->camera, &this->camera->getViewPointFinish(), this->shaderZTexture);
		this->shaderZTexture->end();

		this->gDev->bufferClear(D3DCOLOR_RGBA(255,255,255,255));

	//	�J�����Ńf�o�C�X�X�V
		this->camera->viewTransform(device);

	//	�w�i�̕`��
		this->back->motion(&this->camera->getViewPointFinish());
		this->back->drawBack(device, this->gDev->getSprite());

	//	�[�x�o�b�t�@�V�F�[�_�ŕ`��
		this->shaderDepthBuf->setCameraViewProjMatrix(
			&((*this->camera->getMatrixView()) * (*this->camera->getMatrixProjection())));

		this->playerChara->draw(device);
		this->gameMap->drawEnemyG(device, &this->camera->getViewPointFinish());

	//	���C�e�B���O�𖳌�
		this->shaderDepthBuf->begin();
		this->gameMap->squeezeDrawWithDepthSha(this->shaderDepthBuf);
		this->shaderDepthBuf->end();

		this->effectCenter->drawEffects(device, this->camera);

	//-------------------------------------------------
	//	2D���̂̕`��
	//-------------------------------------------------
		this->gDev->spriteBegin();
		this->eventCommentG->drawComment(this->gDev->getSprite());
		this->effectCenter->drawEffects2D(this->gDev->getSprite());
		this->gDev->spriteEnd();

	//	���̕`��
		this->gDev->renderCurtainInDev();
	//	�`��I��
		this->gDev->renderEnd();
	}
}
//-----------------------------------------------------------------------
//!	�V�[���̖��O���
//-----------------------------------------------------------------------