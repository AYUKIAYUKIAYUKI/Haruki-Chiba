//============================================================================
// 
// オブジェクト(2D)、ヘッダーファイル [object.2D.h]
// Author : 福田歩希
// 
//============================================================================

#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

//****************************************************
// 2Dオブジェクトクラス
//****************************************************
class CObject2D : public CObject
{
	//****************************************************
	// 静的メンバ定数 (非公開)
	//****************************************************

	// 頂点数
	static constexpr WORD NUM_VTX = 4;

	// プリミティブ数
	static constexpr WORD NUM_PRIM = 2;

public:

	//****************************************************
	// 静的メンバ変数 (公開)
	//****************************************************

	// デフォルトのファクトリ
	static std::function<bool(CObject2D*)> s_fpDefaultFactory;

	//****************************************************
	// special function
	//****************************************************
	CObject2D(OBJ::TYPE Type, OBJ::LAYER Layer); // コンストラクタ
	~CObject2D() override;                       // デストラクタ

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

	// テクスチャの操作用
	inline LPDIRECT3DTEXTURE9 GetTex() const                  { return useful::PtrCheck(m_pTex, "Texture"); }
	inline void               SetTex(LPDIRECT3DTEXTURE9 pTex) { m_pTex = pTex; }

	// サイズ操作用
	inline const D3DXVECTOR2& GetSize() const                  { return m_Size; }
	inline void               SetSize(const D3DXVECTOR2& Size) { m_Size = Size; }

	// 向き操作用
	inline float GetRot() const     { return m_fRot; }
	inline void  SetRot(float fRot) { m_fRot = fRot; }

	// 位置操作用
	inline const D3DXVECTOR2& GetPos() const                 { return m_Pos; }
	inline void               SetPos(const D3DXVECTOR2& Pos) { m_Pos = Pos; }

	// 色操作用
	inline const D3DXCOLOR& GetCol() const               { return m_Col; }
	inline void             SetCol(const D3DXCOLOR& Col) { m_Col = Col; }

	// 展開用対角線取得
	inline float GetLength() const { return m_fLength; }

	// テクスチャサイズ操作用
	inline const D3DXVECTOR2& GetTexSize() const                  { return m_TexSize; }
	inline void               SetTexSize(const D3DXVECTOR2& Size) { m_TexSize = Size; }

	// テクスチャ横分割幅操作用
	inline float GetTexWidth() const       { return m_fTexWidth; }
	inline void  SetTexWidth(float fWidth) { m_fTexWidth = fWidth; }

	// テクスチャ縦分割幅操作用
	inline float GetTexHeight() const        { return m_fTexHeight; }
	inline void  SetTexHeight(float fHeight) { m_fTexHeight = fHeight; }

	// 現在のテクスチャ横分割幅操作用
	int  GetNowPatternU() const           { return m_nNowPatternU; }
	void SetNowPatternU(int nNowPatternU) { m_nNowPatternU = nNowPatternU; }

	// 現在のテクスチャ縦分割幅操作用
	int  GetNowPatternV() const           { return m_nNowPatternV; }
	void SetNowPatternV(int nNowPatternV) { m_nNowPatternV = nNowPatternV; }

private:

	//****************************************************
	// function
	//****************************************************
	HRESULT CreateVtxBuff(); // 頂点バッファの生成

	//****************************************************
	// data
	//****************************************************
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;     // 頂点バッファのポインタ
	LPDIRECT3DTEXTURE9		m_pTex;         // テクスチャのポインタ
	D3DXVECTOR2				m_Size;         // サイズ
	float                   m_fRot;	        // 向き
	D3DXVECTOR2				m_Pos;			// 位置
	D3DXCOLOR				m_Col;			// 色
	float					m_fLength;		// 展開用対角線
	float					m_fAngle;		// 対角線用角度
	D3DXVECTOR2				m_TexSize;		// テクスチャサイズ
	float					m_fTexWidth;	// 横テクスチャ分割幅
	float					m_fTexHeight;	// 縦テクスチャ分縦幅
	int						m_nNowPatternU;	// 現在の横テクスチャ種類
	int						m_nNowPatternV;	// 現在の縦テクスチャ種類
};