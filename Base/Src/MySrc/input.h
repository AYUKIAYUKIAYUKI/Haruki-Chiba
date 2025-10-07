//============================================================================
// 
// 入力処理管理、ヘッダファイル [input.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _INPUT_H_
#define _INPUT_H_	// 二重インクルード防止

//****************************************************
// マクロ定義
//****************************************************
#define MAX_KEY	256	// キー最大数

//****************************************************
// インプットクラス
//****************************************************
class CInput
{
public:

	// <special function>
	CInput();	// コンストラクタ
	 ~CInput();	// デストラクタ

	// <function>
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期設定
	virtual void	Uninit();								// 終了処理
	virtual void	Update() = 0;							// 更新処理

protected:

	// <data>
	LPDIRECTINPUTDEVICE8 m_pDevice;	// 入力デバイス情報

	// <static data>
	static LPDIRECTINPUT8 m_Input;	// DirectInputオブジェクト
};

#endif // _INPUT_H_