//============================================================================
// 
// �I�u�W�F�N�g(2D)�A�w�b�_�[�t�@�C�� [object.2D.h]
// Author : ���c����
// 
//============================================================================

#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"

//****************************************************
// 2D�I�u�W�F�N�g�N���X
//****************************************************
class CObject2D : public CObject
{
	//****************************************************
	// �ÓI�����o�萔 (����J)
	//****************************************************

	// ���_��
	static constexpr WORD NUM_VTX = 4;

	// �v���~�e�B�u��
	static constexpr WORD NUM_PRIM = 2;

public:

	//****************************************************
	// �ÓI�����o�ϐ� (���J)
	//****************************************************

	// �f�t�H���g�̃t�@�N�g��
	static std::function<bool(CObject2D*)> s_fpDefaultFactory;

	//****************************************************
	// special function
	//****************************************************
	CObject2D(OBJ::TYPE Type, OBJ::LAYER Layer); // �R���X�g���N�^
	~CObject2D() override;                       // �f�X�g���N�^

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

	// �e�N�X�`���̑���p
	inline LPDIRECT3DTEXTURE9 GetTex() const                  { return useful::PtrCheck(m_pTex, "Texture"); }
	inline void               SetTex(LPDIRECT3DTEXTURE9 pTex) { m_pTex = pTex; }

	// �T�C�Y����p
	inline const D3DXVECTOR2& GetSize() const                  { return m_Size; }
	inline void               SetSize(const D3DXVECTOR2& Size) { m_Size = Size; }

	// ��������p
	inline float GetRot() const     { return m_fRot; }
	inline void  SetRot(float fRot) { m_fRot = fRot; }

	// �ʒu����p
	inline const D3DXVECTOR2& GetPos() const                 { return m_Pos; }
	inline void               SetPos(const D3DXVECTOR2& Pos) { m_Pos = Pos; }

	// �F����p
	inline const D3DXCOLOR& GetCol() const               { return m_Col; }
	inline void             SetCol(const D3DXCOLOR& Col) { m_Col = Col; }

	// �W�J�p�Ίp���擾
	inline float GetLength() const { return m_fLength; }

	// �e�N�X�`���T�C�Y����p
	inline const D3DXVECTOR2& GetTexSize() const                  { return m_TexSize; }
	inline void               SetTexSize(const D3DXVECTOR2& Size) { m_TexSize = Size; }

	// �e�N�X�`��������������p
	inline float GetTexWidth() const       { return m_fTexWidth; }
	inline void  SetTexWidth(float fWidth) { m_fTexWidth = fWidth; }

	// �e�N�X�`���c����������p
	inline float GetTexHeight() const        { return m_fTexHeight; }
	inline void  SetTexHeight(float fHeight) { m_fTexHeight = fHeight; }

	// ���݂̃e�N�X�`��������������p
	int  GetNowPatternU() const           { return m_nNowPatternU; }
	void SetNowPatternU(int nNowPatternU) { m_nNowPatternU = nNowPatternU; }

	// ���݂̃e�N�X�`���c����������p
	int  GetNowPatternV() const           { return m_nNowPatternV; }
	void SetNowPatternV(int nNowPatternV) { m_nNowPatternV = nNowPatternV; }

private:

	//****************************************************
	// function
	//****************************************************
	HRESULT CreateVtxBuff(); // ���_�o�b�t�@�̐���

	//****************************************************
	// data
	//****************************************************
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;     // ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTex;         // �e�N�X�`���̃|�C���^
	D3DXVECTOR2				m_Size;         // �T�C�Y
	float                   m_fRot;	        // ����
	D3DXVECTOR2				m_Pos;			// �ʒu
	D3DXCOLOR				m_Col;			// �F
	float					m_fLength;		// �W�J�p�Ίp��
	float					m_fAngle;		// �Ίp���p�p�x
	D3DXVECTOR2				m_TexSize;		// �e�N�X�`���T�C�Y
	float					m_fTexWidth;	// ���e�N�X�`��������
	float					m_fTexHeight;	// �c�e�N�X�`�����c��
	int						m_nNowPatternU;	// ���݂̉��e�N�X�`�����
	int						m_nNowPatternV;	// ���݂̏c�e�N�X�`�����
};