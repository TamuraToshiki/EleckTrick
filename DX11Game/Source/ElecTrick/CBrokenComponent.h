//======================================================================
//											CBrokenComponent.h
//	オブジェクト破壊コンポーネント
//
//======================================================================
//	author :	AT12A 20 土屋美月
//======================================================================
//	開発履歴
//	2020/12/03	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CElecGimmickBase.h"
#include "CBrokenObject.h"
#include "CObjectManager.h"


//===== クラス定義 =====
class CBrokenComponent : public CElecGimmickBase
{
private:
	// --- 変数宣言 ---
	float m_fBlockHP;
	Vector3 m_initPos;

public:
	// --- 関数宣言 ---


	// 作成時に呼ばれます
	void Start() override
	{
		m_fBlockHP = 10;
		m_fUseResource = 0;
		// 座標
		m_initPos = m_pParent->m_pTrans->m_pos;
		m_energyOffTexture = "data/model/BreakBrock/BreakBrock.png";
		m_energyOnTexture = "data/model/BreakBrock/BreakBrock.png";
	}

	void Restart() override
	{
		m_fBlockHP = 10;
		m_fUseResource = 0;
		// 座標
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




