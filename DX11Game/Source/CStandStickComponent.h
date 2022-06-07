//======================================================================
//											CStandStickComponent.h
//	コンポーネント作成用のベースレイアウト
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/11/16	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CCollision2D.h"
#include "CEnemyBase.h"

//===== クラスの前方宣言 =====
class CEnemyBase;


//===== クラス定義 =====
class CStandStickComponent : public CComponent
{
private:
	// --- 変数宣言 ---
	bool m_bGrandflg;

	std::weak_ptr<CEnemyBase> m_enemy;
	std::weak_ptr<CCollision2D> m_col;

public:
	// --- 関数宣言 ---

	// 作成時に呼ばれます
	void Start() override;

	// 毎フレーム呼ばれます
	void Update() override;

	// 毎フレームの更新後に呼ばれます
	void LateUpdate() override;

	// 可視範囲の大きさと方向変更関数
	void SetStandStick(std::weak_ptr<CEnemyBase> comp);

	// 地面接触判定取得関数
	bool GetGrandflg();

	//===== 使わない時は消してください() =====
	//--- コールバック関数 ---
	// 2D当たり判定
	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override;
	void OnCollisionExit2D(const std::shared_ptr <CCollision2D>& collsion2d) override;
};
