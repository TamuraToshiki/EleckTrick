//======================================================================
//											CInputMoveComponent.h
//	���͍s���R���|�[�l���g�e�X�g
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
#include "input.h"
#include "CRigidbody2D.h"



//===== �N���X��` =====
class CInputMoveComponent : public CComponent
{
private:
	// �ړ����x
	float m_speed;
	float m_jump;
	std::weak_ptr<CRigidbody2D> rb;

public:

	// �쐬���ɌĂ΂�܂�
	void Start() override
	{
		m_speed = 2.0f;
		m_jump = 3.0f;
		rb = m_pParent->GetComponent<CRigidbody2D>();
	}

	// ���t���[���Ă΂�܂�
	void Update() override
	{
		if (GetKeyPress(VK_RIGHT))
		{
			//m_pParent->m_pTrans->m_pos->x += m_speed;
			rb.lock()->AddForceX(m_speed);
		}
		if (GetKeyPress(VK_LEFT))
		{
			//m_pParent->m_pTrans->m_pos->x -= m_speed;
			rb.lock()->AddForceX(-m_speed);
		}

		if (GetKeyPress(VK_SPACE))
		{
			rb.lock()->SetForceY(m_jump);
		}
	}

	// ���t���[���̍X�V��ɌĂ΂�܂�
	void LateUpdate() override
	{

	}
};



