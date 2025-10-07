//============================================================================
// 
// ���[�X�t���A�w�b�_�[�t�@�C�� [useful.h]
// Author : ���c����
// 
//============================================================================

#pragma once

//****************************************************
// ���O���useful���`
//****************************************************
namespace useful
{
	//****************************************************
	// using�錾
	//****************************************************
	using std::unique_ptr;
	using std::make_unique;
	using std::to_string;

	//****************************************************
	// �^�G�C���A�X���`
	//****************************************************
	
	// std::unique_ptr
	template <typename T> using up = unique_ptr<T>;

	// DirectX9
	using XCol = D3DXCOLOR;
	using Vec2 = D3DXVECTOR2;
	using Vec3 = D3DXVECTOR3;
	using Mtx  = D3DXMATRIX;

	//****************************************************
	// �R���Z�v�g�̒�`
	//****************************************************

	// �����o��SRT�̌��ݒl�E�ڕW�l�̗�����������N���X�ł��邱�Ƃ̃R���Z�v�g
	template <typename T>
	concept HasSRTAndChangeable = requires(T t, const Vec3& SRT)
	{
		{ t.GetPos() } -> std::same_as<const Vec3&>;
		{ t.SetPos(SRT) } -> std::same_as<void>;

		{ t.GetPosTarget() } -> std::same_as<const Vec3&>;
		{ t.SetPosTarget(SRT) } -> std::same_as<void>;

		{ t.GetRot() } -> std::same_as<const Vec3&>;
		{ t.SetRot(SRT) } -> std::same_as<void>;

		{ t.GetRotTarget() } -> std::same_as<const Vec3&>;
		{ t.SetRotTarget(SRT) } -> std::same_as<void>;

		{ t.GetSize() } -> std::same_as<const Vec3&>;
		{ t.SetSize(SRT) } -> std::same_as<void>;

		{ t.GetSizeTarget() } -> std::same_as<const Vec3&>;
		{ t.SetSizeTarget(SRT) } -> std::same_as<void>;
	};
	
	//****************************************************
	// �ÓI�萔���`
	//****************************************************

	// D3DXCOLOR�������p
	static const XCol XCOL_ZERO_INIT = XCol(0.0f, 0.0f, 0.0f, 0.0f);
	static const XCol XCOL_ONE_INIT  = XCol(1.0f, 1.0f, 1.0f, 1.0f);

	// D3DXVECTOR2�������p
	static const Vec2 VEC2_ZERO_INIT = Vec2(0.0f, 0.0f);
	static const Vec2 VEC2_ONE_INIT  = Vec2(1.0f, 1.0f);

	// D3DXVECTOR3�������p
	static const Vec3 VEC3_ZERO_INIT = Vec3(0.0f, 0.0f, 0.0f);
	static const Vec3 VEC3_ONE_INIT  = Vec3(1.0f, 1.0f, 1.0f);

	//****************************************************
	// ���C���p�\���̂��`
	//****************************************************
	struct RAY
	{
		// �f�t�H���g�R���X�g���N�^
		RAY()
			: Origin(VEC3_ZERO_INIT)
			, Direction(VEC3_ZERO_INIT)
		{}

		Vec3 Origin;	// ���C�̎n�_
		Vec3 Direction;	// ���C�̕���
	};

	//****************************************************
	// �C�����C���֐��e���v���[�g���`
	//****************************************************

	// '~' ���Z�q�̃I�[�o�[���[�h
	template <typename T, typename U = unsigned char>
	inline T operator~(T l)
	{
		return static_cast<T>(~static_cast<U>(l));
	}

	// '&' ���Z�q�̃I�[�o�[���[�h
	template <typename T, typename U = unsigned char>
	inline T operator&(T l, T r)
	{
		return static_cast<T>(static_cast<U>(l) & static_cast<U>(r));
	}

	// '|' ���Z�q�̃I�[�o�[���[�h
	template <typename T, typename U = unsigned char>
	inline T operator|(T l, T r)
	{
		return static_cast<T>(static_cast<U>(l) | static_cast<U>(r));
	}

	/**
	 * �_���ς̔�r���̏ȗ��`�ł��A�񋓎q��'None'�̒�`���O��ł�
	 *
	 * Flags -> 'None'�Ɣ�r�������t���O
	 */
	template <typename T>
	inline bool AndCheck(T Flags)
	{
		return Flags != T::None;
	}

	/**
	 * �|�C���^�^�̃����o��null�`�F�b�N
	 * �Ԃ�l�̓`�F�b�N�����|�C���^�����̂܂�
	 *
	 * Ptr   -> �`�F�b�N�̑Ώ�
	 * pName -> �G���[���O�p�̕ϐ���
	 */
	template <typename T> inline T* PtrCheck(T* Ptr, const char* pName)
	{
		if (!Ptr)
		{
			throw std::runtime_error(static_cast<std::string>(pName) + "is nullptr");
		}

		return Ptr;
	}

	/**
	 * �ėp�����̂݁A��O�����Ȃǂ͈�؍s���܂���
	 * �Ԃ�l����
	 * 
	 * up        -> ������̃|�C���^�̎Q�� 
	 * fpFactory -> �t�@�N�g���֐�
	 */
	template <typename T, typename U>
	inline void Compose(std::unique_ptr<T>& up, U&& fpFactory)
	{
		up = std::forward<U>(fpFactory)();
	}

	/**
	 * ���`���
	 * �Ԃ�l�͖���̌v�Z����
	 *
	 * Base   -> ���ݒl
	 * Target -> �ڕW�l
	 * fRate  -> �����̋��x
	 */
	template <typename T>
	inline T Lerp(const T& Base, const T& Target, float fRate)
	{
		return Base + (Target - Base) * fRate;
	}

	/**
	 * �w������
	 * �Ԃ�l����
	 *
	 * Base   -> ���ݒl
	 * Target -> �ڕW�l
	 * fCoef  -> �����̋��x
	 */
	template <typename T>
	inline void ExponentialDecay(T& Base, T Target, float fCoef)
	{
		Base += (Target - Base) * fCoef;
	}

	/**
	 * �I���W�i����������
	 * �Ԃ�l�͎w�肵���^�̗���
	 */
	template <typename T>
	inline T GetRandomValue()
	{
		// ���l�̎��͈͂�ݒ�
		const int nRange = 180;

		// �{�͈̔͂��烉���_���Ȑ����𐶐�
		int nRandom{ rand() % (nRange * 2) };

		// ���Ƃ͈̔͂𒴂��Ă����ꍇ�����𔽓]
		if (nRandom > nRange)
		{
			nRandom %= nRange;
			nRandom *= -1;
		}

		// �������w�肳�ꂽ�^�ɃL���X�g
		return static_cast<T>(nRandom);
	}

	/**
	 * �I���W�i����������2
	 * �Ԃ�l�͎w�肵���^�̗���
	 */
	template <typename T> 
	T GetRandomValue(T Range)
	{
		// ���l�̎��͈͂�ݒ�
		const int nRange = static_cast<int>(Range);

		// �{�͈̔͂��烉���_���Ȑ����𐶐�
		int nRandom{ rand() % (nRange * 2) };

		// ���Ƃ͈̔͂𒴂��Ă����ꍇ�����𔽓]
		if (nRandom > nRange)
		{
			nRandom %= nRange;
			nRandom *= -1;
		}

		// �߂�l�̌^�͈����ˑ�
		return static_cast<T>(nRandom);
	}

	/**
	 * �_�E���L���X�g
	 * �Ԃ�l�͎w�肵���^�̃|�C���^
	 * 
	 * pBase -> �ύX�������^�̃|�C���^
	 */
	template <typename T1, typename T2>
	T1* DownCast(T2* pBase)
	{
		// �����̃I�u�W�F�N�g���w�肳�ꂽ�N���X�Ƀ_�E���L���X�g
		T1* pObj = dynamic_cast<T1*>(pBase);

		// �_�E���L���X�g���s
		if (!pObj)
		{
			throw std::bad_cast();
		}

		return pObj;
	}

	//****************************************************
	// �֐��e���v���[�g���`
	//****************************************************

	/**
	 * �ėp�����A�O��Ƃ��� 'Inirialzie()' �� 'Release()' �������o�Ɏ��N���X����
	 * �Ԃ�l�̓C���X�^���X�̃|�C���^
	 */
	template <typename T>
	T* Create();

	/**
	 * �ėp���j�[�N�|�C���^�����A�O��Ƃ��� 'Inirialzie()' �� 'Release()' �������o�Ɏ��N���X����
	 * �e���v���[�g�����Ɏw�肳�ꂽ�^�̃f�t�H���g�R���X�g���N�^�Ő������܂�
	 * �Ԃ�l�̓C���X�^���X�̃��j�[�N�|�C���^
	 */
	template <typename T>
	up<T> CreateUniquePtr();

	/**
	 * �ėp����A�O��Ƃ��� 'Release()' �������o�Ɏ��N���X����
	 * �Ԃ�l����
	 *
	 * p -> ����������C���X�^���X���Q��
	 */
	template <typename T>
	void Release(T*& p);

	/**
	 * unique_ptr�^�̒��g���A�e���v���[�g�����Ɏw�肳�ꂽ�^�̃f�t�H���g�R���X�g���N�^��make_unique����������
	 * �Ԃ�l����
	 *
	 * up -> ����������ΏہA����ѐV�K�C���X�^���X�̊i�[���unique_ptr
	 */
	template <typename T, typename U>
	void ChangeUniquePtr(up<T>& up);

	/**
	 * �I�u�W�F�N�g�̌�����Yaw��]
	 * �Ԃ�l����
	 *
	 * Base  -> 'GetRot()' �� 'SetRot()' �������o�Ɏ��I�u�W�F�N�g����
	 * fCoef -> ���Z�����]�ʁA�������]�������w��
	 */
	template <typename T>
	void RotateYawAxis(T& Base, float fCoef);

	/**
	 * �I�u�W�F�N�g�̖ڕW������Yaw��]
	 * �Ԃ�l����
	 *
	 * Base  -> 'GetRotTarget()' �� 'SetRotTarget()' �������o�Ɏ��I�u�W�F�N�g����
	 * fCoef -> ���Z�����]�ʁA�������]�������w��
	 */
	template <typename T>
	void RotateTargetYawAxis(T& Base, float fCoef);

	/**
	 * SRT�����o���w������
	 * 
	 * Base  -> �Ώۂ̃I�u�W�F�N�g
	 * fCoef -> �����̋��x
	 */
	//template <typename T>
	//void ExponentialDecayForSRT(T& Base, float fCoef);

	//****************************************************
	// �v���g�^�C�v�錾
	//****************************************************

	/**
	 * ���_�t�H�[�}�b�g�̎��e�N�X�`���}�b�s���O�������������܂�
	 * �Ԃ�l����
	 *
	 * rVtx -> ���_�t�H�[�}�b�g�̃|�C���^�Q�ƁA�K�����b�N���ꂽ����̃|�C���^��n���Ă�������
	 */
	void InitializeTextureMappingInVertex(VERTEX_2D*& rVtx);
	void InitializeTextureMappingInVertex(VERTEX_3D*& rVtx);

	/**
	 * ���[���h�s��̌v�Z�A�����Ń[���w�肷��Ƃ��̗v�f�𖳎����܂�
	 * �Ԃ�l����
	 *
	 * mtxWorld -> ���[���h�s��
	 * Scale    -> �g�k
	 * Rot      -> ����
	 * Pos      -> ���W
	 */
	void UpdateMtxWorld(Mtx& mtxWorld, const Vec3& Scale, const Vec3& Rot, const Vec3& Pos);

	/**
	 * �}�E�X�ʒu�����ɂ��āA���[���h��ԏ�̃��C�����쐬���܂�
	 * �Ԃ�l�͍쐬���ꂽ���C���
	 * 
	 * pDev -> �f�o�C�X
	 * hWnd -> �E�B���h�E�n���h��
	 */
	RAY CalcRayInWorldFromMousePos(LPDIRECT3DDEVICE9 pDev, HWND& hWnd);

	/**
	 * �Ώۂ̃��b�V���ɉ����āA���[���h��ԓ��̃��C�������[�J���C�Y
	 * �Ԃ�l�͕ϊ����ꂽ���C���
	 * 
	 * mtxWolrdFromMesh -> �Ώۂ̃��b�V���̃��[���h�s��
	 * Ray              -> ���C���
	 */
	RAY LocalizeRayForTargetMesh(const D3DXMATRIX& mtxWolrdFromMesh, const RAY& Ray);

	/**
	 * ���b�V���ƃ��C�̓����蔻��
	 * �Ԃ�l�͐ڐG�_�̍��W�A�ڐG��������Ώ��������
	 * 
	 * pMesh        -> �Ώۂ̃��b�V��
	 * RayLocalized -> ���[�J���C�Y���ꂽ���C���
	 */
	D3DXVECTOR3 RaycastForMesh(const LPD3DXBASEMESH& pMesh, const RAY& RayLocalized);

	/**
	 * �ڕW�l�֊p�x�����킹��A��Ԍn���g�p����ۂȂ�
	 * �Ԃ�l����
	 *
	 * fAngle  -> ���݂̊p�x
	 * fDest   -> �ڕW�̊p�x
	 * fRadius -> ������
	 */
	void NormalizeAngleToDest(float& fAngle, float fDest);
	void NormalizeAngleToDest(float& fAngle, float fDest, float fRadius);

	/**
	 * �p�x��͈͓��Ɏ��߂�A360�x�񂷍ۂȂ�
	 * �Ԃ�l����
	 * 
	 * fAngle1�E2 -> ���̒l��360�x���ɕϊ�
	 */
	void WrapAngleToRange(float& fAngle);
	void WrapAngleToRange(float& fAngle1, float& fAngle2);

	/**
	 * JSON�t�@�C���̓W�J
	 * �Ԃ�l��
	 * 
	 * FilePath -> JSON�t�@�C���̃p�X�w��
	 */
	nlohmann::json OpenJsonFile(const std::string& FilePath);

	/**
	 * ��O���o��������JSON�t�@�C���̓W�J
	 * �Ԃ�l��
	 *
	 * FilePath -> JSON�t�@�C���̃p�X�w��
	 */
	nlohmann::json OpenJsonFileMaybeThrow(const std::string& FilePath);

	/**
	 * UTF8��JSON�f�[�^��SJIS�ɕϊ�
	 * �Ԃ�l��SJIS�̕�����
	 *
	 * Json -> �v�f�w�肳�ꂽ�f�[�^
	 */
	std::string JsonConvertToSJIS(const nlohmann::json& Json);

	/**
	 * JSON�f�[�^��D3DXVECTOR2�ɕϊ�
	 * �Ԃ�l��D3DXVECTOR2
	 *
	 * Json -> �v�f�w�肳�ꂽ�f�[�^
	 */
	D3DXVECTOR2	JsonConvertToVec2(const nlohmann::json& Json);

	/**
	 * JSON�f�[�^��D3DXVECTOR3�ɕϊ�
	 * �Ԃ�l��D3DXVECTOR3
	 *
	 * Json -> �v�f�w�肳�ꂽ�f�[�^
	 */
	D3DXVECTOR3	JsonConvertToVec3(const nlohmann::json& Json);

	/**
	 * JSON�f�[�^��D3DXCOLOR�ɕϊ�
	 * �Ԃ�l��D3DXCOLOR
	 *
	 * Json -> �v�f�w�肳�ꂽ�f�[�^
	 */
	D3DXCOLOR JsonConvertToXCol(const nlohmann::json& Json);

	//****************************************************
	// ���O���MIS���`
	//****************************************************
	namespace MIS
	{
		/* ImGui���g�p����ۂ̎��������V���[�g�J�b�g�ł� */

		/**
		 * ImGui��Window�𐶐��A�K��'ImGui::End()'�ƃZ�b�g�Ŏg�p���邱��
		 * �Ԃ�l�̓E�B���h�E�����̉�
		 */
		bool MyImGuiShortcut_BeginWindow(const char* pName);
		bool MyImGuiShortcut_BeginWindow(const char* pName, const ImVec2& Rect);
		bool MyImGuiShortcut_BeginWindow(const char* pName, const ImVec2& Rect, const ImVec2& Pos);
		bool MyImGuiShortcut_BeginWindow(const char* pName, const ImVec2& Rect, const ImVec2& Pos, ImGuiWindowFlags Flags);

		/**
		 * �{�^���̃E�B�W�F�b�g���A�Y���s�̎c��̕��ɋϓ��ɕ~���l�߂邽�߂̃T�C�Y���Z�o���܂�
		 * �K���E�l�߂ɂȂ�܂��A�����̍��ɐ������L�ڂ���ۂ͐�s���ăe�L�X�g��z�u���邱��
		 * �Ԃ�l�̓{�^��1�̃T�C�Y
		 */
		ImVec2 CalcButtonSpacing(unsigned short wNum);
		ImVec2 CalcButtonSpacing(unsigned short wNum, float fHeight);

		/**
		 * Vector�n�\���̂̃h���b�O����
		 * �Ԃ�l����
		 */
		void MyImGuiShortcut_DragVector(const char* pLabel, Vec2& Vec2, float fSpeed);
		void MyImGuiShortcut_DragVector(const char* pLabel, Vec3& Vec3, float fSpeed);

		/**
		 * Vector�n�\���̂̃��Z�b�g����
		 */
		void MyImGuiShortcut_ResetVector(const char* pLabel, Vec2& Vec2);
		void MyImGuiShortcut_ResetVector(const char* pLabel, Vec2& Vec2, float fInitValue);
		void MyImGuiShortcut_ResetVector(const char* pLabel, Vec3& Vec3);
		void MyImGuiShortcut_ResetVector(const char* pLabel, Vec3& Vec3, float fInitValue);
	}
}

// �e���v���[�g�����t�@�C��
#include "useful.impl"