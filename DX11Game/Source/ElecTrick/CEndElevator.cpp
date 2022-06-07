//======================================================================
//												CEndElevator.cpp
//	�G���h�G���x�[�^�[
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/03	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CEndElevator.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CEndElevatorComponent.h"


//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/Ev/Elevator_Ainm3.fbx"
#define TEXTURE_PATH			"data/model/Ev/Ev_4.png"

//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CEndElevator::CEndElevator()
{
	// �g�����X�t�H�[��

	// �^�O�̐ݒ�
	m_name = m_tag = "EndElevator";

	// ���b�V���̑I��
	

	// ���f���̃��[�h
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();

	AddComponent<CEndElevatorComponent>();

	// �R���W����
	col->SetScale({ 200, 1, 400 });

	// ���W�b�h�{�f�B
	rb->SetSolid(true);
	rb->SetUseGravity(false);
}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CEndElevator::~CEndElevator()
{

}
