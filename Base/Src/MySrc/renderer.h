//============================================================================
// 
// �����_���[�A�w�b�_�[�t�@�C�� [renderer.h]
// Author : ���c����
// 
//============================================================================

#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "singleton.h"
#include "camera_manager.h"

//****************************************************
// �R���Z�v�g�̒�`
//****************************************************

// �X�V�\�ł��邱�Ƃ̃R���Z�v�g
template <typename T>
concept Updatable = requires(T t)
{
	{ t->Draw() } -> std::same_as<void>;
};

// �X�V�\�R���e�i�ł��邱�Ƃ̃R���Z�v�g
template <typename T>
concept UpdatableContainer = requires(T t)
{
	{ *t.begin() } -> Updatable;
};

// �`��\�ł��邱�Ƃ̃R���Z�v�g
template <typename T>
concept Drawable = requires(T t)
{
	{ t->Draw() } -> std::same_as<void>;
};

// �`��\�R���e�i�ł��邱�Ƃ̃R���Z�v�g
template <typename T>
concept DrawableContainer = requires(T t)
{
	{ *t.begin() } -> Drawable;
};

//****************************************************
// �����_���[�N���X
//****************************************************
class CRenderer final : public CSingleton<CRenderer>
{
	//****************************************************
	// �t�����h�錾
	//****************************************************
	friend struct std::default_delete<CRenderer>;
	friend CRenderer& CSingleton<CRenderer>::RefInstance();

	//****************************************************
	// �ÓI�����o�萔�̒�` (����J)
	//****************************************************
	static constexpr float FOG_BWGIN = 1500.0f; // �t�H�O�̏����n�_
	static constexpr float FOG_END   = 2500.0f; // �t�H�O�̏����I�_

public:

	//****************************************************
	// function
	//****************************************************
	
	// ����������
	bool Initialize(HWND hWnd, BOOL bWindiw);
	bool Initialize() { return true; }

	// �I������
	void Finalize();

	/*
		�� �c �R���Z�v�g�ɉ������R���e�i�̂ݓn����Ȃ炻�̂܂ܔ͈�for�ŏ���
		�� �c �����P�ʂœn���ă����_���[�̏������ɋ���ł��炤����
	*/

	// �X�V������
	template <UpdatableContainer T>
	void Update_��(const T& t);

	// �X�V������
	template <typename T>
	void Update_��(T&& fp);

	// �`�揈����
	template <DrawableContainer T>
	void Draw_��(const T& t);

	// �`�揈����
	template <typename T>
	void Draw_��(T&& fp);

	//****************************************************
	// inline function
	//****************************************************

	// �f�o�C�X�̎擾
	inline       LPDIRECT3DDEVICE9 GetDevice()            { return useful::PtrCheck(m_pDevice, "Device"); }
	inline const LPDIRECT3DDEVICE9 GetDeviceConst() const { return useful::PtrCheck(m_pDevice, "Device"); }

	// �w����ԕ\������e�L�X�g�̒ǉ�
	inline void AddText(std::string Str, int nCnt) { m_vText.push_back({ Str, nCnt }); }

private:

	//****************************************************
	// special function
	//****************************************************
	CRenderer();           // �f�t�H���g�R���X�g���N�^
	~CRenderer() override; // �f�X�g���N�^

	//****************************************************
	// function
	//****************************************************
	void InitializeRenderState(); // �����_�[�X�e�[�g�̏����Z�b�g�A�b�v
	void AdjustBuckBufferSize();  // �o�b�N�o�b�t�@�̃T�C�Y�𒲐�
	void PrintText();             // �e�L�X�g�̕\��

	//****************************************************
	// data
	//****************************************************
	LPDIRECT3D9       m_pD3D;    // Direct3D
	LPDIRECT3DDEVICE9 m_pDevice; // �f�o�C�X

	// �w����ԕ\������e�L�X�g
	std::vector<std::pair<std::string, int>> m_vText;
};

//============================================================================
// �X�V������
//============================================================================
template <UpdatableContainer T>
void CRenderer::Update_��(const T& t)
{
	// ImGui�̍X�V���J�n
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

#if 0
	// �E�B���h�E�T�C�Y�̕ύX����������
	if (m_ResizeWindowWidth != 0 && m_ResizeWindowHeight != 0)
	{
		// �o�b�N�o�b�t�@�̃T�C�Y�𒲐�
		AdjustBuckBufferSize();
	}
#endif

	// �e�C���X�^���X�̍X�V
	for (auto& it : t)
	{
		it->Update();
	}

	// ImGui�̍X�V���I��
	ImGui::EndFrame();
}

//============================================================================
// �X�V������
//============================================================================
template <typename T>
void CRenderer::Update_��(T&& fp)
{
	// ImGui�̍X�V���J�n
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

#if 0
	// �E�B���h�E�T�C�Y�̕ύX����������
	if (m_ResizeWindowWidth != 0 && m_ResizeWindowHeight != 0)
	{
		// �o�b�N�o�b�t�@�̃T�C�Y�𒲐�
		AdjustBuckBufferSize();
	}
#endif

	// �e�C���X�^���X�̍X�V
	fp();

	// ImGui�̍X�V���I��
	ImGui::EndFrame();
}

//============================================================================
// �`�揈����
//============================================================================
template <DrawableContainer T>
void CRenderer::Draw_��(const T& t)
{
	// ��ʃo�b�t�@�N���A
	m_pDevice->Clear(
		0,                                  // �N���A�������l�p�`�̐���ݒ� (�r���[�|�[�g�S�̂̏ꍇ��0)
		nullptr,                            // �l�p�`�\���̂̃|�C���^��ݒ� (nullptr��n�����ƂŃr���[�|�[�g�S�͈̂̔�)
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // �N���A����T�[�t�F�X���w�肷��
		D3DCOLOR_ARGB(0, 0, 0, 0),          // ���̃J���[�Ń^�[�Q�b�g���N���A
		1.0f,                               // ���̒l�ɑ債�ăf�v�X�o�b�t�@���N���A
		0);                                 // ���̒l�ŃX�e���V���o�b�t�@���N���A

	if (SUCCEEDED(m_pDevice->BeginScene()))
	{
		// �J�������Z�b�g
		if (CCamera::GetStateUseFreeCam())
		{
			CCamera::RefFreeCam()->SetCamera({ 0.0f, 0.0f, 0.0f });
		}
		else
		{
			CCamera_Manager::RefInstance().RefSelectCamera().SetCamera({ 0.0f, 0.0f, 0.0f });
		}

		// �e�C���X�^���X�̕`��
		for (auto& it : t)
		{
			it->Draw();
		}

		// �e�L�X�g�̕`��
		PrintText();

		// ImGui�̕`��
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		// �`��I��
		m_pDevice->EndScene();
	}

	// ����ʂ�\��ʂɔ��f
	m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//============================================================================
// �`�揈����
//============================================================================
template <typename T>
void CRenderer::Draw_��(T&& fp)
{
	// ��ʃo�b�t�@�N���A
	m_pDevice->Clear(
		0,                                  // �N���A�������l�p�`�̐���ݒ� (�r���[�|�[�g�S�̂̏ꍇ��0)
		nullptr,                            // �l�p�`�\���̂̃|�C���^��ݒ� (nullptr��n�����ƂŃr���[�|�[�g�S�͈̂̔�)
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // �N���A����T�[�t�F�X���w�肷��
		D3DCOLOR_ARGB(0, 0, 0, 0),          // ���̃J���[�Ń^�[�Q�b�g���N���A
		1.0f,                               // ���̒l�ɑ債�ăf�v�X�o�b�t�@���N���A
		0);                                 // ���̒l�ŃX�e���V���o�b�t�@���N���A

	if (SUCCEEDED(m_pDevice->BeginScene()))
	{
		// �J�������Z�b�g
		if (CCamera::GetStateUseFreeCam())
		{
			CCamera::RefFreeCam()->SetCamera({ 0.0f, 0.0f, 0.0f });
		}
		else
		{
			CCamera_Manager::RefInstance().RefSelectCamera().SetCamera({ 0.0f, 0.0f, 0.0f });
		}

		// �e�C���X�^���X�̕`��
		fp();

		// �e�L�X�g�̕`��
		PrintText();

		// ImGui�̕`��
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		// �`��I��
		m_pDevice->EndScene();
	}

	// ����ʂ�\��ʂɔ��f
	m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
}