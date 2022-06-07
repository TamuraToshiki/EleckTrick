//======================================================================
//											CEnemyBase.h
//	�G�l�~�[�x�[�X���C�A�E�g
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
#include "CVisibleComponent.h"
#include "CStandStickComponent.h"
#include "CBlocksensorComponent.h"
#include "CRespawnComponent.h"
#include "CElecGimmickBase.h"

//===== �N���X�̑O���錾 =====
class CVisibleComponent;
class CStandStickComponent;
class CBlocksensorComponent;


//===== �N���X��` =====
class CEnemyBase : public CElecGimmickBase
{
private:
	// --- �ϐ��錾 ---
	std::weak_ptr<CRespawnComponent> m_res;
	std::weak_ptr<CRigidbody2D> m_rb;

protected:
	std::weak_ptr<CObject> m_player;
	std::weak_ptr<CVisibleComponent> m_visi;
	std::weak_ptr<CStandStickComponent> m_stand;
	std::weak_ptr<CBlocksensorComponent> m_blsen;
	bool m_bFindflg;
	bool m_bStandflg;
	bool m_bBlockflg;
	bool m_bOutageflg;

public:
	// --- �֐��錾 ---
	void Start() override;

	// �M�~�b�N�X�V�֐�
	void GimmickUpdate() override;

	// �ŏI�X�V�֐�
	virtual void LateUpdate() override;

	virtual void End() override;

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

	void SetPlayer(std::weak_ptr<CObject>);

	// ���֐�
	virtual void EngagementEnemy() = 0;

	// �ʏ�֐�
	virtual void NormalEnemy() = 0;

	// �@�\��~
	virtual void Outage() = 0;

	void OnCollisionStay2D(const std::shared_ptr<CCollision2D>& collsion2d) override;
	void OnCollisionEnter2D(const std::shared_ptr<CCollision2D>& collsion2d) override;

};