//============================================================================
// 
// �V���O���g���N���X�e���v���[�g�A�w�b�_�[�t�@�C�� [singleton.h]
// Author : ���c����
// 
//============================================================================

#pragma once

#if 0
//****************************************************
// �R���Z�v�g�̒�`
//****************************************************
template <typename T>
concept AutoInitializableAndFinalizable = requires(T t)
{
	{ t.Initialize() } -> std::same_as<bool>;
}
&& requires(T t)
{
	{ t.Finalize() } -> std::same_as<void>;
};
#endif

//****************************************************
// �V���O���g���N���X�e���v���[�g�̒�`
//****************************************************
template <typename T>
class CSingleton
{
public:

	//****************************************************
	// operator
	//****************************************************
	CSingleton& operator=(const CSingleton&) = delete; // �R�s�[������Z�q���폜
	CSingleton& operator=(CSingleton&&)      = delete; // ���[�u������Z�q���폜

	//****************************************************
	// special function
	//****************************************************
	CSingleton(const CSingleton&) = delete; // �R�s�[�R���X�g���N�^���폜
	CSingleton(CSingleton&&)      = delete; // ���[�u�R���X�g���N�^���폜

	//****************************************************
	// static function
	//****************************************************

	// �C���X�^���X�̎Q�Ƃ��擾���܂�
	static T& RefInstance();

protected:

	//****************************************************
	// special function
	//****************************************************
	        CSingleton()  = default; // �f�t�H���g�R���X�g���N�^
	virtual ~CSingleton() = default; // �f�X�g���N�^

	//****************************************************
	// data
	//****************************************************
	static std::unique_ptr<T> s_upInstance; // �C���X�^���X�̃|�C���^
};

// �e���v���[�g�����t�@�C��
#include "singleton.impl"