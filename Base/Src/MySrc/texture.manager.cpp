//============================================================================
// 
// �e�N�X�`���}�l�[�W���[ [texture.manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "texture.manager.h"

// �f�o�C�X�擾�p
#include "renderer.h"

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CTextureManager::CTextureManager()
	: m_upRegistry(std::make_unique<CRegistry<LPDIRECT3DTEXTURE9>>())
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTextureManager::~CTextureManager()
{
	// �I������
	Finalize();
}

//============================================================================
// ����������
//============================================================================
bool CTextureManager::Initialize()
{
	// ��������
	std::function<LPDIRECT3DTEXTURE9(const std::string&)> fpCreate = 
		[](const std::string& Path) -> LPDIRECT3DTEXTURE9
	{
		// �e�N�X�`���̃|�C���^
		LPDIRECT3DTEXTURE9 pElement = nullptr;

		// �f�o�C�X���擾
		auto pDev = CRenderer::RefInstance().GetDevice();

		// �e�N�X�`���𐶐�
		HRESULT hr = D3DXCreateTextureFromFileA(
			pDev,
			Path.c_str(),
			&pElement);

		// �������s
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to Create Texture");
		}

		// �e�N�X�`���̃|�C���^��Ԃ�
		return pElement;
	};

	// �������
	std::function<void(LPDIRECT3DTEXTURE9&)> fpRelease =
		[](LPDIRECT3DTEXTURE9& pElement) -> void
	{
		// �e�N�X�`�������
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
void CTextureManager::Finalize()
{
	// ���W�X�g���̏I������
	m_upRegistry->Finalize();
}