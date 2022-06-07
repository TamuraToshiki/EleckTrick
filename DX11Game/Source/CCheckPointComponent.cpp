//======================================================================
//											CCheckPointComponent.cpp
//	�`�F�b�N�|�C���g�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/15	�쐬
//
//======================================================================

//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CCollision2D.h"
#include "CCheckPointComponent.h"
#include "CObjectManager.h"


#include "Sound.h"


//===== �}�N����` =====


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CCheckPointComponent::Start()
{
	m_bOn = false;
	m_renderer = m_pParent->GetComponent<CAssimpRenderer>();

	// �v���C���[�̃��X�|�[��
	const auto& player = CObjectManager::SearchObjectTag("Player");
	if (!player.expired())
	{
		m_respawn = player.lock()->GetComponent<CRespawnComponent>();
	}

	m_energyOffTexture = "data/model/Resporn/Resporn_Tex10.png";
	m_energyOnTexture = "data/model/Resporn/Resporn_Tex_Change.png";

}

//===================================
//
//	�M�~�b�N�X�V�֐�
//
//===================================
void CCheckPointComponent::GimmickUpdate()
{
	if (m_bOn)
	{
		m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseTexture(m_energyOnTexture.c_str());
	}
	else
	{
		m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseTexture(m_energyOffTexture.c_str());
	}
}

//===================================
//
//	�ѓd�N���֐�
//
//===================================
void CCheckPointComponent::ChargePlayerEnter(std::weak_ptr<CCollision2D> collsion2d)
{
	EnergyOn();
}

//===================================
//
//	�ѓd�؍݊֐�
//
//===================================
void CCheckPointComponent::ChargePlayerStay(std::weak_ptr<CCollision2D> collsion2d)
{
	EnergyOn();
}

//===================================
//
//	�ѓd�P�ފ֐�
//
//===================================
void CCheckPointComponent::ChargePlayerExit(std::weak_ptr<CCollision2D> collsion2d)
{

}

//===================================
//
//	�d�C�e�ғ��֐�
//
//===================================
void CCheckPointComponent::EnergyOn()
{
	if (!m_bOn)
	{
		// ���X�|�[�����Z�b�g
		if (!m_respawn.expired())
		{
			m_respawn.lock()->SetRespawnPoint(m_pParent->m_pTrans->m_pos);
		}
		m_bOn = true;

		// �T�E���h
		CSound::PlaySE("CheckPointOn.wav");

		// �G�t�F�N�g�̐���
		Vector3 pos = m_pParent->m_pTrans->m_pos;
		pos->y += 100;
		Vector3 scale = { 300,300,300 };
		const auto& obj = CObjectManager::CreateObject<CObject>(pos);
		obj->AddComponent<CEffectComponent>()->SetEffect(CEffectComponent::eSaveEffect);

		obj->m_pTrans->m_scale = scale;
	}
}

//===================================
//
//	�d�C�e�x�~�֐�
//
//===================================
void CCheckPointComponent::EnergyOff()
{

}