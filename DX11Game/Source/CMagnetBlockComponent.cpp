//======================================================================
//											CMagnetBlockComponent.h
//	電磁床コンポーネント
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/12/01	
//
//======================================================================

//====== インクルード部 ======
#include "CObject.h"
#include "CMagnetBlockComponent.h"
#include "CEnemyBase.h"

//===== マクロ定義 =====
#define PLAYER_SPEED	4.0f

//===== プロトタイプ宣言 =====


//===== グローバル変数 =====
std::weak_ptr<CMagnetBlockComponent> CMagnetBlockComponent::g_mainMagnet;
int CMagnetBlockComponent::m_nMaxNum = 0;
int CMagnetBlockComponent::m_nCurrNum = 0;

//===================================
//
//	スタート関数
//
//===================================
void CMagnetBlockComponent::Start()
{
	CElecGimmickBase::Start();

	m_fUseResource = 0.075f;
	m_player = CObjectManager::SearchObjectName("Player").lock()->tish();

	m_nNum = m_nMaxNum;
	m_nMaxNum++;

	m_energyOffTexture = "data/model/Magnet/Magnet.png";
	m_energyOnTexture = "data/model/Magnet/Magnet_Change.png";

	// コントローラー数
	m_nJoyCount = GetJoyCount();
}

//===================================
//
//	レスポーン
//
//===================================
void CMagnetBlockComponent::Restart()
{
	m_fEnergyBulletResource = 0.0f;
}

//===================================
//
//	ギミック更新関数
//
//===================================
void CMagnetBlockComponent::GimmickUpdate()
{

}

//===================================
//
//	エンド関数
//
//===================================
void CMagnetBlockComponent::End()
{
	m_nMaxNum--;
	CObjectManager::DestroyObject(m_pParent);
}


//===================================
//
//	帯電侵入関数
//
//===================================
void CMagnetBlockComponent::ChargePlayerEnter(std::weak_ptr<CCollision2D> collsion2d)
{
	ChargePlayerStay(collsion2d);
}

//===================================
//
//	帯電滞在関数
//
//===================================
void CMagnetBlockComponent::ChargePlayerStay(std::weak_ptr<CCollision2D> collsion2d)
{
	m_fEnergyBulletResource = 4.0f;

	EnergyOn();
}

//===================================
//
//	帯電撤退関数
//
//===================================
void CMagnetBlockComponent::ChargePlayerExit(std::weak_ptr<CCollision2D> collsion2d)
{
	
}

//===================================
//
//	電気弾稼働関数
//
//===================================
void CMagnetBlockComponent::EnergyOn()
{
	if (m_nCurrNum != m_nNum)
	{
		m_fEnergyBulletResource = 0;
		return;
	}

	const auto& player = m_player.lock();
	if (!player) return;
	const auto& rb = player->GetComponent<CRigidbody2D>();
	if (!rb) return;

	Vector3 vec = m_pParent->m_pTrans->m_pos - player->m_pTrans->m_pos;
	rb->SetForce(vec / 40);

	// コントローラー
	float JoyY = -(float)GetJoyY(JOYSTICKID1);
	JoyY /= 32767.0f;
	if (m_nJoyCount <= 0) JoyY = 1.0f;

	// 停止
	float speed = 1.0f;
	if (GetJoyButton(JOYSTICKID1, JOYBUTTON_LB) ||
		GetJoyButton(JOYSTICKID1, JOYBUTTON_RB))
	{
		speed = 0.0f;
	}

	if (GetKeyPress(VK_UP) || (m_nJoyCount > 0 && JoyY > 0.4f))
	{
		rb->AddForceY(PLAYER_SPEED * fabsf(JoyY) * speed);
	}
	if (GetKeyPress(VK_DOWN) || (m_nJoyCount > 0 && JoyY < -0.4f))
	{
		rb->AddForceY(-PLAYER_SPEED * fabsf(JoyY) * speed);
	}
}

//===================================
//
//	電気弾休止関数
//
//===================================
void CMagnetBlockComponent::EnergyOff()
{

}

void CMagnetBlockComponent::OnCollisionEnter2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	if (collsion2d->GetParent()->GetTag() == "Player" ||
		collsion2d->GetParent()->GetTag() == "EnergyBullet")
	{
		m_nCurrNum = m_nNum;
		if (collsion2d->GetParent()->GetTag() == "EnergyBullet")
		{
			CSound::PlaySE("MagnetOn.wav");
		}
	}

	CElecGimmickBase::OnCollisionEnter2D(collsion2d);
}

void CMagnetBlockComponent::OnCollisionStay2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	CElecGimmickBase::OnCollisionStay2D(collsion2d);

	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		if (m_fEnergyBulletResource >= 6.0f)
		{
			m_fEnergyBulletResource = 5.0f;
		}
	}
	else if (collsion2d->GetParent()->GetTag() == "EnergyBullet")
	{
		m_nCurrNum = m_nNum;
	}
}
