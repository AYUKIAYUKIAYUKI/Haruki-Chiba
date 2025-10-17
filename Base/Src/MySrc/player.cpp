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
// �ÓI�����o�ϐ��̒�` (����J)
//****************************************************

// �� �ŏI�I�ɂ͒萔�ƂȂ�܂����A�Q���I�ɕҏW�\�Ȓl�Ƃ��܂�

// �ڕW�l�ւ̕�ԌW��
float CPlayer::COEF_CORRECT_TARGET = 0.6f;

// �d�͉����x
float CPlayer::COEF_GRAVITY = -0.15f;

// �W�����v�\��
int CPlayer::NUM_LEFT_JUMP = 1;

// �W�����v��
float CPlayer::COEF_TRIGGER_JUMP = 2.75f;

// �ړ����x
float CPlayer::COEF_MOVE_SPEED     = 2.0f;
float CPlayer::COEF_MOVE_SPEED_AIR = 0.3f;

// ������
float CPlayer::COEF_BRAKING = 0.5f;

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
	, m_nFrame(0)
	, m_State(State::DEFAULT)
	//, m_nLeftNumJump(NUM_LEFT_JUMP)
	, m_Velocity(VEC3_ZERO_INIT)
	, m_SizeTarget(VEC3_ONE_INIT)
	, m_RotTarget(VEC3_ZERO_INIT)
	, m_PosTarget(VEC3_ZERO_INIT)
	, m_afpExecuteState{}
	, m_vMichos()
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

	// ��Ԃ��Ƃ̎��s���e���`���Ă����F�ʏ�
	m_afpExecuteState[static_cast<unsigned char>(State::DEFAULT)] =
		[this]() -> bool
	{
		// �ړ�
		Move(COEF_MOVE_SPEED);

		if (CInputManager::RefInstance().GetKeyboard()->GetTrigger(DIK_B))
		{
			Change(State::DAMAGE, 0, [this]() -> void {});
		}

		// �W�����v
		if (CInputManager::RefInstance().GetKeyboard()->GetTrigger(DIK_SPACE))
		{
			Change(State::JUMP, 0, [this]() -> void
				{
					// �����x�FXZ���F���x��}���]�C���₷
					// �@�@�@�FY�� �F�W�����v�͂�^����
					m_Velocity =
					{
						m_Velocity.x * COEF_MOVE_SPEED_AIR,
						COEF_TRIGGER_JUMP,
						m_Velocity.z * COEF_MOVE_SPEED_AIR
					};

					// �݂��吔
					const int nMichosRepeat = 3;

					// �݂�����ƕ�
					std::array<float, nMichosRepeat> afScales =
					{
						-0.25f, -0.35f, -0.15f
					};

					// �݂�����Ƃ��Ă݂�
					for (int i = 0; i < nMichosRepeat; ++i)
					{
						i % 2 == 0 ?
							m_vMichos.push_back({ 1.0f + afScales[i], 1.0f - afScales[i], 1.0f + afScales[i] }) :
							m_vMichos.push_back({ 1.0f - afScales[i], 1.0f + afScales[i], 1.0f - afScales[i] });
					}

					// �Ō�Ɍ��ɖ߂�
					m_vMichos.push_back(VEC3_ONE_INIT);
				});
		}

		return true;
	};

	// ��ԁF�W�����v
	m_afpExecuteState[static_cast<unsigned char>(State::JUMP)] =
		[this]() -> bool
	{
		// �d�͉���
		bool bLanding = Gravity();

		// �n�ʂɓ��B������ʏ��Ԃ�
		if (bLanding)
		{
			Change(State::DEFAULT, 0, [this]() -> void
				{
					// �݂��吔
					const int nMichosRepeat = 5;

					// �݂�����ƕ�
					std::array<float, nMichosRepeat> afScales =
					{
						0.25f, 0.20f, 0.15f, 0.10f, -0.15f
					};

					// �݂�����Ƃ��Ă݂�
					for (int i = 0; i < nMichosRepeat; ++i)
					{
						i % 2 == 0 ?
							m_vMichos.push_back({ 1.0f + afScales[i], 1.0f - afScales[i], 1.0f + afScales[i] }) :
							m_vMichos.push_back({ 1.0f - afScales[i], 1.0f + afScales[i], 1.0f - afScales[i] });
					}

					// �Ō�Ɍ��ɖ߂�
					m_vMichos.push_back(VEC3_ONE_INIT);
				});
		}

		return true;
	};

	// ��ԁF�I�V��
	m_afpExecuteState[static_cast<unsigned char>(State::HIP)] =
		[this]() -> bool
	{
		return true;
	};

	// ��ԁF�_���[�W
	m_afpExecuteState[static_cast<unsigned char>(State::DAMAGE)] =
		[this]() -> bool
	{
		bool a = Hit();
		if (a)
		{
			Change(State::DEFAULT, 0, [this]() -> void {});
		}
		return true;
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
	// ���l�ҏW
	ValueEdit();

	// ���񂽂�X�e�[�g�̎��s
	m_afpExecuteState[static_cast<unsigned char>(m_State)]();

	// �U���Đ�
	PlayWave();

	// �ڕW�ʒu�F����A�����x��������
	m_PosTarget += m_Velocity;

	// ���݃T�C�Y -> �ڕW�T�C�Y : �w������
	Vec3 Size = GetSize();
	ExponentialDecay(Size, m_SizeTarget, COEF_CORRECT_TARGET);
	SetSize(Size);

	// ���݌��� -> �ڕW���� : �w������
	Vec3 Rot = GetRot();
	NormalizeAngleToDest(Rot.y, m_RotTarget.y);
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

//============================================================================
// ���񂽂��ԑJ��
//============================================================================
void CPlayer::Change(State State, int nLimit, std::function<void()> fpOpt)
{
	// ���񂽂�J�E���g
	++m_nFrame;

	// �K��t���[���o�߂�
	if (m_nFrame >= nLimit)
	{
		// �ύX���A���łɕK�v�ȏ���
		if (fpOpt)
		{
			fpOpt();
		}

		// ��Ԃ��w�肵�����̂֕ύX
		m_nFrame = 0;
		m_State  = State;
	}
}

//============================================================================
// �ړ�
//============================================================================
void CPlayer::Move(float fSpeed)
{
	// ���͕������擾
	std::optional<float> opDir = CInputManager::s_fpGetInputDir();

	// ���͂�����Ȃ�
	if (opDir)
	{
		// �����x�FXZ���F�����ɉ����ĒP�ʃx�N�g���ɑ��x�W�����|�������̂�ݒ�
		m_Velocity.x = sinf(opDir.value()) * fSpeed;
		m_Velocity.z = cosf(opDir.value()) * fSpeed;

		// �ڕW�����FY���F�ړ������Ɍ����悤�ɂ���
		m_RotTarget.y = atan2f(-m_Velocity.x, -m_Velocity.z);
	}

	// �����x�FXZ���F���Ԃɔ����������Ă���
	ExponentialDecay(m_Velocity.x, 0.0f, COEF_BRAKING);
	ExponentialDecay(m_Velocity.z, 0.0f, COEF_BRAKING);
}

//============================================================================
// �d�͉���
//============================================================================
bool CPlayer::Gravity()
{
	// �����x�FY���F�������։����x�𑝉�
	m_Velocity.y += COEF_GRAVITY;

	// ���̃t���[���A�n�ʂɓ��B����Ȃ�
	if (m_PosTarget.y + m_Velocity.y < 0.0f)
	{
		// �����x�@�FY���F��~
		// �ڕW�ʒu�FY���F�n�ʂɍ��킹��
		m_Velocity.y  = 0.0f;
		m_PosTarget.y = 0.0f;

		return true;
	}

	return false;
}

//============================================================================
// �U���Đ�
//============================================================================
void CPlayer::PlayWave()
{
	// �w�肪����������Ώ������Ȃ�
	if (m_vMichos.empty())
	{
		return;
	}

	// �擪�v�f�̎Q��
	const Vec3& front = m_vMichos.front();

	// �ڕW�T�C�Y�F�݂�����Ƃɂ���
	m_SizeTarget = front;

	// ���݂̃T�C�Y���擾
	const Vec3& Size = GetSize();

	// �ߎ��l��ݒ肷��
	const float f�� = 0.1f;

	// ���݂̃T�C�Y�ƖڕW�T�C�Y���\���߂����
	if (fabsf(Size.x - m_SizeTarget.x) < fabsf(f��) &&
		fabsf(Size.y - m_SizeTarget.y) < fabsf(f��) &&
		fabsf(Size.z - m_SizeTarget.z) < fabsf(f��))
	{
		// �擪�v�f���폜
		m_vMichos.pop_front();
	}
}

//============================================================================
// �U��������������
//============================================================================
bool CPlayer::Hit()
{
	static int Num = 0;
	Num++;

	if (Num > 120)
	{
		Num = 0;
		return true;
	}
	return false;
}

//============================================================================
// ���l�ҏW
//============================================================================
void CPlayer::ValueEdit()
{
	// �ÓI�����o�ϐ��������l�Ƃ��āA�S�ăR�s�[
	float fCoefCrorrectTarget = CPlayer::COEF_CORRECT_TARGET;
	float fCoefGravity        = CPlayer::COEF_GRAVITY;
	float fCoefTriggerJump    = CPlayer::COEF_TRIGGER_JUMP;
	float fCoefMoveSpeed      = CPlayer::COEF_MOVE_SPEED;
	float fCoefMoveSpeedAir   = CPlayer::COEF_MOVE_SPEED_AIR;
	float fCoefBraking        = CPlayer::COEF_BRAKING;

	// �ω���
	const float fSpeed = 0.01f;

	// ImGui�ŕҏW
	MIS::MyImGuiShortcut_BeginWindow(reinterpret_cast<const char*>(u8"�v���C���[�̊e��p�����[�^����"));
	{
		ImGui::DragFloat("CrorrectTarget", &fCoefCrorrectTarget, fSpeed, fSpeed,  1.0f);
		ImGui::DragFloat("Gravity",        &fCoefGravity,        fSpeed, FLT_MIN, 0.0f);
		ImGui::DragFloat("TriggerJump",    &fCoefTriggerJump,    fSpeed, fSpeed,  FLT_MAX);
		ImGui::DragFloat("MoveSpeed",      &fCoefMoveSpeed,      fSpeed, fSpeed,  FLT_MAX);
		ImGui::DragFloat("MoveSpeedAir",   &fCoefMoveSpeedAir,   fSpeed, fSpeed,  FLT_MAX);
		ImGui::DragFloat("Braking",        &fCoefBraking,        fSpeed, fSpeed,  FLT_MAX);

		// �T�C�Y���o��
		const Vec3& Size = GetSize();
		ImGui::Text("Size:(%.2f, %.2f, %.2f)", Size.x, Size.y, Size.z);

		// �������o��
		const Vec3& Rot = GetRot();
		ImGui::Text("Rot:(%.2f, %.2f, %.2f)", Rot.x, Rot.y, Rot.z);
		
		// ���W���o��
		const Vec3& Pos = GetPos();
		ImGui::Text("Pos:(%.2f, %.2f, %.2f)", Pos.x, Pos.y, Pos.z);

		// �����x���o��
		ImGui::Text("Velocity:(%.2f, %.2f, %.2f)", m_Velocity.x, m_Velocity.y, m_Velocity.z);

		// �X�e�[�g���o��
		ImGui::Text("State:%s", ToString(m_State));
	}
	ImGui::End();

	// �ҏW�����l�������o�ϐ��ɔ��f
	CPlayer::COEF_CORRECT_TARGET = fCoefCrorrectTarget;
	CPlayer::COEF_GRAVITY        = fCoefGravity;
	CPlayer::COEF_TRIGGER_JUMP   = fCoefTriggerJump;
	CPlayer::COEF_MOVE_SPEED     = fCoefMoveSpeed;
	CPlayer::COEF_MOVE_SPEED_AIR = fCoefMoveSpeedAir;
	CPlayer::COEF_BRAKING        = fCoefBraking;
}

//============================================================================
// ���݂̃X�e�[�g���݂���
//============================================================================
const char* CPlayer::ToString(State s)
{
	std::vector<const char*> StateName = { "DEFAULT","JUMP","HIP","DAMAGE" };

	return StateName[static_cast<int>(s)];
}