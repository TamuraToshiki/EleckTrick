//==================================================================
//								CSceneEnding.h
//	エンディングシーン
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2021/01/23	作成
//
//===================================================================
#pragma once


//====== インクルード部 ======
#include "CScene.h"

//===== クラス定義 =====
class CSceneEnding : public CScene
{
public:
	CSceneEnding();
	~CSceneEnding();

	// 四大処理
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

private:


};


