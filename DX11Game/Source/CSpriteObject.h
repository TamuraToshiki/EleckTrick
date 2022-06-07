//======================================================================
//											CSpriteObject.h
//	SpriteObjectBase
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/06	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CSpriteRenderer.h"


//===== 構造体定義 =====


//===== クラス定義 =====
class CSpriteObject : public CObject
{
public:
	CSpriteObject();
	virtual ~CSpriteObject();

protected:
	std::weak_ptr<CSpriteRenderer> m_renderer;

private:
};


//===== プロトタイプ宣言 =====


