//======================================================================
//												CVisible.cpp
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
#include "CVisible.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CVisibleComponent.h"

//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/box.fbx"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CVisible::CVisible()
{
	// トランスフォーム
	m_pTrans;

	// タグの設定
	m_tag = "Visible";

	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	AddComponent<CVisibleComponent>();

}

//===================================
//
//	デストラクタ
//
//===================================
CVisible::~CVisible()
{

}
