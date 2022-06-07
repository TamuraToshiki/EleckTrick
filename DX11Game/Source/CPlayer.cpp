//======================================================================
//												CPlayer.cpp
//	プレイヤー
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/11/16	作成
//
//======================================================================


//===== インクルード部 =====
#include "CPlayer.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"

#include "CPlayerActionComponent.h"
#include "CPlayerInput.h"
#include "CEnergyShot.h"
#include "CProductEnergy.h"
#include "CRespawnComponent.h"

#include "CPlayerUIComponent.h"


//===== マクロ定義 =====
#define DEFAULT_MODEL			"data/model/MainAnim_Blender/Main_Re_Blender.fbx"
#define TEXTURE_PATH			"data/model/Model_Main/texture_10.png"


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CPlayer::CPlayer()
{
	// トランスフォーム
	//m_pTrans->SetScale({ 100, 50, 50 });


	// タグの設定
	m_name = m_tag = "Player";

	// モデルのロード
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);


	//--- コンポーネントの追加
	const auto& col = AddComponent<CCollision2D>();
	AddComponent<CRigidbody2D>();

	AddComponent<CResourceEnergy>();
	AddComponent<CProductEnergy>();
	AddComponent<CRespawnComponent>();
	
	AddComponent<CPlayerActionComponent>();
	AddComponent<CEnergyShot>();
	AddComponent<CPlayerInput>();

	AddComponent<CPlayerUIComponent>();

	// コリジョン
	col->SetScale({ 90, 90, 90 });
	col->SetOffSet({ 0, 40, 0 });
}

//===================================
//
//	デストラクタ
//
//===================================
CPlayer::~CPlayer()
{

}

