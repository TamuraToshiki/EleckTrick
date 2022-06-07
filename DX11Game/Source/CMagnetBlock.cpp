//======================================================================
//												CMagnetBlock.cpp
//	�d���΃u���b�N
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/11/16	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CMagnetBlock.h"
#include "CMagnetBlockComponent.h"
#include "CTestComponent.h"


//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/Magnet/Magnet.fbx"
#define TEXTURE_PATH			"data/model/Magnet/Magnet.png"

//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CMagnetBlock::CMagnetBlock()
{
	// �g�����X�t�H�[��

	// �^�O�̐ݒ�
	m_name = m_tag = "MagnetBlock";

	// ���f���̃��[�h
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();
	AddComponent<CMagnetBlockComponent>();

	// �R���W����
	col->SetScale({ 100, 100, 100 });

	// ���W�b�h�{�f�B
	rb->SetSolid(true);
	rb->SetUseGravity(false);

}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CMagnetBlock::~CMagnetBlock()
{

}
