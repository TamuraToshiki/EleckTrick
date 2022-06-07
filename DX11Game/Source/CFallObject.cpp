//======================================================================
//												CFallObject.cpp
//	Assimpモデルベースのオブジェクトレイアウト
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/11/16	作成
//
//======================================================================


//===== インクルード部 =====
#include "CFallObject.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CFallComponent.h"

//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/EnagyFallBrock/EnagyFallBrock.fbx"
#define TEXTURE_PATH			"data/model/EnagyFallBrock/EnagyFallBrock.png"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CFallObject::CFallObject()
{
	// トランスフォーム

	// タグの設定
	m_tag = "FallObj";

	// モデルのロード
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();
	AddComponent<CFallComponent>();


	// コリジョン
	col->SetScale({ 100, 100, 100 });

	// リジッドボディ
	rb->SetSolid(true);
	rb->SetUseGravity(false);
	rb->SetMass(4);

}

//===================================
//
//	デストラクタ
//
//===================================
CFallObject::~CFallObject()
{

}
