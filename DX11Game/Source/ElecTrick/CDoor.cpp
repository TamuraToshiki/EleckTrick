//======================================================================
//												CDoor.cpp
//	ドア
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/11/20	作成
//
//======================================================================


//===== インクルード部 =====
#include "CDoor.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CDoorComponent.h"

//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/Door/Door.fbx"
#define TEXTURE_PATH			"data/model/Door/Door.png"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CDoor::CDoor()
{
	// トランスフォーム
	//m_pTrans->SetScale({25.0f,300.0f,200.0f});
	//m_pTrans->m_rot->y = 90;
	m_pTrans->m_pos->y -= 50;

	// タグの設定
	m_name = m_tag = "Door";

	// モデルのロード
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();
	const auto& dc = AddComponent<CDoorComponent>();

	//GetComponent<CDoorComponent>()->CreateSwitch(Vector3{100, 0, 0});
	//GetComponent<CDoorComponent>()->CreateSwitch(Vector3{200, 0, 0});
	//GetComponent<CDoorComponent>()->CreateSwitch(Vector3{300, 0, 0});

	// コリジョン
	col->SetScale({ 50.0f, 200.0f, 200.0f });
	col->SetOffSet({ 0, 100, 0 });

	// リジッドボディ
	rb->SetSolid(true);
	rb->SetUseGravity(false);

}

//===================================
//
//	デストラクタ
//
//===================================
CDoor::~CDoor()
{

}
