//============================================================================
// 
// オブジェクト(Xモデル) [object.X.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "object.X.h"
#include "renderer.h"

// 初期化処理でXモデルを取得するため
#include "X.manager.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 静的メンバ変数の定義 (公開)
//****************************************************

// デフォルトのファクトリ
std::function<bool(CObjectX*)> CObjectX::s_fpDefaultFactory =
[](CObjectX* pObj) -> bool
	{
		// Xオブジェクトの初期設定
		pObj->Initialize();

		// モデルの取得
		auto pModel = useful::PtrCheck(CXManager::RefInstance().RefRegistry().BindAtKey("Unko"), "Syokika Lamda no Naka Model Nai");

		// モデルの設定
		pObj->SetModel(pModel);

		return true;
	};

//============================================================================
// コンストラクタ
//============================================================================
CObjectX::CObjectX(OBJ::TYPE Type, OBJ::LAYER Layer)
	: CObject(Type, Layer)
	, m_pModel(nullptr)
	, m_Size(VEC3_ONE_INIT)
	, m_Pos(VEC3_ZERO_INIT)
	, m_Rot(VEC3_ZERO_INIT)
	, m_Col(XCOL_ONE_INIT)
{
	// ワールド行列を初期化
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// デストラクタ
//============================================================================
CObjectX::~CObjectX()
{}

//============================================================================
// 初期化処理
//============================================================================
bool CObjectX::Initialize()
{
	return true;
}

//============================================================================
// 終了処理
//============================================================================
void CObjectX::Finalize()
{}

//============================================================================
// 更新処理
//============================================================================
void CObjectX::Update()
{
	// ワールド行列の更新
	UpdateMtxWorld(m_MtxWorld, m_Size, m_Rot, m_Pos);
}

//============================================================================
// 描画処理
//============================================================================
void CObjectX::Draw()
{
	// モデルが設定されていない
	if (m_pModel == nullptr)
	{
		std::runtime_error("Moderu ga Naiyo");
	}

	// マテリアルのバッファが異常
	if (m_pModel->pBuffMat == nullptr)
	{
		std::runtime_error("Moderu no Materiaru ga Yabaiyo");
	}

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::RefInstance().GetDevice();

	// 頂点法線の自動正規化を有効に
	pDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// ワールドマトリックスの設定
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// 現在のマテリアルを取得
	pDev->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pModel->pBuffMat->GetBufferPointer();

	for (WORD wCntMat = 0; wCntMat < static_cast<WORD>(m_pModel->dwNumMat); wCntMat++)
	{
		// マテリアル色の設定
		m_bUseCol ? pMat[wCntMat].MatD3D.Diffuse = m_Col : pMat[wCntMat].MatD3D.Diffuse = m_pModel->apColMat[wCntMat];

		// マテリアルの設定
		pDev->SetMaterial(&pMat[wCntMat].MatD3D);

		// テクスチャの設定
		pDev->SetTexture(0, m_pModel->apTex[wCntMat]);

		// オブジェクトパーツの描画
		m_pModel->pMesh->DrawSubset(wCntMat);
	}

	// 保存していたマテリアルを戻す
	pDev->SetMaterial(&matDef);

	// 頂点法線の自動正規化を無効に
	pDev->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}