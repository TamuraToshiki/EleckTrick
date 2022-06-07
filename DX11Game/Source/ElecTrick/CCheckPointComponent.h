//======================================================================
//											CCheckPointComponent.h
//	�`�F�b�N�|�C���g�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/15	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CElecGimmickBase.h"
#include "Renderer.h"
#include "CRespawnComponent.h"


//===== �N���X��` =====
class CCheckPointComponent : public CElecGimmickBase
{
private:
	// --- �ϐ��錾 ---
	bool m_bOn;
	std::weak_ptr<CAssimpRenderer> m_renderer;
	std::weak_ptr<CRespawnComponent> m_respawn;

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