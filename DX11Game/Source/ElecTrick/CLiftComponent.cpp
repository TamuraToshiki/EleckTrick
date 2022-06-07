//======================================================================
//											CLiftComponent.h
//	回転床(自転)用コンポーネント
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/12/01	作成(優先度を下げたため後回し)
//
//======================================================================

//====== インクルード部 ======
#include "CObject.h"
#include "CLiftComponent.h"
#include "CRigidbody2D.h"
#include "CCollision2D.h"
#include "CLiftPoint.h"

#include "Sound.h"


//===== マクロ定義 =====


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CLiftComponent::Start()
{
	m_rb = m_pParent->GetComponent<CRigidbody2D>();
	m_re = m_pParent->GetComponent<CResourceEnergy>();
	currentRoot = 0;
	OldcurrentRoot = currentRoot;
	speed = 2.0f;
	vec = Vector3{ 0,0,0 };
	m_fUseResource = 0.05f;
	bLiftMoveflg = false;
	m_bTurnFlg = false;
	m_bBulletFlg = false;

	m_energyOffTexture = "data/model/Lift/Lift_Tex2.png";
	m_energyOnTexture = "data/model/Lift/Lift_Tex_Change.png";
}

//===================================
//
//	エンド関数
//
//===================================
void CLiftComponent::End()
{
	for (const auto& point : m_aLiftPint)
	{
		const auto point_s = point.lock();
		if(point_s)
			CObjectManager::DestroyObject(point_s);
	}

}

void CLiftComponent::GimmickUpdate()
{
	// ポイントが無いならはじく
	if (MaxWayPoint <= 0) return;

	if (m_bTurnFlg) return;

	// サウンド
	CSound::PlaySE("RiftMove.wav", 0.03f);

	// 目指すポイントの現在の座標の距離
	vec = wayPoints[currentRoot] - m_pParent->m_pTrans->GetPos();
	// 距離のベクトルの長さが1以下だとポイントにたどり着いてるので
	if (Vector3::Length(vec) <= 1.0f)
	{

		if (currentRoot == 0)
		{
			OldcurrentRoot = currentRoot;
			vec = Vector3{ 0,0,0 };
			m_rb.lock()->SetForce({ 0,0,0 });
			// サウンド
			CSound::StopSE("RiftMove.wav");
		}
		else
		{
			OldcurrentRoot = currentRoot;
			//配列の要素を減らし、次のポイントを指すようにする
			currentRoot--;
			vec = wayPoints[currentRoot] - m_pParent->m_pTrans->GetPos();
		}
	}
	// ベクトルを正規化する
	vec = Vector3::Normalize(vec);

	// 正規化したベクトルにスピードをかけて、リフトを動かす
	m_rb.lock()->SetForce(vec * speed);
	bLiftMoveflg = true;
}

void CLiftComponent::CreateLiftPoint(Vector3 pos)
{
	MaxWayPoint++;

	wayPoints.push_back(pos);
	m_aLiftPint.push_back(CObjectManager::CreateObject<CLiftPoint>(pos));
}

//===================================
//
//	帯電侵入関数
//
//===================================
void CLiftComponent::ChargePlayerEnter(std::weak_ptr<CCollision2D> collsion2d)
{
	if (currentRoot < (MaxWayPoint - 1))currentRoot++;

}

//===================================
//
//	帯電滞在関数
//
//===================================
void CLiftComponent::ChargePlayerStay(std::weak_ptr<CCollision2D> collsion2d)
{
	const auto& rb = collsion2d.lock()->GetParent()->GetComponent<CRigidbody2D>();
	// それぞれのギミックの処理
	// プレイヤーが上に載っているかの判断

	if (rb->GetHitDir() == DIRECTION::DOWN)
	{
		// ポイントが無いならはじく
		if (MaxWayPoint <= 0) return;

		// 目指すポイントの現在の座標の距離
		vec = wayPoints[currentRoot] - m_pParent->m_pTrans->GetPos();
		// 距離のベクトルの長さが1以下だとポイントにたどり着いてるので
		if (Vector3::Length(vec) <= 1.0f)
		{
			OldcurrentRoot = currentRoot;
			//配列の要素を増やし、次のポイントを指すようにする
			if (currentRoot < (MaxWayPoint - 1))currentRoot++;

			vec = wayPoints[currentRoot] - m_pParent->m_pTrans->GetPos();
		}
		// ベクトルを正規化する
		vec = Vector3::Normalize(vec);

		// 正規化したベクトルにスピードをかけて、リフトとプレイヤーを動かす
		bLiftMoveflg = true;
		m_bTurnFlg = true;
		m_rb.lock()->SetForce(vec * speed);

		// サウンド
		CSound::PlaySE("RiftMove.wav", 0.03f);
	}

}

//===================================
//
//	帯電撤退関数
//
//===================================
void CLiftComponent::ChargePlayerExit(std::weak_ptr<CCollision2D> collsion2d)
{
	if (!m_bBulletFlg)
	{
		m_fEnergyBulletResource += 2;
	}
}

//===================================
//
//	電気弾稼働関数
//
//===================================
void CLiftComponent::EnergyOn()
{
	// ポイントが無いならはじく
	if (MaxWayPoint <= 0) return;

	if (!m_bBulletFlg)
	{
		if (currentRoot < (MaxWayPoint - 1))currentRoot = currentRoot++;
	}

	// 目指すポイントの現在の座標の距離
	vec = wayPoints[currentRoot] - m_pParent->m_pTrans->GetPos();
	// 距離のベクトルの長さが1以下だとポイントにたどり着いてるので
	if (Vector3::Length(vec) <= 1.0f)
	{
		OldcurrentRoot = currentRoot;
		//配列の要素を増やし、次のポイントを指すようにする
		if (currentRoot < (MaxWayPoint - 1))currentRoot = currentRoot++;

		vec = wayPoints[currentRoot] - m_pParent->m_pTrans->GetPos();
	}
	// ベクトルを正規化する
	vec = Vector3::Normalize(vec);

	// 正規化したベクトルにスピードをかけて、リフトを動かす
	m_rb.lock()->SetForce(vec * speed);
	bLiftMoveflg = true;
	m_bTurnFlg = true;
	m_bBulletFlg = true;

	// サウンド
	CSound::PlaySE("RiftMove.wav", 0.03f);
}

//===================================
//
//	電気弾休止関数
//
//===================================
void CLiftComponent::EnergyOff()
{
	currentRoot = OldcurrentRoot;
	bLiftMoveflg = false;
	m_bTurnFlg = false;
	m_bBulletFlg = false;
}

void CLiftComponent::OnCollisionEnter2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	CElecGimmickBase::OnCollisionEnter2D(collsion2d);

	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		if (m_fEnergyBulletResource > 1)
		{
			m_fEnergyBulletResource = 1;
		}
	}
}

void CLiftComponent::OnCollisionStay2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	CElecGimmickBase::OnCollisionStay2D(collsion2d);

	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		const auto& rb = collsion2d->GetParent()->GetComponent<CRigidbody2D>();

		// リフトらではの処理
		// リフトの上にプレイヤーが乗ってるならプレイヤーも動かす
		if (rb->GetHitDir() == DIRECTION::DOWN)
		{
			// 電気弾で動いてる時
			if (bLiftMoveflg)
			{
				rb->SetForce(vec * speed);
			}
		}
	}
}
