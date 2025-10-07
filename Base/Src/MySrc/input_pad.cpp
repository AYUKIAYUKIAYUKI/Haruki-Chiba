//============================================================================
// 
// パッド入力 [input_pad.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "input_pad.h"

//****************************************************
// プリプロセッサディレクティブ
//****************************************************
#define LEFT_JOYSTICK_CONVERT 0	// 左スティック入力を変換

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CInputPad::CInputPad() :
	m_aKeyState{},
	m_aKeyStateTrigger{},
	m_aKeyStateRelease{}
{

}

//============================================================================
// デストラクタ
//============================================================================
CInputPad::~CInputPad()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CInputPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_Input == nullptr)
	{
		// インプットクラスの初期設定
		if (FAILED(CInput::Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
	}

	// プレス情報クリア
	memset(&m_aKeyState, 0, sizeof(XINPUT_STATE));

	// トリガー情報クリア
	memset(&m_aKeyStateTrigger, 0, sizeof(XINPUT_STATE));

	// リリース情報クリア
	memset(&m_aKeyStateRelease, 0, sizeof(XINPUT_STATE));

	// XInputのステート設定を有効にする
	XInputEnable(true);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CInputPad::Uninit()
{
	// XInputのステート設定を無効にする
	XInputEnable(false);

	// インプットクラスの終了処理
	CInput::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CInputPad::Update()
{
	XINPUT_STATE StateJoypad = {};	// パッドの入力情報

	// パッドの状態を取得
	if (XInputGetState(0, &StateJoypad) == ERROR_SUCCESS)
	{
#if LEFT_JOYSTICK_CONVERT	// 左スティック入力を変換
		// 左スティック入力を十字キーに変換
		g_state_joypad.press.Gamepad.wButtons |= ConvertJoyStick();
#else
		// スティック入力の調整
		AdjustJoyStick();
#endif	// LEFT_JOYSTICK_CONVERT

		// パッドのリリース情報を保存
		m_aKeyStateRelease.Gamepad.wButtons = m_aKeyState.Gamepad.wButtons & ~StateJoypad.Gamepad.wButtons;

		// パッドのトリガー情報を保存
		m_aKeyStateTrigger.Gamepad.wButtons = ~m_aKeyState.Gamepad.wButtons & StateJoypad.Gamepad.wButtons;
		m_aKeyStateTrigger.Gamepad.bRightTrigger = ~m_aKeyState.Gamepad.bRightTrigger & StateJoypad.Gamepad.bRightTrigger;
		
#if 0 // パッドのプレス情報を保存
		// ボタンのみ
		m_aKeyState.Gamepad.wButtons = StateJoypad.Gamepad.wButtons;
#else
		// ジョイスティック・トリガーボタンを含む
		m_aKeyState.Gamepad = StateJoypad.Gamepad;
#endif
	}
}

//============================================================================
// プレス情報取得
//============================================================================
bool CInputPad::GetPress(JOYKEY Key)
{
	return (m_aKeyState.Gamepad.wButtons & (0x01 << static_cast<int>(Key))) ? true : false;
}

//============================================================================
// トリガー情報取得
//============================================================================
bool CInputPad::GetTrigger(JOYKEY Key)
{
	return (m_aKeyStateTrigger.Gamepad.wButtons & (0x01 << static_cast<int>(Key))) ? true : false;
}

//============================================================================
// リリース情報取得
//============================================================================
bool CInputPad::GetRelease(JOYKEY Key)
{
	return (m_aKeyStateRelease.Gamepad.wButtons & (0x01 << static_cast<int>(Key))) ? true : false;
}

//============================================================
// 右トリガーボタンの入力を取得
//============================================================
bool CInputPad::GetDepthTriggerR()
{
	return (m_aKeyStateTrigger.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

//============================================================================
// 左スティックの傾き取得
//============================================================================
CInputPad::JOYSTICK CInputPad::GetJoyStickL(void)
{
	// スティックの傾きを格納
	JOYSTICK JoystickL =
	{
		m_aKeyState.Gamepad.sThumbLX,
		m_aKeyState.Gamepad.sThumbLY
	};

	return JoystickL;
}

//============================================================================
// 右スティックの傾き取得
//============================================================================
CInputPad::JOYSTICK CInputPad::GetJoyStickR(void)
{
	// スティックの傾きを格納
	JOYSTICK JoystickR =
	{
		m_aKeyState.Gamepad.sThumbRX,
		m_aKeyState.Gamepad.sThumbRY
	};

	return JoystickR;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// スティック入力の調整
//============================================================================
void CInputPad::AdjustJoyStick(void)
{
	// 左
	if (m_aKeyState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		m_aKeyState.Gamepad.sThumbLX = 0;
		m_aKeyState.Gamepad.sThumbLY = 0;
	}

	// 右
	if (m_aKeyState.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbRY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		m_aKeyState.Gamepad.sThumbRX = 0;
		m_aKeyState.Gamepad.sThumbRY = 0;
	}
}