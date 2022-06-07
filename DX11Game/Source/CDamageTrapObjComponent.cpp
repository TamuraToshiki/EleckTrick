//======================================================================
//											CDamageTrapObjComponent.cpp
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
#include "CDamageTrapObjComponent.h"
#include "CDamegeTrapPoint.h"
#include "CDamageTrapCollsion.h"
#include "CBillboardObject.h"
#include "CAnimater2D.h"


//===== マクロ定義 =====


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CDamageTrapObjComponent::Start()
{
	m_col = m_pParent->GetComponent<CCollision2D>();
	m_DamegeTrap = m_pParent->GetComponent<CDamageTrapCollisionComponent>();
	m_res = CObjectManager::SearchObjectName("Player").lock()->GetComponent<CRespawnComponent>();
	m_nCurrentRoot = 0;
	m_MaxTrapPoint = 1;
	pos = Vector3{0,0,0};
	scale = Vector3{ 0,0,0 };
	m_Radian = Vector3{ 0,0,0 };

	m_vTrapPoint.push_back(m_pParent->m_pTrans->GetPos());
	m_aTrapPoint.push_back(CObjectManager::CreateObject<CDamegeTrapPoint>(m_pParent->m_pTrans->GetPos()));
}


//===================================
//
//	更新関数
//
//===================================
void CDamageTrapObjComponent::Update()
{
	
}


//===================================
//
//	後更新関数
//
//===================================
void CDamageTrapObjComponent::LateUpdate()
{

}


//===================================
//
//	終了関数
//
//===================================
void CDamageTrapObjComponent::End()
{
	for (const auto& point : m_aTrapPoint)
	{
		const auto point_s = point.lock();
		
		if (point_s)	CObjectManager::DestroyObject(point_s);
	}

	for (const auto& collision : m_aTrapcol)
	{
		const auto collision_s = collision.lock();

		if (collision_s)	CObjectManager::DestroyObject(collision_s);
	}

	for (const auto& effect : m_aTrapEffect)
	{
		const auto efffect_s = effect.lock();

		if (efffect_s)	CObjectManager::DestroyObject(efffect_s);
	}
}


//===== 使わない時は消してください() =====
//===================================
//
//	2D当たり判定(Enter) コールバック関数
//
//===================================
void CDamageTrapObjComponent::OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// タグなどで判定
	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		m_res.lock()->Respawn();
	}
}

//-------------------------------------------------------------------------//
//	ダメージトラップポイント作成
//-------------------------------------------------------------------------//
void CDamageTrapObjComponent::CreateTrapPoint(Vector3 pos)
{
	m_MaxTrapPoint++;

	m_vTrapPoint.push_back(pos);
	m_aTrapPoint.push_back(CObjectManager::CreateObject<CDamegeTrapPoint>(pos));

	CreateTrap();
}

//-------------------------------------------------------------------------//
//	ダメージトラップ生成
//-------------------------------------------------------------------------//
void CDamageTrapObjComponent::CreateTrap()
{
	// ポイントが無いならはじく
	if (m_MaxTrapPoint <= 0) return;

	// 現在のポイントと次のポイントの中間をposに代入する
	pos = (m_vTrapPoint[m_nCurrentRoot + 1] - m_vTrapPoint[m_nCurrentRoot]) / 2;
	m_Radian = GetRadian(m_vTrapPoint[m_nCurrentRoot], m_vTrapPoint[m_nCurrentRoot + 1]);

	scale = pos;
	if (scale->x < 0) { scale->x *= -1; }
	if (scale->y < 0) { scale->y *= -1; }

	if (scale->x < 10.0f)
	{
		scale->x = 40;
	}
	if (scale->y < 10.0f)
	{
		scale->y = 40;
	}

	// 中間ポイントにオブジェクトを生成する
	m_aTrapcol.push_back(CObjectManager::CreateObject<CDamageTrapCollision>((m_vTrapPoint[m_nCurrentRoot] + pos), { 0,0,0 }, scale));

	// エフェクトの生成
	Vector3 v = m_vTrapPoint[m_nCurrentRoot + 1] - m_vTrapPoint[m_nCurrentRoot];
	const int nEffectNum = v.magnitude() / 25.0f;
	Vector3 pos = m_vTrapPoint[m_nCurrentRoot];
	Vector3 addPos = v / nEffectNum;
	Vector3 rot;
	
	rot->z = acosf(Vector3::Dot(Vector3(1, 0, 0), v.normalized()));
	 
	if(v->y < 0)
		pos += v.normalized().RotationZ(90) * 25;
	else
		pos += v.normalized().RotationZ(-90) * 25;


	for (int i = 0; i < nEffectNum - 1; i++)
	{
		// 座標加算
		pos += addPos;

		for (int j = 0; j < 3; j++)
		{
			float size = rand() % 100;
			Vector3 scale = { 100 + size,100 + size,100 };
			const auto& obj = CObjectManager::CreateObject<CBillboardObject>(pos, rot, scale);

			// スプライト
			const auto& spr = obj->GetComponent<CBillboardRenderer>();
			char szName[256];
			sprintf_s(szName, "data/texture/DamageTexture/Elect%d.png", j + 1);
			spr->SetDiffuseTexture(szName);
			spr->SetMeshBlendState(BS_ADDITIVE);

			// アニメーター
			const auto& anim = obj->AddComponent<CAnimator2D>();
			anim->SetSplit(4, 4);
			anim->SetAnimationSpeed(2);
			anim->AddAnimationDefault("effect", 4 * 4);
			anim->SetAnimation("effect");
			anim->SetStartAnimNo(rand() % 16);

			// 格納
			m_aTrapEffect.push_back(obj);
		}
	}

	// 現在のポイントを更新
	m_nCurrentRoot++;
}

Vector3 CDamageTrapObjComponent::GetRadian(Vector3 a, Vector3 b)
{
	m_Radian->z = atan2f(b->y - a->y, b->x - a->x);
	m_Radian->z = m_Radian->z * 180 / 3.141592f;
	return m_Radian;
}
