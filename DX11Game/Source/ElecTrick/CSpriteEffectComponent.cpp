//======================================================================
//											CSpriteEffectComponent.h
//	スプライトエフェクトコンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2021/01/24	作成
//
//======================================================================


//===== インクルード部 =====
#include "CSpriteEffectComponent.h"
#include "CObjectManager.h"


//===== マクロ定義 =====


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CSpriteEffectComponent::Start()
{
	//m_pParent->m_pTrans->m_scale = Vector3{ 100,100,100 };

	// レンダラーの指定
	m_renderer = m_pParent->AddComponent<CSpriteRenderer>();
	m_renderer.lock()->SetMeshBlendState(BS_ADDITIVE);
	m_renderer.lock()->SetLayer(CSpriteRenderer::eUI3);

	// アニメーターの取得
	m_animator = m_pParent->AddComponent<CAnimator2D>();

	// ループ指定
	m_bLoop = false;
}


//===================================
//
//	更新関数
//
//===================================
void CSpriteEffectComponent::Update()
{

}


//===================================
//
//	後更新関数
//
//===================================
void CSpriteEffectComponent::LateUpdate()
{

}


//===================================
//
//	終了関数
//
//===================================
void CSpriteEffectComponent::End()
{

}


//===================================
//
// エフェクトの指定
//
//===================================
void CSpriteEffectComponent::SetEffect(EEffectType eType)
{
	switch (eType)
	{
	case CSpriteEffectComponent::eHitEffect:

		// テクスチャ
		m_renderer.lock()->SetTexture("data/texture/Effect/HitEffect.png");

		// アニメーション
		m_animator.lock()->SetSplit(5, 2);
		m_animator.lock()->AddAnimationDefault("Effect", 5 * 2);
		m_animator.lock()->SetAnimationSpeed(2);
		m_animator.lock()->SetAnimation("Effect");

		// ループ指定
		m_bLoop = false;

		break;
	case CSpriteEffectComponent::eSaveEffect:
		// テクスチャ
		m_renderer.lock()->SetTexture("data/texture/Effect/SaveEffect.png");

		// アニメーション
		m_animator.lock()->SetSplit(5, 2);
		m_animator.lock()->AddAnimationDefault("Effect", 5 * 2);
		m_animator.lock()->SetAnimationSpeed(3);
		m_animator.lock()->SetAnimation("Effect");

		// ループ指定
		m_bLoop = false;

		break;

	case CSpriteEffectComponent::eSmokeEffect:
		// テクスチャ
		m_renderer.lock()->SetTexture("data/texture/Effect/SmokeEffect01.png");
		m_renderer.lock()->SetColor({ 0.5f,0.5f,0.5f});

		// アニメーション
		m_animator.lock()->SetSplit(4, 1);
		m_animator.lock()->AddAnimationDefault("Effect", 4 * 1);
		m_animator.lock()->SetAnimationSpeed(32 / 4);
		m_animator.lock()->SetAnimation("Effect");

		// ループ指定
		m_bLoop = false;

		break;

	case CSpriteEffectComponent::eMaxEffect:
		break;
	default:
		break;
	}

	// マトリックスを更新
	m_renderer.lock()->UpdateTexMatrix();

}

//===================================
//
// 任意エフェクトの指定
//
//===================================
void CSpriteEffectComponent::SetAnyEffect(
	std::string filename, int nSplitX, int nSplitY, int nSpeed, bool bLoop, int nStartNo)
{
	// テクスチャ
	m_renderer.lock()->SetTexture(filename.c_str());

	// アニメーション
	m_animator.lock()->SetSplit(nSplitX, nSplitY);
	m_animator.lock()->AddAnimationDefault("Effect", nSplitX * nSplitY);
	m_animator.lock()->SetAnimationSpeed(nSpeed);
	m_animator.lock()->SetAnimation("Effect");
	m_animator.lock()->SetStartAnimNo(nStartNo);

	// ループ指定
	m_bLoop = bLoop;

	// マトリックスを更新
	m_renderer.lock()->UpdateTexMatrix();
}


//===================================
//
//	2Dアニメーション終了時 コールバック関数
//
//===================================
void CSpriteEffectComponent::OnAnimationExit2D(const std::shared_ptr <CAnimator2D>& animator2d)
{
	// ループか
	if (m_bLoop) return;

	// 破棄
	CObjectManager::DestroyObject(m_pParent);
}

