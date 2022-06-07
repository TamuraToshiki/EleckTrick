//======================================================================
//												CBlocksensor.cpp
//	Assimpモデルベースのオブジェクトレイアウト
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/11/16	作成
//
//======================================================================


//===== インクルード部 =====
#include "CBlocksensor.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CBlocksensorComponent.h"

//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/box.fbx"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CBlocksensor::CBlocksensor()
{
	// トランスフォーム
	m_pTrans;

	// タグの設定
	m_tag = "Blocksensor";

	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	AddComponent<CBlocksensorComponent>();

	col->SetScale({ 10.0f,10.0f,50.0f });
	col->SetOffSet({ -60.0f,50.0f,0.0f });

}

//===================================
//
//	デストラクタ
//
//===================================
CBlocksensor::~CBlocksensor()
{

}
