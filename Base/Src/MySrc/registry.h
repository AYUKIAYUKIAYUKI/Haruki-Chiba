//============================================================================
// 
// ���W�X�g���N���X�e���v���[�g�A�w�b�_�[�t�@�C�� [registry.h]
// Author : ���c����
// 
//============================================================================
	
#pragma once

//****************************************************
// ���W�X�g���N���X�e���v���[�g�̒�`
//****************************************************
template <typename T>
class CRegistry
{
public:

	//****************************************************
	// special function
	//****************************************************
	CRegistry();  // �f�t�H���g�R���X�g���N�^
	~CRegistry(); // �f�X�g���N�^

	//****************************************************
	// function
	//****************************************************s

	// ����������
	// Path      -> �f�[�^�̃p�X
	// fpFactory -> �f�[�^�̂��߂̃t�@�N�g���֐�
	// fpRelease -> �f�[�^�̂��߂̃����[�X�֐�
	bool Initialize(const std::string& Path, std::function<T(const std::string&)>&& fpFactory, std::function<void(T&)>&& fpRelease);
	
	// �I������
	void Finalize();

	// �L�[���󂯎���āA�ۗL���Ă��镨��Ԃ�
	T BindAtKey(const std::string& Key);

	// �����^�C�������̂̐V�K�o�^
	bool Register(const std::string& Key, const std::string& Path);

private:

	//****************************************************
	// data
	//****************************************************
	std::function<T(const std::string&)> m_fpFactory; // �f�[�^�̂��߂̃t�@�N�g���֐�
	std::function<void(T&)>              m_fpRelease; // �f�[�^�̂��߂̃����[�X�֐�
	std::map<std::string, T>             m_mData;     // �L�[�ƃf�[�^�̃}�b�v
};

// �e���v���[�g�����t�@�C��
#include "registry.impl"