//==================================================================
//									Animator.h
//	コリジョン
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/07/08	アニメータークラスの作成
//
//===================================================================

//====== インクルード部 ======
#include "CAnimater2D.h"
#include "CAnimation2D.h"
#include "CSpriteRenderer.h"
#include "InstancingMeshRenderer.h"


//===== マクロ定義 =====


//===== クラス定義 =====



//===== プロトタイプ宣言 =====



//========================================
//
//	コンストラクタ
//
//========================================
CAnimator2D::CAnimator2D()
{
	m_nCurrentAnimNo = 0;
	m_nFrame = 0;
	m_nSpeed = 0;
	m_nSplitX = 0;
	m_nSplitY = 0;
}


//========================================
//
//	デストラクタ
//
//========================================
CAnimator2D::~CAnimator2D()
{
	m_animationPool.clear();
}

//========================================
//
//	更新
//
//========================================
void CAnimator2D::Start()
{
	// レンダラー取得
	const auto& bill = m_pParent->GetComponent<CBillboardRenderer>();
	m_renderer = bill;
	if (!bill)
	{
		// スプライト
		const auto& spr = m_pParent->GetComponent<CSpriteRenderer>();
		m_renderer = spr;
		if (!spr)
		{
			// メッシュ
			const auto& mesh = m_pParent->GetComponent<CMeshRenderer>();
			m_renderer = mesh;
			if (!mesh)
			{
				// アシンプ
				const auto& assimp = m_pParent->GetComponent<CAssimpRenderer>();
				m_renderer = assimp;
				if (!assimp)
				{
					// インスタンスシング
					const auto& instancing = m_pParent->GetComponent<InstancingMeshRenderer>();
					m_renderer = instancing;
				}
			}
		}
	}

	// 自身
	m_this = m_pParent->GetComponent<CAnimator2D>();
}


//========================================
//
//	更新
//
//========================================
void CAnimator2D::Update()
{
	// 現在のアニメーションを取得
	const auto& pAnimation = m_pCurrentAnimation.lock();

	// 空なら飛ばす
	if (!pAnimation) return;

	Vector3 texPos;
	Vector3 texSize = m_renderer.lock()->GetTexSize();

	// アニメーションのコマの更新
	if (0 == m_nFrame % m_nSpeed)
	{
		// テクスチャ座標の更新
		texPos->x = texSize->x * (pAnimation->GetAnimNoArray()[m_nCurrentAnimNo] % m_nSplitX);
		texPos->y = texSize->y * (pAnimation->GetAnimNoArray()[m_nCurrentAnimNo] / m_nSplitX);
		m_renderer.lock()->SetTexPos(texPos);
		m_renderer.lock()->UpdateTexMatrix();

		// 現在のコマ番号の更新
		m_nCurrentAnimNo++;
		m_nFrame = 0;
	}

	//  フレームの更新
	m_nFrame++;

	// アニメーションの終了(最終フレーム)
	if (m_nCurrentAnimNo >= pAnimation->GetArraySize() && m_nFrame >= m_nSpeed)
	{
		// アニメーション終了のコールバック関数
		const auto& list = m_pParent->GetComponentList();
		std::for_each(list.begin(), list.end(), [this](const auto& com)
		{
			com->OnAnimationExit2D(m_this.lock());
		});

		m_nCurrentAnimNo = 0;
	}



}


//========================================
//
//	アニメーターにアニメーションのセット
//
//========================================
void CAnimator2D::SetAnimation(std::string name)
{
	// アニメーションの検索
	const auto& itr = m_animationPool.find(name);

	// 存在したら
	if (m_animationPool.end() != itr)
	{
		const auto& curAnim = m_pCurrentAnimation.lock();
		// アニメーションが変わってないなら
		if (curAnim == itr->second) return;

		m_pCurrentAnimation = itr->second;
		m_nFrame = 0;
		m_nCurrentAnimNo = 0;
		return;
	}
}

//========================================
//
//	アニメーターにアニメーションのセット
//
//========================================
void CAnimator2D::SetAnimation(const std::string name, int nSpeed)
{
	SetAnimation(name);
	if(nSpeed > 0)
		m_nSpeed = nSpeed;
}

//========================================
//
//	アニメーターにアニメーションのセット
//
//========================================
void CAnimator2D::SetAnimation(const std::string name, int nSpeed, int nSplitX, int nSplitY)
{
	SetAnimation(name, nSpeed);
	m_nSplitX = nSplitX;
	m_nSplitY = nSplitY;

	Vector3 texSize;
	texSize->x = 1.0f / nSplitX;
	texSize->y = 1.0f / nSplitY;

	m_renderer.lock()->SetTexSize(texSize);
}


//========================================
//
//	アニメーションの追加
//
//========================================
void CAnimator2D::AddAnimationSelect(const std::string name, int nMaxFrame, ...)
{
	// 可変引数リスト
	va_list args;
	int nCount;

	// 動的にアニメーションクラスの確保
	CAnimation2D *pAnim = new CAnimation2D;

	// 可変引数リスト作成
	va_start(args, nMaxFrame);

	// 可変引数の数を計算
	nCount = nMaxFrame;

	// 可変引数分メモリ確保
	pAnim->m_pAnimNoArray = new int[nCount];

	// 配列にnAnimNoを順番に格納
	for (int i = 0; i < nCount; i++)
	{
		pAnim->m_pAnimNoArray[i] = va_arg(args, int);
	}

	// 可変引数リスト終了
	va_end(args);

	// アニメーションの名前の格納
	pAnim->m_name = name;
	// 配列(アニメーションのコマ)の数を格納
	pAnim->m_nArraySize = nCount;

	// 最後にアニメーターのリストに新しく作ったアニメーションを格納
	m_animationPool.emplace(name, pAnim);

}

//========================================
//
//	生成 (自動でコマ番号振り分け)
//
//========================================
void CAnimator2D::AddAnimationDefault(const std::string name, int nMaxFrame)
{
	// 動的にアニメーションクラスの確保
	CAnimation2D *pAnim = new CAnimation2D;

	// メモリ確保
	pAnim->m_pAnimNoArray = new int[nMaxFrame];

	// 配列にnAnimNoを順番に格納
	for (int i = 0; i < (int)nMaxFrame; i++)
	{
		pAnim->m_pAnimNoArray[i] = i;
	}

	// アニメーションの名前の格納
	pAnim->m_name = name;
	// 配列(アニメーションのコマ)の数を格納
	pAnim->m_nArraySize = nMaxFrame;

	// 最後にアニメーターのリストに新しく作ったアニメーションを格納
	m_animationPool.emplace(name, pAnim);

}