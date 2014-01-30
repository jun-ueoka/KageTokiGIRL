/*	CInput.h
 *
 *	
 *
 *	CKeyConfig	���͊Ǘ��N���X
 *
 *
 */
#include "DiClassEx.h"

/*	�񋓑�	*/
//�L�[������
typedef enum PLAYKEYINFO
{
	ePLAYKEY_NONE = -1,					//����������Ă��Ȃ�
	ePLAYKEY_UP,						//��
	ePLAYKEY_DOWN,						//��
	ePLAYKEY_LEFT,						//��
	ePLAYKEY_RIGHT,						//�E
	ePLAYKEY_A,							//A
	ePLAYKEY_B,							//B
	ePLAYKEY_C,							//C
	ePLAYKEY_D,							//D
	ePLAYKEY_L1,						//L1
	ePLAYKEY_L2,						//L2
	ePLAYKEY_R1,						//R1
	ePLAYKEY_R2,						//R2
	ePLAYKEY_UPLEFT,					//����
	ePLAYKEY_UPRIGHT,					//�E��
	ePLAYKEY_DOWNLEFT,					//����
	ePLAYKEY_DOWNRIGHT,					//�E��
	//�ȉ��g��Ȃ����͍̂폜�\��
	ePLAYKEY_AB,						//A+B
	ePLAYKEY_AC,						//A+C
	ePLAYKEY_AD,						//A+D
	ePLAYKEY_BC,						//B+C
	ePLAYKEY_BD,						//B+D
	ePLAYKEY_CD,						//C+D
	ePLAYKEY_ABC,						//A+B+C
	ePLAYKEY_ABD,						//A+B+D
	ePLAYKEY_ACD,						//A+C+D
	ePLAYKEY_BCD,						//B+C+D
	//�����܂�
	ePLAYKEY_ALLBUTTON,					//A+B+C+D
	eMAX_PLAYKEY,						//�L�[�̍ő吔
};


/*	�\����	*/

	//��`�Ȃ�

/*	�N���X	*/
/*�N���X��		�FCInput
 *�@�\			�F�W���C�X�e�B�b�N�̃L�[�R���g���[���̐�����s��
 *�C���X�^���X	�FctrlID				�R���g���[���̔ԍ�
				  handle				�E�B���h�E�n���h��
 *���\�b�h		�FCInput				�R���X�g���N�^
				  initialize			����������
				  ~CInput				�f�X�g���N�^
				  finalize				�I���O����
				  getPressedCrossKey	�\���L�[�̃`�F�b�N
				  getPressedButton		�{�^���L�[�̃`�F�b�N
				  checkPadState			�p�b�h�̓��̓`�F�b�N
 */
class CInput
{
	private:
		CTRLID	ctrlID;
		HWND	handle;
		void initialize(HWND inWnd, CTRLID inID);
		void finalize();
	protected:
	public:
		//�R���X�g���N�^
		CInput(HWND inWnd, CTRLID inCtrlID)		{ initialize(inWnd, inCtrlID); }
		//�f�X�g���N�^
		~CInput()					{ }
		//���̓L�[�`�F�b�N
		PLAYKEYINFO getPressedCrossKey(DiClassEx* device);
		PLAYKEYINFO getPressedButton(DiClassEx* device);
		//�擾�����L�[�̕ϊ�
		PLAYKEYINFO checkPadState(DiClassEx* device);
};
