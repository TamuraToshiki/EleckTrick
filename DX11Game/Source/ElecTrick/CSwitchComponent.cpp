//======================================================================
//											CSwitchComponent.h
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
#include "CSwitchComponent.h"

#include "Sound.h"


//===== �}�N����` =====


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CSwitchComponent::Start()
{
	bSwitchflg = false;
	m_fUseResource = 0.1f;

	m_energyOffTexture = "data/model/Electrode/Electrode.png";
	m_energyOnTexture = "data/model/Electrode/Electrode_Change.png";
}

void CSwitchComponent::LateUpdate()
{
	CElecGimmickBase::LateUpdate();

	//�J���[�ύX
	if (bSwitchflg)
	{
		//m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseColor({ 1.0f, 1.0f, 0.0f, 1.0f });
		m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseTexture(m_energyOnTexture.c_str());
	}
	else
	{
		//m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseTexture(m_energyOffTexture.c_str());
	}
}

//===================================
//
//	�ѓd�N���֐�
//
//===================================
void CSwitchComponent::ChargePlayerEnter(std::weak_ptr<CCollision2D> collsion2d)
{
	if (!bSwitchflg)
	{
		bSwitchflg = true;
		m_fUseResource = 0;
		CSound::PlaySE("SwitchOn.mp3");
	}
}

//===================================
//
//	�ѓd�؍݊֐�
//
//===================================
void CSwitchComponent::ChargePlayerStay(std::weak_ptr<CCollision2D> collsion2d)
{
	// ���ꂼ��̃M�~�b�N�̏���
	if (!bSwitchflg)
	{
		bSwitchflg = true;
		m_fUseResource = 0;
		CSound::PlaySE("SwitchOn.mp3");
	}
}

//===================================
//
//	�ѓd�P�ފ֐�
//
//===================================
void CSwitchComponent::ChargePlayerExit(std::weak_ptr<CCollision2D> collsion2d)
{

}

//===================================
//
//	�d�C�e�ғ��֐�
//
//===================================
void CSwitchComponent::EnergyOn()
{
	if (!bSwitchflg)
	{
		bSwitchflg = true;
		m_fUseResource = 0;
		CSound::PlaySE("SwitchOn.mp3");
	}
}

//===================================
//
//	�d�C�e�x�~�֐�
//
//===================================
void CSwitchComponent::EnergyOff()
{

}

bool CSwitchComponent::GetSwitchflg()
{
	return bSwitchflg;
}
