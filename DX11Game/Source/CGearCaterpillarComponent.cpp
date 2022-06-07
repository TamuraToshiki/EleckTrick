//======================================================================
//											CGearCaterpillarComponent.h
//	�M�A�E�L���^�s�������R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2021/01/20	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CGearCaterpillarComponent.h"

//===== �}�N����` =====


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CGearCaterpillarComponent::Start()
{

}


//===================================
//
//	�X�V�֐�
//
//===================================
void CGearCaterpillarComponent::Update()
{

}


//===================================
//
//	��X�V�֐�
//
//===================================
void CGearCaterpillarComponent::LateUpdate()
{
	const auto& player = m_player.lock();
	if (!player) return;

	// ���W�̓���
	m_pParent->m_pTrans->m_pos = m_player.lock()->m_pos;
	m_pParent->m_pTrans->m_rot = m_player.lock()->m_rot;
	m_pParent->m_pTrans->m_scale = m_player.lock()->m_scale;
}


//===================================
//
//	�I���֐�
//
//===================================
void CGearCaterpillarComponent::End()
{

}

