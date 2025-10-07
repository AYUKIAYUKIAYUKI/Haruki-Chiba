
// VS2022�����S�Ή�����܂Ń��W���[���͑S�ʓI�Ɏg�p�o���Ȃ����̂Ƃ��܂�

////============================================================================
//// 
//// ���W�X�g���N���X�e���v���[�g�A���W���[���C���^�[�t�F�[�X [registry.ixx]
//// Author : ���c����
//// 
////============================================================================
//
////****************************************************
//// ���W���[���錾
////****************************************************
//export module registry;
//
////****************************************************
//// ���W�X�g���N���X�e���v���[�g�̒�`
////****************************************************
//export template <typename T>
//class CRegistry
//{
//public:
//
//	//****************************************************
//	// special function
//	//****************************************************
//	CRegistry();  // �f�t�H���g�R���X�g���N�^
//	~CRegistry(); // �f�X�g���N�^
//
//	//****************************************************
//	// function
//	//****************************************************s
//
//	// ����������
//	// Path      -> �f�[�^�̃p�X
//	// fpFactory -> �f�[�^�̂��߂̃t�@�N�g���֐�
//	// fpRelease -> �f�[�^�̂��߂̃����[�X�֐�
//	bool Initialize(const std::string& Path, std::function<T(const std::string&)>&& fpFactory, std::function<void(T&)>&& fpRelease);
//	
//	// �I������
//	void Finalize();
//
//	// �L�[���󂯎���āA�ۗL���Ă��镨��Ԃ�
//	T BindAtKey(const std::string& Key);
//
//	// �����^�C�������̂̐V�K�o�^
//	bool Register(const std::string& Key, const std::string& Path);
//
//private:
//
//	//****************************************************
//	// data
//	//****************************************************
//	std::function<T(const std::string&)> m_fpFactory; // �f�[�^�̂��߂̃t�@�N�g���֐�
//	std::function<void(T&)>              m_fpRelease; // �f�[�^�̂��߂̃����[�X�֐�
//	std::map<std::string, T>             m_mData;     // �L�[�ƃf�[�^�̃}�b�v
//};
//
////============================================================================
//// �f�t�H���g�R���X�g���N�^
////============================================================================
//template <typename T>
//CRegistry<T>::CRegistry()
//	: m_fpFactory(nullptr)
//	, m_fpRelease(nullptr)
//	, m_mData{}
//{}
//
////============================================================================
//// �f�X�g���N�^
////============================================================================
//template <typename T>
//CRegistry<T>::~CRegistry()
//{
//	// �I������
//	Finalize();
//}
//
////============================================================================
//// ����������
////============================================================================
//template <typename T>
//bool CRegistry<T>::Initialize(const std::string& Path, std::function<T(const std::string&)>&& fpFactory, std::function<void(T&)>&& fpRelease)
//{
//	// �������j���̊֐��������ꍇ�������o���Ȃ�
//	if (!fpFactory || !fpRelease)
//	{
//		return false;
//	}
//
//	// �󂯎�����֐������[�u
//	m_fpFactory = std::move(fpFactory);
//	m_fpRelease = std::move(fpRelease);
//
//	// �w��p�X��JSON�t�@�C����W�J
//	const JSON& Json = useful::OpenJsonFileMaybeThrow(Path);
//
//	// �f�[�^�����m�F
//	const decltype(Json["Element"].size()) MaxSize = Json["Element"].size();
//
//	// �f�[�^�̐������s���A�}�b�v�Ɋi�[����
//	for (int nCntIdx = 0; nCntIdx < MaxSize; ++nCntIdx)
//	{
//		m_mData.insert(std::make_pair(Json["Element"][nCntIdx][0], m_fpFactory(Json["Element"][nCntIdx][1])));
//	}
//
//	return true;
//}
//
////============================================================================
//// �I������
////============================================================================
//template <typename T> 
//void CRegistry<T>::Finalize()
//{
//	// ����p�̊֐�������
//	if (m_fpRelease)
//	{
//		std::runtime_error("Kaihouyou Kansu ga Nai");
//	}
//
//	// ���O�Ɏ󂯎���Ă����֐��őS�Ẵf�[�^�����
//	for (auto& it : m_mData)
//	{
//		m_fpRelease(it.second);
//	}
//
//	// �}�b�v���N���A
//	m_mData.clear();
//}
//
////============================================================================
//// �L�[���󂯎���āA�ۗL���Ă��镨��Ԃ�
////============================================================================
//template <typename T>
//T CRegistry<T>::BindAtKey(const std::string& Key)
//{
//	// �w�肳�ꂽ�L�[�ɑΉ�����f�[�^��T��
//	const decltype(m_mData)::iterator& it = m_mData.find(Key);
//
//	// ������Ȃ������ꍇ�͗�O�𓊂���
//	if (it == m_mData.end())
//	{
//		throw std::runtime_error("'BindAtKey' was Failed : " + Key);
//	}
//
//	// ���������ꍇ�͑Ή�����f�[�^��Ԃ�
//	return it->second;
//}
//
////============================================================================
//// �����^�C�������̂̐V�K�o�^
////============================================================================
//template <typename T>
//bool CRegistry<T>::Register(const std::string& Key, const std::string& Path)
//{
//	// �����p�̊֐�������
//	if (m_fpFactory)
//	{
//		std::runtime_error("Seiseiyou Kansu ga Nai");
//	}
//
//	// �w�肳�ꂽ�L�[�ɑΉ�����f�[�^��T��
//	const decltype(m_mData)::iterator& it = m_mData.find(Key);
//
//	// ���ɓo�^����Ă���ꍇ�͓o�^�o���Ȃ�
//	if (it != m_mData.end())
//	{
//		return false;
//	}
//
//	// �V�K�o�^
//	m_mData.insert(std::make_pair(Key, m_fpFactory(Path)));
//
//	// �o�^�̐�����ʒm����
//	return true;
//}