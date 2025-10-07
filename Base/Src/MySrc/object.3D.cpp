//============================================================================
// 
// �I�u�W�F�N�g(3D) [object.3D.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.3D.h"
#include "renderer.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//============================================================================
// �R���X�g���N�^
//============================================================================
CObject3D::CObject3D(OBJ::TYPE Type, OBJ::LAYER Layer)
	: CObject(Type, Layer)
	, m_pVtxBuff(nullptr)
	, m_pTex(nullptr)
	, m_Size(VEC3_ZERO_INIT)
	, m_Rot(VEC3_ZERO_INIT)
	, m_Pos(VEC3_ZERO_INIT)
	, m_Col(XCOL_ONE_INIT)
	, m_fLength(0.0f)
	, m_fAngle(0.0f)
{
	// ���[���h�s��̏�����
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject3D::~CObject3D()
{}

//============================================================================
// ����������
//============================================================================
bool CObject3D::Initialize()
{
	// ���_�o�b�t�@�̐���
	if (FAILED(CreateVtxBuff()))
	{
		return false;
	}

	// ��߂悤
	m_Size = { 500.0f, 500.0f, 0.0f };

	return true;
}

//============================================================================
// �I������
//============================================================================
void CObject3D::Finalize()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//============================================================================
// �X�V����
//============================================================================
void CObject3D::Update()
{
	if (m_pVtxBuff == nullptr)
	{ // ���_�o�b�t�@������
		assert(false);
	}

	// �K�v�Ȑ��l���v�Z
	m_fAngle = atan2f(m_Size.x, m_Size.y);
	m_fLength = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y);

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = {
		sinf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		-cosf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[1].pos = {
		sinf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		-cosf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[2].pos = {
		sinf(m_Rot.z - m_fAngle) * m_fLength,
		-cosf(m_Rot.z - m_fAngle) * m_fLength,
		0.0f
	};

	pVtx[3].pos = {
		sinf(m_Rot.z + m_fAngle) * m_fLength,
		-cosf(m_Rot.z + m_fAngle) * m_fLength,
		0.0f
	};

	// ���_�F�̐ݒ�
	for (WORD wCntVtx = 0; wCntVtx < NUM_VTX; ++wCntVtx)
	{
		pVtx[wCntVtx].col = m_Col;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// ���[���h�s��̐ݒ�
	SetMtxWorld();
}

//============================================================================
// �`�揈��
//============================================================================
void CObject3D::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::RefInstance().GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDev->SetRenderState(D3DRS_ALPHAREF, 0);
	pDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���C�g���f�𖳌��ɂ���
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�s��̐ݒ�
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDev->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDev->SetTexture(0, m_pTex);

	// �|���S���̕`��
	pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,										// ���_���̐擪�A�h���X
		NUM_PRIM);								// �v���~�e�B�u��

	// ���C�g���f��L���ɂ���
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//============================================================================
// �e�N�X�`������
//============================================================================
void CObject3D::BindTex(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTex = pTex;
}

//============================================================================
// �T�C�Y�擾
//============================================================================
const D3DXVECTOR3& CObject3D::GetSize() const
{
	return m_Size;
}

//============================================================================
// �T�C�Y�ݒ�
//============================================================================
void CObject3D::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// �����擾
//============================================================================
const D3DXVECTOR3& CObject3D::GetRot()const
{
	return m_Rot;
}

//============================================================================
// �����ݒ�
//============================================================================
void CObject3D::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// ���W�擾
//============================================================================
const D3DXVECTOR3& CObject3D::GetPos() const
{
	return m_Pos;
}

//============================================================================
// ���W�ݒ�
//============================================================================
void CObject3D::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// �F���擾
//============================================================================
const D3DXCOLOR& CObject3D::GetCol() const
{
	return m_Col;
}

//============================================================================
// �F��ݒ�
//============================================================================
void CObject3D::SetCol(D3DXCOLOR Col)
{
	m_Col = Col;
}

//============================================================================
// �A���t�@�l���擾
//============================================================================
const float& CObject3D::GetAlpha() const
{
	return m_Col.a;
}

//============================================================================
// �A���t�@�l��ݒ�
//============================================================================
void CObject3D::SetAlpha(float fAlpha)
{
	m_Col.a = fAlpha;
}

//============================================================================
// �W�J�p�Ίp���擾
//============================================================================
const float& CObject3D::GetLength() const
{
	return m_fLength;
}

//============================================================================
// ����
//============================================================================
CObject3D* CObject3D::Create(OBJ::TYPE Type, OBJ::LAYER Layer, LPDIRECT3DTEXTURE9 pTexture)
{
	// �C���X�^���X�𐶐�
	CObject3D* pNewInstance = DBG_NEW CObject3D(Type, Layer);

	// �����o���Ă����珉���ݒ�
	if (pNewInstance == nullptr)
	{
		std::runtime_error("Seisei Situpai 2D");
	}

	// 3D�I�u�W�F�N�g�̏����ݒ�
	pNewInstance->Initialize();

	// �e�N�X�`���w��
	pNewInstance->BindTex(pTexture);

	return pNewInstance;
}

//============================================================================
// ���_�o�b�t�@�̐���
//============================================================================
HRESULT CObject3D::CreateVtxBuff()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::RefInstance().GetDevice();

	// ���_�o�b�t�@�̐���
	pDev->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VTX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (m_pVtxBuff == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	for (WORD wNumVtx = 0; wNumVtx < NUM_VTX; ++wNumVtx)
	{
		// ���_���W�̐ݒ�
		pVtx[wNumVtx].pos = { useful::VEC3_ZERO_INIT };

		// �@���x�N�g���̐ݒ�
		pVtx[wNumVtx].nor = { useful::VEC3_ZERO_INIT };

		// ���_�F�̐ݒ�
		pVtx[wNumVtx].col = useful::XCOL_ONE_INIT;
	}

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = { 0.0f, 0.0f };
	pVtx[1].tex = { 1.0f, 0.0f };
	pVtx[2].tex = { 0.0f, 1.0f };
	pVtx[3].tex = { 1.0f, 1.0f };

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// ���[���h�s��ݒ�
//============================================================================
void CObject3D::SetMtxWorld()
{
	// �v�Z�p�s��
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�s���������
	D3DXMatrixIdentity(&m_MtxWorld);

	// ��]�s��쐬
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_Rot.y,
		m_Rot.x,
		m_Rot.z);

	// ��]�s��Ƃ̊|���Z
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxRot);

	// ���s�ړ��s��쐬
	D3DXMatrixTranslation(&mtxTrans,
		m_Pos.x,
		m_Pos.y,
		m_Pos.z);

	// ���s�ړ��s��Ƃ̊|���Z
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxTrans);
}