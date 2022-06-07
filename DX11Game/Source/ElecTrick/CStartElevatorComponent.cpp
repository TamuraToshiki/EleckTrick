//======================================================================
//											CStartElevatorComponent.cpp
//	スタートエレベーターコンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/01	作成
//
//======================================================================


//===== インクルード部 =====
#include "CStartElevatorComponent.h"
#include "CPlayer.h"
#include "Camera.h"
#include "CObjectManager.h"
#include "input.h"

#include "CPlayerInput.h"
#include "CPlayerActionComponent.h"
#include "CRespawnComponent.h"

#include "CElevatorRoad.h"
#include "CElevatorRoadEnd.h"

#include "CHierarchyComponent.h"

#include "Sound.h"
#include "CEffectComponent.h"



//===== マクロ定義 =====
#define EV_CAMERA_OFFSET (225)
#define PLAYER_CAMERA_OFFSET (150)

//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CStartElevatorComponent::Start()
{
	m_pParent->m_pTrans->m_pos->y -= 50;
	m_pParent->m_pTrans->m_pos->z = 150;


	m_goalPos = m_pParent->m_pTrans->m_pos;
	m_startPos = m_goalPos - Vector3{ 0, 1000, 0 };

	m_pParent->m_pTrans->m_pos = m_startPos;

	m_rb = m_pParent->GetComponent<CRigidbody2D>();
	m_rb.lock()->SetMass(3);

	m_render = m_pParent->GetComponent<CAssimpRenderer>();
	const auto& r = m_render.lock();
	if (r)
	{
		r->SetAnimIndex(2);
	}

	// プレイヤーの取得
	m_player = CObjectManager::SearchObjectName("Player");
	const auto& player = m_player.lock();
	if (player)
	{
		player->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
		player->RemoveComponet<CPlayerInput>();

		// リスポーンポイントの登録
		player->GetComponent<CRespawnComponent>()->SetRespawnPoint(m_goalPos);
		player->GetComponent<CRigidbody2D>()->SetTrigger(true);

		// プレイヤーがストップじゃない
		if (!player->GetStop())
		{
			// カメラ同期
			CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);
			CCamera::GetMainCamera()->SetOffSet({ 0, EV_CAMERA_OFFSET, 0 });
			CCamera::GetMainCamera()->SetLengthOffset(200);
			CCamera::GetMainCamera()->UpdateCameraPos(100);
			m_curState = eStartMove;
		}
		else
		{
			// 待機
			m_curState = eWait;
		}
	}

	// エレベーターの通路モデルを生成
	Vector3 offset = { 0,90,0 };
	Vector3 evPos = m_goalPos + offset;

	// エレベーターの扉部分
	m_aEVRoad.push_back( CObjectManager::CreateObject<CElevatorRoadEnd>(evPos));

	// エレベーターの通路部分
	for (int i = 0; i < 10; i++)
	{
		evPos->y -= 200;
		m_aEVRoad.push_back(CObjectManager::CreateObject<CElevatorRoad>(evPos));
	}
	
	// サウンド
	CSound::PlaySE("ElevaterUpdown.mp3");
}


//===================================
//
//	更新関数
//
//===================================
void CStartElevatorComponent::Update()
{
	switch (m_curState)
	{
		// プレイヤーの待機
	case CStartElevatorComponent::eWait:
	{
		const auto& player = m_player.lock();
		if (player)
		{
			if (!player->GetStop())
			{
				// カメラ同期
				CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);
				CCamera::GetMainCamera()->SetOffSet({ 0, EV_CAMERA_OFFSET, 0 });
				CCamera::GetMainCamera()->UpdateCameraPos(100);
				m_curState = eStartMove;
			}
		}
	}
		break;

		// エレベーターのスタート演出
	case CStartElevatorComponent::eStartMove:
		// 移動
		m_rb.lock()->SetForceY(4.0f);
		// サウンド
		//if (!CSound::IsPlaying("ElevaterUpdown.mp3"))
		//{
		//	CSound::PlaySE("ElevaterUpdown.mp3");
		//}
		if (m_player.lock())
		{
			m_player.lock()->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
		}

		// 到着したら
		if (m_goalPos->y < m_pParent->m_pTrans->m_pos->y)
		{
			m_pParent->m_pTrans->m_pos = m_goalPos;

			// プレイヤーの生成
			//m_player = CObjectManager::CreateObject<CPlayer>(m_pParent->m_pTrans->m_pos);
			if (m_player.lock())
			{
				m_player.lock()->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
				m_player.lock()->GetComponent<CRigidbody2D>()->SetTrigger(false);

				//m_player.lock()->RemoveComponet<CPlayerInput>();
			}

			// 状態移動
			m_curState = eOpenDoor;
			m_rb.lock()->SetForceY(0.0f);

			// アニメーション再生
			m_render.lock()->SetAnimIndex(0);
			m_render.lock()->PlayAnim();

			// 階層
			const auto& obj = CObjectManager::CreateObject<CObject>();
			obj->AddComponent<CHierarchyComponent>();

			// サウンド
			CSound::StopSE("ElevaterUpdown.mp3");
			CSound::PlaySE("Elevator.mp3");
		}


		break;
	case CStartElevatorComponent::eOpenDoor:

		break;
	case CStartElevatorComponent::eOutMove:
	{
		// プレイヤーの移動
		const auto& player = m_player.lock();
		if (!player) return;

		const auto& rb = player->GetComponent<CPlayerActionComponent>();
		if (!rb) return;

		if (player->m_pTrans->m_pos->z > 0.0f)
		{
			// 移動させる
			rb->FrontMove();
		}
		else
		{
			// 到着した
			rb->StopMove();

			// アニメーション再生
			m_render.lock()->SetAnimIndex(1);
			m_render.lock()->PlayAnim();

			// 状態移動
			m_curState = eCloseDoor;
		}
	}
		break;
	case CStartElevatorComponent::eCloseDoor:


		break;
	case CStartElevatorComponent::eMaxState:
		break;
	}
}


//===================================
//
//	後更新関数
//
//===================================
void CStartElevatorComponent::LateUpdate()
{
	switch (m_curState)
	{
		// エレベーターのスタート演出
	case CStartElevatorComponent::eStartMove:
	{
		const auto& player = m_player.lock();
		if (player)
		{
			player->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
		}
		CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);
	}
	break;

	case CStartElevatorComponent::eOpenDoor:
	{
		const auto& player = m_player.lock();
		if (player)
		{
			player->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
		}
		CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);
	}
	break;

	case CStartElevatorComponent::eOutMove:
		CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);
		break;

	}
}


//===================================
//
//	終了関数
//
//===================================
void CStartElevatorComponent::End()
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
void CStartElevatorComponent::OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// タグなどで判定
	if (collsion2d)
	{

	}
}


//============================================
//
//	3Dアニメーション終了時コールバック関数
//
//============================================
void CStartElevatorComponent::OnAnimationExit3D(const std::shared_ptr <CAssimpRenderer>& renderer)
{
	// 扉が開き終わった
	if (renderer->GetCurrentAnimIndex() == 0)
	{
		if (m_curState == eOpenDoor)
		{
			m_curState = eOutMove;
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

			// コンポーネントを追加
			if (!player->GetComponent<CPlayerInput>())
			{
				player->AddComponent<CPlayerInput>();
			}
			player->SetStop(false);
			CCamera::GetMainCamera()->SetOffSet({ 0, PLAYER_CAMERA_OFFSET, 0 });

			// 状態移動
			m_curState = eMaxState;
		}
	}
}