//============================================================================
// 
// オブジェクト(2D) [object.2D.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "object.2D.h"
#include "renderer.h"

// クライアント領域のサイズ取得用
#include "window.h"

// 初期化処理でテクスチャを取得するため
#include "texture.manager.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 静的メンバ変数の定義 (公開)
//****************************************************

// デフォルトのファクトリ
std::function<bool(CObject2D*)> CObject2D::s_fpDefaultFactory =
[](CObject2D* pObj) -> bool
	{
		// 2Dオブジェクトの初期設定
		pObj->Initialize();

		// テクスチャの取得
		auto pTex = useful::PtrCheck(CTextureManager::RefInstance().RefRegistry().BindAtKey("Unko"), "Syokika Lamda no Naka Texture Nai");

		// テクスチャ指定
		pObj->SetTex(pTex);

		// サイズ指定
		pObj->SetSize({ 30.0f, 30.0f });
		pObj->SetPos({ CWindow::FWINDOW_WIDTH * 0.5f, CWindow::FWINDOW_HEIGHT * 0.5f });

		return true;
	};

//============================================================================
// コンストラクタ
//============================================================================
CObject2D::CObject2D(OBJ::TYPE Type, OBJ::LAYER Layer)
	: CObject(Type, Layer)
	, m_pVtxBuff(nullptr)
	, m_pTex(nullptr)
	, m_Size(VEC2_ZERO_INIT)
	, m_fRot(0.0f)
	, m_Pos(VEC2_ZERO_INIT)
	, m_Col(XCOL_ONE_INIT)
	, m_fLength(0.0f)
	, m_fAngle(0.0f)
	, m_TexSize(VEC2_ZERO_INIT)
	, m_fTexWidth(1.0f)
	, m_fTexHeight(1.0f)
	, m_nNowPatternU(0)
	, m_nNowPatternV(0)
{}

//============================================================================
// デストラクタ
//============================================================================
CObject2D::~CObject2D()
{}

//============================================================================
// 初期化処理
//============================================================================
bool CObject2D::Initialize()
{
	// 頂点バッファの生成
	if (FAILED(CreateVtxBuff()))
	{
		return false;
	}

	return true;
}

//============================================================================
// 終了処理
//============================================================================
void CObject2D::Finalize()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//============================================================================
// 更新処理
//============================================================================
void CObject2D::Update()
{
	// 頂点バッファが消失
	if (!m_pVtxBuff)
	{ 
		std::runtime_error("Buffer ga Naiyo 2D");
	}

	// 必要な数値を算出
	m_fLength = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y);
	m_fAngle  = atan2f(m_Size.x, m_Size.y);

	// 現在のクライアント領域のサイズの参照
	const RECT rNowWindowRect = CWindow::RefInstance().GetWindowRect();

	// スケーリング係数
	float fWindowScaleX = static_cast<float>(rNowWindowRect.right)  / CWindow::FWINDOW_WIDTH;
	float fWindowScaleY = static_cast<float>(rNowWindowRect.bottom) / CWindow::FWINDOW_HEIGHT;

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// 頂点座標の設定
	pVtx[0].pos = 
	{
		(m_Pos.x + sinf(m_fRot - (D3DX_PI - m_fAngle)) * m_fLength) * fWindowScaleX,
		(m_Pos.y + cosf(m_fRot - (D3DX_PI - m_fAngle)) * m_fLength) * fWindowScaleY,
		0.0f
	};

	pVtx[1].pos = 
	{
		(m_Pos.x + sinf(m_fRot + (D3DX_PI - m_fAngle)) * m_fLength) * fWindowScaleX,
		(m_Pos.y + cosf(m_fRot + (D3DX_PI - m_fAngle)) * m_fLength) * fWindowScaleY,
		0.0f
	};

	pVtx[2].pos = 
	{
		(m_Pos.x + sinf(m_fRot - m_fAngle) * m_fLength) * fWindowScaleX,
		(m_Pos.y + cosf(m_fRot - m_fAngle) * m_fLength) * fWindowScaleY,
		0.0f
	};

	pVtx[3].pos = 
	{
		(m_Pos.x + sinf(m_fRot + m_fAngle) * m_fLength) * fWindowScaleX,
		(m_Pos.y + cosf(m_fRot + m_fAngle) * m_fLength) * fWindowScaleY,
		0.0f
	};

	// 頂点色の設定
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// テクスチャ座標の設定
	pVtx[0].tex = { m_fTexWidth * m_nNowPatternU, m_fTexHeight * m_nNowPatternV };
	pVtx[1].tex = { m_fTexWidth * (m_nNowPatternU + 1) - m_TexSize.x, m_fTexHeight * m_nNowPatternV };
	pVtx[2].tex = { m_fTexWidth * (m_nNowPatternU), m_fTexHeight * (m_nNowPatternV + 1) };
	pVtx[3].tex = { m_fTexWidth * (m_nNowPatternU + 1) - m_TexSize.x, m_fTexHeight * (m_nNowPatternV + 1) };

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================================================================
// 描画処理
//============================================================================
void CObject2D::Draw()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::RefInstance().GetDevice();

	// 頂点バッファをデータストリームに設定
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDev->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDev->SetTexture(0, m_pTex);

	// ポリゴンの描画
	pDev->DrawPrimitive(
		D3DPT_TRIANGLESTRIP, // プリミティブの種類
		0,                   // 頂点情報の先頭アドレス
		NUM_PRIM);           // プリミティブ数
}

//============================================================================
// 頂点バッファの生成
//============================================================================
HRESULT CObject2D::CreateVtxBuff()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::RefInstance().GetDevice();

	// 頂点バッファの生成
	pDev->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VTX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	// 生成失敗
	if (!m_pVtxBuff)
	{
		return E_FAIL;
	}

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	for (WORD wNumVtx = 0; wNumVtx < NUM_VTX; ++wNumVtx)
	{
		// 頂点座標の設定
		pVtx[wNumVtx].pos = VEC3_ZERO_INIT;

		// 除算数の設定
		pVtx[wNumVtx].rhw = 1.0f;

		// 頂点色の設定
		pVtx[wNumVtx].col = XCOL_ONE_INIT;

		// テクスチャ座標の設定
		pVtx[wNumVtx].tex = VEC2_ZERO_INIT;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}