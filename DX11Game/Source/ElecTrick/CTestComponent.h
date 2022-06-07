//======================================================================
//											CTestComponent.h
//	デバックテスト用コンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/11/16	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CObjectManager.h"
#include "input.h"

#include "CPlayer.h"
#include "CEditorObject.h"
#include "CEditorComponent.h"


//===== クラス定義 =====
class CTestComponent final : public CComponent
{
private:
	// --- 変数宣言 ---
	std::weak_ptr<CObject> m_player;
	std::weak_ptr<CObject> m_editor;

	bool m_bEditFlg;

public:
	// --- 関数宣言 ---

	// 作成時に呼ばれます
	void Start() override
	{
		m_bEditFlg = true;

		m_player = CObjectManager::CreateObject<CPlayer>({1200, 1200, 0});
		m_editor = CObjectManager::CreateObject<CEditorObject>({ 1200, 1200, 0 });

		const auto& player = m_player.lock();
		if (player)
		{
			player->SetStop(!m_bEditFlg);
			//CCamera::GetMainCamera()->SetCameraTarget(player->m_pTrans);
		}

		const auto& edit = m_editor.lock();
		if (edit)
		{
			edit->SetStop(m_bEditFlg);
			edit->GetComponent<CAssimpRenderer>()->SetDiffuseColor({ 1, 1, 1, 0 });
		}
	}

	// 毎フレーム呼ばれます
	void Update() override
	{

	}

	// 毎フレームの更新後に呼ばれます
	void LateUpdate() override
	{
		if (GetKeyTrigger(VK_P))
		{
			m_bEditFlg ^= 1;

			const auto& edit = m_editor.lock();
			const auto& player = m_player.lock();
			if (player && edit)
			{
				player->SetStop(!m_bEditFlg);
				if (!m_bEditFlg)
				{
					CCamera::GetMainCamera()->SetCameraTarget(player->m_pTrans);
					edit->GetComponent<CAssimpRenderer>()->SetDiffuseColor({ 1, 1, 1, 1 });
				}

				edit->SetStop(m_bEditFlg);
				if (m_bEditFlg)
				{
					CCamera::GetMainCamera()->SetCameraTarget(edit->m_pTrans);
					edit->m_pTrans->m_pos = player->m_pTrans->m_pos;
					edit->GetComponent<CAssimpRenderer>()->SetDiffuseColor({ 1, 1, 1, 0 });
					// 勝手に戻す
					edit->GetComponent<CEditorComponent>()->Reset();
				}
			}
		}
	}


	void End() override
	{

	}

	//===== 使わない時は消してください() =====
	//--- コールバック関数 ---
	// 2D当たり判定
	void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override
	{
		// タグなどで判定
		if (collsion2d->m_pParent->GetTag() == "CBock")
		{

		}
	}

	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override
	{

	}

	void OnCollisionExit2D(const std::shared_ptr <CCollision2D>& collsion2d) override
	{

	}

};



