//==================================================================
//									CCollision2D.h
//	コリジョンコンポーネント
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/11/16	CCollision2Dクラスの作成
//
//===================================================================
#pragma once

//====== インクルード部 ======
#include "CComponent.h"
#include <list>

//===== マクロ定義 =====


//===== クラス定義 =====
class CObject;
class CRigidbody2D;


//===== クラス定義 =====
class CCollision2D final : public CComponent
{
public:
	CCollision2D();
	~CCollision2D();

	void Start() override;
	void Update() override;

	static void CollisionUpdate();
	static void CollisionDraw();


	// 矩形と矩形
	static bool CheckCollisionRectToRect(Vector3 centerPos1, Vector3 centerPos2, Vector3 size1, Vector3 size2);
	// 円と円
	static bool CheckCollisionCircleToCircle(Vector3 centerPos1, Vector3 centerPos2, float radius1, float radius2);
	// 円と矩形
	static bool CheckCollisionCircleToRect(Vector3 centerPos1, Vector3 centerPos2, float radius1, Vector3 size2);
	// 線と線
	static bool CheckCollisionLine(Vector3 pos1, Vector3 pos2, Vector3 pos3, Vector3 pos4, Vector3* outPos, float *outT1);

	// セット関数
	void SetOffSet(Vector3 offset)	{ m_offset = offset; }
	void SetScale(Vector3 scale) { m_scale = scale; }
	void SetRb(const std::weak_ptr<CRigidbody2D>& rb) { m_rb = rb; }

	// ゲット関数
	const Vector3& GetOffSet() { return m_offset; }
	const Vector3& GetScale() { return m_scale; }

private:
	static std::list<std::weak_ptr<CCollision2D>> m_list;

	std::weak_ptr<CCollision2D> m_this;
	std::weak_ptr<CRigidbody2D> m_rb;

	Vector3 m_offset;
	Vector3 m_scale;
	Vector3 m_rot;

	bool m_bOldState;
	bool m_bCurState;

	XMFLOAT4X4 m_mtxWorld;

	// 型定義
	using col_weak = std::weak_ptr<CCollision2D>;
	static inline void Collision(const col_weak& main_w, const std::list<col_weak>& m_pSubList);

	static void DefaultCollisionUpdate();
	static void CellCollisionUpdate();

};



