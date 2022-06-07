//======================================================================
//											CEndingFadeComponent.h
//	エンディング演出用のフェードコンポーネント
//
//======================================================================
//	author :	AT12A 15 榊原龍我
//======================================================================
//	開発履歴
//	2021/01/21 作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CSpriteRenderer.h"
#include <functional>


//===== クラス定義 =====
class CEndingFadeComponent : public CComponent
{
private:
	// --- 変数宣言 ---
	float m_fTimer;			//タイマー
	float m_fAlpha;			//α値用
	float m_fAlpha2;		//α値用2
	float m_fAlpha3;		//α値用3
	float m_fTextColor;		//ハイライト用

	bool m_bMoveEnd;		//移動終了フラグ
	bool m_bFadeEnd;		//フェード終了フラグ
	
	std::weak_ptr<CSpriteRenderer> m_SpriteEnding[5];	//レンダラー

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



