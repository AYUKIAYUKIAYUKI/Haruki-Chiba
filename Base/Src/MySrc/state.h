//======================================================
//
//ステイトパターン実装[state.h] 
//Auther:haruki Chiba
//
//======================================================


//==========================================================
//インクルードガード
#pragma once


//==========================================================
//インクルード
#include "player.h"


//==========================================================
//状態の基礎クラス
//==========================================================
class CStateBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CStateBase() {};

	/**
	 * @brief デストラクタ
	 */
	~CStateBase() {};

public:

	/**
	 * @brief ステートが始まるときに一度だけ呼ばれる関数
	 */
	virtual void OnStart() {}

	/**
	 * @brief ステートが更新されるときに呼ばれる関数
	 */
	virtual void OnUpdate() {}

	/**
	* @brief ステートが終了する時に一度だけ呼ばれる関数
	*/
	virtual void OnExit() {}
};


//==========================================================
//状態の管理クラス
//==========================================================
class CStateMachine
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CStateMachine() {};

	/**
	 * @brief デストラクタ
	 */
	~CStateMachine() 
	{ 
		m_spNowState.reset();
	}

	/**
	 * @brief 状態を変更する処理
	 */
	void ChangeState(std::shared_ptr<CStateBase> a_spNewState)
	{
		// すでにステートがセットされてたら終了する
		if (m_spNowState != nullptr)
		{
			m_spNowState->OnExit();
			m_spNowState = nullptr;
		}

		// 新しいステートをセットする
		m_spNowState = a_spNewState;

		// 新しいステートを開始する
		m_spNowState->OnStart();
	}

	/**
	 * @brief 更新処理
	 */
	void Update()
	{
		if (m_spNowState != nullptr)
		{
			m_spNowState->OnUpdate();
		}
	}

private:
	std::shared_ptr<CStateBase> m_spNowState = nullptr; //状態の基礎クラスのスマートポインター
};

//==========================================================
// プレイヤー専用の状態の基底クラス
//==========================================================
class CPlayerStateBase :public CStateBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CPlayerStateBase() {};

	/**
	 * @brief デストラクタ
	 */
	~CPlayerStateBase()
	{

	}

	/**
	 * @brief プレイヤーの情報を同期
	 * @param [in] a_pPlayer : プレイヤーのポインター
	 */
	inline void SetOwner(CPlayer* a_pPlayer)
	{
		m_pPlayer = a_pPlayer;
	}

	/**
	 * @brief 移動キーが押されたかどうかの判定処理
	 */
	bool MoveKeyflag();

	/**
	 * @brief ジャンプ処理を呼ぶ処理
	 */
	bool CallJump();

	/**
	 * @brief 状態の変更処理をまとめる処理
	 * @param [in] (テンプレート)Classname : クラス名
	 */
	template<class Classname>
	Classname* TChangeState()
	{
		auto spStandState = std::make_shared<Classname>(); //設定したクラス情報を代入
		m_pPlayer->ChangeState(spStandState);              //その情報を設定する
		return nullptr;
	}

	inline CPlayer* GetPlayer() { return m_pPlayer; }      //プレイヤー情報を返す
private:
	CPlayer* m_pPlayer = nullptr;                          //プレイヤーのポインター
};


//==========================================================
// プレイヤーの待機状態
//==========================================================
class CPlayer_DefaultState :public CPlayerStateBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CPlayer_DefaultState() {};

	/**
	 * @brief デストラクタ
	 */
	~CPlayer_DefaultState() {};

	/**
	 * @brief 一度だけ呼ばれる初めの処理
	 * @param  オーバライドしている
	 */
	void OnStart()override;

	/**
	 * @brief 更新処理
	 * @param  オーバライドしている
	 */
	void OnUpdate()override;

	/**
	 * @brief 一度だけ呼ばれる終わりの処理
	 * @param  オーバライドしている
	 */
	void OnExit()override {};
};


//==========================================================
// プレイヤーの移動状態
//==========================================================
class CPlayer_MoveState :public CPlayerStateBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CPlayer_MoveState() {};

	/**
	 * @brief デストラクタ
	 */
	~CPlayer_MoveState() {};

	/**
	 * @brief 一度だけ呼ばれる初めの処理
	 * @param  オーバライドしている
	 */
	void OnStart()override {};

	/**
	 * @brief 更新処理
	 * @param  オーバライドしている
	 */
	void OnUpdate()override;

	/**
	 * @brief 一度だけ呼ばれる終わりの処理
	 * @param  オーバライドしている
	 */
	void OnExit()override {};
};


//==========================================================
// プレイヤーのジャンプ状態
//==========================================================
class CPlayer_JumpState :public CPlayerStateBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CPlayer_JumpState() {};

	/**
	 * @brief デストラクタ
	 */
	~CPlayer_JumpState() {};

	/**
	 * @brief 一度だけ呼ばれる初めの処理
	 * @param  オーバライドしている
	 */
	void OnStart()override {};

	/**
	 * @brief 更新処理
	 * @param  オーバライドしている
	 */
	void OnUpdate()override;

	/**
	 * @brief 一度だけ呼ばれる終わりの処理
	 * @param  オーバライドしている
	 */
	void OnExit()override {};
};


//==========================================================
// プレイヤーのダメージ状態
//==========================================================
class CPlayer_DamageState :public CPlayerStateBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CPlayer_DamageState() {};

	/**
	 * @brief デストラクタ
	 */
	~CPlayer_DamageState() {};

	/**
	 * @brief 一度だけ呼ばれる初めの処理
	 * @param  オーバライドしている
	 */
	void OnStart()override {};

	/**
	 * @brief 更新処理
	 * @param  オーバライドしている
	 */
	void OnUpdate()override;

	/**
	 * @brief 一度だけ呼ばれる終わりの処理
	 * @param  オーバライドしている
	 */
	void OnExit()override {};
};