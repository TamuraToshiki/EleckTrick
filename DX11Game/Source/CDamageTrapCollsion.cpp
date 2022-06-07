//======================================================================
//												CDamageTrapCollision.cpp
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
#include "CDamageTrapCollsion.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CDamageTrapCollisionComponent.h"

//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/box.fbx"
//#define TEXTURE_PATH			"data/model/trap.png"		


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CDamageTrapCollision::CDamageTrapCollision()
{
	// �g�����X�t�H�[��
	m_pTrans;

	// �^�O�̐ݒ�
	m_name = m_tag = "DamageTrapCol";

	// ���f���̃��[�h
	//m_renderer.lock()->SetMeshBlendState(BS_ADDITIVE);
	//m_renderer.lock()->SetDiffuseColor({ 1,0,0,1 });

	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	col->SetScale({ 100,100,100 });
	AddComponent<CDamageTrapCollisionComponent>();
}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CDamageTrapCollision::~CDamageTrapCollision()
{

}
