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
	// ���񂽂�X�e�[�g�p�̗񋓌^���`
	//****************************************************
	enum class State : unsigned char
	{
		DEFAULT = 0, // �ʏ�
		JUMP,        // �W�����v
		HIP,         // �I�V��
		DAMAGE,		 // �_���[�W
		MAX
	};

	//****************************************************
	// �ÓI�����o�萔���` (����J)
	//****************************************************

	/* �ŏI�I�ɂ͒萔�ƂȂ�܂����A�Q���I�ɕҏW�\�Ȓl�Ƃ��܂� */

	// �ڕW�l�ւ̕�ԌW��
	static float COEF_CORRECT_TARGET;

	// �d�͉����x
	static float COEF_GRAVITY;

	// �W�����v�\��
	static int NUM_LEFT_JUMP;

	// �W�����v��
	static float COEF_TRIGGER_JUMP;

	// �ړ����x
	static float COEF_MOVE_SPEED;
	static float COEF_MOVE_SPEED_AIR;

	// ������
	static float COEF_BRAKING;

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
	//inline int  GetLeftNumJump() const   { return m_nLeftNumJump; }
	//inline void SetLeftNumJump(int nNum) { m_nLeftNumJump = nNum; }

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
	// function
	//****************************************************

	// ���񂽂��ԑJ��
	void Change(State State, int nLimit, std::function<void()> fpOpt);

	// ����Ȃ�
	void Move(float fSpeed); // �ړ�
	bool Gravity();          // �d�͉���
	void SetWave();          // �U���ݒ�
	void PlayWave();         // �U���Đ�
	void ValueEdit();        // ���l�ҏW
	bool Hit();				 // �U��������������
	const char* ToString(State s);//���݂̃X�e�[�g��Ԃ�����
	//****************************************************
	// data
	//****************************************************
	int         m_nFrame;       // �t���[����
	State       m_State;        // ���񂽂�X�e�[�g
	//int       m_nLeftNumJump; // �W�����v�\��
	D3DXVECTOR3 m_Velocity;     // �����x
	D3DXVECTOR3 m_SizeTarget;   // �ڕW�T�C�Y
	D3DXVECTOR3 m_RotTarget;    // �ڕW����
	D3DXVECTOR3 m_PosTarget;    // �ڕW�ʒu

	// ��Ԃ��Ƃ̓���̔z��
	using Executer = std::function<bool()>;
	std::array<Executer, static_cast<unsigned char>(State::MAX)> m_afpExecuteState;

	// �U���Đ��p�F�ڕW�T�C�Y���X�g
	using Michos = std::list<D3DXVECTOR3>;
	Michos m_vMichos;
};