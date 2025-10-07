//============================================================================
// 
// プレイヤー、ヘッダーファイル [player.h]
// Author : 福田歩希
// 
//============================================================================

#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "object.X.h"

//****************************************************
// プレイヤークラスの定義
//****************************************************
class CPlayer : public CObjectX
{
	//****************************************************
	// 静的メンバ定数を定義 (非公開)
	//****************************************************

	// 目標値への補間係数
	static constexpr float COEF_CORRECT_TARGET = 0.1f;

	// 速度
	static constexpr float COEF_MOVE_SPEED = 2.5f;

	// 制動力
	static constexpr float COEF_BRAKING = 0.5f;

public:

	//****************************************************
	// 静的メンバ変数の宣言 (公開)
	//****************************************************

	// デフォルトのファクトリ
	static std::function<bool(CPlayer*)> s_fpDefaultFactory;

	//****************************************************
	// special function
	//****************************************************
	CPlayer(OBJ::TYPE Type, OBJ::LAYER Layer); // コンストラクタ
	~CPlayer() override;                       // デストラクタ

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

	// 残りジャンプ回数操作用
	inline int  GetLeftNumJump() const   { return m_nLeftNumJump; }
	inline void SetLeftNumJump(int nNum) { m_nLeftNumJump = nNum; }

	// 加速度操作用
	inline const D3DXVECTOR3& GetVelocity() const                 { return m_Velocity; }
	inline void               SetVelocity(const D3DXVECTOR3& Vel) { m_Velocity = Vel; }

	// 目標サイズ操作用
	inline const D3DXVECTOR3& GetSizeTarget() const                        { return m_SizeTarget; }
	inline void               SetSizeTarget(const D3DXVECTOR3& SizeTarget) { m_SizeTarget = SizeTarget; }

	// 目標向き操作用
	inline const D3DXVECTOR3& GetRotTarget() const                       { return m_RotTarget; }
	inline void               SetRotTarget(const D3DXVECTOR3& RotTarget) { m_RotTarget = RotTarget; }

	// 目標位置操作用
	inline const D3DXVECTOR3& GetPosTarget() const                       { return m_PosTarget; }
	inline void               SetPosTarget(const D3DXVECTOR3& PosTarget) { m_PosTarget = PosTarget; }

private:

	//****************************************************
	// data
	//****************************************************
	int         m_nLeftNumJump; // ジャンプ可能回数
	D3DXVECTOR3 m_Velocity;     // 加速度
	D3DXVECTOR3 m_SizeTarget;   // 目標サイズ
	D3DXVECTOR3 m_RotTarget;    // 目標向き
	D3DXVECTOR3 m_PosTarget;    // 目標位置

	std::function<void()> m_fpDefaultMove; // デフォルトの移動処理
	std::function<void()> m_fpDecorate;    // オマケの動作
};