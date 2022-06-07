//======================================================================
//											CSecurityRoboComponent.h
//	警備ロボ用コンポーネント
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/12/05	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CVisibleComponent.h"
#include "CEnemyBase.h"
#include "CRespawnComponent.h"

//===== クラス定義 =====
class CSecurityRoboComponent : public CEnemyBase
{
private:
	// --- 変数宣言 ---
	int npt_cnt;		// 交戦パターンカウント
	bool m_bDelayStand;	// 高速反転防止変数
	float spd;			// スピード
	std::weak_ptr<CRigidbody2D> m_rb;
	std::weak_ptr<CAssimpRenderer> m_renderer;

	Vector3 vec;		// ベクトル
	Vector3 Eye;		// 目のポジ
	Vector3 m_U_turn;	// 
	Vector3 OldPos;		// 過去座標

	Vector3 m_initPos;	// 初期座標

	// キャタピラアニメーション
	std::weak_ptr<CAssimpRenderer> m_caterpollar;
	// ギアアニメーション
	std::weak_ptr<CAssimpRenderer> m_gear;

public:
	// --- 関数宣言 ---


	// 作成時に呼ばれます
	void Start() override;

	// 最終更新関数
	void LateUpdate() override;

	void End() override;

	void EngagementEnemy() override;

	void NormalEnemy() override;

	void Outage() override;

	void Restart() override;
};

