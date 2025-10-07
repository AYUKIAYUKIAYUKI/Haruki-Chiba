//============================================================================
// 
// オブジェクト(3D) [object.3D.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "object.3D.h"
#include "renderer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//============================================================================
// コンストラクタ
//============================================================================
CObject3D::CObject3D(OBJ::TYPE Type, OBJ::LAYER Layer)
	: CObject(Type, Layer)
	, m_pVtxBuff(nullptr)
	, m_pTex(nullptr)
	, m_Size(VEC3_ZERO_INIT)
	, m_Rot(VEC3_ZERO_INIT)
	, m_Pos(VEC3_ZERO_INIT)
	, m_Col(XCOL_ONE_INIT)
	, m_fLength(0.0f)
	, m_fAngle(0.0f)
{
	// ワールド行列の初期化
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// デストラクタ
//============================================================================
CObject3D::~CObject3D()
{}

//============================================================================
// 初期化処理
//============================================================================
bool CObject3D::Initialize()
{
	// 頂点バッファの生成
	if (FAILED(CreateVtxBuff()))
	{
		return false;
	}

	// やめよう
	m_Size = { 500.0f, 500.0f, 0.0f };

	return true;
}

//============================================================================
// 終了処理
//============================================================================
void CObject3D::Finalize()
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
void CObject3D::Update()
{
	if (m_pVtxBuff == nullptr)
	{ // 頂点バッファが消失
		assert(false);
	}

	// 必要な数値を計算
	m_fAngle = atan2f(m_Size.x, m_Size.y);
	m_fLength = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y);

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// 頂点座標の設定
	pVtx[0].pos = {
		sinf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		-cosf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[1].pos = {
		sinf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		-cosf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[2].pos = {
		sinf(m_Rot.z - m_fAngle) * m_fLength,
		-cosf(m_Rot.z - m_fAngle) * m_fLength,
		0.0f
	};

	pVtx[3].pos = {
		sinf(m_Rot.z + m_fAngle) * m_fLength,
		-cosf(m_Rot.z + m_fAngle) * m_fLength,
		0.0f
	};

	// 頂点色の設定
	for (WORD wCntVtx = 0; wCntVtx < NUM_VTX; ++wCntVtx)
	{
		pVtx[wCntVtx].col = m_Col;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// ワールド行列の設定
	SetMtxWorld();
}

//============================================================================
// 描画処理
//============================================================================
void CObject3D::Draw()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::RefInstance().GetDevice();

	// アルファテストを有効にする
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDev->SetRenderState(D3DRS_ALPHAREF, 0);
	pDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ライト反映を無効にする
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールド行列の設定
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// 頂点バッファをデータストリームに設定
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDev->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDev->SetTexture(0, m_pTex);

	// ポリゴンの描画
	pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,										// 頂点情報の先頭アドレス
		NUM_PRIM);								// プリミティブ数

	// ライト反映を有効にする
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	// アルファテストを無効にする
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//============================================================================
// テクスチャ割当
//============================================================================
void CObject3D::BindTex(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTex = pTex;
}

//============================================================================
// サイズ取得
//============================================================================
const D3DXVECTOR3& CObject3D::GetSize() const
{
	return m_Size;
}

//============================================================================
// サイズ設定
//============================================================================
void CObject3D::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// 向き取得
//============================================================================
const D3DXVECTOR3& CObject3D::GetRot()const
{
	return m_Rot;
}

//============================================================================
// 向き設定
//============================================================================
void CObject3D::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// 座標取得
//============================================================================
const D3DXVECTOR3& CObject3D::GetPos() const
{
	return m_Pos;
}

//============================================================================
// 座標設定
//============================================================================
void CObject3D::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// 色を取得
//============================================================================
const D3DXCOLOR& CObject3D::GetCol() const
{
	return m_Col;
}

//============================================================================
// 色を設定
//============================================================================
void CObject3D::SetCol(D3DXCOLOR Col)
{
	m_Col = Col;
}

//============================================================================
// アルファ値を取得
//============================================================================
const float& CObject3D::GetAlpha() const
{
	return m_Col.a;
}

//============================================================================
// アルファ値を設定
//============================================================================
void CObject3D::SetAlpha(float fAlpha)
{
	m_Col.a = fAlpha;
}

//============================================================================
// 展開用対角線取得
//============================================================================
const float& CObject3D::GetLength() const
{
	return m_fLength;
}

//============================================================================
// 生成
//============================================================================
CObject3D* CObject3D::Create(OBJ::TYPE Type, OBJ::LAYER Layer, LPDIRECT3DTEXTURE9 pTexture)
{
	// インスタンスを生成
	CObject3D* pNewInstance = DBG_NEW CObject3D(Type, Layer);

	// 生成出来ていたら初期設定
	if (pNewInstance == nullptr)
	{
		std::runtime_error("Seisei Situpai 2D");
	}

	// 3Dオブジェクトの初期設定
	pNewInstance->Initialize();

	// テクスチャ指定
	pNewInstance->BindTex(pTexture);

	return pNewInstance;
}

//============================================================================
// 頂点バッファの生成
//============================================================================
HRESULT CObject3D::CreateVtxBuff()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::RefInstance().GetDevice();

	// 頂点バッファの生成
	pDev->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VTX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (m_pVtxBuff == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	for (WORD wNumVtx = 0; wNumVtx < NUM_VTX; ++wNumVtx)
	{
		// 頂点座標の設定
		pVtx[wNumVtx].pos = { useful::VEC3_ZERO_INIT };

		// 法線ベクトルの設定
		pVtx[wNumVtx].nor = { useful::VEC3_ZERO_INIT };

		// 頂点色の設定
		pVtx[wNumVtx].col = useful::XCOL_ONE_INIT;
	}

	// テクスチャ座標の設定
	pVtx[0].tex = { 0.0f, 0.0f };
	pVtx[1].tex = { 1.0f, 0.0f };
	pVtx[2].tex = { 0.0f, 1.0f };
	pVtx[3].tex = { 1.0f, 1.0f };

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// ワールド行列設定
//============================================================================
void CObject3D::SetMtxWorld()
{
	// 計算用行列
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールド行列を初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 回転行列作成
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_Rot.y,
		m_Rot.x,
		m_Rot.z);

	// 回転行列との掛け算
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxRot);

	// 平行移動行列作成
	D3DXMatrixTranslation(&mtxTrans,
		m_Pos.x,
		m_Pos.y,
		m_Pos.z);

	// 平行移動行列との掛け算
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxTrans);
}