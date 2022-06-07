//======================================================================
//												CBg.cpp
//	2D�w�i
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/01	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CBg.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"


//===== �}�N����` =====
#define TEXTURE_PATH			"data/texture/wall/4.jpg"


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===== �ÓI�����o =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CBg::CBg()
{
	// �g�����X�t�H�[��
	m_pTrans->SetScale({ 100, 100, 100 });
	m_pTrans->m_rot->x = -90;


	// �^�O�̐ݒ�
	m_name = m_tag = "Bg";

	// ���b�V���̑I��
	m_renderer.lock()->MakePlane(m_tag, 50, 50, 100, 100, 100, 100);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);
	m_renderer.lock()->SetNormalTexture("data/texture/normal/Cross_Normal.png");
	m_renderer.lock()->SetCullMode(false);
	m_renderer.lock()->SetZBufferFlg(false);
	//m_renderer.lock()->SetAmbientTexture("data/texture/skydome.png");

	//--- �R���|�[�l���g�̒ǉ�

}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CBg::~CBg()
{

}
