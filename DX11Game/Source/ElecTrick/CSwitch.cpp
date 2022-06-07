//======================================================================
//												CSwitch.cpp
//	Assimp���f���x�[�X�̃I�u�W�F�N�g���C�A�E�g
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/11/18	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CSwitch.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CDoorComponent.h"
#include "CSwitchComponent.h"
#include "CResourceEnergy.h"


//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/Electrode/Electrode.fbx"
#define TEXTURE_PATH			"data/model/Electrode/Electrode.png"


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CSwitch::CSwitch()
{
	// �g�����X�t�H�[��
	//m_pTrans->SetScale({ 25.0f,25.0f,25.0f });
	m_pTrans->m_pos->z += 100;
	m_pTrans->m_rot->x = -90;

	// �^�O�̐ݒ�
	m_name = m_tag = "Switch";

	// ���f���̃��[�h
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();

	// �X�C�b�`
	AddComponent<CSwitchComponent>();
	AddComponent<CResourceEnergy>();

	// �R���W����
	col->SetScale({ 25.0f, 25.0f, 25.0f });

	// ���W�b�h�{�f�B
	rb->SetSolid(true);
	rb->SetUseGravity(false);

}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CSwitch::~CSwitch()
{

}


