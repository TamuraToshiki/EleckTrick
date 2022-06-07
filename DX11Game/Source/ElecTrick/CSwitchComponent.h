//======================================================================
//											CSwitchComponent.h
//	�R���|�[�l���g�쐬�p�̃x�[�X���C�A�E�g
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/11/23	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CCollision2D.h"
#include "CResourceEnergy.h"
#include "CElecGimmickBase.h"

//===== �N���X��` =====
class CSwitchComponent : public CElecGimmickBase
{
private:
	// --- �ϐ��錾 ---
	bool bSwitchflg;


public:
	// --- �֐��錾 ---
	bool GetSwitchflg();

	// �쐬���ɌĂ΂�܂�
	void Start() override;


	// ���t���[���̍X�V��ɌĂ΂�܂�
	void LateUpdate() override;


	void ChargePlayerEnter(std::weak_ptr<CCollision2D> collsion2d) override;

	// �I�[�o�[���C�h
	void ChargePlayerStay(std::weak_ptr<CCollision2D> collsion2d) override;

	void ChargePlayerExit(std::weak_ptr<CCollision2D> collsion2d) override;

	// �e�M�~�b�N����
	void EnergyOn() override;

	void EnergyOff() override;

	// �t���O
	void SetSwitchFlg(bool flg) { bSwitchflg = flg; }
	void ResetResource() { m_fEnergyBulletResource = 0.0f; }
};



