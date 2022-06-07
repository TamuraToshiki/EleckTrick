//======================================================================
//											CAssimpObject.h
//	AssimpObjectBase
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/11/15	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "Renderer.h"


//===== 構造体定義 =====


//===== クラス定義 =====
class CAssimpObject : public CObject
{
public:
	CAssimpObject();
	virtual ~CAssimpObject();

protected:
	std::weak_ptr<CAssimpRenderer> m_renderer;

private:
};


//===== プロトタイプ宣言 =====


