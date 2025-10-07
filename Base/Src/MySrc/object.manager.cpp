//============================================================================
// 
// オブジェクトマネージャー [object.manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "object.manager.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace OBJ;

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CObjectManager::CObjectManager()
	: m_aAllObjList{}
{}

//============================================================================
// デストラクタ
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
// 一斉更新処理
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
// 一斉遅延更新処理
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
// 一斉描画処理
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
// 一斉解放予約
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