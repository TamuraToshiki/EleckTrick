//======================================================================
//												CFadeObject.cpp
//	�t�F�[�h�I�u�W�F�N�g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/06	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CFadeObject.h"
#include "CFadeComponent.h"


//===== �}�N����` =====
#define TEXTURE_PATH			"data/texture/circle2.png"


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CFadeObject::CFadeObject()
{
	// �g�����X�t�H�[��
	m_pTrans->SetScale({ SCREEN_WIDTH, SCREEN_HEIGHT, 1 });

	// �^�O�̐ݒ�
	m_name = m_tag = "FadeObject";

	// ���b�V���̑I��
	m_renderer.lock()->SetTexture(TEXTURE_PATH);
	m_renderer.lock()->SetLayer(CSpriteRenderer::eFade1);
	//m_renderer.lock()->SetMeshBlendState(EBlendState::BS_ADDITIVE);
	m_renderer.lock()->SetMeshBlendState(EBlendState::BS_SUBTRACTION);

	//--- �R���|�[�l���g�̒ǉ�
	AddComponent<CFadeComponent>();

}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CFadeObject::~CFadeObject()
{

}
