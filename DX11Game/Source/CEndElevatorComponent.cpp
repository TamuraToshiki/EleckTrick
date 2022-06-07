//======================================================================
//											CEndElevatorComponent.cpp
//	�G���h�G���x�[�^�[�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/03	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CEndElevatorComponent.h"
#include "CPlayer.h"
#include "Camera.h"
#include "CObjectManager.h"
#include "input.h"

#include "CPlayerInput.h"
#include "CPlayerActionComponent.h"

#include "CElevatorRoad.h"
#include "CElevatorRoadEnd.h"

#include "CSceneManager.h"
#include "CSceneGame.h"
#include "CSceneEnding.h"

#include "CClearComponent.h"
#include "CEditorComponent.h"


#include "Sound.h"


//===== �}�N����` =====


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CEndElevatorComponent::Start()
{
	m_pParent->m_pTrans->m_pos->y -= 50;
	m_pParent->m_pTrans->m_pos->z = 150;

	m_startPos = m_pParent->m_pTrans->m_pos;
	m_goalPos = m_startPos + Vector3{ 0, 1000, 0 };

	m_pParent->m_pTrans->m_pos = m_startPos;

	m_curState = eWait;

	m_rb = m_pParent->GetComponent<CRigidbody2D>();

	m_render = m_pParent->GetComponent<CAssimpRenderer>();
	const auto& r = m_render.lock();
	if (r)
	{
		r->SetAnimIndex(2);
	}

	// �G���x�[�^�[�̒ʘH���f���𐶐�
	Vector3 offset = { 0,90,0 };
	Vector3 evPos = m_startPos + offset;

	// �G���x�[�^�[�̔�����
	m_aEVRoad.push_back(CObjectManager::CreateObject<CElevatorRoadEnd>(evPos));

	// �G���x�[�^�[�̒ʘH����
	for (int i = 0; i < 10; i++)
	{
		evPos->y += 200;
		m_aEVRoad.push_back(CObjectManager::CreateObject<CElevatorRoad>(evPos));
	}

}


//===================================
//
//	�X�V�֐�
//
//===================================
void CEndElevatorComponent::Update()
{
	switch (m_curState)
	{
		// �v���C���[��ҋ@
	case CEndElevatorComponent::eWait:

		break;

		// �h�A�̑O�܂ňړ�����
	case CEndElevatorComponent::eGaolPosMove:
	{
		// �m�F
		const auto& player = m_player.lock();
		if (!player) return;
		const auto& rb = player->GetComponent<CRigidbody2D>();
		const auto& action = player->GetComponent<CPlayerActionComponent>();
		if (!rb && !action) return;

		// �v���C���[�ƃh�A�̃x�N�g��
		Vector3 vec = m_pParent->m_pTrans->m_pos - player->m_pTrans->m_pos;
		vec->y = 0.0f;
		vec->z = 0.0f;

		// ����������Ă���
		if (vec.magnitude() > 1.1f)
		{
			// �h�A�̑O�܂ňړ�
			action->FreeMove(vec.normalized() * 1.5f);
			//CSound::StopBGM("BGM.mp3");
		}
		else
		{
			// ����
			player->m_pTrans->m_pos->x = m_pParent->m_pTrans->m_pos->x;
			action->StopMove();

			// ��Ԉړ�
			m_curState = eClearMove;

			// �v���C���[�̉�]
			if(player->m_pTrans->m_rot->y >= 0.0f)	rb->SetTorqueY( 1.0f);
			else									rb->SetTorqueY(-1.0f);
		}
	}
		break;

		// �N���A�̊�у��[�V����
	case CEndElevatorComponent::eClearMove:
	{
		// �m�F
		const auto& player = m_player.lock();
		if (!player) return;
		const auto& rb = player->GetComponent<CRigidbody2D>();
		if (!rb) return;

		// �v���C���[�̕���
		if (std::fabsf(player->m_pTrans->m_rot->y) >= 180.0f)
		{
			// ��]�I��
			rb->SetTorque({ 0,0,0 });
			player->m_pTrans->m_rot->y = 180.0f;

			// ��Ԉړ�
			m_curState = eOpenDoor;

			// �A�j���[�V�����Đ�
			// �h�A���J��
			m_render.lock()->SetAnimIndex(0);
			m_render.lock()->PlayAnim();

			// �T�E���h
			CSound::PlaySE("Elevator.mp3");
		}
	}
		break;

		// �h�A���J��
	case CEndElevatorComponent::eOpenDoor:

		break;

		// ���ɓ���
	case CEndElevatorComponent::eInMove:
	{
		// �m�F
		const auto& player = m_player.lock();
		if (!player) return;
		const auto& rb = player->GetComponent<CPlayerActionComponent>();
		if (!rb) return;


		if (player->m_pTrans->m_pos->z < m_pParent->m_pTrans->m_pos->z)
		{
			// �ړ�������
			rb->BackMove();
		}
		else
		{
			// ��������
			rb->StopMove();
			player->m_pTrans->m_pos->z = m_pParent->m_pTrans->m_pos->z;

			// �A�j���[�V�����Đ�
			m_render.lock()->SetAnimIndex(1);
			m_render.lock()->PlayAnim();

			// �K�w
			const auto& obj = CObjectManager::CreateObject<CObject>();
			obj->AddComponent<CClearComponent>();

			// ��Ԉړ�
			m_curState = eCloseDoor;
		}
	}
		break;

		// �h�A��߂�
	case CEndElevatorComponent::eCloseDoor:
		break;

		// ��ɓ����Ă���
	case CEndElevatorComponent::eEndMove:
		// �ړ�
		m_rb.lock()->SetForceY(4.0f);

		// ����������
		if (m_pParent->m_pTrans->m_pos->y > m_goalPos->y)
		{
			// �X�g�b�v
			//m_pParent->m_pTrans->m_pos = m_goalPos;
			//m_rb.lock()->SetForceY(0.0f);

			// ��Ԉړ�
			m_curState = eMaxState;

			if (CEditorComponent::GetCurrentStageNum() < CEditorComponent::GetMaxStateNum())
			{
				// �V�[���J��
				CSceneManager::Get()->SetNextScene<CSceneGame>();
			}
			else
			{
				CSceneManager::Get()->SetNextScene<CSceneEnding>();
			}

			// �T�E���h
			CSound::PlaySE("fadeout.mp3");
			CSound::StopSE("ElevaterUpdown.mp3");
		}
		break;

		// ���ɂȂ�
	case CEndElevatorComponent::eMaxState:
		break;
	}
}


//===================================
//
//	��X�V�֐�
//
//===================================
void CEndElevatorComponent::LateUpdate()
{

}


//===================================
//
//	�I���֐�
//
//===================================
void CEndElevatorComponent::End()
{
	for (const auto& evRoad : m_aEVRoad)
	{
		const auto& ev_s = evRoad.lock();
		if (ev_s)
			CObjectManager::DestroyObject(ev_s);
	}
}


//===== �g��Ȃ����͏����Ă�������() =====
//==================================================
//
//	2D�����蔻��(Enter) �R�[���o�b�N�֐�
//
//==================================================
void CEndElevatorComponent::OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// �^�O�ȂǂŔ���
	if (collsion2d->GetParent()->GetTag() == "Player")
	{
		if (m_curState == eWait)
		{
			// �v���C���[�擾
			m_player = collsion2d->GetParent()->tish();

			// ���S����
			const auto& player = m_player.lock();
			if (!player) return;

			// �J��������
			CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);

			// �R���|�[�l���g�̍폜
			player->RemoveComponet<CPlayerInput>();

			// ��Ԉړ�
			m_curState = eGaolPosMove;

		}
	}
}


//============================================
//
//	3D�A�j���[�V�����I�����R�[���o�b�N�֐�
//
//============================================
void CEndElevatorComponent::OnAnimationExit3D(const std::shared_ptr <CAssimpRenderer>& renderer)
{
	// �����J���I�����
	if (renderer->GetCurrentAnimIndex() == 0)
	{
		if (m_curState == eOpenDoor)
		{
			m_curState = eInMove;
			renderer->StopAnim();
		}
	}
	// �������I�����
	else if (renderer->GetCurrentAnimIndex() == 1)
	{
		if (m_curState == eCloseDoor)
		{
			const auto& player = m_player.lock();
			if (!player) return;

			renderer->StopAnim();

			// ��Ԉړ�
			m_curState = eEndMove;

			// �T�E���h
			CSound::PlaySE("ElevaterUpdown.mp3");

		}
	}
}