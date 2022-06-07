//======================================================================
//											CPlayerActionComponent.h
//	�v���C���[�̃A�N�V�����R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/16	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "Renderer.h"


//===== �N���X��` =====
class CPlayerActionComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---
		// �ړ����x
	float m_speed;
	float m_jump;			//�W�����v���x
	bool m_bjunpFlag;		//�W�����v�t���O
	Vector3 m_dir;			//����
	std::weak_ptr<CRigidbody2D> m_rb;
	bool m_right;
	int m_nMoveSECnt;

	// �L���^�s���A�j���[�V����
	std::weak_ptr<CAssimpRenderer> m_caterpollar;
	// �M�A�A�j���[�V����
	std::weak_ptr<CAssimpRenderer> m_gear;

public:
	// --- �֐��錾 ---

	// �쐬���ɌĂ΂�܂�
	void Start() override;

	// ���t���[���Ă΂�܂�
	void Update() override;

	// ���t���[���̍X�V��ɌĂ΂�܂�
	void LateUpdate() override;

	// �������ɌĂ΂�܂�
	void End() override;

	//===== �g��Ȃ����͏����Ă�������() =====
	//--- �R�[���o�b�N�֐� ---
	// 2D�����蔻��
	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	//-------------------------------------------//
	//	�v���C���[�̌����擾
	//-------------------------------------------//
	Vector3 GetPlayerDir(void)
	{
		m_dir = m_dir.normalized();
		return m_dir;
	}

	void ResetDir() {
		m_dir = Vector3{ 0,0,0 };
	}

	void CheckDir()
	{
		if (m_dir->x == 0.0f && m_dir->y == 0.0f)
		{
			if (m_right) m_dir->x = 1.0f;
			else m_dir->x = -1.0f;
		}
	}

	void UpDir()	{ m_dir->y += 1.0f; }
	void DownDir()	{ m_dir->y -= 1.0f; }

	void SetDir(Vector3 dir) { m_dir = dir; }

	void SetJumpFlg(bool bJump) { m_bjunpFlag = bJump; }

	// �ړ��֐�
	// �E�ړ�
	void RightMove(float fSpeed = 1.0f);
	// ���ړ�
	void LeftMove(float fSpeed = -1.0f);
	// �O�ړ�
	void FrontMove();
	// ���ړ�
	void BackMove();

	// �ړ��X�g�b�v
	void StopMove();
	// ���R���[�u
	void FreeMove(Vector3 force);

	// �W�����v
	bool Jump();

	// �T�E���h��~
	void StopSE();
};



