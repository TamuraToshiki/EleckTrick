//==================================================================
//								MenuScript.cpp
//	���j���[�R���|�[�l���g�̈ڐA
//
//==================================================================
//	author :	AT13C 12 �匴����
//==================================================================
//	�J������
//
//	2021/06/17	�ڐA�J�n
//
//===================================================================


//====== �C���N���[�h�� ======

// ���g
#include "MenuScript.h"

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



//******************** �X�N���v�g���\�b�h ********************


//========================================
//
//	�J�n��
//
//========================================
void MenuScript::Start()
{
	m_fTimer = 0.0f;
	m_bOpen = true;
	m_bButton = false;

	if (m_bOpen)
	{
		m_nMenuIndex = 0;
	}

	//�I�����I�u�W�F�N�g���쐬
	
}

//========================================
//
//	�X�V��
//
//========================================
void MenuScript::Update()
{
}

//========================================
//
//	��X�V��
//
//========================================
void MenuScript::LateUpdate()
{
}

//========================================
//
//	�I����
//
//========================================
void MenuScript::End()
{
}


//---------------------------------------
//
//	�C���f�b�N�X�擾
//
//---------------------------------------
int MenuScript::GetIndex()
{
	return m_nMenuIndex;
}

//---------------------------------------
//
//	�\���t���O�擾
//
//---------------------------------------
bool MenuScript::GetOpenFrag()
{
	return m_bOpen;
}

//---------------------------------------
//
//	�\���t���O�ݒ�
//
//---------------------------------------
void MenuScript::SetOpenFlag(bool bFlag)
{
	m_bOpen = bFlag;
}

//******************** �R�[���o�b�N�֐� ********************


//========================================
//
// ����������
//
//========================================
void MenuScript::OnCollisionEnter(Collider* collider)
{

}

//========================================
//
// �������Ă����
//
//========================================
void MenuScript::OnCollisionStay(Collider* collider)
{

}

//========================================
//
// ���ꂽ��
//
//========================================
void MenuScript::OnCollisionExit(Collider* collider)
{

}


//===== ECS =====

//========================================
//
// ����������
//
//========================================
void MenuScript::OnDeltaCollisionEnter(DeltaCollider* collider)
{
}

//========================================
//
// �������Ă����
//
//========================================
void MenuScript::OnDeltaCollisionStay(DeltaCollider* collider)
{
}

//========================================
//
// ���ꂽ��
//
//========================================
void MenuScript::OnDeltaCollisionExit(DeltaCollider* collider)
{
}

