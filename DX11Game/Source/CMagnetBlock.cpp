//======================================================================
//												CMagnetBlock.cpp
//	電磁石ブロック
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/11/16	作成
//
//======================================================================


//===== インクルード部 =====
#include "CMagnetBlock.h"
#include "CMagnetBlockComponent.h"
#include "CTestComponent.h"


//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/Magnet/Magnet.fbx"
#define TEXTURE_PATH			"data/model/Magnet/Magnet.png"

//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CMagnetBlock::CMagnetBlock()
{
	// トランスフォーム

	// タグの設定
	m_name = m_tag = "MagnetBlock";

	// モデルのロード
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	const auto& rb = AddComponent<CRigidbody2D>();
	AddComponent<CMagnetBlockComponent>();

	// コリジョン
	col->SetScale({ 100, 100, 100 });

	// リジッドボディ
	rb->SetSolid(true);
	rb->SetUseGravity(false);

}

//===================================
//
//	デストラクタ
//
//===================================
CMagnetBlock::~CMagnetBlock()
{

}
