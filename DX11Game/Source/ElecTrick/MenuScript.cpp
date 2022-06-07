//==================================================================
//								MenuScript.cpp
//	メニューコンポーネントの移植
//
//==================================================================
//	author :	AT13C 12 榊原龍我
//==================================================================
//	開発履歴
//
//	2021/06/17	移植開始
//
//===================================================================


//====== インクルード部 ======

// 自身
#include "MenuScript.h"

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



//******************** スクリプトメソッド ********************


//========================================
//
//	開始時
//
//========================================
void MenuScript::Start()
{
	m_fTimer = 0.0f;
	m_bOpen = true;
	m_bButton = false;

	if (m_bOpen)
	{
		m_nMenuIndex = 0;
	}

	//選択肢オブジェクトを作成
	
}

//========================================
//
//	更新時
//
//========================================
void MenuScript::Update()
{
}

//========================================
//
//	後更新時
//
//========================================
void MenuScript::LateUpdate()
{
}

//========================================
//
//	終了時
//
//========================================
void MenuScript::End()
{
}


//---------------------------------------
//
//	インデックス取得
//
//---------------------------------------
int MenuScript::GetIndex()
{
	return m_nMenuIndex;
}

//---------------------------------------
//
//	表示フラグ取得
//
//---------------------------------------
bool MenuScript::GetOpenFrag()
{
	return m_bOpen;
}

//---------------------------------------
//
//	表示フラグ設定
//
//---------------------------------------
void MenuScript::SetOpenFlag(bool bFlag)
{
	m_bOpen = bFlag;
}

//******************** コールバック関数 ********************


//========================================
//
// 当たった時
//
//========================================
void MenuScript::OnCollisionEnter(Collider* collider)
{

}

//========================================
//
// 当たっている間
//
//========================================
void MenuScript::OnCollisionStay(Collider* collider)
{

}

//========================================
//
// 離れた時
//
//========================================
void MenuScript::OnCollisionExit(Collider* collider)
{

}


//===== ECS =====

//========================================
//
// 当たった時
//
//========================================
void MenuScript::OnDeltaCollisionEnter(DeltaCollider* collider)
{
}

//========================================
//
// 当たっている間
//
//========================================
void MenuScript::OnDeltaCollisionStay(DeltaCollider* collider)
{
}

//========================================
//
// 離れた時
//
//========================================
void MenuScript::OnDeltaCollisionExit(DeltaCollider* collider)
{
}

