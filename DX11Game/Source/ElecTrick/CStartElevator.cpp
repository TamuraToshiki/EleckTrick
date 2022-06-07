//======================================================================
//												CStartElevator.cpp
//	�X�^�[�g�G���x�[�^�[
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/01	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CStartElevator.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CStartElevatorComponent.h"


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
CStartElevator::CStartElevator()
{
	// �g�����X�t�H�[��

	// �^�O�̐ݒ�
	m_name = m_tag = "StartElevator";

	// ���b�V���̑I��
	

	// ���f���̃��[�h
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);


	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();

	AddComponent<CStartElevatorComponent>();

	// �R���W����
	col->SetScale({ 200, 5, 400 });

	// ���W�b�h�{�f�B
	rb->SetSolid(true);
	rb->SetUseGravity(false);
}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CStartElevator::~CStartElevator()
{

}
