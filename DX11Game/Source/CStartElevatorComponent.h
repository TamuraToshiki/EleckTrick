//======================================================================
//											CStartElevatorComponent.h
//	スタートエレベーターコンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/01	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "Renderer.h"


//===== クラス定義 =====
class CStartElevatorComponent : public CComponent
{
private:
	// --- 変数宣言 ---
	std::weak_ptr<CRigidbody2D> m_rb;
	std::weak_ptr<CAssimpRenderer> m_render;
	std::weak_ptr<CObject> m_player;

	std::vector<std::weak_ptr<CObject>> m_aEVRoad;

	// 初期位置
	Vector3 m_startPos;
	// 到着位置
	Vector3 m_goalPos;

	enum EState
	{
		eWait,
		eStartMove,
		eOpenDoor,
		eOutMove,
		eCloseDoor,

		eMaxState,
	};

	// 現在の状態
	EState m_curState;

public:
	// --- 関数宣言 ---

	// 作成時に呼ばれます
	void Start() override;

	// 毎フレーム呼ばれます
	void Update() override;

	// 毎フレームの更新後に呼ばれます
	void LateUpdate() override;

	// 終了関数
	void End() override;

	//===== 使わない時は消してください() =====
	//--- コールバック関数 ---
	// 2D当たり判定
	void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override;
	void OnAnimationExit3D(const std::shared_ptr <CAssimpRenderer>& collsion2d) override;

};



