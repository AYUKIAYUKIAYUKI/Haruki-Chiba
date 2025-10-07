//============================================================================
// 
// インプットマネージャー [input.manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "input.manager.h"

// ウィンドウハンドルの取得のため
#include "window.h"

//****************************************************
// 無名名前空間を定義 
//****************************************************
namespace
{
	/* この名前空間を削除してください */

	// 引数付き初期化
	template <typename T> std::unique_ptr<T> DedicatedInitialize(HINSTANCE hInstance, HWND hWnd)
	{
		useful::up<T> upInstance = std::make_unique<T>();

		if (!upInstance)
		{
			return nullptr;
		}

		if (FAILED(upInstance->Init(hInstance, hWnd)))
		{
			return nullptr;
		}

		return upInstance;
	}

	// 引数付き解放
	template <typename T> void DedicatedRelease(T* pInstance)
	{
		if (pInstance)
		{
			pInstance->Uninit();
		}
	}
};

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 静的メンバ変数の定義
//****************************************************

// 入力 -> 方向 汎用関数
std::function<std::optional<float>()> CInputManager::s_fpGetInputDir =
[]() -> std::optional<float> {

	// 入力装置の取得
	auto pKeyboard = CInputManager::RefInstance().GetKeyboard();
	auto pPad      = CInputManager::RefInstance().GetPad();
	
	// 移動入力の受付確認用
	float InputX = 0.0f;
	float InputY = 0.0f;

	// スティックの傾きを取得
	InputX = pPad->GetJoyStickL().X;
	InputY = pPad->GetJoyStickL().Y;

	// X軸
	if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::JOYKEY::LEFT))
	{
		InputX += -1.0f;
	}
	if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT))
	{
		InputX += 1.0f;
	}

	// Y軸
	if (pKeyboard->GetPress(DIK_W) || pPad->GetPress(CInputPad::JOYKEY::UP))
	{
		InputY += 1.0f;
	}
	if (pKeyboard->GetPress(DIK_S) || pPad->GetPress(CInputPad::JOYKEY::DOWN))
	{
		InputY += -1.0f;
	}

	// 移動方向を単位円で正規化するため、初期値を利用して明確に入力が「無い」ことを判別する
	if (InputX == 0.0f && InputY == 0.0f)
	{
		return std::nullopt;
	}

	// 入力方向を角度として変換
	return atan2f(InputX, InputY);
};

//============================================================================
// 初期化処理
//============================================================================
bool CInputManager::Initialize(HINSTANCE hInstance)
{
	// ウィンドウハンドルの取得
	HWND hWnd = CWindow::RefInstance().GetWindowHandle();

	// キーボードの生成
	m_upKeyboard = DedicatedInitialize<CInputKeyboard>(hInstance, hWnd);

	// マウスの生成
	m_upMouse = DedicatedInitialize<CInputMouse>(hInstance, hWnd);

	// パッドの生成
	m_upPad = DedicatedInitialize<CInputPad>(hInstance, hWnd);

	return true;
}

//============================================================================
// 終了処理
//============================================================================
void CInputManager::Finalize()
{
	// パッドの終了処理
	DedicatedRelease<CInputPad>(m_upPad.get());

	// マウスの終了処理
	DedicatedRelease<CInputMouse>(m_upMouse.get());

	// キーボードの終了処理
	DedicatedRelease<CInputKeyboard>(m_upKeyboard.get());
}

//============================================================================
// 更新処理
//============================================================================
void CInputManager::Update()
{
	// キーボードの更新
	m_upKeyboard->Update();

	// マウスの更新
	m_upMouse->Update();

	// パッドの更新
	m_upPad->Update();
}

//============================================================================
// コンストラクタ
//============================================================================
CInputManager::CInputManager()
	: m_upKeyboard(nullptr)
	, m_upMouse(nullptr)
	, m_upPad(nullptr)
{}

//============================================================================
// デストラクタ
//============================================================================
CInputManager::~CInputManager()
{
	// 終了処理
	Finalize();
}