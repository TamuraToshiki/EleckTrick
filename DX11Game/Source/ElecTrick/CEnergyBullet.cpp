//======================================================================
//												CEnegyBullet.cpp
//	エネルギー弾
//
//======================================================================
//	author :	AT12A 15 榊原龍我
//======================================================================
//	開発履歴
//	2020/11/26	作成
//
//======================================================================


//===== インクルード部 =====
#include "CEnergyBullet.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CAnimater2D.h"
#include "CBulletComponent.h"
#include "CObject.h"
#include "CComponent.h"
#include "CResourceEnergy.h"


//===== マクロ定義 =====
#define TEXTURE_PATH			"data/texture/ElectricBall_Sprite2.png"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CEnergyBullet::CEnergyBullet()
{
	// トランスフォーム
	m_pTrans->SetScale({ 100, 100, 100 });

	// タグの設定
	m_name = m_tag = "EnergyBullet";

	// メッシュの選択
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);
	m_renderer.lock()->SetMeshBlendState(BS_ADDITIVE);

	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();
	const auto& am = AddComponent<CAnimator2D>();
	const auto& bullet = AddComponent<CBulletComponent>();
	AddComponent<CResourceEnergy>();

	// コリジョン
	col->SetScale({ 50, 50, 50 });

	// リジッドボディ
	//rb->SetSolid(true);
	rb->SetUseGravity(false);
	rb->SetGravityForce(0);
	rb->SetDragX(0);
	rb->SetTrigger(true);


	// アニメーション
	am->SetAnimationSpeed(1);
	am->SetSplit(9, 9);
	am->AddAnimationDefault("test", 9 * 9);
	am->SetAnimation("test");
}

//===================================
//
//	デストラクタ
//
//===================================
CEnergyBullet::~CEnergyBullet()
{

}
