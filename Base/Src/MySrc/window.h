//============================================================================
// 
// ウィンドウ、ヘッダーファイル [window.h]
// Author : 福田歩希
// 
//============================================================================

#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "singleton.h"

//****************************************************
// ウィンドウクラスの定義
//****************************************************
class CWindow final : public CSingleton<CWindow>
{
	//****************************************************
	// フレンド宣言
	//****************************************************
	friend struct std::default_delete<CWindow>;
	friend CWindow& CSingleton<CWindow>::RefInstance();

	//****************************************************
	// 静的メンバ定数の定義 (非公開)
	//****************************************************

	// ウィンドウクラスの名前
	static constexpr const char* s_pClassName = "Window Class";

	// ウィンドウの名前
	static constexpr const char* s_pWindowName = "Base";

public:

	//****************************************************
	// 静的メンバ定数の定義 (公開)
	//****************************************************

	// ウィンドウ横幅
	static constexpr WORD  WWINDOW_WIDTH = 1280;
	static constexpr float FWINDOW_WIDTH = 1280.0f;

	// ウィンドウ縦幅
	static constexpr WORD  WWINDOW_HEIGHT = 720;
	static constexpr float FWINDOW_HEIGHT = 720.0f;

	//****************************************************
	// 静的メンバ変数の定義 (公開)
	//****************************************************

	// スクリーン横幅
	static const WORD  WSCREEN_WIDTH;
	static const float FSCREEN_WIDTH;

	// スクリーン縦幅
	static const WORD  WSCREEN_HEIGHT;
	static const float FSCREEN_HEIGHT;

	//****************************************************
	// function
	//****************************************************

	// 初期化処理
	bool Initialize(HINSTANCE hInstance);
	bool Initialize() { return true; }

	// 終了処理
	void Finalize();

	// メッセージループ
	// fp -> 更新処理
	template <typename T>
	void MessageLoop(T&& fpMainLoop);

	//****************************************************
	// inline function
	//***************************************************

	// クライアント領域のサイズの取得
	inline RECT GetWindowRect() const { return m_Rect; }

	// ウィンドウハンドルの取得
	inline HWND GetWindowHandle() const { return m_hWnd; }

private:

	//****************************************************
	// special function
	//****************************************************
	CWindow();           // デフォルトコンストラクタ
	~CWindow() override; // デストラクタ

	//****************************************************
	// function
	//****************************************************
	void InitializeWindowClass(HINSTANCE hInstance); // ウィンドウクラスの初期化処理
	void InitializeWindowRect();					 // クライアント領域の初期化処理
	void MyCreateWindow(HINSTANCE hInstance);		 // ウィンドウの生成

	// ウインドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	//****************************************************
	// data
	//****************************************************
	WNDCLASSEX m_Wcex;	  // ウィンドウクラス
	RECT	   m_Rect;	  // クライアント領域のサイズ
	DWORD	   m_dwStyle; // ウィンドウの表示スタイル
	HWND	   m_hWnd;	  // ウィンドウハンドル
	MSG		   m_Msg;	  // メッセージを格納する変数
};

// テンプレート実装ファイル
#include "window.impl"