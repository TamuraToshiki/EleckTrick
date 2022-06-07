//======================================================================
//												CObjectBaseLayout.cpp
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
#include "CObjectBaseLayout.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"

//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/box.fbx"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CObjectBaseLayout::CObjectBaseLayout()
{
	// トランスフォーム
	m_pTrans;

	// タグの設定
	m_name = m_tag = "Object";

	// モデルのロード
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);

	//--- コンポーネントの追加
	AddComponent<CCollision2D>();
	AddComponent<CRigidbody2D>();

}

//===================================
//
//	デストラクタ
//
//===================================
CObjectBaseLayout::~CObjectBaseLayout()
{

}
