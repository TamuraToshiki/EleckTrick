//======================================================================
//											CTestComponent.h
//	�f�o�b�N�e�X�g�p�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/11/16	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CObjectManager.h"
#include "input.h"

#include "CPlayer.h"
#include "CEditorObject.h"
#include "CEditorComponent.h"


//===== �N���X��` =====
class CTestComponent final : public CComponent
{
private:
	// --- �ϐ��錾 ---
	std::weak_ptr<CObject> m_player;
	std::weak_ptr<CObject> m_editor;

	bool m_bEditFlg;

public:
	// --- �֐��錾 ---

	// �쐬���ɌĂ΂�܂�
	void Start() override
	{
		m_bEditFlg = true;

		m_player = CObjectManager::CreateObject<CPlayer>({1200, 1200, 0});
		m_editor = CObjectManager::CreateObject<CEditorObject>({ 1200, 1200, 0 });

		const auto& player = m_player.lock();
		if (player)
		{
			player->SetStop(!m_bEditFlg);
			//CCamera::GetMainCamera()->SetCameraTarget(player->m_pTrans);
		}

		const auto& edit = m_editor.lock();
		if (edit)
		{
			edit->SetStop(m_bEditFlg);
			edit->GetComponent<CAssimpRenderer>()->SetDiffuseColor({ 1, 1, 1, 0 });
		}
	}

	// ���t���[���Ă΂�܂�
	void Update() override
	{

	}

	// ���t���[���̍X�V��ɌĂ΂�܂�
	void LateUpdate() override
	{
		if (GetKeyTrigger(VK_P))
		{
			m_bEditFlg ^= 1;

			const auto& edit = m_editor.lock();
			const auto& player = m_player.lock();
			if (player && edit)
			{
				player->SetStop(!m_bEditFlg);
				if (!m_bEditFlg)
				{
					CCamera::GetMainCamera()->SetCameraTarget(player->m_pTrans);
					edit->GetComponent<CAssimpRenderer>()->SetDiffuseColor({ 1, 1, 1, 1 });
				}

				edit->SetStop(m_bEditFlg);
				if (m_bEditFlg)
				{
					CCamera::GetMainCamera()->SetCameraTarget(edit->m_pTrans);
					edit->m_pTrans->m_pos = player->m_pTrans->m_pos;
					edit->GetComponent<CAssimpRenderer>()->SetDiffuseColor({ 1, 1, 1, 0 });
					// ����ɖ߂�
					edit->GetComponent<CEditorComponent>()->Reset();
				}
			}
		}
	}


	void End() override
	{

	}

	//===== �g��Ȃ����͏����Ă�������() =====
	//--- �R�[���o�b�N�֐� ---
	// 2D�����蔻��
	void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override
	{
		// �^�O�ȂǂŔ���
		if (collsion2d->m_pParent->GetTag() == "CBock")
		{

		}
	}

	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override
	{

	}

	void OnCollisionExit2D(const std::shared_ptr <CCollision2D>& collsion2d) override
	{

	}

};



