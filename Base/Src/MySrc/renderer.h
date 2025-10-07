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

// �e���v���[�g�����t�@�C��
#include "renderer.impl"