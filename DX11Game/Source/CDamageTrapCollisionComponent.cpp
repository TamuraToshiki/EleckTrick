//======================================================================
//											CDamageTrapCollisionComponent.cpp
//	�_���[�W�g���b�v
//
//======================================================================
//	author :	AT12A 15 �匴����
//======================================================================
//	�J������
//	2020/12/13	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CDamageTrapCollisionComponent.h"
#include "CDamegeTrapPoint.h"

//===== �}�N����` =====


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CDamageTrapCollisionComponent::Start()
{
	m_col = m_pParent->GetComponent<CCollision2D>();
	m_res = CObjectManager::SearchObjectName("Player").lock()->GetComponent<CRespawnComponent>();

	m_pParent->m_pTrans->SetRotation(m_pParent->m_pTrans->GetRotation());
	// �I�u�W�F�N�g�̑傫����ύX�@�����ꂾ�Ǝ΂߂̎��ɑ傫���Ȃ�̂Ō�ŕύX
	m_pParent->m_pTrans->SetScale({ (m_pParent->m_pTrans->GetScale()->x * 2),m_pParent->m_pTrans->GetScale()->y * 2,100 });
	// �����蔻��̑傫���ύX�@����L���l�A�΂߂��Ƒ傫���Ȃ�
	m_col.lock()->SetScale({ m_pParent->m_pTrans->GetScale()->x,m_pParent->m_pTrans->GetScale()->y,100 });
}


//===================================
//
//	�X�V�֐�
//
//===================================
void CDamageTrapCollisionComponent::Update()
{
}


//===================================
//
//	��X�V�֐�
//
//===================================
void CDamageTrapCollisionComponent::LateUpdate()
{

}


//===================================
//
//	�I���֐�
//
//===================================
void CDamageTrapCollisionComponent::End()
{
	CObjectManager::DestroyObject(m_pParent);
}


//===== �g��Ȃ����͏����Ă�������() =====
//===================================
//
//	2D�����蔻��(Enter) �R�[���o�b�N�֐�
//
//===================================
void CDamageTrapCollisionComponent::OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// �^�O�ȂǂŔ���
	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		m_res.lock()->Respawn();
	}
}