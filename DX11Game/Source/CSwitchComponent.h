//======================================================================
//											CSwitchComponent.h
//	コンポーネント作成用のベースレイアウト
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/11/23	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CCollision2D.h"
#include "CResourceEnergy.h"
#include "CElecGimmickBase.h"

//===== クラス定義 =====
class CSwitchComponent : public CElecGimmickBase
{
private:
	// --- 変数宣言 ---
	bool bSwitchflg;


public:
	// --- 関数宣言 ---
	bool GetSwitchflg();

	// 作成時に呼ばれます
	void Start() override;


	// 毎フレームの更新後に呼ばれます
	void LateUpdate() override;


	void ChargePlayerEnter(std::weak_ptr<CCollision2D> collsion2d) override;

	// オーバーライド
	void ChargePlayerStay(std::weak_ptr<CCollision2D> collsion2d) override;

	void ChargePlayerExit(std::weak_ptr<CCollision2D> collsion2d) override;

	// 弾ギミック処理
	void EnergyOn() override;

	void EnergyOff() override;

	// フラグ
	void SetSwitchFlg(bool flg) { bSwitchflg = flg; }
	void ResetResource() { m_fEnergyBulletResource = 0.0f; }
};



