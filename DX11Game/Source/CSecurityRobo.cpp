//======================================================================
//												CSecurityRobo.cpp
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
#include "CSecurityRobo.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CSecurityRoboComponent.h"
#include "CObjectManager.h"

//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/Enemy_Anim_Blender/Enemy_Anim_Blender.fbx"
#define TEXTURE_PATH			"data/model/EnemyA_Mix/EnemyA_Tex4.png"


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CSecurityRobo::CSecurityRobo()
{
	// �g�����X�t�H�[��
	m_pTrans;

	// �^�O�̐ݒ�
	m_tag = "SecurityRobo";

	// ���f���̃��[�h
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	AddComponent<CRigidbody2D>();
	AddComponent<CSecurityRoboComponent>();

	col->SetScale({ 80.0f,80.0f,100.0f });
	col->SetOffSet({ 0, 40, 0 });
}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CSecurityRobo::~CSecurityRobo()
{

}
