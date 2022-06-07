//==================================================================
//								CSceneEnding.h
//	エンディングシーン
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2021/01/23	作成
//
//===================================================================



//====== インクルード部 ======
#include "CSceneEnding.h"
#include "CObjectManager.h"
#include "input.h"
#include "camera.h"
#include "CBg.h"
#include "CSpriteObject.h"
#include "CSceneManager.h"
#include "CSceneGame.h"
#include "Sound.h"

#include "CEndingObj.h"
#include "CGameManagerObject.h"
#include "CEditorObject.h"
#include "CEditorComponent.h"


//===== マクロ定義 =====


//===== グローバル変数 =====


//========================================
//
//	コンストラクタ
//
//========================================
CSceneEnding::CSceneEnding()
{
	m_sceneName = "Ending";
}


//========================================
//
//	デストラクタ
//
//========================================
CSceneEnding::~CSceneEnding()
{

}


//========================================
//
//	初期化
//
//========================================
void CSceneEnding::Init()
{
	CSound::StopBGM("BGM.mp3");

	// エンディング
	CObjectManager::CreateObject<CEndingObj>();

	// ステージを初期化
	CEditorComponent::SetCurrentStageNum(0);
}

//========================================
//
//	終了処理
//
//========================================
void CSceneEnding::Uninit()
{
}

//========================================
//
//	全てのオブジェクトの更新
//
//========================================
void CSceneEnding::Update()
{
}


//========================================
//
//	全てのオブジェクトの描画
//
//========================================
void CSceneEnding::Draw()
{
}

