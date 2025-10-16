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
	// かんたんステート用の列挙型を定義
	//****************************************************
	enum class State : unsigned char
	{
		DEFAULT = 0, // 通常
		JUMP,        // ジャンプ
		HIP,         // オシリ
		DAMAGE,		 // ダメージ
		MAX
	};

	//****************************************************
	// 静的メンバ定数を定義 (非公開)
	//****************************************************

	/* 最終的には定数となりますが、漸次的に編集可能な値とします */

	// 目標値への補間係数
	static float COEF_CORRECT_TARGET;

	// 重力加速度
	static float COEF_GRAVITY;

	// ジャンプ可能回数
	static int NUM_LEFT_JUMP;

	// ジャンプ力
	static float COEF_TRIGGER_JUMP;

	// 移動速度
	static float COEF_MOVE_SPEED;
	static float COEF_MOVE_SPEED_AIR;

	// 制動力
	static float COEF_BRAKING;

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
	//inline int  GetLeftNumJump() const   { return m_nLeftNumJump; }
	//inline void SetLeftNumJump(int nNum) { m_nLeftNumJump = nNum; }

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
	// function
	//****************************************************

	// かんたん状態遷移
	void Change(State State, int nLimit, std::function<void()> fpOpt);

	// 操作など
	void Move(float fSpeed); // 移動
	bool Gravity();          // 重力加速
	void SetWave();          // 振動設定
	void PlayWave();         // 振動再生
	void ValueEdit();        // 数値編集
	bool Hit();				 // 攻撃当たった処理
	const char* ToString(State s);//現在のステートを返す処理
	//****************************************************
	// data
	//****************************************************
	int         m_nFrame;       // フレーム数
	State       m_State;        // かんたんステート
	//int       m_nLeftNumJump; // ジャンプ可能回数
	D3DXVECTOR3 m_Velocity;     // 加速度
	D3DXVECTOR3 m_SizeTarget;   // 目標サイズ
	D3DXVECTOR3 m_RotTarget;    // 目標向き
	D3DXVECTOR3 m_PosTarget;    // 目標位置

	// 状態ごとの動作の配列
	using Executer = std::function<bool()>;
	std::array<Executer, static_cast<unsigned char>(State::MAX)> m_afpExecuteState;

	// 振動再生用：目標サイズリスト
	using Michos = std::list<D3DXVECTOR3>;
	Michos m_vMichos;
};