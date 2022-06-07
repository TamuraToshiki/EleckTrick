//==================================================================
//								CHierarchyComponent.h
//	タイトルでのΔ数表示UI
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2021/01/15	タイトルΔ表示UIクラス作成
//
//===================================================================

#pragma once

//====== インクルード部 ======
#include "CSpriteRenderer.h"
#include "CComponent.h"
#include "CEditorComponent.h"

//===== マクロ定義 =====


//===== クラス定義 =====

class CHierarchyComponent;

class CHierarchyComponentData :public CComponent
{
	friend CHierarchyComponent;
private:
	// ----- メンバ -----
	std::weak_ptr<CSpriteRenderer> m_sprite;

	int m_nAnimNo;

public:
	// ----- メソッド -----

	// 開始時に呼ばれます
	void Start() override;

};

class CHyphenComponent :public CComponent
{
	friend CHierarchyComponent;
private:
	// ----- メンバ -----
	std::weak_ptr<CSpriteRenderer> m_sprite;

public:
	// ----- メソッド -----

	// 開始時に呼ばれます
	void Start() override;
};

class CHierarchyComponent : public CComponent
{
private:
	// ----- メンバ -----

	int m_nScore;
	int m_nNumberCnt;
	float m_fAlpha;

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

	// 数字
	std::vector<std::weak_ptr<CHierarchyComponentData>> m_data;
	std::weak_ptr<CHyphenComponent> m_hyphen;

public:
	// スコアのセット
	void SetScore(int nScore) { m_nScore = nScore; }

	// ----- メソッド -----

	// 開始時に呼ばれます
	void Start() override;
	// 毎フレーム呼ばれます
	void Update() override;
	// 毎フレーム更新後に呼ばれます
	void LateUpdate() override;
	// 終了時に呼ばれます
	void End() override;

	// UV更新
	void UpdateTexCoord();
};
