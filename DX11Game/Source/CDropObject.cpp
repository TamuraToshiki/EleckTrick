//======================================================================
//												CDropObject.cpp
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
#include "CDropObject.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CDropComponent.h"

//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/FallBrock/FallBrock.fbx"
#define TEXTURE_PATH			"data/model/FallBrock/FallBrock.png"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CDropObject::CDropObject()
{
	// トランスフォーム
	m_pTrans->m_pos->y -= 50;

	// タグの設定
	m_tag = "DropObj";

	// モデルのロード
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb =  AddComponent<CRigidbody2D>();
	AddComponent<CDropComponent>();


	// コリジョン
	col->SetScale({ 100, 100, 100 });
	col->SetOffSet({ 0,50,0 });

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
CDropObject::~CDropObject()
{

}
