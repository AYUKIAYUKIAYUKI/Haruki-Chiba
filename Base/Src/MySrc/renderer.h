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

// テンプレート実装ファイル
#include "renderer.impl"