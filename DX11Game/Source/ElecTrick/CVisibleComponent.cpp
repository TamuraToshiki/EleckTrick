//======================================================================
//											CVisibleComponent.cpp
//	可視範囲コンポーネント
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/12/04	作成
//		 12/05	完成
//
//======================================================================


//===== インクルード部 =====
#include "CVisibleComponent.h"
#include "CCollision2D.h"
#include "CEnemyBase.h"

//===== マクロ定義 =====


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===== 列挙体宣言 =====


//===================================
//
//	スタート関数
//
//===================================
void CVisibleComponent::Start()
{
	m_col = m_pParent->GetComponent<CCollision2D>();
	m_bVisibleflg = false;
	m_bTouchflg = false;
}


//===================================
//
//	更新関数
//
//===================================
void CVisibleComponent::Update()
{
	m_bTouchflg = false;

	const auto& enemy = m_enemy.lock();
	if (!enemy)return;

	m_pParent->m_pTrans->SetPos(enemy->m_pParent->m_pTrans->GetPos());
}


//===================================
//
//	後更新関数
//
//===================================
void CVisibleComponent::LateUpdate()
{

}

//===================================
//
//	大きさ&方向変更関数
//
//===================================
void CVisibleComponent::SetTargetEnemy(std::weak_ptr<CEnemyBase> enemy ,Vector3 scale, Vector3 offset)
{
	m_enemy = enemy;
	m_col.lock()->SetScale(scale);
	m_col.lock()->SetOffSet(offset);

	m_pParent->m_pTrans->SetPos(m_enemy.lock()->m_pParent->m_pTrans->GetPos());
}

//===================================
//
//	方向変更関数
//
//===================================
void CVisibleComponent::SetDir(Vector3 offset)
{
	m_col.lock()->SetOffSet(offset);
}

Vector3 CVisibleComponent::GetDir()
{
	return m_col.lock()->GetOffSet();
}

//===================================
//
//	当たり判定取得関数
//
//===================================
bool CVisibleComponent::GetVisibleflg()
{
	return m_bVisibleflg;
}

//===== 使わない時は消してください() =====
//===================================
//
//	2D当たり判定(Enter) コールバック関数
//
//===================================
void CVisibleComponent::OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// タグなどで判定
	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		m_bVisibleflg = true;
		m_bTouchflg = true;
		m_enemy.lock()->SetPlayer(collsion2d->GetParent()->tish());
	}
	if (!m_bTouchflg)
	{
		OnCollisionExit2D(collsion2d);
	}
}

void CVisibleComponent::OnCollisionExit2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// タグなどで判定
	//if (collsion2d->GetParent()->GetTag() == "Player")
	//{
		m_bVisibleflg = false;
	//}
}
