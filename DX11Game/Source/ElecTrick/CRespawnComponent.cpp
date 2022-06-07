//======================================================================
//											CRespawnComponent.cpp
//	�R���|�[�l���g�쐬�p�̃x�[�X���C�A�E�g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/11/16	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CRespawnComponent.h"
#include "CSceneManager.h"
#include "Camera.h"

#include "Sound.h"
#include "CPlayerInput.h"


//===== �}�N����` =====


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CRespawnComponent::Start()
{
	RespawnPoint = Vector3{ 600, 1500, 0 };			// �������X�|�[���|�C���g
	//m_pi = m_pParent->GetComponent<CPlayerInput>();
	//m_pr = m_pParent->GetComponent<CResourceEnergy>();

	m_bNowFade = false;
}

//===================================
//
//	�X�V�֐�
//
//===================================
void CRespawnComponent::Update()
{

}


//===================================
//
//	��X�V�֐�
//
//===================================
void CRespawnComponent::LateUpdate()
{
	// ���X�|�[������
	if (m_pParent->m_pTrans->m_pos->y <= 0.1f)
	{
		Respawn();
	}
}

void CRespawnComponent::SetRespawnPoint(Vector3 pos)
{
	RespawnPoint = pos;
	RespawnPoint->z = 0.0f;
}

void CRespawnComponent::Respawn()
{
	if (!m_bNowFade && !CSceneManager::Get()->GetNowFade())
	{
		// �T�E���h
		CSound::PlaySE("Respawn1.mp3");

		// �t�F�[�h�t���O
		m_bNowFade = true;

		// �v���C���[�̃X�g�b�v
		//m_pParent->SetStop(true);
		m_pParent->RemoveComponet<CPlayerInput>();
		m_pParent->GetComponent<CRigidbody2D>()->SetForce({ 0,0,0 });
		m_pParent->GetComponent<CRigidbody2D>()->SetUseGravity(false);
		m_pParent->GetComponent<CRigidbody2D>()->SetSolid(true);

		CSceneManager::Get()->StartFade([this]() {
			// �ꊇ�������̎d����������Ȃ��������߁A�Œ���̃v���C���[�ƃ��\�[�X�̏�����
			/*const auto& pi = m_pi.lock();
			if (pi)
				pi->Start();
			const auto& pr = m_pr.lock();
			if (pr)
				pr->Start();*/
			// �������������ƂɁA�v���C���[�̈ʒu�����X�|�[���|�C���g�ɕύX����
			m_pParent->m_pTrans->m_pos = RespawnPoint;

			// �J�����̍X�V
			CCamera::GetMainCamera()->UpdateCameraPos(50);

			// �t�F�[�h�t���O
			m_bNowFade = false;

			// �M�~�b�N�̏�����
			m_edit = CObjectManager::SearchObjectTag("Editor").lock()->GetComponent<CEditorComponent>();
			m_edit.lock()->GimmickRestart();

			// �v���C���[�̃X�g�b�v
			//m_pParent->SetStop(false);
			if (!m_pParent->GetComponent<CPlayerInput>())
			{
				m_pParent->AddComponent<CPlayerInput>();
			}
			m_pParent->GetComponent<CRigidbody2D>()->SetUseGravity(true);
			m_pParent->GetComponent<CRigidbody2D>()->SetSolid(false);
			});
	}
}
