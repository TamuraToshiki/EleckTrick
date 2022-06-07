//======================================================================
//											CStandStickComponent.cpp
//	�R���|�[�l���g�쐬�p�̃x�[�X���C�A�E�g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/11/16	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CStandStickComponent.h"
#include "CCollision2D.h"
#include "CEnemyBase.h"

//===== �}�N����` =====


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CStandStickComponent::Start()
{
	m_col = m_pParent->GetComponent<CCollision2D>();
	m_bGrandflg = false;
}


//===================================
//
//	�X�V�֐�
//
//===================================
void CStandStickComponent::Update()
{
	const auto& enemy = m_enemy.lock();
	if (!enemy)return;

	m_pParent->m_pTrans->SetPos(enemy->m_pParent->m_pTrans->GetPos());
}


//===================================
//
//	��X�V�֐�
//
//===================================
void CStandStickComponent::LateUpdate()
{

}

void CStandStickComponent::SetStandStick(std::weak_ptr<CEnemyBase> enemy)
{
	m_enemy = enemy;
	m_pParent->m_pTrans->SetPos(m_enemy.lock()->m_pParent->m_pTrans->GetPos());
}

bool CStandStickComponent::GetGrandflg()
{
	return m_bGrandflg;
}

//===== �g��Ȃ����͏����Ă�������() =====
//===================================
//
//	2D�����蔻��(Stay) �R�[���o�b�N�֐�
//
//===================================

void CStandStickComponent::OnCollisionStay2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	if (collsion2d->GetParent()->GetTag() != "SecurityRobo")
	{
		m_bGrandflg = true;
	}
}

void CStandStickComponent::OnCollisionExit2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	m_bGrandflg = false;
}
