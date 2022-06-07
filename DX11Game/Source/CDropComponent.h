//======================================================================
//											CDropComponent.h
//	�e�𓖂Ă��痎����R���|�[�l���g
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
#include "CDropObject.h"
#include "CObjectManager.h"
#include "CResourceEnergy.h"

#include "Sound.h"


//===== �N���X��` =====
class CDropComponent : public CElecGimmickBase
{
private:
	// --- �ϐ��錾 ---

	std::weak_ptr<CRigidbody2D> rb;
	bool bflg;
	float fBlockHP;
	Vector3 m_initPos;

public:
	// --- �֐��錾 ---

	// �쐬���ɌĂ΂�܂�
	void Start() override
	{
		bflg = false;
		rb = m_pParent->GetComponent<CRigidbody2D>();
		fBlockHP = 10.0f;
		m_fUseResource = 0;

		m_initPos = m_pParent->m_pTrans->m_pos;

		m_energyOffTexture   = "data/model/FallBrock/FallBrock.png";
		m_energyOnTexture  = "data/model/FallBrock/FallBrock_Change.png";
	}

	void Restart() override
	{
		m_pParent->m_pTrans->m_pos = m_initPos;
		fBlockHP = 10.0f;
		bflg = false;
		rb.lock()->SetSolid(true);
		rb.lock()->SetUseGravity(false);
		rb.lock()->SetForce({ 0,0,0 });
	}

	void GimmickUpdate() override
	{
		if (bflg)
		{
			if (rb.lock()->GetForce()->y > -5)
			{
				rb.lock()->AddForceY(-9.8 / 60);
			}
		}
		else
		{
			rb.lock()->SetForceY(0);
		}
	}

	void ChargePlayerEnter(std::weak_ptr<CCollision2D>) override
	{
		
	}

	void ChargePlayerStay(std::weak_ptr<CCollision2D>) override
	{
		bflg = false;
	}

	void ChargePlayerExit(std::weak_ptr<CCollision2D>) override
	{
		
	}

	void EnergyOn() override
	{
		m_fUseResource = m_fEnergyBulletResource;
		fBlockHP -= m_fUseResource;

		if (fBlockHP < 0)
		{
			bflg = true;
			CSound::PlaySE("Drop.wav");
		}
	}

	void EnergyOff() override
	{

	}

	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override
	{
		CElecGimmickBase::OnCollisionStay2D(collsion2d);

		if (collsion2d->GetParent()->GetTag() == "Block" || collsion2d->GetParent()->GetTag() == "DropObj")
		{
			bflg = false;
		}
	}
};





