//============================================================================
// 
// レンダラー [renderer.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "renderer.h"

// テキスト描画用
#include "window.h"
#include "font.manager.h"

//============================================================================
// 初期設定
//============================================================================
bool CRenderer::Initialize(HWND hWnd, BOOL bWindiw)
{
	// Direct3Dのセットアップ用変数
	D3DDISPLAYMODE        D3ddm; // ディスプレイモード
	D3DPRESENT_PARAMETERS D3dpp; // プレゼンテーションパラメータ

	// パラメータのゼロクリア
	ZeroMemory(&D3dpp, sizeof(D3dpp));

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	// Direct3Dオブジェクトの生成チェック
	useful::PtrCheck(m_pD3D, "Direct3D");

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &D3ddm)))
	{ 
		return false;
	}

	// デバイスのプレゼンテーションパラメータの設定
	D3dpp.BackBufferWidth            = 1280;                        // ゲームサイズの横幅
	D3dpp.BackBufferHeight           = 720;                         // ゲームサイズの高さ
	D3dpp.BackBufferFormat           = D3ddm.Format;                // バックバッファの形式
	D3dpp.BackBufferCount            = 1;                           // バックバッファの数
	D3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;       // ダブルバッファの切り替え(映像信号に同期)
	D3dpp.EnableAutoDepthStencil     = TRUE;                        // デプスバッファとステンシルバッファを作成
	D3dpp.AutoDepthStencilFormat     = D3DFMT_D16;                  // デプスバッファとして16bitを使う
	D3dpp.Windowed                   = bWindiw;                     // ウインドウモード
	D3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;     // リフレッシュレート
	D3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT; // インターバル

	// Direct3Dデバイスのセットアップ - ハードウェア処理優先
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&D3dpp,
		&m_pDevice)))
	{
		// Direct3Dデバイスのセットアップ - ソフトウェア処理優先
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&D3dpp,
			&m_pDevice)))
		{
			// Direct3Dデバイスのセットアップ - リファレンスデバイス
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&D3dpp,
				&m_pDevice)))
			{
				return false;
			}
		}
	}

	// レンダーステートの初期化処理
	InitializeRenderState();

	// ImGuiのコンテキストを作成
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// ImGuiの入出力設定
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// ImGuiの表示スタイルを設定
	ImGui::StyleColorsDark();

	// バックエンドの初期設定
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(m_pDevice);

	// ImGuiのフォント設定
	io.Fonts->AddFontFromFileTTF("Data\\FONT\\Meiryo\\meiryo.ttc", 20.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	return true;
}

//============================================================================
// 終了処理
//============================================================================
void CRenderer::Finalize()
{
	// ImGUiの終了処理
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// デバイスの解放
	if (m_pDevice != nullptr)
	{
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// Direct3Dオブジェクトの解放
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CRenderer::CRenderer()
	: m_pD3D(nullptr)
	, m_pDevice(nullptr)
	, m_vText{}
{}

//============================================================================
// デストラクタ
//============================================================================
CRenderer::~CRenderer()
{
	Finalize();
}

//============================================================================
// レンダーステートの初期セットアップ
//============================================================================
void CRenderer::InitializeRenderState()
{
	// レンダーステートの初期設定
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);           // カリングモードの設定
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);          // アルファブレンドの有効化
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);     // ソースブレンドの設定
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // ターゲットブレンドの設定
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);           // アルファテストの有効化
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x24);                  // アルファ参照値
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);       // アルファテスト合格基準
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);          // アルファテストの無効化
	m_pDevice->SetRenderState(D3DRS_STENCILMASK, 0x000000ff);         // ステンシルマスクを設定
	
	// フォグ設定
	//m_pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);                             // フォグの有効化
	//m_pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);                 // フォグモードの設定
	//m_pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)); // フォグカラーの設定
	//m_pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_fFogStart));           // 始点を設定
	//m_pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_fFogEnd));               // 終点を設定

	// テクスチャステージステートの初期設定
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

//============================================================================
// バックバッファのサイズを調整
//============================================================================
void CRenderer::AdjustBuckBufferSize()
{}

//============================================================================
// テキストの表示
//============================================================================
void CRenderer::PrintText()
{
	// フォントマネージャーの取得
	LPD3DXFONT pFont = CFontManager::RefInstance().RefRegistry().BindAtKey("Meiryo");

	// 表示範囲調整用
	static WORD wAdjustHeight = 0;

	// 表示位置
	RECT Rect = { 0, 0 - wAdjustHeight, CWindow::WWINDOW_WIDTH, CWindow::WWINDOW_HEIGHT - wAdjustHeight };

	// 表示する文字列
	std::string PrintStr = {};

	for (int nIdx = 0; nIdx < m_vText.size(); ++nIdx)
	{
		if (m_vText[nIdx].second > 0)
		{
			// 表示時間をカウントダウン
			m_vText[nIdx].second--;

			// 文章をつなげる
			PrintStr += m_vText[nIdx].first + "\n";
		}
		else
		{
			// 文章を消去
			m_vText.erase(m_vText.begin() + nIdx);
		}
	}

	// テキストの描画
	pFont->DrawText(NULL, PrintStr.c_str(), -1, &Rect, DT_CENTER | DT_VCENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
}