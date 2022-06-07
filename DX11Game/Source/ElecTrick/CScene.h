//==================================================================
//								CScene.h
//	シーン
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/07/16	シーンクラス作成
//
//===================================================================

#pragma once

//====== インクルード部 ======
#include <string>

//===== クラス定義 =====
class CScene
{
public:
	CScene();
	virtual ~CScene();

	// 四大処理
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	std::string& GetSceneName() { return m_sceneName; }

protected:
	std::string m_sceneName;
};


