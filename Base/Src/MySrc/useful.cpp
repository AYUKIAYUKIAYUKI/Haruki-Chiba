//============================================================================
// 
// ユースフル [useful.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "useful.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//============================================================================
// UVマッピング初期化
//============================================================================
void useful::InitializeTextureMappingInVertex(VERTEX_2D*& rVtx)
{
	rVtx[0].tex = { 0.0f, 0.0f };
	rVtx[1].tex = { 1.0f, 0.0f };
	rVtx[2].tex = { 0.0f, 1.0f };
	rVtx[3].tex = { 1.0f, 1.0f };
}

//============================================================================
// UVマッピング初期化
//============================================================================
void useful::InitializeTextureMappingInVertex(VERTEX_3D*& rVtx)
{
	rVtx[0].tex = { 0.0f, 0.0f };
	rVtx[1].tex = { 1.0f, 0.0f };
	rVtx[2].tex = { 0.0f, 1.0f };
	rVtx[3].tex = { 1.0f, 1.0f };
}

//============================================================================
// ワールド行列計算
//============================================================================
void useful::UpdateMtxWorld(Mtx& mtxWorld, const Vec3& Scale, const Vec3& Rot, const Vec3& Pos)
{
	// 計算用行列
	Mtx mtxScale, mtxRot, mtxTrans;

	// ワールド行列を初期化
	D3DXMatrixIdentity(&mtxWorld);

	if (Scale != VEC3_ZERO_INIT)
	{
		// 拡縮行列作成
		D3DXMatrixScaling(&mtxScale, Scale.x, Scale.y, Scale.z);

		// 拡縮行列との掛け算
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);
	}

	if (Rot != VEC3_ZERO_INIT)
	{
		// 回転行列作成
		D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);

		// 回転行列との掛け算
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
	}

	if (Pos != VEC3_ZERO_INIT)
	{
		// 平行移動行列作成
		D3DXMatrixTranslation(&mtxTrans, Pos.x, Pos.y, Pos.z);

		// 平行移動行列との掛け算
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
	}
}

//============================================================================
// マウス位置からワールド空間上のレイを作成
//============================================================================
RAY useful::CalcRayInWorldFromMousePos(LPDIRECT3DDEVICE9 pDev, HWND& hWnd)
{
	// 必要な変数を用意
	RAY
		Ray; // レイ用情報構造体
	POINT
		MouseClientPos = {}; // マウスのクライアント領域上の相対座標
	Vec3
		ScreenNearClip = VEC3_ZERO_INIT, // スクリーン座標でのニアクリップ
		ScreenFarClip = VEC3_ZERO_INIT,  // スクリーン座標でのファークリップ
		WorldNearClip = VEC3_ZERO_INIT,  // ワールド座標でのニアクリップ
		WorldFarClip = VEC3_ZERO_INIT;   // ワールド座標でのファークリップ
	D3DVIEWPORT9
		Viewport = {}; // ビューポート情報
	D3DXMATRIX
		mtxProj = {},  // プロジェクション行列
		mtxView = {},  // ビュー行列
		mtxWorld = {}; // ワールド行列

	// マウスのクライアント座標の取得
	GetCursorPos(&MouseClientPos);
	ScreenToClient(hWnd, &MouseClientPos);

	// ビューポート情報の取得
	pDev->GetViewport(&Viewport);

	// 現在のプロジェクション行列を取得
	pDev->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// 現在のビュー行列を取得
	pDev->GetTransform(D3DTS_VIEW, &mtxView);

	// ワールド座標の初期化
	D3DXMatrixIdentity(&mtxWorld);

	// ①マウス座標からZ値のみ0と1に固定し、スクリーン上のニア / ファークリップを作成する
	ScreenNearClip = { static_cast<float>(MouseClientPos.x), static_cast<float>(MouseClientPos.y), 0.0f };
	ScreenFarClip  = { static_cast<float>(MouseClientPos.x), static_cast<float>(MouseClientPos.y), 1.0f };

	// ②スクリーン空間の座標をアンプロジェクションし、ワールド空間上での座標に変換する
	D3DXVec3Unproject(&WorldNearClip, &ScreenNearClip, &Viewport, &mtxProj, &mtxView, &mtxWorld);
	D3DXVec3Unproject(&WorldFarClip, &ScreenFarClip, &Viewport, &mtxProj, &mtxView, &mtxWorld);

	// ③レイを情報をセット
	Ray.Origin    = WorldNearClip;                     // マウス座標をワールド空間上に変換した時のニアクリップが始点となる
	Ray.Direction = WorldFarClip - WorldNearClip;      // ニアクリップからファークリップへの方向ベクトルを算出
	D3DXVec3Normalize(&Ray.Direction, &Ray.Direction); // 方向ベクトルを正規化し単位ベクトルに変換

	// 計算結果を返す
	return Ray;
}

//============================================================================
// 対象のメッシュに応じてワールド空間内のレイをローカライズ
//============================================================================
RAY useful::LocalizeRayForTargetMesh(const D3DXMATRIX& mtxWorldFromMesh, const RAY& Ray)
{
	// 必要な変数を用意
	RAY
		RayLocalized; // ローカライズされたレイ情報
	D3DXMATRIX
		mtxInverseWorldFromMesh; // メッシュから得た逆行列

	// メッシュのワールド行列から逆行列を求める
	D3DXMatrixInverse(&mtxInverseWorldFromMesh, nullptr, &mtxWorldFromMesh);

	// レイの位置ベクトル (始点) を、対象メッシュのローカル座標系に変換
	D3DXVec3TransformCoord(&RayLocalized.Origin, &Ray.Origin, &mtxInverseWorldFromMesh);

	// レイの方向ベクトルを、回転とスケールのみ反映させてローカル座標系に変換
	D3DXVec3TransformNormal(&RayLocalized.Direction, &Ray.Direction, &mtxInverseWorldFromMesh);

	// 方向ベクトルを正規化し単位ベクトルに変換
	D3DXVec3Normalize(&RayLocalized.Direction, &RayLocalized.Direction);

	// ローカライズ後のレイ情報を返す
	return RayLocalized;
}

//============================================================================
// メッシュとレイの当たり判定
//============================================================================
D3DXVECTOR3 useful::RaycastForMesh(const LPD3DXBASEMESH& pMesh, const RAY& RayLocalized)
{
	// 必要な変数を用意
	Vec3
		HitPos = VEC3_ZERO_INIT; // 衝突点の座標
	BOOL
		bHit = false; // 衝突判定
	DWORD
		dwFaceIdx = 0; // 面のインデックス
	float
		fU    = 0.0f,
		fV    = 0.0f,
		fDist = 0.0f; // 始点から交点までの距離

	// レイがメッシュと交差するかを判定
	D3DXIntersect(
		pMesh,                   // メッシュ情報
		&RayLocalized.Origin,    // レイの始点
		&RayLocalized.Direction, // レイの方向
		&bHit,                   // 衝突判定の格納先
		&dwFaceIdx,              // 衝突した面のインデックスの格納先
		&fU,
		&fV,
		&fDist,                  // 始点から交点までの距離の格納先
		nullptr,                 // 複数のヒット情報の格納先
		nullptr);                // 複数のヒット数の格納先

	// 衝突を検出できたら
	if (bHit)
	{
		// 衝突点をワールド空間内の位置に変換
		HitPos = RayLocalized.Origin + (RayLocalized.Direction * fDist);
	}

	// 衝突点の座標を返す
	return HitPos;
}

//============================================================================
// 目標値へ角度を向ける - ①
//============================================================================
void useful::NormalizeAngleToDest(float& fAngle, float fDest)
{
	// 目標角度と現在角度との差を計算
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
// 目標値へ角度を向ける - ②
//============================================================================
void useful::NormalizeAngleToDest(float& fAngle, float fDest, float fRadius)
{
	// 目標角度と現在角度との差を計算
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
// 角度を範囲内に収める - ①
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
// 角度を範囲内に収める - ②
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
// JSONファイルの展開
//============================================================================
nlohmann::json useful::OpenJsonFile(const std::string& FilePath)
{
	// JSONファイルを読み取り展開
	std::ifstream ifs(FilePath);

	// ファイル展開失敗
	if (!ifs.good())
	{
		return nullptr;
	}

	// JSONデータをパース
	nlohmann::json JSON;
	ifs >> JSON;

	// JSONオブジェクトを引き渡す
	return JSON;
}

//============================================================================
// JSONファイルの展開 - 例外送出
//============================================================================
nlohmann::json useful::OpenJsonFileMaybeThrow(const std::string& FilePath)
{
	// JSONファイルを読み取り展開
	std::ifstream ifs(FilePath);

	// ファイル展開失敗
	if (!ifs.good())
	{
		throw std::ios_base::failure("JSON Open Failed:" + FilePath);
	}

	// JSONデータをパース
	nlohmann::json JSON;
	ifs >> JSON;

	// JSONオブジェクトを引き渡す
	return JSON;
}

//============================================================================
// UTF8のJSONデータをSJISに変換
//============================================================================
std::string useful::JsonConvertToSJIS(const nlohmann::json& Json)
{
	// 文字列の長さを格納
	int nLength = 0;

	// UTF8の文字列をstring型にコピー
	const std::string& Str = Json;

	// UTF8の文字列の長さを取得
	nLength = MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, nullptr, 0);

	// エラー発生
	if (nLength == 0)
	{
		throw std::runtime_error("UTF8 Convert To UTF16 Failed");
	}

	// UTF16変換後の文字列を格納
	std::wstring WideStr(nLength, L'\0');

	// UTF8をUTF16に変換
	MultiByteToWideChar(
		CP_UTF8,     // 変換コードページ
		0,           // 変換の種類
		Str.c_str(), // 変換したい文字列の
		-1,          // 文字列の長さ (-1で全体指定)
		&WideStr[0], // 変換後の文字列を受け取るポインタ
		nLength);    // 文字列のバッファサイズ

	// UTF16の文字列の長さを取得
	nLength = WideCharToMultiByte(CP_ACP, 0, WideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);

	// エラー発生
	if (nLength == 0)
	{
		throw std::runtime_error("UTF16 Convert To SJIS Failed");
	}

	// SJIS変換後の文字列を格納
	std::string SjisStr(nLength, '\0');

	// UTF16をSJISに変換
	WideCharToMultiByte(
		CP_ACP,          // 変換コードページ
		0,               // 変換の種類
		WideStr.c_str(), // 変換したいワイド文字列
		-1,	             // 文字列の長さ (-1で全体指定)
		&SjisStr[0],     // 変換後の文字列を受け取るポインタ
		nLength,         // 文字列のバッファサイズ
		nullptr,         // 変換できない文字の代わりに出す文字
		nullptr);        // 代わりの文字の使用判定を受け取るポインタ

	return SjisStr;
}

//============================================================================
// JSONデータをD3DXVECTOR2に変換
//============================================================================
D3DXVECTOR2 useful::JsonConvertToVec2(const nlohmann::json& Json)
{
	return D3DXVECTOR2(static_cast<float>(Json[0]), static_cast<float>(Json[1]));
}

//============================================================================
// JSONデータをD3DXVECTOR3に変換
//============================================================================
D3DXVECTOR3 useful::JsonConvertToVec3(const nlohmann::json& Json)
{
	return D3DXVECTOR3(static_cast<float>(Json[0]), static_cast<float>(Json[1]), static_cast<float>(Json[2]));
}

//============================================================================
// JSONデータをD3DXCOLORに変換
//============================================================================
D3DXCOLOR useful::JsonConvertToXCol(const nlohmann::json& Json)
{
	return D3DXCOLOR(static_cast<float>(Json[0]), static_cast<float>(Json[1]), static_cast<float>(Json[2]), static_cast<float>(Json[3]));
}

//============================================================================
// ImGuiのウィンドウ生成 - ①
//============================================================================
bool useful::MIS::MyImGuiShortcut_BeginWindow(const char* pName)
{
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

	return ImGui::Begin(pName);
}

//============================================================================
// ImGuiのウィンドウ生成 - ②
//============================================================================
bool useful::MIS::MyImGuiShortcut_BeginWindow(const char* pName, const ImVec2& Rect)
{
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

	return ImGui::Begin(pName);
}

//============================================================================
// ImGuiのウィンドウ生成 - ③
//============================================================================
bool useful::MIS::MyImGuiShortcut_BeginWindow(const char* pName, const ImVec2& Rect, const ImVec2& Pos)
{
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos(Pos);

	return ImGui::Begin(pName);
}

//============================================================================
// ImGuiのウィンドウ生成 - ④
//============================================================================
bool useful::MIS::MyImGuiShortcut_BeginWindow(const char* pName, const ImVec2& Rect, const ImVec2& Pos, ImGuiWindowFlags Flags)
{
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos(Pos);

	return ImGui::Begin(pName, nullptr, Flags);
}

//============================================================================
// ボタン自動配置 - ①
//============================================================================
ImVec2 useful::MIS::CalcButtonSpacing(unsigned short wNum)
{
	// ウィンドウとウィジェットの幅をコピー
	float
		fWindowWidth = ImGui::GetContentRegionAvail().x,
		fSeparate = ImGui::GetStyle().ItemSpacing.x,
		fNumButtons = static_cast<float>(1.0f * wNum);

	// 1つあたりのボタンのサイズ
	ImVec2 Rect =
	{
		(fWindowWidth + -fSeparate * (fNumButtons - 1.0f)) / fNumButtons,
		25.0f
	};

	return Rect;
}

//============================================================================
// ボタン自動配置 - ②
//============================================================================
ImVec2 useful::MIS::CalcButtonSpacing(unsigned short wNum, float fHeight)
{
	// ウィンドウとウィジェットの幅をコピー
	float
		fWindowWidth = ImGui::GetContentRegionAvail().x,
		fSeparate = ImGui::GetStyle().ItemSpacing.x,
		fNumButtons = static_cast<float>(1.0f * wNum);

	// 1つあたりのボタンのサイズ
	ImVec2 Rect =
	{
		(fWindowWidth + -fSeparate * fNumButtons) / (fNumButtons + 1.0f),
		fHeight
	};

	return Rect;
}

//============================================================================
// Vector系構造体のドラッグ操作 - ①
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
// Vector系構造体のドラッグ操作 - ②
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
// Vector系構造体のリセット操作 - ①
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* pLabel, Vec2& Vec2)
{
	std::string
		sX = static_cast<std::string>("X##") + pLabel,
		sY = static_cast<std::string>("Y##") + pLabel,
		sAll = static_cast<std::string>("All##") + pLabel;

	// 先行してテキストを描画、残りの隙間に自動でボタンを敷き詰める
	ImGui::Text(reinterpret_cast<const char*>(u8"リセット :"));
	ImGui::SameLine();

	// 1つあたりのボタンのサイズを取得
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
// Vector系構造体のリセット操作 - ②
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* pLabel, Vec2& Vec2, float fInitValue)
{
	std::string
		sX = static_cast<std::string>("X##") + pLabel,
		sY = static_cast<std::string>("Y##") + pLabel,
		sAll = static_cast<std::string>("All##") + pLabel;

	// 先行してテキストを描画、残りの隙間に自動でボタンを敷き詰める
	ImGui::Text(reinterpret_cast<const char*>(u8"リセット :"));
	ImGui::SameLine();

	// 1つあたりのボタンのサイズを取得
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
// Vector系構造体のリセット操作 - ③
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* pLabel, Vec3& Vec3)
{
	std::string
		sX = static_cast<std::string>("X##") + pLabel,
		sY = static_cast<std::string>("Y##") + pLabel,
		sZ = static_cast<std::string>("Z##") + pLabel,
		sAll = static_cast<std::string>("All##") + pLabel;

	// 先行してテキストを描画、残りの隙間に自動でボタンを敷き詰める
	ImGui::Text(reinterpret_cast<const char*>(u8"リセット :"));
	ImGui::SameLine();

	// 1つあたりのボタンのサイズを取得
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
// Vector系構造体のリセット操作 - ④
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* pLabel, Vec3& Vec3, float fInitValue)
{
	std::string
		sX = static_cast<std::string>("X##") + pLabel,
		sY = static_cast<std::string>("Y##") + pLabel,
		sZ = static_cast<std::string>("Z##") + pLabel,
		sAll = static_cast<std::string>("All##") + pLabel;

	// 先行してテキストを描画、残りの隙間に自動でボタンを敷き詰める
	ImGui::Text(reinterpret_cast<const char*>(u8"リセット :"));
	ImGui::SameLine();

	// 1つあたりのボタンのサイズを取得
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