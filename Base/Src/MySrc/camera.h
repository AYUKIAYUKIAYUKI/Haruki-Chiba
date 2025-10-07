//============================================================================
//
// カメラ、ヘッダファイル [camera.h]
// Author : 福田歩希
//
//============================================================================

// インクルードガード
#pragma once

//****************************************************
// カメラクラス
//****************************************************
class CCamera
{
public:

	//****************************************************
	// special function
	//****************************************************
	CCamera();								// デフォルトコンストラクタ
	CCamera(const JSON& json);				// 変換コンストラクタ
	CCamera(const CCamera& obj) = default;	// コピーコンストラクタ
	~CCamera();								// デストラクタ

	//****************************************************
	// function
	//****************************************************
	
	/**
	 * メンバ変数の初期化はイニシャライザーで事足りているため
	 * この初期化処理は現在不使用です
	 */
	[[ deprecated ]] bool Initialize();

	/**
	 * 定点カメラでない場合、メインループで呼び出してください
	 */
	void Update();

	/**
	 * オブジェクトの描画直前に呼び出すことで、カメラをセットします
	 * 
	 * pos - 引数に数値を渡さないでください、{ 0.0f, 0.0f, 0.0f }固定で使用してください
	 */
	void SetCamera(D3DXVECTOR3 pos);

	/**
	 * 目標値から実値を強制的に少しずらし、振動再現します
	 * 自動補間との併用となるので必要範囲で毎フレーム呼び出してください
	 * 
	 * coef - ずらし強度
	 */
	void SetVibration(float coef);

	// ビュー上書きの使用状態を取得
	bool GetStateOverwriteView();

	// 向きの操作用
	inline		 D3DXVECTOR3& RefRot()						 { return m_Rot; }
	inline const D3DXVECTOR3& GetRot() const				 { return m_Rot; }
	inline		 void		  SetRot(const D3DXVECTOR3& rot) { m_Rot = rot; }

	// 目標向きの操作用
	inline		 D3DXVECTOR3& RefRotTarget()							 { return m_RotTarget; }
	inline const D3DXVECTOR3& GetRotTarget() const						 { return m_RotTarget; }
	inline		 void		  SetRotTarget(const D3DXVECTOR3& rotTarget) { m_RotTarget = rotTarget; }

	// 座標の操作用
	inline		 D3DXVECTOR3& RefPos()						 { return m_Pos; }
	inline const D3DXVECTOR3& GetPos() const				 { return m_Pos; }
	inline		 void		  SetPos(const D3DXVECTOR3& pos) { m_Pos = pos; }

	// 目標座標の操作用
	inline		 D3DXVECTOR3& RefPosTarget()							 { return m_PosTarget; }
	inline const D3DXVECTOR3& GetPosTarget() const						 { return m_PosTarget; }
	inline		 void		  SetPosTarget(const D3DXVECTOR3& posTarget) { m_PosTarget = posTarget; }

	// 視点座標の取得
	inline const D3DXVECTOR3& GetPosV() const { return m_PosV; }

	// 注視点座標の取得
	inline const D3DXVECTOR3& GetPosR() const { return m_PosR; }

	// 距離の操作用
	inline float GetDistance() const		 { return m_fDistance; }
	inline void	 SetDistance(float distance) { m_fDistance = distance; }

	// 目標距離の操作用
	inline float GetDistanceTarget() const				 { return m_fDistanceTarget; }
	inline void	 SetDistanceTarget(float distanceTarget) { m_fDistanceTarget = distanceTarget; }

	// 俯瞰度合いの操作用
	inline float GetUpAdjust() const		 { return m_fUpAdjust; }
	inline void	 SetUpAdjust(float upAdjust) { m_fUpAdjust = upAdjust; }

	// 目標俯瞰度合いの操作用
	inline float GetUpAdjustTarget() const				 { return m_fUpAdjustTarget; }
	inline void	 SetUpAdjustTarget(float upAdjustTarget) { m_fUpAdjustTarget = upAdjustTarget; }

	// ビュー上書きをセット
	inline void SetOverwriteView(std::function<void(CCamera&)>&& fp) { m_fpOverwriteView = fp; }

	// 指数減衰の使用状態設定
	inline void SetUseDecayState(bool state) { m_bUseDecay = state; }

	/**
	 * フリーカメラのみの更新を行います
	 */
	static void UpdateFreeCam();

	// フリーカメラの参照
	static inline std::unique_ptr<CCamera>& RefFreeCam() { return s_upFreeCam; }

	// フリーカメラの使用状態取得
	static inline bool GetStateUseFreeCam() { return s_bUseFree; }

private:

	//****************************************************
	// 静的メンバ定数を定義
	//****************************************************
	static constexpr float COEF_ADJUST = 0.1f;

	//****************************************************
	// function
	//****************************************************
	void Control();						// カメラ操作処理
	void ControlImGui();				// ImGuiによる操作
	void ControlFly();					// フライ操作
	void ControlOrbit();				// オービット操作
	void CorrectToTarget();				// 目標値への補間
	void AutoRestrictPitch();			// ピッチ角の範囲制限
	void CameraTransformFly();			// フライ操作でのカメラ適応
	void CameraTransformOrbit();		// オービット操作でのカメラ適応
	void CalcMtxProjection();			// プロジェクション行列計算
	void CalcMtxView(D3DXVECTOR3 pos);	// ビュー行列計算
	void PrintDebug();					// デバッグ表示
	
	//****************************************************
	// data
	//****************************************************
	
	// カメラパラメータ
	D3DXVECTOR3	m_Rot;				// 向き
	D3DXVECTOR3	m_RotTarget;		// 目標向き
	D3DXVECTOR3	m_Pos;				// 座標
	D3DXVECTOR3	m_PosTarget;		// 目標座標
	D3DXVECTOR3	m_PosV;				// 視点座標
	D3DXVECTOR3	m_PosTargetV;		// 目標視点座標
	D3DXVECTOR3	m_PosR;				// 注視点座標
	D3DXVECTOR3	m_PosTargetR;		// 目標注視点座標
	float		m_fDistance;		// 視点 -> 注視点間の距離
	float		m_fDistanceTarget;	// 目標間距離
	float		m_fUpAdjust;		// 俯瞰度合い
	float		m_fUpAdjustTarget;	// 目標俯瞰度合い
	D3DXVECTOR3	m_VecU;				// 上方向ベクトル
	D3DXMATRIX	m_MtxProjection;	// プロジェクション行列
	D3DXMATRIX	m_MtxView;			// ビュー行列

	// 拡張処理
	std::function<void(CCamera&)> m_fpOverwriteView;	// 強制ビュー上書き

	// ビュー状態
	bool  m_bUseFirstPerson;	// 一人称視点の使用状態 (falseの場合は三人称視点)
	bool  m_bUseDecay;			// 指数減衰の使用状態
	bool  m_bUseControlDrag;	// ドラッグ操作の使用状態
	bool  m_bUseHorizon;		// 水平ビューの使用状態
	bool  m_bUseVertical;		// 垂直ビューの使用状態
	float m_fDragMoveSpeed;		// ドラッグ操作時の移動速度
	float m_fDragRotateSpeed;	// ドラッグ操作時の回転速度

	// フリーカメラ情報
	static bool s_bUseFree;							// 使用状態
	static std::unique_ptr<CCamera> s_upFreeCam;	// カメラのインスタンス
};