//======================================================================
//											CEnemyBase.cpp
//	ギミックベースレイアウト
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/12/01
//
//======================================================================

//　インクルード部
#include "CEnemyBase.h"
#include "CVisible.h"
#include "CStandStick.h"
#include "CBlocksensor.h"
#include "Sound.h"


// 初期化関数
void CEnemyBase::Start()
{
	CElecGimmickBase::Start();

	m_fUseResource = 0.0;
	m_bFindflg = false;
	m_bStandflg = true;
	m_bOutageflg = false;
	const auto& player = CObjectManager::SearchObjectName("Player").lock();
	if (player)
	{
		m_res = player->GetComponent<CRespawnComponent>();
	}
	m_rb = m_pParent->GetComponent<CRigidbody2D>();
	const auto& visible = CObjectManager::CreateObject<CVisible>();
	m_visi = visible->GetComponent<CVisibleComponent>();
	const auto& stand = CObjectManager::CreateObject<CStandStick>();
	m_stand = stand->GetComponent<CStandStickComponent>();
	const auto& block = CObjectManager::CreateObject<CBlocksensor>();
	m_blsen = block->GetComponent<CBlocksensorComponent>();
}

// 更新関数
void CEnemyBase::GimmickUpdate()
{
	m_bFindflg = m_visi.lock()->GetVisibleflg();
	m_bStandflg = m_stand.lock()->GetGrandflg();
	m_bBlockflg = m_blsen.lock()->GetWallflg();

	if (!m_bOutageflg)
	{
		// 発見したかどうか
		if (m_bFindflg)
		{
			// 交戦
			EngagementEnemy();
		}
		else
		{
			// 通常
			NormalEnemy();
		}
	}
}

// 最終更新
void CEnemyBase::LateUpdate()
{
}

void CEnemyBase::End()
{
	const auto& visi = m_visi.lock();
	if (visi)
	{
		CObjectManager::DestroyObject(visi->m_pParent->tish().lock());

	}

	const auto& stick = m_stand.lock();
	if (stick)
	{
		CObjectManager::DestroyObject(stick->m_pParent->tish().lock());
	}

	const auto& sensor = m_blsen.lock();
	if (sensor)
	{
		CObjectManager::DestroyObject(sensor->m_pParent->tish().lock());
	}
}

// 帯電侵入関数
void CEnemyBase::ChargePlayerEnter(std::weak_ptr<CCollision2D> collsion2d)
{
	
}

// 帯電滞在関数
void CEnemyBase::ChargePlayerStay(std::weak_ptr<CCollision2D> collsion2d)
{
}

// 帯電撤退関数
void CEnemyBase::ChargePlayerExit(std::weak_ptr<CCollision2D> collsion2d)
{
}

// 電気弾稼働関数
void CEnemyBase::EnergyOn()
{
	m_bOutageflg = true;
	Outage();
}

// 電気弾休止関数
void CEnemyBase::EnergyOff()
{
	m_bOutageflg = false;
	m_fUseResource = 0.0;
}

void CEnemyBase::SetPlayer(std::weak_ptr<CObject> player)
{
	const auto& play = player.lock();
	if (!play)return;
	m_player = player;
}

void CEnemyBase::OnCollisionStay2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	CElecGimmickBase::OnCollisionStay2D(collsion2d);

	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		if (!m_bOutageflg)
		{
			const auto& res = m_res.lock();
			if (!res) return;
			res->Respawn();
		}
	}

	if (collsion2d->GetParent()->GetTag() == "EnergyBullet")
	{
		CSound::PlaySE("HitSound.mp3");
	}
}

void CEnemyBase::OnCollisionEnter2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	CElecGimmickBase::OnCollisionEnter2D(collsion2d);

	if (collsion2d->GetParent()->GetTag() == "EnergyBullet")
	{
		CSound::PlaySE("HitSound.mp3");
	}
}