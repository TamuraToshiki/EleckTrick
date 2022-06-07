//======================================================================
//											CVisibleComponent.h
//	可視範囲用コンポーネント
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/12/04	作成
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
class CVisibleComponent : public CComponent
{
private:
	// --- 変数宣言 ---
	// 当たり判定フラグ
	bool m_bVisibleflg;
	bool m_bTouchflg;

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
	void SetTargetEnemy(std::weak_ptr<CEnemyBase> enemy, Vector3 scale = Vector3{ -100,50,0 }, Vector3 offset = Vector3{ -100,50,0 });

	// 方向変更用変数
	void SetDir(Vector3 offset);

	Vector3 GetDir();

	// 当たり判定取得関数
	bool GetVisibleflg();


	//===== 使わない時は消してください() =====
	//--- コールバック関数 ---
	// 2D当たり判定
	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	void OnCollisionExit2D(const std::shared_ptr <CCollision2D>& collsion2d) override;
};



