//======================================================================
//												CFadeObject.cpp
//	フェードオブジェクト
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/06	作成
//
//======================================================================


//===== インクルード部 =====
#include "CFadeObject.h"
#include "CFadeComponent.h"


//===== マクロ定義 =====
#define TEXTURE_PATH			"data/texture/circle2.png"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CFadeObject::CFadeObject()
{
	// トランスフォーム
	m_pTrans->SetScale({ SCREEN_WIDTH, SCREEN_HEIGHT, 1 });

	// タグの設定
	m_name = m_tag = "FadeObject";

	// メッシュの選択
	m_renderer.lock()->SetTexture(TEXTURE_PATH);
	m_renderer.lock()->SetLayer(CSpriteRenderer::eFade1);
	//m_renderer.lock()->SetMeshBlendState(EBlendState::BS_ADDITIVE);
	m_renderer.lock()->SetMeshBlendState(EBlendState::BS_SUBTRACTION);

	//--- コンポーネントの追加
	AddComponent<CFadeComponent>();

}

//===================================
//
//	デストラクタ
//
//===================================
CFadeObject::~CFadeObject()
{

}
