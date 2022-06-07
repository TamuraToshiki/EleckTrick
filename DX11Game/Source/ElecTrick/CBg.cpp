//======================================================================
//												CBg.cpp
//	2D背景
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/01	作成
//
//======================================================================


//===== インクルード部 =====
#include "CBg.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"


//===== マクロ定義 =====
#define TEXTURE_PATH			"data/texture/wall/4.jpg"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===== 静的メンバ =====


//===================================
//
//	コンストラクタ
//
//===================================
CBg::CBg()
{
	// トランスフォーム
	m_pTrans->SetScale({ 100, 100, 100 });
	m_pTrans->m_rot->x = -90;


	// タグの設定
	m_name = m_tag = "Bg";

	// メッシュの選択
	m_renderer.lock()->MakePlane(m_tag, 50, 50, 100, 100, 100, 100);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);
	m_renderer.lock()->SetNormalTexture("data/texture/normal/Cross_Normal.png");
	m_renderer.lock()->SetCullMode(false);
	m_renderer.lock()->SetZBufferFlg(false);
	//m_renderer.lock()->SetAmbientTexture("data/texture/skydome.png");

	//--- コンポーネントの追加

}

//===================================
//
//	デストラクタ
//
//===================================
CBg::~CBg()
{

}
