//======================================================================
//											CStandStickComponent.cpp
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
#include "CStandStickComponent.h"
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
void CStandStickComponent::Start()
{
	m_col = m_pParent->GetComponent<CCollision2D>();
	m_bGrandflg = false;
}


//===================================
//
//	更新関数
//
//===================================
void CStandStickComponent::Update()
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
void CStandStickComponent::LateUpdate()
{

}

void CStandStickComponent::SetStandStick(std::weak_ptr<CEnemyBase> enemy)
{
	m_enemy = enemy;
	m_pParent->m_pTrans->SetPos(m_enemy.lock()->m_pParent->m_pTrans->GetPos());
}

bool CStandStickComponent::GetGrandflg()
{
	return m_bGrandflg;
}

//===== 使わない時は消してください() =====
//===================================
//
//	2D当たり判定(Stay) コールバック関数
//
//===================================

void CStandStickComponent::OnCollisionStay2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	if (collsion2d->GetParent()->GetTag() != "SecurityRobo")
	{
		m_bGrandflg = true;
	}
}

void CStandStickComponent::OnCollisionExit2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	m_bGrandflg = false;
}
