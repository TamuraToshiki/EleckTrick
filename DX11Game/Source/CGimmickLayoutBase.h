//======================================================================
//											CGimmickLayoutBase.h
//	ギミックベースレイアウト
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
#include "CElecGimmickBase.h"


//===== クラス定義 =====
class CGimmickLayoutBase : public CElecGimmickBase
{
private:
	// --- 変数宣言 ---

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