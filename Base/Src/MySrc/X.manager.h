//============================================================================
// 
// X���f���}�l�[�W���[�A�w�b�_�[�t�@�C�� [X.manager.h]
// Author : ���c����
// 
//============================================================================

#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "singleton.h"
#include "registry.h"

//****************************************************
// X���f���}�l�[�W���[�N���X�̒�`
//****************************************************
class CXManager final : public CSingleton<CXManager>
{
	//****************************************************
	// �t�����h�錾
	//****************************************************
	friend struct std::default_delete<CXManager>;
	friend CXManager& CSingleton<CXManager>::RefInstance();

	//****************************************************
	// �ÓI�����o�ϐ��̒�`
	//****************************************************
	static constexpr const char* INITIALIZE_PATH = "Data\\JSON\\Model.List.json";

public:

	//****************************************************
	// ���f�����̍\���̂̒�`
	//****************************************************
	struct MODEL
	{
		LPD3DXMESH			pMesh;    // ���b�V�����
		D3DXVECTOR3			Size;     // �T�C�Y
		LPD3DXBUFFER		pBuffMat; // �}�e���A�����
		DWORD				dwNumMat; // �}�e���A����
		D3DXCOLOR*			apColMat; // �}�e���A���F	
		LPDIRECT3DTEXTURE9* apTex;    // �e�N�X�`��
	};

	//****************************************************
	// function
	//****************************************************

	// ���W�X�g���̎Q�Ƃ̎擾
	inline CRegistry<MODEL*>& RefRegistry() const { return *m_upRegistry; }

private:

	//****************************************************
	// special function
	//****************************************************
	CXManager();           // �f�t�H���g�R���X�g���N�^
	~CXManager() override; // �f�X�g���N�^

	//****************************************************
	// function
	//****************************************************
	bool Initialize(); // ����������
	void Finalize();   // �I������
	
	// �T�C�Y�ǂݍ���
	D3DXVECTOR3 LoadSize(const std::string& sPath);

	//****************************************************
	// data
	//****************************************************
	std::unique_ptr<CRegistry<MODEL*>>        m_upRegistry; // X���f���p���W�X�g��
	std::map<std::string, LPDIRECT3DTEXTURE9> m_mCache;     // �e�N�X�`���̃L���b�V�����
};