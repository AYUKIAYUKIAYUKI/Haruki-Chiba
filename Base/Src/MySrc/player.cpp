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
// 静的メンバ変数の定義 (非公開)
//****************************************************

// ▽ 最終的には定数となりますが、漸次的に編集可能な値とします

// 目標値への補間係数
float CPlayer::COEF_CORRECT_TARGET = 0.6f;

// 重力加速度
float CPlayer::COEF_GRAVITY = -0.15f;

// ジャンプ可能回数
int CPlayer::NUM_LEFT_JUMP = 1;

// ジャンプ力
float CPlayer::COEF_TRIGGER_JUMP = 2.75f;

// 移動速度
float CPlayer::COEF_MOVE_SPEED     = 2.0f;
float CPlayer::COEF_MOVE_SPEED_AIR = 0.3f;

// 制動力
float CPlayer::COEF_BRAKING = 0.5f;

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
	, m_nFrame(0)
	, m_State(State::DEFAULT)
	//, m_nLeftNumJump(NUM_LEFT_JUMP)
	, m_Velocity(VEC3_ZERO_INIT)
	, m_SizeTarget(VEC3_ONE_INIT)
	, m_RotTarget(VEC3_ZERO_INIT)
	, m_PosTarget(VEC3_ZERO_INIT)
	, m_afpExecuteState{}
	, m_vMichos()
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

	// 状態ごとの実行内容を定義していく：通常
	m_afpExecuteState[static_cast<unsigned char>(State::DEFAULT)] =
		[this]() -> bool
	{
		// 移動
		Move(COEF_MOVE_SPEED);

		if (CInputManager::RefInstance().GetKeyboard()->GetTrigger(DIK_B))
		{
			Change(State::DAMAGE, 0, [this]() -> void {});
		}

		// ジャンプ
		if (CInputManager::RefInstance().GetKeyboard()->GetTrigger(DIK_SPACE))
		{
			Change(State::JUMP, 0, [this]() -> void
				{
					// 加速度：XZ軸：速度を抑えつつ余韻を遺す
					// 　　　：Y軸 ：ジャンプ力を与える
					m_Velocity =
					{
						m_Velocity.x * COEF_MOVE_SPEED_AIR,
						COEF_TRIGGER_JUMP,
						m_Velocity.z * COEF_MOVE_SPEED_AIR
					};

					// みちょ数
					const int nMichosRepeat = 3;

					// みちょっと幅
					std::array<float, nMichosRepeat> afScales =
					{
						-0.25f, -0.35f, -0.15f
					};

					// みちょっとしてみる
					for (int i = 0; i < nMichosRepeat; ++i)
					{
						i % 2 == 0 ?
							m_vMichos.push_back({ 1.0f + afScales[i], 1.0f - afScales[i], 1.0f + afScales[i] }) :
							m_vMichos.push_back({ 1.0f - afScales[i], 1.0f + afScales[i], 1.0f - afScales[i] });
					}

					// 最後に元に戻す
					m_vMichos.push_back(VEC3_ONE_INIT);
				});
		}

		return true;
	};

	// 状態：ジャンプ
	m_afpExecuteState[static_cast<unsigned char>(State::JUMP)] =
		[this]() -> bool
	{
		// 重力加速
		bool bLanding = Gravity();

		// 地面に到達したら通常状態へ
		if (bLanding)
		{
			Change(State::DEFAULT, 0, [this]() -> void
				{
					// みちょ数
					const int nMichosRepeat = 5;

					// みちょっと幅
					std::array<float, nMichosRepeat> afScales =
					{
						0.25f, 0.20f, 0.15f, 0.10f, -0.15f
					};

					// みちょっとしてみる
					for (int i = 0; i < nMichosRepeat; ++i)
					{
						i % 2 == 0 ?
							m_vMichos.push_back({ 1.0f + afScales[i], 1.0f - afScales[i], 1.0f + afScales[i] }) :
							m_vMichos.push_back({ 1.0f - afScales[i], 1.0f + afScales[i], 1.0f - afScales[i] });
					}

					// 最後に元に戻す
					m_vMichos.push_back(VEC3_ONE_INIT);
				});
		}

		return true;
	};

	// 状態：オシリ
	m_afpExecuteState[static_cast<unsigned char>(State::HIP)] =
		[this]() -> bool
	{
		return true;
	};

	// 状態：ダメージ
	m_afpExecuteState[static_cast<unsigned char>(State::DAMAGE)] =
		[this]() -> bool
	{
		bool a = Hit();
		if (a)
		{
			Change(State::DEFAULT, 0, [this]() -> void {});
		}
		return true;
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
	// 数値編集
	ValueEdit();

	// かんたんステートの実行
	m_afpExecuteState[static_cast<unsigned char>(m_State)]();

	// 振動再生
	PlayWave();

	// 目標位置：毎回、加速度を加える
	m_PosTarget += m_Velocity;

	// 現在サイズ -> 目標サイズ : 指数減衰
	Vec3 Size = GetSize();
	ExponentialDecay(Size, m_SizeTarget, COEF_CORRECT_TARGET);
	SetSize(Size);

	// 現在向き -> 目標向き : 指数減衰
	Vec3 Rot = GetRot();
	NormalizeAngleToDest(Rot.y, m_RotTarget.y);
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

//============================================================================
// かんたん状態遷移
//============================================================================
void CPlayer::Change(State State, int nLimit, std::function<void()> fpOpt)
{
	// かんたんカウント
	++m_nFrame;

	// 規定フレーム経過で
	if (m_nFrame >= nLimit)
	{
		// 変更時、ついでに必要な処理
		if (fpOpt)
		{
			fpOpt();
		}

		// 状態を指定したものへ変更
		m_nFrame = 0;
		m_State  = State;
	}
}

//============================================================================
// 移動
//============================================================================
void CPlayer::Move(float fSpeed)
{
	// 入力方向を取得
	std::optional<float> opDir = CInputManager::s_fpGetInputDir();

	// 入力があるなら
	if (opDir)
	{
		// 加速度：XZ軸：方向に沿って単位ベクトルに速度係数を掛けたものを設定
		m_Velocity.x = sinf(opDir.value()) * fSpeed;
		m_Velocity.z = cosf(opDir.value()) * fSpeed;

		// 目標向き：Y軸：移動方向に向くようにする
		m_RotTarget.y = atan2f(-m_Velocity.x, -m_Velocity.z);
	}

	// 加速度：XZ軸：時間に伴い減衰していく
	ExponentialDecay(m_Velocity.x, 0.0f, COEF_BRAKING);
	ExponentialDecay(m_Velocity.z, 0.0f, COEF_BRAKING);
}

//============================================================================
// 重力加速
//============================================================================
bool CPlayer::Gravity()
{
	// 加速度：Y軸：下方向へ加速度を増加
	m_Velocity.y += COEF_GRAVITY;

	// このフレーム、地面に到達するなら
	if (m_PosTarget.y + m_Velocity.y < 0.0f)
	{
		// 加速度　：Y軸：停止
		// 目標位置：Y軸：地面に合わせる
		m_Velocity.y  = 0.0f;
		m_PosTarget.y = 0.0f;

		return true;
	}

	return false;
}

//============================================================================
// 振動再生
//============================================================================
void CPlayer::PlayWave()
{
	// 指定が何も無ければ処理しない
	if (m_vMichos.empty())
	{
		return;
	}

	// 先頭要素の参照
	const Vec3& front = m_vMichos.front();

	// 目標サイズ：みちょっとにする
	m_SizeTarget = front;

	// 現在のサイズを取得
	const Vec3& Size = GetSize();

	// 近似値を設定する
	const float fε = 0.1f;

	// 現在のサイズと目標サイズが十分近ければ
	if (fabsf(Size.x - m_SizeTarget.x) < fabsf(fε) &&
		fabsf(Size.y - m_SizeTarget.y) < fabsf(fε) &&
		fabsf(Size.z - m_SizeTarget.z) < fabsf(fε))
	{
		// 先頭要素を削除
		m_vMichos.pop_front();
	}
}

//============================================================================
// 攻撃当たった処理
//============================================================================
bool CPlayer::Hit()
{
	static int Num = 0;
	Num++;

	if (Num > 120)
	{
		Num = 0;
		return true;
	}
	return false;
}

//============================================================================
// 数値編集
//============================================================================
void CPlayer::ValueEdit()
{
	// 静的メンバ変数を初期値として、全てコピー
	float fCoefCrorrectTarget = CPlayer::COEF_CORRECT_TARGET;
	float fCoefGravity        = CPlayer::COEF_GRAVITY;
	float fCoefTriggerJump    = CPlayer::COEF_TRIGGER_JUMP;
	float fCoefMoveSpeed      = CPlayer::COEF_MOVE_SPEED;
	float fCoefMoveSpeedAir   = CPlayer::COEF_MOVE_SPEED_AIR;
	float fCoefBraking        = CPlayer::COEF_BRAKING;

	// 変化量
	const float fSpeed = 0.01f;

	// ImGuiで編集
	MIS::MyImGuiShortcut_BeginWindow(reinterpret_cast<const char*>(u8"プレイヤーの各種パラメータ操作"));
	{
		ImGui::DragFloat("CrorrectTarget", &fCoefCrorrectTarget, fSpeed, fSpeed,  1.0f);
		ImGui::DragFloat("Gravity",        &fCoefGravity,        fSpeed, FLT_MIN, 0.0f);
		ImGui::DragFloat("TriggerJump",    &fCoefTriggerJump,    fSpeed, fSpeed,  FLT_MAX);
		ImGui::DragFloat("MoveSpeed",      &fCoefMoveSpeed,      fSpeed, fSpeed,  FLT_MAX);
		ImGui::DragFloat("MoveSpeedAir",   &fCoefMoveSpeedAir,   fSpeed, fSpeed,  FLT_MAX);
		ImGui::DragFloat("Braking",        &fCoefBraking,        fSpeed, fSpeed,  FLT_MAX);

		// サイズを出力
		const Vec3& Size = GetSize();
		ImGui::Text("Size:(%.2f, %.2f, %.2f)", Size.x, Size.y, Size.z);

		// 向きを出力
		const Vec3& Rot = GetRot();
		ImGui::Text("Rot:(%.2f, %.2f, %.2f)", Rot.x, Rot.y, Rot.z);
		
		// 座標を出力
		const Vec3& Pos = GetPos();
		ImGui::Text("Pos:(%.2f, %.2f, %.2f)", Pos.x, Pos.y, Pos.z);

		// 加速度を出力
		ImGui::Text("Velocity:(%.2f, %.2f, %.2f)", m_Velocity.x, m_Velocity.y, m_Velocity.z);

		// ステートを出力
		ImGui::Text("State:%s", ToString(m_State));
	}
	ImGui::End();

	// 編集した値をメンバ変数に反映
	CPlayer::COEF_CORRECT_TARGET = fCoefCrorrectTarget;
	CPlayer::COEF_GRAVITY        = fCoefGravity;
	CPlayer::COEF_TRIGGER_JUMP   = fCoefTriggerJump;
	CPlayer::COEF_MOVE_SPEED     = fCoefMoveSpeed;
	CPlayer::COEF_MOVE_SPEED_AIR = fCoefMoveSpeedAir;
	CPlayer::COEF_BRAKING        = fCoefBraking;
}

//============================================================================
// 現在のステートをみたい
//============================================================================
const char* CPlayer::ToString(State s)
{
	std::vector<const char*> StateName = { "DEFAULT","JUMP","HIP","DAMAGE" };

	return StateName[static_cast<int>(s)];
}