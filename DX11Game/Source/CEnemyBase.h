//======================================================================
//											CEnemyBase.h
//	エネミーベースレイアウト
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/12/01
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CVisibleComponent.h"
#include "CStandStickComponent.h"
#include "CBlocksensorComponent.h"
#include "CRespawnComponent.h"
#include "CElecGimmickBase.h"

//===== クラスの前方宣言 =====
class CVisibleComponent;
class CStandStickComponent;
class CBlocksensorComponent;


//===== クラス定義 =====
class CEnemyBase : public CElecGimmickBase
{
private:
	// --- 変数宣言 ---
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
	// --- 関数宣言 ---
	void Start() override;

	// ギミック更新関数
	void GimmickUpdate() override;

	// 最終更新関数
	virtual void LateUpdate() override;

	virtual void End() override;

	// 帯電侵入処理
	void ChargePlayerEnter(std::weak_ptr<CCollision2D>) override;

	// 帯電滞在処理
	void ChargePlayerStay(std::weak_ptr<CCollision2D>) override;

	// 帯電撤退処理
	void ChargePlayerExit(std::weak_ptr<CCollision2D>) override;

	// 電気弾稼働関数
	void EnergyOn() override;

	// 電気弾休止関数
	void EnergyOff() override;

	void SetPlayer(std::weak_ptr<CObject>);

	// 交戦関数
	virtual void EngagementEnemy() = 0;

	// 通常関数
	virtual void NormalEnemy() = 0;

	// 機能停止
	virtual void Outage() = 0;

	void OnCollisionStay2D(const std::shared_ptr<CCollision2D>& collsion2d) override;
	void OnCollisionEnter2D(const std::shared_ptr<CCollision2D>& collsion2d) override;

};