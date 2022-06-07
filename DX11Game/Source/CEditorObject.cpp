//======================================================================
//												CEditorObject.cpp
//	�G�f�B�^�[�̃I�u�W�F�N�g
//
//======================================================================
//	author :	AT12A 12 �I��
//======================================================================
//	�J������
//	2020/12/03	�쐬
//				�I����������p��/�匴
//======================================================================


//===== �C���N���[�h�� =====
#include "CEditorObject.h"
#include "CEditorComponent.h"


//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/box.fbx"


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CEditorObject::CEditorObject()
{
	// �g�����X�t�H�[��
	m_pTrans->m_scale;

	// �^�O�̐ݒ�
	m_name = m_tag = "Editor";

	// ���f���̃��[�h
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture("data/texure/white.png");
	m_renderer.lock()->SetMeshBlendState(EBlendState::BS_ALPHABLEND);
	m_renderer.lock()->SetDiffuseColor({ 0,1,0,1.0f });


	//--- �R���|�[�l���g�̒ǉ�
	AddComponent<CEditorComponent>();
}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CEditorObject::~CEditorObject()
{

}
