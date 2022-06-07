//======================================================================
//												CElevatorRoad.cpp
//	�G���x�[�^�[���[�h
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/05	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CElevatorRoad.h"


//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/Ev_Road/Ev_Road.fbx"
#define TEXTURE_PATH			"data/model/Ev_Road/Ev_Road_Tex.png"

//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CElevatorRoad::CElevatorRoad()
{
	// �g�����X�t�H�[��

	// �^�O�̐ݒ�
	m_name = m_tag = "ElevatorRoad";

	// ���b�V���̑I��
	

	// ���f���̃��[�h
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);


	//--- �R���|�[�l���g�̒ǉ�

}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CElevatorRoad::~CElevatorRoad()
{

}