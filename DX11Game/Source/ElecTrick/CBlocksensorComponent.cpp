//======================================================================
//											CBlocksensorComponent.cpp
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
#include "CBlocksensorComponent.h"
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
void CBlocksensorComponent::Start()
{
	m_col = m_pParent->GetComponent<CCollision2D>();
	m_bWallflg = false;
}


//===================================
//
//	�X�V�֐�
//
//===================================
void CBlocksensorComponent::Update()
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
void CBlocksensorComponent::LateUpdate()
{

}

void CBlocksensorComponent::SetBlocksensorComponent(std::weak_ptr<CEnemyBase> enemy)
{
	m_enemy = enemy;
	m_pParent->m_pTrans->SetPos(m_enemy.lock()->m_pParent->m_pTrans->GetPos());
}

bool CBlocksensorComponent::GetWallflg()
{
	return m_bWallflg;
}

//===================================
//
//	�����ύX�֐�
//
//===================================
void CBlocksensorComponent::SetDir(Vector3 offset)
{
	m_col.lock()->SetOffSet(offset);
}

Vector3 CBlocksensorComponent::GetDir()
{
	return m_col.lock()->GetOffSet();
}

//===== �g��Ȃ����͏����Ă�������() =====
//===================================
//
//	2D�����蔻��(Stay) �R�[���o�b�N�֐�
//
//===================================

void CBlocksensorComponent::OnCollisionStay2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	if (collsion2d->GetParent()->GetTag() != "Player")
	{
		if (collsion2d->GetParent()->GetTag() != "EnergyBullet")
		{
			if (collsion2d->GetParent()->GetTag() != "Visible")
			{
				m_bWallflg = true;
			}
		}
	}
}

void CBlocksensorComponent::OnCollisionExit2D(const std::shared_ptr<CCollision2D>& collsion2d)
{
	m_bWallflg = false;
}
