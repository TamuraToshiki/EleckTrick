//======================================================================
//											CPlayerUIComponent.h
//	プレイヤーUIコンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/12	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CSpriteObject.h"
#include "CSpriteRenderer.h"
#include "CResourceEnergy.h"


//===== クラス定義 =====
class CPlayerUIComponent : public CComponent
{
private:
	// --- 変数宣言 ---

	// ゲージ
	std::weak_ptr<CObject> m_gaugeObj;
	std::weak_ptr<CSpriteRenderer> m_gaugeSprite;

	// UI本体
	std::weak_ptr<CObject> m_UIObj;
	std::weak_ptr<CSpriteRenderer> m_UISprite;

	// ゲージエフェクト
	std::weak_ptr<CObject> m_gaugeEffectObj;

	// リソース
	std::weak_ptr<CResourceEnergy> m_resource;


	// 値
	float m_fResourceNum;

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

};



