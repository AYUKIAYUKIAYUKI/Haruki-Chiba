//============================================================================
// 
// �C���v�b�g�}�l�[�W���[ [input.manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "input.manager.h"

// �E�B���h�E�n���h���̎擾�̂���
#include "window.h"

//****************************************************
// �������O��Ԃ��` 
//****************************************************
namespace
{
	/* ���̖��O��Ԃ��폜���Ă������� */

	// �����t��������
	template <typename T> std::unique_ptr<T> DedicatedInitialize(HINSTANCE hInstance, HWND hWnd)
	{
		useful::up<T> upInstance = std::make_unique<T>();

		if (!upInstance)
		{
			return nullptr;
		}

		if (FAILED(upInstance->Init(hInstance, hWnd)))
		{
			return nullptr;
		}

		return upInstance;
	}

	// �����t�����
	template <typename T> void DedicatedRelease(T* pInstance)
	{
		if (pInstance)
		{
			pInstance->Uninit();
		}
	}
};

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �ÓI�����o�ϐ��̒�`
//****************************************************

// ���� -> ���� �ėp�֐�
std::function<std::optional<float>()> CInputManager::s_fpGetInputDir =
[]() -> std::optional<float> {

	// ���͑��u�̎擾
	auto pKeyboard = CInputManager::RefInstance().GetKeyboard();
	auto pPad      = CInputManager::RefInstance().GetPad();
	
	// �ړ����͂̎�t�m�F�p
	float InputX = 0.0f;
	float InputY = 0.0f;

	// �X�e�B�b�N�̌X�����擾
	InputX = pPad->GetJoyStickL().X;
	InputY = pPad->GetJoyStickL().Y;

	// X��
	if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::JOYKEY::LEFT))
	{
		InputX += -1.0f;
	}
	if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT))
	{
		InputX += 1.0f;
	}

	// Y��
	if (pKeyboard->GetPress(DIK_W) || pPad->GetPress(CInputPad::JOYKEY::UP))
	{
		InputY += 1.0f;
	}
	if (pKeyboard->GetPress(DIK_S) || pPad->GetPress(CInputPad::JOYKEY::DOWN))
	{
		InputY += -1.0f;
	}

	// �ړ�������P�ʉ~�Ő��K�����邽�߁A�����l�𗘗p���Ė��m�ɓ��͂��u�����v���Ƃ𔻕ʂ���
	if (InputX == 0.0f && InputY == 0.0f)
	{
		return std::nullopt;
	}

	// ���͕������p�x�Ƃ��ĕϊ�
	return atan2f(InputX, InputY);
};

//============================================================================
// ����������
//============================================================================
bool CInputManager::Initialize(HINSTANCE hInstance)
{
	// �E�B���h�E�n���h���̎擾
	HWND hWnd = CWindow::RefInstance().GetWindowHandle();

	// �L�[�{�[�h�̐���
	m_upKeyboard = DedicatedInitialize<CInputKeyboard>(hInstance, hWnd);

	// �}�E�X�̐���
	m_upMouse = DedicatedInitialize<CInputMouse>(hInstance, hWnd);

	// �p�b�h�̐���
	m_upPad = DedicatedInitialize<CInputPad>(hInstance, hWnd);

	return true;
}

//============================================================================
// �I������
//============================================================================
void CInputManager::Finalize()
{
	// �p�b�h�̏I������
	DedicatedRelease<CInputPad>(m_upPad.get());

	// �}�E�X�̏I������
	DedicatedRelease<CInputMouse>(m_upMouse.get());

	// �L�[�{�[�h�̏I������
	DedicatedRelease<CInputKeyboard>(m_upKeyboard.get());
}

//============================================================================
// �X�V����
//============================================================================
void CInputManager::Update()
{
	// �L�[�{�[�h�̍X�V
	m_upKeyboard->Update();

	// �}�E�X�̍X�V
	m_upMouse->Update();

	// �p�b�h�̍X�V
	m_upPad->Update();
}

//============================================================================
// �R���X�g���N�^
//============================================================================
CInputManager::CInputManager()
	: m_upKeyboard(nullptr)
	, m_upMouse(nullptr)
	, m_upPad(nullptr)
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CInputManager::~CInputManager()
{
	// �I������
	Finalize();
}