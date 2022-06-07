//======================================================================
//												CBrokenObject.cpp
//	Assimpモデルベースのオブジェクトレイアウト
//
//======================================================================
//	author :	AT12A 20 土屋美月
//======================================================================
//	開発履歴
//	2020/12/03	作成
//
//======================================================================


//===== インクルード部 =====
#include "CBrokenObject.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CBrokenComponent.h"

//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/BreakBrock/BreakBrock.fbx"
#define TEXTURE_PATH			"data/texture/BreakBrock/BreakBrock.png"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CBrokenObject::CBrokenObject()
{
	// トランスフォーム
	//m_pTrans->SetScale({ 1, 1, 1 });

	// タグの設定
	m_tag = "BrokenObj";

	// モデルのロード
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();
	AddComponent<CBrokenComponent>();


	// コリジョン
	col->SetScale({ 100, 100, 100 });

	// リジッドボディ
	rb->SetSolid(true);
	rb->SetUseGravity(false);

}

//===================================
//
//	デストラクタ
//
//===================================
CBrokenObject::~CBrokenObject()
{

}
