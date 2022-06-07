//======================================================================
//											CDamageTrapCollisionComponent.cpp
//	ダメージトラップ
//
//======================================================================
//	author :	AT12A 15 榊原龍我
//======================================================================
//	開発履歴
//	2020/12/13	作成
//
//======================================================================


//===== インクルード部 =====
#include "CDamageTrapCollisionComponent.h"
#include "CDamegeTrapPoint.h"

//===== マクロ定義 =====


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CDamageTrapCollisionComponent::Start()
{
	m_col = m_pParent->GetComponent<CCollision2D>();
	m_res = CObjectManager::SearchObjectName("Player").lock()->GetComponent<CRespawnComponent>();

	m_pParent->m_pTrans->SetRotation(m_pParent->m_pTrans->GetRotation());
	// オブジェクトの大きさを変更　※これだと斜めの時に大きくなるので後で変更
	m_pParent->m_pTrans->SetScale({ (m_pParent->m_pTrans->GetScale()->x * 2),m_pParent->m_pTrans->GetScale()->y * 2,100 });
	// 当たり判定の大きさ変更　※上記同様、斜めだと大きくなる
	m_col.lock()->SetScale({ m_pParent->m_pTrans->GetScale()->x,m_pParent->m_pTrans->GetScale()->y,100 });
}


//===================================
//
//	更新関数
//
//===================================
void CDamageTrapCollisionComponent::Update()
{
}


//===================================
//
//	後更新関数
//
//===================================
void CDamageTrapCollisionComponent::LateUpdate()
{

}


//===================================
//
//	終了関数
//
//===================================
void CDamageTrapCollisionComponent::End()
{
	CObjectManager::DestroyObject(m_pParent);
}


//===== 使わない時は消してください() =====
//===================================
//
//	2D当たり判定(Enter) コールバック関数
//
//===================================
void CDamageTrapCollisionComponent::OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// タグなどで判定
	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		m_res.lock()->Respawn();
	}
}