//======================================================================
//											CFallComponent.h
//	電気与えてないと落ちるコンポーネント
//
//======================================================================
//	author :	AT12A 20 土屋美月
//======================================================================
//	開発履歴
//	2020/12/03	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CElecGimmickBase.h"
#include "CFallObject.h"
#include "CObjectManager.h"
#include "CResourceEnergy.h"
#include "CRigidbody2D.h"

#include "Sound.h"


//===== クラス定義 =====
class CFallComponent : public CElecGimmickBase
{
private:
	// --- 変数宣言 ---

	std::weak_ptr<CRigidbody2D> rb;
	bool bflg;

	Vector3 m_initPos;
public:
	// --- 関数宣言 ---

	// 作成時に呼ばれます
	void Start() override
	{
		bflg = true;
		rb = m_pParent->GetComponent<CRigidbody2D>();
		m_fUseResource = 0.21f;

		// 座標
		m_initPos = m_pParent->m_pTrans->m_pos;

		m_energyOffTexture = "data/model/EnagyFallBrock/EnagyFallBrock.png";
		m_energyOnTexture = "data/model/EnagyFallBrock/EnagyFallBrock_Change.png";
	}

	void Restart() override
	{
		m_pParent->m_pTrans->m_pos = m_initPos;
		bflg = true;
		rb.lock()->SetSolid(true);
		rb.lock()->SetUseGravity(false);
	}

	void ChargePlayerEnter(std::weak_ptr<CCollision2D>) override
	{

	}

	void ChargePlayerStay(std::weak_ptr<CCollision2D>) override
	{
		bflg = true;
	}

	void ChargePlayerExit(std::weak_ptr<CCollision2D>) override
	{

	}

	void EnergyOn() override
	{
		bflg = true;
		m_fEnergyBulletResource = 0;
	}

	void EnergyOff() override
	{

	}

	void OnCollisionStay2D(const std::shared_ptr<CCollision2D>& collsion2d) override
	{
		CElecGimmickBase::OnCollisionStay2D(collsion2d);

		if (collsion2d->GetParent()->GetTag() == "Player")
		{
			if (!bflg)
			{
				rb.lock()->SetSolid(false);
				rb.lock()->SetUseGravity(true);

			}
			bflg = false;
		}
	}
};

