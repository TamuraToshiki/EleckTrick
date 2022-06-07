//======================================================================
//												CDamegeTrapPoint.cpp
//	ダメージトラップ
//
//======================================================================
//	author :	AT12A 18　田村敏基
//======================================================================
//	開発履歴
//	2020/12/14	作成
//
//======================================================================


//===== インクルード部 =====
#include "CDamegeTrapPoint.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"


//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/DamageTrap/DamageTrap.fbx"
#define TEXTURE_PATH			"data/model/DamageTrap/DamageTrap_On.png"		


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CDamegeTrapPoint::CDamegeTrapPoint()
{
	// トランスフォーム
	m_pTrans->m_rot->x = -90;
	m_pTrans->m_pos->z += 50;

	// タグの設定
	m_name = m_tag = "DamageTrapPoint";

	// モデルのロード
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);
	//m_renderer.lock()->SetMeshBlendState(BS_ADDITIVE);
	//m_renderer.lock()->SetDiffuseColor({ 0,1,1,1 });

	//--- コンポーネントの追加

}

//===================================
//
//	デストラクタ
//
//===================================
CDamegeTrapPoint::~CDamegeTrapPoint()
{

}
