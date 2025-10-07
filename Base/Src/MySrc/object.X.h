//============================================================================
// 
// �I�u�W�F�N�g(X���f��)�A�w�b�_�[�t�@�C�� [object.X.h]
// Author : ���c����
// 
//============================================================================

#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"
#include "X.manager.h"

//****************************************************
// X�I�u�W�F�N�g�N���X�̒�`
//****************************************************
class CObjectX : public CObject
{
public:

	//****************************************************
	// �ÓI�����o�ϐ� (���J)
	//****************************************************

	// �f�t�H���g�̃t�@�N�g��
	static std::function<bool(CObjectX*)> s_fpDefaultFactory;

	//****************************************************
	// special function
	//****************************************************
	CObjectX(OBJ::TYPE Type, OBJ::LAYER Layer); // �R���X�g���N�^
	~CObjectX() override;                       // �f�X�g���N�^

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

	// ���f������p
	inline CXManager::MODEL* GetModel() const                         { return useful::PtrCheck(m_pModel, "Model"); }
	inline void              SetModel(CXManager::MODEL* const pModel) { m_pModel = pModel; }

	// �T�C�Y����p
	inline const D3DXVECTOR3& GetSize() const                  { return m_Size; }
	inline void               SetSize(const D3DXVECTOR3& Size) { m_Size = Size; }

	// ��������p
	inline const D3DXVECTOR3& GetRot() const                 { return m_Rot; }
	inline void               SetRot(const D3DXVECTOR3& Rot) { m_Rot = Rot; }

	// �ʒu����p
	inline const D3DXVECTOR3& GetPos() const                 { return m_Pos; }
	inline void               SetPos(const D3DXVECTOR3& Pos) { m_Pos = Pos; }

	// �F����p
	inline const D3DXCOLOR& GetCol() const               { return m_Col; }
	inline void             SetCol(const D3DXCOLOR& Col) { m_Col = Col; }

	// �F���f���
	inline void SetUseColState(bool bState = true) { m_bUseCol = bState; };

	// ���[���h�s����擾
	inline const D3DXMATRIX& GetMtxWorld() const { return m_MtxWorld; }

private:

	//****************************************************
	// data
	//****************************************************
	CXManager::MODEL* m_pModel;   // ���f�����
	D3DXVECTOR3	      m_Size;     // �T�C�Y
	D3DXVECTOR3	      m_Rot;      // ����
	D3DXVECTOR3	      m_Pos;      // ���W
	D3DXCOLOR         m_Col;      // �F
	bool              m_bUseCol;  // �F���f���
	D3DXMATRIX        m_MtxWorld; // ���[���h�s��
};