//============================================================================
// 
// シーンマネージャー、ヘッダーファイル [scene.manager.h]
// Author : 福田歩希
// 
//============================================================================

#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "singleton.h"

//****************************************************
// 前方宣言
//****************************************************
class CScene;

//****************************************************
// コンセプトの定義
//****************************************************
template <typename T>
concept DerivedFromScene = std::derived_from<T, CScene>
&& requires(T t)
{
	{ t.Initialize() } -> std::same_as<bool>;
}
&& requires(T t)
{
	{ t.Finalize() } -> std::same_as<void>;
};

//****************************************************
// シーンマネージャークラスの定義
//****************************************************
class CSceneManager final : public CSingleton<CSceneManager>
{
	//****************************************************
	// フレンド宣言
	//****************************************************
	friend struct std::default_delete<CSceneManager>;
	friend CSceneManager& CSingleton<CSceneManager>::RefInstance();

public:

	//****************************************************
	// function
	//****************************************************
	
	// 初期化処理
	template <DerivedFromScene T>
	bool Initialize();

	// シーン変更
	template <DerivedFromScene T>
	void ChangeScene(useful::up<T>&& upScene);

	// シーン取得
	inline const useful::up<CScene>& GetScene() { return m_upScene; };

private:

	//****************************************************
	// special function
	//****************************************************
	CSceneManager();           // デフォルトコンストラクタ
	~CSceneManager() override; // デストラクタ

	//****************************************************
	// function
	//****************************************************
	
	// 初期化処理
	bool Initialize() { return true; }

	// 終了処理
	void Finalize() {}

	//****************************************************
	// data
	//****************************************************
	useful::up<CScene> m_upScene; // 現在のシーン
};

// テンプレート実装ファイル
#include "scene.manager.impl"