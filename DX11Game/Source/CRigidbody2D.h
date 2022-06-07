//==================================================================
//									CRigidbody2D.h
//	リジッドボディ
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/07/12	リジッドボディクラス作成
//
//===================================================================
#pragma once

//====== インクルード部 ======
#include "CComponent.h"

//===== マクロ定義 =====

namespace DIRECTION
{
	enum EDIRECTION
	{
		RIGHT,
		LEFT,
		UP,
		DOWN,

		MAX_DIRECTION,
	};
}

//===== クラス定義 =====
class CTransform;
class CCollision2D;

// メモ： 汎用型にして
class CRigidbody2D : public CComponent
{
public:
	CRigidbody2D();
	~CRigidbody2D();

	void Start() override;
	void Update() override;

	// 押し出し
	void Extrusion(const std::shared_ptr<CRigidbody2D>& pOther);
	void ExtrusionCircleToCircle(CRigidbody2D *pOther);

	// ゲット関数
	CTransform* GetObj() { return m_pTrans; }
	bool GetUseGravity() { return m_bUseGravity; }
	DIRECTION::EDIRECTION GetHitDir() { return m_enHitDir; }
	Vector3 GetForce() { return m_force; }
	Vector3 GetVelocity() { return m_velocity; }
	Vector3 GetDir() { return m_dir; }
	bool GetSolid() { return m_bSolid; }
	void SetMoveLimit(bool bLimit) { m_bMoveLimit = bLimit; }

	// セット関数
	void SetForce(Vector3 force) { m_force = force; }
	void SetForceX(float forceX) { m_force->x = forceX; }
	void SetForceY(float forceY) { m_force->y = forceY; }
	void SetForceZ(float forceZ) { m_force->z = forceZ; }
	void AddForceX(float forceX) { m_force->x += forceX; }
	void AddForceY(float forceY) { m_force->y += forceY; }
	void AddForceZ(float forceZ) { m_force->z += forceZ; }
	void AddForce(Vector3 force) { m_force += force; }
	void SetDragX(float fDragX) { m_drag->x = fDragX; }
	void SetDragY(float fDragY) { m_drag->y = fDragY; }
	void SetDragZ(float fDragZ) { m_drag->z = fDragZ; }

	void SetTorque(Vector3 torque) { m_torque = torque; }
	void SetTorqueX(float torqueX) { m_torque->x = torqueX; }
	void SetTorqueY(float torqueY) { m_torque->y = torqueY; }
	void SetTorqueZ(float torqueZ) { m_torque->z = torqueZ; }
	void AddTorqueX(float torqueX) { m_torque->x += torqueX; }
	void AddTorqueY(float torqueY) { m_torque->y += torqueY; }
	void AddTorqueZ(float torqueZ) { m_torque->z += torqueZ; }
	void AddTorque(Vector3 torque) { m_torque += torque; }
	void SetTorqueDragX(float fTorqueDragX) { m_torqueDrag->x = fTorqueDragX; }
	void SetTorqueDragY(float fTorqueDragY) { m_torqueDrag->y = fTorqueDragY; }
	void SetTorqueDragZ(float fTorqueDragZ) { m_torqueDrag->z = fTorqueDragZ; }


	void SetGravityForce(float fForce) { m_fGraviyForce = fForce; }
	void SetUseGravity(bool bUse) { m_bUseGravity = bUse; }
	void SetTrigger(bool bTrigger) { m_bTrigger = bTrigger; }
	void SetSolid(bool bSolid) { m_bSolid = bSolid; }
	void SetMass(float fMass) { m_fMass = fMass; }

	void SetCol(const std::shared_ptr<CCollision2D> col) { m_col = col; }
private:
	CTransform *m_pTrans;
	std::weak_ptr<CCollision2D> m_col;

	// パラメータ
	Vector3 m_velocity;
	Vector3 m_force;
	Vector3 m_drag;

	Vector3 m_angularVelocity;
	Vector3 m_torque;
	Vector3 m_torqueDrag;

	float m_fMass;
	bool m_bSolid;		// 不動オブジェクト
	Vector3 m_dir;		// 向き
	float m_fGraviyForce;

	DIRECTION::EDIRECTION m_enHitDir;

	bool m_bUseGravity;
	bool m_bTrigger;
	bool m_bMoveLimit;
};


