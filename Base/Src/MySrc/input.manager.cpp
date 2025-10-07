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
	auto pPad = CInputManager::RefInstance().GetPad();

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
	m_upKeyboard = make_unique<CInputKeyboard>();

	if (FAILED(m_upKeyboard->Init(hInstance, hWnd)))
	{
		return false;
	}

	// �}�E�X�̐���
	m_upMouse = make_unique<CInputMouse>();

	if (FAILED(m_upMouse->Init(hInstance, hWnd)))
	{
		return false;
	}

	// �p�b�h�̐���
	m_upPad = make_unique<CInputPad>();

	if (FAILED(m_upPad->Init(hInstance, hWnd)))
	{
		return false;
	}

	return true;
}

//============================================================================
// �I������
//============================================================================
void CInputManager::Finalize()
{}

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