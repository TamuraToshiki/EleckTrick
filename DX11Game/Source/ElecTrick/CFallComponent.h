//======================================================================
//											CFallComponent.h
//	�d�C�^���ĂȂ��Ɨ�����R���|�[�l���g
//
//======================================================================
//	author :	AT12A 20 �y������
//======================================================================
//	�J������
//	2020/12/03	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CElecGimmickBase.h"
#include "CFallObject.h"
#include "CObjectManager.h"
#include "CResourceEnergy.h"
#include "CRigidbody2D.h"

#include "Sound.h"


//===== �N���X��` =====
class CFallComponent : public CElecGimmickBase
{
private:
	// --- �ϐ��錾 ---

	std::weak_ptr<CRigidbody2D> rb;
	bool bflg;

	Vector3 m_initPos;
public:
	// --- �֐��錾 ---

	// �쐬���ɌĂ΂�܂�
	void Start() override
	{
		bflg = true;
		rb = m_pParent->GetComponent<CRigidbody2D>();
		m_fUseResource = 0.21f;

		// ���W
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

