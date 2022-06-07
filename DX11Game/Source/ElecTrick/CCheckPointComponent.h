//======================================================================
//											CCheckPointComponent.h
//	チェックポイントコンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/15	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CElecGimmickBase.h"
#include "Renderer.h"
#include "CRespawnComponent.h"


//===== クラス定義 =====
class CCheckPointComponent : public CElecGimmickBase
{
private:
	// --- 変数宣言 ---
	bool m_bOn;
	std::weak_ptr<CAssimpRenderer> m_renderer;
	std::weak_ptr<CRespawnComponent> m_respawn;

public:
	// --- 関数宣言 ---

	// 作成時に呼ばれます
	void Start() override;

	// ギミック更新関数
	void GimmickUpdate() override;

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

};