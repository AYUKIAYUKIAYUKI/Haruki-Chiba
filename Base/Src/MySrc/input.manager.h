//============================================================================
// 
// �C���v�b�g�}�l�[�W���[�A�w�b�_�[�t�@�C�� [input.managaer.h]
// Author : ���c����
// 
//============================================================================

#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "singleton.h"

#include "input_keyboard.h"
#include "input_mouse.h"
#include "input_pad.h"

//****************************************************
// �C���v�b�g�}�l�[�W���[�N���X�̒�`
//****************************************************
class CInputManager final : public CSingleton<CInputManager>
{
	//****************************************************
	// �t�����h�錾
	//****************************************************
	friend struct std::default_delete<CInputManager>;
	friend CInputManager& CSingleton<CInputManager>::RefInstance();

public:

	//****************************************************
	// �ÓI�����o�ϐ��̐錾 (���J)
	//****************************************************
	
	// ���� -> ���� �ėp�֐�
	static std::function<std::optional<float>()> s_fpGetInputDir;

	//****************************************************
	// function
	//****************************************************
	
	// ����������
	bool Initialize(HINSTANCE hInstance);
	bool Initialize() { return true; }
	
	// �I������
	void Finalize();

	// �X�V����
	void Update();

	//****************************************************
	// inline funciton
	//****************************************************
	
	// �L�[�{�[�h�̎擾
	inline CInputKeyboard* GetKeyboard() const { return m_upKeyboard.get(); }
	
	// �}�E�X�̎擾
	inline CInputMouse* GetMouse() const { return m_upMouse.get(); }

	// �p�b�h�̎擾
	inline CInputPad* GetPad() const { return m_upPad.get(); }

private:

	//****************************************************
	// special function
	//****************************************************
	CInputManager();           // �f�t�H���g�R���X�g���N�^
	~CInputManager() override; // �f�X�g���N�^

	//****************************************************
	// data
	//****************************************************
	std::unique_ptr<CInputKeyboard> m_upKeyboard; // �L�[�{�[�h
	std::unique_ptr<CInputMouse>    m_upMouse;    // �}�E�X
	std::unique_ptr<CInputPad>      m_upPad;      // �p�b�h
};