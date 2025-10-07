//============================================================================
// 
// フォントマネージャー [font.manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "font.manager.h"

// デバイス取得用
#include "renderer.h"

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CFontManager::CFontManager()
	: m_upRegistry(std::make_unique<CRegistry<LPD3DXFONT>>())
{}

//============================================================================
// デストラクタ
//============================================================================
CFontManager::~CFontManager()
{
	// 終了処理
	Finalize();
}

//============================================================================
// 初期化処理
//============================================================================
bool CFontManager::Initialize()
{
	// 生成処理
	std::function<LPD3DXFONT(const std::string&)> fpCreate =
		[](const std::string& Path) -> LPD3DXFONT
		{
			// フォントのポインタ
			LPD3DXFONT pElement = nullptr;

			// デバイスを取得
			auto pDev = CRenderer::RefInstance().GetDevice();

			// JSONファイルを展開
			const JSON& Json = useful::OpenJsonFileMaybeThrow(Path);

			// パラメータをキャスト
			const int          nSize = Json["Size"].get<int>();
			const std::string& sName = useful::JsonConvertToSJIS(Json["Name"]);

			// フォントを生成
			HRESULT hr = D3DXCreateFont(
				pDev,
				nSize,
				0,
				FW_HEAVY,
				0,
				FALSE,
				SHIFTJIS_CHARSET,
				OUT_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH,
				sName.c_str(),
				&pElement);

			// 生成失敗
			if (FAILED(hr))
			{
				throw std::runtime_error("Failed to Create Font");
			}

			// フォントのポインタを返す
			return pElement;
		};

	// 解放処理
	std::function<void(LPD3DXFONT&)> fpRelease =
		[](LPD3DXFONT& pElement) -> void
		{
			// フォントを解放
			if (pElement)
			{
				pElement->Release();
				pElement = nullptr;
			}
		};

	// レジストリの初期化処理
	if (!m_upRegistry->Initialize(INITIALIZE_PATH, std::move(fpCreate), std::move(fpRelease)))
	{
		return false;
	}

	return true;
}

//============================================================================
// 終了処理
//============================================================================
void CFontManager::Finalize()
{
	// レジストリの終了処理
	m_upRegistry->Finalize();
}