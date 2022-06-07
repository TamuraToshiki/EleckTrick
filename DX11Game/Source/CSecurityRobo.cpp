//======================================================================
//												CSecurityRobo.cpp
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
#include "CSecurityRobo.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CSecurityRoboComponent.h"
#include "CObjectManager.h"

//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/Enemy_Anim_Blender/Enemy_Anim_Blender.fbx"
#define TEXTURE_PATH			"data/model/EnemyA_Mix/EnemyA_Tex4.png"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CSecurityRobo::CSecurityRobo()
{
	// トランスフォーム
	m_pTrans;

	// タグの設定
	m_tag = "SecurityRobo";

	// モデルのロード
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);

	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	AddComponent<CRigidbody2D>();
	AddComponent<CSecurityRoboComponent>();

	col->SetScale({ 80.0f,80.0f,100.0f });
	col->SetOffSet({ 0, 40, 0 });
}

//===================================
//
//	デストラクタ
//
//===================================
CSecurityRobo::~CSecurityRobo()
{

}
