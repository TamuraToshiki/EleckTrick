//======================================================================
//												CSwitch.cpp
//	Assimpモデルベースのオブジェクトレイアウト
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/11/18	作成
//
//======================================================================


//===== インクルード部 =====
#include "CSwitch.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CDoorComponent.h"
#include "CSwitchComponent.h"
#include "CResourceEnergy.h"


//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/Electrode/Electrode.fbx"
#define TEXTURE_PATH			"data/model/Electrode/Electrode.png"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CSwitch::CSwitch()
{
	// トランスフォーム
	//m_pTrans->SetScale({ 25.0f,25.0f,25.0f });
	m_pTrans->m_pos->z += 100;
	m_pTrans->m_rot->x = -90;

	// タグの設定
	m_name = m_tag = "Switch";

	// モデルのロード
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();

	// スイッチ
	AddComponent<CSwitchComponent>();
	AddComponent<CResourceEnergy>();

	// コリジョン
	col->SetScale({ 25.0f, 25.0f, 25.0f });

	// リジッドボディ
	rb->SetSolid(true);
	rb->SetUseGravity(false);

}

//===================================
//
//	デストラクタ
//
//===================================
CSwitch::~CSwitch()
{

}


