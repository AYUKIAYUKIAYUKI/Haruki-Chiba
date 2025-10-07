//============================================================================
// 
// カメラ [camera.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "camera.h"
#include "camera_manager.h"
#include "input.manager.h"
#include "window.h"

// デバイス取得用
#include "renderer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************

// フリーカメラの使用フラグ
bool CCamera::s_bUseFree = false;

// フリーカメラのインスタンス
std::unique_ptr<CCamera> CCamera::s_upFreeCam = std::make_unique<CCamera>();

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CCamera::CCamera() :
	m_Rot(VEC3_ZERO_INIT),
	m_RotTarget(VEC3_ZERO_INIT),
	m_Pos(VEC3_ZERO_INIT),
	m_PosTarget(VEC3_ZERO_INIT),
	m_PosV(VEC3_ZERO_INIT),
	m_PosTargetV(VEC3_ZERO_INIT),
	m_PosR(VEC3_ZERO_INIT),
	m_PosTargetR(VEC3_ZERO_INIT),
	m_fDistance(0.0f),
	m_fDistanceTarget(0.0f),
	m_fUpAdjust(0.0f),
	m_fUpAdjustTarget(0.0f),
	m_VecU(0.0f, 1.0f, 0.0f),
	m_fpOverwriteView(nullptr),
	m_bUseFirstPerson(false),
	m_bUseDecay(false),
	m_bUseControlDrag(true),
	m_bUseHorizon(false),
	m_bUseVertical(false),
	m_fDragMoveSpeed(0.35f),
	m_fDragRotateSpeed(0.075f)
{
	// 行列を初期化
	D3DXMatrixIdentity(&m_MtxProjection);	// プロジェクション行列
	D3DXMatrixIdentity(&m_MtxView);			// ビュー行列
}

//============================================================================
// 変換コンストラクタ
//============================================================================
CCamera::CCamera(const JSON& json) :
	m_Rot(useful::JsonConvertToVec3(json["Rot"])),
	m_RotTarget(useful::JsonConvertToVec3(json["RotTarget"])),
	m_Pos(useful::JsonConvertToVec3(json["Pos"])),
	m_PosTarget(useful::JsonConvertToVec3(json["PosTarget"])),
	m_PosV(VEC3_ZERO_INIT),
	m_PosTargetV(VEC3_ZERO_INIT),
	m_PosR(VEC3_ZERO_INIT),
	m_PosTargetR(VEC3_ZERO_INIT),
	m_fDistance(static_cast<float>(json["Distance"])),
	m_fDistanceTarget(static_cast<float>(json["DistanceTarget"])),
	m_fUpAdjust(static_cast<float>(json["UpAdjust"])),
	m_fUpAdjustTarget(static_cast<float>(json["UpAdjustTarget"])),
	m_VecU(0.0f, 1.0f, 0.0f),
	m_fpOverwriteView(nullptr),
	m_bUseFirstPerson(false),
	m_bUseControlDrag(false),
	m_bUseDecay(static_cast<bool>(json["UseDecay"])),
	m_bUseHorizon(false),
	m_bUseVertical(false),
	m_fDragMoveSpeed(0.35f),
	m_fDragRotateSpeed(0.075f)
{
	// 行列を初期化
	D3DXMatrixIdentity(&m_MtxProjection);	// プロジェクション行列
	D3DXMatrixIdentity(&m_MtxView);			// ビュー行列
}

//============================================================================
// デストラクタ
//============================================================================
CCamera::~CCamera()
{}

//============================================================================
// 初期化処理
//============================================================================
bool CCamera::Initialize()
{
	return 0;
}

//============================================================================
// 更新処理
//============================================================================
void CCamera::Update()
{
	// ビューが上書きされていたらその処理を実行
	if (m_fpOverwriteView)
	{
		m_fpOverwriteView(*this);
	}

	// 目標値への補間
	if (m_bUseDecay)
	{
		CorrectToTarget();
	}

	// 何人称視点のカメラ適応をするか切り替え
	if (m_bUseFirstPerson)
	{
		// フライ操作でのカメラ適応
		CameraTransformFly();
	}
	else
	{
		// オービット操作でのカメラ適応
		CameraTransformOrbit();
	}

#ifdef _DEBUG
	// デバッグ表示
	PrintDebug();
#endif // _DEBUG
}

//============================================================================
// カメラをセット
//============================================================================
void CCamera::SetCamera(D3DXVECTOR3 pos)
{
#if 0	// バッファのクリアはレンダラーに委ねる
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// 画面バッファクリア
	pDev->Clear(0,
		nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);
#endif

	// プロジェクション行列を計算
	CalcMtxProjection();

	// ビュー行列を計算
	CalcMtxView(pos);
}

//============================================================================
// カメラ振動
//============================================================================
void CCamera::SetVibration(float coef)
{
	// 指定された強度でカメラ座標をランダムにずらす
	m_Pos += {
		useful::GetRandomValue<float>() * coef,
		useful::GetRandomValue<float>() * coef,
		useful::GetRandomValue<float>() * coef,
	};
}

//============================================================================
// ビュー上書き状態の取得
//============================================================================
bool CCamera::GetStateOverwriteView()
{
	if (m_fpOverwriteView)
	{
		return true;
	}

	return false;
}

//============================================================================
// フリーカメラ更新
//============================================================================
void CCamera::UpdateFreeCam()
{
#if SWITCH_STARTUP_MODE
	// 目標値への補間を強制
	s_upFreeCam->m_bUseDecay ? 0 : s_upFreeCam->m_bUseDecay = !s_upFreeCam->m_bUseDecay;

	// 操作
	s_upFreeCam->Control();

	// 更新
	s_upFreeCam->Update();
#else
	// ウィンドウのサイズと位置の設定
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

	// ビューモード選択用ウィンドウを表示
	if (ImGui::Begin("##Camera Debug Menu", nullptr, ImGuiWindowFlags_NoTitleBar))
	{
		if (ImGui::Checkbox(reinterpret_cast<const char*>(u8"フリーカメラ"), &s_bUseFree))
		{
			s_upFreeCam->m_bUseDecay ? 0 : s_upFreeCam->m_bUseDecay = !s_upFreeCam->m_bUseDecay;
		}
	}
	ImGui::End();

	// フリーカメラが有効なら
	if (s_bUseFree)
	{
		// 操作
		s_upFreeCam->Control();

		// 更新
		s_upFreeCam->Update();
	}
#endif
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// カメラ操作処理
//============================================================================
void CCamera::Control()
{
	// ImGuiによる操作
	ControlImGui();

	// 何人称視点の操作をするか切り替え
	if (m_bUseControlDrag)
	{
		if (m_bUseFirstPerson)
		{
			// フライ操作
			ControlFly();
		}
		else
		{
			// オービット操作
			ControlOrbit();
		}
	}

	// ビューリセット
	if (CInputManager::RefInstance().GetKeyboard()->GetTrigger(DIK_F1))
	{
		// カメラ向きを正面に
		m_Rot.x = 0.0f;
		m_Rot.y = 0.0f;
		m_RotTarget.x = 0.0f;
		m_RotTarget.y = 0.0f;
	}

	// 水平ビューが有効なら
	if (m_bUseHorizon)
	{
		// 俯瞰度合いも無視し必ず真横からの向きに
		m_Rot.x = 0.0f;
		m_RotTarget.x = 0.0f;
		m_fUpAdjustTarget = 0.0f;
	}

	// 垂直ビューが有効なら
	if (m_bUseVertical)
	{
		// 0.0f ～ 0.5f
		float f = 0.48f;

		// 必ず天頂から見おろす向きに
		m_Rot.x = D3DX_PI * -f;
		m_RotTarget.x = D3DX_PI * -f;
		m_fUpAdjustTarget = 0.0f;
	}
}

//============================================================================
// ImGuiでの操作処理
//============================================================================
void CCamera::ControlImGui()
{
	// フリーカメラ操作用ウィンドウのサイズと位置の設定
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

	// フリーカメラ操作用ウィンドウの表示
	if (ImGui::Begin(reinterpret_cast<const char*>(u8"フリーカメラ操作")))
	{
		// ウィンドウとウィジェットの幅をコピー
		float fWidth = ImGui::GetContentRegionAvail().x, fSeparate = ImGui::GetStyle().ItemSpacing.x;

		// リセットボタンのサイズ
		const float fResetButtonWidth = (fWidth + -fSeparate * 3.0f) / 4.0f;

		// 何人称視点かの切り替え
		ImGui::Checkbox(reinterpret_cast<const char*>(u8"現在の視点 :"), &m_bUseFirstPerson);

		// 視点情報の出力
		ImGui::SameLine();
		if (m_bUseFirstPerson)
		{
			ImGui::Text(reinterpret_cast<const char*>(u8"一人称視点"));
		}
		else
		{
			ImGui::Text(reinterpret_cast<const char*>(u8"三人称視点"));
		}

		// ドラッグ操作の有効化
		ImGui::Checkbox(reinterpret_cast<const char*>(u8"ドラッグ操作"), &m_bUseControlDrag);

		// ビューモードの切り替え
		if (ImGui::Checkbox(reinterpret_cast<const char*>(u8"水平ビュー"), &m_bUseHorizon))
		{
			// 水平ビューの有効化のため、垂直ビューを強制無効化
			m_bUseVertical = false;
		}
		ImGui::SameLine();
		if (ImGui::Checkbox(reinterpret_cast<const char*>(u8"垂直ビュー"), &m_bUseVertical))
		{
			// 垂直ビューの有効化のため、水平ビューを強制無効化
			m_bUseHorizon = false;
		}
		ImGui::Separator();

		// ドラッグ操作時の係数変更
		ImGui::DragFloat(reinterpret_cast<const char*>(u8"移動速度"), &m_fDragMoveSpeed, 0.1f, 0.01f, FLT_MAX);

		// カメラ目標座標操作
		const char* PosTargetLabel = reinterpret_cast<const char*>(u8"目標座標 :");
		MIS::MyImGuiShortcut_DragVector(PosTargetLabel, m_PosTarget, 1.0f);
		MIS::MyImGuiShortcut_ResetVector(PosTargetLabel, m_PosTarget);
		ImGui::Separator();

		// カメラ目標向き操作
		const char* RotTargetLabel = reinterpret_cast<const char*>(u8"目標向き :");
		MIS::MyImGuiShortcut_DragVector(RotTargetLabel, m_RotTarget, 0.01f);
		MIS::MyImGuiShortcut_ResetVector(RotTargetLabel, m_RotTarget);
		ImGui::Separator();

		// 目標間距離
		ImGui::DragFloat(reinterpret_cast<const char*>(u8"間距離"), &m_fDistanceTarget);

		// 目標俯瞰度合い
		ImGui::DragFloat(reinterpret_cast<const char*>(u8"ふかん度合い"), &m_fUpAdjustTarget);

		// 使用してるカメラ側に同期
		ImGui::SetNextItemWidth(-1.0f);
		if (ImGui::Button(reinterpret_cast<const char*>(u8"現在の通常カメラに同期")))
		{
			CCamera& refCam = CCamera_Manager::RefInstance().RefSelectCamera();
			m_Rot = refCam.GetRot();
			m_RotTarget = refCam.GetRotTarget();
			m_Pos = refCam.GetPos();
			m_PosTarget = refCam.GetPosTarget();
			m_fDistance = refCam.GetDistance();
			m_fDistanceTarget = refCam.GetDistanceTarget();
			m_fUpAdjust = refCam.GetUpAdjust();
			m_fUpAdjustTarget = refCam.GetUpAdjustTarget();
		}
	}
	ImGui::End();
}

//============================================================================
// フライ操作
//============================================================================
void CCamera::ControlFly()
{
	// マウスのポインタとその移動量を取得
	CInputMouse* const pMouse = CInputManager::RefInstance().GetMouse();
	D3DXVECTOR2 MouseMove = pMouse->GetMouseMove();

	// 移動量を単位ベクトルに変換
	D3DXVec2Normalize(&MouseMove, &MouseMove);

	if (pMouse->GetPress(0) && MouseMove != VEC2_ZERO_INIT)
	{
		// 現在のビューに対する前方向ベクトルを作成
		Vec3 ViewDirectionForward =
		{
			sinf(m_Rot.y) * cosf(m_Rot.x),
			sinf(m_Rot.x),
			cosf(m_Rot.y) * cosf(m_Rot.x)
		};

		// 現在のビューに対する右方向ベクトルを作成
		Vec3 ViewDirectionRight =
		{
			cosf(m_Rot.y),
			0.0f,
			-sinf(m_Rot.y)
		};

		// ビュー方向に対しフライ操作した移動量を加算
		Vec3 FlyMove =
			ViewDirectionForward * (-MouseMove.y * m_fDragMoveSpeed)	// 前後
			+ ViewDirectionRight * (MouseMove.x * m_fDragMoveSpeed);	// 左右

		// 最後に目標位置に移動量を加算
		m_PosTarget += FlyMove;
	}
	else if (CInputManager::RefInstance().GetMouse()->GetPress(1))
	{
		// 向き上下左右
		m_RotTarget.x += -MouseMove.y * m_fDragRotateSpeed;
		m_RotTarget.y += MouseMove.x * m_fDragRotateSpeed;
	}
}

//============================================================================
// オービット操作
//============================================================================
void CCamera::ControlOrbit()
{
	// マウスのポインタとその移動量を取得
	CInputMouse* const pMouse = CInputManager::RefInstance().GetMouse();
	D3DXVECTOR2 MouseMove = pMouse->GetMouseMove();

	// 移動量を単位ベクトルに変換
	D3DXVec2Normalize(&MouseMove, &MouseMove);

	if (pMouse->GetPress(0) && MouseMove != VEC2_ZERO_INIT)
	{
		// 現在のビューに対する右方向ベクトルを作成
		Vec3 ViewDirectionRight =
		{
			cosf(m_Rot.y),
			0.0f,
			-sinf(m_Rot.y)
		};

		// 現在のビューに対する上方向ベクトルを作成
		Vec3 ViewDirectionUp = { 0.0f, 1.0f, 0.0f };

		// オービット軌道の対象地点を、ビュー方向に対しパン操作
		Vec3 PanMove =
			ViewDirectionRight * (MouseMove.x * m_fDragMoveSpeed)	// 左右
			+ ViewDirectionUp * (-MouseMove.y * m_fDragMoveSpeed);	// 上下

		// 最後に目標位置に移動量を加算
		m_PosTarget += PanMove;
	}
	else if (CInputManager::RefInstance().GetMouse()->GetPress(1))
	{
		// 向き上下左右
		m_RotTarget.x += -MouseMove.y * m_fDragRotateSpeed;
		m_RotTarget.y += MouseMove.x * m_fDragRotateSpeed;
	}

	// 距離リセット
	if (CInputManager::RefInstance().GetMouse()->GetTrigger(2))
	{
		m_fDistanceTarget = 300.0f;
	}

	// ズームイン / アウト
	m_fDistanceTarget += -CInputManager::RefInstance().GetMouse()->GetWheelScroll() * 0.01f;

	// ズームインの下限を設定
	if (m_fDistanceTarget < 10.0f)
	{
		m_fDistanceTarget = 10.0f;
	}
}

//============================================================================
// 目標値への補間
//============================================================================
void CCamera::CorrectToTarget()
{
	// ピッチ角の制限
	AutoRestrictPitch();

	// ヨー角の制限
	useful::WrapAngleToRange(m_RotTarget.y, m_Rot.y);

	// 目標向きと実値の向きの差を制限
	//useful::NormalizeAngleToDest(, , D3DX_PI);

	// 向き
	ExponentialDecay(m_Rot, m_RotTarget, COEF_ADJUST);

	// 座標
	ExponentialDecay(m_Pos, m_PosTarget, COEF_ADJUST);

	// 間距離
	ExponentialDecay(m_fDistance, m_fDistanceTarget, COEF_ADJUST);

	// 俯瞰度合い
	ExponentialDecay(m_fUpAdjust, m_fUpAdjustTarget, COEF_ADJUST);
}

//============================================================================
// ピッチ角の範囲を制限
//============================================================================
void CCamera::AutoRestrictPitch()
{
	// 0.0f ～ 0.5f
	float fCoeff = 0.48f;

	if (m_RotTarget.x > D3DX_PI * fCoeff)
	{
		m_RotTarget.x = D3DX_PI * fCoeff;
	}
	else if (m_RotTarget.x < -D3DX_PI * fCoeff)
	{
		m_RotTarget.x = -D3DX_PI * fCoeff;
	}
}

//============================================================================
// フライ操作でのカメラ適応
//============================================================================
void CCamera::CameraTransformFly()
{
	// 視点はカメラ位置に
	m_PosV = m_Pos;

	// 注視点を対象物の前方に
	m_PosR =
	{
		m_Pos.x + sinf(m_Rot.y) * cosf(m_Rot.x),
		m_Pos.y + sinf(m_Rot.x),
		m_Pos.z + cosf(m_Rot.y) * cosf(m_Rot.x)
	};
}

//============================================================================
// オービット操作でのカメラ適応
//============================================================================
void CCamera::CameraTransformOrbit()
{
	// 視点は対象物の後方に
	m_PosV =
	{
		m_Pos.x - (sinf(m_Rot.y) * cosf(m_Rot.x) * (m_fDistance)),
		m_Pos.y - (sinf(m_Rot.x) * (m_fDistance)),
		m_Pos.z - (cosf(m_Rot.y) * cosf(m_Rot.x) * (m_fDistance))
	};

	// 注視点を対象物の前方に
	m_PosR =
	{
		m_Pos.x + (sinf(m_Rot.y) * cosf(m_Rot.x) * m_fDistance),
		m_Pos.y + sinf(m_Rot.x) * m_fDistance,
		m_Pos.z + (cosf(m_Rot.y) * cosf(m_Rot.x) * m_fDistance)
	};
}

//============================================================================
// プロジェクション行列計算
//============================================================================
void CCamera::CalcMtxProjection()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::RefInstance().GetDevice();

	// プロジェクション行列の初期化
	D3DXMatrixIdentity(&m_MtxProjection);

	// プロジェクション行列を生成
#if 1
	// 透視投影
	D3DXMatrixPerspectiveFovLH(
		&m_MtxProjection,
		D3DXToRadian(45.0f),
		CWindow::FWINDOW_WIDTH / CWindow::FWINDOW_HEIGHT,
		10.0f,
		5000.0f);
#else
	// 平行投影
	D3DXMatrixOrthoLH(&m_MtxProjection,
		(float)SCREEN_WIDTH * 0.25f,
		(float)SCREEN_HEIGHT * 0.25f,
		10.0f,
		5000.0f);
#endif

	// プロジェクション行列の設定
	pDev->SetTransform(
		D3DTS_PROJECTION,
		&m_MtxProjection);
}

//============================================================================
// ビュー行列計算
//============================================================================
void CCamera::CalcMtxView(D3DXVECTOR3 /*pos*/)
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::RefInstance().GetDevice();

	// ビュー行列の初期化
	D3DXMatrixIdentity(&m_MtxView);

	// 視点座標の調整用
	Vec3
		posV = m_PosV,	// 視点
		posR = m_PosR;	// 注視点

	// 俯瞰度合いを加算
	posV.y += m_fUpAdjust;

#if 0
	// 初期座標以外を渡された場合その視点をコピー
	if (pos != VEC3_ZERO_INIT)
	{
		posV = pos;
		posV.y += -30.0f;
		posR = VEC3_ZERO_INIT;
	}
#endif

	// ビュー行列の生成
	D3DXMatrixLookAtLH(
		&m_MtxView,
		&posV,
		&posR,
		&m_VecU);

	// ビュー行列の設定
	pDev->SetTransform(
		D3DTS_VIEW,
		&m_MtxView);
}

//============================================================================
// デバッグ表示
//============================================================================
void CCamera::PrintDebug()
{
	//// ロガーのインスタンスを取得
	//auto& rLogger = CLogger::RefInstance();

	//// 同期ログを出力
	//rLogger.AddSynchronization(std::to_string(m_RotTarget.x) + " :Pitch Angle(RotTarget)");
	//rLogger.AddSynchronization(std::to_string(m_RotTarget.y) + " :Yaw Angle(RotTarget)");
	//rLogger.AddSynchronization(std::to_string(m_PosTarget.x) + " :PosTarget");
	//rLogger.AddSynchronization(std::to_string(m_PosTarget.y) + " :PosTarget");
	//rLogger.AddSynchronization(std::to_string(m_PosTarget.z) + " :PosTarget");
	//rLogger.AddSynchronization(std::to_string(m_fDistanceTarget) + " :DistanceTarget");
	//rLogger.AddSynchronization(std::to_string(m_fUpAdjustTarget) + " :UpAdjustTarget");
}