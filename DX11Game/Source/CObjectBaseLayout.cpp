//======================================================================
//												CObjectBaseLayout.cpp
//	Assimp���f���x�[�X�̃I�u�W�F�N�g���C�A�E�g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/11/16	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CObjectBaseLayout.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"

//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/box.fbx"


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CObjectBaseLayout::CObjectBaseLayout()
{
	// �g�����X�t�H�[��
	m_pTrans;

	// �^�O�̐ݒ�
	m_name = m_tag = "Object";

	// ���f���̃��[�h
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);

	//--- �R���|�[�l���g�̒ǉ�
	AddComponent<CCollision2D>();
	AddComponent<CRigidbody2D>();

}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CObjectBaseLayout::~CObjectBaseLayout()
{

}
