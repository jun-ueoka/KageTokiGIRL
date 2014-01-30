/*	CInput.h
 *
 *	
 *
 *	CKeyConfig	入力管理クラス
 *
 *
 */
#include "DiClassEx.h"

/*	列挙体	*/
//キー操作情報
typedef enum PLAYKEYINFO
{
	ePLAYKEY_NONE = -1,					//何も押されていない
	ePLAYKEY_UP,						//上
	ePLAYKEY_DOWN,						//下
	ePLAYKEY_LEFT,						//左
	ePLAYKEY_RIGHT,						//右
	ePLAYKEY_A,							//A
	ePLAYKEY_B,							//B
	ePLAYKEY_C,							//C
	ePLAYKEY_D,							//D
	ePLAYKEY_L1,						//L1
	ePLAYKEY_L2,						//L2
	ePLAYKEY_R1,						//R1
	ePLAYKEY_R2,						//R2
	ePLAYKEY_UPLEFT,					//左上
	ePLAYKEY_UPRIGHT,					//右上
	ePLAYKEY_DOWNLEFT,					//左下
	ePLAYKEY_DOWNRIGHT,					//右下
	//以下使わないものは削除予定
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
	//ここまで
	ePLAYKEY_ALLBUTTON,					//A+B+C+D
	eMAX_PLAYKEY,						//キーの最大数
};


/*	構造体	*/

	//定義なし

/*	クラス	*/
/*クラス名		：CInput
 *機能			：ジョイスティックのキーコントロールの制御を行う
 *インスタンス	：ctrlID				コントローラの番号
				  handle				ウィンドウハンドル
 *メソッド		：CInput				コンストラクタ
				  initialize			初期化処理
				  ~CInput				デストラクタ
				  finalize				終了前処理
				  getPressedCrossKey	十字キーのチェック
				  getPressedButton		ボタンキーのチェック
				  checkPadState			パッドの入力チェック
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
		//コンストラクタ
		CInput(HWND inWnd, CTRLID inCtrlID)		{ initialize(inWnd, inCtrlID); }
		//デストラクタ
		~CInput()					{ }
		//入力キーチェック
		PLAYKEYINFO getPressedCrossKey(DiClassEx* device);
		PLAYKEYINFO getPressedButton(DiClassEx* device);
		//取得したキーの変換
		PLAYKEYINFO checkPadState(DiClassEx* device);
};
