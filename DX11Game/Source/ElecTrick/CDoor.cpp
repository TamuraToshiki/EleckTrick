//======================================================================
//												CDoor.cpp
//	�h�A
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/11/20	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CDoor.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CDoorComponent.h"

//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/Door/Door.fbx"
#define TEXTURE_PATH			"data/model/Door/Door.png"


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CDoor::CDoor()
{
	// �g�����X�t�H�[��
	//m_pTrans->SetScale({25.0f,300.0f,200.0f});
	//m_pTrans->m_rot->y = 90;
	m_pTrans->m_pos->y -= 50;

	// �^�O�̐ݒ�
	m_name = m_tag = "Door";

	// ���f���̃��[�h
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();
	const auto& dc = AddComponent<CDoorComponent>();

	//GetComponent<CDoorComponent>()->CreateSwitch(Vector3{100, 0, 0});
	//GetComponent<CDoorComponent>()->CreateSwitch(Vector3{200, 0, 0});
	//GetComponent<CDoorComponent>()->CreateSwitch(Vector3{300, 0, 0});

	// �R���W����
	col->SetScale({ 50.0f, 200.0f, 200.0f });
	col->SetOffSet({ 0, 100, 0 });

	// ���W�b�h�{�f�B
	rb->SetSolid(true);
	rb->SetUseGravity(false);

}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CDoor::~CDoor()
{

}
