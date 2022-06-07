//==================================================================
//								MenuScript.h
//	メニューコンポーネントの移植
//
//==================================================================
//	author :	AT13C 12 榊原龍我
//==================================================================
//	開発履歴
//
//	2021/06/17	移植開始
//
//===================================================================

#pragma once

//====== インクルード部 ======
#include "../Engine/ECSCompoent/Script.h"


//===== マクロ定義 =====
#define MAX_MENU (3)

//===== クラス定義 =====
namespace ECS
{
	class MenuScript : public Script
	{
	private:
		// ----- メンバ -----
		bool m_bStick;					//コントローラーのスティック入力
		bool m_bOpen;					//メニューが表示されたか
		bool m_bButton;					//ボタンが入力されたか
		int m_nMenuIndex;				//選択肢
		float m_fChoiseColor;			//選択肢のハイライト
		float m_fTimer;					//連続入力防止
		std::weak_ptr<SpriteRenderer>	//スプライト受け取り用
			m_SpriteMenu[MAX_MENU];

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

		//インデックスの取得
		int GetIndex();

		//表示フラグの取得
		bool GetOpenFrag();

		//表示フラグのセット
		void SetOpenFlag(bool);

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
