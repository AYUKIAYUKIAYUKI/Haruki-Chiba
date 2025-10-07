//============================================================================
// 
// プレイヤー [player.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "player.h"

// 入力を取得するため
#include "input.manager.h"

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
std::function<bool(CPlayer*)> CPlayer::s_fpDefaultFactory =
[](CPlayer* pPlayer) -> bool
	{
		// 初期化処理
		pPlayer->Initialize();

		// モデルの取得
		auto pModel = useful::PtrCheck(CXManager::RefInstance().RefRegistry().BindAtKey("Player"), "Syokika Lamda no Naka Model Nai");

		// モデルの設定
		pPlayer->SetModel(pModel);

		return true;
	};

//============================================================================
// コンストラクタ
//============================================================================
CPlayer::CPlayer(OBJ::TYPE Type, OBJ::LAYER Layer)
	: CObjectX(Type, Layer)
	, m_nLeftNumJump(1)
	, m_Velocity(VEC3_ZERO_INIT)
	, m_SizeTarget(VEC3_ONE_INIT)
	, m_RotTarget(VEC3_ZERO_INIT)
	, m_PosTarget(VEC3_ZERO_INIT)
	, m_fpDefaultMove(nullptr)
	, m_fpDecorate(nullptr)
{}

//============================================================================
// デストラクタ
//============================================================================
CPlayer::~CPlayer()
{}

//============================================================================
// 初期化処理
//============================================================================
bool CPlayer::Initialize()
{
	// オブジェクト(Xモデル)の初期化処理
	if (FAILED(CObjectX::Initialize()))
	{
		return false;
	}
	
	// オマケの動作
	m_fpDecorate =
		[this]() -> void 
		{
			m_SizeTarget = VEC3_ONE_INIT;

			static float Hz = 0.0f;

			++Hz;

			m_SizeTarget.y = 1.0f + cosf(Hz) * 0.25f;
		};

	return true;
}

//============================================================================
// 終了処理
//============================================================================
void CPlayer::Finalize()
{
	// オブジェクト(Xモデル)の終了処理
	CObjectX::Finalize();
}

//============================================================================
// 更新処理
//============================================================================
void CPlayer::Update()
{
	// 入力方向を取得
	std::optional<float> opDir = CInputManager::s_fpGetInputDir();

	// 方向を加速度とみなす
	if (opDir)
	{
		// 漸次的にXZ平面を移動するものとする
		m_Velocity.x = sinf(opDir.value());
		m_Velocity.z = cosf(opDir.value());

		// オマケの動作
		m_fpDecorate();
	}

	// 加速度は時間に伴い減衰していく
	ExponentialDecay(m_Velocity, VEC3_ZERO_INIT, COEF_BRAKING);

	// 目標位置を加速度分変動させていく
	m_PosTarget += m_Velocity * COEF_MOVE_SPEED;

	// 現在サイズ -> 目標サイズ : 指数減衰
	Vec3 Size = GetSize();
	ExponentialDecay(Size, m_SizeTarget, COEF_CORRECT_TARGET);
	SetSize(Size);

	// 現在向き -> 目標向き : 指数減衰
	Vec3 Rot = GetRot();
	ExponentialDecay(Rot, m_RotTarget, COEF_CORRECT_TARGET);
	SetRot(Rot);

	// 現在位置 -> 目標位置 : 指数減衰
	Vec3 Pos = GetPos();
	ExponentialDecay(Pos, m_PosTarget, COEF_CORRECT_TARGET);
	SetPos(Pos);

	// オブジェクト(Xモデル)の更新処理
	// 行列の再計算を含んでいるため更新処理の終わりに呼びます
	CObjectX::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CPlayer::Draw()
{
	// オブジェクト(Xモデル)の描画処理
	CObjectX::Draw();
}