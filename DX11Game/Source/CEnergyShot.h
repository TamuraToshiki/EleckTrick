//======================================================================
//											CEnergyShot.h
//	�G�l���M�[�e����
//
//======================================================================
//	author :	AT12A 15 �匴����
//======================================================================
//	�J������
//	2020/11/24	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CResourceEnergy.h"
#include "CObjectManager.h"
#include "input.h"
#include "CEnergyBullet.h"
#include "CPlayerActionComponent.h"
#include "CBulletComponent.h"
#include "Sound.h"


//===== �N���X��` =====
class CEnergyShot : public CComponent
{
private:
	// --- �ϐ��錾 ---
	float m_fResourceShot;	//�e�̃G�l���M�[�� 
	int   m_nLevel;			//�e�̃��x��
	float m_shotForce;		//�e�̑��x
	float m_rimit;			//�e�̐�������
	std::weak_ptr<CResourceEnergy> m_resource;	//���\�[�X
	std::weak_ptr<CPlayerActionComponent> m_input;		//�C���v�b�g

public:
	// --- �֐��錾 ---

	// �쐬���ɌĂ΂�܂�
	void Start() override
	{
		m_resource = m_pParent->GetComponent<CResourceEnergy>();
		m_input = m_pParent->GetComponent<CPlayerActionComponent>();
		m_shotForce = 10.0f;
	}

	// ���t���[���Ă΂�܂�
	void Update() override
	{
	}

	// ���t���[���̍X�V��ɌĂ΂�܂�
	void LateUpdate() override
	{
	}


	//===== �g��Ȃ����͏����Ă�������() =====
	//--- �R�[���o�b�N�֐� ---
	// 2D�����蔻��
	void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override
	{
		// �^�O�ȂǂŔ���
		if (collsion2d)
		{
			
		}
	}

	//--------------------------------------------------
	//	�e����
	//--------------------------------------------------
	bool CreateShot()
	{
		const auto& resource = m_resource.lock();
		if (!resource) return false;
		if (resource->GetResource() < 0.5f) return false;

		m_nLevel = resource->GetLevel();

		if (m_nLevel != ZERO)
		{
			//���\�[�X�̃R�s�[�E���Z
			//m_fResourceShot = resource->GetResource();
			////resource->SubEnergy(m_fResourceShot);
			//resource->ClaerResource();
			resource->ShotUsedResource(10);
			m_fResourceShot = 10.0f;

			//�I�u�W�F�N�g�̐���
			const auto &bullet = CObjectManager::CreateObject<CEnergyBullet>();
			const auto &rb     = bullet->GetComponent<CRigidbody2D>();
			const auto &br     = bullet->GetComponent<CResourceEnergy>();

			const auto &input = m_input.lock();
			

			if (rb && input && br)
			{
				Vector3 dir = input->GetPlayerDir();
				Vector3 pos = m_pParent->m_pTrans->m_pos;
				pos->y += 50;

				rb->SetForce(dir * m_shotForce);
				bullet->m_pTrans->SetPos(pos + dir * 50.0f);

				br->AddEnergy(m_fResourceShot);

				// �T�E���h
				CSound::PlaySE("Shot.mp3");

				return true;
			}
		}

		return false;
	}

	
};



