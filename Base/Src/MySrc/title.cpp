//============================================================================
// 
// タイトル [title.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "title.h"
#include "sound.h"

// 入力取得用
#include "renderer.h"
#include "input.manager.h"

// シーン遷移用
#include "tutorial.h"
#include "scene.manager.h"
#include "object.manager.h"

// オブジェクト生成用
#include "object.2D.h"
#include "texture.manager.h"
#include "object.X.h"

//****************************************************
// 無名名前空間の定義
//****************************************************
namespace
{
	// オブジェクトのポインタ -> メンバなどに持たせちゃってOK
	static CObject2D* pSample2D = nullptr;
	static CObjectX*  pSampleX  = nullptr;

	// サンプル初期化処理_2D
	auto fpSampleInitialize_2D = []() -> void
	{
		// 2Dオブジェクトを生成
		pSample2D = CObject::Create<CObject2D>(OBJ::TYPE::NONE, OBJ::LAYER::DEFAULT, CObject2D::s_fpDefaultFactory);
	};

	// サンプル更新処理_2D
	auto fpSampleUpdate_2D = []() -> void
	{
		using namespace useful;

		if (MIS::MyImGuiShortcut_BeginWindow(reinterpret_cast<const char*>(u8"2D")))
		{
			if (pSample2D)
			{
				// 数値を取得して…
				Vec2  Size    = pSample2D->GetSize();
				float fRot    = pSample2D->GetRot();
				Vec2  Pos     = pSample2D->GetPos();
				float fWidth  = pSample2D->GetTexWidth();
				float fHeight = pSample2D->GetTexHeight();
				int   nU      = pSample2D->GetNowPatternU();
				int   nV      = pSample2D->GetNowPatternV();

				// 数値の操作速度
				const float fDragSpeed = 0.5f;

				// エディットなどして…
				ImGui::Text(reinterpret_cast<const char*>(u8"2Dオブジェクトの数値操作のサンプル処理です"));

				// サイズ
				MIS::MyImGuiShortcut_DragVector("2D Sample Size", Size, fDragSpeed);
				MIS::MyImGuiShortcut_ResetVector("2D Sample Size", Size, 1.0f);

				// 向き
				ImGui::Text("2D Sample Rot");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(-1.0f);
				ImGui::DragFloat("## 2D Sample Rot", &fRot, fDragSpeed * 0.1f);
				ImGui::Text(reinterpret_cast<const char*>(u8"リセット : "));
				ImGui::SameLine();
				if (ImGui::Button(("Z"), { -1.0f, 25.0f }))
				{
					fRot = 0.0f;
				}

				// 位置
				MIS::MyImGuiShortcut_DragVector("2D Sample Pos", Pos, fDragSpeed);
				MIS::MyImGuiShortcut_ResetVector("2D Sample Pos", Pos, 0.0f);

				// 横分割幅
				ImGui::Text("2D Sample Width");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(-1.0f);
				ImGui::DragFloat("## 2D Sample Width", &fWidth, fDragSpeed / 5.0f);
				ImGui::Text(reinterpret_cast<const char*>(u8"リセット : "));
				ImGui::SameLine();
				if (ImGui::Button(("Width"), { -1.0f, 25.0f }))
				{
					fWidth = 0.0f;
				}

				// 縦分割幅
				ImGui::Text("2D Sample Height");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(-1.0f);
				ImGui::DragFloat("## 2D Sample Height", &fHeight, fDragSpeed / 5.0f);
				ImGui::Text(reinterpret_cast<const char*>(u8"リセット : "));
				ImGui::SameLine();
				if (ImGui::Button(("Height"), { -1.0f, 25.0f }))
				{
					fHeight = 0.0f;
				}

				// 現在のテクスチャ幅
				ImGui::InputInt("2D Sample Pattern U", &nU, 1);
				ImGui::InputInt("2D Sample Pattern V", &nV, 1);

				// いじったものを反映する
				pSample2D->SetSize(Size);
				pSample2D->SetRot(fRot);
				pSample2D->SetPos(Pos);
				pSample2D->SetTexWidth(fWidth);
				pSample2D->SetTexHeight(fHeight);
				pSample2D->SetNowPatternU(nU);
				pSample2D->SetNowPatternV(nV);
			}
		}

		ImGui::End();
	};

	// サンプル初期化処理_X
	auto fpSampleInitialize_X = []() -> void
	{
		// オブジェクトを生成
		pSampleX = CObject::Create<CObjectX>(OBJ::TYPE::NONE, OBJ::LAYER::DEFAULT, CObjectX::s_fpDefaultFactory);
	};

	// サンプル更新処理_X
	auto fpSampleUpdate_X = []() -> void
	{
		using namespace useful;

		if (MIS::MyImGuiShortcut_BeginWindow(reinterpret_cast<const char*>(u8"X : 旧Twitter")))
		{
			if (pSampleX)
			{
				// 数値を取得して…
				Vec3 Size = pSampleX->GetSize();
				Vec3 Rot  = pSampleX->GetRot();
				Vec3 Pos  = pSampleX->GetPos();

				// 数値の操作速度
				const float fDragSpeed = 0.1f;

				// エディットなどして…
				ImGui::Text(reinterpret_cast<const char*>(u8"Xオブジェクトの数値操作のサンプル処理です"));

				// サイズ
				MIS::MyImGuiShortcut_DragVector("X Sample Size", Size, fDragSpeed);
				MIS::MyImGuiShortcut_ResetVector("X Sample Size", Size, 1.0f);

				// 向き
				MIS::MyImGuiShortcut_DragVector("X Sample Rot", Rot, fDragSpeed);
				MIS::MyImGuiShortcut_ResetVector("X Sample Rot", Rot, 0.0f);

				// 位置
				MIS::MyImGuiShortcut_DragVector("X Sample Pos", Pos, fDragSpeed);
				MIS::MyImGuiShortcut_ResetVector("X Sample Pos", Pos, 0.0f);

				// いじったものを反映する
				pSampleX->SetSize(Size);
				pSampleX->SetRot(Rot);
				pSampleX->SetPos(Pos);
			}
		}

		// X : 旧Twitter
		ImGui::End();
	};
}

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CTitle::CTitle()
	: CScene()
{}

//============================================================================
// デストラクタ
//============================================================================
CTitle::~CTitle()
{
	// 終了処理
	Finalize();
}

//============================================================================
// 更新処理
//============================================================================
void CTitle::Update()
{
#ifdef _DEBUG
	CRenderer::RefInstance().AddText("タイトル", 1);
#endif // _DEBUG

	/* 更新処理のサンプル */
	fpSampleUpdate_2D();
	fpSampleUpdate_X();

	// 次のシーンへ遷移
	if (CInputManager::RefInstance().GetKeyboard()->GetTrigger(DIK_RETURN))
	{
		Change();
	}
}

//============================================================================
// 描画処理
//============================================================================
void CTitle::Draw()
{}

//============================================================================
// 次のシーンへ
//============================================================================
void CTitle::Change()
{
	// 全オブジェクト解放予約
	CObjectManager::RefInstance().SetDeathAllObject();

	// サウンド by AKR TNK
	CSound::GetInstance()->Stop(CSound::LABEL::BGM_0);

	// チュートリアルシーンへ変更
	up<CTutorial> upScene = CreateUniquePtr<CTutorial>();
	CSceneManager::RefInstance().ChangeScene(std::move(upScene));
}

//============================================================================
// 初期化処理
//============================================================================
bool CTitle::Initialize()
{
	// サウンド by AKR TNK
	CSound::GetInstance()->Play(CSound::LABEL::BGM_0);

	// カメラを選択
	CCamera_Manager::RefInstance().SetSelectKey("Title");

	/* 初期化処理のサンプル */
	fpSampleInitialize_2D();
	fpSampleInitialize_X();

	return true;
}

//============================================================================
// 終了処理
//============================================================================
void CTitle::Finalize()
{}