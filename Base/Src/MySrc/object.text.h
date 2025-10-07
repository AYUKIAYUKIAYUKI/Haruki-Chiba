//============================================================================
// 
// テキストオブジェクト、ヘッダーファイル [object.text.h]
// Author : 福田歩希
// 
//============================================================================

#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

//****************************************************
// テキストオブジェクトクラスの定義
//****************************************************
class CObjectText : public CObject
{
	//****************************************************
	// 静的メンバ定数の定義 (非公開)
	//****************************************************

	// 幽霊時間の規定値
	static constexpr int DEFAULT_GHOSTLIMIT = 20;

	// 波打ち強度の規定値
	static constexpr float DEFAULT_WAEVCOEF = 3.0f;

public:

	//****************************************************
	// special function
	//****************************************************
	CObjectText(OBJ::TYPE Type, OBJ::LAYER Layer); // コンストラクタ
	~CObjectText() override;                       // デストラクタ

	//****************************************************
	// function
	//****************************************************

	// 初期化処理
	bool Initialize() { return 0; };

	// 終了処理
	void Finalize() {};

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

	// 生成
	static CObjectText* Create(const JSON& Json);

	//****************************************************
	// inline function
	//****************************************************
	
	// 位置の操作用
	inline const D3DXVECTOR2& GetPos() const                 { return m_Pos; }
	inline void               SetPos(const D3DXVECTOR2& Pos) { m_Pos = Pos; }

	// サイズの操作用
	inline const D3DXVECTOR2& GetSize() const                  { return m_Size; }
	inline void               SetSize(const D3DXVECTOR2& Size) { m_Size = Size; }

	// 色の操作用
	inline const D3DXCOLOR& GetCol() const               { return m_Col; }
	inline void             SetCol(const D3DXCOLOR& Col) { m_Col = Col; }

private:

	//****************************************************
	// function
	//****************************************************
	void TextAnimation(); // テキスト送りのアニメーション

	//****************************************************
	// data
	//****************************************************
	LPD3DXFONT  m_pFont;       // フォントのポインタ
	std::string m_sText;       // 文章
	std::string m_sTextTarget; // 目標文章
	int         m_nLimit;      // 表示時間
	int         m_nGhostLimit; // 幽霊時間
	int         m_nSpeed;      // 表示速度
	int         m_nStopSpeed;  // 遅延速度
	bool        m_bWave;       // 波打ち使用状態
	D3DXVECTOR2 m_WavePos;     // 波打ち変動位置
	float       m_fWaveCoef;   // 波打ち強度
	D3DXVECTOR2 m_Pos;         // 位置
	D3DXVECTOR2 m_Size;        // サイズ
	D3DXCOLOR   m_Col;         // 色
};