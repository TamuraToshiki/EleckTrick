//======================================================================
//											CBulletComponent.h
//	�e
//
//======================================================================
//	author :	AT12A 15 �匴����
//======================================================================
//	�J������
//	2020/11/28	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CComponent.h"
#include "CObjectManager.h"
#include "CEffectComponent.h"
#include "Sound.h"

//===== �N���X��` =====
class CBulletComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---
	int m_rimit;	//�e�̐�������
	int m_timeCnt;	//���ݎ���

public:
	// --- �֐��錾 ---


	// �쐬���ɌĂ΂�܂�
	void Start() override
	{
		m_rimit = 2 * 60;
	}

	// ���t���[���Ă΂�܂�
	void Update() override
	{
		m_timeCnt++;
		if (m_rimit < m_timeCnt)
		{
			CObjectManager::DestroyObject(m_pParent);
		}
	}

	// ���t���[���̍X�V��ɌĂ΂�܂�
	void LateUpdate() override
	{

	}


	//===== �g��Ȃ����͏����Ă�������() =====
	//--- �R�[���o�b�N�֐� ---
	// 2D�����蔻��
	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override
	{
		// �^�O�ȂǂŔ���
		if (collsion2d->m_pParent->GetTag() != "Player" && collsion2d->m_pParent->GetTag() != "EnergyBullet" &&
			collsion2d->m_pParent->GetTag() != "Visible" && collsion2d->m_pParent->GetTag() != "StandStick" &&
			collsion2d->m_pParent->GetTag() != "Blocksensor")
		{
			if (m_timeCnt <= 900)
			{
				// �G�t�F�N�g�̐���
				const auto& obj = CObjectManager::CreateObject<CObject>(m_pParent->m_pTrans->m_pos);
				obj->AddComponent<CEffectComponent>()->SetEffect(CEffectComponent::eHitEffect);
			}

			CSound::PlaySE("BulletHit.mp3");

			m_timeCnt = 999;
		}
	}
};



