//======================================================================
//											CDamageTrapCollisionComponent.h
//	ダメージトラップ
//
//======================================================================
//	author :	AT12A 15 榊原龍我
//======================================================================
//	開発履歴
//	2020/12/13	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CObjectManager.h"
#include "CDamageTrapObj.h"
#include "CRespawnComponent.h"
#include <vector>

#include "debugproc.h"


//===== クラス定義 =====
class CDamageTrapCollisionComponent : public CComponent
{
private:
	// --- 変数宣言 ---
	std::weak_ptr<CCollision2D> m_col;		//リジッドボディ用
	std::weak_ptr<CRespawnComponent> m_res;

public:
	// --- 関数宣言 ---

	// 作成時に呼ばれます
	void Start() override;

	// 毎フレーム呼ばれます
	void Update() override;

	// 毎フレームの更新後に呼ばれます
	void LateUpdate() override;

	// 消去時に呼ばれます
	void End() override;

	//===== 使わない時は消してください() =====
	//--- コールバック関数 ---
	// 2D当たり判定
	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

};



