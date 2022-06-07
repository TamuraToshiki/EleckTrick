//======================================================================
//												CStandStick.cpp
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
#include "CStandStick.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CStandStickComponent.h"

//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/box.fbx"


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CStandStick::CStandStick()
{
	// �g�����X�t�H�[��
	m_pTrans;

	// �^�O�̐ݒ�
	m_tag = "StandStick";

	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	AddComponent<CStandStickComponent>();

	col->SetScale({ 20.0f,20.0f,20.0f });
	col->SetOffSet({ 0.0f,-50.0f,0.0f });
}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CStandStick::~CStandStick()
{

}
