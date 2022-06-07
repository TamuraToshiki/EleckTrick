//======================================================================
//											CMagnetBlockComponent.h
//	電磁床コンポーネント
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/12/01
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CElecGimmickBase.h"


//===== クラス定義 =====
class CMagnetBlockComponent : public CElecGimmickBase
{
private:
	// --- 変数宣言 ---
	std::weak_ptr<CObject> m_player;
	static std::weak_ptr<CMagnetBlockComponent>	g_mainMagnet;

	int m_nNum;	// 自分の番号
	static int m_nMaxNum;	// 全体の番号
	static int m_nCurrNum;	// 現在の処理されるブロック番号

	// コントローラー数
	int m_nJoyCount;

public:
	// --- 関数宣言 ---

	// 作成時に呼ばれます
	void Start() override;

	// リスタート
	void Restart() override;

	// ギミック更新関数
	void GimmickUpdate() override;

	void End() override;

	// 帯電侵入処理
	void ChargePlayerEnter(std::weak_ptr<CCollision2D>) override;

	// 帯電滞在処理
	void ChargePlayerStay(std::weak_ptr<CCollision2D>) override;

	// 帯電撤退処理
	void ChargePlayerExit(std::weak_ptr<CCollision2D>) override;

	// 電気弾稼働関数
	void EnergyOn() override;

	// 電気弾休止関数
	void EnergyOff() override;

	void OnCollisionEnter2D(const std::shared_ptr<CCollision2D>& collsion2d) override;

	void OnCollisionStay2D(const std::shared_ptr<CCollision2D>& collsion2d) override;

};