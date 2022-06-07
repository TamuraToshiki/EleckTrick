//======================================================================
//											CBulletComponent.h
//	弾
//
//======================================================================
//	author :	AT12A 15 榊原龍我
//======================================================================
//	開発履歴
//	2020/11/28	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CComponent.h"
#include "CObjectManager.h"
#include "CEffectComponent.h"
#include "Sound.h"

//===== クラス定義 =====
class CBulletComponent : public CComponent
{
private:
	// --- 変数宣言 ---
	int m_rimit;	//弾の生存時間
	int m_timeCnt;	//現在時間

public:
	// --- 関数宣言 ---


	// 作成時に呼ばれます
	void Start() override
	{
		m_rimit = 2 * 60;
	}

	// 毎フレーム呼ばれます
	void Update() override
	{
		m_timeCnt++;
		if (m_rimit < m_timeCnt)
		{
			CObjectManager::DestroyObject(m_pParent);
		}
	}

	// 毎フレームの更新後に呼ばれます
	void LateUpdate() override
	{

	}


	//===== 使わない時は消してください() =====
	//--- コールバック関数 ---
	// 2D当たり判定
	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override
	{
		// タグなどで判定
		if (collsion2d->m_pParent->GetTag() != "Player" && collsion2d->m_pParent->GetTag() != "EnergyBullet" &&
			collsion2d->m_pParent->GetTag() != "Visible" && collsion2d->m_pParent->GetTag() != "StandStick" &&
			collsion2d->m_pParent->GetTag() != "Blocksensor")
		{
			if (m_timeCnt <= 900)
			{
				// エフェクトの生成
				const auto& obj = CObjectManager::CreateObject<CObject>(m_pParent->m_pTrans->m_pos);
				obj->AddComponent<CEffectComponent>()->SetEffect(CEffectComponent::eHitEffect);
			}

			CSound::PlaySE("BulletHit.mp3");

			m_timeCnt = 999;
		}
	}
};



