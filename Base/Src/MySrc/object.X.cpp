//============================================================================
// 
// �I�u�W�F�N�g(X���f��) [object.X.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.X.h"
#include "renderer.h"

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
std::function<bool(CObjectX*)> CObjectX::s_fpDefaultFactory =
[](CObjectX* pObj) -> bool
	{
		// X�I�u�W�F�N�g�̏����ݒ�
		pObj->Initialize();

		// ���f���̎擾
		auto pModel = useful::PtrCheck(CXManager::RefInstance().RefRegistry().BindAtKey("Unko"), "Syokika Lamda no Naka Model Nai");

		// ���f���̐ݒ�
		pObj->SetModel(pModel);

		return true;
	};

//============================================================================
// �R���X�g���N�^
//============================================================================
CObjectX::CObjectX(OBJ::TYPE Type, OBJ::LAYER Layer)
	: CObject(Type, Layer)
	, m_pModel(nullptr)
	, m_Size(VEC3_ONE_INIT)
	, m_Pos(VEC3_ZERO_INIT)
	, m_Rot(VEC3_ZERO_INIT)
	, m_Col(XCOL_ONE_INIT)
{
	// ���[���h�s���������
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObjectX::~CObjectX()
{}

//============================================================================
// ����������
//============================================================================
bool CObjectX::Initialize()
{
	return true;
}

//============================================================================
// �I������
//============================================================================
void CObjectX::Finalize()
{}

//============================================================================
// �X�V����
//============================================================================
void CObjectX::Update()
{
	// ���[���h�s��̍X�V
	UpdateMtxWorld(m_MtxWorld, m_Size, m_Rot, m_Pos);
}

//============================================================================
// �`�揈��
//============================================================================
void CObjectX::Draw()
{
	// ���f�����ݒ肳��Ă��Ȃ�
	if (m_pModel == nullptr)
	{
		std::runtime_error("Moderu ga Naiyo");
	}

	// �}�e���A���̃o�b�t�@���ُ�
	if (m_pModel->pBuffMat == nullptr)
	{
		std::runtime_error("Moderu no Materiaru ga Yabaiyo");
	}

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::RefInstance().GetDevice();

	// ���_�@���̎������K����L����
	pDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// ���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;

	// ���݂̃}�e���A�����擾
	pDev->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pModel->pBuffMat->GetBufferPointer();

	for (WORD wCntMat = 0; wCntMat < static_cast<WORD>(m_pModel->dwNumMat); wCntMat++)
	{
		// �}�e���A���F�̐ݒ�
		m_bUseCol ? pMat[wCntMat].MatD3D.Diffuse = m_Col : pMat[wCntMat].MatD3D.Diffuse = m_pModel->apColMat[wCntMat];

		// �}�e���A���̐ݒ�
		pDev->SetMaterial(&pMat[wCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDev->SetTexture(0, m_pModel->apTex[wCntMat]);

		// �I�u�W�F�N�g�p�[�c�̕`��
		m_pModel->pMesh->DrawSubset(wCntMat);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDev->SetMaterial(&matDef);

	// ���_�@���̎������K���𖳌���
	pDev->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}