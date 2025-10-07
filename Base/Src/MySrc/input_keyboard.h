//============================================================================
// 
// キーボード入力、ヘッダファイル [input_keyboard.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _INPUT_KEYBOARD_H_
#define _INPUT_KEYBOARD_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "input.h"

//****************************************************
// キーボードクラス
//****************************************************
class CInputKeyboard : public CInput
{
public:

	// <special function>
	CInputKeyboard();	// コンストラクタ
	~CInputKeyboard();	// デストラクタ

	// <function>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;	// 初期設定
	void	Uninit() override;								// 終了処理
	void	Update() override;								// 更新処理

	// <getter>
	bool GetPress(int nKey);	// プレス情報取得
	bool GetTrigger(int nKey);	// トリガー情報取得
	bool GetRelease(int nKey);	// リリース情報取得

private:

	// <data>
	BYTE m_aKeyState[MAX_KEY];			// プレス情報
	BYTE m_aKeyStateTrigger[MAX_KEY];	// トリガー情報
	BYTE m_aKeyStateRelease[MAX_KEY];	// リリース情報
};

#endif	// _INPUT_KEYBOARD_