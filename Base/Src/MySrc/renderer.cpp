//============================================================================
// 
// �����_���[ [renderer.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "renderer.h"

// �e�L�X�g�`��p
#include "window.h"
#include "font.manager.h"

//============================================================================
// �����ݒ�
//============================================================================
bool CRenderer::Initialize(HWND hWnd, BOOL bWindiw)
{
	// Direct3D�̃Z�b�g�A�b�v�p�ϐ�
	D3DDISPLAYMODE        D3ddm; // �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS D3dpp; // �v���[���e�[�V�����p�����[�^

	// �p�����[�^�̃[���N���A
	ZeroMemory(&D3dpp, sizeof(D3dpp));

	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	// Direct3D�I�u�W�F�N�g�̐����`�F�b�N
	useful::PtrCheck(m_pD3D, "Direct3D");

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &D3ddm)))
	{ 
		return false;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	D3dpp.BackBufferWidth            = 1280;                        // �Q�[���T�C�Y�̉���
	D3dpp.BackBufferHeight           = 720;                         // �Q�[���T�C�Y�̍���
	D3dpp.BackBufferFormat           = D3ddm.Format;                // �o�b�N�o�b�t�@�̌`��
	D3dpp.BackBufferCount            = 1;                           // �o�b�N�o�b�t�@�̐�
	D3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;       // �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	D3dpp.EnableAutoDepthStencil     = TRUE;                        // �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	D3dpp.AutoDepthStencilFormat     = D3DFMT_D16;                  // �f�v�X�o�b�t�@�Ƃ���16bit���g��
	D3dpp.Windowed                   = bWindiw;                     // �E�C���h�E���[�h
	D3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;     // ���t���b�V�����[�g
	D3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT; // �C���^�[�o��

	// Direct3D�f�o�C�X�̃Z�b�g�A�b�v - �n�[�h�E�F�A�����D��
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&D3dpp,
		&m_pDevice)))
	{
		// Direct3D�f�o�C�X�̃Z�b�g�A�b�v - �\�t�g�E�F�A�����D��
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&D3dpp,
			&m_pDevice)))
		{
			// Direct3D�f�o�C�X�̃Z�b�g�A�b�v - ���t�@�����X�f�o�C�X
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&D3dpp,
				&m_pDevice)))
			{
				return false;
			}
		}
	}

	// �����_�[�X�e�[�g�̏���������
	InitializeRenderState();

	// ImGui�̃R���e�L�X�g���쐬
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// ImGui�̓��o�͐ݒ�
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// ImGui�̕\���X�^�C����ݒ�
	ImGui::StyleColorsDark();

	// �o�b�N�G���h�̏����ݒ�
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(m_pDevice);

	// ImGui�̃t�H���g�ݒ�
	io.Fonts->AddFontFromFileTTF("Data\\FONT\\Meiryo\\meiryo.ttc", 20.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	return true;
}

//============================================================================
// �I������
//============================================================================
void CRenderer::Finalize()
{
	// ImGUi�̏I������
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// �f�o�C�X�̉��
	if (m_pDevice != nullptr)
	{
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// Direct3D�I�u�W�F�N�g�̉��
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CRenderer::CRenderer()
	: m_pD3D(nullptr)
	, m_pDevice(nullptr)
	, m_vText{}
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CRenderer::~CRenderer()
{
	Finalize();
}

//============================================================================
// �����_�[�X�e�[�g�̏����Z�b�g�A�b�v
//============================================================================
void CRenderer::InitializeRenderState()
{
	// �����_�[�X�e�[�g�̏����ݒ�
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);           // �J�����O���[�h�̐ݒ�
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);          // �A���t�@�u�����h�̗L����
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);     // �\�[�X�u�����h�̐ݒ�
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // �^�[�Q�b�g�u�����h�̐ݒ�
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);           // �A���t�@�e�X�g�̗L����
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x24);                  // �A���t�@�Q�ƒl
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);       // �A���t�@�e�X�g���i�
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);          // �A���t�@�e�X�g�̖�����
	m_pDevice->SetRenderState(D3DRS_STENCILMASK, 0x000000ff);         // �X�e���V���}�X�N��ݒ�
	
	// �t�H�O�ݒ�
	//m_pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);                             // �t�H�O�̗L����
	//m_pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);                 // �t�H�O���[�h�̐ݒ�
	//m_pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)); // �t�H�O�J���[�̐ݒ�
	//m_pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_fFogStart));           // �n�_��ݒ�
	//m_pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_fFogEnd));               // �I�_��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̏����ݒ�
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

//============================================================================
// �o�b�N�o�b�t�@�̃T�C�Y�𒲐�
//============================================================================
void CRenderer::AdjustBuckBufferSize()
{}

//============================================================================
// �e�L�X�g�̕\��
//============================================================================
void CRenderer::PrintText()
{
	// �t�H���g�}�l�[�W���[�̎擾
	LPD3DXFONT pFont = CFontManager::RefInstance().RefRegistry().BindAtKey("Meiryo");

	// �\���͈͒����p
	static WORD wAdjustHeight = 0;

	// �\���ʒu
	RECT Rect = { 0, 0 - wAdjustHeight, CWindow::WWINDOW_WIDTH, CWindow::WWINDOW_HEIGHT - wAdjustHeight };

	// �\�����镶����
	std::string PrintStr = {};

	for (int nIdx = 0; nIdx < m_vText.size(); ++nIdx)
	{
		if (m_vText[nIdx].second > 0)
		{
			// �\�����Ԃ��J�E���g�_�E��
			m_vText[nIdx].second--;

			// ���͂��Ȃ���
			PrintStr += m_vText[nIdx].first + "\n";
		}
		else
		{
			// ���͂�����
			m_vText.erase(m_vText.begin() + nIdx);
		}
	}

	// �e�L�X�g�̕`��
	pFont->DrawText(NULL, PrintStr.c_str(), -1, &Rect, DT_CENTER | DT_VCENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
}