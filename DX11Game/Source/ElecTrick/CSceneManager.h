//==================================================================
//								CSceneManager.h
//	シーン管理
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/07/16	シーン管理クラス作成
//
//===================================================================

#pragma once

//====== インクルード部 ======
#include "CScene.h"
#include "CFadeObject.h"
#include "CFadeComponent.h"
#include "CObjectManager.h"


//===== クラス定義 =====

class CSceneManager
{
public:
	// 四大処理
	void Init();
	void Uninit();
	void Update();
	void Draw();

	// シーンのセット
	template <class T>
	void SetNextScene();

	void SetSceneChange();

	// 更新ストップフラグ
	void SetStopFlag(bool bStop) { m_bStop = bStop; }

	static CSceneManager* Get() {
		static CSceneManager Instance;
		return &Instance;
	}

	CScene* GetCurrentScene() { return m_pCurrentScene; }

	void StartFade(std::function<void(void)> endFadeFunc)
	{
		// 関数オブジェクトセット
		m_fade.GetComponent<CFadeComponent>()->SetFadeEndFunc(endFadeFunc);

		// フェードスタート
		m_fade.GetComponent<CFadeComponent>()->StartFadeOut();
	}

	bool GetNowFade() { return m_fade.GetComponent<CFadeComponent>()->GetIsFade(); }

private:
	CScene* m_pCurrentScene;
	CScene* m_pNextScene;

	bool m_bStop;
	bool m_bSceneChange;

	CFadeObject m_fade;

	CSceneManager();
	~CSceneManager();
};

//========================================================
//
//	次のシーンをセット
//
//========================================================
template <class T>
void CSceneManager::SetNextScene()
{
	// シーン変更中
	if (m_bSceneChange) return;

	// 次のシーンの生成・格納
	m_pNextScene = new T();

	// シーン変更フラグオン
	m_bSceneChange = true;

	// 関数オブジェクトセット
	m_fade.GetComponent<CFadeComponent>()->SetFadeEndFunc([this]() { this->SetSceneChange(); });

	// フェードスタート
	m_fade.GetComponent<CFadeComponent>()->StartFadeOut();
}