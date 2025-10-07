//============================================================================
// 
// パッド入力、ヘッダファイル [input_pad.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _INPUT_PAD_H_
#define _INPUT_PAD_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "input.h"

//****************************************************
// パッドクラス
//****************************************************
class CInputPad : public CInput
{
public:

	//****************************************************
	// パッドのキー識別
	//****************************************************
	enum class JOYKEY
	{
		UP = 0,	// 十字上
		DOWN,	// 十字下
		LEFT,	// 十字左
		RIGHT,	// 十字右
		START,	// スタートボタン
		BACK,	// バックボタン
		L3,		// 左スティック押し込み
		R3,		// 右スティック押し込み
		L,		// Lボタン
		R,		// Rボタン
		NONE1,
		NONE2,
		A,		// Aボタン
		B,		// Bボタン
		X,		// Xボタン
		Y,		// Yボタン
		MAX,
	};

	//****************************************************
	// スティック情報の構造体
	//****************************************************
	struct JOYSTICK
	{
		SHORT X;	// X軸の傾き
		SHORT Y;	// Y軸の傾き
	};

	// <special function>
	CInputPad();	// コンストラクタ
	~CInputPad();	// デストラクタ

	// <function>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期設定
	void	Uninit() override;						// 終了処理
	void	Update() override;						// 更新処理

	// <data>
	bool		GetPress(JOYKEY Key);	// プレス情報取得
	bool		GetTrigger(JOYKEY Key);	// トリガー情報取得
	bool		GetRelease(JOYKEY Key);	// リリース情報取得
	bool		GetDepthTriggerR();		// 右トリガーボタンの取得
	JOYSTICK	GetJoyStickL();			// 左スティックの傾き取得
	JOYSTICK	GetJoyStickR();			// 右スティックの傾き取得

private:

	// <function>
	void AdjustJoyStick();	// スティックの調整

	// <data>
	XINPUT_STATE m_aKeyState;			// プレス情報
	XINPUT_STATE m_aKeyStateTrigger;	// トリガー情報
	XINPUT_STATE m_aKeyStateRelease;	// リリース情報
};

#endif	// _INPUT_PAD_H_