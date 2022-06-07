//======================================================================
//												CLift.cpp
//	���t�g
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/11/18	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CLift.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"

#include "CLiftComponent.h"


//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/Lift/Lift.fbx"
#define TEXTURE_PATH			"data/model/Lift/Lift_Tex2.png"

//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CLift::CLift()
{
	// �g�����X�t�H�[��

	// �^�O�̐ݒ�
	m_name = m_tag = "Lift";

	// ���b�V���̑I��
	

	// ���f���̃��[�h
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);


	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();

	AddComponent<CLiftComponent>();

	// �R���W����
	col->SetScale({ 100, 20, 100 });

	// ���W�b�h�{�f�B
	rb->SetSolid(true);
	rb->SetUseGravity(false);
}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CLift::~CLift()
{

}
