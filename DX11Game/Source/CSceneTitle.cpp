//==================================================================
//								CSceneTitle.cpp
//	タイトルシーン
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/12/15	作成
//
//===================================================================



//====== インクルード部 ======
#include "CSceneTitle.h"
#include "CObjectManager.h"
#include "input.h"
#include "camera.h"
#include "CBg.h"
#include "CSceneManager.h"
#include "CSceneGame.h"
#include "Sound.h"

#include "CTitleMenuObj.h"
#include "CEndingObj.h"
#include "CGameManagerObject.h"
#include "CEditorObject.h"
#include "CEditorComponent.h"
#include "CSecurityRobo.h"


//===== マクロ定義 =====
#define MAP_NUM		(100)
#define TITLE_TEXTURE_PATH		"data/texture/title.png"
#define PUSH_TEXTURE_PATH		"data/texture/Title_menu/PUSH_BUTTON.png"

//===== グローバル変数 =====


//========================================
//
//	コンストラクタ
//
//========================================
CSceneTitle::CSceneTitle()
{
	m_sceneName = "Title";
	m_bOpen = false;
}


//========================================
//
//	デストラクタ
//
//========================================
CSceneTitle::~CSceneTitle()
{

}


//========================================
//
//	初期化
//
//========================================
void CSceneTitle::Init()
{
	// BGM
	CSound::PlayBGM("TitleBGM.wav", 0.3f);

	//ステージ番号設定
	CEditorComponent::SetCurrentStageNum(MAP_NUM);

	//背景生成
	CObjectManager::CreateObject<CBg>({ 50, 50, 150 });
	const auto& editor = CObjectManager::CreateObject<CEditorObject>();
	editor->SetStop(true);

	//セキュリティロボ設定
	const auto& robo = CObjectManager::CreateObject<CSecurityRobo>();
	robo.get()->m_pTrans->m_pos = Vector3({ 1600.0f,1000.0f,0.0f });

	//カメラ設定
	CCamera::GetMainCamera()->SetCameraTarget(robo->m_pTrans);
	CCamera::GetMainCamera()->UpdateCameraPos(100);
	CCamera::GetMainCamera()->SetOffSet({ 0.0f,200.0f, 0.0f });

	// タイトル
	const auto& title = CObjectManager::CreateObject<CSpriteObject>();
	const auto& titleSpr = title->GetComponent<CSpriteRenderer>();
	const auto& push = CObjectManager::CreateObject<CSpriteObject>();
	const auto& pushSpr = push->GetComponent<CSpriteRenderer>();
	
	//レンダラー代入
	m_Sprite = titleSpr;
	m_Push = pushSpr;

	// タイトル設定
	title->m_pTrans->SetScale({ SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f });
	titleSpr->m_pParent->m_pTrans->m_pos = Vector3{ 0.0f,100.0f,0.0f };
	titleSpr->SetLayer(titleSpr->eUI1);
	titleSpr->SetTexture(TITLE_TEXTURE_PATH);

	//ボタン示唆設定
	push->m_pTrans->SetScale({ 860.0f,82.0f,1.0f });
	pushSpr->SetTexture(PUSH_TEXTURE_PATH);
	pushSpr->m_pParent->m_pTrans->m_pos->y = -230.0f;



}
//========================================
//
//	終了処理
//
//========================================
void CSceneTitle::Uninit()
{
	CSound::StopBGM("TitleBGM.wav");
}

//========================================
//
//	全てのオブジェクトの更新
//
//========================================
void CSceneTitle::Update()
{
	//タイトル上下
	static int nCnt = 10;
	nCnt += 2;
	m_Sprite.lock()->m_pParent->m_pTrans->m_pos->y += ((sinf(nCnt * 3.1415f / 100)));

	
	static int nTime = 0;
	nTime += 1;
	m_fAlpha = abs((sinf(nTime * 3.1415f / 100)));
	if(!m_bAlpha)
	m_Push.lock()->SetAlpha(m_fAlpha);

	//シーン変更
	if (GetKeyTrigger(VK_SPACE) || GetKeyTrigger(VK_RETURN) ||
		GetJoyRelease(JOYSTICKID1, JOYBUTTON_A))
	{
		m_Push.lock()->SetAlpha(0.0f);
		m_bAlpha = true;
		CSound::PlaySE("ChoiseDecide.mp3");
		if(!m_bOpen)
		CObjectManager::CreateObject<CTitleMenuObj>();

		m_bOpen = true;
	}

	//エンディングへ
#ifdef _DEBUG
	if (GetKeyTrigger(VK_P))
	{
		CSound::StopBGM("TitleBGM.wav");
		CObjectManager::CreateObject<CEndingObj>();
	}
#endif


}


//========================================
//
//	全てのオブジェクトの描画
//
//========================================
void CSceneTitle::Draw()
{
}

