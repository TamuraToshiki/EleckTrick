//======================================================================
//											CBlocksensorComponent.cpp
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
#include "CBlocksensorComponent.h"
#include "CCollision2D.h"
#include "CEnemyBase.h"

//===== マクロ定義 =====


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CBlocksensorComponent::Start()
{
	m_col = m_pParent->GetComponent<CCollision2D>();
	m_bWallflg = false;
}


//===================================
//
//	更新関数
//
//===================================
void CBlocksensorComponent::Update()
{
	const auto& enemy = m_enemy.lock();
	if (!enemy)return;

	m_pParent->m_pTrans->SetPos(enemy->m_pParent->m_pTrans->GetPos());
}


//===================================
//
//	後更新関数
//
//===================================
void CBlocksensorComponent::LateUpdate()
{

}

void CBlocksensorComponent::SetBlocksensorComponent(std::weak_ptr<CEnemyBase> enemy)
{
	m_enemy = enemy;
	m_pParent->m_pTrans->SetPos(m_enemy.lock()->m_pParent->m_pTrans->GetPos());
}

bool CBlocksensorComponent::GetWallflg()
{
	return m_bWallflg;
}

//===================================
//
//	方向変更関数
//
//===================================
void CBlocksensorComponent::SetDir(Vector3 offset)
{
	m_col.lock()->SetOffSet(offset);
}

Vector3 CBlocksensorComponent::GetDir()
{
	return m_col.lock()->GetOffSet();
}

//===== 使わない時は消してください() =====
//===================================
//
//	2D当たり判定(Stay) コールバック関数
//
//===================================

void CBlocksensorComponent::OnCollisionStay2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	if (collsion2d->GetParent()->GetTag() != "Player")
	{
		if (collsion2d->GetParent()->GetTag() != "EnergyBullet")
		{
			if (collsion2d->GetParent()->GetTag() != "Visible")
			{
				m_bWallflg = true;
			}
		}
	}
}

void CBlocksensorComponent::OnCollisionExit2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	m_bWallflg = false;
}
