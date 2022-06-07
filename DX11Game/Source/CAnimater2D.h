//==================================================================
//									animator.h
//	アニメーター
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/07/08	アニメータークラス作成
//
//===================================================================
#pragma once

//====== インクルード部 ======
#include "CComponent.h"
#include "Renderer.h"


//===== マクロ定義 =====


//===== クラス定義 =====
class CAnimation2D;

// メモ： 汎用型にして
class CAnimator2D : public CComponent
{
public:
	CAnimator2D();
	~CAnimator2D();
	void Start()  override;
	void Update() override;

	// アニメーションリスト管理
	void AddAnimationSelect(const std::string name, int nMaxFrame, ...);
	void AddAnimationDefault(const std::string name, int nMaxFrame);
	//void DestroyAnimation(const std::string name);
	void SetAnimation(const std::string name);
	void SetAnimation(const std::string name, int nSpeed);
	void SetAnimation(const std::string name, int nSpeed, int nSplitX, int nSplitY);

	// セット関数
	void SetSplit(int nSplitX, int nSplitY) {
		m_nSplitX = nSplitX; m_nSplitY = m_nSplitY; 
		Vector3 texSize;
		texSize->x = 1.0f / nSplitX;
		texSize->y = 1.0f / nSplitY;
		m_renderer.lock()->SetTexSize(texSize);
	}
	void SetAnimationSpeed(int nSpeed) { m_nSpeed = nSpeed; }
	void SetStartFrame(int nStart) { m_nFrame = nStart; }
	void SetStartAnimNo(int nAnimNo) { m_nCurrentAnimNo = nAnimNo; }

	//void SetCurAnimNo(int nAnimNo) { m_nCurrentAnimNo = nAnimNo; }

	// ゲット関数
	std::weak_ptr<CAnimation2D> GetCurrentAnimation() { return m_pCurrentAnimation; }

private:
	// プール
	std::map<const std::string, std::shared_ptr<CAnimation2D>> m_animationPool;

	// this
	std::weak_ptr<CAnimator2D> m_this;
	// レンダラー
	std::weak_ptr<CRenderer> m_renderer;

	// アニメーションの変数
	int m_nSpeed;
	int m_nSplitX;
	int m_nSplitY;
	int m_nFrame;
	int m_nCurrentAnimNo;
	std::weak_ptr<CAnimation2D> m_pCurrentAnimation;
};

