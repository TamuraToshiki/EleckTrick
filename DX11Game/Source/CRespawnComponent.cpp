//======================================================================
//											CRespawnComponent.cpp
//	コンポーネント作成用のベースレイアウト
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/11/16	作成
//
//======================================================================


//===== インクルード部 =====
#include "CRespawnComponent.h"
#include "CSceneManager.h"
#include "Camera.h"

#include "Sound.h"
#include "CPlayerInput.h"


//===== マクロ定義 =====


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CRespawnComponent::Start()
{
	RespawnPoint = Vector3{ 600, 1500, 0 };			// 初期リスポーンポイント
	//m_pi = m_pParent->GetComponent<CPlayerInput>();
	//m_pr = m_pParent->GetComponent<CResourceEnergy>();

	m_bNowFade = false;
}

//===================================
//
//	更新関数
//
//===================================
void CRespawnComponent::Update()
{

}


//===================================
//
//	後更新関数
//
//===================================
void CRespawnComponent::LateUpdate()
{
	// リスポーン条件
	if (m_pParent->m_pTrans->m_pos->y <= 0.1f)
	{
		Respawn();
	}
}

void CRespawnComponent::SetRespawnPoint(Vector3 pos)
{
	RespawnPoint = pos;
	RespawnPoint->z = 0.0f;
}

void CRespawnComponent::Respawn()
{
	if (!m_bNowFade && !CSceneManager::Get()->GetNowFade())
	{
		// サウンド
		CSound::PlaySE("Respawn1.mp3");

		// フェードフラグ
		m_bNowFade = true;

		// プレイヤーのストップ
		//m_pParent->SetStop(true);
		m_pParent->RemoveComponet<CPlayerInput>();
		m_pParent->GetComponent<CRigidbody2D>()->SetForce({ 0,0,0 });
		m_pParent->GetComponent<CRigidbody2D>()->SetUseGravity(false);
		m_pParent->GetComponent<CRigidbody2D>()->SetSolid(true);

		CSceneManager::Get()->StartFade([this]() {
			// 一括初期化の仕方が分からなかったため、最低限のプレイヤーとリソースの初期化
			/*const auto& pi = m_pi.lock();
			if (pi)
				pi->Start();
			const auto& pr = m_pr.lock();
			if (pr)
				pr->Start();*/
			// 初期化したあとに、プレイヤーの位置をリスポーンポイントに変更する
			m_pParent->m_pTrans->m_pos = RespawnPoint;

			// カメラの更新
			CCamera::GetMainCamera()->UpdateCameraPos(50);

			// フェードフラグ
			m_bNowFade = false;

			// ギミックの初期化
			m_edit = CObjectManager::SearchObjectTag("Editor").lock()->GetComponent<CEditorComponent>();
			m_edit.lock()->GimmickRestart();

			// プレイヤーのストップ
			//m_pParent->SetStop(false);
			if (!m_pParent->GetComponent<CPlayerInput>())
			{
				m_pParent->AddComponent<CPlayerInput>();
			}
			m_pParent->GetComponent<CRigidbody2D>()->SetUseGravity(true);
			m_pParent->GetComponent<CRigidbody2D>()->SetSolid(false);
			});
	}
}
