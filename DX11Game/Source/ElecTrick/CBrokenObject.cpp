//======================================================================
//												CBrokenObject.cpp
//	Assimp���f���x�[�X�̃I�u�W�F�N�g���C�A�E�g
//
//======================================================================
//	author :	AT12A 20 �y������
//======================================================================
//	�J������
//	2020/12/03	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CBrokenObject.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CBrokenComponent.h"

//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/BreakBrock/BreakBrock.fbx"
#define TEXTURE_PATH			"data/texture/BreakBrock/BreakBrock.png"


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CBrokenObject::CBrokenObject()
{
	// �g�����X�t�H�[��
	//m_pTrans->SetScale({ 1, 1, 1 });

	// �^�O�̐ݒ�
	m_tag = "BrokenObj";

	// ���f���̃��[�h
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();
	AddComponent<CBrokenComponent>();


	// �R���W����
	col->SetScale({ 100, 100, 100 });

	// ���W�b�h�{�f�B
	rb->SetSolid(true);
	rb->SetUseGravity(false);

}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CBrokenObject::~CBrokenObject()
{

}
