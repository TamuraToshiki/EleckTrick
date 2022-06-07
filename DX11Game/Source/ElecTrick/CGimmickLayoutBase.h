//======================================================================
//											CGimmickLayoutBase.h
//	�M�~�b�N�x�[�X���C�A�E�g
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
class CGimmickLayoutBase : public CElecGimmickBase
{
private:
	// --- �ϐ��錾 ---

public:
	// --- �֐��錾 ---

	// �쐬���ɌĂ΂�܂�
	void Start() override;

	// �M�~�b�N�X�V�֐�
	void GimmickUpdate() override;

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

};