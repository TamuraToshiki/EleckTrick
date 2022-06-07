//======================================================================
//											CBrokenComponent.h
//	�I�u�W�F�N�g�j��R���|�[�l���g
//
//======================================================================
//	author :	AT12A 20 �y������
//======================================================================
//	�J������
//	2020/12/03	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CElecGimmickBase.h"
#include "CBrokenObject.h"
#include "CObjectManager.h"


//===== �N���X��` =====
class CBrokenComponent : public CElecGimmickBase
{
private:
	// --- �ϐ��錾 ---
	float m_fBlockHP;
	Vector3 m_initPos;

public:
	// --- �֐��錾 ---


	// �쐬���ɌĂ΂�܂�
	void Start() override
	{
		m_fBlockHP = 10;
		m_fUseResource = 0;
		// ���W
		m_initPos = m_pParent->m_pTrans->m_pos;
		m_energyOffTexture = "data/model/BreakBrock/BreakBrock.png";
		m_energyOnTexture = "data/model/BreakBrock/BreakBrock.png";
	}

	void Restart() override
	{
		m_fBlockHP = 10;
		m_fUseResource = 0;
		// ���W
		m_pParent->m_pTrans->m_pos = m_initPos;

	}

	void ChargePlayerEnter(std::weak_ptr<CCollision2D>) override
	{

	}

	void ChargePlayerStay(std::weak_ptr<CCollision2D>) override
	{

	}

	void ChargePlayerExit(std::weak_ptr<CCollision2D>) override
	{

	}

	void EnergyOn() override
	{
		m_fUseResource = m_fEnergyBulletResource;
		m_fBlockHP -= m_fUseResource;

		if (m_fBlockHP < 0)
		{
			m_pParent->m_pTrans->m_pos = Vector3{ 99999,99999,99999 };
		}
	}

	void EnergyOff() override
	{

	}

	void CreateBlockHP(float fHP)
	{
		m_fBlockHP = fHP;
	}
	

};




