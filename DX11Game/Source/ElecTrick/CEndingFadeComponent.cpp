//======================================================================
//											CEndingFadeComponent.cpp
//	エンディング演出用のフェードコンポーネント
//
//======================================================================
//	author :	AT12A 15 榊原龍我
//======================================================================
//	開発履歴
//	2021/01/21 作成
//
//======================================================================


//===== インクルード部 =====
#include "CEndingFadeComponent.h"
#include "CObjectManager.h"
#include "input.h"
#include "Sound.h"

#include "CElvLeftDoorObj.h"
#include "CElvRightDoorObj.h"
#include "CWhiteTileObj.h"
#include "CEndingPicObj.h"
#include "CContenueObj.h"

#include "CSceneManager.h"
#include "CSceneTitle.h"

#include "debugproc.h"


//===== マクロ定義 =====
#define TIME_UNTILL_START	(6.0f)		//ドアが動き出すまでの時間
#define TIME_UNTILL_FADE	(12.0f)		//フェードの時間
#define ADD_ALPHA			(0.1f)		//1フレームに加算されるα値
#define ADD_TIMER			(0.1f)		//タイマーへの加算値

#define DOOR_MOVE_X			(4.0f)		//ドア移動量 / f

#define DOOR_SCALE_X		(1280)		//ドアスケール
#define DOOR_SCALE_Y		(720)
#define ENDING_SCALE_X		(1280)		//一枚絵スケール
#define ENDING_SCALE_Y		(720)
#define CONTENUE_SCALE_X	(350.0f)	//Contenueスケール
#define CONTENUE_SCALE_Y	(100.0f)

#define CONTENUE_POS_X		(450.0f)	//Contenue位置
#define CONTENUE_POS_Y		(-300.0f)

#define ALPHA3			(0.01f)

//テクスチャパス
#define ELVR_TEXTURE_PATH		"data/texture/Ending/Right.png"
#define ELVL_TEXTURE_PATH		"data/texture/Ending/Left.png"
#define WHITE_TEXTURE_PATH		"data/texture/Ending/WhiteTile.png"
#define ENDING_TEXTURE_PATH		"data/texture/Ending/Ending.png"
#define BACK_TEXTURE_PATH		"data/texture/Ending/EndingBack.png"
#define CONTENUE_TEXTURE_PATH	"data/texture/Ending/ToBeContenued.png"

//===== プロトタイプ宣言 =====


//===== グローバル変数 =====

//===================================
//
//	スタート関数
//
//===================================
void CEndingFadeComponent::Start()
{
	//初期化
	m_fTimer = 0.0f;
	m_fAlpha = 1.0f;
	m_fAlpha2 = 0.0f;
	m_fAlpha3 = 0.0f;
	m_bFadeEnd = false;	
	m_bMoveEnd = false;

	//演出用オブジェクトの生成
	//レンダラー取得
	m_SpriteEnding[0] = CObjectManager::CreateObject<CElvRightDoorObj>(
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		{ DOOR_SCALE_X,DOOR_SCALE_Y,0.0f })
		->GetComponent<CSpriteRenderer>();

	m_SpriteEnding[1] = CObjectManager::CreateObject<CElvLeftDoorObj>(
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		{ DOOR_SCALE_X,DOOR_SCALE_Y,0.0f })
		->GetComponent<CSpriteRenderer>();

	m_SpriteEnding[2] = CObjectManager::CreateObject<CWhiteTileObj>(
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		{ ENDING_SCALE_X,ENDING_SCALE_Y,0.0f })
		->GetComponent<CSpriteRenderer>();

	m_SpriteEnding[3] = CObjectManager::CreateObject<CEndingPicObj>(
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		{ ENDING_SCALE_X,ENDING_SCALE_Y,0.0f })
		->GetComponent<CSpriteRenderer>();

	m_SpriteEnding[4] = CObjectManager::CreateObject<CContenueObj>(
		{ CONTENUE_POS_X,CONTENUE_POS_Y,0.0f },
		{ 0.0f,0.0f,0.0f },
		{ CONTENUE_SCALE_X,CONTENUE_SCALE_Y,0.0f })
		->GetComponent<CSpriteRenderer>();

	

	//レイヤーセット
	m_SpriteEnding[0].lock()->SetLayer(CSpriteRenderer::eUI5);		//エレベータードアR
	m_SpriteEnding[1].lock()->SetLayer(CSpriteRenderer::eUI5);		//エレベータードアL
	m_SpriteEnding[2].lock()->SetLayer(CSpriteRenderer::eUI4);		//白背景
	m_SpriteEnding[3].lock()->SetLayer(CSpriteRenderer::eUI4);		//荒廃世界
	m_SpriteEnding[4].lock()->SetLayer(CSpriteRenderer::eUI4);		//Contenue文字

	//テクスチャセット
	m_SpriteEnding[0].lock()->SetTexture(ELVR_TEXTURE_PATH);
	m_SpriteEnding[1].lock()->SetTexture(ELVL_TEXTURE_PATH);
	m_SpriteEnding[2].lock()->SetTexture(WHITE_TEXTURE_PATH);
	m_SpriteEnding[3].lock()->SetTexture(ENDING_TEXTURE_PATH);
	m_SpriteEnding[4].lock()->SetTexture(CONTENUE_TEXTURE_PATH);
	

	//アルファ値セット
	m_SpriteEnding[0].lock()->SetAlpha(m_fAlpha);
	m_SpriteEnding[1].lock()->SetAlpha(m_fAlpha);
	m_SpriteEnding[2].lock()->SetAlpha(1.0f);
	m_SpriteEnding[3].lock()->SetAlpha(m_fAlpha2);
	m_SpriteEnding[4].lock()->SetAlpha(0.0f);
}


//===================================
//
//	更新関数
//
//===================================
void CEndingFadeComponent::Update()
{
	//タイマーに加算
	m_fTimer += ADD_TIMER;

	//文字ハイライト
	static int nCnt = 0;
	nCnt += 1;
	m_fTextColor = abs(sinf(nCnt * 3.1415f / 100));

	//タイマーが設定値を超えたら
	if (m_fTimer > TIME_UNTILL_START)
	{
		//ドア移動
		m_SpriteEnding[0].lock()->m_pParent->m_pTrans->m_pos->x += DOOR_MOVE_X;
		m_SpriteEnding[1].lock()->m_pParent->m_pTrans->m_pos->x -= DOOR_MOVE_X;
		CSound::PlayBGM("EndingDoorMove.mp3");

		if (m_SpriteEnding[0].lock()->m_pParent->m_pTrans->m_pos->x == SCREEN_CENTER_X &&
			m_SpriteEnding[1].lock()->m_pParent->m_pTrans->m_pos->x == -SCREEN_CENTER_X)
		{
			m_SpriteEnding[0].lock()->m_pParent->m_pTrans->m_pos->x =  SCREEN_CENTER_X;
			m_SpriteEnding[1].lock()->m_pParent->m_pTrans->m_pos->x = -SCREEN_CENTER_X;
		} 
		
		//一枚絵表示
		if (m_SpriteEnding[0].lock()->m_pParent->m_pTrans->m_pos->x > SCREEN_CENTER_X &&
			m_SpriteEnding[1].lock()->m_pParent->m_pTrans->m_pos->x < -SCREEN_CENTER_X)
		{
			CSound::StopBGM("EndingDoorMove.mp3");
			CSound::PlayBGM("Strong_Wind.mp3");
			m_fAlpha2 += 0.01f;
			m_SpriteEnding[3].lock()->SetAlpha(m_fAlpha2);
			if (m_fAlpha2 > 1.0f)
			{
				m_fAlpha2 = 1.0f;
				m_bFadeEnd = true;
			}
		}
	}

	//表示し終わったら
	if (m_bFadeEnd)
	{
		m_fAlpha3 += ALPHA3;
		m_SpriteEnding[4].lock()->SetAlpha(m_fAlpha3);
		m_SpriteEnding[4].lock()->SetColor(m_fTextColor, m_fTextColor, m_fTextColor);
		m_bMoveEnd = true;
	}

	if (m_bMoveEnd)
	{
		if (GetKeyTrigger(VK_RETURN) || GetKeyTrigger(VK_SPACE) ||
			GetJoyRelease(JOYSTICKID1, JOYBUTTON_A))
		{
			//タイトルシーンへ移行
			CSceneManager::Get()->SetNextScene<CSceneTitle>();
			CObjectManager::DestroyObject(m_pParent);
			CSound::StopBGM("Strong_Wind.mp3");
		}
	}
	
}


//===================================
//
//	後更新関数
//
//===================================
void CEndingFadeComponent::LateUpdate()
{

}


//===================================
//
//	終了関数
//
//===================================
void CEndingFadeComponent::End()
{

}


