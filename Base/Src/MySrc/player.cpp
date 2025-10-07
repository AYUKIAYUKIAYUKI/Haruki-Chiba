//============================================================================
// 
// �v���C���[ [player.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player.h"

// ���͂��擾���邽��
#include "input.manager.h"

// ������������X���f�����擾���邽��
#include "X.manager.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �ÓI�����o�ϐ��̒�` (���J)
//****************************************************

// �f�t�H���g�̃t�@�N�g��
std::function<bool(CPlayer*)> CPlayer::s_fpDefaultFactory =
[](CPlayer* pPlayer) -> bool
	{
		// ����������
		pPlayer->Initialize();

		// ���f���̎擾
		auto pModel = useful::PtrCheck(CXManager::RefInstance().RefRegistry().BindAtKey("Player"), "Syokika Lamda no Naka Model Nai");

		// ���f���̐ݒ�
		pPlayer->SetModel(pModel);

		return true;
	};

//============================================================================
// �R���X�g���N�^
//============================================================================
CPlayer::CPlayer(OBJ::TYPE Type, OBJ::LAYER Layer)
	: CObjectX(Type, Layer)
	, m_nLeftNumJump(1)
	, m_Velocity(VEC3_ZERO_INIT)
	, m_SizeTarget(VEC3_ONE_INIT)
	, m_RotTarget(VEC3_ZERO_INIT)
	, m_PosTarget(VEC3_ZERO_INIT)
	, m_fpDefaultMove(nullptr)
	, m_fpDecorate(nullptr)
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPlayer::~CPlayer()
{}

//============================================================================
// ����������
//============================================================================
bool CPlayer::Initialize()
{
	// �I�u�W�F�N�g(X���f��)�̏���������
	if (FAILED(CObjectX::Initialize()))
	{
		return false;
	}
	
	// �I�}�P�̓���
	m_fpDecorate =
		[this]() -> void 
		{
			m_SizeTarget = VEC3_ONE_INIT;

			static float Hz = 0.0f;

			++Hz;

			m_SizeTarget.y = 1.0f + cosf(Hz) * 0.25f;
		};

	return true;
}

//============================================================================
// �I������
//============================================================================
void CPlayer::Finalize()
{
	// �I�u�W�F�N�g(X���f��)�̏I������
	CObjectX::Finalize();
}

//============================================================================
// �X�V����
//============================================================================
void CPlayer::Update()
{
	// ���͕������擾
	std::optional<float> opDir = CInputManager::s_fpGetInputDir();

	// �����������x�Ƃ݂Ȃ�
	if (opDir)
	{
		// �Q���I��XZ���ʂ��ړ�������̂Ƃ���
		m_Velocity.x = sinf(opDir.value());
		m_Velocity.z = cosf(opDir.value());

		// �I�}�P�̓���
		m_fpDecorate();
	}

	// �����x�͎��Ԃɔ����������Ă���
	ExponentialDecay(m_Velocity, VEC3_ZERO_INIT, COEF_BRAKING);

	// �ڕW�ʒu�������x���ϓ������Ă���
	m_PosTarget += m_Velocity * COEF_MOVE_SPEED;

	// ���݃T�C�Y -> �ڕW�T�C�Y : �w������
	Vec3 Size = GetSize();
	ExponentialDecay(Size, m_SizeTarget, COEF_CORRECT_TARGET);
	SetSize(Size);

	// ���݌��� -> �ڕW���� : �w������
	Vec3 Rot = GetRot();
	ExponentialDecay(Rot, m_RotTarget, COEF_CORRECT_TARGET);
	SetRot(Rot);

	// ���݈ʒu -> �ڕW�ʒu : �w������
	Vec3 Pos = GetPos();
	ExponentialDecay(Pos, m_PosTarget, COEF_CORRECT_TARGET);
	SetPos(Pos);

	// �I�u�W�F�N�g(X���f��)�̍X�V����
	// �s��̍Čv�Z���܂�ł��邽�ߍX�V�����̏I���ɌĂт܂�
	CObjectX::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CPlayer::Draw()
{
	// �I�u�W�F�N�g(X���f��)�̕`�揈��
	CObjectX::Draw();
}