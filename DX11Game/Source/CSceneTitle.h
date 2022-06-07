//==================================================================
//								CSceneTitle.h
//	タイトルシーン
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/12/15	作成
//
//===================================================================
#pragma once


//====== インクルード部 ======
#include "CScene.h"
#include "CSpriteRenderer.h"

//===== クラス定義 =====
class CSceneTitle : public CScene
{
public:
	CSceneTitle();
	~CSceneTitle();

	// 四大処理
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

private:
	std::weak_ptr<CSpriteRenderer> m_Sprite;
	std::weak_ptr<CSpriteRenderer> m_Push;

	float m_fAlpha;	//PUSHBUTTONのアルファ値

	bool m_bOpen = false;
	bool m_bAlpha = false;

};


