//======================================================================
//											CElecGimmickBase.h
//	コンポーネント作成用のベースレイアウト
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/11/26	作成
//		 11/27	Enter,Stay,Exitの純粋仮想関数 追加
//				帯電true→falseになった際にギミックが止まらないバグが見つかったため
//		 11/29	ChargeGimmickが分かりにくかったため、ChargePlayerに変更
//				それに伴い継承してる<CLiftComponent><CSwitchComponent>も変更
//		 11/30  バグの原因のため、ChargePlayerExitのリソース消費排除
//				m_fEnergyBulletResourceの残量が0になった際にギミックが止まらないバグを発見
//				純粋仮想関数EnergyOff()を追加し、差別を図るため、EnergyGimmickをEnergyOnに名称変更
//				
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CResourceEnergy.h"
#include "CEnergyShot.h"


//===== クラス定義 =====
class CElecGimmickBase : public CComponent
{
private:
	// --- 変数宣言 ---
	bool m_bCurChargeflg;
	bool m_bOldChargeflg;

protected:
	float m_fUseResource;	// 消費量
	float m_fEnergyBulletResource;		// エネルギー量を保管する変数

	std::string m_energyOnTexture;		// 通電時のテクスチャ
	std::string m_energyOffTexture;		// 非通電時のテクスチャ

public:
	// --- 関数宣言 ---
	// 帯電純粋仮想関数
	virtual void ChargePlayerEnter(std::weak_ptr<CCollision2D>) = 0;
	virtual void ChargePlayerStay(std::weak_ptr<CCollision2D>) = 0;
	virtual void ChargePlayerExit(std::weak_ptr<CCollision2D>) = 0;
	// 電気弾純粋仮想関数
	virtual void EnergyOn() = 0;
	virtual void EnergyOff() = 0;
	// ギミックアップデート
	virtual void GimmickUpdate() {};

	// 帯電

	// 作成時に呼ばれます
	virtual void Start() override;

	// 毎フレーム呼ばれます
	void Update() final override;

	// 毎フレームの更新後に呼ばれます
	virtual void LateUpdate() override;

	// ギミックのリスタート
	virtual void Restart(){}


	//===== 使わない時は消してください() =====
	//--- コールバック関数 ---
	// 2D当たり判定
	void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	
	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	void OnCollisionExit2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

};



