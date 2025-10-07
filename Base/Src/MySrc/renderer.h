//============================================================================
// 
// レンダラー、ヘッダーファイル [renderer.h]
// Author : 福田歩希
// 
//============================================================================

#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "singleton.h"
#include "camera_manager.h"

//****************************************************
// コンセプトの定義
//****************************************************

// 更新可能であることのコンセプト
template <typename T>
concept Updatable = requires(T t)
{
	{ t->Draw() } -> std::same_as<void>;
};

// 更新可能コンテナであることのコンセプト
template <typename T>
concept UpdatableContainer = requires(T t)
{
	{ *t.begin() } -> Updatable;
};

// 描画可能であることのコンセプト
template <typename T>
concept Drawable = requires(T t)
{
	{ t->Draw() } -> std::same_as<void>;
};

// 描画可能コンテナであることのコンセプト
template <typename T>
concept DrawableContainer = requires(T t)
{
	{ *t.begin() } -> Drawable;
};

//****************************************************
// レンダラークラス
//****************************************************
class CRenderer final : public CSingleton<CRenderer>
{
	//****************************************************
	// フレンド宣言
	//****************************************************
	friend struct std::default_delete<CRenderer>;
	friend CRenderer& CSingleton<CRenderer>::RefInstance();

	//****************************************************
	// 静的メンバ定数の定義 (非公開)
	//****************************************************
	static constexpr float FOG_BWGIN = 1500.0f; // フォグの初期始点
	static constexpr float FOG_END   = 2500.0f; // フォグの初期終点

public:

	//****************************************************
	// function
	//****************************************************
	
	// 初期化処理
	bool Initialize(HWND hWnd, BOOL bWindiw);
	bool Initialize() { return true; }

	// 終了処理
	void Finalize();

	/*
		ρ … コンセプトに沿ったコンテナのみ渡せるならそのまま範囲forで処理
		ε … 処理単位で渡してレンダラーの諸処理に挟んでもらうだけ
	*/

	// 更新処理ρ
	template <UpdatableContainer T>
	void Update_ρ(const T& t);

	// 更新処理ε
	template <typename T>
	void Update_ε(T&& fp);

	// 描画処理ρ
	template <DrawableContainer T>
	void Draw_ρ(const T& t);

	// 描画処理ε
	template <typename T>
	void Draw_ε(T&& fp);

	//****************************************************
	// inline function
	//****************************************************

	// デバイスの取得
	inline       LPDIRECT3DDEVICE9 GetDevice()            { return useful::PtrCheck(m_pDevice, "Device"); }
	inline const LPDIRECT3DDEVICE9 GetDeviceConst() const { return useful::PtrCheck(m_pDevice, "Device"); }

	// 指定期間表示するテキストの追加
	inline void AddText(std::string Str, int nCnt) { m_vText.push_back({ Str, nCnt }); }

private:

	//****************************************************
	// special function
	//****************************************************
	CRenderer();           // デフォルトコンストラクタ
	~CRenderer() override; // デストラクタ

	//****************************************************
	// function
	//****************************************************
	void InitializeRenderState(); // レンダーステートの初期セットアップ
	void AdjustBuckBufferSize();  // バックバッファのサイズを調整
	void PrintText();             // テキストの表示

	//****************************************************
	// data
	//****************************************************
	LPDIRECT3D9       m_pD3D;    // Direct3D
	LPDIRECT3DDEVICE9 m_pDevice; // デバイス

	// 指定期間表示するテキスト
	std::vector<std::pair<std::string, int>> m_vText;
};

//============================================================================
// 更新処理ρ
//============================================================================
template <UpdatableContainer T>
void CRenderer::Update_ρ(const T& t)
{
	// ImGuiの更新を開始
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

#if 0
	// ウィンドウサイズの変更があったら
	if (m_ResizeWindowWidth != 0 && m_ResizeWindowHeight != 0)
	{
		// バックバッファのサイズを調整
		AdjustBuckBufferSize();
	}
#endif

	// 各インスタンスの更新
	for (auto& it : t)
	{
		it->Update();
	}

	// ImGuiの更新を終了
	ImGui::EndFrame();
}

//============================================================================
// 更新処理ε
//============================================================================
template <typename T>
void CRenderer::Update_ε(T&& fp)
{
	// ImGuiの更新を開始
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

#if 0
	// ウィンドウサイズの変更があったら
	if (m_ResizeWindowWidth != 0 && m_ResizeWindowHeight != 0)
	{
		// バックバッファのサイズを調整
		AdjustBuckBufferSize();
	}
#endif

	// 各インスタンスの更新
	fp();

	// ImGuiの更新を終了
	ImGui::EndFrame();
}

//============================================================================
// 描画処理ρ
//============================================================================
template <DrawableContainer T>
void CRenderer::Draw_ρ(const T& t)
{
	// 画面バッファクリア
	m_pDevice->Clear(
		0,                                  // クリアしたい四角形の数を設定 (ビューポート全体の場合は0)
		nullptr,                            // 四角形構造体のポインタを設定 (nullptrを渡すことでビューポート全体の範囲)
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // クリアするサーフェスを指定する
		D3DCOLOR_ARGB(0, 0, 0, 0),          // このカラーでターゲットをクリア
		1.0f,                               // この値に大してデプスバッファをクリア
		0);                                 // この値でステンシルバッファをクリア

	if (SUCCEEDED(m_pDevice->BeginScene()))
	{
		// カメラをセット
		if (CCamera::GetStateUseFreeCam())
		{
			CCamera::RefFreeCam()->SetCamera({ 0.0f, 0.0f, 0.0f });
		}
		else
		{
			CCamera_Manager::RefInstance().RefSelectCamera().SetCamera({ 0.0f, 0.0f, 0.0f });
		}

		// 各インスタンスの描画
		for (auto& it : t)
		{
			it->Draw();
		}

		// テキストの描画
		PrintText();

		// ImGuiの描画
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		// 描画終了
		m_pDevice->EndScene();
	}

	// 裏画面を表画面に反映
	m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//============================================================================
// 描画処理ε
//============================================================================
template <typename T>
void CRenderer::Draw_ε(T&& fp)
{
	// 画面バッファクリア
	m_pDevice->Clear(
		0,                                  // クリアしたい四角形の数を設定 (ビューポート全体の場合は0)
		nullptr,                            // 四角形構造体のポインタを設定 (nullptrを渡すことでビューポート全体の範囲)
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // クリアするサーフェスを指定する
		D3DCOLOR_ARGB(0, 0, 0, 0),          // このカラーでターゲットをクリア
		1.0f,                               // この値に大してデプスバッファをクリア
		0);                                 // この値でステンシルバッファをクリア

	if (SUCCEEDED(m_pDevice->BeginScene()))
	{
		// カメラをセット
		if (CCamera::GetStateUseFreeCam())
		{
			CCamera::RefFreeCam()->SetCamera({ 0.0f, 0.0f, 0.0f });
		}
		else
		{
			CCamera_Manager::RefInstance().RefSelectCamera().SetCamera({ 0.0f, 0.0f, 0.0f });
		}

		// 各インスタンスの描画
		fp();

		// テキストの描画
		PrintText();

		// ImGuiの描画
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		// 描画終了
		m_pDevice->EndScene();
	}

	// 裏画面を表画面に反映
	m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
}