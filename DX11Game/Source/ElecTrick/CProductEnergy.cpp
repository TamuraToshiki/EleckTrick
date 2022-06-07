//======================================================================
//											CProductEnergy.cpp
//	���d�p�̃R���|�[�l���g
//
//======================================================================
//	author :	AT12A 15 �匴����
//======================================================================
//	�J������
//	2020/12/11	�쐬
//				�w�b�_�[�ɏ����Ă������̂�cpp��
//======================================================================


//===== �C���N���[�h�� =====
#include "CProductEnergy.h"
#include "Sound.h"
#include "CObjectManager.h"
#include "CAssimpObject.h"
#include "CGearCaterpillarComponent.h"
#include "CBillboardObject.h"
#include "CEffectComponent.h"
#include "CSpriteEffectComponent.h"
#include "CAnimater2D.h"


#include "debugproc.h"

//===== �}�N����` =====
#define PROPELLER_SIZE_X (259 / 2.0f)
#define PROPELLER_SIZE_Y (265 / 2.0f)


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CProductEnergy::Start()
{
	m_resource = m_pParent->GetComponent<CResourceEnergy>();
	m_fAdd = 0.0f;
	m_fSub = 0.0f;
	m_bTrigger_Z = false;
	m_bTrigger_C = false;

	// ���[�^�[��]��
	m_fRotSpeed = 0.0f;
	m_fMaxRotSpeed = 0.3f;
	m_fAddRotSpeed = 0.012f;
	m_fSubRotSpeed = 0.0008f;

	// ���[�^�[
	const auto& gene = CObjectManager::CreateObject<CAssimpObject>();
	gene->AddComponent<CGearCaterpillarComponent>()->SetPlayer(m_pParent->m_pTrans);
	m_generater = gene->GetComponent<CAssimpRenderer>();
	m_generater.lock()->ModelLoad("data/model/MainAnim_Blender/Generare_Anim_Blender.fbx");
	m_generater.lock()->SetDiffuseTexture("data/model/Model_Main/texture_10.png");
	m_generater.lock()->SetAnimIndex(1);
	m_generater.lock()->SetAnimSpeed(1);
	m_generater.lock()->PlayAnim();

	// �v���y��
	const auto& prope = CObjectManager::CreateObject<CSpriteObject>();
	prope->GetComponent<CSpriteRenderer>()->SetTexture("data/texture/propeller.png");
	m_propeller = prope->AddComponent<CRigidbody2D>();
	m_propeller.lock()->SetSolid(true);
	m_propeller.lock()->SetGravityForce(0.0f);
	m_propeller.lock()->SetMoveLimit(false);

	prope->m_pTrans->m_pos	 = 
		Vector3{ -SCREEN_CENTER_X + PROPELLER_SIZE_X, SCREEN_CENTER_Y - PROPELLER_SIZE_Y / 8 * 7, 0.0f };
	prope->m_pTrans->m_scale = Vector3{PROPELLER_SIZE_X, PROPELLER_SIZE_Y, 0};

}


//===================================
//
//	�X�V�֐�
//
//===================================
void CProductEnergy::Update()
{
	if (m_fRotSpeed > m_fMaxRotSpeed)
		m_fRotSpeed = m_fMaxRotSpeed;

	// ����
	m_fRotSpeed -= m_fSubRotSpeed;
	if (m_fRotSpeed <= 0)
	{
		m_fRotSpeed = 0;
		m_resource.lock()->AddEnergy(-0.1f);
	}

	// ��]���𔽉f
	m_resource.lock()->AddEnergy(m_fRotSpeed);
	// UI
	m_propeller.lock()->SetTorqueZ(-m_fRotSpeed * 100.0f);


	//--- ��]���ɉ����ăG�t�F�N�g�𐶐�����
	if (m_fRotSpeed <= 0.001f) return;

	m_nEffectCount--;
	if (m_nEffectCount > 0) return;
	m_nEffectCount = 32 - m_fRotSpeed * 100;


	// ���W
	Vector3 pos = m_pParent->m_pTrans->m_pos;
	pos->x += rand() % 80 - 40;
	pos->y += rand() % 100;
	//pos->z -= 50;

	Vector3 rot;
	rot->z = rand() % 360;
	float size = rand() % 150;
	Vector3 scale = { size, size,100 };


	const auto& obj = CObjectManager::CreateObject<CObject>(pos, rot, scale);
	const auto& anim = obj->AddComponent<CEffectComponent>();
	char szName[256];
	sprintf_s(szName, "data/texture/DamageTexture/Elect%d.png", rand() % 3 + 1);
	anim->SetAnyEffect(szName, 4, 4, rand() % 3 + 1);


	//--- �v���y��UI�G�t�F�N�g
	pos = m_propeller.lock()->m_pParent->m_pTrans->m_pos;
	pos->x += rand() % 150 - 75;
	pos->y += rand() % 150 - 75;
	//pos->z -= 50;

	rot->z = rand() % 360;
	size = rand() % 250;
	scale = Vector3{ size,size,1 };


	const auto& ui = CObjectManager::CreateObject<CObject>(pos, rot, scale);
	const auto& uiAnim = ui->AddComponent<CSpriteEffectComponent>();
	sprintf_s(szName, "data/texture/DamageTexture/Elect%d.png", rand() % 3 + 1);
	uiAnim->SetAnyEffect(szName, 4, 4, rand() % 3 + 1);
	

}


//===================================
//
//	��X�V�֐�
//
//===================================
void CProductEnergy::LateUpdate()
{
	// �A�j���[�V����
	m_generater.lock()->SetAnimSpeed(m_fRotSpeed);
}


//===================================
//
//	�I���֐�
//
//===================================
void CProductEnergy::End()
{
	// ���ɏ����Ȃ�
}

//--------------------------------------------------
//	���Z�ʃZ�b�g
//--------------------------------------------------
void CProductEnergy::SetAdd(float add)
{
	m_fAdd = add;
}

//--------------------------------------------------
//	���Z�ʃZ�b�g
//--------------------------------------------------
void CProductEnergy::SetSub(float sub)
{
	m_fSub = sub;
}

//--------------------------------------------------
//	���d	
//--------------------------------------------------
bool CProductEnergy::GenerateEnergyLeft(void)
{
	//�Е��̃{�^���A�ł݂̂̔��d��h�~
	if (!m_bTrigger_Z)
	{
		m_bTrigger_Z = true;
		m_bTrigger_C = false;
		
		// ��]�������Z
		m_fRotSpeed += m_fAddRotSpeed;
		if (m_fRotSpeed > m_fMaxRotSpeed)
			m_fRotSpeed = m_fMaxRotSpeed;

		// �T�E���h
		CSound::PlaySE("PlayerMotor.wav");

		// �A�j���[�V����


		return true;
	}

	return false;
}

//--------------------------------------------------
//	���d	
//--------------------------------------------------
bool CProductEnergy::GenerateEnergyRight(void)
{
	//�Е��̃{�^���A�ł݂̂̔��d��h�~
	if (!m_bTrigger_C)
	{
		m_bTrigger_C = true;
		m_bTrigger_Z = false;

		// ��]�������Z
		m_fRotSpeed += m_fAddRotSpeed;
		if (m_fRotSpeed > m_fMaxRotSpeed)
			m_fRotSpeed = m_fMaxRotSpeed;

		// �T�E���h
		CSound::PlaySE("PlayerMotor.wav");

		return true;
	}

	return false;
}


//--------------------------------------------------
//	����
//--------------------------------------------------
void CProductEnergy::ExpenseEnergy(void)
{
	m_resource.lock()->SubEnergy(m_fSub);
}