//==================================================================
//								EndingFadeScript.h
//	エンディング演出用スクリプト
//
//==================================================================
//	author :	AT13C 12 榊原龍我
//==================================================================
//	開発履歴
//
//	2021/06/18	移植開始
//
//===================================================================

#pragma once

//====== インクルード部 ======
#include "../Engine/ECSCompoent/Script.h"


//===== マクロ定義 =====


//===== クラス定義 =====
namespace ECS
{
	class EndingFadeScript : public Script
	{
	private:
		// ----- メンバ -----
		float m_fTimer;			//タイマー
		float m_fAlpha;			//α値用
		float m_fAlpha2;		//α値用2
		float m_fAlpha3;		//α値用3
		float m_fHighlight;		//ハイライト用

		bool m_bMoveEnd;		//移動終了したか
		bool m_bFadeEnd;		//フェード終了したか

		std::weak_ptr<SpriteRenderer>
			m_EndingSprite[5];	//レンダラー

	protected:
		// ----- メソッド -----

		// 開始時に呼ばれます
		void Start() override;
		// 毎フレーム呼ばれます
		void Update() override;
		// 毎フレーム更新後に呼ばれます
		void LateUpdate() override;
		// 終了時に呼ばれます
		void End() override;


		// ----- コールバック関数 -----

		// 当たった時
		void OnCollisionEnter(Collider* collider) override;
		// 当たっている間
		void OnCollisionStay (Collider* collider) override;
		// 離れた時
		void OnCollisionExit (Collider* collider) override;

		// ECS
		// 当たった時
		void OnDeltaCollisionEnter(DeltaCollider* collider) override;
		// 当たっている間
		void OnDeltaCollisionStay(DeltaCollider* collider) override;
		// 離れた時
		void OnDeltaCollisionExit(DeltaCollider* collider) override;
	};
}
