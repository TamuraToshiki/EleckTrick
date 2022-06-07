//======================================================================
//											CMenuComponent.cpp
//	メニュー用のコンポーネント
//
//======================================================================
//	author :	AT12A 15 榊原龍我
//======================================================================
//	開発履歴
//	2021/01/19 作成
//
//======================================================================


//===== インクルード部 =====
#include "CMenuComponent.h"
#include "input.h"
#include "Sound.h"

#include "CSceneManager.h"
#include "CSceneGame.h"

#include "CEditorComponent.h"
#include "CGameStartObj.h"
#include "CEditorModeObj.h"
#include "CExitObj.h"


//===== マクロ定義 =====
#define MIN_INDEX (0)
#define MAX_INDEX (2)

#define TIME_INTERVAL	(2.0f)		//連続入力防止間隔
#define TIME_INTERVAL2	(100.0f)	

#define C_POS_Y		(-230.0f)		//選択肢位置Y
#define C_SIZE_X	(280.0f)		//選択肢サイズX
#define C_SIZE_Y	(159.0f)		//選択肢サイズY

#define EDIT_SIZE_X	(280.0f * 1.2)		//はてなサイズX
#define EDIT_SIZE_Y	(159.0f * 1.2)		//はてなサイズY

#define MAP_NUM		(-1)					//ステージ番号

//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CMenuComponent::Start()
{
	m_fTimer = 0.0f;
	m_bOpen = true;
	m_bButton = false;

	if (m_bOpen)
	{
		m_nMenuIndex = 0;
	}


	//選択肢のオブジェクトを生成
	//オブジェクトを配列へ格納
	m_SpriteMenu[0] =
		CObjectManager::CreateObject<CGameStartObj>({ -370.0f,C_POS_Y,0.0f }, 
													{ 0.0f,0.0f,0.0f }, 
													{ C_SIZE_X,C_SIZE_Y,1.0f })
		->GetComponent<CSpriteRenderer>();
	
	m_SpriteMenu[1] = 
		CObjectManager::CreateObject<CEditorModeObj>({20.0f,C_POS_Y,0.0f},
													 {0.0f,0.0f,0.0f}, 
													 { EDIT_SIZE_X,EDIT_SIZE_Y,1.0f})
		->GetComponent<CSpriteRenderer>();
	
	m_SpriteMenu[2] =
		CObjectManager::CreateObject<CExitObj>({ 400.0f,C_POS_Y,0.0f }, 
											   { 0.0f,0.0f,0.0f }, 
											   { C_SIZE_X,C_SIZE_Y,1.0f })
		->GetComponent<CSpriteRenderer>();
	
	//レイヤーのセット
	m_SpriteMenu[0].lock()->SetLayer(4);
	m_SpriteMenu[1].lock()->SetLayer(4);
	m_SpriteMenu[2].lock()->SetLayer(4);
}


//===================================
//
//	更新関数
//
//===================================
void CMenuComponent::Update()
{
	m_fTimer += 1.0f;

	//文字ハイライト
	static int nCnt = 0;
	nCnt += 2;
	m_fChoicesColor = abs(sinf(nCnt * 3.1415f / 100));

	float JoyX = (float)GetJoyX(JOYSTICKID1);
	JoyX /= 32767.0f;
	if (GetJoyCount() <= 0)
	{
		JoyX = 0;
	}
	if (fabsf(JoyX) < 0.3f)
	{
		m_bStick = true;
	}


	//左右入力で選択肢を変える
	if (GetKeyTrigger(VK_LEFT) || (JoyX < -0.8f && m_bStick))
	{
		m_nMenuIndex--;
		if (m_nMenuIndex < MIN_INDEX)
			m_nMenuIndex = MIN_INDEX;
		else
			CSound::PlaySE("CursorMove.mp3");

		m_bStick = false;
	}
	if (GetKeyTrigger(VK_RIGHT) || (JoyX > 0.8f && m_bStick))
	{
		m_nMenuIndex++;
		if (m_nMenuIndex > MAX_INDEX)
			m_nMenuIndex = MAX_INDEX;
		else
			CSound::PlaySE("CursorMove.mp3");

		m_bStick = false;
	}

	
	switch (m_nMenuIndex)
	{
	case 0:
		//メインゲームのオブジェクトを持ってきて
		//色を変えたり・ハイライト表示する
		m_SpriteMenu[0].lock()->SetColor({ 1.0f, 1.0f, m_fChoicesColor });
		m_SpriteMenu[1].lock()->SetColor({ 1.0f, 1.0f, 1.0f });
		m_SpriteMenu[2].lock()->SetColor({ 1.0f, 1.0f, 1.0f });
		break;
	case 1:
		//Editモードのオブジェクトを持ってきて
		//ハイライト表示
		m_SpriteMenu[0].lock()->SetColor({ 1.0f, 1.0f, 1.0f });
		m_SpriteMenu[1].lock()->SetColor({ 1.0f, 1.0f, m_fChoicesColor });
		m_SpriteMenu[2].lock()->SetColor({ 1.0f, 1.0f, 1.0f });
		
		break;
	case 2:
		//ゲーム終了オブジェクトを持ってきて
		//ハイライト表示
		m_SpriteMenu[0].lock()->SetColor({ 1.0f, 1.0f, 1.0f });
		m_SpriteMenu[1].lock()->SetColor({ 1.0f, 1.0f, 1.0f });
		m_SpriteMenu[2].lock()->SetColor({ 1.0f, 1.0f, m_fChoicesColor });
		break;
	}
	
	
	if (m_fTimer > TIME_INTERVAL)
	{
 		if (GetKeyTrigger(VK_SPACE) || GetKeyTrigger(VK_RETURN) ||
			GetJoyRelease(JOYSTICKID1, JOYBUTTON_A))
		{
   			if (!m_bButton)
			{
				//SE再生
				CSound::PlaySE("ChoiseDecide.mp3");

				
				switch (m_nMenuIndex)
				{
				case 0:
					//メインゲーム
					m_bButton = true;
					CEditorComponent::SetCurrentStageNum(MAP_NUM);
					CSceneManager::Get()->SetNextScene<CSceneGame>();
					break;

				case 1:
					//Editモード
					//CSceneManager::Get()->SetNextScene<CSceneEdit>();
					break;

				case 2:
					m_bButton = true;
					//Exit処理(ゲームの終了)
					HWND hWnd = GetMainWnd();
					PostMessage(hWnd, WM_CLOSE, 0, 0);	// [x]が押されたように振舞う
					break;
				}
			}
		}
	}
}


//===================================
//
//	後更新関数
//
//===================================
void CMenuComponent::LateUpdate()
{
	
}


//===================================
//
//	終了関数
//
//===================================
void CMenuComponent::End()
{
	m_bOpen = false;
}

//インデックス取得
int CMenuComponent::GetIndex()
{
	return m_nMenuIndex;
}

//フラグ取得
bool CMenuComponent::GetBOpen()
{
	return m_bOpen;
}

//フラグセット
void CMenuComponent::SetFlag(bool bFlag)
{
	m_bOpen = bFlag;
}