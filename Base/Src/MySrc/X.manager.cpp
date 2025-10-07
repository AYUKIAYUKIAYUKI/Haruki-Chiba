//============================================================================
// 
// Xモデルマネージャー [X.manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "X.manager.h"

// デバイス取得用
#include "renderer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CXManager::CXManager()
	: m_upRegistry(std::make_unique<CRegistry<MODEL*>>())
	, m_mCache{}
{}

//============================================================================
// デストラクタ
//============================================================================
CXManager::~CXManager()
{
	// 終了処理
	Finalize();
}

//============================================================================
// 初期化処理
//============================================================================
bool CXManager::Initialize()
{
	// 生成処理用のラムダオブジェクト作成
	std::function<MODEL*(const std::string&)> fpCreate =
		[this](const std::string& path) -> MODEL*
	{
		// Xモデルのインスタンスを生成
		MODEL* pElement = DBG_NEW MODEL();

		// デバイスを取得
		auto pDev = CRenderer::RefInstance().GetDevice();

		// Xモデルの生成
		HRESULT hr = D3DXLoadMeshFromX(
			path.c_str(),
			D3DXMESH_SYSTEMMEM,
			pDev,
			nullptr,
			&pElement->pBuffMat,
			nullptr,
			&pElement->dwNumMat,
			&pElement->pMesh);

		// 生成失敗
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to Import X Model : " + path);
		}

		// モデルのサイズを取得する
		pElement->Size = LoadSize(path);

		// マテリアルデータへのポインタを取得
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)pElement->pBuffMat->GetBufferPointer();

		// マテリアル数分、マテリアル色情報のポインタ配列を確保
		pElement->apColMat = DBG_NEW D3DXCOLOR[pElement->dwNumMat];

		// マテリアル数分、テクスチャ情報のポインタ配列を確保
		pElement->apTex = DBG_NEW LPDIRECT3DTEXTURE9[pElement->dwNumMat];

		// マテリアルごとにテクスチャの有無を確認
		for (WORD wCntMat = 0; wCntMat < pElement->dwNumMat; ++wCntMat)
		{
			// マテリアルの色をモデル情報内に保存しておく
			pElement->apColMat[wCntMat] = pMat[wCntMat].MatD3D.Diffuse;

			// テクスチャ名を取得失敗
			if (pMat[wCntMat].pTextureFilename == nullptr)
			{
				// テクスチャのポインタを初期化しておく
				pElement->apTex[wCntMat] = nullptr;

				continue;
			}

			/* テクスチャの生成前に、キャッシュ内に同一のものがないかチェック */
			auto it = m_mCache.find(pMat[wCntMat].pTextureFilename);

			/* 存在すれば使いまわす */
			if (it != m_mCache.end())
			{
				pElement->apTex[wCntMat] = it->second;
			}
			else
			{
				/* 無い場合のみ新規生成 */
				hr = D3DXCreateTextureFromFileA(
					pDev,
					pMat[wCntMat].pTextureFilename,
					&pElement->apTex[wCntMat]);

				/* このテクスチャの情報をキャッシュに追加 */
				m_mCache[pMat[wCntMat].pTextureFilename] = pElement->apTex[wCntMat];
			}

			// テクスチャ生成失敗
			if (FAILED(hr))
			{
				// エラーハンドリング
				switch (hr)
				{
				case D3DERR_INVALIDCALL:
					MessageBoxA(nullptr, "無効な呼び出し（引数が不正など）", "エラー", MB_OK);
					break;
				case D3DERR_OUTOFVIDEOMEMORY:
					MessageBoxA(nullptr, "ビデオメモリ不足", "エラー", MB_OK);
					break;
				case E_OUTOFMEMORY:
					MessageBoxA(nullptr, "メモリ不足", "エラー", MB_OK);
					break;
				default:
					MessageBoxA(nullptr, "テクスチャ読み込み失敗", "エラー", MB_OK);
					break;
				}

				throw std::runtime_error("Failed to Create X Model - Texture");
			}
		}

		// インスタンスのポインタを返す
		return pElement;
	};

	// 解放処理用のラムダオブジェクト作成
	std::function<void(MODEL*&)> fpRelease =
		[](MODEL*& element) -> void
	{
		// マテリアル用のテクスチャを破棄
		for (WORD wCntMat = 0; wCntMat < element->dwNumMat; ++wCntMat)
		{
			if (element->apTex[wCntMat] != nullptr)
			{
				element->apTex[wCntMat]->Release();
				element->apTex[wCntMat] = nullptr;
			}
		}

		// テクスチャポインタ配列の解放
		if (element->apTex != nullptr)
		{
			delete[] element->apTex;
			element->apTex = nullptr;
		}

		// マテリアル色ポインタ配列の破棄
		if (element->apColMat != nullptr)
		{
			delete[] element->apColMat;
			element->apColMat = nullptr;
		}

		// マテリアルバッファの破棄
		if (element->pBuffMat != nullptr)
		{
			element->pBuffMat->Release();
			element->pBuffMat = nullptr;
		}

		// メッシュバッファの破棄
		if (element->pMesh != nullptr)
		{
			element->pMesh->Release();
			element->pMesh = nullptr;
		}

		// インスタンスを解放
		if (element)
		{
			delete element;
			element = nullptr;
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
// サイズ読み込み
//============================================================================
D3DXVECTOR3 CXManager::LoadSize(const std::string& sPath)
{
	// 該当モデルファイルを展開
	std::ifstream ModelFile(sPath);

	// 展開に失敗
	if (!ModelFile.good())
	{
		return VEC3_ZERO_INIT;
	}

	// 汎用文字列格納用
	std::string Str;

	// テキストから頂点情報の行を検索
	while (std::getline(ModelFile, Str))
	{
		// 頂点情報を発見したので検索終了
		if (Str == "Mesh {")
		{
			// 不要な行を一度読み込む
			std::getline(ModelFile, Str);

			break;
		}
	}

	// 比較処理用に数値を入れておく
	Vec3 ResultMin = { FLT_MAX, FLT_MAX, FLT_MAX };
	Vec3 ResultMax = { FLT_MIN, FLT_MIN, FLT_MIN };

	// 取得予定の軸方向数
	const WORD wAxis = 3;

	// 数値を比較していく
	while (std::getline(ModelFile, Str))
	{
		// 終了条件
		if (Str == " ")
		{
			break;
		}

		// 数値格納用
		float fNum[wAxis] = { 0.0f, 0.0f, 0.0f };

		// 読みとった数値を一旦格納する
		for (WORD wCntAxis = 0; wCntAxis < wAxis; ++wCntAxis)
		{
			fNum[wCntAxis] = std::stof(Str);
			Str = Str.substr(Str.find(";") + 1, Str.back());
		}

		// 各軸の最大・最小地点を更新
		if (ResultMin.x > fNum[0])
		{
			ResultMin.x = fNum[0];
		}

		if (ResultMin.y > fNum[1])
		{
			ResultMin.y = fNum[1];
		}

		if (ResultMin.z > fNum[2])
		{
			ResultMin.z = fNum[2];
		}

		if (ResultMax.x < fNum[0])
		{
			ResultMax.x = fNum[0];
		}

		if (ResultMax.y < fNum[1])
		{
			ResultMax.y = fNum[1];
		}

		if (ResultMax.z < fNum[2])
		{
			ResultMax.z = fNum[2];
		}
	}

	// ファイルを閉じる
	ModelFile.close();
	
	return ResultMax;
}

//============================================================================
// 終了処理
//============================================================================
void CXManager::Finalize()
{
	// レジストリの終了処理
	m_upRegistry->Finalize();
}