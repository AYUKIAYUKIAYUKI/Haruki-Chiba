//============================================================================
// 
// �E�B���h�E�A�w�b�_�[�t�@�C�� [window.h]
// Author : ���c����
// 
//============================================================================

#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "singleton.h"

//****************************************************
// �E�B���h�E�N���X�̒�`
//****************************************************
class CWindow final : public CSingleton<CWindow>
{
	//****************************************************
	// �t�����h�錾
	//****************************************************
	friend struct std::default_delete<CWindow>;
	friend CWindow& CSingleton<CWindow>::RefInstance();

	//****************************************************
	// �ÓI�����o�萔�̒�` (����J)
	//****************************************************

	// �E�B���h�E�N���X�̖��O
	static constexpr const char* s_pClassName = "Window Class";

	// �E�B���h�E�̖��O
	static constexpr const char* s_pWindowName = "Base";

public:

	//****************************************************
	// �ÓI�����o�萔�̒�` (���J)
	//****************************************************

	// �E�B���h�E����
	static constexpr WORD  WWINDOW_WIDTH = 1280;
	static constexpr float FWINDOW_WIDTH = 1280.0f;

	// �E�B���h�E�c��
	static constexpr WORD  WWINDOW_HEIGHT = 720;
	static constexpr float FWINDOW_HEIGHT = 720.0f;

	//****************************************************
	// �ÓI�����o�ϐ��̒�` (���J)
	//****************************************************

	// �X�N���[������
	static const WORD  WSCREEN_WIDTH;
	static const float FSCREEN_WIDTH;

	// �X�N���[���c��
	static const WORD  WSCREEN_HEIGHT;
	static const float FSCREEN_HEIGHT;

	//****************************************************
	// function
	//****************************************************

	// ����������
	bool Initialize(HINSTANCE hInstance);
	bool Initialize() { return true; }

	// �I������
	void Finalize();

	// ���b�Z�[�W���[�v
	// fp -> �X�V����
	template <typename T>
	void MessageLoop(T&& fpMainLoop);

	//****************************************************
	// inline function
	//***************************************************

	// �N���C�A���g�̈�̃T�C�Y�̎擾
	inline RECT GetWindowRect() const { return m_Rect; }

	// �E�B���h�E�n���h���̎擾
	inline HWND GetWindowHandle() const { return m_hWnd; }

private:

	//****************************************************
	// special function
	//****************************************************
	CWindow();           // �f�t�H���g�R���X�g���N�^
	~CWindow() override; // �f�X�g���N�^

	//****************************************************
	// function
	//****************************************************
	void InitializeWindowClass(HINSTANCE hInstance); // �E�B���h�E�N���X�̏���������
	void InitializeWindowRect();					 // �N���C�A���g�̈�̏���������
	void MyCreateWindow(HINSTANCE hInstance);		 // �E�B���h�E�̐���

	// �E�C���h�E�v���V�[�W��
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	//****************************************************
	// data
	//****************************************************
	WNDCLASSEX m_Wcex;	  // �E�B���h�E�N���X
	RECT	   m_Rect;	  // �N���C�A���g�̈�̃T�C�Y
	DWORD	   m_dwStyle; // �E�B���h�E�̕\���X�^�C��
	HWND	   m_hWnd;	  // �E�B���h�E�n���h��
	MSG		   m_Msg;	  // ���b�Z�[�W���i�[����ϐ�
};

// �e���v���[�g�����t�@�C��
#include "window.impl"