//======================================================================
//											CFadeComponent.h
//	フェードコンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/06	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CSpriteObject.h"
#include <functional>


//===== クラス定義 =====
class CFadeComponent : public CComponent
{
private:
	// --- 変数宣言 ---
	std::weak_ptr<CSpriteRenderer> m_render;
	std::shared_ptr<CObject> m_obj[4];

	float m_fScale;	// フェード用ポリゴンのアルファ値(0～255)
	int m_nState;	// 状態
	int m_nFrame;	// フェードにかかるフレーム数

	bool m_bNowFade;// フェード中か

	std::function<void(void)> m_fadeEndFunc;

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

	// フェードアウトの開始
	void StartFadeOut(int nFrame = 60);

	// フェード中か
	bool GetIsFade() { return m_bNowFade; }

	// フェード後の関数オブジェクトを登録
	void SetFadeEndFunc(std::function<void(void)> func) { m_fadeEndFunc = func; }

};



