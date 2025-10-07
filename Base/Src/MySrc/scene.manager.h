//============================================================================
// 
// �V�[���}�l�[�W���[�A�w�b�_�[�t�@�C�� [scene.manager.h]
// Author : ���c����
// 
//============================================================================

#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "singleton.h"

//****************************************************
// �O���錾
//****************************************************
class CScene;

//****************************************************
// �R���Z�v�g�̒�`
//****************************************************
template <typename T>
concept DerivedFromScene = std::derived_from<T, CScene>
&& requires(T t)
{
	{ t.Initialize() } -> std::same_as<bool>;
}
&& requires(T t)
{
	{ t.Finalize() } -> std::same_as<void>;
};

//****************************************************
// �V�[���}�l�[�W���[�N���X�̒�`
//****************************************************
class CSceneManager final : public CSingleton<CSceneManager>
{
	//****************************************************
	// �t�����h�錾
	//****************************************************
	friend struct std::default_delete<CSceneManager>;
	friend CSceneManager& CSingleton<CSceneManager>::RefInstance();

public:

	//****************************************************
	// function
	//****************************************************
	
	// ����������
	template <DerivedFromScene T>
	bool Initialize();

	// �V�[���ύX
	template <DerivedFromScene T>
	void ChangeScene(useful::up<T>&& upScene);

	// �V�[���擾
	inline const useful::up<CScene>& GetScene() { return m_upScene; };

private:

	//****************************************************
	// special function
	//****************************************************
	CSceneManager();           // �f�t�H���g�R���X�g���N�^
	~CSceneManager() override; // �f�X�g���N�^

	//****************************************************
	// function
	//****************************************************
	
	// ����������
	bool Initialize() { return true; }

	// �I������
	void Finalize() {}

	//****************************************************
	// data
	//****************************************************
	useful::up<CScene> m_upScene; // ���݂̃V�[��
};

// �e���v���[�g�����t�@�C��
#include "scene.manager.impl"