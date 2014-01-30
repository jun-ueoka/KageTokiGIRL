//***********************************************************************************
//!	�t�@�C����		Class_GameMap.h
//
//	�@�\			���̃Q�[����p��3D�}�b�v�̒�`
//	�L�q			�㉪�@��
//	�v���W�F�N�g	�u�v
//	�X�V			2008/07/25	<�V�K>
//***********************************************************************************

#pragma	once

//-----------------------------------------------------------------------
//!	�C���^�[�t�F�C�X�w�b�_�[
//-----------------------------------------------------------------------
#include	"Class_3DMap.h"					//!< 3D�I�u�W�F�N�g�}�b�v
#include	"Class_Mesh.h"					//!< 3D�I�u�W�F�N�g���b�V��
#include	"Class_SkinMesh.h"				//!< 3D�I�u�W�F�N�g�X�L�����b�V��
#include	"Class_Texture.h"				//!< 3D�I�u�W�F�N�g�X�L�����b�V��

//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------
namespace O3D
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//!	�N���X��		CGAME_MAP
	//
	//	�@�\			���̃Q�[����p��3D�}�b�v
	//	�X�V			2008/07/25	<�V�K>
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class CGAME_MAP : public C3D_MAP
	{
	private:		//-------------------- private --------------------//

				#define	NUM_FALSE	-10000

	protected:		//-------------------- protected --------------------//

				C3D_OBJECT_GROUP	friendG;						//!< �����O���[�v
				C3D_OBJECT_GROUP	enemyG;							//!< �G�O���[�v

				float				fEnemyGDistance;				//!< �G�O���[�v�`��̈�

			//==================================================================
			//	�I�u�W�F�N�g����
			//==================================================================
		virtual	BOOL	createObject(CONST DEV	  inDev,
									 C3D_OBJECT** inObj,
									 OB_DATA*	  inData);

			//==================================================================
			//	���������I�u�W�F�N�g�̃O���[�v�o�^
			//==================================================================
		virtual	BOOL	addObject(C3D_OBJECT* inObject, int inGroupType);

			//==================================================================
			//	�I�u�W�F�N�g�̃��b�Z�[�W���`�F�b�N
			//==================================================================
		virtual	BOOL	checkMessage(vector<C3D_OBJECT*>::iterator* inObjP);

	public:			//-------------------- public --------------------//

				CGAME_MAP(	void);									//!< �R���X�g���N�^
				CGAME_MAP(	CONST DEV	 inDev,
							int			 inTxtNum);					//!< �R���X�g���N�^(�S������)
		virtual	~CGAME_MAP(void);									//!< �f�X�g���N�^

		virtual	void	finalize(void);								//!< ���

			//==================================================================
			//	access	�����O���[�v
			//==================================================================
				C3D_OBJECT_GROUP*	getFriendG(void)				{ return	&this->friendG;	}
				void				addFriendG(C3D_OBJECT* inObj)	{ this->friendG.add(inObj);	}

			//==================================================================
			//	access	�G�O���[�v
			//==================================================================
				C3D_OBJECT_GROUP*	getEnemyG(void)					{ return	&this->enemyG;	}
				void				addEnemyG(C3D_OBJECT* inObj)	{ this->enemyG.add(inObj);	}
			//	�G�O���[�v�̕`��
				void				drawEnemyG(CONST DEV inDev, VEC3* inViewPoint);
				float	getEnemyGDistance(void)const			{ return	fEnemyGDistance;	}
				void	setEnemyGDistance(float inDist)			{ fEnemyGDistance	= inDist;	}
	};
}
//-----------------------------------------------------------------------
//!	3D�I�u�W�F�N�g�̖��O���
//-----------------------------------------------------------------------