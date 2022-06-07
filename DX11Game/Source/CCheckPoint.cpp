//======================================================================
//												CCheckPoint.cpp
//	チェックポイント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/13	作成
//
//======================================================================


//===== インクルード部 =====
#include "CCheckPoint.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CCheckPointComponent.h"


//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/Resporn/Resporn.fbx"
#define TEXTURE_PATH			"data/model/Resporn/Resporn_Tex10.png"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CCheckPoint::CCheckPoint()
{
	// トランスフォーム
	m_pTrans->m_pos->y -= 50;

	// タグの設定
	m_name = m_tag = "CheckPoint";

	// モデルのロード
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();

	AddComponent<CCheckPointComponent>();

	// 当たり判定
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
CCheckPoint::~CCheckPoint()
{

}
