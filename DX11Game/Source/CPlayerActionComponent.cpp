//======================================================================
//											CPlayerActionComponent.cpp
//	�v���C���[�̃A�N�V�����R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/16	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CPlayerActionComponent.h"
#include "Sound.h"
#include "CCollision2D.h"
#include "CObjectManager.h"
#include "CAssimpObject.h"
#include "CGearCaterpillarComponent.h"


//===== �}�N����` =====
#define MOVE_SE_FRAME (5)


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CPlayerActionComponent::Start()
{
	m_speed = 4.0f;
	m_jump = 7.5f;
	m_right = true;
	m_bjunpFlag = true;
	m_nMoveSECnt = 0;

	m_rb = m_pParent->GetComponent<CRigidbody2D>();


	// �L���^�s���̐���
	const auto& cater = CObjectManager::CreateObject<CAssimpObject>();
	cater->AddComponent<CGearCaterpillarComponent>()->SetPlayer(m_pParent->m_pTrans);
	m_caterpollar = cater->GetComponent<CAssimpRenderer>();
	m_caterpollar.lock()->ModelLoad("data/model/MainAnim_Blender/Caterpillar_Anim.fbx");
	m_caterpollar.lock()->SetDiffuseTexture("data/model/Model_Main/texture_10.png");
	m_caterpollar.lock()->SetAnimIndex(0);
	m_caterpollar.lock()->SetAnimSpeed(2);
	m_caterpollar.lock()->PlayAnim();

	// �M�A�̐���
	const auto& gear = CObjectManager::CreateObject<CAssimpObject>();
	gear->AddComponent<CGearCaterpillarComponent>()->SetPlayer(m_pParent->m_pTrans);
	m_gear = gear->GetComponent<CAssimpRenderer>();
	m_gear.lock()->ModelLoad("data/model/MainAnim_Blender/Gear_Anim_Blender.fbx");
	m_gear.lock()->SetDiffuseTexture("data/model/Model_Main/texture_10.png");
	m_gear.lock()->SetAnimIndex(1);
	m_gear.lock()->SetAnimSpeed(2);
	m_gear.lock()->PlayAnim();

}


//===================================
//
//	�X�V�֐�
//
//===================================
void CPlayerActionComponent::Update()
{
	// ���W����
	m_caterpollar.lock()->m_pParent->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
	m_gear.lock()->m_pParent->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
}


//===================================
//
//	��X�V�֐�
//
//===================================
void CPlayerActionComponent::LateUpdate()
{
	if (m_nMoveSECnt >= 0)
	{
		m_nMoveSECnt--;
	}

	// ���W����
	m_caterpollar.lock()->m_pParent->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
	m_gear.lock()->m_pParent->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;


	// ���x
	Vector3 velo = m_rb.lock()->GetVelocity();

	// �A�j���[�V����
	float animSpeed = velo->x + velo->z;
	animSpeed = fabsf(animSpeed);

	m_caterpollar.lock()->SetAnimSpeed(animSpeed);
	m_gear.lock()->SetAnimSpeed(animSpeed);
}


//===================================
//
//	�I���֐�
//
//===================================
void CPlayerActionComponent::End()
{

}


//===== �g��Ȃ����͏����Ă�������() =====
//===================================
//
//	2D�����蔻��(Enter) �R�[���o�b�N�֐�
//
//===================================
void CPlayerActionComponent::OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	if (collsion2d->m_pParent->GetTag() != "Bullet")
	{
		const auto& rb = collsion2d->m_pParent->GetComponent<CRigidbody2D>();
		if (rb)
		{
			if (rb->GetHitDir() == DIRECTION::UP)
			{
				m_bjunpFlag = true;
			}
		}
	}
}


// �E�ړ�
void CPlayerActionComponent::RightMove(float fSpeed)
{
	m_rb.lock()->AddForceX(m_speed * fSpeed);
	m_dir->x = 1.0f;
	m_right = true;
	m_pParent->m_pTrans->m_rot->y = -90;

	if (m_nMoveSECnt < 0)
	{
		// �T�E���h
		CSound::PlaySE("PlayerMove.wav");
		m_nMoveSECnt = MOVE_SE_FRAME;
	}
}

// ���ړ�
void CPlayerActionComponent::LeftMove(float fSpeed)
{
	m_rb.lock()->AddForceX(m_speed * fSpeed);
	m_dir->x = -1.0f;
	m_right = false;
	m_pParent->m_pTrans->m_rot->y = 90;

	if (m_nMoveSECnt < 0)
	{
		// �T�E���h
		CSound::PlaySE("PlayerMove.wav");
		m_nMoveSECnt = MOVE_SE_FRAME;
	}
}

// �O�ړ�
void CPlayerActionComponent::FrontMove()
{
	m_rb.lock()->AddForceZ(-m_speed * 0.2f);
	m_pParent->m_pTrans->m_rot->y = 0;

	if (m_nMoveSECnt < 0)
	{
		// �T�E���h
		CSound::PlaySE("PlayerMove.wav");
		m_nMoveSECnt = MOVE_SE_FRAME;
	}
}

// ���ړ�
void CPlayerActionComponent::BackMove()
{
	m_rb.lock()->AddForceZ(m_speed * 0.2f);
	m_pParent->m_pTrans->m_rot->y = 180;

	if (m_nMoveSECnt < 0)
	{
		// �T�E���h
		CSound::PlaySE("PlayerMove.wav");
		m_nMoveSECnt = MOVE_SE_FRAME;
	}
}

// �ړ��X�g�b�v
void CPlayerActionComponent::StopMove()
{
	m_rb.lock()->SetForce({ 0,0,0 });
	StopSE();
}

// �T�E���h��~
void CPlayerActionComponent::StopSE()
{
	CSound::StopSE("PlayerMove.wav");
	m_nMoveSECnt = 0;
}

// ���R���[�u
void CPlayerActionComponent::FreeMove(Vector3 force)
{
	m_rb.lock()->AddForce(force);

	if (m_nMoveSECnt < 0)
	{
		// �T�E���h
		CSound::PlaySE("PlayerMove.wav");
		m_nMoveSECnt = MOVE_SE_FRAME;
	}
}

// �W�����v
bool CPlayerActionComponent::Jump()
{
	if (!m_bjunpFlag) return false;

	//�n�ʂɐݒu���Ă��鎞�̂�
	m_rb.lock()->SetForceY(m_jump);
	m_bjunpFlag = false;
	// �T�E���h
	CSound::PlaySE("Jump.mp3");

	return true;
}

