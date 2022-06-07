//======================================================================
//											CClearComponent.cpp
//	コンポーネント作成用のベースレイアウト
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/11/16	作成
//
//======================================================================


//===== インクルード部 =====
#include "CClearComponent.h"
#include "CObjectManager.h"
#include "CStarParticleComponent.h"
#include "Sound.h"

//===== マクロ定義 =====
#define SCREEN_SCALE_X	(SCREEN_WIDTH / 1920.0f)
#define SCREEN_SCALE_Y	(SCREEN_HEIGHT / 1080.0f)

#define FADE_IN_SPEED	(1.2f / 60.0f)	// 60/60は1フレーム 1/60は1秒
#define FADE_OUT_SPEED	(0.8f / 60.0f)
#define FADE_KEEP	(2.0f)	// 秒

#define CLEAR_SIZE_X	(1000)
#define CLEAR_SIZE_Y	(CLEAR_SIZE_X / 2)

//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CClearComponent::Start()
{
	m_pParent->m_pTrans->m_pos = Vector3{ 0,0,0 };
	m_pParent->m_pTrans->m_scale = Vector3{ CLEAR_SIZE_X,CLEAR_SIZE_Y,0 };

	// レンダラー
	m_renderer = m_pParent->AddComponent<CSpriteRenderer>();
	m_renderer.lock()->SetTexture("data/texture/HEWclear.png");
	m_renderer.lock()->SetLayer(m_renderer.lock()->eUI5);
	m_renderer.lock()->SetMeshBlendState(BS_ALPHABLEND);

	m_fAlpha = 0.0f;
	m_nNumberCnt = 0;
	m_curFade = eNone;
	m_nStarCnt = 0;

	m_renderer.lock()->SetAlpha(m_fAlpha);

	// サウンド
	CSound::PlaySE("StageClear.mp3");

}


//===================================
//
//	更新関数CClearComponent
//
//===================================
void CClearComponent::Update()
{
	m_nStarCnt++;
	if (m_nStarCnt >= 45 && m_curFade != eFadeOut)
	{

		for (int i = 0; i < 30; i++)
		{
			const auto& star = CObjectManager::CreateObject<CObject>(m_pParent->m_pTrans->GetPos());
			star->AddComponent<CStarParticleComponent>();
			star->GetComponent<CStarParticleComponent>()->SetStarAlphe(m_fAlpha);
		}
		m_nStarCnt = 0;
	}

	switch (m_curFade)
	{
	case CClearComponent::eNone:
		m_curFade = eFadeIn;
		break;

	case CClearComponent::eFadeIn:
		if (m_fAlpha <= 1)
		{
			m_fAlpha += FADE_IN_SPEED;
		}
		else if (m_fAlpha >= 1)
		{
			m_curFade = eFadeKeep;
		}
		m_renderer.lock()->SetAlpha(m_fAlpha);
		break;

	case CClearComponent::eFadeKeep:
		m_nNumberCnt++;
		if (m_nNumberCnt >= (FADE_KEEP * 60))
		{
			m_curFade = eFadeOut;
			m_nNumberCnt = 0;
		}
		break;

	case CClearComponent::eFadeOut:
		if (m_fAlpha >= 0)
		{
			m_fAlpha -= FADE_OUT_SPEED;
		}
		else if (m_fAlpha <= 0)
		{
			CObjectManager::DestroyObject(m_pParent);
		}
		m_renderer.lock()->SetAlpha(m_fAlpha);
		break;
	case CClearComponent::eMaxState:

		break;
	default:
		break;
	}
}


//===================================
//
//	後更新関数
//
//===================================
void CClearComponent::LateUpdate()
{

}


//===================================
//
//	終了関数
//
//===================================
void CClearComponent::End()
{

}


//===== 使わない時は消してください() =====
//===================================
//
//	2D当たり判定(Enter) コールバック関数
//
//===================================
void CClearComponent::OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// タグなどで判定
	if (collsion2d)
	{

	}
}

