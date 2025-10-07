//============================================================================
// 
// �I�u�W�F�N�g(2D) [object.2D.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.2D.h"
#include "renderer.h"

// �N���C�A���g�̈�̃T�C�Y�擾�p
#include "window.h"

// �����������Ńe�N�X�`�����擾���邽��
#include "texture.manager.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �ÓI�����o�ϐ��̒�` (���J)
//****************************************************

// �f�t�H���g�̃t�@�N�g��
std::function<bool(CObject2D*)> CObject2D::s_fpDefaultFactory =
[](CObject2D* pObj) -> bool
	{
		// 2D�I�u�W�F�N�g�̏����ݒ�
		pObj->Initialize();

		// �e�N�X�`���̎擾
		auto pTex = useful::PtrCheck(CTextureManager::RefInstance().RefRegistry().BindAtKey("Unko"), "Syokika Lamda no Naka Texture Nai");

		// �e�N�X�`���w��
		pObj->SetTex(pTex);

		// �T�C�Y�w��
		pObj->SetSize({ 30.0f, 30.0f });
		pObj->SetPos({ CWindow::FWINDOW_WIDTH * 0.5f, CWindow::FWINDOW_HEIGHT * 0.5f });

		return true;
	};

//============================================================================
// �R���X�g���N�^
//============================================================================
CObject2D::CObject2D(OBJ::TYPE Type, OBJ::LAYER Layer)
	: CObject(Type, Layer)
	, m_pVtxBuff(nullptr)
	, m_pTex(nullptr)
	, m_Size(VEC2_ZERO_INIT)
	, m_fRot(0.0f)
	, m_Pos(VEC2_ZERO_INIT)
	, m_Col(XCOL_ONE_INIT)
	, m_fLength(0.0f)
	, m_fAngle(0.0f)
	, m_TexSize(VEC2_ZERO_INIT)
	, m_fTexWidth(1.0f)
	, m_fTexHeight(1.0f)
	, m_nNowPatternU(0)
	, m_nNowPatternV(0)
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject2D::~CObject2D()
{}

//============================================================================
// ����������
//============================================================================
bool CObject2D::Initialize()
{
	// ���_�o�b�t�@�̐���
	if (FAILED(CreateVtxBuff()))
	{
		return false;
	}

	return true;
}

//============================================================================
// �I������
//============================================================================
void CObject2D::Finalize()
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
void CObject2D::Update()
{
	// ���_�o�b�t�@������
	if (!m_pVtxBuff)
	{ 
		std::runtime_error("Buffer ga Naiyo 2D");
	}

	// �K�v�Ȑ��l���Z�o
	m_fLength = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y);
	m_fAngle  = atan2f(m_Size.x, m_Size.y);

	// ���݂̃N���C�A���g�̈�̃T�C�Y�̎Q��
	const RECT rNowWindowRect = CWindow::RefInstance().GetWindowRect();

	// �X�P�[�����O�W��
	float fWindowScaleX = static_cast<float>(rNowWindowRect.right)  / CWindow::FWINDOW_WIDTH;
	float fWindowScaleY = static_cast<float>(rNowWindowRect.bottom) / CWindow::FWINDOW_HEIGHT;

	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = 
	{
		(m_Pos.x + sinf(m_fRot - (D3DX_PI - m_fAngle)) * m_fLength) * fWindowScaleX,
		(m_Pos.y + cosf(m_fRot - (D3DX_PI - m_fAngle)) * m_fLength) * fWindowScaleY,
		0.0f
	};

	pVtx[1].pos = 
	{
		(m_Pos.x + sinf(m_fRot + (D3DX_PI - m_fAngle)) * m_fLength) * fWindowScaleX,
		(m_Pos.y + cosf(m_fRot + (D3DX_PI - m_fAngle)) * m_fLength) * fWindowScaleY,
		0.0f
	};

	pVtx[2].pos = 
	{
		(m_Pos.x + sinf(m_fRot - m_fAngle) * m_fLength) * fWindowScaleX,
		(m_Pos.y + cosf(m_fRot - m_fAngle) * m_fLength) * fWindowScaleY,
		0.0f
	};

	pVtx[3].pos = 
	{
		(m_Pos.x + sinf(m_fRot + m_fAngle) * m_fLength) * fWindowScaleX,
		(m_Pos.y + cosf(m_fRot + m_fAngle) * m_fLength) * fWindowScaleY,
		0.0f
	};

	// ���_�F�̐ݒ�
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = { m_fTexWidth * m_nNowPatternU, m_fTexHeight * m_nNowPatternV };
	pVtx[1].tex = { m_fTexWidth * (m_nNowPatternU + 1) - m_TexSize.x, m_fTexHeight * m_nNowPatternV };
	pVtx[2].tex = { m_fTexWidth * (m_nNowPatternU), m_fTexHeight * (m_nNowPatternV + 1) };
	pVtx[3].tex = { m_fTexWidth * (m_nNowPatternU + 1) - m_TexSize.x, m_fTexHeight * (m_nNowPatternV + 1) };

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//============================================================================
// �`�揈��
//============================================================================
void CObject2D::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::RefInstance().GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDev->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDev->SetTexture(0, m_pTex);

	// �|���S���̕`��
	pDev->DrawPrimitive(
		D3DPT_TRIANGLESTRIP, // �v���~�e�B�u�̎��
		0,                   // ���_���̐擪�A�h���X
		NUM_PRIM);           // �v���~�e�B�u��
}

//============================================================================
// ���_�o�b�t�@�̐���
//============================================================================
HRESULT CObject2D::CreateVtxBuff()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::RefInstance().GetDevice();

	// ���_�o�b�t�@�̐���
	pDev->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VTX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	// �������s
	if (!m_pVtxBuff)
	{
		return E_FAIL;
	}

	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	for (WORD wNumVtx = 0; wNumVtx < NUM_VTX; ++wNumVtx)
	{
		// ���_���W�̐ݒ�
		pVtx[wNumVtx].pos = VEC3_ZERO_INIT;

		// ���Z���̐ݒ�
		pVtx[wNumVtx].rhw = 1.0f;

		// ���_�F�̐ݒ�
		pVtx[wNumVtx].col = XCOL_ONE_INIT;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[wNumVtx].tex = VEC2_ZERO_INIT;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}