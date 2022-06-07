//======================================================================
//											CEffectComponent.h
//	エフェクトコンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2021/01/23	作成
//
//======================================================================


//===== インクルード部 =====
#include "CEffectComponent.h"
#include "CObjectManager.h"


//===== マクロ定義 =====


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CEffectComponent::Start()
{
	m_pParent->m_pTrans->m_scale = Vector3{ 100,100,100 };

	// レンダラーの指定
	m_renderer = m_pParent->AddComponent<CBillboardRenderer>();
	m_renderer.lock()->SetMeshBlendState(BS_ADDITIVE);

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
void CEffectComponent::Update()
{

}


//===================================
//
//	後更新関数
//
//===================================
void CEffectComponent::LateUpdate()
{

}


//===================================
//
//	終了関数
//
//===================================
void CEffectComponent::End()
{

}


//===================================
//
// エフェクトの指定
//
//===================================
void CEffectComponent::SetEffect(EEffectType eType)
{
	switch (eType)
	{
	case CEffectComponent::eHitEffect:

		// テクスチャ
		m_renderer.lock()->SetDiffuseTexture("data/texture/Effect/HitEffect.png");

		// アニメーション
		m_animator.lock()->SetSplit(5, 2);
		m_animator.lock()->AddAnimationDefault("Effect", 5 * 2);
		m_animator.lock()->SetAnimationSpeed(2);
		m_animator.lock()->SetAnimation("Effect");

		// ループ指定
		m_bLoop = false;

		break;
	case CEffectComponent::eSaveEffect:
		// テクスチャ
		m_renderer.lock()->SetDiffuseTexture("data/texture/Effect/SaveEffect.png");

		// アニメーション
		m_animator.lock()->SetSplit(5, 2);
		m_animator.lock()->AddAnimationDefault("Effect", 5 * 2);
		m_animator.lock()->SetAnimationSpeed(3);
		m_animator.lock()->SetAnimation("Effect");

		// ループ指定
		m_bLoop = false;

		break;

	case CEffectComponent::eSmokeEffect:
		// テクスチャ
		m_renderer.lock()->SetDiffuseTexture("data/texture/Effect/SmokeEffect01.png");
		m_renderer.lock()->SetDiffuseColor({ 0.5f,0.5f,0.5f,0.5f });

		// アニメーション
		m_animator.lock()->SetSplit(4, 1);
		m_animator.lock()->AddAnimationDefault("Effect", 4 * 1);
		m_animator.lock()->SetAnimationSpeed(32 / 4);
		m_animator.lock()->SetAnimation("Effect");

		// ループ指定
		m_bLoop = false;

		break;

	case CEffectComponent::eMaxEffect:
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
void CEffectComponent::SetAnyEffect(std::string filename, int nSplitX, int nSplitY, int nSpeed, bool bLoop)
{
	// テクスチャ
	m_renderer.lock()->SetDiffuseTexture(filename.c_str());

	// アニメーション
	m_animator.lock()->SetSplit(nSplitX, nSplitY);
	m_animator.lock()->AddAnimationDefault("Effect", nSplitX * nSplitY);
	m_animator.lock()->SetAnimationSpeed(nSpeed);
	m_animator.lock()->SetAnimation("Effect");

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
void CEffectComponent::OnAnimationExit2D(const std::shared_ptr <CAnimator2D>& animator2d)
{
	// ループか
	if (m_bLoop) return;

	// 破棄
	CObjectManager::DestroyObject(m_pParent);
}

