//======================================================================
//												CDamageTrapObj.cpp
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
#include "CDamageTrapObj.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CDamageTrapObjComponent.h"

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
CDamageTrapObj::CDamageTrapObj()
{
	// �g�����X�t�H�[��
	m_pTrans;

	// �^�O�̐ݒ�
	m_name = m_tag = "DamageTrap";

	// ���f���̃��[�h
	//m_renderer.lock()->SetMeshBlendState(BS_ADDITIVE);
	//m_renderer.lock()->SetDiffuseColor({ 0,1,0,1 });

	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	AddComponent<CDamageTrapObjComponent>();

	col->SetScale({ 50,50,50 });

}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CDamageTrapObj::~CDamageTrapObj()
{

}
