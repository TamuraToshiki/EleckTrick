//======================================================================
//											CBillboardObject.h
//	BillboardObjectBase
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/11/19	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "Renderer.h"


//===== 構造体定義 =====


//===== クラス定義 =====
class CBillboardObject : public CObject
{
public:
	CBillboardObject();
	virtual ~CBillboardObject();

protected:
	std::weak_ptr<CBillboardRenderer> m_renderer;

private:
};


//===== プロトタイプ宣言 =====


