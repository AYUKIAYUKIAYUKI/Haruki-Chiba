//============================================================================
// 
// �v���C���[�A�w�b�_�[�t�@�C�� [player.h]
// Author : ���c����
// 
//============================================================================

#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.X.h"

//****************************************************
// �v���C���[�N���X�̒�`
//****************************************************
class CPlayer : public CObjectX
{
	//****************************************************
	// �ÓI�����o�萔���` (����J)
	//****************************************************

	// �ڕW�l�ւ̕�ԌW��
	static constexpr float COEF_CORRECT_TARGET = 0.1f;

	// ���x
	static constexpr float COEF_MOVE_SPEED = 2.5f;

	// ������
	static constexpr float COEF_BRAKING = 0.5f;

public:

	//****************************************************
	// �ÓI�����o�ϐ��̐錾 (���J)
	//****************************************************

	// �f�t�H���g�̃t�@�N�g��
	static std::function<bool(CPlayer*)> s_fpDefaultFactory;

	//****************************************************
	// special function
	//****************************************************
	CPlayer(OBJ::TYPE Type, OBJ::LAYER Layer); // �R���X�g���N�^
	~CPlayer() override;                       // �f�X�g���N�^

	//****************************************************
	// function
	//****************************************************

	// ����������
	bool Initialize();

	// �I������
	void Finalize();

	// �X�V����
	void Update() override;

	// �`�揈��
	void Draw() override;

	//****************************************************
	// inline function
	//****************************************************

	// �c��W�����v�񐔑���p
	inline int  GetLeftNumJump() const   { return m_nLeftNumJump; }
	inline void SetLeftNumJump(int nNum) { m_nLeftNumJump = nNum; }

	// �����x����p
	inline const D3DXVECTOR3& GetVelocity() const                 { return m_Velocity; }
	inline void               SetVelocity(const D3DXVECTOR3& Vel) { m_Velocity = Vel; }

	// �ڕW�T�C�Y����p
	inline const D3DXVECTOR3& GetSizeTarget() const                        { return m_SizeTarget; }
	inline void               SetSizeTarget(const D3DXVECTOR3& SizeTarget) { m_SizeTarget = SizeTarget; }

	// �ڕW��������p
	inline const D3DXVECTOR3& GetRotTarget() const                       { return m_RotTarget; }
	inline void               SetRotTarget(const D3DXVECTOR3& RotTarget) { m_RotTarget = RotTarget; }

	// �ڕW�ʒu����p
	inline const D3DXVECTOR3& GetPosTarget() const                       { return m_PosTarget; }
	inline void               SetPosTarget(const D3DXVECTOR3& PosTarget) { m_PosTarget = PosTarget; }

private:

	//****************************************************
	// data
	//****************************************************
	int         m_nLeftNumJump; // �W�����v�\��
	D3DXVECTOR3 m_Velocity;     // �����x
	D3DXVECTOR3 m_SizeTarget;   // �ڕW�T�C�Y
	D3DXVECTOR3 m_RotTarget;    // �ڕW����
	D3DXVECTOR3 m_PosTarget;    // �ڕW�ʒu

	std::function<void()> m_fpDefaultMove; // �f�t�H���g�̈ړ�����
	std::function<void()> m_fpDecorate;    // �I�}�P�̓���
};