//============================================================================
// 
// カメラネージャー、ヘッダファイル [camera_manager.h]
// Author : 福田歩希
// 
//============================================================================

// インクルードガード
#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "singleton.h"
#include "registry.h"
#include "camera.h"

//****************************************************
// カメラマネージャークラスの定義
//****************************************************
class CCamera_Manager final : public CSingleton<CCamera_Manager>
{
	//****************************************************
	// フレンド宣言
	//****************************************************
	friend struct std::default_delete<CCamera_Manager>;
	friend CCamera_Manager& CSingleton<CCamera_Manager>::RefInstance();

	//****************************************************
	// 型エイリアスを定義
	//****************************************************
	using upCamera = std::unique_ptr<CCamera>;

	//****************************************************
	// 静的メンバ変数の定義
	//****************************************************
	static constexpr const char* INITIALIZE_PATH = "Data\\JSON\\Camera.List.json";

	////****************************************************
	//// トランジション情報構造体の定義
	////****************************************************
	//struct TRANSITION
	//{
	//	/**
	//	 * 処理の実行です、オーバーライド用のため例外をスローします
	//	 *
	//	 * 実行が完了した場合 true を返します、状態を切り替えてください
	//	 */
	//	virtual bool Execute(CCamera_Manager*& cameraManager);

	//	/**
	//	 * 状態を切り替えます、オーバーライド用のため例外をスローします
	//	 */
	//	virtual void Change(TRANSITION*& r);

	//	/**
	//	 * 現在のトランジションの実行及び状態確認を同時に行います
	//	 */
	//	static void UpdateTransition(CCamera_Manager*& cameraManager);

	//	std::string m_NextKey;	// 遷移予定のカメラのキー
	//};

	////****************************************************
	//// イントロ構造体の定義
	////****************************************************
	//struct INTRO : public TRANSITION
	//{
	//	/**
	//	 * 遷移予定のカメラのキーを受け取りつつ生成
	//	 */
	//	inline INTRO(const std::string& key) { m_NextKey = key; }

	//	// 実行
	//	bool Execute(CCamera_Manager*& cameraManager) override;

	//	// 変更
	//	void Change(TRANSITION*& r) override;
	//};

	////****************************************************
	//// アウトロ構造体の定義
	////****************************************************
	//struct OUTRO : public TRANSITION
	//{
	//	// 実行
	//	bool Execute(CCamera_Manager*& cameraManager) override;

	//	// 変更
	//	void Change(TRANSITION*& r) override;
	//};

public:

	//****************************************************
	// function
	//****************************************************

	/**
	 * 保有している全てのカメラインスタンスの更新処理を行います
	 */
	void Update();

	/**
	 * インスタンス同士でパラメータを繋ぎ合わせてトランジションをします
	 * 指定されたキーに該当するインスタンスが存在しなければ例外をスローします
	 *
	 * key - 遷移予定のカメラのキー
	 */
	void SetTransition(const std::string& key);

	// レジストリの参照の取得
	inline CRegistry<CCamera*>& RefRegistry() const { return *m_upRegistry; }

	// 選択中のカメラの参照の取得
	inline CCamera& RefSelectCamera() const { return *m_upRegistry->BindAtKey(m_SelectKey); }

	// カメラの選択
	inline void SetSelectKey(const std::string& key) { m_SelectKey = key; }

private:

	//****************************************************
	// special function
	//****************************************************
	CCamera_Manager();				// デフォルトコンストラクタ
	~CCamera_Manager() override;	// デストラクタ

	//****************************************************
	// function
	//****************************************************
	bool Initialize();	// 初期化処理
	void Finalize();	// 終了処理

	//****************************************************
	// data
	//****************************************************
	std::unique_ptr<CRegistry<CCamera*>> m_upRegistry;	// カメラ用レジストリ

	std::string m_SelectKey;	// 選択中カメラのキー
	//TRANSITION* m_pTransition;	// トランジション情報のポインタ
};