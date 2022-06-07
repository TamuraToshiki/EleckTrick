//======================================================================
//											CLiftComponent.h
//	リフトを動かすコンポーネント
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/11/19	作成
//		 11/20　完成
//		 11/28　ChargePlayerEnter,ChargePlayerStay,ChargePlayerExit追加
//		 11/30  CreatePointが名前被りを引き起こす可能性があるため、
//				CreateLiftPointに変更
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CCollision2D.h"
#include "CResourceEnergy.h"
#include "CElecGimmickBase.h"

#include "debugproc.h"


//===== クラス定義 =====
class CLiftComponent : public CElecGimmickBase
{
private:
	// --- 変数宣言 ---
	std::weak_ptr<CRigidbody2D> m_rb;
	std::weak_ptr<CResourceEnergy> m_re;
	int currentRoot;						// リフトポイント配列の要素
	int OldcurrentRoot;
	std::vector<Vector3> wayPoints;			// リフトポイント(リフトの目指すポイント)
	int MaxWayPoint;		// リフトポイントの最大要素数
	Vector3 vec;							// リフト方向
	float speed;							// リフトスピード
	bool bLiftMoveflg;
	bool m_bTurnFlg;						// 通電フラグ
	bool m_bBulletFlg;

	std::vector<std::weak_ptr<CObject>> m_aLiftPint;

public:
	// --- 関数宣言 ---

	// 作成時に呼ばれます
	void Start() override;

	void End() override;

	void GimmickUpdate() override;

	void ChargePlayerEnter(std::weak_ptr<CCollision2D> collsion2d) override;

	void CreateLiftPoint(Vector3 pos);

	// オーバーライド
	// 帯電ギミック処理
	void ChargePlayerStay(std::weak_ptr<CCollision2D> collsion2d) override;

	// プレイヤーがリフトと離れたとき
	void ChargePlayerExit(std::weak_ptr<CCollision2D> collsion2d) override;

	// 弾ギミック処理
	void EnergyOn() override;

	void EnergyOff() override;

	void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	std::vector<std::weak_ptr<CObject>>GetLiftPoint() { return m_aLiftPint; }
};

