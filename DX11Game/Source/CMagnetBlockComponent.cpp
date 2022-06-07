//======================================================================
//											CMagnetBlockComponent.h
//	�d�����R���|�[�l���g
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
#include "CMagnetBlockComponent.h"
#include "CEnemyBase.h"

//===== �}�N����` =====
#define PLAYER_SPEED	4.0f

//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====
std::weak_ptr<CMagnetBlockComponent> CMagnetBlockComponent::g_mainMagnet;
int CMagnetBlockComponent::m_nMaxNum = 0;
int CMagnetBlockComponent::m_nCurrNum = 0;

//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CMagnetBlockComponent::Start()
{
	CElecGimmickBase::Start();

	m_fUseResource = 0.075f;
	m_player = CObjectManager::SearchObjectName("Player").lock()->tish();

	m_nNum = m_nMaxNum;
	m_nMaxNum++;

	m_energyOffTexture = "data/model/Magnet/Magnet.png";
	m_energyOnTexture = "data/model/Magnet/Magnet_Change.png";

	// �R���g���[���[��
	m_nJoyCount = GetJoyCount();
}

//===================================
//
//	���X�|�[��
//
//===================================
void CMagnetBlockComponent::Restart()
{
	m_fEnergyBulletResource = 0.0f;
}

//===================================
//
//	�M�~�b�N�X�V�֐�
//
//===================================
void CMagnetBlockComponent::GimmickUpdate()
{

}

//===================================
//
//	�G���h�֐�
//
//===================================
void CMagnetBlockComponent::End()
{
	m_nMaxNum--;
	CObjectManager::DestroyObject(m_pParent);
}


//===================================
//
//	�ѓd�N���֐�
//
//===================================
void CMagnetBlockComponent::ChargePlayerEnter(std::weak_ptr<CCollision2D> collsion2d)
{
	ChargePlayerStay(collsion2d);
}

//===================================
//
//	�ѓd�؍݊֐�
//
//===================================
void CMagnetBlockComponent::ChargePlayerStay(std::weak_ptr<CCollision2D> collsion2d)
{
	m_fEnergyBulletResource = 4.0f;

	EnergyOn();
}

//===================================
//
//	�ѓd�P�ފ֐�
//
//===================================
void CMagnetBlockComponent::ChargePlayerExit(std::weak_ptr<CCollision2D> collsion2d)
{
	
}

//===================================
//
//	�d�C�e�ғ��֐�
//
//===================================
void CMagnetBlockComponent::EnergyOn()
{
	if (m_nCurrNum != m_nNum)
	{
		m_fEnergyBulletResource = 0;
		return;
	}

	const auto& player = m_player.lock();
	if (!player) return;
	const auto& rb = player->GetComponent<CRigidbody2D>();
	if (!rb) return;

	Vector3 vec = m_pParent->m_pTrans->m_pos - player->m_pTrans->m_pos;
	rb->SetForce(vec / 40);

	// �R���g���[���[
	float JoyY = -(float)GetJoyY(JOYSTICKID1);
	JoyY /= 32767.0f;
	if (m_nJoyCount <= 0) JoyY = 1.0f;

	// ��~
	float speed = 1.0f;
	if (GetJoyButton(JOYSTICKID1, JOYBUTTON_LB) ||
		GetJoyButton(JOYSTICKID1, JOYBUTTON_RB))
	{
		speed = 0.0f;
	}

	if (GetKeyPress(VK_UP) || (m_nJoyCount > 0 && JoyY > 0.4f))
	{
		rb->AddForceY(PLAYER_SPEED * fabsf(JoyY) * speed);
	}
	if (GetKeyPress(VK_DOWN) || (m_nJoyCount > 0 && JoyY < -0.4f))
	{
		rb->AddForceY(-PLAYER_SPEED * fabsf(JoyY) * speed);
	}
}

//===================================
//
//	�d�C�e�x�~�֐�
//
//===================================
void CMagnetBlockComponent::EnergyOff()
{

}

void CMagnetBlockComponent::OnCollisionEnter2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	if (collsion2d->GetParent()->GetTag() == "Player" ||
		collsion2d->GetParent()->GetTag() == "EnergyBullet")
	{
		m_nCurrNum = m_nNum;
		if (collsion2d->GetParent()->GetTag() == "EnergyBullet")
		{
			CSound::PlaySE("MagnetOn.wav");
		}
	}

	CElecGimmickBase::OnCollisionEnter2D(collsion2d);
}

void CMagnetBlockComponent::OnCollisionStay2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	CElecGimmickBase::OnCollisionStay2D(collsion2d);

	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		if (m_fEnergyBulletResource >= 6.0f)
		{
			m_fEnergyBulletResource = 5.0f;
		}
	}
	else if (collsion2d->GetParent()->GetTag() == "EnergyBullet")
	{
		m_nCurrNum = m_nNum;
	}
}
