//============================================================================
// 
// マウス入力 [input_mouse.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "input_mouse.h"

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CInputMouse::CInputMouse() :
	m_State{},
	m_StateTrigger{},
	m_StateRelease{}
{

}

//============================================================================
// デストラクタ
//============================================================================
CInputMouse::~CInputMouse()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_Input == nullptr)
	{
		// インプットクラスの初期設定
		if (FAILED(CInput::Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
	}

	// 入力デバイス(マウス)の生成
	if (FAILED(m_Input->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットの設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// 協調モードを設定：フォアグラウンド
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// マウスへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CInputMouse::Uninit()
{
	// 入力デバイス(マウス)の破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

//============================================================================
// 更新処理
//============================================================================
void CInputMouse::Update()
{
	DIMOUSESTATE2 StateMouse = {};	// マウス入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &StateMouse)))
	{
		for (WORD wCntKey = 0; wCntKey < 8; ++wCntKey)
		{
			// マウスのリリース情報を保存
			m_StateRelease.rgbButtons[wCntKey] = m_State.rgbButtons[wCntKey] & ~StateMouse.rgbButtons[wCntKey];

			// マウスのトリガー情報を保存
			m_StateTrigger.rgbButtons[wCntKey] = ~m_State.rgbButtons[wCntKey] & StateMouse.rgbButtons[wCntKey];

			// マウスのプレス情報を保存
			m_State.rgbButtons[wCntKey] = StateMouse.rgbButtons[wCntKey];
		}

		// マウスの移動情報を保存
		m_State = StateMouse;
	}
	else
	{
		// マウスのアクセス権を獲得
		m_pDevice->Acquire();
	}
}

//============================================================================
// プレス情報取得
//============================================================================
bool CInputMouse::GetPress(int nKey)
{
	return ((m_State.rgbButtons[nKey] & 0x80) != 0) ? true : false;
}

//============================================================================
// トリガー情報取得
//============================================================================
bool CInputMouse::GetTrigger(int nKey)
{
	return ((m_StateTrigger.rgbButtons[nKey] & 0x80) != 0) ? true : false;
}

//============================================================================
// リリース情報取得
//============================================================================
bool CInputMouse::GetRelease(int nKey)
{
	return ((m_StateRelease.rgbButtons[nKey] & 0x80) != 0) ? true : false;
}

//============================================================================
// マウスの移動量を取得
//============================================================================
D3DXVECTOR2 CInputMouse::GetMouseMove()
{
	return { static_cast<float>(m_State.lX), static_cast<float>(m_State.lY) };
}

//============================================================================
// ホイールのスクロール量を取得
//============================================================================
float CInputMouse::GetWheelScroll()
{
	return static_cast<float>(m_State.lZ);
}