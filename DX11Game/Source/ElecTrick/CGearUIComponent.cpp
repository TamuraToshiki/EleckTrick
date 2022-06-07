//======================================================================
//											CGearUIComponent.cpp
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
#include "CGearUIComponent.h"
#include "CObjectManager.h"

//===== マクロ定義 =====
#define SCREEN_SCALE_X	(SCREEN_WIDTH / 1920.0f)
#define SCREEN_SCALE_Y	(SCREEN_HEIGHT / 1080.0f)
#define SCREEN_CENETR_X	(SCREEN_WIDTH / 2)
#define SCREEN_CENETR_Y	(SCREEN_HEIGHT / 2)

#define FADE_SPEED	(0.6f / 60.0f)	// 60/60は1フレーム 1/60は1秒
#define FADE_KEEP	(0.5f)	// 秒

#define STAR_SIZE_MAX	(70)

#define ROLL_SPEED	(300)

//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CGearUIComponent::Start()
{
	// レンダラー
	m_renderer = m_pParent->AddComponent<CSpriteRenderer>();
	m_renderer.lock()->SetTexture("data/texture/gear2.png");
	m_renderer.lock()->SetLayer(m_renderer.lock()->eUI4);
	m_renderer.lock()->SetMeshBlendState(BS_ALPHABLEND);
	m_fRoll = 1.0f;

	m_fAlpha = 0.0f;
	m_nNumberCnt = 0;
	m_curFade = eNone;

	m_renderer.lock()->SetAlpha(m_fAlpha);
}


//===================================
//
//	更新関数CGearUIComponent
//
//===================================
void CGearUIComponent::Update()
{
	//文字ハイライト
	static int nCnt = 10;
	nCnt += 1;
	m_pParent->m_pTrans->m_pos->y += ((sinf(nCnt * 3.1415f / 100)));
	m_fRoll += (ROLL_SPEED / m_pParent->m_pTrans->GetScale()->x) * m_nrot;
	m_pParent->m_pTrans->SetRotation({ 0, 0, m_fRoll });
	m_renderer.lock()->SetAlpha(m_fAlpha);

	switch (m_curFade)
	{
	case CGearUIComponent::eNone:
		m_curFade = eFadeIn;
		break;

	case CGearUIComponent::eFadeIn:
		if (m_fAlpha <= 1)
		{
			m_fAlpha += FADE_SPEED;
		}
		else if (m_fAlpha >= 1)
		{
			m_curFade = eFadeKeep;
		}
		break;

	case CGearUIComponent::eFadeKeep:
		m_nNumberCnt++;
		if (m_nNumberCnt >= (FADE_KEEP * 60))
		{
			m_curFade = eFadeOut;
			m_nNumberCnt = 0;
		}
		break;

	case CGearUIComponent::eFadeOut:
		if (m_fAlpha >= 0)
		{
			m_fAlpha -= FADE_SPEED;
		}
		else if (m_fAlpha <= 0)
		{
			CObjectManager::DestroyObject(m_pParent);
		}
		break;
	case CGearUIComponent::eMaxState:

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
void CGearUIComponent::LateUpdate()
{

}


//===================================
//
//	終了関数
//
//===================================
void CGearUIComponent::End()
{

}

void CGearUIComponent::SetGearRot(int rot)
{
	m_nrot = rot;
}

//void CGearUIComponent::SetStarAlphe(float alpha)
//{
//	m_fAlpha = alpha;
//}

//===== 使わない時は消してください() =====
//===================================
//
//	2D当たり判定(Enter) コールバック関数
//
//===================================
void CGearUIComponent::OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// タグなどで判定
	if (collsion2d)
	{

	}
}

