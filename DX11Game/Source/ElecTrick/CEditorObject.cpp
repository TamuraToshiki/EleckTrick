//======================================================================
//												CEditorObject.cpp
//	エディターのオブジェクト
//
//======================================================================
//	author :	AT12A 12 紀平
//======================================================================
//	開発履歴
//	2020/12/03	作成
//				紀平から引き継ぎ/榊原
//======================================================================


//===== インクルード部 =====
#include "CEditorObject.h"
#include "CEditorComponent.h"


//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/box.fbx"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CEditorObject::CEditorObject()
{
	// トランスフォーム
	m_pTrans->m_scale;

	// タグの設定
	m_name = m_tag = "Editor";

	// モデルのロード
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture("data/texure/white.png");
	m_renderer.lock()->SetMeshBlendState(EBlendState::BS_ALPHABLEND);
	m_renderer.lock()->SetDiffuseColor({ 0,1,0,1.0f });


	//--- コンポーネントの追加
	AddComponent<CEditorComponent>();
}

//===================================
//
//	デストラクタ
//
//===================================
CEditorObject::~CEditorObject()
{

}
