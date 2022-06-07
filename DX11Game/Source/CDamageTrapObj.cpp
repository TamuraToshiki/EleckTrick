//======================================================================
//												CDamageTrapObj.cpp
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
#include "CDamageTrapObj.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CDamageTrapObjComponent.h"

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
CDamageTrapObj::CDamageTrapObj()
{
	// トランスフォーム
	m_pTrans;

	// タグの設定
	m_name = m_tag = "DamageTrap";

	// モデルのロード
	//m_renderer.lock()->SetMeshBlendState(BS_ADDITIVE);
	//m_renderer.lock()->SetDiffuseColor({ 0,1,0,1 });

	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	AddComponent<CDamageTrapObjComponent>();

	col->SetScale({ 50,50,50 });

}

//===================================
//
//	デストラクタ
//
//===================================
CDamageTrapObj::~CDamageTrapObj()
{

}
