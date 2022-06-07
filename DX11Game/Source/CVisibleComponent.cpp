//======================================================================
//											CVisibleComponent.cpp
//	���͈̓R���|�[�l���g
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/12/04	�쐬
//		 12/05	����
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CVisibleComponent.h"
#include "CCollision2D.h"
#include "CEnemyBase.h"

//===== �}�N����` =====


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===== �񋓑̐錾 =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CVisibleComponent::Start()
{
	m_col = m_pParent->GetComponent<CCollision2D>();
	m_bVisibleflg = false;
	m_bTouchflg = false;
}


//===================================
//
//	�X�V�֐�
//
//===================================
void CVisibleComponent::Update()
{
	m_bTouchflg = false;

	const auto& enemy = m_enemy.lock();
	if (!enemy)return;

	m_pParent->m_pTrans->SetPos(enemy->m_pParent->m_pTrans->GetPos());
}


//===================================
//
//	��X�V�֐�
//
//===================================
void CVisibleComponent::LateUpdate()
{

}

//===================================
//
//	�傫��&�����ύX�֐�
//
//===================================
void CVisibleComponent::SetTargetEnemy(std::weak_ptr<CEnemyBase> enemy ,Vector3 scale, Vector3 offset)
{
	m_enemy = enemy;
	m_col.lock()->SetScale(scale);
	m_col.lock()->SetOffSet(offset);

	m_pParent->m_pTrans->SetPos(m_enemy.lock()->m_pParent->m_pTrans->GetPos());
}

//===================================
//
//	�����ύX�֐�
//
//===================================
void CVisibleComponent::SetDir(Vector3 offset)
{
	m_col.lock()->SetOffSet(offset);
}

Vector3 CVisibleComponent::GetDir()
{
	return m_col.lock()->GetOffSet();
}

//===================================
//
//	�����蔻��擾�֐�
//
//===================================
bool CVisibleComponent::GetVisibleflg()
{
	return m_bVisibleflg;
}

//===== �g��Ȃ����͏����Ă�������() =====
//===================================
//
//	2D�����蔻��(Enter) �R�[���o�b�N�֐�
//
//===================================
void CVisibleComponent::OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// �^�O�ȂǂŔ���
	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		m_bVisibleflg = true;
		m_bTouchflg = true;
		m_enemy.lock()->SetPlayer(collsion2d->GetParent()->tish());
	}
	if (!m_bTouchflg)
	{
		OnCollisionExit2D(collsion2d);
	}
}

void CVisibleComponent::OnCollisionExit2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// �^�O�ȂǂŔ���
	//if (collsion2d->GetParent()->GetTag() == "Player")
	//{
		m_bVisibleflg = false;
	//}
}
