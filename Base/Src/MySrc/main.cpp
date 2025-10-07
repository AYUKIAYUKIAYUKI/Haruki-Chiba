//============================================================================
// 
// エントリーポイント [main.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "main.h"

// マネージャー系統
#include "window.h"
#include "renderer.h"

// アセット系統
#include "font.manager.h"
#include "texture.manager.h"
#include "X.manager.h"
#include "sound.h"

// ゲーム系統
#include "camera_manager.h"
#include "light.h"
#include "input.manager.h"
#include "scene.manager.h"
#include "title.h"
#include "object.manager.h"

//============================================================================
// エントリーポイント
//============================================================================
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE /*hInstancePrev*/, _In_ LPSTR /*lpCmdLine*/, _In_ int /*nCmdShow*/)
{
	// CRTメモリリーク検出用
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ウィンドウの明示的な生成と初期化
	if (!CWindow::RefInstance().Initialize(hInstance))
	{
		return 0;
	}

	// ウィンドウハンドルの取得
	HWND hWnd = CWindow::RefInstance().GetWindowHandle();

	// レンダラーの明示的な生成と初期化
	if (!CRenderer::RefInstance().Initialize(hWnd, TRUE))
	{
		return 0;
	}

	// フォントマネージャーの明示的な生成
	CFontManager::RefInstance();

	// テクスチャマネージャーの明示的な生成
	CTextureManager::RefInstance();

	// Xモデルマネージャーの明示的な生成
	CXManager::RefInstance();

	// サウンド by AKR TNK
	CSound::GetInstance()->Init(hWnd);

	// カメラマネージャーの明示的な生成 : 魔界
	CCamera_Manager::RefInstance();

	// ライトの生成
	useful::up<CLight> upLight = std::make_unique<CLight>();

	// インプットマネージャーの明示的な生成
	if (!CInputManager::RefInstance().Initialize(hInstance))
	{
		return 0;
	}

	// シーンマネージャーの明示的な生成
	if (!CSceneManager::RefInstance().Initialize<CTitle>())
	{
		return 0;
	}

	// メッセージループ
	CWindow::CSingleton::RefInstance().MessageLoop(
		[]() -> void
		{
			// レンダラーの更新処理
			CRenderer::CSingleton::RefInstance().Update_ε(
				[]() -> void
				{
					// インプットの更新処理
					CInputManager::RefInstance().Update();

					// フリーカメラの更新
					CCamera::UpdateFreeCam();

					// 選択中カメラの更新
					CCamera_Manager::RefInstance().RefSelectCamera().Update();

					// シーンの更新処理
					CSceneManager::RefInstance().GetScene()->Update();

					// オブジェクトリストの更新
					CObjectManager::RefInstance().UpdateAllObject();
					CObjectManager::RefInstance().LateUpdateAllObject();
				});

			// レンダラーの描画処理
			CRenderer::CSingleton::RefInstance().Draw_ε(
				[]() -> void
				{
					// オブジェクトリストの描画
					CObjectManager::RefInstance().DrawAllObject();
				});
		});

	// サウンド by AKR TNK
	CSound::GetInstance()->Release();

	return 0;
}