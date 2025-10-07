//============================================================================
// 
// テクスチャマネージャー [texture.manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "texture.manager.h"

// デバイス取得用
#include "renderer.h"

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CTextureManager::CTextureManager()
	: m_upRegistry(std::make_unique<CRegistry<LPDIRECT3DTEXTURE9>>())
{}

//============================================================================
// デストラクタ
//============================================================================
CTextureManager::~CTextureManager()
{
	// 終了処理
	Finalize();
}

//============================================================================
// 初期化処理
//============================================================================
bool CTextureManager::Initialize()
{
	// 生成処理
	std::function<LPDIRECT3DTEXTURE9(const std::string&)> fpCreate = 
		[](const std::string& Path) -> LPDIRECT3DTEXTURE9
	{
		// テクスチャのポインタ
		LPDIRECT3DTEXTURE9 pElement = nullptr;

		// デバイスを取得
		auto pDev = CRenderer::RefInstance().GetDevice();

		// テクスチャを生成
		HRESULT hr = D3DXCreateTextureFromFileA(
			pDev,
			Path.c_str(),
			&pElement);

		// 生成失敗
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to Create Texture");
		}

		// テクスチャのポインタを返す
		return pElement;
	};

	// 解放処理
	std::function<void(LPDIRECT3DTEXTURE9&)> fpRelease =
		[](LPDIRECT3DTEXTURE9& pElement) -> void
	{
		// テクスチャを解放
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
void CTextureManager::Finalize()
{
	// レジストリの終了処理
	m_upRegistry->Finalize();
}