//======================================================================
//											CSecurityRoboComponent.h
//	�x�����{�p�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/12/05	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CVisibleComponent.h"
#include "CEnemyBase.h"
#include "CRespawnComponent.h"

//===== �N���X��` =====
class CSecurityRoboComponent : public CEnemyBase
{
private:
	// --- �ϐ��錾 ---
	int npt_cnt;		// ���p�^�[���J�E���g
	bool m_bDelayStand;	// �������]�h�~�ϐ�
	float spd;			// �X�s�[�h
	std::weak_ptr<CRigidbody2D> m_rb;
	std::weak_ptr<CAssimpRenderer> m_renderer;

	Vector3 vec;		// �x�N�g��
	Vector3 Eye;		// �ڂ̃|�W
	Vector3 m_U_turn;	// 
	Vector3 OldPos;		// �ߋ����W

	Vector3 m_initPos;	// �������W

	// �L���^�s���A�j���[�V����
	std::weak_ptr<CAssimpRenderer> m_caterpollar;
	// �M�A�A�j���[�V����
	std::weak_ptr<CAssimpRenderer> m_gear;

public:
	// --- �֐��錾 ---


	// �쐬���ɌĂ΂�܂�
	void Start() override;

	// �ŏI�X�V�֐�
	void LateUpdate() override;

	void End() override;

	void EngagementEnemy() override;

	void NormalEnemy() override;

	void Outage() override;

	void Restart() override;
};

