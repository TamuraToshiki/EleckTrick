//======================================================================
//												CDropObject.cpp
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
#include "CDropObject.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CDropComponent.h"

//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/FallBrock/FallBrock.fbx"
#define TEXTURE_PATH			"data/model/FallBrock/FallBrock.png"


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CDropObject::CDropObject()
{
	// �g�����X�t�H�[��
	m_pTrans->m_pos->y -= 50;

	// �^�O�̐ݒ�
	m_tag = "DropObj";

	// ���f���̃��[�h
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb =  AddComponent<CRigidbody2D>();
	AddComponent<CDropComponent>();


	// �R���W����
	col->SetScale({ 100, 100, 100 });
	col->SetOffSet({ 0,50,0 });

	// ���W�b�h�{�f�B
	rb->SetSolid(true);
	rb->SetUseGravity(false);
	rb->SetMass(4);
}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CDropObject::~CDropObject()
{

}
