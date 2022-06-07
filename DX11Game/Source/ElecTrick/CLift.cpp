//======================================================================
//												CLift.cpp
//	リフト
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/11/18	作成
//
//======================================================================


//===== インクルード部 =====
#include "CLift.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"

#include "CLiftComponent.h"


//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/Lift/Lift.fbx"
#define TEXTURE_PATH			"data/model/Lift/Lift_Tex2.png"

//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CLift::CLift()
{
	// トランスフォーム

	// タグの設定
	m_name = m_tag = "Lift";

	// メッシュの選択
	

	// モデルのロード
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);


	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();

	AddComponent<CLiftComponent>();

	// コリジョン
	col->SetScale({ 100, 20, 100 });

	// リジッドボディ
	rb->SetSolid(true);
	rb->SetUseGravity(false);
}

//===================================
//
//	デストラクタ
//
//===================================
CLift::~CLift()
{

}
