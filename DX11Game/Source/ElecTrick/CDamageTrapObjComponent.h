//======================================================================
//											CDamageTrapObjComponent.h
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
#include "CDamageTrapCollisionComponent.h"
#include "CRespawnComponent.h"
#include <vector>

#include "debugproc.h"


//===== クラス定義 =====
class CDamageTrapObjComponent : public CComponent
{
private:
	// --- 変数宣言 ---
	std::weak_ptr<CCollision2D> m_col;		//リジッドボディ用
	std::vector<Vector3> m_vTrapPoint;		//ダメージトラップのポイント
	int m_nCurrentRoot;						//ダメージトラップの配列要素数
	int m_MaxTrapPoint;						//トラップポイントの最大数
	Vector3 pos;							//トラップの向き
	Vector3 scale;		//大きさ
	Vector3 m_Radian;	//角度
	
	std::weak_ptr<CDamageTrapCollisionComponent> m_DamegeTrap;
	std::vector<std::weak_ptr<CObject>> m_aTrapPoint;
	std::vector<std::weak_ptr<CObject>> m_aTrapcol;
	std::vector<std::weak_ptr<CObject>> m_aTrapEffect;
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
	void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	//-- ダメージトラップポイント作成 --//
	void CreateTrapPoint(Vector3);

	//-- ダメージトラップ作成 --//
	void CreateTrap();

	// 角度を求める関数
	Vector3 GetRadian(Vector3, Vector3);

	// トラップポイント
	std::vector<std::weak_ptr<CObject>>& GetTrapPoint() { return m_aTrapPoint; }
};



