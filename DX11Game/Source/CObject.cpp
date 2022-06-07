//==================================================================
//								CObject.h
//	�I�u�W�F�N�g�x�[�X�N���X
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2020/09/23	�I�u�W�F�N�g�N���X�쐬
//
//===================================================================


//===== �C���N���[�h�� =====
#include "CObject.h"
#include "CObjectManager.h"


//===== �}�N����` =====


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CObject::CObject()
{
	m_pTrans = this->AddComponent<CTransform>();
	// �����ō��W����ꂽ��!
	m_pTrans->m_pos = CObjectManager::m_posBuffer;
	m_pTrans->m_rot = CObjectManager::m_rotBuffer;
	m_pTrans->m_scale = CObjectManager::m_scaleBuffer;

	m_name = m_tag = "Object";
	m_bStop = false;
}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CObject::~CObject()
{
	// ���X�g���N���A
	m_ComponentList.clear();
	m_DestroyComponentList.clear();
}

