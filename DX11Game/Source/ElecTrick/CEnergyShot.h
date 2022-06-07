//======================================================================
//											CEnergyShot.h
//	エネルギー弾発射
//
//======================================================================
//	author :	AT12A 15 榊原龍我
//======================================================================
//	開発履歴
//	2020/11/24	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CResourceEnergy.h"
#include "CObjectManager.h"
#include "input.h"
#include "CEnergyBullet.h"
#include "CPlayerActionComponent.h"
#include "CBulletComponent.h"
#include "Sound.h"


//===== クラス定義 =====
class CEnergyShot : public CComponent
{
private:
	// --- 変数宣言 ---
	float m_fResourceShot;	//弾のエネルギー量 
	int   m_nLevel;			//弾のレベル
	float m_shotForce;		//弾の速度
	float m_rimit;			//弾の生存時間
	std::weak_ptr<CResourceEnergy> m_resource;	//リソース
	std::weak_ptr<CPlayerActionComponent> m_input;		//インプット

public:
	// --- 関数宣言 ---

	// 作成時に呼ばれます
	void Start() override
	{
		m_resource = m_pParent->GetComponent<CResourceEnergy>();
		m_input = m_pParent->GetComponent<CPlayerActionComponent>();
		m_shotForce = 10.0f;
	}

	// 毎フレーム呼ばれます
	void Update() override
	{
	}

	// 毎フレームの更新後に呼ばれます
	void LateUpdate() override
	{
	}


	//===== 使わない時は消してください() =====
	//--- コールバック関数 ---
	// 2D当たり判定
	void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override
	{
		// タグなどで判定
		if (collsion2d)
		{
			
		}
	}

	//--------------------------------------------------
	//	弾生成
	//--------------------------------------------------
	bool CreateShot()
	{
		const auto& resource = m_resource.lock();
		if (!resource) return false;
		if (resource->GetResource() < 0.5f) return false;

		m_nLevel = resource->GetLevel();

		if (m_nLevel != ZERO)
		{
			//リソースのコピー・減算
			//m_fResourceShot = resource->GetResource();
			////resource->SubEnergy(m_fResourceShot);
			//resource->ClaerResource();
			resource->ShotUsedResource(10);
			m_fResourceShot = 10.0f;

			//オブジェクトの生成
			const auto &bullet = CObjectManager::CreateObject<CEnergyBullet>();
			const auto &rb     = bullet->GetComponent<CRigidbody2D>();
			const auto &br     = bullet->GetComponent<CResourceEnergy>();

			const auto &input = m_input.lock();
			

			if (rb && input && br)
			{
				Vector3 dir = input->GetPlayerDir();
				Vector3 pos = m_pParent->m_pTrans->m_pos;
				pos->y += 50;

				rb->SetForce(dir * m_shotForce);
				bullet->m_pTrans->SetPos(pos + dir * 50.0f);

				br->AddEnergy(m_fResourceShot);

				// サウンド
				CSound::PlaySE("Shot.mp3");

				return true;
			}
		}

		return false;
	}

	
};



