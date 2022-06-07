//======================================================================
//											CSpriteEffectComponent.h
//	スプライトエフェクトコンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2021/01/24	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CSpriteRenderer.h"
#include "CAnimater2D.h"


//===== クラス定義 =====
class CSpriteEffectComponent : public CComponent
{
public:
	// エフェクトの種類
	enum EEffectType
	{
		eHitEffect,
		eSaveEffect,
		eSmokeEffect,

		eMaxEffect,
	};

private:
	// --- 変数宣言 ---

	// レンダラー
	std::weak_ptr<CSpriteRenderer> m_renderer;

	// アニメーター
	std::weak_ptr<CAnimator2D> m_animator;

	// ループ
	bool m_bLoop;

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

	// エフェクトの指定
	void SetEffect(EEffectType eType);

	// 任意エフェクトの指定
	void SetAnyEffect(std::string filename, int nSplitX, int nSplitY, int nSpeed = 2, bool bLoop = false, int nStartNo = 0);

	//--- コールバック関数 ---
	// アニメーション
	void OnAnimationExit2D(const std::shared_ptr <CAnimator2D>& animator2d) override;
};



