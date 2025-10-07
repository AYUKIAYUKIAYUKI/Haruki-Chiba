//============================================================================
// 
// �t�H���g�}�l�[�W���[ [font.manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "font.manager.h"

// �f�o�C�X�擾�p
#include "renderer.h"

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CFontManager::CFontManager()
	: m_upRegistry(std::make_unique<CRegistry<LPD3DXFONT>>())
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CFontManager::~CFontManager()
{
	// �I������
	Finalize();
}

//============================================================================
// ����������
//============================================================================
bool CFontManager::Initialize()
{
	// ��������
	std::function<LPD3DXFONT(const std::string&)> fpCreate =
		[](const std::string& Path) -> LPD3DXFONT
		{
			// �t�H���g�̃|�C���^
			LPD3DXFONT pElement = nullptr;

			// �f�o�C�X���擾
			auto pDev = CRenderer::RefInstance().GetDevice();

			// JSON�t�@�C����W�J
			const JSON& Json = useful::OpenJsonFileMaybeThrow(Path);

			// �p�����[�^���L���X�g
			const int          nSize = Json["Size"].get<int>();
			const std::string& sName = useful::JsonConvertToSJIS(Json["Name"]);

			// �t�H���g�𐶐�
			HRESULT hr = D3DXCreateFont(
				pDev,
				nSize,
				0,
				FW_HEAVY,
				0,
				FALSE,
				SHIFTJIS_CHARSET,
				OUT_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH,
				sName.c_str(),
				&pElement);

			// �������s
			if (FAILED(hr))
			{
				throw std::runtime_error("Failed to Create Font");
			}

			// �t�H���g�̃|�C���^��Ԃ�
			return pElement;
		};

	// �������
	std::function<void(LPD3DXFONT&)> fpRelease =
		[](LPD3DXFONT& pElement) -> void
		{
			// �t�H���g�����
			if (pElement)
			{
				pElement->Release();
				pElement = nullptr;
			}
		};

	// ���W�X�g���̏���������
	if (!m_upRegistry->Initialize(INITIALIZE_PATH, std::move(fpCreate), std::move(fpRelease)))
	{
		return false;
	}

	return true;
}

//============================================================================
// �I������
//============================================================================
void CFontManager::Finalize()
{
	// ���W�X�g���̏I������
	m_upRegistry->Finalize();
}