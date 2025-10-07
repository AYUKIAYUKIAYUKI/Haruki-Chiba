//============================================================================
// 
// ���[�X�t�� [useful.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "useful.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//============================================================================
// UV�}�b�s���O������
//============================================================================
void useful::InitializeTextureMappingInVertex(VERTEX_2D*& rVtx)
{
	rVtx[0].tex = { 0.0f, 0.0f };
	rVtx[1].tex = { 1.0f, 0.0f };
	rVtx[2].tex = { 0.0f, 1.0f };
	rVtx[3].tex = { 1.0f, 1.0f };
}

//============================================================================
// UV�}�b�s���O������
//============================================================================
void useful::InitializeTextureMappingInVertex(VERTEX_3D*& rVtx)
{
	rVtx[0].tex = { 0.0f, 0.0f };
	rVtx[1].tex = { 1.0f, 0.0f };
	rVtx[2].tex = { 0.0f, 1.0f };
	rVtx[3].tex = { 1.0f, 1.0f };
}

//============================================================================
// ���[���h�s��v�Z
//============================================================================
void useful::UpdateMtxWorld(Mtx& mtxWorld, const Vec3& Scale, const Vec3& Rot, const Vec3& Pos)
{
	// �v�Z�p�s��
	Mtx mtxScale, mtxRot, mtxTrans;

	// ���[���h�s���������
	D3DXMatrixIdentity(&mtxWorld);

	if (Scale != VEC3_ZERO_INIT)
	{
		// �g�k�s��쐬
		D3DXMatrixScaling(&mtxScale, Scale.x, Scale.y, Scale.z);

		// �g�k�s��Ƃ̊|���Z
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);
	}

	if (Rot != VEC3_ZERO_INIT)
	{
		// ��]�s��쐬
		D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);

		// ��]�s��Ƃ̊|���Z
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
	}

	if (Pos != VEC3_ZERO_INIT)
	{
		// ���s�ړ��s��쐬
		D3DXMatrixTranslation(&mtxTrans, Pos.x, Pos.y, Pos.z);

		// ���s�ړ��s��Ƃ̊|���Z
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
	}
}

//============================================================================
// �}�E�X�ʒu���烏�[���h��ԏ�̃��C���쐬
//============================================================================
RAY useful::CalcRayInWorldFromMousePos(LPDIRECT3DDEVICE9 pDev, HWND& hWnd)
{
	// �K�v�ȕϐ���p��
	RAY
		Ray; // ���C�p���\����
	POINT
		MouseClientPos = {}; // �}�E�X�̃N���C�A���g�̈��̑��΍��W
	Vec3
		ScreenNearClip = VEC3_ZERO_INIT, // �X�N���[�����W�ł̃j�A�N���b�v
		ScreenFarClip = VEC3_ZERO_INIT,  // �X�N���[�����W�ł̃t�@�[�N���b�v
		WorldNearClip = VEC3_ZERO_INIT,  // ���[���h���W�ł̃j�A�N���b�v
		WorldFarClip = VEC3_ZERO_INIT;   // ���[���h���W�ł̃t�@�[�N���b�v
	D3DVIEWPORT9
		Viewport = {}; // �r���[�|�[�g���
	D3DXMATRIX
		mtxProj = {},  // �v���W�F�N�V�����s��
		mtxView = {},  // �r���[�s��
		mtxWorld = {}; // ���[���h�s��

	// �}�E�X�̃N���C�A���g���W�̎擾
	GetCursorPos(&MouseClientPos);
	ScreenToClient(hWnd, &MouseClientPos);

	// �r���[�|�[�g���̎擾
	pDev->GetViewport(&Viewport);

	// ���݂̃v���W�F�N�V�����s����擾
	pDev->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// ���݂̃r���[�s����擾
	pDev->GetTransform(D3DTS_VIEW, &mtxView);

	// ���[���h���W�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �@�}�E�X���W����Z�l�̂�0��1�ɌŒ肵�A�X�N���[����̃j�A / �t�@�[�N���b�v���쐬����
	ScreenNearClip = { static_cast<float>(MouseClientPos.x), static_cast<float>(MouseClientPos.y), 0.0f };
	ScreenFarClip  = { static_cast<float>(MouseClientPos.x), static_cast<float>(MouseClientPos.y), 1.0f };

	// �A�X�N���[����Ԃ̍��W���A���v���W�F�N�V�������A���[���h��ԏ�ł̍��W�ɕϊ�����
	D3DXVec3Unproject(&WorldNearClip, &ScreenNearClip, &Viewport, &mtxProj, &mtxView, &mtxWorld);
	D3DXVec3Unproject(&WorldFarClip, &ScreenFarClip, &Viewport, &mtxProj, &mtxView, &mtxWorld);

	// �B���C�������Z�b�g
	Ray.Origin    = WorldNearClip;                     // �}�E�X���W�����[���h��ԏ�ɕϊ��������̃j�A�N���b�v���n�_�ƂȂ�
	Ray.Direction = WorldFarClip - WorldNearClip;      // �j�A�N���b�v����t�@�[�N���b�v�ւ̕����x�N�g�����Z�o
	D3DXVec3Normalize(&Ray.Direction, &Ray.Direction); // �����x�N�g���𐳋K�����P�ʃx�N�g���ɕϊ�

	// �v�Z���ʂ�Ԃ�
	return Ray;
}

//============================================================================
// �Ώۂ̃��b�V���ɉ����ă��[���h��ԓ��̃��C�����[�J���C�Y
//============================================================================
RAY useful::LocalizeRayForTargetMesh(const D3DXMATRIX& mtxWorldFromMesh, const RAY& Ray)
{
	// �K�v�ȕϐ���p��
	RAY
		RayLocalized; // ���[�J���C�Y���ꂽ���C���
	D3DXMATRIX
		mtxInverseWorldFromMesh; // ���b�V�����瓾���t�s��

	// ���b�V���̃��[���h�s�񂩂�t�s������߂�
	D3DXMatrixInverse(&mtxInverseWorldFromMesh, nullptr, &mtxWorldFromMesh);

	// ���C�̈ʒu�x�N�g�� (�n�_) ���A�Ώۃ��b�V���̃��[�J�����W�n�ɕϊ�
	D3DXVec3TransformCoord(&RayLocalized.Origin, &Ray.Origin, &mtxInverseWorldFromMesh);

	// ���C�̕����x�N�g�����A��]�ƃX�P�[���̂ݔ��f�����ă��[�J�����W�n�ɕϊ�
	D3DXVec3TransformNormal(&RayLocalized.Direction, &Ray.Direction, &mtxInverseWorldFromMesh);

	// �����x�N�g���𐳋K�����P�ʃx�N�g���ɕϊ�
	D3DXVec3Normalize(&RayLocalized.Direction, &RayLocalized.Direction);

	// ���[�J���C�Y��̃��C����Ԃ�
	return RayLocalized;
}

//============================================================================
// ���b�V���ƃ��C�̓����蔻��
//============================================================================
D3DXVECTOR3 useful::RaycastForMesh(const LPD3DXBASEMESH& pMesh, const RAY& RayLocalized)
{
	// �K�v�ȕϐ���p��
	Vec3
		HitPos = VEC3_ZERO_INIT; // �Փ˓_�̍��W
	BOOL
		bHit = false; // �Փ˔���
	DWORD
		dwFaceIdx = 0; // �ʂ̃C���f�b�N�X
	float
		fU    = 0.0f,
		fV    = 0.0f,
		fDist = 0.0f; // �n�_�����_�܂ł̋���

	// ���C�����b�V���ƌ������邩�𔻒�
	D3DXIntersect(
		pMesh,                   // ���b�V�����
		&RayLocalized.Origin,    // ���C�̎n�_
		&RayLocalized.Direction, // ���C�̕���
		&bHit,                   // �Փ˔���̊i�[��
		&dwFaceIdx,              // �Փ˂����ʂ̃C���f�b�N�X�̊i�[��
		&fU,
		&fV,
		&fDist,                  // �n�_�����_�܂ł̋����̊i�[��
		nullptr,                 // �����̃q�b�g���̊i�[��
		nullptr);                // �����̃q�b�g���̊i�[��

	// �Փ˂����o�ł�����
	if (bHit)
	{
		// �Փ˓_�����[���h��ԓ��̈ʒu�ɕϊ�
		HitPos = RayLocalized.Origin + (RayLocalized.Direction * fDist);
	}

	// �Փ˓_�̍��W��Ԃ�
	return HitPos;
}

//============================================================================
// �ڕW�l�֊p�x�������� - �@
//============================================================================
void useful::NormalizeAngleToDest(float& fAngle, float fDest)
{
	// �ڕW�p�x�ƌ��݊p�x�Ƃ̍����v�Z
	const float& fDifference = fAngle - fDest;

	if (fDifference > D3DX_PI)
	{
		fAngle += -D3DX_PI * 2.0f;
	}
	else if (fDifference < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	}
}

//============================================================================
// �ڕW�l�֊p�x�������� - �A
//============================================================================
void useful::NormalizeAngleToDest(float& fAngle, float fDest, float fRadius)
{
	// �ڕW�p�x�ƌ��݊p�x�Ƃ̍����v�Z
	const float& fDifference = fAngle - fDest;

	if (fDifference > fRadius)
	{
		fAngle += -fRadius * 2.0f;
	}
	else if (fDifference < -fRadius)
	{
		fAngle += fRadius * 2.0f;
	}
}

//============================================================================
// �p�x��͈͓��Ɏ��߂� - �@
//============================================================================
void useful ::WrapAngleToRange(float& fAngle)
{
	if (fAngle > D3DX_PI)
	{
		fAngle += -D3DX_PI * 2.0f;
	}
	else if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	}
}

//============================================================================
// �p�x��͈͓��Ɏ��߂� - �A
//============================================================================
void useful::WrapAngleToRange(float& fAngle1, float& fAngle2)
{
	if (fAngle1 > D3DX_PI)
	{
		fAngle1 += -D3DX_PI * 2.0f;
		fAngle2 += -D3DX_PI * 2.0f;
	}
	else if (fAngle1 < -D3DX_PI)
	{
		fAngle1 += D3DX_PI * 2.0f;
		fAngle2 += D3DX_PI * 2.0f;
	}
}

//============================================================================
// JSON�t�@�C���̓W�J
//============================================================================
nlohmann::json useful::OpenJsonFile(const std::string& FilePath)
{
	// JSON�t�@�C����ǂݎ��W�J
	std::ifstream ifs(FilePath);

	// �t�@�C���W�J���s
	if (!ifs.good())
	{
		return nullptr;
	}

	// JSON�f�[�^���p�[�X
	nlohmann::json JSON;
	ifs >> JSON;

	// JSON�I�u�W�F�N�g�������n��
	return JSON;
}

//============================================================================
// JSON�t�@�C���̓W�J - ��O���o
//============================================================================
nlohmann::json useful::OpenJsonFileMaybeThrow(const std::string& FilePath)
{
	// JSON�t�@�C����ǂݎ��W�J
	std::ifstream ifs(FilePath);

	// �t�@�C���W�J���s
	if (!ifs.good())
	{
		throw std::ios_base::failure("JSON Open Failed:" + FilePath);
	}

	// JSON�f�[�^���p�[�X
	nlohmann::json JSON;
	ifs >> JSON;

	// JSON�I�u�W�F�N�g�������n��
	return JSON;
}

//============================================================================
// UTF8��JSON�f�[�^��SJIS�ɕϊ�
//============================================================================
std::string useful::JsonConvertToSJIS(const nlohmann::json& Json)
{
	// ������̒������i�[
	int nLength = 0;

	// UTF8�̕������string�^�ɃR�s�[
	const std::string& Str = Json;

	// UTF8�̕�����̒������擾
	nLength = MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, nullptr, 0);

	// �G���[����
	if (nLength == 0)
	{
		throw std::runtime_error("UTF8 Convert To UTF16 Failed");
	}

	// UTF16�ϊ���̕�������i�[
	std::wstring WideStr(nLength, L'\0');

	// UTF8��UTF16�ɕϊ�
	MultiByteToWideChar(
		CP_UTF8,     // �ϊ��R�[�h�y�[�W
		0,           // �ϊ��̎��
		Str.c_str(), // �ϊ��������������
		-1,          // ������̒��� (-1�őS�̎w��)
		&WideStr[0], // �ϊ���̕�������󂯎��|�C���^
		nLength);    // ������̃o�b�t�@�T�C�Y

	// UTF16�̕�����̒������擾
	nLength = WideCharToMultiByte(CP_ACP, 0, WideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);

	// �G���[����
	if (nLength == 0)
	{
		throw std::runtime_error("UTF16 Convert To SJIS Failed");
	}

	// SJIS�ϊ���̕�������i�[
	std::string SjisStr(nLength, '\0');

	// UTF16��SJIS�ɕϊ�
	WideCharToMultiByte(
		CP_ACP,          // �ϊ��R�[�h�y�[�W
		0,               // �ϊ��̎��
		WideStr.c_str(), // �ϊ����������C�h������
		-1,	             // ������̒��� (-1�őS�̎w��)
		&SjisStr[0],     // �ϊ���̕�������󂯎��|�C���^
		nLength,         // ������̃o�b�t�@�T�C�Y
		nullptr,         // �ϊ��ł��Ȃ������̑���ɏo������
		nullptr);        // ����̕����̎g�p������󂯎��|�C���^

	return SjisStr;
}

//============================================================================
// JSON�f�[�^��D3DXVECTOR2�ɕϊ�
//============================================================================
D3DXVECTOR2 useful::JsonConvertToVec2(const nlohmann::json& Json)
{
	return D3DXVECTOR2(static_cast<float>(Json[0]), static_cast<float>(Json[1]));
}

//============================================================================
// JSON�f�[�^��D3DXVECTOR3�ɕϊ�
//============================================================================
D3DXVECTOR3 useful::JsonConvertToVec3(const nlohmann::json& Json)
{
	return D3DXVECTOR3(static_cast<float>(Json[0]), static_cast<float>(Json[1]), static_cast<float>(Json[2]));
}

//============================================================================
// JSON�f�[�^��D3DXCOLOR�ɕϊ�
//============================================================================
D3DXCOLOR useful::JsonConvertToXCol(const nlohmann::json& Json)
{
	return D3DXCOLOR(static_cast<float>(Json[0]), static_cast<float>(Json[1]), static_cast<float>(Json[2]), static_cast<float>(Json[3]));
}

//============================================================================
// ImGui�̃E�B���h�E���� - �@
//============================================================================
bool useful::MIS::MyImGuiShortcut_BeginWindow(const char* pName)
{
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

	return ImGui::Begin(pName);
}

//============================================================================
// ImGui�̃E�B���h�E���� - �A
//============================================================================
bool useful::MIS::MyImGuiShortcut_BeginWindow(const char* pName, const ImVec2& Rect)
{
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

	return ImGui::Begin(pName);
}

//============================================================================
// ImGui�̃E�B���h�E���� - �B
//============================================================================
bool useful::MIS::MyImGuiShortcut_BeginWindow(const char* pName, const ImVec2& Rect, const ImVec2& Pos)
{
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos(Pos);

	return ImGui::Begin(pName);
}

//============================================================================
// ImGui�̃E�B���h�E���� - �C
//============================================================================
bool useful::MIS::MyImGuiShortcut_BeginWindow(const char* pName, const ImVec2& Rect, const ImVec2& Pos, ImGuiWindowFlags Flags)
{
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos(Pos);

	return ImGui::Begin(pName, nullptr, Flags);
}

//============================================================================
// �{�^�������z�u - �@
//============================================================================
ImVec2 useful::MIS::CalcButtonSpacing(unsigned short wNum)
{
	// �E�B���h�E�ƃE�B�W�F�b�g�̕����R�s�[
	float
		fWindowWidth = ImGui::GetContentRegionAvail().x,
		fSeparate = ImGui::GetStyle().ItemSpacing.x,
		fNumButtons = static_cast<float>(1.0f * wNum);

	// 1������̃{�^���̃T�C�Y
	ImVec2 Rect =
	{
		(fWindowWidth + -fSeparate * (fNumButtons - 1.0f)) / fNumButtons,
		25.0f
	};

	return Rect;
}

//============================================================================
// �{�^�������z�u - �A
//============================================================================
ImVec2 useful::MIS::CalcButtonSpacing(unsigned short wNum, float fHeight)
{
	// �E�B���h�E�ƃE�B�W�F�b�g�̕����R�s�[
	float
		fWindowWidth = ImGui::GetContentRegionAvail().x,
		fSeparate = ImGui::GetStyle().ItemSpacing.x,
		fNumButtons = static_cast<float>(1.0f * wNum);

	// 1������̃{�^���̃T�C�Y
	ImVec2 Rect =
	{
		(fWindowWidth + -fSeparate * fNumButtons) / (fNumButtons + 1.0f),
		fHeight
	};

	return Rect;
}

//============================================================================
// Vector�n�\���̂̃h���b�O���� - �@
//============================================================================
void useful::MIS::MyImGuiShortcut_DragVector(const char* pLabel, Vec2& Vec2, float fSpeed)
{
	std::string sLabel = static_cast<std::string>("##") + pLabel;

	ImGui::Text(pLabel);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-1.0f);
	ImGui::DragFloat2(sLabel.c_str(), Vec2, fSpeed);
}

//============================================================================
// Vector�n�\���̂̃h���b�O���� - �A
//============================================================================
void useful::MIS::MyImGuiShortcut_DragVector(const char* pLabel, Vec3& Vec3, float fSpeed)
{
	std::string sLabel = static_cast<std::string>("##") + pLabel;

	ImGui::Text(pLabel);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-1.0f);
	ImGui::DragFloat3(sLabel.c_str(), Vec3, fSpeed);
}

//============================================================================
// Vector�n�\���̂̃��Z�b�g���� - �@
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* pLabel, Vec2& Vec2)
{
	std::string
		sX = static_cast<std::string>("X##") + pLabel,
		sY = static_cast<std::string>("Y##") + pLabel,
		sAll = static_cast<std::string>("All##") + pLabel;

	// ��s���ăe�L�X�g��`��A�c��̌��ԂɎ����Ń{�^����~���l�߂�
	ImGui::Text(reinterpret_cast<const char*>(u8"���Z�b�g :"));
	ImGui::SameLine();

	// 1������̃{�^���̃T�C�Y���擾
	const ImVec2& Rect = CalcButtonSpacing(3);

	if (ImGui::Button(sX.c_str(), Rect))
	{
		Vec2.x = 0.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button(sY.c_str(), Rect))
	{
		Vec2.y = 0.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button(sAll.c_str(), Rect))
	{
		Vec2 = { 0.0f, 0.0f };
	}
}

//============================================================================
// Vector�n�\���̂̃��Z�b�g���� - �A
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* pLabel, Vec2& Vec2, float fInitValue)
{
	std::string
		sX = static_cast<std::string>("X##") + pLabel,
		sY = static_cast<std::string>("Y##") + pLabel,
		sAll = static_cast<std::string>("All##") + pLabel;

	// ��s���ăe�L�X�g��`��A�c��̌��ԂɎ����Ń{�^����~���l�߂�
	ImGui::Text(reinterpret_cast<const char*>(u8"���Z�b�g :"));
	ImGui::SameLine();

	// 1������̃{�^���̃T�C�Y���擾
	const ImVec2& Rect = CalcButtonSpacing(3);

	if (ImGui::Button(sX.c_str(), Rect))
	{
		Vec2.x = fInitValue;
	}
	ImGui::SameLine();
	if (ImGui::Button(sY.c_str(), Rect))
	{
		Vec2.y = fInitValue;
	}
	ImGui::SameLine();
	if (ImGui::Button(sAll.c_str(), Rect))
	{
		Vec2 = { fInitValue, fInitValue };
	}
}

//============================================================================
// Vector�n�\���̂̃��Z�b�g���� - �B
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* pLabel, Vec3& Vec3)
{
	std::string
		sX = static_cast<std::string>("X##") + pLabel,
		sY = static_cast<std::string>("Y##") + pLabel,
		sZ = static_cast<std::string>("Z##") + pLabel,
		sAll = static_cast<std::string>("All##") + pLabel;

	// ��s���ăe�L�X�g��`��A�c��̌��ԂɎ����Ń{�^����~���l�߂�
	ImGui::Text(reinterpret_cast<const char*>(u8"���Z�b�g :"));
	ImGui::SameLine();

	// 1������̃{�^���̃T�C�Y���擾
	const ImVec2& Rect = CalcButtonSpacing(4);

	if (ImGui::Button(sX.c_str(), Rect))
	{
		Vec3.x = 0.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button(sY.c_str(), Rect))
	{
		Vec3.y = 0.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button(sZ.c_str(), Rect))
	{
		Vec3.z = 0.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button(sAll.c_str(), Rect))
	{
		Vec3 = { 0.0f, 0.0f, 0.0f };
	}
}

//============================================================================
// Vector�n�\���̂̃��Z�b�g���� - �C
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* pLabel, Vec3& Vec3, float fInitValue)
{
	std::string
		sX = static_cast<std::string>("X##") + pLabel,
		sY = static_cast<std::string>("Y##") + pLabel,
		sZ = static_cast<std::string>("Z##") + pLabel,
		sAll = static_cast<std::string>("All##") + pLabel;

	// ��s���ăe�L�X�g��`��A�c��̌��ԂɎ����Ń{�^����~���l�߂�
	ImGui::Text(reinterpret_cast<const char*>(u8"���Z�b�g :"));
	ImGui::SameLine();

	// 1������̃{�^���̃T�C�Y���擾
	const ImVec2& Rect = CalcButtonSpacing(4);

	if (ImGui::Button(sX.c_str(), Rect))
	{
		Vec3.x = fInitValue;
	}
	ImGui::SameLine();
	if (ImGui::Button(sY.c_str(), Rect))
	{
		Vec3.y = fInitValue;
	}
	ImGui::SameLine();
	if (ImGui::Button(sZ.c_str(), Rect))
	{
		Vec3.z = fInitValue;
	}
	ImGui::SameLine();
	if (ImGui::Button(sAll.c_str(), Rect))
	{
		Vec3 = { fInitValue, fInitValue, fInitValue };
	}
}