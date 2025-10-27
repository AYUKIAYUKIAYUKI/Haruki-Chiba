//============================================================================
// 
// 衝撃波 [impact.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "impact.h"
#include "object.manager.h"
#include "player.h"
#include "state.h"
#include "renderer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 静的メンバ変数の定義 (公開)
//****************************************************

// デフォルトのファクトリ
std::function<bool(CImpact*)> CImpact::s_fpDefaultFactory =
[](CImpact* pImpact) -> bool
{
	// 初期化処理
	pImpact->Initialize();
	
	return true;
};

//============================================================================
// コンストラクタ
//============================================================================
CImpact::CImpact(OBJ::TYPE Type, OBJ::LAYER Layer)
	: CObjectX(Type, Layer)
	, m_nFrame(0)
	, m_Velocity(0.0f)
	, m_SizeTarget(0.0f)
	, m_Radius(0.0f)
	, m_pCreator(nullptr)
	, m_NowFrame(0)
	, m_pMesh(nullptr)
{}

//============================================================================
// デストラクタ
//============================================================================
CImpact::~CImpact()
{
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
}

//============================================================================
// 初期化処理
//============================================================================
bool CImpact::Initialize()
{
	// オブジェクト(Xモデル)の初期化処理
	if (FAILED(CObjectX::Initialize()))
	{
		return false;
	}

	return true;
}

//============================================================================
// 終了処理
//============================================================================
void CImpact::Finalize()
{
	// オブジェクト(Xモデル)の終了処理
	CObjectX::Finalize();
}

//============================================================================
// 更新処理
//============================================================================
void CImpact::Update()
{
	//円を大きくしていく
	ExpandRadius();

	//衝突判定
	CheckCollision();

	// オブジェクト(Xモデル)の更新処理
	// 行列の再計算を含んでいるため更新処理の終わりに呼びます
	CObjectX::Update();

	if (m_Radius >= m_SizeTarget)
	{//半径が目標の大きさになったら消す

		SetDeath();
	}
}

//============================================================================
// 描画処理
//============================================================================
void CImpact::Draw()
{
	//球のメッシュ生成
	CreateSphere();

	// オブジェクト(Xモデル)の描画処理
	//CObjectX::Draw();
}

//============================================================================
// 衝突判定
//============================================================================
void CImpact::CheckCollision()
{
	//プレイヤーが格納されているリストを取得
	std::list<CObject*> playerlist = CObjectManager::RefInstance().RefObjList(OBJ::TYPE::PLAYER);

	//コライダーの更新処理
	for (auto other : playerlist)
	{
		if (other == m_pCreator)
		{//発生させた者のとき
			continue;
		}

		CPlayer* pOtherPlayer = dynamic_cast<CPlayer*>(other);

		//円の判定
		bool IsHit = useful::CircleCollision(
			GetPos(),					//発生位置
			pOtherPlayer->GetPos(),		//相手の位置
			m_Radius,					//自分の半径
			pOtherPlayer->GetRadius()	//相手の半径
		);

		if (IsHit)
		{//当たってた

			auto NextState = std::make_shared<CPlayer_DamageState>();
			pOtherPlayer->Hit();
			pOtherPlayer->Getmachine()->ChangeState<CPlayer_DamageState>(); //ダメージ状態へ移行
		}
	}
}

//============================================================================
// 半径を設定
//============================================================================
void CImpact::SetRadius(float Radius)
{
	m_Radius = Radius;
}

//============================================================================
//発生させた者を保存
//============================================================================
void CImpact::SetCreator(CPlayer* Creator)
{
	m_pCreator = Creator;
}

//============================================================================
//半径とか設定した後に行う最初の処理
//============================================================================
void CImpact::Start()
{
	//最大の半径の大きさを設定
	m_SizeTarget = m_Radius * 2;

	//拡大する速さを設定
	m_nFrame = 30;

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ
	pDevice = CRenderer::RefInstance().GetDevice();//デバイスの取得
	D3DXCreateSphere(pDevice, m_Radius, 10, 7, &m_pMesh, NULL);//球のメッシュ生成

}

//============================================================================
// 半径を大きくする
//============================================================================
void CImpact::ExpandRadius()
{
	m_NowFrame++;

	float result, diff;

	//相対値を求める
	float Cur = (float)m_NowFrame / (float)m_nFrame;

	//差分を計算
	diff = m_SizeTarget - m_Radius;

	//差分の数値に補間係数を掛ける
	result = diff * Cur;

	//差分を足す
	m_Radius = m_Radius + result;
}

//============================================================================
// 球のメッシュを生成
//===========================================================================
void CImpact::CreateSphere()
{
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ
	pDevice = CRenderer::RefInstance().GetDevice();//デバイスの取得
	D3DXCreateSphere(pDevice, 1.0f, 10, 7, &m_pMesh, NULL);//球のメッシュ生成

	if (m_pMesh == nullptr)
	{
		return;
	}

	D3DXMATRIX mtxWorld, mtxRot, mtxTrans, mtxScale;//計算用マトリックス
	D3DMATERIAL9 matDef;//現在のマテリアル保存用
	D3DXMATERIAL Mat{};//マテリアル

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);
	
	// スケール：半径を反映
	D3DXMatrixScaling(&mtxScale, m_Radius, 1.0f, m_Radius);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//現在のマトリックスを取得
	pDevice->GetMaterial(&matDef);

	//法線の長さを1にする
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// マテリアル設定（白っぽい球にする）
	D3DMATERIAL9 mat = {};
	mat.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pDevice->SetMaterial(&mat);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//描画
	m_pMesh->DrawSubset(0);

}