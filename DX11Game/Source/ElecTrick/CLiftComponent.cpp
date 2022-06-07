//======================================================================
//											CLiftComponent.h
//	��]��(���])�p�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/12/01	�쐬(�D��x�����������ߌ��)
//
//======================================================================

//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CLiftComponent.h"
#include "CRigidbody2D.h"
#include "CCollision2D.h"
#include "CLiftPoint.h"

#include "Sound.h"


//===== �}�N����` =====


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CLiftComponent::Start()
{
	m_rb = m_pParent->GetComponent<CRigidbody2D>();
	m_re = m_pParent->GetComponent<CResourceEnergy>();
	currentRoot = 0;
	OldcurrentRoot = currentRoot;
	speed = 2.0f;
	vec = Vector3{ 0,0,0 };
	m_fUseResource = 0.05f;
	bLiftMoveflg = false;
	m_bTurnFlg = false;
	m_bBulletFlg = false;

	m_energyOffTexture = "data/model/Lift/Lift_Tex2.png";
	m_energyOnTexture = "data/model/Lift/Lift_Tex_Change.png";
}

//===================================
//
//	�G���h�֐�
//
//===================================
void CLiftComponent::End()
{
	for (const auto& point : m_aLiftPint)
	{
		const auto point_s = point.lock();
		if(point_s)
			CObjectManager::DestroyObject(point_s);
	}

}

void CLiftComponent::GimmickUpdate()
{
	// �|�C���g�������Ȃ�͂���
	if (MaxWayPoint <= 0) return;

	if (m_bTurnFlg) return;

	// �T�E���h
	CSound::PlaySE("RiftMove.wav", 0.03f);

	// �ڎw���|�C���g�̌��݂̍��W�̋���
	vec = wayPoints[currentRoot] - m_pParent->m_pTrans->GetPos();
	// �����̃x�N�g���̒�����1�ȉ����ƃ|�C���g�ɂ��ǂ蒅���Ă�̂�
	if (Vector3::Length(vec) <= 1.0f)
	{

		if (currentRoot == 0)
		{
			OldcurrentRoot = currentRoot;
			vec = Vector3{ 0,0,0 };
			m_rb.lock()->SetForce({ 0,0,0 });
			// �T�E���h
			CSound::StopSE("RiftMove.wav");
		}
		else
		{
			OldcurrentRoot = currentRoot;
			//�z��̗v�f�����炵�A���̃|�C���g���w���悤�ɂ���
			currentRoot--;
			vec = wayPoints[currentRoot] - m_pParent->m_pTrans->GetPos();
		}
	}
	// �x�N�g���𐳋K������
	vec = Vector3::Normalize(vec);

	// ���K�������x�N�g���ɃX�s�[�h�������āA���t�g�𓮂���
	m_rb.lock()->SetForce(vec * speed);
	bLiftMoveflg = true;
}

void CLiftComponent::CreateLiftPoint(Vector3 pos)
{
	MaxWayPoint++;

	wayPoints.push_back(pos);
	m_aLiftPint.push_back(CObjectManager::CreateObject<CLiftPoint>(pos));
}

//===================================
//
//	�ѓd�N���֐�
//
//===================================
void CLiftComponent::ChargePlayerEnter(std::weak_ptr<CCollision2D> collsion2d)
{
	if (currentRoot < (MaxWayPoint - 1))currentRoot++;

}

//===================================
//
//	�ѓd�؍݊֐�
//
//===================================
void CLiftComponent::ChargePlayerStay(std::weak_ptr<CCollision2D> collsion2d)
{
	const auto& rb = collsion2d.lock()->GetParent()->GetComponent<CRigidbody2D>();
	// ���ꂼ��̃M�~�b�N�̏���
	// �v���C���[����ɍڂ��Ă��邩�̔��f

	if (rb->GetHitDir() == DIRECTION::DOWN)
	{
		// �|�C���g�������Ȃ�͂���
		if (MaxWayPoint <= 0) return;

		// �ڎw���|�C���g�̌��݂̍��W�̋���
		vec = wayPoints[currentRoot] - m_pParent->m_pTrans->GetPos();
		// �����̃x�N�g���̒�����1�ȉ����ƃ|�C���g�ɂ��ǂ蒅���Ă�̂�
		if (Vector3::Length(vec) <= 1.0f)
		{
			OldcurrentRoot = currentRoot;
			//�z��̗v�f�𑝂₵�A���̃|�C���g���w���悤�ɂ���
			if (currentRoot < (MaxWayPoint - 1))currentRoot++;

			vec = wayPoints[currentRoot] - m_pParent->m_pTrans->GetPos();
		}
		// �x�N�g���𐳋K������
		vec = Vector3::Normalize(vec);

		// ���K�������x�N�g���ɃX�s�[�h�������āA���t�g�ƃv���C���[�𓮂���
		bLiftMoveflg = true;
		m_bTurnFlg = true;
		m_rb.lock()->SetForce(vec * speed);

		// �T�E���h
		CSound::PlaySE("RiftMove.wav", 0.03f);
	}

}

//===================================
//
//	�ѓd�P�ފ֐�
//
//===================================
void CLiftComponent::ChargePlayerExit(std::weak_ptr<CCollision2D> collsion2d)
{
	if (!m_bBulletFlg)
	{
		m_fEnergyBulletResource += 2;
	}
}

//===================================
//
//	�d�C�e�ғ��֐�
//
//===================================
void CLiftComponent::EnergyOn()
{
	// �|�C���g�������Ȃ�͂���
	if (MaxWayPoint <= 0) return;

	if (!m_bBulletFlg)
	{
		if (currentRoot < (MaxWayPoint - 1))currentRoot = currentRoot++;
	}

	// �ڎw���|�C���g�̌��݂̍��W�̋���
	vec = wayPoints[currentRoot] - m_pParent->m_pTrans->GetPos();
	// �����̃x�N�g���̒�����1�ȉ����ƃ|�C���g�ɂ��ǂ蒅���Ă�̂�
	if (Vector3::Length(vec) <= 1.0f)
	{
		OldcurrentRoot = currentRoot;
		//�z��̗v�f�𑝂₵�A���̃|�C���g���w���悤�ɂ���
		if (currentRoot < (MaxWayPoint - 1))currentRoot = currentRoot++;

		vec = wayPoints[currentRoot] - m_pParent->m_pTrans->GetPos();
	}
	// �x�N�g���𐳋K������
	vec = Vector3::Normalize(vec);

	// ���K�������x�N�g���ɃX�s�[�h�������āA���t�g�𓮂���
	m_rb.lock()->SetForce(vec * speed);
	bLiftMoveflg = true;
	m_bTurnFlg = true;
	m_bBulletFlg = true;

	// �T�E���h
	CSound::PlaySE("RiftMove.wav", 0.03f);
}

//===================================
//
//	�d�C�e�x�~�֐�
//
//===================================
void CLiftComponent::EnergyOff()
{
	currentRoot = OldcurrentRoot;
	bLiftMoveflg = false;
	m_bTurnFlg = false;
	m_bBulletFlg = false;
}

void CLiftComponent::OnCollisionEnter2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	CElecGimmickBase::OnCollisionEnter2D(collsion2d);

	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		if (m_fEnergyBulletResource > 1)
		{
			m_fEnergyBulletResource = 1;
		}
	}
}

void CLiftComponent::OnCollisionStay2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	CElecGimmickBase::OnCollisionStay2D(collsion2d);

	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		const auto& rb = collsion2d->GetParent()->GetComponent<CRigidbody2D>();

		// ���t�g��ł͂̏���
		// ���t�g�̏�Ƀv���C���[������Ă�Ȃ�v���C���[��������
		if (rb->GetHitDir() == DIRECTION::DOWN)
		{
			// �d�C�e�œ����Ă鎞
			if (bLiftMoveflg)
			{
				rb->SetForce(vec * speed);
			}
		}
	}
}
