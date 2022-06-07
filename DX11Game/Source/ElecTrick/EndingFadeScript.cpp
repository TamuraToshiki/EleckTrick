//==================================================================
//								EndingFadeEndingFadeScript.cpp
//	エンディング演出用フェードスクリプト
//
//==================================================================
//	author :	AT13C 12 榊原龍我
//==================================================================
//	開発履歴
//
//	2021/06/18	移植開始
//
//===================================================================


//====== インクルード部 ======

// 自身
#include "EndingFadeScript.h"

// システム
#include "../Engine/System/input.h"
#include "../Engine/System/Sound.h"

// マネージャー
#include "../Engine/ECS/Entity/EntityManager.h"

// エンティティ
#include "../Engine/ECSEntity/GameObject.h"

// コンポーネント
#include "../Engine/ECSCompoent/Transform.h"
#include "../Engine/ECSCompoent/MeshRenderer.h"
#include "../Engine/ECSCompoent/BillboardRenderer.h"
#include "../Engine/ECSCompoent/AssimpRenderer.h"
#include "../Engine/ECSCompoent/SpriteRenderer.h"
#include "../Engine/ECSCompoent/InstancingMeshRenderer.h"
#include "../Engine/ECSCompoent/Rigidbody.h"
#include "../Engine/ECSCompoent/BoxCollider.h"
#include "../Engine/ECSCompoent/SphereCollider.h"

// ECSコンポーネント
#include "../Engine/ECSCompoent/DeltaCollider.h"

// ECSシステム
#include "../Engine/ECSSystem/DeltaCollisionSystem.h"


// ネームスペース
using namespace ECS;


//===== マクロ定義 =====
#define ADD_TIMER	(0.1f)

//******************** スクリプトメソッド ********************


//========================================
//
//	開始時
//
//========================================
void EndingFadeScript::Start()
{
	//タグ・名前設定
	gameObject().lock()->SetName("Ending");
	gameObject().lock()->SetTag("Ending");

	//初期化
	m_fTimer = 0.0f;
	m_fAlpha = 1.0f;
	m_fAlpha2 = 0.0f;
	m_fAlpha3 = 0.0f;
	m_bFadeEnd = false;
	m_bMoveEnd = false;

	//レンダラー
	//const auto& renderer = gameObject().lock()->AddComponent<SpriteRenderer>();
	//テクスチャのセット
	//レイヤーのセット
	//テクスチャのサイズセット
	//格納
	
}

//========================================
//
//	更新時
//
//========================================
void EndingFadeScript::Update()
{
	//タイマー開始
}

//========================================
//
//	後更新時
//
//========================================
void EndingFadeScript::LateUpdate()
{
}

//========================================
//
//	終了時
//
//========================================
void EndingFadeScript::End()
{
}


//******************** コールバック関数 ********************


//========================================
//
// 当たった時
//
//========================================
void EndingFadeScript::OnCollisionEnter(Collider* collider)
{

}

//========================================
//
// 当たっている間
//
//========================================
void EndingFadeScript::OnCollisionStay(Collider* collider)
{

}

//========================================
//
// 離れた時
//
//========================================
void EndingFadeScript::OnCollisionExit(Collider* collider)
{

}


//===== ECS =====

//========================================
//
// 当たった時
//
//========================================
void EndingFadeScript::OnDeltaCollisionEnter(DeltaCollider* collider)
{
}

//========================================
//
// 当たっている間
//
//========================================
void EndingFadeScript::OnDeltaCollisionStay(DeltaCollider* collider)
{
}

//========================================
//
// 離れた時
//
//========================================
void EndingFadeScript::OnDeltaCollisionExit(DeltaCollider* collider)
{
}

