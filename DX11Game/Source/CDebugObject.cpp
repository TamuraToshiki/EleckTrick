//======================================================================
//												CDebugObject.cpp
//	デバック用オブジェクト
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/11/26	作成
//
//======================================================================


//===== インクルード部 =====
#include "CDebugObject.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CAnimater2D.h"
#include "CTestComponent.h"

//===== マクロ定義 =====
#define TEXTURE_PATH			"data/texture/circle2.png"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CDebugObject::CDebugObject()
{
	// トランスフォーム
	//m_pTrans->SetScale({ SCREEN_WIDTH, SCREEN_HEIGHT, 1 });


	// タグの設定
	m_name = m_tag = "DebugObject";

	// メッシュの選択
	//m_renderer.lock()->SetTexture(TEXTURE_PATH);
	//m_renderer.lock()->SetMeshBlendState(BS_ADDITIVE);
	//m_renderer.lock()->SetLayer(CSpriteRenderer::eFade1);
	//m_renderer.lock()->SetMeshBlendState(EBlendState::BS_SUBTRACTION);
	//m_renderer.lock()->SetTexSize({ 2, 2, 0 });
	//m_renderer.lock()->SetTexPos({ -1.0, -1.0, 0 });

	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();
	const auto& am = AddComponent<CAnimator2D>();

	// コリジョン
	col->SetScale({ 100, 100, 100 });

	// リジッドボディ
	rb->SetSolid(true);
	rb->SetUseGravity(false);


	// アニメーター
	//am->SetSplit(9, 9);
	//am->SetAnimationSpeed(1);
	//am->AddAnimationDefault("test", 9*9);

	//am->SetAnimation("test");
}

//===================================
//
//	デストラクタ
//
//===================================
CDebugObject::~CDebugObject()
{

}
