//============================================================================
// 
// カメラマネージャー [camera_manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "camera_manager.h"

// デバイス取得用
#include "renderer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

////============================================================================
//// 実行 - 基底
////============================================================================
//bool CCamera_Manager::TRANSITION::Execute(CCamera_Manager*& cameraManager)
//{
//	throw std::runtime_error("'TRANSITION::Execute' was Failed : " + static_cast<std::string>(typeid(cameraManager).name()));
//}
//
////============================================================================
//// 変更 - 基底
////============================================================================
//void CCamera_Manager::TRANSITION::Change(TRANSITION*& r)
//{
//	throw std::runtime_error("'TRANSITION::Change' was Failed : " + static_cast<std::string>(typeid(r).name()));
//}
//
////============================================================================
//// トランジションの更新処理
////============================================================================
//void CCamera_Manager::TRANSITION::UpdateTransition(CCamera_Manager*& cameraManager)
//{
//	// トランジション情報のインスタンスの参照
//	TRANSITION*& r = cameraManager->m_pTransition;
//
//	if (!r)
//	{
//		return;
//	}
//
//	if (r->Execute(cameraManager))
//	{
//		r->Change(r);
//	}
//}
//
////============================================================================
//// 実行 - イントロ
////============================================================================
//bool CCamera_Manager::INTRO::Execute(CCamera_Manager*& cameraManager)
//{
//	// マネージャーのレジストリのコンテナを参照
//	std::map<std::string, CCamera*>& rResource = cameraManager->m_upRegistry->RefDynamicResource();
//
//	// 現在選択されているカメラのキーをコピー
//	const std::string& SelectKey = cameraManager->m_SelectKey;
//
//	// 現在のキーに対応するインスタンスが保有されていない
//	if (rResource.find(SelectKey) == rResource.end())
//	{
//		throw std::runtime_error("'SetTransition' was Failed at FindOld : " + SelectKey);
//	}
//
//	// 遷移予定のキーに対応するインスタンスが保有されていない
//	if (rResource.find(m_NextKey) == rResource.end())
//	{
//		throw std::runtime_error("'SetTransition' was Failed at FindNew : " + m_NextKey);
//	}
//
//	// 現在のカメラと、遷移予定のカメラの参照
//	CCamera
//		*& rOld = rResource.at(SelectKey),
//		*& rNew = rResource.at(m_NextKey);
//
//	// ① - 現在のカメラのパラメータを
//	rOld->SetRotTarget(rNew->GetRot());
//	rOld->SetPosTarget(rNew->GetPos());
//	rOld->SetDistanceTarget(rNew->GetDistance());
//	rOld->SetUpAdjustTarget(rNew->GetUpAdjust());
//
//	// 閾値
//	const float fThreshold = 0.1f;
//
//	// ある程度パラメータが目標値に迫った場合状態変更
//	if (rOld->GetRot().x - rNew->GetRot().x <= fThreshold)
//	{
//		return true;
//	}
//
//	return false;
//}
//
////============================================================================
//// 変更 - イントロ
////============================================================================
//void CCamera_Manager::INTRO::Change(TRANSITION*& r)
//{
//	if (r)
//	{
//		delete r;
//		r = DBG_NEW OUTRO();
//	}
//}
//
////============================================================================
//// 実行 - アウトロ
////============================================================================
//bool CCamera_Manager::OUTRO::Execute(CCamera_Manager*& cameraManager)
//{
//	// 回転
//	RotateTargetYawAxis(cameraManager->RefSelectCamera(), 0.01f);
//
//	return false;
//}
//
////============================================================================
//// 変更 - アウトロ
////============================================================================
//void CCamera_Manager::OUTRO::Change(TRANSITION*& r)
//{
//	if (r)
//	{
//		delete r;
//		r = nullptr;
//	}
//}

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CCamera_Manager::Update()
{
	//// 保有する全カメラの更新処理
	//for (auto& it : m_upRegistry->RefDynamicResource())
	//{
	//	it.second->Update();
	//}

	//// カメラマネージャーのポインタ
	//CCamera_Manager* pCameraManager = this;

	//// トランジションの更新
	//TRANSITION::UpdateTransition(pCameraManager);
}

////============================================================================
//// トランジションのセット
////============================================================================
//void CCamera_Manager::SetTransition(const std::string& key)
//{
//#if 0
//	// すでにトランジション中なら、一旦強制敵に状態を新規生成
//	if (m_pTransition)
//	{
//		//return;
//		delete m_pTransition;
//		m_pTransition = nullptr;
//	}
//
//	// 選択中のキーを渡しイントロを生成
//	m_pTransition = DBG_NEW INTRO(key);
//#else
//	// レジストリのコンテナを参照
//	std::map<std::string, CCamera*>& rResource = m_upRegistry->RefDynamicResource();
//
//	// 現在のキーに対応するインスタンスが保有されていない
//	if (rResource.find(m_SelectKey) == rResource.end())
//	{
//		throw std::runtime_error("'SetTransition' was Failed at FindOld : " + m_SelectKey);
//	}
//
//	// 遷移予定のキーに対応するインスタンスが保有されていない
//	if (rResource.find(key) == rResource.end())
//	{
//		throw std::runtime_error("'SetTransition' was Failed at FindNew : " + key);
//	}
//
//	// 現在のカメラと、遷移予定のカメラの参照
//	CCamera
//		*& rOld = rResource.at(m_SelectKey),
//		*& rNew = rResource.at(key);
//
//	// 遷移予定のカメラの初期値を、現在のパラメータにする
//	rNew->SetRot(rOld->GetRot());
//	rNew->SetPos(rOld->GetPos());
//	rNew->SetDistance(rOld->GetDistance());
//	rNew->SetUpAdjust(rOld->GetUpAdjust());
//
//	// 選択しているカメラのキーを遷移予定のものに変更
//	m_SelectKey = key;
//#endif
//}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CCamera_Manager::CCamera_Manager() :
	m_upRegistry(std::make_unique<CRegistry<CCamera*>>()),
	m_SelectKey("Title")
	//m_pTransition(nullptr)
{}

//============================================================================
// デストラクタ
//============================================================================
CCamera_Manager::~CCamera_Manager()
{
	// 終了処理
	Finalize();
}

//============================================================================
// 初期化処理
//============================================================================
bool CCamera_Manager::Initialize()
{
	// 生成処理用のラムダオブジェクト作成
	std::function<CCamera* (const std::string&)> fpCreate =
		[](const std::string& path) -> CCamera*
	{
		// 必要な追加情報を取得
		const JSON& Json = useful::OpenJsonFileMaybeThrow(path);

		// カメラのインスタンスを生成
		CCamera* pElement = DBG_NEW CCamera(Json);

		// インスタンスのポインタを返す
		return pElement;
	};

	// 解放処理用のラムダオブジェクト作成
	std::function<void(CCamera*&)> fpRelease =
		[](CCamera*& element) noexcept -> void
		{
			// 終了処理無し、リソースの解放のみ
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
// 終了処理
//============================================================================
void CCamera_Manager::Finalize()
{
	// レジストリの終了処理
	m_upRegistry->Finalize();

	//// トランジション情報の解放
	//if (m_pTransition)
	//{
	//	delete m_pTransition;
	//	m_pTransition = nullptr;
	//}
}