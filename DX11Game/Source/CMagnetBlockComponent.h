//======================================================================
//											CMagnetBlockComponent.h
//	�d�����R���|�[�l���g
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/12/01
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CElecGimmickBase.h"


//===== �N���X��` =====
class CMagnetBlockComponent : public CElecGimmickBase
{
private:
	// --- �ϐ��錾 ---
	std::weak_ptr<CObject> m_player;
	static std::weak_ptr<CMagnetBlockComponent>	g_mainMagnet;

	int m_nNum;	// �����̔ԍ�
	static int m_nMaxNum;	// �S�̂̔ԍ�
	static int m_nCurrNum;	// ���݂̏��������u���b�N�ԍ�

	// �R���g���[���[��
	int m_nJoyCount;

public:
	// --- �֐��錾 ---

	// �쐬���ɌĂ΂�܂�
	void Start() override;

	// ���X�^�[�g
	void Restart() override;

	// �M�~�b�N�X�V�֐�
	void GimmickUpdate() override;

	void End() override;

	// �ѓd�N������
	void ChargePlayerEnter(std::weak_ptr<CCollision2D>) override;

	// �ѓd�؍ݏ���
	void ChargePlayerStay(std::weak_ptr<CCollision2D>) override;

	// �ѓd�P�ޏ���
	void ChargePlayerExit(std::weak_ptr<CCollision2D>) override;

	// �d�C�e�ғ��֐�
	void EnergyOn() override;

	// �d�C�e�x�~�֐�
	void EnergyOff() override;

	void OnCollisionEnter2D(const std::shared_ptr<CCollision2D>& collsion2d) override;

	void OnCollisionStay2D(const std::shared_ptr<CCollision2D>& collsion2d) override;

};