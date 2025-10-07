//============================================================================
// 
// �I�u�W�F�N�g�A�w�b�_�[�t�@�C�� [object.h]
// Author : ���c����
// 
//============================================================================

#pragma once

//****************************************************
// ���O���OBJ���`
//****************************************************
namespace OBJ
{
	/* ���̏ꏊ�̓����o�Ɏ�������^�C�v�����p��enum�Ȃǂ̂ݒ�`���Ă��܂� */

	//****************************************************
	// �^�C�v���ʗp�̗񋓌^���`
	//****************************************************
	enum class TYPE : unsigned char
	{
		NONE = 0, // ����
		PLAYER,   // �v���C���[
		MAX
	};

	//****************************************************
	// �`��D��x�w��p�̗񋓌^���`
	//****************************************************
	enum class LAYER : unsigned char
	{
		BG = 0,  // �w�i
		DEFAULT, // �ʏ�
		FRONT,   // �O��
		STENCIL, // �X�e���V��
		UI,      // UI
		MAX
	};
}

//****************************************************
// �O���錾
//****************************************************
class CObject;

//****************************************************
// �R���Z�v�g���`
//****************************************************

// �I�u�W�F�N�g���p�����Ă��邱�Ƃ̃R���Z�v�g
template <typename T>
concept DerivedFromObject = std::derived_from<T, CObject>;

#define CONCEPT_�� 0
#define CONCEPT_�� 0
// �I�u�W�F�N�g�̃|�C���^���󂯎��֐��I�u�W�F�N�g�ł��邱�Ƃ̃R���Z�v�g
#if CONCEPT_��
template <typename T>
concept ObjectFactoryFunction = requires(T t, CObject* pObj)
{
	{ t(pObj) } -> std::same_as<bool>;
};
#elif CONCEPT_��
concept ObjectFactoryFunction = std::is_pointer_v<T>
&& std::is_function_v<std::remove_pointer_t<T>>;
#endif

//****************************************************
// �I�u�W�F�N�g�N���X���`
//****************************************************
class CObject
{
public:

	//****************************************************
	// special function
	//****************************************************

	// �R���X�g���N�^
	CObject(OBJ::TYPE Type, OBJ::LAYER Layer);

	// �f�X�g���N�^
	virtual ~CObject() = default;

	//****************************************************
	// virtual function
	//****************************************************
	virtual void Update() = 0; // �X�V����
	virtual void Draw()   = 0; // �`�揈��

	//****************************************************
	// function
	//****************************************************

	// ��������
#if CONCEPT_�� || CONCEPT_��
	template <DerivedFromObject T, ObjectFactoryFunction U>
	static T* Create(OBJ::TYPE Type, OBJ::LAYER Layer, U&& fpInitialize);
#else
	template <DerivedFromObject T, typename U>
	static T* Create(OBJ::TYPE Type, OBJ::LAYER Layer, U&& fpInitialize);
#endif

	//****************************************************
	// inline function
	//****************************************************

	// �^�C�v�̑���p
	inline OBJ::TYPE GetType() const         { return m_Type; }
	inline void      SetType(OBJ::TYPE Type) { m_Type = Type; }

	// �`��D��x�̑���p
	inline OBJ::LAYER GetLayer() const           { return m_Layer; }
	inline void       SetLayer(OBJ::LAYER Layer) { m_Layer = Layer; }

	// ���S�t���O�̐ݒ�
	inline bool GetDeath() const             { return m_bDeath; }
	inline void SetDeath(bool bState = true) { m_bDeath = bState; }

private:

	//****************************************************
	// data
	//****************************************************
	OBJ::TYPE  m_Type;   // �I�u�W�F�N�g�^�C�v
	OBJ::LAYER m_Layer;  // �`��D��x
	bool       m_bDeath; // ���S�t���O
};

// �e���v���[�g�����t�@�C��
#include "object.impl"