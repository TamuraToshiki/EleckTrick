//======================================================================
//												CCheckPoint.cpp
//	�`�F�b�N�|�C���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/13	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CCheckPoint.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CCheckPointComponent.h"


//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/Resporn/Resporn.fbx"
#define TEXTURE_PATH			"data/model/Resporn/Resporn_Tex10.png"


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CCheckPoint::CCheckPoint()
{
	// �g�����X�t�H�[��
	m_pTrans->m_pos->y -= 50;

	// �^�O�̐ݒ�
	m_name = m_tag = "CheckPoint";

	// ���f���̃��[�h
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();

	AddComponent<CCheckPointComponent>();

	// �����蔻��
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
CCheckPoint::~CCheckPoint()
{

}
