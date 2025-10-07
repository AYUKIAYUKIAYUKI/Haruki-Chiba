//============================================================================
// 
// ウィンドウ [window.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "window.h"

//****************************************************
// 静的メンバ変数の定義
//****************************************************

// スクリーン横幅
const WORD  CWindow::WSCREEN_WIDTH = static_cast<WORD>(GetSystemMetrics(SM_CXSCREEN));
const float CWindow::FSCREEN_WIDTH = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));

// スクリーン縦幅
const WORD  CWindow::WSCREEN_HEIGHT = static_cast<WORD>(GetSystemMetrics(SM_CYSCREEN));
const float CWindow::FSCREEN_HEIGHT = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

//============================================================================
// コンストラクタ
//============================================================================
CWindow::CWindow()
	: m_Wcex{}
	, m_Rect{}
	, m_dwStyle(0)
	, m_hWnd(nullptr)
	, m_Msg{}
{}

//============================================================================
// デストラクタ
//============================================================================
CWindow::~CWindow()
{
	// 終了処理
	Finalize();
}

//============================================================================
// 初期化処理
//============================================================================
bool CWindow::Initialize(HINSTANCE hInstance)
{
	// ウィンドウクラスの初期化処理
	InitializeWindowClass(hInstance);

	// ウィンドウクラスの登録
	RegisterClassEx(&m_Wcex);

	// クライアント領域の初期化処理
	InitializeWindowRect();

	// ウィンドウの生成
	MyCreateWindow(hInstance);

	// 分解能を設定
	timeBeginPeriod(1);

	return true;
}

//============================================================================
// 終了処理
//============================================================================
void CWindow::Finalize()
{
	// 分解能の解放
	timeEndPeriod(1);
}

//============================================================================
// ウィンドウクラスの初期化処理
//============================================================================
void CWindow::InitializeWindowClass(HINSTANCE hInstance)
{
	m_Wcex.cbSize        = sizeof(WNDCLASSEX);				// WNDCLASSEXのメモリサイズ
	m_Wcex.style         = CS_CLASSDC;						// ウィンドウのスタイル
	m_Wcex.lpfnWndProc   = WindowProc;					    // ウィンドウプロシージャ
	m_Wcex.cbClsExtra    = 0;
	m_Wcex.cbWndExtra    = 0;
	m_Wcex.hInstance     = hInstance;						// インスタンスハンドル
	m_Wcex.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);	// タスクバーのアイコン
	m_Wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);		// マウスカーソル
	m_Wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);		    // クライアント領域の背景色
	m_Wcex.lpszMenuName  = NULL;							// メニューバー
	m_Wcex.lpszClassName = s_pClassName;				    // ウィンドウクラスの名前
	m_Wcex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);	// ファイルのアイコン
}

//============================================================================
// クライアント領域の初期化処理
//============================================================================
void CWindow::InitializeWindowRect()
{
	// クライアント領域のサイズを設定
	m_Rect = { 0, 0, WWINDOW_WIDTH, WWINDOW_HEIGHT};

	// ウィンドウの表示スタイルを設定
	m_dwStyle = WS_OVERLAPPEDWINDOW;

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&m_Rect, m_dwStyle, FALSE);
}

//============================================================================
// ウィンドウの生成
//============================================================================
void CWindow::MyCreateWindow(HINSTANCE hInstance)
{
	// ウィンドウの生成
	m_hWnd = CreateWindowEx(
		0,	                        // 拡張ウィンドウのスタイル
		s_pClassName,			    // ウィンドウクラスの名前
		s_pWindowName,			    // ウィンドウの名前
		WS_OVERLAPPEDWINDOW,	    // ウィンドウのスタイル
		125,					    // ウィンドウの左上のX座標
		50,						    // ウィンドウの左上のY座標
		m_Rect.right - m_Rect.left, // ウィンドウの幅
		m_Rect.bottom - m_Rect.top, // ウィンドウの高さ
		NULL,					    // 親ウィンドウのハンドル
		NULL,					    // メニューバーまたは子ウィンドウID
		hInstance,				    // インスタンスハンドル
		NULL);					    // ウィンドウ作成データ

	// ウィンドウの表示
	ShowWindow(m_hWnd, SW_NORMAL);
	UpdateWindow(m_hWnd);
}

// メッセージハンドラーの宣言
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//============================================================================
// ウィンドウプロシージャ
//============================================================================
LRESULT CALLBACK CWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// ImGuiの入力を受け付けるメッセージハンドラー
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	// 返り値格納先
	int nID = 0;

	switch (uMsg)
	{
		// ウィンドウ破棄のメッセージを送る
	case WM_DESTROY:

		PostQuitMessage(0);

		break;

		// ×マークを押しているメッセージを送る
	case WM_CLOSE:

		// メッセージボックスを表示
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd); // 「WM_DESTROYというメッセージを送る」
		}
		else
		{
			return 0;
		}

		break;

		// キーが押されているメッセージを送る
	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_ESCAPE:

			// メッセージボックスを表示
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd); // 「WM_DESTROYというメッセージを送る」
			}

			break;
		}

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); // 既定の処理を返す
}