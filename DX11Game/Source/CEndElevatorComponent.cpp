//======================================================================
//											CEndElevatorComponent.cpp
//	エンドエレベーターコンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/03	作成
//
//======================================================================


//===== インクルード部 =====
#include "CEndElevatorComponent.h"
#include "CPlayer.h"
#include "Camera.h"
#include "CObjectManager.h"
#include "input.h"

#include "CPlayerInput.h"
#include "CPlayerActionComponent.h"

#include "CElevatorRoad.h"
#include "CElevatorRoadEnd.h"

#include "CSceneManager.h"
#include "CSceneGame.h"
#include "CSceneEnding.h"

#include "CClearComponent.h"
#include "CEditorComponent.h"


#include "Sound.h"


//===== マクロ定義 =====


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CEndElevatorComponent::Start()
{
	m_pParent->m_pTrans->m_pos->y -= 50;
	m_pParent->m_pTrans->m_pos->z = 150;

	m_startPos = m_pParent->m_pTrans->m_pos;
	m_goalPos = m_startPos + Vector3{ 0, 1000, 0 };

	m_pParent->m_pTrans->m_pos = m_startPos;

	m_curState = eWait;

	m_rb = m_pParent->GetComponent<CRigidbody2D>();

	m_render = m_pParent->GetComponent<CAssimpRenderer>();
	const auto& r = m_render.lock();
	if (r)
	{
		r->SetAnimIndex(2);
	}

	// エレベーターの通路モデルを生成
	Vector3 offset = { 0,90,0 };
	Vector3 evPos = m_startPos + offset;

	// エレベーターの扉部分
	m_aEVRoad.push_back(CObjectManager::CreateObject<CElevatorRoadEnd>(evPos));

	// エレベーターの通路部分
	for (int i = 0; i < 10; i++)
	{
		evPos->y += 200;
		m_aEVRoad.push_back(CObjectManager::CreateObject<CElevatorRoad>(evPos));
	}

}


//===================================
//
//	更新関数
//
//===================================
void CEndElevatorComponent::Update()
{
	switch (m_curState)
	{
		// プレイヤーを待機
	case CEndElevatorComponent::eWait:

		break;

		// ドアの前まで移動する
	case CEndElevatorComponent::eGaolPosMove:
	{
		// 確認
		const auto& player = m_player.lock();
		if (!player) return;
		const auto& rb = player->GetComponent<CRigidbody2D>();
		const auto& action = player->GetComponent<CPlayerActionComponent>();
		if (!rb && !action) return;

		// プレイヤーとドアのベクトル
		Vector3 vec = m_pParent->m_pTrans->m_pos - player->m_pTrans->m_pos;
		vec->y = 0.0f;
		vec->z = 0.0f;

		// 距離が離れている
		if (vec.magnitude() > 1.1f)
		{
			// ドアの前まで移動
			action->FreeMove(vec.normalized() * 1.5f);
			//CSound::StopBGM("BGM.mp3");
		}
		else
		{
			// 到着
			player->m_pTrans->m_pos->x = m_pParent->m_pTrans->m_pos->x;
			action->StopMove();

			// 状態移動
			m_curState = eClearMove;

			// プレイヤーの回転
			if(player->m_pTrans->m_rot->y >= 0.0f)	rb->SetTorqueY( 1.0f);
			else									rb->SetTorqueY(-1.0f);
		}
	}
		break;

		// クリアの喜びモーション
	case CEndElevatorComponent::eClearMove:
	{
		// 確認
		const auto& player = m_player.lock();
		if (!player) return;
		const auto& rb = player->GetComponent<CRigidbody2D>();
		if (!rb) return;

		// プレイヤーの方向
		if (std::fabsf(player->m_pTrans->m_rot->y) >= 180.0f)
		{
			// 回転終了
			rb->SetTorque({ 0,0,0 });
			player->m_pTrans->m_rot->y = 180.0f;

			// 状態移動
			m_curState = eOpenDoor;

			// アニメーション再生
			// ドアを開く
			m_render.lock()->SetAnimIndex(0);
			m_render.lock()->PlayAnim();

			// サウンド
			CSound::PlaySE("Elevator.mp3");
		}
	}
		break;

		// ドアを開く
	case CEndElevatorComponent::eOpenDoor:

		break;

		// 中に入る
	case CEndElevatorComponent::eInMove:
	{
		// 確認
		const auto& player = m_player.lock();
		if (!player) return;
		const auto& rb = player->GetComponent<CPlayerActionComponent>();
		if (!rb) return;


		if (player->m_pTrans->m_pos->z < m_pParent->m_pTrans->m_pos->z)
		{
			// 移動させる
			rb->BackMove();
		}
		else
		{
			// 到着した
			rb->StopMove();
			player->m_pTrans->m_pos->z = m_pParent->m_pTrans->m_pos->z;

			// アニメーション再生
			m_render.lock()->SetAnimIndex(1);
			m_render.lock()->PlayAnim();

			// 階層
			const auto& obj = CObjectManager::CreateObject<CObject>();
			obj->AddComponent<CClearComponent>();

			// 状態移動
			m_curState = eCloseDoor;
		}
	}
		break;

		// ドアを閉める
	case CEndElevatorComponent::eCloseDoor:
		break;

		// 上に動いていく
	case CEndElevatorComponent::eEndMove:
		// 移動
		m_rb.lock()->SetForceY(4.0f);

		// 到着したら
		if (m_pParent->m_pTrans->m_pos->y > m_goalPos->y)
		{
			// ストップ
			//m_pParent->m_pTrans->m_pos = m_goalPos;
			//m_rb.lock()->SetForceY(0.0f);

			// 状態移動
			m_curState = eMaxState;

			if (CEditorComponent::GetCurrentStageNum() < CEditorComponent::GetMaxStateNum())
			{
				// シーン遷移
				CSceneManager::Get()->SetNextScene<CSceneGame>();
			}
			else
			{
				CSceneManager::Get()->SetNextScene<CSceneEnding>();
			}

			// サウンド
			CSound::PlaySE("fadeout.mp3");
			CSound::StopSE("ElevaterUpdown.mp3");
		}
		break;

		// 特になし
	case CEndElevatorComponent::eMaxState:
		break;
	}
}


//===================================
//
//	後更新関数
//
//===================================
void CEndElevatorComponent::LateUpdate()
{

}


//===================================
//
//	終了関数
//
//===================================
void CEndElevatorComponent::End()
{
	for (const auto& evRoad : m_aEVRoad)
	{
		const auto& ev_s = evRoad.lock();
		if (ev_s)
			CObjectManager::DestroyObject(ev_s);
	}
}


//===== 使わない時は消してください() =====
//==================================================
//
//	2D当たり判定(Enter) コールバック関数
//
//==================================================
void CEndElevatorComponent::OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// タグなどで判定
	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		if (m_curState == eWait)
		{
			// プレイヤー取得
			m_player = collsion2d->GetParent()->tish();

			// 安全処理
			const auto& player = m_player.lock();
			if (!player) return;

			// カメラ同期
			CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);

			// コンポーネントの削除
			player->RemoveComponet<CPlayerInput>();

			// 状態移動
			m_curState = eGaolPosMove;

		}
	}
}


//============================================
//
//	3Dアニメーション終了時コールバック関数
//
//============================================
void CEndElevatorComponent::OnAnimationExit3D(const std::shared_ptr <CAssimpRenderer>& renderer)
{
	// 扉が開き終わった
	if (renderer->GetCurrentAnimIndex() == 0)
	{
		if (m_curState == eOpenDoor)
		{
			m_curState = eInMove;
			renderer->StopAnim();
		}
	}
	// 扉が閉じ終わった
	else if (renderer->GetCurrentAnimIndex() == 1)
	{
		if (m_curState == eCloseDoor)
		{
			const auto& player = m_player.lock();
			if (!player) return;

			renderer->StopAnim();

			// 状態移動
			m_curState = eEndMove;

			// サウンド
			CSound::PlaySE("ElevaterUpdown.mp3");

		}
	}
}