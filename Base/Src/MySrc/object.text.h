//============================================================================
// 
// �e�L�X�g�I�u�W�F�N�g�A�w�b�_�[�t�@�C�� [object.text.h]
// Author : ���c����
// 
//============================================================================

#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"

//****************************************************
// �e�L�X�g�I�u�W�F�N�g�N���X�̒�`
//****************************************************
class CObjectText : public CObject
{
	//****************************************************
	// �ÓI�����o�萔�̒�` (����J)
	//****************************************************

	// �H�쎞�Ԃ̋K��l
	static constexpr int DEFAULT_GHOSTLIMIT = 20;

	// �g�ł����x�̋K��l
	static constexpr float DEFAULT_WAEVCOEF = 3.0f;

public:

	//****************************************************
	// special function
	//****************************************************
	CObjectText(OBJ::TYPE Type, OBJ::LAYER Layer); // �R���X�g���N�^
	~CObjectText() override;                       // �f�X�g���N�^

	//****************************************************
	// function
	//****************************************************

	// ����������
	bool Initialize() { return 0; };

	// �I������
	void Finalize() {};

	// �X�V����
	void Update() override;

	// �`�揈��
	void Draw() override;

	// ����
	static CObjectText* Create(const JSON& Json);

	//****************************************************
	// inline function
	//****************************************************
	
	// �ʒu�̑���p
	inline const D3DXVECTOR2& GetPos() const                 { return m_Pos; }
	inline void               SetPos(const D3DXVECTOR2& Pos) { m_Pos = Pos; }

	// �T�C�Y�̑���p
	inline const D3DXVECTOR2& GetSize() const                  { return m_Size; }
	inline void               SetSize(const D3DXVECTOR2& Size) { m_Size = Size; }

	// �F�̑���p
	inline const D3DXCOLOR& GetCol() const               { return m_Col; }
	inline void             SetCol(const D3DXCOLOR& Col) { m_Col = Col; }

private:

	//****************************************************
	// function
	//****************************************************
	void TextAnimation(); // �e�L�X�g����̃A�j���[�V����

	//****************************************************
	// data
	//****************************************************
	LPD3DXFONT  m_pFont;       // �t�H���g�̃|�C���^
	std::string m_sText;       // ����
	std::string m_sTextTarget; // �ڕW����
	int         m_nLimit;      // �\������
	int         m_nGhostLimit; // �H�쎞��
	int         m_nSpeed;      // �\�����x
	int         m_nStopSpeed;  // �x�����x
	bool        m_bWave;       // �g�ł��g�p���
	D3DXVECTOR2 m_WavePos;     // �g�ł��ϓ��ʒu
	float       m_fWaveCoef;   // �g�ł����x
	D3DXVECTOR2 m_Pos;         // �ʒu
	D3DXVECTOR2 m_Size;        // �T�C�Y
	D3DXCOLOR   m_Col;         // �F
};