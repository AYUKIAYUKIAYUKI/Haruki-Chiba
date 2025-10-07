//============================================================================
// 
// インプットマネージャー、ヘッダーファイル [input.managaer.h]
// Author : 福田歩希
// 
//============================================================================

#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "singleton.h"

#include "input_keyboard.h"
#include "input_mouse.h"
#include "input_pad.h"

//****************************************************
// インプットマネージャークラスの定義
//****************************************************
class CInputManager final : public CSingleton<CInputManager>
{
	//****************************************************
	// フレンド宣言
	//****************************************************
	friend struct std::default_delete<CInputManager>;
	friend CInputManager& CSingleton<CInputManager>::RefInstance();

public:

	//****************************************************
	// 静的メンバ変数の宣言 (公開)
	//****************************************************
	
	// 入力 -> 方向 汎用関数
	static std::function<std::optional<float>()> s_fpGetInputDir;

	//****************************************************
	// function
	//****************************************************
	
	// 初期化処理
	bool Initialize(HINSTANCE hInstance);
	bool Initialize() { return true; }
	
	// 終了処理
	void Finalize();

	// 更新処理
	void Update();

	//****************************************************
	// inline funciton
	//****************************************************
	
	// キーボードの取得
	inline CInputKeyboard* GetKeyboard() const { return m_upKeyboard.get(); }
	
	// マウスの取得
	inline CInputMouse* GetMouse() const { return m_upMouse.get(); }

	// パッドの取得
	inline CInputPad* GetPad() const { return m_upPad.get(); }

private:

	//****************************************************
	// special function
	//****************************************************
	CInputManager();           // デフォルトコンストラクタ
	~CInputManager() override; // デストラクタ

	//****************************************************
	// data
	//****************************************************
	std::unique_ptr<CInputKeyboard> m_upKeyboard; // キーボード
	std::unique_ptr<CInputMouse>    m_upMouse;    // マウス
	std::unique_ptr<CInputPad>      m_upPad;      // パッド
};