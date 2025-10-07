//============================================================================
// 
// キーボード入力 [input_keyboard.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "input_keyboard.h"

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CInputKeyboard::CInputKeyboard() :
	m_aKeyState{},
	m_aKeyStateTrigger{},
	m_aKeyStateRelease{}
{

}

//============================================================================
// デストラクタ
//============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_Input == nullptr)
	{
		// インプットクラスの初期設定
		if (FAILED(CInput::Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
	}

	// 入力デバイス(キーボード)の生成
	if (m_pDevice == nullptr)
	{
		if (FAILED(m_Input->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
		{
			return E_FAIL;
		}
	}

	// データフォーマットの設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定：フォアグラウンド
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CInputKeyboard::Uninit()
{
	// インプットクラスの終了処理
	CInput::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CInputKeyboard::Update()
{
	BYTE StateKeyboard[MAX_KEY] = {};	// キー入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(StateKeyboard), &StateKeyboard[0])))
	{
		for (WORD i = 0; i < MAX_KEY; ++i)
		{
			// キーボードのリリース情報を保存
			m_aKeyStateRelease[i] = m_aKeyState[i] & ~StateKeyboard[i];

			// キーボードのトリガー情報を保存
			m_aKeyStateTrigger[i] = ~m_aKeyState[i] & StateKeyboard[i];

			// キーボードのプレス情報を保存
			m_aKeyState[i] = StateKeyboard[i];
		}
	}
	else
	{
		// キーボードのアクセス権を獲得
		m_pDevice->Acquire();
	}
}

//============================================================================
// プレス情報取得
//============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return ((m_aKeyState[nKey] & 0x80) != 0) ? true : false;
}

//============================================================================
// トリガー情報取得
//============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return ((m_aKeyStateTrigger[nKey] & 0x80) != 0) ? true : false;
}

//============================================================================
// リリース情報取得
//============================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return ((m_aKeyStateRelease[nKey] & 0x80) != 0) ? true : false;
}