//============================================================================
// 
// マウス入力、ヘッダファイル [input_mouse.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _INPUT_MOUSE_H_
#define _INPUT_MOUSE_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "input.h"

//****************************************************
// マウスクラス
//****************************************************
class CInputMouse : public CInput
{
public:

	// <special function>
	CInputMouse();	// コンストラクタ
	~CInputMouse();	// デストラクタ

	// <function>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;	// 初期設定
	void	Uninit() override;								// 終了処理
	void	Update() override;								// 更新処理

	// <getter>
	bool		GetPress(int nKey);		// プレス情報取得
	bool		GetTrigger(int nKey);	// トリガー情報取得
	bool		GetRelease(int nKey);	// リリース情報取得
	D3DXVECTOR2	GetMouseMove();			// マウスの移動量を取得
	float		GetWheelScroll();		// ホイールのスクロール量を取得

private:

	// <data>
	DIMOUSESTATE2 m_State;			// プレス情報
	DIMOUSESTATE2 m_StateTrigger;	// トリガー情報
	DIMOUSESTATE2 m_StateRelease;	// リリース情報
};

#endif	// _INPUT_KEYBOARD_