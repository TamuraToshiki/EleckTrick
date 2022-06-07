//======================================================================
//												CDebugObject.cpp
//	�f�o�b�N�p�I�u�W�F�N�g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/11/26	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CDebugObject.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CAnimater2D.h"
#include "CTestComponent.h"

//===== �}�N����` =====
#define TEXTURE_PATH			"data/texture/circle2.png"


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CDebugObject::CDebugObject()
{
	// �g�����X�t�H�[��
	//m_pTrans->SetScale({ SCREEN_WIDTH, SCREEN_HEIGHT, 1 });


	// �^�O�̐ݒ�
	m_name = m_tag = "DebugObject";

	// ���b�V���̑I��
	//m_renderer.lock()->SetTexture(TEXTURE_PATH);
	//m_renderer.lock()->SetMeshBlendState(BS_ADDITIVE);
	//m_renderer.lock()->SetLayer(CSpriteRenderer::eFade1);
	//m_renderer.lock()->SetMeshBlendState(EBlendState::BS_SUBTRACTION);
	//m_renderer.lock()->SetTexSize({ 2, 2, 0 });
	//m_renderer.lock()->SetTexPos({ -1.0, -1.0, 0 });

	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();
	const auto& am = AddComponent<CAnimator2D>();

	// �R���W����
	col->SetScale({ 100, 100, 100 });

	// ���W�b�h�{�f�B
	rb->SetSolid(true);
	rb->SetUseGravity(false);


	// �A�j���[�^�[
	//am->SetSplit(9, 9);
	//am->SetAnimationSpeed(1);
	//am->AddAnimationDefault("test", 9*9);

	//am->SetAnimation("test");
}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CDebugObject::~CDebugObject()
{

}
