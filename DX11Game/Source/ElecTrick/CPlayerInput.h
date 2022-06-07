//======================================================================
//											CPlayerInput.h
//	プレイヤー移動
//
//======================================================================
//	author :	AT12A 15 榊原龍我
//======================================================================
//	開発履歴
//	2020/11/16	作成
//	2020/12/16	宇佐美編集
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CCollision2D.h"
#include "CPlayerActionComponent.h"
#include "Renderer.h"
#include "CEnergyShot.h"
#include "CProductEnergy.h"


//===== クラス定義 =====
class CPlayerInput : public CComponent
{
private:
	// プレイヤーのアクション
	std::weak_ptr<CPlayerActionComponent> m_playerAction;
	std::weak_ptr<CEnergyShot> m_shot;
	std::weak_ptr<CProductEnergy> m_product;

	// 本体アニメーション
	std::weak_ptr<CAssimpRenderer> m_renderer;

	// コントローラーがあるか
	int m_nJoyCount;

	enum EPlayerAnimation
	{
		eAttack,
		eGeneration,
		eJump,
		eRun,

		eMaxAnim,
	};

public:

	// 作成時に呼ばれます
	void Start() override;

	// 毎フレーム呼ばれます
	void Update() override;

	// 毎フレームの更新後に呼ばれます
	void LateUpdate() override;

	// 消去時に呼ばれます
	void End() override;

	//--- コールバック関数 ---
	// 2D当たり判定
	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	// 3Dアニメーション
	void OnAnimationExit3D(const std::shared_ptr <CAssimpRenderer>& renderer) override;
};



