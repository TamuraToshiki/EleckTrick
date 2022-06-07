//======================================================================
//											CStartElevatorComponent.cpp
//	�X�^�[�g�G���x�[�^�[�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/01	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CStartElevatorComponent.h"
#include "CPlayer.h"
#include "Camera.h"
#include "CObjectManager.h"
#include "input.h"

#include "CPlayerInput.h"
#include "CPlayerActionComponent.h"
#include "CRespawnComponent.h"

#include "CElevatorRoad.h"
#include "CElevatorRoadEnd.h"

#include "CHierarchyComponent.h"

#include "Sound.h"
#include "CEffectComponent.h"



//===== �}�N����` =====
#define EV_CAMERA_OFFSET (225)
#define PLAYER_CAMERA_OFFSET (150)

//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CStartElevatorComponent::Start()
{
	m_pParent->m_pTrans->m_pos->y -= 50;
	m_pParent->m_pTrans->m_pos->z = 150;


	m_goalPos = m_pParent->m_pTrans->m_pos;
	m_startPos = m_goalPos - Vector3{ 0, 1000, 0 };

	m_pParent->m_pTrans->m_pos = m_startPos;

	m_rb = m_pParent->GetComponent<CRigidbody2D>();
	m_rb.lock()->SetMass(3);

	m_render = m_pParent->GetComponent<CAssimpRenderer>();
	const auto& r = m_render.lock();
	if (r)
	{
		r->SetAnimIndex(2);
	}

	// �v���C���[�̎擾
	m_player = CObjectManager::SearchObjectName("Player");
	const auto& player = m_player.lock();
	if (player)
	{
		player->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
		player->RemoveComponet<CPlayerInput>();

		// ���X�|�[���|�C���g�̓o�^
		player->GetComponent<CRespawnComponent>()->SetRespawnPoint(m_goalPos);
		player->GetComponent<CRigidbody2D>()->SetTrigger(true);

		// �v���C���[���X�g�b�v����Ȃ�
		if (!player->GetStop())
		{
			// �J��������
			CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);
			CCamera::GetMainCamera()->SetOffSet({ 0, EV_CAMERA_OFFSET, 0 });
			CCamera::GetMainCamera()->SetLengthOffset(200);
			CCamera::GetMainCamera()->UpdateCameraPos(100);
			m_curState = eStartMove;
		}
		else
		{
			// �ҋ@
			m_curState = eWait;
		}
	}

	// �G���x�[�^�[�̒ʘH���f���𐶐�
	Vector3 offset = { 0,90,0 };
	Vector3 evPos = m_goalPos + offset;

	// �G���x�[�^�[�̔�����
	m_aEVRoad.push_back( CObjectManager::CreateObject<CElevatorRoadEnd>(evPos));

	// �G���x�[�^�[�̒ʘH����
	for (int i = 0; i < 10; i++)
	{
		evPos->y -= 200;
		m_aEVRoad.push_back(CObjectManager::CreateObject<CElevatorRoad>(evPos));
	}
	
	// �T�E���h
	CSound::PlaySE("ElevaterUpdown.mp3");
}


//===================================
//
//	�X�V�֐�
//
//===================================
void CStartElevatorComponent::Update()
{
	switch (m_curState)
	{
		// �v���C���[�̑ҋ@
	case CStartElevatorComponent::eWait:
	{
		const auto& player = m_player.lock();
		if (player)
		{
			if (!player->GetStop())
			{
				// �J��������
				CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);
				CCamera::GetMainCamera()->SetOffSet({ 0, EV_CAMERA_OFFSET, 0 });
				CCamera::GetMainCamera()->UpdateCameraPos(100);
				m_curState = eStartMove;
			}
		}
	}
		break;

		// �G���x�[�^�[�̃X�^�[�g���o
	case CStartElevatorComponent::eStartMove:
		// �ړ�
		m_rb.lock()->SetForceY(4.0f);
		// �T�E���h
		//if (!CSound::IsPlaying("ElevaterUpdown.mp3"))
		//{
		//	CSound::PlaySE("ElevaterUpdown.mp3");
		//}
		if (m_player.lock())
		{
			m_player.lock()->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
		}

		// ����������
		if (m_goalPos->y < m_pParent->m_pTrans->m_pos->y)
		{
			m_pParent->m_pTrans->m_pos = m_goalPos;

			// �v���C���[�̐���
			//m_player = CObjectManager::CreateObject<CPlayer>(m_pParent->m_pTrans->m_pos);
			if (m_player.lock())
			{
				m_player.lock()->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
				m_player.lock()->GetComponent<CRigidbody2D>()->SetTrigger(false);

				//m_player.lock()->RemoveComponet<CPlayerInput>();
			}

			// ��Ԉړ�
			m_curState = eOpenDoor;
			m_rb.lock()->SetForceY(0.0f);

			// �A�j���[�V�����Đ�
			m_render.lock()->SetAnimIndex(0);
			m_render.lock()->PlayAnim();

			// �K�w
			const auto& obj = CObjectManager::CreateObject<CObject>();
			obj->AddComponent<CHierarchyComponent>();

			// �T�E���h
			CSound::StopSE("ElevaterUpdown.mp3");
			CSound::PlaySE("Elevator.mp3");
		}


		break;
	case CStartElevatorComponent::eOpenDoor:

		break;
	case CStartElevatorComponent::eOutMove:
	{
		// �v���C���[�̈ړ�
		const auto& player = m_player.lock();
		if (!player) return;

		const auto& rb = player->GetComponent<CPlayerActionComponent>();
		if (!rb) return;

		if (player->m_pTrans->m_pos->z > 0.0f)
		{
			// �ړ�������
			rb->FrontMove();
		}
		else
		{
			// ��������
			rb->StopMove();

			// �A�j���[�V�����Đ�
			m_render.lock()->SetAnimIndex(1);
			m_render.lock()->PlayAnim();

			// ��Ԉړ�
			m_curState = eCloseDoor;
		}
	}
		break;
	case CStartElevatorComponent::eCloseDoor:


		break;
	case CStartElevatorComponent::eMaxState:
		break;
	}
}


//===================================
//
//	��X�V�֐�
//
//===================================
void CStartElevatorComponent::LateUpdate()
{
	switch (m_curState)
	{
		// �G���x�[�^�[�̃X�^�[�g���o
	case CStartElevatorComponent::eStartMove:
	{
		const auto& player = m_player.lock();
		if (player)
		{
			player->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
		}
		CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);
	}
	break;

	case CStartElevatorComponent::eOpenDoor:
	{
		const auto& player = m_player.lock();
		if (player)
		{
			player->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
		}
		CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);
	}
	break;

	case CStartElevatorComponent::eOutMove:
		CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);
		break;

	}
}


//===================================
//
//	�I���֐�
//
//===================================
void CStartElevatorComponent::End()
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
void CStartElevatorComponent::OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// �^�O�ȂǂŔ���
	if (collsion2d)
	{

	}
}


//============================================
//
//	3D�A�j���[�V�����I�����R�[���o�b�N�֐�
//
//============================================
void CStartElevatorComponent::OnAnimationExit3D(const std::shared_ptr <CAssimpRenderer>& renderer)
{
	// �����J���I�����
	if (renderer->GetCurrentAnimIndex() == 0)
	{
		if (m_curState == eOpenDoor)
		{
			m_curState = eOutMove;
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

			// �R���|�[�l���g��ǉ�
			if (!player->GetComponent<CPlayerInput>())
			{
				player->AddComponent<CPlayerInput>();
			}
			player->SetStop(false);
			CCamera::GetMainCamera()->SetOffSet({ 0, PLAYER_CAMERA_OFFSET, 0 });

			// ��Ԉړ�
			m_curState = eMaxState;
		}
	}
}