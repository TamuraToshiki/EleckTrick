//======================================================================
//												CDamageTrapCollision.cpp
//	ダメージトラップ
//
//======================================================================
//	author :	AT12A 15 榊原龍我
//======================================================================
//	開発履歴
//	2020/12/13	作成
//
//======================================================================


//===== インクルード部 =====
#include "CDamageTrapCollsion.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CDamageTrapCollisionComponent.h"

//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/box.fbx"
//#define TEXTURE_PATH			"data/model/trap.png"		


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CDamageTrapCollision::CDamageTrapCollision()
{
	// トランスフォーム
	m_pTrans;

	// タグの設定
	m_name = m_tag = "DamageTrapCol";

	// モデルのロード
	//m_renderer.lock()->SetMeshBlendState(BS_ADDITIVE);
	//m_renderer.lock()->SetDiffuseColor({ 1,0,0,1 });

	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	col->SetScale({ 100,100,100 });
	AddComponent<CDamageTrapCollisionComponent>();
}

//===================================
//
//	デストラクタ
//
//===================================
CDamageTrapCollision::~CDamageTrapCollision()
{

}
