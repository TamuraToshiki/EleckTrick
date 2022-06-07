//==================================================================
//								EndingFadeEndingFadeScript.cpp
//	�G���f�B���O���o�p�t�F�[�h�X�N���v�g
//
//==================================================================
//	author :	AT13C 12 �匴����
//==================================================================
//	�J������
//
//	2021/06/18	�ڐA�J�n
//
//===================================================================


//====== �C���N���[�h�� ======

// ���g
#include "EndingFadeScript.h"

// �V�X�e��
#include "../Engine/System/input.h"
#include "../Engine/System/Sound.h"

// �}�l�[�W���[
#include "../Engine/ECS/Entity/EntityManager.h"

// �G���e�B�e�B
#include "../Engine/ECSEntity/GameObject.h"

// �R���|�[�l���g
#include "../Engine/ECSCompoent/Transform.h"
#include "../Engine/ECSCompoent/MeshRenderer.h"
#include "../Engine/ECSCompoent/BillboardRenderer.h"
#include "../Engine/ECSCompoent/AssimpRenderer.h"
#include "../Engine/ECSCompoent/SpriteRenderer.h"
#include "../Engine/ECSCompoent/InstancingMeshRenderer.h"
#include "../Engine/ECSCompoent/Rigidbody.h"
#include "../Engine/ECSCompoent/BoxCollider.h"
#include "../Engine/ECSCompoent/SphereCollider.h"

// ECS�R���|�[�l���g
#include "../Engine/ECSCompoent/DeltaCollider.h"

// ECS�V�X�e��
#include "../Engine/ECSSystem/DeltaCollisionSystem.h"


// �l�[���X�y�[�X
using namespace ECS;


//===== �}�N����` =====
#define ADD_TIMER	(0.1f)

//******************** �X�N���v�g���\�b�h ********************


//========================================
//
//	�J�n��
//
//========================================
void EndingFadeScript::Start()
{
	//�^�O�E���O�ݒ�
	gameObject().lock()->SetName("Ending");
	gameObject().lock()->SetTag("Ending");

	//������
	m_fTimer = 0.0f;
	m_fAlpha = 1.0f;
	m_fAlpha2 = 0.0f;
	m_fAlpha3 = 0.0f;
	m_bFadeEnd = false;
	m_bMoveEnd = false;

	//�����_���[
	//const auto& renderer = gameObject().lock()->AddComponent<SpriteRenderer>();
	//�e�N�X�`���̃Z�b�g
	//���C���[�̃Z�b�g
	//�e�N�X�`���̃T�C�Y�Z�b�g
	//�i�[
	
}

//========================================
//
//	�X�V��
//
//========================================
void EndingFadeScript::Update()
{
	//�^�C�}�[�J�n
}

//========================================
//
//	��X�V��
//
//========================================
void EndingFadeScript::LateUpdate()
{
}

//========================================
//
//	�I����
//
//========================================
void EndingFadeScript::End()
{
}


//******************** �R�[���o�b�N�֐� ********************


//========================================
//
// ����������
//
//========================================
void EndingFadeScript::OnCollisionEnter(Collider* collider)
{

}

//========================================
//
// �������Ă����
//
//========================================
void EndingFadeScript::OnCollisionStay(Collider* collider)
{

}

//========================================
//
// ���ꂽ��
//
//========================================
void EndingFadeScript::OnCollisionExit(Collider* collider)
{

}


//===== ECS =====

//========================================
//
// ����������
//
//========================================
void EndingFadeScript::OnDeltaCollisionEnter(DeltaCollider* collider)
{
}

//========================================
//
// �������Ă����
//
//========================================
void EndingFadeScript::OnDeltaCollisionStay(DeltaCollider* collider)
{
}

//========================================
//
// ���ꂽ��
//
//========================================
void EndingFadeScript::OnDeltaCollisionExit(DeltaCollider* collider)
{
}

