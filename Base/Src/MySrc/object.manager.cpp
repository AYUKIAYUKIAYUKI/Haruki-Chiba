//============================================================================
// 
// �I�u�W�F�N�g�}�l�[�W���[ [object.manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.manager.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace OBJ;

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CObjectManager::CObjectManager()
	: m_aAllObjList{}
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObjectManager::~CObjectManager()
{
	for (auto& RefList : m_aAllObjList)
	{
		for (auto itObj = RefList.begin(); itObj != RefList.end(); )
		{
			auto aa = *itObj;
			delete aa;
			aa = nullptr;
			++itObj;
		}
	}
}

//============================================================================
// ��čX�V����
//============================================================================
void CObjectManager::UpdateAllObject()
{
	for (const auto& itRefList : m_aAllObjList)
	{
		for (const auto& itRefObj : itRefList)
		{
			itRefObj->Update();
		}
	}
}

//============================================================================
// ��Ēx���X�V����
//============================================================================
void CObjectManager::LateUpdateAllObject()
{
	for (auto& RefList : m_aAllObjList)
	{
		for (auto itObj = RefList.begin(); itObj != RefList.end(); )
		{
			auto aa = *itObj;

			bool bDeath = aa->GetDeath();

			if (bDeath)
			{
				delete aa;
				aa = nullptr;
				itObj = RefList.erase(itObj);

				continue;
			}

			++itObj;
		}
	}
}

//============================================================================
// ��ĕ`�揈��
//============================================================================
void CObjectManager::DrawAllObject()
{
	for (unsigned char ucLayerIdx = 0; ucLayerIdx < static_cast<unsigned char>(OBJ::LAYER::MAX); ++ucLayerIdx)
	{
		for (const auto& itRefList : m_aAllObjList)
		{
			for (const auto& itRefObj : itRefList)
			{
				OBJ::LAYER Layer = itRefObj->GetLayer();

				if (Layer == static_cast<OBJ::LAYER>(ucLayerIdx))
				{
					itRefObj->Draw();
				}
			}
		}
	}
}

//============================================================================
// ��ĉ���\��
//============================================================================
void CObjectManager::SetDeathAllObject()
{
	for (const auto& itRefList : m_aAllObjList)
	{
		for (const auto& itRefObj : itRefList)
		{
			itRefObj->SetDeath();
		}
	}
}