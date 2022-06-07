//==================================================================
//								CSceneManager.h
//	シーン管理
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/07/16	シーン管理クラス作成
//
//===================================================================


//====== インクルード部 ======
#include "CSceneManager.h"

#include "CSceneTitle.h"
#include "CSceneGame.h"

#include "CObjectManager.h"
#include "CCollision2D.h"


//===== マクロ定義 =====


//===== グローバル変数 =====


//===== 静的メンバ =====


//========================================
//
//	コンストラクタ
//
//========================================
CSceneManager::CSceneManager()
{
	// 最初のシーン
	m_pCurrentScene = nullptr;
	m_pNextScene = nullptr;

	m_bStop = false;
	m_bSceneChange = false;

}


//========================================
//
//	デストラクタ
//
//========================================
CSceneManager::~CSceneManager()
{
	// 最初のシーン
	m_pCurrentScene = nullptr;
	m_pNextScene = nullptr;

	m_bStop = false;
	m_bSceneChange = false;
}


//========================================
//
//	初期化
//
//========================================
void CSceneManager::Init()
{
	// ストップフラグ
	m_bStop = false;
	// シーン変更
	m_bSceneChange = false;

	// フェードの初期化
	for (const auto& com : m_fade.GetComponentList()) com->Start();

	// 最初のシーンのセット
	m_pCurrentScene	= new CSceneTitle();
	m_pNextScene	= m_pCurrentScene;


	// オブジェクトクリア
	CObjectManager::ClearObjectList();

	// 最初のシーンの初期化
	m_pCurrentScene->Init();

}


//========================================
//
//	終了処理
//
//========================================
void CSceneManager::Uninit()
{
	// シーンの終了処理
	m_pCurrentScene->Uninit();

	CObjectManager::ClearObjectList();
}


//========================================
//
//	全てのオブジェクトの更新
//
//========================================
void CSceneManager::Update()
{
	// 現在のシーンの更新処理
	m_pCurrentScene->Update();

	// オブジェクトの更新
	CObjectManager::Update();

	// 当たり判定
	CCollision2D::CollisionUpdate();

	// オブジェクトの後更新
	CObjectManager::LateUpdate();

	// オブジェクトの消去
	CObjectManager::DestroyUpdate();

	// フェードの更新
	m_fade.Update();
	m_fade.LateUpdate();
}


//========================================
//
//	全てのオブジェクトの描画
//
//========================================
void CSceneManager::Draw()
{
	// 現在のシーンの描画処理
	m_pCurrentScene->Draw();

	// 当たり判定の描画
	CCollision2D::CollisionDraw();
}



//========================================================
//
//	次のシーンへ移動
//
//========================================================
void CSceneManager::SetSceneChange()
{
	// 現在のシーンの終了処理を呼ぶ
	m_pCurrentScene->Uninit();

	// オブジェクトクリア
	CObjectManager::ClearObjectList();

	// 現在のシーンの消去
	delete m_pCurrentScene;

	// 現在のシーンの変更
	m_pCurrentScene = m_pNextScene;

	// 次のシーンの初期化
	m_pCurrentScene->Init();

	// シーン変更フラグオフ
	m_bSceneChange = false;
}
