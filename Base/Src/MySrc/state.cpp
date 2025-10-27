//========================================
//
//状態処理[state.cpp]
//Auther:Haruki Chiba
//
//========================================


//========================================
//インクルード
#include "state.h"           
#include "input.manager.h"   //キーボード処理
#include "object.manager.h"
#include "impact.h"

//========================================
//名前空間
namespace PLAYER_INFO
{
	float f_Check_Velocity = 0.001f; //加速度の確認用変数
}


//==================================================================================================================================================
//基礎となるプレイヤーの状態処理
//==================================================================================================================================================

//========================================
//移動キー判定処理
//========================================
bool CPlayerStateBase::MoveKeyflag()
{
	//汚いがいったん代用
	if (CInputManager::RefInstance().GetKeyboard()->GetPress(DIK_W) || CInputManager::RefInstance().GetKeyboard()->GetPress(DIK_A) ||
		CInputManager::RefInstance().GetKeyboard()->GetPress(DIK_S) || CInputManager::RefInstance().GetKeyboard()->GetPress(DIK_D))
	{
		return true;
	}
	return false;
}


//========================================
//プレイヤージャンプ処理を呼ぶ処理
//========================================
bool CPlayerStateBase::CallJump()
{
	//SPACEキーが押された時
	if (CInputManager::RefInstance().GetKeyboard()->GetTrigger(DIK_SPACE))
	{
		GetPlayer()->Jump();                  //ジャンプ処理
		TChangeState<CPlayer_JumpState>();    //ジャンプ状態へ移行

		return true;
	}

	return false;
}


//==================================================================================================================================================
//プレイヤーの通常状態処理
//==================================================================================================================================================

//========================================
//通常状態の一度だけ呼ばれる処理
//========================================
void CPlayer_DefaultState::OnStart()
{
	//GetPlayer()->SetVelocity({ 0.0f,0.0f,0.0f });                           //加速度の初期化
	//GetPlayer()->Change(CPlayer::State::DEFAULT, 0, [this]() -> void {}); //現在のプレイヤーステートを変える
}


//========================================
//通常状態の更新処理
//========================================
void CPlayer_DefaultState::OnUpdate()
{
	//加速度が、限りなく０に近くない時（ずっと処理が回らないように） NOTE:汚いがいったん代用
	if (GetPlayer()->GetVelocity().x > PLAYER_INFO::f_Check_Velocity || GetPlayer()->GetVelocity().x < -PLAYER_INFO::f_Check_Velocity
		|| GetPlayer()->GetVelocity().z > PLAYER_INFO::f_Check_Velocity || GetPlayer()->GetVelocity().z < -PLAYER_INFO::f_Check_Velocity)
	{
		GetPlayer()->Move(GetPlayer()->GetMOVE_SPEED()); //移動処理 
	}
	else
	{
		GetPlayer()->SetVelocity({ 0.0f,0.0f,0.0f });
	}

	//移動キーが押された時
	if (MoveKeyflag() == true)
	{
		TChangeState<CPlayer_MoveState>();    //移動状態へ移行
		return;
	}


	//ジャンプ処理
	if (CallJump() == true)
	{
		return;
	}


	//Bキーが押された時
	if (CInputManager::RefInstance().GetKeyboard()->GetTrigger(DIK_B))
	{
		TChangeState<CPlayer_DamageState>();  //ダメージ状態へ移行
		return;
	}
}

//========================================
//現在のステートを教えるよ
//========================================
const char* CPlayer_DefaultState::GetStateName()
{ 
	const char* NowState = "Default";
	return NowState ;
}

//==================================================================================================================================================
//プレイヤーの移動状態処理
//==================================================================================================================================================

//========================================
//移動状態の更新処理
//========================================
void CPlayer_MoveState::OnUpdate()
{
	GetPlayer()->Move(GetPlayer()->GetMOVE_SPEED()); //移動処理 

	//ジャンプ処理
	if (CallJump() == true)
	{
		return;
	}

	//移動キーが押されていない時
	if (MoveKeyflag() == false)
	{
		TChangeState<CPlayer_DefaultState>(); //通常状態へ移行
		return;
	}
}

//========================================
//現在のステートを教えるよ
//========================================
const char* CPlayer_MoveState::GetStateName()
{
	const char* NowState = "Move";
	return NowState;
}

//==================================================================================================================================================
//プレイヤーのジャンプ状態処理
//==================================================================================================================================================

//========================================
//ジャンプ状態の更新処理
//========================================
void CPlayer_JumpState::OnUpdate()
{
	GetPlayer()->Move(GetPlayer()->GetMOVE_SPEED()); //移動処理 

	//地面についた時(処理を完結)
	if (GetPlayer()->InJump() == true)
	{

		//衝撃波生成
		using namespace OBJ;
		auto pImpact = CObject::Create<CImpact>(TYPE::IMPUCT, LAYER::DEFAULT, CImpact::s_fpDefaultFactory);
		pImpact->SetPos(GetPlayer()->GetPos());			//位置を保存
		pImpact->SetCreator(GetPlayer());				//発生させた者を保存
		pImpact->SetRadius(GetPlayer()->GetRadius());	//半径を保存
		pImpact->Start();

		//プレイヤーが格納されているリストを取得
		std::list<CObject*> playerlist = CObjectManager::RefInstance().RefObjList(OBJ::TYPE::PLAYER);

		//コライダーの更新処理
		for (auto other : playerlist)
		{
			if (other == GetPlayer())
			{//発生させた者ではない
				continue;
			}

		}

		//ダメージ処理
		GetPlayer()->Damage();

		TChangeState<CPlayer_DefaultState>(); //通常状態へ移行
		return;
	}
}

//========================================
//現在のステートを教えるよ
//========================================
const char* CPlayer_JumpState::GetStateName()
{
	const char* NowState = "Jump";
	return NowState;
}

//==================================================================================================================================================
//プレイヤーのダメージ状態処理
//==================================================================================================================================================

//========================================
//ダメージ状態の更新処理
//========================================
void CPlayer_DamageState::OnUpdate()
{
	nCount++;

	if (nCount > 120)
	{
		GetPlayer()->Damage(); //ダメージ処理

		TChangeState<CPlayer_DefaultState>();     //通常状態へ移行
	}
	
	return;
}

//========================================
//現在のステートを教えるよ
//========================================
const char* CPlayer_DamageState::GetStateName()
{
	const char* NowState = "Damage";
	return NowState;
}