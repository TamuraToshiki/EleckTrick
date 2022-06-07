//======================================================================
//												CEndElevator.cpp
//	エンドエレベーター
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/03	作成
//
//======================================================================


//===== インクルード部 =====
#include "CEndElevator.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CEndElevatorComponent.h"


//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/Ev/Elevator_Ainm3.fbx"
#define TEXTURE_PATH			"data/model/Ev/Ev_4.png"

//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CEndElevator::CEndElevator()
{
	// トランスフォーム

	// タグの設定
	m_name = m_tag = "EndElevator";

	// メッシュの選択
	

	// モデルのロード
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();

	AddComponent<CEndElevatorComponent>();

	// コリジョン
	col->SetScale({ 200, 1, 400 });

	// リジッドボディ
	rb->SetSolid(true);
	rb->SetUseGravity(false);
}

//===================================
//
//	デストラクタ
//
//===================================
CEndElevator::~CEndElevator()
{

}
