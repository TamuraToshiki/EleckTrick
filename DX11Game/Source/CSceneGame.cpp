//==================================================================
//								CSceneGame.cpp
//	ゲームシーン
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/12/14	作成
//
//===================================================================



//====== インクルード部 ======
#include "CSceneGame.h"
#include "CObjectManager.h"
#include "transform.h"
#include "input.h"
#include "camera.h"
#include "CCollision2D.h"
#include "CObjectBaseLayout.h"
#include "CPlayer.h"
#include "CBlock.h"
#include "CBg.h"
#include "CEnergyBullet.h"
#include "CStartElevator.h"
#include "CEndElevator.h"
#include "CDebugObject.h"
#include "CEditorObject.h"

#include "CGameManagerObject.h"

#include "Sound.h"


//===== マクロ定義 =====


//===== グローバル変数 =====

//========================================
//
//	コンストラクタ
//
//========================================
CSceneGame::CSceneGame()
{
	m_sceneName = "Game";
}


//========================================
//
//	デストラクタ
//
//========================================
CSceneGame::~CSceneGame()
{

}


//========================================
//
//	初期化
//
//========================================
void CSceneGame::Init()
{
	// オブジェクトマネージャー
	CObjectManager::CreateObject<CGameManagerObject>();

	// BGM
	CSound::PlayBGM("BGM.mp3", 0.5f);

	// 背景の生成
	CObjectManager::CreateObject<CBg>({ 50, 50, 150 });

	// バレット
	CObjectManager::CreateObject<CEnergyBullet>({ 10000, 10000, 1000 });
}
//========================================
//
//	終了処理
//
//========================================
void CSceneGame::Uninit()
{

}

//========================================
//
//	全てのオブジェクトの更新
//
//========================================
void CSceneGame::Update()
{

}


//========================================
//
//	全てのオブジェクトの描画
//
//========================================
void CSceneGame::Draw()
{
}

