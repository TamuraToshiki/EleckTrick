//======================================================================
//											CEnemyBase.cpp
//	�M�~�b�N�x�[�X���C�A�E�g
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/12/01
//
//======================================================================

//�@�C���N���[�h��
#include "CEnemyBase.h"
#include "CVisible.h"
#include "CStandStick.h"
#include "CBlocksensor.h"
#include "Sound.h"


// �������֐�
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

// �X�V�֐�
void CEnemyBase::GimmickUpdate()
{
	m_bFindflg = m_visi.lock()->GetVisibleflg();
	m_bStandflg = m_stand.lock()->GetGrandflg();
	m_bBlockflg = m_blsen.lock()->GetWallflg();

	if (!m_bOutageflg)
	{
		// �����������ǂ���
		if (m_bFindflg)
		{
			// ���
			EngagementEnemy();
		}
		else
		{
			// �ʏ�
			NormalEnemy();
		}
	}
}

// �ŏI�X�V
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

// �ѓd�N���֐�
void CEnemyBase::ChargePlayerEnter(std::weak_ptr<CCollision2D> collsion2d)
{
	
}

// �ѓd�؍݊֐�
void CEnemyBase::ChargePlayerStay(std::weak_ptr<CCollision2D> collsion2d)
{
}

// �ѓd�P�ފ֐�
void CEnemyBase::ChargePlayerExit(std::weak_ptr<CCollision2D> collsion2d)
{
}

// �d�C�e�ғ��֐�
void CEnemyBase::EnergyOn()
{
	m_bOutageflg = true;
	Outage();
}

// �d�C�e�x�~�֐�
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