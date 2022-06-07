//======================================================================
//											CGearUIComponent.h
//	コンポーネント作成用のベースレイアウト
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/11/16	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CSpriteRenderer.h"
#include "CRigidbody2D.h"
#include "CClearComponent.h"

//===== クラス定義 =====
class CGearUIComponent : public CComponent
{
private:
	// --- 変数宣言 ---

	int m_nNumberCnt;	// キープ時間
	float m_fRoll;
	float m_fAlpha;		// 透明度
	int m_nrot;

	std::weak_ptr<CSpriteRenderer> m_renderer;

	enum NUMBER_FADE
	{
		eNone,
		eFadeIn,
		eFadeKeep,
		eFadeOut,

		eMaxState,
	};

	NUMBER_FADE m_curFade;

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

	void SetGearRot(int rot);

	//void SetStarForce(Vector3);

	//===== 使わない時は消してください() =====
	//--- コールバック関数 ---
	// 2D当たり判定
	void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override;
};
