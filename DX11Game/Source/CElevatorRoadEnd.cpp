//======================================================================
//												CElevatorRoadEnd.cpp
//	エレベーターロードエンド
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/05	作成
//
//======================================================================


//===== インクルード部 =====
#include "CElevatorRoadEnd.h"


//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/Ev_RoadEnd/Ev_RoadEnd.fbx"
#define TEXTURE_PATH			"data/model/Ev_RoadEnd/Ev_RoadEnd_Tx.png"

//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CElevatorRoadEnd::CElevatorRoadEnd()
{
	// トランスフォーム

	// タグの設定
	m_name = m_tag = "ElevatorRoadEnd";

	// メッシュの選択
	

	// モデルのロード
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);


	//--- コンポーネントの追加

}

//===================================
//
//	デストラクタ
//
//===================================
CElevatorRoadEnd::~CElevatorRoadEnd()
{

}
