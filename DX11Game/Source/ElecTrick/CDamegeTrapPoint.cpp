//======================================================================
//												CDamegeTrapPoint.cpp
//	�_���[�W�g���b�v
//
//======================================================================
//	author :	AT12A 18�@�c���q��
//======================================================================
//	�J������
//	2020/12/14	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CDamegeTrapPoint.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"


//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/DamageTrap/DamageTrap.fbx"
#define TEXTURE_PATH			"data/model/DamageTrap/DamageTrap_On.png"		


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CDamegeTrapPoint::CDamegeTrapPoint()
{
	// �g�����X�t�H�[��
	m_pTrans->m_rot->x = -90;
	m_pTrans->m_pos->z += 50;

	// �^�O�̐ݒ�
	m_name = m_tag = "DamageTrapPoint";

	// ���f���̃��[�h
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);
	//m_renderer.lock()->SetMeshBlendState(BS_ADDITIVE);
	//m_renderer.lock()->SetDiffuseColor({ 0,1,1,1 });

	//--- �R���|�[�l���g�̒ǉ�

}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CDamegeTrapPoint::~CDamegeTrapPoint()
{

}
