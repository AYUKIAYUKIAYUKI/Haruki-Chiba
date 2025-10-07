//============================================================================
// 
// オブジェクト(3D)、ヘッダーファイル [object.3D.h]
// Author : 福田歩希
// 
//============================================================================

#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

//****************************************************
// 3Dオブジェクトクラス
//****************************************************
class CObject3D : public CObject
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
	// special function
	//****************************************************
	CObject3D(OBJ::TYPE Type, OBJ::LAYER Layer); // コンストラクタ
	~CObject3D() override;                       // デストラクタ

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

	// <setter>
	void BindTex(LPDIRECT3DTEXTURE9 pTex);		// テクスチャ割当

	// <getter/setter>
	const D3DXVECTOR3& GetSize() const;	// サイズ取得
	void SetSize(D3DXVECTOR3 Size);		// サイズ設定

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// 向き取得
	void SetRot(D3DXVECTOR3 Rot);		// 向き設定

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// 座標取得
	void SetPos(D3DXVECTOR3 Pos);		// 座標設定

	// <getter/setter>
	const D3DXCOLOR& GetCol() const;	// 色取得
	void SetCol(D3DXCOLOR Col);			// 色設定

	// <getter/setter>
	const float& GetAlpha() const;	// アルファ値取得
	void SetAlpha(float fAlpha);	// アルファ値設定

	// <getter>
	const float& GetLength() const;	// 展開用対角線取得

	// <static function>
	static CObject3D* Create(OBJ::TYPE Type, OBJ::LAYER Layer, LPDIRECT3DTEXTURE9 pTexture); // 生成

private:

	//****************************************************
	// function
	//****************************************************
	HRESULT CreateVtxBuff(); // 頂点バッファの生成
	void	SetMtxWorld();		// ワールド行列設定
		
	//****************************************************
	// data
	//****************************************************
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;	// 頂点バッファのポインタ
	LPDIRECT3DTEXTURE9	m_pTex;		// テクスチャのポインタ
	D3DXVECTOR3	m_Size;		// サイズ
	D3DXVECTOR3	m_Rot;		// 向き
	D3DXVECTOR3	m_Pos;		// 座標
	D3DXVECTOR3 m_Scale;	// 拡大率
	D3DXCOLOR	m_Col;		// 色
	float		m_fLength;	// 展開用対角線
	float		m_fAngle;	// 対角線用角度
	D3DXMATRIX	m_MtxWorld;	// ワールド行列
};