//==================================================================
//									CRigidbody2D.h
//	���W�b�h�{�f�B
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2020/07/12	���W�b�h�{�f�B�N���X�쐬
//
//===================================================================

//====== �C���N���[�h�� ======
#include "CRigidbody2D.h"
#include "CObject.h"
#include "CCollision2D.h"

//===== �}�N����` =====
#define GRAVITY (-9.8f / 60.0f)
#define DRAG (1.0f)

#define MAX_VELOCITY (48)

//===== �N���X��` =====



//===== �v���g�^�C�v�錾 =====



//========================================
//
//	�R���X�g���N�^
//
//========================================
CRigidbody2D::CRigidbody2D()
{
	m_fMass = 1.0f;

	m_force = Vector3{ 0, 0 ,0 };
	m_torque = Vector3{ 0, 0 ,0 };
	m_velocity = Vector3{ 0, 0, 0 };
	m_angularVelocity = Vector3{ 0, 0, 0 };
	m_drag = Vector3{ DRAG, 0.0f, DRAG };
	m_torqueDrag = Vector3{ 0, 0, 0 };

	m_bUseGravity = true;
	m_bSolid = false;
	m_enHitDir = DIRECTION::MAX_DIRECTION;
	m_dir = Vector3{ 0, 0, 0 };
	m_fGraviyForce = GRAVITY;
	m_bTrigger = false;
	m_pTrans = nullptr;
	m_bMoveLimit = true;
}


//========================================
//
//	�f�X�g���N�^
//
//========================================
CRigidbody2D::~CRigidbody2D()
{

}

//========================================
//
//	�X�V
//
//========================================
void CRigidbody2D::Start()
{
	m_pTrans = &*m_pParent->m_pTrans;

	// �R���W��������������
	const auto& col = m_pParent->GetComponent<CCollision2D>();
	if (col)
	{
		col->SetRb(m_pParent->GetComponent<CRigidbody2D>());
		m_col = col;
	}
}

//========================================
//
//	�X�V
//
//========================================
void CRigidbody2D::Update()
{

	CRigidbody2D *pRb;
	CTransform *pTrans;
	Vector3 pos;
	Vector3 rot;
	// ���W�b�h�{�f�B�擾
	pRb = this;
	// �I�u�W�F�N�g�̎擾
	pTrans = &(*m_pParent->m_pTrans);

	// �J�����O�Ȃ�͂���


	// �X�g�b�v���I���Ȃ�Ƃ΂�
	//if (pRb->m_bStop) return;

	//if (pRb->m_bSolid) return;

	//===== ��] =====

	// �ړ���
	pRb->m_angularVelocity->x = 0;
	pRb->m_angularVelocity->y = 0;
	pRb->m_angularVelocity->z = 0;

	//// �O��
	pRb->m_angularVelocity->x += pRb->m_torque->x;
	pRb->m_angularVelocity->y += pRb->m_torque->y;
	pRb->m_angularVelocity->z += pRb->m_torque->z;

	// �����ő��x����������v�Z
	//pRb->m_dir = Normalize(pRb->m_angularVelocity);

	// �ʒu�̍X�V
	rot = pTrans->GetRotation();
	rot->x += pRb->m_angularVelocity->x;
	rot->y += pRb->m_angularVelocity->y;
	rot->z += pRb->m_angularVelocity->z;
	pTrans->SetRotation(rot);

	// ��R�ŉ����x�����炷	fDrag(0.0f �` 1.0f)
	pRb->m_torque->x *= (1.0f - pRb->m_torqueDrag->x);
	if (fabsf(pRb->m_torque->x) < 0.01f) pRb->m_torque->x = 0.0f;
	pRb->m_torque->y *= (1.0f - pRb->m_torqueDrag->y);
	if (fabsf(pRb->m_torque->y) < 0.01f) pRb->m_torque->y = 0.0f;
	pRb->m_torque->z *= (1.0f - pRb->m_torqueDrag->z);
	if (fabsf(pRb->m_torque->z) < 0.01f) pRb->m_torque->z = 0.0f;


	//===== �ړ� =====

	// �ړ���
	pRb->m_velocity->x = 0;
	pRb->m_velocity->y = 0;
	pRb->m_velocity->z = 0;

	// �d��
	if (pRb->m_bUseGravity)
	{
		pRb->m_force->y += pRb->m_fGraviyForce;
	}
	// �d�͂��I��
	if(!pRb->m_bSolid)
		pRb->m_bUseGravity = true;

	//// �O��	�O�͂�AddForce��
	pRb->m_velocity->x += pRb->m_force->x;
	pRb->m_velocity->y += pRb->m_force->y;
	pRb->m_velocity->z += pRb->m_force->z;

	// �����ő��x����������v�Z
	//pRb->m_dir = Normalize(pRb->m_velocity);

	// ���x���E
	if (pRb->m_velocity->x > MAX_VELOCITY) pRb->m_velocity->x = MAX_VELOCITY;
	if (pRb->m_velocity->y > MAX_VELOCITY) pRb->m_velocity->y = MAX_VELOCITY;
	if (pRb->m_velocity->z > MAX_VELOCITY) pRb->m_velocity->z = MAX_VELOCITY;
	if (pRb->m_velocity->x < -MAX_VELOCITY) pRb->m_velocity->x = -MAX_VELOCITY;
	if (pRb->m_velocity->y < -MAX_VELOCITY) pRb->m_velocity->y = -MAX_VELOCITY;
	if (pRb->m_velocity->z < -MAX_VELOCITY) pRb->m_velocity->z = -MAX_VELOCITY;

	// �ʒu�̍X�V
	pos = pTrans->GetPos();
	pos->x += pRb->m_velocity->x;
	pos->y += pRb->m_velocity->y;
	pos->z += pRb->m_velocity->z;
	// �ړ����E
	if (m_bMoveLimit)
	{
		if (pos->x < 0.0f) pos->x = 0.0f;
		if (pos->y < 0.0f) pos->y = 0.0f;
	}
	pTrans->SetPos(pos);

	// ��R�ŉ����x�����炷	fDrag(0.0f �` 1.0f)
	pRb->m_force->x *= (1.0f - pRb->m_drag->x);
	if (fabsf(pRb->m_force->x) < 0.01f) pRb->m_force->x = 0.0f;
	//pRb->m_force->y *= (1.0f - pRb->m_fDrag);
	if (fabsf(pRb->m_force->y) < 0.01f) pRb->m_force->y = 0.0f;
	pRb->m_force->z *= (1.0f - pRb->m_drag->z);
	if (fabsf(pRb->m_force->z) < 0.01f) pRb->m_force->z = 0.0f;

}

//========================================
//
//	�����o������
//
//========================================
void CRigidbody2D::Extrusion(const std::shared_ptr<CRigidbody2D>& pOther)
{
	// �\���b�h�Ȃ�
	if (m_bSolid) return;

	// �ŒZ
	float fMinLenX;
	float fMinLenY;
	float fLenX;
	float fLenY;
	// �Փˌ�̗�
	float fForceX;
	float fForceY;
	// ���W
	Vector3 main_pos = m_pTrans->GetPos() + m_col.lock()->GetOffSet();
	Vector3 sub_pos = pOther->m_pTrans->GetPos() + pOther->m_col.lock()->GetOffSet();
	// �X�P�[��
	Vector3 main_scale = m_col.lock()->GetScale();
	Vector3 sub_scale = pOther->m_col.lock()->GetScale();
	// �I�t�Z�b�g
	Vector3 main_off = m_col.lock()->GetOffSet();
	Vector3 sub_off = pOther->m_col.lock()->GetOffSet();

	// �����̏�����
	m_enHitDir = DIRECTION::MAX_DIRECTION;

	//--- �ŒZ�ʒu�̌v�Z ---
	// ��
	fLenX = sub_pos->x - sub_scale->x / 2 - main_pos->x - main_scale->x / 2;
	fMinLenX = fLenX;

	// �E
	fLenX = sub_pos->x + sub_scale->x / 2 - main_pos->x + main_scale->x / 2;
	if (fabsf(fLenX) < fabsf(fMinLenX))
	{
		fMinLenX = fLenX;
	}

	// ��
	fLenY = sub_pos->y - sub_scale->y / 2 - main_pos->y - main_scale->y / 2;
	fMinLenY = fLenY;

	// ��
	fLenY = sub_pos->y + sub_scale->y / 2 - main_pos->y + main_scale->y / 2;
	if (fabsf(fLenY) < fabsf(fMinLenY))
	{
		fMinLenY = fLenY;
	}

	// �w�C�x�̍ŒZ��r
	if (fabsf(fMinLenX) < fabsf(fMinLenY))	// �����łԂ�����������������!!
	{	// �w��

		// ����
		if (fMinLenX < 0)
		{
			// ��
			m_enHitDir = DIRECTION::LEFT;
			pOther->m_enHitDir = DIRECTION::RIGHT;
		}
		else
		{
			// �E
			m_enHitDir = DIRECTION::RIGHT;
			pOther->m_enHitDir = DIRECTION::LEFT;
		}

		// �g���K�[�Ȃ牟���o�����Ȃ�
		if (m_bTrigger) return;
		if (pOther->m_bTrigger) return;
		
		// ���݂��\���b�h����Ȃ�
		if (!pOther->m_bSolid)
		{
			m_velocity->x += fMinLenX;
			// �����������̗͐ς����߂�
			fForceX = ((m_fMass - pOther->m_fMass) * m_velocity->x + pOther->m_fMass * pOther->m_velocity->x) / (m_fMass + pOther->m_fMass);
			m_force->x = -fForceX;

			m_velocity->x += m_force->x;
			m_pTrans->SetPos(Vector3{ main_pos->x + m_velocity->x - main_off->x, main_pos->y - main_off->y, main_pos->z });
			return;
		}

		// ���葤�\���b�h�Ȃ�
		if (fMinLenX > 0)
		{
			// �E����
			// �����o��
			m_pTrans->SetPos(Vector3{ sub_pos->x + sub_scale->x / 2 + main_scale->x / 2 - main_off->x, 
				main_pos->y - main_off->y, main_pos->z });
		}
		else if(fMinLenX < 0)
		{
			// ������
			// �����o��
			m_pTrans->SetPos(Vector3{ sub_pos->x - sub_scale->x / 2 - main_scale->x / 2 - main_off->x,
				main_pos->y - main_off->y, main_pos->z });
		}
	}
	else
	{	// �x��
		
		// �ォ��Ԃ��������̓f�t�H���g�ŏd�͂�����
		if (fMinLenY < 0)
		{
			m_enHitDir = DIRECTION::UP;
			pOther->m_enHitDir = DIRECTION::DOWN;
			// �g���K�[�Ȃ牟���o�����Ȃ�
			if (m_bTrigger) return;
			if (pOther->m_bTrigger) return;
			// ��
			m_bUseGravity = false;
			if (m_force->y > 0)
				m_force->y = 0;
		}
		else
		{
			m_enHitDir = DIRECTION::DOWN;
			pOther->m_enHitDir = DIRECTION::UP;
			// �g���K�[�Ȃ牟���o�����Ȃ�
			if (m_bTrigger) return;
			if (pOther->m_bTrigger) return;
			// ��
			if (m_force->y < 0)
				m_force->y = 0;
		}

		// ���݂��\���b�h����Ȃ�
		if (!pOther->m_bSolid)
		{
			m_velocity->y += fMinLenY;
			// �����������̗͐ς����߂�
			fForceY = ((m_fMass - pOther->m_fMass) * m_velocity->y + pOther->m_fMass * pOther->m_velocity->y) / (m_fMass + pOther->m_fMass);
			m_force->y = -fForceY;
			
			m_velocity->y += m_force->y;
			m_pTrans->SetPos(Vector3{ main_pos->x - main_off->x, main_pos->y + m_velocity->y - main_off->y, main_pos->z });
			return;
		}

		// ���葤�\���b�h�Ȃ�
		if (fMinLenY > 0)
		{
			// ������
			// �����o��
			m_pTrans->SetPos(Vector3{ main_pos->x - main_off->x,
				sub_pos->y + sub_scale->y / 2 + main_scale->y / 2 - main_off->y, main_pos->z });
		}
		else if(fMinLenY < 0)
		{
			// �ォ��
			// �����o��
			m_pTrans->SetPos(Vector3{ main_pos->x - main_off->x,
				sub_pos->y - sub_scale->y / 2 - main_scale->y / 2 - main_off->y, main_pos->z });
		}
	}
}

//========================================
//
//	�����o������ �~�Ɖ~
//
//========================================
void CRigidbody2D::ExtrusionCircleToCircle(CRigidbody2D *pOther)
{
//	float TotalWeight = m_fMass + pOther->m_fMass; // ���ʂ̍��v
//	float RefRate = (1.0f);// + res_A * res_B); // ������
//
//	Vector3 C = pOther->GetObj()->GetPos(); // �Փˎ��x�N�g��
//	C->x -= main_pos->x;
//	C->y -= main_pos->y;
//	C->z -= main_pos->z;
//	// ���K��
////	C = Normalize(C);
//
//	Vector3 velowork = m_velocity;
//	velowork->x -= pOther->m_velocity->x;
//	velowork->y -= pOther->m_velocity->y;
//	velowork->z -= pOther->m_velocity->z;
////	float fDot = Dot(velowork, C); // ���ώZ�o
//
//	Vector3 ConstVec;
////	ConstVec->x = RefRate * fDot / TotalWeight * C->x; // �萔�x�N�g��
////	ConstVec->y = RefRate * fDot / TotalWeight * C->y; // �萔�x�N�g��
////	ConstVec->z = RefRate * fDot / TotalWeight * C->z; // �萔�x�N�g��
//
//	
//	// �Փˌ㑬�x�x�N�g���̎Z�o
//	Vector3 veloA;
//	Vector3 veloB;
//
//	veloA->x = -pOther->m_fMass * ConstVec->x + m_velocity->x;
//	veloA->y = -pOther->m_fMass * ConstVec->y + m_velocity->y;
//	veloA->z = -pOther->m_fMass * ConstVec->z + m_velocity->z;
//
//	veloB->x = m_fMass * ConstVec->x + pOther->m_velocity->x;
//	veloB->y = m_fMass * ConstVec->y + pOther->m_velocity->y;
//	veloB->z = m_fMass * ConstVec->z + pOther->m_velocity->z;
//
//	m_velocity			= veloA;
//	pOther->m_velocity	= veloB;
//
//	
//	// �Փˌ�ʒu�̎Z�o
//	Vector3 posA = main_pos;
//	Vector3 posB = pOther->GetObj()->GetPos();
//
//	posA->x += veloA->x;
//	posA->y += veloA->y;
//	posA->z += veloA->z;
//
//	posB->x += veloB->x;
//	posB->y += veloB->y;
//	posB->z += veloB->z;
//
//	m_pTrans->SetPos(posA);
//	pOther->GetObj()->SetPos(posB);
//
//	return;
}
