//======================================================================
//											CElecGimmickBase.h
//	ギミックベースレイアウト
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
#include "CRigidbody2D.h"
#include "CCollision2D.h"
#include "CElecGimmickBase.h"

//===== マクロ定義 =====
#define MAX_RESOURCE (30.0f)


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CElecGimmickBase::Start()
{
	m_fEnergyBulletResource = 0;
	m_bCurChargeflg = false;
	m_bOldChargeflg = false;
}

void CElecGimmickBase::Update()
{
	m_bOldChargeflg = m_bCurChargeflg;

	if (m_fEnergyBulletResource > 0)
	{
		EnergyOn();
		m_fEnergyBulletResource -= m_fUseResource;
		if (m_fEnergyBulletResource <= 0)
		{
			m_fEnergyBulletResource = 0;
			EnergyOff();
			if (m_bCurChargeflg)
			{
				m_bOldChargeflg = false;
			}
		}
	}

	// テクスチャ変更
	if (m_bCurChargeflg || m_fEnergyBulletResource > 0)
	{
		//m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseColor({ 1.0f, 1.0f, 0.0f, 1.0f });
		m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseTexture(m_energyOnTexture.c_str());
	}
	else
	{
		m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseTexture(m_energyOffTexture.c_str());
	}

	GimmickUpdate();
}

void CElecGimmickBase::LateUpdate()
{

}

void CElecGimmickBase::OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// タグなどで判定
		// 帯電側
	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		// 侵入してきたタグにコンポーネント追加
		const auto& resorce = collsion2d->GetParent()->GetComponent<CResourceEnergy>();

		// バグ抑制
		if (resorce)
		{
			// 帯電してたら
			if (resorce->GetFlag())
			{
				// 電気が残ってる
				//if (m_fEnergyBulletResource > 0) return;

				//消費
				resorce->SubEnergy(m_fUseResource);
				m_fUseResource == 0 ? resorce->m_bEnergyUse = false : resorce->m_bEnergyUse = true;
				ChargePlayerEnter(collsion2d);

				m_bCurChargeflg = true;
			}
		}
	}
}

void CElecGimmickBase::OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// タグなどで判定
		// 帯電側
	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		// 侵入してきたタグにリジットボディ追加
		const auto& resorce = collsion2d->GetParent()->GetComponent<CResourceEnergy>();

		// バグ抑制
		if (resorce)
		{
			m_bCurChargeflg = resorce->GetFlag();

			// エンター
			if (m_bCurChargeflg && !m_bOldChargeflg)
			{
				// 電気が残ってる
				//if (m_fEnergyBulletResource > 0) return;

				//消費
				resorce->SubEnergy(m_fUseResource);
				m_fUseResource == 0 ? resorce->m_bEnergyUse = false : resorce->m_bEnergyUse = true;
				ChargePlayerEnter(collsion2d);
			}
			// ステイ
			else if (m_bCurChargeflg && m_bOldChargeflg)
			{
				// 電気が残ってる
				if (m_fEnergyBulletResource > 0) return;

				//消費
				resorce->SubEnergy(m_fUseResource);
				m_fUseResource == 0 ? resorce->m_bEnergyUse = false : resorce->m_bEnergyUse = true;
				ChargePlayerStay(collsion2d);
			}
			// イグジット
			else if (!m_bCurChargeflg && m_bOldChargeflg)
			{
				ChargePlayerExit(collsion2d);
				resorce->m_bEnergyUse = false;
			}
		}
	
	}
	else if (collsion2d->GetParent()->GetTag() == "EnergyBullet")
	{
		const auto& resorce = collsion2d->GetParent()->GetComponent<CResourceEnergy>();

		if (resorce)
		{
			m_fEnergyBulletResource += resorce->GetResource();
			// 上限処理
			if (m_fEnergyBulletResource > MAX_RESOURCE)
				m_fEnergyBulletResource = MAX_RESOURCE;
		}
	}
}

void CElecGimmickBase::OnCollisionExit2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// タグなどで判定
	// 帯電側
	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		// 侵入してきたタグにリジットボディ追加
		const auto& resorce = collsion2d->GetParent()->GetComponent<CResourceEnergy>();

		// バグ抑制
		if (resorce)
		{
			if (resorce->GetFlag())
			{
				ChargePlayerExit(collsion2d);
			}
			resorce->m_bEnergyUse = false;
			m_bCurChargeflg = false;
		}
	}
}