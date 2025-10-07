//============================================================================
// 
// �E�B���h�E [window.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "window.h"

//****************************************************
// �ÓI�����o�ϐ��̒�`
//****************************************************

// �X�N���[������
const WORD  CWindow::WSCREEN_WIDTH = static_cast<WORD>(GetSystemMetrics(SM_CXSCREEN));
const float CWindow::FSCREEN_WIDTH = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));

// �X�N���[���c��
const WORD  CWindow::WSCREEN_HEIGHT = static_cast<WORD>(GetSystemMetrics(SM_CYSCREEN));
const float CWindow::FSCREEN_HEIGHT = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

//============================================================================
// �R���X�g���N�^
//============================================================================
CWindow::CWindow()
	: m_Wcex{}
	, m_Rect{}
	, m_dwStyle(0)
	, m_hWnd(nullptr)
	, m_Msg{}
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CWindow::~CWindow()
{
	// �I������
	Finalize();
}

//============================================================================
// ����������
//============================================================================
bool CWindow::Initialize(HINSTANCE hInstance)
{
	// �E�B���h�E�N���X�̏���������
	InitializeWindowClass(hInstance);

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&m_Wcex);

	// �N���C�A���g�̈�̏���������
	InitializeWindowRect();

	// �E�B���h�E�̐���
	MyCreateWindow(hInstance);

	// ����\��ݒ�
	timeBeginPeriod(1);

	return true;
}

//============================================================================
// �I������
//============================================================================
void CWindow::Finalize()
{
	// ����\�̉��
	timeEndPeriod(1);
}

//============================================================================
// �E�B���h�E�N���X�̏���������
//============================================================================
void CWindow::InitializeWindowClass(HINSTANCE hInstance)
{
	m_Wcex.cbSize        = sizeof(WNDCLASSEX);				// WNDCLASSEX�̃������T�C�Y
	m_Wcex.style         = CS_CLASSDC;						// �E�B���h�E�̃X�^�C��
	m_Wcex.lpfnWndProc   = WindowProc;					    // �E�B���h�E�v���V�[�W��
	m_Wcex.cbClsExtra    = 0;
	m_Wcex.cbWndExtra    = 0;
	m_Wcex.hInstance     = hInstance;						// �C���X�^���X�n���h��
	m_Wcex.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);	// �^�X�N�o�[�̃A�C�R��
	m_Wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);		// �}�E�X�J�[�\��
	m_Wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);		    // �N���C�A���g�̈�̔w�i�F
	m_Wcex.lpszMenuName  = NULL;							// ���j���[�o�[
	m_Wcex.lpszClassName = s_pClassName;				    // �E�B���h�E�N���X�̖��O
	m_Wcex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);	// �t�@�C���̃A�C�R��
}

//============================================================================
// �N���C�A���g�̈�̏���������
//============================================================================
void CWindow::InitializeWindowRect()
{
	// �N���C�A���g�̈�̃T�C�Y��ݒ�
	m_Rect = { 0, 0, WWINDOW_WIDTH, WWINDOW_HEIGHT};

	// �E�B���h�E�̕\���X�^�C����ݒ�
	m_dwStyle = WS_OVERLAPPEDWINDOW;

	// �N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&m_Rect, m_dwStyle, FALSE);
}

//============================================================================
// �E�B���h�E�̐���
//============================================================================
void CWindow::MyCreateWindow(HINSTANCE hInstance)
{
	// �E�B���h�E�̐���
	m_hWnd = CreateWindowEx(
		0,	                        // �g���E�B���h�E�̃X�^�C��
		s_pClassName,			    // �E�B���h�E�N���X�̖��O
		s_pWindowName,			    // �E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,	    // �E�B���h�E�̃X�^�C��
		125,					    // �E�B���h�E�̍����X���W
		50,						    // �E�B���h�E�̍����Y���W
		m_Rect.right - m_Rect.left, // �E�B���h�E�̕�
		m_Rect.bottom - m_Rect.top, // �E�B���h�E�̍���
		NULL,					    // �e�E�B���h�E�̃n���h��
		NULL,					    // ���j���[�o�[�܂��͎q�E�B���h�EID
		hInstance,				    // �C���X�^���X�n���h��
		NULL);					    // �E�B���h�E�쐬�f�[�^

	// �E�B���h�E�̕\��
	ShowWindow(m_hWnd, SW_NORMAL);
	UpdateWindow(m_hWnd);
}

// ���b�Z�[�W�n���h���[�̐錾
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//============================================================================
// �E�B���h�E�v���V�[�W��
//============================================================================
LRESULT CALLBACK CWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// ImGui�̓��͂��󂯕t���郁�b�Z�[�W�n���h���[
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	// �Ԃ�l�i�[��
	int nID = 0;

	switch (uMsg)
	{
		// �E�B���h�E�j���̃��b�Z�[�W�𑗂�
	case WM_DESTROY:

		PostQuitMessage(0);

		break;

		// �~�}�[�N�������Ă��郁�b�Z�[�W�𑗂�
	case WM_CLOSE:

		// ���b�Z�[�W�{�b�N�X��\��
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd); // �uWM_DESTROY�Ƃ������b�Z�[�W�𑗂�v
		}
		else
		{
			return 0;
		}

		break;

		// �L�[��������Ă��郁�b�Z�[�W�𑗂�
	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_ESCAPE:

			// ���b�Z�[�W�{�b�N�X��\��
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd); // �uWM_DESTROY�Ƃ������b�Z�[�W�𑗂�v
			}

			break;
		}

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); // ����̏�����Ԃ�
}