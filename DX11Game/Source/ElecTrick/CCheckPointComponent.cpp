//======================================================================
//											CCheckPointComponent.cpp
//	チェックポイントコンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/15	作成
//
//======================================================================

//====== インクルード部 ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CCollision2D.h"
#include "CCheckPointComponent.h"
#include "CObjectManager.h"


#include "Sound.h"


//===== マクロ定義 =====


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CCheckPointComponent::Start()
{
	m_bOn = false;
	m_renderer = m_pParent->GetComponent<CAssimpRenderer>();

	// プレイヤーのリスポーン
	const auto& player = CObjectManager::SearchObjectTag("Player");
	if (!player.expired())
	{
		m_respawn = player.lock()->GetComponent<CRespawnComponent>();
	}

	m_energyOffTexture = "data/model/Resporn/Resporn_Tex10.png";
	m_energyOnTexture = "data/model/Resporn/Resporn_Tex_Change.png";

}

//===================================
//
//	ギミック更新関数
//
//===================================
void CCheckPointComponent::GimmickUpdate()
{
	if (m_bOn)
	{
		m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseTexture(m_energyOnTexture.c_str());
	}
	else
	{
		m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseTexture(m_energyOffTexture.c_str());
	}
}

//===================================
//
//	帯電侵入関数
//
//===================================
void CCheckPointComponent::ChargePlayerEnter(std::weak_ptr<CCollision2D> collsion2d)
{
	EnergyOn();
}

//===================================
//
//	帯電滞在関数
//
//===================================
void CCheckPointComponent::ChargePlayerStay(std::weak_ptr<CCollision2D> collsion2d)
{
	EnergyOn();
}

//===================================
//
//	帯電撤退関数
//
//===================================
void CCheckPointComponent::ChargePlayerExit(std::weak_ptr<CCollision2D> collsion2d)
{

}

//===================================
//
//	電気弾稼働関数
//
//===================================
void CCheckPointComponent::EnergyOn()
{
	if (!m_bOn)
	{
		// レスポーンをセット
		if (!m_respawn.expired())
		{
			m_respawn.lock()->SetRespawnPoint(m_pParent->m_pTrans->m_pos);
		}
		m_bOn = true;

		// サウンド
		CSound::PlaySE("CheckPointOn.wav");

		// エフェクトの生成
		Vector3 pos = m_pParent->m_pTrans->m_pos;
		pos->y += 100;
		Vector3 scale = { 300,300,300 };
		const auto& obj = CObjectManager::CreateObject<CObject>(pos);
		obj->AddComponent<CEffectComponent>()->SetEffect(CEffectComponent::eSaveEffect);

		obj->m_pTrans->m_scale = scale;
	}
}

//===================================
//
//	電気弾休止関数
//
//===================================
void CCheckPointComponent::EnergyOff()
{

}