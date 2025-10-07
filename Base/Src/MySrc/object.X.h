//============================================================================
// 
// オブジェクト(Xモデル)、ヘッダーファイル [object.X.h]
// Author : 福田歩希
// 
//============================================================================

#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"
#include "X.manager.h"

//****************************************************
// Xオブジェクトクラスの定義
//****************************************************
class CObjectX : public CObject
{
public:

	//****************************************************
	// 静的メンバ変数 (公開)
	//****************************************************

	// デフォルトのファクトリ
	static std::function<bool(CObjectX*)> s_fpDefaultFactory;

	//****************************************************
	// special function
	//****************************************************
	CObjectX(OBJ::TYPE Type, OBJ::LAYER Layer); // コンストラクタ
	~CObjectX() override;                       // デストラクタ

	//****************************************************
	// function
	//****************************************************

	// 初期化処理
	bool Initialize();

	// 終了処理
	void Finalize();

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

	//****************************************************
	// inline function
	//****************************************************

	// モデル操作用
	inline CXManager::MODEL* GetModel() const                         { return useful::PtrCheck(m_pModel, "Model"); }
	inline void              SetModel(CXManager::MODEL* const pModel) { m_pModel = pModel; }

	// サイズ操作用
	inline const D3DXVECTOR3& GetSize() const                  { return m_Size; }
	inline void               SetSize(const D3DXVECTOR3& Size) { m_Size = Size; }

	// 向き操作用
	inline const D3DXVECTOR3& GetRot() const                 { return m_Rot; }
	inline void               SetRot(const D3DXVECTOR3& Rot) { m_Rot = Rot; }

	// 位置操作用
	inline const D3DXVECTOR3& GetPos() const                 { return m_Pos; }
	inline void               SetPos(const D3DXVECTOR3& Pos) { m_Pos = Pos; }

	// 色操作用
	inline const D3DXCOLOR& GetCol() const               { return m_Col; }
	inline void             SetCol(const D3DXCOLOR& Col) { m_Col = Col; }

	// 色反映状態
	inline void SetUseColState(bool bState = true) { m_bUseCol = bState; };

	// ワールド行列を取得
	inline const D3DXMATRIX& GetMtxWorld() const { return m_MtxWorld; }

private:

	//****************************************************
	// data
	//****************************************************
	CXManager::MODEL* m_pModel;   // モデル情報
	D3DXVECTOR3	      m_Size;     // サイズ
	D3DXVECTOR3	      m_Rot;      // 向き
	D3DXVECTOR3	      m_Pos;      // 座標
	D3DXCOLOR         m_Col;      // 色
	bool              m_bUseCol;  // 色反映状態
	D3DXMATRIX        m_MtxWorld; // ワールド行列
};