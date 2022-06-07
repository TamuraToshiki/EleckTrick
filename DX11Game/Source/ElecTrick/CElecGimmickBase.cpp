//======================================================================
//											CElecGimmickBase.h
//	�M�~�b�N�x�[�X���C�A�E�g
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/12/01	
//
//======================================================================

//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CCollision2D.h"
#include "CElecGimmickBase.h"

//===== �}�N����` =====
#define MAX_RESOURCE (30.0f)


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
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

	// �e�N�X�`���ύX
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
	// �^�O�ȂǂŔ���
		// �ѓd��
	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		// �N�����Ă����^�O�ɃR���|�[�l���g�ǉ�
		const auto& resorce = collsion2d->GetParent()->GetComponent<CResourceEnergy>();

		// �o�O�}��
		if (resorce)
		{
			// �ѓd���Ă���
			if (resorce->GetFlag())
			{
				// �d�C���c���Ă�
				//if (m_fEnergyBulletResource > 0) return;

				//����
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
	// �^�O�ȂǂŔ���
		// �ѓd��
	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		// �N�����Ă����^�O�Ƀ��W�b�g�{�f�B�ǉ�
		const auto& resorce = collsion2d->GetParent()->GetComponent<CResourceEnergy>();

		// �o�O�}��
		if (resorce)
		{
			m_bCurChargeflg = resorce->GetFlag();

			// �G���^�[
			if (m_bCurChargeflg && !m_bOldChargeflg)
			{
				// �d�C���c���Ă�
				//if (m_fEnergyBulletResource > 0) return;

				//����
				resorce->SubEnergy(m_fUseResource);
				m_fUseResource == 0 ? resorce->m_bEnergyUse = false : resorce->m_bEnergyUse = true;
				ChargePlayerEnter(collsion2d);
			}
			// �X�e�C
			else if (m_bCurChargeflg && m_bOldChargeflg)
			{
				// �d�C���c���Ă�
				if (m_fEnergyBulletResource > 0) return;

				//����
				resorce->SubEnergy(m_fUseResource);
				m_fUseResource == 0 ? resorce->m_bEnergyUse = false : resorce->m_bEnergyUse = true;
				ChargePlayerStay(collsion2d);
			}
			// �C�O�W�b�g
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
			// �������
			if (m_fEnergyBulletResource > MAX_RESOURCE)
				m_fEnergyBulletResource = MAX_RESOURCE;
		}
	}
}

void CElecGimmickBase::OnCollisionExit2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// �^�O�ȂǂŔ���
	// �ѓd��
	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		// �N�����Ă����^�O�Ƀ��W�b�g�{�f�B�ǉ�
		const auto& resorce = collsion2d->GetParent()->GetComponent<CResourceEnergy>();

		// �o�O�}��
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