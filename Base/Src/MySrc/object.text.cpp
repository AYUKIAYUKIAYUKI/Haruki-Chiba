//============================================================================
// 
// �e�L�X�g�I�u�W�F�N�g [object.text.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.text.h"

// �E�B���h�E�T�C�Y�̎擾�̂���
#include "window.h"

// �t�H���g�̕`��@�\�̎擾�̂���
#include "font.manager.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//============================================================================
// �R���X�g���N�^
//============================================================================
CObjectText::CObjectText(OBJ::TYPE Type, OBJ::LAYER Layer)
	: CObject(Type, Layer)
	, m_pFont(nullptr)
	, m_sText{}
	, m_sTextTarget{}
	, m_nLimit(0)
	, m_nGhostLimit(DEFAULT_GHOSTLIMIT)
	, m_nSpeed(0)
	, m_nStopSpeed(0)
	, m_bWave(true)
	, m_WavePos(VEC2_ZERO_INIT)
	, m_fWaveCoef(DEFAULT_WAEVCOEF)
	, m_Size(VEC3_ZERO_INIT)
	, m_Pos(VEC3_ZERO_INIT)
	, m_Col(XCOL_ONE_INIT)
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObjectText::~CObjectText()
{}

//============================================================================
// ����
//============================================================================
CObjectText* CObjectText::Create(const JSON& Json)
{
	// �C���X�^���X����
	CObjectText* p = DBG_NEW CObjectText(OBJ::TYPE::NONE, OBJ::LAYER::UI);

	// �p�����[�^�ݒ�
	p->m_pFont = CFontManager::RefInstance().RefRegistry().BindAtKey(Json["Font"].get<std::string>());
	p->m_sTextTarget = useful::JsonConvertToSJIS(Json["Text"].get<std::string>());
	p->m_nLimit = Json["Limit"].get<int>();
	//p->m_nGhostLimit = Json["GhostLimit"].get<int>();
	p->m_nSpeed = Json["Speed"].get<int>();
	//p->m_nStopSpeed = Json["StiopSpeed"].get<int>();
	p->m_Pos = useful::JsonConvertToVec2(Json["Pos"]);
	//p->m_Size = useful::JsonConvertToVec2(Json["Size"]);
	p->m_Col = useful::JsonConvertToXCol(Json["Col"]);

	return p;
}

//============================================================================
// �X�V
//============================================================================
void CObjectText::Update()
{
	// �e�L�X�g�A�j���[�V����
	TextAnimation();

	if (m_nLimit > 0)
	{
		// �\�����Ԃ��J�E���g�_�E��
		--m_nLimit;

		// �H�쎞�Ԃ̋K��l�����������
		if (m_nLimit < m_nGhostLimit)
		{
			// �A���t�@�l��A�������Ĕ����Ȃ��Ă���
			m_Col.a = (1.0f / static_cast<float>(m_nGhostLimit)) * m_nLimit;
		}
	}
	else
	{
		// ���͂�����
		SetDeath();
	}
}

//============================================================================
// �`��
//============================================================================
void CObjectText::Draw()
{
	// �g�ł��ʒu�����Z������Ԃ�
	Vec2 Pos = m_Pos + m_WavePos;

	// �\���ʒu���m��
	RECT Rect = 
	{ 
		int(Pos.x),
		int(Pos.y),
		CWindow::WWINDOW_WIDTH + int(Pos.x),
		CWindow::WWINDOW_HEIGHT + int(Pos.y)
	};

	// �e�L�X�g�̕`��
	m_pFont->DrawText(NULL, m_sText.c_str(), -1, &Rect, DT_CENTER | DT_VCENTER, m_Col);
}

//============================================================================
// �e�L�X�g����
//============================================================================
void CObjectText::TextAnimation()
{
	// �e�L�X�g���S�ĕ\������Ă����珈�������Ȃ�
	if (m_sText == m_sTextTarget)
	{
		return;
	}

	// �x�����x�̐ݒ�t���[�����A���l�̉��Z�̂ݍs��
	++m_nStopSpeed;

	// �J�E���g���x�����x�̋K��l�ɒB������
	if (m_nStopSpeed > m_nSpeed)
	{
		// �x�����x�J�E���g�����Z�b�g
		m_nStopSpeed = 0;

		// �ڕW�e�L�X�g����1�����؂蕪����
		std::string SjisChar = m_sTextTarget.substr(m_sText.size(), 2);

		// ���s�V���{���ŕ����u������
		if (SjisChar == "\n#")
		{
			// ���s�V���{���̏ꏊ���R�s�[
			WORD wReturnPos = static_cast<WORD>(m_sTextTarget.find("\n#", 0));

			// ���K�̉��s�����ɒu��
			m_sTextTarget.replace(wReturnPos, 2, "\n");

			// �؂蕪�����������u��
			SjisChar = "\n";

			// �e�L�X�g����J�E���g�Ƀf�B���C
			/*m_nCntDisp = m_nTextDelay;*/
		}

		// �e�L�X�g��1�����ǉ�
		m_sText += SjisChar;
	}

	// �g�ł��ݒ肪�L���ȏꍇ
	if (m_bWave)
	{
		// �ڕW���͂֓��B����܂ŁA�㉺�ɂ��炵���ʒu���쐬����
		m_WavePos.y = cosf(float(m_nStopSpeed)) * m_fWaveCoef;
	}
}