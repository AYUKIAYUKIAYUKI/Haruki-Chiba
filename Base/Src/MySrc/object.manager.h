//============================================================================
// 
// �I�u�W�F�N�g�}�l�[�W���[�A�w�b�_�[�t�@�C�� [object.manager.h]
// Author : ���c����
// 
//============================================================================

#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "singleton.h"
#include "object.h"

//****************************************************
// �I�u�W�F�N�g�}�l�[�W���[�N���X���`
//****************************************************
class CObjectManager : public CSingleton<CObjectManager>
{
	//****************************************************
	// �t�����h�錾
	//****************************************************
	friend struct std::default_delete<CObjectManager>;
	friend CObjectManager& CSingleton<CObjectManager>::RefInstance();

	//****************************************************
	// �^�G�C���A�X���`
	//****************************************************
	using ObjList = std::list<CObject*>;

	//****************************************************
	// �ÓI�����o�萔���` (����J)
	//****************************************************

	// ���e����I�u�W�F�N�g�̍ő吔
	template <typename T = unsigned char>
	static constexpr T MAX_OBJECT = (std::numeric_limits<T>::max)();

public:

	//****************************************************
	// special funciton
	//****************************************************
	CObjectManager();  // �f�t�H���g�R���X�g���N�^
	~CObjectManager(); // �f�X�g���N�^

	//****************************************************
	// funciton
	//****************************************************

	// ����������
	bool Initialize() { return true; }

	// �I������
	void Finalize() {}

	// �ۗL����S�I�u�W�F�N�g�̍X�V
	void UpdateAllObject();

	// �ۗL����S�I�u�W�F�N�g�̒x���X�V
	// ���S�t���O�̑Ώ��Ȃ񂩂�����
	void LateUpdateAllObject();

	// �ۗL����S�I�u�W�F�N�g�ɑ΂��Ď��S�t���O�𗧂Ă�
	void SetDeathAllObject();

	// �ۗL����S�I�u�W�F�N�g�̕`��
	void DrawAllObject();

	//****************************************************
	// inline function
	//****************************************************

	// �V�K�I�u�W�F�N�g�̓o�^
	inline void RegisterObj(CObject*&& pObj) { m_aAllObjList[static_cast<unsigned char>(pObj->GetType())].push_back(pObj); }

	// �w�肵���^�C�v�̃I�u�W�F�N�g�̃��X�g���擾
	inline const ObjList& RefObjList(OBJ::TYPE Type) { return m_aAllObjList[static_cast<unsigned char>(Type)]; }

	// �I�u�W�F�N�g�̃��X�g���擾
	inline const std::array<ObjList, static_cast<unsigned char>(OBJ::TYPE::MAX)>& RefObjList() { return m_aAllObjList; }

private:

	//****************************************************
	// data
	//****************************************************

	// �ۗL����S�Ẵ^�C�v���̃��X�g
	std::array<ObjList, static_cast<unsigned char>(OBJ::TYPE::MAX)> m_aAllObjList;
};