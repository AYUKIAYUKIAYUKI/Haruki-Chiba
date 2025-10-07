//============================================================================
// 
// �^�C�g�� [title.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "title.h"
#include "sound.h"

// ���͎擾�p
#include "renderer.h"
#include "input.manager.h"

// �V�[���J�ڗp
#include "tutorial.h"
#include "scene.manager.h"
#include "object.manager.h"

// �I�u�W�F�N�g�����p
#include "object.2D.h"
#include "texture.manager.h"
#include "object.X.h"

//****************************************************
// �������O��Ԃ̒�`
//****************************************************
namespace
{
	// �I�u�W�F�N�g�̃|�C���^ -> �����o�ȂǂɎ������������OK
	static CObject2D* pSample2D = nullptr;
	static CObjectX*  pSampleX  = nullptr;

	// �T���v������������_2D
	auto fpSampleInitialize_2D = []() -> void
	{
		// 2D�I�u�W�F�N�g�𐶐�
		pSample2D = CObject::Create<CObject2D>(OBJ::TYPE::NONE, OBJ::LAYER::DEFAULT, CObject2D::s_fpDefaultFactory);
	};

	// �T���v���X�V����_2D
	auto fpSampleUpdate_2D = []() -> void
	{
		using namespace useful;

		if (MIS::MyImGuiShortcut_BeginWindow(reinterpret_cast<const char*>(u8"2D")))
		{
			if (pSample2D)
			{
				// ���l���擾���āc
				Vec2  Size    = pSample2D->GetSize();
				float fRot    = pSample2D->GetRot();
				Vec2  Pos     = pSample2D->GetPos();
				float fWidth  = pSample2D->GetTexWidth();
				float fHeight = pSample2D->GetTexHeight();
				int   nU      = pSample2D->GetNowPatternU();
				int   nV      = pSample2D->GetNowPatternV();

				// ���l�̑��쑬�x
				const float fDragSpeed = 0.5f;

				// �G�f�B�b�g�Ȃǂ��āc
				ImGui::Text(reinterpret_cast<const char*>(u8"2D�I�u�W�F�N�g�̐��l����̃T���v�������ł�"));

				// �T�C�Y
				MIS::MyImGuiShortcut_DragVector("2D Sample Size", Size, fDragSpeed);
				MIS::MyImGuiShortcut_ResetVector("2D Sample Size", Size, 1.0f);

				// ����
				ImGui::Text("2D Sample Rot");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(-1.0f);
				ImGui::DragFloat("## 2D Sample Rot", &fRot, fDragSpeed * 0.1f);
				ImGui::Text(reinterpret_cast<const char*>(u8"���Z�b�g : "));
				ImGui::SameLine();
				if (ImGui::Button(("Z"), { -1.0f, 25.0f }))
				{
					fRot = 0.0f;
				}

				// �ʒu
				MIS::MyImGuiShortcut_DragVector("2D Sample Pos", Pos, fDragSpeed);
				MIS::MyImGuiShortcut_ResetVector("2D Sample Pos", Pos, 0.0f);

				// ��������
				ImGui::Text("2D Sample Width");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(-1.0f);
				ImGui::DragFloat("## 2D Sample Width", &fWidth, fDragSpeed / 5.0f);
				ImGui::Text(reinterpret_cast<const char*>(u8"���Z�b�g : "));
				ImGui::SameLine();
				if (ImGui::Button(("Width"), { -1.0f, 25.0f }))
				{
					fWidth = 0.0f;
				}

				// �c������
				ImGui::Text("2D Sample Height");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(-1.0f);
				ImGui::DragFloat("## 2D Sample Height", &fHeight, fDragSpeed / 5.0f);
				ImGui::Text(reinterpret_cast<const char*>(u8"���Z�b�g : "));
				ImGui::SameLine();
				if (ImGui::Button(("Height"), { -1.0f, 25.0f }))
				{
					fHeight = 0.0f;
				}

				// ���݂̃e�N�X�`����
				ImGui::InputInt("2D Sample Pattern U", &nU, 1);
				ImGui::InputInt("2D Sample Pattern V", &nV, 1);

				// �����������̂𔽉f����
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

	// �T���v������������_X
	auto fpSampleInitialize_X = []() -> void
	{
		// �I�u�W�F�N�g�𐶐�
		pSampleX = CObject::Create<CObjectX>(OBJ::TYPE::NONE, OBJ::LAYER::DEFAULT, CObjectX::s_fpDefaultFactory);
	};

	// �T���v���X�V����_X
	auto fpSampleUpdate_X = []() -> void
	{
		using namespace useful;

		if (MIS::MyImGuiShortcut_BeginWindow(reinterpret_cast<const char*>(u8"X : ��Twitter")))
		{
			if (pSampleX)
			{
				// ���l���擾���āc
				Vec3 Size = pSampleX->GetSize();
				Vec3 Rot  = pSampleX->GetRot();
				Vec3 Pos  = pSampleX->GetPos();

				// ���l�̑��쑬�x
				const float fDragSpeed = 0.1f;

				// �G�f�B�b�g�Ȃǂ��āc
				ImGui::Text(reinterpret_cast<const char*>(u8"X�I�u�W�F�N�g�̐��l����̃T���v�������ł�"));

				// �T�C�Y
				MIS::MyImGuiShortcut_DragVector("X Sample Size", Size, fDragSpeed);
				MIS::MyImGuiShortcut_ResetVector("X Sample Size", Size, 1.0f);

				// ����
				MIS::MyImGuiShortcut_DragVector("X Sample Rot", Rot, fDragSpeed);
				MIS::MyImGuiShortcut_ResetVector("X Sample Rot", Rot, 0.0f);

				// �ʒu
				MIS::MyImGuiShortcut_DragVector("X Sample Pos", Pos, fDragSpeed);
				MIS::MyImGuiShortcut_ResetVector("X Sample Pos", Pos, 0.0f);

				// �����������̂𔽉f����
				pSampleX->SetSize(Size);
				pSampleX->SetRot(Rot);
				pSampleX->SetPos(Pos);
			}
		}

		// X : ��Twitter
		ImGui::End();
	};
}

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CTitle::CTitle()
	: CScene()
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTitle::~CTitle()
{
	// �I������
	Finalize();
}

//============================================================================
// �X�V����
//============================================================================
void CTitle::Update()
{
#ifdef _DEBUG
	CRenderer::RefInstance().AddText("�^�C�g��", 1);
#endif // _DEBUG

	/* �X�V�����̃T���v�� */
	fpSampleUpdate_2D();
	fpSampleUpdate_X();

	// ���̃V�[���֑J��
	if (CInputManager::RefInstance().GetKeyboard()->GetTrigger(DIK_RETURN))
	{
		Change();
	}
}

//============================================================================
// �`�揈��
//============================================================================
void CTitle::Draw()
{}

//============================================================================
// ���̃V�[����
//============================================================================
void CTitle::Change()
{
	// �S�I�u�W�F�N�g����\��
	CObjectManager::RefInstance().SetDeathAllObject();

	// �T�E���h by AKR TNK
	CSound::GetInstance()->Stop(CSound::LABEL::BGM_0);

	// �`���[�g���A���V�[���֕ύX
	up<CTutorial> upScene = CreateUniquePtr<CTutorial>();
	CSceneManager::RefInstance().ChangeScene(std::move(upScene));
}

//============================================================================
// ����������
//============================================================================
bool CTitle::Initialize()
{
	// �T�E���h by AKR TNK
	CSound::GetInstance()->Play(CSound::LABEL::BGM_0);

	// �J������I��
	CCamera_Manager::RefInstance().SetSelectKey("Title");

	/* �����������̃T���v�� */
	fpSampleInitialize_2D();
	fpSampleInitialize_X();

	return true;
}

//============================================================================
// �I������
//============================================================================
void CTitle::Finalize()
{}