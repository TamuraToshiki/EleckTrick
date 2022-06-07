//======================================================================
//											CMenuComponent.h
//	メニュー用のコンポーネント
//
//======================================================================
//	author :	AT12A 15 榊原龍我
//======================================================================
//	開発履歴
//	2020/11/16	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CSpriteRenderer.h"

//定数定義
#define MAX_MENU	(3)

//===== クラス定義 =====
class CMenuComponent : public CComponent
{
private:
	// --- 変数宣言 ---
	bool m_bOpen;				//メニュー表示フラグ
	bool m_bButton;				//決定フラグ
	int m_nMenuIndex;			//メニュー選択肢
	float m_fChoicesColor;		//ハイライト用
	float m_fTimer;				//連続入力防止タイマー
	std::weak_ptr<CSpriteRenderer> m_SpriteMenu[MAX_MENU];	//レンダラー受け取り用

	// コントローラーのスティック用
	bool m_bStick;

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

	//インデックス取得
	int GetIndex();

	//フラグ取得
	bool GetBOpen();

	//フラグセット
	void SetFlag(bool);
};
