//============================================================================
// 
// �I�u�W�F�N�g(3D)�A�w�b�_�[�t�@�C�� [object.3D.h]
// Author : ���c����
// 
//============================================================================

#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"

//****************************************************
// 3D�I�u�W�F�N�g�N���X
//****************************************************
class CObject3D : public CObject
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
	// special function
	//****************************************************
	CObject3D(OBJ::TYPE Type, OBJ::LAYER Layer); // �R���X�g���N�^
	~CObject3D() override;                       // �f�X�g���N�^

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

	// <setter>
	void BindTex(LPDIRECT3DTEXTURE9 pTex);		// �e�N�X�`������

	// <getter/setter>
	const D3DXVECTOR3& GetSize() const;	// �T�C�Y�擾
	void SetSize(D3DXVECTOR3 Size);		// �T�C�Y�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// �����擾
	void SetRot(D3DXVECTOR3 Rot);		// �����ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// ���W�擾
	void SetPos(D3DXVECTOR3 Pos);		// ���W�ݒ�

	// <getter/setter>
	const D3DXCOLOR& GetCol() const;	// �F�擾
	void SetCol(D3DXCOLOR Col);			// �F�ݒ�

	// <getter/setter>
	const float& GetAlpha() const;	// �A���t�@�l�擾
	void SetAlpha(float fAlpha);	// �A���t�@�l�ݒ�

	// <getter>
	const float& GetLength() const;	// �W�J�p�Ίp���擾

	// <static function>
	static CObject3D* Create(OBJ::TYPE Type, OBJ::LAYER Layer, LPDIRECT3DTEXTURE9 pTexture); // ����

private:

	//****************************************************
	// function
	//****************************************************
	HRESULT CreateVtxBuff(); // ���_�o�b�t�@�̐���
	void	SetMtxWorld();		// ���[���h�s��ݒ�
		
	//****************************************************
	// data
	//****************************************************
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;	// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DTEXTURE9	m_pTex;		// �e�N�X�`���̃|�C���^
	D3DXVECTOR3	m_Size;		// �T�C�Y
	D3DXVECTOR3	m_Rot;		// ����
	D3DXVECTOR3	m_Pos;		// ���W
	D3DXVECTOR3 m_Scale;	// �g�嗦
	D3DXCOLOR	m_Col;		// �F
	float		m_fLength;	// �W�J�p�Ίp��
	float		m_fAngle;	// �Ίp���p�p�x
	D3DXMATRIX	m_MtxWorld;	// ���[���h�s��
};