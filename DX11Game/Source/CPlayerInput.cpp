//======================================================================
//											CPlayerInput.cpp
//	�v���C���[�ړ�
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/16	�쐬 �F�����ҏW
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CPlayerInput.h"
#include "input.h"
#include "Camera.h"
#include "Light.h"
#include "Sound.h"
#include "Renderer.h"


//===== �}�N����` =====
#define MOVE_SE_FRAME (5)

//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CPlayerInput::Start()
{
	m_playerAction = m_pParent->GetComponent<CPlayerActionComponent>();
	m_shot = m_pParent->GetComponent<CEnergyShot>();
	m_product = m_pParent->GetComponent<CProductEnergy>();

	// �����_���[�擾
	m_renderer = m_pParent->GetComponent<CAssimpRenderer>();

	// �J�����^�[�Q�b�g�̃Z�b�g
	CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);

	// �A�j���[�V����
	m_renderer.lock()->SetAnimIndex(eRun);
	m_renderer.lock()->PlayAnim();

	// �R���g���[���[�̐�
	m_nJoyCount = GetJoyCount();
}


//===================================
//
//	�X�V�֐�
//
//===================================
void CPlayerInput::Update()
{
	if (m_playerAction.expired()) return;

	// �����̏�����
	m_playerAction.lock()->ResetDir();

	// �ړ�
	float JoyX = (float)GetJoyX(JOYSTICKID1);
	float JoyY = (float)GetJoyY(JOYSTICKID1);

	Vector3 dir = { JoyX, -JoyY, 0 };
	dir /= 32767.0f;
	if (m_nJoyCount <= 0)
	{
		dir = Vector3{ 0,0,0 };

	}

	float speed = 1.0f;
	// ��~
	if (GetJoyButton(JOYSTICKID1, JOYBUTTON_LB) ||
		GetJoyButton(JOYSTICKID1, JOYBUTTON_RB))
	{
		speed = 0.0f;
	}
	
	if (GetKeyPress(VK_RIGHT))
	{
		m_playerAction.lock()->RightMove();
	}
	else if (GetKeyPress(VK_LEFT))
	{
		m_playerAction.lock()->LeftMove();
	}
	else if (dir->x > 0.3f)
	{
		m_playerAction.lock()->RightMove(dir->x * speed);
	}
	else if (dir->x < -0.3f)
	{
		m_playerAction.lock()->LeftMove(dir->x * speed);
	}
	else
	{
		m_playerAction.lock()->StopSE();
	}

	// �e�̌���
	if (m_nJoyCount > 0 && dir.magnitude() > 0.3f)
	{
		m_playerAction.lock()->SetDir(dir);
	}

	if (GetKeyPress(VK_UP))
	{
		m_playerAction.lock()->UpDir();
	}
	if (GetKeyPress(VK_DOWN))
	{
		m_playerAction.lock()->DownDir();
	}

	// �����̏C��
	m_playerAction.lock()->CheckDir();

	// �W�����v
	if (GetKeyTrigger(VK_SPACE) ||
		GetJoyRelease(JOYSTICKID1, JOYBUTTON_A))
	{
		if (m_playerAction.lock()->Jump())
		{
			// �A�j���[�V����
			m_renderer.lock()->SetAnimIndex(eJump);
		}
	}
	m_playerAction.lock()->SetJumpFlg(false);

	// �V���b�g
	if (GetKeyTrigger(VK_X)
		|| GetJoyRelease(JOYSTICKID1, JOYBUTTON_X))
	{
		if (m_shot.lock()->CreateShot())
		{
			// �A�j���[�V����
			m_renderer.lock()->SetAnimIndex(eAttack);
		}
	}

	// �`���[�W
	float JoyZ = (float)GetJoyZ(JOYSTICKID1);
	JoyZ /= 32767.0f;
	if (m_nJoyCount <= 0) JoyZ = 0.0f;

	if (GetKeyTrigger(VK_C) || JoyZ > 0.3f)
	{
		if (m_product.lock()->GenerateEnergyRight())
		{
			// �A�j���[�V����
			//m_renderer.lock()->SetAnimIndex(eGeneration);
		}
	}
	else if (GetKeyTrigger(VK_Z) || JoyZ < -0.3f)
	{
		if (m_product.lock()->GenerateEnergyLeft())
		{
			// �A�j���[�V����
			//m_renderer.lock()->SetAnimIndex(eGeneration);
		}
	}
	else
	{
		m_product.lock()->ExpenseEnergy();
	}
}


//===================================
//
//	��X�V�֐�
//
//===================================
void CPlayerInput::LateUpdate()
{
	//CLight::GetMainLight()->SetTargetPos(m_pParent->m_pTrans->m_pos.GetFloat3());
	// �J�����^�[�Q�b�g�̃Z�b�g
	CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);

}


//===================================
//
//	�I���֐�
//
//===================================
void CPlayerInput::End()
{

}


//===================================
//
//	2D�����蔻��(Enter) �R�[���o�b�N�֐�
//
//===================================
void CPlayerInput::OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d)
{

}

//============================================
//
//	3D�A�j���[�V�����I�����R�[���o�b�N�֐�
//
//============================================
void CPlayerInput::OnAnimationExit3D(const std::shared_ptr <CAssimpRenderer>& renderer)
{
	// �A�j���[�V����
	if (renderer->GetCurrentAnimIndex() == eJump)
	{
		m_renderer.lock()->SetAnimIndex(eRun);
	}
	else if (renderer->GetCurrentAnimIndex() == eGeneration)
	{
		m_renderer.lock()->SetAnimIndex(eRun);
	}
	else if (renderer->GetCurrentAnimIndex() == eAttack)
	{
		m_renderer.lock()->SetAnimIndex(eRun);
	}
	else if (renderer->GetCurrentAnimIndex() == eRun)
	{
		m_renderer.lock()->SetAnimIndex(eRun);
	}
}