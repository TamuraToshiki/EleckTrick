//======================================================================
//												CEnegyBullet.cpp
//	�G�l���M�[�e
//
//======================================================================
//	author :	AT12A 15 �匴����
//======================================================================
//	�J������
//	2020/11/26	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CEnergyBullet.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CAnimater2D.h"
#include "CBulletComponent.h"
#include "CObject.h"
#include "CComponent.h"
#include "CResourceEnergy.h"


//===== �}�N����` =====
#define TEXTURE_PATH			"data/texture/ElectricBall_Sprite2.png"


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CEnergyBullet::CEnergyBullet()
{
	// �g�����X�t�H�[��
	m_pTrans->SetScale({ 100, 100, 100 });

	// �^�O�̐ݒ�
	m_name = m_tag = "EnergyBullet";

	// ���b�V���̑I��
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);
	m_renderer.lock()->SetMeshBlendState(BS_ADDITIVE);

	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();
	const auto& am = AddComponent<CAnimator2D>();
	const auto& bullet = AddComponent<CBulletComponent>();
	AddComponent<CResourceEnergy>();

	// �R���W����
	col->SetScale({ 50, 50, 50 });

	// ���W�b�h�{�f�B
	//rb->SetSolid(true);
	rb->SetUseGravity(false);
	rb->SetGravityForce(0);
	rb->SetDragX(0);
	rb->SetTrigger(true);


	// �A�j���[�V����
	am->SetAnimationSpeed(1);
	am->SetSplit(9, 9);
	am->AddAnimationDefault("test", 9 * 9);
	am->SetAnimation("test");
}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CEnergyBullet::~CEnergyBullet()
{

}
